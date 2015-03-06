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
								function 'SQLGetFunctions()'.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2015.
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
SQLRETURN GetFunctions(SQLHDBC ConnectionHandle, SQLUSMALLINT FunctionId,
	SQLUSMALLINT *SupportedPtr, OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_DBC, ConnectionHandle, ::SQLGetFunctions,
		(ConnectionHandle, FunctionId, SupportedPtr)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetFunctions(SQLHDBC ConnectionHandle, SQLUSMALLINT FunctionId,
	SQLUSMALLINT &SupportedPtr, OdbcThrowFlags::Flags throw_flags)
{
	if ((FunctionId == SQL_API_ALL_FUNCTIONS) ||
		(FunctionId == SQL_API_ODBC3_ALL_FUNCTIONS))
		throw OdbcException("Invocation of the scalar overload of the "
			"'GetFunctions()' interface with a non-scalar 'FunctionId' "
			"argument (" + MLB::Utility::AnyToString(FunctionId) + ").");

	return(GetFunctions(ConnectionHandle, FunctionId, &SupportedPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetFunctions(SQLHDBC ConnectionHandle,
	GetFunctions_ApiAll &SupportedPtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetFunctions(ConnectionHandle, SQL_API_ALL_FUNCTIONS, SupportedPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetFunctions(SQLHDBC ConnectionHandle,
	GetFunctions_ApiAll_ODBC3 &SupportedPtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetFunctions(ConnectionHandle, SQL_API_ODBC3_ALL_FUNCTIONS,
		reinterpret_cast<SQLUSMALLINT *>(SupportedPtr), throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

