// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the class OdbcParseCmdLineArg.

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

#include <Utility/InlineContainer.hpp>

#include <OdbcWrapper/OdbcParseCmdLineArg.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

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
		("-SERVERNAME")
		("-SERVER"),
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

