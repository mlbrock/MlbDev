// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Publishes UDP/IP Multicast packets read from a file.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
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

	std::string             packet_file_name_;
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
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketSendBase::CannedPacketSendBase(int argc, char **argv)
	:packet_file_name_()
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
			"-packet_file <packet-file> " <<
			"[ -packet_format <packet-format> ] " <<
			"[ -multicast_address <ip-address>/<ip-port>[/<host-interface>] ] " <<
			"[ -interface_address <host-interface> ] " <<
			"[ -delay <microseconds-between-packets> ( = " << delay_usecs_ <<
				" ) ] " <<
			"[ -spin_sleep <spin-cycles-between-packets> ( = " << spin_sleep_ <<
				" ) ] " <<
			"[ -time_to_live <ttl> ] " <<
			"[ -first_packet_index <first-packet-index> ] " <<
			"[ -max_packet_count <maximum-number-of-packets> ] " <<
			"[ -iteration_count <number-of-times-to-iterate> ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -packet_file <packet-file>\n\
      Specifies the name of the packet file which contains the packets to be\n\
      processed.\n\n\
   -packet_format { RAW | LENGTH | TIME[IPSOURCE] | IPSOURCE[TLEN] }\n\
      Optional parameter which specifies the format in which packets are\n\
      stored in the packet file:\n\n\
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
      If not specified, this parameter defaults to 'RAW'.\n\n"
<<
"\
   -multicast_address <mc-address>\n\
      Specifies the multicast address on which the canned data packets are to\n\
      be sent.\n\n\
      An <mc-address> has the following format:\n\
         <ip-address>/<ip-port>[/<host-interface>]\n\n\
      Note that this parameter is optional only when the program is invoked\n\
      with the '-packet-format' parameter equal to 'IPSOURCE', 'TIMEIPSOURCE',\n\
      or 'IPSOURCETLEN'. In those cases, canned data packets will be sent on\n\
      the multicast address on which each packet was originally received.\n\n\
   -host_interface <host-interface>\n\
      Specifies the interface on which canned data packets are to be sent.\n\n\
   -delay <microsecond-between-packets>\n\
      Specifies the time in microseconds to delay between packet sends.\n\n\
      If not specified, this parameter defaults to " << delay_usecs_ <<
		" microseconds.\n\n\
      Note that this parameter may be overridden if the '-spin_sleep' is also\n\
      specified; see below.\n\n\
   -spin_sleep <spin-cycles-between-packets>\n\
      Specifies the number of spin wait cycles to execute between packet\n\
      sends. A spin wait cycle, in this context, is defined as the time\n\
      required for this program to write a value to a volatile 32-bit\n\
      unsigned integer value.\n\n\
      The expected actual elapsed time of the delay in wall clock units will\n\
      of course vary depending upon the exact hardware and software employed\n\
      in your environment. But it does avoid the problem of 'usleep()' being\n\
      rounded up to millisecond sleeps by earlier Linux versions.\n\n\
      If this parameter is specified as a non-zero value, it will override\n\
      the value of the '-delay' parameter.\n\n\
      If not specified, this parameter defaults to " << spin_sleep_ <<
		" microseconds.\n\n\
   -time_to_live <ttl>\n\
      Specifies the hop count for packets sent by this process.\n\n\
   -first_packet_index <first-packet-index>\n\
      Optional parameter which specifies the index of the first packet to be\n\
      processed.\n\n\
      Numbering for packet indices begins at '0'.\n\n\
      If not specified, this parameter defaults to 0 (the first packet).\n\n\
   -max_packet_count <maximum-number-of-packets>\n\
      Optional parameter which specifies the maximum number of packets to\n\
      process.\n\n\
      If not specified, all packets will be processed.\n\n\
   -iteration_count <number-of-times-to-iterate>\n\
      Optional parameter which specifies the number of times to iterate over\n\
      the packets in the file.\n\n\
      If not specified, this parameter defaults to '1'." <<
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
		else if (SocketIoParseCmdLineArg::ParseCmdLineNumeric(
			MLB::Utility::MakeInlineVector<std::string>
			("-FIRST_PACKET_INDEX")
			("-FIRST_PACKETINDEX")
			("-FIRSTPACKET_INDEX")
			("-FIRSTPACKETINDEX")
			("-FIRST_PACKET")
			("-FIRSTPACKET")
			("-PACKET_INDEX")
			("-PACKETINDEX")
			("-FIRST_INDEX")
			("-FIRSTINDEX"),
			count_1, argc, argv, first_packet_index_))
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
		else if (SocketIoParseCmdLineArg::ParseCmdLineNumeric
			<std::vector<std::string>, unsigned int>(
			MLB::Utility::MakeInlineVector<std::string>
			("-ITERATION_COUNT")
			("-ITERATIONCOUNT")
			("-CYCLE_COUNT")
			("-CYCLECOUNT")
			("-COUNT"),
			count_1, argc, argv, iteration_count_, 1))
			;
		else
			SocketIoParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	if (packet_file_name_.empty())
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
	,packet_file_reader_(packet_file_name_, packet_format_)
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

	std::cout << "File Name    : " << packet_file_name_ << std::endl;
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

