// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for LSF functionality 'ls_unlockhost()'.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LSFWrapper/LSFWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

//	////////////////////////////////////////////////////////////////////////////
int LSF_unlockhost(bool ignore_applicability_flag)
{
	int result;

	if ((result = ls_unlockhost()) < 0) {
		if ((result == -1) && (lserrno == LSE_LIM_ALOCKED) &&
			ignore_applicability_flag)
			return(0);
		std::ostringstream error_text;
		error_text << "Invocation of 'ls_lockhost()' failed with a negative "
			"return code (" << result << ").";
		throw LSFExceptionStatus(lserrno, error_text);
	}

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UnlockHost(bool ignore_applicability_flag)
{
	LSF_unlockhost(ignore_applicability_flag);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#include <iomanip>

#include <Utility/Sleep.hpp>
#include <Utility/CriticalEventHandler.hpp>

#ifdef TEST_MAIN

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
static void TEST_UnlockHost(unsigned int lock_seconds)
{
	LockHost(static_cast<time_t>(lock_seconds));

	std::cout << "Locked Platform/LSF host '" << MLB::Utility::GetHostName() <<
		"' for " << lock_seconds << " seconds..." << std::flush;

	time_t end_time = time(NULL) + lock_seconds;

	while (!MLB::Utility::CriticalEventTest()) {
		MLB::Utility::SleepMilliSecs(100);
		if (time(NULL) >= end_time)
			break;
		std::cout << " Unlocking early..." << std::flush;
		UnlockHost();
	}

	std::cout << " done." << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code           = EXIT_SUCCESS;
	unsigned int lock_seconds = 60;

	std::cout << "Test of function 'UnlockHost()'" << std::endl;
	std::cout << "---- -- -------- --------------" << std::endl;

	try {
		TEST_UnlockHost(lock_seconds);
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

