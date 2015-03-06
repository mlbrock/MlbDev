// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts packet files to the PCap format.

	Revision History	:	2009-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2009 - 2015.
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
#include <SocketIo/CalcIPChecksum.hpp>

#include <Utility/InlineContainer.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>

#include <pcap.h>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
#pragma pack(1)
struct MyPacketHeader {
	//	Ethernet header
	Native_UInt8  ether_dst_addr_[6];
	Native_UInt8  ether_src_addr_[6];
	Native_UInt16 ether_type_;

	//	IP header.
	Native_UInt8  ip_version_ihl_;
	Native_UInt8  ip_service_;
	Native_UInt16 ip_total_length_;
	Native_UInt16 ip_id_;
	Native_UInt16 ip_fragment_;
	Native_UInt8  ip_ttl_;
	Native_UInt8  ip_protocol_;
	Native_UInt16 ip_checksum_;
	Native_UInt32 ip_src_addr_;
	Native_UInt32 ip_dst_addr_;

	//	UDP header.
	Native_UInt16 udp_src_port_;
	Native_UInt16 udp_dst_port_;
	Native_UInt16 udp_length_;		//	Length of UDP header + data.
	Native_UInt16 udp_checksum_;

	Native_UInt16 CalcIpChecksum()
	{
		return(MLB::SocketIo::CalcIPChecksum(
			1 + 1 + 2 + 2 + 2 + 1 + 1 + 2 + 4 + 4,
			reinterpret_cast<const Native_UInt16 *>(&ip_version_ihl_)));
	}

