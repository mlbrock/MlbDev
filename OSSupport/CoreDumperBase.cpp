// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the CoreDumperBase class.

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

#include <OSSupport/OSSupport_CoreDumperBase.hpp>

#include <Utility/PathName.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
CoreDumperBase::CoreDumperBase(const char *file_name_base,
	bool is_full_name, int dump_flags, void *user_data_ptr)
	:file_name_base_(ResolveBaseName(file_name_base, is_full_name))
	,dump_flags_(dump_flags)
	,user_data_ptr_(user_data_ptr)
	,mini_dump_instance_(0)
	,the_lock_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CoreDumperBase::CoreDumperBase(const std::string &file_name_base,
	bool is_full_name, int dump_flags, void *user_data_ptr)
	:file_name_base_(ResolveBaseName(file_name_base.c_str(), is_full_name))
	,dump_flags_(dump_flags)
	,user_data_ptr_(user_data_ptr)
	,mini_dump_instance_(0)
	,the_lock_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CoreDumperBase::CoreDumperBase(const char *file_name_base,
	const char *dir_name, bool is_full_name, int dump_flags,
	void *user_data_ptr)
	:file_name_base_(ResolveBaseName(file_name_base, dir_name, is_full_name))
	,dump_flags_(dump_flags)
	,user_data_ptr_(user_data_ptr)
	,mini_dump_instance_(0)
	,the_lock_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CoreDumperBase::CoreDumperBase(const std::string &file_name_base,
	const std::string &dir_name, bool is_full_name, int dump_flags,
	void *user_data_ptr)
	:file_name_base_(ResolveBaseName(file_name_base.c_str(), dir_name.c_str(),
		is_full_name))
	,dump_flags_(dump_flags)
	,user_data_ptr_(user_data_ptr)
	,mini_dump_instance_(0)
	,the_lock_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CoreDumperBase::~CoreDumperBase()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::GetFileBaseName() const
{
	ScopedLock my_lock(GetLockRef());

	return(GetFileBaseNameImpl());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int CoreDumperBase::GetDumpFlags() const
{
	ScopedLock my_lock(GetLockRef());

	return(GetDumpFlagsImpl());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void *CoreDumperBase::GetUserDataPtr() const
{
	ScopedLock my_lock(GetLockRef());

	return(GetUserDataPtrImpl());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::GetNextFileName() const
{
	ScopedLock my_lock(GetLockRef());

	return(GetNextFileNameImpl());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::SetFileBaseName(const std::string &file_name_base,
	bool is_full_name, MLB::Utility::ProcessId process_id,
	const std::string &host_name, const MLB::Utility::TimeT &time_stamp)
{
	ScopedLock my_lock(GetLockRef());

	return(SetFileBaseNameImpl(file_name_base, is_full_name, process_id,
		host_name, time_stamp));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::SetFileBaseName(const std::string &file_name_base,
	const std::string &dir_name, bool is_full_name,
	MLB::Utility::ProcessId process_id, const std::string &host_name,
	const MLB::Utility::TimeT &time_stamp)
{
	ScopedLock my_lock(GetLockRef());

	return(SetFileBaseNameImpl(file_name_base, dir_name, is_full_name,
		process_id, host_name, time_stamp));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int CoreDumperBase::SetDumpFlags(int new_dump_flags)
{
	ScopedLock my_lock(GetLockRef());

	return(SetDumpFlagsImpl(new_dump_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void *CoreDumperBase::SetUserDataPtr(void *new_user_data_ptr)
{
	ScopedLock my_lock(GetLockRef());

	return(SetUserDataPtrImpl(new_user_data_ptr));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CoreDumperBase::CoreDumpIsSupported() const
{
	return(CoreDumpIsSupportedImpl());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::PerformCoreDump(void *other_data_ptr, int dump_flags)
{
	ScopedLock my_lock(GetLockRef());

	return(PerformCoreDumpImpl(other_data_ptr, dump_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CoreDumperBase::LockType &CoreDumperBase::GetLockRef() const
{
	return(the_lock_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::DecorateBaseName(const std::string &file_name_base,
	MLB::Utility::ProcessId process_id, const std::string &host_name,
	const MLB::Utility::TimeT &time_stamp)
{
	return(DecorateBaseName(file_name_base.c_str(), process_id, host_name,
		time_stamp));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::DecorateBaseName(const char *file_name_base,
	MLB::Utility::ProcessId process_id, const std::string &host_name,
	const MLB::Utility::TimeT &time_stamp)
{
	std::string tmp_base;

	if ((file_name_base == NULL) || (!(*file_name_base))) {
		try {
			GetProcessExecutableName(tmp_base);
			tmp_base = MLB::Utility::GetFileNamePortion(tmp_base);
#ifdef _Windows
			std::string::size_type base_size = tmp_base.size();
			if (base_size > 4) {
				if ((!stricmp(tmp_base.data() + (base_size - 4), ".EXE")) ||
					(!stricmp(tmp_base.data() + (base_size - 4), ".COM")))
					tmp_base.assign(tmp_base, 0, base_size - 4);
			}
#elif _VMS_
			std::string::size_type base_size = tmp_base.size();
			if (base_size > 4) {
				if (!stricmp(tmp_base.data() + (base_size - 4), ".COM"))
					tmp_base.assign(tmp_base, 0, base_size - 4);
			}
#endif // #ifdef _Windows
		}
		catch (const std::exception &) {
		}
		if (tmp_base.empty())
			tmp_base = "_UNSPECIFIED_CORE_DUMP_NAME_";
	}
	else {
		tmp_base = file_name_base;
		tmp_base = MLB::Utility::GetFileNamePortion(tmp_base);
#ifdef _Windows
		std::string::size_type base_size = tmp_base.size();
		if (base_size > 4) {
			if ((!stricmp(tmp_base.data() + (base_size - 4), ".EXE")) ||
				(!stricmp(tmp_base.data() + (base_size - 4), ".COM")))
				tmp_base.assign(tmp_base, 0, base_size - 4);
		}
#elif _VMS_
		std::string::size_type base_size = tmp_base.size();
		if (base_size > 4) {
			if (!stricmp(tmp_base.data() + (base_size - 4), ".COM"))
				tmp_base.assign(tmp_base, 0, base_size - 4);
		}
#endif // #ifdef _Windows
	}

	tmp_base = MLB::Utility::GetFileNamePortion(tmp_base);
	tmp_base = MLB::Utility::Trim(tmp_base, ". \t\r\n\v\f");

	if (tmp_base.empty())
		tmp_base = "_UNSPECIFIED_CORE_DUMP_NAME_";

	std::string time_string(time_stamp.ToString());

	time_string[10]  = '.';
	time_string[13]  = '.';
	time_string[16]  = '.';
	tmp_base         = "CoreDump." + tmp_base + "." + time_string + "." +
		host_name + "." + MLB::Utility::ZeroFill(process_id, 10);

	return(tmp_base);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::GetFileBaseNameImpl() const
{
	return(file_name_base_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int CoreDumperBase::GetDumpFlagsImpl() const
{
	return(dump_flags_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void *CoreDumperBase::GetUserDataPtrImpl() const
{
	return(user_data_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::GetNextFileNameImpl() const
{
	return(file_name_base_ + "." +
		MLB::Utility::ZeroFill(mini_dump_instance_, 10) + "." +
		GetDefaultCoreExt());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::SetFileBaseNameImpl(
	const std::string &file_name_base, bool is_full_name,
	MLB::Utility::ProcessId process_id, const std::string &host_name,
	const MLB::Utility::TimeT &time_stamp)
{
	std::string new_base_name(ResolveBaseName(file_name_base.c_str(),
		is_full_name, process_id, host_name, time_stamp));

	std::string old_base_name(file_name_base_);

	file_name_base_.swap(new_base_name);

	return(old_base_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::SetFileBaseNameImpl(
	const std::string &file_name_base, const std::string &dir_name,
	bool is_full_name, MLB::Utility::ProcessId process_id,
	const std::string &host_name, const MLB::Utility::TimeT &time_stamp)
{
	std::string new_file_name_base(ResolveBaseName(file_name_base.c_str(),
		dir_name.c_str(), is_full_name, process_id, host_name, time_stamp));

	std::string old_base_name(file_name_base_);

	file_name_base_.swap(new_file_name_base);

	return(old_base_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int CoreDumperBase::SetDumpFlagsImpl(int new_dump_flags)
{
	int old_dump_flags = dump_flags_;

	dump_flags_ = new_dump_flags;

	return(old_dump_flags);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void *CoreDumperBase::SetUserDataPtrImpl(void *new_user_data_ptr)
{
	void *old_user_data_ptr = user_data_ptr_;

	user_data_ptr_ = new_user_data_ptr;

	return(old_user_data_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CoreDumperBase::CoreDumpIsSupportedImpl() const
{
	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::PerformCoreDumpImpl(void *other_data_ptr,
	int dump_flags)
{
	ThrowNotSupported("CoreDumperBase::PerformCoreDumpImpl(" +
		MLB::Utility::ValueToStringHex(other_data_ptr) + ", " +
		MLB::Utility::AnyToString(dump_flags) + ")");

	return("");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::ResolveBaseName(const char *file_name_base,
	bool is_full_name, MLB::Utility::ProcessId process_id,
	const std::string &host_name, const MLB::Utility::TimeT &time_stamp)
{
	if ((file_name_base == NULL) || (!(*file_name_base)))
		return(MLB::Utility::CanonicalizePathNameSlashes(
			GetDefaultCoreDir() + MLB::Utility::PathNameSeparatorCanonical_String +
			DecorateBaseName(NULL, process_id, host_name, time_stamp)));

	if (!is_full_name)
		return(MLB::Utility::CanonicalizePathNameSlashes(GetDefaultCoreDir() +
			MLB::Utility::PathNameSeparatorCanonical_String +
			DecorateBaseName(file_name_base, process_id, host_name, time_stamp)));

	std::string tmp_base(MLB::Utility::GetFileNamePortion(file_name_base));
	std::string dir_part(MLB::Utility::GetDirNamePortion(file_name_base));

	if (tmp_base.empty())
		tmp_base = DecorateBaseName(NULL, process_id, host_name, time_stamp);

	if (dir_part.empty())
		dir_part = GetDefaultCoreDir();

	MLB::Utility::ResolveDirectoryPath(dir_part, "", true);

	return(MLB::Utility::CanonicalizePathNameSlashes(dir_part +
		MLB::Utility::PathNameSeparatorCanonical_String + tmp_base));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::ResolveBaseName(const char *file_name_base,
	const char *dir_name, bool is_full_name, MLB::Utility::ProcessId process_id,
	const std::string &host_name, const MLB::Utility::TimeT &time_stamp)
{
	if ((dir_name == NULL) || (!(*dir_name)))
		return(ResolveBaseName(file_name_base, is_full_name, process_id,
			host_name, time_stamp));

	std::string dir_part(dir_name);

	MLB::Utility::ResolveDirectoryPath(dir_part, "", true);

	std::string tmp_base;

	if ((file_name_base == NULL) || (!(*file_name_base)) || (!is_full_name))
		tmp_base = DecorateBaseName(file_name_base, process_id, host_name,
			time_stamp);
	else
		tmp_base = MLB::Utility::GetFileNamePortion(file_name_base);

	return(MLB::Utility::CanonicalizePathNameSlashes(dir_part +
		MLB::Utility::PathNameSeparatorCanonical_String + tmp_base));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::GetDefaultCoreDir()
{
#ifdef _Windows
	return("C:\\");
#else
	return("/tmp/core");
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CoreDumperBase::GetDefaultCoreExt()
{
#ifdef _Windows
	return("dmp");
#else
	return("core");
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
void TEST_NamingOneScheme(const CoreDumperBase &test_ref,
	const std::string &test_name, int &return_code)
{
	try {
		std::cout << "Test " << test_name << ":" << std::endl <<
			"   Base = " << test_ref.GetFileBaseName() << std::endl <<
			"   Next = " << test_ref.GetNextFileName() << std::endl <<
			std::endl;
	}
	catch (const std::exception &except) {
		std::cout << "ERROR IN TEST '" << test_name << "': " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_NamingSchemes(int &return_code)
{
#ifdef _Windows
	std::string core_dir("C:\\Program Files");
#else
	std::string core_dir(CoreDumperBase::GetDefaultCoreDir());
#endif // #ifdef _Windows

	TEST_NamingOneScheme(CoreDumperBase("",            false), "(\"\", false)",                        return_code);
	TEST_NamingOneScheme(CoreDumperBase("",            true),  "(\"\", true)",                         return_code);

	TEST_NamingOneScheme(CoreDumperBase("X",           false), "(\"X\", false)",                       return_code);
	TEST_NamingOneScheme(CoreDumperBase("X",           true),  "(\"X\", true)",                        return_code);

	TEST_NamingOneScheme(CoreDumperBase("",  "",       false), "(\"\", \"\", false)",                  return_code);
	TEST_NamingOneScheme(CoreDumperBase("",  "",       true),  "(\"\", \"\", true)",                   return_code);

	TEST_NamingOneScheme(CoreDumperBase("X", "",       false), "(\"X\", \"\", false)",                 return_code);
	TEST_NamingOneScheme(CoreDumperBase("X", "",       true),  "(\"X\", \"\", true)",                  return_code);

	TEST_NamingOneScheme(CoreDumperBase("",  core_dir, false), "(\"\", \"" + core_dir + "\", false)",  return_code);
	TEST_NamingOneScheme(CoreDumperBase("",  core_dir, true),  "(\"\", \"" + core_dir + "\", true)",   return_code);

	TEST_NamingOneScheme(CoreDumperBase("X", core_dir, false), "(\"X\", \"" + core_dir + "\", false)", return_code);
	TEST_NamingOneScheme(CoreDumperBase("X", core_dir, true),  "(\"X\", \"" + core_dir + "\", true)",  return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_NamingSchemes(return_code);
	}
	catch (const std::exception &except) {
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

