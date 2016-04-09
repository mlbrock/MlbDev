// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of GetFileInfoStringList().

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

#include <OSSupport_Win.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector GetFileInfoStringList(const std::string &file_name,
	WORD lang_code, WORD code_page)
{
	VS_FIXEDFILEINFO  version_info;
	OS_VersionInfoSet string_map;

	OS_GetFileVersionInfo(file_name, lang_code, code_page, version_info,
		string_map);

	MLB::Utility::StringVector   out_list;
	OS_VersionInfoSetIterC iter_b(string_map.begin());
	OS_VersionInfoSetIterC iter_e(string_map.end());

	out_list.reserve(string_map.size());

	for ( ; iter_b != iter_e; ++iter_b) {
		std::ostringstream o_str;
		o_str
			<< std::hex << std::setfill('0')
			<< "0x" << std::setw(sizeof(iter_b->lang_code_) * 2)
				<< iter_b->lang_code_
			<< " "
			<< "0x" << std::setw(sizeof(iter_b->code_page_) * 2)
				<< iter_b->code_page_
			<< " "
			<< std::dec << std::setfill(' ')
			<< std::left << std::setw(OS_PredefinedVersionInfoNameMax)
				<< iter_b->info_name_
			<< " " << std::right
			<< iter_b->info_value_;
		out_list.push_back(o_str.str());
	}

	return(out_list);
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
		std::cout << "Test GetFileInfoStringList():" << std::endl;
		std::vector<std::string>::const_iterator iter_b(file_list.begin());
		std::vector<std::string>::const_iterator iter_e(file_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::cout << "   " << *iter_b << ": " << std::endl;
			std::vector<std::string> string_list(GetFileInfoStringList(*iter_b));
			std::copy(string_list.begin(), string_list.end(),
				std::ostream_iterator<std::vector<std::string>::value_type>(
				std::cout, "\n"));
		}
		std::cout << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/GetFileInfoStringList.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

