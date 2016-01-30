// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketReceiverMC class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketReceiverMC_hpp__HH

#define HH__MLB__SocketIo__PacketReceiverMC_hpp__HH	1

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
class PacketReceiverMC {
public:
	PacketReceiverMC(boost::asio::io_service &io_service,
		const std::string &mc_group, IpPortType ip_port,
		const std::string &host_interface = "");
	PacketReceiverMC(boost::asio::io_service &io_service,
		const std::string &mc_group, const std::string &ip_port,
		const std::string &host_interface = "");
	PacketReceiverMC(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec);

	virtual ~PacketReceiverMC();

	bool                          IsOpen() const;
	void                          Close();
	boost::asio::io_service      &GetIoService();
	boost::asio::ip::udp::socket &GetSocket();

	/**
		Determine the IP address of the multicast group in host order.

		\return the IP address of the multicast group.
	*/
	IpAddressType                 GetIpAddress() const;

	/**
		Determine the port of the multicast group in host order.

		\return The port of the multicast group.
	*/
	IpPortType                    GetPort() const;

	std::size_t RecvFrom(const boost::asio::mutable_buffers_1 &packet_buffer,
		boost::asio::ip::udp::endpoint &src_endpoint);
	std::size_t RecvFrom(std::size_t packet_length, void *packet_ptr,
		boost::asio::ip::udp::endpoint &src_endpoint);
	std::size_t RecvFrom(void *begin_ptr, void *end_ptr,
		boost::asio::ip::udp::endpoint &src_endpoint);
	std::size_t RecvFrom(std::string &packet_data,
		boost::asio::ip::udp::endpoint &src_endpoint);

	std::size_t RecvFrom(const boost::asio::mutable_buffers_1 &packet_buffer);
	std::size_t RecvFrom(std::size_t packet_length, void *packet_ptr);
	std::size_t RecvFrom(void *begin_ptr, void *end_ptr);
	std::size_t RecvFrom(std::string &packet_data);

	void        SetRecvBufferSize(std::size_t buffer_size);
	std::size_t GetRecvBufferSize() const;

protected:
	boost::asio::ip::address       my_address_;
	boost::asio::ip::address       host_interface_;
	boost::asio::ip::udp::endpoint endpoint_;
	boost::asio::ip::udp::socket   socket_;

private:
	PacketReceiverMC(const PacketReceiverMC &other);
	PacketReceiverMC & operator = (const PacketReceiverMC &other);

	void FixUpConstruction();
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketReceiverMC_hpp__HH

