//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for class ServerX.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/Sleep.hpp>
#include <Utility/CriticalEventHandler.hpp>

#include <SockLibX/ServerX.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
ServerX::ServerX(SocketX &socket, unsigned int listen_backlog)
	:endpoint_(socket.GetSockName())
	,listen_backlog_((listen_backlog) ? listen_backlog : SOMAXCONN)
	,socket_(socket)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServerX::ServerX(const AddressIP &server_endpoint, int port,
	unsigned int listen_backlog, AddressFamily address_family,
	SocketType socket_type, ProtocolType protocol_type)
	:endpoint_(server_endpoint, port)
	,listen_backlog_((listen_backlog) ? listen_backlog : SOMAXCONN)
	,socket_(address_family, socket_type, protocol_type)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServerX::ServerX(const AddressIP &server_endpoint, const char *port,
	unsigned int listen_backlog, AddressFamily address_family,
	SocketType socket_type, ProtocolType protocol_type)
	:endpoint_(server_endpoint, port)
	,listen_backlog_((listen_backlog) ? listen_backlog : SOMAXCONN)
	,socket_(address_family, socket_type, protocol_type)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServerX::ServerX(const AddressIP &server_endpoint, const std::string &port,
	unsigned int listen_backlog, AddressFamily address_family,
	SocketType socket_type, ProtocolType protocol_type)
	:endpoint_(server_endpoint, port)
	,listen_backlog_((listen_backlog) ? listen_backlog : SOMAXCONN)
	,socket_(address_family, socket_type, protocol_type)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServerX::ServerX(int port, unsigned int listen_backlog,
	AddressFamily address_family, SocketType socket_type,
	ProtocolType protocol_type)
	:endpoint_(EndPointIP(port, address_family))
	,listen_backlog_((listen_backlog) ? listen_backlog : SOMAXCONN)
	,socket_(address_family, socket_type, protocol_type)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServerX::ServerX(const char *port, unsigned int listen_backlog,
	AddressFamily address_family, SocketType socket_type,
	ProtocolType protocol_type)
	:endpoint_(EndPointIP(port, address_family))
	,listen_backlog_((listen_backlog) ? listen_backlog : SOMAXCONN)
	,socket_(address_family, socket_type, protocol_type)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServerX::ServerX(const std::string &port, unsigned int listen_backlog,
	AddressFamily address_family, SocketType socket_type,
	ProtocolType protocol_type)
	:endpoint_(EndPointIP(port, address_family))
	,listen_backlog_((listen_backlog) ? listen_backlog : SOMAXCONN)
	,socket_(address_family, socket_type, protocol_type)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SocketX &ServerX::GetSocketRef()
{
	return(socket_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ServerX::Accept(SocketX &client_socket, unsigned int max_wait_usecs)
{
	MLB::Utility::TimeVal time_out(
		max_wait_usecs / 1000000, max_wait_usecs % 1000000);

	if (socket_.SelectRead(&time_out))
		return(AcceptInternal(client_socket));

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ServerX::WaitForAccept(SocketX &client_socket,
	const MLB::Utility::TimeVal &wait_until_time, unsigned int cycle_usecs,
	volatile MLB::Utility::AtomicFlag *cycle_interrupt_flag_ptr)
{
	if (ShouldTryAccept(cycle_interrupt_flag_ptr)) {
		//	Always try the accept() at least once, even when the time has passed...
		if (MLB::Utility::TimeVal() <= wait_until_time)
			return(Accept(client_socket, 1));
		MLB::Utility::TimeVal current_time;
		while (ShouldTryAccept(cycle_interrupt_flag_ptr) &&
			(current_time > wait_until_time)) {
			double ticks_to_wait =
				MLB::Utility::TimeVal::GetDifferenceTicksDouble(wait_until_time,
					current_time);
			ticks_to_wait = (ticks_to_wait < 0.0) ? 1.0 : ticks_to_wait;
			if (Accept(client_socket,
				static_cast<unsigned int>(std::min(static_cast<double>(cycle_usecs),
				ticks_to_wait))))
				return(true);
			MLB::Utility::YieldThreadQuantum();
			MLB::Utility::TimeVal().swap(current_time);
		}
	}

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ServerX::ShouldTryAccept(
	volatile MLB::Utility::AtomicFlag *cycle_interrupt_flag_ptr) const
{
	return((!MLB::Utility::CriticalEventTest()) &&
		((cycle_interrupt_flag_ptr == NULL) ||
		(!MLB::Utility::AtomicFlagTest(cycle_interrupt_flag_ptr))));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ServerX::ShouldTryAccept() const
{
	return(!MLB::Utility::CriticalEventTest());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ServerX::Initialize()
{
	socket_.Bind(endpoint_);
	socket_.Listen(listen_backlog_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ServerX::AcceptInternal(SocketX &client_socket)
{
	socket_.Accept(client_socket);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#ifdef TEST_MAIN

# include <cstdlib>
# include <iostream>

#include <Utility/Sleep.hpp>

using namespace MLB::SockLibX;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int         return_code = EXIT_SUCCESS;
	const char *port;

	port = (argc > 1) ? argv[1] : "7777";

	try {
		SockLibXContext socket_lib;
		ServerX         this_server(port);
		SocketX         my_client;
		if (!atoi(port))
			std::cout << "Server port assigned by system." << std::endl;
		std::cout << "Server listening on port " <<
			this_server.GetSocketRef().GetSockName().GetPort() << "..." <<
			std::flush;
		while (this_server.ShouldTryAccept() &&
			(!this_server.Accept(my_client, 100000)))
			MLB::Utility::SleepSecs(0);
		if (MLB::Utility::CriticalEventTest()) 
			std::cout << std::endl << "Exiting because a signal was received." <<
				std::endl;
		else {
			std::cout << " done!!!" << std::endl;
			std::cout << "Local  endpoint: [" << my_client.GetSockName() << "]" <<
				std::endl;
			std::cout << "Remote endpoint: [" << my_client.GetPeerName() << "]" <<
				std::endl;
		}
	}
	catch (std::exception &except) {
		std::cout << std::endl << "MLB::SockLibX::ServerX error: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << "Test completed." << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

