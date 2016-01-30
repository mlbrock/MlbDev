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

#include <SockLibX/SocketX.hpp>

//	////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

# include <iostream>

#include <SockLibX/ParseCmdLineArg.hpp>

using namespace MLB::SockLibX;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	std::string  host("localhost");
	std::string  port("6666");
	unsigned int send_count    = 1;
	unsigned int sleep_ms      = 0;
	unsigned int post_sleep_ms = 5000;

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -host <host-name> ( = " << host << " ) ] " <<
			"[ -port <port-number> ( = " << port << " ) ] " <<
			"[ -send_count <send-count> ( = " << send_count << " ) ] " <<
			"[ -sleep_time <sleep-milliseconds> ( = " << sleep_ms << " ) ] " <<
			"[ -post_sleep_time <sleep-milliseconds> ( = " << sleep_ms << " ) ]" <<
			std::endl;
		exit(EXIT_SUCCESS);
	}

	int return_code = EXIT_SUCCESS;

	try {
		SockLibXContext socket_lib;
		unsigned int    count_1;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (SockLibXParseCmdLineArg::ParseCmdLineHostResolved(count_1, argc,
				argv, host))
				;
			else if (SockLibXParseCmdLineArg::ParseCmdLineServiceResolved(count_1,
				argc, argv, port))
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
		for (count_1 = 0; count_1 < send_count; ++count_1) {
			my_socket.Send("status " + MLB::Utility::GetHostNameCanonical() +
				".CatsRvTrafficMonitor &green " +
				MLB::Utility::TimeTM::TimeLocal().AscTimeSane() +
				" All quiet on the Polaris front!");
			::Sleep(sleep_ms);
		}
		::Sleep(post_sleep_ms);
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

