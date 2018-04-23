// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketFileWriter class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketFileWriter_hpp__HH

#define HH__MLB__SocketIo__PacketFileWriter_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/BoostAsioInclude.hpp>
#include <SocketIo/PacketFile.hpp>

#include <Utility/TimeSupport.hpp>

#include <fstream>

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

namespace {
// ////////////////////////////////////////////////////////////////////////////
const unsigned int PacketFileWriterBufferLength = sizeof(MsgLengthType) +
	sizeof(IpAddressType) + sizeof(IpPortType) + 0xFFFF + 1;
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
class PacketFileWriter {
public:
	PacketFileWriter(const std::string &file_name,
		PacketFormat packet_format);

	std::string        GetFileName() const;
	unsigned long long GetInitialSize() const;
	unsigned long long GetCurrentOffset() const;

	unsigned long long WritePacket(std::size_t packet_length,
		const void *packet_ptr, IpAddressType ip_address = 0,
		IpPortType ip_port = 0, MsgTimeSecsType time_secs = 0,
		MsgTimeNSecsType time_nsecs = 0);
	unsigned long long WritePacket(std::size_t packet_length,
		const void *packet_ptr, IpAddressType ip_address,
		IpPortType ip_port, const MLB::Utility::TimeSpec &packet_timestamp);
	unsigned long long WritePacket(std::size_t packet_length,
		const void *packet_ptr,
		const boost::asio::ip::udp::endpoint &src_endpoint,
		MsgTimeSecsType time_secs = 0, MsgTimeNSecsType time_nsecs = 0);
	unsigned long long WritePacket(std::size_t packet_length,
		const void *packet_ptr,
		const boost::asio::ip::udp::endpoint &src_endpoint,
		const MLB::Utility::TimeSpec &packet_timestamp);

private:
	void WriteTimestampHeader(char *buffer_ptr, MsgTimeSecsType time_secs,
		MsgTimeNSecsType time_nsecs);
	void WriteIpSourceHeader(char *buffer_ptr, IpAddressType ip_address,
		IpPortType ip_port);
	void SeekToOffsetFile(unsigned long long file_offset,
		std::ios_base::seekdir offset_base = std::ios_base::beg);

	std::string        file_name_;
	PacketFormat       packet_format_;
	std::ofstream      msg_file_;
	unsigned long long captured_count_;
	unsigned long long initial_file_size_;
	unsigned long long file_offset_;

	PacketFileWriter(const PacketFileWriter &other);
	PacketFileWriter & operator = (const PacketFileWriter &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<PacketFileWriter> PacketFileWriterSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketFileWriter_hpp__HH

