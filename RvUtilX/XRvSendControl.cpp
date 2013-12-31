// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for the XRvSendControl class.

	Revision History	:	2005-07-04 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Sleep.hpp>
#include <Utility/PathName.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/CriticalEventHandler.hpp>

#include <RvUtilX/XRvSendControl.hpp>

#include <RvUtilX/AddRvField.hpp>
#include <RvUtilX/RvParseCmdLineArg.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
XRvSendControl::XRvSendControl()
	:my_name_()
	,field_name_("DATA")
	,field_id_(0)
	,field_type_(TIBRVMSG_STRING)
	,subject_name_()
	,reply_subject_()
	,wait_for_reply_flag_(false)
	,reply_delay_(1000)
	,field_data_()
	,file_flag_(false)
	,snd_spec_()
	,repeat_count_(1)
	,delay_ms_(0)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool XRvSendControl::CheckCmdLineForHelpRequest(int argc, char **argv)
{
	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (RvParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -field_name <RV-field-name> ( = \"" << field_name_ << "\" ) ] " <<
			"[ -field_id <RV-field-id> ( = " << field_id_ << " ) ] " <<
			"[ -field_type <RV-field-type> ( = '" <<
				MsgFieldTypeToString(field_type_) << " ) ] " <<
			"[ -repeat <repeat-count> ( = " << repeat_count_ << " ) ] " <<
			"[ -delay <milliseconds-between-repeats> ( = " << delay_ms_ <<
				" ) ] " <<
			"[ -reply_subject <subject-name> ] " <<
			"[ -wait_for_reply <boolean> ( = " <<
				MLB::Utility::AnyToString(wait_for_reply_flag_) << " ) ] " <<
			"[ -reply_delay <milliseconds-to-wait> ( = " << reply_delay_ <<
				" ) ] " <<
			"[ -file <file-name> ] " <<
			"[ -service <RV-service> ] " <<
			"[ -network <RV-network> ] " <<
			"[ -daemon <RV-daemon> ] " <<
			"<subject-name> " <<
			"<data-text>\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -field_name <RV-field-name>\n\
      The name of the Rendezvous message field of the message text.\n\n\
   -field_id <RV-field-id>\n\
      The id of the Rendezvous message field of the message text.\n\n\
   -field_type <RV-field-type>\n\
      The type of the Rendezvous field data to be sent. Can be 'STRING',\n\
      'XML' or 'OPAQUE'.\n\n\
   -repeat <repeat-count>\n\
      The number of times the message is to be sent.\n\n\
   -delay <milliseconds-between-repeats>\n\
      The time in milliseconds to delay between multiple message sends.\n\n\
   -reply_subject <subject-name>\n\
      The reply subject to be included in the message.\n\n\
   -wait_for_reply <boolean>\n\n\
      Specifies that the program is to wait until a reply is received. The\n\
      '-reply_subject' parameter must also be specified in order use this\n\
      feature.\n\n\
   -reply_delay <milliseconds-to-wait>\n\
      Specifies the maximum amount of time in milliseconds to wait for a\n\
      reply. The '-reply_wait' parameter must be specified as true in order\n\
      to use this feature.\n\n\
   -file <file-name>\n\
      The name of a file which contains the data to be sent.\n\n\
   -service <RV-service>\n\
      The Tib/Rendezvous service for sending the message.\n\n\
   -network <RV-network>\n\
      The Tib/Rendezvous network for sending the message.\n\n\
   -daemon <RV-daemon>\n\
      The Tib/Rendezvous daemon for sending the message.\n\n\
   <subject-name>\n\
      The subject name on which the XML message is to be sent.\n\n\
   <data-text>\n\
      The text to be sent. Must be specified if the '-file' parameter is not\n\
      used." <<
			std::endl << std::endl;
		return(true);
	}
	// //////////////////////////////////////////////////////////////////////

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void XRvSendControl::ParseCmdLine(int argc, char **argv)
{
	unsigned int count_1;
	std::string  tmp_field_type;
	std::string  tmp_string;
	bool         field_data_specified = false;
	bool         trailing_count       = 0;

	my_name_ = MLB::Utility::GetFileNamePortion(argv[0]);

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (snd_spec_.ParseCmdLineServiceNetworkDaemon(count_1, argc,
			argv))
			;
		else if (RvParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-FIELD_NAME")
				("-FIELDNAME"),
				count_1, argc, argv, field_name_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-FIELD_IDENTIFIER")
				("-FIELDIDENTIFIER")
				("-FIELD_ID")
				("-FIELDID"),
				count_1, argc, argv, field_id_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-FIELD_TYPE")
				("-FIELDTYPE"),
				count_1, argc, argv, tmp_field_type)) {
			field_type_ = StringToMsgFieldType(tmp_field_type);
			if ((field_type_ != TIBRVMSG_OPAQUE) &&
				(field_type_ != TIBRVMSG_STRING) &&
				(field_type_ != TIBRVMSG_XML))
				MLB::Utility::ThrowInvalidArgument("Invalid Tib/Rendezvous field "
					"type specified ('" + tmp_field_type + "'): Expected one of "
					"'STRING', 'XML' or 'OPAQUE'.");
		}
		else if (RvParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-FILE")
				("-FILE_NAME")
				("-FILENAME"),
				count_1, argc, argv, tmp_string)) {
				LoadFile(tmp_string);
				file_flag_           = true;
				field_data_specified = true;
				std::cerr << "Loaded " <<
					static_cast<unsigned int>(field_data_.size()) << " bytes from "
					"file '" << tmp_string << "'" << std::endl;
		}
		else if (RvParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-REPLY_SUBJECT_NAME")
				("-REPLY_SUBJECTNAME")
				("-REPLYSUBJECT_NAME")
				("-REPLYSUBJECTNAME")
				("-REPLY_SUBJECT")
				("-REPLYSUBJECT")
				("-REPLY_NAME")
				("-REPLYNAME"), count_1, argc, argv, tmp_string)) {
			try {
				SubjectNameFlat tmp_name(tmp_string);
				tmp_name.CheckReplyName();
				reply_subject_ = tmp_string;
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except, "Invalid reply subject name: " +
					std::string(except.what()));
			}
		}
		else if (RvParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-WAIT_FOR_REPLY")
				("-WAIT_FORREPLY")
				("-WAITFOR_REPLY")
				("-WAITFORREPLY"),
				count_1, argc, argv, wait_for_reply_flag_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-REPLY_DELAY_MILLISECONDS")
				("-REPLY_DELAYMILLISECONDS")
				("-REPLYDELAY_MILLISECONDS")
				("-REPLYDELAYMILLISECONDS")
				("-REPLY_DELAY_MILLISECS")
				("-REPLY_DELAYMILLISECS")
				("-REPLYDELAY_MILLISECS")
				("-REPLYDELAYMILLISECS")
				("-REPLY_DELAY_MS")
				("-REPLY_DELAYMS")
				("-REPLYDELAY_MS")
				("-REPLYDELAYMS")
				("-REPLY_DELAY")
				("-REPLYDELAY"),
				count_1, argc, argv, reply_delay_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineNumeric(
			MLB::Utility::MakeInlineVector<std::string>
				("-REPEAT_COUNT")
				("-REPEATCOUNT")
				("-REPEAT"), count_1, argc, argv, repeat_count_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-DELAY_MILLISECONDS")
				("-DELAYMILLISECONDS")
				("-DELAY_MILLISECS")
				("-DELAYMILLISECS")
				("-DELAY_MS")
				("-DELAYMS")
				("-DELAY"), count_1, argc, argv, delay_ms_))
			;
		else if (!trailing_count) {
			RvParseCmdLineArg::ParseCmdLineSubjectFull(argv[count_1],
				subject_name_);
			++trailing_count;
		}
		else if ((trailing_count == 1) && (!file_flag_)) {
			field_data_          = argv[count_1];
			field_data_specified = true;
			++trailing_count;
		}
		else
			RvParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	//	Must have at least one subject name pattern...
	if (subject_name_.empty())
		MLB::Utility::ThrowInvalidArgument("No subject name specified.");

	//	Must have a field fname...
	if (field_name_.empty())
		MLB::Utility::ThrowInvalidArgument("No field name specified.");

	//	Must have some data to send (but the data may be an empty string)...
	if (!field_data_specified)
		MLB::Utility::ThrowInvalidArgument("No field data content specified.");
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void XRvSendControl::LoadFile(const std::string &file_name)
{
	try {
		MLB::Utility::ReadFileData(file_name, field_data_);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to read Tib/Rendezvous message "
			"contents file: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void XRvSendControl::SendMessage()
{
	RvContext                          rv_context;
	MLB::Utility::CriticalEventContext critical_event_context;
	TibrvNetTransport                  transport;

	//	Create the transport...
	snd_spec_.CreateTransport(transport);
	RvUtilX_THROW_TIBRV_STATUS_IF(transport.setDescription,
		((my_name_ + ": Transport").c_str()));

	TibrvMsg msg_send;

	if (field_type_ == TIBRVMSG_OPAQUE)
		AddRvFieldOpaque(msg_send, field_name_, field_data_, field_id_);
	else if (field_type_ == TIBRVMSG_STRING)
		AddRvField(msg_send, field_name_, field_data_, field_id_);
	else if (field_type_ == TIBRVMSG_XML)
		AddRvFieldXml(msg_send, field_name_, field_data_, field_id_);

	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
		(subject_name_.c_str()));

	if (!reply_subject_.empty()) {
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setReplySubject,
			(reply_subject_.c_str()));
	}
	else if (wait_for_reply_flag_) {
		char inbox_name[MaxSubjectSize + 1];
		RvUtilX_THROW_TIBRV_STATUS_IF(transport.createInbox,
			(inbox_name, sizeof(inbox_name) - 1));
		reply_subject_ = inbox_name;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setReplySubject,
			(reply_subject_.c_str()));
	}

	std::cout << "Will send message " << repeat_count_ << " time" <<
		((repeat_count_ == 1) ? "" : "s") << "..." << std::endl;
	if (wait_for_reply_flag_)
		std::cout << "Will wait for replies on subject " << reply_subject_ <<
			" for a maximum of " << reply_delay_ << " milliseconds." << std::endl;
	std::cout << std::endl;

	unsigned int count_1;
	for (count_1 = 0; count_1 < repeat_count_; ++count_1) {
		if (wait_for_reply_flag_) {
			TibrvMsg msg_reply;
			SendRequestSpecificReplyThrow(transport, msg_send, msg_reply,
				static_cast<double>(reply_delay_ / 1000));
			std::cout << "REPLY MESSAGE: [" <<
				MLB::Utility::TimeVal().ToString(MLB::Utility::Length_TimeT) <<
				"]: subject=" << reply_subject_ << ", message={" <<
				RvMsgToString(msg_reply, RvMsgToStringFlags_Default) << "}" <<
				std::endl << std::endl;
		}
		else
			RvUtilX_THROW_TIBRV_STATUS_IF(transport.send,
				(msg_send));
		if (MLB::Utility::CriticalEventTest())
			break;
		if ((count_1 + 1) < repeat_count_)
			MLB::Utility::SleepMilliSecs(delay_ms_);
	}

	if (MLB::Utility::CriticalEventTest())
		std::cout << "Exiting due to receipt of a signal." << std::endl;
	else
		std::cout << "Send complete!" << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // MLB

