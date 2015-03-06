// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ResultInfoTables class.

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
ResultInfoTablesRaw::ResultInfoTablesRaw()
{
	Clear();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ResultInfoTablesRaw::Clear()
{
	memset(this, '\0', sizeof(*this));

	table_cat_ind_   = SQL_NULL_DATA;
	table_schem_ind_ = SQL_NULL_DATA;
	table_name_ind_  = SQL_NULL_DATA;
	table_type_ind_  = SQL_NULL_DATA;
	remarks_ind_     = SQL_NULL_DATA;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ResultInfoTablesRaw::Bind(const OdbcHandleStmt &stmt_handle,
	SQLSMALLINT col_count)
{
	if (col_count >= 5)
		col_count = 5;
	else
		MLB::Utility::ThrowException("Expected to receive at least 5 result "
			"columns in response to a call to '::SQLTables()', actually "
			"received " + MLB::Utility::AnyToString(col_count) + ".");

	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 1, SQL_C_CHAR, table_cat_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 2, SQL_C_CHAR, table_schem_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 3, SQL_C_CHAR, table_name_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 4, SQL_C_CHAR, table_type_);
	ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, 5, SQL_C_CHAR, remarks_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ResultInfoTablesRaw::operator < (const ResultInfoTablesRaw &other) const
{
	return(CompareByName(*this, other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoTablesRaw::CompareByName(const ResultInfoTablesRaw &lhs,
	const ResultInfoTablesRaw &rhs)
{
	return(CompareByName(lhs.table_cat_, lhs.table_schem_, lhs.table_name_,
		rhs.table_cat_, rhs.table_schem_, rhs.table_name_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoTablesRaw::CompareByName(const char *table_cat_1,
	const char *table_schem_1, const char *table_name_1, const char *table_cat_2,
	const char *table_schem_2, const char *table_name_2)
{
	int cmp;

	if ((cmp = stricmp(table_cat_1, table_cat_2)) != 0)
		return(cmp);
	else if ((cmp = stricmp(table_schem_1, table_schem_2)) != 0)
		return(cmp);

	return(stricmp(table_name_1, table_name_2));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper
} // namespace MLB

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
ResultInfoTables::ResultInfoTables(const char *table_cat,
	const char *table_schem, const char *table_name, const char *table_type,
	const char *remarks)
	:table_cat_(table_cat)
	,table_schem_(table_schem)
	,table_name_(table_name)
	,table_type_(table_type)
	,remarks_(remarks)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ResultInfoTables::ResultInfoTables(const ResultInfoTablesRaw &raw_info)
	:table_cat_((raw_info.table_cat_ind_     == SQL_NULL_DATA) ?
		"" : raw_info.table_cat_)
	,table_schem_((raw_info.table_schem_ind_ == SQL_NULL_DATA) ?
		"" : raw_info.table_schem_)
	,table_name_((raw_info.table_name_ind_   == SQL_NULL_DATA) ?
		"" : raw_info.table_name_)
	,table_type_((raw_info.table_type_ind_   == SQL_NULL_DATA) ?
		"" : raw_info.table_type_)
	,remarks_((raw_info.remarks_ind_         == SQL_NULL_DATA) ?
		"" : raw_info.remarks_)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ResultInfoTables::operator < (const ResultInfoTables &other) const
{
	return(CompareByName(*this, other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int ResultInfoTables::CompareByName(const ResultInfoTables &lhs,
	const ResultInfoTables &rhs)
{
	return(ResultInfoTablesRaw::CompareByName(lhs.table_cat_.c_str(),
		lhs.table_schem_.c_str(), lhs.table_name_.c_str(), rhs.table_cat_.c_str(),
		rhs.table_schem_.c_str(), rhs.table_name_.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const ResultInfoTables &datum)
{
	o_str
		<< datum.table_cat_   << ":"
		<< datum.table_schem_ << ":"
		<< datum.table_name_  << ":"
		<< datum.table_type_  << ":"
		<< datum.remarks_;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

