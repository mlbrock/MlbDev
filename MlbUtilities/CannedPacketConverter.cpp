// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts packet files between the various formats.

	Revision History	:	2009-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2009 - 2017.
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
#include <SocketIo/PacketFileReader.hpp>
#include <SocketIo/PacketFileWriter.hpp>
#include <SocketIo/SocketIoParseCmdLineArg.hpp>

#include <Utility/InlineContainer.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

namespace {

// ////////////////////////////////////////////////////////////////////////////
class CannedPacketConverterBase {
protected:
	CannedPacketConverterBase(int argc, char **argv);
	virtual ~CannedPacketConverterBase();

	std::string  src_packet_file_name_;
	std::string  dst_packet_file_name_;
	PacketFormat src_packet_format_;
	PacketFormat dst_packet_format_;
	unsigned int remove_leading_chars_;
	unsigned int first_packet_index_;
	unsigned int max_packet_count_;
	unsigned int iteration_count_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketConverterBase::CannedPacketConverterBase(int argc, char **argv)
	:src_packet_file_name_()
	,dst_packet_file_name_()
	,src_packet_format_(PacketFormat_Raw)
	,dst_packet_format_(PacketFormat_Raw)
	,remove_leading_chars_(0)
	,first_packet_index_(0)
	,max_packet_count_(0)
	,iteration_count_(1)
{
	if (SocketIoParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"-source_packet_file <src-packet-file> " <<
			"-destination_packet_file <dst-packet-file> " <<
			"[ -source_packet_format <packet-format> ] " <<
			"[ -destination_packet_format <packet-format> ] " <<
			"[ -remove_leading_bytes <bytes-count-to-remove> ] " <<
			"[ -first_packet_index <first-packet-index> ] " <<
			"[ -max_packet_count <maximum-number-of-packets> ] " <<
			"[ -iteration_count <number-of-times-to-iterate> ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -source_packet_file <packet-file>\n\
      Specifies the name of the packet file which contains the packets to be\n\
      converted.\n\n\
   -destination_packet_file <packet-file>\n\
      Specifies the name of the packet file to which converted packets are to\n\
      be written.\n\n\
   -source_packet_format { RAW | LENGTH | TIME[IPSOURCE] | IPSOURCE[TLEN] }\n\
      Optional parameter which specifies the format in which packets are\n\
      stored in the source packet file:\n\n\
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
   -destination_packet_format { RAW | LENGTH | TIME[IPSOURCE] | IPSOURCE[TLEN]}\n\
      Optional parameter which specifies the format to which packets are\n\
      to be converted:\n\n\
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
   -remove_leading_bytes <bytes-count-to-remove>\n\
      Specifies the number of bytes at the beginning of each source packet\n\
      which are to be removed by this program.\n\n\
      This option was added specifically to permit the conversion of the new\n\
      CME Fix/FAST 2.0 protocol packets with their five-byte preamble to the\n\
      old format.\n\n\
      If not specified, this parameter defaults to 0 (do not remove leading\n\
      bytes).\n\n\
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
			("-SOURCE_PACKET_FILE_NAME")
			("-SOURCE_PACKET_FILENAME")
			("-SOURCE_PACKETFILE_NAME")
			("-SOURCEPACKET_FILE_NAME")
			("-SOURCE_PACKETFILENAME")
			("-SOURCEPACKET_FILENAME")
			("-SOURCEPACKETFILE_NAME")
			("-SOURCEPACKETFILENAME")
			("-SOURCE_PACKET_FILE")
			("-SOURCE_PACKETFILE")
			("-SOURCEPACKET_FILE")
			("-SOURCEPACKETFILE")
			("-SOURCE_FILE")
			("-SOURCEFILE")
			("-SRC_PACKET_FILE_NAME")
			("-SRC_PACKET_FILENAME")
			("-SRC_PACKETFILE_NAME")
			("-SRCPACKET_FILE_NAME")
			("-SRC_PACKETFILENAME")
			("-SRCPACKET_FILENAME")
			("-SRCPACKETFILE_NAME")
			("-SRCPACKETFILENAME")
			("-SRC_PACKET_FILE")
			("-SRC_PACKETFILE")
			("-SRCPACKET_FILE")
			("-SRCPACKETFILE")
			("-SRC_FILE")
			("-SRCFILE")
				,
			count_1, argc, argv, src_packet_file_name_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-DESTINATION_PACKET_FILE_NAME")
			("-DESTINATION_PACKET_FILENAME")
			("-DESTINATION_PACKETFILE_NAME")
			("-DESTINATIONPACKET_FILE_NAME")
			("-DESTINATION_PACKETFILENAME")
			("-DESTINATIONPACKET_FILENAME")
			("-DESTINATIONPACKETFILE_NAME")
			("-DESTINATIONPACKETFILENAME")
			("-DESTINATION_PACKET_FILE")
			("-DESTINATION_PACKETFILE")
			("-DESTINATIONPACKET_FILE")
			("-DESTINATIONPACKETFILE")
			("-DESTINATION_FILE")
			("-DESTINATIONFILE")
			("-DEST_PACKET_FILE_NAME")
			("-DEST_PACKET_FILENAME")
			("-DEST_PACKETFILE_NAME")
			("-DESTPACKET_FILE_NAME")
			("-DEST_PACKETFILENAME")
			("-DESTPACKET_FILENAME")
			("-DESTPACKETFILE_NAME")
			("-DESTPACKETFILENAME")
			("-DEST_PACKET_FILE")
			("-DEST_PACKETFILE")
			("-DESTPACKET_FILE")
			("-DESTPACKETFILE")
			("-DEST_FILE")
			("-DESTFILE")
			("-DST_PACKET_FILE_NAME")
			("-DST_PACKET_FILENAME")
			("-DST_PACKETFILE_NAME")
			("-DSTPACKET_FILE_NAME")
			("-DST_PACKETFILENAME")
			("-DSTPACKET_FILENAME")
			("-DSTPACKETFILE_NAME")
			("-DSTPACKETFILENAME")
			("-DST_PACKET_FILE")
			("-DST_PACKETFILE")
			("-DSTPACKET_FILE")
			("-DSTPACKETFILE")
			("-DST_FILE")
			("-DSTFILE")
				,
			count_1, argc, argv, dst_packet_file_name_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLinePacketFormat(
			MLB::Utility::MakeInlineVector<std::string>
			("-SOURCE_PACKET_FORMAT")
			("-SOURCE_PACKETFORMAT")
			("-SOURCEPACKET_FORMAT")
			("-SOURCEPACKETFORMAT")
			("-SOURCE_FORMAT")
			("-SOURCEFORMAT")
			("-SRC_PACKET_FORMAT")
			("-SRC_PACKETFORMAT")
			("-SRCPACKET_FORMAT")
			("-SRCPACKETFORMAT")
			("-SRC_FORMAT")
			("-SRCFORMAT")
			("-SOURCE_PACKET_FMT")
			("-SOURCE_PACKETFMT")
			("-SOURCEPACKET_FMT")
			("-SOURCEPACKETFMT")
			("-SOURCE_FMT")
			("-SOURCEFMT")
			("-SRC_PACKET_FMT")
			("-SRC_PACKETFMT")
			("-SRCPACKET_FMT")
			("-SRCPACKETFMT")
			("-SRC_FMT")
			("-SRCFMT")
				,
			count_1, argc, argv, src_packet_format_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLinePacketFormat(
			MLB::Utility::MakeInlineVector<std::string>
			("-DESTINATION_PACKET_FORMAT")
			("-DESTINATION_PACKETFORMAT")
			("-DESTINATIONPACKET_FORMAT")
			("-DESTINATIONPACKETFORMAT")
			("-DESTINATION_FORMAT")
			("-DESTINATIONFORMAT")
			("-DEST_PACKET_FORMAT")
			("-DEST_PACKETFORMAT")
			("-DESTPACKET_FORMAT")
			("-DESTPACKETFORMAT")
			("-DEST_FORMAT")
			("-DESTFORMAT")
			("-DST_PACKET_FORMAT")
			("-DST_PACKETFORMAT")
			("-DSTPACKET_FORMAT")
			("-DSTPACKETFORMAT")
			("-DST_FORMAT")
			("-DSTFORMAT")
			("-DESTINATION_PACKET_FMT")
			("-DESTINATION_PACKETFMT")
			("-DESTINATIONPACKET_FMT")
			("-DESTINATIONPACKETFMT")
			("-DESTINATION_FMT")
			("-DESTINATIONFMT")
			("-DEST_PACKET_FMT")
			("-DEST_PACKETFMT")
			("-DESTPACKET_FMT")
			("-DESTPACKETFMT")
			("-DEST_FMT")
			("-DESTFMT")
			("-DST_PACKET_FMT")
			("-DST_PACKETFMT")
			("-DSTPACKET_FMT")
			("-DSTPACKETFMT")
			("-DST_FMT")
			("-DSTFMT")
				,
			count_1, argc, argv, dst_packet_format_))
			;
		else if (SocketIoParseCmdLineArg::ParseCmdLineDatum(
			MLB::Utility::RegexParamNameAdaptor(
			"^\\-REMOVE(_)*LEAD(ING)*(_)*((BYTE)|(CHAR(ACTER)*))S*$",
			count_1, argc, argv), count_1, argc, argv, remove_leading_chars_))
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

	if (src_packet_file_name_.empty())
		MLB::Utility::ThrowInvalidArgument("The source packet file name was not "
			"specified.");
	else if (dst_packet_file_name_.empty())
		MLB::Utility::ThrowInvalidArgument("The destination packet file name "
			"was not specified.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketConverterBase::~CannedPacketConverterBase()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class CannedPacketConverter : public CannedPacketConverterBase {
public:
	CannedPacketConverter(int argc, char **argv);

	void Run();

private:
	PacketFileReader packet_file_reader_;
	PacketFileWriter packet_file_writer_;

	void RunInternal();

	CannedPacketConverter(const CannedPacketConverter &other);
	CannedPacketConverter operator = (const CannedPacketConverter &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketConverter::CannedPacketConverter(int argc, char **argv)
	:CannedPacketConverterBase(argc, argv)
	,packet_file_reader_(src_packet_file_name_, src_packet_format_)
	,packet_file_writer_(dst_packet_file_name_, dst_packet_format_)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketConverter::Run()
{
	RunInternal();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketConverter::RunInternal()
{
	unsigned int count_1;
	unsigned int packet_count = 0;

	std::cout << "Src File Name    : " << src_packet_file_name_ << std::endl;
	std::cout << "Dst File Name    : " << dst_packet_file_name_ << std::endl;
	std::cout << "Src Packet Format: " << src_packet_format_ << std::endl;
	std::cout << "Dst Packet Format: " << dst_packet_format_ << std::endl;
	std::cout << "Src File Size    : " << std::setw(22) <<
		packet_file_reader_.GetFileSize() << std::endl;
	std::cout << "Packet Count     : " << std::setw(22) <<
		packet_file_reader_.GetPacketCount() << std::endl;
	std::cout << "First Index      : " << std::setw(22) <<
		first_packet_index_ << std::endl;
	std::cout << "Max Packets      : " << std::setw(22) <<
		max_packet_count_ << std::endl;
	std::cout << "Iterations       : " << std::setw(22) <<
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
		MsgTimeSecsType time_secs;
		MsgTimeNSecsType time_nsecs;
		while (packet_file_reader_.GetPacket(packet_length, packet_ptr,
			ip_address, ip_port, time_secs, time_nsecs)) {
			if (packet_index++ >= first_packet_index_) {
				if (remove_leading_chars_) {
					if (packet_length > remove_leading_chars_)
						packet_file_writer_.WritePacket(
							packet_length - remove_leading_chars_,
							static_cast<const char *>(packet_ptr) +
							remove_leading_chars_, ip_address, ip_port,
							time_secs, time_nsecs);
				}
				else
					packet_file_writer_.WritePacket(packet_length, packet_ptr,
						ip_address, ip_port, time_secs, time_nsecs);
				++packet_count;
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
	}

	std::cerr <<
		MLB::Utility::PadLeft("", 1 + 22 + 2 + 22 + 2 + 22 + 1, '\b') <<
		"[" << std::setw(22) << packet_count << "][" <<
		std::setw(22) << 0 << "][" << std::setw(22) << 0 << "]" << std::endl;
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
		CannedPacketConverter canned_packet_convert(argc, argv);
		canned_packet_convert.Run();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << argv[0] << ": fatal error encountered: " <<
			except.what() << std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

