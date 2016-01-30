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

#include <Utility/IncrementalBuffer.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
struct GetInfoTypeDatumRaw {
	SQLUSMALLINT  info_type_;
	const char   *info_type_name_;
	const char   *data_type_name_;
	bool          is_string_;
	bool          is_small_;
	bool          is_mask_;
	bool          is_flag_;
	bool          is_handle_;
	bool          is_handle_2way_;

	static const GetInfoTypeDatumRaw *GetInfoTypeDatumPtr(
		SQLUSMALLINT info_type);
	static const GetInfoTypeDatumRaw *GetInfoTypeDatumPtrChecked(
		SQLUSMALLINT info_type);
	static unsigned int               GetInfoTypeRawData(
		const GetInfoTypeDatumRaw *&list_ptr);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct GetInfoTypeDatumValue {
	GetInfoTypeDatumValue();
	GetInfoTypeDatumValue(const GetInfoTypeDatumRaw &raw_descriptor);
	virtual ~GetInfoTypeDatumValue();

	bool operator < (const GetInfoTypeDatumValue &other) const;

	GetInfoTypeDatumValue &swap(GetInfoTypeDatumValue &other);

	std::string &ToString(std::string &out_string) const;
	std::string  ToString() const;
	std::string &ToStringPadded(std::string &out_string) const;
	std::string  ToStringPadded() const;

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
	const GetInfoTypeDatumValue &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
GetInfoTypeDatumValue::GetInfoTypeDatumValue()
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

GetInfoTypeDatumValue::GetInfoTypeDatumValue(
	const GetInfoTypeDatumRaw &raw_descriptor)
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

GetInfoTypeDatumValue::~GetInfoTypeDatumValue()
{
}

bool GetInfoTypeDatumValue::operator < (
	const GetInfoTypeDatumValue &other) const
{
	return(info_type_ < other.info_type_);
}

GetInfoTypeDatumValue &GetInfoTypeDatumValue::swap(
	GetInfoTypeDatumValue &other)
{
	std::swap(info_type_, other.info_type_);
	std::swap(is_string_, other.is_string_);
	std::swap(is_small_, other.is_small_);
	std::swap(is_mask_, other.is_mask_);
	std::swap(is_flag_, other.is_flag_);
	std::swap(is_handle_, other.is_handle_);
	std::swap(is_handle_2way_, other.is_handle_2way_);
	value_string_.swap(other.value_string_);
	std::swap(value_USMALLINT_, other.value_USMALLINT_);
	std::swap(value_UINTEGER_, other.value_UINTEGER_);

	return(*this);
}

std::string &GetInfoTypeDatumValue::ToString(
	std::string &out_string) const
{
	if (is_string_)
		out_string.assign(value_string_);
	else {
		std::ostringstream o_str;
		if (is_small_)
			o_str << value_USMALLINT_;
		else if (is_mask_)
			o_str << MLB::Utility::ValueToStringBin(value_UINTEGER_);
		else if (is_flag_)
			o_str <<
				MLB::Utility::AnyToString((value_UINTEGER_) ? true : false);
		else if (is_handle_ || is_handle_2way_)
			o_str << MLB::Utility::ValueToStringHex(value_UINTEGER_);
		else
			o_str << value_UINTEGER_;
		out_string = o_str.str();
	}

	return(out_string);
}

std::string GetInfoTypeDatumValue::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}

std::string &GetInfoTypeDatumValue::ToStringPadded(
	std::string &out_string) const
{
	if (is_string_)
		out_string.assign(value_string_);
	else {
		std::ostringstream o_str;
		if (is_small_)
			o_str << std::setw(
				MLB::Utility::IntegralValueMaxLengthDecUnsigned<SQLUSMALLINT>()) <<
				value_USMALLINT_;
		else if (is_mask_)
			o_str << MLB::Utility::ValueToStringBin(value_UINTEGER_);
		else if (is_flag_)
			o_str <<
				MLB::Utility::AnyToString((value_UINTEGER_) ? true : false);
		else if (is_handle_ || is_handle_2way_)
			o_str << MLB::Utility::ValueToStringHex(value_UINTEGER_);
		else
			o_str << std::setw(
				MLB::Utility::IntegralValueMaxLengthDecUnsigned<SQLUINTEGER>()) <<
				value_UINTEGER_;
		out_string = o_str.str();
	}

	return(out_string);
}

std::string GetInfoTypeDatumValue::ToStringPadded() const
{
	std::string out_string;

	return(ToStringPadded(out_string));
}

std::ostream & operator << (std::ostream &o_str,
	const GetInfoTypeDatumValue &datum)
{
	std::string out_string;

	o_str
		<< datum.ToString(out_string);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct GetInfoTypeDatum : public GetInfoTypeDatumValue {
	         GetInfoTypeDatum();
	explicit GetInfoTypeDatum(
		const GetInfoTypeDatumRaw &descriptor_raw,
		const GetInfoTypeDatumValue &descriptor_val =
		GetInfoTypeDatumValue());
	//	Used for find()...
	explicit GetInfoTypeDatum(SQLUSMALLINT info_type);
	virtual ~GetInfoTypeDatum();

	bool operator < (const GetInfoTypeDatum &other) const;

	GetInfoTypeDatum &swap(GetInfoTypeDatum &other);

	std::string &ToString(std::string &out_string) const;
	std::string  ToString() const;
	std::string &ToStringPadded(std::string &out_string,
		std::streamsize padding_width = 35) const;
	std::string  ToStringPadded(std::streamsize padding_width = 35) const;

	std::string  info_type_name_;
	std::string  data_type_name_;
};
std::ostream & operator << (std::ostream &o_str,
	const GetInfoTypeDatum &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
GetInfoTypeDatum::GetInfoTypeDatum()
	:GetInfoTypeDatumValue()
{
}

GetInfoTypeDatum::GetInfoTypeDatum(
	const GetInfoTypeDatumRaw &descriptor_raw,
	const GetInfoTypeDatumValue &descriptor_val)
	:GetInfoTypeDatumValue(descriptor_val)
	,info_type_name_(descriptor_raw.info_type_name_)
	,data_type_name_(descriptor_raw.data_type_name_)
{
}

GetInfoTypeDatum::GetInfoTypeDatum(SQLUSMALLINT info_type)
	:GetInfoTypeDatumValue()
{
	info_type_ = info_type;
}

GetInfoTypeDatum::~GetInfoTypeDatum()
{
}

bool GetInfoTypeDatum::operator < (
	const GetInfoTypeDatum &other) const
{
	return(info_type_ < other.info_type_);
}

GetInfoTypeDatum &GetInfoTypeDatum::swap(GetInfoTypeDatum &other)
{
	GetInfoTypeDatumValue::swap(other);

	info_type_name_.swap(other.info_type_name_);
	data_type_name_.swap(other.data_type_name_);

	return(*this);
}

std::string &GetInfoTypeDatum::ToString(std::string &out_string) const
{
	std::string(info_type_name_ + "=" +
		GetInfoTypeDatumValue::ToString()).swap(out_string);

	return(out_string);
}

std::string GetInfoTypeDatum::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}

std::string &GetInfoTypeDatum::ToStringPadded(
	std::string &out_string, std::streamsize padding_width) const
{
	std::ostringstream o_str;
	std::string        tmp_string;

	o_str
		<< std::left << std::setw(padding_width) << info_type_name_
		<<	std::right << ": "
		<< GetInfoTypeDatumValue::ToStringPadded(tmp_string);

	return(out_string.assign(o_str.str()));
}

std::string GetInfoTypeDatum::ToStringPadded(
	std::streamsize padding_width) const
{
	std::string out_string;

	return(ToStringPadded(out_string, padding_width));
}

std::ostream & operator << (std::ostream &o_str,
	const GetInfoTypeDatum &datum)
{
	std::string out_string;

	o_str
		<< datum.ToString(out_string);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	const GetInfoTypeDatumRaw *InfoType,
	GetInfoTypeDatumValue &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	const GetInfoTypeDatumRaw *InfoType,
	GetInfoTypeDatum &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	SQLUSMALLINT InfoType, GetInfoTypeDatumValue &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	SQLUSMALLINT InfoType, GetInfoTypeDatum &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<GetInfoTypeDatum> GetInfoTypeDatumSet;
typedef GetInfoTypeDatumSet::const_iterator           GetInfoTypeDatumSetIterC;
typedef GetInfoTypeDatumSet::iterator                 GetInfoTypeDatumSetIter;

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class GetInfoTypeDatumList {
protected:
	GetInfoTypeDatumSetIter FindInternal(SQLUSMALLINT info_type)
	{
		return(descriptor_set_.find(GetInfoTypeDatum(info_type)));
	}

	GetInfoTypeDatumSetIterC FindInternal(SQLUSMALLINT info_type) const
	{
		return(descriptor_set_.find(GetInfoTypeDatum(info_type)));
	}

	void ThrowNotLocated(SQLUSMALLINT info_type) const
	{
		throw OdbcException("Unable to locate the specified '::SQLGetInfo()' "
			"information type (" + MLB::Utility::AnyToString(info_type) + ").");
	}

public:
	GetInfoTypeDatumList()
		:descriptor_set_()
	{
	}
	virtual ~GetInfoTypeDatumList()
	{
	}

	GetInfoTypeDatumList &swap(GetInfoTypeDatumList &other)
	{
		descriptor_set_.swap(other.descriptor_set_);

		return(*this);
	}

	bool IsInSet(SQLUSMALLINT info_type) const
	{
		return(FindInternal(info_type) != descriptor_set_.end());
	}
	bool CheckIsInSet(SQLUSMALLINT info_type) const
	{
		if (!IsInSet(info_type))
			ThrowNotLocated(info_type);
		return(true);
	}
	
	GetInfoTypeDatum &Get(SQLUSMALLINT info_type,
		GetInfoTypeDatum &out_datum) const
	{
		GetInfoTypeDatumSetIterC iter(FindInternal(info_type));

		if (iter == descriptor_set_.end())
			ThrowNotLocated(info_type);

		return(GetInfoTypeDatum(*iter).swap(out_datum));
	}
	GetInfoTypeDatum  Get(SQLUSMALLINT info_type) const
	{
		GetInfoTypeDatum out_datum;

		return(Get(info_type, out_datum));
	}

	void InsertOrUpdate(const GetInfoTypeDatum &new_item)
	{
		descriptor_set_.insert(new_item);
	}

	void GetInfoData(SQLHDBC ConnectionHandle)
	{
		Create(ConnectionHandle, *this);
	}

	GetInfoTypeDatumSet descriptor_set_;

	static GetInfoTypeDatumList &Create(SQLHDBC ConnectionHandle,
		GetInfoTypeDatumList &out_datum)
	{
		GetInfoTypeDatumList tmp_datum;

		unsigned int               count_1;
		unsigned int               raw_count;
		const GetInfoTypeDatumRaw *raw_list;

		raw_count = GetInfoTypeDatumRaw::GetInfoTypeRawData(raw_list);

		for (count_1 = 0; count_1 < raw_count; ++count_1) {
			if (!raw_list[count_1].is_handle_2way_) {
				GetInfoTypeDatum info_value;
				GetInfo(ConnectionHandle,
					raw_list + count_1, info_value);
				tmp_datum.InsertOrUpdate(info_value);
			}
		}

		out_datum.swap(tmp_datum);
	}

	static GetInfoTypeDatumList Create(SQLHDBC ConnectionHandle)
	{
		GetInfoTypeDatumList out_datum;

		return(Create(ConnectionHandle, out_datum));
	}
};
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
const GetInfoTypeDatumRaw GetInfoTypeRawList[] = {
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
const unsigned int          GetInfoTypeRawCount  =
	sizeof(GetInfoTypeRawList) / sizeof(GetInfoTypeRawList[0]);
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const GetInfoTypeDatumRaw *GetInfoTypeDatumRaw::GetInfoTypeDatumPtr(
	SQLUSMALLINT info_type)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < GetInfoTypeRawCount; ++count_1) {
		if (info_type == GetInfoTypeRawList[count_1].info_type_)
			return(GetInfoTypeRawList + count_1);
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const GetInfoTypeDatumRaw *GetInfoTypeDatumRaw::GetInfoTypeDatumPtrChecked(
	SQLUSMALLINT info_type)
{
	const GetInfoTypeDatumRaw *found_ptr = GetInfoTypeDatumPtr(info_type);

	if (found_ptr == NULL)
		throw OdbcException("Unable to locate the '::SQLGetInfo()' information "
			"type descriptor entry for " + MLB::Utility::AnyToString(info_type) +
			".");

	return(found_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int GetInfoTypeDatumRaw::GetInfoTypeRawData(
	const GetInfoTypeDatumRaw *&list_ptr)
{
	list_ptr = GetInfoTypeRawList;

	return(GetInfoTypeRawCount);
}
// ////////////////////////////////////////////////////////////////////////////

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
	const GetInfoTypeDatumRaw *InfoType,
	GetInfoTypeDatumValue &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	SQLRETURN                  return_code;
	GetInfoTypeDatumValue tmp_value(*InfoType);

	if (tmp_value.is_string_)
		return_code = GetInfo(ConnectionHandle, tmp_value.info_type_,
			tmp_value.value_string_, throw_flags);
	else if (tmp_value.is_small_)
		return_code = GetInfo(ConnectionHandle, tmp_value.info_type_,
			tmp_value.value_USMALLINT_, throw_flags);
	else
		return_code = GetInfo(ConnectionHandle, tmp_value.info_type_,
			tmp_value.value_UINTEGER_, throw_flags);

	InfoValuePtr.swap(tmp_value);

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	const GetInfoTypeDatumRaw *InfoType,
	GetInfoTypeDatum &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	SQLRETURN                  return_code;
	GetInfoTypeDatumValue tmp_value;

	return_code = GetInfo(ConnectionHandle, InfoType, tmp_value, throw_flags);

	GetInfoTypeDatum(*InfoType, tmp_value).swap(InfoValuePtr);

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetInfo(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType,
	GetInfoTypeDatumValue &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetInfo(ConnectionHandle,
		GetInfoTypeDatumRaw::GetInfoTypeDatumPtrChecked(InfoType), InfoValuePtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetInfo(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType,
	GetInfoTypeDatum &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(GetInfo(ConnectionHandle,
		GetInfoTypeDatumRaw::GetInfoTypeDatumPtrChecked(InfoType), InfoValuePtr,
		throw_flags));
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
		unsigned int               count_1;
		unsigned int               raw_count;
		const GetInfoTypeDatumRaw *raw_list;
		raw_count = GetInfoTypeDatumRaw::GetInfoTypeRawData(raw_list);
		for (count_1 = 0; count_1 < raw_count; ++count_1) {
			if (!GetInfoTypeRawList[count_1].is_handle_2way_) {
				GetInfoTypeDatum info_value;
				GetInfo(odbc_dbc_sptr->GetNativeHandle(),
					raw_list + count_1, info_value);
				std::cout << std::setw(5) << info_value.info_type_ << " " <<
					info_value.ToStringPadded() << std::endl;
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

