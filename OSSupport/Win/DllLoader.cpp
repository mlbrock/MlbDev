// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the DllLoader class.

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

#include <OSSupport_Win.hpp>

#include <Utility/PathName.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
DllLoader::DllLoader(const std::string &dll_name,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
	:dll_name_(MLB::Utility::GetFileNamePortion(dll_name))
	,dll_load_flags_(Default)
	,dll_predicate_ptr_(new
		LocateDllByVersionPredicate(min_version, max_version))
	,dll_full_name_(dll_name)
	,dll_handle_(NULL)
	,dll_version_()
{
	PostCreate();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DllLoader::DllLoader(const std::string &dll_name,
	bool load_highest_version_flag,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
	:dll_name_(MLB::Utility::GetFileNamePortion(dll_name))
	,dll_load_flags_(Default)
	,dll_predicate_ptr_((!load_highest_version_flag) ?
		(new LocateDllByVersionPredicate(min_version, max_version)) :
		(new LocateDllByVersionHighestPredicate(min_version, max_version)))
	,dll_full_name_(dll_name)
	,dll_handle_(NULL)
	,dll_version_()
{
	PostCreate();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DllLoader::DllLoader(const std::string &dll_name,
	DllLoaderFlags dll_load_flags, bool load_highest_version_flag,
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
	:dll_name_(MLB::Utility::GetFileNamePortion(dll_name))
	,dll_load_flags_(dll_load_flags)
	,dll_predicate_ptr_((!load_highest_version_flag) ?
		(new LocateDllByVersionPredicate(min_version, max_version)) :
		(new LocateDllByVersionHighestPredicate(min_version, max_version)))
	,dll_full_name_(dll_name)
	,dll_handle_(NULL)
	,dll_version_()
{
	PostCreate();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DllLoader::DllLoader(const std::string &dll_name,
	DllLoaderFlags dll_load_flags, LocateDllPredicateSPtr predicate_ptr)
	:dll_name_(MLB::Utility::GetFileNamePortion(dll_name))
	,dll_load_flags_(dll_load_flags)
	,dll_predicate_ptr_(predicate_ptr)
	,dll_full_name_(dll_name)
	,dll_handle_(NULL)
	,dll_version_()
{
	PostCreate();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DllLoader::~DllLoader()
{
	if (dll_load_flags_ & UnloadOnDestruction) {
		try {
			UnloadDll();
		}
		catch (const std::exception &) {
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DllLoader::PostCreate()
{
	if (dll_load_flags_ & LoadOnCreation)
		LoadDll();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string DllLoader::GetDllName() const
{
	return(dll_name_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string DllLoader::GetDllFullName() const
{
	return(dll_full_name_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
HMODULE DllLoader::GetDllHandle() const
{
	return(dll_handle_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::VersionNumber DllLoader::GetDllVersion() const
{
	return(dll_version_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DllLoader::IsLoaded() const
{
	return(dll_handle_ != NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DllLoader::CheckIsLoaded() const
{
	if (!IsLoaded())
		MLB::Utility::ThrowException("Dll '" + dll_name_ + "' is not loaded.");
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DllLoader::CheckIsNotLoaded() const
{
	if (IsLoaded())
		MLB::Utility::ThrowException("Dll '" + dll_name_ + "' is already "
			"loaded.");

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DllLoader::LoadDll()
{
	if (IsLoaded())
		return;

	HMODULE                     this_handle;
	MLB::Utility::VersionNumber this_version;

	if ((this_handle = OS_GetModuleHandle(dll_name_, false)) != NULL) {
		if (!(dll_load_flags_ & PassIfAlreadyLoaded))
			MLB::Utility::ThrowException("Unable to load dll '" + dll_name_ +
				"' because it is already loaded into this process's address "
				"space.");
		try {
			std::string                 tmp_name(OS_GetModuleFileName(this_handle));
			MLB::Utility::VersionNumber tmp_version(
				DetermineProductVersion(tmp_name));
			this_version = tmp_version;
			if (!dll_predicate_ptr_->IsVersionMatch(tmp_name))
				MLB::Utility::ThrowException("The matching predicate reports that "
					"the loaded dll (product version " + tmp_version.ToString() +
					") is not compatible (" +
					dll_predicate_ptr_->DescribePredicate() + ").");
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Unable to load dll '" + dll_name_ +
				"' which is already loaded into this process's address space: " +
				std::string(except.what()));
		}
	}
	else if ((this_handle = ResolveAndLoadDll(dll_name_, *dll_predicate_ptr_,
		dll_full_name_)) == NULL)
		MLB::Utility::ThrowException("Unable to load dll '" + dll_name_ +
			"' because it could not be located (a total of " +
			MLB::Utility::AnyToString(dll_predicate_ptr_->candidate_list_.size()) +
			" potential candidate(s) were found, with the highest version number "
			"being " + dll_predicate_ptr_->GetBestMatchVersion().ToString() +
			").");
	else
		this_version = dll_predicate_ptr_->GetBestMatchVersion();

	dll_handle_  = this_handle;
	dll_version_ = this_version;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DllLoader::UnloadDll()
{
	if (!IsLoaded())
		return;

	try {
		OS_FreeLibrary(dll_handle_);
	}
	catch (const std::exception &except) {
		MLB::Utility::ThrowException("Attempt to unload dll '" + dll_name_ +
			"' (full name '" + dll_full_name_ + "') failed: " +
			std::string(except.what()));
	}

	dll_handle_    = NULL;

	dll_full_name_.clear();
	dll_version_.SetToMinimumValue();
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/EnumFlagOps.hpp>

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
void TEST_DllLoaderExplicit(const std::string &dll_name,
	bool load_highest_version_flag = false,
	const MLB::Utility::VersionNumber &min_version =
		MLB::Utility::VersionNumber::GetMinimumValue(),
	const MLB::Utility::VersionNumber &max_version =
		MLB::Utility::VersionNumber::GetMaximumValue())
{
	try {
		std::cout << "Explicit Lifetime: " << dll_name << std::endl;
		std::cout << "   Min Version   : " << min_version << std::endl;
		std::cout << "   Max Version   : " << max_version << std::endl;
		std::cout << "   High Version  : " <<
			MLB::Utility::AnyToString(load_highest_version_flag) << std::endl;
		DllLoader dll_loader(dll_name, DllLoader::None, load_highest_version_flag,
			min_version, max_version);
		dll_loader.LoadDll();
		std::cout << "   Succeeded     : " << dll_loader.GetDllFullName() <<
			" " << dll_loader.GetDllVersion() << std::endl;
		dll_loader.UnloadDll();
	}
	catch (const std::exception &except) {
		std::cout << "   Failed        : " << except.what() << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_DllLoaderAuto(const std::string &dll_name,
	bool load_highest_version_flag = false,
	const MLB::Utility::VersionNumber &min_version =
		MLB::Utility::VersionNumber::GetMinimumValue(),
	const MLB::Utility::VersionNumber &max_version =
		MLB::Utility::VersionNumber::GetMaximumValue())
{
	try {
		std::cout << "Scoped Lifetime  : " << dll_name << std::endl;
		std::cout << "   Min Version   : " << min_version << std::endl;
		std::cout << "   Max Version   : " << max_version << std::endl;
		std::cout << "   High Version  : " <<
			MLB::Utility::AnyToString(load_highest_version_flag) << std::endl;
		DllLoader dll_loader(dll_name, DllLoader::Default,
			load_highest_version_flag, min_version, max_version);
		std::cout << "   Succeeded     : " << dll_loader.GetDllFullName() <<
			" " << dll_loader.GetDllVersion() << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << "   Failed        : " << except.what() << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_DllLoaderDoubleLoad(const std::string &dll_name,
	bool load_highest_version_flag = false,
	const MLB::Utility::VersionNumber &min_version =
		MLB::Utility::VersionNumber::GetMinimumValue(),
	const MLB::Utility::VersionNumber &max_version =
		MLB::Utility::VersionNumber::GetMaximumValue())
{
	try {
		std::cout << "Double Load      : " << dll_name << std::endl;
		std::cout << "   Min Version   : " << min_version << std::endl;
		std::cout << "   Max Version   : " << max_version << std::endl;
		std::cout << "   High Version  : " <<
			MLB::Utility::AnyToString(load_highest_version_flag) << std::endl;
		DllLoader dll_loader(dll_name, DllLoader::Default,
			load_highest_version_flag, min_version, max_version);
		{
			DllLoader dll_loader_inner(dll_name, MLB::Utility::EnumFlagOr(
				DllLoader::Default, DllLoader::PassIfAlreadyLoaded),
				load_highest_version_flag, min_version, max_version);
		}
		std::cout << "   Succeeded     : " << dll_loader.GetDllFullName() <<
			" " << dll_loader.GetDllVersion() << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << "   Failed        : " << except.what() << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_DllLoaderCycle(const std::string &dll_name,
	bool load_highest_version_flag = false,
	const MLB::Utility::VersionNumber &min_version =
		MLB::Utility::VersionNumber::GetMinimumValue(),
	const MLB::Utility::VersionNumber &max_version =
		MLB::Utility::VersionNumber::GetMaximumValue())
{
	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	TEST_DllLoaderExplicit(dll_name, load_highest_version_flag, min_version,
      max_version);

	std::cout << std::setfill('-') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	TEST_DllLoaderAuto(dll_name, load_highest_version_flag, min_version,
      max_version);

	std::cout << std::setfill('-') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	TEST_DllLoaderDoubleLoad(dll_name, load_highest_version_flag, min_version,
      max_version);

	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_DllLoaderSpec()
{
	TEST_DllLoaderCycle("dbghelp");

	TEST_DllLoaderCycle("dbghelp", false, MLB::Utility::VersionNumber(6));

	TEST_DllLoaderCycle("dbghelp", true, MLB::Utility::VersionNumber(6));

	std::string original_path(getenv("PATH"));
	std::string tmp_path("PATH=" + original_path +
		";C:\\SomeDir1\\SomeDir2\\lib\\MsvcDbgHelp\\v6.7.5.0");
	putenv(tmp_path.c_str());
	TEST_DllLoaderCycle("dbghelp", true, MLB::Utility::VersionNumber(6));
	putenv(("PATH=" + original_path).c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_DllLoaderSpec();
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

