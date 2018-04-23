// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for LSF functionality 'ls_rkill()'.

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

#include <LSFWrapper/LSFWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

int  LSF_rkill(int tid, int signal_id = 0);
bool RKill(int tid, int signal_id = 0);

//	////////////////////////////////////////////////////////////////////////////
int LSF_rkill(int tid, int signal_id)
{
	int result;

	if ((result = ls_rkill(tid, signal_id)) < 0) {
/*
		if ((result == -1) && (lserrno == LSE_NORCHILD) && task_not_required)
			return(0);
*/
		std::ostringstream error_text;
		error_text << "Invocation of 'ls_rkill(" << tid << ", " <<
			signal_id << ")' failed with a negative return code (" << result <<
			").";
		throw LSFExceptionStatus(lserrno, error_text);
	}

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RKill(int tid, int signal_id)
{
	return(LSF_rkill(tid, signal_id) == 0);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/Sleep.hpp>

#include <iomanip>

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
static void TEST_RKill()
{
	//	Need to set up before calling RTask()...
	RexInitDoneContext rex_context(1);

	HostList host_list = PlaceReq("", 1, OK_ONLY);

	ArgvList argv_list;

	argv_list.push_back("ping");
	argv_list.push_back("-t");
	argv_list.push_back("localhost");

	int remote_task_id = RTask(host_list[0], argv_list);

MLB::Utility::SleepMilliSecs(5000);

try {
	RKill(remote_task_id, -1);
}
catch (const std::exception &except) {
	std::cout << "RKill() ERROR: " << except.what() << std::endl;
}

	LSF_RWaitResult wait_result;

MLB::Utility::SleepMilliSecs(5000);

	RWait(wait_result, 0x01);

	std::cout << "RWait() Result: process id=" << wait_result.process_id_ <<
		", process_status=" << wait_result.process_status_ << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test of function 'RKill()'" << std::endl;
	std::cout << "---- -- -------- ---------" << std::endl;

	try {
		TEST_RKill();
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

