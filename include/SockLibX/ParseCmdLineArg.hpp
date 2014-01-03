//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The SockLibXParseCmdLineArg class include file.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SockLibX__SockLibX_ParseCmdLineArg_hpp__HH

#define HH__MLB__SockLibX__SockLibX_ParseCmdLineArg_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/ParseCmdLineArg.hpp>

#include <SockLibX/SocketSpec.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
struct API_SOCKLIBX SockLibXParseCmdLineArg :
	public MLB::Utility::ParseCmdLineArg {
	static bool ParseCmdLineHost(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineHostResolved(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineService(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineServiceResolved(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum,
		const std::string &protocol = "");
	static bool ParseCmdLinePort(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLinePortResolved(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum,
		const std::string &protocol = "");
	static bool ParseCmdLineBufferSizeRecv(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool ParseCmdLineBufferSizeSend(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool ParseCmdLineSocketDebugFlag(unsigned int &current_index,
		int argc, char **argv, bool &out_datum);
	static bool ParseCmdLineDontLingerFlag(unsigned int &current_index,
		int argc, char **argv, bool &out_datum);
	static bool ParseCmdLineDontRouteFlag(unsigned int &current_index,
		int argc, char **argv, bool &out_datum);
	static bool ParseCmdLineKeepAliveFlag(unsigned int &current_index,
		int argc, char **argv, bool &out_datum);
	static bool ParseCmdLineOobInlineFlag(unsigned int &current_index,
		int argc, char **argv, bool &out_datum);
	static bool ParseCmdLineTcpNoDelayFlag(unsigned int &current_index,
		int argc, char **argv, bool &out_datum);
	static bool ParseCmdLineSocketSpec(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		MLB::SockLibX::SocketSpec &out_datum, bool require_mc = false);
	static bool ParseCmdLineSocketSpec(
		const std::vector<std::string> &spec_cont, unsigned int &current_index,
		int argc, char **argv, MLB::SockLibX::SocketSpec &out_datum,
		bool require_mc = false);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#endif // #ifndef HH__MLB__SockLibX__SockLibX_ParseCmdLineArg_hpp__HH

