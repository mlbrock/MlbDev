//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The SocketIo ParseCmdLineArg class include file.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock
								1996-04-10 --- Based upon the portable Network
													Programming Suppoprt Library (NPSL).
									Michael L. Brock

		Copyright Michael L. Brock 1996 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__SocketIoParseCmdLineArg_hpp__HH

#define HH__MLB__SocketIo__SocketIoParseCmdLineArg_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketSpecLocal.hpp>
#include <SocketIo/PacketFormat.hpp>
#include <SocketIo/IpAddressGeneric.hpp>
#include <SocketIo/ResolveHost.hpp>
#include <SocketIo/ResolveService.hpp>

#include <Utility/ParseCmdLineArg.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

//	////////////////////////////////////////////////////////////////////////////
struct API_SOCKETIO SocketIoParseCmdLineArg :
	public MLB::Utility::ParseCmdLineArg {
	static bool ParseCmdLineHost(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineHostResolved(unsigned int &current_index, int argc,
		char **argv, HostEntry &out_datum, bool require_ip_address = false);
	static bool ParseCmdLineHostResolved(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineHostResolved(unsigned int &current_index,
		int argc, char **argv, IpAddressType &out_datum);
	static bool ParseCmdLineService(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineServiceResolved(unsigned int &current_index,
		int argc, char **argv, ServiceEntry &out_datum,
		const std::string &protocol = "");
	static bool ParseCmdLineServiceResolved(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum,
		const std::string &protocol = "");
	static bool ParseCmdLineServiceResolved(unsigned int &current_index,
		int argc, char **argv, IpPortType &out_datum,
		const std::string &protocol = "");
	static bool ParseCmdLinePort(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLinePortResolved(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum,
		const std::string &protocol = "");
	static bool ParseCmdLineBufferSizeRecv(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum, unsigned int min_size = 0,
		unsigned int max_size = std::numeric_limits<unsigned int>::max());
	static bool ParseCmdLineBufferSizeSend(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum, unsigned int min_size = 0,
		unsigned int max_size = std::numeric_limits<unsigned int>::max());
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
	static bool ParseCmdLinePacketFormat(
		const std::vector<std::string> &param_name_list,
		unsigned int &current_index, int argc, char **argv,
		PacketFormat &out_datum);
	static bool ParseCmdLinePacketFormat(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		PacketFormat &out_datum);
	static bool ParseCmdLinePacketFormat(unsigned int &current_index, int argc,
		char **argv, PacketFormat &out_datum);
	static bool ParseCmdLineMCAddressList(unsigned int &current_index, int argc,
		char **argv, std::vector<SocketSpec> &out_datum);
	static bool ParseCmdLineSocketSpec(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		MLB::SocketIo::SocketSpec &out_datum, bool require_mc = false);
	static bool ParseCmdLineSocketSpec(
		const std::vector<std::string> &spec_cont, unsigned int &current_index,
		int argc, char **argv, MLB::SocketIo::SocketSpec &out_datum,
		bool require_mc = false);
	static bool ParseCmdLineSocketSpec(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		SocketSpecLocal &out_datum);
	static bool ParseCmdLineSocketSpec(const std::vector<std::string> &spec_cont,
		unsigned int &current_index, int argc, char **argv,
		SocketSpecLocal &out_datum);
	static bool ParseCmdLineHostInterface(
		const std::vector<std::string> &param_name_list,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool ParseCmdLineHostInterface(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool ParseCmdLineHostInterface(unsigned int &current_index, int argc,
		char **argv, std::string &out_datum);
	static bool ParseCmdLineIpAddress(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		IpAddressGeneric &out_datum, bool ipv6_ok = true, bool mc_ok = true, 
		bool require_mc = false);
	static bool ParseCmdLineIpAddress(const std::vector<std::string> &spec_cont,
		unsigned int &current_index, int argc, char **argv,
		IpAddressGeneric &out_datum, bool ipv6_ok = true, bool mc_ok = true,
		bool require_mc = false);
	static bool ParseCmdLineIpAddressList(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		IpAddressGenericList &out_datum, bool ipv6_ok = true, bool mc_ok = true,
		bool require_mc = false);
	static bool ParseCmdLineIpAddressList(
		const std::vector<std::string> &spec_cont, unsigned int &current_index,
		int argc, char **argv, IpAddressGenericList &out_datum,
		bool ipv6_ok = true, bool mc_ok = true, bool require_mc = false);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__SocketIoParseCmdLineArg_hpp__HH

