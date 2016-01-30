// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the StreamSocket class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/StreamAcceptor.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptor::StreamAcceptor(boost::asio::io_service &io_service)
	:SocketBase<boost::asio::ip::tcp::acceptor>(io_service)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptor::StreamAcceptor(boost::asio::io_service &io_service,
	IpPortType ip_port, const std::string &host_interface, bool reuse_addr)
	:SocketBase<boost::asio::ip::tcp::acceptor>(io_service, ip_port,
		host_interface, reuse_addr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptor::StreamAcceptor(boost::asio::io_service &io_service,
	const std::string &ip_port, const std::string &host_interface,
	bool reuse_addr)
	:SocketBase<boost::asio::ip::tcp::acceptor>(io_service, ip_port,
		host_interface, reuse_addr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptor::StreamAcceptor(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec, bool reuse_addr)
	:SocketBase<boost::asio::ip::tcp::acceptor>(io_service, socket_spec,
		reuse_addr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptor::~StreamAcceptor()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StreamAcceptor::Accept(
	SocketBase<boost::asio::ip::tcp::socket> &peer_socket)
{
	try {
		socket_.accept(peer_socket.GetSocket(), peer_socket.GetEndPoint());
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Accept on a socket failed: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StreamAcceptor::Listen(int backlog)
{
	try {
		socket_.listen(backlog);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Listen on a socket failed for a backlog "
			"of " + MLB::Utility::AnyToString(backlog) + " connections: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

#include <SocketIo/StreamSocket.hpp>

#include <Utility/CriticalEventHandler.hpp>
#include <Utility/Utility_Exception.hpp>

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		std::string ip_port("8612");
		std::string host_interface;
		if (argc > 1) {
			ip_port = argv[1];
			if (argc > 2)
				host_interface = argv[2];
		}
		SocketSpec srv_addr("127.0.0.1", ip_port, host_interface);
		std::cout << "Server will listen on " << srv_addr << std::endl;
		boost::asio::io_service io_service;
		StreamAcceptor          test_acceptor(io_service, ip_port, host_interface);
		test_acceptor.Listen();
		for ( ; ; ) {
			StreamSocket test_peer(io_service);
			test_acceptor.Accept(test_peer);
			SocketSpec cln_addr(test_peer.GetIpAddress(), test_peer.GetPort());
			std::cout << "Accepted a connection on from " << cln_addr << "." <<
				std::endl;
			std::string send_text("Line 1: YOUR ADDR INFO=" + cln_addr.ToString());
			test_peer.Send(send_text);
			std::cout << "SENT: [" << send_text << "]" << std::endl;
			send_text = "Line 2: Some data.";
			test_peer.Send(send_text);
			std::cout << "SENT: [" << send_text << "]" << std::endl;
			send_text = "Line 3: Some more data (and the last line).";
			test_peer.Send(send_text);
			std::cout << "SENT: [" << send_text << "]" << std::endl;
			std::cout << "Disconnecting from " << cln_addr << "." << std::endl;
			if (MLB::Utility::CriticalEventTest())
				throw MLB::Utility::ExceptionCriticalEvent();
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

