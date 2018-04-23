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
OdbcHandleDbc::OdbcHandleDbc(SQLHENV env_handle)
	:OdbcHandleBase<SQLHDBC>(SQL_HANDLE_DBC, env_handle, "Connect",
		GetConnectAttrBasic, SetConnectAttrBasic)
	,is_connected_(false)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcHandleDbc::~OdbcHandleDbc()
{
	if (IsConnected())
		Disconnect(OdbcThrowFlags::None);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcHandleDbc::IsConnected() const
{
	return(is_connected_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcHandleDbc::CheckIsConnected() const
{
	if (!IsConnected())
		throw OdbcException("The database connection handle " +
			MLB::Utility::ValueToStringHex(GetNativeHandle()) +
			" is not connected.");

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcHandleDbc::CheckIsNotConnected() const
{
	if (IsConnected())
		throw OdbcException("The database connection handle " +
			MLB::Utility::ValueToStringHex(GetNativeHandle()) +
			" is already connected.");

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::Connect(SQLHWND WindowHandle,
	SQLCHAR *InConnectionString, SQLSMALLINT StringLength1,
	SQLCHAR *OutConnectionString, SQLSMALLINT BufferLength,
	SQLSMALLINT *StringLength2Ptr, SQLUSMALLINT DriverCompletion,
	OdbcThrowFlags::Flags throw_flags)
{
	CheckIsNotConnected();

	SQLRETURN return_code;

	try {
		return_code   = DriverConnect(GetNativeHandle(), WindowHandle,
			InConnectionString, StringLength1, OutConnectionString, BufferLength,
			StringLength2Ptr, DriverCompletion, throw_flags);
		is_connected_ = true;
	}
	catch (const OdbcExceptionStatus &except) {
		if (except.GetStatus() == SQL_SUCCESS_WITH_INFO)
			is_connected_ = true;
		throw;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::Connect(SQLHWND WindowHandle,
	const std::string &InConnectionString, std::string &OutConnectionString,
	SQLUSMALLINT DriverCompletion, OdbcThrowFlags::Flags throw_flags)
{
	CheckIsNotConnected();

	SQLRETURN return_code;

	try {
		return_code   = DriverConnect(GetNativeHandle(), WindowHandle,
			InConnectionString, OutConnectionString, DriverCompletion,
			throw_flags);
		is_connected_ = true;
	}
	catch (const OdbcExceptionStatus &except) {
		if (except.GetStatus() == SQL_SUCCESS_WITH_INFO)
			is_connected_ = true;
		throw;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::Connect(SQLHWND WindowHandle,
	const std::string &InConnectionString, SQLUSMALLINT DriverCompletion,
	OdbcThrowFlags::Flags throw_flags)
{
	CheckIsNotConnected();

	SQLRETURN return_code;

	try {
		return_code   = DriverConnect(GetNativeHandle(), WindowHandle,
			InConnectionString, DriverCompletion, throw_flags);
		is_connected_ = true;
	}
	catch (const OdbcExceptionStatus &except) {
		if (except.GetStatus() == SQL_SUCCESS_WITH_INFO)
			is_connected_ = true;
		throw;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::Connect(SQLCHAR *InConnectionString,
	SQLSMALLINT StringLength1, SQLCHAR *OutConnectionString,
	SQLSMALLINT BufferLength, SQLSMALLINT *StringLength2Ptr,
	SQLUSMALLINT DriverCompletion, OdbcThrowFlags::Flags throw_flags)
{
	return(Connect(NULL, InConnectionString, StringLength1, OutConnectionString,
		BufferLength, StringLength2Ptr, DriverCompletion, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::Connect(const std::string &InConnectionString,
	std::string &OutConnectionString, SQLUSMALLINT DriverCompletion,
	OdbcThrowFlags::Flags throw_flags)
{
	return(Connect(NULL, InConnectionString, OutConnectionString,
		DriverCompletion, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::Connect(const std::string &InConnectionString,
	SQLUSMALLINT DriverCompletion, OdbcThrowFlags::Flags throw_flags)
{
	return(Connect(NULL, InConnectionString, DriverCompletion, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::Connect(const DriverConnectParams &connect_params,
	SQLUSMALLINT DriverCompletion, OdbcThrowFlags::Flags throw_flags)
{
	return(Connect(NULL, connect_params.GetConnectionString(), DriverCompletion,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::Disconnect(OdbcThrowFlags::Flags throw_flags)
{
	CheckIsConnected();

	SQLRETURN return_code;

	try {
		return_code   =
			MLB::OdbcWrapper::Disconnect(GetNativeHandle(), throw_flags);
		is_connected_ = false;
	}
	catch (const OdbcExceptionStatus &except) {
		if (except.GetStatus() == SQL_SUCCESS_WITH_INFO)
			is_connected_ = false;
		throw;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetFunctions(SQLUSMALLINT FunctionId,
	SQLUSMALLINT *SupportedPtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetFunctions(GetNativeHandle(), FunctionId,
		SupportedPtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetFunctions(SQLUSMALLINT FunctionId,
	SQLUSMALLINT &SupportedPtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetFunctions(GetNativeHandle(), FunctionId,
		SupportedPtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetFunctions(GetFunctions_ApiAll &SupportedPtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetFunctions(GetNativeHandle(), SupportedPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetFunctions(GetFunctions_ApiAll_ODBC3 &SupportedPtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetFunctions(GetNativeHandle(), SupportedPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetInfo(SQLUSMALLINT InfoType,
	SQLPOINTER InfoValuePtr, SQLSMALLINT BufferLength,
	SQLSMALLINT *StringLengthPtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetInfo(GetNativeHandle(), InfoType, InfoValuePtr,
		BufferLength, StringLengthPtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetInfo(SQLUSMALLINT InfoType,
	std::string &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetInfo(GetNativeHandle(), InfoType, InfoValuePtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetInfo(SQLUSMALLINT InfoType,
	SQLUSMALLINT &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetInfo(GetNativeHandle(), InfoType, InfoValuePtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetInfo(SQLUSMALLINT InfoType,
	SQLUINTEGER &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetInfo(GetNativeHandle(), InfoType, InfoValuePtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetInfo(const GetInfoTypeDatumRaw *InfoType,
	GetInfoTypeDatumValue &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetInfo(GetNativeHandle(), InfoType, InfoValuePtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetInfo(const GetInfoTypeDatumRaw *InfoType,
	GetInfoTypeDatum &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetInfo(GetNativeHandle(), InfoType, InfoValuePtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetInfo(SQLUSMALLINT InfoType,
	GetInfoTypeDatumValue &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetInfo(GetNativeHandle(), InfoType, InfoValuePtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN OdbcHandleDbc::GetInfo(SQLUSMALLINT InfoType,
	GetInfoTypeDatum &InfoValuePtr, OdbcThrowFlags::Flags throw_flags)
{
	return(MLB::OdbcWrapper::GetInfo(GetNativeHandle(), InfoType, InfoValuePtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcHandleStmtSPtr OdbcHandleDbc::CreateStatement()
{
	return(OdbcHandleStmtSPtr(new OdbcHandleStmt(GetNativeHandle())));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
GetInfoTypeDatumList &OdbcHandleDbc::CreateGetInfoDatumList(
	GetInfoTypeDatumList &out_datum) const
{
	return(GetInfoTypeDatumList::Create(GetNativeHandle(), out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
GetInfoTypeDatumList OdbcHandleDbc::CreateGetInfoDatumList() const
{
	return(GetInfoTypeDatumList::Create(GetNativeHandle()));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OdbcWrapper;

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
		odbc_dbc_sptr->Connect(connect_params.GetConnectionString());
		GetInfoTypeDatumList     get_info_list(odbc_dbc_sptr->CreateGetInfoDatumList());
		GetInfoTypeDatumSetIterC iter_b(get_info_list.Begin());
		GetInfoTypeDatumSetIterC iter_e(get_info_list.End());
		for ( ; iter_b != iter_e; ++iter_b)
			std::cout << std::setw(5) << iter_b->ToStringPadded() << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR IN REGRESSION TEST FOR "
			"'MLB::OdbcWrapper::OdbcHandleDbc': " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

