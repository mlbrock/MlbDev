// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the OdbcThrowFlags class.

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

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

//	////////////////////////////////////////////////////////////////////////////
OdbcThrowFlags::Flags OdbcThrowFlags::SqlReturnToFlags(SQLRETURN odbc_status)
{
	if (odbc_status == SQL_SUCCESS)
		return(OdbcThrowFlags::None);
	else if (odbc_status == SQL_SUCCESS_WITH_INFO)
		return(OdbcThrowFlags::SuccessWithInfo);
	else if (odbc_status == SQL_NO_DATA)
		return(OdbcThrowFlags::NoData);
	else if (odbc_status == SQL_STILL_EXECUTING)
		return(OdbcThrowFlags::StillExecuting);
	else if (odbc_status == SQL_NEED_DATA)
		return(OdbcThrowFlags::NeedData);
	else if (odbc_status == SQL_ERROR)
		return(OdbcThrowFlags::Error);
	else if (odbc_status != SQL_INVALID_HANDLE)
		MLB::Utility::ThrowInvalidArgument("The specified ODBC return code (" +
			MLB::Utility::AnyToString(odbc_status) + ") is unsupported.");

	return(OdbcThrowFlags::InvalidHandle);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

