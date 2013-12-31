// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the StreamSocket class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__StreamSocket_hpp__HH

#define HH__MLB__SocketIo__StreamSocket_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketBase.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class StreamSocket : public SocketBase<boost::asio::ip::tcp::socket> {
public:
	StreamSocket(boost::asio::io_service &io_service);
	StreamSocket(boost::asio::io_service &io_service, IpPortType ip_port,
		const std::string &host_interface = "");
	StreamSocket(boost::asio::io_service &io_service,
		const std::string &ip_port, const std::string &host_interface = "");
	StreamSocket(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec);

	virtual ~StreamSocket();

	void Connect(const char *ip_address, const char *ip_port,
		const char *local_interface);
	void Connect(const boost::asio::ip::address &ip_address, IpPortType ip_port,
		const boost::asio::ip::address &local_interface =
		boost::asio::ip::address());
	void Connect(const std::string &ip_address, const std::string &ip_port,
		const std::string &host_interface = "");
	void Connect(const SocketSpec &socket_spec);
	std::size_t Recv(const boost::asio::mutable_buffers_1 &packet_buffer,
		boost::asio::socket_base::message_flags flags = 0);
	std::size_t Recv(std::size_t packet_length, void *packet_ptr,
		boost::asio::socket_base::message_flags flags = 0);
	std::size_t Recv(void *begin_ptr, void *end_ptr,
		boost::asio::socket_base::message_flags flags = 0);
	std::size_t Recv(std::string &packet_data,
		boost::asio::socket_base::message_flags flags = 0);

	std::size_t Send(const boost::asio::const_buffers_1 &packet_buffer,
		boost::asio::socket_base::message_flags flags = 0);
	std::size_t Send(std::size_t packet_length, const void *packet_ptr,
		boost::asio::socket_base::message_flags flags = 0);
	std::size_t Send(const void *begin_ptr, const void *end_ptr,
		boost::asio::socket_base::message_flags flags = 0);
	std::size_t Send(const std::string &packet_data,
		boost::asio::socket_base::message_flags flags = 0);

private:
	StreamSocket(const StreamSocket &other);
	StreamSocket & operator = (const StreamSocket &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<StreamSocket> StreamSocketSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__StreamSocket_hpp__HH

