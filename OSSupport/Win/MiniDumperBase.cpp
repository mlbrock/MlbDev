// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the MiniDumperBase class.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2014.
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

#include <Tlhelp32.h>

namespace MLB {
namespace OSSupport {
// ////////////////////////////////////////////////////////////////////////////
HANDLE OS_CreateToolhelp32Snapshot(DWORD flags,
	MLB::Utility::ProcessId process_id)
{
	HANDLE return_handle;

	if ((return_handle = ::CreateToolhelp32Snapshot(flags, process_id)) ==
		INVALID_HANDLE_VALUE)
		MLB::Utility::ThrowSystemError("Call to 'CreateToolhelp32Snapshot(" +
			MLB::Utility::ValueToStringHex(flags) + ", " +
			MLB::Utility::AnyToString(process_id) + ")' failed");

	return(return_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOL OS_Thread32First(HANDLE snapshot_handle, THREADENTRY32 *thread_info)
{
	BOOL return_code = ::Thread32First(snapshot_handle, thread_info);

	if ((return_code != TRUE) && (::GetLastError() != ERROR_NO_MORE_FILES))
		MLB::Utility::ThrowSystemError("Call to 'Thread32First(" +
			MLB::Utility::ValueToStringHex(snapshot_handle) + ", " +
			MLB::Utility::ValueToStringHex(thread_info) + ")' failed");

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOL OS_Thread32First(HANDLE snapshot_handle, THREADENTRY32 &thread_info)
{
	return(OS_Thread32First(snapshot_handle, &thread_info));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOL OS_Thread32Next(HANDLE snapshot_handle, THREADENTRY32 *thread_info)
{
	BOOL return_code = ::Thread32Next(snapshot_handle, thread_info);

	if ((return_code != TRUE) && (::GetLastError() != ERROR_NO_MORE_FILES))
		MLB::Utility::ThrowSystemError("Call to 'Thread32Next(" +
			MLB::Utility::ValueToStringHex(snapshot_handle) + ", " +
			MLB::Utility::ValueToStringHex(thread_info) + ")' failed");

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOL OS_Thread32Next(HANDLE snapshot_handle, THREADENTRY32 &thread_info)
{
	return(OS_Thread32Next(snapshot_handle, &thread_info));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<THREADENTRY32>          ThreadEntry32Vector;
typedef ThreadEntry32Vector::iterator       ThreadEntry32VectorIter;
typedef ThreadEntry32Vector::const_iterator ThreadEntry32VectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ThreadEntry32Vector &GetThreadEntry32VectorForProcess(
	MLB::Utility::ProcessId process_id, ThreadEntry32Vector &thread_entry_list)
{
	ThreadEntry32Vector tmp_thread_entry_list;

	try {
		boost::shared_ptr<void> snapshot_handle(
			OS_CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0), ::CloseHandle);
		THREADENTRY32 thread_entry;
		thread_entry.dwSize = sizeof(thread_entry);
		if (OS_Thread32First(snapshot_handle.get(), thread_entry)) {
			do {
            if (thread_entry.th32OwnerProcessID == process_id)
					tmp_thread_entry_list.push_back(thread_entry);
				thread_entry.dwSize = sizeof(thread_entry);
			} while (OS_Thread32Next(snapshot_handle.get(), thread_entry));
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to retrieve the thread list for "
			"process " + MLB::Utility::AnyToString(process_id) + " failed: " +
			std::string(except.what()));
	}

	thread_entry_list.swap(tmp_thread_entry_list);

	return(thread_entry_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SuspendResumeProcess(const ThreadEntry32Vector &thread_entry_list,
	bool is_resume)
{
	ThreadEntry32VectorIterC iter_b(thread_entry_list.begin());
	ThreadEntry32VectorIterC iter_e(thread_entry_list.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		try {
			boost::shared_ptr<void> thread_handle(
				::OpenThread(THREAD_SUSPEND_RESUME, FALSE, iter_b->th32ThreadID),
				::CloseHandle);
			if (is_resume)
				::ResumeThread(thread_handle.get());
			else
				::SuspendThread(thread_handle.get());
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Attempt to perform a process " +
				std::string((is_resume) ? "resumption" : "suspension") +
				" of process id " +
				MLB::Utility::AnyToString(iter_b->th32OwnerProcessID) +
				" failed: " + std::string(except.what()));
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SuspendResumeProcess(MLB::Utility::ProcessId process_id, bool is_resume)
{
	ThreadEntry32Vector thread_entry_list;

	SuspendResumeProcess(GetThreadEntry32VectorForProcess(process_id,
		thread_entry_list), is_resume);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport
} // namespace MLB

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
MiniDumperBase::MiniDumperBase(const std::string &file_name_base,
	bool is_full_name, int dump_flags, bool load_highest_version_flag,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
	:CoreDumperBase(file_name_base, is_full_name, ResolveDumpFlags(dump_flags))
	,dump_flags_mask_()
	,process_handle_(::GetCurrentProcess())
	,process_id_(MLB::Utility::CurrentProcessId())
	,write_dump_func_ptr_(NULL)
{
	ReloadDbgHelp("dbghelp", load_highest_version_flag, min_version,
		max_version);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MiniDumperBase::MiniDumperBase(const char *file_name_base,
	bool is_full_name, int dump_flags, bool load_highest_version_flag,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
	:CoreDumperBase(file_name_base, is_full_name, ResolveDumpFlags(dump_flags))
	,dump_flags_mask_()
	,process_handle_(::GetCurrentProcess())
	,process_id_(MLB::Utility::CurrentProcessId())
	,write_dump_func_ptr_(NULL)
{
	ReloadDbgHelp("dbghelp", load_highest_version_flag, min_version,
		max_version);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MiniDumperBase::MiniDumperBase(const std::string &file_name_base,
	const std::string &dir_name, bool is_full_name, int dump_flags,
	bool load_highest_version_flag,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
	:CoreDumperBase(file_name_base, dir_name, is_full_name,
		ResolveDumpFlags(dump_flags))
	,dump_flags_mask_()
	,process_handle_(::GetCurrentProcess())
	,process_id_(MLB::Utility::CurrentProcessId())
	,write_dump_func_ptr_(NULL)
{
	ReloadDbgHelp("dbghelp", load_highest_version_flag, min_version,
		max_version);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MiniDumperBase::MiniDumperBase(const char *file_name_base,
	const char *dir_name, bool is_full_name, int dump_flags,
	bool load_highest_version_flag,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
	:CoreDumperBase(file_name_base, dir_name, is_full_name,
		ResolveDumpFlags(dump_flags))
	,dump_flags_mask_()
	,process_handle_(::GetCurrentProcess())
	,process_id_(MLB::Utility::CurrentProcessId())
	,write_dump_func_ptr_(NULL)
{
	ReloadDbgHelp("dbghelp", load_highest_version_flag, min_version,
		max_version);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MiniDumperBase::~MiniDumperBase()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::VersionNumber MiniDumperBase::ReloadDbgHelp(
	const std::string &dll_name, bool load_highest_version_flag,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
{
	DllLoader dll_loader((dll_name.empty()) ? "dbghelp" : dll_name,
		MLB::Utility::EnumFlagOr(DllLoader::LoadOnCreation,
		DllLoader::PassIfAlreadyLoaded), load_highest_version_flag,
		min_version, max_version);

	MLB::Utility::VersionNumber dbghelp_version = dll_loader.GetDllVersion();

	ScopedLock my_lock(GetLockRef());

	dump_flags_mask_ = GetMiniDumpTypeMaskForVersion(dbghelp_version);

#pragma warning(disable:4191)
	write_dump_func_ptr_ = reinterpret_cast<OS_FPtr_MiniDumpWriteDump>(
		OS_GetProcAddress(dll_loader.GetDllHandle(), "MiniDumpWriteDump", true));
#pragma warning(default:4191)

	return(dbghelp_version);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string MiniDumperBase::PerformMiniDump(EXCEPTION_POINTERS *except_ptrs,
	int dump_flags)
{
	ScopedLock my_lock(GetLockRef());

	return(PerformMiniDumpImpl(except_ptrs, dump_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool MiniDumperBase::CoreDumpIsSupportedImpl() const
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string MiniDumperBase::PerformCoreDumpImpl(void *other_data_ptr,
	int dump_flags)
{
	return(PerformMiniDumpImpl(
		static_cast<EXCEPTION_POINTERS *>(other_data_ptr), dump_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string MiniDumperBase::PerformMiniDumpImpl(EXCEPTION_POINTERS *except_ptrs,
	int dump_flags)
{
	dump_flags = (dump_flags < 0) ? dump_flags_ : dump_flags;

	std::string tmp_file_name(GetNextFileNameImpl());

   ++mini_dump_instance_;

	EmitMiniDump(tmp_file_name, write_dump_func_ptr_,
		dump_flags & dump_flags_mask_.internal_valid_mask_, except_ptrs,
		process_id_, MLB::Utility::CurrentThreadId(), process_handle_);

	return(tmp_file_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::VersionNumber MiniDumperBase::LoadDbgHelp(
	const std::string &dll_name, bool load_highest_version_flag,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
{
	return(MiniDumperBase("SomeDumpFileNameBase", true, -1,
		load_highest_version_flag, min_version, max_version).
		ReloadDbgHelp(dll_name, load_highest_version_flag, min_version,
		max_version));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MiniDumperBase::MiniDumpProcess(const std::string &process_id,
	const std::string &dump_file_name, int dump_flags, void *other_data_ptr)
{
	MiniDumpProcess(ResolveProcessSpecifier(process_id.c_str()).first,
		dump_file_name, dump_flags, other_data_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MiniDumperBase::MiniDumpProcess(MLB::Utility::ProcessId process_id,
	const std::string &dump_file_name, int dump_flags, void *other_data_ptr)
{
/*
	HANDLE process_handle;

	if ((process_handle = ::OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ, FALSE, process_id)) == NULL)
		MLB::Utility::ThrowSystemError("Call to OpenProcess() for process id " +
			MLB::Utility::AnyToString(process_id) + " failed");

*/


/*
	boost::shared_ptr<void> handle_sptr_1(
		ResolveProcessIdToHandle(process_id), ::CloseHandle);
*/

	SuspendResumeProcess(process_id, false);

	{
		dump_flags &=
			GetMiniDumpTypeMaskForVersion(DetermineProductVersion("dbghelp")).
			external_valid_mask_;
		boost::shared_ptr<void> handle_sptr_2(::OpenProcess(
//			PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ, FALSE, process_id), ::CloseHandle);
			PROCESS_ALL_ACCESS, FALSE, process_id), ::CloseHandle);
		if (handle_sptr_2.get() == NULL)
			MLB::Utility::ThrowSystemError("Call to 'OpenProcess()' for process "
				"id " + MLB::Utility::AnyToString(process_id) + " failed.");
		EmitMiniDump(dump_file_name, dump_flags,
			static_cast<EXCEPTION_POINTERS *>(other_data_ptr),
			process_id, 0, handle_sptr_2.get());
	}

	SuspendResumeProcess(process_id, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MiniDumperBase::EmitMiniDump(const std::string &file_name,
	int dump_flags, EXCEPTION_POINTERS *except_ptrs,
	MLB::Utility::ProcessId process_id, MLB::Utility::ThreadId thread_id,
	HANDLE process_handle)
{
	EmitMiniDump(file_name, Get_OS_FPtr_MiniDumpWriteDump(true), dump_flags,
		except_ptrs, process_id, thread_id, process_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MiniDumperBase::EmitMiniDump(const std::string &file_name,
	OS_FPtr_MiniDumpWriteDump mini_dump_func_ptr, int dump_flags,
	EXCEPTION_POINTERS *except_ptrs, MLB::Utility::ProcessId process_id,
	MLB::Utility::ThreadId thread_id, HANDLE process_handle)
{
	dump_flags &= GetMiniDumpTypeMask();

	try {
		mini_dump_func_ptr = (mini_dump_func_ptr != NULL) ? mini_dump_func_ptr :
			Get_OS_FPtr_MiniDumpWriteDump(true);
		/*
			Hacked code from the Oleg Starodumov sample 'MaxiDump.cpp' on his
			site www.debuginfo.com.
		*/
		HANDLE dump_handle = ::CreateFile(file_name.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if ((dump_handle == NULL ) || (dump_handle == INVALID_HANDLE_VALUE))
			MLB::Utility::ThrowSystemError("Invocation of 'CreateFile()' failed");
		// Create the minidump
		MINIDUMP_EXCEPTION_INFORMATION mdei;
		mdei.ThreadId          = thread_id;
		mdei.ExceptionPointers = except_ptrs;
		mdei.ClientPointers    =
			(process_id == MLB::Utility::CurrentProcessId()) ? FALSE : true;
		try {
			OS_MiniDumpWriteDump(mini_dump_func_ptr, process_handle, process_id,
				dump_handle, static_cast<MINIDUMP_TYPE>(dump_flags),
				(except_ptrs != NULL) ? &mdei : NULL, NULL, NULL);
		}
		catch (const std::exception &) {
			::CloseHandle(dump_handle);
			throw;
		}
		::CloseHandle(dump_handle);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to create the mini-dump file '" +
			file_name + "' failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int MiniDumperBase::GetMiniDumpTypeMask()
{
	return(GetMiniDumpTypeMaskForVersion(DetermineProductVersion("dbghelp")).
		internal_valid_mask_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MINIDUMP_TYPE MiniDumperBase::ResolveDumpFlags(int dump_flags)
{
	if (dump_flags <= 0)
		return(static_cast<MINIDUMP_TYPE>(MiniDumperDefaultFlagsNormal));

	if (dump_flags & (~GetMiniDumpTypeMask()))
		MLB::Utility::ThrowInvalidArgument("The specified Windows mini-dump "
			"flags (" + MLB::Utility::ValueToStringBin(dump_flags) + " = " +
			MLB::Utility::AnyToString(dump_flags) + ") includes flags (" +
			MLB::Utility::ValueToStringBin(dump_flags & (~GetMiniDumpTypeMask()))+
			" = " + MLB::Utility::AnyToString(dump_flags &
				(~GetMiniDumpTypeMask())) + ") which are not in the valid set of "
			"bits (" + MLB::Utility::ValueToStringBin(GetMiniDumpTypeMask()) +
			" = " + MLB::Utility::AnyToString(GetMiniDumpTypeMask()) + ").");

	return(static_cast<MINIDUMP_TYPE>(dump_flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/LogManager.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668 4625 4626)
#endif // #ifdef _Windows

# include <boost/thread/thread.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668 4625 4626)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Defines the global logging objects...
//	////////////////////////////////////////////////////////////////////////////
LogManagerMacroDefinition(MB_LIB_LOCAL)
//	////////////////////////////////////////////////////////////////////////////

const unsigned int TEST_MilliSecsToWait = 5000;

namespace MLB {
namespace OSSupport {
// ////////////////////////////////////////////////////////////////////////////
class CoreDumpCapableBoostThreadBase {
protected:
	template <typename ActualDumpType>
	CoreDumpCapableBoostThreadBase(const ActualDumpType *,
		MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr,
		bool core_dump_flag = true, void *user_data_ptr = NULL)
		:cdmp_sptr_(CheckCoreDumpSPtr<ActualDumpType>(cdmp_sptr))
		,core_dump_flag_(core_dump_flag)
		,user_data_ptr_(user_data_ptr)
	{
	}
	virtual ~CoreDumpCapableBoostThreadBase()
	{
	}

	virtual void RunThread() = 0;

	MLB::OSSupport::CoreDumperBaseSPtr  cdmp_sptr_;
	bool                                core_dump_flag_;
	void                               *user_data_ptr_;

private:
	template <typename ActualDumpType> static
		MLB::OSSupport::CoreDumperBaseSPtr &CheckCoreDumpSPtr(
		MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr)
	{
		if ((cdmp_sptr != NULL) &&
			(dynamic_cast<ActualDumpType *>(cdmp_sptr.get()) == NULL))
			MLB::Utility::ThrowInvalidArgument("Attempt to create an instance of "
				"'MLB::OSSupport::CoreDumpCapableBoostThreadBase' with a shared "
				"pointer (" + MLB::Utility::ValueToStringHex(cdmp_sptr.get()) +
				") which is not convertible to the designated dereived type.");

		return(cdmp_sptr);
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\brief Thread classes in Windows programs can inherit from this class in
	order to ensure that structured exceptions encountered by the thread will
	result in a mini-dump.

   In order to do so, the thread class in question need only implement:
	\code

//	Canonical --- forwards to the base class method which wraps an exception
//	handler around the call to \e RunThreadImpl() .
void operator() () {
	RunThread();
}

// Called from with the base class exception handler wrapper in \e RunThread() .
void RunThreadImpl() {
	//	... Your logic here...
}
	\endcode
*/
// ////////////////////////////////////////////////////////////////////////////
class MiniDumpCapableBoostThreadBase : public CoreDumpCapableBoostThreadBase {
	typedef MLB::OSSupport::MiniDumperBase MyDumperType;
protected:
	MiniDumpCapableBoostThreadBase(MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr,
		bool core_dump_flag = true, void *user_data_ptr = NULL)
		:CoreDumpCapableBoostThreadBase(static_cast<MyDumperType *>(NULL),
			cdmp_sptr, core_dump_flag, user_data_ptr)
	{
	}
	virtual ~MiniDumpCapableBoostThreadBase()
	{
	}

	virtual void RunThread();
	virtual void RunThreadImpl() = 0;

	virtual void HandleException(MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr,
		EXCEPTION_POINTERS *except_ptrs);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MiniDumpCapableBoostThreadBase::RunThread()
{
	if ((core_dump_flag_) && (cdmp_sptr_ != NULL)) {
		__try {
			RunThreadImpl();
		}
		__except(HandleException(cdmp_sptr_, GetExceptionInformation()),
			EXCEPTION_EXECUTE_HANDLER) {
			LogError << "Exiting thread due to the occurrence of a fatal "
				"error." << std::endl;
		}
	}
	else {
		RunThreadImpl();
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MiniDumpCapableBoostThreadBase::HandleException(
	MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr,
	EXCEPTION_POINTERS *except_ptrs)
{
	if (cdmp_sptr != NULL) {
		std::string dump_file(cdmp_sptr->PerformCoreDump(except_ptrs));
		LogInfo << "Wrote mini-dump file to '" << dump_file << "'" << std::endl;
	}
	else
		LogInfo << "Windows exception occurred, but no mini-dump has been "
			"configured." << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport
} // namespace MLB


#include <Utility/InlineContainer.hpp>
#include <Utility/PathName.hpp>
#include <Utility/ExecProcess.hpp>

#if _MSC_VER >= 1300
# pragma warning(disable:4217 4668)
#endif // #ifdef _MSC_VER >= 1300

#include <boost/shared_array.hpp>

# include <boost/filesystem/operations.hpp>
# include <boost/filesystem/path.hpp>
# include <boost/filesystem/exception.hpp>

#if _MSC_VER >= 1300
# pragma warning(default:4217 4668)
#endif // #ifdef _MSC_VER >= 1300

using namespace MLB::OSSupport;

namespace {
// ////////////////////////////////////////////////////////////////////////////
class FaultInvokerThread : public MiniDumpCapableBoostThreadBase {
public:
	FaultInvokerThread(MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr)
		:MiniDumpCapableBoostThreadBase(cdmp_sptr)
	{
	}

	void operator() () {
		RunThread();
	}

	void RunThreadImpl() {
		::srand(MLB::Utility::CurrentThreadId());
		int wait_msecs = ((::rand() % 10) * 1000) + TEST_MilliSecsToWait;
		std::string test_string("This long(ish) string forces a heap allocation.");
		LogInfo << "Will try to seg fault in " <<
			(static_cast<double>(wait_msecs) / 1000.0) << " seconds..." <<
			std::endl;
		::Sleep(TEST_MilliSecsToWait);
		LogInfo << "******** Before the segmentation fault ********" << std::endl;
		*reinterpret_cast<unsigned int *>(1) = 0x12345678;
		LogInfo << "******** After the segmentation fault  ********" << std::endl;
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InvokeFault()
{
	std::string test_string("This long(ish) string forces a heap allocation.");

	LogInfo << "Will try to seg fault in " <<
		(static_cast<double>(TEST_MilliSecsToWait) / 1000.0) <<
		" seconds..." << std::endl;

	::Sleep(TEST_MilliSecsToWait);

	LogInfo << "******** Before the segmentation fault ********" << std::endl;

	*reinterpret_cast<unsigned int *>(1) = 0x12345678;

	LogInfo << "******** After the segmentation fault  ********" << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void HandleException(MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr,
	EXCEPTION_POINTERS *except_ptrs)
{
	if (cdmp_sptr != NULL) {
		std::string dump_file(cdmp_sptr->PerformCoreDump(except_ptrs));
		LogInfo << "Wrote mini-dump file to '" << dump_file << "'" << std::endl;
	}
	else {
		LogInfo << "Windows exception occurred, but no mini-dump has been "
			"configured." << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int WrappedMain_Logic(MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr,
	const std::vector<std::string> &warnings_list, int, char **)
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		if (cdmp_sptr != NULL) {
			std::cout << "Windows mini-dump facility usage is configured." <<
				std::endl;
			std::cout << "First mini-dump file name will be '" <<
				cdmp_sptr->GetNextFileName() << std::endl;
		}
		else
			std::cout << "Windows Mini-dump facility could not be configured." <<
				std::endl;
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		std::vector<std::string>::const_iterator iter_b(warnings_list.begin());
		std::vector<std::string>::const_iterator iter_e(warnings_list.end());
		for ( ; iter_b != iter_e; ++iter_b)
			std::cout << "LOG_WARNING: " << *iter_b << std::endl;
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		std::cout << std::endl;
		InvokeFault();
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int WrappedMain_Handler(MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr,
	const std::vector<std::string> &warnings_list, int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	__try {
		return_code = WrappedMain_Logic(cdmp_sptr, warnings_list, argc, argv);
	}
	__except(HandleException(cdmp_sptr, GetExceptionInformation()),
		EXCEPTION_EXECUTE_HANDLER) {
		std::cerr << "Exiting program due to the occurrence of a fatal error." <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int WrappedThread_Handler(MLB::OSSupport::CoreDumperBaseSPtr &cdmp_sptr,
	const std::vector<std::string> &, int, char **)
{
	int return_code = EXIT_SUCCESS;

	try {
		unsigned int        worker_thread_count = 10;
		boost::thread_group thread_pool;
		LogDetail << "Creating " << worker_thread_count << " worker thread" <<
			((worker_thread_count == 1) ? "" : "s") << "." << std::endl;
		unsigned int count_1;
		for (count_1 = 0; count_1 < worker_thread_count; ++count_1) {
			FaultInvokerThread
				*new_thread_ptr(new FaultInvokerThread(cdmp_sptr));
			thread_pool.create_thread(*new_thread_ptr);
		}
		LogDetail << "Created " << worker_thread_count << " worker thread" <<
			((worker_thread_count == 1) ? "" : "s") << "." << std::endl;
		thread_pool.join_all();
	}
	catch (const std::exception &except) {
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int TEST_DumpInternalProcess(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::OSSupport::CoreDumperBaseSPtr cdmp_sptr;
		std::vector<std::string>           warnings_list;
		try {
			cdmp_sptr.reset(new MLB::OSSupport::MiniDumperBase("", "C:\\",
				false, -1, true, MLB::Utility::VersionNumber(5, 2, 3790, 1830)));
		}
		catch (const std::exception &except) {
			warnings_list.push_back("Attempt to initialize the Windows minidump "
				"logic failed: " + std::string(except.what()));
			return_code = EXIT_FAILURE;
		}
		return_code = WrappedThread_Handler(cdmp_sptr, warnings_list, argc, argv);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "TEST_DumpInternalProcess() failed: " +
			std::string(except.what()));
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int TEST_DumpExternalProcesses(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	EnsureLoadedLibrary("C:\\MLB\\Tests\\DbgHelp\\6.7.5.1\\dbghelp.dll",
		false);

	try {
		{
			MLB::OSSupport::MiniDumperBase mdmp("", "C:\\",
				false, -1, true, MLB::Utility::VersionNumber(5, 2, 3790, 1830));
		}
		int count_1;
		for (count_1 = 1; count_1 < argc; ++ count_1) {
			std::cout << argv[count_1] << ": " << std::flush;
			try {
				std::pair<MLB::Utility::ProcessId, std::string> proc_spec =
					ResolveProcessSpecifier(argv[count_1]);
				std::string dump_file_name(
					CoreDumperBase::ResolveBaseName(proc_spec.second.c_str(), false,
					proc_spec.first) + "." + CoreDumperBase::GetDefaultCoreExt());
				MiniDumperBase::MiniDumpProcess(proc_spec.first, dump_file_name);
				std::cout << std::setw(10) << proc_spec.first << " " <<
					proc_spec.second << " ---> Dump file '" << dump_file_name <<
					std::endl;
			}
			catch (const std::exception &except) {
				std::cout << "Attempt to resolve failed: " << except.what() <<
					std::endl;
				return_code = EXIT_FAILURE;
			}
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "TEST_DumpExternalProcesses() failed: " +
			std::string(except.what()));
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_DetermineValidExternalProcessFlags()
{
	MLB::Utility::VersionNumber dbghelp_version(MiniDumperBase::LoadDbgHelp(
		"dbghelp", true, MLB::Utility::VersionNumber(5, 2, 3790, 1830)));

	unsigned int mask_flag     = 1;
	unsigned int valid_flags   = 0;
	unsigned int invalid_flags = 0;

	//	Create a temporary file name...
	std::string dump_file_name(CoreDumperBase::ResolveBaseName(
		"TMP_TEST_FILE_NAME") + CoreDumperBase::GetDefaultCoreExt());

	while (mask_flag & MiniDumpValidTypeFlags) {
		//	Start the ping process...
		MLB::Utility::ExecProcessInfo exec_info(MLB::Utility::ExecProcess(
			"C:\\WINNT\\system32\\ping.exe",
			MLB::Utility::MakeInlineVector<std::string>("-n")("100")("localhost")));
		try {
			MiniDumperBase::MiniDumpProcess(exec_info.process_id_, dump_file_name,
				mask_flag);
			//	Worked, so valid...
			valid_flags |= mask_flag;
		}
		catch (const std::exception &) {
			//	Threw, so invalid...
			invalid_flags |= mask_flag;
		}
		mask_flag <<= 1;
		//	Nuke the process we started above...
		try {
			MLB::Utility::KillProcess(exec_info, true);
		}
		catch (const std::exception &) {
		}
		try {
			MLB::Utility::RemoveFile(dump_file_name, false);
		}
		catch (const std::exception &) {
		}
	}

	std::cout << "Version of dbghelp loaded: " << dbghelp_version << std::endl;
	std::cout << "Valid dump flags         : " <<
		MLB::Utility::ValueToStringHex(valid_flags) << std::endl;
	std::cout << "Invalid dump flags       : " <<
		MLB::Utility::ValueToStringHex(invalid_flags) << std::endl;
	std::cout << "-------------------------- " << "------------" << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct DbgHelpAnalysis {
	DbgHelpAnalysis(const DllMatchItem &dll_item = DllMatchItem())
		:dll_item_(dll_item)
		,internal_valid_mask_(0)
		,internal_invalid_mask_(0)
		,external_valid_mask_(0)
		,external_invalid_mask_(0)
	{
	}

	bool operator < (const DbgHelpAnalysis &other) const {
		return(dll_item_.version_number_ < other.dll_item_.version_number_);
	}

	DllMatchItem dll_item_;
	unsigned int internal_valid_mask_;
	unsigned int internal_invalid_mask_;
	unsigned int external_valid_mask_;
	unsigned int external_invalid_mask_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<DbgHelpAnalysis>          DbgHelpAnalysisSet;
typedef DbgHelpAnalysisSet::iterator       DbgHelpAnalysisSetIter;
typedef DbgHelpAnalysisSet::const_iterator DbgHelpAnalysisSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_AnalyzeOneDbgHelpExternal(DbgHelpAnalysis &analysis_ref)
{
	unsigned int mask_flag     = 1;
	unsigned int valid_flags   = 0;
	unsigned int invalid_flags = 0;

	//	Create a temporary file name...
	std::string dump_file_name(CoreDumperBase::ResolveBaseName(
		"TMP_TEST_FILE_NAME.EXTERNAL.") + CoreDumperBase::GetDefaultCoreExt());

	unsigned int count_1;

	for (count_1 = 0; count_1 < (sizeof(unsigned int) * CHAR_BIT); ++count_1) {
		if (mask_flag > MiniDumpValidTypeFlags)
			invalid_flags |= mask_flag;
		else {
			//	Start the ping process...
			MLB::Utility::ExecProcessInfo exec_info(MLB::Utility::ExecProcess(
				MLB::Utility::CanonicalizePathNameSlashes(OS_GetSystemDirectory() +
				MLB::Utility::PathNameSeparatorCanonical + "ping.exe"),
				MLB::Utility::MakeInlineVector<std::string>("-n")("100")
					("localhost")));
			try {
				MiniDumperBase::MiniDumpProcess(exec_info.process_id_,
					dump_file_name, mask_flag);
				//	Worked, so valid...
				valid_flags |= mask_flag;
			}
			catch (const std::exception &) {
				//	Threw, so invalid...
				invalid_flags |= mask_flag;
			}
			//	Nuke the process we started above...
			try {
				MLB::Utility::KillProcess(exec_info, true);
			}
			catch (const std::exception &) {
			}
			//	Remove the dump file if it exists...
			try {
				MLB::Utility::RemoveFile(dump_file_name, false);
			}
			catch (const std::exception &) {
			}
		}
		mask_flag <<= 1;
	}

	analysis_ref.internal_valid_mask_   = valid_flags;
	analysis_ref.internal_invalid_mask_ = invalid_flags;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_AnalyzeOneDbgHelpInternal(DbgHelpAnalysis &analysis_ref)
{
	unsigned int mask_flag     = 1;
	unsigned int valid_flags   = 0;
	unsigned int invalid_flags = 0;

	//	Create a temporary file name...
	std::string dump_file_name(CoreDumperBase::ResolveBaseName(
		"TMP_TEST_FILE_NAME.INTERNAL.") + CoreDumperBase::GetDefaultCoreExt());

	unsigned int count_1;

	for (count_1 = 0; count_1 < (sizeof(unsigned int) * CHAR_BIT); ++count_1) {
		if (mask_flag > MiniDumpValidTypeFlags)
			invalid_flags |= mask_flag;
		else {
			try {
				MiniDumperBase::EmitMiniDump(dump_file_name, mask_flag);
				//	Worked, so valid...
				valid_flags |= mask_flag;
			}
			catch (const std::exception &) {
				//	Threw, so invalid...
				invalid_flags |= mask_flag;
			}
			//	Remove the dump file if it exists...
			try {
				MLB::Utility::RemoveFile(dump_file_name, false);
			}
			catch (const std::exception &) {
			}
		}
		mask_flag <<= 1;
	}

	analysis_ref.external_valid_mask_   = valid_flags;
	analysis_ref.external_invalid_mask_ = invalid_flags;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_AnalyzeOneDbgHelpVersion(const DllMatchItem &dll_item,
	DbgHelpAnalysisSet &analysis_set)
{
	HMODULE dll_handle = EnsureLoadedLibrary(dll_item.file_name_, false);

	try {
		DbgHelpAnalysis analysis_item(dll_item);
		TEST_AnalyzeOneDbgHelpInternal(analysis_item);
		TEST_AnalyzeOneDbgHelpExternal(analysis_item);
		analysis_set.insert(analysis_item);
	}
	catch (const std::exception &except) {
		try {
			OS_FreeLibrary(dll_handle);
		}
		catch (const std::exception &) {
		}
		MLB::Utility::Rethrow(except, "Unable to analyze DLL '" +
			dll_item.file_name_ + "', version " +
			dll_item.version_number_.ToString() + ": " +
			std::string(except.what()));
	}
	
	OS_FreeLibrary(dll_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_AnalyzeDbgHelpVersions()
{
	EnsureLoadedLibrary("psapi.dll", true);
	EnsureLoadedLibrary("powrprof.dll", true);

	std::string        test_dir_name("C:\\MLB\\Tests\\DbgHelp");
	DllMatchItemVector dll_list;
	DbgHelpAnalysisSet analysis_set;
	
	try {	
		boost::filesystem::path storage_path(test_dir_name,
											boost::filesystem::native);
		boost::filesystem::directory_iterator iter_b(storage_path.normalize());
		boost::filesystem::directory_iterator iter_e;
		while (iter_b != iter_e) {
			if (boost::filesystem::is_directory(*iter_b)) {
				std::string this_dir(boost::filesystem::system_complete(*iter_b).
					native_file_string());
				std::string dll_name(MLB::Utility::CanonicalizePathNameSlashes(
					iter_b->string() + MLB::Utility::PathNameSeparatorCanonical +
					"dbghelp.dll"));
std::cout << dll_name << std::endl;
				MLB::Utility::ResolveFilePath(dll_name, "", true);
				TEST_AnalyzeOneDbgHelpVersion(DllMatchItem(dll_name,
					DetermineProductVersion(dll_name)), analysis_set);
			}
			++iter_b;
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to read the test directory '" +
			test_dir_name + "', which should contain directories in which reside "
			"various versions of 'dbghelp.dll': " + std::string(except.what()));
	}

	std::cout << "namespace { // Anonymous namespace" << std::endl;

	std::cout << std::endl;

	std::cout << "// " << std::setfill('/') << std::setw(76) << "" <<
		std::setfill(' ') << std::endl;
	std::cout << "// " << std::setfill('/') << std::setw(76) << "" <<
		std::setfill(' ') << std::endl;
	std::cout << "// Mapping of DbgHelp.dll versions to mini-dump features..." <<
		std::endl;
	std::cout << "// " << std::setfill('/') << std::setw(76) << "" <<
		std::setfill(' ') << std::endl;

	std::cout << "/*" << std::endl;
	std::cout << "   DbgHelp.dll Analysis" << std::endl;
	std::cout << "   ----------- --------" << std::endl;
	std::cout << std::endl;

	std::cout << "   " << std::setfill('=') << std::setw(19) << "" << " " <<
		std::setw(12) << "" << " " << std::setw(12) << "" << " " <<
		std::setw(12) << "" << " " << std::setw(12) << "" << std::setfill(' ') <<
		std::endl;
	std::cout << "   " << std::setw(19) << "Version Number" << " " <<
		std::setw(12) << "Int Valid" << " " << std::setw(12) << "Int Invalid" <<
		" " <<
		std::setw(12) << "Ext Valid" << " " << std::setw(12) << "Ext Invalid" <<
		std::endl;
	std::cout << "   " << std::setfill('-') << std::setw(19) << "" << " " <<
		std::setw(12) << "" << " " << std::setw(12) << "" << " " <<
		std::setw(12) << "" << " " << std::setw(12) << "" << std::setfill(' ') <<
		std::endl;

	{
		DbgHelpAnalysisSetIterC iter_b(analysis_set.begin());
		DbgHelpAnalysisSetIterC iter_e(analysis_set.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::cout << "   " << std::setw(19) <<
				iter_b->dll_item_.version_number_ << " " <<
				std::setw(12) <<
					MLB::Utility::ValueToStringHex(iter_b->external_valid_mask_) <<
					" " <<
				std::setw(12) <<
					MLB::Utility::ValueToStringHex(iter_b->external_invalid_mask_) <<
					" " <<
				std::setw(12) <<
					MLB::Utility::ValueToStringHex(iter_b->internal_valid_mask_) <<
					" " <<
				std::setw(12) <<
					MLB::Utility::ValueToStringHex(iter_b->internal_invalid_mask_) <<
					std::endl;
		}
	}

	std::cout << "   " << std::setfill('=') << std::setw(19) << "" << " " <<
		std::setw(12) << "" << " " << std::setw(12) << "" << " " <<
		std::setw(12) << "" << " " << std::setw(12) << "" << std::setfill(' ') <<
		std::endl;

	std::cout << std::endl;
	std::cout << "*/" << std::endl;
	std::cout << std::endl;

	std::cout << "/**" << std::endl;
	std::cout << "	Contains a list mapping dbghelp version numbers to a mask of supported" << std::endl;
	std::cout << "	bits in the flags enumeration \\e MINIDUMP_TYPE ." << std::endl;
	std::cout << std::endl;
	std::cout << "	\\note This information is not canonical; it is the result of guesses" << std::endl;
	std::cout << "	and testing using versions of the dbghelp.dll I had access to at the time" << std::endl;
	std::cout << "	of this writing." << std::endl;
	std::cout << std::endl;
	std::cout << "	\\note Should you have need to add elements to this array, be sure to add" << std::endl;
	std::cout << "	them in ascending order by version number." << std::endl;
	std::cout << "*/" << std::endl;

	std::cout << "// " << std::setfill('/') << std::setw(76) << "" <<
		std::setfill(' ') << std::endl;

	std::cout << "const MiniDumpTypeVersionMaskMap MiniDumpTypeList[] = {" <<
		std::endl;
	std::cout << "   { MLB::Utility::VersionNumber(    0,    0,    0,    0), " <<
		"static_cast<unsigned int>(0x00000000), " <<
		"static_cast<unsigned int>(0xffffffff), " <<
		"static_cast<unsigned int>(0x00000000), " <<
		"static_cast<unsigned int>(0xffffffff)";

	{
		DbgHelpAnalysisSetIterC iter_first(analysis_set.begin());
		DbgHelpAnalysisSetIterC iter_b(analysis_set.begin());
		DbgHelpAnalysisSetIterC iter_e(analysis_set.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::cout << " }," << std::endl;
			std::cout << "   { MLB::Utility::VersionNumber(" <<
				std::setw(5) << iter_b->dll_item_.version_number_.GetMajor() <<
				"," <<
				std::setw(5) << iter_b->dll_item_.version_number_.GetMinor() <<
				"," <<
				std::setw(5) << iter_b->dll_item_.version_number_.GetRelease() <<
				"," <<
				std::setw(5) << iter_b->dll_item_.version_number_.GetBuild() <<
				"), ";
			std::cout << "static_cast<unsigned int>(" <<
				MLB::Utility::ValueToStringHex(iter_b->external_valid_mask_) <<
				"), ";
			std::cout << "static_cast<unsigned int>(" <<
				MLB::Utility::ValueToStringHex(iter_b->external_invalid_mask_) <<
				"), ";
			std::cout << "static_cast<unsigned int>(" <<
				MLB::Utility::ValueToStringHex(iter_b->internal_valid_mask_) <<
				"), ";
			std::cout << "static_cast<unsigned int>(" <<
				MLB::Utility::ValueToStringHex(iter_b->internal_invalid_mask_) <<
				")";
		}
		std::cout << " }" << std::endl;
		std::cout << "};" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "const unsigned int               MiniDumpTypeCount  =" <<
		std::endl;
	std::cout << "   sizeof(MiniDumpTypeList) / sizeof(MiniDumpTypeList[0]);" <<
		std::endl;
	std::cout << "// " << std::setfill('/') << std::setw(76) << "" <<
		std::setfill(' ') << std::endl;

	std::cout << std::endl;

	std::cout << "} // Anonymous namespace" << std::endl;

	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ { <process-name> | <process-id> } [ ... ] ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		TEST_AnalyzeDbgHelpVersions();
		{
			boost::shared_ptr<MLB::Utility::LogHandlerConsole>
				my_log_handler(new MLB::Utility::LogHandlerConsole());
			MyLogManager.HandlerInstall(my_log_handler);
		}
		if (argc < 2)
			return_code = TEST_DumpInternalProcess(argc, argv);
		else
			return_code = TEST_DumpExternalProcesses(argc, argv);
	}
	catch (const std::exception &except) {
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

