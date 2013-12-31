// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ColumnAttr class.

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
ColumnAttr::ColumnAttr()
	:column_number_(0)
	,auto_unique_value_(false)
	,base_column_name_()
	,base_table_name_()
	,case_sensitive_(false)
	,catalog_name_()
	,concise_type_(0)
	,count_(0)
	,display_size_(0)
	,fixed_prec_scale_(false)
	,label_()
	,length_(0)
	,literal_prefix_()
	,literal_suffix_()
	,local_type_name_()
	,name_()
	,nullable_(SQL_NULLABLE_UNKNOWN)
	,is_nullable_(false)
	,is_nullable_known_(false)
	,num_prec_radix_(0)
	,octet_length_(0)
	,precision_(0)
	,scale_(0)
	,schema_name_()
	,searchable_(SQL_PRED_NONE)
	,is_searchable_(false)
	,is_searchable_like_only_(false)
	,is_searchable_not_like_(false)
	,is_searchable_any_(false)
	,table_name_()
	,type_(0)
	,type_name_()
	,unnamed_(0)
	,is_unnamed_(false)
	,unsigned_(false)
	,updatable_(SQL_ATTR_READWRITE_UNKNOWN)
	,is_updatable_(false)
	,is_updatable_known_(false)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ColumnAttr::ColumnAttr(SQLHSTMT statement_handle, SQLSMALLINT column_number)
	:column_number_(column_number)
	,auto_unique_value_(false)
	,base_column_name_()
	,base_table_name_()
	,case_sensitive_(false)
	,catalog_name_()
	,concise_type_(0)
	,count_(0)
	,display_size_(0)
	,fixed_prec_scale_(false)
	,label_()
	,length_(0)
	,literal_prefix_()
	,literal_suffix_()
	,local_type_name_()
	,name_()
	,nullable_(SQL_NULLABLE_UNKNOWN)
	,is_nullable_(false)
	,is_nullable_known_(false)
	,num_prec_radix_(0)
	,octet_length_(0)
	,precision_(0)
	,scale_(0)
	,schema_name_()
	,searchable_(SQL_PRED_NONE)
	,is_searchable_(false)
	,is_searchable_like_only_(false)
	,is_searchable_not_like_(false)
	,is_searchable_any_(false)
	,table_name_()
	,type_(0)
	,type_name_()
	,unnamed_(0)
	,is_unnamed_(false)
	,unsigned_(false)
	,updatable_(SQL_ATTR_READWRITE_UNKNOWN)
	,is_updatable_(false)
	,is_updatable_known_(false)
{
#define ColumnAttr_PopulateInternal(stmt_hndl, col_num, fld_id, member_name)	\
	{																									\
		try {																							\
			ColAttribute(stmt_hndl, col_num, fld_id, member_name);					\
		}																								\
		catch (const std::exception &except) {												\
			MLB::Utility::Rethrow(except, "Attempt to populate member '" +			\
				std::string(#member_name) + "' of an instance of the "				\
				"'ColumnAttr' class failed: " + std::string(except.what()));		\
		}																								\
	}

	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_AUTO_UNIQUE_VALUE, auto_unique_value_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_BASE_COLUMN_NAME,	base_column_name_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_BASE_TABLE_NAME,	base_table_name_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_CASE_SENSITIVE,	case_sensitive_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_CATALOG_NAME,		catalog_name_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_CONCISE_TYPE,		concise_type_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_COUNT,				count_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_DISPLAY_SIZE,		display_size_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_FIXED_PREC_SCALE,	fixed_prec_scale_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_LABEL,				label_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_LENGTH,				length_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_LITERAL_PREFIX,	literal_prefix_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_LITERAL_SUFFIX,	literal_suffix_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_LOCAL_TYPE_NAME,	local_type_name_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_NAME,					name_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_NULLABLE,			nullable_);
	is_nullable_       = (nullable_ == SQL_NULLABLE);
	is_nullable_known_ = (nullable_ != SQL_NULLABLE_UNKNOWN);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_NUM_PREC_RADIX,	num_prec_radix_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_OCTET_LENGTH,		octet_length_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_PRECISION,			precision_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_SCALE,				scale_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_SCHEMA_NAME,		schema_name_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_SEARCHABLE,			searchable_);
	is_searchable_           = (searchable_ != SQL_PRED_NONE);
	is_searchable_like_only_ = (searchable_ == SQL_PRED_CHAR);
	is_searchable_not_like_  = (searchable_ == SQL_PRED_BASIC);
	is_searchable_any_       = (searchable_ == SQL_PRED_SEARCHABLE);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_TABLE_NAME,			table_name_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_TYPE,					type_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_TYPE_NAME,			type_name_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_UNNAMED,				unnamed_);
	is_unnamed_ = (unnamed_ == SQL_UNNAMED);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_UNSIGNED,			unsigned_);
	ColumnAttr_PopulateInternal(statement_handle, column_number,
		SQL_DESC_UPDATABLE,			updatable_);
	is_updatable_       = (updatable_ == SQL_ATTR_WRITE);
	is_updatable_known_ = (updatable_ != SQL_ATTR_READWRITE_UNKNOWN);

