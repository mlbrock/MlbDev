// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketFileReaderTmp class.

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

#include <SocketIo/PacketFileReaderTmp.hpp>

#include <Utility/PathName.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __GNUC__
# include <memory.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketFileReaderTmp::PacketFileReaderTmp(const std::string &file_name,
	PacketFormat packet_format)
	:file_name_(file_name)
	,packet_format_(CheckPacketFormat(packet_format))
	,msg_file_(file_name.c_str(), std::ios_base::binary)
	,file_size_(0)
	,file_offset_(0)
	,packet_offset_(0)
	,packet_count_(0)
	,packet_count_is_known_(false)
	,file_is_loaded_(false)
	,file_data_sptr_()
	,packet_buffer_sptr_(new unsigned char[65536])
{
	try {
		if (packet_format_ == PacketFormat_Raw)
			MLB::Utility::ThrowInvalidArgument("Unable to support raw packet "
				"files.");
		if (msg_file_.fail())
			MLB::Utility::ThrowSystemError("Open failed.");
		file_size_ = MLB::Utility::GetFileSize(file_name_);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to open packet file '" +
			file_name_ + "' for reading: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string PacketFileReaderTmp::GetFileName() const
{
	return(file_name_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileReaderTmp::GetFileSize() const
{
	return(file_size_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileReaderTmp::GetFileOffset() const
{
	return(file_offset_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileReaderTmp::GetPacketOffset() const
{
	return(packet_offset_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::GetPacket(std::size_t &packet_length,
	const void *&packet_ptr)
{
	IpAddressType ip_address;
	IpPortType    ip_port;

	return(GetPacket(packet_length, packet_ptr, ip_address, ip_port));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::GetPacket(std::size_t &packet_length,
	const void *&packet_ptr, IpAddressType &ip_address, IpPortType &ip_port)
{
	MsgTimeSecsType  tmp_time_secs;
	MsgTimeNSecsType tmp_time_nsecs;

	return(GetPacket(packet_length, packet_ptr, ip_address, ip_port,
		tmp_time_secs, tmp_time_nsecs));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::GetPacket(std::size_t &packet_length,
	const void *&packet_ptr, IpAddressType &ip_address, IpPortType &ip_port,
	MLB::Utility::TimeSpec &packet_timestamp)
{
	MsgTimeSecsType  tmp_time_secs;
	MsgTimeNSecsType tmp_time_nsecs;

	if (!GetPacket(packet_length, packet_ptr, ip_address, ip_port,
		tmp_time_secs, tmp_time_nsecs))
		return(false);

	packet_timestamp.tv_sec  = tmp_time_secs;
	packet_timestamp.tv_nsec = static_cast<long>(tmp_time_nsecs);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::GetPacket(std::size_t &packet_length,
	const void *&packet_ptr, IpAddressType &ip_address, IpPortType &ip_port,
	MsgTimeSecsType &time_secs, MsgTimeNSecsType &time_nsecs)
{
	if (file_offset_ >= file_size_)
		return(false);

/*
	CODE NOTE: Raw is not supported.
	if (packet_format_ == PacketFormat_Raw) {
		packet_length  = static_cast<std::size_t>(file_size_);
		packet_ptr     = file_data_sptr_.get();
		file_offset_   = file_size_;
		packet_offset_ = file_offset_;
		return(true);
	}
*/

	if ((file_size_ - file_offset_) < sizeof(MsgLengthType)) {
		std::ostringstream o_str;
		o_str << "Insufficient bytes remaining in the file to provide a "
			"packet length consisting of an 'unsigned short' (" <<
			(file_size_ - file_offset_) << ") --- file size is " <<
			file_size_ << " and file offset is " << file_offset_ << ".";
		MLB::Utility::ThrowLogicError(o_str);
	}

	unsigned long long     tmp_packet_offset = file_offset_;
	MsgLengthType          tmp_length        = 0;
	IpAddressType          tmp_ip_address    = 0;
	IpPortType             tmp_ip_port       = 0;
	MsgTimeSecsType        tmp_time_secs     = 0;
	MsgTimeNSecsType       tmp_time_nsecs    = 0;

	FileOffsetSaver saved_offset(*this);

	if (packet_format_ == PacketFormat_IpSourceTLen)
		ReadIpSourceHeader(tmp_ip_address, tmp_ip_port);

	msg_file_.read(reinterpret_cast<char *>(&tmp_length), sizeof(tmp_length));
	file_offset_ += sizeof(tmp_length);

	tmp_length = ntohs(tmp_length);

	if (!tmp_length)
		MLB::Utility::ThrowLogicError("The data length of a packet as read from "
			"the file is '0'.");

	if ((packet_format_ == PacketFormat_Time) ||
		 (packet_format_ == PacketFormat_TimeIpSource))
		ReadTimestampHeader(tmp_time_secs, tmp_time_nsecs);

	if ((packet_format_ == PacketFormat_IpSource) ||
		 (packet_format_ == PacketFormat_TimeIpSource))
		ReadIpSourceHeader(tmp_ip_address, tmp_ip_port);

	if ((file_size_ - file_offset_) < tmp_length) {
		std::ostringstream o_str;
		o_str << "Insufficient bytes remaining in the file to provide data for "
			"the indicated packet length of " << tmp_length << " bytes (" <<
			(file_size_ - file_offset_) << ") --- file size is " << file_size_ <<
			"and file offset is " << file_offset_ << ".";
		MLB::Utility::ThrowLogicError(o_str);
	}

	msg_file_.read(reinterpret_cast<char *>(packet_buffer_sptr_.get()),
		static_cast<std::streamsize>(tmp_length));
	file_offset_ += tmp_length;

	packet_length   = tmp_length;
	packet_ptr      = packet_buffer_sptr_.get();
	ip_address      = tmp_ip_address;
	ip_port         = tmp_ip_port;
	time_secs       = tmp_time_secs;
	time_nsecs      = tmp_time_nsecs;
	packet_offset_  = tmp_packet_offset;

	saved_offset.Reset();

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::GetPacket(const void *&begin_ptr, const void *&end_ptr)
{
	IpAddressType ip_address;
	IpPortType    ip_port;

	return(GetPacket(begin_ptr, end_ptr, ip_address, ip_port));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::GetPacket(const void *&begin_ptr, const void *&end_ptr,
	IpAddressType &ip_address, IpPortType &ip_port)
{
	MLB::Utility::TimeSpec packet_timestamp(0, 0);

	return(GetPacket(begin_ptr, end_ptr, ip_address, ip_port, packet_timestamp));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::GetPacket(const void *&begin_ptr, const void *&end_ptr,
	IpAddressType &ip_address, IpPortType &ip_port,
	MLB::Utility::TimeSpec &packet_timestamp)
{

	MsgTimeSecsType  tmp_time_secs;
	MsgTimeNSecsType tmp_time_nsecs;

	if (!GetPacket(begin_ptr, end_ptr, ip_address, ip_port, tmp_time_secs,
		tmp_time_nsecs))
		return(false);

	packet_timestamp.tv_sec  = tmp_time_secs;
	packet_timestamp.tv_nsec = static_cast<long>(tmp_time_nsecs);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::GetPacket(const void *&begin_ptr, const void *&end_ptr,
	IpAddressType &ip_address, IpPortType &ip_port, MsgTimeSecsType &time_secs,
	MsgTimeNSecsType &time_nsecs)
{
	std::size_t packet_length;

	if (!GetPacket(packet_length, begin_ptr, ip_address, ip_port, time_secs,
		time_nsecs))
		return(false);

	end_ptr = static_cast<const char *>(begin_ptr) + packet_length;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int PacketFileReaderTmp::GetPacketCount()
{
	if (packet_count_is_known_)
		return(packet_count_);

	FileOffsetSaver    saved_offset(*this);
	unsigned long long packet_offset = packet_offset_;
	unsigned int       packet_count;

	SeekToPacket(std::numeric_limits<unsigned int>::max(), packet_count);

	packet_offset_         = packet_offset;
	packet_count_          = packet_count;
	packet_count_is_known_ = true;

	return(packet_count);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::SeekToPacket(unsigned int packet_index)
{
	unsigned int packet_count;

	return(SeekToPacket(packet_index, packet_count));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::SeekToPacket(unsigned int packet_index,
	unsigned int &packet_count)
{
	packet_count = 0;

	if (!packet_index) {
		SeekToOffset(0);
		return(true);
	}
	else if (packet_count_is_known_ && (packet_count_ <= packet_index))
		return(false);

	FileOffsetSaver saved_offset(*this);

	SeekToOffset(0);

	unsigned long long file_offset = file_offset_;

	while (packet_count < packet_index) {
		const void         *begin_ptr;
		const void         *end_ptr;
		if (!GetPacket(begin_ptr, end_ptr))
			return(false);
		file_offset = file_offset_;
		++packet_count;
	}

	SeekToOffset(file_offset);

	packet_offset_ = file_offset;

	saved_offset.Reset();

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileReaderTmp::SeekToOffset(unsigned long long file_offset,
	std::ios_base::seekdir offset_base)
{
	if (!file_is_loaded_)
		SeekToOffsetFile(file_offset, offset_base);
	else if (offset_base != std::ios_base::beg)
		MLB::Utility::ThrowLogicError("Only seeks from the beginning are "
			"permitted for files loaded into memory.");
	//	You can seek to just beyond the end-of-file, but no further...
	else if (file_offset > file_size_) {
		std::ostringstream o_str;
		o_str << "The specified file offset (" << file_offset << ") is not less "
			"than or equal to the file size (" << file_size_ << ").";
		MLB::Utility::ThrowLogicError(o_str);
	}

	file_offset_ = file_offset;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileReaderTmp::LoadFile()
{
	if (file_is_loaded_)
		return(false);

	if ((static_cast<std::size_t>(file_size_) >
		std::numeric_limits<std::size_t>::max()) ||
		 (static_cast<std::streamsize>(file_size_) >
		std::numeric_limits<std::streamsize>::max())) {
		std::ostringstream o_str;
		o_str << "File '" << file_name_ << "' cannot be loaded into memory as "
			"its size (" << file_size_ << " bytes) exceeds the maximum "
			"permissible (" << std::min<std::size_t>(
			std::numeric_limits<std::size_t>::max(),
			static_cast<std::size_t>(
			std::numeric_limits<std::streamsize>::max())) << " bytes).";
		MLB::Utility::ThrowLogicError(o_str);
	}


/*
	CODE NOTE: Original code; temporarily replaced.	
	boost::shared_array<unsigned char> tmp_data(
		new unsigned char[static_cast<std::size_t>(file_size_)]);
*/

	unsigned long long                 tmp_file_size = file_size_;
	boost::shared_array<unsigned char> tmp_data;
	for ( ; ; ) {
		try {
			boost::shared_array<unsigned char> try_data(
				new unsigned char[static_cast<std::size_t>(tmp_file_size)]);
			tmp_data.swap(try_data);
			break;
		}
		catch (const std::bad_alloc &) {
			if (tmp_file_size >= 200000000)
				tmp_file_size -=100000000;
			else
				throw;
		}
	}
	file_size_ = tmp_file_size;

	FileOffsetSaver saved_offset(*this);

	SeekToOffsetFile(0, std::ios_base::beg);

	msg_file_.read(reinterpret_cast<char *>(tmp_data.get()),
		static_cast<std::streamsize>(file_size_));

	file_data_sptr_.swap(tmp_data);

	file_is_loaded_ = true;

	saved_offset.Reset();

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const unsigned char *PacketFileReaderTmp::GetBeginPtr() const
{
	return(file_data_sptr_.get());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const unsigned char *PacketFileReaderTmp::GetEndPtr() const
{
	return(GetBeginPtr() + file_size_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileReaderTmp::ReadTimestampHeader(MsgTimeSecsType &time_secs,
	MsgTimeNSecsType &time_nsecs)
{
	if ((file_size_ - file_offset_) < MsgTimeStampSize) {
		std::ostringstream o_str;
		o_str << "Insufficient bytes remaining in the file to provide a "
			"packet header consisting of an 32-bit 'long' 'time_t' equivalent "
			"and a 32-bit 'long' nanoseconds value (" <<
			(file_size_ - file_offset_) << ") --- file size is " <<
			file_size_ << "and file offset is " << file_offset_ << ".";
		MLB::Utility::ThrowLogicError(o_str);
	}

/*
	CODE NOTE: To be removed.
	MsgTimeSecsType tmp_time_secs;

	::memcpy(&tmp_time_secs, file_data_sptr_.get() + file_offset_,
		sizeof(tmp_time_secs));
	file_offset_ += sizeof(tmp_time_secs);
	time_secs     = ntohl(tmp_time_secs);

	MsgTimeNSecsType tmp_time_nsecs;

	::memcpy(&tmp_time_nsecs, file_data_sptr_.get() + file_offset_,
		sizeof(tmp_time_nsecs));
	file_offset_ += sizeof(tmp_time_nsecs);
	time_nsecs    = ntohl(tmp_time_nsecs);
*/
	double this_buffer[(MsgTimeStampSize / sizeof(double)) + 1];

	msg_file_.read(reinterpret_cast<char *>(this_buffer),
		static_cast<std::streamsize>(MsgTimeStampSize));
	file_offset_ += MsgTimeStampSize;

	MsgTimeSecsType tmp_time_secs;

	::memcpy(&tmp_time_secs, this_buffer, sizeof(tmp_time_secs));
	time_secs = ntohl(tmp_time_secs);

	MsgTimeNSecsType tmp_time_nsecs;

	::memcpy(&tmp_time_nsecs, reinterpret_cast<char *>(this_buffer) +
		sizeof(tmp_time_secs), sizeof(tmp_time_nsecs));
	time_nsecs = ntohl(tmp_time_nsecs);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileReaderTmp::ReadIpSourceHeader(IpAddressType &ip_address,
	IpPortType &ip_port)
{
	if ((file_size_ - file_offset_) < Ip4AddPortSize) {
		std::ostringstream o_str;
		o_str << "Insufficient bytes remaining in the file to provide a "
			"packet header consisting of an 32-bit 'unsigned long' IP "
			"address and a 16-bit 'unsigned short' IP port (" <<
			(file_size_ - file_offset_) << ") --- file size is " <<
			file_size_ << "and file offset is " << file_offset_ << ".";
		MLB::Utility::ThrowLogicError(o_str);
	}

/*
	CODE NOTE: To be removed.
	IpAddressType tmp_ip_address;

	::memcpy(&tmp_ip_address, file_data_sptr_.get() + file_offset_,
		sizeof(tmp_ip_address));
	file_offset_ += sizeof(ip_address);
	ip_address    = ntohl(tmp_ip_address);

	IpPortType tmp_ip_port;

	::memcpy(&tmp_ip_port, file_data_sptr_.get() + file_offset_,
		sizeof(tmp_ip_port));
	file_offset_ += sizeof(tmp_ip_port);
	ip_port       = ntohs(tmp_ip_port);
*/
	double this_buffer[(Ip4AddPortSize / sizeof(double)) + 1];

 	msg_file_.read(reinterpret_cast<char *>(this_buffer),
		static_cast<std::streamsize>(Ip4AddPortSize));
	file_offset_ += Ip4AddPortSize;

  IpAddressType tmp_ip_address;

	::memcpy(&tmp_ip_address, this_buffer, sizeof(tmp_ip_address));
	ip_address = ntohl(tmp_ip_address);

	IpPortType tmp_ip_port;

	::memcpy(&tmp_ip_port,
		reinterpret_cast<char *>(this_buffer) + sizeof(tmp_ip_address),
		sizeof(tmp_ip_port));
	ip_port = ntohs(tmp_ip_port);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileReaderTmp::SeekToOffsetFile(unsigned long long file_offset,
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

#ifdef TEST_MAIN

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		PacketFormat packet_format = PacketFormat_Raw;
		std::cout << "Format: " << PacketFormatToString(packet_format) <<
			std::endl;
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			PacketFormat tmp_packet_format;
			if (IsPacketFormatString(argv[count_1], tmp_packet_format)) {
				packet_format = tmp_packet_format;
				std::cout << "Format: " <<
					PacketFormatToString(packet_format) << std::endl;
			}
			else {
				try {
					std::cout << "File  : " << argv[count_1] << " " << std::flush;
					PacketFileReaderTmp  packet_reader(argv[count_1], packet_format);
					unsigned int      packet_count = 0;
					const void       *begin_ptr;
					const void       *end_ptr;
					while (packet_reader.GetPacket(begin_ptr, end_ptr))
						++packet_count;
					std::cout << ": Packets = " << packet_count << std::endl;
				}
				catch (const std::exception &except) {
					std::cout << ": Error processing packet file '" <<
						argv[count_1] << "': " << except.what() << std::endl;
					return_code = EXIT_FAILURE;
				}
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

