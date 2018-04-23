//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of portable process page locking.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

								What is a trivial wrapper function when implemented
								for the various Unixes becomes a bit hairy when ported
								to Windows.

								There is no page locking as such using the documented
								Windows APIs. What one can do is specify that pages are
								locked within the working set for a process --- which
								is a guarantee that the page will be resident in RAM
								only while some thread of the process is running. At
								any time no threads of a process are running, no
								guarantees concerning the memory residency of a page
								are made by the Windows.

								An option would be to gate to the kernel-mode function
								MmProbeAndLockPages()... But if you use that and don't
								unlock before exiting the process, you blue-screen the
								machine.

								Users of this interface shouldn't bother using the
								unlock facility (MemUnlockProcess) under Windows.
								Because there's no wasy way to determine whether a
								particular section has been locked, the function
								(rather simplistically, I'll admit) invokes
								VirtualUnlock() on every section which would have met
								the criteria for being locked (see the N.B., below).
								If there are pages in a section which have not been
								locked by a previous call to VirtualLock(), the call to
								VirtualUnlock() will remove them from the working set.
								Which result may not be what one intended. Not
								unlocking matters very little, as the pages will be
								unlocked at the end-of-life of the working set (that
								is, when the process exits).

								Nota Bene:
									According to the MSDN documentation, only committed
									pages (MEM_COMMIT = 0x1000) which are not marked as
									as inaccessible (PAGE_NOACCESS = 0x01) can be locked.

									Some experimentation (Windows 2000) has shown there
									are additonal section types which also can't be
									locked:

										1) Guard pages (PAGE_GUARD = 0x100)

										2) Pages with read-only protection set on
											(PAGE_READONLY = 0x02) which are also private
											(MEM_PRIVATE = 0x20000).

		Copyright Michael L. Brock 1993 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

#include <algorithm>

#ifdef _Windows
# include <iomanip>
#else
# include <mman.h>
#endif // ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

#ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
bool CanLockSection(const MEMORY_BASIC_INFORMATION &page_info)
{
	return((page_info.State & MEM_COMMIT) &&
		(!(page_info.Protect & PAGE_NOACCESS)) &&
		(!(page_info.Protect & PAGE_GUARD)) &&
		((!(page_info.Protect & PAGE_READONLY)) ||
		(!(page_info.Type & MEM_PRIVATE))));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string DescribePageInfo(const MEMORY_BASIC_INFORMATION &page_info)
{
	std::ostringstream out_text;
	SYSTEM_INFO        system_data;

	GetSystemInfo(&system_data);

  	out_text <<
		"Base Address=" << ValueToStringHex(page_info.BaseAddress) << ", " <<
		"Allocation Base=" << ValueToStringHex(page_info.AllocationBase) <<
			", " <<
		"Region Size=" << static_cast<unsigned long>(page_info.RegionSize) <<
			" (" <<
			((page_info.RegionSize % system_data.dwPageSize) ? "ERR" : "OK") <<
			"), " <<
		"Page State=" << ((page_info.State == MEM_COMMIT) ? "COMMIT " :
			(page_info.State == MEM_FREE)    ? "FREE   " :
			(page_info.State == MEM_RESERVE) ? "RESERVE" : "???????") << "=" <<
			ValueToStringHex(page_info.State) << ", " <<
		"Page Protection=" << ValueToStringHex(page_info.Protect) << ", " <<
		"Page Type=" << ValueToStringHex(page_info.Type) << " [" <<
		((CanLockSection(page_info)) ? "L" : " ") << "]";

	return(out_text.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SIZE_T GetMaxAddressableSize()
{
	SYSTEM_INFO system_data;

	GetSystemInfo(&system_data);

	return(static_cast<SIZE_T>(
		(static_cast<const char *>(system_data.lpMaximumApplicationAddress) -
		 static_cast<const char *>(system_data.lpMinimumApplicationAddress)) + 1));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::pair<unsigned int, unsigned int> ProcessPageSizes;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
enum TraverseProcessPagesAction {
	TraverseProcessPagesAction_Lock   = 0,
	TraverseProcessPagesAction_Unlock = 1,
	TraverseProcessPagesAction_Total  = 2
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ProcessPageSizes TraverseProcessPages(TraverseProcessPagesAction action_type,
	bool continue_flag = false)
{
	SYSTEM_INFO system_data;

	GetSystemInfo(&system_data);

	LPCVOID      min_address   = system_data.lpMinimumApplicationAddress;
	LPCVOID      max_address   = system_data.lpMaximumApplicationAddress;

	unsigned int total_size    = 0;
	unsigned int lockable_size = 0;

	while (min_address < max_address) {
		MEMORY_BASIC_INFORMATION page_info;
		SIZE_T                   buffer_size;
		buffer_size = VirtualQuery(min_address, &page_info, sizeof(page_info));
		if (buffer_size != sizeof(page_info)) {
			std::ostringstream error_text;
			error_text << "Invocation of 'VirtualQuery(" <<
				ValueToStringHex(min_address) << ", " <<
				ValueToStringHex(&page_info) << ", " << sizeof(page_info) <<
				") returned a 'MEMORY_BASIC_INFORMATION' structure size of " <<
				static_cast<unsigned int>(buffer_size) <<
				", but the actual size is " << sizeof(page_info) << ".";
			ThrowLogicError(error_text);
		}
		if (!page_info.RegionSize) {
			std::ostringstream error_text;
			error_text << "Invocation of 'VirtualQuery(" <<
				ValueToStringHex(min_address) << ", " <<
				ValueToStringHex(&page_info) << ", " << sizeof(page_info) <<
				") returned a 'MEMORY_BASIC_INFORMATION' structure which "
				"contains a 'RegionSize' member equal to zero (0).";
			ThrowLogicError(error_text);
		}
		total_size  += static_cast<unsigned int>(page_info.RegionSize);
		if (CanLockSection(page_info)) {
			//	Did it this way to make my debugging a bit easier... it's not as
			//	if this function is called very often.
			try {
				if (action_type == TraverseProcessPagesAction_Lock) {
					if (VirtualLock(const_cast<LPVOID>(min_address),
						page_info.RegionSize) == 0)
						ThrowSystemError("Invocation of 'VirtualLock()' failed for " +
							DescribePageInfo(page_info));
				}
				else if (action_type == TraverseProcessPagesAction_Unlock) {
					if (VirtualLock(const_cast<LPVOID>(min_address),
						page_info.RegionSize) == 0)
						ThrowSystemError("Invocation of 'VirtualUnlock()' failed for " +
							DescribePageInfo(page_info));
				}
				lockable_size += static_cast<unsigned int>(page_info.RegionSize);
			}
			catch (const std::exception &) {
				if (!continue_flag)
					throw;
			}
		}
		min_address = static_cast<LPCVOID>(
			static_cast<const char *>(min_address) + page_info.RegionSize);
	}

	return(ProcessPageSizes(total_size, lockable_size));
}
//	////////////////////////////////////////////////////////////////////////////
#endif // ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
void MemLockProcess(bool continue_flag)
{
	try {
#ifdef _Windows
		//	Determine the total size of all lockable sections...
		ProcessPageSizes results =
			TraverseProcessPages(TraverseProcessPagesAction_Total, continue_flag);
		SIZE_T needed_size      = results.second;
		//	Get the maximum number of bytes addressable by this process...
		SIZE_T addressable_size = GetMaxAddressableSize();
		// The call into the logic to lock the pages may result in expansion of
		//	a section. To avoid potential problems, we therefore add the system
		//	allocation granularity to the needed size (assuming we aren't within
		//	an allocation granularity of the maximum VM space, of course)...
		if ((needed_size + static_cast<SIZE_T>(GetPageAllocGranularitySize())) <=
			addressable_size)
			needed_size += static_cast<SIZE_T>(GetPageAllocGranularitySize());
		//	Now we get the current working set boundaries. Regardless of the
		//	value of the 'continue_flag' parameter, we throw if this fails...
		SIZE_T min_set_size;
		SIZE_T max_set_size;
		if (GetProcessWorkingSetSize(GetCurrentProcess(), &min_set_size,
			&max_set_size) == 0)
			ThrowSystemError("Invocation of 'GetProcessWorkingSetSize()' failed");
		//	If the needed size is outside of the working set boundaries, we ask
		//	the OS to expand our working set...
		if (needed_size > std::min(min_set_size, max_set_size)) {
			min_set_size = std::max(needed_size, min_set_size);
			max_set_size = std::max(needed_size, max_set_size);
			if ((SetProcessWorkingSetSize(GetCurrentProcess(),
				min_set_size, max_set_size) == 0) && (!continue_flag))
				ThrowSystemError("Invocation of 'SetProcessWorkingSetSize(" +
					ValueToStringHex(GetCurrentProcess()) + ", " +
					AnyToString(static_cast<unsigned int>(min_set_size)) + ", " +
					AnyToString(static_cast<unsigned int>(max_set_size)) +
					") failed");
		}
		TraverseProcessPages(TraverseProcessPagesAction_Lock, continue_flag);
#else
		if (mlockall(MCL_CURRENT))
			ThrowErrnoError("Invocation of mlockall() failed");
#endif // ifdef _Windows
	}
	catch (const std::exception &except) {
		std::ostringstream error_text;
		error_text << "Call to MemLockProcess(" << continue_flag <<
			") failed: " << except.what();
		Rethrow(except, error_text);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MemUnlockProcess(bool continue_flag)
{
	try {
#ifdef _Windows
		TraverseProcessPages(TraverseProcessPagesAction_Unlock, continue_flag);
#else
		if (munlockall())
			ThrowErrnoError("Invocation of munlockall() failed");
	}
#endif // ifdef _Windows
	}
	catch (const std::exception &except) {
		std::ostringstream error_text;
		error_text << "Call to MemUnlockProcess(" << continue_flag <<
			") failed: " << except.what();
		Rethrow(except, error_text);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MemLockArea(const void *area_ptr, unsigned int area_length)
{
	try {
		if (!area_length)
			ThrowStdException("Area length parameter was zero (0).");
#ifdef _Windows
		if (VirtualLock(const_cast<void *>(area_ptr), area_length) == 0)
			ThrowSystemError("Invocation of 'VirtualLock()' failed");
#else
		if (mlock(area_ptr, area_length))
			ThrowErrnoError("Invocation of mlockall() failed");
#endif // ifdef _Windows
	}
	catch (const std::exception &except) {
		std::ostringstream error_text;
		error_text << "Call to MemLockArea(" << ValueToStringHex(area_ptr) <<
			", " << area_length << ") failed: " << except.what();
		Rethrow(except, error_text);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MemUnlockArea(const void *area_ptr, unsigned int area_length)
{
	try {
		if (!area_length)
			ThrowStdException("Area length parameter was zero (0).");
#ifdef _Windows
		if (VirtualUnlock(const_cast<void *>(area_ptr), area_length) == 0)
			ThrowSystemError("Invocation of 'VirtualUnlock()' failed");
#else
		if (munlock(area_ptr, area_length))
			ThrowErrnoError("Invocation of munlockall() failed");
#endif // ifdef _Windows
	}
	catch (const std::exception &except) {
		std::ostringstream error_text;
		error_text << "Call to MemUnlockArea(" << ValueToStringHex(area_ptr) <<
			", " << area_length << ") failed: " << except.what();
		Rethrow(except, error_text);
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::Utility;

#ifdef _Windows
/*
	Some useful info:
	---- ------ -----

#define SECTION_QUERY       0x0001
#define SECTION_MAP_WRITE   0x0002
#define SECTION_MAP_READ    0x0004
#define SECTION_MAP_EXECUTE 0x0008
#define SECTION_EXTEND_SIZE 0x0010

#define SECTION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SECTION_QUERY|\
                            SECTION_MAP_WRITE |      \
                            SECTION_MAP_READ |       \
                            SECTION_MAP_EXECUTE |    \
                            SECTION_EXTEND_SIZE)
#define PAGE_NOACCESS          0x01     
#define PAGE_READONLY          0x02     
#define PAGE_READWRITE         0x04     
#define PAGE_WRITECOPY         0x08     
#define PAGE_EXECUTE           0x10     
#define PAGE_EXECUTE_READ      0x20     
#define PAGE_EXECUTE_READWRITE 0x40     
#define PAGE_EXECUTE_WRITECOPY 0x80     
#define PAGE_GUARD            0x100     
#define PAGE_NOCACHE          0x200     
#define PAGE_WRITECOMBINE     0x400     
#define MEM_COMMIT           0x1000     
#define MEM_RESERVE          0x2000     
#define MEM_DECOMMIT         0x4000     
#define MEM_RELEASE          0x8000     
#define MEM_FREE            0x10000     
#define MEM_PRIVATE         0x20000     
#define MEM_MAPPED          0x40000     
#define MEM_RESET           0x80000     
#define MEM_TOP_DOWN       0x100000     
#define MEM_WRITE_WATCH    0x200000     
#define MEM_PHYSICAL       0x400000     
#define MEM_LARGE_PAGES  0x20000000     
#define MEM_4MB_PAGES    0x80000000     
#define SEC_FILE           0x800000     
#define SEC_IMAGE         0x1000000     
#define SEC_RESERVE       0x4000000     
#define SEC_COMMIT        0x8000000     
#define SEC_NOCACHE      0x10000000     
#define MEM_IMAGE         SEC_IMAGE     
#define WRITE_WATCH_FLAG_RESET 0x01     
*/
//	////////////////////////////////////////////////////////////////////////////
void TEST_TraversePages()
{
	SYSTEM_INFO system_data;

	GetSystemInfo(&system_data);

	DWORD   page_size   = system_data.dwPageSize;
	LPCVOID min_address = system_data.lpMinimumApplicationAddress;
	LPCVOID max_address = system_data.lpMaximumApplicationAddress;

	SIZE_T  total_size  = 0;

	std::cout << std::setfill('=') << std::setw(78) << "" << std::setfill(' ') <<
		std::endl;

	while (min_address < max_address) {
		MEMORY_BASIC_INFORMATION page_info;
		SIZE_T                   buffer_size;
		buffer_size = VirtualQuery(min_address, &page_info, sizeof(page_info));
		if (buffer_size != sizeof(page_info)) {
			std::ostringstream error_text;
			error_text << "Invocation of 'VirtualQuery(" <<
				ValueToStringHex(min_address) << ", " <<
				ValueToStringHex(&page_info) << ", " << sizeof(page_info) <<
				") returned a 'MEMORY_BASIC_INFORMATION' structure size of " <<
				static_cast<unsigned int>(buffer_size) <<
				", but the actual size is " << sizeof(page_info) << ".";
			ThrowLogicError(error_text);
		}
		if (!page_info.RegionSize) {
			std::ostringstream error_text;
			error_text << "Invocation of 'VirtualQuery(" <<
				ValueToStringHex(min_address) << ", " <<
				ValueToStringHex(&page_info) << ", " << sizeof(page_info) <<
				") returned a 'MEMORY_BASIC_INFORMATION' structure which "
				"contains a 'RegionSize' member equal to zero (0).";
			ThrowLogicError(error_text);
		}
		std::cout << ValueToStringHex(page_info.BaseAddress) << " " <<
			ValueToStringHex(page_info.AllocationBase) << " " <<
			std::setw(10) << static_cast<unsigned long>(page_info.RegionSize) <<
				" " <<
			((page_info.RegionSize % page_size) ? "ERR" : "OK ") << " " <<
			((page_info.State == MEM_COMMIT) ? "COMMIT " :
			(page_info.State == MEM_FREE)    ? "FREE   " :
			(page_info.State == MEM_RESERVE) ? "RESERVE" : "???????") << "=" <<
			ValueToStringHex(page_info.State) << " " <<
			ValueToStringHex(page_info.Protect) << " " <<
			ValueToStringHex(page_info.Type) <<
			((CanLockSection(page_info)) ? "" : "*") << std::endl;
		total_size  += page_info.RegionSize;
		min_address  = static_cast<LPCVOID>(
			static_cast<const char *>(min_address) + page_info.RegionSize);
	}

	std::cout << "                      ----------" << std::endl;
	std::cout << "                      " << std::setw(10) <<
		static_cast<unsigned long>(total_size) << std::endl;
	std::cout << "                      ==========" << std::endl;

	std::cout << std::setfill('=') << std::setw(78) << "" << std::setfill(' ') <<
		std::endl << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////
#endif // ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test routine for class 'MemLockProcess'" << std::endl;
	std::cout << "---- ------- --- ----- ----------------" << std::endl;

	try {
#ifdef _Windows
		ProcessPageSizes results;
		TEST_TraversePages();
		results = TraverseProcessPages(TraverseProcessPagesAction_Total, true);
		std::cout << "                      " <<
			std::setw(10) << results.first << " " <<
			std::setw(10) << results.second << std::endl;
		std::cout << "                      ========== ==========" << std::endl;
		MemLockProcess(true);
		TEST_TraversePages();
		results = TraverseProcessPages(TraverseProcessPagesAction_Total, true);
		std::cout << "                      " <<
			std::setw(10) << results.first << " " <<
			std::setw(10) << results.second << std::endl;
		std::cout << "                      ========== ==========" << std::endl;
#else
		MemLockProcess(true);
#endif // ifdef _Windows
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl;
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

