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
								GetProcessIoCounters().

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

#include <OSSupport_Win.h>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

//	////////////////////////////////////////////////////////////////////////////
IO_COUNTERS *OS_GetProcessIoCounters(HANDLE process_handle,
	IO_COUNTERS *datum_ptr)
{
	if (::GetProcessIoCounters(process_handle, datum_ptr) == 0)
		MLB::Utility::ThrowSystemError("Call to 'GetProcessIoCounters()' "
			"for process handle " +
			MLB::Utility::ValueToStringHex(process_handle) + " failed");

	return(datum_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
IO_COUNTERS &OS_GetProcessIoCounters(HANDLE process_handle, IO_COUNTERS &datum)
{
	return(*OS_GetProcessIoCounters(process_handle, &datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
IO_COUNTERS &OS_GetProcessIoCounters(IO_COUNTERS &datum)
{
	return(OS_GetProcessIoCounters(::GetCurrentProcess(), datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
IO_COUNTERS OS_GetProcessIoCounters(HANDLE process_handle)
{
	IO_COUNTERS datum;

	return(OS_GetProcessIoCounters(process_handle, datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
IO_COUNTERS OS_GetProcessIoCounters()
{
	return(OS_GetProcessIoCounters(::GetCurrentProcess()));
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
		IO_COUNTERS datum(OS_GetProcessIoCounters());
#pragma warning(disable:4244)
		std::cout << "Test OS_GetProcessIoCounters(): " << std::endl;
		std::cout << "   ReadOperationCount : " <<
			std::setw(22) << datum.ReadOperationCount  << std::endl;
		std::cout << "   WriteOperationCount: " <<
			std::setw(22) << datum.WriteOperationCount << std::endl;
		std::cout << "   OtherOperationCount: " <<
			std::setw(22) << datum.OtherOperationCount << std::endl;
		std::cout << "   ReadTransferCount  : " <<
			std::setw(22) << datum.ReadTransferCount   << std::endl;
		std::cout << "   WriteTransferCount : " <<
			std::setw(22) << datum.WriteTransferCount  << std::endl;
		std::cout << "   OtherTransferCount : " <<
			std::setw(22) << datum.OtherTransferCount  << std::endl;
#pragma warning(default:4244)
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetProcessIoCounters.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

