// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of:
									DetermineFileVersion()
									DetermineProductVersion()

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport_Win.h>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

namespace {
// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::VersionNumber DetermineVersionInfo(
	const std::string &file_name, const std::string &file_info_name,
	WORD lang_code, WORD code_page)
{
	std::string                 tmp_value;
	MLB::Utility::VersionNumber version_number;

	DetermineFileInfo(file_name, file_info_name, lang_code, code_page,
		tmp_value, true);

	try {
		MLB::Utility::VersionNumber::FromString(tmp_value).swap(version_number);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to extract the '" +
			file_info_name + "' version information from the file '" +
			file_name + "': " + std::string(except.what()));
	}

	return(version_number);
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::VersionNumber DetermineFileVersion(const std::string &file_name,
	WORD lang_code, WORD code_page)
{
	return(DetermineVersionInfo(file_name, "FileVersion", lang_code,
		code_page));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::VersionNumber DetermineProductVersion(
	const std::string &file_name, WORD lang_code, WORD code_page)
{
	return(DetermineVersionInfo(file_name, "ProductVersion", lang_code,
		code_page));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ <file-name> [ <file-name> ... ] ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		std::vector<std::string> file_list;
		if (argc == 1)
			file_list.push_back(argv[0]);
		else
			std::vector<std::string>(argv + 1, argv + argc).swap(file_list);
		std::cout << "Test DetermineFileVersion()   :" << std::endl;
		std::cout << "     DetermineProductVersion():" << std::endl;
		std::vector<std::string>::const_iterator iter_b(file_list.begin());
		std::vector<std::string>::const_iterator iter_e(file_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::cout << "   " << *iter_b <<    ": File Version   : " <<
				DetermineFileVersion(*iter_b) << std::endl;
			std::cout << "   " <<
				std::setw(static_cast<unsigned int>(iter_b->size())) << " " <<
				": Product Version: " << DetermineProductVersion(*iter_b) <<
				std::endl;
		}
		std::cout << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/DetermineVersionInfo.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

