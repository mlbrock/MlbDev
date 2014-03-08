// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketReceiverMC class.

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

#include <SocketIo/PacketReceiverMC.hpp>
#include <SocketIo/ResolveService.hpp>
#include <SocketIo/SocketSettings.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketReceiverMC::PacketReceiverMC(boost::asio::io_service &io_service,
	const std::string &mc_group, IpPortType ip_port,
	const std::string &host_interface)
	:my_address_(StringToIpAddress(mc_group))
	,host_interface_(HostInterfaceStringToIpAddress(host_interface))
	,endpoint_(StringToIpAddress("0.0.0.0"), ResolveService(ip_port))
	,socket_(io_service)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiverMC::PacketReceiverMC(boost::asio::io_service &io_service,
	const std::string &mc_group, const std::string &ip_port,
	const std::string &host_interface)
	:my_address_(StringToIpAddress(mc_group))
	,host_interface_(HostInterfaceStringToIpAddress(host_interface))
	,endpoint_(StringToIpAddress("0.0.0.0"), ResolveService(ip_port))
	,socket_(io_service)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiverMC::PacketReceiverMC(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec)
	:my_address_(socket_spec.ip_address_)
	,host_interface_(socket_spec.local_interface_)
	,endpoint_(StringToIpAddress("0.0.0.0"), socket_spec.ip_port_)
	,socket_(io_service)
{
	FixUpConstruction();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketReceiverMC::~PacketReceiverMC()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketReceiverMC::IsOpen() const
{
	return(socket_.is_open());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketReceiverMC::Close()
{
	if (IsOpen())
		socket_.close();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::asio::ip::udp::socket &PacketReceiverMC::GetSocket()
{
	return(socket_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::asio::io_service &PacketReceiverMC::GetIoService()
{
	return(socket_.get_io_service());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
IpAddressType PacketReceiverMC::GetIpAddress() const
{
	return(my_address_.to_v4().to_ulong());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
IpPortType PacketReceiverMC::GetPort() const
{
	return(endpoint_.port());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketReceiverMC::RecvFrom(
	const boost::asio::mutable_buffers_1 &packet_buffer,
	boost::asio::ip::udp::endpoint &src_endpoint)
{
	return(socket_.receive_from(packet_buffer, src_endpoint));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketReceiverMC::RecvFrom(std::size_t packet_length,
	void *packet_ptr, boost::asio::ip::udp::endpoint &src_endpoint)
{
	return(RecvFrom(boost::asio::buffer(packet_ptr, packet_length),
		src_endpoint));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketReceiverMC::RecvFrom(void *begin_ptr, void *end_ptr,
	boost::asio::ip::udp::endpoint &src_endpoint)
{
	if (begin_ptr >= end_ptr)
		MLB::Utility::ThrowInvalidArgument("The length of the data in the "
			"packet to be sent is specified by a begin pointer (" +
			MLB::Utility::ValueToStringHex(begin_ptr) + ") which is not less "
			"than the end pointer (" + MLB::Utility::ValueToStringHex(end_ptr) + 
			").");

	return(RecvFrom(static_cast<std::size_t>(static_cast<char *>(end_ptr) -
		static_cast<char *>(begin_ptr)), begin_ptr, src_endpoint));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketReceiverMC::RecvFrom(std::string &packet_data,
	boost::asio::ip::udp::endpoint &src_endpoint)
{
	char packet_buffer[0xFFFF + 1];

	std::size_t packet_size = RecvFrom(sizeof(packet_buffer), packet_buffer,
		src_endpoint);

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
std::size_t PacketReceiverMC::RecvFrom(
	const boost::asio::mutable_buffers_1 &packet_buffer)
{
	boost::asio::ip::udp::endpoint src_endpoint;

	return(RecvFrom(packet_buffer, src_endpoint));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketReceiverMC::RecvFrom(std::size_t packet_length,
	void *packet_ptr)
{
	boost::asio::ip::udp::endpoint src_endpoint;

	return(RecvFrom(packet_length, packet_ptr, src_endpoint));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketReceiverMC::RecvFrom(void *begin_ptr, void *end_ptr)
{
	boost::asio::ip::udp::endpoint src_endpoint;

	return(RecvFrom(begin_ptr, end_ptr, src_endpoint));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketReceiverMC::RecvFrom(std::string &packet_data)
{
	boost::asio::ip::udp::endpoint src_endpoint;

	return(RecvFrom(packet_data, src_endpoint));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketReceiverMC::SetRecvBufferSize(std::size_t buffer_size)
{
	try {
		boost::asio::socket_base::receive_buffer_size option(
			static_cast<int>(buffer_size));
		socket_.set_option(option);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to set the socket receive buffer "
			"size to " + MLB::Utility::AnyToString(buffer_size) + " failed: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketReceiverMC::GetRecvBufferSize() const
{
	std::size_t buffer_size = 0;

	try {
		boost::asio::socket_base::receive_buffer_size option;
		socket_.get_option(option);
		int tmp_buffer_size = option.value();
		buffer_size = static_cast<std::size_t>(tmp_buffer_size);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to get the socket receive buffer "
			"size failed: " + std::string(except.what()));
	}

	return(buffer_size);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketReceiverMC::FixUpConstruction()
{
	const char *action_name;

	try {
		action_name = "open the socket";
		socket_.open(endpoint_.protocol());
		action_name = "set socket open for address reuse";
		socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
		action_name = "bind the socket to the local endpoint";
#if __linux__
		socket_.bind(boost::asio::ip::udp::endpoint(my_address_,
			endpoint_.port()));
#else
/*
		CODE NOTE: Original code. To be removed.
		socket_.bind(endpoint_);
*/
		boost::asio::ip::udp::socket::endpoint_type
			bind_endpoint(host_interface_, endpoint_.port());
		socket_.bind(bind_endpoint);
#endif // #if __linux__
/*
		CODE NOTE: Original code. To be removed.
		action_name = "join the multicast group";
      socket_.set_option(boost::asio::ip::multicast::join_group(
			my_address_.to_v4(), endpoint_.address().to_v4()));
*/
		action_name = "join the multicast group";
      socket_.set_option(boost::asio::ip::multicast::join_group(
			my_address_.to_v4(), host_interface_.to_v4()));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to complete construction of an "
			"instance of 'PacketReceiverMC' failed: Attempt to " +
			std::string(action_name) + " failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		std::string ip_address("239.255.1.46");
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
		boost::asio::io_service io_service;
		PacketReceiverMC        test_receiver(io_service, ip_address, ip_port,
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

