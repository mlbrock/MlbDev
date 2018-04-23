// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Manages the forwarding of UDP and IP multicast packets.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/BoostAsioInclude.hpp>

#include <SocketIo/StreamAcceptorAsyncFwd.hpp>
#include <SocketIo/ResolveHost.hpp>
#include <SocketIo/ResolveService.hpp>
#include <SocketIo/TimerHandlerCriticalEvent.hpp>
#include <SocketIo/TimerHandlerFwd.hpp>
#include <SocketIo/StreamSocketAsyncFwd.hpp>
#include <SocketIo/PacketReceiverAsyncMCFwd.hpp>
#include <SocketIo/PacketSenderMC.hpp>
#include <SocketIo/SocketSpecLocal.hpp>
#include <SocketIo/SocketIoParseCmdLineArg.hpp>

#include <Utility/InlineContainer.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/Sleep.hpp>
#include <Utility/PathName.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

namespace {
// ////////////////////////////////////////////////////////////////////////////
const unsigned int MaxPacketSize = 16384;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetCompilationDateTime()
{
	return(std::string(__DATE__ + std::string(" ") + __TIME__));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const StreamSocketAsyncSPtr &datum)
{
	o_str << SocketSpec(datum->GetIpAddress(), datum->GetPort());

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
struct SendAgent {
	SendAgent(const SocketSpec &packet_spec, StreamSocketAsyncSPtr &socket_sptr);

	SocketSpec            packet_spec_;
	StreamSocketAsyncSPtr socket_sptr_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SendAgent::SendAgent(const SocketSpec &packet_spec,
	StreamSocketAsyncSPtr &socket_sptr)
	:packet_spec_(packet_spec)
	,socket_sptr_(socket_sptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct SendRecvSpec {
	SendRecvSpec()
		:data_spec_()
		,host_spec_()
		,send_index_(0)
	{
	}

	SendRecvSpec(const SocketSpec &data_spec, const SocketSpec &host_spec,
		std::size_t send_index)
		:data_spec_(data_spec)
		,host_spec_(host_spec)
		,send_index_(send_index)
	{
	}

	bool operator < (const SendRecvSpec &other) const
	{
		return(data_spec_ < other.data_spec_);
	}

	SocketSpec  data_spec_;
	SocketSpec  host_spec_;
	std::size_t send_index_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::multiset<SendRecvSpec>      SendRecvSpecMSet;
typedef SendRecvSpecMSet::iterator       SendRecvSpecMSetIter;
typedef SendRecvSpecMSet::const_iterator SendRecvSpecMSetIterC;

//typedef boost::shared_ptr<SendRecvSpecMSet> SendRecvSpecMSetSPtr;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<StreamSocketAsyncSPtr>     ClientSPtrSet;
typedef ClientSPtrSet::iterator             ClientSPtrSetIter;
typedef ClientSPtrSet::const_iterator       ClientSPtrSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct SendEntry {
	SendEntry()
		:data_spec_()
		,client_set_()
	{
	}

	explicit SendEntry(const SendRecvSpec &send_recv_spec)
		:data_spec_(send_recv_spec.data_spec_)
		,send_index_(send_recv_spec.send_index_)
		,client_set_()
	{
	}

	bool operator < (const SendEntry &other) const
	{
		if (data_spec_.ip_address_ < other.data_spec_.ip_address_)
			return(true);
		else if (data_spec_.ip_address_ == other.data_spec_.ip_address_) {
			if (data_spec_.ip_port_ < other.data_spec_.ip_port_)
				return(true);
			else if ((data_spec_.ip_port_ == other.data_spec_.ip_port_) &&
				(data_spec_.local_interface_ < other.data_spec_.local_interface_))
				return(true);
		}

		return(false);
	}

	SocketSpec    data_spec_;
	std::size_t   send_index_;
	ClientSPtrSet client_set_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<SendEntry>          SendEntrySet;
typedef SendEntrySet::iterator       SendEntrySetIter;
typedef SendEntrySet::const_iterator SendEntrySetIterC;

typedef std::vector<SendEntry>        SendEntryList;
typedef SendEntryList::iterator       SendEntryListIter;
typedef SendEntryList::const_iterator SendEntryListIterC;

typedef boost::shared_ptr<SendEntryList> SendEntryListSPtr;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PacketForwarder {
public:
	PacketForwarder(int argc, char **argv)
		:log_file_name_()
		,send_spec_mset_()
		,recv_spec_mset_()
		,retry_connect_flag_(false)
		,retry_seconds_(30)
		,tcp_no_delay_flag_(false)
		,send_entry_list_()
		,is_server_(false)
	{
		ParseCmdLine(argc, argv);

		Run();
	}

	bool GetTcpNoDelayFlag() const
	{
		return(tcp_no_delay_flag_);
	}

	void RemoveClient(StreamSocketAsyncSPtr &socket_sptr)
	{
		bool remove_done = false;

		if (!send_entry_list_.empty()) {
			ClientSPtrSetIter iter_f(
				send_entry_list_.front().client_set_.find(socket_sptr));
			if (iter_f != send_entry_list_.front().client_set_.end()) {
				send_entry_list_.front().client_set_.erase(iter_f);
				remove_done = true;
				std::cout << "Removed a connection from " << socket_sptr <<
					"." << std::endl;
			}
		}

		std::cout << "Connection from " << socket_sptr << " was not performed "
			"as it was not found in the connection list." << std::endl;
	}

	SendRecvSpecMSet &GetSendMSetRef() { return(send_spec_mset_); }
	SendRecvSpecMSet &GetReceiveMSetRef() { return(recv_spec_mset_); }

	SendEntryList &GetSendEntryListRef() { return(send_entry_list_); }

private:
	void ParseCmdLine(int argc, char **argv);
	void Run();
	void RunServer();
	void RunClient();

	std::string      log_file_name_;
	SendRecvSpecMSet send_spec_mset_;
	SendRecvSpecMSet recv_spec_mset_;
	bool             retry_connect_flag_;
	unsigned int     retry_seconds_;
	bool             tcp_no_delay_flag_;
	SendEntryList    send_entry_list_;
	bool             is_server_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct TimeLoggerSrvFwd {
private:
	struct RecvMCFwd;
	typedef PacketReceiverAsyncMCFwd<RecvMCFwd> MyRecvMCType;
	typedef boost::shared_ptr<MyRecvMCType>     MyRecvMCTypeSPtr;

	struct RecvMCFwd {
		RecvMCFwd(const SendRecvSpec &send_recv_spec,
			SendEntryList &send_entry_list)
			:send_recv_spec_(send_recv_spec)
			,send_entry_list_(send_entry_list)
		{
		}

		bool operator () (MyRecvMCType & /* mc_receiver */,
			const boost::system::error_code & /* error */, std::size_t bytes_received,
			const void *data_buffer,
			const boost::asio::ip::udp::endpoint & /* sender_endpoint */)
		{
			if (send_entry_list_.front().client_set_.empty())
				return(true);

			if (!bytes_received) {
				std::ostringstream o_str;
				o_str << "Received a zero-length packet on " <<
					send_recv_spec_.data_spec_ << ".";
				MLB::Utility::ThrowLogicError(o_str.str());
			}
			else if (bytes_received > MaxPacketSize) {
				std::ostringstream o_str;
				o_str << "Received a packet with a length (" << bytes_received <<
					" bytes) which exceeds the maximum permissible (" << 0xFFFF <<
					" bytes).";
				MLB::Utility::ThrowLogicError(o_str.str());
			}

			MsgLengthType packet_length     =
				static_cast<MsgLengthType>(sizeof(MsgLengthType) + bytes_received);
			MsgLengthType packet_length_net = htons(packet_length);
			char send_buffer[sizeof(MsgLengthType) + MaxPacketSize];

			::memcpy(send_buffer, &packet_length_net, sizeof(packet_length_net));
			::memcpy(send_buffer + sizeof(packet_length), data_buffer,
				bytes_received);



/*
	CODE NOTE: Logging point.
std::cout << "SERVER: SEND LENGTH " << std::setw(5) << packet_length << std::endl;
*/



			ClientSPtrSetIter iter_b(send_entry_list_.front().client_set_.begin());
			ClientSPtrSetIter iter_e(send_entry_list_.front().client_set_.end());

			for ( ; iter_b != iter_e; ) {
				try {
					(*iter_b)->Send(packet_length, send_buffer);
					++iter_b;
				}
				catch (const std::exception &except) {
					std::cerr << "Send to " << *iter_b << " failed: " <<
						except.what() << std::endl;
					StreamSocketAsyncSPtr socket_sptr(*iter_b);
					ClientSPtrSetIter     tmp_iter(iter_b);
					++iter_b;
					send_entry_list_.front().client_set_.erase(tmp_iter);
					std::cout << "Removed a connection from " << socket_sptr << "." <<
						std::endl;
				}
			}

			return(true);
		}

		SendRecvSpec     send_recv_spec_;
		SendEntryList   &send_entry_list_;

	private:
		RecvMCFwd & operator = (const RecvMCFwd &other);
	};

public:
	typedef MLB::SocketIo::StreamAcceptorAsyncFwd<TimeLoggerSrvFwd> MyRecvType;

	TimeLoggerSrvFwd(boost::asio::io_service &io_service,
		PacketForwarder &controller_ref, const SendRecvSpec &send_recv_spec)
		:io_service_(io_service)
		,controller_ref_(controller_ref)
		,send_recv_spec_(send_recv_spec)
		,send_entry_list_(controller_ref_.GetSendEntryListRef())
		,recv_mc_sptr_(MyRecvMCTypeSPtr(new MyRecvMCType(
			RecvMCFwd(send_recv_spec_, send_entry_list_), 65536, io_service_,
			send_recv_spec_.data_spec_)))
	{
	}

	bool operator () (MyRecvType & /* receiver */,
		const boost::system::error_code &error_code,
		StreamSocketAsyncSPtr &new_socket_sptr)
	{
		if (error_code) {
			std::ostringstream o_str;
			o_str << "Encountered error code " << error_code << " on an "
				"asynchronous accept attempt --- exiting.";
			MLB::Utility::ThrowLogicError(o_str.str());
		}

		new_socket_sptr->SetSockOpt_TcpNoDelay(
			controller_ref_.GetTcpNoDelayFlag());

		if (!recv_mc_sptr_->IsRunning())
			recv_mc_sptr_->Run();

		if (!send_entry_list_.empty()) {
			send_entry_list_.front().client_set_.insert(new_socket_sptr);
			std::cout << "Accepted a connection from " << new_socket_sptr << "." <<
				std::endl;
		}

		return(true);
	}

	boost::asio::io_service &io_service_;
	PacketForwarder         &controller_ref_;
	SendRecvSpec             send_recv_spec_;
	SendEntryList           &send_entry_list_;
	MyRecvMCTypeSPtr         recv_mc_sptr_;

private:
	TimeLoggerSrvFwd & operator = (const TimeLoggerSrvFwd &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<TimeLoggerSrvFwd> TimeLoggerSrvFwdSPtr;
typedef std::vector<TimeLoggerSrvFwdSPtr>   TimeLoggerSrvFwdSPtrList;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_UTILITY) ClientConnectException :
	public MLB::Utility::ExceptionGeneral {
public:
	ClientConnectException(const char *except_string)
		:MLB::Utility::ExceptionGeneral(except_string)
	{
	}

	virtual void Rethrow(const char *except_string = NULL) const
	{
		ClientConnectException tmp_except(*this);

		tmp_except.SetWhat(except_string);

		throw tmp_except;
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct TimeLoggerClnFwd {
private:
	typedef boost::shared_ptr<PacketSenderMC> PacketSenderMCSPtr;

public:
	typedef MLB::SocketIo::StreamSocketAsyncFwd<TimeLoggerClnFwd> MyRecvType;

	TimeLoggerClnFwd(boost::asio::io_service &io_service,
		PacketForwarder &controller_ref, const SendRecvSpec &send_recv_spec)
		:controller_ref_(controller_ref)
		,send_recv_spec_(send_recv_spec)
		,send_mc_sptr_(new PacketSenderMC(io_service, send_recv_spec_.data_spec_))
		,recv_length_(0)
		,buffered_length_(0)
	{
	}

	//	Client asynchronous socket receive method...
	bool operator () (MyRecvType & /* receiver */,
		const boost::system::error_code &error_code,
		std::size_t bytes_received, const void *data_buffer)
	{
		if (error_code.value() == 2)
			throw ClientConnectException("Encountered error code 2 on an "
				"asynchronous receive attempt --- server has probably "
				"disconnected.");
		else if (error_code) {
			std::ostringstream o_str;
			o_str << "Encountered error code " << error_code << " on an "
				"asynchronous receive attempt --- exiting.";
			MLB::Utility::ThrowLogicError(o_str.str());
		}

/*
	CODE NOTE: Logging point.
std::cout << "CLIENT: RECV LENGTH " << std::setw(5) << bytes_received << std::endl;
*/

		if (!bytes_received)
			return(true);

		char        *dst_ptr   = recv_buffer_ + buffered_length_;
		const char  *src_ptr   = reinterpret_cast<const char *>(data_buffer);
		std::size_t  src_count = bytes_received;

		while (src_count) {
			if (buffered_length_ < sizeof(MsgLengthType)) {
				std::size_t this_length = std::min(src_count,
					sizeof(MsgLengthType) - buffered_length_);
				::memcpy(dst_ptr, src_ptr, this_length);
				dst_ptr          += this_length;
				src_ptr          += this_length;
				src_count        -= this_length;
				buffered_length_ += this_length;
				if (buffered_length_ == sizeof(MsgLengthType)) {
					::memcpy(&recv_length_, recv_buffer_, sizeof(MsgLengthType));
					recv_length_ = ntohs(recv_length_);
				}
				if (!src_count)
					return(true);
			}
			else if (buffered_length_ >= recv_length_)
				MLB::Utility::ThrowLogicError("Internal error detected: the "
					"current buffered length (" +
					MLB::Utility::AnyToString(buffered_length_) + "is not less "
					"than the expected total buffer receive length (" +
					MLB::Utility::AnyToString(recv_length_) + ").");
			std::size_t remaining_length = recv_length_ - buffered_length_;
			std::size_t this_length      = std::min(src_count, remaining_length);
			::memcpy(dst_ptr, src_ptr, this_length);
			if (remaining_length > this_length) {
				buffered_length_ += this_length;
				return(true);
			}
			try {
				std::size_t send_length = recv_length_ - sizeof(MsgLengthType);
				recv_length_      = 0;
				buffered_length_  = 0;
				dst_ptr           = recv_buffer_;
				src_ptr          += this_length;
				src_count        -= this_length;
				send_mc_sptr_->SendTo(send_length,
					recv_buffer_ + sizeof(MsgLengthType));
/*
	CODE NOTE: Logging point.
std::cout << "CLIENT: SENT LENGTH " << std::setw(5) << send_length << std::endl;
*/
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except, "Attempt to send packet data on "
					"channel " + send_recv_spec_.data_spec_.ToString() + " which "
					"was received from the source server at " +
					send_recv_spec_.host_spec_.ToString() + " failed: " +
					std::string(except.what()));
			}
		}

		return(true);
	}

	PacketForwarder    &controller_ref_;
	SendRecvSpec        send_recv_spec_;
	PacketSenderMCSPtr  send_mc_sptr_;
	MsgLengthType       recv_length_;
	std::size_t         buffered_length_;
	char                recv_buffer_[sizeof(MsgLengthType) + MaxPacketSize];

private:
	TimeLoggerClnFwd & operator = (const TimeLoggerClnFwd &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct TimeLoggerTimeFwd {
	typedef MLB::SocketIo::TimerHandlerFwd<TimeLoggerTimeFwd> MyTimerType;

	TimeLoggerTimeFwd(PacketForwarder &controller_ref)
		:controller_ref_(controller_ref)
	{
	}

	bool operator () (MLB::SocketIo::TimerHandler & /* timer_handler */,
		const boost::system::error_code &error_code)
	{
		if (error_code)
			return(false);

/*
	CODE NOTE: Logging point.
		std::cout << "TIME CHECK: " << MLB::Utility::TimeSpec() << std::endl;
*/

		return(true);
	}

private:
	PacketForwarder &controller_ref_;

	TimeLoggerTimeFwd & operator = (const TimeLoggerTimeFwd &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class ClientTimerHandlerCriticalEvent :
	public MLB::SocketIo::TimerHandlerCriticalEvent {
public:
	ClientTimerHandlerCriticalEvent(boost::asio::io_service &io_service,
		unsigned int gran_ticks, unsigned int gran_usecs = 100000)
		:TimerHandlerCriticalEvent(io_service, gran_ticks, gran_usecs)
	{
	}
	virtual ~ClientTimerHandlerCriticalEvent()
	{
	}

protected:
	virtual bool TimerEventImpl(const boost::system::error_code &/* asio_code */)
	{
		if (MLB::Utility::CriticalEventTest()) {
			try {
				timer_.get_io_service().stop();
			}
			catch (const std::exception &) {
				throw;
			}
			return(false);
		}

		return(true);
	}

private:
	ClientTimerHandlerCriticalEvent(
		const ClientTimerHandlerCriticalEvent &other);
	ClientTimerHandlerCriticalEvent & operator = (
		const ClientTimerHandlerCriticalEvent &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketForwarder::ParseCmdLine(int argc, char **argv)
{
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE (as of " << GetCompilationDateTime() << "): " <<
			std::endl <<
			"   " << argv[0] << " " <<
			"[ -server <boolean> ( = false) ] " <<
			"[ -send_specification <udp-or-ip-mc-spec>=<dst-spec> ] " <<
			"[ -receive_specification <udp-or-ip-mc-spec>=<src-spec> ] " <<
			"[ -retry_connection <boolean> ( = " <<
            MLB::Utility::AnyToString(retry_connect_flag_) << " ) ] " <<
			"[ -retry_seconds <seconds> ( = " << retry_seconds_ << " ) ] " <<
			"[ -tcp_no_delay <boolean> ( = " <<
				MLB::Utility::AnyToString(tcp_no_delay_flag_) << " ) ]" <<
			std::endl << std::endl <<
"\
   -help\n\
      Produces this output.\n\n\
   -server <boolean>\n\
      If 'true', specifies that the program is to run in server mode. The\n\
      default is to run in client mode.\n\n\
   -send_specification <udp-or-ip-mc-spec>=<dst-spec>\n\
      Specifies that UDP or IP multicast packets received on the socket\n\
      specified by <udp-or-ip-mc-spec> are to be forwarded to any TCP/IP\n\
      clients which connect on <dst-spec>.\n\n\
      A <udp-or-ip-mc-spec> has the form:\n\n\
         <ip-multicast-group>/<ip-port-or-service>[/<local-interface>]\n\n\
      A <dst-spec> has the form:\n\n\
         <ip-port-or-service>[/<local-interface>]\n\n\
      Note that more than one -send_specification may be present.\n\n\
   -receive_specification <udp-or-ip-mc-spec>=<src-spec>\n\
      Specifies that the program is to open a TCP/IP connection to the server\n\
      indicated by <src-spec> and that any packets which are received over\n\
      that connection are to be published as the appropriate packet type\n\
      (UDP or IP multicast) as specified by <udp-or-ip-mc-spec>.\n\n\
      A <udp-or-ip-mc-spec> has the form:\n\n\
         <ip-multicast-group>/<ip-port-or-service>[/<local-interface>]\n\n\
      A <src-spec> has the form:\n\n\
         <host-name-or-address>/<ip-port-or-service>[/<local-interface>]\n\n\
      Note that more than one -receive_specification may be present.\n\n\
   -retry_connection <boolean>\n\
      Specifies for a client that connection to a server should be retried\n\
      if the initial connection attempt fails or a successful connection is\n\
      subsequently broken.\n\n\
      If not specified, this parameter defaults to '" <<
			MLB::Utility::AnyToString(retry_connect_flag_) << "'.\n\n\
   -retry_seconds <seconds>\n\
      Specifies for a client the number of seconds to wait between attempts\n\
      to connect (or reconnect) to a server.\n\n\
      This parameter is effective only if the '-retry_connection' parameter\n\
      has been specified as 'true'.\n\n\
      If not specified, this parameter defaults to " <<
			retry_seconds_ << ".\n\n\
   -tcp_no_delay <boolean>\n\
      Specifies for a server whether client sockets will be opened with the\n\
      'setsockopt()' flag 'TCP_NODELAY' flag set to 'true'.\n\n\
      If not specified, this parameter defaults to '" <<
			MLB::Utility::AnyToString(tcp_no_delay_flag_) << "'." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	unsigned int count_1;
	std::string              tmp_string;
	std::vector<std::string> tmp_list;
	SendEntrySet             send_entry_set;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (SocketIoParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-IS_SERVER")
				("-ISSERVER")
				("-SERVER")
				("-IS_SRVR")
				("-ISSRVR")
				("-SRVR")
				("-IS_SRV")
				("-ISSRV")
				("-SRV")
					,
			count_1, argc, argv, is_server_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLineDatum(
			MLB::Utility::RegexParamNameAdaptor(
			"^\\-SE*ND_*SPEC(IFICATION)*$",
			count_1, argc, argv), count_1, argc, argv, tmp_string)) {
			try {
				MLB::Utility::SplitString(tmp_string, "=", tmp_list, 0, false);
				if (tmp_list.size() != 2)
					MLB::Utility::ThrowInvalidArgument("Expected format is '"
						"<udp-or-ip-mc-spec>=<dst-spec>'.");
				SocketSpec      lhs_spec;
				SocketSpecLocal rhs_spec;
				lhs_spec.FromString(tmp_list[0]);
				rhs_spec.FromString(tmp_list[1]);
				SendRecvSpec     tmp_spec(lhs_spec, rhs_spec, 0);
				SendEntrySetIter iter_f(send_entry_set.find(SendEntry(tmp_spec)));
				if (iter_f == send_entry_set.end()) {
					tmp_spec.send_index_ = send_entry_list_.size();
					send_spec_mset_.insert(tmp_spec);
					send_entry_list_.push_back(SendEntry(tmp_spec));
				}
				else {
					tmp_spec.send_index_ = iter_f->send_index_;
					send_spec_mset_.insert(tmp_spec);
				}				
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except, "The '" +
					std::string(argv[count_1 - 1]) + "' parameter ('" +
					std::string(argv[count_1]) + "') is invalid: " +
					std::string(except.what()));
			}
		}
		else if (SocketIoParseCmdLineArg::ParseCmdLineDatum(
			MLB::Utility::RegexParamNameAdaptor(
			"^\\-((RECEIVE)|(RECV)|(RCV))_*SPEC(IFICATION)*$",
			count_1, argc, argv), count_1, argc, argv, tmp_string)) {
			try {
				MLB::Utility::SplitString(tmp_string, "=", tmp_list, 0, false);
				if (tmp_list.size() != 2)
					MLB::Utility::ThrowInvalidArgument("Expected format is '"
						"<udp-or-ip-mc-spec>=<src-spec>'.");
				SocketSpec lhs_spec;
				SocketSpec rhs_spec;
				lhs_spec.FromString(tmp_list[0]);
				rhs_spec.FromString(tmp_list[1]);
				recv_spec_mset_.insert(SendRecvSpec(lhs_spec, rhs_spec, 0));
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except, "The '" +
					std::string(argv[count_1 - 1]) + "' parameter ('" +
					std::string(argv[count_1]) + "') is invalid: " +
					std::string(except.what()));
			}
		}
		else if (SocketIoParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-RETRY_CONNECTION")
				("-RETRYCONNECTION")
				("-RETRY_CONNECT")
				("-RETRYCONNECT")
				("-RETRY_CONN")
				("-RETRYCONN")
					,
			count_1, argc, argv, retry_connect_flag_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLineNumeric<
			std::vector<std::string>, unsigned int>(
			MLB::Utility::MakeInlineVector<std::string>
				("-RETRY_SECONDS")
				("-RETRYSECONDS")
				("-RETRY_SECOND")
				("-RETRYSECOND")
				("-RETRY_SECS")
				("-RETRYSECS")
				("-RETRY_SEC")
				("-RETRYSEC")
				("-RETRY_TIME")
				("-RETRYTIME")
					,
			count_1, argc, argv, retry_seconds_, 1))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLineTcpNoDelayFlag(count_1,
			argc, argv, tcp_no_delay_flag_))
			;
		else
			SocketIoParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	if (is_server_ && send_spec_mset_.empty())
		MLB::Utility::ThrowInvalidArgument("Server operation was specified, "
			"but no '-send_specification' parameters were found.");
	else if ((!is_server_) && recv_spec_mset_.empty())
		MLB::Utility::ThrowInvalidArgument("Client operation was specified, "
			"but no '-receive_specification' parameters were found.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketForwarder::Run()
{
	if (is_server_)
		RunServer();
	else
		RunClient();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketForwarder::RunClient()
{
	std::cout << "PacketForwarder: Running in client mode on pid " <<
		MLB::Utility::CurrentProcessId() << "." << std::endl << std::endl;

	for ( ; ; ) {
		try {
			boost::asio::io_service         io_service;
			ClientTimerHandlerCriticalEvent critical_timer(io_service, 1, 100000);
			TimeLoggerClnFwd                receive_functor(io_service,
				*this, *recv_spec_mset_.begin());
			StreamSocketAsyncFwd<TimeLoggerClnFwd> receiver(receive_functor,
				1000000, io_service);
			critical_timer.Run();
			try {
				receiver.Connect(recv_spec_mset_.begin()->host_spec_);
			}
			catch (const std::exception &except) {
				throw ClientConnectException(except.what());
			}
			std::cout << "Connected to server at " <<
				recv_spec_mset_.begin()->host_spec_ << "." << std::endl;
			receiver.Run();
			io_service.run();
		}
		catch (const ClientConnectException &except) {
			if (!retry_connect_flag_)
				throw;
			std::cout << "Server connection error: " << except.what() << std::endl;
		}
		if (MLB::Utility::CriticalEventTest())
			throw MLB::Utility::ExceptionCriticalEvent();
		std::cout << std::endl << "Will retry a connection to a server at " <<
			recv_spec_mset_.begin()->host_spec_ << " in " << retry_seconds_ <<
			" seconds." << std::endl;
		MLB::Utility::TimeT end_time(MLB::Utility::TimeT().AddSeconds(retry_seconds_));
		while (MLB::Utility::TimeT() < end_time) {
			if (MLB::Utility::CriticalEventTest())
				throw MLB::Utility::ExceptionCriticalEvent();
			MLB::Utility::SleepMilliSecs(100);
		}
		std::cout << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketForwarder::RunServer()
{
	send_entry_list_.push_back(SendEntry(*send_spec_mset_.begin()));

	std::cout << "PacketForwarder: Running in server mode on pid " <<
		MLB::Utility::CurrentProcessId() << "." << std::endl << std::endl;

	std::cout << "PacketForwarder: Waiting for clients on " <<
		MLB::Utility::GetHostNameCanonical() << "/" <<
		send_spec_mset_.begin()->host_spec_.ip_port_ << std::endl;

	boost::asio::io_service   io_service;
	TimerHandlerCriticalEvent critical_timer(io_service, 1, 100000);
	TimeLoggerSrvFwd          accept_functor(io_service, *this,
		*send_spec_mset_.begin());
	TimeLoggerTimeFwd         timer_function(*this);

	StreamAcceptorAsyncFwd<TimeLoggerSrvFwd> acceptor(accept_functor,
		0xFFFF + 1, io_service, send_spec_mset_.begin()->host_spec_, true);

	acceptor.SetSockOpt_ReuseAddr(true);

	TimerHandlerFwd<TimeLoggerTimeFwd> periodic_timer(
		timer_function, io_service, 100, 100000);

	critical_timer.Run();
	acceptor.Listen();

	try {
		acceptor.Run();
		io_service.run();
	}
	catch (const std::exception &) {
		try {
			send_entry_list_.clear();
		}
		catch (const std::exception &) {
		}
		throw;
	}

	send_entry_list_.clear();
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		PacketForwarder                    time_logger(argc, argv);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << argv[0] << ": fatal error encountered: " <<
			except.what() << std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

