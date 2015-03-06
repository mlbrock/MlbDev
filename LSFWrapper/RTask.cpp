// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for LSF functionality 'ls_rtask()'.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>

#include <LSFWrapper/LSFWrapper.hpp>

#if defined(_Windows) && defined(_DEBUG)
# include "CRTDBG.h"
#endif // #if defined(_Windows) && defined(_DEBUG)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

//	////////////////////////////////////////////////////////////////////////////
int LSF_rtask(const char *host, char **argv, int options)
{
	if (host == NULL)
		throw LSFException("The 'ls_rtask()' host name is 'NULL'.");

	if (!(*host))
		throw LSFException("The 'ls_rtask()' host name is an empty string.");

	if (argv == NULL)
		throw LSFException("The 'ls_rtask()' argument list pointer is 'NULL'.");

	if (argv[0] == NULL)
		throw LSFException("The 'ls_rtask()' first argument list pointer is "
			"'NULL' --- minimally, the name of the command to execute must be "
			"specified.");

	if (!(*argv[0]))
		throw LSFException("The 'ls_rtask()' first argument list pointer is "
			"an empty string--- minimally, the name of the command to execute "
			"must be specified.");
		
	int result;

#if defined(_Windows) && defined(_DEBUG)
	//	Get the old CRT debug flags...
	int old_mem_debug_flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	//	Turn them all off for this operation...
	_CrtSetDbgFlag(_CRTDBG_CHECK_DEFAULT_DF);
	//	Make the call to Platform LSF...
#endif // #if defined(_Windows) && defined(_DEBUG)

	result = ls_rtask(const_cast<char *>(host), argv, options);
	//	Restore the original CRT debug flags...

#if defined(_Windows) && defined(_DEBUG)
	_CrtSetDbgFlag(old_mem_debug_flag);
	//	Throw if necessary...
#endif // #if defined(_Windows) && defined(_DEBUG)

	if (result < 0) {
		std::ostringstream error_text;
		error_text << "Invocation of 'ls_rtask()' on host '" << host <<
			"' failed with a negative return code (" << result << ").";
		ThrowLSFStatus(lserrno, error_text);
	}

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int RTask(const std::string &host_name, const ArgvList &argument_list,
	int rtask_options)
{
	//	Construct the NULL pointer terminated char** array...
	MLB::Utility::C_ListTypeString<unsigned int> argv_list;
	argv_list.FromContainer(argument_list,
		"Invocation of Platform/LSF function 'ls_rtask()' for argv_list");
	argv_list.AppendEmpty(
		"Invocation of Platform/LSF function 'ls_rtask()' for argv_list");

	return(LSF_rtask(host_name.c_str(), argv_list.GetList(), rtask_options));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int RTask(const std::string &host_name, const std::string &argument_0,
	int rtask_options)
{
	return(RTask(host_name, ArgvList(1, argument_0), rtask_options));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#include <iomanip>

#ifdef TEST_MAIN

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
struct TEST_RTaskInfo {
	TEST_RTaskInfo(const std::string &host_name, bool rtask_done,
		int process_id, const std::string &error_text = "") :
		host_name_(host_name)
		,rtask_done_(rtask_done)
		,process_id_(process_id)
		,error_text_(error_text) { }

	std::string host_name_;
	bool        rtask_done_;
	int         process_id_;
	std::string error_text_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_RTask(unsigned int &node_count, int &rtask_options,
	const std::string &node_name, const ArgvList &argv_list)
{
	//	Need to set up before calling RTask()...
	RexInitDoneContext rex_context(1);

	HostList host_list;

	if (node_count > 1) {
		host_list = PlaceReq("", node_count, OK_ONLY);
		std::sort(host_list.begin(), host_list.end());
	}
	else if (node_name.empty())
		host_list = PlaceReq("", 1, OK_ONLY);
	else
		host_list.push_back(node_name);

	ArgvList tmp_argv_list;

	if (!argv_list.empty())
		tmp_argv_list = argv_list;
	else {
#ifdef _Windows
		tmp_argv_list = ArgvList(1, "C:\\WinNT\\system32\\hostname.exe");
#else
		tmp_argv_list = ArgvList(1, "hostname");
#endif // #ifdef _Windows
	}

	std::deque<TEST_RTaskInfo> rtask_info;

	unsigned int             count_1 = 0;
	HostList::const_iterator iter_b(host_list.begin());
	HostList::const_iterator iter_e(host_list.end());

	while (iter_b != iter_e) {
		try {
			std::cout << std::setw(5) << count_1 << ": [" <<
				std::setw(20) << *iter_b << "] = " << std::flush;
			int process_id = RTask(*iter_b, tmp_argv_list, rtask_options);
			std::cout << "PID " << std::setw(5) << process_id << std::endl;
			++count_1;
			rtask_info.push_back(TEST_RTaskInfo(*iter_b, true, process_id));
		}
		catch (const std::exception &except) {
			rtask_info.push_back(TEST_RTaskInfo(*iter_b, false, 0,
				except.what()));
			std::cout << "ERROR: " << except.what() << std::endl;
		}
		++iter_b;
	}

	Sleep(10000);

	for (count_1 = 0; count_1 < rtask_info.size(); ++count_1) {
		try {
			LSF_RWaitResult wait_result;
			std::cout << std::setw(5) << count_1 << ": " <<
				std::setw(20) << rtask_info[count_1].host_name_ << ": " <<
				std::flush;
			if (rtask_info[count_1].rtask_done_) {
				std::cout << "PID = " << std::setw(5) <<
					rtask_info[count_1].process_id_ << ": " << std::flush;
				RWaitTid(rtask_info[count_1].process_id_, wait_result, 0x0);
				std::cout << " Process exit code = " << std::setw(6) <<
					wait_result.process_status_ << std::endl;
			}
			else
				std::cout << "ERROR A: " << rtask_info[count_1].error_text_ <<
					std::endl;
		}
		catch (const std::exception &except) {
			std::cout << "ERROR B: " << except.what() << std::endl;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Get the command line parameters...
void GetParameters(int argc, char **argv, unsigned int &node_count,
	int &rtask_options, std::string &node_name, ArgvList &argv_list)
{
	unsigned int count_1;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(
			"-NODE_COUNT", count_1, argc, argv, node_count, 1))
			;
		else if (MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric<int>("-RTASK_OPTIONS",
			count_1, argc, argv, rtask_options))
			;
		else if (MLB::Utility::ParseCmdLineArg::ParseCmdLineFollowing("-HOST_NAME", count_1,
			argc, argv, node_name) ||
			MLB::Utility::ParseCmdLineArg::ParseCmdLineFollowing("-NODE_NAME", count_1, argc,
			argv, node_name))
			;
		else if (MLB::Utility::ParseCmdLineArg::ParseCmdLineAndRemainder("-EXECUTE", count_1,
			argc, argv, argv_list))
			;
		else
			MLB::Utility::ParseCmdLineArg::InvalidArgument(argv[count_1]);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code   = EXIT_SUCCESS;
	unsigned int node_count    = 1;
	int          rtask_options = 0;
	std::string  node_name;
	ArgvList     argv_list;

	std::cout << "Test of function 'RTask()'" << std::endl;
	std::cout << "---- -- -------- ---------" << std::endl;

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -node_count <number-of-nodes> ( = " << node_count << " ) ] " <<
			"[ -host_name <name-of-host-node-on-which-to-execute> ] " <<
			"[ -rtask_options <rtask-option-flags> ] " <<
			"[ -execute <program-name> [ <args> ... ] ] " << std::endl <<
			" *** NOTE: The '-execute' parameter (and its associated " <<
			"parameters which follow it) must be last on the command line." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		GetParameters(argc, argv, node_count, rtask_options, node_name,
			argv_list);
		TEST_RTask(node_count, rtask_options, node_name, argv_list);
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

