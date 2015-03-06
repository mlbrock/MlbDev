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

#include <Utility/IncrementalBuffer.hpp>
#include <Utility/EnumFlagOps.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

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
			if (!raw_list[count_1].is_handle_2way_) {
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

