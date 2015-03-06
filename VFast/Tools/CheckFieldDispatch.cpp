// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Tools Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Checks to ensure that all fields in an instruction
								context are fully-resolvable to the dispatch methods
								of ExcContext.

	Revision History	:	2008-02-23 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/ExcContext.hpp>

#include <Utility/StringSupport.hpp>

#include <iterator>

// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::VFast;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"<XML-template-file> [ <XML-template-file> ... ] " <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	try {
		if (argc < 2)
			MLB::Utility::ThrowInvalidArgument("Expected at least one XML "
				"template file name on the command line. Use '-h' for help.");
		unsigned int total_errors = 0;
		int          count_1;
		for (count_1 = 1; count_1 < argc; ++count_1) {
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << "Template File: " << argv[count_1] << std::endl;
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			try {
				std::vector<std::string> error_list;
				InsContext               ins_context(argv[count_1]);
				if (!ExcContext::CheckFieldDispatch(ins_context, error_list)) {
					std::copy(error_list.begin(), error_list.end(),
						std::ostream_iterator<std::string>(std::cout, "\n"));
					std::cout << std::endl;
				}
				std::cout << error_list.size() << " errors encountered." <<
					std::endl;
				total_errors += static_cast<unsigned int>(error_list.size());
			}
			catch (const std::exception &except) {
				std::cout << "Error occurred while processing XML template "
					"file '" << argv[count_1] << "': " << except.what() << std::endl;
			}
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << std::endl;
		}
		std::cout << "Encountered " << total_errors << " error(s) in " <<
			(argc - 1) << " processed XML template file(s)." << std::endl <<
			std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

/*
	Testing command line for Windows:
	------- ------- ---- --- --------
	C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20080925_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20081007_A.xml C:\DevEnv\Tests\FastCommon\Templates\ISE\ISE_fast_templates_v2.0.ORIGINAL.xml > C:\DevEnv\Tests\VFast\UnitTests\CheckFieldDispatch.txt
*/

