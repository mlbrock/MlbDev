/*
	Added files:
	----- ------
	OSSupport_CoreDumperBase.hpp

	CoreDumperBase.cpp
	Win/FileTimeToTimeSpec.cpp
	Win/OS_GetProcessIoCounters.cpp
	Win/OS_GetProcessTimes.cpp

*/
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
								GetProcessTimes().

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

#include <OSSupport/Win/OSSupport_Win.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

//	////////////////////////////////////////////////////////////////////////////
void OS_GetProcessTimes(HANDLE process_handle, FILETIME *creation_time,
	FILETIME *exit_time, FILETIME *kernel_time, FILETIME *user_time)
{
	if (::GetProcessTimes(process_handle, creation_time, exit_time, kernel_time,
		user_time) == 0)
		MLB::Utility::ThrowSystemError("Call to 'GetProcessTimes()' failed.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OS_GetProcessTimes(HANDLE process_handle, FILETIME &creation_time,
	FILETIME &exit_time, FILETIME &kernel_time, FILETIME &user_time)
{
	OS_GetProcessTimes(process_handle, &creation_time, &exit_time, &kernel_time,
		&user_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OS_GetProcessTimes(HANDLE process_handle,
	MLB::Utility::TimeSpec &creation_time, MLB::Utility::TimeSpec &exit_time,
	MLB::Utility::TimeSpec &kernel_time, MLB::Utility::TimeSpec &user_time)
{
	FILETIME tmp_creation_time;
	FILETIME tmp_exit_time;
	FILETIME tmp_kernel_time;
	FILETIME tmp_user_time;

	OS_GetProcessTimes(process_handle, &tmp_creation_time, &tmp_exit_time,
		&tmp_kernel_time, &tmp_user_time);

	FileTimeToTimeSpecAbsolute(tmp_creation_time, creation_time);
	FileTimeToTimeSpecAbsolute(tmp_exit_time,     exit_time);
	FileTimeToTimeSpecInterval(tmp_kernel_time,   kernel_time);
	FileTimeToTimeSpecInterval(tmp_user_time,     user_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OS_ProcessTimes &OS_GetProcessTimes(HANDLE process_handle,
	OS_ProcessTimes &process_times)
{
	OS_GetProcessTimes(process_handle, process_times.creation_time_,
		process_times.exit_time_, process_times.kernel_time_,
		process_times.user_time_);

	return(process_times);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OS_ProcessTimes &OS_GetProcessTimes(OS_ProcessTimes &process_times)
{
	return(OS_GetProcessTimes(::GetCurrentProcess(), process_times));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OS_ProcessTimes OS_GetProcessTimes(HANDLE process_handle)
{
	OS_ProcessTimes process_times;

	return(OS_GetProcessTimes(process_handle, process_times));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OS_ProcessTimes OS_GetProcessTimes()
{
	return(OS_GetProcessTimes(::GetCurrentProcess()));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		OS_ProcessTimes datum(OS_GetProcessTimes());
		std::cout << "Test OS_GetProcessTimes(): " << std::endl;
		std::cout << "   Creation Time: " << datum.creation_time_ << std::endl;
		std::cout << "   Exit Time    : " << datum.exit_time_     << std::endl;
		std::cout << "   Kernel Time  : " <<
			datum.kernel_time_.ToStringInterval()                  << std::endl;
		std::cout << "   User Time    : " <<
			datum.user_time_.ToStringInterval()                    << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetProcessTimes.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

