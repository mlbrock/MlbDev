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
								GetModuleHandle().

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport/Win/OSSupport_Win.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
HMODULE OS_GetModuleHandle(const char *module_name, bool throw_if_not_loaded)
{
	HMODULE module_handle;

	if ((module_handle = ::GetModuleHandleA(
		reinterpret_cast<LPCSTR>(module_name))) == NULL) {
		if (throw_if_not_loaded || (!module_name)) {
			std::ostringstream o_str;
			o_str << "Call to 'GetModuleHandleA()' to determine the module "
				"handle for ";
			if (module_name)
				o_str << "module name '" << module_name << "' failed";
			else
				o_str << "the module of the executable which created the .exe "
					"process which invoked GetModuleHandleA() (specified with a "
					"NULL module name parameter) failed";
			MLB::Utility::ThrowSystemError(o_str.str());
		}
	}

	return(module_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
HMODULE OS_GetModuleHandle(const std::string &module_name,
	bool throw_if_not_loaded)
{
	return(OS_GetModuleHandle(module_name.c_str(), throw_if_not_loaded));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
HMODULE OS_GetModuleHandle()
{
	return(OS_GetModuleHandle(NULL, true));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		if (argc < 1)
			MLB::Utility::ThrowInvalidArgument("main() argc is < 1.");
		std::cout << "Test OS_GetModuleHandle(\"ntdll.dll\", true): " <<
			MLB::Utility::AnyToString(OS_GetModuleHandle("ntdll.dll", true)) <<
			std::endl;
		std::cout << "Test OS_GetModuleHandle(\"" << argv[0] << "\", true): " <<
			MLB::Utility::AnyToString(OS_GetModuleHandle(argv[0], true)) <<
			std::endl;
		std::cout << "Test OS_GetModuleHandle(NULL, true): " <<
			MLB::Utility::AnyToString(OS_GetModuleHandle(NULL, true)) <<
			std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetModuleHandle.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

