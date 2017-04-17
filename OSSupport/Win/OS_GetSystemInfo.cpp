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
								GetSystemInfo().

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2017.
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

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
SYSTEM_INFO &OS_GetSystemInfo(SYSTEM_INFO &datum)
{
	::GetSystemInfo(&datum);

	return(datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SYSTEM_INFO OS_GetSystemInfo()
{
	SYSTEM_INFO datum;

	return(OS_GetSystemInfo(datum));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ValueToStringRadix.hpp>

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		SYSTEM_INFO datum(OS_GetSystemInfo());
		std::cout << "Test OS_GetSystemInfo(): " << std::endl;
		std::cout << "   wProcessorArchitecture     : " <<
			std::setw(22) << datum.wProcessorArchitecture << std::endl;
		std::cout << "   wReserved                  : " <<
			std::setw(22) << datum.wReserved << std::endl;
		std::cout << "   dwPageSize                 : " <<
			std::setw(22) << datum.dwPageSize << std::endl;
		std::cout << "   lpMinimumApplicationAddress: " <<
			std::setw(22) <<
			static_cast<MLB::Utility::MemorySizeType>(reinterpret_cast<ULONG_PTR>(
			datum.lpMinimumApplicationAddress)) << " = " <<
			MLB::Utility::ValueToStringHex(
			static_cast<MLB::Utility::MemorySizeType>(reinterpret_cast<ULONG_PTR>(
			datum.lpMinimumApplicationAddress))) << std::endl;
		std::cout << "   lpMaximumApplicationAddress: " <<
			std::setw(22) <<
			static_cast<MLB::Utility::MemorySizeType>(reinterpret_cast<ULONG_PTR>(
			datum.lpMaximumApplicationAddress)) << " = " <<
			MLB::Utility::ValueToStringHex(
			static_cast<MLB::Utility::MemorySizeType>(reinterpret_cast<ULONG_PTR>(
			datum.lpMaximumApplicationAddress))) << std::endl;
		std::cout << "   dwActiveProcessorMask      : " <<
			std::setw(22) <<
			MLB::Utility::ValueToStringBin(datum.dwActiveProcessorMask) <<
			std::endl;
		std::cout << "   dwNumberOfProcessors       : " <<
			std::setw(22) << datum.dwNumberOfProcessors << std::endl;
		std::cout << "   dwProcessorType            : " <<
			std::setw(22) << datum.dwProcessorType << std::endl;
		std::cout << "   dwAllocationGranularity    : " <<
			std::setw(22) << datum.dwAllocationGranularity << std::endl;
		std::cout << "   wProcessorLevel            : " <<
			std::setw(22) << datum.wProcessorLevel << std::endl;
		std::cout << "   wProcessorRevision         : " <<
			std::setw(22) << datum.wProcessorRevision << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetSystemInfo.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

