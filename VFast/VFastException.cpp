// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The basic VFast exception class implementation file.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFastException.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
VFastException::VFastException(const char *except_string)
	:MLB::Utility::ExceptionGeneral(GetFixedString(except_string))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFastException::VFastException(const std::string &except_string)
	:MLB::Utility::ExceptionGeneral(GetFixedString(except_string.c_str()))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFastException::VFastException(const std::ostringstream &except_string)
	:MLB::Utility::ExceptionGeneral(GetFixedString(except_string.str().c_str()))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFastException::~VFastException() MBCOMPAT_EXCEPT_NOTHROW
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void VFastException::Rethrow(const char *except_string) const
{
	VFastException tmp_except(*this);

	tmp_except.SetWhat(except_string);

	throw tmp_except;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string VFastException::GetFixedString(const char *except_string)
{
	return(((except_string != NULL) && *except_string) ? except_string :
		"Unspecified VFast sub-system exception encountered.");
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

