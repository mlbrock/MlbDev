// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the class 'OdbcDiagState'.

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

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
OdbcDiagState::OdbcDiagState()
	:cursor_row_count_(0)
	,dynamic_function_()
	,dynamic_function_code_(0)
	,number_(0)
	,returncode_(SQL_SUCCESS)
	,row_count_(0)
	,diag_record_list_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcDiagState::OdbcDiagState(SQLSMALLINT handle_type, SQLHANDLE handle_value,
	bool throw_on_diag_field_error)
	:cursor_row_count_(0)
	,dynamic_function_()
	,dynamic_function_code_(0)
	,number_(0)
	,returncode_(SQL_SUCCESS)
	,row_count_(0)
	,diag_record_list_()
{
	LoadDiagnosticRecordList(handle_type, handle_value,
		throw_on_diag_field_error);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t OdbcDiagState::LoadDiagnosticRecordList(SQLSMALLINT handle_type,
	SQLHANDLE handle_value, bool throw_on_diag_field_error)
{
	try {
		SQLINTEGER        tmp_cursor_row_count      = 0;
		std::string       tmp_dynamic_function;
		SQLINTEGER        tmp_dynamic_function_code = 0;
		SQLINTEGER        tmp_number                = 0;
		SQLRETURN         tmp_returncode            = 0;
		SQLINTEGER        tmp_row_count             = 0;
		OdbcDiagRecExList tmp_diag_record_list;
		if (handle_type == SQL_HANDLE_STMT)
			GetDiagField_SQLINTEGER(handle_type, handle_value, 0,
				SQL_DIAG_CURSOR_ROW_COUNT, tmp_cursor_row_count,
				throw_on_diag_field_error);
		if (handle_type == SQL_HANDLE_STMT)
			GetDiagField_String(handle_type, handle_value, 0,
				SQL_DIAG_DYNAMIC_FUNCTION, tmp_dynamic_function,
				throw_on_diag_field_error);
		if (handle_type == SQL_HANDLE_STMT)
			GetDiagField_SQLINTEGER(handle_type, handle_value, 0,
				SQL_DIAG_DYNAMIC_FUNCTION_CODE, tmp_dynamic_function_code,
				throw_on_diag_field_error);
		tmp_number = GetDiagRecordNumberCount(handle_type, handle_value);
		GetDiagField_SQLRETURN(handle_type, handle_value, 0,
			SQL_DIAG_RETURNCODE, tmp_returncode, throw_on_diag_field_error);
		if (handle_type == SQL_HANDLE_STMT)
			GetDiagField_SQLINTEGER(handle_type, handle_value, 0,
				SQL_DIAG_ROW_COUNT, tmp_row_count, throw_on_diag_field_error);
		if (tmp_number) {
			tmp_diag_record_list.reserve(tmp_number);
			SQLINTEGER count_1;
			for (count_1 = 1; count_1 <= tmp_number; ++count_1) {
				try {
					tmp_diag_record_list.push_back(
						OdbcDiagRecEx(handle_type, handle_value,
							static_cast<SQLSMALLINT>(count_1),
							throw_on_diag_field_error));
				}
				catch (const std::exception &except) {
					MLB::Utility::Rethrow(except, "Error occurred while attempting "
						"to construct diagnostic record element number " +
						MLB::Utility::AnyToString(count_1) + " out of a total of " +
						MLB::Utility::AnyToString(tmp_number) + " elements: " +
						std::string(except.what()));
				}
			}
		}
		std::swap(cursor_row_count_, tmp_cursor_row_count);
		dynamic_function_.swap(tmp_dynamic_function);
		std::swap(dynamic_function_code_, tmp_dynamic_function_code);
		std::swap(number_, tmp_number);
		std::swap(returncode_, tmp_returncode);
		std::swap(row_count_, tmp_row_count);
		diag_record_list_.swap(tmp_diag_record_list);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to construct an instance of "
			"'OdbcDiagState': " + std::string(except.what()));
	}

	return(diag_record_list_.size());
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

