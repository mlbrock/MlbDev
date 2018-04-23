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

#include <OdbcWrapper/OdbcHandle.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

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
};
//	----------------------------------------------------------------------------
template <> struct HandleTypeInfo<SQL_HANDLE_DBC> {
	typedef SQLHDBC                          HandleTypeStrict;
	typedef OdbcHandleDbc                    HandleTypeLib;
	typedef boost::shared_ptr<HandleTypeLib> HandleTypeLibSPtr;
	static HandleGetAttrFunc GetAttrFuncBasic() { return(GetConnectAttrBasic); }
	static HandleSetAttrFunc SetAttrFuncBasic() { return(SetConnectAttrBasic); }

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
};
//	----------------------------------------------------------------------------
template <> struct HandleTypeInfo<SQL_HANDLE_STMT> {
	typedef SQLHSTMT                         HandleTypeStrict;
	typedef OdbcHandleStmt                   HandleTypeLib;
	typedef boost::shared_ptr<HandleTypeLib> HandleTypeLibSPtr;
	static HandleGetAttrFunc GetAttrFuncBasic() { return(GetStmtAttrBasic); }
	static HandleSetAttrFunc SetAttrFuncBasic() { return(SetStmtAttrBasic); }

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
};
// ////////////////////////////////////////////////////////////////////////////

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
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Connect attributes...
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
	public virtual AttrType_SQLUINTEGER {
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
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_TRANSLATE_OPTION };
};
//	----------------------------------------------------------------------------
//	CODE NOTE: Must be a 32-bit quantity.
template <> struct AttrConnectTypeInfo<SQL_ATTR_TXN_ISOLATION> :
	public virtual HandleTypeInfo<SQL_HANDLE_DBC>,
	public virtual AttrType_SQLUINTEGER {
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
template <> struct AttrStmtTypeInfo<SQL_ATTR_APP_ROW_DESC> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_APP_ROW_DESC };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ASYNC_ENABLE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ASYNC_ENABLE };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_CONCURRENCY> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CONCURRENCY };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_SCROLLABLE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CURSOR_SCROLLABLE };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_SENSITIVITY> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CURSOR_SENSITIVITY };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_TYPE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_CURSOR_TYPE };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ENABLE_AUTO_IPD> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ENABLE_AUTO_IPD };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_FETCH_BOOKMARK_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_FETCH_BOOKMARK_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_IMP_PARAM_DESC> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_IMP_PARAM_DESC };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_IMP_ROW_DESC> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_IMP_ROW_DESC };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_KEYSET_SIZE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_KEYSET_SIZE };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_MAX_LENGTH> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_MAX_LENGTH };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_MAX_ROWS> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_MAX_ROWS };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_METADATA_ID> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_METADATA_ID };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_NOSCAN> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_NOSCAN };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_BIND_OFFSET_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PARAM_BIND_OFFSET_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_BIND_TYPE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PARAM_BIND_TYPE };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_OPERATION_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PARAM_OPERATION_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_STATUS_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PARAM_STATUS_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAMS_PROCESSED_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PARAMS_PROCESSED_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAMSET_SIZE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_PARAMSET_SIZE };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_QUERY_TIMEOUT> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_QUERY_TIMEOUT };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_RETRIEVE_DATA> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_RETRIEVE_DATA };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_ARRAY_SIZE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_ARRAY_SIZE };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_BIND_OFFSET_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_BIND_OFFSET_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_BIND_TYPE> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_BIND_TYPE };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_NUMBER> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_NUMBER };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_OPERATION_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_OPERATION_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_STATUS_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROW_STATUS_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROWS_FETCHED_PTR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_ROWS_FETCHED_PTR };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_SIMULATE_CURSOR> :
	public virtual HandleTypeInfo<SQL_HANDLE_STMT>,
	public virtual AttrType_SQLUINTEGER {
	enum { ThisAttrSelector = SQL_ATTR_SIMULATE_CURSOR };
};
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
//	----------------------------------------------------------------------------
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
//	----------------------------------------------------------------------------
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
//	----------------------------------------------------------------------------
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
//	----------------------------------------------------------------------------
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
template <typename SqlType>
	struct OdbcTypeInfo {
};
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

	const void *GetPointer(const void *row_pointer) const
	{
		return(static_cast<const char *>(row_pointer) + col_offset_);
	}

	SQLSMALLINT col_number_;
	std::size_t col_offset_;
	std::size_t col_length_;
	std::size_t col_length_used_;
	std::size_t col_indicator_offset_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<OdbcBinderCol>        OdbcBinderColList;
typedef OdbcBinderColList::iterator       OdbcBinderColListIter;
typedef OdbcBinderColList::const_iterator OdbcBinderColListIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcBinderRow {
	OdbcBinderRow(OdbcHandleStmtSPtr &odbc_stmt_sptr,
		const std::vector<SQLSMALLINT> &col_number_list =
		std::vector<SQLSMALLINT>());
	OdbcBinderRow(unsigned int row_alloc_count,
		OdbcHandleStmtSPtr &odbc_stmt_sptr,
		const std::vector<SQLSMALLINT> &col_number_list =
		std::vector<SQLSMALLINT>());

	OdbcHandleStmtSPtr odbc_stmt_sptr_;
	unsigned int       row_alloc_count_;
	SQLSMALLINT        total_col_count_;
	std::size_t        total_row_size_;
	OdbcBinderColList  col_list_;

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
	,col_list_()
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
{
	Initialize(row_alloc_count, odbc_stmt_sptr, col_number_list);
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
		std::vector<SQLSMALLINT>        tmp_col_number_list;
		const std::vector<SQLSMALLINT> &col_number_list_ref =
			(col_number_list.empty()) ? tmp_col_number_list : col_number_list;
		if (col_number_list.empty()) {
			tmp_col_number_list.resize(total_col_count_);
			SQLSMALLINT count_1;
			for (count_1 = 0; count_1 < total_col_count_; ++count_1)
				tmp_col_number_list[count_1] = count_1 + 1;
		}
		else {
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
			}
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to construct an 'OdbcBinderRow' "
			"instance failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OdbcWrapper;

// ////////////////////////////////////////////////////////////////////////////
void TEST_RawBinder(int &return_code, OdbcHandleDbcSPtr &odbc_dbc_sptr)
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
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
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

