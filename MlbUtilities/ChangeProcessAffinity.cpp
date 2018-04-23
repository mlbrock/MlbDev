// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Program
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Changes the process affinity for the specified
								process(es).

	Revision History	:	2001-10-17 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/PathName.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/ParseCmdLineArg.hpp>

#include <Psapi.h>

#include <iomanip>

#ifdef _Windows
# pragma warning(disable:4217 4668)
# include <boost/shared_array.hpp>
# pragma warning(default:4217 4668)
#else
# include <boost/shared_array.hpp>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {
namespace Utility {
// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into Utility.hpp
#ifdef _Windows
typedef DWORD ProcessorAffinity;
#else
# error "Type 'MLB::Utility::ProcessorAffinity' not yet supported under this compilation environment."
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into Utility.hpp
unsigned int ProcessNameToProcessId(const std::string &process_name,
	std::vector<ProcessId> &process_id_list, unsigned int max_count = 0);
ProcessId    ProcessNameToProcessId(const std::string &process_name);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessId.cpp
unsigned int ProcessNameToProcessId(const std::string &process_name,
	std::vector<ProcessId> &process_id_list, unsigned int max_count)
{
#ifdef _Windows
	unsigned int               pid_count           = 0;
	unsigned int               pid_count_allocated = 8192;
	boost::shared_array<DWORD> pid_list(new DWORD[pid_count_allocated]);

	do {
		DWORD current_size = pid_count_allocated * sizeof(DWORD);
		DWORD needed_size  = 0;
		if (!::EnumProcesses(pid_list.get(), current_size, &needed_size))
			ThrowSystemError("Call to EnumProcesses() failed");
		//	We assume that if the number of bytes used is less than the number
		//	allocated that we've received the entire list of process ids.
		//	Otherwise, we re-allocate and re-try the operation.
		if (needed_size < current_size) {
			pid_count = needed_size / sizeof(DWORD);
			break;
		}
		pid_count_allocated *= 2;
		pid_list.reset(new DWORD[pid_count_allocated]);
	} while (pid_count_allocated);

	std::vector<ProcessId> tmp_process_id_list;
	unsigned int           done_count = 0;
	unsigned int           count_1;

	for (count_1 = 0; count_1 < pid_count; ++count_1) {
		HANDLE process_handle;
		if ((process_handle = ::OpenProcess(PROCESS_SET_INFORMATION |
			PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,
			pid_list[count_1])) != NULL) {
			//	First module is main EXE, so only need an array of one...
			HMODULE module_handle;
			DWORD   needed_size;
			if (::EnumProcessModules(process_handle, &module_handle,
				sizeof(module_handle), &needed_size) != 0) {
				char module_name[(MaxPathNameLength * 2) + 1];
				if (::GetModuleBaseName(process_handle, module_handle, module_name,
					sizeof(module_name) - 1) != 0) {
					if (!stricmp(process_name.c_str(), module_name)) {
						try {
							tmp_process_id_list.push_back(pid_list[count_1]);
							++done_count;
						}
						catch (...) {
							::CloseHandle(process_handle);
							throw;
						}
					}
				}
			}
			::CloseHandle(process_handle);
			if (max_count && (done_count >= max_count))
				break;
		}
	}

	if (!tmp_process_id_list.size())
		ThrowException("Function 'ProcessNameToProcessId()' was unable to "
			"locate a process named '" + process_name + "'.");

	process_id_list.swap(tmp_process_id_list);

	return(static_cast<unsigned int>(process_id_list.size()));
#else
	ThrowException("Function 'ProcessNameToProcessId()' not yet supported "
		"under this operating system.");

	return(0);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessId.cpp
ProcessId ProcessNameToProcessId(const std::string &process_name)
{
	std::vector<ProcessId> process_id_list;

	ProcessNameToProcessId(process_name, process_id_list, 1);

	return(process_id_list[0]);
}
// ////////////////////////////////////////////////////////////////////////////
} // namespace Utility
} // namespace MLB

