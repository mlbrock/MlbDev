// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the OdbcHandleStmt class.

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
OdbcHandleStmt::OdbcHandleStmt(SQLHDBC dbc_handle)
	:OdbcHandleBase<SQLHSTMT>(SQL_HANDLE_STMT, dbc_handle, "Stmt",
		GetStmtAttrBasic, SetStmtAttrBasic)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcHandleStmt::~OdbcHandleStmt()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ResultInfoColumnsSet &OdbcHandleStmt::GetResultInfoColumns(
	ResultInfoColumnsSet &out_list, const char *CatalogName,
	const char *SchemaName, const char *TableName, const char *ColumnName,
	OdbcThrowFlags::Flags throw_flags) const
{
	ResultInfoColumnsSet tmp_list;

	try {
		Columns(CatalogName, SchemaName, TableName, ColumnName,  throw_flags);
		SQLSMALLINT col_count;
		NumResultCols(&col_count);
		SQLRETURN            row_return_code;
		ResultInfoColumnsRaw raw_data;
		raw_data.Bind(*this, col_count);
		while (SQL_SUCCEEDED(row_return_code = Fetch())) {
			tmp_list.insert(raw_data);
			raw_data.Clear();
		}
 	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to load column information: " +
			std::string(except.what()));
	}

  	out_list.swap(tmp_list);

	return(out_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ResultInfoColumnsSet OdbcHandleStmt::GetResultInfoColumns(
	const char *CatalogName, const char *SchemaName, const char *TableName,
	const char *ColumnName, OdbcThrowFlags::Flags throw_flags) const
{
	ResultInfoColumnsSet out_list;

	return(GetResultInfoColumns(out_list, CatalogName, SchemaName, TableName,
		ColumnName, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ResultInfoTablesSet &OdbcHandleStmt::GetResultInfoTables(
	ResultInfoTablesSet &out_list, const char *CatalogName,
	const char *SchemaName, const char *TableName, const char *TableType,
	OdbcThrowFlags::Flags throw_flags) const
{
	ResultInfoTablesSet tmp_list;

	try {
		Tables(CatalogName, SchemaName, TableName, TableType,  throw_flags);
		SQLSMALLINT col_count;
		NumResultCols(&col_count);
		SQLRETURN           row_return_code;
		ResultInfoTablesRaw raw_data;
		raw_data.Bind(*this, col_count);
		while (SQL_SUCCEEDED(row_return_code = Fetch())) {
			tmp_list.insert(raw_data);
			raw_data.Clear();
		}
 	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to load table information: " +
			std::string(except.what()));
	}

  	out_list.swap(tmp_list);

	return(out_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ResultInfoTablesSet OdbcHandleStmt::GetResultInfoTables(const char *CatalogName,
	const char *SchemaName, const char *TableName, const char *TableType,
	OdbcThrowFlags::Flags throw_flags) const
{
	ResultInfoTablesSet out_list;

	return(GetResultInfoTables(out_list, CatalogName, SchemaName, TableName,
		TableType, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::BindCol(SQLUSMALLINT ColumnNumber,
	SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StrLen_or_IndPtr, OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::BindCol(GetNativeHandle(), ColumnNumber,
		TargetType, TargetValuePtr, BufferLength, StrLen_or_IndPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::Cancel(OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::Cancel(GetNativeHandle(), throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::ColAttribute(SQLUSMALLINT ColumnNumber,
	SQLUSMALLINT FieldIdentifier, SQLPOINTER CharacterAttributePtr,
	SQLSMALLINT BufferLength, SQLSMALLINT *StringLengthPtr,
	SQLPOINTER NumericAttributePtr, OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::ColAttribute(GetNativeHandle(), ColumnNumber,
		FieldIdentifier, CharacterAttributePtr, BufferLength, StringLengthPtr,
		NumericAttributePtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::ColAttribute(SQLUSMALLINT ColumnNumber,
	SQLUSMALLINT FieldIdentifier, std::string &CharacterAttributePtr,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::ColAttribute(GetNativeHandle(), ColumnNumber,
		FieldIdentifier, CharacterAttributePtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::ColAttribute(SQLUSMALLINT ColumnNumber,
	SQLUSMALLINT FieldIdentifier, SQLINTEGER &NumericAttributePtr,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::ColAttribute(GetNativeHandle(), ColumnNumber,
		FieldIdentifier, NumericAttributePtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::ColAttribute(SQLUSMALLINT ColumnNumber,
	SQLUSMALLINT FieldIdentifier, bool &NumericAttributePtr,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::ColAttribute(GetNativeHandle(), ColumnNumber,
		FieldIdentifier, NumericAttributePtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::Columns(SQLCHAR *CatalogName, SQLSMALLINT NameLength1,
	SQLCHAR *SchemaName, SQLSMALLINT NameLength2, SQLCHAR *TableName,
	SQLSMALLINT NameLength3, SQLCHAR *ColumnName, SQLSMALLINT NameLength4,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::Columns(GetNativeHandle(), CatalogName, NameLength1,
		SchemaName, NameLength2, TableName, NameLength3, ColumnName, NameLength4,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::Columns(const char *CatalogName,
	const char *SchemaName, const char *TableName, const char *ColumnName,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::Columns(GetNativeHandle(), CatalogName, SchemaName,
		TableName, ColumnName, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::DescribeCol(SQLSMALLINT ColumnNumber,
	SQLCHAR *ColumnName, SQLSMALLINT BufferLength, SQLSMALLINT *NameLengthPtr,
	SQLSMALLINT *DataTypePtr, SQLUINTEGER *ColumnSizePtr,
	SQLSMALLINT *DecimalDigitsPtr, SQLSMALLINT *NullablePtr,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::DescribeCol(GetNativeHandle(), ColumnNumber,
		ColumnName, BufferLength, NameLengthPtr, DataTypePtr, ColumnSizePtr,
		DecimalDigitsPtr, NullablePtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::DescribeCol(SQLSMALLINT ColumnNumber,
	std::string &ColumnName, SQLSMALLINT *DataTypePtr,
	SQLUINTEGER *ColumnSizePtr, SQLSMALLINT *DecimalDigitsPtr,
	SQLSMALLINT *NullablePtr, OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::DescribeCol(GetNativeHandle(), ColumnNumber,
		ColumnName, DataTypePtr, ColumnSizePtr, DecimalDigitsPtr,
		NullablePtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::ExecDirect(SQLCHAR *StatementText,
	SQLINTEGER TextLength, OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::ExecDirect(GetNativeHandle(), StatementText,
		TextLength, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::ExecDirect(const std::string &StatementText,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::ExecDirect(GetNativeHandle(), StatementText,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::ExtendedFetch(SQLUSMALLINT FetchOrientation,
	SQLINTEGER FetchOffset, SQLUINTEGER *RowCountPtr,
	SQLUSMALLINT *RowStatusArray, OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::ExtendedFetch(GetNativeHandle(), 
		FetchOrientation, FetchOffset, RowCountPtr, RowStatusArray, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::Fetch(OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::Fetch(GetNativeHandle(), throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::FetchScroll(SQLSMALLINT FetchOrientation,
	SQLINTEGER FetchOffset, OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::FetchScroll(GetNativeHandle(), FetchOrientation,
		FetchOffset, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::GetData(SQLUSMALLINT ColumnNumber,
	SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StrLen_or_IndPtr, OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::GetData(GetNativeHandle(), ColumnNumber,
		TargetType, TargetValuePtr, BufferLength, StrLen_or_IndPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::MoreResults(OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::MoreResults(GetNativeHandle(), throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::NumResultCols(SQLSMALLINT *ColumnCountPtr,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::NumResultCols(GetNativeHandle(), ColumnCountPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::RowCount(SQLINTEGER *RowCountPtr,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::RowCount(GetNativeHandle(), RowCountPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::RowCount(SQLINTEGER &RowCountPtr,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::RowCount(GetNativeHandle(), RowCountPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::Tables(SQLCHAR *CatalogName, SQLSMALLINT NameLength1,
	SQLCHAR *SchemaName, SQLSMALLINT NameLength2, SQLCHAR *TableName,
	SQLSMALLINT NameLength3, SQLCHAR *TableType, SQLSMALLINT NameLength4,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::Tables(GetNativeHandle(), CatalogName, NameLength1,
		SchemaName, NameLength2, TableName, NameLength3, TableType, NameLength4,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleStmt::Tables(const char *CatalogName,
	const char *SchemaName, const char *TableName, const char *TableType,
	OdbcThrowFlags::Flags throw_flags) const
{
	return(MLB::OdbcWrapper::Tables(GetNativeHandle(), CatalogName, SchemaName,
		TableName, TableType, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#ifdef TEST_MAIN

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_array.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

using namespace MLB::OdbcWrapper;

// ////////////////////////////////////////////////////////////////////////////
void TEST_OdbcHandleStmt_GetData(OdbcHandleDbcSPtr &odbc_dbc_sptr,
	std::vector<std::string> &table_list, int &return_code)
{
	const char *this_name = "TEST_OdbcHandleStmt_GetData()";

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << this_name << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		std::vector<std::string> tmp_table_list;
		OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
		std::cout << "   Statement created." << std::endl;
		odbc_stmt_sptr->Tables();
		SQLSMALLINT col_count;
		odbc_stmt_sptr->NumResultCols(&col_count);
		std::cout << "   Total of " << col_count << " columns located in "
			"tables." << std::endl;
		unsigned int row_count = 0;
		SQLRETURN    row_return_code;
		while (SQL_SUCCEEDED(row_return_code = odbc_stmt_sptr->Fetch())) {
			std::cout << "   " << std::setw(10) << ++row_count << ": " <<
				std::flush;
			SQLRETURN    col_return_code;
			SQLINTEGER   indicator;
			char         buffer[4095 + 1];
			SQLSMALLINT  col_index;
			for (col_index = 1; col_index <= col_count; ++col_index) {
				col_return_code = odbc_stmt_sptr->GetData(col_index, SQL_C_CHAR,
					buffer, sizeof(buffer), &indicator);
				if ((col_index == 3) && (indicator != SQL_NULL_DATA))
					tmp_table_list.push_back(buffer);
				std::cout <<
					((col_index > 1) ? "|" : "") << 
					((indicator == SQL_NULL_DATA) ? "*NULL*" : buffer) <<
					std::flush;
			}
			std::cout << std::endl;
		}
		std::cout << "   ---> " << row_count << " rows found." << std::endl;
		table_list.swap(tmp_table_list);
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_OdbcHandleStmt_BindCol(OdbcHandleDbcSPtr &odbc_dbc_sptr,
	std::vector<std::string> &table_list, int &return_code)
{
	const char *this_name = "TEST_OdbcHandleStmt_BindCol()";

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << this_name << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		std::vector<std::string> tmp_table_list;
		OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
		std::cout << "   Statement created." << std::endl;
		odbc_stmt_sptr->Tables();
		SQLSMALLINT col_count;
		odbc_stmt_sptr->NumResultCols(&col_count);
		std::cout << "   Total of " << col_count << " columns located in "
			"tables." << std::endl;
		typedef char BufferType[1023 + 1];
		boost::shared_array<BufferType> buffer_list(new BufferType[col_count]);
		boost::shared_array<SQLLEN>     ind_list(new SQLLEN[col_count]);
		SQLSMALLINT col_index;
		for (col_index = 0; col_index < col_count; ++col_index)
			odbc_stmt_sptr->BindCol(col_index + 1, SQL_C_CHAR,
				buffer_list[col_index], sizeof(buffer_list[col_index]),
				&(ind_list[col_index]));
		unsigned int row_count = 0;
		SQLRETURN    row_return_code;
		while (SQL_SUCCEEDED(row_return_code = odbc_stmt_sptr->Fetch())) {
			std::cout << "   " << std::setw(10) << ++row_count << ": " <<
				std::flush;
			for (col_index = 0; col_index < col_count; ++col_index) {
				std::cout <<
					((col_index > 0) ? "|" : "") << 
					((ind_list[col_index] == SQL_NULL_DATA) ? "*NULL*" :
					buffer_list[col_index]) << std::flush;
			}
			std::cout << std::endl;
		}
		std::cout << "   ---> " << row_count << " rows found." << std::endl;
		table_list.swap(tmp_table_list);
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ColumnContainerType>
	void TEST_EmitColumnSet(const ColumnContainerType &column_set)
{
	ColumnContainerType::const_iterator iter_b(column_set.begin());
	ColumnContainerType::const_iterator iter_e(column_set.end());

	for ( ; iter_b != iter_e; ++iter_b)
		std::cout << "   " << std::setw(4) << iter_b->ordinal_position_ << " " <<
			iter_b->column_name_ << std::endl;

	std::cout << "   " << MLB::Utility::PadLeft("", 76, '-') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_OdbcHandleStmt_GetResultInfoTables(OdbcHandleDbcSPtr &odbc_dbc_sptr,
	std::vector<std::string> &table_list, int &return_code)
{
	const char *this_name = "TEST_OdbcHandleStmt_GetResultInfoTables()";

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << this_name << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		ResultInfoTablesSet tmp_table_set;
		{
			OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
			odbc_stmt_sptr->GetResultInfoTables(tmp_table_set);
		}
		ResultInfoTablesSetIterC iter_b(tmp_table_set.begin());
		ResultInfoTablesSetIterC iter_e(tmp_table_set.end());
		std::vector<std::string> tmp_table_list;
		for ( ; iter_b != iter_e; ++iter_b) {
			OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
			std::cout << *iter_b << std::endl;
			ResultInfoColumnsSet tmp_column_set;
			odbc_stmt_sptr->GetResultInfoColumns(tmp_column_set, 
				iter_b->table_cat_.c_str(), iter_b->table_schem_.c_str(),
				iter_b->table_name_.c_str(), "%");
			TEST_EmitColumnSet(tmp_column_set);
			TEST_EmitColumnSet(ResultInfoColumnsSetByOrdinal(
				tmp_column_set.begin(), tmp_column_set.end()));
			tmp_table_list.push_back(iter_b->table_name_);
		}
		std::cout << "   ---> " <<
			MLB::Utility::AnyToString(tmp_table_set.size()) << " rows found." <<
			std::endl;
		table_list.swap(tmp_table_list);
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_OdbcHandleStmt_ExecDirect(OdbcHandleDbcSPtr &odbc_dbc_sptr,
	const std::vector<std::string> &table_list, int &return_code)
{
	const char *this_name = "TEST_OdbcHandleStmt_ExecDirect()";

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << this_name << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		std::vector<std::string>::const_iterator iter_b(table_list.begin());
		std::vector<std::string>::const_iterator iter_e(table_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
			std::cout << "   Statement created." << std::endl;
			std::string sql_string("select * from " + *iter_b);
			odbc_stmt_sptr->ExecDirect(sql_string);
			std::cout << "   Statement executed ('" << sql_string << "')" <<
				std::endl;
			SQLSMALLINT col_count;
			odbc_stmt_sptr->NumResultCols(&col_count);
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			std::cout << "   Total of " << col_count << " columns located in "
				"table '" << *iter_b << "':" << std::endl;
			ColumnAttrList col_attr_list;
			ColumnAttr::GetColumnAttrList(odbc_stmt_sptr->GetNativeHandle(),
				col_attr_list);
			std::copy(col_attr_list.begin(), col_attr_list.end(),
				std::ostream_iterator<ColumnAttrList::value_type>(std::cout, "\n"));
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			typedef char BufferType[1023 + 1];
			boost::shared_array<BufferType> buffer_list(new BufferType[col_count]);
			boost::shared_array<SQLLEN>     ind_list(new SQLLEN[col_count]);
			SQLSMALLINT                     col_index;
			for (col_index = 0; col_index < col_count; ++col_index)
				odbc_stmt_sptr->BindCol(col_index + 1, SQL_C_CHAR,
					buffer_list[col_index], sizeof(buffer_list[col_index]),
					&(ind_list[col_index]));
			unsigned int row_count = 0;
			SQLRETURN    row_return_code;
			while (SQL_SUCCEEDED(row_return_code = odbc_stmt_sptr->Fetch())) {
				std::cout << "   " << std::setw(10) << ++row_count << ": " <<
					std::flush;
				for (col_index = 0; col_index < col_count; ++col_index) {
					std::cout <<
						((col_index > 0) ? "|" : "") << 
						((ind_list[col_index] == SQL_NULL_DATA) ? "*NULL*" :
						buffer_list[col_index]) << std::flush;
				}
				std::cout << std::endl;
			}
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			std::cout << "   ---> " << row_count << " rows found  in table '" <<
				*iter_b << "'." << std::endl;
		}
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_OdbcHandleStmt(int &return_code)
{
	const char *this_name = "TEST_OdbcHandleStmt()";

	std::cout << this_name << std::endl;

	try {
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
		OdbcHandleDbcSPtr odbc_dbc_sptr(odbc_env.AddConnection());
		odbc_dbc_sptr->Connect(
//			"Database=AdventureWorksDW;Driver={SQL Native Client};Uid=sa;Server=(local);Pwd=S_a_0");
"Database=GDSP_LON_TRD_TST;Driver={SQL Native Client};Uid=sa;Server=(local);Pwd=S_a_0");
		std::cout << "   Connection created." << std::endl;
		std::vector<std::string> table_list_1;
		std::vector<std::string> table_list_2;
		std::vector<std::string> table_list_3;
		TEST_OdbcHandleStmt_GetData(odbc_dbc_sptr, table_list_1, return_code);
		TEST_OdbcHandleStmt_BindCol(odbc_dbc_sptr, table_list_2, return_code);
		TEST_OdbcHandleStmt_GetResultInfoTables(odbc_dbc_sptr, table_list_3,
			return_code);
		TEST_OdbcHandleStmt_ExecDirect(odbc_dbc_sptr, table_list_1, return_code);
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_OdbcHandleStmt(return_code);
	}
	catch (const std::exception &except) {
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

