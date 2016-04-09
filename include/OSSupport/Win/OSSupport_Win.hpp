// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the operating system support library
								for Windows operating systems.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH___MLB__OSSupport__OSSupport_Win_hpp___HH

#define HH___MLB__OSSupport__OSSupport_Win_hpp___HH	1

#define MLB_LIB_OSSupport

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file OSSupport_Win.hpp

	\brief	The MLB Operating System Support header file for Microsoft Windows.

	Contains Microsoft Windows operating system specific classes and definitions.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#ifndef _MSC_VER
# error "This is not a Windows-targeted compilation!!!"
#endif // #ifndef _MSC_VER

#include <Utility/TimeSupport.hpp>
#include <Utility/VersionNumber.hpp>

#include <OSSupport/OSSupport.hpp>

#include <map>

#include <psapi.h>

#pragma warning(push)
#pragma warning(disable:4217 4668)

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

# pragma warning(pop)

// ////////////////////////////////////////////////////////////////////////////

/**
	\namespace MLB

	\brief The Michael L. Brock namespace.
*/
namespace MLB {

/**
	\namespace MLB::OSSupport

	\brief The OSSupport namespace.
*/
namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
struct API_OSSUPPORT DllMatchItem {
	DllMatchItem(const std::string &file_name = "",
		const MLB::Utility::VersionNumber &version_number =
		MLB::Utility::VersionNumber::GetMinimumValue())
		:file_name_(file_name)
		,version_number_(version_number)
	{
	}

