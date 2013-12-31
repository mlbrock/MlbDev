// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the portable functions to set all
								process resource limits to their maximum values.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/ResourceLimits.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __MSDOS__
#elif _Windows
#else
# include <sys/resource.h>
#endif // #ifdef __MSDOS__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
#ifdef __MSDOS__
#elif _Windows
#else
typedef struct {
	const char *name;
	int         selector;
	const char *selector_name;
} RLIMIT_DEF;
# define POPULATE_DATA(rlimit_text, rlimit_name)	\
	{ rlimit_text, rlimit_name, #rlimit_name }
# if __SVR4
static const RLIMIT_DEF RLimitsList[] = {
	POPULATE_DATA("maximum core file size", 			RLIMIT_CORE),
	POPULATE_DATA("maximum cpu milliseconds",			RLIMIT_CPU),
	POPULATE_DATA("maximum data size", 					RLIMIT_DATA),
	POPULATE_DATA("maximum file size", 					RLIMIT_FSIZE),
	POPULATE_DATA("maximum descriptor count", 		RLIMIT_NOFILE),
	POPULATE_DATA("maximum stack size", 				RLIMIT_STACK),
	POPULATE_DATA("maximum mapped address size", 	RLIMIT_VMEM)
};
# else
static const RLIMIT_DEF RLimitsList[] = {
	POPULATE_DATA("maximum cpu milliseconds",			RLIMIT_CPU),
	POPULATE_DATA("maximum file size", 					RLIMIT_FSIZE),
	POPULATE_DATA("maximum data size", 					RLIMIT_DATA),
	POPULATE_DATA("maximum stack size", 				RLIMIT_STACK),
	POPULATE_DATA("maximum core file size", 			RLIMIT_CORE),
	POPULATE_DATA("maximum resident set size", 		RLIMIT_RSS),
	POPULATE_DATA("maximum descriptor count", 		RLIMIT_NOFILE)
};
# endif // # if __SVR4
#endif // #ifdef __MSDOS__
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SetResourceLimitsToMax()
{
#ifdef __MSDOS__
#elif _Windows
#else
	struct rlimit rlimit_data;

	unsigned int count_1;
	for (count_1 = 0; count_1 < (sizeof(RLimitsList) / sizeof(RLimitsList[0]));
		++count_1) {
		// ////////////////////////////////////////////////////////////////
		//	Get the current soft and hard limits...
		if (::getrlimit(RLimitsList[count_1].selector, &rlimit_data)) {
			std::ostringstream error_text;
			error_text << "Attempt to get the operating system resource limit " <<
				RLimitsList[count_1].selector << " (" <<
				RLimitsList[count_1].selector_name << " = '" <<
				RLimitsList[count_1].name << "') failed: ";
			ThrowErrno(error_text);
		}
		// ////////////////////////////////////////////////////////////////
		//	First try to set the 'soft' resource limit to its 'hard'
		//	(or maximum) value . . .
		rlimit_data.rlim_cur = rlimit_data.rlim_max;
		if (::setrlimit(RLimitsList[count_1].selector, &rlimit_data)) {
			std::ostringstream error_text;
			error_text <<
				"Attempt to change the operating system resource limit " <<
				RLimitsList[count_1].selector << " (" <<
				RLimitsList[count_1].selector_name << " = '" <<
				RLimitsList[count_1].name << "') from its soft limit of " <<
				static_cast<unsigned int>(rlimit_data.rlim_cur) <<
				"to its hard of limit of " <<
				static_cast<unsigned int>(rlimit_data.rlim_max) << " failed: ";
			ThrowErrno(error_text);
		}
		// ////////////////////////////////////////////////////////////////
		/*
			Now try to set both the 'soft' and 'hard' limits to their
			maximum values --- just in case the process is executing with
			'root' privilege.

			Note that we ignore the return status from 'setrlimit()' just
			in case the process isn't running as 'root'.
		*/
		rlimit_data.rlim_cur = RLIM_INFINITY;
		rlimit_data.rlim_max = RLIM_INFINITY;
		::setrlimit(RLimitsList[count_1].selector, &rlimit_data);
	}
#endif // #ifdef __MSDOS__
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <iostream>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << "Calling SetResourceLimitsToMax()..." << std::endl;
		SetResourceLimitsToMax();
		std::cout << "...done!" << std::endl;
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

