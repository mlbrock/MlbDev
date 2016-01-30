// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Get send and reply subjects from a TibrvMsg object.

	Revision History	:	2004-12-30 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
const char *GetSendSubjectPtr(const TibrvMsg &msg)
{
	const char  *subject_name;
	TibrvStatus  rv_status =
		const_cast<TibrvMsg &>(msg).getSendSubject(subject_name);

	if (rv_status == TIBRV_OK)
		return(subject_name);

	//	Returned by the RV library when a message has no send subject.
	if (rv_status != TIBRV_NOT_FOUND)
		MLB::RvUtilX::ThrowTibrvStatus(rv_status,
			"Attempt to retrieve send subject failed.");

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *GetReplySubjectPtr(const TibrvMsg &msg)
{
	const char  *subject_name;
	TibrvStatus  rv_status =
		const_cast<TibrvMsg &>(msg).getReplySubject(subject_name);

	if (rv_status == TIBRV_OK)
		return(subject_name);

	//	Returned by the RV library when a message has no reply subject.
	if (rv_status != TIBRV_NOT_FOUND)
		MLB::RvUtilX::ThrowTibrvStatus(rv_status,
			"Attempt to retrieve reply subject failed.");

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &GetSendSubject(const TibrvMsg &msg, std::string &out_subject)
{
	const char *subject_name = GetSendSubjectPtr(msg);

	out_subject = (subject_name != NULL) ? subject_name : "";

	return(out_subject);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &GetReplySubject(const TibrvMsg &msg, std::string &out_subject)
{
	const char *subject_name = GetReplySubjectPtr(msg);

	out_subject = (subject_name != NULL) ? subject_name : "";

	return(out_subject);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetSendSubject(const TibrvMsg &msg)
{
	std::string out_subject;

	return(GetSendSubject(msg, out_subject));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetReplySubject(const TibrvMsg &msg)
{
	std::string out_subject;

	return(GetReplySubject(msg, out_subject));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvMsg &SetReplySubject(TibrvMsg &msg, const char *subject_name)
{
	RvUtilX_THROW_TIBRV_STATUS_IF(msg.setReplySubject,
		(subject_name));

	return(msg);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvMsg &SetReplySubject(TibrvMsg &msg, const std::string &subject_name)
{
	return(SetReplySubject(msg, subject_name.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvMsg &SetSendSubject(TibrvMsg &msg, const char *subject_name)
{
	RvUtilX_THROW_TIBRV_STATUS_IF(msg.setSendSubject,
		(subject_name));

	return(msg);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvMsg &SetSendSubject(TibrvMsg &msg, const std::string &subject_name)
{
	return(SetSendSubject(msg, subject_name.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#ifdef TEST_MAIN

#include <RvUtilX/RvParseCmdLineArg.hpp>
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int         return_code  = EXIT_SUCCESS;
	std::string subject_name = ">";
	std::string  my_name     = MLB::Utility::GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (RvParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -service <RV-service> ] " <<
			"[ -network <RV-network> ] " <<
			"[ -daemon <RV-daemon> ] " <<
			"[ <subject-name-pattern> ( = '>' ) [" <<
			" *** NOTE: The <subject-name-pattern> parameter must be last " <<
			"on the command line." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Get the command line parameters...
		// ////////////////////////////////////////////////////////////////
		unsigned int                count_1;
		MLB::RvUtilX::SNDSpec       snd_spec;
		std::deque<SubjectNameFlat> subject_list;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (snd_spec.ParseCmdLineServiceNetworkDaemon(count_1, argc,
				argv))
				;
			else
				RvParseCmdLineArg::ParseCmdLineSubject(argv[count_1], subject_name);
		}
		//	Initialize the Rendezvous library...
		RvContext rv_context;
		TibrvMsg  msg_recv;
		std::cerr << my_name << ": Listening to subject " << subject_name <<
			std::endl;
		WaitForOneShotMessage(snd_spec, subject_name, msg_recv,
			TIBRV_WAIT_FOREVER);
		if (!MLB::Utility::CriticalEventTest()) {
			const char  *subject_send_ptr  = GetSendSubjectPtr(msg_recv);
			const char  *subject_reply_ptr = GetReplySubjectPtr(msg_recv);
			std::string  subject_send      = GetSendSubject(msg_recv);
			std::string  subject_reply     = GetReplySubject(msg_recv);
			std::cout << "Message received:" << std::endl;
			std::cout << "   Send subject :" <<
				((subject_send_ptr != NULL) ? subject_send_ptr : "*NULL*") <<
				std::endl;
			std::cout << "   Send subject :" << subject_send << std::endl;
			std::cout << "   Reply subject:" <<
				((subject_reply_ptr != NULL) ? subject_reply_ptr : "*NULL*") <<
				std::endl;
			std::cout << "   Reply subject:" <<subject_reply << std::endl;
		}
		else {
			std::ostringstream log_text;
			std::cerr << my_name << " is performing an asynchronous exit " <<
				"due to receipt of a signal.";
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << my_name << ": fatal error encountered: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