	std::string                 file_name_;
	MLB::Utility::VersionNumber version_number_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<DllMatchItem>          DllMatchItemVector;
typedef DllMatchItemVector::iterator       DllMatchItemVectorIter;
typedef DllMatchItemVector::const_iterator DllMatchItemVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT LocateDllPredicate {
public:
	LocateDllPredicate();
	virtual ~LocateDllPredicate();

	virtual bool SearchStartUpDir() const;
	virtual bool SearchCurrentDir() const;
	virtual bool SearchSystemDir() const;
	virtual bool SearchWindowsDir() const;

	virtual bool         IsGlobalPredicate() const;
	virtual bool         IsMatchingDll(const std::string &full_name);
	virtual bool         IsVersionMatch(const std::string &full_name) const;
	virtual bool         IsVersionMatch(const std::string &full_name,
		MLB::Utility::VersionNumber &this_version) const;
	virtual DllMatchItem GetBestMatch() const;

	std::string                 GetBestMatchName() const;
	MLB::Utility::VersionNumber GetBestMatchVersion() const;

	virtual std::string DescribePredicate() const;

	unsigned int       candidate_count_;
	DllMatchItem       best_match_;
	DllMatchItemVector candidate_list_;
	DllMatchItemVector matched_list_;

protected:
	virtual bool         IsGlobalPredicateInternal() const;
	virtual bool         IsVersionMatchImpl(const std::string &full_name) const;
	virtual bool         IsVersionMatchImpl(const std::string &full_name,
		MLB::Utility::VersionNumber &this_version) const;
	virtual bool         IsMatchingDllInternal(const std::string &full_name);
	virtual DllMatchItem GetBestMatchInternal() const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT LocateDllByVersionPredicate : public LocateDllPredicate {
public:
	explicit LocateDllByVersionPredicate(
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());

	MLB::Utility::VersionNumber min_version_;
	MLB::Utility::VersionNumber max_version_;

	virtual std::string DescribePredicate() const;

protected:
	virtual bool IsMatchingDllInternal(const std::string &full_name);

	bool IsVersionMatchImpl(const std::string &full_name) const;
	bool IsVersionMatchImpl(const std::string &full_name,
		MLB::Utility::VersionNumber &this_version) const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT LocateDllByVersionHighestPredicate :
	public LocateDllByVersionPredicate {
public:
	explicit LocateDllByVersionHighestPredicate(
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());

	virtual std::string DescribePredicate() const;

protected:
	virtual bool IsGlobalPredicateInternal() const;
	virtual bool IsMatchingDllInternal(const std::string &full_name);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_OSSUPPORT OS_VersionInfoKey {
	static const WORD DefaultLanguageCode = 1033;
	static const WORD DefaultCodePage     = 1200;
	OS_VersionInfoKey(const char *info_name = "",
		WORD lang_code = DefaultLanguageCode,
		WORD code_page = DefaultCodePage,
		const char *info_value = "")
		:info_name_(info_name)
		,lang_code_(lang_code)
		,code_page_(code_page)
		,info_value_(info_value)
	{
	}

	bool operator < (const OS_VersionInfoKey &other) const {
		if (lang_code_ < other.lang_code_)
			return(true);
		else if (lang_code_ > other.lang_code_)
			return(false);
		if (code_page_ < other.code_page_)
			return(true);
		else if (code_page_ > other.code_page_)
			return(false);
		return(stricmp(info_name_.c_str(), other.info_name_.c_str()) < 0);
	}

	static OS_VersionInfoKey &ZeroLangAndCode(OS_VersionInfoKey &datum) {
		datum.lang_code_ = 0;
		datum.code_page_ = 0;

		return(datum);
	}

	static void ZeroLangAndCode(std::set<OS_VersionInfoKey> &data) {
		std::set<OS_VersionInfoKey>           tmp_data;
		std::set<OS_VersionInfoKey>::iterator iter_b(data.begin());
		std::set<OS_VersionInfoKey>::iterator iter_e(data.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			OS_VersionInfoKey tmp_key(*iter_b);
			tmp_data.insert(ZeroLangAndCode(tmp_key));
		}
		data.swap(tmp_data);
	}

	std::string info_name_;
	WORD        lang_code_;
	WORD        code_page_;
	std::string info_value_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::map<FARPROC, Native_UInt16> WinProcAddrMap;
typedef WinProcAddrMap::iterator         WinProcAddrMapIter;
typedef WinProcAddrMap::const_iterator   WinProcAddrMapIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<OS_VersionInfoKey>        OS_VersionInfoSet;
typedef OS_VersionInfoSet::iterator        OS_VersionInfoSetIter;
typedef OS_VersionInfoSet::const_iterator  OS_VersionInfoSetIterC;
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct OS_ProcessTimes {
	MLB::Utility::TimeSpec creation_time_;
	MLB::Utility::TimeSpec exit_time_;
	MLB::Utility::TimeSpec kernel_time_;
	MLB::Utility::TimeSpec user_time_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#define OS_WIN32_FILETIME_EPOCH_BIAS	116444736000000000i64
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef COMPAT_FN_TYPE(BOOL (WINAPI *OS_FPtr_EnumProcesses),
	(DWORD *, DWORD, DWORD *));
typedef COMPAT_FN_TYPE(BOOL (WINAPI *OS_FPtr_EnumProcessModules),
	(HANDLE, HMODULE *, DWORD, LPDWORD));
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	From psapi.dll
API_OSSUPPORT OS_FPtr_EnumProcesses      Get_OS_FPtr_EnumProcesses(
	bool require_proc = true);
API_OSSUPPORT OS_FPtr_EnumProcessModules Get_OS_FPtr_EnumProcessModules(
	bool require_proc = true);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Kernel32.lib/GetCurrentDirectory()
API_OSSUPPORT char        *OS_GetCurrentDirectory(char *name_buffer,
	unsigned int name_size);
API_OSSUPPORT std::string &OS_GetCurrentDirectory(std::string &name_buffer);
API_OSSUPPORT std::string  OS_GetCurrentDirectory();

//	Kernel32.lib/GetSystemDirectory()
API_OSSUPPORT char        *OS_GetSystemDirectory(char *name_buffer,
	unsigned int name_size);
API_OSSUPPORT std::string &OS_GetSystemDirectory(std::string &name_buffer);
API_OSSUPPORT std::string  OS_GetSystemDirectory();

//	Kernel32.lib/GetWindowsDirectory()
API_OSSUPPORT char        *OS_GetWindowsDirectory(char *name_buffer,
	unsigned int name_size);
API_OSSUPPORT std::string &OS_GetWindowsDirectory(std::string &name_buffer);
API_OSSUPPORT std::string  OS_GetWindowsDirectory();

//	Kernel32.lib/GetModuleBaseName()
API_OSSUPPORT char *OS_GetModuleBaseName(HANDLE process_handle,
	HMODULE module_handle, char *module_name, DWORD module_name_length);
API_OSSUPPORT std::string &OS_GetModuleBaseName(HANDLE process_handle,
	HMODULE module_handle, std::string &module_name);
API_OSSUPPORT std::string OS_GetModuleBaseName(HANDLE process_handle,
	HMODULE module_handle);

//	Kernel32.lib/GetModuleFileName()
API_OSSUPPORT char        *OS_GetModuleFileName(HMODULE module_handle,
	char *module_name, DWORD module_name_length);
API_OSSUPPORT std::string &OS_GetModuleFileName(HMODULE module_handle,
	std::string &module_name);
API_OSSUPPORT std::string  OS_GetModuleFileName(HMODULE module_handle);

//	Psapi.lib/GetModuleFileNameExA()
API_OSSUPPORT char        *OS_GetModuleFileNameEx(HANDLE process_handle,
	HMODULE module_handle, char *module_name, DWORD module_name_length);
API_OSSUPPORT std::string &OS_GetModuleFileNameEx(HANDLE process_handle,
	HMODULE module_handle, std::string &module_name);
API_OSSUPPORT std::string  OS_GetModuleFileNameEx(HANDLE process_handle,
	HMODULE module_handle);

//	Kernel32.lib/GetModuleHandle()
API_OSSUPPORT HMODULE OS_GetModuleHandle(const char *module_name,
	bool throw_if_not_loaded = true);
API_OSSUPPORT HMODULE OS_GetModuleHandle(const std::string &module_name,
	bool throw_if_not_loaded = true);
API_OSSUPPORT HMODULE OS_GetModuleHandle();

//	Kernel32.lib/GetModuleHandleEx()
API_OSSUPPORT HMODULE OS_GetModuleHandleEx(DWORD flags,
	const char *module_name, bool throw_if_not_loaded = true);
API_OSSUPPORT HMODULE OS_GetModuleHandleEx(DWORD flags,
	const std::string &module_name, bool throw_if_not_loaded = true);
API_OSSUPPORT HMODULE OS_GetModuleHandleEx(bool inc_ref_count = false);

//	Kernel32.lib/FreeLibrary()
API_OSSUPPORT BOOL    OS_FreeLibrary(HMODULE module_handle);

//	Kernel32.lib/LoadLibrary()
API_OSSUPPORT HMODULE OS_LoadLibrary(const std::string &library_name);

//	Kernel32.lib/GetProcAddress()
API_OSSUPPORT FARPROC OS_GetProcAddress(HMODULE module_handle,
	const char *proc_spec, bool require_proc = true);
API_OSSUPPORT FARPROC OS_GetProcAddress(HMODULE module_handle,
	Native_UInt16 proc_ordinal, bool require_proc = true);
API_OSSUPPORT FARPROC OS_GetProcAddress(HMODULE module_handle,
	const std::string &proc_name, bool require_proc = true);
API_OSSUPPORT FARPROC OS_GetProcAddress(const std::string &module_name,
	const char *proc_spec, bool require_proc = true);
API_OSSUPPORT FARPROC OS_GetProcAddress(const std::string &module_name,
	Native_UInt16 proc_ordinal, bool require_proc = true);
API_OSSUPPORT FARPROC OS_GetProcAddress(const std::string &module_name,
	const std::string &proc_name, bool require_proc = true);
API_OSSUPPORT std::string OS_GetProcAddressArgDescription(
	const char *proc_spec);
API_OSSUPPORT WinProcAddrMap GetWinProcAddrMap(HMODULE module_handle);
API_OSSUPPORT WinProcAddrMap GetWinProcAddrMap(const std::string &module_name);

//	Version.lib/GetFileVersionInfoSize()
API_OSSUPPORT DWORD OS_GetFileVersionInfoSize(const std::string &file_name,
	DWORD &unknown_value);
API_OSSUPPORT DWORD OS_GetFileVersionInfoSize(const std::string &file_name);

//	Version.lib/GetFileVersionInfo()
API_OSSUPPORT void OS_GetFileVersionInfo(const std::string &file_name,
	DWORD buffer_length, char *buffer_ptr, WORD lang_code, WORD code_page,
	VS_FIXEDFILEINFO &version_info, OS_VersionInfoSet &string_map);
API_OSSUPPORT void OS_GetFileVersionInfo(const std::string &file_name,
	WORD lang_code, WORD code_page, VS_FIXEDFILEINFO &version_info,
	OS_VersionInfoSet &string_map);
API_OSSUPPORT void OS_GetFileVersionInfo(const std::string &file_name,
	VS_FIXEDFILEINFO &version_info, OS_VersionInfoSet &string_map);

//	Psapi.lib/GetProcessImageFileName()
API_OSSUPPORT char        *OS_GetProcessImageFileName(HANDLE process_handle,
	char *file_name, DWORD file_name_length);
API_OSSUPPORT std::string &OS_GetProcessImageFileName(HANDLE process_handle,
	std::string &file_name);
API_OSSUPPORT std::string  OS_GetProcessImageFileName(HANDLE process_handle);
API_OSSUPPORT std::string  OS_GetProcessImageFileName();

//	Psapi.lib/EnumProcesses()
API_OSSUPPORT bool                OS_EnumProcesses(DWORD *process_list,
	unsigned int process_list_bytes, unsigned int *process_needed_bytes);
API_OSSUPPORT bool                OS_EnumProcesses(DWORD *process_list,
	unsigned int process_count, unsigned int &process_count_needed);
API_OSSUPPORT bool                OS_EnumProcesses(DWORD *process_list,
	unsigned int process_count);
API_OSSUPPORT std::vector<DWORD> &OS_EnumProcesses(
	std::vector<DWORD> &process_list);
API_OSSUPPORT std::vector<DWORD>  OS_EnumProcesses();

//	Psapi.lib/EnumProcessModules()
API_OSSUPPORT bool                  OS_EnumProcessModules(HANDLE process_handle,
	HMODULE *module_list, unsigned int module_list_bytes,
	unsigned int *module_needed_bytes);
API_OSSUPPORT bool                  OS_EnumProcessModules(HANDLE process_handle,
	HMODULE *module_list, unsigned int module_count,
	unsigned int &module_count_needed);
API_OSSUPPORT bool                  OS_EnumProcessModules(HANDLE process_handle,
	HMODULE *module_list, unsigned int module_count);
API_OSSUPPORT std::vector<HMODULE> &OS_EnumProcessModules(HANDLE process_handle,
	std::vector<HMODULE> &module_list);
API_OSSUPPORT std::vector<HMODULE>  OS_EnumProcessModules(HANDLE process_handle);

//	Kernel32.lib/GlobalMemoryStatusEx()
API_OSSUPPORT MEMORYSTATUSEX &OS_GlobalMemoryStatusEx(MEMORYSTATUSEX &datum);
API_OSSUPPORT MEMORYSTATUSEX  OS_GlobalMemoryStatusEx();

//	Psapi.lib/GetProcessIoCounters()
API_OSSUPPORT IO_COUNTERS *OS_GetProcessIoCounters(HANDLE process_handle,
	IO_COUNTERS *datum_ptr);
API_OSSUPPORT IO_COUNTERS &OS_GetProcessIoCounters(HANDLE process_handle,
	IO_COUNTERS &datum);
API_OSSUPPORT IO_COUNTERS &OS_GetProcessIoCounters(IO_COUNTERS &datum);
API_OSSUPPORT IO_COUNTERS  OS_GetProcessIoCounters(HANDLE process_handle);
API_OSSUPPORT IO_COUNTERS  OS_GetProcessIoCounters();

//	Psapi.lib/GetProcessMemoryInfo()
API_OSSUPPORT PROCESS_MEMORY_COUNTERS *OS_GetProcessMemoryInfo(
	HANDLE process_handle, PROCESS_MEMORY_COUNTERS *datum_ptr);
API_OSSUPPORT PROCESS_MEMORY_COUNTERS &OS_GetProcessMemoryInfo(
	HANDLE process_handle, PROCESS_MEMORY_COUNTERS &datum);
API_OSSUPPORT PROCESS_MEMORY_COUNTERS &OS_GetProcessMemoryInfo(
	PROCESS_MEMORY_COUNTERS &datum);
API_OSSUPPORT PROCESS_MEMORY_COUNTERS  OS_GetProcessMemoryInfo(
	HANDLE process_handle);
API_OSSUPPORT PROCESS_MEMORY_COUNTERS  OS_GetProcessMemoryInfo();

//	Kernel32.lib/GetProcessTimes()
API_OSSUPPORT void             OS_GetProcessTimes(HANDLE process_handle,
	FILETIME *creation_time, FILETIME *exit_time,
	FILETIME *kernel_time, FILETIME *user_time);
API_OSSUPPORT void             OS_GetProcessTimes(HANDLE process_handle,
	FILETIME &creation_time, FILETIME &exit_time,
	FILETIME &kernel_time, FILETIME &user_time);
API_OSSUPPORT void             OS_GetProcessTimes(HANDLE process_handle,
	MLB::Utility::TimeSpec &creation_time, MLB::Utility::TimeSpec &exit_time,
	MLB::Utility::TimeSpec &kernel_time, MLB::Utility::TimeSpec &user_time);
API_OSSUPPORT OS_ProcessTimes &OS_GetProcessTimes(HANDLE process_handle,
	OS_ProcessTimes &process_times);
API_OSSUPPORT OS_ProcessTimes &OS_GetProcessTimes(
	OS_ProcessTimes &process_times);
API_OSSUPPORT OS_ProcessTimes  OS_GetProcessTimes(HANDLE process_handle);
API_OSSUPPORT OS_ProcessTimes  OS_GetProcessTimes();

API_OSSUPPORT SYSTEM_INFO &OS_GetSystemInfo(SYSTEM_INFO &datum);
API_OSSUPPORT SYSTEM_INFO  OS_GetSystemInfo();
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_OSSUPPORT std::string &GetStartUpDirectory(std::string &out_dir);
API_OSSUPPORT std::string  GetStartUpDirectory();

API_OSSUPPORT HMODULE EnsureLoadedLibrary(const std::string &module_name,
	bool already_loaded_ok_flag = true);

API_OSSUPPORT std::string &DetermineModuleFileName(HMODULE module_handle,
	std::string &module_name);
API_OSSUPPORT std::string DetermineModuleFileName(HMODULE module_handle);

API_OSSUPPORT std::string DetermineModuleFileNameEx(HANDLE process_handle,
	HMODULE module_handle, std::string &module_name);
API_OSSUPPORT std::string DetermineModuleFileNameEx(HANDLE process_handle,
	HMODULE module_handle);

API_OSSUPPORT bool         DetermineFileInfo(const std::string &file_name,
	const std::string &file_info_name, WORD lang_code, WORD code_page,
	std::string &file_info_value, bool throw_if_not_found = false);
API_OSSUPPORT bool         DetermineFileInfo(const std::string &file_name,
	const std::string &file_info_name, std::string &file_info_value,
	bool throw_if_not_found = false);

API_OSSUPPORT MLB::Utility::StringVector GetFileInfoStringList(
	const std::string &file_name,
	WORD lang_code = OS_VersionInfoKey::DefaultLanguageCode,
	WORD code_page = OS_VersionInfoKey::DefaultCodePage);

API_OSSUPPORT MLB::Utility::VersionNumber DetermineFileVersion(
	const std::string &file_name,
	WORD lang_code = OS_VersionInfoKey::DefaultLanguageCode,
	WORD code_page = OS_VersionInfoKey::DefaultCodePage);
API_OSSUPPORT MLB::Utility::VersionNumber DetermineProductVersion(
	const std::string &file_name,
	WORD lang_code = OS_VersionInfoKey::DefaultLanguageCode,
	WORD code_page = OS_VersionInfoKey::DefaultCodePage);

API_OSSUPPORT std::string &DetermineProcessImageFileName(HANDLE process_handle,
	std::string &file_name);
API_OSSUPPORT std::string  DetermineProcessImageFileName(HANDLE process_handle);
API_OSSUPPORT std::string &DetermineProcessImageFileName(std::string &file_name);
API_OSSUPPORT std::string  DetermineProcessImageFileName();

API_OSSUPPORT MLB::Utility::StringVector &LocateDllInList(
	const std::string &dll_name, const MLB::Utility::StringVector &path_list,
	MLB::Utility::StringVector &out_dll_list);
API_OSSUPPORT MLB::Utility::StringVector &LocateDllInPathString(
	const std::string &dll_name, const std::string &path_string,
	MLB::Utility::StringVector &out_dll_list);
API_OSSUPPORT MLB::Utility::StringVector &LocateDllInPath(
	const std::string &dll_name, MLB::Utility::StringVector &out_dll_list);
API_OSSUPPORT MLB::Utility::StringVector &LocateDllInPath(
	const std::string &dll_name);
API_OSSUPPORT bool          LocateDll(const std::string &full_dll_name,
	LocateDllPredicate &dll_predicate, std::string &out_dll_name);
API_OSSUPPORT bool          LocateDll(const std::string &path_name,
	const std::string &dll_name, LocateDllPredicate &dll_predicate,
	std::string &out_dll_name);
API_OSSUPPORT bool          LocateDll(
	const MLB::Utility::StringVector &name_list,
	LocateDllPredicate &dll_predicate, std::string &out_dll_name);

API_OSSUPPORT bool          ResolveDll(const std::string &dll_name,
	LocateDllPredicate &dll_predicate, std::string &full_dll_name);
API_OSSUPPORT HMODULE       ResolveAndLoadDll(const std::string &dll_name,
	LocateDllPredicate &dll_predicate, std::string &full_dll_name);

API_OSSUPPORT HMODULE GetProcessModuleHandle(HANDLE process_handle);
API_OSSUPPORT HMODULE GetProcessModuleHandle();

API_OSSUPPORT unsigned int            ResolveProcessNameToId(
	const std::string &process_name,
	std::vector<MLB::Utility::ProcessId> &process_id_list,
	unsigned int max_count = 0, bool throw_if_not_found = false);
API_OSSUPPORT MLB::Utility::ProcessId ResolveProcessNameToId(
	const std::string &process_name);

API_OSSUPPORT std::string &ResolveProcessIdToName(
	MLB::Utility::ProcessId process_id, std::string &process_name);
API_OSSUPPORT std::string  ResolveProcessIdToName(
	MLB::Utility::ProcessId process_id);

API_OSSUPPORT HANDLE ResolveProcessIdToHandle(
	MLB::Utility::ProcessId process_id);

API_OSSUPPORT HANDLE ResolveProcessNameToHandle(const std::string &process_name,
	bool throw_if_not_found = false);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const unsigned int OS_PredefinedVersionInfoNameMax    = 16;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_OSSUPPORT MLB::Utility::TimeSpec &FileTimeToTimeSpecAbsolute(
	const FILETIME &src, MLB::Utility::TimeSpec &dst);
API_OSSUPPORT MLB::Utility::TimeSpec  FileTimeToTimeSpecAbsolute(
	const FILETIME &src);
API_OSSUPPORT MLB::Utility::TimeSpec &FileTimeToTimeSpecInterval(
	const FILETIME &src, MLB::Utility::TimeSpec &dst);
API_OSSUPPORT MLB::Utility::TimeSpec  FileTimeToTimeSpecInterval(
	const FILETIME &src);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT DllLoader {
public:
	typedef boost::shared_ptr<LocateDllPredicate> LocateDllPredicateSPtr;
	enum DllLoaderFlags {
		None                = 0x0000,
		LoadOnCreation      = 0x0001,
		UnloadOnDestruction = 0x0002,
		PassIfAlreadyLoaded = 0x0004,
		RAIIFlags           = LoadOnCreation | UnloadOnDestruction,
		Default             = LoadOnCreation | UnloadOnDestruction,
		Mask                = LoadOnCreation | UnloadOnDestruction | PassIfAlreadyLoaded
	};
	//	Constrains loaded DLLs to the specified version number range...
	DllLoader(const std::string &dll_name,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());
	//	Loads the highest available version of the DLL within the version range...
	DllLoader(const std::string &dll_name, bool load_highest_version_flag,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());

	//	Constrains loaded DLLs to the specified version number range...
	DllLoader(const std::string &dll_name, DllLoaderFlags dll_load_flags,
		bool load_highest_version_flag,
		const MLB::Utility::VersionNumber &min_version =
			MLB::Utility::VersionNumber::GetMinimumValue(),
		const MLB::Utility::VersionNumber &max_version =
			MLB::Utility::VersionNumber::GetMaximumValue());
	//	Constrains loaded DLLs using a pointer to the specified predicate...
	DllLoader(const std::string &dll_name, DllLoaderFlags dll_load_flags,
		LocateDllPredicateSPtr predicate_ptr);

	//	Constrains loaded DLLs by creating predicate of the specified type...
	template <typename PredicateType>
	DllLoader(const std::string &dll_name, DllLoaderFlags dll_load_flags)
		:dll_name_(MLB::Utility::GetFileNamePortion(dll_name))
		,dll_load_flags_(dll_load_flags)
		,dll_predicate_ptr_(new PredicateType)
		,dll_full_name_(dll_name)
		,dll_handle_(NULL)
		,dll_version_()
	{
		PostCreate();
	}

	virtual ~DllLoader();

	std::string                 GetDllName() const;
	std::string                 GetDllFullName() const;
	HMODULE                     GetDllHandle() const;
	MLB::Utility::VersionNumber GetDllVersion() const;

	bool IsLoaded() const;
	bool CheckIsLoaded() const;
	bool CheckIsNotLoaded() const;

	void LoadDll();
	void UnloadDll();

protected:
	std::string                 dll_name_;
	DllLoaderFlags              dll_load_flags_;
	LocateDllPredicateSPtr      dll_predicate_ptr_;
	std::string                 dll_full_name_;
	HMODULE                     dll_handle_;
	MLB::Utility::VersionNumber dll_version_;

private:
	//	Not a present copyable. To make it so, you'll need to implement a copy
	//	constructor and assignment operator which transfer ownership of opened
	//	file handles.
	MLB_Utility_NonCopyable_Macro(DllLoader);

	void PostCreate();
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#endif // #ifndef HH___MLB__OSSupport__OSSupport_Win_hpp___HH

