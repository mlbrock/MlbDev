// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for LSF functionality 'ls_rwaittid()'.

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

//	////////////////////////////////////////////////////////////////////////////
int LSF_rwaittid(int tid, LS_WAIT_T *status, int options, struct rusage *ru)
{
	if (status == NULL)
		throw LSFException("The 'ls_rwaittid()' status pointer is 'NULL'.");

	int result;

	LSFWrapper_THROW_IF_NEGATIVE(result, ls_rwaittid,
		(tid, status, options, ru));

	if ((result > 0) && (result != tid)) {
		std::ostringstream error_text;
		error_text << "The Platform/LSF function 'ls_rwaittid()' returned " <<
			result << ", which should be equal to the remote task identifier " <<
			"specified as an argument (" << tid << ").";
		throw LSFException(error_text);
	}

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RWaitTid(int rtask_id, LSF_RWaitResult &wait_result, int rwait_options)
{
	wait_result.process_id_ = LSF_rwaittid(rtask_id,
		&wait_result.process_status_, rwait_options, &wait_result.rusage_data_);

	return((wait_result.process_id_) ? true : false);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#include <iomanip>

#ifdef TEST_MAIN

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
static void TEST_RWaitTid()
{
	//	Need to set up before calling RWaitTid()...
	InitRex(1);
	HostList host_list = PlaceReq("", 1, OK_ONLY);

	ArgvList argv_list;
	argv_list.push_back("ping");
	argv_list.push_back("localhost");

	int tid = RTask(host_list[0], argv_list);

	LSF_RWaitResult wait_result;

	RWaitTid(tid, wait_result, 0x01);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test of function 'RWaitTid()'" << std::endl;
	std::cout << "---- -- -------- ------------" << std::endl;

	try {
		TEST_RWaitTid();
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

