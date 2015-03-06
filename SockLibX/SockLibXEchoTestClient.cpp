//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Test Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Client test program for MLB::SockLibX.

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
	std::string  host("localhost");
	std::string  port("6666");
	unsigned int send_count    = 1;
	unsigned int sleep_ms      = 0;
	unsigned int post_sleep_ms = 0;
	unsigned int send_size     = 32767;

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -host <host-name> ( = " << host << " ) ] " <<
			"[ -port <port-number> ( = " << port << " ) ] " <<
			"[ -size <buffer-size> ( = " << send_size << " ) ] " <<
			"[ -send_count <send-count> ( = " << send_count << " ) ] " <<
			"[ -sleep_time <sleep-milliseconds> ( = " << sleep_ms << " ) ] " <<
			"[ -post_sleep_time <sleep-milliseconds> ( = " << sleep_ms << " ) ]" <<
			std::endl;
		exit(EXIT_SUCCESS);
	}

	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		SockLibXContext                    socket_lib;
		unsigned int                       count_1;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (SockLibXParseCmdLineArg::ParseCmdLineHostResolved(count_1, argc,
				argv, host))
				;
			else if (SockLibXParseCmdLineArg::ParseCmdLineServiceResolved(count_1,
				argc, argv, port))
				;
			else if (SockLibXParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(
				"-SIZE", count_1, argc, argv, send_size, 1))
				;
			else if (SockLibXParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(
				"-SEND_COUNT", count_1, argc, argv, send_count, 1))
				;
			else if (SockLibXParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(
				"-SLEEP_TIME", count_1, argc, argv, sleep_ms, 0))
				;
			else if (SockLibXParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(
				"-POST_SLEEP_TIME", count_1, argc, argv, post_sleep_ms, 0))
				;
			else
				SockLibXParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		std::cout << "Attempting to connect...";
		SocketX my_socket(EndPointIP(AddressIP(host), port));
		std::cout << " connected to " <<
			EndPointIP(AddressIP(host), port).ToString() << std::endl;
//		my_socket.SetRecvBufferSize(2 << 19);
//		my_socket.SetSendBufferSize(2 << 19);
		boost::scoped_array<char> send_buffer(new char[send_size]);
		boost::scoped_array<char> recv_buffer(new char[TEST_RecvSize]);
//		struct timeval            recv_time_out = { 0, 100 * 1000 };
		struct timeval            recv_time_out = { 0, 1 };
		unsigned long long        send_bytes    = 0;
		unsigned long long        recv_bytes    = 0;
		unsigned long long        total_bytes   = 0;
		unsigned int              loop_count    = 0;
		unsigned int tmp_bytes;
		memset(send_buffer.get(), '?', send_size);
std::cout << "SEND COUNT = " << send_count << std::endl;
//		for (count_1 = 0; count_1 < send_count; ++count_1) {
for ( ; ; ) {
			if (MLB::Utility::CriticalEventTest())
				MLB::Utility::ThrowException("Critical event detected.");
			//	Send a bunch o' data...
//			while (!my_socket.SelectWaitOnWrite(&recv_time_out))
//				::Sleep(1);
			if (my_socket.SelectWaitOnWrite(&recv_time_out)) {
				tmp_bytes   = my_socket.Send(send_buffer.get(), send_size);
				send_bytes += tmp_bytes;
/*
				std::cout << "Sent     " << std::setw(10) << tmp_bytes <<
					" bytes --- total = " << std::setw(22) << send_bytes <<
					std::endl;
*/
			}
			::Sleep(sleep_ms);
			if (MLB::Utility::CriticalEventTest())
				MLB::Utility::ThrowException("Critical event detected.");
			//	Get and discard any returned data...
			if (my_socket.SelectWaitOnRead(&recv_time_out)) {
				tmp_bytes   = my_socket.Recv(recv_buffer.get(), TEST_RecvSize);
				recv_bytes += tmp_bytes;
			}
			loop_count++;
			if (!(loop_count % 100)) {
				std::cout << "Received " << std::setw(10) << tmp_bytes <<
					" bytes --- total = " << std::setw(22) << recv_bytes <<
					std::endl;
				loop_count = 0;
			}
		}
		::Sleep(post_sleep_ms);
		while (send_bytes > recv_bytes) {
			if (my_socket.SelectWaitOnRead(&recv_time_out)) {
				tmp_bytes   = my_socket.Recv(recv_buffer.get(), TEST_RecvSize);
				recv_bytes += tmp_bytes;
				std::cout << "Received " << std::setw(10) << tmp_bytes <<
					" bytes --- total = " << std::setw(22) << recv_bytes <<
					std::endl;
			}
			::Sleep(sleep_ms + 1);
		}
		my_socket.Disconnect();
		my_socket.Close();
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

