// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	SocketBridge.cpp

	File Description	:	Bridges INET protocol family sockets.

	Revision History	:	2017-08-03 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2017 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketSenderMC.hpp>
#include <SocketIo/PacketFileReader.hpp>
#include <SocketIo/SocketIoParseCmdLineArg.hpp>
#include <SocketIo/SocketSettings.hpp>

#include <Utility/InlineContainer.hpp>
#include <Utility/Sleep.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

namespace {

// ////////////////////////////////////////////////////////////////////////////
typedef std::pair<SocketSpec, SocketSpec>   SocketBridgeSrcDstPair;
typedef std::vector<SocketBridgeSrcDstPair> SocketBridgeSrcDstPairVec;
typedef std::set<SocketBridgeSrcDstPair>    SocketBridgeSrcDstPairSet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class CannedPacketSendBase {
#ifdef _MSC_VER
	typedef volatile unsigned __int32   MyVolatileValue;
#elif defined(__linux__)
	typedef volatile u_int32_t          MyVolatileValue;
#else
	typedef volatile unsigned int       MyVolatileValue;
#endif // #ifdef _MSC_VER

protected:
	CannedPacketSendBase(int argc, char **argv);
	virtual ~CannedPacketSendBase();

	std::string             capture_file_name_;
	PacketFormat            packet_format_;
	std::vector<SocketSpec> mc_spec_list_;
	std::string             mc_interface_;
	unsigned int            delay_usecs_;
	unsigned long long      spin_sleep_;
	int                     time_to_live_;
	unsigned int            first_packet_index_;
	unsigned int            max_packet_count_;
	unsigned int            iteration_count_;
	MyVolatileValue         my_volatile_value_;

	SocketBridgeSrcDstPairVec pair_list_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct MyParamsSocketIoParseCmdLineArg : public SocketIoParseCmdLineArg {
	bool ParseSrcDstPair(unsigned int &current_index, int argc, char **argv,
		SocketBridgeSrcDstPair &out_datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SocketBridgeSrcDstPair MyParamsSocketIoParseCmdLineArg::ParseSrcDstPair(
	const std::string &in_src_dst_pair)
{
	SocketBridgeSrcDstPair out_src_dst_pair;

	try {
		const char               *error_text =
			"Invalid source/destination pair string";
		std::vector<std::string> tmp_list;
		MLB::Utility::SplitString(in_src_dst_pair, "/", tmp_list, 0, false);
		if (tmp_list.size() != 2)
			MLB::Utility::ThrowInvalidArgument("Invalid format encountered: "
				"Expected format is <source-socket-spec>/"
				"<destination-socket-spec>.");
		error_text = "Invalid source socket specification";
		out_src_dst_pair.first.FromString(tmp_list[0], ':');
		error_text = "Invalid destination socket specification";
		out_src_dst_pair.second.FromString(tmp_list[1], ':');
		error_text = "Invalid combined source/destination pair";
		if (out_src_dst_pair.first == out_src_dst_pair.second)
			MLB::Utility::ThrowInvalidArgument("Source and destination socket "
				"specifications are identical.");
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Unable to parse the socket bridge string '" <<
			in_src_dst_pair << "' : " << error_text << ": " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketSendBase::CannedPacketSendBase(int argc, char **argv)
	:capture_file_name_()
	,packet_format_(PacketFormat_Raw)
	,mc_spec_list_()
	,mc_interface_()
	,delay_usecs_(1000)
	,spin_sleep_(0)
	,time_to_live_(-1)
	,first_packet_index_(0)
	,max_packet_count_(0)
	,iteration_count_(1)
	,my_volatile_value_(0)
{
	if (SocketIoParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"-capture_file <capture-file> " <<
			"<socket-pair-spec> [ <socket-pair-spec> ... ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -capture_file <capture-file>\n\
      Specifies the name of the packet file which contains the packets to be\n\
      processed.\n\n\
   <socket-pair-spec>\n\
      Specifies the source and destination sockets which are to be bridged.\n\n\
      An <socket-pair-spec> has the following format:\n\
         <source-socket-spec>/<destination-socket-spec>\n\n\
      The source and destination socket specs have the same format:\n\n\
         <host-or-ip-address>:<service-or-ip-port>[:<host-interface>]\n\n\
      It is an error if the two socket specifications are identical." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	unsigned int              count_1;
	std::string               tmp_string;
	SocketBridgeSrcDstPairSet pair_set;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
 		if (SocketIoParseCmdLineArg::ParseCmdLineDatum<std::string>(
			MLB::Utility::RegexParamNameAdaptor(
				"^--?CAP(TURE)?(-|_)?FILE((-|_)?NAME)?$",
			count_1, argc, argv), count_1, argc, argv, capture_file_name_))
			;
/*
		else if (SocketIoParseCmdLineArg::ParseCmdLinePacketFormat(count_1, argc,
			argv, packet_format_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLineMCAddressList(count_1,
			argc, argv, mc_spec_list_))
			;
 		else if (SocketIoParseCmdLineArg::ParseCmdLineDatum<std::string>(
			MLB::Utility::RegexParamNameAdaptor(
				"^--?(HOST(-|_)?)?INTERFACE(-|_)?(ADDR(ESS)?)?$",
			count_1, argc, argv), count_1, argc, argv, mc_interface_)) {
			try {
				StringToIpAddress(mc_interface_);
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except,
					std::string("Invalid host interface: ") + except.what());
			}
		}
		else if (SocketIoParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-DELAY_MICROSECONDS")
			("-DELAYMICROSECONDS")
			("-DELAY_MICROSECS")
			("-DELAYMICROSECS")
			("-DELAY_USECONDS")
			("-DELAYUSECONDS")
			("-DELAY_USECS")
			("-DELAYUSECS")
			("-DELAY"),
			count_1, argc, argv, delay_usecs_))
			;
 		else if (SocketIoParseCmdLineArg::ParseCmdLineDatum(
			MLB::Utility::RegexParamNameAdaptor(
				"^\\--*SPIN_*((SLEEP)|((CYCLE)S*)|(WAIT))$",
				count_1, argc, argv), count_1, argc, argv, spin_sleep_))
			;
 		else if (SocketIoParseCmdLineArg::ParseCmdLineDatum<int>(
			MLB::Utility::RegexParamNameAdaptor(
				"^\\-\\-*(PACKET(_|\\-)*)*((TTL)|(HOPS*)|"
				"((TIME|HOPS*)(_|\\-)*TO(_|\\-)*LIVE))$",
			count_1, argc, argv), count_1, argc, argv, time_to_live_))
			;
*/
		else if (argv[count_1][0] != '-') {
			SocketBridgeSrcDstPair new_pair(MyParamsSocketIoParseCmdLineArg::
				ParseSrcDstPair(argv[count_1]));
			if (!pair_set.insert(new_pair).second) {
				std::ostringstream o_str;
				o_str << "Duplicate src/dst socket pair encountered (" <<
					new_pair.first << "/" << new_pair.second << ").";
				MLB::Utility::ThrowInvalidArgument(o_str.str());
			}
			pair_list_.push_back(new_pair);
		}
		else
			SocketIoParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	if (pair_list_.empty())
		MLB::Utility::ThrowInvalidArgument("No source/destination socket pairs "
			"were specified.");
/*
	if (capture_file_name_.empty())
		MLB::Utility::ThrowInvalidArgument("The packet file name was not "
			"specified.");

	if (mc_spec_list_.empty() && (packet_format_ != PacketFormat_IpSource) &&
		(packet_format_ != PacketFormat_TimeIpSource) &&
		(packet_format_ != PacketFormat_IpSourceTLen))
		MLB::Utility::ThrowInvalidArgument("No sending addresses were "
			"specified, and the packet file format is not " +
			PacketFormatToString(PacketFormat_IpSource) + " or " +
			PacketFormatToString(PacketFormat_TimeIpSource) + " or " +
			PacketFormatToString(PacketFormat_IpSourceTLen) + ".");
	else if ((!mc_spec_list_.empty()) &&
		((packet_format_ == PacketFormat_IpSource) ||
		 (packet_format_ == PacketFormat_TimeIpSource) ||
		 (packet_format_ == PacketFormat_IpSourceTLen)))
		std::cerr << "WARNING: Packet format is " << packet_format_ << ", but "
			"the IP addresses in the file will not be used. Instead, all packets "
			"will be sent on " << mc_spec_list_[0] << ", which was specified on "
			"the command line." << std::endl;
	else if ((!mc_spec_list_.empty()) && (!mc_interface_.empty()))
		std::cerr << "A multicast group was specified on the command line as "
			"the packet destination (" << mc_spec_list_[0] << "), but an "
			"interface (" << mc_interface_ << ") was specified separately and "
			"will be ignored." << std::endl;

	if (spin_sleep_)
		delay_usecs_ = 0;
*/
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketSendBase::~CannedPacketSendBase()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class CannedPacketSend : public CannedPacketSendBase {
	typedef boost::shared_ptr<PacketSenderMC>        PacketSenderMCSPtr;
	typedef std::map<SocketSpec, PacketSenderMCSPtr> PacketSenderMCSPtrMap;
public:
	CannedPacketSend(int argc, char **argv, boost::asio::io_service &io_service);

	void Run();

private:
	boost::asio::io_service &io_service_;
	PacketFileReader         packet_file_reader_;
	PacketSenderMCSPtr       sender_mc_sptr_;
	PacketSenderMCSPtrMap    sender_mc_map_;

	void RunInternal();

	CannedPacketSend(const CannedPacketSend &other);
	CannedPacketSend operator = (const CannedPacketSend &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketSend::CannedPacketSend(int argc, char **argv,
	boost::asio::io_service &io_service)
	:CannedPacketSendBase(argc, argv)
	,io_service_(io_service)
	,packet_file_reader_(capture_file_name_, packet_format_)
	,sender_mc_sptr_()
	,sender_mc_map_()
{
	if (!mc_spec_list_.empty()) {
		sender_mc_sptr_.reset(
			new PacketSenderMC(io_service_, mc_spec_list_[0]));
		if (time_to_live_ > -1)
			SetSockOpt_TimeToLive(sender_mc_sptr_->socket_, time_to_live_);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketSend::Run()
{
	RunInternal();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketSend::RunInternal()
{
	unsigned int count_1;
	unsigned int packet_count = 0;

	std::cout << "File Name    : " << capture_file_name_ << std::endl;
	std::cout << "File Size    : " << std::setw(22) <<
		packet_file_reader_.GetFileSize() << std::endl;
	std::cout << "Packet Count : " << std::setw(22) <<
		packet_file_reader_.GetPacketCount() << std::endl;
	std::cout << "Packet Format: " << packet_format_ << std::endl;
	std::cout << "MGroup Info  : " << ((mc_spec_list_.empty()) ?
		"??? (specified by packet)" : mc_spec_list_[0].ToString()) << std::endl;
	std::cout << "Interface    : " <<
		((mc_interface_.empty()) ? "* DEFAULT *" : mc_interface_) << std::endl;
	std::cout << "Time-to-Live : " <<
		((time_to_live_ == -1) ? "DEFAULT" :
		MLB::Utility::AnyToString(time_to_live_)) << std::endl;
	std::cout << "Sleep uSecs  : " << std::setw(22) <<
		delay_usecs_ << std::endl;
	std::cout << "Spin Sleep   : " << std::setw(22) <<
		spin_sleep_ << std::endl;
	std::cout << "First Index  : " << std::setw(22) <<
		first_packet_index_ << std::endl;
	std::cout << "Max Packets  : " << std::setw(22) <<
		max_packet_count_ << std::endl;
	std::cout << "Iterations   : " << std::setw(22) <<
		iteration_count_ << std::endl;

	std::cerr <<
		"[" << std::setw(22) << 0 << "][" << std::setw(22) << 0 << "][" <<
		std::setw(22) << 0 << "]" << std::flush;

	for (count_1 = 0; count_1 < iteration_count_; ++count_1) {
		unsigned int   packet_index = 0;
		std::size_t    packet_length;
		const void    *packet_ptr;
		IpAddressType  ip_address;
		IpPortType     ip_port;
		while (packet_file_reader_.GetPacket(packet_length, packet_ptr,
			ip_address, ip_port)) {
			if (packet_index++ >= first_packet_index_) {
				++packet_count;
				if (sender_mc_sptr_.get() != NULL)
					sender_mc_sptr_->SendTo(packet_length, packet_ptr);
				else {
					SocketSpec tmp_spec(ip_address, ip_port, mc_interface_);
					PacketSenderMCSPtrMap::const_iterator iter_f(
						sender_mc_map_.find(tmp_spec));
					if (iter_f != sender_mc_map_.end())
						iter_f->second->SendTo(packet_length, packet_ptr);
					else {
						sender_mc_map_[tmp_spec] = PacketSenderMCSPtr(
							new PacketSenderMC(io_service_, tmp_spec));
						if (time_to_live_ > -1)
							SetSockOpt_TimeToLive(
								sender_mc_map_[tmp_spec]->socket_, time_to_live_);
						sender_mc_map_[tmp_spec]->SendTo(packet_length, packet_ptr);
					}
				}
				if (spin_sleep_) {
					for (unsigned long long count_2 = 0; count_2 < spin_sleep_;
						++count_2)
						my_volatile_value_ = 666;
				}
				else if (delay_usecs_)
					MLB::Utility::SleepMicroSecs(delay_usecs_);
			}
			if (max_packet_count_ && (packet_count >= max_packet_count_))
				return;
			if (packet_count && (!(packet_count % 1000)))
				std::cerr <<
					MLB::Utility::PadLeft("", 1 + 22 + 2 + 22 + 2 + 22 + 1, '\b') <<
					"[" << std::setw(22) << packet_count << "][" <<
					std::setw(22) << count_1 << "][" << std::setw(22) <<
					packet_index << "]" << std::flush;
		}
		packet_file_reader_.SeekToOffset(0);
	}

	std::cerr <<
		MLB::Utility::PadLeft("", 1 + 22 + 2 + 22 + 2 + 22 + 1, '\b') <<
		"[" << std::setw(22) << packet_count << "][" <<
		std::setw(22) << count_1 << "][" << std::setw(22) << 0 << "]" <<
		std::endl;
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
		boost::asio::io_service io_service;
		CannedPacketSend        canned_packet_send(argc, argv, io_service);
		canned_packet_send.Run();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << argv[0] << ": fatal error encountered: " <<
			except.what() << std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