namespace MLB {
namespace Utility {
// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into Utility.hpp.
HANDLE WINDOWS_ProcessIdToHandle(MLB::Utility::ProcessId process_id);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into OSNativeSupport.cpp.
HANDLE WINDOWS_ProcessIdToHandle(MLB::Utility::ProcessId process_id)
{
	HANDLE process_handle;

	if ((process_handle = ::OpenProcess(PROCESS_SET_INFORMATION, FALSE,
		process_id)) == NULL)
		MLB::Utility::ThrowSystemError("Call to OpenProcess() for process id " +
			MLB::Utility::AnyToString(process_id) + " failed");

	return(process_handle);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility
} // namespace MLB

namespace {

#ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
void NATIVE_GetAffinityInternal(DWORD &process_affinity_mask,
	DWORD &system_affinity_mask, HANDLE process_handle);
DWORD NATIVE_GetAffinitySystem();
DWORD NATIVE_GetAffinityProcess(MLB::Utility::ProcessId process_id);
DWORD NATIVE_GetAffinityProcess();
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
void NATIVE_GetAffinityInternal(DWORD &process_affinity_mask,
	DWORD &system_affinity_mask, HANDLE process_handle)
{
	if (::GetProcessAffinityMask(process_handle, &process_affinity_mask,
		&system_affinity_mask) == 0)
		MLB::Utility::ThrowSystemError("Call to GetProcessAffinityMask() for "
			"process handle " + MLB::Utility::ValueToStringHex(process_handle) +
			" failed");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
DWORD NATIVE_GetAffinitySystem()
{
	DWORD process_affinity_mask;
	DWORD system_affinity_mask;

	NATIVE_GetAffinityInternal(process_affinity_mask, system_affinity_mask,
		GetCurrentProcess());

	return(system_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
DWORD NATIVE_GetAffinityProcess(MLB::Utility::ProcessId process_id)
{
	if (!process_id)
		return(NATIVE_GetAffinityProcess());

	DWORD process_affinity_mask;
	DWORD system_affinity_mask;

	HANDLE process_handle = MLB::Utility::WINDOWS_ProcessIdToHandle(process_id);

	try {
		NATIVE_GetAffinityInternal(process_affinity_mask, system_affinity_mask,
			process_handle);
	}
	catch (...) {
		::CloseHandle(process_handle);
		throw;
	}

	::CloseHandle(process_handle);

	return(process_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
DWORD NATIVE_GetAffinityProcess()
{
	DWORD process_affinity_mask;
	DWORD system_affinity_mask;

	NATIVE_GetAffinityInternal(process_affinity_mask, system_affinity_mask,
		::GetCurrentProcess());

	return(process_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
DWORD NATIVE_SetAffinityProcessByHandle(HANDLE process_handle,
	DWORD desired_affinity_mask)
{
	if (!::SetProcessAffinityMask(process_handle, desired_affinity_mask))
		MLB::Utility::ThrowSystemError("Call to SetProcessAffinityMask() for "
			"process handle " + MLB::Utility::ValueToStringHex(process_handle) +
			" failed");

	return(desired_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
DWORD NATIVE_SetAffinityProcess(unsigned int process_id,
	DWORD desired_affinity_mask)
{
	HANDLE process_handle = MLB::Utility::WINDOWS_ProcessIdToHandle(process_id);

	try {
		NATIVE_SetAffinityProcessByHandle(process_handle, desired_affinity_mask);
	}
	catch (...) {
		::CloseHandle(process_handle);
		throw;
	}

	::CloseHandle(process_handle);

	return(desired_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

/*
	CODE NOTE: Obsoleted. To be removed.
// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
DWORD NATIVE_SetAffinityProcessInternal(HANDLE process_handle,
	MLB::Utility::ProcessId process_id, unsigned int cpu_count, DWORD system_affinity_mask,
	DWORD &current_cpu, bool randomize_flag)
{
	DWORD new_affinity_mask = 0;

	//	Only bother if more than one CPU is available...
	if (cpu_count > 0) {
		if (!system_affinity_mask)
			system_affinity_mask = NATIVE_GetAffinitySystem();
		if (randomize_flag) {
			DWORD one_cpu = 0;
			do {
				one_cpu = 1 << (rand() % ((sizeof(DWORD) * CHAR_BIT) - 1));
			} while (!(one_cpu & system_affinity_mask));
			new_affinity_mask = one_cpu;
		}
		else {
			//	Is initialized in main() to zero...
			current_cpu = (!current_cpu) ? 1 : (current_cpu << 1);
			while (!(current_cpu & system_affinity_mask)) {
				if (current_cpu > system_affinity_mask)
					current_cpu = 1;
				else
					current_cpu <<= 1;
			}
			new_affinity_mask = current_cpu;
		}
		if (!::SetProcessAffinityMask(process_handle, new_affinity_mask))
			ThrowSystemError("Call to SetProcessAffinityMask() for process "
				"id " + AnyToString(process_id) + " failed");
	}
	else
		new_affinity_mask = system_affinity_mask;

	return(new_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
void NATIVE_SetAffinityProcess(const std::string &process_name,
	DWORD system_affinity_mask, DWORD &current_cpu, bool randomize_flag)
{
	unsigned int               pid_count           = 0;
	unsigned int               pid_count_allocated = 8192;
	boost::shared_array<DWORD> pid_list(new DWORD[pid_count_allocated]);

	do {
		DWORD current_size = pid_count_allocated * sizeof(DWORD);
		DWORD needed_size  = 0;
		if (!::EnumProcesses(pid_list.get(), current_size, &needed_size))
			ThrowSystemError("Call to EnumProcesses() failed");
		//	We assume that if the number of bytes used is less than the number
		//	allocated that we've received the entire list of process ids.
		//	Otherwise, we re-allocate and re-try the operation.
		if (needed_size < current_size) {
			pid_count = needed_size / sizeof(DWORD);
			break;
		}
		pid_count_allocated *= 2;
		pid_list.reset(new DWORD[pid_count_allocated]);
	} while (pid_count_allocated);

	unsigned int done_count = 0;
	unsigned int count_1;

	for (count_1 = 0; count_1 < pid_count; ++count_1) {
		HANDLE process_handle;
		if ((process_handle = ::OpenProcess(PROCESS_SET_INFORMATION |
			PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,
			pid_list[count_1])) != NULL) {
			//	First module is main EXE, so only need an array of one...
			HMODULE module_handle;
			DWORD   needed_size;
			if (::EnumProcessModules(process_handle, &module_handle,
				sizeof(module_handle), &needed_size) != 0) {
				char module_name[(MaxPathNameLength * 2) + 1];
				if (::GetModuleBaseName(process_handle, module_handle, module_name,
					sizeof(module_name) - 1) != 0) {
					if (!stricmp(process_name.c_str(), module_name)) {
						try {
							DWORD new_affinity_mask =
								SetAffinityProcessInternal(process_handle,
								pid_list[count_1], cpu_count, system_affinity_mask,
								current_cpu, randomize_flag);
							std::cout << "Process id " << pid_list[count_1] <<
								" (" << process_name << ") set to CPU affinity " <<
								ValueToStringBin(new_affinity_mask) << "." <<
								std::endl;
							++done_count;
						}
						catch (...) {
							::CloseHandle(process_handle);
							throw;
						}
					}
				}
			}
			::CloseHandle(process_handle);
		}
	}

	if (!done_count)
		std::cerr << "Process named '" << process_name << "' not found." <<
			std::endl;
}
// ////////////////////////////////////////////////////////////////////////////
*/
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
MLB::Utility::ProcessorAffinity INTERNAL_FixUpAffinityRandom(
	MLB::Utility::ProcessorAffinity desired_affinity_mask);
MLB::Utility::ProcessorAffinity INTERNAL_FixUpAffinityProgressive(
	MLB::Utility::ProcessorAffinity &current_cpu,
	MLB::Utility::ProcessorAffinity desired_affinity_mask);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
MLB::Utility::ProcessorAffinity INTERNAL_FixUpAffinityRandom(
	MLB::Utility::ProcessorAffinity desired_affinity_mask)
{
	desired_affinity_mask = (desired_affinity_mask > 0) ? desired_affinity_mask :
      NATIVE_GetAffinitySystem();

	MLB::Utility::ProcessorAffinity new_affinity_mask = 0;

	do {
		new_affinity_mask = 1 << (rand() % ((sizeof(DWORD) * CHAR_BIT) - 1));
	} while (!(new_affinity_mask & desired_affinity_mask));

	return(new_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
MLB::Utility::ProcessorAffinity INTERNAL_FixUpAffinityProgressive(
	MLB::Utility::ProcessorAffinity &current_cpu,
	MLB::Utility::ProcessorAffinity desired_affinity_mask)
{
	desired_affinity_mask = (desired_affinity_mask > 0) ? desired_affinity_mask :
      NATIVE_GetAffinitySystem();

	MLB::Utility::ProcessorAffinity new_affinity_mask = 0;

	do {
		current_cpu = (!current_cpu) ? 1 : (current_cpu << 1);
		if (current_cpu > desired_affinity_mask)
			current_cpu = 1;
	} while (!(current_cpu && desired_affinity_mask));

	return(new_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

}	//	Anonymous namespace

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into Utility.hpp
ProcessorAffinity GetAffinitySystem();
ProcessorAffinity GetAffinityProcess(ProcessId process_id);
ProcessorAffinity GetAffinityProcess();

void              SetAffinityProcess(const std::string &process_name,
	ProcessorAffinity &current_cpu,
	ProcessorAffinity system_affinity_mask = GetAffinitySystem());
ProcessorAffinity SetAffinityProcessRandom(ProcessId process_id,
	ProcessorAffinity desired_affinity_mask = GetAffinitySystem());
ProcessorAffinity SetAffinityProcessProgressive(ProcessId process_id,
	ProcessorAffinity &current_cpu,
	ProcessorAffinity desired_affinity_mask = GetAffinitySystem());

void SetAffinityProcess(const std::string &process_name,
	ProcessorAffinity desired_affinity_mask = GetAffinitySystem());
void SetAffinityProcessRandom(const std::string &process_name,
	ProcessorAffinity desired_affinity_mask = GetAffinitySystem());
void SetAffinityProcessProgressive(const std::string &process_name,
	ProcessorAffinity &current_cpu,
	ProcessorAffinity desired_affinity_mask = GetAffinitySystem(),
	unsigned int max_count = 0);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
ProcessorAffinity GetAffinitySystem()
{
	return(NATIVE_GetAffinitySystem());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
ProcessorAffinity GetAffinityProcess(ProcessId process_id)
{
	return(NATIVE_GetAffinityProcess(process_id));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
ProcessorAffinity GetAffinityProcess()
{
	return(NATIVE_GetAffinityProcess());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
ProcessorAffinity SetAffinityProcess(ProcessId process_id,
	ProcessorAffinity desired_affinity_mask)
{
	return(NATIVE_SetAffinityProcess(process_id, desired_affinity_mask));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
ProcessorAffinity SetAffinityProcessRandom(ProcessId process_id,
	ProcessorAffinity desired_affinity_mask)
{
	return(SetAffinityProcess(process_id,
		INTERNAL_FixUpAffinityRandom(desired_affinity_mask)));
}
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
ProcessorAffinity SetAffinityProcessProgressive(ProcessId process_id,
	ProcessorAffinity &current_cpu, ProcessorAffinity desired_affinity_mask)
{
	ProcessorAffinity tmp_current_cpu = current_cpu;

	ProcessorAffinity tmp_affinity_mask =
		INTERNAL_FixUpAffinityProgressive(tmp_current_cpu, desired_affinity_mask);

	ProcessorAffinity new_affinity_mask =
		SetAffinityProcess(process_id, tmp_affinity_mask);

	tmp_current_cpu = current_cpu;

	return(new_affinity_mask);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
void SetAffinityProcess(const std::string &process_name,
	ProcessorAffinity desired_affinity_mask)
{
	ProcessorAffinity current_cpu = 0;

	desired_affinity_mask = INTERNAL_FixUpAffinityProgressive(current_cpu,
		desired_affinity_mask);

	SetAffinityProcessProgressive(process_name, desired_affinity_mask, 1);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
void SetAffinityProcessRandom(const std::string &process_name,
	ProcessorAffinity desired_affinity_mask)
{
	std::vector<ProcessId> process_id_list;

	ProcessNameToProcessId(process_name, process_id_list);

	std::vector<ProcessId>::const_iterator iter_b(process_id_list.begin());
	std::vector<ProcessId>::const_iterator iter_e(process_id_list.end());

	while (iter_b != iter_e) {
		SetAffinityProcess(*iter_b,
			INTERNAL_FixUpAffinityRandom(desired_affinity_mask));
		++iter_b;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into ProcessAffinity.cpp
void SetAffinityProcessProgressive(const std::string &process_name,
	ProcessorAffinity &current_cpu, ProcessorAffinity desired_affinity_mask,
	unsigned int max_count)
{
	std::vector<ProcessId> process_id_list;

	ProcessNameToProcessId(process_name, process_id_list, max_count);

	std::vector<ProcessId>::const_iterator iter_b(process_id_list.begin());
	std::vector<ProcessId>::const_iterator iter_e(process_id_list.end());

	while (iter_b != iter_e) {
		ProcessorAffinity tmp_current_cpu = current_cpu;
		SetAffinityProcess(*iter_b, INTERNAL_FixUpAffinityProgressive(tmp_current_cpu,
			desired_affinity_mask));
		current_cpu = tmp_current_cpu;
		++iter_b;
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code = EXIT_SUCCESS;
	std::string  my_name     = GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -randomize ] " <<
			"{ <process-id> | <process-name> } [ ... ] " <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Get the process affinity mask...
		// ////////////////////////////////////////////////////////////////
		DWORD process_affinity_mask;
		DWORD system_affinity_mask;
		if (GetProcessAffinityMask(GetCurrentProcess(),
			&process_affinity_mask, &system_affinity_mask) == 0)
			ThrowSystemError("Call to GetProcessAffinityMask() for this process "
				"failed");
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Determine the number of CPUs on this machine...
		// ////////////////////////////////////////////////////////////////
		unsigned int cpu_count = 0;
		unsigned int cpu_mask  = system_affinity_mask;
		while (cpu_mask) {
			cpu_count += cpu_mask & 1;
			cpu_mask >>= 1;
		}
		std::cout << "Available CPUs on this machine: " << cpu_count << std::endl;
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Process the command line parameters...
		// ////////////////////////////////////////////////////////////////
		bool         randomize_flag = false;
		DWORD        current_cpu    = 0;
		unsigned int count_1;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (ParseCmdLineArg::IsCmdLineArg("-RANDOMIZE", count_1, argc, argv) ||
				ParseCmdLineArg::IsCmdLineArg("-RANDOM", count_1, argc, argv))
				randomize_flag = true;
			else {
				try {
					std::string this_process_id;
					char        *end_ptr;
					const char  *start_ptr  = argv[count_1];
					double       tmp_double = strtod(start_ptr, &end_ptr);
					if (end_ptr != (start_ptr + strlen(start_ptr))) {
						//	Find by process name...
						if (randomize_flag)
							SetAffinityProcessRandom(argv[count_1], system_affinity_mask);
						else
							SetAffinityProcessProgressive(argv[count_1], current_cpu,
								system_affinity_mask);
					}
					else {
						//	Find by process identifier...
						if ((tmp_double <
							static_cast<double>(
							std::numeric_limits<unsigned int>::min())) ||
							(tmp_double >
							static_cast<double>(
							std::numeric_limits<unsigned int>::max())))
							ThrowInvalidArgument("Process identifier '" +
								std::string(argv[count_1]) + " is outside of the "
								"permissible range (from " +
								AnyToString(std::numeric_limits<unsigned int>::min()) +
								" to " +
								AnyToString(std::numeric_limits<unsigned int>::max()) +
								", inclusive).");
						ProcessId this_pid = static_cast<unsigned int>(tmp_double);
						if (randomize_flag)
							SetAffinityProcessRandom(this_pid, system_affinity_mask);
						else
							SetAffinityProcessProgressive(this_pid, current_cpu,
								system_affinity_mask);
					}
				}
				catch (const std::exception &except) {
					std::cerr << "Process '" << argv[count_1] << "' CPU affinity "
						"setting failed: " << except.what() << std::endl;
					return_code = EXIT_FAILURE;
				}
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << my_name << ": fatal error encountered: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

