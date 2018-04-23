// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Program
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of a Tib/Rv listener program which
								provides functionality enhanced beyond 'tibrvlisten'.

	Revision History	:	2001-10-17 --- Creation.
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
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <RvUtilX/RvCallback.hpp>
#include <RvUtilX/RvParseCmdLineArg.hpp>

#include <map>
#include <iomanip>

#ifdef _Windows
# pragma warning(disable:4217 4668)
# include <boost/shared_ptr.hpp>
# pragma warning(default:4217 4668)
#else
# include <boost/shared_ptr.hpp>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
typedef unsigned long long QuantityType;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
enum DisplayFlags {
	None      = 0x0000,
	TimeUTC   = 0x0001,
	XmlOutput = 0x0002,
	XmlOnly   = 0x0004,
	NamesOnly = 0x0008,
	Default   = None
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class XRvListen {
public:
	XRvListen() :
		 rv_context_()
		,flags_(None)
		,msg_data_flags_(static_cast<RvMsgToStringFlags>(
			RvMsgToStringFlags_MsgDepth |
			RvMsgToStringFlags_MsgFieldIndex |
			RvMsgToStringFlags_FieldTypeId |
			RvMsgToStringFlags_FieldTypeName |
			RvMsgToStringFlags_FieldId |
			RvMsgToStringFlags_ArrayCount |
			RvMsgToStringFlags_ArrayIndex |
			RvMsgToStringFlags_FieldCommaSep |
			RvMsgToStringFlags_ArrayCommaSep |
			RvMsgToStringFlags_DateFieldNSecs |
			RvMsgToStringFlags_ExpandXml |
			RvMsgToStringFlags_ExpandOpaque |
			RvMsgToStringFlags_DefaultFPPrec))
		,max_displayed_field_length_(-1)
		,snd_spec_()
		,subject_list_()
		,exclude_list_()
		,total_received_(0)
		,total_excluded_(0)
		,queue_()
		,transport_()
		,listener_list_()
		,callback_data_(GetThisRef())
	{
	}

	void ParseCmdLine(int argc, char **argv);
	void Start(const std::string &my_name);
	void ProcessEventOnMsg(TibrvMsgCallback *, TibrvListener *listener,
		TibrvMsg &msg_recv);

	void XmlEmitMsg(TibrvMsg &msg_recv,
		const std::string &field_name_base = "<ROOT MESSAGE>") const;
	void XmlEmitComment(const std::string &in_string) const;
	void XmlEmitSepartor(char sep_char) const;

	void ReportException(const std::exception *except_ptr) {
		std::cerr << "Exception encountered in Tib/Rendezvous callback: " <<
			((except_ptr != NULL) ? except_ptr->what() : "*UNSPECIFIED*") <<
			std::endl;
	}

	RvContext                                     rv_context_;
	DisplayFlags                                  flags_;
	RvMsgToStringFlags                            msg_data_flags_;
	int                                           max_displayed_field_length_;
	MLB::RvUtilX::SNDSpec                         snd_spec_;
	MLB::RvUtilX::SubjectNameFlatSet              subject_list_;
	MLB::RvUtilX::SubjectNameFlatSet              exclude_list_;
	QuantityType                                  total_received_;
	QuantityType                                  total_excluded_;
	TibrvQueue                                    queue_;
	TibrvNetTransport                             transport_;
	std::deque<boost::shared_ptr<TibrvListener> > listener_list_;
	MLB::RvUtilX::RvCallbackMsgEvent<XRvListen>   callback_data_;

	XRvListen &GetThisRef() {
		return(*this);
	}

	MLB_Utility_NonCopyable_Macro(XRvListen);
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void XRvListen::ParseCmdLine(int argc, char **argv)
{
	unsigned int count_1;
	std::string  subject_name;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (RvParseCmdLineArg::IsCmdLineArg("-UTC", count_1, argc, argv))
			flags_ = static_cast<DisplayFlags>(static_cast<int>(flags_) |
				static_cast<int>(TimeUTC));
		else if (RvParseCmdLineArg::IsCmdLineArg("-XML", count_1, argc, argv))
			flags_ = static_cast<DisplayFlags>(static_cast<int>(flags_) |
				static_cast<int>(XmlOutput));
		else if (RvParseCmdLineArg::IsCmdLineArg("-XML_ONLY", count_1,
			argc, argv))
			flags_ = static_cast<DisplayFlags>(static_cast<int>(flags_) |
				static_cast<int>(XmlOnly));
		else if (RvParseCmdLineArg::IsCmdLineArg("-NAMES_ONLY", count_1,
			argc, argv))
			flags_ = static_cast<DisplayFlags>(static_cast<int>(flags_) |
				static_cast<int>(NamesOnly));
		else if (snd_spec_.ParseCmdLineServiceNetworkDaemon(count_1, argc,
			argv))
			;
		else if (RvParseCmdLineArg::ParseCmdLineSubject("-EXCLUDE", count_1,
			argc, argv, subject_name))
			exclude_list_.insert(subject_name);
		else if (RvParseCmdLineArg::ParseCmdLineNumeric(
			MLB::Utility::MakeInlineVector<std::string>
				("-MAXIMUM_FIELD_LENGTH")
				("-MAXIMUM_FIELDLENGTH")
				("-MAXIMUMFIELD_LENGTH")
				("-MAXIMUMFIELDLENGTH")
				("-MAXIMUM_FIELD_LEN")
				("-MAXIMUM_FIELDLEN")
				("-MAXIMUMFIELD_LEN")
				("-MAXIMUMFIELDLEN")
				("-MAX_FIELD_LENGTH")
				("-MAX_FIELDLENGTH")
				("-MAXFIELD_LENGTH")
				("-MAXFIELDLENGTH")
				("-MAX_FIELD_LEN")
				("-MAX_FIELDLEN")
				("-MAXFIELD_LEN")
				("-MAXFIELDLEN")
				("-MAXIMUM_FIELD")
				("-MAXIMUMFIELD")
				("-MAX_FIELD")
				("-MAXFIELD"),
			count_1, argc, argv, max_displayed_field_length_, -1))
				;
		else if (RvParseCmdLineArg::ParseCmdLineSubject(argv[count_1],
			subject_name))
			subject_list_.insert(subject_name);
	}

	//	Must have at least one subject name pattern...
	if (subject_list_.empty())
		MLB::Utility::ThrowInvalidArgument("No subject names specified.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void XRvListen::Start(const std::string &my_name)
{
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.create,
		());
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setLimitPolicy,
		(TIBRVQUEUE_DISCARD_NONE, 0, 0));
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setPriority,
		(2));

	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setName,
		((my_name + ": Queue").c_str()));

	//	Create the transport...
	snd_spec_.CreateTransport(transport_);
	RvUtilX_THROW_TIBRV_STATUS_IF(transport_.setDescription,
		((my_name + ": Transport on host " +
		MLB::Utility::GetHostNameCanonical() + ", PID " +
		MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId())).c_str()));

	//	Subscribe to the subjects of interest...
	MLB::RvUtilX::SubjectNameFlatSetIterC iter_b(subject_list_.begin());
	MLB::RvUtilX::SubjectNameFlatSetIterC iter_e(subject_list_.end());
	while (iter_b != iter_e) {
		boost::shared_ptr<TibrvListener> the_listener(new TibrvListener);
		RvUtilX_THROW_TIBRV_STATUS_IF(the_listener->create,
			(&queue_, &callback_data_, &transport_,
			iter_b->ToString().c_str()));
		std::cerr << my_name << ": Listening to subject " <<
			iter_b->ToString() << std::endl;
		listener_list_.push_back(the_listener);
		++iter_b;
	}

	//	Dispatch RV events...
	TibrvStatus rv_status;
	do {
		rv_status = queue_.timedDispatch(0.1);
// CODE NOTE: Test code. To be removed.
rv_status = Tibrv::defaultQueue()->timedDispatch(0.1);
		if (MLB::Utility::CriticalEventTest()) {
			std::ostringstream log_text;
			std::cerr << my_name << " is performing an asynchronous exit " <<
				"due to receipt of a signal.";
			break;
		}
	} while ((rv_status == TIBRV_OK) || (rv_status == TIBRV_TIMEOUT));

	if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
		throw RvExceptionStatus(rv_status,
			"Error occurred while dispatching from a Rendezvous queue.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void XRvListen::ProcessEventOnMsg(TibrvMsgCallback *,
	TibrvListener *listener_ptr, TibrvMsg &msg_recv)
{
	if (MLB::Utility::CriticalEventTest()) {
		listener_ptr->destroy();
		return;
	}

	MLB::Utility::TimeSpec recv_time;

	MLB::Utility::IncrementToMax(total_received_);

	std::string send_subject(MLB::RvUtilX::GetSendSubject(msg_recv));

// CODE NOTE: Test code. To be removed.
std::cout << "***** [" << send_subject << "] *****" << std::endl;

	if (!exclude_list_.empty()) {
		MLB::RvUtilX::SubjectNameFlat         match_name(send_subject);
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_b(exclude_list_.begin());
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_e(exclude_list_.end());
		while (iter_b != iter_e) {
			if (iter_b->MatchSubjectName(match_name)) {
				MLB::Utility::IncrementToMax(total_excluded_);
				return;
			}
			++iter_b;
		}
	}

	if (flags_ & NamesOnly)
		std::cout << "[" << ((flags_ & TimeUTC) ? recv_time.ToString() :
			recv_time.ToStringLocal()) << "]: subject=" << send_subject <<
			std::endl;
	else if (flags_ & XmlOutput) {
		std::string reply_subject(MLB::RvUtilX::GetReplySubject(msg_recv));
		XmlEmitSepartor('*');
		XmlEmitComment(std::string("MESSAGE TIME : ") +
			((flags_ & TimeUTC) ? recv_time.ToString() :
			recv_time.ToStringLocal()));
		XmlEmitComment(std::string("SEND SUBJECT : ") + send_subject);
		XmlEmitComment(std::string("REPLY SUBJECT: ") + reply_subject);
		XmlEmitSepartor('=');
		XmlEmitMsg(msg_recv);
	}
	else {
		std::string reply_subject(MLB::RvUtilX::GetReplySubject(msg_recv));
		std::cout << "[" << ((flags_ & TimeUTC) ?
			recv_time.ToString(MLB::Utility::Length_TimeT) :
			recv_time.ToStringLocal(MLB::Utility::Length_TimeT)) <<
			"]: subject=" << send_subject << ((reply_subject.empty()) ? "" :
			", reply=") << reply_subject << ", message=" <<
			RvMsgToString(msg_recv, msg_data_flags_) << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void XRvListen::XmlEmitMsg(TibrvMsg &msg_recv,
	const std::string &field_name_base) const
{
	tibrv_u32 field_count;

	RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getNumFields,
		(field_count));

	tibrv_u32 count_1;
	for (count_1 = 0; count_1 < field_count; count_1++) {
		TibrvMsgField msg_field;
		RvMsgSnapshotContext rvmsg_snapshot_context(msg_recv);
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getFieldByIndex,
			(msg_field, count_1));
		if (msg_field.getType() == TIBRVMSG_XML) {
			if (field_name_base != "<ROOT MESSAGE>")
				XmlEmitSepartor('_');
			const char *field_name = msg_field.getName();
			XmlEmitComment("XML Field: " + field_name_base + "." +
				((field_name != NULL) ? field_name : "") + "{" +
				MLB::Utility::AnyToString(msg_field.getId()) + "}");
			std::string xml_string(
				static_cast<const char *>(msg_field.getData().buf),
				(max_displayed_field_length_ < 0) ? msg_field.getSize() :
				std::min<int>(max_displayed_field_length_,
				msg_field.getSize()));
			std::cout << xml_string << std::endl;
		}
		else if (msg_field.getType() == TIBRVMSG_MSG) {
			const char *field_name = msg_field.getName();
			TibrvMsg inner_msg(msg_field.getData().msg, TIBRV_FALSE);
			XmlEmitMsg(inner_msg, field_name_base + "." +
				((field_name != NULL) ? field_name : "") + "{" +
				MLB::Utility::AnyToString(msg_field.getId()) + "}");
		}
		else if (!(flags_ & XmlOnly)) {
			if (field_name_base != "<ROOT MESSAGE>")
				XmlEmitSepartor('_');
			const char *field_name = msg_field.getName();
			XmlEmitComment("Non-XML Field: " + field_name_base + "." +
				((field_name != NULL) ? field_name : "") + "{" +
				MLB::Utility::AnyToString(msg_field.getId()) + "}");
			std::string data_string(RvMsgFieldDataToString(msg_field,
				msg_data_flags_));
			if ((max_displayed_field_length_ < 0) ||
				(max_displayed_field_length_ >=
				static_cast<int>(data_string.size())))
				std::cout << data_string << std::endl;
			else
				std::cout << std::left << std::setw(max_displayed_field_length_) <<
					data_string << std::right << std::endl;
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void XRvListen::XmlEmitComment(const std::string &in_string) const
{
	std::cout << "<!-- " << in_string << " -->" << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void XRvListen::XmlEmitSepartor(char sep_char) const
{
	std::cout << "<!-- " << std::setfill(sep_char) << std::setw(70) << "" <<
		std::setfill(' ') << " -->" << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code = EXIT_SUCCESS;
	std::string  my_name     = MLB::Utility::GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (RvParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -utc ] " <<
			"[ -names_only ] " <<
			"[ -xml ] " <<
			"[ -xml_only ] " <<
			"[ -exclude <subject-name-pattern> ] " <<
			"[ -max_field_length <maximum-field-length-to-display> ] " <<
			"[ -service <RV-service> ] " <<
			"[ -network <RV-network> ] " <<
			"[ -daemon <RV-daemon> ] " <<
			"<subject-name-pattern> [ <subject-name-pattern> ...] " <<
			" *** NOTE: The <subject-name-pattern> parameter(s) must be last " <<
			"on the command line.\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -utc\n\
      Specifies that the timestamp for messages received is emitted in\n\
      UTC/GTM.\n\n\
   -names_only\n\
      Specifies that only subject name information is to be emitted (the\n\
      default is the entire message).\n\n\
   -xml\n\
      Specifies that messages which contain XML fields are to have those\n\
      fields emitted as XML text.\n\n\
   -xml_only\n\
      Specifies that only XML fields are to be emitted. Implies the '-xml'\n\
      flag.\n\n\
   -exclude <subject-name-pattern>\n\
      Exclude <subject-name-pattern> from message capture.\n\n\
   -max_field_length <maximum-field-length-to-display>\n\
      Specifies the maximum length of a field to be displayed for a scalar\n\
      message field in bytes. The default is -1, which means that no length\n\
      restriction is to be imposed.\n\n\
   -service <RV-service>\n\
      The Tib/Rendezvous service for message capture.\n\n\
   -network <RV-network>\n\
      The Tib/Rendezvous network for message capture.\n\n\
   -daemon <RV-daemon>\n\
      The Tib/Rendezvous daemon for message capture.\n\n\
   <subject-name-pattern> [ <subject-name-pattern> ... ]\n\
      Specifies the subject names to be captured." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		XRvListen                          measure_control;
		measure_control.ParseCmdLine(argc, argv);
		if ((measure_control.flags_ & XmlOutput) &&
			(!(measure_control.flags_ & NamesOnly)))
			std::cout << "<XRvListenEnvelope>" << std::endl;
		measure_control.Start(my_name);
		if ((measure_control.flags_ & XmlOutput) &&
			(!(measure_control.flags_ & NamesOnly)))
			std::cout << "</XRvListenEnvelope>" << std::endl;
		std::cerr << std::endl;
		std::cerr << "Messages Received: " <<
			std::setw(MLB::Utility::IntegralValueMaxLengthDec<QuantityType>()) <<
			measure_control.total_received_ << std::endl;
		std::cerr << "Messages Excluded: " <<
			std::setw(MLB::Utility::IntegralValueMaxLengthDec<QuantityType>()) <<
			measure_control.total_excluded_ << std::endl << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << my_name << ": fatal error encountered: " <<
			except.what() << std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

