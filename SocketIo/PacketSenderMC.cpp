// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the PacketSenderMC class.

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

#include <SocketIo/PacketSenderMC.hpp>
#include <SocketIo/ResolveService.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
PacketSenderMC::PacketSenderMC(boost::asio::io_service &io_service,
	const std::string &mc_group, IpPortType ip_port,
	const std::string &host_interface)
	:endpoint_(StringToIpAddress(mc_group), ResolveService(ip_port))
	,socket_(io_service, endpoint_.protocol())
{
	if (!host_interface.empty())
		socket_.set_option(boost::asio::ip::multicast::outbound_interface(
			StringToIpAddressV4(host_interface)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderMC::PacketSenderMC(boost::asio::io_service &io_service,
	const std::string &mc_group, const std::string &ip_port,
	const std::string &host_interface)
	:endpoint_(StringToIpAddress(mc_group), ResolveService(ip_port))
	,socket_(io_service, endpoint_.protocol())
{
	if (!host_interface.empty())
		socket_.set_option(boost::asio::ip::multicast::outbound_interface(
			StringToIpAddressV4(host_interface)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderMC::PacketSenderMC(boost::asio::io_service &io_service,
	const SocketSpec &socket_spec)
	:endpoint_(socket_spec.ip_address_, socket_spec.ip_port_)
	,socket_(io_service, endpoint_.protocol())
{
	if (!(socket_spec.local_interface_ == boost::asio::ip::address()))
		socket_.set_option(boost::asio::ip::multicast::outbound_interface(
			socket_spec.local_interface_.to_v4()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketSenderMC::~PacketSenderMC()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketSenderMC::SendTo(
	const boost::asio::const_buffers_1 &packet_buffer)
{
	return(socket_.send_to(packet_buffer, endpoint_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketSenderMC::SendTo(std::size_t packet_length,
	const void *packet_ptr)
{
	if (packet_length < 1)
		MLB::Utility::ThrowInvalidArgument("The length of the data in the "
			"packet to be sent is specified by a length of zero.");
	else if (packet_length > std::numeric_limits<MsgLengthType>::max())
		MLB::Utility::ThrowInvalidArgument("The length of the data in the "
			"packet to be sent is specified by a length (" +
			MLB::Utility::AnyToString(packet_length) + ") which exceeds the "
			"maximum permissible (" + MLB::Utility::AnyToString(
			std::numeric_limits<MsgLengthType>::max()) + ").");

	return(SendTo(boost::asio::buffer(packet_ptr, packet_length)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketSenderMC::SendTo(const void *begin_ptr,
	const void *end_ptr)
{
	if (begin_ptr >= end_ptr)
		MLB::Utility::ThrowInvalidArgument("The length of the data in the "
			"packet to be sent is specified by a begin pointer (" +
			MLB::Utility::ValueToStringHex(begin_ptr) + ") which is not less "
			"than the end pointer (" + MLB::Utility::ValueToStringHex(end_ptr) + 
			").");

	return(SendTo(static_cast<std::size_t>(
		static_cast<const char *>(end_ptr) -
		static_cast<const char *>(begin_ptr)), begin_ptr));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketSenderMC::SendTo(const std::string &packet_data)
{
	return(SendTo(packet_data.size(), packet_data.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketSenderMC::SetSendBufferSize(std::size_t buffer_size)
{
	try {
		boost::asio::socket_base::send_buffer_size option(
			static_cast<int>(buffer_size));
		socket_.set_option(option);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to set the socket send buffer "
			"size to " + MLB::Utility::AnyToString(buffer_size) + " failed: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PacketSenderMC::GetSendBufferSize() const
{
	std::size_t buffer_size = 0;

	try {
		boost::asio::socket_base::send_buffer_size option;
		socket_.get_option(option);
		int tmp_buffer_size = option.value();
		buffer_size = static_cast<std::size_t>(tmp_buffer_size);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to get the socket send buffer "
			"size failed: " + std::string(except.what()));
	}

	return(buffer_size);
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
		PacketSenderMC          packet_sender(io_service, ip_address, ip_port,
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