#undef ColumnAttr_PopulateInternal
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ColumnAttr::swap(ColumnAttr &other)
{
	std::swap(column_number_, other.column_number_);
	std::swap(auto_unique_value_, other.auto_unique_value_);
	base_column_name_.swap(other.base_column_name_);
	base_table_name_.swap(other.base_table_name_);
	std::swap(case_sensitive_, other.case_sensitive_);
	catalog_name_.swap(other.catalog_name_);
	std::swap(concise_type_, other.concise_type_);
	std::swap(count_, other.count_);
	std::swap(display_size_, other.display_size_);
	std::swap(fixed_prec_scale_, other.fixed_prec_scale_);
	label_.swap(other.label_);
	std::swap(length_, other.length_);
	literal_prefix_.swap(other.literal_prefix_);
	literal_suffix_.swap(other.literal_suffix_);
	local_type_name_.swap(other.local_type_name_);
	name_.swap(other.name_);
	std::swap(nullable_, other.nullable_);
	std::swap(is_nullable_, other.is_nullable_);
	std::swap(is_nullable_known_, other.is_nullable_known_);
	std::swap(num_prec_radix_, other.num_prec_radix_);
	std::swap(octet_length_, other.octet_length_);
	std::swap(precision_, other.precision_);
	std::swap(scale_, other.scale_);
	schema_name_.swap(other.schema_name_);
	std::swap(searchable_, other.searchable_);
	std::swap(is_searchable_, other.is_searchable_);
	std::swap(is_searchable_like_only_, other.is_searchable_like_only_);
	std::swap(is_searchable_not_like_, other.is_searchable_not_like_);
	std::swap(is_searchable_any_, other.is_searchable_any_);
	table_name_.swap(other.table_name_);
	std::swap(type_, other.type_);
	type_name_.swap(other.type_name_);
	std::swap(unnamed_, other.unnamed_);
	std::swap(is_unnamed_, other.is_unnamed_);
	std::swap(unsigned_, other.unsigned_);
	std::swap(updatable_, other.updatable_);
	std::swap(is_updatable_, other.is_updatable_);
	std::swap(is_updatable_known_, other.is_updatable_known_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ColumnAttr::operator < (const ColumnAttr &other) const
{
	return(column_number_ < other.column_number_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ColumnAttr::NullabilityIsKnown() const
{
	return(is_nullable_known_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ColumnAttr::IsNullable() const
{
	return(is_nullable_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ColumnAttr::ToStringTest() const
{
	std::ostringstream o_str;

	o_str
		<< std::setw(4) << column_number_ << " "
		<< std::left << std::setw(31) << label_ << " "
		<< std::right
		<< std::setw(6) << concise_type_ << " "
		<< std::setw(6) << type_ << " "
		<< std::setw(10) << octet_length_ << " "
		<< std::setw(3) << type_name_;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ColumnAttr::LoadColumnAttr(SQLHSTMT statement_handle,
	SQLSMALLINT column_number)
{
	ColumnAttr(statement_handle, column_number).swap(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ColumnAttrList &ColumnAttr::GetColumnAttrList(SQLHSTMT statement_handle,
	ColumnAttrList &out_list)
{
	SQLSMALLINT col_count;

	NumResultCols(statement_handle, &col_count);

	ColumnAttrList tmp_list;

	tmp_list.resize(col_count);

	SQLSMALLINT col_index;

	for (col_index = 0; col_index < col_count; ++col_index)
		ColumnAttr(statement_handle, col_index + 1).swap(tmp_list[col_index]);

	out_list.swap(tmp_list);

	return(out_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ColumnAttrList ColumnAttr::GetColumnAttrList(SQLHSTMT statement_handle)
{
	ColumnAttrList out_list;

	return(GetColumnAttrList(statement_handle, out_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ColumnAttrSet &ColumnAttr::GetColumnAttrSet(
	SQLHSTMT statement_handle, ColumnAttrSet &out_set)
{
	ColumnAttrList tmp_list;

	GetColumnAttrList(statement_handle, tmp_list);

	ColumnAttrSet(tmp_list.begin(), tmp_list.end()).swap(out_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const ColumnAttr &datum)
{
	o_str
		<< std::setw(4) << datum.column_number_ << " "
		<< datum.catalog_name_ << " "
		<< datum.base_table_name_ << " "
		<< datum.base_column_name_ << " "
		<<	datum.local_type_name_;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#ifdef TEST_MAIN

#include <OdbcWrapper/OdbcHandle.hpp>

using namespace MLB::OdbcWrapper;

// ////////////////////////////////////////////////////////////////////////////
struct TEST_OdbcReverseType {
	const char *type_name_;
	int         type_value_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#define TEST_INIT_ODBC_REV_TYPE(type_name)	\
	{	#type_name,	type_name	}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const TEST_OdbcReverseType TEST_OdbcReverseTypeList[] = {
	TEST_INIT_ODBC_REV_TYPE(SQL_CHAR),
	TEST_INIT_ODBC_REV_TYPE(SQL_NUMERIC),
	TEST_INIT_ODBC_REV_TYPE(SQL_DECIMAL),
	TEST_INIT_ODBC_REV_TYPE(SQL_INTEGER),
	TEST_INIT_ODBC_REV_TYPE(SQL_SMALLINT),
	TEST_INIT_ODBC_REV_TYPE(SQL_FLOAT),
	TEST_INIT_ODBC_REV_TYPE(SQL_REAL),
	TEST_INIT_ODBC_REV_TYPE(SQL_DOUBLE),
	TEST_INIT_ODBC_REV_TYPE(SQL_DATETIME),
	TEST_INIT_ODBC_REV_TYPE(SQL_INTERVAL),
	TEST_INIT_ODBC_REV_TYPE(SQL_TIME),
	TEST_INIT_ODBC_REV_TYPE(SQL_TIMESTAMP),
	TEST_INIT_ODBC_REV_TYPE(SQL_VARCHAR),
	TEST_INIT_ODBC_REV_TYPE(SQL_TYPE_DATE),
	TEST_INIT_ODBC_REV_TYPE(SQL_TYPE_TIME),
	TEST_INIT_ODBC_REV_TYPE(SQL_TYPE_TIMESTAMP),
	TEST_INIT_ODBC_REV_TYPE(SQL_LONGVARCHAR),
	TEST_INIT_ODBC_REV_TYPE(SQL_BINARY),
	TEST_INIT_ODBC_REV_TYPE(SQL_VARBINARY),
	TEST_INIT_ODBC_REV_TYPE(SQL_LONGVARBINARY),
	TEST_INIT_ODBC_REV_TYPE(SQL_BIGINT),
	TEST_INIT_ODBC_REV_TYPE(SQL_TINYINT),
	TEST_INIT_ODBC_REV_TYPE(SQL_BIT),
	TEST_INIT_ODBC_REV_TYPE(SQL_GUID),
	TEST_INIT_ODBC_REV_TYPE(SQL_WCHAR),			//	Unicode support: sqlucode.h
	TEST_INIT_ODBC_REV_TYPE(SQL_WVARCHAR),		//	Unicode support: sqlucode.h
	TEST_INIT_ODBC_REV_TYPE(SQL_WLONGVARCHAR)	//	Unicode support: sqlucode.h
};
//	----------------------------------------------------------------------------
const unsigned int         TEST_OdbcReverseTypeCount  =
	sizeof(TEST_OdbcReverseTypeList) / sizeof(TEST_OdbcReverseTypeList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char         *TEST_SelectList[] = {
	"SELECT * FROM GDSP_LON_TRD_TST.dbo.TableOfColumnTypes",
/*
	"SELECT * FROM GDSP_LON_TRD_TST.dbo.attribute_type",
	"SELECT * FROM GDSP_LON_TRD_TST.dbo.attribute",
	"SELECT * FROM GDSP_LON_TRD_TST.dbo.attribute_tag",
	"SELECT * FROM GDSP_LON_TRD_TST.dbo.attribute A,\n"
		"              GDSP_LON_TRD_TST.dbo.attribute_type B\n"
		"   WHERE A.attribute_type_name = B.attribute_type_name"
*/
};
const unsigned int  TEST_SelectCount  =
	sizeof(TEST_SelectList) / sizeof(TEST_SelectList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitTypeReverseList()
{
 	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "Type Mapping Reversal Attempt:" << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;

  unsigned int count_1;

	for (count_1 = 0; count_1 < TEST_OdbcReverseTypeCount; ++count_1)
		std::cout << std::left << std::setw(20) <<
			TEST_OdbcReverseTypeList[count_1].type_name_ << std::right << " " <<
			std::setw(6) << TEST_OdbcReverseTypeList[count_1].type_value_ <<
			std::endl;

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void TEST_EmitList(const ContainerType &data_list)
{
	ContainerType::const_iterator iter_b(data_list.begin());
	ContainerType::const_iterator iter_e(data_list.end());

	for ( ; iter_b != iter_e; ++iter_b)
		std::cout
			<< iter_b->ToStringTest() << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_PerformTest(int &return_code, OdbcHandleDbcSPtr &odbc_dbc_sptr,
	const char *sql_string)
{
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "SQL Statement:" << std::endl << sql_string << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;

	try {
		OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
		{
			std::cout << "ColumnAttr:" << std::endl;
			odbc_stmt_sptr->ExecDirect(sql_string);
			ColumnAttrList col_attr_list;
			ColumnAttr::GetColumnAttrList(odbc_stmt_sptr->GetNativeHandle(),
				col_attr_list);
			TEST_EmitList(col_attr_list);
		}
		odbc_stmt_sptr->MoreResults();
		std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		{
			std::cout << "ColumnAttrSimple:" << std::endl;
			odbc_stmt_sptr->ExecDirect(sql_string);
			ColumnAttrSimpleList col_attr_list;
			ColumnAttrSimple::GetColumnAttrList(odbc_stmt_sptr->GetNativeHandle(),
				col_attr_list);
			TEST_EmitList(col_attr_list);
		}
	}
	catch (const std::exception &except) {
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_InternalList(int &return_code, OdbcHandleDbcSPtr &odbc_dbc_sptr)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < TEST_SelectCount; ++count_1)
		TEST_PerformTest(return_code, odbc_dbc_sptr, TEST_SelectList[count_1]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_CommandLine(int argc, char **argv, int &return_code,
	OdbcHandleDbcSPtr &odbc_dbc_sptr)
{
	int count_1;

	for (count_1 = 1; count_1 < argc; ++count_1)
		TEST_PerformTest(return_code, odbc_dbc_sptr, argv[count_1]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_EmitTypeReverseList();
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
		OdbcHandleDbcSPtr odbc_dbc_sptr(odbc_env.AddConnection());
		odbc_dbc_sptr->Connect("Database=GDSP_LON_TRD_TST;"
			"Driver={SQL Native Client};Uid=sa;Server=(local);Pwd=S_a_0");
		if (argc > 1)
			TEST_CommandLine(argc, argv, return_code, odbc_dbc_sptr);
		else
			TEST_InternalList(return_code, odbc_dbc_sptr);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR IN REGRESSION TEST FOR "
			"'OdbcWrapper/ColumnAttr.cpp': " << except.what() << std::endl <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

