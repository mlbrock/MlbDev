// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketReceiver class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketReceiver.hpp>
#include <SocketIo/ResolveService.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketReceiver::PacketReceiver(boost::asio::io_service &io_service,
	const std::string &socket_address, IpPortType ip_port,
	const std::string &host_interface, bool bind_local)
	:SocketBase<boost::asio::ip::udp::socket>(
		io_service, ip_port, host_interface, true, bind_local)
	,my_address_(StringToIpAddress(socket_address))
{
	endpoint_ = EndpointType(StringToIpAddress(socket_address),
		ResolveService(ip_port));

	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiver::PacketReceiver(boost::asio::io_service &io_service,
	const std::string &socket_address, const std::string &ip_port,
	const std::string &host_interface, bool bind_local)
	:SocketBase<boost::asio::ip::udp::socket>(
		io_service, ip_port, host_interface, true, bind_local)
	,my_address_(StringToIpAddress(socket_address))
{
	endpoint_ = EndpointType(StringToIpAddress(socket_address),
		ResolveService(ip_port));

	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiver::PacketReceiver(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec, bool bind_local)
	:SocketBase<boost::asio::ip::udp::socket>(
		io_service, socket_spec, true, bind_local)
	,my_address_(socket_spec.ip_address_)
{
	endpoint_ = EndpointType(socket_spec.ip_address_, socket_spec.ip_port_);

	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiver::~PacketReceiver()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketReceiver::FixUpConstruction()
{
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
		PacketReceiver          test_receiver(io_service, ip_address, ip_port,
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

