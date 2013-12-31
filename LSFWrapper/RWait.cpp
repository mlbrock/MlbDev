// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for LSF functionality 'ls_rwait()'.

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

#include <LSFWrapper/LSFWrapper.hpp>

#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

//	////////////////////////////////////////////////////////////////////////////
int LSF_rwait(LS_WAIT_T *status, int options, struct rusage *ru,
	bool task_not_required)
{
	if (status == NULL)
		throw LSFException("The 'ls_rwait()' status pointer is 'NULL'.");

	int result;

	if ((result = ls_rwait(status, options, ru)) < 0) {
		if ((result == -1) && (lserrno == LSE_NORCHILD) && task_not_required)
			return(0);
		std::ostringstream error_text;
		error_text << "Invocation of 'ls_rwait(" <<
			MLB::Utility::ValueToStringHex(status) << ", " << options << ", " <<
			MLB::Utility::ValueToStringHex(ru) <<
			")' failed with a negative return code (" << result << ").";
		throw LSFExceptionStatus(lserrno, error_text);
	}

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RWait(LSF_RWaitResult &wait_result, int rwait_options,
	bool task_not_required)
{
	wait_result.process_id_ = LSF_rwait(&wait_result.process_status_,
		rwait_options, &wait_result.rusage_data_, task_not_required);

	return((wait_result.process_id_) ? true : false);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#include <iomanip>

#ifdef TEST_MAIN

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
static void TEST_RWait()
{
	//	Need to set up before calling RWait()...
	InitRex(1);
	HostList host_list = PlaceReq("", 1, OK_ONLY);

	ArgvList argv_list;
	argv_list.push_back("ping");
	argv_list.push_back("localhost");
	RTask(host_list[0], argv_list);

	LSF_RWaitResult wait_result;

	RWait(wait_result, 0x01);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test of function 'RWait()'" << std::endl;
	std::cout << "---- -- -------- ---------" << std::endl;

	try {
		TEST_RWait();
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

