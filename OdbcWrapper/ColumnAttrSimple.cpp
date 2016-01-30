// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ColumnAttrSimple class.

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
ColumnAttrSimple::ColumnAttrSimple()
	:column_number_(0)
	,column_name_()
	,data_type_(0)
	,column_size_(0)
	,decimal_digits_(0)
	,nullability_(SQL_NULLABLE_UNKNOWN)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ColumnAttrSimple::ColumnAttrSimple(SQLHSTMT statement_handle,
	SQLSMALLINT column_number)
	:column_number_(column_number)
	,column_name_()
	,data_type_(0)
	,column_size_(0)
	,decimal_digits_(0)
	,nullability_(SQL_NULLABLE_UNKNOWN)
{
	DescribeCol(statement_handle, column_number_, column_name_,
		&data_type_, &column_size_, &decimal_digits_, &nullability_,
		OdbcThrowFlags::Ok_SWI);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ColumnAttrSimple::swap(ColumnAttrSimple &other)
{
	std::swap(column_number_, other.column_number_);
	column_name_.swap(other.column_name_);
	std::swap(data_type_, other.data_type_);
	std::swap(column_size_, other.column_size_);
	std::swap(decimal_digits_, other.decimal_digits_);
	std::swap(nullability_, other.nullability_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ColumnAttrSimple::operator < (const ColumnAttrSimple &other) const
{
	return(column_number_ < other.column_number_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ColumnAttrSimple::NullabilityIsKnown() const
{
	return(nullability_ != SQL_NULLABLE_UNKNOWN);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ColumnAttrSimple::IsNullable() const
{
	return(nullability_ != SQL_NULLABLE);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ColumnAttrSimple::LoadColumnAttr(SQLHSTMT statement_handle,
	SQLSMALLINT column_number)
{
	ColumnAttrSimple(statement_handle, column_number).swap(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ColumnAttrSimple::ToStringTest() const
{
	std::ostringstream o_str;

	o_str
		<< std::setw(4) << column_number_ << " "
		<< std::left << std::setw(31) << column_name_ << " "
		<< std::right
		<< std::setw(6) << data_type_ << " "
		<< std::setw(5) << column_size_;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ColumnAttrSimpleList &ColumnAttrSimple::GetColumnAttrList(
	SQLHSTMT statement_handle, ColumnAttrSimpleList &out_list)
{
	SQLSMALLINT col_count;

	NumResultCols(statement_handle, &col_count);

	ColumnAttrSimpleList tmp_list;

	tmp_list.resize(col_count);

	SQLSMALLINT col_index;

	for (col_index = 0; col_index < col_count; ++col_index)
		ColumnAttrSimple(statement_handle, col_index + 1).
			swap(tmp_list[col_index]);

	out_list.swap(tmp_list);

	return(out_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ColumnAttrSimpleList ColumnAttrSimple::GetColumnAttrList(
	SQLHSTMT statement_handle)
{
	ColumnAttrSimpleList out_list;

	return(GetColumnAttrList(statement_handle, out_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ColumnAttrSimpleSet &ColumnAttrSimple::GetColumnAttrSet(
	SQLHSTMT statement_handle, ColumnAttrSimpleSet &out_set)
{
	ColumnAttrSimpleList tmp_list;

	GetColumnAttrList(statement_handle, tmp_list);

	ColumnAttrSimpleSet(tmp_list.begin(), tmp_list.end()).swap(out_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const ColumnAttrSimple &datum)
{
	o_str
		<< std::setw(4) << datum.column_number_ << " "
		<< std::left << std::setw(32) << datum.column_name_ << std::right << " "
		<< std::setw(4) << datum.data_type_ << " "
		<< std::setw(10) << datum.column_size_ << " "
		<< std::setw(2)  << datum.decimal_digits_ << " "
		<< "Nullability=" << ((datum.NullabilityIsKnown()) ?
			MLB::Utility::AnyToString(datum.IsNullable()) : "Unknown");

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

