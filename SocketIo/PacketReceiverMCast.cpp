// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketReceiverMCast class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketReceiverMCast.hpp>
#include <SocketIo/MulticastIP.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketReceiverMCast::PacketReceiverMCast(boost::asio::io_service &io_service,
	const std::string &socket_address, IpPortType ip_port,
	const std::string &host_interface)
	:PacketReceiver(io_service, socket_address, ip_port, host_interface, false)
	,my_host_interface_(HostInterfaceStringToIpAddress(host_interface))
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiverMCast::PacketReceiverMCast(boost::asio::io_service &io_service,
	const std::string &socket_address, const std::string &ip_port,
	const std::string &host_interface)
	:PacketReceiver(io_service, socket_address, ip_port, host_interface, false)
	,my_host_interface_(HostInterfaceStringToIpAddress(host_interface))
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiverMCast::PacketReceiverMCast(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec)
	:PacketReceiver(io_service, socket_spec, false)
	,my_host_interface_(socket_spec.local_interface_)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiverMCast::~PacketReceiverMCast()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketReceiverMCast::FixUpConstruction()
{
	const char *action_name;

	try {
		action_name = "check that the address is a valid IP multicast group";
		CheckIsMulticastIP(my_address_);
		action_name = "bind the socket to the local endpoint";
		{
#if linux
			socket_.bind(boost::asio::ip::udp::endpoint(endpoint_.address(),
				endpoint_.port()));
#else
			EndpointType bind_endpoint(my_host_interface_, endpoint_.port());
			socket_.bind(bind_endpoint);
#endif // #if linux
		}
		action_name = "join the multicast group";
      socket_.set_option(boost::asio::ip::multicast::join_group(
			endpoint_.address().to_v4(), my_host_interface_.to_v4()));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to complete construction of an "
			"instance of 'PacketReceiverMCast' failed: Attempt to " +
			std::string(action_name) + " failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		std::string ip_address("239.255.1.46");
		std::string ip_port("8612");
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
		std::cout << "Receiving IP multicasts on " <<
			SocketSpec(ip_address, ip_port, host_interface) << std::endl;
		boost::asio::io_service io_service;
		PacketReceiverMCast     test_receiver(io_service, ip_address, ip_port,
			host_interface);
		boost::asio::ip::udp::endpoint src_endpoint;
		std::string packet_data;
		test_receiver.RecvFrom(packet_data, src_endpoint);
		std::cout << "First message of three  : " << packet_data << std::endl;
		test_receiver.RecvFrom(packet_data, src_endpoint);
		std::cout << "Second message of three : " << packet_data << std::endl;
		test_receiver.RecvFrom(packet_data, src_endpoint);
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

