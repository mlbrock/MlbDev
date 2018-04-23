// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the path name support logic.

	Revision History	:	1998-04-08 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__PathName_hpp__HH

#define HH__MLB__Utility__PathName_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file PathName.hpp

	\brief	The path name support logic header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
#ifdef __MSDOS__
const char PathNameSeparatorCanonical          = '\\';
const char PathNameSeparatorCanonical_String[] = "\\";
#elif _Windows
const char PathNameSeparatorCanonical          = '\\';
const char PathNameSeparatorCanonical_String[] = "\\";
#else
const char PathNameSeparatorCanonical          = '/';
const char PathNameSeparatorCanonical_String[] = "/";
#endif // #ifdef __MSDOS__
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef __MSDOS__
const char PathNameMetaChar_String[] = "\\/:";
#elif _Windows
const char PathNameMetaChar_String[] = "\\/:";
#elif _VMS_
const char PathNameMetaChar_String[] = "[]:";
#else
const char PathNameMetaChar_String[] = "/";
#endif // #ifdef __MSDOS__
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY bool         IsPathNameSlash(char in_path_char);
API_UTILITY bool         IsPathNameSlash(const char *in_path_string);

API_UTILITY bool         IsPathMetaChar(char in_path_char);
API_UTILITY bool         IsPathMetaChar(const char *in_path_string);

API_UTILITY std::string  CanonicalizePathNameSlashes(
	const std::string &path_name_in);
API_UTILITY char        *CanonicalizePathNameSlashes(const char *path_name_in,
	char *path_name_out);
API_UTILITY std::string  GetCurrentPath();
API_UTILITY char        *GetCurrentPath(char *current_dir,
	unsigned int current_dir_length);
API_UTILITY void         SetCurrentPath(const char *new_path);
API_UTILITY void         SetCurrentPath(const std::string &new_path);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY void CheckDirectoryPath(const std::string &in_path,
	const std::string &base_path = "", bool require_existence = false);
API_UTILITY void CheckFilePath(const std::string &in_path,
	const std::string &base_path = "", bool require_existence = false);
API_UTILITY void CheckFilePathGeneral(const std::string &in_path,
	const std::string &base_path = "", bool require_existence = false,
	bool is_directory = false, bool is_file = false);

API_UTILITY bool ResolveDirectoryPath(std::string &in_path,
	const std::string &base_path = "", bool require_existence = false);
API_UTILITY bool ResolveFilePath(std::string &in_path,
	const std::string &base_path = "", bool require_existence = false);
API_UTILITY bool ResolveFilePathGeneral(const std::string &in_path,
	std::string &out_path, const std::string &base_path = "",
	bool require_existence = false, bool is_directory = false,
	bool is_file = false);

API_UTILITY void        PathNameToGeneric(const std::string &in_path,
	std::string &out_path, const std::string &base_path = "",
	bool require_existence = false, bool is_directory = false,
	bool is_file = false);
API_UTILITY std::string PathNameToGeneric(const std::string &in_path,
	const std::string &base_path = "", bool require_existence = false,
	bool is_directory = false, bool is_file = false);

API_UTILITY bool CreatePathDir(const std::string &path_name,
	bool pre_exists_is_ok = false);
API_UTILITY bool CreatePathDirExtended(const std::string &path_name,
	bool pre_exists_is_ok = false);

API_UTILITY unsigned long long GetFileSize(const char *file_name);
API_UTILITY unsigned long long GetFileSize(const std::string &file_name);

API_UTILITY void CopyFile(const std::string &src_path,
	const std::string &dst_path);

API_UTILITY void RenamePath(const std::string &src_path,
	const std::string &dst_path);

API_UTILITY void RemoveFile(const std::string &file_name,
	bool require_existence = false);
API_UTILITY void RemoveDirectory(const std::string &path_name,
	bool require_existence = false);
API_UTILITY void RemovePath(const std::string &in_path,
	bool require_existence = false);

API_UTILITY void GetPathElementList(const std::string &in_path_name,
	StringVector &path_element_list);

API_UTILITY std::string &GetPathRootPath(const std::string &in_path_name,
	std::string &out_root_name);
API_UTILITY std::string  GetPathRootPath(const std::string &in_path_name);

API_UTILITY std::string &GetCanonicalNativePath(
	const std::string &in_path_name, std::string &out_path_name);
API_UTILITY std::string  GetCanonicalNativePath(
	const std::string &in_path_name);

API_UTILITY char         *GetFileNamePortion(const char *in_string);
API_UTILITY std::string   GetFileNamePortion(const std::string &in_string);
API_UTILITY unsigned int  GetDirNameLength(const char *in_string);
API_UTILITY unsigned int  GetDirNameLength(const std::string &in_string);
API_UTILITY std::string   GetDirNamePortion(const std::string &in_string);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY void TruncateFileSize(const char *file_name,
	unsigned long long new_file_size);
API_UTILITY void TruncateFileSize(const std::string &file_name,
	unsigned long long new_file_size);
API_UTILITY void TruncateFileSize(FileHandleNative file_handle,
	unsigned long long new_file_size);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY const char  *GetExtensionPtr(const char *path_name,
	std::size_t path_length);
API_UTILITY const char  *GetExtensionPtr(const char *path_name);
API_UTILITY const char  *GetExtensionPtr(const std::string &path_name);
API_UTILITY std::string  GetExtension(const std::string &path_name);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__PathName_hpp__HH

