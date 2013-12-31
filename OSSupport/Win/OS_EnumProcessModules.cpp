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
								EnumProcessModules().

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

#include <BoostX.hpp>

#include <OSSupport_Win.h>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

namespace {
// ////////////////////////////////////////////////////////////////////////////
const unsigned int OS_EnumProcessModulesDefault = 1024;
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
OS_FPtr_EnumProcessModules Get_OS_FPtr_EnumProcessModules(bool require_proc)
{
	HMODULE module_handle = EnsureLoadedLibrary("psapi", true);

#pragma warning(disable:4191)
	return(reinterpret_cast<OS_FPtr_EnumProcessModules>(OS_GetProcAddress(
		module_handle, "EnumProcessModules", require_proc)));
#pragma warning(default:4191)
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Returns true if all modules for the process fit into the list...
bool OS_EnumProcessModules(HANDLE process_handle, HMODULE *module_list,
	unsigned int module_list_bytes, unsigned int *module_needed_bytes)
{
	if (module_list_bytes % sizeof(HANDLE))
		MLB::Utility::ThrowException("An invocation of "
			"'OS_EnumProcessModules()' with a module list byte size of " +
			MLB::Utility::AnyToString(module_list_bytes) + " is invalid "
			"because it is not an integral multiple of the sizeof of a 'HANDLE' "
			"(" + MLB::Utility::AnyToString(sizeof(HANDLE)) + ").");

	DWORD tmp_bytes_required;

	OS_FPtr_EnumProcessModules proc_addr = Get_OS_FPtr_EnumProcessModules(true);

	if ((*proc_addr)(process_handle, module_list, module_list_bytes,
		&tmp_bytes_required) == 0)
		MLB::Utility::ThrowSystemError("Call to EnumProcesses(" +
			MLB::Utility::ValueToStringHex(process_handle) + ", " +
			MLB::Utility::ValueToStringHex(module_list) + ", " +
			MLB::Utility::AnyToString(module_list_bytes) + ", " +
			MLB::Utility::ValueToStringHex(module_needed_bytes) + " = " +
			MLB::Utility::AnyToString(*module_needed_bytes) + ") failed");

	*module_needed_bytes = tmp_bytes_required;

	if (*module_needed_bytes % sizeof(HANDLE))
		MLB::Utility::ThrowException("An invocation of 'EnumProcessModules()' "
			"with a module list byte size of " +
			MLB::Utility::AnyToString(module_list_bytes) + " resulted in "
			"the returned number of bytes required to contain the handle list "
			"of " + MLB::Utility::AnyToString((module_needed_bytes == NULL) ?
			tmp_bytes_required : *module_needed_bytes) + ", which is not an "
			"integral multiple of the sizeof of a 'HANDLE' (" +
			MLB::Utility::AnyToString(sizeof(HANDLE)) + ").");

	return(module_list_bytes >= *module_needed_bytes);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Returns true if all modules for the process fit into the list...
bool OS_EnumProcessModules(HANDLE process_handle, HMODULE *module_list,
	unsigned int module_count, unsigned int &module_count_needed)
{
	unsigned int module_needed_bytes;
	bool         return_code = OS_EnumProcessModules(process_handle,
		module_list, sizeof(module_list[0]) * module_count,
		&module_needed_bytes);

	module_count_needed = module_needed_bytes / sizeof(HMODULE);

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Returns true if all modules for the process fit into the list...
bool OS_EnumProcessModules(HANDLE process_handle, HMODULE *module_list,
	unsigned int module_count)
{
	unsigned int module_count_needed;

	return(OS_EnumProcessModules(process_handle, module_list,
		module_count * sizeof(HMODULE), &module_count_needed));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<HMODULE> &OS_EnumProcessModules(HANDLE process_handle,
	std::vector<HMODULE> &module_list)
{
	unsigned int                 tmp_module_count = OS_EnumProcessModulesDefault;
	HMODULE                      tmp_module_list[OS_EnumProcessModulesDefault];
	boost::shared_array<HMODULE> tmp_module_list_ptr(tmp_module_list,
		MLB::BoostX::NullDeleter());

	for ( ; ; ) {
		unsigned int module_count_needed;
		if (OS_EnumProcessModules(process_handle, tmp_module_list_ptr.get(),
			tmp_module_count, module_count_needed)) {
			std::vector<HMODULE>(tmp_module_list_ptr.get(),
				tmp_module_list_ptr.get() + module_count_needed).swap(module_list);
			break;
		}
		tmp_module_count = module_count_needed;
		tmp_module_list_ptr.reset(new HMODULE[module_count_needed]);
	} 

	return(module_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<HMODULE> OS_EnumProcessModules(HANDLE process_handle)
{
	std::vector<HMODULE> module_list;

	return(OS_EnumProcessModules(process_handle, module_list));
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
			"[ { <process-name> | <process-id> } [ ... ] ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		std::vector<std::string> process_list;
		if (argc == 1)
			process_list.push_back(argv[0]);
		else
			std::vector<std::string>(argv + 1, argv + argc).swap(process_list);
		std::vector<std::string>::const_iterator iter_p_b(process_list.begin());
		std::vector<std::string>::const_iterator iter_p_e(process_list.end());
		std::cout << "Test OS_EnumProcessModules():" << std::endl;
		for ( ; iter_p_b != iter_p_e; ++iter_p_b) {
			std::pair<MLB::Utility::ProcessId, std::string> proc_spec;
			try {
				proc_spec = ResolveProcessSpecifier(*iter_p_b);
				std::cout << " " << std::setw(7) << proc_spec.first << ": " <<
					proc_spec.second << std::endl;
				HANDLE proc_handle = ResolveProcessIdToHandle(proc_spec.first);
				try {
					std::vector<HMODULE>                 module_list(
						OS_EnumProcessModules(proc_handle));
					std::vector<HMODULE>::const_iterator iter_m_b(module_list.begin());
					std::vector<HMODULE>::const_iterator iter_m_e(module_list.end());
					for ( ; iter_m_b != iter_m_e; ++iter_m_b)
						std::cout << "    " <<
							MLB::Utility::ValueToStringHex(*iter_m_b) << ": " <<
							OS_GetModuleFileName(*iter_m_b) << std::endl;
				}
				catch (const std::exception &) {
					::CloseHandle(proc_handle);
					throw;
				}
				::CloseHandle(proc_handle);
				std::cout << std::endl;
			}
			catch (const std::exception &except) {
				std::cout << "FAILED FOR '" << *iter_p_b << "': " <<
					except.what() << std::endl;
				return_code = EXIT_FAILURE;
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_EnumProcessModules.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

