// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Program
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Resolves path names specified on the command line to
								their generic equivalents.

	Revision History	:	2001-10-17 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/PathName.hpp>
#include <Utility/ParseCmdLineArg.hpp>

#include <iomanip>

// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code = EXIT_SUCCESS;
	std::string  my_name     = MLB::Utility::GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -continue ] " <<
			"<path-pattern> [ <path-pattern> ...]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -continue\n\
      Specifies whether program operation is to continue after an error is\n\
      encountered.\n\n\
   <path-pattern> [ <path-pattern> ...]\n\n\
      The paths to be resolved to their generic equivalents by this program." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Process the command line parameters...
		// ////////////////////////////////////////////////////////////////
		bool         continue_flag  = false;
		unsigned int path_count     = 0;
		unsigned int count_1;
		if (ParseCmdLineArg::HasCmdLineArg("-CONTINUE", argc, argv))
			continue_flag = true;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (!ParseCmdLineArg::IsCmdLineArg("-CONTINUE", count_1, argc, argv)) {
				++path_count;
				try {
					std::string result_path(PathNameToGeneric(argv[count_1]));
					std::cout << argv[count_1] << "\t" << result_path << std::endl;
				}
				catch (const std::exception &except) {
					std::string error_string("Error on attempt to determine the "
						"generic path name equivalent for '" +
						std::string(argv[count_1]) + "': " +
						std::string(except.what()));
					if (continue_flag)
						std::cerr << error_string << std::endl;
					else
						Rethrow(except, error_string);
				}
			}
		}
		if (!path_count)
			ThrowInvalidArgument("No path names specified on the command line.");
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << my_name << ": fatal error encountered: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

