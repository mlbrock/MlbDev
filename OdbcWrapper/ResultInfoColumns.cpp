// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ResultInfoColumns class.

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

#include <OdbcWrapper/OdbcHandle.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {
namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
ResultInfoColumnsRaw::ResultInfoColumnsRaw()
{
	Clear();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ResultInfoColumnsRaw::Clear()
{
	memset(this, '\0', sizeof(*this));

	table_cat_ind_         = SQL_NULL_DATA;
	table_schem_ind_       = SQL_NULL_DATA;
	table_name_ind_        = SQL_NULL_DATA;
	column_name_ind_       = SQL_NULL_DATA;
	data_type_ind_         = SQL_NULL_DATA;
	type_name_ind_         = SQL_NULL_DATA;
	column_size_ind_       = SQL_NULL_DATA;
	buffer_length_ind_     = SQL_NULL_DATA;
	decimal_digits_ind_    = SQL_NULL_DATA;
	num_prec_radix_ind_    = SQL_NULL_DATA;
	nullable_ind_          = SQL_NULL_DATA;
	remarks_ind_           = SQL_NULL_DATA;
	//	ODBC 3.0+ members...
	column_def_ind_        = SQL_NULL_DATA;
	sql_data_type_ind_     = SQL_NULL_DATA;
	sql_datetime_sub_ind_  = SQL_NULL_DATA;
	char_octet_length_ind_ = SQL_NULL_DATA;
	ordinal_position_ind_  = SQL_NULL_DATA;
	is_nullable_ind_       = SQL_NULL_DATA;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ResultInfoColumnsRaw::Bind(const OdbcHandleStmt &stmt_handle,
	SQLSMALLINT col_count)
{
	if ((col_count == 12) || ((col_count > 12) && (col_count < 18)))
		col_count = 12;
	else if (col_count >= 18)
		col_count = 18;
	else
		MLB::Utility::ThrowException("Expected to receive either exactly 12 "
			"or at least 18 result columns in response to a call to "
			"'::SQLColumns()', actually received " +
			MLB::Utility::AnyToString(col_count) + ".");

	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  1, SQL_C_CHAR,  table_cat_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  2, SQL_C_CHAR,  table_schem_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  3, SQL_C_CHAR,  table_name_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  4, SQL_C_CHAR,  column_name_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  5, SQL_C_SHORT, data_type_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  6, SQL_C_CHAR,  type_name_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  7, SQL_C_LONG,  column_size_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  8, SQL_C_LONG,  buffer_length_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle,  9, SQL_C_SHORT, decimal_digits_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 10, SQL_C_SHORT, num_prec_radix_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 11, SQL_C_SHORT, nullable_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 12, SQL_C_CHAR,  remarks_);

	if (col_count < 18)
		return;

	//	ODBC 3.0+ members...
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 13, SQL_C_CHAR,  column_def_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 14, SQL_C_SHORT, sql_data_type_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 15, SQL_C_SHORT, sql_datetime_sub_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 16, SQL_C_SHORT, char_octet_length_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 17, SQL_C_SHORT, ordinal_position_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 18, SQL_C_CHAR,  is_nullable_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ResultInfoColumnsRaw::operator < (const ResultInfoColumnsRaw &other) const
{
	return(CompareByName(*this, other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoColumnsRaw::CompareByName(const ResultInfoColumnsRaw &lhs,
	const ResultInfoColumnsRaw &rhs)
{
	return(CompareByName(lhs.table_cat_, lhs.table_schem_, lhs.table_name_,
		lhs.column_name_, rhs.table_cat_, rhs.table_schem_, rhs.table_name_,
		rhs.column_name_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoColumnsRaw::CompareByName(const char *table_cat_1,
	const char *table_schem_1, const char *table_name_1,
	const char *column_name_1, const char *table_cat_2,
	const char *table_schem_2, const char *table_name_2,
	const char *column_name_2)
{
	int cmp;

	if ((cmp = stricmp(table_cat_1, table_cat_2)) != 0)
		return(cmp);
	else if ((cmp = stricmp(table_schem_1, table_schem_2)) != 0)
		return(cmp);
	else if ((cmp = stricmp(table_name_1, table_name_2)) != 0)
		return(cmp);

	return(stricmp(column_name_1, column_name_2));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoColumnsRaw::CompareByOrdinal(const ResultInfoColumnsRaw &lhs,
	const ResultInfoColumnsRaw &rhs)
{
	return(CompareByOrdinal(lhs.table_cat_, lhs.table_schem_, lhs.table_name_,
		lhs.ordinal_position_, rhs.table_cat_, rhs.table_schem_, rhs.table_name_,
      rhs.ordinal_position_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoColumnsRaw::CompareByOrdinal(const char *table_cat_1,
  	const char *table_schem_1, const char *table_name_1,
	SQLSMALLINT ordinal_position_1, const char *table_cat_2,
	const char *table_schem_2, const char *table_name_2,
	SQLSMALLINT ordinal_position_2)
{
	int cmp;

	if ((cmp = stricmp(table_cat_1, table_cat_2)) != 0)
		return(cmp);
	else if ((cmp = stricmp(table_schem_1, table_schem_2)) != 0)
		return(cmp);
	else if ((cmp = stricmp(table_name_1, table_name_2)) != 0)
		return(cmp);

	return(ordinal_position_1 - ordinal_position_2);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper
} // namespace MLB

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
ResultInfoColumns::ResultInfoColumns(const char *table_cat,
	const char *table_schem, const char *table_name, const char *column_name,
	SQLSMALLINT data_type, const char *type_name, SQLINTEGER column_size,
	SQLINTEGER buffer_length, SQLSMALLINT decimal_digits,
	SQLSMALLINT num_prec_radix, SQLSMALLINT nullable, const char *remarks,
	const char *column_def, SQLSMALLINT sql_data_type,
	SQLSMALLINT sql_datetime_sub, SQLSMALLINT char_octet_length,
	SQLSMALLINT ordinal_position, const char *is_nullable)
	:table_cat_(table_cat)
	,table_schem_(table_schem)
	,table_name_(table_name)
	,column_name_(column_name)
	,data_type_(data_type)
	,type_name_(type_name)
	,column_size_(column_size)
	,buffer_length_(buffer_length)
	,decimal_digits_(decimal_digits)
	,num_prec_radix_(num_prec_radix)
	,nullable_(nullable)
	,remarks_(remarks)
	//	ODBC 3.0+ members...
	,column_def_(column_def)
	,sql_data_type_(sql_data_type)
	,sql_datetime_sub_(sql_datetime_sub)
	,char_octet_length_(char_octet_length)
	,ordinal_position_(ordinal_position)
	,is_nullable_(is_nullable)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ResultInfoColumns::ResultInfoColumns(const ResultInfoColumnsRaw &raw_info)
	:table_cat_((raw_info.table_cat_ind_                 == SQL_NULL_DATA) ?
		"" : raw_info.table_cat_)
	,table_schem_((raw_info.table_schem_ind_             == SQL_NULL_DATA) ?
		"" : raw_info.table_schem_)
	,table_name_((raw_info.table_name_ind_               == SQL_NULL_DATA) ?
		"" : raw_info.table_name_)
	,column_name_((raw_info.column_name_ind_             == SQL_NULL_DATA) ?
		"" : raw_info.column_name_)
	,data_type_((raw_info.data_type_ind_                 == SQL_NULL_DATA) ?
		0 : raw_info.data_type_)
	,type_name_((raw_info.type_name_ind_                 == SQL_NULL_DATA) ?
		"" : raw_info.type_name_)
	,column_size_((raw_info.column_size_ind_             == SQL_NULL_DATA) ?
		0 : raw_info.column_size_)
	,buffer_length_((raw_info.buffer_length_ind_         == SQL_NULL_DATA) ?
		0 : raw_info.buffer_length_)
	,decimal_digits_((raw_info.decimal_digits_ind_       == SQL_NULL_DATA) ?
		0 : raw_info.decimal_digits_)
	,num_prec_radix_((raw_info.num_prec_radix_ind_       == SQL_NULL_DATA) ?
		0 : raw_info.num_prec_radix_)
	,nullable_((raw_info.nullable_ind_                   == SQL_NULL_DATA) ?
		0 : raw_info.nullable_)
	,remarks_((raw_info.remarks_ind_                     == SQL_NULL_DATA) ?
		"" : raw_info.remarks_)
	//	ODBC 3.0+ members...
	,column_def_((raw_info.column_def_ind_               == SQL_NULL_DATA) ?
		"" : raw_info.column_def_)
	,sql_data_type_((raw_info.sql_data_type_ind_         == SQL_NULL_DATA) ?
		0 : raw_info.sql_data_type_)
	,sql_datetime_sub_((raw_info.sql_datetime_sub_ind_   == SQL_NULL_DATA) ?
		0 : raw_info.sql_datetime_sub_)
	,char_octet_length_((raw_info.char_octet_length_ind_ == SQL_NULL_DATA) ?
		0 : raw_info.char_octet_length_)
	,ordinal_position_((raw_info.ordinal_position_ind_   == SQL_NULL_DATA) ?
		0 : raw_info.ordinal_position_)
	,is_nullable_((raw_info.is_nullable_ind_             == SQL_NULL_DATA) ?
		"" : raw_info.is_nullable_)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ResultInfoColumns::ResultInfoColumns(const std::string &table_cat,
	const std::string &table_schem, const std::string &table_name,
	const std::string &column_name)
	:table_cat_(table_cat)
	,table_schem_(table_schem)
	,table_name_(table_name)
	,column_name_(column_name)
	,data_type_(0)
	,type_name_("")
	,column_size_(0)
	,buffer_length_(0)
	,decimal_digits_(0)
	,num_prec_radix_(0)
	,nullable_(0)
	,remarks_("")
	//	ODBC 3.0+ members...
	,column_def_("")
	,sql_data_type_(0)
	,sql_datetime_sub_(0)
	,char_octet_length_(0)
	,ordinal_position_(0)
	,is_nullable_("")
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ResultInfoColumns::ResultInfoColumns(const char *table_cat,
	const char *table_schem, const char *table_name, const char *column_name)
	:table_cat_(table_cat)
	,table_schem_(table_schem)
	,table_name_(table_name)
	,column_name_(column_name)
	,data_type_(0)
	,type_name_("")
	,column_size_(0)
	,buffer_length_(0)
	,decimal_digits_(0)
	,num_prec_radix_(0)
	,nullable_(0)
	,remarks_("")
	//	ODBC 3.0+ members...
	,column_def_("")
	,sql_data_type_(0)
	,sql_datetime_sub_(0)
	,char_octet_length_(0)
	,ordinal_position_(0)
	,is_nullable_("")
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ResultInfoColumns::operator < (const ResultInfoColumns &other) const
{
	return(CompareByName(*this, other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoColumns::CompareByName(const ResultInfoColumns &lhs,
	const ResultInfoColumns &rhs)
{
	return(ResultInfoColumnsRaw::CompareByName(lhs.table_cat_.c_str(),
		lhs.table_schem_.c_str(), lhs.table_name_.c_str(),
		lhs.column_name_.c_str(), rhs.table_cat_.c_str(),
		rhs.table_schem_.c_str(), rhs.table_name_.c_str(),
		rhs.column_name_.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoColumns::CompareByOrdinal(const ResultInfoColumns &lhs,
	const ResultInfoColumns &rhs)
{
	return(ResultInfoColumnsRaw::CompareByOrdinal(lhs.table_cat_.c_str(),
		lhs.table_schem_.c_str(), lhs.table_name_.c_str(), lhs.ordinal_position_,
		rhs.table_cat_.c_str(), rhs.table_schem_.c_str(), rhs.table_name_.c_str(),
		rhs.ordinal_position_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const ResultInfoColumns &datum)
{
	o_str
		<< datum.table_cat_         << ":"
		<< datum.table_schem_       << ":"
		<< datum.table_name_        << ":"
		<< datum.column_name_       << ":"
		<< datum.data_type_         << ":"
		<< datum.type_name_         << ":"
		<< datum.column_size_       << ":"
		<< datum.buffer_length_     << ":"
		<< datum.decimal_digits_    << ":"
		<< datum.num_prec_radix_    << ":"
		<< datum.nullable_          << ":"
		<< datum.remarks_           << ":"
		<< datum.column_def_        << ":"
		<< datum.sql_data_type_     << ":"
		<< datum.sql_datetime_sub_  << ":"
		<< datum.char_octet_length_ << ":"
		<< datum.ordinal_position_  << ":"
		<< datum.is_nullable_;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

