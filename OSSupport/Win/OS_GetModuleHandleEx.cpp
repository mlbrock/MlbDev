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
								GetModuleHandleEx(). Excluded from the Windows 2000
								distribution for compatibility reasons as the
								function wasn't added to Kernel32.dll until XP.

	Revision History	:	2007-02-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2018.
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
#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
HMODULE OS_GetModuleHandleEx(DWORD flags, const char *module_name,
	bool throw_if_not_loaded)
{
	HMODULE module_handle;

	if (::GetModuleHandleExA(flags, reinterpret_cast<LPCSTR>(module_name),
		&module_handle) == 0) {
		if (throw_if_not_loaded ||
			(flags & GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS)) {
			std::ostringstream o_str;
			o_str << "Call to 'GetModuleHandleExA(" <<
				MLB::Utility::ValueToStringHex(flags) << ", ";
			if (flags & GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS)
				o_str << MLB::Utility::ValueToStringHex(module_name);
			else
				o_str << "\"" << module_name << "\"";
			o_str << ", " << MLB::Utility::ValueToStringHex(&module_handle) <<
				")' failed";
			MLB::Utility::ThrowSystemError(o_str.str());
		 }
	}

	return(module_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
HMODULE OS_GetModuleHandleEx(DWORD flags, const std::string &module_name,
	bool throw_if_not_loaded)
{
	return(OS_GetModuleHandleEx(flags, module_name.c_str(),
		throw_if_not_loaded));
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const char WIN_OS_GetModuleHandleExTargetDatum = '?';
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
HMODULE OS_GetModuleHandleEx(bool inc_ref_count)
{
	HMODULE module_handle = 0;

	try {
		module_handle = OS_GetModuleHandleEx(static_cast<DWORD>(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			((inc_ref_count) ? 0 : GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT)),
			&WIN_OS_GetModuleHandleExTargetDatum, true);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to determine the module handle "
			"of the current module failed: " + std::string(except.what()));
	}

	return(module_handle);
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
		std::cout << "Test OS_GetModuleHandleEx(0, \"ntdll.dll\", true): " <<
			MLB::Utility::AnyToString(OS_GetModuleHandleEx(0,
			"ntdll.dll", true)) << std::endl;
		std::cout << "Test OS_GetModuleHandleEx(0, \"" <<
			argv[0] << "\", true): " <<
			MLB::Utility::AnyToString(OS_GetModuleHandleEx(0, argv[0], true)) <<
			std::endl;
		std::cout << "Test OS_GetModuleHandleEx(false): " <<
			MLB::Utility::AnyToString(OS_GetModuleHandleEx(false)) <<
			std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetModuleHandleEx.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

