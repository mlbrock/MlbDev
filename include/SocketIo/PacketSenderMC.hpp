// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketSenderMC class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketSenderMC_hpp__HH

#define HH__MLB__SocketIo__PacketSenderMC_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketSpec.hpp>
#include <SocketIo/StringToIpAddress.hpp>
#include <SocketIo/PacketFile.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class PacketSenderMC {
public:
	PacketSenderMC(boost::asio::io_service &io_service,
		const std::string &mc_group, IpPortType ip_port,
		const std::string &host_interface = "");
	PacketSenderMC(boost::asio::io_service &io_service,
		const std::string &mc_group, const std::string &ip_port,
		const std::string &host_interface = "");
	PacketSenderMC(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec);

	virtual ~PacketSenderMC();

	boost::asio::ip::udp::endpoint endpoint_;
	boost::asio::ip::udp::socket   socket_;

	std::size_t SendTo(const boost::asio::const_buffers_1 &packet_buffer);
	std::size_t SendTo(std::size_t packet_length, const void *packet_ptr);
	std::size_t SendTo(const void *begin_ptr, const void *end_ptr);
	std::size_t SendTo(const std::string &packet_data);

	void        SetSendBufferSize(std::size_t buffer_size);
	std::size_t GetSendBufferSize() const;

private:
	PacketSenderMC(const PacketSenderMC &other);
	PacketSenderMC & operator = (const PacketSenderMC &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketSenderMC_hpp__HH

