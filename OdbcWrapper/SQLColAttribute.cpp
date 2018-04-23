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
								function 'SQLColAttribute()'.

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

#include <Utility/IncrementalBuffer.hpp>
#include <Utility/EnumFlagOps.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN ColAttribute(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber,
	SQLUSMALLINT FieldIdentifier, SQLPOINTER CharacterAttributePtr,
	SQLSMALLINT BufferLength, SQLSMALLINT *StringLengthPtr,
	SQLPOINTER NumericAttributePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_STMT, StatementHandle, ::SQLColAttribute,
		(StatementHandle, ColumnNumber, FieldIdentifier, CharacterAttributePtr,
		BufferLength, StringLengthPtr, NumericAttributePtr)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN ColAttribute(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber,
	SQLUSMALLINT FieldIdentifier, std::string &CharacterAttributePtr,
	OdbcThrowFlags::Flags throw_flags)
{
	SQLRETURN                                   return_code;
	MLB::Utility::IncrementalBuffer<char, 4096> buffer;

	throw_flags = MLB::Utility::EnumFlagAnd(throw_flags,
		static_cast<OdbcThrowFlags::Flags>(~(OdbcThrowFlags::SuccessWithInfo)));

	for ( ; ; ) {
		SQLSMALLINT actual_len;
		return_code = ColAttribute(StatementHandle, ColumnNumber,
			FieldIdentifier,
			reinterpret_cast<SQLPOINTER>(buffer.GetPtr()),
			buffer.GetAllocationCountAsType<SQLSMALLINT>(), &actual_len, NULL,
			throw_flags);
		if (return_code == SQL_SUCCESS) {
			if (actual_len > 0) {
				buffer.SetCount(actual_len, true);
				std::string(reinterpret_cast<const char *>(buffer.GetPtr()),
					buffer.GetCount()).swap(CharacterAttributePtr);
			}
			else
				CharacterAttributePtr.clear();
			break;
		}
		else if (return_code == SQL_SUCCESS_WITH_INFO) {
			if (!buffer.SetCount(actual_len, false))
				throw OdbcExceptionDiag(return_code, SQL_HANDLE_STMT,
					StatementHandle, "Call to '::SQLColAttribute()' returned "
					"'SQL_SUCCESS_WITH_INFO', but no returned lengths qualified "
					"for that return code --- indicated actual length is " +
					MLB::Utility::AnyToString(actual_len) + " bytes.");
		}
		else
			break;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN ColAttribute(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber,
	SQLUSMALLINT FieldIdentifier, SQLINTEGER &NumericAttributePtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(ColAttribute(StatementHandle, ColumnNumber, FieldIdentifier,
		NULL, SQL_IS_INTEGER, NULL, &NumericAttributePtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN ColAttribute(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber,
	SQLUSMALLINT FieldIdentifier, bool &NumericAttributePtr,
	OdbcThrowFlags::Flags throw_flags)
{
	SQLRETURN  return_code;
	SQLINTEGER tmp_value = 0;

	return_code = ColAttribute(StatementHandle, ColumnNumber, FieldIdentifier,
		tmp_value, throw_flags);

	NumericAttributePtr = NumericAttributePtr != 0;

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

