// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of:
									LocateDll()
									LocateDllInList()
									LocateDllInPathString()
									LocateDllInPath()
									ResolveDll()
									ResolveAndLoadDll()

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
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

#include <Utility/PathName.hpp>
#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

namespace {
// ////////////////////////////////////////////////////////////////////////////
bool LocateDllInternal(const std::string &full_dll_name,
	std::string &out_dll_name)
{
	try {
		std::string tmp_full_dll_name(full_dll_name);
		if (tmp_full_dll_name.size() < 4)
			tmp_full_dll_name += ".dll";
		else {
			std::string last_4(tmp_full_dll_name.substr(
				tmp_full_dll_name.size() - 4));
			if (MLB::Utility::UpperCase(last_4) != ".DLL")
				tmp_full_dll_name += ".dll";
		}
		if (MLB::Utility::ResolveFilePath(tmp_full_dll_name, "", false)) {
			out_dll_name.swap(tmp_full_dll_name);
			return(true);
		}
	}
	catch (const std::exception &) {
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LocateDllInternal(const std::string &base_dll_name,
	const std::string &path_name, std::string &out_dll_name)
{
	std::string full_dll_name(MLB::Utility::CanonicalizePathNameSlashes(
		path_name + MLB::Utility::PathNameSeparatorCanonical + base_dll_name));

	return(LocateDllInternal(MLB::Utility::CanonicalizePathNameSlashes(
		full_dll_name), out_dll_name));
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector &LocateDllInList(const std::string &dll_name,
	const MLB::Utility::StringVector &path_list,
	MLB::Utility::StringVector &out_dll_list)
{
	MLB::Utility::StringVector      tmp_dll_list;
	MLB::Utility::StringVectorIterC iter_b(path_list.begin());
	MLB::Utility::StringVectorIterC iter_e(path_list.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		std::string out_dll_name;
		if (LocateDllInternal(dll_name, *iter_b, out_dll_name))
			tmp_dll_list.push_back(out_dll_name);
	}

	out_dll_list.swap(tmp_dll_list);

	return(out_dll_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector &LocateDllInPathString(const std::string &dll_name,
	const std::string &path_string, MLB::Utility::StringVector &out_dll_list)
{
	MLB::Utility::StringVector path_list_1;
	MLB::Utility::StringVector path_list_2;
	MLB::Utility::StringVector dll_list;

	MLB::Utility::SplitString(MLB::Utility::Trim(path_string), ";",
		path_list_1, 0, true);

	MLB::Utility::StringVectorIter iter_b(path_list_1.begin());
	MLB::Utility::StringVectorIter iter_e(path_list_1.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (!MLB::Utility::Trim(*iter_b).empty())
			path_list_2.push_back(*iter_b);
	}

	return(LocateDllInList(dll_name, path_list_2, out_dll_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector &LocateDllInPath(const std::string &dll_name,
	MLB::Utility::StringVector &out_dll_list)
{
	const char *path_string = getenv("PATH");

	return(LocateDllInPathString(dll_name,
		(path_string != NULL) ? path_string : "", out_dll_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector &LocateDllInPath(const std::string &dll_name)
{
	MLB::Utility::StringVector out_dll_list;

	return(LocateDllInPath(dll_name, out_dll_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LocateDll(const std::string &full_dll_name,
	LocateDllPredicate &dll_predicate, std::string &out_dll_name)
{
	std::string tmp_dll_name;

	if (!LocateDllInternal(full_dll_name, tmp_dll_name))
		return(false);

	if (!dll_predicate.IsMatchingDll(tmp_dll_name))
		return(false);

	out_dll_name.swap(tmp_dll_name);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LocateDll(const std::string &path_name, const std::string &dll_name,
	LocateDllPredicate &dll_predicate, std::string &out_dll_name)
{
	std::string tmp_dll_name;

	if (!LocateDllInternal(dll_name, path_name, tmp_dll_name))
		return(false);

	if (!dll_predicate.IsMatchingDll(tmp_dll_name))
		return(false);

	out_dll_name.swap(tmp_dll_name);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LocateDll(const MLB::Utility::StringVector &name_list,
	LocateDllPredicate &dll_predicate, std::string &out_dll_name)
{
	MLB::Utility::StringVectorIterC iter_b(name_list.begin());
	MLB::Utility::StringVectorIterC iter_e(name_list.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (LocateDll(*iter_b, dll_predicate, out_dll_name))
			return(true);
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ResolveDll(const std::string &dll_name,
	LocateDllPredicate &dll_predicate, std::string &full_dll_name)
{
	if ((dll_predicate.SearchStartUpDir()) &&
		LocateDll(GetStartUpDirectory(), dll_name, dll_predicate,
		full_dll_name) && (!dll_predicate.IsGlobalPredicate()))
		return(true);

	if ((dll_predicate.SearchCurrentDir()) &&
		LocateDll(OS_GetCurrentDirectory(), dll_name, dll_predicate,
		full_dll_name) && (!dll_predicate.IsGlobalPredicate()))
		return(true);

	if ((dll_predicate.SearchSystemDir()) &&
		LocateDll(OS_GetSystemDirectory(), dll_name, dll_predicate,
		full_dll_name) && (!dll_predicate.IsGlobalPredicate()))
		return(true);

	if ((dll_predicate.SearchWindowsDir()) &&
		LocateDll(OS_GetWindowsDirectory(), dll_name, dll_predicate,
		full_dll_name) && (!dll_predicate.IsGlobalPredicate()))
		return(true);

	MLB::Utility::StringVector dll_list;

	if (LocateDll(LocateDllInPath(dll_name, dll_list), dll_predicate,
		full_dll_name) && (!dll_predicate.IsGlobalPredicate()))
		return(true);

	std::string best_match(dll_predicate.GetBestMatch().file_name_);

	if (!best_match.empty())
		full_dll_name.swap(best_match);
	else
		full_dll_name.clear();

	return(!full_dll_name.empty());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
HMODULE ResolveAndLoadDll(const std::string &dll_name,
	LocateDllPredicate &dll_predicate, std::string &full_dll_name)
{
	if (ResolveDll(dll_name, dll_predicate, full_dll_name))
		return(EnsureLoadedLibrary(full_dll_name, false));

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
template <typename ThisDllPred>
	void TEST_ResolveDll(ThisDllPred &dll_predicate, const char *predicate_name,
	const std::string &in_dll_name)
{
	std::string dll_name(MLB::Utility::GetFileNamePortion(in_dll_name));
	std::string full_dll_name;

	try {
		std::cout << "      " << predicate_name << ": " << std::flush;
		if (ResolveDll(dll_name, dll_predicate, full_dll_name))
			std::cout << std::setw(19) << dll_predicate.GetBestMatchVersion() <<
				" " << full_dll_name << std::endl;
		else
			std::cout << "*NOT RESOLVED*" << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << predicate_name << ":DLL resolution error: " <<
			except.what() << std::endl;
	}
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
			"[ <dll-file-name> [ <dll-file-name> ... ] ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		std::vector<std::string> file_list;
		if (argc == 1)
			file_list.push_back("dbghelp.dll");
		else
			std::vector<std::string>(argv + 1, argv + argc).swap(file_list);
		std::cout << "Test ResolveDll():" << std::endl;
		std::vector<std::string>::iterator iter_b(file_list.begin());
		std::vector<std::string>::iterator iter_e(file_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::cout << "   " << MLB::Utility::GetFileNamePortion(*iter_b) <<
				std::endl;
			{
				LocateDllPredicate dll_predicate;
				TEST_ResolveDll(dll_predicate, "First match", *iter_b);
			}
			{
				LocateDllByVersionHighestPredicate dll_predicate;
				TEST_ResolveDll(dll_predicate, "Highest ver", *iter_b);
			}
			{
				LocateDllByVersionPredicate dll_predicate(
					MLB::Utility::VersionNumber(5));
				TEST_ResolveDll(dll_predicate, "Min ver = 5", *iter_b);
			}
			{
				LocateDllByVersionPredicate dll_predicate(
					MLB::Utility::VersionNumber(6));
				TEST_ResolveDll(dll_predicate, "Min ver = 6", *iter_b);
			}
			{
				LocateDllByVersionPredicate dll_predicate(
					MLB::Utility::VersionNumber(7));
				TEST_ResolveDll(dll_predicate, "Min ver = 7", *iter_b);
			}
			{
				LocateDllByVersionPredicate dll_predicate(
					MLB::Utility::VersionNumber::GetMinimumValue(),
					MLB::Utility::VersionNumber(5));
				TEST_ResolveDll(dll_predicate, "Max ver = 5", *iter_b);
			}
			{
				LocateDllByVersionPredicate dll_predicate(
					MLB::Utility::VersionNumber::GetMinimumValue(),
					MLB::Utility::VersionNumber(6));
				TEST_ResolveDll(dll_predicate, "Max ver = 6", *iter_b);
			}
			{
				LocateDllByVersionPredicate dll_predicate(
					MLB::Utility::VersionNumber::GetMinimumValue(),
					MLB::Utility::VersionNumber(7));
				TEST_ResolveDll(dll_predicate, "Max ver = 7", *iter_b);
			}
			std::cout << std::setfill('-') << std::setw(79) << " " <<
				std::setfill(' ') << std::endl;
		}
		std::cout << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

