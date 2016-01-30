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
								function 'SQLDescribeCol()'.

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
#include <Utility/EnumFlagOps.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN DescribeCol(SQLHSTMT StatementHandle, SQLSMALLINT ColumnNumber,
	SQLCHAR *ColumnName, SQLSMALLINT BufferLength, SQLSMALLINT *NameLengthPtr,
	SQLSMALLINT *DataTypePtr, SQLUINTEGER *ColumnSizePtr,
	SQLSMALLINT *DecimalDigitsPtr, SQLSMALLINT *NullablePtr,
	OdbcThrowFlags::Flags throw_flags)
{
	return(OdbcWrapper_THROW_STATUS_IF_LEVEL_1(throw_flags,
		SQL_HANDLE_STMT, StatementHandle, ::SQLDescribeCol,
		(StatementHandle, ColumnNumber, ColumnName, BufferLength, NameLengthPtr,
		DataTypePtr, ColumnSizePtr, DecimalDigitsPtr, NullablePtr)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN DescribeCol(SQLHSTMT StatementHandle, SQLSMALLINT ColumnNumber,
	std::string &ColumnName, SQLSMALLINT *DataTypePtr,
	SQLUINTEGER *ColumnSizePtr, SQLSMALLINT *DecimalDigitsPtr,
	SQLSMALLINT *NullablePtr, OdbcThrowFlags::Flags throw_flags)
{
	SQLRETURN                                     return_code;
	MLB::Utility::IncrementalBuffer<SQLCHAR, 256> buffer;
	OdbcThrowFlags::Flags                         tmp_throw_flags;

	tmp_throw_flags = MLB::Utility::EnumFlagAnd(throw_flags,
		static_cast<OdbcThrowFlags::Flags>(~(OdbcThrowFlags::SuccessWithInfo)));

	for ( ; ; ) {
		SQLSMALLINT actual_len;
		return_code = DescribeCol(StatementHandle, ColumnNumber,
			reinterpret_cast<SQLCHAR *>(buffer.GetPtr()),
			buffer.GetAllocationCountAsType<SQLSMALLINT>(), &actual_len,
         DataTypePtr, ColumnSizePtr, DecimalDigitsPtr, NullablePtr,
			tmp_throw_flags);
		if (return_code == SQL_SUCCESS) {
			std::string(reinterpret_cast<const char *>(buffer.GetPtr()),
				actual_len).swap(ColumnName);
			break;
		}
		else if (return_code == SQL_SUCCESS_WITH_INFO) {
			if (!buffer.SetCount(actual_len, false)) {
				std::string(reinterpret_cast<const char *>(buffer.GetPtr()),
					actual_len).swap(ColumnName);
				if (throw_flags & OdbcThrowFlags::SuccessWithInfo)
					throw OdbcException("Call to '::SQLDescribeCol()' returned "
						"'SQL_SUCCESS_WITH_INFO', but no returned lengths qualified "
						"for that return code.");
				break;
			}
		}
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OdbcWrapper;

// ////////////////////////////////////////////////////////////////////////////
void TEST_ConnectToDatabase(const DriverConnectParams &connect_params,
	const std::vector<std::string> &table_list = std::vector<std::string>())
{
	try {
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
		OdbcHandleDbcSPtr odbc_dbc_sptr(odbc_env.AddConnection());
		odbc_dbc_sptr->Connect(connect_params.GetConnectionString());
		std::cout << "   Connection created." << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_ConnectToDatabase(DriverConnectParams("sa", "S_a_0",
			"GDSP_LON_TRD_TST", "(local)"));
	}
	catch (const std::exception &except) {
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

