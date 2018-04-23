// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the portable process id functions.

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

#include <Utility.hpp>

#if defined(__unix__)
# include <unistd.h>
#endif // #if defined(__unix__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
ProcessId CurrentProcessId()
{
#ifdef __MSDOS__
	//	CODE NOTE: Fix me! --- Should return the PSP value.
	return(static_cast<ProcessId>(4096));
#elif _Windows
	return(static_cast<ProcessId>(::GetCurrentProcessId()));
#else
	return(static_cast<ProcessId>(::getpid()));
#endif // #ifdef __MSDOS__
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <cstdlib>
#include <iostream>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << "CurrentProcessId(): " << CurrentProcessId() << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << std::endl;
		std::cout << "Regression test error: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

