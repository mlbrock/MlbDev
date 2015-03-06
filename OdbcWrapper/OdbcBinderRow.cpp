// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the OdbcHandleDbc class.

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

//	CODE NOTE: boost/shared_array.hpp needed only for OdbcBinderRow
#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_array.hpp>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {






















// ////////////////////////////////////////////////////////////////////////////
struct OdbcDataTypeInfoRaw {
	const char  *data_type_name_;
	SQLSMALLINT  data_type_;
	const char  *c_type_name_;
	SQLSMALLINT  c_type_;
	std::size_t  c_type_size_;
	std::size_t  c_type_alignment_;
	bool         is_string_;
	bool         is_struct_;
	bool         is_blob_;

	static const OdbcDataTypeInfoRaw *GetInfoForType(SQLSMALLINT data_type);
	static const OdbcDataTypeInfoRaw *GetInfoForTypeChecked(
		SQLSMALLINT data_type);
	static const OdbcDataTypeInfoRaw *GetInfoForType(SQLINTEGER data_type);
	static const OdbcDataTypeInfoRaw *GetInfoForTypeChecked(
		SQLINTEGER data_type);

	static void  EmitTabularList();
};
// ////////////////////////////////////////////////////////////////////////////

namespace {
// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> struct OdbcDataTypeAlign {
	char     char_;
	DataType data_;
};
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
#define ODBC_DATA_TYPE_INFO_RAW(d_type, c_type, is_string, is_struct, is_blob)\
	{	#d_type, d_type, #c_type, ((is_string) ? SQL_C_CHAR : d_type),				\
		sizeof(c_type), offsetof(OdbcDataTypeAlign<c_type>, data_), is_string,	\
		is_struct, is_blob	}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const OdbcDataTypeInfoRaw OdbcDataTypeInfoRawList[] = {
	ODBC_DATA_TYPE_INFO_RAW(SQL_CHAR,				char,							true ,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_NUMERIC,			SQL_NUMERIC_STRUCT,		false,	true ,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_DECIMAL,			SQL_NUMERIC_STRUCT,		false,	true ,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_INTEGER,			SQLINTEGER,					false,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_SMALLINT,			SQLSMALLINT,				false,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_FLOAT,				double,						false,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_REAL,				float,						false,	false,	false),
//	ODBC_DATA_TYPE_INFO_RAW(SQL_DOUBLE,				XXXX,							false,	false,	false),
//	ODBC_DATA_TYPE_INFO_RAW(SQL_DATETIME,			XXXX,							false,	false,	false),
//	ODBC_DATA_TYPE_INFO_RAW(SQL_INTERVAL,			XXXX,							false,	false,	false),
//	ODBC_DATA_TYPE_INFO_RAW(SQL_TIME,				XXXX,							false,	false,	false),
//	ODBC_DATA_TYPE_INFO_RAW(SQL_TIMESTAMP,			XXXX,							false,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_VARCHAR,			char,							true ,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_TYPE_DATE,			SQL_DATE_STRUCT,			false,	true ,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_TYPE_TIME,			SQL_TIME_STRUCT,			false,	true ,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_TYPE_TIMESTAMP,	SQL_TIMESTAMP_STRUCT,	false,	true ,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_LONGVARCHAR,		char,							true ,	false,	true ),
	ODBC_DATA_TYPE_INFO_RAW(SQL_BINARY,				char,							true ,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_VARBINARY,			char,							true ,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_LONGVARBINARY,	char,							true ,	false,	true ),
	ODBC_DATA_TYPE_INFO_RAW(SQL_BIGINT,				long long,					false,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_TINYINT,			unsigned char,				false,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_BIT,					char,							false,	false,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_GUID,				UUID,							false,	true ,	false),
	ODBC_DATA_TYPE_INFO_RAW(SQL_WCHAR,				char[2],						true ,	false,	false),	//	Unicode support: sqlucode.h
	ODBC_DATA_TYPE_INFO_RAW(SQL_WVARCHAR,			char[2],						true ,	false,	false),	//	Unicode support: sqlucode.h
	ODBC_DATA_TYPE_INFO_RAW(SQL_WLONGVARCHAR,		char[2],						true ,	false,	true )	//	Unicode support: sqlucode.h
};
//	----------------------------------------------------------------------------
const unsigned int         OdbcDataTypeInfoRawCount  =
	sizeof(OdbcDataTypeInfoRawList) / sizeof(OdbcDataTypeInfoRawList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const OdbcDataTypeInfoRaw *OdbcDataTypeInfoRaw::GetInfoForType(
	SQLSMALLINT data_type)
{
	unsigned int               data_idx = OdbcDataTypeInfoRawCount;
	const OdbcDataTypeInfoRaw *data_ptr = OdbcDataTypeInfoRawList;

	for ( ; data_idx; --data_idx, ++data_ptr) {
		if (data_type == data_ptr->data_type_)
			return(data_ptr);
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const OdbcDataTypeInfoRaw *OdbcDataTypeInfoRaw::GetInfoForTypeChecked(
	SQLSMALLINT data_type)
{
	const OdbcDataTypeInfoRaw *type_ptr = GetInfoForType(data_type);

	if (type_ptr == NULL)
		MLB::Utility::ThrowRuntimeError("Attempt to locate Odbc data type "
			"information for type id " + MLB::Utility::AnyToString(data_type) +
			"failed.");

	return(type_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OdbcDataTypeInfoRaw::EmitTabularList()
{

	unsigned int               data_idx = OdbcDataTypeInfoRawCount;
	const OdbcDataTypeInfoRaw *data_ptr = OdbcDataTypeInfoRawList;

	for ( ; data_idx; --data_idx, ++data_ptr) {
		std::ostringstream o_str;
		o_str
			<< std::left << std::setw(18) << data_ptr->data_type_name_ <<
				std::right << " "
			<< std::setw(4) << data_ptr->data_type_ << " "
			<< std::left << std::setw(20) << data_ptr->c_type_name_ <<
				std::right << " "
			<< std::setw(3) <<
				MLB::Utility::AnyToString(data_ptr->c_type_size_) << " "
			<< std::setw(3) <<
				MLB::Utility::AnyToString(data_ptr->c_type_alignment_) << " "
			<< ((data_ptr->is_string_) ? "True " : "False") << " "
			<< ((data_ptr->is_struct_) ? "True " : "False") << " "
			<< ((data_ptr->is_blob_)   ? "True " : "False");
std::cout << o_str.str() << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////










// ////////////////////////////////////////////////////////////////////////////
template <SQLSMALLINT HandleType>
	struct HandleTypeInfo {
};
//	----------------------------------------------------------------------------
template <> struct HandleTypeInfo<SQL_HANDLE_ENV> {
	typedef SQLHENV                          HandleTypeStrict;
	typedef OdbcHandleEnv                    HandleTypeLib;
	typedef boost::shared_ptr<HandleTypeLib> HandleTypeLibSPtr;
	static HandleGetAttrFunc GetAttrFuncBasic() { return(GetEnvAttrBasic); }
	static HandleSetAttrFunc SetAttrFuncBasic() { return(SetEnvAttrBasic); }

	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLSMALLINT &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUSMALLINT &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLINTEGER &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUINTEGER &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		std::string &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrFlags32 &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrWindowHandle &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}

	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLSMALLINT ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUSMALLINT ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLINTEGER ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUINTEGER ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		const std::string &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrFlags32 ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrWindowHandle ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetEnvAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
};
//	----------------------------------------------------------------------------
template <> struct HandleTypeInfo<SQL_HANDLE_DBC> {
	typedef SQLHDBC                          HandleTypeStrict;
	typedef OdbcHandleDbc                    HandleTypeLib;
	typedef boost::shared_ptr<HandleTypeLib> HandleTypeLibSPtr;
	static HandleGetAttrFunc GetAttrFuncBasic() { return(GetConnectAttrBasic); }
	static HandleSetAttrFunc SetAttrFuncBasic() { return(SetConnectAttrBasic); }

	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLSMALLINT &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUSMALLINT &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLINTEGER &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUINTEGER &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		std::string &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrFlags32 &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrWindowHandle &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}

	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLSMALLINT ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUSMALLINT ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLINTEGER ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUINTEGER ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		const std::string &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrFlags32 ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrWindowHandle ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetConnectAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
};
//	----------------------------------------------------------------------------
template <> struct HandleTypeInfo<SQL_HANDLE_STMT> {
	typedef SQLHSTMT                         HandleTypeStrict;
	typedef OdbcHandleStmt                   HandleTypeLib;
	typedef boost::shared_ptr<HandleTypeLib> HandleTypeLibSPtr;
	static HandleGetAttrFunc GetAttrFuncBasic() { return(GetStmtAttrBasic); }
	static HandleSetAttrFunc SetAttrFuncBasic() { return(SetStmtAttrBasic); }

	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLSMALLINT &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUSMALLINT &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLINTEGER &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUINTEGER &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		std::string &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrFlags32 &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN GetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrWindowHandle &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(GetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}

	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLSMALLINT ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUSMALLINT ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLINTEGER ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		SQLUINTEGER ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		const std::string &ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrFlags32 ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
	static SQLRETURN SetAttr(HandleTypeStrict Handle, SQLINTEGER Attribute,
		OdbcAttrWindowHandle ValuePtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData)
	{
		return(SetStmtAttr(Handle, Attribute, ValuePtr, throw_flags));
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Supported attribute data types...
// ////////////////////////////////////////////////////////////////////////////
struct AttrType_SQLSMALLINT {
	typedef SQLSMALLINT value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_SQLUSMALLINT {
	typedef SQLUSMALLINT value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_SQLINTEGER {
	typedef SQLINTEGER value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_SQLUINTEGER {
	typedef SQLUINTEGER value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_StdString {
	typedef std::string value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_BitFlags32 {
	typedef OdbcAttrFlags32 value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_WindowHandle32 {
	typedef OdbcAttrWindowHandle value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_VoidPointer {
	typedef void *value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_SQLUINTEGERPointer {
	typedef SQLUINTEGER *value_type;
};
//	----------------------------------------------------------------------------
struct AttrType_SQLUSMALLINTPointer {
	typedef SQLUSMALLINT *value_type;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Environment attributes...
// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct AttrEnvTypeInfo {
};
//	----------------------------------------------------------------------------
template <> struct AttrEnvTypeInfo<SQL_ATTR_CONNECTION_POOLING> :
	public virtual HandleTypeInfo<SQL_HANDLE_ENV>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CONNECTION_POOLING };
};
//	----------------------------------------------------------------------------
template <> struct AttrEnvTypeInfo<SQL_ATTR_CP_MATCH> :
	public virtual HandleTypeInfo<SQL_HANDLE_ENV>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CP_MATCH };
};
//	----------------------------------------------------------------------------
template <> struct AttrEnvTypeInfo<SQL_ATTR_ODBC_VERSION> :
	public virtual HandleTypeInfo<SQL_HANDLE_ENV>,
	public virtual AttrType_SQLINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ODBC_VERSION };
};
//	----------------------------------------------------------------------------
template <> struct AttrEnvTypeInfo<SQL_ATTR_OUTPUT_NTS> :
	public virtual HandleTypeInfo<SQL_HANDLE_ENV>,
	public virtual AttrType_SQLINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_OUTPUT_NTS };
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Connection attributes...
// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct AttrConnectTypeInfo {
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_ACCESS_MODE> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ACCESS_MODE };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_ASYNC_ENABLE> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ASYNC_ENABLE };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_AUTO_IPD> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_AUTO_IPD };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_AUTOCOMMIT> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_AUTOCOMMIT };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_CONNECTION_DEAD> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CONNECTION_DEAD };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_CONNECTION_TIMEOUT> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CONNECTION_TIMEOUT };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_CURRENT_CATALOG> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_StdString {
	enum { ThisAttrSelector = SQL_ATTR_CURRENT_CATALOG };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_LOGIN_TIMEOUT> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_LOGIN_TIMEOUT };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_METADATA_ID> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_METADATA_ID };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_ODBC_CURSORS> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ODBC_CURSORS };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_PACKET_SIZE> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PACKET_SIZE };
};
//	----------------------------------------------------------------------------
//	CODE NOTE: Must be a 32-bit window handle (hwnd).
template <> struct AttrConnectTypeInfo<SQL_ATTR_QUIET_MODE> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_WindowHandle32 {
	enum { ThisAttrSelector = SQL_ATTR_QUIET_MODE };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_TRACE> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_TRACE };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_TRACEFILE> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_StdString {
	enum { ThisAttrSelector = SQL_ATTR_TRACEFILE };
};
//	----------------------------------------------------------------------------
template <> struct AttrConnectTypeInfo<SQL_ATTR_TRANSLATE_LIB> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_StdString {
	enum { ThisAttrSelector = SQL_ATTR_TRANSLATE_LIB };
};
//	----------------------------------------------------------------------------
//	CODE NOTE: Must be a 32-bit quantity.
template <> struct AttrConnectTypeInfo<SQL_ATTR_TRANSLATE_OPTION> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_BitFlags32 {
	enum { ThisAttrSelector = SQL_ATTR_TRANSLATE_OPTION };
};
//	----------------------------------------------------------------------------
//	CODE NOTE: Must be a 32-bit quantity.
template <> struct AttrConnectTypeInfo<SQL_ATTR_TXN_ISOLATION> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_BitFlags32 {
	enum { ThisAttrSelector = SQL_ATTR_TXN_ISOLATION };
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Statement attributes...
// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct AttrStmtTypeInfo {
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_APP_PARAM_DESC> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_APP_PARAM_DESC };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_APP_ROW_DESC> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_APP_ROW_DESC };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ASYNC_ENABLE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ASYNC_ENABLE };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_CONCURRENCY> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CONCURRENCY };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_SCROLLABLE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CURSOR_SCROLLABLE };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_SENSITIVITY> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CURSOR_SENSITIVITY };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_TYPE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CURSOR_TYPE };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ENABLE_AUTO_IPD> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ENABLE_AUTO_IPD };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_FETCH_BOOKMARK_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_VoidPointer {
	enum { ThisAttrSelector = SQL_ATTR_FETCH_BOOKMARK_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_IMP_PARAM_DESC> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_IMP_PARAM_DESC };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_IMP_ROW_DESC> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_IMP_ROW_DESC };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_KEYSET_SIZE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_KEYSET_SIZE };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_MAX_LENGTH> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_MAX_LENGTH };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_MAX_ROWS> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_MAX_ROWS };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_METADATA_ID> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_METADATA_ID };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_NOSCAN> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_NOSCAN };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_BIND_OFFSET_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGERPointer {
	enum { ThisAttrSelector = SQL_ATTR_PARAM_BIND_OFFSET_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_BIND_TYPE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PARAM_BIND_TYPE };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_OPERATION_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUSMALLINTPointer {
	enum { ThisAttrSelector = SQL_ATTR_PARAM_OPERATION_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_STATUS_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUSMALLINTPointer {
	enum { ThisAttrSelector = SQL_ATTR_PARAM_STATUS_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAMS_PROCESSED_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGERPointer {
	enum { ThisAttrSelector = SQL_ATTR_PARAMS_PROCESSED_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAMSET_SIZE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PARAMSET_SIZE };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_QUERY_TIMEOUT> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_QUERY_TIMEOUT };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_RETRIEVE_DATA> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_RETRIEVE_DATA };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_ARRAY_SIZE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_ARRAY_SIZE };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_BIND_OFFSET_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGERPointer {
	enum { ThisAttrSelector = SQL_ATTR_ROW_BIND_OFFSET_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_BIND_TYPE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_BIND_TYPE };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_NUMBER> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_NUMBER };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_OPERATION_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUSMALLINTPointer {
	enum { ThisAttrSelector = SQL_ATTR_ROW_OPERATION_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_STATUS_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUSMALLINTPointer {
	enum { ThisAttrSelector = SQL_ATTR_ROW_STATUS_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROWS_FETCHED_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGERPointer {
	enum { ThisAttrSelector = SQL_ATTR_ROWS_FETCHED_PTR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_SIMULATE_CURSOR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_SIMULATE_CURSOR };
};
//	----------------------------------------------------------------------------
template <> struct AttrStmtTypeInfo<SQL_ATTR_USE_BOOKMARKS> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_USE_BOOKMARKS };
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename TypeInfoType>
	struct SavedAttr : public TypeInfoType {
protected:
	typedef typename TypeInfoType::value_type        AttrType;
	typedef typename TypeInfoType::HandleTypeStrict  HandleTypeStrict;
	typedef typename TypeInfoType::HandleTypeLib     HandleTypeLib;
	typedef typename TypeInfoType::HandleTypeLibSPtr HandleTypeLibSPtr;

	SavedAttr(HandleTypeStrict my_handle)
	{
		GetAttr(my_handle, ThisAttrSelector, saved_value_);
	}
	SavedAttr(HandleTypeLib &my_handle)
	{
		GetAttr(my_handle.GetNativeHandle(), ThisAttrSelector, saved_value_);
	}
	SavedAttr(HandleTypeLibSPtr &my_handle)
	{
		GetAttr(my_handle->GetNativeHandle(), ThisAttrSelector, saved_value_);
	}

public:
	SQLINTEGER  attr_selector_;
	AttrType    saved_value_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct SavedEnvAttr : public SavedAttr<AttrEnvTypeInfo<AttrSelector> >
{
	typedef typename AttrEnvTypeInfo<AttrSelector>::value_type        AttrType;
	typedef typename AttrEnvTypeInfo<AttrSelector>::HandleTypeStrict  HandleTypeStrict;
	typedef typename AttrEnvTypeInfo<AttrSelector>::HandleTypeLib     HandleTypeLib;
	typedef typename AttrEnvTypeInfo<AttrSelector>::HandleTypeLibSPtr HandleTypeLibSPtr;

	SavedEnvAttr(HandleTypeStrict my_handle)
		:SavedAttr<AttrEnvTypeInfo<AttrSelector> >(my_handle)
	{
	}
	SavedEnvAttr(HandleTypeLib &my_handle)
		:SavedAttr<AttrEnvTypeInfo<AttrSelector> >(my_handle)
	{
	}
	SavedEnvAttr(HandleTypeLibSPtr &my_handle)
		:SavedAttr<AttrEnvTypeInfo<AttrSelector> >(my_handle)
	{
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct SavedConnectAttr : public SavedAttr<AttrConnectTypeInfo<AttrSelector> >
{
	typedef typename AttrConnectTypeInfo<AttrSelector>::value_type        AttrType;
	typedef typename AttrConnectTypeInfo<AttrSelector>::HandleTypeStrict  HandleTypeStrict;
	typedef typename AttrConnectTypeInfo<AttrSelector>::HandleTypeLib     HandleTypeLib;
	typedef typename AttrConnectTypeInfo<AttrSelector>::HandleTypeLibSPtr HandleTypeLibSPtr;

	SavedConnectAttr(HandleTypeStrict my_handle)
		:SavedAttr<AttrConnectTypeInfo<AttrSelector> >(my_handle)
	{
	}
	SavedConnectAttr(HandleTypeLib &my_handle)
		:SavedAttr<AttrConnectTypeInfo<AttrSelector> >(my_handle)
	{
	}
	SavedConnectAttr(HandleTypeLibSPtr &my_handle)
		:SavedAttr<AttrConnectTypeInfo<AttrSelector> >(my_handle)
	{
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct SavedStmtAttr : public SavedAttr<AttrStmtTypeInfo<AttrSelector> >
{
	typedef typename AttrStmtTypeInfo<AttrSelector>::value_type       AttrType;
	typedef typename AttrStmtTypeInfo<AttrSelector>::HandleTypeStrict HandleTypeStrict;
	typedef typename AttrStmtTypeInfo<AttrSelector>::HandleTypeLib    HandleTypeLib;
	typedef typename AttrStmtTypeInfo<AttrSelector>::HandleTypeLibSPtr HandleTypeLibSPtr;

	SavedStmtAttr(HandleTypeStrict my_handle)
		:SavedAttr<AttrStmtTypeInfo<AttrSelector> >(my_handle)
	{
	}
	SavedStmtAttr(HandleTypeLib &my_handle)
		:SavedAttr<AttrStmtTypeInfo<AttrSelector> >(my_handle)
	{
	}
	SavedStmtAttr(HandleTypeLibSPtr &my_handle)
		:SavedAttr<AttrStmtTypeInfo<AttrSelector> >(my_handle)
	{
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename TypeInfoType>
	struct PushedAttr : public SavedAttr<TypeInfoType> {
protected:
	typedef typename TypeInfoType::value_type        AttrType;
	typedef typename TypeInfoType::HandleTypeStrict  HandleTypeStrict;
	typedef typename TypeInfoType::HandleTypeLib     HandleTypeLib;
	typedef typename TypeInfoType::HandleTypeLibSPtr HandleTypeLibSPtr;

	PushedAttr(HandleTypeStrict my_handle, const AttrType &new_value)
		:SavedAttr<TypeInfoType>(my_handle)
		,my_handle_(my_handle)
	{
		SetAttr(my_handle_, ThisAttrSelector, new_value);
	}
	PushedAttr(HandleTypeLib &my_handle, const AttrType &new_value)
		:SavedAttr<TypeInfoType>(my_handle)
		,my_handle_(my_handle.GetNativeHandle())
	{
		SetAttr(my_handle_, ThisAttrSelector, new_value);
	}
	PushedAttr(HandleTypeLibSPtr &my_handle, const AttrType &new_value)
		:SavedAttr<TypeInfoType>(my_handle)
		,my_handle_(my_handle->GetNativeHandle())
	{
		SetAttr(my_handle_, ThisAttrSelector, new_value);
	}
	~PushedAttr()
	{
		try {
			SetAttr(my_handle_, ThisAttrSelector, saved_value_);
		}
		catch (const std::exception &) {
		}
	}

public:
	HandleTypeStrict my_handle_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct PushedEnvAttr : public PushedAttr<AttrEnvTypeInfo<AttrSelector> >
{
	typedef typename AttrEnvTypeInfo<AttrSelector>::value_type        AttrType;
	typedef typename AttrEnvTypeInfo<AttrSelector>::HandleTypeStrict  HandleTypeStrict;
	typedef typename AttrEnvTypeInfo<AttrSelector>::HandleTypeLib     HandleTypeLib;
	typedef typename AttrEnvTypeInfo<AttrSelector>::HandleTypeLibSPtr HandleTypeLibSPtr;

	PushedEnvAttr(HandleTypeStrict my_handle, const AttrType &new_value)
		:PushedAttr<AttrEnvTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
	PushedEnvAttr(HandleTypeLib &my_handle, const AttrType &new_value)
		:PushedAttr<AttrEnvTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
	PushedEnvAttr(HandleTypeLibSPtr &my_handle, const AttrType &new_value)
		:PushedAttr<AttrEnvTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct PushedConnectAttr : public PushedAttr<AttrConnectTypeInfo<AttrSelector> >
{
	typedef typename AttrConnectTypeInfo<AttrSelector>::value_type        AttrType;
	typedef typename AttrConnectTypeInfo<AttrSelector>::HandleTypeStrict  HandleTypeStrict;
	typedef typename AttrConnectTypeInfo<AttrSelector>::HandleTypeLib     HandleTypeLib;
	typedef typename AttrConnectTypeInfo<AttrSelector>::HandleTypeLibSPtr HandleTypeLibSPtr;

	PushedConnectAttr(HandleTypeStrict my_handle, const AttrType &new_value)
		:PushedAttr<AttrConnectTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
	PushedConnectAttr(HandleTypeLib &my_handle, const AttrType &new_value)
		:PushedAttr<AttrConnectTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
	PushedConnectAttr(HandleTypeLibSPtr &my_handle, const AttrType &new_value)
		:PushedAttr<AttrConnectTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct PushedStmtAttr : public PushedAttr<AttrStmtTypeInfo<AttrSelector> >
{
	typedef typename AttrStmtTypeInfo<AttrSelector>::value_type       AttrType;
	typedef typename AttrStmtTypeInfo<AttrSelector>::HandleTypeStrict HandleTypeStrict;
	typedef typename AttrStmtTypeInfo<AttrSelector>::HandleTypeLib    HandleTypeLib;
	typedef typename AttrStmtTypeInfo<AttrSelector>::HandleTypeLibSPtr HandleTypeLibSPtr;

	PushedStmtAttr(HandleTypeStrict my_handle, const AttrType &new_value)
		:PushedAttr<AttrStmtTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
	PushedStmtAttr(HandleTypeLib &my_handle, const AttrType &new_value)
		:PushedAttr<AttrStmtTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
	PushedStmtAttr(HandleTypeLibSPtr &my_handle, const AttrType &new_value)
		:PushedAttr<AttrStmtTypeInfo<AttrSelector> >(my_handle, new_value)
	{
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <int SqlTypeCode>
	struct OdbcTypeInfo {
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_CHAR> {
	typedef SQLSCHAR ValueType;
	enum { AddedLength = 1 };
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_TINYINT> {
	typedef unsigned ValueType;
	enum { AddedLength = 0 };
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_LONG> {
	typedef SQLINTEGER ValueType;
	enum { AddedLength = 0 };
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_SHORT> {
	typedef SQLSMALLINT ValueType;
	enum { AddedLength = 0 };
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_FLOAT> {
	typedef SQLFLOAT ValueType;
	enum { AddedLength = 0 };
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_DOUBLE> {
	typedef SQLDOUBLE ValueType;
	enum { AddedLength = 0 };
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_DATE> {
	typedef SQL_DATE_STRUCT ValueType;
	enum { AddedLength = 0 };
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_TIME> {
	typedef SQL_TIME_STRUCT ValueType;
	enum { AddedLength = 0 };
};
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_C_TIMESTAMP> {
	typedef SQL_TIMESTAMP_STRUCT ValueType;
	enum { AddedLength = 0 };
};
/*
//	----------------------------------------------------------------------------
template <> struct OdbcTypeInfo<SQL_TYPE_TIMESTAMP> {
	typedef VVV ValueType;
	enum { AddedLength = 0 };
};
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::size_t OdbcRowAlignment = 16;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcBinderCol {
	OdbcBinderCol()
		:col_number_(0)
		,col_offset_(0)
		,col_length_(0)
		,col_length_used_(0)
		,col_indicator_offset_(0)
	{
	}

	const char       *GetPointer(const void *row_pointer) const
	{
		return(static_cast<const char *>(row_pointer) + col_offset_);
	}
	const SQLINTEGER *GetPointerInd(const void *row_pointer) const
	{
		return(static_cast<const SQLINTEGER *>(row_pointer) + col_offset_);
	}
	const SQLINTEGER  GetInd(const void *row_pointer) const
	{
		return(*GetPointerInd(row_pointer));
	}

	SQLSMALLINT                col_number_;
	std::size_t                col_offset_;
	std::size_t                col_length_;
	std::size_t                col_length_used_;
	std::size_t                col_indicator_offset_;
	const OdbcDataTypeInfoRaw *col_type_ptr_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<OdbcBinderCol>        OdbcBinderColList;
typedef OdbcBinderColList::iterator       OdbcBinderColListIter;
typedef OdbcBinderColList::const_iterator OdbcBinderColListIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcBinderRow {
private:
	typedef PushedStmtAttr<SQL_ATTR_ROW_BIND_TYPE>    MyRowBindAttr;
	typedef PushedStmtAttr<SQL_ATTR_ROW_ARRAY_SIZE>   MyRowArrayAttr;
	typedef PushedStmtAttr<SQL_ATTR_ROWS_FETCHED_PTR> MyRowsFetchedAttr;
	typedef boost::shared_ptr<MyRowBindAttr>          MyRowBindAttrSPtr;
	typedef boost::shared_ptr<MyRowArrayAttr>         MyRowArrayAttrSPtr;
	typedef boost::shared_ptr<MyRowsFetchedAttr>      MyRowsFetchedAttrSPtr;

public:
	OdbcBinderRow(OdbcHandleStmtSPtr &odbc_stmt_sptr,
		const std::vector<SQLSMALLINT> &col_number_list =
		std::vector<SQLSMALLINT>());
	OdbcBinderRow(unsigned int row_alloc_count,
		OdbcHandleStmtSPtr &odbc_stmt_sptr,
		const std::vector<SQLSMALLINT> &col_number_list =
		std::vector<SQLSMALLINT>());

	SQLRETURN FetchScroll(SQLSMALLINT FetchOrientation = SQL_FETCH_NEXT,
		SQLINTEGER FetchOffset = 0);

	bool IsValidRow(unsigned int row_index) const;
	bool IsValidCol(unsigned int col_index) const;
	bool IsValidRowCol(unsigned int row_index, unsigned int col_index) const;
	bool CheckIsValidRow(unsigned int row_index) const;
	bool CheckIsValidCol(unsigned int col_index) const;
	bool CheckIsValidRowCol(unsigned int row_index, unsigned int col_index) const;

	unsigned int      GetRowCount() const;
	const char       *GetPointer(unsigned int row_index, unsigned int col_index) const;
	const SQLINTEGER *GetPointerInd(unsigned int row_index, unsigned int col_index) const;
	const SQLINTEGER  GetInd(unsigned int row_index, unsigned int col_index) const;



	OdbcHandleStmtSPtr        odbc_stmt_sptr_;
	unsigned int              row_alloc_count_;
	SQLSMALLINT               total_col_count_;
	std::size_t               total_row_size_;
	ColumnAttrList            col_attr_list_;
	OdbcBinderColList         col_list_;
	boost::shared_array<char> row_data_;
	SQLUINTEGER               current_row_count_;
	MyRowBindAttrSPtr         attr_row_bind_;
	MyRowArrayAttrSPtr        attr_row_array_;
	MyRowsFetchedAttrSPtr     attr_rows_fetched_;

	std::string              TEST_ToString() const;
	std::vector<std::string> TEST_ToStringList() const;

private:
	void Initialize(unsigned int row_alloc_count,
		OdbcHandleStmtSPtr &odbc_stmt_sptr,
		const std::vector<SQLSMALLINT> &col_number_list =
		std::vector<SQLSMALLINT>());
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcBinderRow::OdbcBinderRow(OdbcHandleStmtSPtr &odbc_stmt_sptr,
	const std::vector<SQLSMALLINT> &col_number_list)
	:odbc_stmt_sptr_(odbc_stmt_sptr)
	,row_alloc_count_(0)
	,total_col_count_(0)
	,total_row_size_(0)
	,col_attr_list_()
	,col_list_()
	,row_data_()
	,current_row_count_(0)
	,attr_row_bind_()
	,attr_row_array_()
	,attr_rows_fetched_()
{
	Initialize(1, odbc_stmt_sptr, col_number_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcBinderRow::OdbcBinderRow(unsigned int row_alloc_count,
	OdbcHandleStmtSPtr &odbc_stmt_sptr,
	const std::vector<SQLSMALLINT> &col_number_list)
	:odbc_stmt_sptr_(odbc_stmt_sptr)
	,row_alloc_count_(0)
	,total_col_count_(0)
	,total_row_size_(0)
	,col_list_()
	,row_data_()
	,current_row_count_(0)
	,attr_row_bind_()
	,attr_row_array_()
	,attr_rows_fetched_()
{
	Initialize(row_alloc_count, odbc_stmt_sptr, col_number_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcBinderRow::FetchScroll(SQLSMALLINT FetchOrientation,
	SQLINTEGER FetchOffset)
{
	return(odbc_stmt_sptr_->FetchScroll(FetchOrientation, FetchOffset));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcBinderRow::IsValidRow(unsigned int row_index) const
{
	return(row_index < row_alloc_count_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcBinderRow::IsValidCol(unsigned int col_index) const
{
	return(col_index < col_list_.size());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcBinderRow::IsValidRowCol(unsigned int row_index,
	unsigned int col_index) const
{
	return(IsValidRow(row_index) && IsValidCol(col_index));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcBinderRow::CheckIsValidRow(unsigned int row_index) const
{
	if (!IsValidRow(row_index))
		MLB::Utility::ThrowInvalidArgument("The specified row index (" +
			MLB::Utility::AnyToString(row_index) + ") is outside of the "
			"permissible range (from 0 to " +
			MLB::Utility::AnyToString(row_alloc_count_) + ", inclusive.");

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcBinderRow::CheckIsValidCol(unsigned int col_index) const
{
	if (!IsValidCol(col_index))
		MLB::Utility::ThrowInvalidArgument("The specified column index (" +
			MLB::Utility::AnyToString(col_index) + ") is outside of the "
			"permissible range (from 0 to " +
			MLB::Utility::AnyToString(col_list_.size()) + ", inclusive.");

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcBinderRow::CheckIsValidRowCol(unsigned int row_index,
	unsigned int col_index) const
{
	CheckIsValidRow(row_index);
	CheckIsValidCol(col_index);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int OdbcBinderRow::GetRowCount() const
{
	return(current_row_count_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *OdbcBinderRow::GetPointer(unsigned int row_index,
	unsigned int col_index) const
{
	CheckIsValidRowCol(row_index, col_index);

	return(col_list_[col_index].
		GetPointer(row_data_.get() + (row_index * total_row_size_)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const SQLINTEGER *OdbcBinderRow::GetPointerInd(unsigned int row_index,
	unsigned int col_index) const
{
	CheckIsValidRowCol(row_index, col_index);

	return(col_list_[col_index].
		GetPointerInd(row_data_.get() + (row_index * total_row_size_)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const SQLINTEGER OdbcBinderRow::GetInd(unsigned int row_index,
	unsigned int col_index) const
{
	CheckIsValidRowCol(row_index, col_index);

	return(col_list_[col_index].
		GetInd(row_data_.get() + (row_index * total_row_size_)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OdbcBinderRow::Initialize(unsigned int row_alloc_count,
	OdbcHandleStmtSPtr &odbc_stmt_sptr,
	const std::vector<SQLSMALLINT> &col_number_list)
{
	try {
		odbc_stmt_sptr->NumResultCols(&total_col_count_);
		if (total_col_count_ < 1)
			MLB::Utility::ThrowRuntimeError("Invocation of '::SQLNumResultCols()' "
				"reports " + MLB::Utility::AnyToString(total_col_count_) +
				" columns in the result set.");
		ColumnAttr::GetColumnAttrList(odbc_stmt_sptr->GetNativeHandle(),
			col_attr_list_);
		if (!col_number_list.empty()) {
			ColumnAttrList                      col_attr_list;
			std::vector<SQLSMALLINT>::size_type count_1;
			std::vector<SQLSMALLINT>::size_type col_count = col_number_list.size();
			for (count_1 = 0; count_1 < col_count; ++count_1) {
				if ((col_number_list[count_1] < 1) ||
					(col_number_list[count_1] > total_col_count_))
					MLB::Utility::ThrowRuntimeError("The element at index " +
						MLB::Utility::AnyToString(count_1) + " of the column number "
						"vector (" +
						MLB::Utility::AnyToString(col_number_list[count_1]) +
						") is invalid --- the valid range is from the 1 to " +
						MLB::Utility::AnyToString(total_col_count_) +
						", inclusive.");
				col_attr_list.push_back(col_attr_list_[col_number_list[count_1]]);
			}
			col_attr_list_.swap(col_attr_list);
		}
		//	Remove any blob types...
		{
			ColumnAttrList                      col_attr_list;
			ColumnAttrListIterC iter_b(col_attr_list_.begin());
			ColumnAttrListIterC iter_e(col_attr_list_.end());
			for ( ; iter_b != iter_e; ++iter_b) {
				if ((iter_b->concise_type_ != SQL_WCHAR) &&
					 (iter_b->concise_type_ != SQL_WVARCHAR) &&
					(iter_b->concise_type_ != SQL_WLONGVARCHAR) &&
					 (iter_b->concise_type_ != -150) &&
					 (iter_b->concise_type_ != -152))
					col_attr_list.push_back(*iter_b);
			}
			col_attr_list_.swap(col_attr_list);
		}
		//	Initial pass t get the column information...
		col_list_.reserve(col_attr_list_.size());
		std::size_t         current_offset = 0;
		ColumnAttrListIterC iter_b(col_attr_list_.begin());
		ColumnAttrListIterC iter_e(col_attr_list_.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			OdbcBinderCol              this_col;
			std::size_t                col_base_size;
			std::size_t                col_total_size;
			const OdbcDataTypeInfoRaw *col_type_ptr =
				OdbcDataTypeInfoRaw::GetInfoForTypeChecked(
				static_cast<SQLSMALLINT>(iter_b->concise_type_));
			if (current_offset % col_type_ptr->c_type_alignment_)
				current_offset += col_type_ptr->c_type_alignment_ -
					(current_offset % col_type_ptr->c_type_alignment_);
			col_base_size                  = iter_b->octet_length_;
			col_total_size                 = col_base_size +
				((col_type_ptr->is_string_) ? 1 : 0);
			this_col.col_number_           = iter_b->column_number_;
			this_col.col_offset_           = current_offset;
			this_col.col_length_           = col_base_size;
			this_col.col_length_used_      = col_total_size;
			this_col.col_indicator_offset_ = 0;
			this_col.col_type_ptr_         = col_type_ptr;
			col_list_.push_back(this_col);
			current_offset += col_total_size;
		}
		//	Second pass to determine the offsets of the indicator values...
		if (current_offset % sizeof(SQLINTEGER))
			current_offset += sizeof(SQLINTEGER) -
				(current_offset % sizeof(SQLINTEGER));
		ColumnAttrList::size_type col_count = col_list_.size();
		ColumnAttrList::size_type col_index;
		for (col_index = 0; col_index < col_count; ++col_index) {
			col_list_[col_index].col_indicator_offset_  = current_offset;
			current_offset                             += sizeof(SQLINTEGER);
		}
		//	Start each row on a 16-byte boundary...
		if (current_offset % OdbcRowAlignment)
			current_offset += OdbcRowAlignment -
				(current_offset % OdbcRowAlignment);
		total_row_size_  = current_offset;
		row_alloc_count_ = row_alloc_count;
		row_data_.reset(new char[row_alloc_count_ * total_row_size_]);
		attr_row_bind_.reset(new MyRowBindAttr(odbc_stmt_sptr,
			static_cast<SQLUINTEGER>(total_row_size_)));
		attr_row_array_.reset(new MyRowArrayAttr(odbc_stmt_sptr,
			static_cast<SQLUINTEGER>(row_alloc_count_)));
/*
		attr_rows_fetched_.reset(new MyRowsFetchedAttr(odbc_stmt_sptr,
			current_row_count_));

MyRowsFetchedAttr XXX(odbc_stmt_sptr, current_row_count_);
*/
PushedStmtAttr<SQL_ATTR_ROWS_FETCHED_PTR> YYY(odbc_stmt_sptr, &current_row_count_);

		for (col_index = 0; col_index < col_count; ++col_index)
			odbc_stmt_sptr->BindCol(col_list_[col_index].col_number_,
				col_list_[col_index].col_type_ptr_->c_type_,
				row_data_.get() + col_list_[col_index].col_offset_,
				static_cast<SQLINTEGER>(col_list_[col_index].col_length_used_),
				reinterpret_cast<SQLINTEGER *>(row_data_.get() +
				col_list_[col_index].col_indicator_offset_));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to construct an 'OdbcBinderRow' "
			"instance failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OdbcBinderRow::TEST_ToString() const
{
	std::ostringstream o_str;

	o_str << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	o_str << "Allocated Row Count : " <<
		static_cast<unsigned int>(row_alloc_count_) << std::endl;
	o_str << "Total Column Count  : " <<
		static_cast<unsigned int>(total_col_count_) << std::endl;
	o_str << "Total Row Size      : " <<
		static_cast<unsigned int>(total_row_size_) << std::endl;
	o_str << "Allocated Row Buffer: " <<
		MLB::Utility::ValueToStringHex(row_data_.get()) << std::endl;

	o_str << MLB::Utility::PadLeft("", 79, '-') << std::endl;

	unsigned int           count_1;
	OdbcBinderColListIterC iter_b(col_list_.begin());
	OdbcBinderColListIterC iter_e(col_list_.end());

	for (count_1 = 0 ; iter_b != iter_e; ++iter_b, ++count_1) {
		o_str
			<< std::setw(4) << iter_b->col_number_           << " "
			<< std::setw(4) <<
				static_cast<unsigned int>(iter_b->col_offset_)           << " "
			<< std::setw(4) <<
				static_cast<unsigned int>(iter_b->col_length_)           << " "
			<< std::setw(4) <<
				static_cast<unsigned int>(iter_b->col_length_used_)      << " "
			<< std::setw(4) <<
				static_cast<unsigned int>(iter_b->col_indicator_offset_) << " "
			<< MLB::Utility::ValueToStringHex(GetPointer(0, count_1)) << std::endl;
	}

	o_str << MLB::Utility::PadLeft("", 79, '=');

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OdbcBinderRow::TEST_ToStringList() const
{
	std::vector<std::string> out_list;

	return(MLB::Utility::SplitString(TEST_ToString(), "\n", out_list, 0, true));
}
// ////////////////////////////////////////////////////////////////////////////








} // namespace OdbcWrapper

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OdbcWrapper;

// ////////////////////////////////////////////////////////////////////////////
void TEST_AttrPush(int &return_code, OdbcHandleDbcSPtr &odbc_dbc_sptr)
{
	try {
		OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
SavedConnectAttr<SQL_ATTR_CURRENT_CATALOG> conn_catalog_1(odbc_dbc_sptr);
std::cout << "SQL_ATTR_CURRENT_CATALOG #1 = " <<
	conn_catalog_1.saved_value_ << std::endl;
{
	PushedConnectAttr<SQL_ATTR_CURRENT_CATALOG> conn_catalog_2(odbc_dbc_sptr,
		"AdventureWorks");
	std::cout << "SQL_ATTR_CURRENT_CATALOG #2 = " <<
		conn_catalog_2.saved_value_ << std::endl;
	SavedConnectAttr<SQL_ATTR_CURRENT_CATALOG> conn_catalog_3(odbc_dbc_sptr);
	std::cout << "SQL_ATTR_CURRENT_CATALOG #3 = " <<
		conn_catalog_3.saved_value_ << std::endl;
}
SavedConnectAttr<SQL_ATTR_CURRENT_CATALOG> conn_catalog_4(odbc_dbc_sptr);
std::cout << "SQL_ATTR_CURRENT_CATALOG #4 = " <<
	conn_catalog_4.saved_value_ << std::endl;

		SavedStmtAttr<SQL_ATTR_ROW_BIND_TYPE>   saved_bind_type(odbc_stmt_sptr);
		SavedStmtAttr<SQL_ATTR_ASYNC_ENABLE>    async_enabled(odbc_stmt_sptr);
		SavedStmtAttr<SQL_ATTR_QUERY_TIMEOUT>   query_timeout(odbc_stmt_sptr);
		SavedStmtAttr<SQL_ATTR_ROW_ARRAY_SIZE>  row_array_size(odbc_stmt_sptr);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}
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
		odbc_stmt_sptr->ExecDirect(sql_string);
		OdbcBinderRow bound_data(odbc_stmt_sptr);
std::cout << bound_data.TEST_ToString() << std::endl;
std::cout << MLB::Utility::ValueToStringHex(bound_data.row_data_.get()) << std::endl;
std::cout << MLB::Utility::ValueToStringHex(bound_data.GetPointer(0, 2)) << std::endl;

SQLROWSETSIZE row_count;
std::cout << SQLSetStmtAttr(odbc_stmt_sptr->GetNativeHandle(), SQL_ATTR_ROWS_FETCHED_PTR, &row_count, 0 ) << std::endl;

		bound_data.FetchScroll();
		const char *col_3_ptr(bound_data.GetPointer(0, 2));
		const char *col_4_ptr(bound_data.GetPointer(0, 3));
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
void TEST_RawBinder(int &return_code, OdbcHandleDbcSPtr &odbc_dbc_sptr)
{
	try {
		TEST_PerformTest(return_code, odbc_dbc_sptr,
			"SELECT * FROM GDSP_LON_TRD_TST.dbo.attribute");
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
//OdbcDataTypeInfoRaw::EmitTabularList();
		DriverConnectParams connect_params("sa", "S_a_0", "GDSP_LON_TRD_TST",
			"(local)", "{SQL Native Client}");
		connect_params.ParseDriverParams(1, argc, argv);
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
		OdbcHandleDbcSPtr odbc_dbc_sptr(odbc_env.AddConnection());
		odbc_dbc_sptr->Connect(connect_params);
		TEST_RawBinder(return_code, odbc_dbc_sptr);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR IN REGRESSION TEST FOR "
			"'MLB::OdbcWrapper::OdbcBinderRowRaw.cpp': " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

