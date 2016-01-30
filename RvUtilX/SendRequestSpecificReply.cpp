// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of logic to support the sending of
								requests with replies sent to a specified, non-inbox
								subject name.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
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

#ifdef _Windows
# pragma warning(disable:4265)
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Though the base class of TibrvMsgCallback doesn't define its destructor as,
//	virtual, this is safe for instances of this class because they are only
//	used internally, are constructed on the stack, and are always referenced
//	through instances.
// ////////////////////////////////////////////////////////////////////////////
struct SendRequestSpecificReplyCB : public TibrvMsgCallback {
	SendRequestSpecificReplyCB(TibrvMsg *msg_recv_ptr = NULL) :
	   msg_recv_ptr_(msg_recv_ptr)
		,done_flag_(false)
		,success_flag_(false)
		,error_text_() { }

	void onMsg(TibrvListener *listener, TibrvMsg &msg_recv) {
		try {
			done_flag_ = true;
			if (msg_recv_ptr_ != NULL)
				RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.createCopy,
					(*msg_recv_ptr_));
			success_flag_ = true;
		}
		catch (const std::exception &except) {
			success_flag_ = false;
			error_text_   = except.what();
		}
		catch (...) {
			success_flag_ = false;
			error_text_   = "Unspecified non-std::exception thrown.";
		}
		//	Destroy the listener so that we'll cease our specialized dispatch...
		listener->destroy();
	}

	TibrvMsg    *msg_recv_ptr_;
	bool         done_flag_;
	bool         success_flag_;
	std::string  error_text_;
};
// ////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
# pragma warning(default:4265)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////
TibrvStatus SendRequestSpecificReply(TibrvTransport &transport,
	const TibrvMsg &msg_send, TibrvMsg &msg_reply, double time_out)
{
	if (transport.isValid() != TIBRV_TRUE)
		throw RvException("The transport specified is not valid.");

	const char *reply_subject = NULL;
	try {
		//	Seems pretty obvious that this SHOULD be a const method....
		RvUtilX_THROW_TIBRV_STATUS_IF(
			const_cast<TibrvMsg &>(msg_send).getReplySubject,
			(reply_subject));
		SubjectNameFlat::CheckReplySubjectName(reply_subject);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except,
			std::string("Invalid reply subject: ") + except.what());
	}

	RvContext rv_context;

	TibrvQueue my_queue;
	//	Create the RV queue...
	RvUtilX_THROW_TIBRV_STATUS_IF(my_queue.create,
		());

	//	We'll process the reply through this object...
	SendRequestSpecificReplyCB reply_callback(&msg_reply);

	TibrvListener my_listener;
	RvUtilX_THROW_TIBRV_STATUS_IF(my_listener.create,
		(&my_queue, &reply_callback, &transport, reply_subject));

	RvUtilX_THROW_TIBRV_STATUS_IF(transport.send,
		(msg_send));

	TibrvStatus rv_status = my_queue.timedDispatch(time_out);
	if (reply_callback.done_flag_) {
		if (reply_callback.success_flag_)
			return(TIBRV_OK);
		throw RvException(reply_callback.error_text_);
	}

	if (rv_status == TIBRV_TIMEOUT)
		return(rv_status);

	throw RvExceptionStatus(rv_status,
		"Error occurred while dispatching from a Rendezvous queue.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SendRequestSpecificReplyThrow(TibrvTransport &transport,
	const TibrvMsg &msg_send, TibrvMsg &msg_reply, double time_out)
{
	if (SendRequestSpecificReply(transport, msg_send, msg_reply, time_out) ==
		TIBRV_TIMEOUT) {
		throw RvExceptionTimeOut(time_out, "A specific send of a request on "
			"subject '" + GetSendSubject(msg_send) + "' did not complete with a "
			"reply on subject '" + GetReplySubject(msg_send) + "' in time.");
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

