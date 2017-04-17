// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Supporting functions for the retrieval of values using
								the ODBC function 'SQLGetDiagField()'.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2017.
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
SQLSMALLINT GetDiagRecordNumberCount(SQLSMALLINT handle_type,
	SQLHANDLE handle_value)
{
	SQLINTEGER record_number;

	try {
		GetDiagField_SQLINTEGER(handle_type, handle_value, 0, SQL_DIAG_NUMBER,
			record_number);
		if (record_number < 0)
			throw OdbcException("Invocation of '::SQLGetDiagField()' with a "
				"diagnostic identifier of 'SQL_DIAG_NUMBER' resulted in a value "
				"of 0, which is invalid.");
		else if (record_number > std::numeric_limits<SQLSMALLINT>::max())
			throw OdbcException("Invocation of '::SQLGetDiagField()' with a "
				"diagnostic identifier of 'SQL_DIAG_NUMBER' resulted in a value "
				"of " + MLB::Utility::AnyToString(record_number) + ", which is "
				"greater than the maximum permissible for a value of type '"
				"SQLSMALLINT' (" + MLB::Utility::AnyToString(
				std::numeric_limits<SQLSMALLINT>::max()) + ").");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to retrieve the count of "
			"diagnostic records for handle type " +
			MLB::Utility::AnyToString(handle_type) + ", handle value " +
			MLB::Utility::AnyToString(handle_value) + " failed: " +
			std::string(except.what()));
	}

	return(static_cast<SQLSMALLINT>(record_number));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SQLSMALLINT CheckIsValidDiagRecordNumber(SQLSMALLINT handle_type,
	SQLHANDLE handle_value, SQLSMALLINT record_number)
{
	if (record_number < 1)
		throw OdbcException("A diagnostic record number of 0 is invalid ("
			"diagnostic records are numbered from 1).");

	SQLSMALLINT record_number_count =
		GetDiagRecordNumberCount(handle_type, handle_value);

	if (record_number > record_number_count)
		throw OdbcException("The specified diagnostic record number of " +
			MLB::Utility::AnyToString(record_number) + "handle type " +
			MLB::Utility::AnyToString(handle_type) + ", handle value " +
			MLB::Utility::AnyToString(handle_value) + " is invalid --- "
			"the maximum available diagnostic record number is " +
			MLB::Utility::AnyToString(record_number_count) + ".");

	return(record_number_count);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

