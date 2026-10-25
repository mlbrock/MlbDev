// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the StreamAcceptor class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2026.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__StreamAcceptor_hpp__HH

#define HH__MLB__SocketIo__StreamAcceptor_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketBase.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class StreamAcceptor : public SocketBase<boost::asio::ip::tcp::acceptor> {
public:
	StreamAcceptor(boost::asio::io_service &io_service);
	StreamAcceptor(boost::asio::io_service &io_service, IpPortType ip_port,
		const std::string &host_interface = "", bool reuse_addr = true);
	StreamAcceptor(boost::asio::io_service &io_service,
		const std::string &ip_port, const std::string &host_interface = "",
		bool reuse_addr = true);
	StreamAcceptor(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec, bool reuse_addr = true);

	virtual ~StreamAcceptor();

	void Accept(SocketBase<boost::asio::ip::tcp::socket> &peer_socket);

	/*
		ASIO version 1.33.0 removed socket_base::max_connections.

		The replacement support (socket_base::max_listen_connections) was added
		in Boost version 1.69.0 / ASIO version 1.12.2.
	*/
#if (BOOST_VERSION < 106900)
	void Listen(int backlog = boost::asio::socket_base::max_connections);
#else
	void Listen(int backlog = boost::asio::socket_base::max_listen_connections);
#endif // #if (BOOST_VERSION < 106900)

private:
	StreamAcceptor(const StreamAcceptor &other);
	StreamAcceptor & operator = (const StreamAcceptor &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<StreamAcceptor> StreamAcceptorSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__StreamAcceptor_hpp__HH

