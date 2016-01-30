// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of wrapper logic around the ODBC
								function 'SQLGetEnvAttr()'.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
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
SQLRETURN GetEnvAttrBasic(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_ENV, EnvironmentHandle, ::SQLGetEnvAttr,
		(EnvironmentHandle, Attribute, ValuePtr, BufferLength, StringLengthPtr)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute, ValuePtr,
		BufferLength, StringLengthPtr, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	SQLINTEGER &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute,
		ValuePtr, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	SQLUINTEGER &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute,
		ValuePtr, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	SQLSMALLINT &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute,
		ValuePtr, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	SQLUSMALLINT &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute,
		ValuePtr, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	OdbcAttrFlags32 &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute,
		ValuePtr, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	OdbcAttrWindowHandle &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute,
		ValuePtr, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	SQLINTEGER *ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute,
		ValuePtr, 0, NULL, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	SQLUINTEGER *ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute,
		ValuePtr, 0, NULL, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	char *ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute, ValuePtr,
		BufferLength, StringLengthPtr, GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute,
	std::string &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(EnvironmentHandle, Attribute, ValuePtr,
		"::SQLGetEnvAttr", GetEnvAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

