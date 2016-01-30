// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Operating system support for the Windows dbghelp.dll.

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

#include <OSSupport_Win_DbgHelp.h>

#include <Utility/EnumFlagOps.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

namespace {
// ////////////////////////////////////////////////////////////////////////////
DbgHelpCallLockType DbgHelpCallLock;
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

namespace {

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Mapping of DbgHelp.dll versions to mini-dump features...
// ////////////////////////////////////////////////////////////////////////////
/*
   DbgHelp.dll Analysis
   ----------- --------

   =================== ============ ============ ============ ============
        Version Number    Int Valid  Int Invalid    Ext Valid  Ext Invalid
   ------------------- ------------ ------------ ------------ ------------
            5.0.2195.1   0x00000000   0xffffffff   0x00000000   0xffffffff
         5.2.3790.1830   0x000007ff   0xfffff800   0x000007df   0xfffff820
              6.0.17.0   0x000003ff   0xfffffc00   0x000002ff   0xfffffd00
              6.2.13.1   0x00001fff   0xffffe000   0x00001fff   0xffffe000
              6.3.17.0   0x00007fff   0xffff8000   0x00007fff   0xffff8000
               6.5.3.7   0x0000ffff   0xffff0000   0x0000fffe   0xffff0001
               6.7.5.0   0x0000ffff   0xffff0000   0x0000ffff   0xffff0000
               6.7.5.1   0x0000ffff   0xffff0000   0x0000ffff   0xffff0000
   =================== ============ ============ ============ ============

*/

/**
	Contains a list mapping dbghelp version numbers to a mask of supported
	bits in the flags enumeration \e MINIDUMP_TYPE .

	\note This information is not canonical; it is the result of guesses
	and testing using versions of the dbghelp.dll I had access to at the time
	of this writing.

	\note Should you have need to add elements to this array, be sure to add
	them in ascending order by version number.
*/
// ////////////////////////////////////////////////////////////////////////////
const MiniDumpTypeVersionMaskMap MiniDumpTypeList[] = {
   { MLB::Utility::VersionNumber(    0,    0,    0,    0), static_cast<unsigned int>(0x00000000), static_cast<unsigned int>(0xffffffff), static_cast<unsigned int>(0x00000000), static_cast<unsigned int>(0xffffffff) },
   { MLB::Utility::VersionNumber(    5,    0, 2195,    1), static_cast<unsigned int>(0x00000000), static_cast<unsigned int>(0xffffffff), static_cast<unsigned int>(0x00000000), static_cast<unsigned int>(0xffffffff) },
   { MLB::Utility::VersionNumber(    5,    2, 3790, 1830), static_cast<unsigned int>(0x000007ff), static_cast<unsigned int>(0xfffff800), static_cast<unsigned int>(0x000007df), static_cast<unsigned int>(0xfffff820) },
   { MLB::Utility::VersionNumber(    6,    0,   17,    0), static_cast<unsigned int>(0x000003ff), static_cast<unsigned int>(0xfffffc00), static_cast<unsigned int>(0x000002ff), static_cast<unsigned int>(0xfffffd00) },
   { MLB::Utility::VersionNumber(    6,    2,   13,    1), static_cast<unsigned int>(0x00001fff), static_cast<unsigned int>(0xffffe000), static_cast<unsigned int>(0x00001fff), static_cast<unsigned int>(0xffffe000) },
   { MLB::Utility::VersionNumber(    6,    3,   17,    0), static_cast<unsigned int>(0x00007fff), static_cast<unsigned int>(0xffff8000), static_cast<unsigned int>(0x00007fff), static_cast<unsigned int>(0xffff8000) },
   { MLB::Utility::VersionNumber(    6,    5,    3,    7), static_cast<unsigned int>(0x0000ffff), static_cast<unsigned int>(0xffff0000), static_cast<unsigned int>(0x0000fffe), static_cast<unsigned int>(0xffff0001) },
   { MLB::Utility::VersionNumber(    6,    7,    5,    0), static_cast<unsigned int>(0x0000ffff), static_cast<unsigned int>(0xffff0000), static_cast<unsigned int>(0x0000ffff), static_cast<unsigned int>(0xffff0000) },
   { MLB::Utility::VersionNumber(    6,    7,    5,    1), static_cast<unsigned int>(0x0000ffff), static_cast<unsigned int>(0xffff0000), static_cast<unsigned int>(0x0000ffff), static_cast<unsigned int>(0xffff0000) }
};

const unsigned int               MiniDumpTypeCount  =
sizeof(MiniDumpTypeList) / sizeof(MiniDumpTypeList[0]);
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
DbgHelpCallLockType &GetDbgHelpCallLockRef()
{
	return(DbgHelpCallLock);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MiniDumpTypeVersionMaskMap GetMiniDumpTypeMaskForVersion(
	const MLB::Utility::VersionNumber &version_number)
{
	unsigned int count_1;

	for (count_1 = 1; count_1 < MiniDumpTypeCount; ++count_1) {
		if (version_number < MiniDumpTypeList[count_1].version_number_)
			return(MiniDumpTypeList[count_1 - 1]);
		else if (version_number == MiniDumpTypeList[count_1].version_number_)
			return(MiniDumpTypeList[count_1]);
	}

	return(MiniDumpTypeList[MiniDumpTypeCount - 1]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OS_FPtr_MiniDumpWriteDump Get_OS_FPtr_MiniDumpWriteDump(bool require_proc)
{
	DllLoader dll_loader("dbghelp",
		MLB::Utility::EnumFlagOr(DllLoader::LoadOnCreation,
		DllLoader::PassIfAlreadyLoaded), true);

#pragma warning(disable:4191)
	return(reinterpret_cast<OS_FPtr_MiniDumpWriteDump>(
		OS_GetProcAddress(dll_loader.GetDllHandle(), "MiniDumpWriteDump",
		require_proc)));
#pragma warning(default:4191)
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OS_MiniDumpWriteDump(OS_FPtr_MiniDumpWriteDump write_dump_proc_addr,
	HANDLE process_handle, DWORD process_id,
	HANDLE dump_handle, MINIDUMP_TYPE dump_flags,
	MINIDUMP_EXCEPTION_INFORMATION *exception_type_ptr,
	MINIDUMP_USER_STREAM_INFORMATION *user_stream_ptr,
	MINIDUMP_CALLBACK_INFORMATION *callback_ptr)
{
	write_dump_proc_addr = (write_dump_proc_addr != NULL) ?
		write_dump_proc_addr : Get_OS_FPtr_MiniDumpWriteDump(true);

	BOOL dump_code;

	{
		DbgHelpCallScopedLockType my_lock(GetDbgHelpCallLockRef());
		dump_code = (*write_dump_proc_addr)(process_handle, process_id,
			dump_handle, dump_flags, exception_type_ptr, user_stream_ptr,
			callback_ptr);
	}

	if (dump_code != TRUE)
		MLB::Utility::ThrowSystemError("Invocation of 'MiniDumpWriteDump(" +
			MLB::Utility::ValueToStringHex(process_handle) + ", " +
			MLB::Utility::AnyToString(process_id) + ", " +
			MLB::Utility::ValueToStringHex(dump_handle) + ", " +
			MLB::Utility::ValueToStringHex(dump_flags) + " = " +
			MLB::Utility::ValueToStringBin(dump_flags) + ", " +
			MLB::Utility::ValueToStringHex(exception_type_ptr) + ", " +
			MLB::Utility::ValueToStringHex(user_stream_ptr) + ", " +
			MLB::Utility::ValueToStringHex(callback_ptr) + ") failed");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OS_MiniDumpWriteDump(HANDLE process_handle, DWORD process_id,
	HANDLE dump_handle, MINIDUMP_TYPE dump_flags,
	MINIDUMP_EXCEPTION_INFORMATION *exception_type_ptr,
	MINIDUMP_USER_STREAM_INFORMATION *user_stream_ptr,
	MINIDUMP_CALLBACK_INFORMATION *callback_ptr)
{
	OS_MiniDumpWriteDump(NULL, process_handle, process_id, dump_handle,
		dump_flags, exception_type_ptr, user_stream_ptr, callback_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

