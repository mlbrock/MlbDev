// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implements one-shot receipt of a message.

	Revision History	:	2004-12-30 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2014.
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
struct OneShotMessageCallBack : public TibrvMsgCallback {
	OneShotMessageCallBack(TibrvMsg *msg_recv_ptr = NULL) :
	   msg_recv_ptr_(msg_recv_ptr) { }

	void onMsg(TibrvListener *listener, TibrvMsg &msg_recv) {
		try {
			if (msg_recv_ptr_ != NULL)
				CopyMsg(msg_recv, *msg_recv_ptr_);
		}
		catch (...) {
		}
		//	Destroy the listener so that we'll cease our specialized dispatch...
		listener->destroy();
	}

	TibrvMsg *msg_recv_ptr_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TibrvStatus WaitForOneShotMessage(TibrvTransport &transport,
	const std::string subject_recv, TibrvMsg &msg_recv, double time_out)
{
	if (transport.isValid() != TIBRV_TRUE)
		throw RvException("The transport specified is not valid.");

	try {
		SubjectNameFlat tmp_name(subject_recv);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except,
			std::string("Invalid receive subject: ") + except.what());
	}

	RvContext rv_context;

	TibrvQueue my_queue;
	//	Create the RV queue...
	RvUtilX_THROW_TIBRV_STATUS_IF(my_queue.create,
		());

	//	We'll process the reply through this object...
	OneShotMessageCallBack reply_callback(&msg_recv);

	TibrvListener my_listener;
	RvUtilX_THROW_TIBRV_STATUS_IF(my_listener.create,
		(&my_queue, &reply_callback, &transport, subject_recv.c_str()));

	TibrvStatus rv_status = my_queue.timedDispatch(time_out);

	if ((rv_status == TIBRV_OK) || (rv_status == TIBRV_TIMEOUT))
		return(rv_status);

	throw MLB::RvUtilX::RvExceptionStatus(rv_status,
		"Error occurred while dispatching from a Rendezvous queue.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TibrvStatus WaitForOneShotMessage(const SNDSpec &snd_spec,
	const std::string subject_recv, TibrvMsg &msg_recv, double time_out)
{
	TibrvNetTransport transport;

	snd_spec.CreateTransport(transport);

	return(WaitForOneShotMessage(transport, subject_recv, msg_recv, time_out));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TibrvStatus WaitForOneShotMessage(const std::string &service,
	const std::string &network, const std::string &daemon,
	const std::string subject_recv, TibrvMsg &msg_recv, double time_out)
{
	return(WaitForOneShotMessage(SNDSpec(service, network, daemon), subject_recv,
		msg_recv, time_out));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#ifdef TEST_MAIN

#include <RvUtilX/RvParseCmdLineArg.hpp>
#include <Utility/CriticalEventHandler.hpp>

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (RvParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"-subject <RV-subject-name> " <<
			"[ -wait <seconds-to-wait> ] " <<
			"[ -service <RV-service> ] " <<
			"[ -network <RV-network> ] " <<
			"[ -daemon <RV-daemon> ] " <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Get the command line parameters...
		// ////////////////////////////////////////////////////////////////
		unsigned int count_1;
		std::string  subject_name;
		double       wait_seconds = TIBRV_WAIT_FOREVER;
		SNDSpec      snd_spec;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (RvParseCmdLineArg::ParseCmdLineSubject("-SUBJECT",
            count_1, argc, argv, subject_name))
				;
			else if (RvParseCmdLineArg::ParseCmdLineNumeric<double>(
				"-WAIT", count_1, argc, argv, wait_seconds, 0.0))
				;
			else if (snd_spec.ParseCmdLineServiceNetworkDaemon(count_1, argc,
				argv))
				;
			else
				RvParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		MLB::Utility::CriticalEventContext critical_event_context;
		RvContext                          rv_context;
		TibrvMsg                           msg_recv;
		ThrowTibrvTimeOutIf(WaitForOneShotMessage(snd_spec, subject_name,
			msg_recv, wait_seconds), wait_seconds,
			"Test time-out expired before message receipt.");
		const std::string send_subject  = GetSendSubject(msg_recv);
		const std::string reply_subject = GetReplySubject(msg_recv);
		std::cout << "MESSAGE TIME : " <<
			MLB::Utility::TimeSpec().ToStringLocal() << std::endl;
		std::cout << "SEND SUBJECT : " << send_subject << std::endl;
		std::cout << "REPLY SUBJECT: " << reply_subject << std::endl;
		std::cout << "MESSAGE DATA : {" << RvMsgToString(msg_recv) << "}" <<
			std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR in 'WaitForOneShotMessage()' "
			"regression test: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

