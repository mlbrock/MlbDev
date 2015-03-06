// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the OdbcTransBlockManualBase class.

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

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
OdbcTransBlockManualBase::OdbcTransBlockManualBase(SQLHANDLE dbc_handle,
	bool strict_flag)
	:dbc_handle_(dbc_handle)
	,set_on_in_dtor_(false)
{
	SQLINTEGER current_autocommit_setting;

	try {
		GetConnectAttr(dbc_handle_, SQL_ATTR_AUTOCOMMIT,
			current_autocommit_setting);
		if (current_autocommit_setting == SQL_AUTOCOMMIT_OFF) {
			if (strict_flag)
				MLB::Utility::ThrowRuntimeError("Auto-commit is already off "
					"the 'strict_flag' parameter is 'true'.");
		}
		else if (current_autocommit_setting == SQL_AUTOCOMMIT_ON) {
			SetConnectAttr(dbc_handle_, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
			set_on_in_dtor_ = true;
		}
		else
			MLB::Utility::ThrowRuntimeError("Attempt to retrieve the current "
				"'SQL_ATTR_AUTOCOMMIT' state returned a value of " +
				MLB::Utility::AnyToString(current_autocommit_setting) +
				" --- expected either 'SQL_AUTOCOMMIT_OFF' (" +
				MLB::Utility::AnyToString(SQL_AUTOCOMMIT_OFF) + ") or "
				"'SQL_AUTOCOMMIT_ON' (" +
				MLB::Utility::AnyToString(SQL_AUTOCOMMIT_ON) + ").");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to create an manual-commit "
			"transaction block for Odbc connection handle " +
			MLB::Utility::ValueToStringHex(dbc_handle_) + " failed: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcTransBlockManualBase::~OdbcTransBlockManualBase()
{
	if (set_on_in_dtor_) {
		try {
			SetConnectAttr(dbc_handle_, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_ON);
		}
		catch (const std::exception &) {
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

