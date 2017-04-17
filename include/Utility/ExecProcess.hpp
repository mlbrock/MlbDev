// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the operating system process
								fork/exec logic support.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__ExecProcess_hpp__HH

#define HH__MLB__Utility__ExecProcess_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file ExecProcess.hpp

	\brief	The MLB process fork/exec support header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/GetEnvironment.hpp>

#if defined(__unix__) && defined(__GNUC__)
# include <sys/types.h>
# include <sys/wait.h>
#endif // #if defined(__unix__) && defined(__GNUC__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
typedef HANDLE ExecProcessHandle;
typedef HANDLE ExecThreadHandle;
#else
typedef int    ExecProcessHandle;
typedef int    ExecThreadHandle;			//	Not used.
#endif // #ifdef _Windows
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY ExecProcessInfo {
	explicit ExecProcessInfo(int process_id = 0,
		ExecProcessHandle process_handle = 0, ExecThreadHandle thread_handle = 0);

	bool operator < (const ExecProcessInfo &other) const;
	bool operator == (const ExecProcessInfo &other) const;

	int               process_id_;
	ExecProcessHandle process_handle_;
	ExecThreadHandle  thread_handle_;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const ExecProcessInfo &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY WaitProcessInfo {
	explicit WaitProcessInfo(int process_id = 0, int process_exit_code = 0);

	bool operator < (const WaitProcessInfo &other) const;
	bool operator == (const WaitProcessInfo &other) const;

	int process_id_;
	int process_exit_code_;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const WaitProcessInfo &datum);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> ArgvList;
typedef ArgvList::iterator       ArgvListIter;
typedef ArgvList::const_iterator ArgvListIterC;
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY ExecProcessInfo ExecProcess(const ArgvList &argv_list,
	const EnvpList &envp_list = EnvpList(), bool background_flag = false,
	bool new_pgrp_flag = false);
API_UTILITY ExecProcessInfo ExecProcess(const std::string &program_name,
	const ArgvList &argv_list = ArgvList(),
	const EnvpList &envp_list = EnvpList(), bool background_flag = false,
	bool new_pgrp_flag = false);
API_UTILITY ExecProcessInfo ExecProcess(char **argv_list,
	char **envp_list = NULL, bool background_flag = false,
	bool new_pgrp_flag = false);
API_UTILITY ExecProcessInfo ExecProcess(const char *program_name,
	char **argv_list = NULL, char **envp_list = NULL,
	bool background_flag = false, bool new_pgrp_flag = false);
API_UTILITY bool            WaitProcess(const ExecProcessInfo &exec_info,
	WaitProcessInfo &wait_info, bool wait_forever = false);
API_UTILITY bool            WaitAnyProcess(WaitProcessInfo &wait_info,
	bool wait_forever = false);
API_UTILITY bool            WaitForOneProcess(
	const std::vector<ExecProcessInfo> &exec_info_list,
	WaitProcessInfo &wait_info, unsigned int &found_index,
	bool wait_forever = false);
API_UTILITY void            KillProcess(const ExecProcessInfo &exec_info,
	bool force_flag = false);
API_UTILITY bool            KillProcess(const ExecProcessInfo &exec_info,
	WaitProcessInfo &wait_info, bool force_flag = false,
	bool wait_forever = false);
API_UTILITY void            KillProcess(
	const std::vector<ExecProcessInfo> &exec_info_list,
	bool force_flag = false);

API_UTILITY bool            CanWaitAnyProcess();
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ExecProcess_hpp__HH

