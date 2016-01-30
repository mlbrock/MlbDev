//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Test Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Server test program for MLB::SockLibX.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SockLibX.hpp>

//	////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

# include <iostream>

#include <SockLibX/SocketX.hpp>
#include <SockLibX/ParseCmdLineArg.hpp>
#include <Utility/CriticalEventHandler.hpp>

using namespace MLB::SockLibX;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	std::string port("6666");

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -port <port-number> ( = " << port << " ) ]" <<
			std::endl;
		exit(EXIT_SUCCESS);
	}

	int return_code = EXIT_SUCCESS;

	try {
		unsigned int count_1;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (SockLibXParseCmdLineArg::ParseCmdLineServiceResolved(count_1,
				argc, argv, port))
				;
			else
				SockLibXParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		MLB::Utility::CriticalEventContext critical_event_context;
		SockLibXContext                    socket_lib;
		std::cout << "Attempting to start the server on port " << port <<
			std::flush;
		SocketX my_socket(AddressFamily_Internet, SocketType_Stream,
			ProtocolType_DEFAULT);
		NativeSockAddr_In this_address;
		memset(&this_address, '\0', sizeof(this_address));
		this_address.sin_family      = AF_INET;
		this_address.sin_port        = htons(((unsigned short) atoi(port.c_str())));
		this_address.sin_addr.s_addr = INADDR_ANY;
		my_socket.Bind(reinterpret_cast<const NativeSockAddr *>(&this_address),
			sizeof(this_address));
		std::cout << " --- listening for connections..." << std::endl;
		my_socket.Listen(10);
		MLB::Utility::TimeVal wait_time(0, 100 * 1000);	//	Sleep up to 100 ms.
		for ( ; ; ) {
			while ((!my_socket.SelectRead(&wait_time)) &&
				(!MLB::Utility::CriticalEventTest()))
				;
			if (MLB::Utility::CriticalEventTest())
				break;
			bool accept_flag = true;
			while (accept_flag && (!MLB::Utility::CriticalEventTest())) {
				NativeSocketHandle client_handle = NPSL_INVALID_SOCKET;
				NativeSockAddr     client_addr;
				NativeSockLen_T    client_addrlen;
				client_addrlen = sizeof(client_addr);
				client_handle = my_socket.Accept(&client_addr, &client_addrlen);
				SocketX     client_socket(client_handle, client_addr);
				EndPointIP  client_endpoint(client_addr);
				std::string client_name(client_endpoint.ToString());
				std::cout << "*[" << client_name <<
					"]: *** Connection from client." << std::endl;
				client_socket.SetBlockingModeOff();
				bool recv_flag = true;
				MLB::Utility::TimeVal expiration_time;
				expiration_time.tv_sec += 10;
				struct timeval recv_time_out = { 0, 100 * 1000 };
				while (recv_flag && (!MLB::Utility::CriticalEventTest())) {
					try {
						if (client_socket.SelectWaitOnRead(&recv_time_out)) {
							std::string recv_buffer;
							if (client_socket.Recv(recv_buffer, 32767)) {
								std::cout << " [" << client_name << "]: " <<
									recv_buffer;
								if (recv_buffer[recv_buffer.size() - 1] != '\n')
									std::cout << std::endl;
								expiration_time         = MLB::Utility::TimeVal();
								expiration_time.tv_sec += 10;
							}
							else {
								std::cout << "*[" << client_name << "]: " <<
									"*** Client closed socket --- disconnecting." <<
									std::endl;
								recv_flag = false;
							}							
						}
						else if (expiration_time <= MLB::Utility::TimeVal()) {
							std::cout << "*[" << client_name << "]: " <<
								"*** Client idle time expiration --- disconnecting." <<
								std::endl;
							recv_flag = false;
						}
						else
							Sleep(10);
					}
					catch (const std::exception &except) {
						std::cout << "*[" << client_name << "]: " <<
							"*** Client error: " << except.what() << std::endl;
						recv_flag = false;
					}
				}
			}
		}
		if (MLB::Utility::CriticalEventTest())
			std::cout << "Exiting because a signal was received." << std::endl;
	}
	catch (std::exception &except) {
		std::cerr << std::endl << "MLB::SockLibX::Socket error: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << "Test completed." << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

