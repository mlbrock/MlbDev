// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Determines the total amount of virtual address space
								available for use by a process.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::MemorySizeType GetVirtualAddressRange(
	MLB::Utility::MemorySizeType &min_address,
	MLB::Utility::MemorySizeType &max_address)
{
#ifdef _Windows
	SYSTEM_INFO datum;

	OS_GetSystemInfo(datum);

	min_address = static_cast<MLB::Utility::MemorySizeType>(
		reinterpret_cast<ULONG_PTR>(datum.lpMinimumApplicationAddress));
	max_address = static_cast<MLB::Utility::MemorySizeType>(
		reinterpret_cast<ULONG_PTR>(datum.lpMaximumApplicationAddress));

	if (max_address < min_address)
		MLB::Utility::ThrowException("Attempt to retrieve the total virtual "
			"address range available by calling '::GetSystemInfo()' "
			"resulted in a 'lpMaximumApplicationAddress' member (" +
			MLB::Utility::AnyToString(max_address) + ") which is "
			"less than the 'lpMinimumApplicationAddress' member (" +
			MLB::Utility::AnyToString(min_address) + ").");

	return(max_address - min_address);
#else
# error "Compilation of 'OSSupport/GetVirtualAddressRange.cpp' is not yet supported under this operating system."
#endif // #ifdef _Windows
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::MemorySizeType GetVirtualAddressRange()
{
	MLB::Utility::MemorySizeType min_address;
	MLB::Utility::MemorySizeType max_address;

	return(GetVirtualAddressRange(min_address, max_address));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

