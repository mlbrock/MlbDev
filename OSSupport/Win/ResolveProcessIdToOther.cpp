// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Windows-specific implementation of logic to resolve
								between process names, identifiers and HANDLEs.

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

#include <Utility/PathName.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
unsigned int ResolveProcessNameToId(const std::string &process_name,
	std::vector<MLB::Utility::ProcessId> &process_id_list, unsigned int max_count,
	bool throw_if_not_found)
{
#ifdef _Windows
	std::string                          tmp_process_name(
		MLB::Utility::UpperCase(MLB::Utility::GetFileNamePortion(process_name)));
	std::vector<DWORD>                   complete_pid_list(OS_EnumProcesses());
	std::vector<MLB::Utility::ProcessId> tmp_process_id_list;
	unsigned int                         done_count = 0;
	std::vector<DWORD>::size_type        count_1;

	for (count_1 = 0; count_1 < complete_pid_list.size(); ++count_1) {
		if ((complete_pid_list[count_1] == 0) ||
			(complete_pid_list[count_1] == 8))
			continue;
		HANDLE process_handle;
		if ((process_handle = ::OpenProcess(PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ, FALSE, complete_pid_list[count_1])) != NULL) {
			try {
				HMODULE     module_handle = GetProcessModuleHandle(process_handle);
				std::string module_name;
				OS_GetModuleBaseName(process_handle, module_handle, module_name);
				if (tmp_process_name == MLB::Utility::UpperCase(module_name)) {
					tmp_process_id_list.push_back(complete_pid_list[count_1]);
					++done_count;
				}
			}
			catch (const std::exception &) {
				::CloseHandle(process_handle);
				throw;
			}
			::CloseHandle(process_handle);
			if (max_count && (done_count >= max_count))
				break;
		}
	}

	if ((!tmp_process_id_list.size()) && throw_if_not_found)
		MLB::Utility::ThrowException("Function 'ResolveProcessNameToId()' was "
			"unable to locate a process named '" + process_name + "'.");

	process_id_list.swap(tmp_process_id_list);

	return(static_cast<unsigned int>(process_id_list.size()));
#else
	ThrowException("Function 'ResolveProcessNameToId()' not yet supported "
		"under this operating system.");

	return(0);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::ProcessId ResolveProcessNameToId(const std::string &process_name)
{
	std::vector<MLB::Utility::ProcessId> process_id_list;

	return((ResolveProcessNameToId(process_name, process_id_list, 1, true) > 0) ?
      process_id_list[0] : static_cast<MLB::Utility::ProcessId>(0));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &ResolveProcessIdToName(MLB::Utility::ProcessId process_id,
	std::string &process_name)
{
	HANDLE process_handle = ResolveProcessIdToHandle(process_id);

	if (process_handle != INVALID_HANDLE_VALUE) {
		try {
			//	First module is main EXE, so only need an array of one...
			HMODULE module_handle;
			OS_EnumProcessModules(process_handle, &module_handle, 1);
			OS_GetModuleFileNameEx(process_handle, module_handle, process_name);
			::CloseHandle(process_handle);
		}
		catch (const std::exception &) {
			::CloseHandle(process_handle);
			throw;
		}
	}
	else
		process_name.clear();

	return(process_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ResolveProcessIdToName(MLB::Utility::ProcessId process_id)
{
	std::string process_name;

	return(ResolveProcessIdToName(process_id, process_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
HANDLE ResolveProcessIdToHandle(MLB::Utility::ProcessId process_id)
{
	HANDLE process_handle;

	if ((process_handle = ::OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ, FALSE, process_id)) == NULL)
		MLB::Utility::ThrowSystemError("Call to OpenProcess() for process id " +
			MLB::Utility::AnyToString(process_id) + " failed");

	return(process_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
HANDLE ResolveProcessNameToHandle(const std::string &process_name,
	bool throw_if_not_found)
{
	std::vector<MLB::Utility::ProcessId> process_id_list;

	if (ResolveProcessNameToId(process_name, process_id_list, 1,
		throw_if_not_found) > 0)
		return(ResolveProcessIdToHandle(process_id_list[0]));


	return(INVALID_HANDLE_VALUE);
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
			"[ <process-name> [ <process-name> ... ] ]" <<
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
		std::cout << "Test ResolveProcessNameToId():" << std::endl;
		std::cout << "     ResolveProcessIdToName():" << std::endl;
		std::vector<std::string>::const_iterator iter_b(file_list.begin());
		std::vector<std::string>::const_iterator iter_e(file_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
         MLB::Utility::ProcessId pid = ResolveProcessNameToId(*iter_b);
			std::cout << "   " << std::setw(10) << pid << ": " << *iter_b <<
				" --- " << std::flush << ResolveProcessIdToName(pid) << std::endl;
		}
		std::cout << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/ResolveProcessIdToOther.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

