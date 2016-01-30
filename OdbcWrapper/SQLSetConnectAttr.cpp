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
								function 'SQLSetConnectAttr()'.

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
SQLRETURN SetConnectAttrBasic(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_DBC, ConnectionHandle, ::SQLSetConnectAttr,
		(ConnectionHandle, Attribute, ValuePtr, StringLength)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		StringLength, SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	SQLINTEGER ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	SQLUINTEGER ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	SQLSMALLINT ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	SQLUSMALLINT ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	OdbcAttrFlags32 ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	OdbcAttrWindowHandle ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	const char *ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		StringLength, SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle, SQLINTEGER Attribute,
	const std::string &ValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(SetAttrForHandle(ConnectionHandle, Attribute, ValuePtr,
		"::SQLSetConnectAttr", SetConnectAttrBasic, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

