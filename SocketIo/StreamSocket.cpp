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

#include <SocketIo/StreamSocket.hpp>

#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
StreamSocket::StreamSocket(boost::asio::io_service &io_service)
	:SocketBase<boost::asio::ip::tcp::socket>(io_service)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocket::StreamSocket(boost::asio::io_service &io_service,
	IpPortType ip_port, const std::string &host_interface)
	:SocketBase<boost::asio::ip::tcp::socket>(io_service, ip_port, host_interface)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocket::StreamSocket(boost::asio::io_service &io_service,
	const std::string &ip_port, const std::string &host_interface)
	:SocketBase<boost::asio::ip::tcp::socket>(io_service, ip_port, host_interface)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocket::StreamSocket(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec)
	:SocketBase<boost::asio::ip::tcp::socket>(io_service, socket_spec)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocket::~StreamSocket()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StreamSocket::Connect(const char *ip_address, const char *ip_port,
	const char *local_interface)
{
	if ((ip_address == NULL) || (!(*ip_address)))
		MLB::Utility::ThrowInvalidArgument("The hostname or IP address is "
			"empty.");

	if ((ip_port == NULL) || (!(*ip_port)))
		MLB::Utility::ThrowInvalidArgument("The IP port number or service name "
			"is empty.");

	boost::asio::ip::address tmp_ip_address;
	IpPortType               tmp_ip_port;

	try {	
		tmp_ip_address = StringToIpAddress(ip_address);
		tmp_ip_port    = ResolveService(ip_port);
	}
	catch (const std::exception &) {
		using namespace boost::asio;
		//	Didn't succeed. Try to resolve it as a host name...
		ip::tcp::resolver           resolver(socket_.get_io_service());
		ip::tcp::resolver::query    query(ip_address, ip_port);
		ip::tcp::resolver::iterator iter_b(resolver.resolve(query));
		ip::tcp::resolver::iterator iter_e;
		if (iter_b == iter_e)
			MLB::Utility::ThrowLogicError("Attempt to resolve hostname '" +
				std::string(ip_address) + "' failed.");
		EndpointType tmp_endpoint(*iter_b);
		tmp_ip_address = tmp_endpoint.address();
		tmp_ip_port    = tmp_endpoint.port();
	}

	boost::asio::ip::address tmp_local_interface(((local_interface == NULL) ||
		(!(*local_interface))) ? StringToIpAddress("0.0.0.0") :
		HostInterfaceStringToIpAddress(local_interface));

	Connect(tmp_ip_address, tmp_ip_port, tmp_local_interface);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StreamSocket::Connect(const boost::asio::ip::address &ip_address,
	IpPortType ip_port, const boost::asio::ip::address &local_interface)
{
	CheckIsNotConnected();

	const char *action_name;

	try {
		action_name = "resolve the peer to an endpoint";
		boost::system::error_code      error_code;
		EndpointType peer_endpoint(ip_address, ip_port);
		action_name = "connect to the peer";
		socket_.connect(peer_endpoint, error_code);
#if defined(BOOST_WINDOWS) && (BOOST_VERSION < 103900)
		/*
			The boost::asio didn't correctly clear the error status under Windows
			prior to version 1.39.
		*/
		if ((error_code.value() != 0) && (error_code.value() != 2))
			throw boost::system::system_error(error_code);
#else
		if (error_code.value() != 0)
			throw boost::system::system_error(error_code);
#endif // defined(BOOST_WINDOWS) && (BOOST_VERSION < 103900)
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to complete construction of an instance of the "
			"'StreamSocket' class for " << ip_address << ":" << ip_port <<
			" using host interface " << local_interface << " failed: Attempt to " <<
			action_name << " failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StreamSocket::Connect(const std::string &ip_address,
	const std::string &ip_port, const std::string &host_interface)
{
	Connect(ip_address.c_str(), ip_port.c_str(), host_interface.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StreamSocket::Connect(const SocketSpec &socket_spec)
{
	Connect(socket_spec.ip_address_, socket_spec.ip_port_,
		socket_spec.local_interface_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t StreamSocket::Recv(
	const boost::asio::mutable_buffers_1 &packet_buffer,
	boost::asio::socket_base::message_flags flags)
{
	return(socket_.receive(packet_buffer, flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t StreamSocket::Recv(std::size_t packet_length,
	void *packet_ptr, boost::asio::socket_base::message_flags flags)
{
	return(Recv(boost::asio::buffer(packet_ptr, packet_length), flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t StreamSocket::Recv(void *begin_ptr, void *end_ptr,
	boost::asio::socket_base::message_flags flags)
{
	if (begin_ptr >= end_ptr)
		MLB::Utility::ThrowInvalidArgument("The length of the data in the "
			"packet to be sent is specified by a begin pointer (" +
			MLB::Utility::ValueToStringHex(begin_ptr) + ") which is not less "
			"than the end pointer (" + MLB::Utility::ValueToStringHex(end_ptr) + 
			").");

	return(Recv(static_cast<std::size_t>(static_cast<char *>(end_ptr) -
		static_cast<char *>(begin_ptr)), begin_ptr, flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t StreamSocket::Recv(std::string &packet_data,
	boost::asio::socket_base::message_flags flags)
{
	char packet_buffer[0xFFFF + 1];

	std::size_t packet_size = Recv(sizeof(packet_buffer), packet_buffer, flags);

	if (packet_size <= packet_data.size()) {
		::memcpy(const_cast<char *>(packet_data.data()), packet_buffer,
			packet_size);
		if (packet_size < packet_data.size())
			packet_data.resize(packet_size);
	}
	else
		std::string(packet_buffer, packet_size).swap(packet_data);

	return(packet_size);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t StreamSocket::Send(
	const boost::asio::const_buffers_1 &packet_buffer,
	boost::asio::socket_base::message_flags flags)
{
	return(socket_.send(packet_buffer, flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t StreamSocket::Send(std::size_t packet_length,
	const void *packet_ptr, boost::asio::socket_base::message_flags flags)
{
	if (packet_length < 1)
		MLB::Utility::ThrowInvalidArgument("The length of the data to be sent "
			"is specified by a length of zero.");

	return(Send(boost::asio::buffer(packet_ptr, packet_length), flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t StreamSocket::Send(const void *begin_ptr, const void *end_ptr,
	boost::asio::socket_base::message_flags flags)
{
	if (begin_ptr >= end_ptr)
		MLB::Utility::ThrowInvalidArgument("The length of the data to be sent "
			"is specified by a begin pointer (" +
			MLB::Utility::ValueToStringHex(begin_ptr) + ") which is not less "
			"than the end pointer (" + MLB::Utility::ValueToStringHex(end_ptr) + 
			").");

	return(Send(static_cast<std::size_t>(
		static_cast<const char *>(end_ptr) -
		static_cast<const char *>(begin_ptr)), begin_ptr, flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t StreamSocket::Send(const std::string &packet_data,
	boost::asio::socket_base::message_flags flags)
{
	return(Send(packet_data.size(), packet_data.c_str(), flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/CriticalEventHandler.hpp>
#include <Utility/Utility_Exception.hpp>

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		std::string ip_address("localhost");
ip_address = "127.0.0.1";
		std::string ip_port("8612");
		std::string host_interface;
		if (argc > 1) {
			ip_address = argv[1];
			if (argc > 2) {
				ip_port = argv[2];
				if (argc > 3)
					host_interface = argv[3];
			}
		}
		SocketSpec srv_addr(ip_address, ip_port, host_interface);
		std::cout << "Client will connect to " << srv_addr << std::endl;
		boost::asio::io_service io_service;
		StreamSocket            test_receiver(io_service);
		test_receiver.Connect(ip_address, ip_port);
		std::string rcv_text;
		test_receiver.Recv(rcv_text);
		std::cout << "Message 1 of 3: [" << rcv_text << "]" << std::endl;
		test_receiver.Recv(rcv_text);
		std::cout << "Message 2 of 3: [" << rcv_text << "]" << std::endl;
		test_receiver.Recv(rcv_text);
		std::cout << "Message 3 of 3: [" << rcv_text << "]" << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

