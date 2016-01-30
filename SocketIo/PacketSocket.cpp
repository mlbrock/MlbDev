// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketSocket class.

	Revision History	:	2010-11-19 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketSocket.hpp>
#include <SocketIo/ResolveService.hpp>
#include <SocketIo/MulticastIP.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketSocket::PacketSocket(boost::asio::io_service &io_service,
	const std::string &socket_address, IpPortType ip_port,
	const std::string &host_interface, bool is_broadcast, bool bind_local)
	:SocketBase<boost::asio::ip::udp::socket>(
		io_service, ip_port, host_interface, true,
		(IsMulticastIP(socket_address)) ? false : bind_local)
	,is_multicast_(IsMulticastIP(socket_address))
	,is_broadcast_(is_multicast_ || is_broadcast)
	,my_address_(StringToIpAddress(socket_address))
	,packet_endpoint_(StringToIpAddress(socket_address), ResolveService(ip_port))
	,recv_endpoint_((is_multicast_) ? StringToIpAddress("0.0.0.0") :
		StringToIpAddress(socket_address), ResolveService(ip_port))
{
	endpoint_ = packet_endpoint_;

	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSocket::PacketSocket(boost::asio::io_service &io_service,
	const std::string &socket_address, const std::string &ip_port,
	const std::string &host_interface, bool is_broadcast, bool bind_local)
	:SocketBase<boost::asio::ip::udp::socket>(
		io_service, ip_port, host_interface, true,
		(IsMulticastIP(socket_address)) ? false : bind_local)
	,is_multicast_(IsMulticastIP(socket_address))
	,is_broadcast_(is_multicast_ || is_broadcast)
	,my_address_(StringToIpAddress(socket_address))
	,packet_endpoint_(StringToIpAddress(socket_address), ResolveService(ip_port))
	,recv_endpoint_((is_multicast_) ? StringToIpAddress("0.0.0.0") :
		StringToIpAddress(socket_address), ResolveService(ip_port))
{
	endpoint_ = packet_endpoint_;

	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSocket::PacketSocket(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec, bool is_broadcast, bool bind_local)
	:SocketBase<boost::asio::ip::udp::socket>(
		io_service, socket_spec, true,
			(socket_spec.IsMulticastIPAddress()) ? false : bind_local)
	,is_multicast_(socket_spec.IsMulticastIPAddress())
	,is_broadcast_(is_multicast_ || is_broadcast)
	,my_address_(socket_spec.ip_address_)
	,packet_endpoint_(socket_spec.ip_address_, socket_spec.ip_port_)
	,recv_endpoint_((is_multicast_) ? StringToIpAddress("0.0.0.0") :
		socket_spec.ip_address_, socket_spec.ip_port_)
{
	endpoint_ = packet_endpoint_;

	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSocket::~PacketSocket()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketSocket::FixUpConstruction()
{
	const char *action_name;

	try {
		if (is_multicast_) {
/*
			action_name = "open the socket";
			socket_.open(endpoint_.protocol());
*/
/*
			action_name = "set socket open for address reuse";
			socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
*/
			action_name = "bind the socket to the local endpoint";
#if __linux__
			socket_.bind(boost::asio::ip::udp::endpoint(my_address_,
				recv_endpoint_.port()));
#else
			socket_.bind(recv_endpoint_);
#endif // #if __linux__
			action_name = "join the multicast group";
			socket_.set_option(boost::asio::ip::multicast::join_group(
				my_address_.to_v4(), recv_endpoint_.address().to_v4()));
			if (host_interface_ != boost::asio::ip::address())
				socket_.set_option(boost::asio::ip::multicast::outbound_interface(
					host_interface_.to_v4()));
		}
		else if (is_broadcast_) {
			action_name = "set socket option to broadcast";
			socket_.set_option(boost::asio::socket_base::broadcast(true));
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to complete construction of "
			"an instance of 'PacketSocket' failed: Attempt to " +
			std::string(action_name) + " failed: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::SocketIo;

namespace {
// ////////////////////////////////////////////////////////////////////////////
std::string GetThisHostIpAddress()
{
	using namespace boost::asio;

	io_service               io_service;
	ip::udp::resolver        resolver(io_service);
	ip::udp::resolver::query query(ip::udp::v4(), MLB::Utility::GetHostName(),
		"", ip::udp::resolver::query::passive |
		ip::udp::resolver::query::address_configured);
	ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);

	return(receiver_endpoint.address().to_string());
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		std::string ip_address(GetThisHostIpAddress());
		std::string ip_port("8610");
		std::string host_interface;
		if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
			std::cout << "USAGE: " << std::endl <<
				"   " << argv[0] << " " <<
				"[ <ip-address> ( = " << ip_address << ") "
				"[ <ip-port> ( = " << ip_port << ") "
				"[ <host-interface> ] ] ]" <<
				std::endl << std::endl;
			return(EXIT_SUCCESS);
		}
		if (argc > 1) {
			ip_address = argv[1];
			if (argc > 2) {
				ip_port = argv[2];
				if (argc > 3)
					host_interface = argv[3];
			}
		}
		std::cout << "Receiving UDP datagrams on " <<
			SocketSpec(ip_address, ip_port, host_interface) << std::endl;
		boost::asio::io_service io_service;
		PacketSocket            test_socket(io_service, ip_address, ip_port,
			host_interface);
		//	Send
		test_socket.SendTo("Sending first message of three from " +
			MLB::Utility::GetHostName() + ":" +
			MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()));
		test_socket.SendTo("Sending second message of three from " +
			MLB::Utility::GetHostName() + ":" +
			MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()));
		test_socket.SendTo("Sending third message of three from " +
			MLB::Utility::GetHostName() + ":" +
			MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()));
		//	Receive
		boost::asio::ip::udp::endpoint src_endpoint;
		std::string packet_data;
		test_socket.RecvFrom(packet_data, src_endpoint);
		std::cout << "First message of three  : " << packet_data << std::endl;
		test_socket.RecvFrom(packet_data, src_endpoint);
		std::cout << "Second message of three : " << packet_data << std::endl;
		test_socket.RecvFrom(packet_data, src_endpoint);
		std::cout << "Third message of three  : " << packet_data << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

