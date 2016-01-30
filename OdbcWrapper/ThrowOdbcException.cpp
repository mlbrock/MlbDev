// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Provides an interface for throwing OdbcWrapper
								exceptions.

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

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

//	////////////////////////////////////////////////////////////////////////////
bool PrepareProvisionalText(OdbcThrowFlags::Flags throw_flags,
	SQLRETURN odbc_status, const char *function_name, const char *other_text,
	std::string &out_string)
{
	if (odbc_status == SQL_SUCCESS) {
		out_string.clear();
		return(false);
	}

	OdbcThrowFlags::Flags return_flag =
		OdbcThrowFlags::SqlReturnToFlags(odbc_status);

	if (!(return_flag & throw_flags)) {
		out_string.clear();
		return(false);
	}

	std::ostringstream o_str;

	o_str << "Invocation of '" <<
		((function_name == NULL) ? "*UNSPECIFIED-FUNCTION-NAME*" :
			function_name) << "()" << "' failed";

	if (odbc_status == SQL_SUCCESS_WITH_INFO)
		o_str
			<< " (return code of 'SQL_SUCCESS_WITH_INFO' was unexpected in "
				"this context)";

	if ((other_text != NULL) && *other_text)
		o_str
			<< ": " << other_text;

	o_str.str().swap(out_string);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN ThrowOdbcStatusProvisional(OdbcThrowFlags::Flags throw_flags,
	SQLRETURN odbc_status, SQLSMALLINT /* handle_type */,
	SQLHANDLE /* handle_value */, const char *function_name,
	const char *function_args, const char *other_text)
{
	std::string out_string;

	if (PrepareProvisionalText(throw_flags, odbc_status, function_name,
		other_text, out_string))
		throw OdbcExceptionStatus(odbc_status, out_string, function_args);

	return(odbc_status);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLRETURN ThrowOdbcDiagProvisional(OdbcThrowFlags::Flags throw_flags,
	SQLRETURN odbc_status, SQLSMALLINT handle_type, SQLHANDLE handle_value,
	const char *function_name, const char *function_args, const char *other_text)
{
	std::string out_string;

	if (PrepareProvisionalText(throw_flags, odbc_status, function_name,
		other_text, out_string))
		throw OdbcExceptionDiag(odbc_status, handle_type, handle_value,
			out_string, function_args);

	return(odbc_status);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ThrowOdbcStatus(SQLRETURN odbc_status, const char *error_text)
{
	throw OdbcExceptionStatus(odbc_status, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ThrowOdbcStatus(SQLRETURN odbc_status, const std::string &error_text)
{
	throw OdbcExceptionStatus(odbc_status, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ThrowOdbcStatus(SQLRETURN odbc_status,
	const std::ostringstream &error_text)
{
	throw OdbcExceptionStatus(odbc_status, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

