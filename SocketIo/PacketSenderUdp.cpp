// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketSenderUdp class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2026.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketSenderUdp.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketSenderUdp::PacketSenderUdp(boost::asio::io_service &io_service,
	const std::string &socket_address, IpPortType ip_port,
	const std::string &host_interface)
	:PacketSender(io_service, socket_address, ip_port, host_interface, true)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderUdp::PacketSenderUdp(boost::asio::io_service &io_service,
	const std::string &socket_address, const std::string &ip_port,
	const std::string &host_interface)
	:PacketSender(io_service, socket_address, ip_port, host_interface, true)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderUdp::PacketSenderUdp(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec)
	:PacketSender(io_service, socket_spec, true)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderUdp::~PacketSenderUdp()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketSenderUdp::FixUpConstruction()
{
	socket_.set_option(boost::asio::socket_base::broadcast(true));
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
/*
	using namespace boost::asio;

	io_service               io_service;
	ip::udp::resolver        resolver(io_service);
	ip::udp::resolver::query query(ip::udp::v4(), MLB::Utility::GetHostName(),
		"", ip::udp::resolver::query::passive |
		ip::udp::resolver::query::address_configured);
	ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);

	return(receiver_endpoint.address().to_string());
*/
	using namespace boost::asio;

	io_service               io_service;
	ip::udp::resolver        resolver(io_service);
#if (BOOST_VERSION <= 104601)
	/*
		ASIO version 1.5.0 removed the resolver query and iterator types.

		The last Boost version which contained the removed elements was Boost
		version 1.46.1 / ASIO version 1.4.9.
	*/
	ip::udp::resolver::query query(ip::udp::v4(), MLB::Utility::GetHostName(),
		"", ip::udp::resolver::query::passive |
		ip::udp::resolver::query::address_configured);
	ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);

	return(receiver_endpoint.address().to_string());
#else
	ip::udp::resolver::results_type results;
	std::string                     ip_address(MLB::Utility::GetHostName());
	std::string                     ip_port("12345");
	try {
		results = resolver.resolve(ip_address.c_str(), ip_port.c_str());
	}
	catch (const std::exception &except) {
		MLB::Utility::ThrowLogicError("Attempt to resolve host '" +
			ip_address + "' with service/port '" +
			ip_port + "' failed: " + std::string(except.what()));
	}
	if (results.empty())
		MLB::Utility::ThrowLogicError("Attempt to resolve host '" +
			ip_address + "' with service/port '" +
			ip_port + "' failed.");
	auto tmp_endpoint(*results.begin());
	return(tmp_endpoint.endpoint().address().to_string());
#endif // #if (BOOST_VERSION <= 104601)
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetThisHostBroadcastAddress()
{
	std::string tmp_address(GetThisHostIpAddress());

	return(tmp_address.substr(0, tmp_address.rfind('.')) + ".255");
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		std::string ip_address(GetThisHostBroadcastAddress());
		std::string ip_port("8611");
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
		std::cout << "Sending UDP broadcasts on " <<
			SocketSpec(ip_address, ip_port, host_interface) << std::endl;
		boost::asio::io_service io_service;
		PacketSenderUdp         packet_sender(io_service, ip_address, ip_port,
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

