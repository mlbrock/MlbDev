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
								function 'SQLGetInfo()'.

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

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
struct GetInfoTypeDescriptorRaw {
	SQLUSMALLINT  info_type_;
	const char   *info_type_name_;
	const char   *data_type_name_;
	bool          is_string_;
	bool          is_small_;
	bool          is_mask_;
	bool          is_flag_;
	bool          is_handle_;
	bool          is_handle_2way_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct GetInfoTypeDescriptorValue {
	GetInfoTypeDescriptorValue()
		:info_type_(0)
		,is_string_(false)
		,is_small_(false)
		,is_mask_(false)
		,is_flag_(false)
		,is_handle_(false)
		,is_handle_2way_(false)
		,value_string_()
		,value_USMALLINT_(0)
		,value_UINTEGER_(0)
	{
	}
	GetInfoTypeDescriptorValue(const GetInfoTypeDescriptorRaw &raw_descriptor)
		:info_type_(raw_descriptor.info_type_)
		,is_string_(raw_descriptor.is_string_)
		,is_small_(raw_descriptor.is_small_)
		,is_mask_(raw_descriptor.is_mask_)
		,is_flag_(raw_descriptor.is_flag_)
		,is_handle_(raw_descriptor.is_handle_)
		,is_handle_2way_(raw_descriptor.is_handle_2way_)
		,value_string_()
		,value_USMALLINT_(0)
		,value_UINTEGER_(0)
	{
	}

	SQLUSMALLINT info_type_;
	bool         is_string_;
	bool         is_small_;
	bool         is_mask_;
	bool         is_flag_;
	bool         is_handle_;
	bool         is_handle_2way_;
	std::string  value_string_;
	SQLUSMALLINT value_USMALLINT_;
	SQLUINTEGER  value_UINTEGER_;
};
std::ostream & operator << (std::ostream &o_str,
	const GetInfoTypeDescriptorValue &datum)
{
	if (datum.is_string_)
		o_str << datum.value_string_;
	else if (datum.is_small_)
		o_str << std::setw(
			MLB::Utility::IntegralValueMaxLengthDecUnsigned<SQLUSMALLINT>()) <<
			datum.value_USMALLINT_;
	else if (datum.is_mask_)
		o_str << MLB::Utility::ValueToStringBin(datum.value_UINTEGER_);
	else if (datum.is_flag_)
		o_str <<
			MLB::Utility::AnyToString((datum.value_UINTEGER_) ? true : false);
	else
		o_str << std::setw(
			MLB::Utility::IntegralValueMaxLengthDecUnsigned<SQLUINTEGER>()) <<
			datum.value_UINTEGER_;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct GetInfoTypeDescriptor {
	GetInfoTypeDescriptor(const GetInfoTypeDescriptorRaw &raw_descriptor)
		:info_type_(raw_descriptor.info_type_)
		,info_type_name_(raw_descriptor.info_type_name_)
		,data_type_name_(raw_descriptor.data_type_name_)
		,is_string_(raw_descriptor.is_string_)
		,is_small_(raw_descriptor.is_small_)
		,is_mask_(raw_descriptor.is_mask_)
		,is_flag_(raw_descriptor.is_flag_)
		,is_handle_(raw_descriptor.is_handle_)
		,is_handle_2way_(raw_descriptor.is_handle_2way_)
		,value_string_()
		,value_USMALLINT_(0)
		,value_UINTEGER_(0)
	{
	}

	SQLUSMALLINT info_type_;
	std::string  info_type_name_;
	std::string  data_type_name_;
	bool         is_string_;
	bool         is_small_;
	bool         is_mask_;
	bool         is_flag_;
	bool         is_handle_;
	bool         is_handle_2way_;
	std::string  value_string_;
	SQLUSMALLINT value_USMALLINT_;
	SQLUINTEGER  value_UINTEGER_;
};
std::ostream & operator << (std::ostream &o_str,
	const GetInfoTypeDescriptor &datum)
{
	if (datum.is_string_)
		o_str << datum.value_string_;
	else if (datum.is_small_)
		o_str << std::setw(
			MLB::Utility::IntegralValueMaxLengthDecUnsigned<SQLUSMALLINT>()) <<
			datum.value_USMALLINT_;
	else if (datum.is_mask_)
		o_str << MLB::Utility::ValueToStringBin(datum.value_UINTEGER_);
	else if (datum.is_flag_)
		o_str <<
			MLB::Utility::AnyToString((datum.value_UINTEGER_) ? true : false);
	else
		o_str << std::setw(
			MLB::Utility::IntegralValueMaxLengthDecUnsigned<SQLUINTEGER>()) <<
			datum.value_UINTEGER_;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#define ODBCWRAPPER_GETINFO_TYPE(info_type, info_name, data_type, is_str,	\
	is_small, is_mask, is_flag, is_handle, is_2way_)								\
	{	info_type, info_name, #data_type, is_str, is_small, is_mask, is_flag,\
		is_handle, is_2way_	}

#define ODBCWRAPPER_GETINFO_TYPE_String(info_type)									\
	ODBCWRAPPER_GETINFO_TYPE(info_type, #info_type, std::string,  true,  false, false, false, false, false)
#define ODBCWRAPPER_GETINFO_TYPE_USmallInt(info_type)								\
	ODBCWRAPPER_GETINFO_TYPE(info_type, #info_type, SQLUSMALLINT, false, true,  false, false, false, false)
#define ODBCWRAPPER_GETINFO_TYPE_Bitmask(info_type)								\
	ODBCWRAPPER_GETINFO_TYPE(info_type, #info_type, SQLUINTEGER,  false, false, true, false,  false, false)
#define ODBCWRAPPER_GETINFO_TYPE_Flag(info_type)									\
	ODBCWRAPPER_GETINFO_TYPE(info_type, #info_type, SQLUINTEGER,  false, false, false, true,  false, false)
#define ODBCWRAPPER_GETINFO_TYPE_Value(info_type)									\
	ODBCWRAPPER_GETINFO_TYPE(info_type, #info_type, SQLUINTEGER,  false, false, false, false, false, false)
#define ODBCWRAPPER_GETINFO_TYPE_Handle(info_type)									\
	ODBCWRAPPER_GETINFO_TYPE(info_type, #info_type, SQLUINTEGER,  false, false, false, false,  true, false)
#define ODBCWRAPPER_GETINFO_TYPE_Handle2Way(info_type)							\
	ODBCWRAPPER_GETINFO_TYPE(info_type, #info_type, SQLUINTEGER,  false, false, false, false,  true,  true)
// ////////////////////////////////////////////////////////////////////////////

namespace {
// ////////////////////////////////////////////////////////////////////////////
const GetInfoTypeDescriptorRaw GetInfoTypeList[] = {
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_ACCESSIBLE_PROCEDURES),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_ACCESSIBLE_TABLES),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_ACTIVE_ENVIRONMENTS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_ACTIVE_ENVIRONMENTS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_ALTER_DOMAIN),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_ALTER_TABLE),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_ASYNC_MODE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_BATCH_ROW_COUNT),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_BATCH_SUPPORT),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_BOOKMARK_PERSISTENCE),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_CATALOG_LOCATION),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_CATALOG_NAME),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_CATALOG_NAME_SEPARATOR),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_CATALOG_TERM),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CATALOG_USAGE),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_COLLATION_SEQ),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_COLUMN_ALIAS),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_CONCAT_NULL_BEHAVIOR),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_BIGINT),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_BINARY),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_BIT),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_CHAR),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_GUID),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_DATE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_DECIMAL),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_DOUBLE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_FLOAT),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_INTEGER),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_INTERVAL_YEAR_MONTH),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_INTERVAL_DAY_TIME),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_LONGVARBINARY),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_LONGVARCHAR),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_NUMERIC),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_REAL),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_SMALLINT),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_TIME),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_TIMESTAMP),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_TINYINT),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_VARBINARY),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_VARCHAR),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CONVERT_FUNCTIONS),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_CORRELATION_NAME),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CREATE_ASSERTION),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CREATE_CHARACTER_SET),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CREATE_COLLATION),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CREATE_DOMAIN),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CREATE_SCHEMA),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CREATE_TABLE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CREATE_TRANSLATION),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_CREATE_VIEW),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_CURSOR_COMMIT_BEHAVIOR),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_CURSOR_ROLLBACK_BEHAVIOR),
#ifdef SQL_CURSOR_ROLLBACK_SQL_CURSOR_SENSITIVITY
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_CURSOR_ROLLBACK_SQL_CURSOR_SENSITIVITY),
#endif // #ifdef SQL_CURSOR_ROLLBACK_SQL_CURSOR_SENSITIVITY
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DATA_SOURCE_NAME),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DATA_SOURCE_READ_ONLY),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DATABASE_NAME),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DATETIME_LITERALS),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DBMS_NAME),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DBMS_VER),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_DDL_INDEX),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_DEFAULT_TXN_ISOLATION),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DESCRIBE_PARAMETER),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DM_VER),
	ODBCWRAPPER_GETINFO_TYPE_Handle(SQL_DRIVER_HDBC),
	ODBCWRAPPER_GETINFO_TYPE_Handle(SQL_DRIVER_HENV),
	ODBCWRAPPER_GETINFO_TYPE_Handle2Way(SQL_DRIVER_HDESC),
	ODBCWRAPPER_GETINFO_TYPE_Handle(SQL_DRIVER_HLIB),
	ODBCWRAPPER_GETINFO_TYPE_Handle2Way(SQL_DRIVER_HSTMT),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DRIVER_NAME),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DRIVER_ODBC_VER),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_DRIVER_VER),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DROP_ASSERTION),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DROP_CHARACTER_SET),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DROP_COLLATION),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DROP_DOMAIN),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DROP_SCHEMA),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DROP_TABLE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DROP_TRANSLATION),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DROP_VIEW),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DYNAMIC_CURSOR_ATTRIBUTES1),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_DYNAMIC_CURSOR_ATTRIBUTES2),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_EXPRESSIONS_IN_ORDERBY),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_GETDATA_EXTENSIONS),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_GROUP_BY),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_IDENTIFIER_CASE),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_IDENTIFIER_QUOTE_CHAR),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_INDEX_KEYWORDS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_INFO_SCHEMA_VIEWS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_INSERT_STATEMENT),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_INTEGRITY),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_KEYSET_CURSOR_ATTRIBUTES1),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_KEYSET_CURSOR_ATTRIBUTES2),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_KEYWORDS),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_LIKE_ESCAPE_CLAUSE),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_MAX_ASYNC_CONCURRENT_STATEMENTS),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_MAX_BINARY_LITERAL_LEN),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_CATALOG_NAME_LEN),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_MAX_CHAR_LITERAL_LEN),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_COLUMN_NAME_LEN),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_COLUMNS_IN_GROUP_BY),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_COLUMNS_IN_INDEX),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_COLUMNS_IN_ORDER_BY),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_COLUMNS_IN_SELECT),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_COLUMNS_IN_TABLE),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_CONCURRENT_ACTIVITIES),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_CURSOR_NAME_LEN),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_DRIVER_CONNECTIONS),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_IDENTIFIER_LEN),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_MAX_INDEX_SIZE),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_PROCEDURE_NAME_LEN),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_MAX_ROW_SIZE),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_MAX_ROW_SIZE_INCLUDES_LONG),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_SCHEMA_NAME_LEN),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_MAX_STATEMENT_LEN),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_TABLE_NAME_LEN),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_TABLES_IN_SELECT),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_MAX_USER_NAME_LEN),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_MULT_RESULT_SETS),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_MULTIPLE_ACTIVE_TXN),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_NEED_LONG_DATA_LEN),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_NON_NULLABLE_COLUMNS),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_NULL_COLLATION),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_NUMERIC_FUNCTIONS),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_ODBC_INTERFACE_CONFORMANCE),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_ODBC_VER),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_OJ_CAPABILITIES),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_ORDER_BY_COLUMNS_IN_SELECT),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_PARAM_ARRAY_ROW_COUNTS),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_PARAM_ARRAY_SELECTS),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_PROCEDURE_TERM),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_PROCEDURES),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_POS_OPERATIONS),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_QUOTED_IDENTIFIER_CASE),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_ROW_UPDATES),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_SCHEMA_TERM),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SCHEMA_USAGE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SCROLL_OPTIONS),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_SEARCH_PATTERN_ESCAPE),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_SERVER_NAME),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_SPECIAL_CHARACTERS),
	ODBCWRAPPER_GETINFO_TYPE_Value(SQL_SQL_CONFORMANCE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_DATETIME_FUNCTIONS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_FOREIGN_KEY_DELETE_RULE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_FOREIGN_KEY_UPDATE_RULE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_GRANT),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_NUMERIC_VALUE_FUNCTIONS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_PREDICATES),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_RELATIONAL_JOIN_OPERATORS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_REVOKE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_ROW_VALUE_CONSTRUCTOR),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_STRING_FUNCTIONS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SQL92_VALUE_EXPRESSIONS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_STANDARD_CLI_CONFORMANCE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_STATIC_CURSOR_ATTRIBUTES1),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_STATIC_CURSOR_ATTRIBUTES2),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_STRING_FUNCTIONS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SUBQUERIES),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_SYSTEM_FUNCTIONS),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_TABLE_TERM),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_TIMEDATE_ADD_INTERVALS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_TIMEDATE_DIFF_INTERVALS),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_TIMEDATE_FUNCTIONS),
	ODBCWRAPPER_GETINFO_TYPE_USmallInt(SQL_TXN_CAPABLE),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_TXN_ISOLATION_OPTION),
	ODBCWRAPPER_GETINFO_TYPE_Bitmask(SQL_UNION),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_USER_NAME),
	ODBCWRAPPER_GETINFO_TYPE_String(SQL_XOPEN_CLI_YEAR)
};
const unsigned int          GetInfoTypeCount  =
	sizeof(GetInfoTypeList) / sizeof(GetInfoTypeList[0]);
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const GetInfoTypeDescriptorRaw *GetDescriptorPtr(SQLUSMALLINT info_type)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < GetInfoTypeCount; ++count_1) {
		if (info_type == GetInfoTypeList[count_1].info_type_)
			return(GetInfoTypeList + count_1);
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const GetInfoTypeDescriptorRaw *GetDescriptorPtrChecked(SQLUSMALLINT info_type)
{
	const GetInfoTypeDescriptorRaw *found_ptr = GetDescriptorPtr(info_type);

	if (found_ptr == NULL)
		throw OdbcException("Unable to locate the '::SQLGetInfo()' information "
			"type descriptor entry for " + MLB::Utility::AnyToString(info_type) +
			".");

	return(found_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	const GetInfoTypeDescriptorRaw *InfoType,
	GetInfoTypeDescriptorValue &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	SQLUSMALLINT InfoType, GetInfoTypeDescriptorValue &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetInfo(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType,
	SQLPOINTER InfoValuePtr, SQLSMALLINT BufferLength,
	SQLSMALLINT *StringLengthPtr, OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_DBC, ConnectionHandle, ::SQLGetInfo,
		(ConnectionHandle, InfoType, InfoValuePtr, BufferLength,
		StringLengthPtr)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetInfo(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType,
	std::string &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	SQLRETURN                                   return_code;
	MLB::Utility::IncrementalBuffer<char, 4096> buffer;

	throw_flags = MLB::Utility::EnumFlagAnd(throw_flags,
		static_cast<OdbcThrowFlags::Flags>(~(OdbcThrowFlags::SuccessWithInfo)));

	for ( ; ; ) {
		SQLSMALLINT actual_len;
		return_code = GetInfo(ConnectionHandle, InfoType,
			reinterpret_cast<SQLPOINTER>(buffer.GetPtr()),
			buffer.GetAllocationCountAsType<SQLSMALLINT>(), &actual_len,
			throw_flags);
		if (return_code == SQL_SUCCESS) {
			if (actual_len > 0) {
				buffer.SetCount(actual_len, true);
				std::string(reinterpret_cast<const char *>(buffer.GetPtr()),
					buffer.GetCount()).swap(InfoValuePtr);
			}
			else
				InfoValuePtr.clear();
			break;
		}
		else if (return_code == SQL_SUCCESS_WITH_INFO) {
			if (!buffer.SetCount(actual_len, false))
				throw OdbcExceptionDiag(return_code, SQL_HANDLE_DBC,
					ConnectionHandle, "Call to '::SQLGetInfo()' returned "
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
SQLRETURN GetInfo(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType,
	SQLUSMALLINT &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	SQLSMALLINT StringLengthPtr;

	return(GetInfo(ConnectionHandle, InfoType, &InfoValuePtr,
		sizeof(InfoValuePtr), &StringLengthPtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetInfo(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType,
	SQLUINTEGER &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	SQLSMALLINT StringLengthPtr;

	return(GetInfo(ConnectionHandle, InfoType, &InfoValuePtr,
		sizeof(InfoValuePtr), &StringLengthPtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	const GetInfoTypeDescriptorRaw *InfoType,
	GetInfoTypeDescriptorValue &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	SQLRETURN                  return_code;
	GetInfoTypeDescriptorValue tmp_value(*InfoType);

	if (tmp_value.is_string_)
		return_code = GetInfo(ConnectionHandle, InfoType->info_type_,
			tmp_value.value_string_, throw_flags);
	else if (tmp_value.is_small_)
		return_code = GetInfo(ConnectionHandle, InfoType->info_type_,
			tmp_value.value_USMALLINT_, throw_flags);
	else
		return_code = GetInfo(ConnectionHandle, InfoType->info_type_,
			tmp_value.value_UINTEGER_, throw_flags);

	InfoValuePtr = tmp_value;

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetInfo(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType,
	GetInfoTypeDescriptorValue &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetInfo(ConnectionHandle, GetDescriptorPtrChecked(InfoType),
		InfoValuePtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#ifdef TEST_MAIN

#include <OdbcWrapper/OdbcHandle.hpp>

using namespace MLB::OdbcWrapper;

int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
		OdbcHandleDbcSPtr odbc_dbc_sptr(odbc_env.AddConnection());
		odbc_dbc_sptr->Connect(
//			"Database=AdventureWorksDW;Driver={SQL Native Client};Uid=sa;Server=(local);Pwd=S_a_0");
"Database=GDSP_LON_TRD_TST;Driver={SQL Native Client};Uid=sa;Server=(local);Pwd=S_a_0");
		unsigned int count_1;
		for (count_1 = 0; count_1 < GetInfoTypeCount; ++count_1) {
			if (!GetInfoTypeList[count_1].is_handle_2way_) {
				GetInfoTypeDescriptorValue info_value;
				GetInfo(odbc_dbc_sptr->GetNativeHandle(), GetInfoTypeList + count_1,
					info_value);
				std::cout << std::setw(5) << GetInfoTypeList[count_1].info_type_
					<< " " << std::left << std::setw(35) <<
					GetInfoTypeList[count_1].info_type_name_ << ": " <<
					std::right << info_value << std::endl;
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR IN REGRESSION TEST FOR "
			"'MLB::OdbcWrapper::GetInfo()': " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}

#endif // #ifdef TEST_MAIN

