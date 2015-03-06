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

#include <SockLibX.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/scoped_array.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

# include <iostream>

#include <SockLibX/SocketX.hpp>
#include <SockLibX/ParseCmdLineArg.hpp>
#include <Utility/CriticalEventHandler.hpp>

using namespace MLB::SockLibX;

//	////////////////////////////////////////////////////////////////////////////
const unsigned int TEST_RecvSize = 2 << 20;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	std::string  port("6666");
	unsigned int echo_flag = false;

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -port <port-number> ( = " << port << " ) ] " <<
			"[ -echo <boolean> ( = false ) ]" <<
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
			else if (SockLibXParseCmdLineArg::ParseCmdLineDatum("-ECHO",
				count_1, argc, argv, echo_flag))
				;
			else
				SockLibXParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		MLB::Utility::CriticalEventContext critical_event_context;
		SockLibXContext                    socket_lib;
		std::cout << "Attempting to start the server on port " << port <<
			"." << std::endl;
		SocketX my_socket(AddressFamily_Internet, SocketType_Stream,
			ProtocolType_DEFAULT);
		NativeSockAddr_In this_address;
		memset(&this_address, '\0', sizeof(this_address));
		this_address.sin_family      = AF_INET;
		this_address.sin_port        = htons(((unsigned short) atoi(port.c_str())));
		this_address.sin_addr.s_addr = INADDR_ANY;
		my_socket.Bind(reinterpret_cast<const NativeSockAddr *>(&this_address),
			sizeof(this_address));
		my_socket.Listen(10);
		MLB::Utility::TimeVal wait_time(0, 100 * 1000);	//	Sleep up to 100 ms.
		for ( ; ; ) {
			std::cout << "Listening for connections on port " << port <<
				"..." << std::endl;
			while ((!my_socket.SelectRead(&wait_time)) &&
				(!MLB::Utility::CriticalEventTest()))
				;
			if (MLB::Utility::CriticalEventTest())
				break;
			bool accept_flag = true;
			while (accept_flag && (!MLB::Utility::CriticalEventTest())) {
				std::cout << std::endl << "Listening for connections on port " <<
					port << "..." << std::endl;
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
//				client_socket.SetRecvBufferSize(2 << 19);
//				client_socket.SetSendBufferSize(2 << 19);
//				client_socket.SetBlockingModeOff();
				bool recv_flag = true;
				MLB::Utility::TimeVal expiration_time;
				expiration_time.tv_sec += 10;
				struct timeval     recv_time_out = { 0, 1 };
				unsigned long long total_bytes   = 0;
				unsigned int       loop_count    = 0;
				unsigned int       tmp_bytes;
				boost::scoped_array<char> recv_buffer(new char[TEST_RecvSize]);
				while (recv_flag && (!MLB::Utility::CriticalEventTest())) {
					try {
						if (client_socket.SelectWaitOnRead(&recv_time_out)) {
						   tmp_bytes    = client_socket.Recv(recv_buffer.get(), TEST_RecvSize);
							total_bytes += tmp_bytes;
							loop_count++;
							if (!(loop_count % 100)) {
								std::cout << "Received " << std::setw(10) <<
									tmp_bytes << " bytes --- total = " <<
									std::setw(22) << total_bytes << std::endl;
								loop_count = 0;
							}
							struct timeval send_time_out = { 0, 1 };
							while (!client_socket.SelectWaitOnWrite(&send_time_out)) {
								if (MLB::Utility::CriticalEventTest()) {
									std::cout << "Exiting because a signal was "
										"received." << std::endl;
									return(0);
								}
								::Sleep(0);
							}
							client_socket.Send(recv_buffer.get(), tmp_bytes);
							expiration_time.tv_sec += 10;
						}
/*
						else {
							std::cout << "*[" << client_name << "]: " <<
								"*** Client closed socket --- disconnecting." <<
								std::endl;
								recv_flag = false;
						}
*/
						else if (expiration_time <= MLB::Utility::TimeVal()) {
							std::cout << "*[" << client_name << "]: " <<
								"*** Client idle time expiration --- disconnecting." <<
								std::endl;
							recv_flag = false;
						}
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

