// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Class OdbcParseCmdLineArg implementation file.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__OdbcParseCmdLineArg_hpp__HH

#define HH__MLB__OdbcParseCmdLineArg_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/ParseCmdLineArg.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
struct OdbcParseCmdLineArg : public MLB::Utility::ParseCmdLineArg {
	static bool OdbcParseCmdLineArg::ParseCmdLineUid(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool OdbcParseCmdLineArg::ParseCmdLinePwd(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool OdbcParseCmdLineArg::ParseCmdLineDatabase(
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool OdbcParseCmdLineArg::ParseCmdLineServer(
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool OdbcParseCmdLineArg::ParseCmdLineDriver(
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool OdbcParseCmdLineArg::ParseCmdLineDriverParams(
		unsigned int &current_index, int argc, char **argv,
		DriverConnectParams &out_datum);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__OdbcParseCmdLineArg_hpp__HH

