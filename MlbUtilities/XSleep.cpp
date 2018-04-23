// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Performs a 'sleep' with up to nanosecond granularity.

	Revision History	:	1986-02-15 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1986 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Sleep.hpp>
#include <Utility/ParseCmdLineArg.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace {
// ////////////////////////////////////////////////////////////////////////////
unsigned long XSleep_GetTime(int argc, char **argv, int argv_index)
{
	double tmp_double;

	if (argc <= argv_index)
		return(1L);
	else if ((tmp_double = std::atof(argv[argv_index])) < 0.0)
		return(0L);
	else if (tmp_double >
		static_cast<double>(std::numeric_limits<unsigned long>::max()))
		return(std::numeric_limits<unsigned long>::max());

	return(static_cast<unsigned long>(tmp_double));
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -h | -help ] " <<
			"[ [ -s ] <sleep-seconds> ] " <<
			"[ -k <sleep-milliseconds> ] " <<
			"[ -u <sleep-microseconds> ] " <<
			"[ -n <sleep-nanoseconds> ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		if (argc == 1)
			SleepSecs(1);
		else if (isdigit(argv[1][0]))
			SleepSecs(XSleep_GetTime(argc, argv, 1));
		else if (!stricmp(argv[1], "-s"))
			SleepSecs(XSleep_GetTime(argc, argv, 2));
		else if (!stricmp(argv[1], "-k"))
			SleepMilliSecs(XSleep_GetTime(argc, argv, 2));
		else if (!stricmp(argv[1], "-u"))
			SleepMicroSecs(XSleep_GetTime(argc, argv, 2));
		else if (!stricmp(argv[1], "-n"))
			SleepNanoSecs(XSleep_GetTime(argc, argv, 2));
		else
			ThrowInvalidArgument("ERROR: Invalid command line --- use '-h' for "
				"help.");
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << argv[0] << ": fatal error encountered: " <<
			except.what() << std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

