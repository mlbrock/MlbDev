// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the ODBC wrapper library.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__OdbcWrapper_hpp__HH

#define HH__MLB__OdbcWrapper_hpp__HH	1

#define MLB__LIB__OdbcWrapper

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <BoostX.hpp>

#include <sql.h>
#include <sqlext.h>

#ifdef MLB__LIB__OdbcWrapper
# ifdef ODBCWRAPPER_DLL_EXPORTS
#  define API_ODBCWRAPPER		MB_LIB_EXPORT
# else
#  define API_ODBCWRAPPER		MB_LIB_IMPORT
# endif // #ifdef ODBCWRAPPER_DLL_EXPORTS
#else
#  define API_ODBCWRAPPER
#endif // #ifdef MLB__LIB__OdbcWrapper

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_ODBCWRAPPER

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER OdbcThrowFlags {
	/**
		\enum Specifies the conditions under which ODBC exceptions are thrown.
	*/
	enum Flags {
		None            = 0x0000,			/**< No exception will be thrown. */
		SuccessWithInfo = 0x0001,			/**< Throw if SQL_SUCCESS_WITH_INFO */
		NoData          = 0x0002,			/**< Throw if SQL_NO_DATA */
		StillExecuting  = 0x0004,			/**< Throw if SQL_STILL_EXECUTING */
		NeedData        = 0x0008,			/**< Throw if SQL_NEED_DATA */
		Error           = 0x8000,			/**< Throw if SQL_ERROR */
		InvalidHandle   = 0x4000,			/**< Throw if SQL_INVALID_HANDLE */
		/** The mask of all valid return handling flags. */
		Mask            = None | SuccessWithInfo | NoData | StillExecuting | NeedData | Error | InvalidHandle,
		/** The default return handling flags. */
		Default         = Mask,
		/** SQL_SUCCESS_WITH_INFO is OK. */
		Ok_SWI          = Mask & (~SuccessWithInfo),
		/** SQL_NO_DATA is OK. */
		Ok_NoData       = Mask & (~NoData),
		/** SQL_SUCCESS_WITH_INFO or SQL_NO_DATA is OK. */
		Ok_SWI_or_NoData= Mask & (~(SuccessWithInfo | NoData))
	};

	static Flags SqlReturnToFlags(SQLRETURN odbc_status);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Typedefs for 32-bit attribute values...
// ////////////////////////////////////////////////////////////////////////////
typedef UINT32 OdbcAttrFlags32;
typedef HWND   OdbcAttrWindowHandle;
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER OdbcDiagRec {
	OdbcDiagRec();
	OdbcDiagRec(SQLSMALLINT handle_type, SQLHANDLE handle_value,
		SQLSMALLINT record_number = 1);
	virtual ~OdbcDiagRec();

	OdbcDiagRec(const OdbcDiagRec &other);
	OdbcDiagRec & operator = (const OdbcDiagRec &other);

	void swap(OdbcDiagRec &other);

	virtual bool LoadDiagnosticRecord(SQLSMALLINT handle_type,
		SQLHANDLE handle_value, SQLSMALLINT record_number = 1);

	SQLSMALLINT handle_type_;		/**< SQLGetDiagRec() HandleType */
	SQLHANDLE   handle_value_;		/**< SQLGetDiagRec() Handle */
	SQLSMALLINT record_number_;	/**< SQLGetDiagRec() RecNumber */
	std::string sql_state_;			/**< SQLGetDiagRec() SqlState */
	SQLINTEGER  native_error_;		/**< SQLGetDiagRec() NativeErrorPtr */
	std::string message_text_;		/**< SQLGetDiagRec() MessageText */
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER OdbcDiagRecEx : public OdbcDiagRec {
	OdbcDiagRecEx();
	OdbcDiagRecEx(SQLSMALLINT handle_type, SQLHANDLE handle_value,
		SQLSMALLINT record_number = 1, bool throw_on_diag_field_error = true);
	virtual ~OdbcDiagRecEx();

	OdbcDiagRecEx(const OdbcDiagRecEx &other);
	OdbcDiagRecEx & operator = (const OdbcDiagRecEx &other);

	void swap(OdbcDiagRecEx &other);

	virtual bool LoadDiagnosticRecord(SQLSMALLINT handle_type,
		SQLHANDLE handle_value, SQLSMALLINT record_number = 1);

	std::string class_origin_;		/**< SQLGetDiagField() SQL_DIAG_CLASS_ORIGIN */
	SQLINTEGER  column_number_;	/**< SQLGetDiagField() SQL_DIAG_COLUMN_NUMBER */
	std::string connection_name_;	/**< SQLGetDiagField() SQL_DIAG_CONNECTION_NAME */
	std::string message_text_ex_;	/**< SQLGetDiagField() SQL_DIAG_MESSAGE_TEXT */
	SQLINTEGER  native_;				/**< SQLGetDiagField() SQL_DIAG_NATIVE */
	SQLINTEGER  row_number_;		/**< SQLGetDiagField() SQL_DIAG_ROW_NUMBER */
	std::string server_name_;		/**< SQLGetDiagField() SQL_DIAG_SERVER_NAME */
	std::string sql_state_ex_;		/**< SQLGetDiagField() SQL_DIAG_SQLSTATE */
	std::string subclass_origin_;	/**< SQLGetDiagField() SQL_DIAG_SUBCLASS_ORIGIN */
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::vector<OdbcDiagRecEx>        OdbcDiagRecExList;
typedef OdbcDiagRecExList::iterator       OdbcDiagRecExListIter;
typedef OdbcDiagRecExList::const_iterator OdbcDiagRecExListIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER OdbcDiagState {
	OdbcDiagState();
	OdbcDiagState(SQLSMALLINT handle_type, SQLHANDLE handle_value,
		bool throw_on_diag_field_error = true);

	std::size_t LoadDiagnosticRecordList(SQLSMALLINT handle_type,
		SQLHANDLE handle_value, bool throw_on_diag_field_error = true);

	/** SQLGetDiagField() SQL_DIAG_CURSOR_ROW_COUNT */
	SQLINTEGER        cursor_row_count_;
	/** SQLGetDiagField() SQL_DIAG_DYNAMIC_FUNCTION */
	std::string       dynamic_function_;
	/** SQLGetDiagField() SQL_DIAG_DYNAMIC_FUNCTION_CODE */
	SQLINTEGER        dynamic_function_code_;
	/** SQLGetDiagField() SQL_DIAG_NUMBER */
	SQLINTEGER        number_;
	/** SQLGetDiagField() SQL_DIAG_RETURNCODE */
	SQLRETURN         returncode_;
	/** SQLGetDiagField() SQL_DIAG_ROW_COUNT */
	SQLINTEGER        row_count_;
#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	OdbcDiagRecExList diag_record_list_;
#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_ODBCWRAPPER) OdbcException :
	public MLB::Utility::ExceptionGeneral {
public:
	OdbcException();
	OdbcException(const char *except_string);
	OdbcException(const std::string &except_string);
	OdbcException(const std::ostringstream &except_string);
	~OdbcException();

	virtual void Rethrow(const char *except_string = NULL) const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_ODBCWRAPPER) OdbcExceptionStatus :
	public OdbcException {
public:
	//	Default constructor.
	OdbcExceptionStatus();
	//	String-only constructors...
	OdbcExceptionStatus(const char *except_string);
	OdbcExceptionStatus(const std::string &except_string);
	OdbcExceptionStatus(const std::ostringstream &except_string);
	//	C++ SQLRETURN - based constructors...
	OdbcExceptionStatus(SQLRETURN odbc_status);
	OdbcExceptionStatus(SQLRETURN odbc_status,
		const char *except_string, const char *function_args = NULL);
	OdbcExceptionStatus(SQLRETURN odbc_status,
		const std::string &except_string, const char *function_args = NULL);
	OdbcExceptionStatus(SQLRETURN odbc_status,
		const std::ostringstream &except_string,
		const char *function_args = NULL);
	~OdbcExceptionStatus();

	SQLRETURN   GetStatus() const;
	std::string GetFunctionArgs() const;

	virtual void Rethrow(const char *except_string = NULL) const;

	static std::string GetStatusString(SQLRETURN odbc_status,
		const char *other_text = NULL);

protected:
	SQLRETURN   odbc_status_;
	std::string function_args_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_ODBCWRAPPER) OdbcExceptionDiag :
	public virtual OdbcExceptionStatus {
public:
	//	Default constructor.
	OdbcExceptionDiag();
	//	Constructors without added text.
	OdbcExceptionDiag(SQLSMALLINT handle_type, SQLHANDLE handle_value);
	OdbcExceptionDiag(SQLRETURN odbc_status, SQLSMALLINT handle_type,
		SQLHANDLE handle_value);
	//	Constructors with added text...
	OdbcExceptionDiag(SQLRETURN odbc_status, SQLSMALLINT handle_type,
		SQLHANDLE handle_value, const char *except_string,
		const char *function_args = NULL);
	OdbcExceptionDiag(SQLRETURN odbc_status, SQLSMALLINT handle_type,
		SQLHANDLE handle_value, const std::string &except_string,
		const char *function_args = NULL);
	OdbcExceptionDiag(SQLRETURN odbc_status, SQLSMALLINT handle_type,
		SQLHANDLE handle_value, const std::ostringstream &except_string,
		const char *function_args = NULL);
	~OdbcExceptionDiag();

	      OdbcDiagState  GetDiagState() const;
	const OdbcDiagState &GetDiagStateRef() const;

	virtual void Rethrow(const char *except_string = NULL) const;

protected:
	OdbcDiagState diagnostic_state_;

	void FixupWhat();
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_ODBCWRAPPER SQLRETURN ThrowOdbcStatusProvisional(
	OdbcThrowFlags::Flags throw_flags, SQLRETURN odbc_status,
	SQLSMALLINT handle_type, SQLHANDLE handle_value,
	const char *function_name = NULL, const char *function_args = NULL,
	const char *other_text = NULL);
API_ODBCWRAPPER SQLRETURN ThrowOdbcDiagProvisional(
	OdbcThrowFlags::Flags throw_flags, SQLRETURN odbc_status,
	SQLSMALLINT handle_type, SQLHANDLE handle_value,
	const char *function_name = NULL, const char *function_args = NULL,
	const char *other_text = NULL);

API_ODBCWRAPPER void      ThrowOdbcStatus(SQLRETURN odbc_status,
	const char *error_text);
API_ODBCWRAPPER void      ThrowOdbcStatus(SQLRETURN odbc_status,
	const std::string &error_text);
API_ODBCWRAPPER void      ThrowOdbcStatus(SQLRETURN odbc_status,
	const std::ostringstream &error_text);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER GetInfoTypeDatumRaw {
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
struct API_ODBCWRAPPER GetInfoTypeDatumValue {
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
struct API_ODBCWRAPPER GetInfoTypeDatum : public GetInfoTypeDatumValue {
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
typedef std::set<GetInfoTypeDatum>          GetInfoTypeDatumSet;
typedef GetInfoTypeDatumSet::const_iterator GetInfoTypeDatumSetIterC;
typedef GetInfoTypeDatumSet::iterator       GetInfoTypeDatumSetIter;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_ODBCWRAPPER GetInfoTypeDatumList {
public:
	GetInfoTypeDatumList();
	virtual ~GetInfoTypeDatumList();

	GetInfoTypeDatumList &swap(GetInfoTypeDatumList &other);

	GetInfoTypeDatumSetIter  Begin();
	GetInfoTypeDatumSetIterC Begin() const;
	GetInfoTypeDatumSetIter  End();
	GetInfoTypeDatumSetIterC End() const;
	GetInfoTypeDatumSetIter  Find(SQLUSMALLINT info_type);
	GetInfoTypeDatumSetIterC Find(SQLUSMALLINT info_type) const;

	bool              IsInSet(SQLUSMALLINT info_type) const;
	bool              CheckIsInSet(SQLUSMALLINT info_type) const;
	GetInfoTypeDatum &Get(SQLUSMALLINT info_type,
		GetInfoTypeDatum &out_datum) const;
	GetInfoTypeDatum  Get(SQLUSMALLINT info_type) const;
	void              InsertOrUpdate(const GetInfoTypeDatum &new_item);
	void              GetInfoData(SQLHDBC ConnectionHandle);

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	GetInfoTypeDatumSet descriptor_set_;
#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

	static GetInfoTypeDatumList &Create(SQLHDBC ConnectionHandle,
		GetInfoTypeDatumList &out_datum);

	static GetInfoTypeDatumList Create(SQLHDBC ConnectionHandle);

protected:
	void ThrowNotLocated(SQLUSMALLINT info_type) const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename LengthType>
	LengthType CheckStringArgInLengthDomain(const std::string &arg_string,
	const char *arg_name, const char *func_name)
{
	std::string::size_type arg_length = arg_string.size();

	if (arg_length > static_cast<std::string::size_type>(
		std::numeric_limits<LengthType>::max()))
		throw OdbcException("The length of the std::string argument '" +
			std::string(arg_name) + "' to function '" + std::string(func_name) +
			"' (" + MLB::Utility::AnyToString(arg_length) + ") exceeds "
			"the maximum for an argument of the specified length type (" +
			MLB::Utility::AnyToString(std::numeric_limits<LengthType>::max()) +
			").");

	return(static_cast<LengthType>(arg_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename LengthType>
	LengthType CheckStringArgInLengthDomain(const char *arg_string,
	const char *arg_name, const char *func_name)
{
	if (arg_string == NULL)
		return(0);

	std::size_t arg_length = strlen(arg_string);

	if (arg_length > static_cast<std::size_t>(
		std::numeric_limits<LengthType>::max()))
		throw OdbcException("The length of the char * argument '" +
			std::string(arg_name) + "' to function '" + std::string(func_name) +
			"' (" + MLB::Utility::AnyToString(arg_length) + ") exceeds "
			"the maximum for an argument of the specified length type (" +
			MLB::Utility::AnyToString(std::numeric_limits<LengthType>::max()) +
			").");

	return(static_cast<LengthType>(arg_length));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//
//		For example, ::SQLAllocHandle() should be invoked as:
//
//		OdbcThrowFlags::Flags tf = OdbcThrowFlags::Default;
//		SQLSMALLINT           ht = SQL_HANDLE_ENV,
//		SQLHANDLE             hv = SQL_NULL_HANDLE;
//		SQLHANDLE             oh;
//		OdbcWrapper_THROW_STATUS_IF_LEVEL_0(tf, ht, hv, ::SQLAllocHandle,
//			(ht, hv, &oh));
#define OdbcWrapper_THROW_STATUS_IF_LEVEL_0(throw_flags, h_type, h_val,		\
		f_name, f_args, other_text)														\
	ThrowOdbcDiagProvisional(throw_flags, (f_name) f_args , h_type, h_val,	\
		#f_name, #f_args, other_text)
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//
//		For example, ::SQLAllocHandle() should be invoked as:
//
//		OdbcThrowFlags::Flags tf = OdbcThrowFlags::Default;
//		SQLSMALLINT           ht = SQL_HANDLE_ENV,
//		SQLHANDLE             hv = SQL_NULL_HANDLE;
//		SQLHANDLE             oh;
//		OdbcWrapper_THROW_STATUS_IF_LEVEL_0(tf, ht, hv, ::SQLAllocHandle,
//			(ht, hv, &oh));
#define OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags, h_type, h_val,		\
		f_name, f_args)																		\
	OdbcWrapper_THROW_STATUS_IF_LEVEL_0(throw_flags, h_type, h_val, f_name,	\
		f_args, NULL)
//	////////////////////////////////////////////////////////////////////////////

/*
//	////////////////////////////////////////////////////////////////////////////
//
//		For example, should be invoked as:
//
//		SQLSMALLINT ht = SQL_HANDLE_ENV,
//		SQLHANDLE   ih = SQL_NULL_HANDLE;
//		SQLHANDLE   oh;
//		OdbcWrapper_THROW_STATUS_IF(::SQLAllocHandle, (ht, ih, &oh));
#define OdbcWrapper_THROW_STATUS_IF(f_name, f_args)	\
	OdbcWrapper_THROW_STATUS_IF_LEVEL_1(false, (f_name), f_args)
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//
//		For example, should be invoked as:
//
//		SQLSMALLINT ht = SQL_HANDLE_ENV,
//		SQLHANDLE   ih = SQL_NULL_HANDLE;
//		SQLHANDLE   oh;
//		OdbcWrapper_THROW_STATUS_IF_WITH_INFO(::SQLAllocHandle, (ht, ih, &oh));
#define OdbcWrapper_THROW_STATUS_IF_WITH_INFO(f_name, f_args)	\
	OdbcWrapper_THROW_STATUS_IF_LEVEL_1(true, (f_name), f_args)
//	////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
	//	Used with SQL_API_ALL_FUNCTIONS...
typedef SQLUSMALLINT GetFunctions_ApiAll[100];

	//	Used with SQL_API_ODBC3_ALL_FUNCTIONS...
typedef SQLSMALLINT  GetFunctions_ApiAll_ODBC3[SQL_API_ODBC3_ALL_FUNCTIONS_SIZE];
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef SQLRETURN (*HandleGetAttrFunc)(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr, OdbcThrowFlags::Flags throw_flags);
typedef SQLRETURN (*HandleSetAttrFunc)(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Basic wrappers around the ODBC API functions...
// ////////////////////////////////////////////////////////////////////////////
	//	Wrapper around the ODBC function 'SQLAllocHandle()'...
API_ODBCWRAPPER SQLRETURN AllocHandle(SQLSMALLINT HandleType,
	SQLHANDLE InputHandle, SQLHANDLE *OutputHandlePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);

	//	Wrapper around the ODBC function 'SQLBindCol()'...
API_ODBCWRAPPER SQLRETURN BindCol(SQLHSTMT StatementHandle,
	SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr,
	SQLINTEGER BufferLength, SQLINTEGER *StrLen_or_IndPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	//	Wrapper around the ODBC function 'SQLCancel()'...
API_ODBCWRAPPER SQLRETURN Cancel(SQLHSTMT StatementHandle,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);

	//	Wrapper around the ODBC function 'SQLColAttribute()'...
API_ODBCWRAPPER SQLRETURN ColAttribute(SQLHSTMT StatementHandle,
	SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier,
	SQLPOINTER CharacterAttributePtr, SQLSMALLINT BufferLength,
	SQLSMALLINT *StringLengthPtr, SQLPOINTER NumericAttributePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN ColAttribute(SQLHSTMT StatementHandle,
	SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier,
	std::string &CharacterAttributePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN ColAttribute(SQLHSTMT StatementHandle,
	SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier,
	SQLINTEGER &NumericAttributePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN ColAttribute(SQLHDBC ConnectionHandle,
	SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier,
	bool &NumericAttributePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	//	Wrapper around the ODBC function 'SQLColumns()'...
API_ODBCWRAPPER SQLRETURN Columns(SQLHSTMT StatementHandle, SQLCHAR *CatalogName,
	SQLSMALLINT NameLength1, SQLCHAR *SchemaName, SQLSMALLINT NameLength2,
	SQLCHAR *TableName, SQLSMALLINT NameLength3, SQLCHAR *ColumnName,
	SQLSMALLINT NameLength4,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
API_ODBCWRAPPER SQLRETURN Columns(SQLHSTMT StatementHandle,
	const char *CatalogName = NULL, const char *SchemaName = NULL,
	const char *TableName = NULL, const char *ColumnName = "",
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);

	//	Wrapper around the ODBC function 'SQLDataSources()'...
API_ODBCWRAPPER SQLRETURN DataSources(SQLHENV EnvironmentHandle,
	SQLUSMALLINT Direction, SQLCHAR *ServerName,
	SQLSMALLINT BufferLength1, SQLSMALLINT *NameLength1Ptr,
	SQLCHAR *Description, SQLSMALLINT BufferLength2,
	SQLSMALLINT *NameLength2Ptr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);

	//	Wrapper around the ODBC function 'SQLDescribeCol()'...
API_ODBCWRAPPER SQLRETURN DescribeCol(SQLHSTMT StatementHandle,
	SQLSMALLINT ColumnNumber, SQLCHAR *ColumnName, SQLSMALLINT BufferLength,
	SQLSMALLINT *NameLengthPtr, SQLSMALLINT *DataTypePtr,
	SQLUINTEGER *ColumnSizePtr, SQLSMALLINT *DecimalDigitsPtr,
	SQLSMALLINT *NullablePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN DescribeCol(SQLHSTMT StatementHandle,
	SQLSMALLINT ColumnNumber, std::string &ColumnName, SQLSMALLINT *DataTypePtr,
	SQLUINTEGER *ColumnSizePtr, SQLSMALLINT *DecimalDigitsPtr,
	SQLSMALLINT *NullablePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	//	Wrapper around the ODBC function 'SQLDisconnect()'...
API_ODBCWRAPPER SQLRETURN Disconnect(SQLHDBC ConnectionHandle,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	//	Wrapper around the ODBC function 'SQLDriverConnect()'...
API_ODBCWRAPPER SQLRETURN DriverConnect(SQLHDBC ConnectionHandle,
	SQLHWND WindowHandle, SQLCHAR *InConnectionString,
	SQLSMALLINT StringLength1, SQLCHAR *OutConnectionString,
	SQLSMALLINT BufferLength, SQLSMALLINT *StringLength2Ptr,
	SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN DriverConnect(SQLHDBC ConnectionHandle,
	SQLHWND WindowHandle, const std::string &InConnectionString,
	std::string &OutConnectionString,
	SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN DriverConnect(SQLHDBC ConnectionHandle,
	SQLHWND WindowHandle, const std::string &InConnectionString,
	SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN DriverConnect(SQLHDBC ConnectionHandle,
	SQLCHAR *InConnectionString, SQLSMALLINT StringLength1,
	SQLCHAR *OutConnectionString, SQLSMALLINT BufferLength,
	SQLSMALLINT *StringLength2Ptr,
	SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN DriverConnect(SQLHDBC ConnectionHandle,
	const std::string &InConnectionString, std::string &OutConnectionString,
	SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN DriverConnect(SQLHDBC ConnectionHandle,
	const std::string &InConnectionString,
	SQLUSMALLINT DriverCompletion = SQL_DRIVER_COMPLETE,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	//	Wrapper around the ODBC function 'SQLDrivers()'...
API_ODBCWRAPPER SQLRETURN Drivers(SQLHENV EnvironmentHandle,
	SQLUSMALLINT Direction, SQLCHAR *DriverDescription,
	SQLSMALLINT BufferLength1, SQLSMALLINT *DescriptionLengthPtr,
	SQLCHAR *DriverAttributes, SQLSMALLINT BufferLength2,
	SQLSMALLINT *AttributesLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);

	//	Wrapper around the ODBC function 'SQLEndTran()'...
API_ODBCWRAPPER SQLRETURN EndTran(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT CompletionType,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
API_ODBCWRAPPER SQLRETURN EndTranCommit(SQLSMALLINT HandleType,
	SQLHANDLE Handle,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
API_ODBCWRAPPER SQLRETURN EndTranRollback(SQLSMALLINT HandleType,
	SQLHANDLE Handle,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);

	//	Wrapper around the ODBC function 'SQLExtendedFetch()'...
API_ODBCWRAPPER SQLRETURN ExtendedFetch(SQLHSTMT StatementHandle,
	SQLUSMALLINT FetchOrientation, SQLINTEGER FetchOffset,
	SQLUINTEGER *RowCountPtr, SQLUSMALLINT *RowStatusArray,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	//	Wrapper around the ODBC function 'SQLExecDirect()'...
API_ODBCWRAPPER SQLRETURN ExecDirect(SQLHSTMT StatementHandle,
	SQLCHAR *StatementText, SQLINTEGER TextLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN ExecDirect(SQLHSTMT StatementHandle,
	const std::string &StatementText,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);

	//	Wrapper around the ODBC function 'SQLFetch()'...
API_ODBCWRAPPER SQLRETURN Fetch(SQLHSTMT StatementHandle,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);

	//	Wrapper around the ODBC function 'SQLFetchScroll()'...
API_ODBCWRAPPER SQLRETURN FetchScroll(SQLHSTMT StatementHandle,
	SQLSMALLINT FetchOrientation, SQLINTEGER FetchOffset,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	//	Wrapper around the ODBC function 'SQLFreeHandle()'...
API_ODBCWRAPPER SQLRETURN FreeHandle(SQLSMALLINT HandleType,
	SQLHANDLE Handle,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);

	//	Wrapper around the ODBC function 'SQLGetConnectAttr()'...
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLINTEGER &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLUINTEGER &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLSMALLINT &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLUSMALLINT &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, OdbcAttrFlags32 &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, OdbcAttrWindowHandle &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLINTEGER *ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLUINTEGER *ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
*/
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, char *ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, std::string &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
	//	Disambiguation function for ODBC function 'SQLGetConnectAttr()'...
API_ODBCWRAPPER SQLRETURN GetConnectAttrBasic(SQLHDBC Handle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags);

	//	Wrapper around the ODBC function 'SQLGetData()'...
API_ODBCWRAPPER SQLRETURN GetData(SQLHSTMT StatementHandle,
	SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr,
	SQLINTEGER BufferLength, SQLINTEGER *StrLen_or_IndPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);

	//	Wrapper around the ODBC function 'SQLGetDiagField()'...
API_ODBCWRAPPER SQLRETURN GetDiagField(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier, SQLPOINTER DiagInfoPtr,
	SQLSMALLINT BufferLength, SQLSMALLINT *StringLengthPtr);
API_ODBCWRAPPER SQLRETURN GetDiagField(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier, std::string &DiagInfoPtr,
	bool throw_flag = true);
API_ODBCWRAPPER SQLRETURN GetDiagField_String(SQLSMALLINT HandleType,
	SQLHANDLE Handle, SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
	std::string &DiagInfoPtr, bool throw_flag = true);
API_ODBCWRAPPER SQLRETURN GetDiagField_SQLINTEGER(SQLSMALLINT HandleType,
	SQLHANDLE Handle, SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
	SQLINTEGER &DiagInfoPtr, bool throw_flag = true);
API_ODBCWRAPPER SQLRETURN GetDiagField_SQLRETURN(SQLSMALLINT HandleType,
	SQLHANDLE Handle, SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
	SQLRETURN &DiagInfoPtr, bool throw_flag = true);

	//	Wrapper around the ODBC function 'SQLGetDiagRec()'...
API_ODBCWRAPPER SQLRETURN GetDiagRec(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, SQLCHAR *Sqlstate, SQLINTEGER *NativeErrorPtr,
	SQLCHAR *MessageText, SQLSMALLINT BufferLength, SQLSMALLINT *TextLengthPtr);
API_ODBCWRAPPER SQLRETURN GetDiagRec(SQLSMALLINT HandleType, SQLHANDLE Handle,
	SQLSMALLINT RecNumber, std::string &Sqlstate, SQLINTEGER *NativeErrorPtr,
	std::string &MessageText, bool throw_flag = true);

	//	Wrapper around the ODBC function 'SQLGetEnvAttr()'...
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLINTEGER &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLUINTEGER &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLSMALLINT &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLUSMALLINT &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, OdbcAttrFlags32 &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, OdbcAttrWindowHandle &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLINTEGER *ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLUINTEGER *ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
*/
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, char *ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, std::string &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
	//	Disambiguation function for ODBC function 'SQLGetEnvAttr()'...
API_ODBCWRAPPER SQLRETURN GetEnvAttrBasic(SQLHENV Handle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags);

	//	Wrapper around the ODBC function 'SQLGetFunctions()'...
API_ODBCWRAPPER SQLRETURN GetFunctions(SQLHDBC ConnectionHandle,
	SQLUSMALLINT FunctionId, SQLUSMALLINT *SupportedPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
API_ODBCWRAPPER SQLRETURN GetFunctions(SQLHDBC ConnectionHandle,
	SQLUSMALLINT FunctionId, SQLUSMALLINT &SupportedPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
API_ODBCWRAPPER SQLRETURN GetFunctions(SQLHDBC ConnectionHandle,
	GetFunctions_ApiAll &SupportedPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
API_ODBCWRAPPER SQLRETURN GetFunctions(SQLHDBC ConnectionHandle,
	GetFunctions_ApiAll_ODBC3 &SupportedPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);

	//	Wrapper around the ODBC function 'SQLGetInfo()'...
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	SQLUSMALLINT InfoType, SQLPOINTER InfoValuePtr, SQLSMALLINT BufferLength,
	SQLSMALLINT *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	SQLUSMALLINT InfoType, std::string &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	SQLUSMALLINT InfoType, SQLUSMALLINT &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
API_ODBCWRAPPER SQLRETURN GetInfo(SQLHDBC ConnectionHandle,
	SQLUSMALLINT InfoType, SQLUINTEGER &InfoValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI);
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

	//	Wrapper around the ODBC function 'SQLGetStmtAttr()'...
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLINTEGER &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLUINTEGER &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLSMALLINT &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLUSMALLINT &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, OdbcAttrFlags32 &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, OdbcAttrWindowHandle &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLINTEGER *ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLUINTEGER *ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
*/
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, char *ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, std::string &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
	//	Disambiguation function for ODBC function 'SQLGetStmtAttr()'...
API_ODBCWRAPPER SQLRETURN GetStmtAttrBasic(SQLHSTMT Handle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr, OdbcThrowFlags::Flags throw_flags);

	//	Wrapper around the ODBC function 'SQLMoreResults()'...
API_ODBCWRAPPER SQLRETURN MoreResults(SQLHSTMT StatementHandle,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);

	//	Wrapper around the ODBC function 'SQLNumResultCols()' functions...
API_ODBCWRAPPER SQLRETURN NumResultCols(SQLHSTMT StatementHandle,
	SQLSMALLINT *ColumnCountPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);

	//	Wrapper around the ODBC function 'SQLRowCount()'...
API_ODBCWRAPPER SQLRETURN RowCount(SQLHSTMT StatementHandle,
	SQLINTEGER *RowCountPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN RowCount(SQLHSTMT StatementHandle,
	SQLINTEGER &RowCountPtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);

	//	Wrapper around the ODBC function 'SQLSetConnectAttr()'...
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLINTEGER ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLUINTEGER ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLSMALLINT ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLUSMALLINT ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, OdbcAttrFlags32 ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, OdbcAttrWindowHandle ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, const char *ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetConnectAttr(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, const std::string &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
	//	Disambiguation function for ODBC function 'SQLSetConnectAttr()'...
API_ODBCWRAPPER SQLRETURN SetConnectAttrBasic(SQLHDBC ConnectionHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);

	//	Wrapper around the ODBC function 'SQLSetEnvAttr()'...
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLINTEGER ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLUINTEGER ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLSMALLINT ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLUSMALLINT ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, OdbcAttrFlags32 ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, OdbcAttrWindowHandle ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, const char *ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetEnvAttr(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, const std::string &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
	//	Disambiguation function for ODBC function 'SQLSetEnvAttr()'...
API_ODBCWRAPPER SQLRETURN SetEnvAttrBasic(SQLHENV EnvironmentHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);

	//	Wrapper around the ODBC function 'SQLSetStmtAttr()'...
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLINTEGER ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLUINTEGER ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLSMALLINT ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLUSMALLINT ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, OdbcAttrFlags32 ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, OdbcAttrWindowHandle ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, const char *ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetStmtAttr(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, const std::string &ValuePtr,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
	//	Disambiguation function for ODBC function 'SQLSetStmtAttr()'...
API_ODBCWRAPPER SQLRETURN SetStmtAttrBasic(SQLHSTMT StatementHandle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);

	//	Wrapper around the ODBC function 'SQLTables()'...
API_ODBCWRAPPER SQLRETURN Tables(SQLHSTMT StatementHandle, SQLCHAR *CatalogName,
	SQLSMALLINT NameLength1, SQLCHAR *SchemaName, SQLSMALLINT NameLength2,
	SQLCHAR *TableName, SQLSMALLINT NameLength3, SQLCHAR *TableType,
	SQLSMALLINT NameLength4,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
API_ODBCWRAPPER SQLRETURN Tables(SQLHSTMT StatementHandle,
	const char *CatalogName = NULL, const char *SchemaName = NULL,
	const char *TableName = NULL, const char *TableType = "TABLE",
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Supporting function for all ODBC 'SQLGet*Attr()' functions...
// ////////////////////////////////////////////////////////////////////////////
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLINTEGER &ValuePtr,
	HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLUINTEGER &ValuePtr,
	HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLSMALLINT &ValuePtr,
	HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLUSMALLINT &ValuePtr,
	HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, OdbcAttrFlags32 &ValuePtr,
	HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, OdbcAttrWindowHandle &ValuePtr,
	HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLINTEGER *ValuePtr,
	HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLUINTEGER *ValuePtr,
	HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
*/
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, char *ValuePtr, SQLINTEGER BufferLength,
	SQLINTEGER *StringLengthPtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN GetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, std::string &ValuePtr, const char *odbc_function_name,
	HandleGetAttrFunc get_attr_function, OdbcThrowFlags::Flags throw_flags);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Supporting function for all ODBC 'SQLSet*Attr()' functions...
// ////////////////////////////////////////////////////////////////////////////
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER StringLength,
	HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLINTEGER ValuePtr,
	HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLUINTEGER ValuePtr,
	HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLSMALLINT ValuePtr,
	HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, SQLUSMALLINT ValuePtr,
	HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, OdbcAttrFlags32 ValuePtr,
	HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, OdbcAttrWindowHandle ValuePtr,
	HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, const char *ValuePtr, SQLINTEGER StringLength,
	HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
API_ODBCWRAPPER SQLRETURN SetAttrForHandle(SQLHANDLE Handle,
	SQLINTEGER Attribute, const std::string &ValuePtr,
	const char *odbc_function_name, HandleSetAttrFunc set_attr_function,
	OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Ok_SWI_or_NoData);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Supporting functions for use of the various '::SQLGetDiagField()' values...
// ////////////////////////////////////////////////////////////////////////////
API_ODBCWRAPPER SQLSMALLINT GetDiagRecordNumberCount(SQLSMALLINT handle_type,
	SQLHANDLE handle_value);
API_ODBCWRAPPER SQLSMALLINT CheckIsValidDiagRecordNumber(
	SQLSMALLINT handle_type, SQLHANDLE handle_value, SQLSMALLINT record_number);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef SQLRETURN (*GetHandleAttrRowFunc)(SQLHENV EnvironmentHandle,
	SQLUSMALLINT Direction, SQLCHAR *DriverDescription,
	SQLSMALLINT BufferLength1, SQLSMALLINT *DescriptionLengthPtr,
	SQLCHAR *DriverAttributes, SQLSMALLINT BufferLength2,
	SQLSMALLINT *AttributesLengthPtr, OdbcThrowFlags::Flags throw_flags);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcTransBlock {
	OdbcTransBlock(SQLSMALLINT handle_type, SQLHANDLE handle_value);
	virtual ~OdbcTransBlock();

	bool IsTransactionOpen() const;
	bool CheckIsTransactionOpen() const;
	void TransactionCommit();
	void TransactionRollback();

private:
	SQLSMALLINT handle_type_;
	SQLHANDLE   handle_value_;
	bool        trans_is_open_;

	MLB_Utility_NonCopyable_Macro(OdbcTransBlock);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcTransBlockManualBase {
protected:
	OdbcTransBlockManualBase(SQLHANDLE dbc_handle, bool strict_flag);
	virtual ~OdbcTransBlockManualBase();

	SQLHANDLE dbc_handle_;
	bool      set_on_in_dtor_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcTransBlockManual :
	public OdbcTransBlockManualBase, public OdbcTransBlock {
	OdbcTransBlockManual(SQLHANDLE dbc_handle, bool strict_flag = false)
		:OdbcTransBlockManualBase(dbc_handle, strict_flag)
		,OdbcTransBlock(SQL_HANDLE_DBC, dbc_handle)
	{
	}

private:
	MLB_Utility_NonCopyable_Macro(OdbcTransBlockManual);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER AttrInfoBasic {
	explicit AttrInfoBasic(const SQLCHAR *description_ptr = NULL,
		SQLSMALLINT description_length = 0,
		const SQLCHAR *attributes_list_ptr = NULL,
		SQLSMALLINT attributes_list_length = 0);

	std::string              description_;
#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	std::vector<std::string> attributes_list_;
#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

	static std::vector<AttrInfoBasic> &AttrInfoBasic::GetEnvAttrList(
		const char *get_attr_row_func_name,
		GetHandleAttrRowFunc get_attr_row_func_ptr, SQLHENV EnvironmentHandle,
		std::vector<AttrInfoBasic> &out_list,
		SQLUSMALLINT Direction = SQL_FETCH_FIRST);

	static std::vector<AttrInfoBasic> &GetAttrInfoBasicList(
		SQLHENV EnvironmentHandle, std::vector<AttrInfoBasic> &out_list,
		SQLUSMALLINT Direction = SQL_FETCH_FIRST);
	static std::vector<AttrInfoBasic>  GetAttrInfoBasicList(
		SQLHENV EnvironmentHandle, SQLUSMALLINT Direction = SQL_FETCH_FIRST);

	static std::vector<AttrInfoBasic> &GetDataSourceBasicList(
		SQLHENV EnvironmentHandle, std::vector<AttrInfoBasic> &out_list,
		SQLUSMALLINT Direction = SQL_FETCH_FIRST);
	static std::vector<AttrInfoBasic>  GetDataSourceBasicList(
		SQLHENV EnvironmentHandle, SQLUSMALLINT Direction = SQL_FETCH_FIRST);
};
API_ODBCWRAPPER std::ostream & operator << (std::ostream &o_str,
	const AttrInfoBasic &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<AttrInfoBasic>          AttrInfoBasicVector;
typedef AttrInfoBasicVector::iterator       AttrInfoBasicVectorIter;
typedef AttrInfoBasicVector::const_iterator AttrInfoBasicVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER AttrDatum {
	explicit AttrDatum(const char *attr_string = NULL);
	//	Really used just for perform finds...
	AttrDatum(bool /* disambiguation */, const char *attr_name);

	bool operator < (const AttrDatum &other) const;

	std::string attr_name_;
	std::string attr_value_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<AttrDatum>          AttrDatumSet;
typedef AttrDatumSet::iterator       AttrDatumSetIter;
typedef AttrDatumSet::const_iterator AttrDatumSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER AttrInfo {
	         AttrInfo();
	explicit AttrInfo(const AttrInfoBasic &driver_info_basic);

	std::string  description_;
#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	AttrDatumSet attributes_set_;
#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

	static std::vector<AttrInfo> &GetAttrInfoList(
		SQLHENV EnvironmentHandle, std::vector<AttrInfo> &out_list,
		SQLUSMALLINT Direction = SQL_FETCH_FIRST);
	static std::vector<AttrInfo> GetAttrInfoList(
		SQLHENV EnvironmentHandle, SQLUSMALLINT Direction = SQL_FETCH_FIRST);

	static std::vector<AttrInfo> &GetDataSourcesList(
		SQLHENV EnvironmentHandle, std::vector<AttrInfo> &out_list,
		SQLUSMALLINT Direction = SQL_FETCH_FIRST);
	static std::vector<AttrInfo>  GetDataSourcesList(
		SQLHENV EnvironmentHandle, SQLUSMALLINT Direction = SQL_FETCH_FIRST);
};
API_ODBCWRAPPER std::ostream & operator << (std::ostream &o_str,
	const AttrInfo &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<AttrInfo>          AttrInfoVector;
typedef AttrInfoVector::iterator       AttrInfoVectorIter;
typedef AttrInfoVector::const_iterator AttrInfoVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER DriverConnectParams {
	explicit DriverConnectParams(const std::string &in_uid = "sa",
		const std::string &in_pwd = "", const std::string &in_database = "master",
		const std::string &in_server = "(local)",
		const std::string &in_driver = "{SQL Native Client}");

	std::string &GetConnectionString(std::string &out_string) const;
	std::string  GetConnectionString() const;

	unsigned int ParseDriverParams(unsigned int start_index,
		int argc, char **argv, bool throw_if_not_matched_flag = true);

	std::string uid_;
	std::string pwd_;
	std::string database_;
	std::string server_;
	std::string driver_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER ColumnAttrSimple {
	ColumnAttrSimple();
	ColumnAttrSimple(SQLHSTMT statement_handle, SQLSMALLINT column_number);

	void swap(ColumnAttrSimple &other);

	bool operator < (const ColumnAttrSimple &other) const;

	//	Assumes 'nullability_' is in domain...
	bool NullabilityIsKnown() const;
	//	Assumes 'nullability_' is in domain...
	bool IsNullable() const;

	void LoadColumnAttr(SQLHSTMT statement_handle,
		SQLSMALLINT column_number);

	std::string ToStringTest() const;

	SQLSMALLINT column_number_;
	std::string column_name_;
	SQLSMALLINT data_type_;
	SQLUINTEGER column_size_;
	SQLSMALLINT decimal_digits_;
	SQLSMALLINT nullability_;

	static std::vector<ColumnAttrSimple> &GetColumnAttrList(
		SQLHSTMT statement_handle, std::vector<ColumnAttrSimple> &out_list);
	static std::vector<ColumnAttrSimple>  GetColumnAttrList(
		SQLHSTMT statement_handle);
	static std::set<ColumnAttrSimple>    &GetColumnAttrSet(
		SQLHSTMT statement_handle, std::set<ColumnAttrSimple> &out_set);
};
API_ODBCWRAPPER std::ostream & operator << (std::ostream &o_str,
	const ColumnAttrSimple &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<ColumnAttrSimple>        ColumnAttrSimpleList;
typedef ColumnAttrSimpleList::iterator       ColumnAttrSimpleListIter;
typedef ColumnAttrSimpleList::const_iterator ColumnAttrSimpleListIterC;

typedef std::set<ColumnAttrSimple>           ColumnAttrSimpleSet;
typedef ColumnAttrSimpleSet::iterator        ColumnAttrSimpleSetIter;
typedef ColumnAttrSimpleSet::const_iterator  ColumnAttrSimpleSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_ODBCWRAPPER ColumnAttr {
	ColumnAttr();
	ColumnAttr(SQLHSTMT statement_handle, SQLSMALLINT column_number);

	void swap(ColumnAttr &other);

	bool operator < (const ColumnAttr &other) const;

	//	Assumes 'nullable_' is in domain...
	bool NullabilityIsKnown() const;
	//	Assumes 'nullable_' is in domain...
	bool IsNullable() const;

	void LoadColumnAttr(SQLHSTMT statement_handle,
		SQLSMALLINT column_number);

	std::string ToStringTest() const;

	SQLSMALLINT column_number_;
	bool        auto_unique_value_;		 	/**< SQLColAttribute().SQL_DESC_AUTO_UNIQUE_VALUE	*/
	std::string base_column_name_;		 	/**< SQLColAttribute().SQL_DESC_BASE_COLUMN_NAME	*/
	std::string base_table_name_;			 	/**< SQLColAttribute().SQL_DESC_BASE_TABLE_NAME		*/
	bool        case_sensitive_;			 	/**< SQLColAttribute().SQL_DESC_CASE_SENSITIVE		*/
	std::string catalog_name_;				 	/**< SQLColAttribute().SQL_DESC_CATALOG_NAME			*/
	SQLINTEGER  concise_type_;				 	/**< SQLColAttribute().SQL_DESC_CONCISE_TYPE			*/
	SQLINTEGER  count_;						 	/**< SQLColAttribute().SQL_DESC_COUNT					*/
	SQLINTEGER  display_size_;				 	/**< SQLColAttribute().SQL_DESC_DISPLAY_SIZE			*/
	bool        fixed_prec_scale_;		 	/**< SQLColAttribute().SQL_DESC_FIXED_PREC_SCALE	*/
	std::string label_;						 	/**< SQLColAttribute().SQL_DESC_LABEL					*/
	SQLINTEGER  length_;						 	/**< SQLColAttribute().SQL_DESC_LENGTH					*/
	std::string literal_prefix_;			 	/**< SQLColAttribute().SQL_DESC_LITERAL_PREFIX		*/
	std::string literal_suffix_;			 	/**< SQLColAttribute().SQL_DESC_LITERAL_SUFFIX		*/
	std::string local_type_name_;			 	/**< SQLColAttribute().SQL_DESC_LOCAL_TYPE_NAME		*/
	std::string name_;						 	/**< SQLColAttribute().SQL_DESC_NAME					*/
	SQLINTEGER  nullable_;					 	/**< SQLColAttribute().SQL_DESC_NULLABLE				*/
	bool        is_nullable_;				 	// True if nullable_ == SQL_ NULLABLE
	bool        is_nullable_known_;		 	//	True if nullable_ != SQL_NULLABLE_UNKNOWN
	SQLINTEGER  num_prec_radix_;			 	/**< SQLColAttribute().SQL_DESC_NUM_PREC_RADIX		*/
	SQLINTEGER  octet_length_;				 	/**< SQLColAttribute().SQL_DESC_OCTET_LENGTH			*/
	SQLINTEGER  precision_;					 	/**< SQLColAttribute().SQL_DESC_PRECISION				*/
	SQLINTEGER  scale_;						 	/**< SQLColAttribute().SQL_DESC_SCALE					*/
	std::string schema_name_;				 	/**< SQLColAttribute().SQL_DESC_SCHEMA_NAME			*/
	SQLINTEGER  searchable_;				 	/**< SQLColAttribute().SQL_DESC_SEARCHABLE			*/
	bool        is_searchable_;			 	//	True if searchable_ != SQL_PRED_NONE
	bool        is_searchable_like_only_;	//	True if searchable_ == SQL_PRED_CHAR
	bool        is_searchable_not_like_; 	//	True if searchable_ == SQL_PRED_BASIC
	bool        is_searchable_any_;		 	//	True if searchable_ == SQL_PRED_SEARCHABLE
	std::string table_name_;				 	/**< SQLColAttribute().SQL_DESC_TABLE_NAME			*/
	SQLINTEGER  type_;						 	/**< SQLColAttribute().SQL_DESC_TYPE					*/
	std::string type_name_;					 	/**< SQLColAttribute().SQL_DESC_TYPE_NAME				*/
	SQLINTEGER  unnamed_;					 	/**< SQLColAttribute().SQL_DESC_UNNAMED				*/
	bool        is_unnamed_;				 	//	True if unnamed_ = SQL_UNNAMED
	bool        unsigned_;					 	/**< SQLColAttribute().SQL_DESC_UNSIGNED				*/
	SQLINTEGER  updatable_;					 	/**< SQLColAttribute().SQL_DESC_UPDATABLE				*/
	bool        is_updatable_;				 	//	True if updatable_ == SQL_ATTR_WRITE
	bool        is_updatable_known_;		 	//	True if updatable_ != SQL_ATTR_READWRITE_UNKNOWN

	static std::vector<ColumnAttr> &GetColumnAttrList(
		SQLHSTMT statement_handle, std::vector<ColumnAttr> &out_list);
	static std::vector<ColumnAttr>  GetColumnAttrList(
		SQLHSTMT statement_handle);
	static std::set<ColumnAttr>    &GetColumnAttrSet(
		SQLHSTMT statement_handle, std::set<ColumnAttr> &out_set);
};
API_ODBCWRAPPER std::ostream & operator << (std::ostream &o_str,
	const ColumnAttr &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<ColumnAttr>        ColumnAttrList;
typedef ColumnAttrList::iterator       ColumnAttrListIter;
typedef ColumnAttrList::const_iterator ColumnAttrListIterC;

typedef std::set<ColumnAttr>           ColumnAttrSet;
typedef ColumnAttrSet::iterator        ColumnAttrSetIter;
typedef ColumnAttrSet::const_iterator  ColumnAttrSetIterC;
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__OdbcWrapper_hpp__HH

