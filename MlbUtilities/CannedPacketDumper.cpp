// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Dumps the contents of a packet a file.

	Revision History	:	2011-11-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2011 - 2014.
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

#include <Utility/InlineContainer.hpp>
#include <Utility/Sleep.hpp>
#include <Utility/ToHexString.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

namespace {

// ////////////////////////////////////////////////////////////////////////////
class CannedPacketSendBase {
protected:
	CannedPacketSendBase(int argc, char **argv);
	virtual ~CannedPacketSendBase();

	std::string  packet_file_name_;
	PacketFormat packet_format_;
	unsigned int first_packet_index_;
	unsigned int max_packet_count_;
	bool         emit_packet_contents_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketSendBase::CannedPacketSendBase(int argc, char **argv)
	:packet_file_name_()
	,packet_format_(PacketFormat_Raw)
	,first_packet_index_(0)
	,max_packet_count_(0)
	,emit_packet_contents_(true)
{
	if (SocketIoParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"-packet_file <packet-file> " <<
			"[ -packet_format <packet-format> ] " <<
			"[ -first_packet_index <first-packet-index> ] " <<
			"[ -max_packet_count <maximum-number-of-packets> ] " <<
			"[ -emit_packet_contents <boolean> ]\n\n" <<
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
   -first_packet_index <first-packet-index>\n\
      Optional parameter which specifies the index of the first packet to be\n\
      processed.\n\n\
      Numbering for packet indices begins at '0'.\n\n\
      If not specified, this parameter defaults to 0 (the first packet).\n\n\
   -max_packet_count <maximum-number-of-packets>\n\
      Optional parameter which specifies the maximum number of packets to\n\
      process.\n\n\
      If not specified, all packets will be processed.\n\n\
   -emit_packet_contents <boolean>\n\
      Optional parameter which specifies whether the contents of the packets\n\
      are to be emitted (in hexadecimal format).\n\
      If not specified, this parameter defaults to 'true'." <<
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
		else if (SocketIoParseCmdLineArg::ParseCmdLineDatum(
			MLB::Utility::RegexParamNameAdaptor(
			"^\\-\\-*((EMIT)|(PRINT))(_|\\-)*(PACKETS*(_|\\-)*)*((DATA)|(CONTENTS*))$",
			count_1, argc, argv), count_1, argc, argv, emit_packet_contents_))
			;
		else
			SocketIoParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	if (packet_file_name_.empty())
		MLB::Utility::ThrowInvalidArgument("The packet file name was not "
			"specified.");
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
	std::cout << "File Name    : " << packet_file_name_ << std::endl;
	std::cout << "File Size    : " << std::setw(22) <<
		packet_file_reader_.GetFileSize() << std::endl;
	std::cout << "Packet Count : " << std::setw(22) <<
		packet_file_reader_.GetPacketCount() << std::endl;
	std::cout << "Packet Format: " << packet_format_ << std::endl;
	std::cout << "First Index  : " << std::setw(22) <<
		first_packet_index_ << std::endl;
	std::cout << "Max Packets  : " << std::setw(22) <<
		max_packet_count_ << std::endl;

	unsigned int   packet_index = 0;
	unsigned int   packet_count = 0;
	std::size_t    packet_length;
	const void    *packet_ptr;
	IpAddressType  ip_address;
	IpPortType     ip_port;
	MLB::Utility::TimeSpec packet_timestamp;

	while (packet_file_reader_.GetPacket(packet_length, packet_ptr,
		ip_address, ip_port, packet_timestamp)) {
		if (packet_index++ >= first_packet_index_) {
			std::cout
				<< std::setw(10)              << packet_count       << " "
				<< std::setw(10)              << (packet_index - 1) << " "
				<< packet_timestamp                                 << " "
				<< std::left
				<< std::setw(21) << SocketSpec(ip_address, ip_port) << " "
				<< std::right
				<< std::setw( 5)              << packet_length;
			if (emit_packet_contents_)
				std::cout
					<< " " << MLB::Utility::ToHexString(packet_length, packet_ptr);
			std::cout
				<< std::endl;
			++packet_count;
		}
		if (max_packet_count_ && (packet_count >= max_packet_count_))
			break;
	}
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

