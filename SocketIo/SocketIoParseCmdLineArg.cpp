//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for class SocketIoParseCmdLineArg.

	Revision History	:	2002-01-26 --- A modification of the logic within the
													SockLibX/SockLibXParseCmdLineArg class.
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketIoParseCmdLineArg.hpp>
#include <SocketIo/StringToIpAddress.hpp>
#include <SocketIo/MulticastIP.hpp>

#include <Utility/InlineContainer.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineHost(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-HOST")
		("-HOST_NAME")
		("-HOSTNAME"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineHostResolved(
	unsigned int &current_index, int argc, char **argv, HostEntry &out_datum,
	bool require_ip_address)
{
	std::string tmp_name;

	if (!ParseCmdLineHost(current_index, argc, argv, tmp_name))
		return(false);

	try {
		HostEntry tmp_datum(HostNameToEntry(tmp_name));
		if (require_ip_address)
			tmp_datum.CheckIpAddress();
		out_datum.swap(tmp_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse the host name value "
			"for the '" + std::string(argv[current_index - 1]) +
			"' parameter: " + std::string(except.what()));
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineHostResolved(
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	HostEntry tmp_datum;

	if (!ParseCmdLineHostResolved(current_index, argc, argv, tmp_datum))
		return(false);

	out_datum.swap(tmp_datum.h_name);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineHostResolved(
	unsigned int &current_index, int argc, char **argv, IpAddressType &out_datum)
{
	HostEntry tmp_datum;

	if (!ParseCmdLineHostResolved(current_index, argc, argv, tmp_datum, true))
		return(false);

	out_datum = tmp_datum.h_addr_list[0];

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineService(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-SERVICE")
		("-SERVICE_NUM")
		("-SERVICENUM")
		("-SERVICE_NUMBER")
		("-SERVICENUMBER")
		("-SERVICE_NAME")
		("-SERVICENAME")
		("-SERV")
		("-SERV_NUM")
		("-SERVNUM")
		("-SERV_NUMBER")
		("-SERVNUMBER")
		("-SERV_NAME")
		("-SERVNAME")
		("-PORT")
		("-PORT_NUM")
		("-PORTNUM")
		("-PORT_NUMBER")
		("-PORTNUMBER")
		("-PORT_NAME")
		("-PORTNAME"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineServiceResolved(
	unsigned int &current_index, int argc, char **argv, ServiceEntry &out_datum,
	const std::string &protocol)
{
	std::string tmp_datum;

	if (!ParseCmdLineService(current_index, argc, argv, tmp_datum))
		return(false);

	try {
		out_datum = ServiceNameToEntry(tmp_datum, protocol);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse the service name value "
			"for the '" + std::string(argv[current_index - 1]) +
			"' parameter: " + std::string(except.what()));
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineServiceResolved(
	unsigned int &current_index, int argc, char **argv, std::string &out_datum,
	const std::string &protocol)
{
	ServiceEntry tmp_datum;

	if (!ParseCmdLineServiceResolved(current_index, argc, argv, tmp_datum,
		protocol))
		return(false);

	out_datum.swap(tmp_datum.s_name);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineServiceResolved(
	unsigned int &current_index, int argc, char **argv, IpPortType &out_datum,
	const std::string &protocol)
{
	ServiceEntry tmp_datum;

	if (!ParseCmdLineServiceResolved(current_index, argc, argv, tmp_datum,
		protocol))
		return(false);

	out_datum = static_cast<IpPortType>(tmp_datum.s_port);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLinePort(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineService(current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLinePortResolved(
	unsigned int &current_index, int argc, char **argv, std::string &out_datum,
	const std::string &protocol)
{
	return(ParseCmdLineServiceResolved(current_index, argc, argv, out_datum,
		protocol));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineBufferSizeRecv(
	unsigned int &current_index, int argc, char **argv, unsigned int &out_datum,
	unsigned int min_size, unsigned int max_size)
{
/*
	return(ParseCmdLineDatumSpec(MLB::Utility::MakeInlineVector<std::string>
		("-RECEIVE_BUFFER_SIZE")
		("-RECEIVE_BUFFERSIZE")
		("-RECEIVEBUFFER_SIZE")
		("-RECEIVEBUFFERSIZE")
		("-RECEIVE_BUFFER")
		("-RECEIVEBUFFER")
		("-RECEIVE_BUFF_SIZE")
		("-RECEIVE_BUFFSIZE")
		("-RECEIVEBUFF_SIZE")
		("-RECEIVEBUFFSIZE")
		("-RECEIVE_BUFF")
		("-RECEIVEBUFF")
		("-RECEIVE_SIZE")
		("-RECEIVESIZE")
		("-RECV_BUFFER_SIZE")
		("-RECV_BUFFERSIZE")
		("-RECVBUFFER_SIZE")
		("-RECVBUFFERSIZE")
		("-RECV_BUFFER")
		("-RECVBUFFER")
		("-RECV_BUFF_SIZE")
		("-RECV_BUFFSIZE")
		("-RECVBUFF_SIZE")
		("-RECVBUFFSIZE")
		("-RECV_BUFF")
		("-RECVBUFF")
		("-RECV_SIZE")
		("-RECVSIZE")
		("-RCV_BUFFER_SIZE")
		("-RCV_BUFFERSIZE")
		("-RCVBUFFER_SIZE")
		("-RCVBUFFERSIZE")
		("-RCV_BUFFER")
		("-RCVBUFFER")
		("-RCV_BUFF_SIZE")
		("-RCV_BUFFSIZE")
		("-RCVBUFF_SIZE")
		("-RCVBUFFSIZE")
		("-RCV_BUFF")
		("-RCVBUFF")
		("-RCV_SIZE")
		("-RCVSIZE"),
		current_index, argc, argv, out_datum));
*/
	return(ParseCmdLineNumeric<unsigned int>(MLB::Utility::RegexParamNameAdaptor(
		"^\\-\\-*((RE*CV)|(RECEIVE))_*(BUFF(ER)*_*)*SIZE$",
		current_index, argc, argv), current_index, argc, argv, out_datum,
		min_size, max_size));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineBufferSizeSend(
	unsigned int &current_index, int argc, char **argv, unsigned int &out_datum,
	unsigned int min_size, unsigned int max_size)
{
/*
	return(ParseCmdLineDatumSpec(MLB::Utility::MakeInlineVector<std::string>
		("-SEND_BUFFER_SIZE")
		("-SEND_BUFFERSIZE")
		("-SENDBUFFER_SIZE")
		("-SENDBUFFERSIZE")
		("-SEND_BUFFER")
		("-SENDBUFFER")
		("-SEND_BUFF_SIZE")
		("-SEND_BUFFSIZE")
		("-SENDBUFF_SIZE")
		("-SENDBUFFSIZE")
		("-SEND_BUFF")
		("-SENDBUFF")
		("-SEND_SIZE")
		("-SENDSIZE")
		("-SND_BUFFER_SIZE")
		("-SND_BUFFERSIZE")
		("-SNDBUFFER_SIZE")
		("-SNDBUFFERSIZE")
		("-SND_BUFFER")
		("-SNDBUFFER")
		("-SND_BUFF_SIZE")
		("-SND_BUFFSIZE")
		("-SNDBUFF_SIZE")
		("-SNDBUFFSIZE")
		("-SND_BUFF")
		("-SNDBUFF")
		("-SND_SIZE")
		("-SNDSIZE"),
		current_index, argc, argv, out_datum));
*/
	return(ParseCmdLineNumeric<unsigned int>(MLB::Utility::RegexParamNameAdaptor(
		"^\\-\\-*((SE*ND)|(SENT))_*(BUFF(ER)*_*)*SIZE$",
		current_index, argc, argv), current_index, argc, argv, out_datum,
		min_size, max_size));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineSocketDebugFlag(
	unsigned int &current_index, int argc, char **argv, bool &out_datum)
{
	return(ParseCmdLineDatumSpec(MLB::Utility::MakeInlineVector<std::string>
		("-SOCKET_DEBUGGING_FLAG")
		("-SOCKET_DEBUGGINGFLAG")
		("-SOCKETDEBUGGING_FLAG")
		("-SOCKETDEBUGGINGFLAG")
		("-SOCKET_DEBUG_FLAG")
		("-SOCKET_DEBUGFLAG")
		("-SOCKETDEBUG_FLAG")
		("-SOCKETDEBUGFLAG")
		("-SOCKET_DBG_FLAG")
		("-SOCKET_DBGFLAG")
		("-SOCKETDBG_FLAG")
		("-SOCKETDBGFLAG")
		("-SOCKET_DEBUG")
		("-SOCKETDEBUG")
		("-SOCKET_DBG")
		("-SOCKETDBG"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineDontLingerFlag(
	unsigned int &current_index, int argc, char **argv, bool &out_datum)
{
	return(ParseCmdLineDatumSpec(MLB::Utility::MakeInlineVector<std::string>
		("-DONT_LINGER_FLAG")
		("-DONT_LINGERFLAG")
		("-DONTLINGER_FLAG")
		("-DONTLINGERFLAG")
		("-DONT_LINGER")
		("-DONTLINGER"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineDontRouteFlag(
	unsigned int &current_index, int argc, char **argv, bool &out_datum)
{
	return(ParseCmdLineDatumSpec(MLB::Utility::MakeInlineVector<std::string>
		("-DONT_ROUTE_FLAG")
		("-DONT_ROUTEFLAG")
		("-DONTROUTE_FLAG")
		("-DONTROUTEFLAG")
		("-DONT_ROUTE")
		("-DONTROUTE"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineKeepAliveFlag(
	unsigned int &current_index, int argc, char **argv, bool &out_datum)
{
	return(ParseCmdLineDatumSpec(MLB::Utility::MakeInlineVector<std::string>
		("-KEEP_ALIVE_FLAG")
		("-KEEP_ALIVEFLAG")
		("-KEEPALIVE_FLAG")
		("-KEEPALIVEFLAG")
		("-KEEP_ALIVE")
		("-KEEPALIVE"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineOobInlineFlag(
	unsigned int &current_index, int argc, char **argv, bool &out_datum)
{
	return(ParseCmdLineDatumSpec(MLB::Utility::MakeInlineVector<std::string>
		("-OOB_INLINE_FLAG")
		("-OOB_INLINEFLAG")
		("-OOBINLINE_FLAG")
		("-OOBINLINEFLAG")
		("-OOB_INLINE")
		("-OOBINLINE"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineTcpNoDelayFlag(
	unsigned int &current_index, int argc, char **argv, bool &out_datum)
{
	return(ParseCmdLineDatumSpec(MLB::Utility::MakeInlineVector<std::string>
		("-TCP_NO_DELAY_FLAG")
		("-TCP_NO_DELAYFLAG")
		("-TCP_NODELAY_FLAG")
		("-TCPNO_DELAY_FLAG")
		("-TCP_NODELAYFLAG")
		("-TCPNO_DELAYFLAG")
		("-TCPNODELAY_FLAG")
		("-TCP_NO_DELAY")
		("-TCP_NODELAY")
		("-TCPNO_DELAY")
		("-TCPNODELAY"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLinePacketFormat(
	const std::vector<std::string> &param_name_list, unsigned int &current_index,
	int argc, char **argv, PacketFormat &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineArg::ParseCmdLineFollowingSpec(param_name_list,
		current_index, argc, argv, tmp_string))
		return(false);

	try {
		out_datum = StringToPacketFormat(tmp_string);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to parse a packet format type "
			"from parameter '" + std::string(argv[current_index - 1]) +
			"' failed: " + std::string(except.what()));
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLinePacketFormat(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, PacketFormat &out_datum)
{
	return(ParseCmdLinePacketFormat(std::vector<std::string>(1, param_name),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLinePacketFormat(
	unsigned int &current_index, int argc, char **argv, PacketFormat &out_datum)
{
	return(ParseCmdLinePacketFormat(
		MLB::Utility::MakeInlineVector<std::string>
			("-PACKET_FORMAT")
			("-PACKETFORMAT")
			("-FORMAT"), current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineMCAddressList(
	unsigned int &current_index, int argc, char **argv,
	SocketSpecList &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(
		MLB::Utility::MakeInlineVector<std::string>
			("-MULTICAST_ADDRESSES")
			("-MULTICASTADDRESSES")
			("-MULTICAST_ADDRESS")
			("-MULTICASTADDRESS")
			("-MULTICAST_ADDRS")
			("-MULTICASTADDRS")
			("-MULTICAST_ADDR")
			("-MULTICASTADDR")
			("-MC_ADDRESSES")
			("-MCADDRESSES")
			("-MC_ADDRESS")
			("-MCADDRESS")
			("-MC_ADDRS")
			("-MCADDRS")
			("-MC_ADDR")
			("-MCADDR")
			("-MULTICAST"),
		current_index, argc, argv, tmp_string))
		return(false);

	try {
		SocketSpec::ParseSocketSpecList(tmp_string, out_datum, true);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse the list of multicast "
			"groups for the '" + std::string(argv[current_index - 1]) +
			"' parameter: " + std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineSocketSpec(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, SocketSpec &out_datum, bool require_mc)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, tmp_string))
		return(false);

	try {
		SocketSpec tmp_datum;
		tmp_datum.FromString(tmp_string);
		if (require_mc)
			tmp_datum.CheckMulticastIPAddress();
		out_datum.swap(tmp_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "The '" + param_name +
			std::string((require_mc) ? "' multicast group " : "' ip address ") +
			"specification string '" + tmp_string + "' is invalid: " +
			std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineSocketSpec(
	const std::vector<std::string> &spec_cont, unsigned int &current_index,
	int argc, char **argv, SocketSpec &out_datum, bool require_mc)
{
	std::vector<std::string>::const_iterator iter_b(spec_cont.begin());
	std::vector<std::string>::const_iterator iter_e(spec_cont.end());

	while (iter_b != iter_e) {
		if (ParseCmdLineSocketSpec(*iter_b, current_index, argc, argv,
			out_datum, require_mc))
			return(true);
		++iter_b;
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineSocketSpec(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, SocketSpecLocal &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, tmp_string))
		return(false);

	try {
		SocketSpecLocal tmp_datum;
		tmp_datum.FromString(tmp_string);
		out_datum.swap(tmp_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "The '" + param_name + "' ip address " +
			"local specification string '" + tmp_string + "' is invalid: " +
			std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineSocketSpec(
	const std::vector<std::string> &spec_cont, unsigned int &current_index,
	int argc, char **argv, SocketSpecLocal &out_datum)
{
	std::vector<std::string>::const_iterator iter_b(spec_cont.begin());
	std::vector<std::string>::const_iterator iter_e(spec_cont.end());

	while (iter_b != iter_e) {
		if (ParseCmdLineSocketSpec(*iter_b, current_index, argc, argv,
			out_datum))
			return(true);
		++iter_b;
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineHostInterface(
	const std::vector<std::string> &param_name_list, unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	std::string tmp_datum;

	if (!ParseCmdLineFollowingSpec(param_name_list, current_index, argc, argv,
		tmp_datum))
		return(false);

	try {
		StringToIpAddress(tmp_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse the host interface value "
			"for the '" + std::string(argv[current_index - 1]) +
			"' parameter: " + std::string(except.what()));
	}

	out_datum.swap(tmp_datum);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineHostInterface(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, std::string &out_datum)
{
	return(ParseCmdLineHostInterface(std::vector<std::string>(1, param_name),
		current_index, argc, argv, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineHostInterface(
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineHostInterface(
		MLB::Utility::MakeInlineVector<std::string>
			("-NETWORK_HOST_INTERFACE")
			("-NETWORK_HOSTINTERFACE")
			("-NETWORKHOST_INTERFACE")
			("-NETWORKHOSTINTERFACE")
			("-NET_HOST_INTERFACE")
			("-NET_HOSTINTERFACE")
			("-NETHOST_INTERFACE")
			("-NETHOSTINTERFACE")
			("-NETWORK_INTERFACE")
			("-NETWORKINTERFACE")
			("-NET_INTERFACE")
			("-NETINTERFACE")
			("-HOST_INTERFACE")
			("-HOSTINTERFACE"),
			current_index, argc, argv, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineIpAddress(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, IpAddressGeneric &out_datum, bool ipv6_ok, bool mc_ok,
	bool require_mc)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, tmp_string))
		return(false);

	try {
		out_datum = ParseIpAddressFromString(tmp_string, ipv6_ok, mc_ok,
			require_mc);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attept to parse the '" + param_name +
			"' parameter failed: " + std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineIpAddress(
	const std::vector<std::string> &spec_cont, unsigned int &current_index,
	int argc, char **argv, IpAddressGeneric &out_datum, bool ipv6_ok,
	bool mc_ok, bool require_mc)
{
	std::vector<std::string>::const_iterator iter_b(spec_cont.begin());
	std::vector<std::string>::const_iterator iter_e(spec_cont.end());

	while (iter_b != iter_e) {
		if (ParseCmdLineIpAddress(*iter_b, current_index, argc, argv,
			out_datum, ipv6_ok, mc_ok, require_mc))
			return(true);
		++iter_b;
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineIpAddressList(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, IpAddressGenericList &out_datum, bool ipv6_ok, bool mc_ok,
	bool require_mc)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, tmp_string))
		return(false);

	try {
	  std::vector<std::string> tmp_list;
		MLB::Utility::SplitString(tmp_string, ",", tmp_list, 0, false);
		IpAddressGenericList                     tmp_datum;
		std::vector<std::string>::const_iterator iter_b(tmp_list.begin());
		std::vector<std::string>::const_iterator iter_e(tmp_list.end());
		for ( ; iter_b != iter_e; ++iter_b)
			tmp_datum.push_back(ParseIpAddressFromString(*iter_b, ipv6_ok,
				mc_ok, require_mc));
		out_datum = tmp_datum;
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse the list of IP "
			"addresses : " + std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketIoParseCmdLineArg::ParseCmdLineIpAddressList(
	const std::vector<std::string> &spec_cont, unsigned int &current_index,
	int argc, char **argv, IpAddressGenericList &out_datum, bool ipv6_ok,
	bool mc_ok, bool require_mc)
{
	std::vector<std::string>::const_iterator iter_b(spec_cont.begin());
	std::vector<std::string>::const_iterator iter_e(spec_cont.end());

	while (iter_b != iter_e) {
		if (ParseCmdLineIpAddressList(*iter_b, current_index, argc, argv,
			out_datum, ipv6_ok, mc_ok, require_mc))
			return(true);
		++iter_b;
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

