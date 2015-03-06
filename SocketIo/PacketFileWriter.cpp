// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketFileWriter class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketFileWriter.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __GNUC__
# include <memory.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketFileWriter::PacketFileWriter(const std::string &file_name,
	PacketFormat packet_format)
	:file_name_(file_name)
	,packet_format_(CheckPacketFormat(packet_format))
	,msg_file_(file_name.c_str(), std::ios_base::binary)
	,captured_count_(0)
	,initial_file_size_(0)
	,file_offset_(0)
{
	try {
		if (msg_file_.fail())
			MLB::Utility::ThrowSystemError("Open failed.");
		SeekToOffsetFile(0, std::ios_base::end);
		initial_file_size_ = static_cast<unsigned long long>(msg_file_.tellp());
		if (msg_file_.fail())
			MLB::Utility::ThrowLogicError("Attempt to determine the end of file "
				"position failed.");
		file_offset_ = initial_file_size_;
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to open packet file '" +
			file_name_ + "' for writing: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string PacketFileWriter::GetFileName() const
{
	return(file_name_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileWriter::GetInitialSize() const
{
	return(initial_file_size_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileWriter::GetCurrentOffset() const
{
	return(file_offset_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileWriter::WritePacket(std::size_t packet_length,
	const void *packet_ptr, IpAddressType ip_address, IpPortType ip_port,
	MsgTimeSecsType time_secs, MsgTimeNSecsType time_nsecs)
{
	if ((!packet_length) ||
		(packet_length > std::numeric_limits<MsgLengthType>::max()))
		MLB::Utility::ThrowInvalidArgument("The length specified for writing a "
			"packet to the canned data file (" +
			MLB::Utility::AnyToString(packet_length) + ") is outside of the "
			"permissible range (1 through " +
			MLB::Utility::AnyToString(std::numeric_limits<MsgLengthType>::max()) +
			", inclusive).");

	char                buffer[PacketFileWriterBufferLength];
	char               *buffer_ptr    = buffer;
	unsigned int        buffer_length = 0;
	unsigned long long  packet_offset = file_offset_;

	if (packet_format_ == PacketFormat_Raw)
		msg_file_.write(reinterpret_cast<const char *>(packet_ptr),
			static_cast<std::streamsize>(packet_length));
	else {		
		//	If IP source info is first, emit it...
		if (packet_format_ == PacketFormat_IpSourceTLen) {
			WriteIpSourceHeader(buffer_ptr, ip_address, ip_port);
			buffer_ptr    += Ip4AddPortSize;
			buffer_length += Ip4AddPortSize;
		}
		//	Now emit the packet data length...
		MsgLengthType tmp_packet_length =
			htons(static_cast<MsgLengthType>(packet_length));
		::memcpy(buffer_ptr, &tmp_packet_length, sizeof(tmp_packet_length));
		buffer_ptr    += sizeof(tmp_packet_length);
		buffer_length += sizeof(tmp_packet_length);
		//	If the packet receive timestamp is to be emitted...
		if ((packet_format_ == PacketFormat_Time) ||
			 (packet_format_ == PacketFormat_TimeIpSource)) {
			WriteTimestampHeader(buffer_ptr, time_secs, time_nsecs);
			buffer_ptr    += MsgTimeStampSize;
			buffer_length += MsgTimeStampSize;
		}
		//	If the IP source info is to be emitted (but not IpSourceTLeN format)...
		if ((packet_format_ == PacketFormat_IpSource) ||
			 (packet_format_ == PacketFormat_TimeIpSource)) {
			WriteIpSourceHeader(buffer_ptr, ip_address, ip_port);
			buffer_ptr    += Ip4AddPortSize;
			buffer_length += Ip4AddPortSize;
		}
		if (packet_length <= (sizeof(buffer) - buffer_length)) {
			//	Placement into buffer so that only a single write() is needed...
			::memcpy(buffer_ptr, packet_ptr, packet_length);
			msg_file_.write(reinterpret_cast<const char *>(buffer),
				static_cast<std::streamsize>(buffer_length + packet_length));
		}
		else {
			// Otherwise use two write() calls...
			msg_file_.write(buffer, static_cast<std::streamsize>(buffer_length));
			msg_file_.write(reinterpret_cast<const char *>(packet_ptr),
				static_cast<std::streamsize>(packet_length));
		}
	}

	file_offset_ += buffer_length + packet_length;

	return(packet_offset);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileWriter::WritePacket(std::size_t packet_length,
	const void *packet_ptr, IpAddressType ip_address, IpPortType ip_port,
	const MLB::Utility::TimeSpec &packet_timestamp)
{
	return(WritePacket(packet_length, packet_ptr, ip_address, ip_port,
		static_cast<MsgTimeSecsType>(packet_timestamp.tv_sec),
		static_cast<MsgTimeNSecsType>(packet_timestamp.tv_nsec)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileWriter::WritePacket(std::size_t packet_length,
	const void *packet_ptr, const boost::asio::ip::udp::endpoint &src_endpoint,
	MsgTimeSecsType time_secs, MsgTimeNSecsType time_nsecs)
{
	return(WritePacket(packet_length, packet_ptr,
		src_endpoint.address().to_v4().to_ulong(), src_endpoint.port(),
		time_secs, time_nsecs));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long PacketFileWriter::WritePacket(std::size_t packet_length,
	const void *packet_ptr, const boost::asio::ip::udp::endpoint &src_endpoint,
	const MLB::Utility::TimeSpec &packet_timestamp)
{
	return(WritePacket(packet_length, packet_ptr,
		src_endpoint.address().to_v4().to_ulong(), src_endpoint.port(),
		static_cast<MsgTimeSecsType>(packet_timestamp.tv_sec),
		static_cast<MsgTimeNSecsType>(packet_timestamp.tv_nsec)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileWriter::WriteTimestampHeader(char *buffer_ptr,
	MsgTimeSecsType time_secs, MsgTimeNSecsType time_nsecs)
{

	MsgTimeSecsType  tmp_time_secs  = htonl(time_secs);
	MsgTimeNSecsType tmp_time_nsecs = htonl(time_nsecs);

	::memcpy(buffer_ptr, &tmp_time_secs, sizeof(tmp_time_secs));
	::memcpy(buffer_ptr + sizeof(tmp_time_secs), &tmp_time_nsecs,
		sizeof(tmp_time_nsecs));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileWriter::WriteIpSourceHeader(char *buffer_ptr,
	IpAddressType ip_address, IpPortType ip_port)
{
	IpAddressType tmp_ip_address = htonl(ip_address);
	IpPortType    tmp_ip_port    = htons(ip_port);

	::memcpy(buffer_ptr, &tmp_ip_address, sizeof(tmp_ip_address));
	::memcpy(buffer_ptr + sizeof(tmp_ip_address), &tmp_ip_port,
		sizeof(tmp_ip_port));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileWriter::SeekToOffsetFile(unsigned long long file_offset,
	std::ios_base::seekdir offset_base)
{
	msg_file_.seekp(static_cast<std::istream::off_type>(file_offset),
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

namespace {
// ////////////////////////////////////////////////////////////////////////////
const char         *TEST_PacketFileWriterDataList[] = {
	"Packet number one.",
	"Packet number two.",
	"Packet number three.",
	"Packet number four.",
	"Last packet (packet number five)."
};
const unsigned int  TEST_PacketFileWriterDataCount  =
	sizeof(TEST_PacketFileWriterDataList) /
	sizeof(TEST_PacketFileWriterDataList[0]);
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::vector<std::string> file_list;
		int                      count_1;
		for (count_1 = static_cast<int>(PacketFormat_Min);
			count_1 <= static_cast<int>(PacketFormat_Max); ++count_1) {
			IpAddressType ip_address;
			IpPortType    ip_port = static_cast<IpPortType>(count_1);
			PacketFormat  packet_format = static_cast<PacketFormat>(count_1);
			std::string  file_name("./TEST_PacketFileWriter." +
				PacketFormatToString(packet_format) + ".binary_packet_data");
			if (count_1 != (static_cast<int>(PacketFormat_Raw)))
				file_list.push_back(file_name);
			::memset(&ip_address, count_1, sizeof(ip_address));
			std::cout << "Writing '" << file_name << "'... " << std::flush;
			{
				PacketFileWriter p_writer(file_name, packet_format);
				const char   **data_ptr = TEST_PacketFileWriterDataList;
				unsigned int   count_2;
				for (count_2 = 0; count_2 < TEST_PacketFileWriterDataCount;
					++count_2, ++data_ptr)
					p_writer.WritePacket(::strlen(*data_ptr), *data_ptr,
						ip_address, ip_port);
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