	void SetIpChecksum()
	{
		ip_checksum_ = 0;
		ip_checksum_ = CalcIpChecksum();
	}
};
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class CannedPacketConverterBase {
public:
	CannedPacketConverterBase(int argc, char **argv);

	virtual ~CannedPacketConverterBase();

	void Run();

private:
	std::string           src_packet_file_name_;
	std::string           dst_packet_file_name_;
	PacketFormat          src_packet_format_;
	bool                  pcap_time_adjust_;
	unsigned int          remove_leading_chars_;
	unsigned int          first_packet_index_;
	unsigned int          max_packet_count_;
	unsigned int          iteration_count_;
	PacketFileReaderSPtr  packet_file_reader_sptr_;
	pcap_t               *pcap_ptr_;
	pcap_dumper_t        *pcap_dump_ptr_;

	void RunInternal();

	void WritePacket(std::size_t packet_length, const void *packet_ptr,
		IpAddressType ip_address, IpPortType ip_port,
		MsgTimeSecsType time_secs, MsgTimeNSecsType time_nsecs);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketConverterBase::CannedPacketConverterBase(int argc, char **argv)
	:src_packet_file_name_()
	,dst_packet_file_name_()
	,src_packet_format_(PacketFormat_Raw)
	,pcap_time_adjust_(true)
	,remove_leading_chars_(0)
	,first_packet_index_(0)
	,max_packet_count_(0)
	,iteration_count_(1)
	,packet_file_reader_sptr_()
	,pcap_ptr_(NULL)
	,pcap_dump_ptr_(NULL)
{
	if (SocketIoParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"-source_packet_file <src-packet-file> " <<
			"-destination_packet_file <dst-packet-file> " <<
			"[ -source_packet_format <packet-format> ] " <<
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
      If not specified, this parameter defaults to 'RAW'.\n\n"
<<
"\
   -pcap_timestamp_adjustment <boolean>\n\
      This parameter specifies, if 'true', that the timestamps are to be\n\
      converted to th pcap file format 'struct timeval' timestamps.\n\n\
      If 'false', the source 'struct timespec' timestamps will be coerced\n\
      into the pcap format 'struct timeval' area.\n\n\
      This is possible because the two structures are the same size. However,\n\
      code which reads the resulting pcap file may become unhappy with the\n\
      magnitudes stored in what is expected to be a microsecond quantity\n\
      (and which, therefore, should never exceed 1,000,000).\n\n\
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
		else if (SocketIoParseCmdLineArg::ParseCmdLineDatum(
			MLB::Utility::RegexParamNameAdaptor("^\\-\\-*PCAP(_|\\-)*"
			"(FILE(_|\\-)*)*TIME(STAMP)*(_|\\-)*ADJ(UST(MENT)*)*$",
			count_1, argc, argv), count_1, argc, argv, pcap_time_adjust_))
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

	packet_file_reader_sptr_.reset(new 
		PacketFileReader(src_packet_file_name_, src_packet_format_));

	pcap_ptr_ = ::pcap_open_dead(DLT_EN10MB, 65535 /* snaplen */);

	pcap_dump_ptr_ = ::pcap_dump_open(pcap_ptr_,
		dst_packet_file_name_.c_str());

	if (pcap_dump_ptr_ == NULL) {
		std::ostringstream o_str;
		o_str << "Invocation of ::pcap_dump_open() failed: " <<
			::pcap_geterr(pcap_ptr_);
		MLB::Utility::ThrowLogicError(o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CannedPacketConverterBase::~CannedPacketConverterBase()
{
	if (pcap_ptr_ != NULL)
		::pcap_close(pcap_ptr_);

	if (pcap_dump_ptr_ != NULL)
		::pcap_dump_close(pcap_dump_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketConverterBase::Run()
{
	RunInternal();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketConverterBase::RunInternal()
{
	unsigned int count_1;
	unsigned int packet_count = 0;

	std::cout << "Src File Name    : " << src_packet_file_name_ << std::endl;
	std::cout << "Dst File Name    : " << dst_packet_file_name_ << std::endl;
	std::cout << "Src Packet Format: " << src_packet_format_ << std::endl;
	std::cout << "Dst Packet Format: PCap" << std::endl;
	std::cout << "Timestamp Adjust : " <<
		MLB::Utility::AnyToString(pcap_time_adjust_) << std::endl;
	std::cout << "Src File Size    : " << std::setw(22) <<
		packet_file_reader_sptr_->GetFileSize() << std::endl;
	std::cout << "Packet Count     : " << std::setw(22) <<
		packet_file_reader_sptr_->GetPacketCount() << std::endl;
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
		while (packet_file_reader_sptr_->GetPacket(packet_length, packet_ptr,
			ip_address, ip_port, time_secs, time_nsecs)) {
			if (packet_index++ >= first_packet_index_) {
				if (remove_leading_chars_) {
					if (packet_length > remove_leading_chars_)
						WritePacket(packet_length - remove_leading_chars_,
							static_cast<const char *>(packet_ptr) +
							remove_leading_chars_, ip_address, ip_port,
							time_secs, time_nsecs);
				}
				else
					WritePacket(packet_length, packet_ptr, ip_address, ip_port,
						time_secs, time_nsecs);
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

// ////////////////////////////////////////////////////////////////////////////
void CannedPacketConverterBase::WritePacket(std::size_t packet_length,
	const void *packet_ptr, IpAddressType ip_address, IpPortType ip_port,
	MsgTimeSecsType time_secs, MsgTimeNSecsType time_nsecs)
{
	unsigned char packet_data[42 + 65536];

	MyPacketHeader *my_header_ptr =
		reinterpret_cast<MyPacketHeader *>(packet_data);

	Native_UInt16   ip_length  = static_cast<Native_UInt16>(42 + packet_length);
	Native_UInt16   udp_length = static_cast<Native_UInt16>(8 + packet_length);
	Native_UInt32   tmp_addr   = htonl(ip_address);
	Native_UInt16   tmp_port   = htons(ip_port);

	my_header_ptr->ether_dst_addr_[0] = 0x00;
	my_header_ptr->ether_dst_addr_[1] = 0x11;
	my_header_ptr->ether_dst_addr_[2] = 0x22;
	my_header_ptr->ether_dst_addr_[3] = 0x33;
	my_header_ptr->ether_dst_addr_[4] = 0x44;
	my_header_ptr->ether_dst_addr_[5] = 0x55;
	my_header_ptr->ether_src_addr_[0] = 0x66;
	my_header_ptr->ether_src_addr_[1] = 0x77;
	my_header_ptr->ether_src_addr_[2] = 0x88;
	my_header_ptr->ether_src_addr_[3] = 0x99;
	my_header_ptr->ether_src_addr_[4] = 0xAA;
	my_header_ptr->ether_src_addr_[5] = 0xBB;
	my_header_ptr->ether_type_        = htons(0x0800);

	my_header_ptr->ip_version_ihl_  = 0x45;
	my_header_ptr->ip_service_      = 0;
	my_header_ptr->ip_total_length_ = htons(ip_length);
	my_header_ptr->ip_id_           = 0;
	my_header_ptr->ip_fragment_     = 0;
	my_header_ptr->ip_ttl_          = 2;
	my_header_ptr->ip_protocol_     = 17;
	my_header_ptr->ip_checksum_     = 0;
	my_header_ptr->ip_src_addr_     = tmp_addr;
	my_header_ptr->ip_dst_addr_     = tmp_addr;

	//	UDP header.
	my_header_ptr->udp_src_port_    = tmp_port;
	my_header_ptr->udp_dst_port_    = tmp_port;
	my_header_ptr->udp_length_      = htons(udp_length); //	Length of UDP header + data.
	my_header_ptr->udp_checksum_    = 0;

	my_header_ptr->SetIpChecksum();

	pcap_pkthdr header;

	header.ts.tv_sec  = time_secs;
	header.ts.tv_usec = (pcap_time_adjust_) ? (time_nsecs / 1000) : time_nsecs;
	header.caplen     = 42 + packet_length;
	header.len        = 42 + packet_length;

	::memcpy(packet_data + 42, static_cast<const char *>(packet_ptr),
		packet_length);

	::pcap_dump(reinterpret_cast<u_char *>(pcap_dump_ptr_), &header,
		packet_data);
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
		CannedPacketConverterBase canned_packet_convert(argc, argv);
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

