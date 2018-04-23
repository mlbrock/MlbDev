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

#include <Utility/IncrementalBuffer.hpp>

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

#include <Utility/ParseCmdLineArg.hpp>

namespace MLB {
namespace OdbcWrapper {

//	CODE NOTE: To be moved into appropriate .h and .cpp..
// ////////////////////////////////////////////////////////////////////////////
struct DriverConnectParams {
	explicit DriverConnectParams(const std::string &in_uid = "sa",
		const std::string &in_pwd = "", const std::string &in_database = "master",
		const std::string &in_server = "(local)",
		const std::string &in_driver = "{SQL Native Client}");

	std::string &GetConnectionString(std::string &out_string) const;
	std::string  GetConnectionString() const;

	bool         ParseDriverParams(unsigned int &current_index,
		int argc, char **argv, bool throw_if_not_matched_flag = true);

	std::string uid_;
	std::string pwd_;
	std::string database_;
	std::string server_;
	std::string driver_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcParseCmdLineArg : public ParseCmdLineArg {
	bool OdbcParseCmdLineArg::ParseCmdLineUid(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	bool OdbcParseCmdLineArg::ParseCmdLinePwd(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	bool OdbcParseCmdLineArg::ParseCmdLineDatabase(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	bool OdbcParseCmdLineArg::ParseCmdLineServer(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	bool OdbcParseCmdLineArg::ParseCmdLineDriver(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	bool OdbcParseCmdLineArg::ParseCmdLineDriverParams(
		unsigned int &current_index, int argc, char **argv,
		DriverConnectParams &out_datum);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DriverConnectParams::DriverConnectParams(const std::string &in_uid,
	const std::string &in_pwd, const std::string &in_database,
	const std::string &in_server, const std::string &in_driver)
	:uid_(in_uid)
	,pwd_(in_pwd)
	,database_(in_database)
	,server_(in_server)
	,driver_(in_driver)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DriverConnectParams::AddEntry(const std::string &new_name,
	const std::string &new_value)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &DriverConnectParams::GetConnectionString(
	std::string &out_string) const
{
	std::string("Database=" + database_ + ";Driver=" + driver_ + ";Uid=" +
		uid_ + ";Server=" + server_ + ";Pwd=" + pwd_).swap(out_string);

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string DriverConnectParams::GetConnectionString() const
{
	std::string out_string;

	return(GetConnectionString(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DriverConnectParams::ParseDriverParams(unsigned int &current_index,
	int argc, char **argv, bool throw_if_not_matched_flag)
{
	bool have_matched_flag = false;

	for (current_index; count_1 < static_cast<unsigned int>(argc);
		current_index) {
		if (!ParseCmdLineDriverParams(current_index, argc, argv, *this)) {
			if (throw_if_not_matched_flag)
				OdbcParseCmdLineArg::InvalidArgument(argv[current_index]);
			break;
		}
		have_matched_flag = true;
	}

	return(have_matched_flag);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcParseCmdLineArg::ParseCmdLineUid(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-USER_IDENTIFIER")
		("-USERIDENTIFIER")
		("-USER_ID")
		("-USERID")
		("-USER_NAME")
		("-USERNAME")
		("-USR_IDENTIFIER")
		("-USRIDENTIFIER")
		("-USR_ID")
		("-USRID")
		("-USR_NAME")
		("-USRNAME")
		("-UID"),
		current_index, argc, argv, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcParseCmdLineArg::ParseCmdLinePwd(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-USER_PASSWORD")
		("-USERPASSWORD")
		("-USR_PASSWORD")
		("-USRPASSWORD")
		("-USER_PWD")
		("-USERPWD")
		("-USR_PWD")
		("-USRPWD")
		("-PASSWORD")
		("-PWD"),
		current_index, argc, argv, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcParseCmdLineArg::ParseCmdLineDatabase(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-DATABASE_NAME")
		("-DATABASENAME")
		("-DB_NAME")
		("-DBNAME")
		("-DB"),
		current_index, argc, argv, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcParseCmdLineArg::ParseCmdLineServer(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-DBMS_SERVER_NAME")
		("-DBMS_SERVERNAME")
		("-DBMSSERVER_NAME")
		("-DBMSSERVERNAME")
		("-DBMS_SERVER")
		("-DBMSSERVER")
		("-DB_SERVER_NAME")
		("-DB_SERVERNAME")
		("-DBSERVER_NAME")
		("-DBSERVERNAME")
		("-DB_SERVER")
		("-DBSERVER")
		("-SERVER_NAME")
		("-SERVERNAME"),
		current_index, argc, argv, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcParseCmdLineArg::ParseCmdLineDriver(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-ODBC_DRIVER_NAME")
		("-ODBC_DRIVERNAME")
		("-ODBCDRIVER_NAME")
		("-ODBCDRIVERNAME")
		("-ODBC_DRIVER")
		("-ODBCDRIVER")
		("-DRIVER")
		("-ODBC_DRV_NAME")
		("-ODBC_DRVNAME")
		("-ODBCDRV_NAME")
		("-ODBCDRVNAME")
		("-ODBC_DRV")
		("-ODBCDRV")
		("-DRV"),
		current_index, argc, argv, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OdbcParseCmdLineArg::ParseCmdLineDriverParams(unsigned int &current_index,
	int argc, char **argv, DriverConnectParams &out_datum)
{
	if (ParseCmdLineUid(current_index, argc, argv, out_datum.uid_))
		return(true);
	else if (ParseCmdLinePwd(current_index, argc, argv, out_datum.pwd_))
		return(true);
	else if (ParseCmdLineDatabase(current_index, argc, argv, out_datum.database_))
		return(true);
	else if (ParseCmdLineServer(current_index, argc, argv, out_datum.server_))
		return(true);
	else if (ParseCmdLineDriver(current_index, argc, argv, out_datum.driver_))
		return(true);

	return(false);
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

