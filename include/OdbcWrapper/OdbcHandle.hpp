// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Handle management include file for the ODBC wrapper
								library.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__OdbcHandle_hpp__HH

#define HH__MLB__OdbcHandle_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OdbcWrapper/OdbcWrapper.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
#define ODBCWRAPPER_BIND_COL_SIMPLE(stmt_handle, col_index, col_type,	\
	col_datum)																				\
	stmt_handle.BindCol(col_index, col_type, &(col_datum),					\
		sizeof(col_datum), &(col_datum##ind_))
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class OdbcHandleStmt;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER ResultInfoTablesRaw {
	ResultInfoTablesRaw();

	void Clear();
	void Bind(const OdbcHandleStmt &stmt_handle, SQLSMALLINT col_count);

	bool operator < (const ResultInfoTablesRaw &other) const;

	char        table_cat_[1023 + 1];
	char        table_schem_[1023 + 1];
	char        table_name_[1023 + 1];
	char        table_type_[1023 + 1];
	char        remarks_[1023 + 1];

	SQLLEN      table_cat_ind_;
	SQLLEN      table_schem_ind_;
	SQLLEN      table_name_ind_;
	SQLLEN      table_type_ind_;
	SQLLEN      remarks_ind_;

	static int CompareByName(const ResultInfoTablesRaw &lhs,
		const ResultInfoTablesRaw &rhs);
	static int CompareByName(const char *table_cat_1, const char *table_schem_1,
		const char *table_name_1, const char *table_cat_2,
		const char *table_schem_2, const char *table_name_2);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER ResultInfoColumnsRaw {
	ResultInfoColumnsRaw();

	void Clear();
	void Bind(const OdbcHandleStmt &stmt_handle, SQLSMALLINT col_count);

	bool operator < (const ResultInfoColumnsRaw &other) const;

	char        table_cat_[1023 + 1];
	char        table_schem_[1023 + 1];
	char        table_name_[1023 + 1];
	char        column_name_[1023 + 1];
	SQLSMALLINT data_type_;
	char        type_name_[1023 + 1];
	SQLINTEGER  column_size_;
	SQLINTEGER  buffer_length_;
	SQLSMALLINT decimal_digits_;
	SQLSMALLINT num_prec_radix_;
	SQLSMALLINT nullable_;
	char        remarks_[1023 + 1];
	//	ODBC 3.0+ members...
	char        column_def_[1023 + 1];
	SQLSMALLINT sql_data_type_;
	SQLSMALLINT sql_datetime_sub_;
	SQLSMALLINT char_octet_length_;
	SQLSMALLINT ordinal_position_;
	char        is_nullable_[1023 + 1];

	SQLLEN      table_cat_ind_;
	SQLLEN      table_schem_ind_;
	SQLLEN      table_name_ind_;
	SQLLEN      column_name_ind_;
	SQLLEN      data_type_ind_;
	SQLLEN      type_name_ind_;
	SQLLEN      column_size_ind_;
	SQLLEN      buffer_length_ind_;
	SQLLEN      decimal_digits_ind_;
	SQLLEN      num_prec_radix_ind_;
	SQLLEN      nullable_ind_;
	SQLLEN      remarks_ind_;
	//	ODBC 3.0+ members...
	SQLLEN      column_def_ind_;
	SQLLEN      sql_data_type_ind_;
	SQLLEN      sql_datetime_sub_ind_;
	SQLLEN      char_octet_length_ind_;
	SQLLEN      ordinal_position_ind_;
	SQLLEN      is_nullable_ind_;

	static int CompareByName(const ResultInfoColumnsRaw &lhs,
		const ResultInfoColumnsRaw &rhs);
	static int CompareByName(const char *table_cat_1, const char *table_schem_1,
		const char *table_name_1, const char *column_name_1,
		const char *table_cat_2, const char *table_schem_2,
		const char *table_name_2, const char *column_name_2);

	static int CompareByOrdinal(const ResultInfoColumnsRaw &lhs,
		const ResultInfoColumnsRaw &rhs);
	static int CompareByOrdinal(const char *table_cat_1,
		const char *table_schem_1, const char *table_name_1,
		SQLSMALLINT ordinal_position_1, const char *table_cat_2,
		const char *table_schem_2, const char *table_name_2,
		SQLSMALLINT ordinal_position_2);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER ResultInfoTables {
	ResultInfoTables(const char *table_cat, const char *table_schem,
		const char *table_name, const char *table_type, const char *remarks);
	ResultInfoTables(const ResultInfoTablesRaw &raw_info);

	bool operator < (const ResultInfoTables &other) const;

	std::string table_cat_;
	std::string table_schem_;
	std::string table_name_;
	std::string table_type_;
	std::string remarks_;

	static int CompareByName(const ResultInfoTables &lhs,
		const ResultInfoTables &rhs);
};
std::ostream & operator << (std::ostream &o_str,
	const ResultInfoTables &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<ResultInfoTables>          ResultInfoTablesSet;
typedef ResultInfoTablesSet::iterator       ResultInfoTablesSetIter;
typedef ResultInfoTablesSet::const_iterator ResultInfoTablesSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER ResultInfoColumns {
	ResultInfoColumns(const char *table_cat, const char *table_schem,
		const char *table_name, const char *column_name, SQLSMALLINT data_type,
		const char *type_name, SQLINTEGER column_size, SQLINTEGER buffer_length,
		SQLSMALLINT decimal_digits, SQLSMALLINT num_prec_radix,
		SQLSMALLINT nullable, const char *remarks,
		const char *column_def = "", SQLSMALLINT sql_data_type = 0,
		SQLSMALLINT sql_datetime_sub = 0, SQLSMALLINT char_octet_length = 0,
		SQLSMALLINT ordinal_position = 0, const char *is_nullable = "");
	ResultInfoColumns(const ResultInfoColumnsRaw &raw_info);
	//	Used for std::set<>.find()...
	ResultInfoColumns(const std::string &table_cat,
		const std::string &table_schem, const std::string &table_name,
		const std::string &column_name);
	//	Used for std::set<>.find()...
	ResultInfoColumns(const char *table_cat, const char *table_schem,
		const char *table_name, const char *column_name);

	bool operator < (const ResultInfoColumns &other) const;

	std::string table_cat_;
	std::string table_schem_;
	std::string table_name_;
	std::string column_name_;
	SQLSMALLINT data_type_;
	std::string type_name_;
	SQLINTEGER  column_size_;
	SQLINTEGER  buffer_length_;
	SQLSMALLINT decimal_digits_;
	SQLSMALLINT num_prec_radix_;
	SQLSMALLINT nullable_;
	std::string remarks_;
	//	ODBC 3.0+ members...
	std::string column_def_;
	SQLSMALLINT sql_data_type_;
	SQLSMALLINT sql_datetime_sub_;
	SQLSMALLINT char_octet_length_;
	SQLSMALLINT ordinal_position_;
	std::string is_nullable_;

	static int  CompareByName(const ResultInfoColumns &lhs,
		const ResultInfoColumns &rhs);
	static int  CompareByOrdinal(const ResultInfoColumns &lhs,
		const ResultInfoColumns &rhs);
	static bool LessThanByOrdinal(const ResultInfoColumns &lhs,
		const ResultInfoColumns &rhs)
	{
		return(CompareByOrdinal(lhs, rhs) < 0);
	}
};
std::ostream & operator << (std::ostream &o_str,
	const ResultInfoColumns &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER ResultInfoColumnsByOrdinalPredicate {
	bool operator () (const ResultInfoColumns &lhs,
		const ResultInfoColumns &rhs) const
	{
		return(ResultInfoColumns::CompareByOrdinal(lhs, rhs) < 0);
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<ResultInfoColumns>          ResultInfoColumnsSet;
typedef ResultInfoColumnsSet::iterator       ResultInfoColumnsSetIter;
typedef ResultInfoColumnsSet::const_iterator ResultInfoColumnsSetIterC;

typedef ResultInfoColumnsSet                 ResultInfoColumnsSetByName;
typedef ResultInfoColumnsSetIter             ResultInfoColumnsSetByNameIter;
typedef ResultInfoColumnsSetIterC            ResultInfoColumnsSetByNameIterC;

typedef std::set<ResultInfoColumns, ResultInfoColumnsByOrdinalPredicate>
															ResultInfoColumnsSetByOrdinal;
typedef ResultInfoColumnsSetByOrdinal::iterator
															ResultInfoColumnsSetByOrdinalIter;
typedef ResultInfoColumnsSetByOrdinal::const_iterator
															ResultInfoColumnsSetByOrdinalIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	class API_ODBCWRAPPER OdbcHandleBase {
	typedef typename HandleType ThisHandleType;
protected:
	OdbcHandleBase(SQLSMALLINT handle_type, SQLHANDLE parent_handle,
		const char *type_base_name, HandleGetAttrFunc get_attr_function,
		HandleSetAttrFunc set_attr_function);
	virtual ~OdbcHandleBase();

	SQLSMALLINT       handle_type_;
	SQLHANDLE         parent_handle_;
	HandleType        this_handle_;
	std::string       get_attr_function_name_;
	std::string       set_attr_function_name_;
	HandleGetAttrFunc get_attr_function_;
	HandleSetAttrFunc set_attr_function_;

public:

	HandleType GetNativeHandle() const;

	SQLRETURN GetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr,
		SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN GetAttr(SQLINTEGER Attribute, SQLINTEGER &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN GetAttr(SQLINTEGER Attribute, SQLUINTEGER &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN GetAttr(SQLINTEGER Attribute, SQLSMALLINT &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN GetAttr(SQLINTEGER Attribute, SQLUSMALLINT &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN GetAttr(SQLINTEGER Attribute, OdbcAttrFlags32 &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN GetAttr(SQLINTEGER Attribute, OdbcAttrWindowHandle &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
	SQLRETURN GetAttr(SQLINTEGER Attribute, SQLINTEGER *ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN GetAttr(SQLINTEGER Attribute, SQLUINTEGER *ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
*/
	SQLRETURN GetAttr(SQLINTEGER Attribute, char *ValuePtr,
		SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN GetAttr(SQLINTEGER Attribute, std::string &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;

	SQLRETURN SetAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr,
		SQLINTEGER StringLength,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN SetAttr(SQLINTEGER Attribute, SQLINTEGER ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN SetAttr(SQLINTEGER Attribute, SQLUINTEGER ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN SetAttr(SQLINTEGER Attribute, SQLSMALLINT ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN SetAttr(SQLINTEGER Attribute, SQLUSMALLINT ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN SetAttr(SQLINTEGER Attribute, OdbcAttrFlags32 ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN SetAttr(SQLINTEGER Attribute, OdbcAttrWindowHandle ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN SetAttr(SQLINTEGER Attribute, const char *ValuePtr,
		SQLINTEGER StringLength,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN SetAttr(SQLINTEGER Attribute, const std::string &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData) const;

private:
	void CheckAttributeFuncs() const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_ODBCWRAPPER OdbcHandleStmt : public OdbcHandleBase<SQLHSTMT> {
public:
	OdbcHandleStmt(SQLHDBC dbc_handle);
	virtual ~OdbcHandleStmt();

	ResultInfoColumnsSet &GetResultInfoColumns(ResultInfoColumnsSet &out_list,
		const char *CatalogName = NULL, const char *SchemaName = NULL,
		const char *TableName = NULL, const char *ColumnName = "",
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;
	ResultInfoColumnsSet  GetResultInfoColumns(const char *CatalogName = NULL,
		const char *SchemaName = NULL, const char *TableName = NULL,
		const char *ColumnName = "",
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;
	ResultInfoTablesSet &GetResultInfoTables(ResultInfoTablesSet &out_list,
		const char *CatalogName = NULL, const char *SchemaName = NULL,
		const char *TableName = NULL, const char *TableType = "TABLE",
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;
	ResultInfoTablesSet  GetResultInfoTables(const char *CatalogName = NULL,
		const char *SchemaName = NULL, const char *TableName = NULL,
		const char *TableType = "TABLE",
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;

	SQLRETURN BindCol(SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
		SQLPOINTER TargetValuePtr, SQLINTEGER BufferLength,
		SQLINTEGER *StrLen_or_IndPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN Cancel(OdbcThrowFlags::Flags throw_flags =
		OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN ColAttribute(SQLUSMALLINT ColumnNumber,
		SQLUSMALLINT FieldIdentifier, SQLPOINTER CharacterAttributePtr,
		SQLSMALLINT BufferLength, SQLSMALLINT *StringLengthPtr,
		SQLPOINTER NumericAttributePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN ColAttribute(SQLUSMALLINT ColumnNumber,
		SQLUSMALLINT FieldIdentifier, std::string &CharacterAttributePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN ColAttribute(SQLUSMALLINT ColumnNumber,
		SQLUSMALLINT FieldIdentifier, SQLINTEGER &NumericAttributePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN ColAttribute(SQLUSMALLINT ColumnNumber,
		SQLUSMALLINT FieldIdentifier, bool &NumericAttributePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN Columns(SQLCHAR *CatalogName, SQLSMALLINT NameLength1,
		SQLCHAR *SchemaName, SQLSMALLINT NameLength2, SQLCHAR *TableName,
		SQLSMALLINT NameLength3, SQLCHAR *ColumnName, SQLSMALLINT NameLength4,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;
	SQLRETURN Columns(const char *CatalogName = NULL,
		const char *SchemaName = NULL, const char *TableName = NULL,
		const char *ColumnName = "",
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;
	SQLRETURN DescribeCol(SQLSMALLINT ColumnNumber, SQLCHAR *ColumnName,
		SQLSMALLINT BufferLength, SQLSMALLINT *NameLengthPtr,
		SQLSMALLINT *DataTypePtr, SQLUINTEGER *ColumnSizePtr,
		SQLSMALLINT *DecimalDigitsPtr, SQLSMALLINT *NullablePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN DescribeCol(SQLSMALLINT ColumnNumber, std::string &ColumnName,
		SQLSMALLINT *DataTypePtr, SQLUINTEGER *ColumnSizePtr,
		SQLSMALLINT *DecimalDigitsPtr, SQLSMALLINT *NullablePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN ExecDirect(SQLCHAR *StatementText, SQLINTEGER TextLength,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
		const;
	SQLRETURN ExecDirect(const std::string &StatementText,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
		const;
	SQLRETURN ExtendedFetch(SQLUSMALLINT FetchOrientation,
		SQLINTEGER FetchOffset, SQLUINTEGER *RowCountPtr,
		SQLUSMALLINT *RowStatusArray,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN Fetch(
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
		const;
	SQLRETURN FetchScroll(SQLSMALLINT FetchOrientation, SQLINTEGER FetchOffset,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN GetData(SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
		SQLPOINTER TargetValuePtr, SQLINTEGER BufferLength,
		SQLINTEGER *StrLen_or_IndPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI) const;
	SQLRETURN MoreResults(OdbcThrowFlags::Flags throw_flags =
		OdbcThrowFlags::Ok_SWI_or_NoData) const;
	SQLRETURN NumResultCols(SQLSMALLINT *ColumnCountPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;
	SQLRETURN RowCount(SQLINTEGER *RowCountPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
		const;
	SQLRETURN RowCount(SQLINTEGER &RowCountPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
		const;
	SQLRETURN Tables(SQLCHAR *CatalogName, SQLSMALLINT NameLength1,
		SQLCHAR *SchemaName, SQLSMALLINT NameLength2, SQLCHAR *TableName,
		SQLSMALLINT NameLength3, SQLCHAR *TableType, SQLSMALLINT NameLength4,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;
	SQLRETURN Tables(const char *CatalogName = NULL,
		const char *SchemaName = NULL, const char *TableName = NULL,
		const char *TableType = "TABLE",
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<OdbcHandleStmt>     OdbcHandleStmtSPtr;
typedef boost::weak_ptr<OdbcHandleStmt>       OdbcHandleStmtWPtr;

typedef std::set<OdbcHandleStmtWPtr>          OdbcHandleStmtWPtrSet;
typedef OdbcHandleStmtWPtrSet::iterator       OdbcHandleStmtWPtrSetIter;
typedef OdbcHandleStmtWPtrSet::const_iterator OdbcHandleStmtWPtrSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_ODBCWRAPPER OdbcHandleDbc : public OdbcHandleBase<SQLHDBC> {
public:
	OdbcHandleDbc(SQLHENV env_handle);
	virtual ~OdbcHandleDbc();

	bool IsConnected() const;
	bool CheckIsConnected() const;
	bool IsNotConnected() const;
	bool CheckIsNotConnected() const;

	SQLRETURN Connect(SQLHWND WindowHandle, SQLCHAR *InConnectionString,
		SQLSMALLINT StringLength1, SQLCHAR *OutConnectionString,
		SQLSMALLINT BufferLength, SQLSMALLINT *StringLength2Ptr,
		SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN Connect(SQLHWND WindowHandle,
		const std::string &InConnectionString, std::string &OutConnectionString,
		SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN Connect(SQLHWND WindowHandle,
		const std::string &InConnectionString,
		SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN Connect(SQLCHAR *InConnectionString, SQLSMALLINT StringLength1,
		SQLCHAR *OutConnectionString, SQLSMALLINT BufferLength,
		SQLSMALLINT *StringLength2Ptr,
		SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN Connect(const std::string &InConnectionString,
		std::string &OutConnectionString,
		SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN Connect(const std::string &InConnectionString,
		SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN Connect(const DriverConnectParams &connect_params,
		SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN Disconnect(
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN GetFunctions(SQLUSMALLINT FunctionId, SQLUSMALLINT *SupportedPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
	SQLRETURN GetFunctions(SQLUSMALLINT FunctionId, SQLUSMALLINT &SupportedPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
	SQLRETURN GetFunctions(GetFunctions_ApiAll &SupportedPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
	SQLRETURN GetFunctions(GetFunctions_ApiAll_ODBC3 &SupportedPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
	SQLRETURN GetInfo(SQLUSMALLINT InfoType, SQLPOINTER InfoValuePtr,
		SQLSMALLINT BufferLength, SQLSMALLINT *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN GetInfo(SQLUSMALLINT InfoType, std::string &InfoValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN GetInfo(SQLUSMALLINT InfoType, SQLUSMALLINT &InfoValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN GetInfo(SQLUSMALLINT InfoType, SQLUINTEGER &InfoValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN GetInfo(const GetInfoTypeDatumRaw *InfoType,
		GetInfoTypeDatumValue &InfoValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN GetInfo(const GetInfoTypeDatumRaw *InfoType,
		GetInfoTypeDatum &InfoValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN GetInfo(SQLUSMALLINT InfoType, GetInfoTypeDatumValue &InfoValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
	SQLRETURN GetInfo(SQLUSMALLINT InfoType, GetInfoTypeDatum &InfoValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	OdbcHandleStmtSPtr CreateStatement();

	GetInfoTypeDatumList &CreateGetInfoDatumList(
		GetInfoTypeDatumList &out_datum) const;
	GetInfoTypeDatumList  CreateGetInfoDatumList() const;

private:

	bool is_connected_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<OdbcHandleDbc>     OdbcHandleDbcSPtr;
typedef boost::weak_ptr<OdbcHandleDbc>       OdbcHandleDbcWPtr;

typedef std::set<OdbcHandleDbcWPtr>          OdbcHandleDbcWPtrSet;
typedef OdbcHandleDbcWPtrSet::iterator       OdbcHandleDbcWPtrSetIter;
typedef OdbcHandleDbcWPtrSet::const_iterator OdbcHandleDbcWPtrSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_ODBCWRAPPER OdbcHandleEnv : public OdbcHandleBase<SQLHENV> {
public:
	OdbcHandleEnv();
	virtual ~OdbcHandleEnv();

	OdbcHandleDbcSPtr CreateConnection();
	OdbcHandleDbcSPtr AddConnection();

	void RemoveConnection(OdbcHandleDbcSPtr &child_sptr);
	void CleanChildList();

	AttrInfoBasicVector &GetAttrInfoBasicList(
		AttrInfoBasicVector &out_list,
		SQLUSMALLINT Direction = SQL_FETCH_FIRST) const;
	AttrInfoBasicVector  GetAttrInfoBasicList(
		SQLUSMALLINT Direction = SQL_FETCH_FIRST) const;

	AttrInfoBasicVector &GetDataSourceBasicList(
		AttrInfoBasicVector &out_list,
		SQLUSMALLINT Direction = SQL_FETCH_FIRST) const;
	AttrInfoBasicVector  GetDataSourceBasicList(
		SQLUSMALLINT Direction = SQL_FETCH_FIRST) const;

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	OdbcHandleDbcWPtrSet child_set_;
#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<OdbcHandleEnv>     OdbcHandleEnvSPtr;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcTransBlockHandle : public OdbcTransBlock {
	OdbcTransBlockHandle(OdbcHandleEnv &handle_env)
		:OdbcTransBlock(SQL_HANDLE_ENV, handle_env.GetNativeHandle())
	{
	}
	OdbcTransBlockHandle(OdbcHandleDbc &handle_dbc)
		:OdbcTransBlock(SQL_HANDLE_DBC, handle_dbc.GetNativeHandle())
	{
	}

private:
	MLB_Utility_NonCopyable_Macro(OdbcTransBlockHandle);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcTransBlockManualHandle : public OdbcTransBlockManual {
	OdbcTransBlockManualHandle(OdbcHandleDbc &handle_dbc,
		bool strict_flag = false)
		:OdbcTransBlockManual(handle_dbc.GetNativeHandle(), strict_flag)
	{
	}

private:
	MLB_Utility_NonCopyable_Macro(OdbcTransBlockManualHandle);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__OdbcHandle_hpp__HH

