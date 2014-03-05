// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the portable path name functions.

	Revision History	:	1996-01-02 --- Original logic in genfuncs/expfpath.c.
									Michael L. Brock
								1998-04-08 --- Modified for use with C++.
									Michael L. Brock

		Copyright Michael L. Brock 1996 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/PathName.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>
#include <Utility/FilesystemSupport.hpp>

#ifdef __MSDOS__
# include <direct.h>
# include <dir.h>
#elif _Windows
# include <direct.h>
# ifndef _MSC_VER
#  ifndef __MWERKS__
#   include <dir.h>
#  endif // #  ifndef __MWERKS__
# endif // #ifndef _MSC_VER
#else
# include <string.h>
# include <errno.h>
# include <pwd.h>
#endif // #ifdef __MSDOS__

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# if _MSC_VER < 1400
#  pragma warning(disable:4347 4625 4626 4640)
# endif // # if _MSC_VER < 1400
# pragma warning(disable:4217 4668)
# if _MSC_VER >= 1500
#  pragma warning(disable: 4347 4365 4571 4625 4626 4640)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/exception.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
bool IsPathNameSlash(char in_path_char)
{
#ifdef __MSDOS__
	return((in_path_char == '/') || (in_path_char == '\\'));
#elif _Windows
	return((in_path_char == '/') || (in_path_char == '\\'));
#else
	return(in_path_char == '/');
#endif // #ifdef __MSDOS__
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool IsPathNameSlash(const char *in_path_string)
{
	return(IsPathNameSlash(*in_path_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CanonicalizePathNameSlashes(const std::string &path_name_in)
{
	char datum[MaxPathNameLength + 1 + 1];
	
	return(CanonicalizePathNameSlashes(path_name_in.c_str(), datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *CanonicalizePathNameSlashes(const char *path_name_in, char *path_name_out)
{
	const char *tmp_ptr_i = path_name_in;
	char       *tmp_ptr_o = path_name_out;

	while (*tmp_ptr_i) {
		if (!IsPathNameSlash(*tmp_ptr_i))
			*tmp_ptr_o++ = *tmp_ptr_i++;
		else {
			*tmp_ptr_o++ = PathNameSeparatorCanonical;
			++tmp_ptr_i;
#if _Windows
			// Need to keep '\\' to support '\\<computer-name>\<path-name>'...
			while (*tmp_ptr_i && IsPathNameSlash(*tmp_ptr_i) &&
				(static_cast<unsigned int>(tmp_ptr_i - path_name_in) > 1))
#else
			while (*tmp_ptr_i && IsPathNameSlash(*tmp_ptr_i))
#endif // _Windows
				++tmp_ptr_i;
		}
	}

	*tmp_ptr_o = '\0';

	return(path_name_out);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetCurrentPath()
{
	char datum[2 + MaxHostNameLength + 1 + (MaxPathNameLength * 3) + 1 + 1];

	return(GetCurrentPath(datum, sizeof(datum) - 1));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *GetCurrentPath(char *current_dir, unsigned int current_dir_length)
{
	char *return_ptr = getcwd(current_dir, static_cast<int>(current_dir_length));

	if (return_ptr == NULL)
		ThrowErrno("Invocation of 'getcwd()' failed");

	return(return_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SetCurrentPath(const char *new_path)
{
	if (new_path == NULL)
		ThrowInvalidArgument("Invocation of 'SetCurrentPath()' with a path name "
			"argument equal to 'NULL'.");
#if _Windows
	if (!::SetCurrentDirectory(new_path))
		ThrowSystemError("Invocation of 'SetCurrentDirectory()' for path "
			"name '" + std::string(new_path) + "' failed");
#else
	if (chdir(new_path) != 0)
		ThrowErrno("Invocation of 'chdir()' for path name '" +
			std::string(new_path) + "' failed");
#endif // _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SetCurrentPath(const std::string &new_path)
{
	SetCurrentPath(new_path.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/* BOH

	NAME			:	ExpandToFullPathName

	SYNOPSIS		:	full_path = ExpandToFullPathName(const std::string &in_path);

						full_path = ExpandToFullPathName(const char *in_path);

						std::string full_path;

	DESCRIPTION	:	Expands directory and file names to their fully-qualified
						equivalents.

   PARAMETERS  :  Parameters to this function are as follows:

						(.) ``in_path`` is the name to be expanded.

   RETURNS     :  Returns from this function are as follow:

						(.) a ``std::string`` containing the full, expanded path
						name equivalent of the ``in_path`` parameter.

						(.) One of the defined non-zero GENFUNCS return codes
						indicative of error as follows:

						(..) ''GENFUNCS_SYSTEM_FAILURE'' indicates that a standard
						library 	function or a system call failed.

						(..) ''GENFUNCS_MEMORY_FAILURE'' indicates that the memory
						required for operation of the function could not be
						allocated.

	THROWS		:	Any exception descended from ``std::exception`` upon an error.

	NOTES			:	Under Unix this function performs tilde (''~'') expansion.

						Under MS-DOS and related operating systems such as Windows,
						this function prepends the drive letter (and associated
						colon character) to the fully-qualified name.

	CAVEATS		:	

	SEE ALSO		:	

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1996 - 2014 Michael L. Brock

	OUTPUT INDEX:	ExpandToFullPathName
						Directory Functions:ExpandToFullPathName
						Utility:Functions:ExpandToFullPathName
						Utility:Directory Functions:ExpandToFullPathName
						General Functions:See Utility

	PUBLISH XREF:	ExpandToFullPathName

	PUBLISH NAME:	ExpandToFullPathName

	ENTRY CLASS	:	Directory Functions

EOH */
// ////////////////////////////////////////////////////////////////////////////
std::string ExpandToFullPathName(const char *in_path)
{
	char *tmp_ptr_1;
	char *tmp_ptr_2;
	char *begin_ptr;

	in_path = (in_path == NULL) ? "" : in_path;

#ifdef __MSDOS__
	unsigned int alloc_length = static_cast<unsigned int>(strlen(in_path)) +
		1 + (MaxPathNameLength * 2) + 1;
#elif _MSC_VER
	unsigned int alloc_length = static_cast<unsigned int>(strlen(in_path)) +
		1 + (MaxPathNameLength * 2) + 1;
#elif _Windows
	unsigned int alloc_length = static_cast<unsigned int>(strlen(in_path)) +
		1 + (MaxPathNameLength * 2) + 1;
#endif /* #ifdef __MSDOS__ */

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Ensure the source path name isn't over the legal length...
	// //////////////////////////////////////////////////////////////////////
	if (strlen(in_path) > MaxPathNameLength) {
		std::ostringstream error_text;
		error_text << "The length of the specified source path name string (" <<
			static_cast<unsigned int>(strlen(in_path)) <<
			") exceeds the maximum permissible (" << MaxPathNameLength << ").";
		ThrowInvalidArgument(error_text);
	}
	// //////////////////////////////////////////////////////////////////////

	char tmp_path[2 + MaxHostNameLength + 1 + (MaxPathNameLength * 3) + 1 + 1];
	memset(tmp_path, '\0', sizeof(tmp_path));

#ifdef __MSDOS__
	tmp_ptr_1 = const_cast<char *>(in_path);
	/*	*****************************************************************	*/
	/*	*****************************************************************	*/
	/*		If the programmer specified the drive letter as part of the		*/
	/*	path, that's what we'll use. Otherwise, we need to determine it.	*/
	/*	*****************************************************************	*/
	int          drive_letter;
	unsigned int drive_number;
	if (isalpha(*tmp_ptr_1) && (tmp_ptr_1[1] == ':')) {
		drive_letter  = toupper(*tmp_ptr_1);
		drive_number  = (drive_letter - 'A') + 1;
		tmp_ptr_1    += 2;
	}
	else {
		_dos_getdrive(&drive_number);          /* get current drive */
		drive_letter = drive_number + ('A' - 1);
	}
	/*	*****************************************************************	*/
	if (IsPathNameSlash(*tmp_ptr_1))
		/*	*****************************************************	*/
		/*	*****************************************************	*/
		/*		If the user passed us an absolute path, copy it.	*/
		/*	*****************************************************	*/
		strcat(tmp_path + 2, tmp_ptr_1);
		/*	*****************************************************	*/
	else {
		/*	*****************************************************	*/
		/*	*****************************************************	*/
		/*		Otherwise, it's a relative path. We'll need to		*/
		/*	determine the current working directory for the drive	*/
		/*	and prepend it to the string passed by the user.		*/
		/*	*****************************************************	*/
		/* Get drivename and its current directory.
		*/
		if (_getdcwd(drive_number, tmp_path, alloc_length - 2) == NULL)
			ThrowErrno("Error getting the current working directory");
		if (*tmp_ptr_1) {
			if (!IsPathNameSlash(tmp_path[strlen(tmp_path) - 1]))
				strcat(tmp_path, PathNameSeparatorCanonical_String);
			strcat(tmp_path, tmp_ptr_1);
		}
		/*	*****************************************************	*/
	}
	tmp_path[0] = ((char) drive_letter);
	tmp_path[1] = ':';
	begin_ptr  = tmp_path + 2;
#elif _MSC_VER
	tmp_ptr_1 = const_cast<char *>(in_path);
	/*	*****************************************************************	*/
	/*	*****************************************************************	*/
	/*		If the programmer specified the drive letter as part of the		*/
	/*	path, that's what we'll use. Otherwise, we need to determine it.	*/
	/*	*****************************************************************	*/
	int          drive_letter;
	unsigned int drive_number;
	if (isalpha(*tmp_ptr_1) && (tmp_ptr_1[1] == ':')) {
		drive_letter  = toupper(*tmp_ptr_1);
		drive_number  = static_cast<unsigned int>(drive_letter - 'A') + 1;
		tmp_ptr_1    += 2;
	}
	else {
		drive_number = static_cast<unsigned int>(_getdrive());
		drive_letter = static_cast<int>(drive_number + 'A') - 1;
	}
	/*	*****************************************************************	*/
	if (IsPathNameSlash(*tmp_ptr_1))
		/*	*****************************************************	*/
		/*	*****************************************************	*/
		/*		If the user passed us an absolute path, copy it.	*/
		/*	*****************************************************	*/
		strcat(tmp_path + 2, tmp_ptr_1);
		/*	*****************************************************	*/
	else {
		/*	*****************************************************	*/
		/*	*****************************************************	*/
		/*		Otherwise, it's a relative path. We'll need to		*/
		/*	determine the current working directory for the drive	*/
		/*	and prepend it to the string passed by the user.		*/
		/*	*****************************************************	*/
		/* Get drivename and its current directory.
		*/
		if (_getdcwd(static_cast<int>(drive_number), tmp_path,
			static_cast<int>(alloc_length - 2)) == NULL)
			ThrowErrno("Error getting the current working directory");
		if (*tmp_ptr_1) {
			if (!IsPathNameSlash(tmp_path[strlen(tmp_path) - 1]))
				strcat(tmp_path, PathNameSeparatorCanonical_String);
			strcat(tmp_path, tmp_ptr_1);
		}
		/*	*****************************************************	*/
	}
	tmp_path[0] = ((char) drive_letter);
	tmp_path[1] = ':';
	begin_ptr  = tmp_path + 2;
#elif _Windows
	tmp_ptr_1 = const_cast<char *>(in_path);
	/*	*****************************************************************	*/
	/*	*****************************************************************	*/
	/*		If the programmer specified the drive letter as part of the		*/
	/*	path, that's what we'll use. Otherwise, we need to determine it.	*/
	/*	*****************************************************************	*/
	int          drive_letter;
	unsigned int drive_number;
	if (isalpha(*tmp_ptr_1) && (tmp_ptr_1[1] == ':')) {
		drive_letter  = toupper(*tmp_ptr_1);
		drive_number  = (drive_letter - 'A') + 1;
		tmp_ptr_1    += 2;
	}
	else {
		drive_number = _getdrive();		/* get current drive */
		drive_letter = drive_number + ('A' - 1);
	}
	/*	*****************************************************************	*/
	if (IsPathNameSlash(*tmp_ptr_1))
		/*	*****************************************************	*/
		/*	*****************************************************	*/
		/*		If the user passed us an absolute path, copy it.	*/
		/*	*****************************************************	*/
		strcat(tmp_path + 2, tmp_ptr_1);
		/*	*****************************************************	*/
	else {
		/*	*****************************************************	*/
		/*	*****************************************************	*/
		/*		Otherwise, it's a relative path. We'll need to		*/
		/*	determine the current working directory for the drive	*/
		/*	and prepend it to the string passed by the user.		*/
		/*	*****************************************************	*/
		/* Get drivename and its current directory.
		*/
		if (_getdcwd(drive_number, tmp_path, alloc_length - 2) == NULL)
			ThrowErrno("Error getting the current working directory");
		if (*tmp_ptr_1) {
			if (!IsPathNameSlash(tmp_path[strlen(tmp_path) - 1]))
				strcat(tmp_path, PathNameSeparatorCanonical_String);
			strcat(tmp_path, tmp_ptr_1);
		}
		/*	*****************************************************	*/
	}
	tmp_path[0] = ((char) drive_letter);
	tmp_path[1] = ':';
	begin_ptr   = tmp_path + 2;
#else
	begin_ptr = tmp_path;
	/*	*****************************************************************	*/
	/*	*****************************************************************	*/
	/*		If the path name begins with a tilde ('~'), we'll need to		*/
	/*	expand it to the home directory of the appropriate user . . .		*/
	/*	*****************************************************************	*/
	if (*in_path == '~') {
		struct passwd *passwd_entry_ptr;
		/*	***********************************************************	*/
		/*	***********************************************************	*/
		/*		If the user name is unspecified, process owner will do.	*/
		/*	***********************************************************	*/
		if ((!in_path[1]) || IsPathNameSlash(in_path[1])) {
			if ((passwd_entry_ptr = ::getpwuid(::getuid())) == NULL)
				ThrowErrno(
					"Error getting the home directory for the current user");
			strcat(strcat(strcpy(tmp_path, passwd_entry_ptr->pw_dir),
				PathNameSeparatorCanonical_String), in_path + 1);
		}
		/*	***********************************************************	*/
		/*	***********************************************************	*/
		/*		Otherwise, ask the OS to resolve the user name to their	*/
		/*	home directory . . .														*/
		/*	***********************************************************	*/
		else {
			if ((tmp_ptr_1 = strchr(strcpy(tmp_path, in_path),
				PathNameSeparatorCanonical)) != NULL) {
				*tmp_ptr_1 = '\0';
				tmp_ptr_2  = const_cast<char *>(in_path) + strlen(tmp_path) + 1;
			}
			else
				tmp_ptr_2 = "";
			if ((passwd_entry_ptr = ::getpwnam(tmp_path + 1)) == NULL) {
				std::ostringstream error_text;
				error_text << "Error getting the home directory for the user '" <<
               (tmp_path + 1) << "'";
				ThrowErrno(error_text);
			}
			strcat(strcat(strcpy(tmp_path, passwd_entry_ptr->pw_dir),
				PathNameSeparatorCanonical_String), tmp_ptr_2);
		}
		/*	***********************************************************	*/
	}
	/*	*****************************************************************	*/
	/*	*****************************************************************	*/
	/*		If the path name is absolute, just copy it . . .					*/
	/*	*****************************************************************	*/
	else if (IsPathNameSlash(*in_path))
		strcpy(tmp_path, in_path);
	/*	*****************************************************************	*/
	/*	*****************************************************************	*/
	/*		Otherwise, determine the current directory and prepend it to	*/
	/*	path name passed to the function . . .										*/
	/*	*****************************************************************	*/
	else if (::getcwd(tmp_path, sizeof(tmp_path) - 1) == NULL)
		ThrowErrno("Error getting the current working directory");
	else {
		if (!IsPathNameSlash(tmp_path[strlen(tmp_path) - 1]))
			strcat(tmp_path, PathNameSeparatorCanonical_String);
		strcat(tmp_path, in_path);
	}
	/*	*****************************************************************	*/
#endif /* #ifdef __MSDOS__ */

	//	Canonicalize directory slashes...
	CanonicalizePathNameSlashes(tmp_path, tmp_path);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Remove references to 'this' directory...
	// //////////////////////////////////////////////////////////////////////
	char buffer[5];
	tmp_ptr_1 = tmp_path;
	buffer[0] = PathNameSeparatorCanonical;
	buffer[1] = '.';
	buffer[2] = '\0';
	while ((tmp_ptr_2 = strstr(tmp_ptr_1, buffer)) != NULL) {
		if (!(*(tmp_ptr_2 + 2))) {
			*tmp_ptr_2 = '\0';
			break;
		}
		else if (*(tmp_ptr_2 + 2) == PathNameSeparatorCanonical) {
			char *tmp_ptr_3 = tmp_ptr_2 + 2;
			while (*tmp_ptr_2)
				*tmp_ptr_2++ = *tmp_ptr_3++;
			tmp_ptr_1 = tmp_ptr_2;
		}
		else
			tmp_ptr_1 = tmp_ptr_2 + 1;
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Rationalize references to the parent directory...
	// //////////////////////////////////////////////////////////////////////
	tmp_ptr_1 = begin_ptr;
	buffer[0] = PathNameSeparatorCanonical;
	buffer[1] = '.';
	buffer[2] = '.';
	buffer[3] = '\0';
	while ((tmp_ptr_2 = strstr(tmp_ptr_1, buffer)) != NULL) {
		if ((!(*(tmp_ptr_2 + 3))) ||
			(*(tmp_ptr_2 + 3) == PathNameSeparatorCanonical)) {
			if (tmp_ptr_2 == begin_ptr)
				ThrowStdException("Error getting the full path name.");
			char *tmp_ptr_3 = tmp_ptr_2 - 1;
			while (*tmp_ptr_3 != PathNameSeparatorCanonical)
				tmp_ptr_3--;
			if (tmp_ptr_3 == begin_ptr)
				tmp_ptr_3++;
			tmp_ptr_1  = tmp_ptr_3;
			tmp_ptr_2 += 3;
			while (*tmp_ptr_3)
				*tmp_ptr_3++ = *tmp_ptr_2++;
		}
		else
			tmp_ptr_1 = tmp_ptr_2 + 1;
	}
	// //////////////////////////////////////////////////////////////////////

#ifdef __MSDOS__
	tmp_name = UpperCase(tmp_name);
#endif // #ifdef __MSDOS__

	return(tmp_path);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ExpandToFullPathName(const std::string &in_path)
{
	return(ExpandToFullPathName(in_path.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void CheckDirectoryPath(const std::string &in_path,
	const std::string &base_path, bool require_existence)
{
	CheckFilePathGeneral(in_path, base_path, require_existence, true, false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void CheckFilePath(const std::string &in_path, const std::string &base_path,
	bool require_existence)
{
	CheckFilePathGeneral(in_path, base_path, require_existence, false, true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void CheckFilePathGeneral(const std::string &in_path,
	const std::string &base_path, bool require_existence, bool is_directory,
	bool is_file)
{
	std::string tmp_path;

	ResolveFilePathGeneral(in_path, tmp_path, base_path, require_existence,
		is_directory, is_file);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ResolveDirectoryPath(std::string &in_path, const std::string &base_path,
	bool require_existence)
{
	return(ResolveFilePathGeneral(in_path, in_path, base_path,
		require_existence, true, false));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ResolveFilePath(std::string &in_path, const std::string &base_path,
	bool require_existence)
{
	return(ResolveFilePathGeneral(in_path, in_path, base_path,
		require_existence, false, true));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ResolveFilePathGeneral(const std::string &in_path, std::string &out_path,
	const std::string &base_path, bool require_existence, bool is_directory,
	bool is_file)
{
	bool exists_flag = false;

	try {
		//	Path name must meet 'native' OS requirements. 
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_path(in_path, boost::filesystem::native);
*/
		boost::filesystem::path tmp_path(BoostFs_ConstructNativePath(in_path));
		//	If the path isn't absolute, prepend a directory. If 'base path'
		//	isn't empty we use that, otherwise it's the current dir...
		if (!tmp_path.has_root_path()) {
			if (base_path.empty())
				tmp_path = boost::filesystem::system_complete(tmp_path);
			else {
				try {
					std::string tmp_base_path;
					ResolveFilePathGeneral(base_path, tmp_base_path, "",
						false, true, false);
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
					boost::filesystem::path tmp_base(tmp_base_path,
						boost::filesystem::native);
*/
					boost::filesystem::path tmp_base(
						BoostFs_ConstructNativePath(tmp_base_path));
					tmp_path = boost::filesystem::complete(tmp_path, tmp_base);
				}
				catch (const std::exception &except) {
					Rethrow(except, "Unable to resolve the specified base path: " +
						std::string(except.what()));
				}
			}
		}
		if (require_existence) {
			if (!boost::filesystem::exists(tmp_path))
				MLB::Utility::ThrowException("Path does not exist.");
			if (is_directory && (!boost::filesystem::is_directory(tmp_path)))
				MLB::Utility::ThrowException("Path is not a directory.");
			if (is_file && boost::filesystem::is_directory(tmp_path))
				MLB::Utility::ThrowException("Path is not a file.");
			//	Changed to set 'exists_flag' to 'true' --- MLB 2007-01-29.
			exists_flag = true;
		}
		else if (boost::filesystem::exists(tmp_path)) {
			if (is_directory && (!boost::filesystem::is_directory(tmp_path)))
				MLB::Utility::ThrowException("Path is not a directory.");
			if (is_file && boost::filesystem::is_directory(tmp_path))
				MLB::Utility::ThrowException("Path is not a file.");
			exists_flag = true;
		}
/*
	CODE NOTE: The method boost::filesystem::path::native_file_string() has
		been obsoleted by Boost Filesystem version 3. This code is therefore to
		be removed.
		out_path = tmp_path.native_file_string();
*/
		out_path = BoostFs_GetNativeFileString(tmp_path);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to resolve path name '" + in_path +
			std::string("': ") + except.what());
	}

	return(exists_flag);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PathNameToGeneric(const std::string &in_path, std::string &out_path,
	const std::string &base_path, bool require_existence, bool is_directory,
	bool is_file)
{
#ifdef _Windows
	std::string tmp_path;

	ResolveFilePathGeneral(in_path, tmp_path, base_path, require_existence,
		is_directory, is_file);

	if ((tmp_path.size() > 2) && (tmp_path[0] == '\\') && (tmp_path[1] == '\\')){
		out_path = tmp_path;
		return;
	}

	char  unc_info[(MaxPathNameLength * 2) + 1];
	DWORD unc_info_size = sizeof(unc_info);
	if (::WNetGetUniversalName(tmp_path.c_str(), UNIVERSAL_NAME_INFO_LEVEL,
		&unc_info, &unc_info_size) == NO_ERROR)
		out_path =
			reinterpret_cast<UNIVERSAL_NAME_INFO *>(unc_info)->lpUniversalName;
	else {
		SystemErrorCode last_error = GetLastError();
		if (last_error != ERROR_NOT_CONNECTED)
			ThrowSystemError("Invocation of 'WNetGetUniversalName()' for path "
				"name '" + tmp_path + "' failed");
		char datum[(MaxPathNameLength * 2) + 1];
		if (!::GetLongPathName(tmp_path.c_str(), datum, sizeof(datum) - 1))
			ThrowSystemError(last_error, "Invocation of 'WNetGetUniversalName()' "
				"for path name '" + tmp_path + "' failed");
		out_path = datum;
	}

#else
	ResolveFilePathGeneral(in_path, out_path, base_path, require_existence,
		is_directory, is_file);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string PathNameToGeneric(const std::string &in_path,
	const std::string &base_path, bool require_existence, bool is_directory,
	bool is_file)
{
	std::string tmp_path;

	PathNameToGeneric(in_path, tmp_path, base_path, require_existence,
		is_directory, is_file);

	return(tmp_path);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CreatePathDir(const std::string &path_name, bool pre_exists_is_ok)
{
	std::string path_name_1;

	try {
		if (ResolveFilePathGeneral(path_name, path_name_1, "", false, true,
			false)) {
			if (!pre_exists_is_ok)
				MLB::Utility::ThrowException("Path '" + path_name +
					"' already exists.");
			return(true);
		}
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_path(path_name_1, boost::filesystem::native);
*/
		boost::filesystem::path tmp_path(
			BoostFs_ConstructNativePath(path_name_1));
		boost::filesystem::create_directory(tmp_path);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to create path name '" + path_name +
			std::string("': ") + except.what());
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CreatePathDirExtended(const std::string &path_name, bool pre_exists_is_ok)
{
	std::string path_name_1;

	if (ResolveFilePathGeneral(path_name, path_name_1, "", false, true, false)) {
		if (!pre_exists_is_ok)
			MLB::Utility::ThrowException("Path '" + path_name +
				"' already exists.");
		return(true);
	}

	StringVector path_element_list;

	GetPathElementList(path_name_1, path_element_list);

	std::string build_path;

	while (!path_element_list.empty()) {
#ifdef __unix__
		if (path_element_list.back() == ".") {
			path_element_list.pop_back();
			continue;
		}
		build_path += PathNameSeparatorCanonical_String;
#elif defined(_Windows)
		if (path_element_list.back() == ".") {
			path_element_list.pop_back();
			continue;
		}
		if (!build_path.empty())
			build_path += PathNameSeparatorCanonical_String;
#else
		//	Account for some other OS here...
#endif // #ifdef __unix__
		build_path += path_element_list.back();
		path_element_list.pop_back();
		CreatePathDir(build_path, true);
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long GetFileSize(const char *file_name)
{
	unsigned long long file_size = 0;

	try {
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_path(file_name, boost::filesystem::native);
*/		boost::filesystem::path tmp_path(BoostFs_ConstructNativePath(file_name));
		file_size = boost::filesystem::file_size(tmp_path);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to determine the size of file '" +
			std::string(file_name) + "' failed: " + std::string(except.what()));
	}

	return(file_size);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long GetFileSize(const std::string &file_name)
{
	return(GetFileSize(file_name.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CopyFile(const std::string &src_path, const std::string &dst_path)
{
	try {
		CheckFilePathGeneral(src_path, "", true, false, false);
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_src(src_path, boost::filesystem::native);
		boost::filesystem::path tmp_dst(dst_path, boost::filesystem::native);
*/
		boost::filesystem::path tmp_src(BoostFs_ConstructNativePath(src_path));
		boost::filesystem::path tmp_dst(BoostFs_ConstructNativePath(dst_path));
		boost::filesystem::copy_file(tmp_src, tmp_dst);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to copy path name from '" +
			src_path + "' to '" + dst_path + "': " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RenamePath(const std::string &src_path, const std::string &dst_path)
{
	try {
		CheckFilePathGeneral(src_path, "", true, false, false);
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_src(src_path, boost::filesystem::native);
		boost::filesystem::path tmp_dst(dst_path, boost::filesystem::native);
*/
		boost::filesystem::path tmp_src(BoostFs_ConstructNativePath(src_path));
		boost::filesystem::path tmp_dst(BoostFs_ConstructNativePath(dst_path));
		boost::filesystem::rename(tmp_src, tmp_dst);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to rename path name from '" +
			src_path + "' to '" + dst_path + "': " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RemoveFile(const std::string &file_name, bool require_existence)
{
	try {
		CheckFilePath(file_name, "", require_existence);
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_file(file_name, boost::filesystem::native);
*/
		boost::filesystem::path tmp_file(BoostFs_ConstructNativePath(file_name));
		boost::filesystem::remove(tmp_file);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to remove file '" + file_name + "': " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RemoveDirectory(const std::string &path_name, bool require_existence)
{
	try {
		CheckDirectoryPath(path_name, "", require_existence);
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_path(path_name, boost::filesystem::native);
*/
		boost::filesystem::path tmp_path(BoostFs_ConstructNativePath(path_name));
		boost::filesystem::remove(tmp_path);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to remove directory '" + path_name + "': " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RemovePath(const std::string &in_path, bool require_existence)
{
	try {
		CheckFilePathGeneral(in_path, "", require_existence, false, false);
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_path(in_path, boost::filesystem::native);
*/
		boost::filesystem::path tmp_path(BoostFs_ConstructNativePath(in_path));
		boost::filesystem::remove(tmp_path);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to remove path name '" + in_path +
			"': " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void GetPathElementList(const std::string &in_path_name,
	StringVector &path_element_list)
{
	std::string tmp_path_name;

	GetCanonicalNativePath(in_path_name, tmp_path_name);

	StringVector tmp_element_list;

	for ( ; ; ) {
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path tmp_path(tmp_path_name,
			boost::filesystem::native);
*/
		boost::filesystem::path tmp_path(
			BoostFs_ConstructNativePath(tmp_path_name));
		if (!tmp_path.has_leaf())
			break;
/*
	CODE NOTE: The method boost::filesystem::path::leaf() method has
		been obsoleted by Boost Filesystem version 3. This code is therefore
		to be removed.
		std::string last_leaf(tmp_path.leaf());
*/
		std::string             last_leaf(BoostFs_GetPathLeafString(tmp_path));
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		boost::filesystem::path leaf_path(last_leaf, boost::filesystem::native);
*/
		boost::filesystem::path leaf_path(BoostFs_ConstructNativePath(last_leaf));
/*
	CODE NOTE: The method boost::filesystem::path::native_file_string() has
		been obsoleted by Boost Filesystem version 3. This code is therefore
		to be removed.
		tmp_element_list.push_back(leaf_path.native_file_string());
*/
		tmp_element_list.push_back(BoostFs_GetNativeFileString(leaf_path));
		if (last_leaf.size() >= tmp_path_name.size())
			break;
		tmp_path_name.resize(tmp_path_name.size() - (last_leaf.size() + 1));
	}

	path_element_list.swap(tmp_element_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &GetPathRootPath(const std::string &in_path_name,
	std::string &out_root_name)
{
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
	boost::filesystem::path tmp_path(in_path_name, boost::filesystem::native);
*/
	boost::filesystem::path tmp_path(BoostFs_ConstructNativePath(in_path_name));

/*
	CODE NOTE: The method boost::filesystem::path::native_file_string() has
		been obsoleted by Boost Filesystem version 3. This code is therefore
		to be removed.
	return(out_root_name.assign(tmp_path.root_path().native_file_string()));
*/
	return(out_root_name.assign(
		BoostFs_GetNativeFileString(tmp_path.root_path())));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetPathRootPath(const std::string &in_path_name)
{
	std::string out_root_name;

	return(GetPathRootPath(in_path_name, out_root_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &GetCanonicalNativePath(const std::string &in_path_name,
	std::string &out_path_name)
{
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
	boost::filesystem::path tmp_path(in_path_name, boost::filesystem::native);
*/
	boost::filesystem::path tmp_path(BoostFs_ConstructNativePath(in_path_name));

/*
	CODE NOTE: The method boost::filesystem::path::native_file_string() has
		been obsoleted by Boost Filesystem version 3. This code is therefore
		to be removed.
	return(out_path_name.assign(CanonicalizePathNameSlashes(
		tmp_path.native_file_string())));
*/
	return(out_path_name.assign(CanonicalizePathNameSlashes(
		BoostFs_GetNativeFileString(tmp_path))));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetCanonicalNativePath(const std::string &in_path_name)
{
	std::string out_path_name;

	return(GetCanonicalNativePath(in_path_name, out_path_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	NOTA BENE: Return is non-const.
char *GetFileNamePortion(const char *in_string)
{
#ifdef _Windows
	const char *break_char = strrchr(in_string, '/');
	break_char = std::max<const char *>(break_char, strrchr(in_string,  '\\'));
	break_char = std::max<const char *>(break_char, strrchr(in_string,  ':'));
#elif _VMS_
	const char *break_char = strrchr(in_string, ']');
#else
	const char *break_char = strrchr(in_string, '/');
#endif // #ifdef _Windows

	return((break_char == NULL) ? const_cast<char *>(in_string) :
		const_cast<char *>(break_char + 1));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetFileNamePortion(const std::string &in_string)
{
	return(GetFileNamePortion(in_string.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	NOTA BENE: Return is non-const.
unsigned int GetDirNameLength(const char *in_string)
{
	char         *file_portion = GetFileNamePortion(in_string);
	unsigned int  dir_length   =
		static_cast<unsigned int>(file_portion - in_string);

	if (dir_length < 2)
		return(dir_length);

	return(((in_string[dir_length - 1] == '/') ||
		(in_string[dir_length - 1] == '\\')) ? (dir_length - 1) : dir_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int GetDirNameLength(const std::string &in_string)
{
	return(GetDirNameLength(in_string.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetDirNamePortion(const std::string &in_string)
{
	unsigned int dir_length = GetDirNameLength(in_string);
	
	return((dir_length) ? std::string(in_string, 0, dir_length) : "");
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TruncateFileSize(const char *file_name, unsigned long long new_file_size)
{
	unsigned long long current_file_size = GetFileSize(file_name);

	if (new_file_size == current_file_size)
		return;

	try {
#ifdef _MSC_VER
		HANDLE file_handle;
		if ((file_handle = ::CreateFile(file_name,
//			FILE_READ_DATA,
			(FILE_READ_DATA | FILE_WRITE_DATA),
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL)) == INVALID_HANDLE_VALUE)
			ThrowSystemError("Attempt to open file '" + std::string(file_name) +
				"' with 'CreateFile()' failed");
		boost::shared_ptr<void> handle_scope(file_handle, ::CloseHandle);
		TruncateFileSize(file_handle, new_file_size);
#else
		if (::truncate(file_name, new_file_size) != 0)
			ThrowSystemError("Attempt to modify the file size with 'truncate()' "
				"failed");
#endif // #ifdef _MSC_VER
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to change the size of file '" +
			std::string(file_name) + "' from " + AnyToString(current_file_size) +
			" bytes to the requested size of " + AnyToString(new_file_size) +
			" bytes: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TruncateFileSize(const std::string &file_name,
	unsigned long long new_file_size)
{
	TruncateFileSize(file_name.c_str(), new_file_size);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	NOTE: Under Windows, if the size to which the file is to be truncated is
//			less than the size of an exisiting 'CreateFileMapping()', the
//			mapping must be dropped before this call can succeed.
void TruncateFileSize(FileHandleNative file_handle,
	unsigned long long new_file_size)
{
	unsigned long long current_file_size = 0;

	try {
#ifdef _MSC_VER
		BY_HANDLE_FILE_INFORMATION file_info;
		if (::GetFileInformationByHandle(file_handle, &file_info) == 0)
			ThrowSystemError("Attempt to determine the file information with "
				"'GetFileInformationByHandle()' failed");
		current_file_size =
			(static_cast<unsigned long long>(file_info.nFileSizeHigh) *
			static_cast<unsigned long long>(0xFFFFFFFF)) +
			static_cast<unsigned long long>(file_info.nFileSizeLow);
		if (new_file_size > current_file_size) {
			HANDLE          mmap_handle = INVALID_HANDLE_VALUE;
			ULARGE_INTEGER  tmp_length;
			tmp_length.QuadPart = new_file_size;
			if ((mmap_handle = ::CreateFileMapping(
				file_handle,
				NULL,
//				PAGE_READONLY,
				PAGE_READWRITE,
				tmp_length.HighPart,
				tmp_length.LowPart,
				NULL)) == NULL)
				ThrowSystemError("Attempt to increase the file size with "
					"'CreateFileMapping()' failed");
			::CloseHandle(mmap_handle);
		}
		else if (new_file_size < current_file_size) {
			LARGE_INTEGER tmp_length;
			tmp_length.QuadPart = static_cast<LONGLONG>(new_file_size);
			if (::SetFilePointerEx(
				file_handle,
				tmp_length,
				NULL,
				FILE_BEGIN) == 0)
				ThrowSystemError("Attempt to prepare for a decrease in the file "
					"size with by setting the file pointer using "
					"'SetFilePointerEx()' failed (distance to move = " +
					AnyToString(new_file_size) + ").");
			if (::SetEndOfFile(file_handle) == 0)
				ThrowSystemError("Attempt to decrease the file size with "
					"'SetEndOfFile()' failed");
		}
#else
		if (::ftruncate(file_handle, new_file_size) != 0)
				ThrowSystemError("Attempt to change the file size with "
					"'ftruncate()' failed");
#endif // #ifdef _MSC_VER
	}
	catch (const std::exception &except) {
		Rethrow(except, "Attempt to change the size of the file represented "
			"by handle " + ValueToStringHex(file_handle) + " from " +
			AnyToString(current_file_size) + " bytes to the requested size of " +
			AnyToString(new_file_size) + " bytes failed: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/TimeSupport.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/UniqueId.hpp>

#include <fstream>
#include <iostream>

#include <boost/version.hpp>

using namespace MLB::Utility;

namespace {

// ////////////////////////////////////////////////////////////////////////////
const char   *TEST_NativePathList[] = {
#ifdef _Windows
	"C:\\Program Files",
	"C:\\Windows\\System32"
#else
	//	Assuming a Unix here. So this test will need work for other OSes...
	"/usr/bin",
	"/usr/include/sys"
#endif // #ifdef _Windows
};
unsigned int  TEST_NativePathCount  =
	sizeof(TEST_NativePathList) / sizeof(TEST_NativePathList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EmitSep(char sep_char, std::streamsize sep_width = 79)
{
	std::cout << std::setfill(sep_char) << std::setw(sep_width) << "" <<
		std::setfill(' ') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitBoostFilesystemInfo()
{
	EmitSep('=');
	std::cout << "Boost Filesystem Version Info:" << std::endl;
	EmitSep('-');

	std::cout << "BOOST_VERSION           : " << BOOST_VERSION << " = "
		<<  (BOOST_VERSION / 100000)      << "."
		<< ((BOOST_VERSION / 100) % 1000) << "."
		<<  (BOOST_VERSION        %  100) << std::endl;
		
	std::cout << "BOOST_FILESYSTEM_VERSION: "
#ifdef BOOST_FILESYSTEM_VERSION
		<< BOOST_FILESYSTEM_VERSION << std::endl;
#else
		<< "*** UNSPECIFIED ***" << std::endl;
#endif // #ifdef BOOST_FILESYSTEM_VERSION

	EmitSep('=');
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_GetCurrentPath(int &return_code)
{
	EmitSep('=');

	std::string cwd(GetCurrentPath());

	std::cout << "Testing GetCurrentPath(): " <<
		((!cwd.empty()) ? "OK" : "GetCurrentPath() returned an empty string!") <<
		std::endl;

	EmitSep('=');
	std::cout << std::endl;

	if (cwd.empty())
		return_code = EXIT_FAILURE;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_SomeOtherStuff(int &return_code, bool emit_detail = false)
{
	// //////////////////////////////////////////////////////////////////////
	//	Test some Windows path slash stuff...
	{
		EmitSep('=');
		std::cout << "Windows Slash Tests:" << std::endl;
		EmitSep('-');
#if _Windows
		std::vector<std::string> path_list;
		path_list.push_back("c:\\MLB\\MyDir\\Some\\Thing\\Else");
		path_list.push_back("\\\\" + GetHostName() +
			"\\c$\\MLB\\MyDir\\Some\\Thing\\Else");
		for (std::size_t count_1 = 0; count_1 < path_list.size(); ++count_1) {
			try {
				std::string            slash_path(path_list[count_1]);
				std::string::size_type slash_pos;
				while ((slash_pos = slash_path.find('\\')) != std::string::npos)
					slash_path[slash_pos] = '/';
				if (emit_detail) {
					std::cout << "Backslash Path: " << path_list[count_1] <<
						std::endl;
					std::cout << "Slash Path    : " << slash_path << std::endl;
					EmitSep('-');
				}
				std::string path_root_1(GetPathRootPath(path_list[count_1]));
				std::string path_root_2(GetPathRootPath(slash_path));
				if (emit_detail)
					std::cout
						<< "Backslash Path Root: " << path_root_1 << std::endl
						<< "Slash  Path Root   : " << path_root_2 << std::endl;
				StringVector element_list_1;
				StringVector element_list_2;
				GetPathElementList(path_list[count_1], element_list_1);
				GetPathElementList(slash_path, element_list_2);
				if (emit_detail) {
					std::cout << "Path Elements      : " <<
						JoinString(element_list_1, "\n                   : ") <<
						std::endl;
					EmitSep('-');
				}
				if (path_root_1 != path_root_2) {
					std::ostringstream o_str;
					o_str << "GetPathRootPath() returned different root elements "
						"for the backslash-delimited path (" << path_root_1 <<
						"') and the slash-delimited path ('" << path_root_2 << "').";
					ThrowLogicError(o_str.str());
				}
				if (element_list_1 != element_list_2) {
					std::ostringstream o_str;
					o_str << "GetPathElementList() returned different path element "
						"lists for the backslash-delimited path ([" <<
						JoinString(element_list_1, "][") << "]) and the slash-"
						"delimited path ([" <<
						JoinString(element_list_2, "][") << "]).";
					ThrowLogicError(o_str.str());
				}
			}
			catch (const std::exception &except) {
				std::cerr << "Path breakdown test of '" << path_list[count_1] <<
					"' failed: " << except.what() << std::endl;
				return_code = EXIT_FAILURE;
			}
		}
#endif // #ifdef _WIndows
		std::cout << "Tests passed." << std::endl;
		EmitSep('=');
		std::cout << std::endl;
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Test path element breakdowns...
	{
		EmitSep('=');
		std::cout << "Path Element Tests:" << std::endl;
		EmitSep('-');
		for (unsigned int count_1 = 0; count_1 < TEST_NativePathCount;
			++count_1) {
			const std::string this_path(TEST_NativePathList[count_1]);
			try {
				if (emit_detail) {
					std::cout << "Path Breakdown Test of Path: " << this_path <<
						std::endl;
					EmitSep('-');
				}
				StringVector element_list;
				GetPathElementList(this_path, element_list);
				if (emit_detail)
					std::cout << "Path Elements: " <<
						JoinString(element_list, "\n             : ") << std::endl;
				if (emit_detail)
					EmitSep('-');
			}
			catch (const std::exception &except) {
				std::cerr << "Path breakdown test of '" << this_path <<
					"' failed: " << except.what() << std::endl;
				return_code = EXIT_FAILURE;
			}
		}
		std::cout << "Tests passed." << std::endl;
		EmitSep('=');
		std::cout << std::endl;
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Test extended directory path creation and removal...
	{
		EmitSep('=');
		std::cout << "Test File and Directory Path Removal:" << std::endl;
		EmitSep('-');
		//	Create vector of incremental directories...
		std::string base_name("./TEST_UTIL_PathName_Create_" +
			UniqueId().ToStringRadix64());
		ResolveDirectoryPath(base_name, "", false);
		std::vector<std::string> path_list(1, base_name);
		path_list.push_back(path_list.back() + "/" + UniqueId().ToStringRadix64());
		path_list.push_back(path_list.back() + "/" + UniqueId().ToStringRadix64());
		path_list.push_back(path_list.back() + "/" + UniqueId().ToStringRadix64());
		if (emit_detail)
			std::cout << "Creating new directory: " << path_list.back() <<
				std::endl;
		//	Note that it's an error if the path already exists!
		CreatePathDirExtended(path_list.back(), false);
		if (emit_detail)
			std::cout << "Created directory : " << path_list.back() << std::endl;
		while (!path_list.empty()) {
			if (emit_detail)
				std::cout << "Removing directory: " << path_list.back() <<
					std::endl;
			try {
				RemoveDirectory(path_list.back());
			}
			catch (const std::exception &except) {
				std::cerr << "WARNING: Attempt to remove temporary test "
					"directory '" << path_list.back() << "' failed: " <<
					except.what() << std::endl;
			}
			path_list.pop_back();
		}
		std::cout << "Tests passed." << std::endl;
		EmitSep('=');
		std::cout << std::endl;
	}
	// //////////////////////////////////////////////////////////////////////
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MoveFileLogic(int &return_code, bool emit_detail = false)
{
	int                      tmp_return_code = EXIT_SUCCESS;
	std::vector<std::string> remove_list;

	EmitSep('=');
	std::cout << "Testing RenamePath():" << std::endl;
	EmitSep('-');

	try {
		std::string base_dir("./TEST_UTIL_PathName_Move_" +
			UniqueId().ToStringRadix64());
		ResolveDirectoryPath(base_dir, "", false);
		remove_list.push_back(base_dir);
		std::string src_dir(base_dir + "/SrcDir");
		std::string dst_dir(base_dir + "/DstDir");
		//	Create test directories...
		CreatePathDirExtended(src_dir, true);
		remove_list.push_back(src_dir);
		CreatePathDirExtended(dst_dir, true);
		remove_list.push_back(dst_dir);
		std::string src_name(src_dir +
			"/TEST_MLB_Utility_RenamePath.SRC.IGNORE_ME");
		std::string dst_name(dst_dir +
			"/TEST_MLB_Utility_RenamePath.DST.IGNORE_ME");
		remove_list.push_back(src_name);
		remove_list.push_back(dst_name);
		{
			std::ofstream src_file(src_name.c_str());
			if (src_file.fail())
				ThrowErrno("Open attempt for file '" + src_name + "' failed.");
			src_file << "Line of text written to file '" << src_name << "'." <<
				std::endl;
		}
		if (emit_detail) {
			std::cout << "   Wrote source test file '" << src_name << "'." <<
				std::endl;
			std::cout << "   Will move file from: " << src_name << std::endl;
			std::cout << "                    to: " << dst_name << std::endl;
		}
		RenamePath(src_name, dst_name);
		if (ResolveFilePath(src_name, "", false))
			ThrowErrno("Source file '" + src_name + "' exists after the "
				"file rename/move attempt.");
		if (!ResolveFilePath(dst_name, "", false))
			ThrowErrno("Destination file '" + dst_name + "' does not exist after "
				"the file rename/move attempt.");
	}
	catch (const std::exception &except) {
		std::cout << "Regression test error: Move file attempt using the "
			"function 'RenamePath()' failed: " << except.what() << std::endl;
		tmp_return_code = EXIT_FAILURE;
	}

	//	Clean up...
	while (!remove_list.empty()) {
		std::string tmp_name;
		try {
			tmp_name.swap(remove_list.back());
			remove_list.pop_back();
			RemovePath(tmp_name);
		}
		catch (const std::exception &except) {
			std::cerr << "WARNING: Attempt to remove temporary test path "
				"name '" << tmp_name << "' failed: " <<  except.what() <<
				std::endl;
		}
	}

	if (tmp_return_code != EXIT_SUCCESS)
		return_code = EXIT_FAILURE;
	else
		std::cout << "Tests passed." << std::endl;

	EmitSep('=');
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_CheckNativeFileString(int &return_code)
{
	using namespace boost::filesystem;

	for (unsigned int count_1 = 0; count_1 < TEST_NativePathCount; ++count_1) {
		std::string native_string;
		bool        native_done = false;
		try {
			path dir_path(
				BoostFs_ConstructNativePath(TEST_NativePathList[count_1]));
			native_string = BoostFs_GetNativeFileString(dir_path);
			native_done   = true;
			if (native_string != TEST_NativePathList[count_1]) {
				std::ostringstream o_str;
				o_str << "The resulting native path ('" << native_string <<
					"') is not identical.";
				MLB::Utility::ThrowLogicError(o_str.str());
			}
		}
		catch (const std::exception &except) {
			std::ostringstream o_str;
			o_str << "Attempt to determine the boost::filesystem native "
				"equivalent of path '" << TEST_NativePathList[count_1] <<
				"' failed: " << ((native_done) ? "" : "Unable to complete "
				"boost::filesystem::path class operations: ") << except.what();
			MLB::Utility::Rethrow(except, o_str.str());
			return_code = EXIT_FAILURE;
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_EmitBoostFilesystemInfo();
		TEST_GetCurrentPath(return_code);
		TEST_CheckNativeFileString(return_code);
		TEST_SomeOtherStuff(return_code);
		TEST_MoveFileLogic(return_code);
	}
	catch (const std::exception &except) {
		std::cout << std::endl;
		std::cout << "Regression test error: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	if (argc > 1) {
		EmitSep('=');
		std::cout << "Test Expansion of Path Names:" << std::endl;
		unsigned int count_1;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			EmitSep('-');
			try {
				std::cout << "From: [" << argv[count_1] << "]" << std::endl;
				std::string full_path(ExpandToFullPathName(argv[count_1]));
				std::cout << "To  : [" << full_path << "]" << std::endl;
			}
			catch (const std::exception &except) {
				std::cout << "Regression test error: " << except.what() <<
					std::endl;
				return_code = EXIT_FAILURE;
			}
		}
		EmitSep('=');
		std::cout << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

