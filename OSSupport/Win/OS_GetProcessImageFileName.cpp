// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of wrappers to the Windows function
								GetProcessImageFileName().

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

#include <OSSupport_Win.h>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
char *OS_GetProcessImageFileName(HANDLE process_handle, char *file_name,
	DWORD file_name_length)
{
	typedef COMPAT_FN_TYPE(BOOL (WINAPI *OS_FPtr_GetProcessImageFileName),
		(HANDLE, LPTSTR, DWORD));

	process_handle = ((process_handle == INVALID_HANDLE_VALUE) ||
		(process_handle == 0)) ? GetCurrentProcess() : process_handle;

	HMODULE module_handle = EnsureLoadedLibrary("psapi", true);

#pragma warning(disable:4191)
	OS_FPtr_GetProcessImageFileName proc_addr =
		reinterpret_cast<OS_FPtr_GetProcessImageFileName>(OS_GetProcAddress(
		module_handle, "GetProcessImageFileName"));
#pragma warning(default:4191)

	if ((*proc_addr)(((FARPROC) process_handle), file_name,
		file_name_length) == 0)
		MLB::Utility::ThrowSystemError("Call to 'GetProcessImageFileName()' "
			"failed.");

	return(file_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &OS_GetProcessImageFileName(HANDLE process_handle,
	std::string &file_name)
{
	char tmp_file_name[(MLB::Utility::MaxPathNameLength * 2) + 1];

	OS_GetProcessImageFileName(process_handle, tmp_file_name,
		sizeof(tmp_file_name) - 1);

	return(file_name.assign(tmp_file_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OS_GetProcessImageFileName(HANDLE process_handle)
{
	std::string file_name;

	return(OS_GetProcessImageFileName(process_handle, file_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OS_GetProcessImageFileName()
{
	return(OS_GetProcessImageFileName(::GetCurrentProcess()));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

