// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of DetermineFileInfo().

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
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
bool DetermineFileInfo(const std::string &file_name,
	const std::string &file_info_name, WORD lang_code, WORD code_page,
	std::string &file_info_value, bool throw_if_not_found)
{
	VS_FIXEDFILEINFO  version_info;
	OS_VersionInfoSet string_map;

	OS_GetFileVersionInfo(file_name, lang_code, code_page, version_info,
		string_map);

	OS_VersionInfoSetIter iter_b(string_map.begin());
	OS_VersionInfoSetIter iter_e(string_map.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (((!lang_code) || (lang_code == iter_b->lang_code_)) &&
			((!code_page) || (code_page == iter_b->code_page_)) &&
			(!stricmp(file_info_name.c_str(), iter_b->info_name_.c_str()))) {
			file_info_value.swap(
				const_cast<OS_VersionInfoKey *>(&(*iter_b))->info_value_);
			return(true);
		}
	}

	if (throw_if_not_found)
		MLB::Utility::ThrowException("Unable to locate file information name '" +
			file_info_name + "' in the file '" + file_name + "'.");

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DetermineFileInfo(const std::string &file_name,
	const std::string &file_info_name, std::string &file_info_value,
	bool throw_if_not_found)
{
	return(DetermineFileInfo(file_name, file_info_name,
		OS_VersionInfoKey::DefaultLanguageCode,
		OS_VersionInfoKey::DefaultCodePage, file_info_value, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

