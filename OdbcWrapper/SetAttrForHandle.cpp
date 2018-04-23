// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the 'SetAttrForHandle()' functions
								which provide generic support for the modification of
								ODBC handle attribute data.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	HandleSetAttrFunc set_attr_function, OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute, ValuePtr, StringLength,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLINTEGER ValuePtr, HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), 0, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLUINTEGER ValuePtr, HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), 0, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLSMALLINT ValuePtr, HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), 0, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLUSMALLINT ValuePtr, HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), 0, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	OdbcAttrFlags32 ValuePtr, HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), 0, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	OdbcAttrWindowHandle ValuePtr, HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), 0, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	const char *ValuePtr, SQLINTEGER StringLength,
	HandleSetAttrFunc set_attr_function, OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(const_cast<char *>(ValuePtr)), StringLength,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	const std::string &ValuePtr, const char *odbc_function_name,
	HandleSetAttrFunc set_attr_function, OdbcThrowFlags::Flags throw_flags)
{
	return((*set_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(const_cast<char *>(ValuePtr.c_str())),
		CheckStringArgInLengthDomain<SQLINTEGER>(ValuePtr, "ValuePtr",
		odbc_function_name), throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

