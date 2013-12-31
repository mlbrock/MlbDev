// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketFileReader class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketFileReader_hpp__HH

#define HH__MLB__SocketIo__PacketFileReader_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketFile.hpp>

#include <Utility/TimeSupport.hpp>

#include <fstream>

#include <boost/shared_array.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class PacketFileReader {
public:
	PacketFileReader(const std::string &file_name, PacketFormat packet_format);

	std::string         GetFileName() const;
	unsigned long long  GetFileSize() const;
	unsigned long long  GetFileOffset() const;
	unsigned long long  GetPacketOffset() const;

	bool                GetPacket(std::size_t &packet_length,
		const void *&packet_ptr);
	bool                GetPacket(std::size_t &packet_length,
		const void *&packet_ptr, IpAddressType &ip_address,
		IpPortType &ip_port);
	bool                GetPacket(std::size_t &packet_length,
		const void *&packet_ptr, IpAddressType &ip_address, IpPortType &ip_port,
		MLB::Utility::TimeSpec &packet_timestamp);
	bool                GetPacket(std::size_t &packet_length,
		const void *&packet_ptr, IpAddressType &ip_address, IpPortType &ip_port,
		MsgTimeSecsType &time_secs, MsgTimeNSecsType &time_nsecs);
	bool                GetPacket(const void *&begin_ptr, const void *&end_ptr);
	bool                GetPacket(const void *&begin_ptr, const void *&end_ptr,
		IpAddressType &ip_address, IpPortType &ip_port);
	bool                GetPacket(const void *&begin_ptr, const void *&end_ptr,
		IpAddressType &ip_address, IpPortType &ip_port,
		MLB::Utility::TimeSpec &packet_timestamp);
	bool                GetPacket(const void *&begin_ptr, const void *&end_ptr,
		IpAddressType &ip_address, IpPortType &ip_port,
		MsgTimeSecsType &time_secs, MsgTimeNSecsType &time_nsecs);

	template <typename DataType>
		bool GetPacketTyped(std::size_t &packet_length,
		const DataType *&packet_ptr);
	template <typename DataType>
		bool GetPacketTyped(std::size_t &packet_length,
		const DataType *&packet_ptr, MsgTimeSecsType &time_secs,
		MsgTimeNSecsType &time_nsecs);
	template <typename DataType>
		bool GetPacketTyped(std::size_t &packet_length,
			const DataType *&packet_ptr, IpAddressType &ip_address,
			IpPortType &ip_port);
	template <typename DataType>
		bool GetPacketTyped(std::size_t &packet_length,
			const DataType *&packet_ptr, IpAddressType &ip_address,
			IpPortType &ip_port, MsgTimeSecsType &time_secs,
			MsgTimeNSecsType &time_nsecs);
	template <typename DataType>
		bool GetPacketTyped(const DataType *&begin_ptr, const DataType *&end_ptr);
	template <typename DataType>
		bool GetPacketTyped(const DataType *&begin_ptr, const DataType *&end_ptr,
		IpAddressType &ip_address, IpPortType &ip_port);
	template <typename DataType>
		bool GetPacketTyped(const DataType *&begin_ptr, const DataType *&end_ptr,
		IpAddressType &ip_address, IpPortType &ip_port,
		MsgTimeSecsType &time_secs, MsgTimeNSecsType &time_nsecs);

	const unsigned char  *GetBeginPtr() const;
	const unsigned char  *GetEndPtr() const;

	unsigned int        GetPacketCount();
	bool                SeekToPacket(unsigned int packet_index);
	bool                SeekToPacket(unsigned int packet_index,
		unsigned int &packet_count);
	void                SeekToOffset(unsigned long long file_offset,
		std::ios_base::seekdir offset_base = std::ios_base::beg);

private:
	bool LoadFile();
	void ReadTimestampHeader(MsgTimeSecsType &time_secs,
		MsgTimeNSecsType &time_nsecs);
	void ReadIpSourceHeader(IpAddressType &ip_address, IpPortType &ip_port);
	void SeekToOffsetFile(unsigned long long file_offset,
		std::ios_base::seekdir offset_base = std::ios_base::beg);

	std::string                        file_name_;
	PacketFormat                       packet_format_;
	std::ifstream                      msg_file_;
	unsigned long long                 file_size_;
	unsigned long long                 file_offset_;
	unsigned long long                 packet_offset_;
	unsigned int                       packet_count_;
	bool                               packet_count_is_known_;
	bool                               file_is_loaded_;
	boost::shared_array<unsigned char> file_data_sptr_;

	class FileOffsetSaver {
	public:
		FileOffsetSaver(PacketFileReader &reader_ref)
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
					reader_ref_.SeekToOffset(file_offset_value_);
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
		PacketFileReader   &reader_ref_;
		unsigned long long  file_offset_value_;
		bool                should_restore_flag_;

		FileOffsetSaver(const FileOffsetSaver &other);
		FileOffsetSaver & operator = (const FileOffsetSaver &other);
	};

	PacketFileReader(const PacketFileReader &other);
	PacketFileReader & operator = (const PacketFileReader &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<PacketFileReader> PacketFileReaderSPtr;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	bool PacketFileReader::GetPacketTyped(std::size_t &packet_length,
		const DataType *&packet_ptr)
{
	return(GetPacket(packet_length,
		reinterpret_cast<const void *&>(packet_ptr)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	bool PacketFileReader::GetPacketTyped(std::size_t &packet_length,
		const DataType *&packet_ptr, MsgTimeSecsType &time_secs,
		MsgTimeNSecsType &time_nsecs)
{
	IpAddressType ip_address;
	IpPortType    ip_port;

	return(GetPacketTyped(packet_length, packet_ptr, ip_address, ip_port,
		time_secs, time_nsecs));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	bool PacketFileReader::GetPacketTyped(std::size_t &packet_length,
		const DataType *&packet_ptr, IpAddressType &ip_address,
		IpPortType &ip_port)
{
	return(GetPacket(packet_length, reinterpret_cast<const void *&>(packet_ptr),
		ip_address, ip_port));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	bool PacketFileReader::GetPacketTyped(std::size_t &packet_length,
		const DataType *&packet_ptr, IpAddressType &ip_address,
		IpPortType &ip_port, MsgTimeSecsType &time_secs,
		MsgTimeNSecsType &time_nsecs)
{
	return(GetPacket(packet_length, reinterpret_cast<const void *&>(packet_ptr),
		ip_address, ip_port, time_secs, time_nsecs));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	bool PacketFileReader::GetPacketTyped(const DataType *&begin_ptr,
		const DataType *&end_ptr)
{
	return(GetPacket(reinterpret_cast<const void *&>(begin_ptr),
		reinterpret_cast<const void *&>(end_ptr)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	bool PacketFileReader::GetPacketTyped(const DataType *&begin_ptr,
		const DataType *&end_ptr, IpAddressType &ip_address, IpPortType &ip_port)
{
	return(GetPacket(reinterpret_cast<const void *&>(begin_ptr),
		reinterpret_cast<const void *&>(end_ptr), ip_address, ip_port));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	bool PacketFileReader::GetPacketTyped(const DataType *&begin_ptr,
		const DataType *&end_ptr, IpAddressType &ip_address, IpPortType &ip_port,
		MsgTimeSecsType &time_secs, MsgTimeNSecsType &time_nsecs)
{
	return(GetPacket(reinterpret_cast<const void *&>(begin_ptr),
		reinterpret_cast<const void *&>(end_ptr), ip_address, ip_port,
		time_secs, time_nsecs));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketFileReader_hpp__HH

