// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Determines the amount of virtual address space in use
								by a process.

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
MLB::Utility::MemorySizeType GetVirtualAddressSpaceInUse()
{
#ifdef _Windows
	MEMORYSTATUSEX datum;

	OS_GlobalMemoryStatusEx(datum);

	if (datum.ullTotalVirtual < datum.ullAvailVirtual)
		MLB::Utility::ThrowException("Attempt to retrieve the amount of the "
			"virtual address space in use by calling '::GlobalMemoryStatusEx()' "
			"resulted in a 'ullTotalVirtual' member (" +
			MLB::Utility::AnyToString(datum.ullTotalVirtual) + ") which is "
			"less than the 'ullAvailVirtual' member (" +
			MLB::Utility::AnyToString(datum.ullAvailVirtual) + ").");

	return(datum.ullTotalVirtual - datum.ullAvailVirtual);
#else
# error "Compilation of 'OSSupport/GetVirtualAddressSpaceInUse.cpp' is not yet supported under this operating system."
#endif // #ifdef _Windows
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

