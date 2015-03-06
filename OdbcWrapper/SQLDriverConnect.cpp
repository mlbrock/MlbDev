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
								function 'SQLDriverConnect()'.

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

#include <Utility/IncrementalBuffer.hpp>
#include <Utility/EnumFlagOps.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN DriverConnect(SQLHDBC ConnectionHandle, SQLHWND WindowHandle,
	SQLCHAR *InConnectionString, SQLSMALLINT StringLength1,
	SQLCHAR *OutConnectionString, SQLSMALLINT BufferLength,
	SQLSMALLINT *StringLength2Ptr, SQLUSMALLINT DriverCompletion,
	OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_DBC, ConnectionHandle, ::SQLDriverConnect,
		(ConnectionHandle, WindowHandle, InConnectionString, StringLength1,
		OutConnectionString, BufferLength, StringLength2Ptr, DriverCompletion)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN DriverConnect(SQLHDBC ConnectionHandle, SQLHWND WindowHandle,
	const std::string &InConnectionString, std::string &OutConnectionString,
	SQLUSMALLINT DriverCompletion, OdbcThrowFlags::Flags throw_flags)
{
	CheckStringArgInLengthDomain<SQLSMALLINT>(InConnectionString,
		"InConnectionString", "::SQLDriverConnect");

	SQLRETURN                                      return_code;
	MLB::Utility::IncrementalBuffer<SQLCHAR, 4096> buffer;

	throw_flags = MLB::Utility::EnumFlagAnd(throw_flags,
		static_cast<OdbcThrowFlags::Flags>(~(OdbcThrowFlags::SuccessWithInfo)));

	for ( ; ; ) {
		SQLSMALLINT actual_len;
		return_code = DriverConnect(ConnectionHandle, WindowHandle,
			reinterpret_cast<SQLCHAR *>(
				const_cast<char *>(InConnectionString.c_str())),
			static_cast<SQLSMALLINT>(InConnectionString.size()),
			reinterpret_cast<SQLCHAR *>(buffer.GetPtr()),
			buffer.GetAllocationCountAsType<SQLSMALLINT>(), &actual_len,
			DriverCompletion, throw_flags);
		if (return_code == SQL_SUCCESS) {
			std::string(reinterpret_cast<const char *>(buffer.GetPtr()),
				actual_len).swap(OutConnectionString);
			break;
		}
		else if (return_code == SQL_SUCCESS_WITH_INFO) {
			//	::SQLDriverConnect() returns 'SQL_SUCCESS_WITH_INFO' for reasons
			//	other than to do with insufficient length of returned strings...	
			if (!buffer.SetCount(actual_len, false))
				break;
		}
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN DriverConnect(SQLHDBC ConnectionHandle, SQLHWND WindowHandle,
	const std::string &InConnectionString, SQLUSMALLINT DriverCompletion,
	OdbcThrowFlags::Flags throw_flags)
{
	std::string OutConnectionString;

	return(DriverConnect(ConnectionHandle, WindowHandle, InConnectionString,
		OutConnectionString, DriverCompletion, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN DriverConnect(SQLHDBC ConnectionHandle, SQLCHAR *InConnectionString,
	SQLSMALLINT StringLength1, SQLCHAR *OutConnectionString,
	SQLSMALLINT BufferLength, SQLSMALLINT *StringLength2Ptr,
	SQLUSMALLINT DriverCompletion, OdbcThrowFlags::Flags throw_flags)
{
	return(DriverConnect(ConnectionHandle, NULL, InConnectionString,
		StringLength1, OutConnectionString, BufferLength, StringLength2Ptr,
		DriverCompletion, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN DriverConnect(SQLHDBC ConnectionHandle,
	const std::string &InConnectionString, std::string &OutConnectionString,
	SQLUSMALLINT DriverCompletion, OdbcThrowFlags::Flags throw_flags)
{
	return(DriverConnect(ConnectionHandle, NULL, InConnectionString,
		OutConnectionString, DriverCompletion, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN DriverConnect(SQLHDBC ConnectionHandle,
	const std::string &InConnectionString, SQLUSMALLINT DriverCompletion,
	OdbcThrowFlags::Flags throw_flags)
{
	return(DriverConnect(ConnectionHandle, NULL, InConnectionString,
		DriverCompletion, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

