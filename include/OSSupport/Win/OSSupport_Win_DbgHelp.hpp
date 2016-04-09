// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the operating system support library
								for the Windows dbghelp.dll.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__OSSupport__OSSupport_Win_DbgHelp_hpp___HH

#define HH___MLB__OSSupport__OSSupport_Win_DbgHelp_hpp___HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport_Win.hpp>
#include <OSSupport_CoreDumperBase.hpp>

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4217 4275 4668 4625 4626)
# if (_MSC_VER >= 1500)
#  pragma warning(disable:4061 4365)
# endif // #if (_MSC_VER >= 1500)
#endif // #ifdef _MSC_VER

#include <boost/thread/recursive_mutex.hpp>

#ifdef _MSC_VER
# pragma warning(pop)
#endif // #ifdef _MSC_VER

#ifndef __out_bcount_opt
# define __out_bcount_opt(x)
#endif // #ifndef __out_bcount_opt

#include <dbghelp.h>

#if API_VERSION_NUMBER < 11
# error "Compilation of 'OSSupport/Win/MiniDumperBase.cpp' requires a 'dbghelp.h' file of at least API_VERSION_NUMBER 11 (version 6.7.5.0)."
#endif // #if API_VERSION_NUMBER < 11

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
typedef COMPAT_FN_TYPE(BOOL (WINAPI *OS_FPtr_MiniDumpWriteDump),
	(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION,
	PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION));
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::recursive_timed_mutex     DbgHelpCallLockType;
typedef DbgHelpCallLockType::scoped_lock DbgHelpCallScopedLockType;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct MiniDumpTypeVersionMaskMap {
	MLB::Utility::VersionNumber version_number_;
	unsigned int                internal_valid_mask_;
	unsigned int                internal_invalid_mask_;
	unsigned int                external_valid_mask_;
	unsigned int                external_invalid_mask_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const int MiniDumperDefaultFlagsNormal =
				  MiniDumpWithDataSegs
				| MiniDumpWithFullMemory
				| MiniDumpWithHandleData
				| MiniDumpWithUnloadedModules
				| MiniDumpWithProcessThreadData
				| MiniDumpWithPrivateReadWriteMemory
				| MiniDumpWithFullMemoryInfo				//	6.7.5.0
				| MiniDumpWithThreadInfo					//	6.7.5.0
				;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_OSSUPPORT DbgHelpCallLockType &GetDbgHelpCallLockRef();

API_OSSUPPORT MiniDumpTypeVersionMaskMap GetMiniDumpTypeMaskForVersion(
	const MLB::Utility::VersionNumber &version_number);

API_OSSUPPORT OS_FPtr_MiniDumpWriteDump Get_OS_FPtr_MiniDumpWriteDump(
	bool require_proc = true);

API_OSSUPPORT void OS_MiniDumpWriteDump(
	OS_FPtr_MiniDumpWriteDump write_dump_proc_addr, HANDLE process_handle,
	DWORD process_id, HANDLE dump_handle, MINIDUMP_TYPE dump_flags,
	MINIDUMP_EXCEPTION_INFORMATION *exception_type_ptr = NULL,
	MINIDUMP_USER_STREAM_INFORMATION *user_stream_ptr = NULL,
	MINIDUMP_CALLBACK_INFORMATION *callback_ptr = NULL);
API_OSSUPPORT void OS_MiniDumpWriteDump(HANDLE process_handle, DWORD process_id,
	HANDLE dump_handle, MINIDUMP_TYPE dump_flags,
	MINIDUMP_EXCEPTION_INFORMATION *exception_type_ptr = NULL,
	MINIDUMP_USER_STREAM_INFORMATION *user_stream_ptr = NULL,
	MINIDUMP_CALLBACK_INFORMATION *callback_ptr = NULL);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT MiniDumperBase : public CoreDumperBase {
public:
	typedef boost::mutex          LockType;
	typedef LockType::scoped_lock ScopedLock;

	explicit MiniDumperBase(const std::string &file_name_base,
		bool is_full_name = false, int dump_flags = MiniDumperDefaultFlagsNormal,
		bool load_highest_version_flag = true,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());
	explicit MiniDumperBase(const char *file_name_base, bool is_full_name = false,
		int dump_flags = MiniDumperDefaultFlagsNormal,
		bool load_highest_version_flag = true,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());
	MiniDumperBase(const std::string &file_name_base,
		const std::string &dir_name, bool is_full_name = false,
		int dump_flags = MiniDumperDefaultFlagsNormal,
		bool load_highest_version_flag = true,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());
	MiniDumperBase(const char *file_name_base, const char *dir_name,
		bool is_full_name = false, int dump_flags = MiniDumperDefaultFlagsNormal,
		bool load_highest_version_flag = true,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());
	virtual ~MiniDumperBase();

	MLB::Utility::VersionNumber ReloadDbgHelp(const std::string &dll_name = "",
		bool load_highest_version_flag = true,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());

	std::string PerformMiniDump(EXCEPTION_POINTERS *except_ptrs = NULL,
		int dump_flags = -1);

	static MLB::Utility::VersionNumber LoadDbgHelp(
		const std::string &dll_name = "",
		bool load_highest_version_flag = true,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());

	static void MiniDumpProcess(const std::string &process_id,
		const std::string &dump_file_name,
		int dump_flags = MiniDumperDefaultFlagsNormal,
		void *other_data_ptr = NULL);
	static void MiniDumpProcess(MLB::Utility::ProcessId process_id,
		const std::string &dump_file_name,
		int dump_flags = MiniDumperDefaultFlagsNormal,
		void *other_data_ptr = NULL);

	static void EmitMiniDump(const std::string &file_name,
		int dump_flags = MiniDumperDefaultFlagsNormal,
		EXCEPTION_POINTERS *except_ptrs = NULL,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		MLB::Utility::ThreadId thread_id = MLB::Utility::CurrentThreadId(),
		HANDLE process_handle = ::GetCurrentProcess());
	static void EmitMiniDump(const std::string &file_name,
		OS_FPtr_MiniDumpWriteDump mini_dump_func_ptr,
		int dump_flags = MiniDumperDefaultFlagsNormal,
		EXCEPTION_POINTERS *except_ptrs = NULL,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		MLB::Utility::ThreadId thread_id = MLB::Utility::CurrentThreadId(),
		HANDLE process_handle = ::GetCurrentProcess());
	static int  GetMiniDumpTypeMask();

	static MINIDUMP_TYPE ResolveDumpFlags(int dump_flags);

protected:
	MiniDumpTypeVersionMaskMap dump_flags_mask_;
	HANDLE                     process_handle_;
	MLB::Utility::ProcessId    process_id_;
	OS_FPtr_MiniDumpWriteDump  write_dump_func_ptr_;

	bool        CoreDumpIsSupportedImpl() const;
	std::string PerformCoreDumpImpl(void *other_data_ptr = NULL,
		int dump_flags = -1);
	std::string PerformMiniDumpImpl(EXCEPTION_POINTERS *except_ptrs = NULL,
		int dump_flags = -1);

private:
	MLB_Utility_NonCopyable_Macro(MiniDumperBase);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<MiniDumperBase> MiniDumperBaseSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#endif // #ifndef HH___MLB__OSSupport__OSSupport_Win_DbgHelp_hpp___HH

