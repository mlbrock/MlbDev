//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for class VFastParseCmdLineArg.

	Revision History	:	2008-12-29 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <VFast/VFastParseCmdLineArg.hpp>

#include <Utility/InlineContainer.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLineXmlTemplateFile(
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-XML_TEMPLATE_FILE_NAME")
		("-XML_TEMPLATE_FILENAME")
		("-XML_TEMPLATEFILE_NAME")
		("-XMLTEMPLATE_FILE_NAME")
		("-XML_TEMPLATEFILENAME")
		("-XMLTEMPLATE_FILENAME")
		("-XMLTEMPLATEFILE_NAME")
		("-XMLTEMPLATEFILENAME")
		("-XML_TEMPLATE_FILE")
		("-XML_TEMPLATEFILE")
		("-XMLTEMPLATE_FILE")
		("-XMLTEMPLATEFILE")
		("-TEMPLATE_FILE_NAME")
		("-TEMPLATE_FILENAME")
		("-TEMPLATEFILE_NAME")
		("-TEMPLATEFILENAME")
		("-TEMPLATE_FILE")
		("-TEMPLATEFILE")
		("-XML_FILE_NAME")
		("-XML_FILENAME")
		("-XMLFILE_NAME")
		("-XMLFILENAME")
		("-XML_FILE")
		("-XMLFILE"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLinePacketDecodeRegime(
	unsigned int &current_index, int argc, char **argv,
	PacketDecodeRegime &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-PACKET_DECODE_REGIME")
		("-PACKET_DECODEREGIME")
		("-PACKETDECODE_REGIME")
		("-PACKETDECODEREGIME")
		("-PACKET_DECODE_TYPE")
		("-PACKET_DECODETYPE")
		("-PACKETDECODE_TYPE")
		("-PACKETDECODETYPE")
		("-DECODE_REGIME")
		("-DECODEREGIME")
		("-DECODE_TYPE")
		("-DECODETYPE")
		("-DECODE")
		("-CONTENT_TYPE")
		("-CONTENTTYPE")
		("-CONTENT"),
		current_index, argc, argv, tmp_string))
		return(false);

	out_datum = StringToPacketDecodeRegime(tmp_string);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLinePacketDecodeRegimeCme(
	unsigned int &current_index, int argc, char **argv,
	PacketDecodeRegime &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-PACKET_DECODE_REGIME")
		("-PACKET_DECODEREGIME")
		("-PACKETDECODE_REGIME")
		("-PACKETDECODEREGIME")
		("-PACKET_DECODE_TYPE")
		("-PACKET_DECODETYPE")
		("-PACKETDECODE_TYPE")
		("-PACKETDECODETYPE")
		("-DECODE_REGIME")
		("-DECODEREGIME")
		("-DECODE_TYPE")
		("-DECODETYPE")
		("-DECODE")
		("-CONTENT_TYPE")
		("-CONTENTTYPE")
		("-CONTENT"),
		current_index, argc, argv, tmp_string))
		return(false);

	out_datum = StringToPacketDecodeRegimeCme(tmp_string);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLineFirstPacketIndex(
	unsigned int &current_index, int argc, char **argv,
	unsigned int &out_datum)
{
	return(ParseCmdLineArg::ParseCmdLineNumeric(
		MLB::Utility::MakeInlineVector<std::string>
		("-FIRST_PACKET_INDEX")
		("-FIRST_PACKETINDEX")
		("-FIRSTPACKET_INDEX")
		("-FIRSTPACKETINDEX")
		("-FIRST_PACKET")
		("-FIRSTPACKET")
		("-PACKET_INDEX")
		("-PACKETINDEX")
		("-FIRST_INDEX")
		("-FIRSTINDEX"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLineMaxPacketCount(
	unsigned int &current_index, int argc, char **argv, unsigned int &out_datum)
{
	return(ParseCmdLineArg::ParseCmdLineNumeric(
		MLB::Utility::MakeInlineVector<std::string>
		("-MAXIMUM_PACKETS_COUNT")
		("-MAXIMUM_PACKETSCOUNT")
		("-MAXIMUMPACKETS_COUNT")
		("-MAXIMUMPACKETSCOUNT")
		("-MAXIMUM_PACKET_COUNT")
		("-MAXIMUM_PACKETCOUNT")
		("-MAXIMUMPACKET_COUNT")
		("-MAXIMUMPACKETCOUNT")
		("-MAXIMUM_PACKETS")
		("-MAXIMUMPACKETS")
		("-MAXIMUM_PACKET")
		("-MAXIMUMPACKET")
		("-MAXIMUM_PKTS_COUNT")
		("-MAXIMUM_PKTSCOUNT")
		("-MAXIMUMPKTS_COUNT")
		("-MAXIMUMPKTSCOUNT")
		("-MAXIMUM_PKT_COUNT")
		("-MAXIMUM_PKTCOUNT")
		("-MAXIMUMPKT_COUNT")
		("-MAXIMUMPKTCOUNT")
		("-MAXIMUM_PKTS")
		("-MAXIMUMPKTS")
		("-MAXIMUM_PKT")
		("-MAXIMUMPKT")
		("-MAX_PACKETS_COUNT")
		("-MAX_PACKETSCOUNT")
		("-MAXPACKETS_COUNT")
		("-MAXPACKETSCOUNT")
		("-MAX_PACKET_COUNT")
		("-MAX_PACKETCOUNT")
		("-MAXPACKET_COUNT")
		("-MAXPACKETCOUNT")
		("-MAX_PACKETS")
		("-MAXPACKETS")
		("-MAX_PACKET")
		("-MAXPACKET")
		("-MAX_PKTS_COUNT")
		("-MAX_PKTSCOUNT")
		("-MAXPKTS_COUNT")
		("-MAXPKTSCOUNT")
		("-MAX_PKT_COUNT")
		("-MAX_PKTCOUNT")
		("-MAXPKT_COUNT")
		("-MAXPKTCOUNT")
		("-MAX_PKTS")
		("-MAXPKTS")
		("-MAX_PKT")
		("-MAXPKT"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLineMaxMessageCount(
	unsigned int &current_index, int argc, char **argv, unsigned int &out_datum)
{
	return(ParseCmdLineArg::ParseCmdLineNumeric(
		MLB::Utility::MakeInlineVector<std::string>
		("-MAXIMUM_MESSAGES_COUNT")
		("-MAXIMUM_MESSAGESCOUNT")
		("-MAXIMUMMESSAGES_COUNT")
		("-MAXIMUMMESSAGESCOUNT")
		("-MAXIMUM_MESSAGE_COUNT")
		("-MAXIMUM_MESSAGECOUNT")
		("-MAXIMUMMESSAGE_COUNT")
		("-MAXIMUMMESSAGECOUNT")
		("-MAXIMUM_MESSAGES")
		("-MAXIMUMMESSAGES")
		("-MAXIMUM_MESSAGE")
		("-MAXIMUMMESSAGE")
		("-MAXIMUM_MSGS_COUNT")
		("-MAXIMUM_MSGSCOUNT")
		("-MAXIMUMMSGS_COUNT")
		("-MAXIMUMMSGSCOUNT")
		("-MAXIMUM_MSG_COUNT")
		("-MAXIMUM_MSGCOUNT")
		("-MAXIMUMMSG_COUNT")
		("-MAXIMUMMSGCOUNT")
		("-MAXIMUM_MSGS")
		("-MAXIMUMMSGS")
		("-MAXIMUM_MSG")
		("-MAXIMUMMSG")
		("-MAX_MESSAGES_COUNT")
		("-MAX_MESSAGESCOUNT")
		("-MAXMESSAGES_COUNT")
		("-MAXMESSAGESCOUNT")
		("-MAX_MESSAGE_COUNT")
		("-MAX_MESSAGECOUNT")
		("-MAXMESSAGE_COUNT")
		("-MAXMESSAGECOUNT")
		("-MAX_MESSAGES")
		("-MAXMESSAGES")
		("-MAX_MESSAGE")
		("-MAXMESSAGE")
		("-MAX_MSGS_COUNT")
		("-MAX_MSGSCOUNT")
		("-MAXMSGS_COUNT")
		("-MAXMSGSCOUNT")
		("-MAX_MSG_COUNT")
		("-MAX_MSGCOUNT")
		("-MAXMSG_COUNT")
		("-MAXMSGCOUNT")
		("-MAX_MSGS")
		("-MAXMSGS")
		("-MAX_MSG")
		("-MAXMSG"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLineIterationCount(
	unsigned int &current_index, int argc, char **argv, unsigned int &out_datum)
{
	return(ParseCmdLineArg::ParseCmdLineNumeric
		<std::vector<std::string>, unsigned int>(
		MLB::Utility::MakeInlineVector<std::string>
		("-ITERATION_COUNT")
		("-ITERATIONCOUNT")
		("-ITERATIONS")
		("-ITERATION")
		("-CYCLE_COUNT")
		("-CYCLECOUNT")
		("-COUNT"),
		current_index, argc, argv, out_datum, 1));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLineDebugFlag(unsigned int &current_index,
	int argc, char **argv, bool &out_datum)
{
	return(ParseCmdLineArg::ParseCmdLineDatumSpec(
		MLB::Utility::MakeInlineVector<std::string>
		("-DEBUGGING_FLAG")
		("-DEBUGGINGFLAG")
		("-DEBUG_FLAG")
		("-DEBUGFLAG")
		("-DEBUGGING")
		("-DEBUG"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VFastParseCmdLineArg::ParseCmdLineElapsedTimer(unsigned int &current_index,
	int argc, char **argv, bool &out_datum)
{
	return(ParseCmdLineArg::ParseCmdLineDatumSpec(
		MLB::Utility::MakeInlineVector<std::string>
		("-ELAPSED_TIMER")
		("-ELAPSEDTIMER")
		("-ELAPSED_TIME")
		("-ELAPSEDTIME")
		("-TIMER"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

