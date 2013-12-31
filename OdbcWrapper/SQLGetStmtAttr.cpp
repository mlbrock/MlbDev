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
								function 'SQLGetStmtAttr()'.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2014.
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
SQLRETURN GetStmtAttrBasic(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_STMT, StatementHandle, ::SQLGetStmtAttr,
		(StatementHandle, Attribute, ValuePtr, BufferLength, StringLengthPtr)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute, ValuePtr,
		BufferLength, StringLengthPtr, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	SQLINTEGER &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute,
		ValuePtr, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	SQLUINTEGER &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute,
		ValuePtr, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	SQLSMALLINT &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute,
		ValuePtr, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	SQLUSMALLINT &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute,
		ValuePtr, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	OdbcAttrFlags32 &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute,
		ValuePtr, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	OdbcAttrWindowHandle &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute,
		ValuePtr, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	SQLINTEGER *ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute,
		ValuePtr, 0, NULL, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	SQLUINTEGER *ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute,
		ValuePtr, 0, NULL, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	char *ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute, ValuePtr,
		BufferLength, StringLengthPtr, GetStmtAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle, SQLINTEGER Attribute,
	std::string &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetAttrForHandle(StatementHandle, Attribute, ValuePtr,
		"::SQLGetStmtAttr", GetStmtAttrBasic, throw_flags));

}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

