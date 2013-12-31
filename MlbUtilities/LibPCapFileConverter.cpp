// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the the libpcap (tcpdump) format
								support.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/TimerHandler.hpp>

#include <Utility/Utility_Exception.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/MACAddress.hpp>

#include <fstream>

#if defined(linux)
# include <sys/types.h>
#endif // #if defined(linux)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
typedef signed __int16   LibPCapType_SInt16;
typedef unsigned __int16 LibPCapType_UInt16;
typedef signed __int32   LibPCapType_SInt32;
typedef unsigned __int32 LibPCapType_UInt32;
#elif defined(linux)
typedef int16_t          LibPCapType_SInt16;
typedef u_int16_t        LibPCapType_UInt16;
typedef int32_t          LibPCapType_SInt32;
typedef u_int32_t        LibPCapType_UInt32;
#else
typedef signed short     LibPCapType_SInt16;
typedef unsigned short   LibPCapType_UInt16;
typedef signed int       LibPCapType_SInt32;
typedef unsigned int     LibPCapType_UInt32;
#endif // #ifdef _MSC_VER
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
union LibPCapFileMagicDatum {
	unsigned char      magic_uchar_[4];
	LibPCapType_UInt32 magic_uint_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//const LibPCapFileMagicDatum LibPCapFileMagic    = { 0xd4, 0xc3, 0xb2, 0xa1 };
const LibPCapFileMagicDatum LibPCapFileMagic    = { 0xa1, 0xb2, 0xc3, 0xd4 };
const LibPCapType_UInt16    LibPCapVersionMajor = 2;
const LibPCapType_UInt16    LibPCapVersionMinor = 4;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct LibPCapFileHeader {
	LibPCapFileHeader();
	LibPCapFileHeader(const void *src_ptr);

	bool IsBigEndian() const
	{
		Check();

		return(magic_number_ == LibPCapFileMagic.magic_uint_);
	}

	void Check() const
	{
		if ((magic_number_ != LibPCapFileMagic.magic_uint_) &&
			(magic_number_ != ntohl(LibPCapFileMagic.magic_uint_)))
			MLB::Utility::ThrowLogicError("Invalid libpcap magic number "
				"encountered (0x" + ToStringMagic() + ").");
	}

	bool IsFixUpNeeded() const
	{
		bool data_bend = IsBigEndian();
		bool arch_bend = MLB::Utility::IsBigEndianArchitecture();

		return(data_bend != arch_bend);
	}

	void FixUp()
	{
		if (IsFixUpNeeded()) {
			magic_number_       = ntohl(magic_number_);
			version_major_      = ntohs(version_major_);	
			version_minor_      = ntohs(version_minor_);	
			time_zone_offset_   = ntohl(time_zone_offset_);
			time_zone_accuracy_ = ntohl(time_zone_accuracy_);
			snapshot_length_    = ntohl(snapshot_length_);
			link_layer_type_    = ntohl(link_layer_type_);
		}
	}

	const LibPCapFileMagicDatum *GetMagicUnionPtr() const;

	std::string ToStringMagic() const;

	LibPCapType_UInt32 magic_number_;
	LibPCapType_UInt16 version_major_;	
	LibPCapType_UInt16 version_minor_;	
	LibPCapType_SInt32 time_zone_offset_;
	LibPCapType_SInt32 time_zone_accuracy_;
	LibPCapType_SInt32 snapshot_length_;
	LibPCapType_SInt32 link_layer_type_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct LibPCapFrameHeader {
	LibPCapFrameHeader();
	LibPCapFrameHeader(const void *src_ptr);

	void FixUp(bool is_fixup_needed)
	{
		if (is_fixup_needed) {
			gmt_seconds_    = ntohl(gmt_seconds_);
			gmt_useconds_   = ntohl(gmt_useconds_);
			bytes_captured_ = ntohl(bytes_captured_);
			bytes_actual_   = ntohl(bytes_actual_);
		}
	}

	LibPCapType_UInt32 gmt_seconds_;
	LibPCapType_UInt32 gmt_useconds_;
	LibPCapType_UInt32 bytes_captured_;
	LibPCapType_UInt32 bytes_actual_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB










namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
#define THIS_CAST_TO(dst_type, src_ptr)	\
	reinterpret_cast<const dst_type *>(src_ptr)

#define CAST_TO_FILE(src_ptr)					\
	THIS_CAST_TO(LibPCapFileHeader, src_ptr)

#define CAST_TO_FRAME(src_ptr)					\
	THIS_CAST_TO(LibPCapFrameHeader, src_ptr)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LibPCapFileHeader::LibPCapFileHeader()
	:magic_number_(LibPCapFileMagic.magic_uint_)
	,version_major_(LibPCapVersionMajor)
	,version_minor_(LibPCapVersionMinor)
	,time_zone_offset_(0)
	,time_zone_accuracy_(0)
	,snapshot_length_(0)
	,link_layer_type_(0)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LibPCapFileHeader::LibPCapFileHeader(const void *src_ptr)
	:magic_number_(CAST_TO_FILE(src_ptr)->magic_number_)
	,version_major_(CAST_TO_FILE(src_ptr)->version_major_)
	,version_minor_(CAST_TO_FILE(src_ptr)->version_minor_)
	,time_zone_offset_(CAST_TO_FILE(src_ptr)->time_zone_offset_)
	,time_zone_accuracy_(CAST_TO_FILE(src_ptr)->time_zone_accuracy_)
	,snapshot_length_(CAST_TO_FILE(src_ptr)->snapshot_length_)
	,link_layer_type_(CAST_TO_FILE(src_ptr)->link_layer_type_)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const LibPCapFileMagicDatum *LibPCapFileHeader::GetMagicUnionPtr() const
{
	return(reinterpret_cast<const LibPCapFileMagicDatum *>(&magic_number_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string LibPCapFileHeader::ToStringMagic() const
{
	using namespace MLB::Utility;

	return(
		AnyToString(GetMagicUnionPtr()->magic_uchar_[0]) +
		AnyToString(GetMagicUnionPtr()->magic_uchar_[1]) +
		AnyToString(GetMagicUnionPtr()->magic_uchar_[2]) +
		AnyToString(GetMagicUnionPtr()->magic_uchar_[3]));
}
// ////////////////////////////////////////////////////////////////////////////

//	****************************************************************************
//	****************************************************************************
//	****************************************************************************

// ////////////////////////////////////////////////////////////////////////////
LibPCapFrameHeader::LibPCapFrameHeader()
	:gmt_seconds_(0)
	,gmt_useconds_(0)
	,bytes_captured_(0)
	,bytes_actual_(0)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LibPCapFrameHeader::LibPCapFrameHeader(const void *src_ptr)
	:gmt_seconds_(CAST_TO_FRAME(src_ptr)->gmt_seconds_)
	,gmt_useconds_(CAST_TO_FRAME(src_ptr)->gmt_useconds_)
	,bytes_captured_(CAST_TO_FRAME(src_ptr)->bytes_captured_)
	,bytes_actual_(CAST_TO_FRAME(src_ptr)->bytes_actual_)
{
}
// ////////////////////////////////////////////////////////////////////////////

//	****************************************************************************
//	****************************************************************************
//	****************************************************************************

// ////////////////////////////////////////////////////////////////////////////
struct LibPCapMacHeader {
	unsigned char      dst_octet[MLB::Utility::MACAddress_Length];
	unsigned char      src_octet[MLB::Utility::MACAddress_Length];
	LibPCapType_UInt16 packet_type_;

	bool IsTypeIpVersion4() const
	{
		return((reinterpret_cast<const char *>(&packet_type_)[0] == 0x80) ||
				 (reinterpret_cast<const char *>(&packet_type_)[1] == 0x00));
	}
};
// ////////////////////////////////////////////////////////////////////////////

//	****************************************************************************
//	****************************************************************************
//	****************************************************************************

// ////////////////////////////////////////////////////////////////////////////
struct LibPCapIpHeader {
	unsigned char  ip_header_len_:4;  // 4-bit header length (in 32-bit words) normally=5 (Means 20 Bytes may be 24 also)  
	unsigned char  ip_version_   :4;  // 4-bit IPv4 version  
	unsigned char  ip_tos_;           // IP type of service  
	unsigned short ip_total_length_;  // Total length  
	unsigned short ip_id_;            // Unique identifier   
	unsigned char  ip_frag_offset_   :5;        // Fragment offset field  
	unsigned char  ip_more_fragment_ :1;  
	unsigned char  ip_dont_fragment_ :1;  
	unsigned char  ip_reserved_zero_ :1;  
	unsigned char  ip_frag_offset1_;    //fragment offset  
	unsigned char  ip_ttl_;           // Time to live  
	unsigned char  ip_protocol_;      // Protocol(TCP,UDP etc)  
	unsigned short ip_checksum_;      // IP checksum  
	unsigned int   ip_srcaddr_;       // Source address  
	unsigned int   ip_destaddr_;      // Source address  
};
// ////////////////////////////////////////////////////////////////////////////

//	****************************************************************************
//	****************************************************************************
//	****************************************************************************

// ////////////////////////////////////////////////////////////////////////////
struct LibPCapUdpHeader {
	LibPCapType_UInt16 src_port_;
	LibPCapType_UInt16 dst_port_;
	LibPCapType_UInt16 length_;
	LibPCapType_UInt16 checksum_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct LibPCapFrame {
	LibPCapFrameHeader frame_header_;
	LibPCapMacHeader   mac_header_;
	LibPCapIpHeader    ip_header_;
	LibPCapUdpHeader   udp_header_;
	LibPCapType_UInt16 packet_length_;
	char               packet_data_[0xFFFF];
};
// ////////////////////////////////////////////////////////////////////////////

//	****************************************************************************
//	****************************************************************************
//	****************************************************************************

// ////////////////////////////////////////////////////////////////////////////
class LibPCapFileConverter {
public:
	LibPCapFileConverter(const std::string &file_name);

	std::string         GetFileName() const;
	unsigned long long  GetFileSize() const;
	unsigned long long  GetFileOffset() const;

	bool ReadFrame(LibPCapFrame &frame_buffer);

private:
	void ReadFileHeader(LibPCapFileHeader &out_datum);
	void ReadFrameHeader(LibPCapFrameHeader &out_datum);
	void ReadMacHeader(LibPCapMacHeader &out_datum);
	void ReadIpHeader(LibPCapIpHeader &out_datum);
	void ReadUdpHeader(LibPCapUdpHeader &out_datum);
	void ReadFromFile(const char *read_type, std::size_t read_length,
		void *read_data);
	void SeekToOffsetFile(unsigned long long file_offset,
		std::ios_base::seekdir offset_base = std::ios_base::beg);

	std::string          file_name_;
	std::ifstream        msg_file_;
	unsigned long long   file_size_;
	unsigned long long   file_offset_;
	LibPCapFileHeader file_header_;
	bool                 is_fixup_needed_;

	class FileOffsetSaver {
	public:
		FileOffsetSaver(LibPCapFileConverter &reader_ref)
			:reader_ref_(reader_ref)
			,file_offset_value_(reader_ref.GetFileOffset())
			,should_restore_flag_(true)
		{
		}
		~FileOffsetSaver()
		{
			Restore();
		}

		unsigned long long Restore()
		{
			if (should_restore_flag_) {
				try {
					reader_ref_.SeekToOffsetFile(file_offset_value_);
				}
				catch (const std::exception &) {
				}
				reader_ref_.file_offset_ = file_offset_value_;
				should_restore_flag_     = false;
			}

			return(file_offset_value_);
		}
		unsigned long long Reset()
		{
			should_restore_flag_ = false;

			return(file_offset_value_);
		}

	private:
		LibPCapFileConverter   &reader_ref_;
		unsigned long long  file_offset_value_;
		bool                should_restore_flag_;

		FileOffsetSaver(const FileOffsetSaver &other);
		FileOffsetSaver & operator = (const FileOffsetSaver &other);
	};

	LibPCapFileConverter(const LibPCapFileConverter &other);
	LibPCapFileConverter & operator = (const LibPCapFileConverter &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB


namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
LibPCapFileConverter::LibPCapFileConverter(const std::string &file_name)
	:file_name_(file_name)
	,msg_file_(file_name.c_str(), std::ios_base::binary)
	,file_size_(0)
	,file_offset_(0)
	,file_header_()
	,is_fixup_needed_(false)
{
	try {
		if (msg_file_.fail())
			MLB::Utility::ThrowSystemError("Open failed.");
		SeekToOffsetFile(0, std::ios_base::end);
		file_size_ = static_cast<unsigned long long>(msg_file_.tellg());
		if (msg_file_.fail())
			MLB::Utility::ThrowLogicError("Attempt to determine the end of file "
				"position failed.");
		SeekToOffsetFile(0, std::ios_base::beg);
		if (file_size_ < 1)
			MLB::Utility::ThrowLogicError("Does not appear to contain packet "
				"data.");
		ReadFileHeader(file_header_);
		file_header_.Check();
		is_fixup_needed_ = file_header_.IsFixUpNeeded();
		file_header_.FixUp();
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to open packet file '" +
			file_name_ + "' for reading: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string LibPCapFileConverter::GetFileName() const
{
	return(file_name_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long LibPCapFileConverter::GetFileSize() const
{
	return(file_size_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long LibPCapFileConverter::GetFileOffset() const
{
	return(file_offset_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LibPCapFileConverter::ReadFrame(LibPCapFrame &frame_buffer)
{
	if (file_offset_ >= file_size_)
		return(false);

	ReadFrameHeader(frame_buffer.frame_header_);
	ReadMacHeader(frame_buffer.mac_header_);

	if (!frame_buffer.mac_header_.IsTypeIpVersion4())
		return(false);

	ReadIpHeader(frame_buffer.ip_header_);

	if ((frame_buffer.ip_header_.ip_header_len_ != 5) ||
		(frame_buffer.ip_header_.ip_version_ != 4) ||
		(frame_buffer.ip_header_.ip_protocol_ != 0x11))
		return(false);

	LibPCapType_UInt16 total_length =
		ntohs(frame_buffer.ip_header_.ip_total_length_);

	if (total_length <=
		(sizeof(frame_buffer.ip_header_) + sizeof(frame_buffer.udp_header_)))
return(false);

	ReadUdpHeader(frame_buffer.udp_header_);

	frame_buffer.packet_length_ = total_length -
		(sizeof(frame_buffer.ip_header_) + sizeof(frame_buffer.udp_header_));

	ReadFromFile("libpcap frame packet data", frame_buffer.packet_length_,
		reinterpret_cast<void *>(frame_buffer.packet_data_));
	
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LibPCapFileConverter::ReadFileHeader(LibPCapFileHeader &out_datum)
{
	ReadFromFile("libpcap file header", sizeof(out_datum),
		reinterpret_cast<void *>(&out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LibPCapFileConverter::ReadFrameHeader(LibPCapFrameHeader &out_datum)
{
	ReadFromFile("libpcap frame header", sizeof(out_datum),
		reinterpret_cast<void *>(&out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LibPCapFileConverter::ReadMacHeader(LibPCapMacHeader &out_datum)
{
	ReadFromFile("libpcap MAC header", sizeof(out_datum),
		reinterpret_cast<void *>(&out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LibPCapFileConverter::ReadIpHeader(LibPCapIpHeader &out_datum)
{
	ReadFromFile("libpcap IP header", sizeof(out_datum),
		reinterpret_cast<void *>(&out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LibPCapFileConverter::ReadUdpHeader(LibPCapUdpHeader &out_datum)
{
	ReadFromFile("libpcap UDP header", sizeof(out_datum),
		reinterpret_cast<void *>(&out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LibPCapFileConverter::ReadFromFile(const char *read_type,
	std::size_t read_length, void *read_data)
{
	try {
		if (read_length > 0xFFFF)
			MLB::Utility::ThrowInvalidArgument("The specified read length (" +
				MLB::Utility::AnyToString(read_length) + " exceeds the maximum "
				"permissible (" + MLB::Utility::AnyToString(0xFFFF) + ").");
		else if (!read_length)
			MLB::Utility::ThrowInvalidArgument("The specified read length is "
				"zero.");
		file_offset_ = static_cast<unsigned long long>(msg_file_.tellg());
		if ((file_offset_ + read_length) > file_size_)
			MLB::Utility::ThrowInvalidArgument("The current file offset (" +
				MLB::Utility::AnyToString(file_offset_) + ") plus the specified "
				"read length (" + MLB::Utility::AnyToString(read_length) +
				") exceeds the file size (" +
				MLB::Utility::AnyToString(file_size_) + ").");
		msg_file_.read(reinterpret_cast<char *>(read_data),
			static_cast<std::streamsize>(read_length));
		file_offset_ = static_cast<unsigned long long>(msg_file_.tellg());
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to read " +
			std::string(read_type) + " data from the libpcap packet file "
			"failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LibPCapFileConverter::SeekToOffsetFile(unsigned long long file_offset,
	std::ios_base::seekdir offset_base)
{
	msg_file_.seekg(static_cast<std::istream::off_type>(file_offset),
		offset_base);

	if (msg_file_.fail()) {
		std::ostringstream o_str;
		o_str << "Attempt to seek to offset " << file_offset << " relative to " <<
			((offset_base == std::ios_base::beg) ? "the beginning" :
			((offset_base == std::ios_base::end) ? "the end" :
			"the current position")) << " of the file failed.";
		MLB::Utility::ThrowLogicError(o_str);
	}

	file_offset_ = file_offset;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#include <SocketIo/PacketFileWriter.hpp>

#include <Utility/PathName.hpp>
#include <Utility/TimeSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
			std::cout << "USAGE: " << std::endl <<
				"   " << argv[0] << " " <<
				"<libpcap-file-name> [ <libpcap-file-name> ... ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   <libpcap-file-name>\n\
      Is the name of libpcap file (perhaps captured by tcpdump) which this\n\
      program is to process." <<
				std::endl << std::endl;
			exit(EXIT_SUCCESS);
		}
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			std::cout << argv[count_1] << std::endl;
			std::string           base_name(
				MLB::Utility::GetFileNamePortion(argv[count_1]));
			LibPCapFileConverter  input_file(argv[count_1]);
			LibPCapFrame          frame_buffer;
			PacketFileWriter      output_file(base_name + ".NEW.pkt",
				PacketFormat_Time);
			while (input_file.ReadFrame(frame_buffer)) {
				output_file.WritePacket(frame_buffer.packet_length_,
					frame_buffer.packet_data_, 0, 0,
					frame_buffer.frame_header_.gmt_seconds_,
					frame_buffer.frame_header_.gmt_useconds_);
/*
std::cout << MLB::Utility::TimeSpec(frame_buffer.frame_header_.gmt_seconds_,
	frame_buffer.frame_header_.gmt_useconds_) << std::endl;
*/
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////
