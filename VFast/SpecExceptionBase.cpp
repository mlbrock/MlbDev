// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The VFast basic specification exception class
								implementation file.

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

#include <VFast/SpecExceptionBase.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
SpecExceptionBase::SpecExceptionBase(int exception_type,
	unsigned int exception_number, const char *except_string)
	:VFastException(GetFixedString(exception_type, exception_number,
		except_string))
	,exception_type_(std::toupper(exception_type))
	,exception_number_(exception_number)
	,exception_id_string_(GetFixedIdString(exception_type, exception_number))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SpecExceptionBase::SpecExceptionBase(int exception_type,
	unsigned int exception_number, const std::string &except_string)
	:VFastException(GetFixedString(exception_type, exception_number,
		except_string.c_str()))
	,exception_type_(std::toupper(exception_type))
	,exception_number_(exception_number)
	,exception_id_string_(GetFixedIdString(exception_type, exception_number))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SpecExceptionBase::SpecExceptionBase(int exception_type,
	unsigned int exception_number, const std::ostringstream &except_string)
	:VFastException(GetFixedString(exception_type, exception_number,
		except_string.str().c_str()))
	,exception_type_(std::toupper(exception_type))
	,exception_number_(exception_number)
	,exception_id_string_(GetFixedIdString(exception_type, exception_number))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SpecExceptionBase::~SpecExceptionBase() MBCOMPAT_EXCEPT_NOTHROW
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char SpecExceptionBase::WhatExceptionType() const
{
	return(static_cast<char>(exception_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int SpecExceptionBase::WhatExceptionNumber() const
{
	return(exception_number_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string SpecExceptionBase::WhatExceptionIdString() const
{
	return(exception_id_string_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SpecExceptionBase::Rethrow(const char *except_string) const
{
	SpecExceptionBase tmp_except(*this);

	tmp_except.SetWhat(except_string);

	throw tmp_except;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string SpecExceptionBase::GetFixedString(int exception_type,
	unsigned int exception_number, const char *except_string)
{
	std::ostringstream o_str;

	o_str << GetFixedIdString(exception_type, exception_number) << ": ";

	switch (std::toupper(exception_type)) {
		case 'D'	:
			o_str << "Dynamic error: ";
			break;
		case 'R'	:
			o_str << "Reportable error: ";
			break;
		case 'S'	:
			o_str << "Static error: ";
			break;
		default	:
			break;
	}

	o_str << (((except_string == NULL) || (!(*except_string))) ?
		"Unspecified exception." : except_string);

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string SpecExceptionBase::GetFixedIdString(int exception_type,
	unsigned int exception_number)
{
	std::ostringstream o_str;

	exception_type = std::toupper(exception_type);

	if ((exception_type == 'D') || (exception_type == 'R') ||
		(exception_type == 'S'))
		o_str << "ERR " << static_cast<char>(exception_type) << exception_number;
	else
		o_str << "ERR ?=ASCII " << static_cast<unsigned int>(exception_type) <<
			"=" << exception_number;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

