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
								function 'SQLGetDiagField()'.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2017.
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

namespace {
//	////////////////////////////////////////////////////////////////////////////
template <typename SQLScalarType>
	SQLRETURN GetDiagField_ScalarInternal(SQLSMALLINT HandleType,
	SQLHANDLE Handle, SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
	SQLScalarType &DiagInfoPtr, bool throw_flag)
{
	SQLRETURN     return_code = SQL_SUCCESS;
	SQLSMALLINT   actual_len;
	SQLScalarType tmp_diag_info;

	try {
		return_code = GetDiagField(HandleType, Handle, RecNumber, DiagIdentifier,
			&tmp_diag_info, sizeof(SQLScalarType), &actual_len);
		if (return_code == SQL_SUCCESS)
			DiagInfoPtr = tmp_diag_info;
		else if (return_code == SQL_SUCCESS_WITH_INFO) {
			if (throw_flag)
				throw OdbcException("Call to '::SQLGetDiagField()' returned "
					"'SQL_SUCCESS_WITH_INFO' for a diagnostic identifier value (" +
						MLB::Utility::AnyToString(DiagIdentifier) + ") which was "
						"expected to have a known size of " +
						MLB::Utility::AnyToString(sizeof(SQLScalarType)) + " bytes "
						"--- indicated actual length is " +
					MLB::Utility::AnyToString(actual_len) + " bytes.");
		}
		else if ((return_code != SQL_NO_DATA) && throw_flag)
			ThrowOdbcStatus(return_code, "Unexpected error return code was "
				"encountered from this ODBC function.");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Call to '::SQLGetDiagField()' with "
			"handle type " + MLB::Utility::AnyToString(HandleType) +
			", handle value " + MLB::Utility::AnyToString(Handle) +
			", diagnostic record number " +
			MLB::Utility::AnyToString(RecNumber) + " and diagnostic identifier " +
			MLB::Utility::AnyToString(DiagIdentifier) + " failed: " +
			std::string(except.what()));
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN GetDiagField(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier, SQLPOINTER DiagInfoPtr,
	SQLSMALLINT BufferLength, SQLSMALLINT *StringLengthPtr)
{
	return(::SQLGetDiagField(HandleType, Handle, RecNumber, DiagIdentifier,
     DiagInfoPtr, BufferLength, StringLengthPtr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN GetDiagField(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier, std::string &DiagInfoPtr,
	bool throw_flag)
{
	SQLRETURN                                      return_code;
	MLB::Utility::IncrementalBuffer<SQLCHAR, 4096> buffer;

	for ( ; ; ) {
		try {
			SQLSMALLINT actual_len;
			return_code = GetDiagField(HandleType, Handle, RecNumber,
				DiagIdentifier, reinterpret_cast<SQLPOINTER>(buffer.GetPtr()),
				buffer.GetAllocationCountAsType<SQLSMALLINT>(), &actual_len);
			if (return_code == SQL_SUCCESS) {
				std::string tmp_diag_info;
				if (actual_len > 0) {
					buffer.SetCount(actual_len, true);
					std::string(reinterpret_cast<const char *>(buffer.GetPtr()),
						buffer.GetCount()).swap(tmp_diag_info);
				}
				DiagInfoPtr.swap(tmp_diag_info);
				break;
			}
			else if (return_code == SQL_SUCCESS_WITH_INFO) {
				if (!buffer.SetCount(actual_len, false))
					throw OdbcException("Call to '::SQLGetDiagField()' returned "
						"'SQL_SUCCESS_WITH_INFO', but no returned lengths qualified "
						"for that return code --- indicated actual length is " +
						MLB::Utility::AnyToString(actual_len) + " bytes.");
			}
			else if (return_code == SQL_NO_DATA)
				break;
			else if (throw_flag)
				ThrowOdbcStatus(return_code, "Unexpected error return code was "
					"encountered from this ODBC function.");
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Call to '::SQLGetDiagField()' with "
				"handle type " + MLB::Utility::AnyToString(HandleType) +
				", handle value " + MLB::Utility::AnyToString(Handle) +
				", diagnostic record number " +
				MLB::Utility::AnyToString(RecNumber) + " and diagnostic "
				"identifier " + MLB::Utility::AnyToString(DiagIdentifier) +
				" failed: " + std::string(except.what()));
		}
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN GetDiagField_String(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier, std::string &DiagInfoPtr,
	bool throw_flag)
{
	return(GetDiagField(HandleType, Handle, RecNumber, DiagIdentifier,
		DiagInfoPtr, throw_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN GetDiagField_SQLINTEGER(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
	SQLINTEGER &DiagInfoPtr, bool throw_flag)
{
	return(GetDiagField_ScalarInternal(HandleType, Handle, RecNumber,
		DiagIdentifier, DiagInfoPtr, throw_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN GetDiagField_SQLRETURN(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
	SQLRETURN &DiagInfoPtr, bool throw_flag)
{
	return(GetDiagField_ScalarInternal(HandleType, Handle, RecNumber,
		DiagIdentifier, DiagInfoPtr, throw_flag));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

