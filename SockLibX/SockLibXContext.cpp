//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for class SockLibXContext.

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

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
SockLibXContext::SockLibXContext()
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_SocketLibStartUp(NULL, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SockLibXContext::~SockLibXContext()
{
	NPSL_SocketLibCleanUp(NULL, NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SockLibXContext::SockLibXContext(const SockLibXContext &)
{
	//	Copy ctor requires a second open for the lvalue as rvalue is dtored.
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_SocketLibStartUp(NULL, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SockLibXContext::IsReady() const
{
	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SockLibXContext::EnsureReady() const
{
	if (!IsReady())
		throw SockLibXException("Socket library not yet initialized.");
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#ifdef TEST_MAIN

# include <cstdlib>
# include <iostream>

#include <SockLibX/SocketX.hpp>

using namespace MLB::SockLibX;


//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	unsigned int  return_code = EXIT_SUCCESS;
	const char   *host;
	const char   *port;

	host = (argc > 1) ? argv[1] : "localhost";
//	port = (argc > 2) ? argv[2] : "8500";
//	port = (argc > 2) ? argv[2] : "7500";
	port = (argc > 2) ? argv[2] : "23";
//	port = (argc > 2) ? argv[2] : "smtp";

	try {
		std::cout << "Creating SockLibXContext..." << std::flush;
		{
			SockLibXContext socket_lib;
			std::cout << "...Creation complete." << std::endl;
			{
				SocketX my_socket(EndPointIP(AddressIP(host), port));
				std::cout << "Connected to " <<
					EndPointIP(AddressIP(host), port).ToString() << std::endl;
			}
			std::cout << "Destroying SockLibXContext..." << std::flush;
		}
		std::cout << "...destruction complete." << std::endl;
	}
	catch (std::exception &except) {
		std::cout << std::endl << "MLB::SockLibX::Socket error: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << "Test completed." << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

