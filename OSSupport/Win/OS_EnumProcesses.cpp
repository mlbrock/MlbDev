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
								EnumProcesses().

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
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

#include <OSSupport/Win/OSSupport_Win.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

namespace {
// ////////////////////////////////////////////////////////////////////////////
const unsigned int OS_EnumProcessesDefault = 512;
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
OS_FPtr_EnumProcesses Get_OS_FPtr_EnumProcesses(bool require_proc)
{
	HMODULE module_handle = EnsureLoadedLibrary("psapi", true);

#pragma warning(disable:4191)
	return(reinterpret_cast<OS_FPtr_EnumProcesses>(OS_GetProcAddress(
		module_handle, "EnumProcesses", require_proc)));
#pragma warning(default:4191)
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Returns true if all processes fit into the list...
bool OS_EnumProcesses(DWORD *process_list, unsigned int process_list_bytes,
	unsigned int *process_needed_bytes)
{
	if (process_list_bytes % sizeof(HANDLE))
		MLB::Utility::ThrowException("An invocation of 'OS_EnumProcesses()' "
			"with a module list byte size of " +
			MLB::Utility::AnyToString(process_list_bytes) + " is invalid "
			"because it is not an integral multiple of the sizeof of a 'HANDLE' "
			"(" + MLB::Utility::AnyToString(sizeof(HANDLE)) + ").");

	OS_FPtr_EnumProcesses proc_addr = Get_OS_FPtr_EnumProcesses(true);
	DWORD                 tmp_bytes_required;

	if ((*proc_addr)(process_list, process_list_bytes, &tmp_bytes_required) == 0)
		MLB::Utility::ThrowSystemError("Call to EnumProcesses(" +
			MLB::Utility::ValueToStringHex(process_list) + ", " +
			MLB::Utility::AnyToString(process_list_bytes) + ", " +
			MLB::Utility::ValueToStringHex(process_needed_bytes) + " = " +
			MLB::Utility::AnyToString(*process_needed_bytes) + ") failed");

	*process_needed_bytes = tmp_bytes_required;

	if (*process_needed_bytes % sizeof(HANDLE))
		MLB::Utility::ThrowException("An invocation of 'EnumProcesses()' "
			"with a module list byte size of " +
			MLB::Utility::AnyToString(process_list_bytes) + " resulted in "
			"the returned number of bytes required to contain the handle list "
			"of " + MLB::Utility::AnyToString((process_needed_bytes == NULL) ?
			tmp_bytes_required : *process_needed_bytes) + ", which is not an "
			"integral multiple of the sizeof of a 'HANDLE' (" +
			MLB::Utility::AnyToString(sizeof(HANDLE)) + ").");

	return(process_list_bytes >= *process_needed_bytes);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Returns true if all processes fit into the list...
bool OS_EnumProcesses(DWORD *process_list, unsigned int process_count,
	unsigned int &process_count_needed)
{
	unsigned int process_needed_bytes;
	bool         return_code = OS_EnumProcesses(process_list,
		sizeof(process_list[0]) * process_count, &process_needed_bytes);

	process_count_needed = process_needed_bytes / sizeof(DWORD);

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Returns true if all processes fit into the list...
bool OS_EnumProcesses(DWORD *process_list, unsigned int process_count)
{
	unsigned int process_count_needed;

	return(OS_EnumProcesses(process_list, process_count * sizeof(DWORD),
		&process_count_needed));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<DWORD> &OS_EnumProcesses(std::vector<DWORD> &process_list)
{
	unsigned int               tmp_process_count = OS_EnumProcessesDefault;
	DWORD                      tmp_process_list[OS_EnumProcessesDefault];
	boost::shared_array<DWORD> tmp_process_list_ptr(tmp_process_list,
		MLB::BoostX::NullDeleter());

	for ( ; ; ) {
		unsigned int process_count_needed;
		if (OS_EnumProcesses(tmp_process_list_ptr.get(), tmp_process_count,
			process_count_needed)) {
			std::vector<DWORD>(tmp_process_list_ptr.get(),
				tmp_process_list_ptr.get() + process_count_needed).
				swap(process_list);
			break;
		}
		tmp_process_count = process_count_needed;
		tmp_process_list_ptr.reset(new DWORD[process_count_needed]);
	} 

	return(process_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<DWORD> OS_EnumProcesses()
{
	std::vector<DWORD> process_list;

	return(OS_EnumProcesses(process_list));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << "Test OS_EnumProcesses():" << std::endl;
		std::vector<DWORD> proc_list(OS_EnumProcesses());
		std::sort(proc_list.begin(), proc_list.end());
		std::vector<DWORD>::const_iterator iter_b(proc_list.begin());
		std::vector<DWORD>::const_iterator iter_e(proc_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::string proc_name;
			if (*iter_b == 0)
				proc_name = "System Idle Process";
			else if (*iter_b == 8)
				proc_name = "System";
			else {
				try {
					proc_name = ResolveProcessIdToName(*iter_b);
				}
				catch (const std::exception &except) {
					proc_name = except.what();
				}
			}
			std::cout << "   " << std::setw(10) << *iter_b << ": " <<
				proc_name << std::endl;
		}
		std::cout << std::endl << "Located " <<
			static_cast<unsigned int>(proc_list.size()) << " processes." <<
			std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_EnumProcesses.cpp: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

