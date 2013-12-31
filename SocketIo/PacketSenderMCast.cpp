// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketSenderMCast class.

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

#include <SocketIo/PacketSenderMCast.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketSenderMCast::PacketSenderMCast(boost::asio::io_service &io_service,
	const std::string &socket_address, IpPortType ip_port,
	const std::string &host_interface)
	:PacketSender(io_service, socket_address, ip_port, host_interface, true)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderMCast::PacketSenderMCast(boost::asio::io_service &io_service,
	const std::string &socket_address, const std::string &ip_port,
	const std::string &host_interface)
	:PacketSender(io_service, socket_address, ip_port, host_interface, true)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderMCast::PacketSenderMCast(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec)
	:PacketSender(io_service, socket_spec, true)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderMCast::~PacketSenderMCast()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketSenderMCast::FixUpConstruction()
{
	socket_.set_option(boost::asio::socket_base::broadcast(true));
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
		std::cout << "Sending IP multicasts on " <<
			SocketSpec(ip_address, ip_port, host_interface) << std::endl;
		boost::asio::io_service io_service;
		PacketSenderMCast       packet_sender(io_service, ip_address, ip_port,
			host_interface);
		packet_sender.SendTo("First message of three.");
		packet_sender.SendTo("Second message of three.");
		packet_sender.SendTo("Third message of three.");
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

