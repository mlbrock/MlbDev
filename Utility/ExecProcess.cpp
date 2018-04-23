//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the process fork/exec logic.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>
#include <Utility/ExecProcess.hpp>
#include <Utility/Utility_Exception.hpp>

#include <algorithm>

#if defined(__unix__) && defined(__GNUC__)
# include <unistd.h>
#endif // #if defined(__unix__) && defined(__GNUC__)

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

#ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
static unsigned int CheckWindowsMultipleWait(DWORD wait_return_code,
	unsigned int handle_count)
{
	if (wait_return_code == WAIT_FAILED)
		ThrowSystemError("'WaitForMultipleObjects()' returned 'WAIT_FAILED'");

//	MSVC++ 7.x through 10.0 emit warning C4296: '<' : expression is always false on the 'if'...
#if defined(_MSC_VER) && (_MSC_VER >= 1310)
# pragma warning(push)
# pragma warning(disable:4296)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1310)

	if ((wait_return_code < WAIT_OBJECT_0) ||
		(wait_return_code >= (WAIT_OBJECT_0 + handle_count))) {
		std::ostringstream error_text;
		error_text << "Error on 'WaitForMultipleObjects()' invocation " <<
			"returned unexpected value (" << wait_return_code <<
			") --- expected values are WAIT_TIMEOUT (" << WAIT_TIMEOUT <<
			") or an indicator of a signaled handle (" << WAIT_OBJECT_0 <<
			" through " << ((WAIT_OBJECT_0 + handle_count) - 1) << ", inclusive)";
		ThrowSystemError(error_text);
	}

#if defined(_MSC_VER) && (_MSC_VER >= 1310)
# pragma warning(pop)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1310)

	return(wait_return_code - WAIT_OBJECT_0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void GetProcessExit(const ExecProcessInfo &exec_info,
	WaitProcessInfo &wait_info)
{
	DWORD process_exit_code;

	if (!::GetExitCodeProcess(exec_info.process_handle_, &process_exit_code))
		ThrowSystemError("Error on 'GetExitCodeProcess()' invocation");

	wait_info.process_id_        = exec_info.process_id_;
	wait_info.process_exit_code_ = static_cast<int>(process_exit_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
ExecProcessInfo ExecProcess(const ArgvList &argv_list,
	const EnvpList &envp_list, bool background_flag, bool new_pgrp_flag)
{
	if (argv_list.empty())
		ThrowInvalidArgument("Argument vector is an empty list.");

	if (argv_list[0].empty())
		ThrowInvalidArgument("The name of the file to execute is an empty string.");

 	ExecProcessInfo fork_process_info;

#ifdef _Windows
	std::ostringstream argv_string;
	std::string        envp_string;

	unsigned int             element_count = 0;
	ArgvList::const_iterator iter_argv_b(argv_list.begin());
	ArgvList::const_iterator iter_argv_e(argv_list.end());

	//	Assemble the argument list...
	while (iter_argv_b != iter_argv_e) {
		if (!element_count)
			argv_string << *iter_argv_b;
		else {
			if ((iter_argv_b->size() < 2) || ((*iter_argv_b)[0] != '\"') ||
				((*iter_argv_b)[iter_argv_b->size() - 1] != '\"')) {
				if (((*iter_argv_b)[iter_argv_b->size() - 1] == '\\') &&
					((*iter_argv_b)[iter_argv_b->size() - 2] != '\\'))
					argv_string << " \"" << *iter_argv_b << "\\\"";
				else
					argv_string << " \"" << *iter_argv_b << "\"";
			}
			else
				argv_string << " " << *iter_argv_b;
		}
		++element_count;
		++iter_argv_b;
	}

	//	Assemble the environment list, if specified..
	if (!envp_list.empty()) {
		element_count = 0;
		EnvpList::const_iterator iter_envp_b(envp_list.begin());
		EnvpList::const_iterator iter_envp_e(envp_list.end());
		while (iter_envp_b != iter_envp_e) {
			if (iter_envp_b->empty()) {
				std::ostringstream error_text;
				error_text << "Element index " << element_count <<
					" of the environment list is empty.";
				ThrowInvalidArgument(error_text);
			}
			envp_string += *iter_envp_b + '\0';
			++element_count;
			++iter_envp_b;
		}
		envp_string += '\0';
	}

	STARTUPINFOA        startup_info;
	PROCESS_INFORMATION process_info;

	::GetStartupInfoA(&startup_info);

	//	Must have a guaranteed non-const copy of the string because the
	//	CreateProcess() function modifies the string.
	char *tmp_argv_string = NULL;
	tmp_argv_string = C_strdup(argv_string.str().c_str(),
		"Temporary argv string for Windows 'CreateProcessA()' function");

	BOOL return_code = ::CreateProcessA(NULL,
		tmp_argv_string,
		NULL,
		NULL,
		FALSE,
		static_cast<DWORD>(
		((background_flag) ? (CREATE_NO_WINDOW | DETACHED_PROCESS) : 0) |
		((new_pgrp_flag) ? CREATE_NEW_PROCESS_GROUP : 0)),
		(envp_string.empty()) ? NULL :
			const_cast<char *>(envp_string.c_str()),
		NULL,
		&startup_info,
		&process_info);

	::free(tmp_argv_string);

	if (!return_code)
		ThrowSystemError("Attempt to use 'CreateProcessA()' for command line '" +
			argv_string.str() + "' failed");

	fork_process_info.process_id_     =
		static_cast<int>(process_info.dwProcessId);
	fork_process_info.process_handle_ = process_info.hProcess;
	fork_process_info.thread_handle_  = process_info.hThread;
#else
	//	Some kind of Unix assumed...

	if (background_flag)
		ThrowLogicError("The 'background_flag' parameter of the function "
			"'MLB::Utility::ExecProcess()' is not yet implemented.");

	if (new_pgrp_flag)
		ThrowLogicError("The 'new_pgrp_flag' parameter of the function "
			"'MLB::Utility::ExecProcess()' is not yet implemented.");

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Note that we construct the parameters in the parent BEFORE invoking
	//	fork(). This is because constuction of the parameters requires use
	//	of resources (such as the memory allocation heap) requiring access
	//	through locks held by threads not duplicated in the child process.
	//	
	//	We avoid the possibility of such deadlock at the cost of creating
	//	objects which will never be used in the parent process, only the
	//	child.
	//	//////////////////////////////////////////////////////////////////////
	C_ListTypeString<unsigned int> tmp_argv_list;
	C_ListTypeString<unsigned int> tmp_envp_list;
	//	Construct the NULL pointer terminated char** array for argv...
	tmp_argv_list.FromContainer(argv_list,
		"Attempt to construct 'argv' list for 'exec()' failed");
	tmp_argv_list.AppendEmpty(
		"Attempt to construct 'argv' list for 'exec()' failed");
	//	Construct the NULL pointer terminated char** array for envp...
	tmp_envp_list.FromContainer(envp_list,
		"Attempt to construct 'envp' list for 'exec()' failed");
	tmp_envp_list.AppendEmpty(
		"Attempt to construct 'envp' list for 'exec()' failed");
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Perform the fork()...
	//	//////////////////////////////////////////////////////////////////////
	int fork_pid = ::fork();
	if (fork_pid == -1)
		ThrowSystemError("Attempt to fork()/exec() failed");
	//	Not defined as part of the interface, but I'm a paranoaic...
	if (fork_pid < 0) {
		std::ostringstream error_text;
		error_text << "Attempt to fork() failed with return code " <<
			fork_pid << " --- defined returns from fork() are -1, 0, and " <<
			"process identifiers, which should be greater that zero.";
		ThrowOutOfRange(error_text);
	}
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	If we're in the child, try to exec(). Failure must be fatal...
	//	//////////////////////////////////////////////////////////////////////
	if (!fork_pid) {
		try {
			::execve(tmp_argv_list.GetList()[0], tmp_argv_list.GetList(),
				tmp_envp_list.GetList());
			//	The exec() family of functions should never return. So...
			ThrowSystemError("Invocation of function 'execve() failed.");
		}
		catch (const std::exception &except) {
			try {
				std::ostringstream error_text;
				error_text << std::endl << std::endl <<
					"Error on an attempt to perform a fork()/exec(): " <<
					except.what() << std::endl << std::endl;
				std::cerr << error_text.str();
			}
			catch (...) {
				;
			}
		}
		catch (...) {
			;
		}
		//	Die, die, die!
		//	Note use of expedited _exit() to avoid double-flush of I/O buffers.
		_exit(EXIT_FAILURE);
	}
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Otherwise, we're in the parent. We'll return...
	//	//////////////////////////////////////////////////////////////////////
	fork_process_info.process_id_     = fork_pid;
	fork_process_info.process_handle_ = fork_pid;
	//	//////////////////////////////////////////////////////////////////////
#endif // #ifdef _Windows

	return(fork_process_info);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ExecProcessInfo ExecProcess(const std::string &program_name,
	const ArgvList &argv_list, const EnvpList &envp_list, bool background_flag,
	bool new_pgrp_flag)
{
	if (program_name.empty())
		ThrowInvalidArgument("Program name is an empty string.");

	ArgvList tmp_argv_list(1, program_name);

	if (!argv_list.empty())
		tmp_argv_list.insert(tmp_argv_list.begin() + 1, argv_list.begin(),
			argv_list.end());

	return(ExecProcess(tmp_argv_list, envp_list, background_flag,
		new_pgrp_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ExecProcessInfo ExecProcess(char **argv_list, char **envp_list,
	bool background_flag, bool new_pgrp_flag)
{
	ArgvList tmp_argv_list;
	if (argv_list != NULL) {
		while (*argv_list != NULL) {
			tmp_argv_list.push_back(*argv_list);
			++argv_list;
		}
	}

	EnvpList tmp_envp_list;
	if (envp_list != NULL) {
		while (*envp_list != NULL) {
			tmp_envp_list.push_back(*envp_list);
			++envp_list;
		}
	}

	return(ExecProcess(tmp_argv_list, tmp_envp_list, background_flag,
		new_pgrp_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ExecProcessInfo ExecProcess(const char *program_name, char **argv_list,
	char **envp_list, bool background_flag, bool new_pgrp_flag)
{
	if (program_name == NULL)
		ThrowInvalidArgument("Program name is NULL.");

	if (!(*program_name))
		ThrowInvalidArgument("Program name is an empty string.");

	ArgvList tmp_argv_list;
	tmp_argv_list.push_back(program_name);
	if (argv_list != NULL) {
		while (*argv_list != NULL) {
			tmp_argv_list.push_back(*argv_list);
			++argv_list;
		}
	}

	EnvpList tmp_envp_list;
	if (envp_list != NULL) {
		while (*envp_list != NULL) {
			tmp_envp_list.push_back(*envp_list);
			++envp_list;
		}
	}

	return(ExecProcess(tmp_argv_list, tmp_envp_list, background_flag,
		new_pgrp_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool WaitProcess(const ExecProcessInfo &exec_info, WaitProcessInfo &wait_info,
	bool wait_forever)
{
#ifdef _Windows
	DWORD return_code = WaitForSingleObject(exec_info.process_handle_,
		(wait_forever) ? INFINITE : 0);
	if (return_code == WAIT_TIMEOUT)
		return(false);
	else if (return_code != WAIT_OBJECT_0)
		ThrowSystemError("Error on 'WaitForSingleObject()' invocation");
	GetProcessExit(exec_info, wait_info);
	::CloseHandle(exec_info.process_handle_);
	if (exec_info.thread_handle_ != INVALID_HANDLE_VALUE)
		::CloseHandle(exec_info.thread_handle_);
	return(true);
#else
	int process_id;
	int process_exit_code;
	process_id = ::waitpid(exec_info.process_id_, &process_exit_code,
		(wait_forever) ? 0 : WNOHANG);
	if (process_id == 0)
		return(false);
	else if (process_id < 0)
		ThrowSystemError("Error on 'waitpid()' invocation");
	wait_info.process_id_        = exec_info.process_id_;
	wait_info.process_exit_code_ = process_exit_code;
	return(true);
#endif // #ifdef _Windows
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
bool WaitAnyProcess(WaitProcessInfo & /* wait_info */, bool /* wait_forever */)
#else
bool WaitAnyProcess(WaitProcessInfo &wait_info, bool wait_forever)
#endif // #ifdef _Windows
{
#ifdef _Windows
	return(false);
#else
	int process_id;
	int process_exit_code;

	process_id = ::waitpid(0, &process_exit_code, (wait_forever) ? 0 : WNOHANG);

	if (process_id == 0)
		return(false);
	else if (process_id < 0)
		ThrowSystemError("Error on 'waitpid()' invocation");

	wait_info.process_id_        = process_id;
	wait_info.process_exit_code_ = process_exit_code;

	return(true);
#endif // #ifdef _Windows
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool WaitForOneProcess(const std::vector<ExecProcessInfo> &exec_info_list,
	WaitProcessInfo &wait_info, unsigned int &found_index, bool wait_forever)
{
	if (exec_info_list.empty())
		return(false);

	unsigned int handle_count = static_cast<unsigned int>(exec_info_list.size());

	//	If only one process for which to wait, take the simple method...
	if (handle_count == 1) {
		bool wait_code = WaitProcess(exec_info_list[0], wait_info, wait_forever);
		found_index = 0;
		return(wait_code);
	}

#ifdef _Windows
	//	Otherwise, we'll need an array of Windows process handles...
	std::vector<ExecProcessInfo>::const_iterator iter_b(exec_info_list.begin());
	std::vector<ExecProcessInfo>::const_iterator iter_e(exec_info_list.end());
	std::vector<HANDLE> handle_list;
	handle_list.reserve(handle_count);
	while (iter_b != iter_e) {
		handle_list.push_back(iter_b->process_handle_);
		++iter_b;
	}

	//	If the number of processes can be checked in a single call to
	//	WaitForMultipleObjects(), do so...
	if (handle_count <= MAXIMUM_WAIT_OBJECTS) {
		DWORD return_code = ::WaitForMultipleObjects(
			handle_count,
			&handle_list[0],
			FALSE,
			(wait_forever) ? INFINITE : 0);
		if (return_code == WAIT_TIMEOUT)
			return(false);
		found_index = CheckWindowsMultipleWait(return_code, handle_count);
		GetProcessExit(exec_info_list[found_index], wait_info);
		::CloseHandle(exec_info_list[found_index].process_handle_);
		return(true);
	}

	//	Otherwise we have to iterate...
	while (wait_forever) {
		unsigned int base_index  = 0;
		unsigned int total_count = handle_count;
		while (total_count) {
			unsigned int this_count  = std::min(total_count,
				static_cast<unsigned int>(MAXIMUM_WAIT_OBJECTS));
			DWORD        return_code = ::WaitForMultipleObjects(
				this_count,
				&handle_list[base_index],
				FALSE,
				0);
			if (return_code != WAIT_TIMEOUT) {
				found_index = CheckWindowsMultipleWait(return_code, this_count) +
					base_index;
				GetProcessExit(exec_info_list[found_index], wait_info);
				::CloseHandle(exec_info_list[found_index].process_handle_);
				return(true);
			}
			base_index  += this_count;
			total_count -= this_count;
		}
		if (!wait_forever)
			break;
		::Sleep(10);							//	Don't want to burn CPU needlessly...
	}

	return(false);
#else
	WaitProcessInfo tmp_wait_info;

	while (true) {
		if (WaitAnyProcess(tmp_wait_info, wait_forever)) {
			std::vector<ExecProcessInfo>::const_iterator iter(
				std::find(exec_info_list.begin(), exec_info_list.end(),
				ExecProcessInfo(wait_info.process_id_, wait_info.process_id_)));
			if (iter != exec_info_list.end()) {
				wait_info   = tmp_wait_info;
				found_index = std::distance(exec_info_list.begin(), iter);
				return(true);
			}
		}
		if (!wait_forever)
			break;
	}

	return(false);
#endif // #ifdef _Windows
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
void KillProcess(const ExecProcessInfo &exec_info, bool /* force_flag */)
#else
void KillProcess(const ExecProcessInfo &exec_info, bool force_flag)
#endif // #ifdef _Windows
{
#ifdef _Windows
	if (!::TerminateProcess(exec_info.process_handle_, 666)) {
		std::ostringstream error_text;
		error_text << "Attempt to use 'TerminateProcess()' to kill process id " <<
			exec_info.process_id_ << " failed";
		ThrowSystemError(error_text);
	}
#else
	::kill(static_cast<int>(exec_info.process_id_),
		(force_flag) ? SIGKILL : SIGINT);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool KillProcess(const ExecProcessInfo &exec_info, WaitProcessInfo &wait_info,
	bool force_flag, bool wait_forever)
{
	KillProcess(exec_info, force_flag);

	return(WaitProcess(exec_info, wait_info, wait_forever));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void KillProcess(const std::vector<ExecProcessInfo> &exec_info_list,
	bool force_flag)
{
	if (exec_info_list.empty())
		return;

	std::vector<ExecProcessInfo>::const_iterator iter_b(exec_info_list.begin());
	std::vector<ExecProcessInfo>::const_iterator iter_e(exec_info_list.end());

	while (iter_b != iter_e) {
		KillProcess(*iter_b, force_flag);
		++iter_b;
	}
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool CanWaitAnyProcess()
{
#ifdef _Windows
	return(false);
#else
	return(true);
#endif // #ifdef _Windows
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB


#ifdef TEST_MAIN

#include <Utility/Sleep.hpp>
#include <Utility/ParseCmdLineArg.hpp>

#include <iomanip>

#ifdef _MSC_VER
//	////////////////////////////////////////////////////////////////////////////
/*
	CODE NOTE: Some sample code from MSDN on how to shutdown an application
	cleanly from http://support.microsoft.com/kb/178893 .

	This can perhaps be used to implement a variably-insistent logic for calls
	to KillProcess() depending upon whether the force_flag is true.
*/
#define TA_FAILED 0
#define TA_SUCCESS_CLEAN 1
#define TA_SUCCESS_KILL 2
#define TA_SUCCESS_16 3

BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )
{
	DWORD dwID ;

	::GetWindowThreadProcessId(hwnd, &dwID) ;

	if(dwID == (DWORD)lParam) {
		::PostMessage(hwnd, WM_CLOSE, 0, 0) ;
	}

	return TRUE ;
}

DWORD WINAPI TerminateApp(DWORD dwPID, DWORD dwTimeout)
{
	HANDLE hProc ;
	DWORD  dwRet ;

	// If we can't open the process with PROCESS_TERMINATE rights,
	// then we give up immediately.
	hProc = ::OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, dwPID);

	if(hProc == NULL) {
		return TA_FAILED ;
	}

	// TerminateAppEnum() posts WM_CLOSE to all windows whose PID
	// matches your process's.
	::EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM) dwPID) ;

	// Wait on the handle. If it signals, great. If it times out,
	// then you kill it.
	if(::WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
		dwRet=static_cast<DWORD>(
			::TerminateProcess(hProc,0) ? TA_SUCCESS_KILL : TA_FAILED);
	else
		dwRet = TA_SUCCESS_CLEAN ;

	::CloseHandle(hProc) ;

	return dwRet ;
}
//	////////////////////////////////////////////////////////////////////////////
#endif // #ifdef _MSC_VER

using namespace MLB::Utility;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	std::cout << "Test routine for 'ExecProcess()'" << std::endl;
	std::cout << "---- ------- --- ---------------" << std::endl;

//DWORD try_pid = 496;
//TerminateApp(try_pid, 1000);

	int          return_code   = EXIT_SUCCESS;
	unsigned int fork_depth    = 5;
	unsigned int sleep_seconds = 0;

	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -depth <fork-process-depth-count> ( = " << fork_depth << " ) ] " <<
			"[ -sleep <final-process-sleep-seconds> ( = " << sleep_seconds <<
				" ) ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	try {
		unsigned int count_1;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (ParseCmdLineArg::ParseCmdLineNumeric<unsigned int>("-DEPTH",
				count_1, argc, argv, fork_depth, 0))
				;
			else if (ParseCmdLineArg::ParseCmdLineNumeric<unsigned int>("-SLEEP",
				count_1, argc, argv, sleep_seconds, 0))
				;
			else
				ParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		std::ostringstream tmp_string;
		tmp_string << "[" << std::setw(10) << CurrentProcessId() << "]: " <<
			"Will start processes to a depth of " << fork_depth << "." <<
			std::endl;
		std::cout << tmp_string.str();
		if (fork_depth) {
			ArgvList argv_list;
			argv_list.push_back(argv[0]);
			argv_list.push_back("-depth");
			tmp_string.str("");
			tmp_string << (fork_depth - 1);
			argv_list.push_back(tmp_string.str());
			argv_list.push_back("-sleep");
			tmp_string.str("");
			tmp_string << sleep_seconds;
			argv_list.push_back(tmp_string.str());
			ExecProcessInfo exec_data = ExecProcess(argv_list);
			WaitProcessInfo wait_data;
			WaitProcess(exec_data, wait_data, true);
		}
		else {
			tmp_string.str("");
			tmp_string << "Sleeping for " << sleep_seconds << " seconds ..." <<
				std::endl;
			std::cout << tmp_string.str();
			SleepSecs(sleep_seconds);
		}
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl;
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

