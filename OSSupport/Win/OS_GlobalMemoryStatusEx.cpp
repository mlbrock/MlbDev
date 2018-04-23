// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of wrappers to the Windows function
								GlobalMemoryStatusEx().

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport/Win/OSSupport_Win.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

//	////////////////////////////////////////////////////////////////////////////
MEMORYSTATUSEX &OS_GlobalMemoryStatusEx(MEMORYSTATUSEX &datum)
{
	datum.dwLength = sizeof(datum);

	if (::GlobalMemoryStatusEx(&datum) == 0)
		MLB::Utility::ThrowSystemError("Call to 'GlobalMemoryStatusEx()' "
			"failed.");

	return(datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MEMORYSTATUSEX OS_GlobalMemoryStatusEx()
{
	MEMORYSTATUSEX datum;

	return(OS_GlobalMemoryStatusEx(datum));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		MEMORYSTATUSEX datum(OS_GlobalMemoryStatusEx());
		std::cout << "Test OS_GlobalMemoryStatusEx(): " << std::endl;
		std::cout << "   dwLength                : " <<
			datum.dwLength                            << std::endl;
		std::cout << "   dwMemoryLoad            : " <<
			datum.dwMemoryLoad                        << std::endl;
		std::cout << "   ullTotalPhys            : " <<
			datum.ullTotalPhys                        << std::endl;
		std::cout << "   ullAvailPhys            : " <<
			datum.ullAvailPhys                        << std::endl;
		std::cout << "   ullTotalPageFile        : " <<
			datum.ullTotalPageFile                    << std::endl;
		std::cout << "   ullAvailPageFile        : " <<
			datum.ullAvailPageFile                    << std::endl;
		std::cout << "   ullTotalVirtual         : " <<
			datum.ullTotalVirtual                     << std::endl;
		std::cout << "   ullAvailVirtual         : " <<
			datum.ullAvailVirtual                     << std::endl;
		std::cout << "   ullAvailExtendedVirtual : " <<
			datum.ullAvailExtendedVirtual             << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GlobalMemoryStatusEx.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

