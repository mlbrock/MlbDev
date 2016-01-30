// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Captures UDP/IP Multicast packets into a file.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
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
#include <SocketIo/TimerHandlerCriticalEvent.hpp>
#include <SocketIo/PacketReceiverMC.hpp>
#include <SocketIo/PacketReceiverAsyncMCFwd.hpp>
#include <SocketIo/PacketFileWriter.hpp>
#include <SocketIo/SocketIoParseCmdLineArg.hpp>

#include <Utility/InlineContainer.hpp>
#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

namespace {

// ////////////////////////////////////////////////////////////////////////////
class CannedPacketRecvBase {
protected:
	CannedPacketRecvBase(int argc, char **argv);
	virtual ~CannedPacketRecvBase();

	std::string             packet_file_name_;
	PacketFormat            packet_format_;
	std::vector<SocketSpec> mc_spec_list_;
	unsigned int            max_packet_count_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketRecvBase::CannedPacketRecvBase(int argc, char **argv)
	:packet_file_name_()
	,packet_format_(PacketFormat_Raw)
	,mc_spec_list_()
	,max_packet_count_(0)
{
	if (SocketIoParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"-packet_file <packet-file> " <<
			"-multicast_address <ip-spec>[,<ip-spec>...] " <<
			"[ -packet_format <packet-format> ] " <<
			"[ -max_packet_count <maximum-number-of-packets> ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -packet_file <packet-file>\n\
      Specifies the name of the packet file into which the receivved packets\n\
      are to be written.\n\n\
   -multicast_address <mc-address>\n\
      Specifies one or more multicast specifications on which the canned data\n\
      packets are to be received.\n\n\
      An <mc-address> has the following format:\n\
         <ip-address>/<ip-port>[/<host-interface>]\n\n\
      Multiple addresses may be specified by separated them with commas.\n\n\
   -packet_format { RAW | LENGTH | TIME[IPSOURCE] | IPSOURCE[TLEN] }\n\
      Optional parameter which specifies the format in which packets are\n\
      to be stored in the packet file:\n\n\
         o RAW          - Each packet immediately follows its predecessor\n\
                          without preamble.\n\n\
         o LENGTH       - Each packet is preceded by its 16-bit packet length\n\
                          in network byte order.\n\n\
         o TIME         - Each packet is preceded by its 16-bit packet length\n\
                          followed by the operating system timestamp of the\n\
                          time the packet was captured. The timestamp is\n\
                          stored as two 32-bit equivalents of the two member\n\
                          data which constitute a 'struct timespec', but are\n\
                          in network byte order.\n\n\
         o IPSOURCE     - Each packet is preceded by it 16-bit packet length\n\
                          followed by the 32-bit IP address of the packet\n\
                          source and the 16-bit port of the packet source in\n\
                          network byte order.\n\n\
         o TIMEIPSOURCE - Each packet is preceded by its 16-bit packet length\n\
                          followed by the operating system timestamp stored\n\
                          as described in the 'TIME' format above. That in\n\
                          turn is followed by the IP address and port number\n\
                          as described in the 'IPSOURCE' format.\n\n\
         o IPSOURCETLEN - As above in the 'IPSOURCE' format except that the\n\
                          length is after the IP address and port (TLEN =\n\
                          'Trailing Length'; backwards compatibility only).\n\n\
      If not specified, this parameter defaults to 'RAW'.\n\n\
   -max_packet_count <maximum-number-of-packets>\n\
      Optional parameter which specifies the maximum number of packets to\n\
      receive before exiting.\n\n\
      If not specified, no such limit will be imposed." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	unsigned int count_1;
	std::string  tmp_string;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (SocketIoParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-PACKET_FILE_NAME")
			("-PACKET_FILENAME")
			("-PACKETFILE_NAME")
			("-PACKETFILENAME")
			("-PACKET_FILE")
			("-PACKETFILE"),
			count_1, argc, argv, packet_file_name_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLinePacketFormat(count_1, argc,
			argv, packet_format_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLineMCAddressList(count_1,
			argc, argv, mc_spec_list_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLineNumeric(
			MLB::Utility::MakeInlineVector<std::string>
			("-MAXIMUM_PACKETS_COUNT")
			("-MAXIMUM_PACKETSCOUNT")
			("-MAXIMUMPACKETS_COUNT")
			("-MAXIMUMPACKETSCOUNT")
			("-MAXIMUM_PACKET_COUNT")
			("-MAXIMUM_PACKETCOUNT")
			("-MAXIMUMPACKET_COUNT")
			("-MAXIMUMPACKETCOUNT")
			("-MAXIMUM_PACKETS")
			("-MAXIMUMPACKETS")
			("-MAXIMUM_PACKET")
			("-MAXIMUMPACKET")
			("-MAX_PACKETS_COUNT")
			("-MAX_PACKETSCOUNT")
			("-MAXPACKETS_COUNT")
			("-MAXPACKETSCOUNT")
			("-MAX_PACKET_COUNT")
			("-MAX_PACKETCOUNT")
			("-MAXPACKET_COUNT")
			("-MAXPACKETCOUNT")
			("-MAX_PACKETS")
			("-MAXPACKETS")
			("-MAX_PACKET")
			("-MAXPACKET"),
			count_1, argc, argv, max_packet_count_))
			;
		else
			SocketIoParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	if (packet_file_name_.empty())
		MLB::Utility::ThrowInvalidArgument("The packet file name was not "
			"specified.");

	if (mc_spec_list_.empty())
		MLB::Utility::ThrowInvalidArgument("No receive addresses were "
			"specified.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketRecvBase::~CannedPacketRecvBase()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class CannedPacketRecv : public CannedPacketRecvBase {
	struct RecvMCFwd;
	typedef PacketReceiverAsyncMCFwd<RecvMCFwd> MyRecvMCType;

	struct RecvMCFwd {
		RecvMCFwd(CannedPacketRecv &dst_ref)
			:dst_ref_(dst_ref)
		{
		}
		bool operator () (MyRecvMCType &mc_receiver,
			const boost::system::error_code &error, std::size_t bytes_received,
			const void *data_buffer,
			const boost::asio::ip::udp::endpoint &sender_endpoint)
		{
			return(dst_ref_.HandlePacket(mc_receiver, error,
				bytes_received, data_buffer, sender_endpoint));
		}

		CannedPacketRecv &dst_ref_;

	private:
		RecvMCFwd & operator = (const RecvMCFwd &other);
	};

	typedef boost::shared_ptr<MyRecvMCType>     MyRecvMCTypeSPtr;
	typedef std::vector<MyRecvMCTypeSPtr>       MyRecvMCTypeSPtrList;
public:
	CannedPacketRecv(int argc, char **argv, boost::asio::io_service &io_service);

	void Run();

private:
	boost::asio::io_service &io_service_;
	PacketFileWriter         packet_file_writer_;
	MyRecvMCTypeSPtrList     recv_handler_list_;
	unsigned long long       packet_count_;

	void RunInternal();
	bool HandlePacket(MyRecvMCType &mc_receiver,
		const boost::system::error_code &error, std::size_t bytes_received,
		const void *data_buffer,
		const boost::asio::ip::udp::endpoint &sender_endpoint);

	CannedPacketRecv(const CannedPacketRecv &other);
	CannedPacketRecv operator = (const CannedPacketRecv &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketRecv::CannedPacketRecv(int argc, char **argv,
	boost::asio::io_service &io_service)
	:CannedPacketRecvBase(argc, argv)
	,io_service_(io_service)
	,packet_file_writer_(packet_file_name_, packet_format_)
	,recv_handler_list_()
	,packet_count_(0)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketRecv::Run()
{
	RunInternal();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketRecv::RunInternal()
{
	std::cout << "File Name    : " << packet_file_name_ << std::endl;
	std::cout << "Packet Format: " << packet_format_ << std::endl;

 	std::vector<SocketSpec>::const_iterator iter_b(mc_spec_list_.begin());
	std::vector<SocketSpec>::const_iterator iter_e(mc_spec_list_.end());
	for ( ; iter_b != iter_e; ++iter_b)
		std::cout << ((iter_b == mc_spec_list_.begin()) ? "MGroup Info  : " :
			"             : ") << iter_b->ToString() << std::endl;

	std::cout << "Max Packets  : " << std::setw(22) <<
		max_packet_count_ << std::endl;

	TimerHandlerCriticalEvent critical_event_detection(io_service_, 1);

	{
		MyRecvMCTypeSPtrList                    tmp_recv_handler_list;
 		std::vector<SocketSpec>::const_iterator iter_b(mc_spec_list_.begin());
		std::vector<SocketSpec>::const_iterator iter_e(mc_spec_list_.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			tmp_recv_handler_list.push_back(MyRecvMCTypeSPtr(
				new MyRecvMCType(RecvMCFwd(*this), 65536, io_service_, *iter_b)));
			tmp_recv_handler_list.back()->SetRecvBufferSize(10000000);
			tmp_recv_handler_list.back()->Run();
		}
		recv_handler_list_.swap(tmp_recv_handler_list);
	}

	std::cerr << "[" << std::setw(22) << 0 << "]" << std::flush;

	io_service_.run();

	std::cerr <<
		MLB::Utility::PadLeft("", 1 + 22 + 2 + 22 + 2 + 22 + 1, '\b') <<
		"[" << std::setw(22) << packet_count_ << "][" <<
		std::setw(22) << 0 << "][" << std::setw(22) << 0 << "]" << std::endl;

	if (MLB::Utility::CriticalEventTest())
		std::cerr << "Server going down due to detection of a signal or other "
			"critical operating system event." << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CannedPacketRecv::HandlePacket(MyRecvMCType &mc_receiver,
	const boost::system::error_code & /* error */, std::size_t bytes_received,
	const void *data_buffer,
	const boost::asio::ip::udp::endpoint &	/* sender_endpoint */)
{
	packet_file_writer_.WritePacket(bytes_received, data_buffer,
		mc_receiver.GetIpAddress(), mc_receiver.GetPort(),
		MLB::Utility::TimeSpec());

	if (!(++packet_count_ % 1000))
		std::cerr << MLB::Utility::PadLeft("", 1 + 22 + 1, '\b') <<
			"[" << std::setw(22) << packet_count_ << "]" << std::flush;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

} // namespace SocketIo

} // namespace MLB

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		boost::asio::io_service io_service;
		CannedPacketRecv        canned_packet_recv(argc, argv, io_service);
		canned_packet_recv.Run();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << argv[0] << ": fatal error encountered: " <<
			except.what() << std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

