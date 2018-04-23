//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for class SockLibXParseCmdLineArg.

	Revision History	:	2002-01-26 --- Creation
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

#include <SockLibX/ParseCmdLineArg.hpp>

#include <Utility/InlineContainer.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLineHost(unsigned int &current_index,
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
bool SockLibXParseCmdLineArg::ParseCmdLineHostResolved(
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	std::string tmp_datum;

	if (!ParseCmdLineHost(current_index, argc, argv, tmp_datum))
		return(false);

	char tmp_host_name[MLB::Utility::MaxHostNameLength + 1];
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_ResolveHostToName(tmp_datum.c_str(), tmp_host_name,
		npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXException(npsl_error_text);

	out_datum = tmp_host_name;

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLineService(unsigned int &current_index,
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
bool SockLibXParseCmdLineArg::ParseCmdLineServiceResolved(
	unsigned int &current_index, int argc, char **argv, std::string &out_datum,
	const std::string &protocol)
{
	std::string tmp_datum;

	if (!ParseCmdLineService(current_index, argc, argv, tmp_datum))
		return(false);

	int  tmp_port;
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_ResolveServToPort(tmp_datum.c_str(),
		(protocol.empty()) ? NULL : protocol.c_str(),
		&tmp_port, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXException(npsl_error_text);

	out_datum = MLB::Utility::AnyToString(tmp_port);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLinePort(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineService(current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLinePortResolved(
	unsigned int &current_index, int argc, char **argv, std::string &out_datum,
	const std::string &protocol)
{
	return(ParseCmdLineServiceResolved(current_index, argc, argv, out_datum,
		protocol));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLineBufferSizeRecv(
	unsigned int &current_index, int argc, char **argv, unsigned int &out_datum)
{
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
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLineBufferSizeSend(
	unsigned int &current_index, int argc, char **argv, unsigned int &out_datum)
{
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
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLineSocketDebugFlag(
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
bool SockLibXParseCmdLineArg::ParseCmdLineDontLingerFlag(
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
bool SockLibXParseCmdLineArg::ParseCmdLineDontRouteFlag(
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
bool SockLibXParseCmdLineArg::ParseCmdLineKeepAliveFlag(
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
bool SockLibXParseCmdLineArg::ParseCmdLineOobInlineFlag(
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
bool SockLibXParseCmdLineArg::ParseCmdLineTcpNoDelayFlag(
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

// ////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLineSocketSpec(
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
			std::string((require_mc) ? "' multicast " : "' ip address ") +
			"specification string '" + tmp_string + "' is invalid: " +
			std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SockLibXParseCmdLineArg::ParseCmdLineSocketSpec(
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

} // namespace SockLibX

} // namespace MLB

