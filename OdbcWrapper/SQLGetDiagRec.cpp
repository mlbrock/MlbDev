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
								function 'SQLGetDiagRec()'.

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

#include <Utility/IncrementalBuffer.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN GetDiagRec(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLCHAR *Sqlstate, SQLINTEGER *NativeErrorPtr,
	SQLCHAR *MessageText, SQLSMALLINT BufferLength, SQLSMALLINT *TextLengthPtr)
{
	return(::SQLGetDiagRec(HandleType, Handle, RecNumber, Sqlstate,
		NativeErrorPtr, MessageText, BufferLength, TextLengthPtr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN GetDiagRec(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, std::string &Sqlstate, SQLINTEGER *NativeErrorPtr,
	std::string &MessageText, bool throw_flag)
{
	SQLRETURN                                      return_code;
	SQLCHAR                                        tmp_sql_state[5 + 1];
	MLB::Utility::IncrementalBuffer<SQLCHAR, 4096> buffer;

	for ( ; ; ) {
		SQLSMALLINT actual_len;
		return_code = ::SQLGetDiagRec(HandleType, Handle, RecNumber,
			tmp_sql_state, NativeErrorPtr,
			reinterpret_cast<SQLCHAR *>(buffer.GetPtr()),
			buffer.GetAllocationCountAsType<SQLSMALLINT>(), &actual_len);
		if (return_code == SQL_SUCCESS) {
			buffer.SetCount(actual_len, true);
			std::string(reinterpret_cast<const char *>(tmp_sql_state), 5).
				swap(Sqlstate);
			std::string(reinterpret_cast<const char *>(buffer.GetPtr()),
				buffer.GetCount()).swap(MessageText);
			break;
		}
		else if (return_code == SQL_SUCCESS_WITH_INFO) {
			if (!buffer.SetCount(actual_len, false))
				throw OdbcException("Call to '::SQLGetDiagRec()' returned "
					"'SQL_SUCCESS_WITH_INFO', but no returned lengths qualified "
					"for that return code.");
		}
		else if (return_code == SQL_NO_DATA)
			break;
		else if (throw_flag)
			ThrowOdbcStatus(return_code, "Call to '::SQLGetDiagRec()' "
				"with handle type " + MLB::Utility::AnyToString(HandleType) +
				", handle value " + MLB::Utility::AnyToString(Handle) +
				" and diagnostic record number " +
				MLB::Utility::AnyToString(RecNumber) + " failed.");
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

