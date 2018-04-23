// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	FAST Instruction context dumper.

	Revision History	:	2008-12-27 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/InsContext.hpp>
#include <VFast/ExcContext.hpp>

#include <Utility/C_StringSupport.hpp>
#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
enum VFastContextDumper {
	DumpFull     = 0,
	DumpSimple   = 1,
	DumpDispatch = 2
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CheckCommandLineForHelp(int argc, char **argv)
{
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"<fast-template-file> [ <fast-template-file> ... ] " <<
			"[ -simple | -full ] [ -dispatch ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   <fast-template-file>\n\
      Is the name of Fix/FAST XML template file to processed by this program.\n\
      Multiple of such files may be specified on the command line and may be\n\
      optionally interspersed with the '-full', '-simple' and '-dispatch'\n\
      parameters described below.\n\n\
      Each parameter takes effect in the order of its occurrence.\n\n\
   -full\n\
      Specifies that the items of the instruction context will be printed in\n\
      their entirity. This is the default setting.\n\n\
   -simple\n\
      Specifies that a simplified, compact version of the instruction context\n\
      will be printed without the inclusion of dictionary name, key name and\n\
      default/initial dictionary value.\n\n\
   -dispatch\n\
      Specifies that the instruction context will be printed along with the\n\
      names of the methods of 'ExcContext' to which the execution context\n\
      logic will dispatch field processing." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	using namespace MLB::VFast;

	int return_code = EXIT_SUCCESS;

	try {
		CheckCommandLineForHelp(argc, argv);
		VFastContextDumper dump_flag = DumpFull;
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			if (!MLB::Utility::Utility_stricmp(argv[count_1], "-full")) {
				dump_flag = DumpFull;
				std::cerr << "Switched to output type " <<
					MLB::Utility::UpperCase(argv[count_1] + 1) << std::endl;
			}
			else if (!MLB::Utility::Utility_stricmp(argv[count_1], "-simple")) {
				dump_flag = DumpSimple;
				std::cerr << "Switched to output type " <<
					MLB::Utility::UpperCase(argv[count_1] + 1) << std::endl;
			}
			else if (!MLB::Utility::Utility_stricmp(argv[count_1], "-dispatch")) {
				dump_flag = DumpDispatch;
				std::cerr << "Switched to output type " <<
					MLB::Utility::UpperCase(argv[count_1] + 1) << std::endl;
			}
			else {
				std::cerr << "Processing XML file '" << argv[count_1] << "'..." <<
					std::endl;
				InsContext ins_context(argv[count_1]);
				ins_context.CheckContext();
				std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
				std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
				std::cout << "Template File: " << argv[count_1] << std::endl;
				std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
				if (dump_flag == DumpFull)
					ins_context.DumpTemplateMapFull();
				else if (dump_flag == DumpSimple)
					ins_context.DumpTemplateMap();
				else
					ins_context.DumpTemplateMapDispatch();
				std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
				std::cout << std::endl;
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

