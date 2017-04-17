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
								GetProcAddress().

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

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
FARPROC OS_GetProcAddress(HMODULE module_handle, const char *proc_spec,
	bool require_proc)
{
	FARPROC proc_addr;

	if (((proc_addr = ::GetProcAddress(module_handle, proc_spec)) == NULL) &&
		 (require_proc)) {
		std::ostringstream o_str;
		o_str << "Call to 'GetProcAddress()' for " <<
			OS_GetProcAddressArgDescription(proc_spec) << " (module handle " <<
			MLB::Utility::ValueToStringHex(module_handle) << ") failed";
		MLB::Utility::ThrowSystemError(o_str.str());
	}

	return(proc_addr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FARPROC OS_GetProcAddress(HMODULE module_handle, Native_UInt16 proc_ordinal,
	bool require_proc)
{
	return(OS_GetProcAddress(module_handle,
		reinterpret_cast<const char *>(proc_ordinal), require_proc));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FARPROC OS_GetProcAddress(HMODULE module_handle, const std::string &proc_name,
	bool require_proc)
{
	return(OS_GetProcAddress(module_handle, proc_name.c_str(), require_proc));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FARPROC OS_GetProcAddress(const std::string &module_name,
	const char *proc_spec, bool require_proc)
{
	FARPROC proc_addr = NULL;

	try {
		proc_addr = OS_GetProcAddress(EnsureLoadedLibrary(module_name, true),
			proc_spec, require_proc);
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Unable to determine the procedure address for module '" <<
			module_name << "': " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(proc_addr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FARPROC OS_GetProcAddress(const std::string &module_name,
	Native_UInt16 proc_ordinal, bool require_proc)
{
	return(OS_GetProcAddress(module_name,
		reinterpret_cast<const char *>(proc_ordinal), require_proc));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FARPROC OS_GetProcAddress(const std::string &module_name,
	const std::string &proc_name, bool require_proc)
{
	return(OS_GetProcAddress(module_name, proc_name.c_str(), require_proc));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OS_GetProcAddressArgDescription(const char *proc_spec)
{
	std::ostringstream o_str;

	if (proc_spec > reinterpret_cast<const char *>(
		std::numeric_limits<Native_UInt16>::max()))
		o_str << "procedure '" << proc_spec << "()'";
	else
		o_str << "procedure ordinal " <<
			reinterpret_cast<unsigned int>(proc_spec);

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
WinProcAddrMap GetWinProcAddrMap(HMODULE module_handle)
{
	Native_UInt16  proc_ordinal = 1;
	FARPROC        proc_addr;
	WinProcAddrMap proc_map;

	/*
		Note that it's possible for the same proc address to map to
		different ordinals. For an example, dumpbin /exports ntdll.dll
		and look for functions named {Nt|Zw}* which have different
		ordinals but the same address.
	*/
	while ((proc_addr = OS_GetProcAddress(module_handle, proc_ordinal,
		false)) != NULL)
		proc_map.insert(std::make_pair(proc_addr, proc_ordinal++));

	return(proc_map);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
WinProcAddrMap GetWinProcAddrMap(const std::string &module_name)
{
	return(GetWinProcAddrMap(EnsureLoadedLibrary(module_name, true)));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/EmitterSep.hpp>

using namespace MLB::Utility;
using namespace MLB::OSSupport;

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest(const char *module_name)
{
	std::cout << "Exports in module '" <<
		((module_name) ?  module_name : "***SELF***") << "':\n" <<
		EmitterSep('-');

	WinProcAddrMap proc_map(GetWinProcAddrMap((module_name) ?
		OS_GetModuleHandle(module_name, true) : OS_GetModuleHandleEx(false)));

	WinProcAddrMapIterC iter_b(proc_map.begin());
	WinProcAddrMapIterC iter_e(proc_map.end());

	for ( ; iter_b != iter_e; ++iter_b)
		std::cout << ValueToStringHex(iter_b->first) <<
			"=" << std::setw(5) << iter_b->second << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest(const char *module_name, const char *function_name)
{
	FARPROC proc_addr = OS_GetProcAddress(module_name, function_name, false);

	std::cout
		<< "Function '" << function_name << "': "
		<< ValueToStringHex(proc_addr) << "\n"
			;
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << EmitterSep('=');
		if (argc < 2)
			TEST_RunTest("ntdll.dll");
		else if (argc < 3)
			TEST_RunTest(argv[1]);
		else {
			std::cout
				<< "Function in module '" << argv[1] << "':\n"
				<< EmitterSep('-')
					;
			for (int count_1 = 2; count_1 < argc; ++count_1)
				TEST_RunTest(argv[1], argv[count_1]);
		}
		std::cout << EmitterSep('=');
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

