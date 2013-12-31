// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the class 'OdbcDiagRecEx'.

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
OdbcDiagRecEx::OdbcDiagRecEx()
	:OdbcDiagRec()
	,class_origin_()
	,column_number_(0)
	,connection_name_()
	,message_text_ex_()
	,native_(0)
	,row_number_(0)
	,server_name_()
	,sql_state_ex_()
	,subclass_origin_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcDiagRecEx::OdbcDiagRecEx(SQLSMALLINT handle_type,
	SQLHANDLE handle_value, SQLSMALLINT record_number,
	bool throw_on_diag_field_error)
try
	:OdbcDiagRec(handle_type, handle_value, record_number)
	,class_origin_()
	,column_number_(0)
	,connection_name_()
	,message_text_ex_()
	,native_(0)
	,row_number_(0)
	,server_name_()
	,sql_state_ex_()
	,subclass_origin_()
{
	if (record_number_ == SQL_NO_DATA)
		OdbcDiagRecEx().swap(*this);

	GetDiagField_String(handle_type, handle_value, record_number,
		SQL_DIAG_CLASS_ORIGIN, class_origin_, throw_on_diag_field_error);
	if (handle_type == SQL_HANDLE_STMT)
		GetDiagField_SQLINTEGER(handle_type, handle_value, record_number,
			SQL_DIAG_COLUMN_NUMBER, column_number_, throw_on_diag_field_error);
	GetDiagField_String(handle_type, handle_value, record_number,
		SQL_DIAG_CONNECTION_NAME, connection_name_, throw_on_diag_field_error);
	GetDiagField_String(handle_type, handle_value, record_number,
		SQL_DIAG_MESSAGE_TEXT, message_text_ex_, throw_on_diag_field_error);
	GetDiagField_SQLINTEGER(handle_type, handle_value, record_number,
		SQL_DIAG_NATIVE, native_, throw_on_diag_field_error);
	if (handle_type == SQL_HANDLE_STMT)
		GetDiagField_SQLINTEGER(handle_type, handle_value, record_number,
			SQL_DIAG_ROW_NUMBER, row_number_, throw_on_diag_field_error);
	GetDiagField_String(handle_type, handle_value, record_number,
		SQL_DIAG_SERVER_NAME, server_name_, throw_on_diag_field_error);
	GetDiagField_String(handle_type, handle_value, record_number,
		SQL_DIAG_SQLSTATE, sql_state_ex_, throw_on_diag_field_error);
	GetDiagField_String(handle_type, handle_value, record_number,
		SQL_DIAG_SUBCLASS_ORIGIN, subclass_origin_, throw_on_diag_field_error);

	handle_type_    = handle_type;
	handle_value_   = handle_value;
	record_number_  = record_number;
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Attempt to construct an instance of the "
		"class 'OdbcDiagRecEx(" + MLB::Utility::AnyToString(handle_type) + ", " +
		MLB::Utility::AnyToString(handle_value) + ", " +
		MLB::Utility::AnyToString(record_number) + ")' failed: " +
		std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcDiagRecEx::OdbcDiagRecEx(const OdbcDiagRecEx &other)
	:OdbcDiagRec(other)
	,class_origin_(other.class_origin_)
	,column_number_(other.column_number_)
	,connection_name_(other.connection_name_)
	,message_text_ex_(other.message_text_ex_)
	,native_(other.native_)
	,row_number_(other.row_number_)
	,server_name_(other.server_name_)
	,sql_state_ex_(other.sql_state_ex_)
	,subclass_origin_(other.subclass_origin_)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcDiagRecEx::~OdbcDiagRecEx()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcDiagRecEx & OdbcDiagRecEx::operator = (const OdbcDiagRecEx &other)
{
	OdbcDiagRecEx tmp_datum(other);

	swap(tmp_datum);

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OdbcDiagRecEx::swap(OdbcDiagRecEx &other)
{
	OdbcDiagRec::swap(other);

	class_origin_.swap(other.class_origin_);
	std::swap(column_number_, other.column_number_);
	connection_name_.swap(other.connection_name_);
	message_text_ex_.swap(other.message_text_ex_);
	std::swap(native_, other.native_);
	std::swap(row_number_, other.row_number_);
	server_name_.swap(other.server_name_);
	sql_state_ex_.swap(other.sql_state_ex_);
	subclass_origin_.swap(other.subclass_origin_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcDiagRecEx::LoadDiagnosticRecord(SQLSMALLINT handle_type,
	SQLHANDLE handle_value, SQLSMALLINT record_number)
{
	OdbcDiagRecEx(handle_type, handle_value, record_number).swap(*this);

	return(record_number_ > 0);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

