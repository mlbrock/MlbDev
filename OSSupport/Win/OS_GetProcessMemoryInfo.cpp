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
								GetProcessMemoryInfo().

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

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

//	////////////////////////////////////////////////////////////////////////////
PROCESS_MEMORY_COUNTERS *OS_GetProcessMemoryInfo(HANDLE process_handle,
	PROCESS_MEMORY_COUNTERS *datum_ptr)
{
	typedef COMPAT_FN_TYPE(BOOL (WINAPI *OS_FPtr_GetProcessMemoryInfo),
		(FARPROC, PROCESS_MEMORY_COUNTERS *, DWORD));

	HMODULE module_handle = EnsureLoadedLibrary("psapi", true);

#pragma warning(disable:4191)
	OS_FPtr_GetProcessMemoryInfo proc_addr =
		reinterpret_cast<OS_FPtr_GetProcessMemoryInfo>(OS_GetProcAddress(
		module_handle, "GetProcessMemoryInfo", true));
#pragma warning(default:4191)

	if ((*proc_addr)(((FARPROC) process_handle), datum_ptr,
		sizeof(*datum_ptr)) == 0)
		MLB::Utility::ThrowSystemError("Call to 'GetProcessMemoryInfo()' "
			"for process handle " +
			MLB::Utility::ValueToStringHex(process_handle) + " failed");

	return(datum_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
PROCESS_MEMORY_COUNTERS &OS_GetProcessMemoryInfo(HANDLE process_handle,
	PROCESS_MEMORY_COUNTERS &datum)
{
	return(*OS_GetProcessMemoryInfo(process_handle, &datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
PROCESS_MEMORY_COUNTERS &OS_GetProcessMemoryInfo(PROCESS_MEMORY_COUNTERS &datum)
{
	return(OS_GetProcessMemoryInfo(::GetCurrentProcess(), datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
PROCESS_MEMORY_COUNTERS OS_GetProcessMemoryInfo(HANDLE process_handle)
{
	PROCESS_MEMORY_COUNTERS datum;

	return(OS_GetProcessMemoryInfo(process_handle, datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
PROCESS_MEMORY_COUNTERS OS_GetProcessMemoryInfo()
{
	return(OS_GetProcessMemoryInfo(::GetCurrentProcess()));
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
		PROCESS_MEMORY_COUNTERS datum(OS_GetProcessMemoryInfo());
#pragma warning(disable:4244)
		std::cout << "Test OS_GetProcessMemoryInfo(): " << std::endl;
		std::cout << "   cb                         : " <<
			datum.cb                                  << std::endl;
		std::cout << "   PageFaultCount             : " <<
			datum.PageFaultCount                      << std::endl;
		std::cout << "   PeakWorkingSetSize         : " <<
			datum.PeakWorkingSetSize                  << std::endl;
		std::cout << "   WorkingSetSize             : " <<
			datum.WorkingSetSize                      << std::endl;
		std::cout << "   QuotaPeakPagedPoolUsage    : " <<
			datum.QuotaPeakPagedPoolUsage             << std::endl;
		std::cout << "   QuotaPagedPoolUsage        : " <<
			datum.QuotaPagedPoolUsage                 << std::endl;
		std::cout << "   QuotaPeakNonPagedPoolUsage : " <<
			datum.QuotaPeakNonPagedPoolUsage          << std::endl;
		std::cout << "   QuotaNonPagedPoolUsage     : " <<
			datum.QuotaNonPagedPoolUsage              << std::endl;
		std::cout << "   PagefileUsage              : " <<
			datum.PagefileUsage                       << std::endl;
		std::cout << "   PeakPagefileUsage          : " <<
			datum.PeakPagefileUsage                   << std::endl;
#pragma warning(default:4244)
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetProcessMemoryInfo.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

