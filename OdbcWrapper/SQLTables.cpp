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
								function 'SQLTables()'.

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
SQLRETURN Tables(SQLHSTMT StatementHandle, SQLCHAR *CatalogName,
	SQLSMALLINT NameLength1, SQLCHAR *SchemaName, SQLSMALLINT NameLength2,
	SQLCHAR *TableName, SQLSMALLINT NameLength3, SQLCHAR *TableType,
	SQLSMALLINT NameLength4, OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_STMT, StatementHandle, ::SQLTables,
		(StatementHandle, CatalogName, NameLength1, SchemaName, NameLength2,
			TableName, NameLength3, TableType,  NameLength4)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN Tables(SQLHSTMT StatementHandle, const char *CatalogName,
	const char *SchemaName, const char *TableName, const char *TableType,
	OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_STMT, StatementHandle, ::SQLTables,
		(StatementHandle,
		reinterpret_cast<SQLCHAR *>(const_cast<char *>(CatalogName)),
		CheckStringArgInLengthDomain<SQLSMALLINT>(CatalogName,
			"CatalogName", "::SQLTables"),
		reinterpret_cast<SQLCHAR *>(const_cast<char *>(SchemaName)),
		CheckStringArgInLengthDomain<SQLSMALLINT>(SchemaName,
			"SchemaName", "::SQLTables"),
		reinterpret_cast<SQLCHAR *>(const_cast<char *>(TableName)),
		CheckStringArgInLengthDomain<SQLSMALLINT>(TableName,
			"TableName", "::SQLTables"),
		reinterpret_cast<SQLCHAR *>(const_cast<char *>(TableType)),
		SQL_NTS)));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

