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
								GetFileVersionInfoSize().

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

#include <OSSupport_Win.h>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
DWORD OS_GetFileVersionInfoSize(const std::string &file_name,
	DWORD &unknown_value)
{
	DWORD info_size;

	HMODULE module_handle = EnsureLoadedLibrary("version", true);

	typedef COMPAT_FN_TYPE(DWORD (WINAPI *OS_FPtr_GetFileVersionInfoSize),
		(LPTSTR, LPDWORD));

#pragma warning(disable:4191)
	OS_FPtr_GetFileVersionInfoSize get_size_proc_addr   =
		reinterpret_cast<OS_FPtr_GetFileVersionInfoSize>(OS_GetProcAddress(
		module_handle, "GetFileVersionInfoSizeA", true));
#pragma warning(default:4191)

	if ((info_size = (*get_size_proc_addr)(const_cast<char *>(file_name.c_str()),
		&unknown_value)) == 0)
		MLB::Utility::ThrowSystemError("Call to 'GetFileVersionInfoSize()' for "
			"file name '" + file_name + "' failed");

	return(info_size);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DWORD OS_GetFileVersionInfoSize(const std::string &file_name)
{
	DWORD unknown_value;

	return(OS_GetFileVersionInfoSize(file_name, unknown_value));
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
		std::cout << "Test OS_GetFileVersionInfoSize():" << std::endl;
		std::vector<std::string>::const_iterator iter_b(file_list.begin());
		std::vector<std::string>::const_iterator iter_e(file_list.end());
		for ( ; iter_b != iter_e; ++iter_b)
			std::cout << "   " << *iter_b << ": " <<
				OS_GetFileVersionInfoSize(*iter_b) << std::endl;
		std::cout << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetFileVersionInfoSize.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

