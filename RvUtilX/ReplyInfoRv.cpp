// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for the ReplyInfoRv class.

	Revision History	:	2006-09-21 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RequestApi.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
ReplyInfoRv::ReplyInfoRv()
	:MLB::Utility::ReplyInfoUtility<TibrvMsg, TibrvMsg>()
	,rv_context_ptr_()
	,transport_ptr_()
	,queue_ptr_()
	,listener_ptr_()
	,callback_ptr_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ReplyInfoRv::ReplyInfoRv(boost::shared_ptr<TibrvNetTransport> &transport_ptr,
	const char *recv_subject)
	:MLB::Utility::ReplyInfoUtility<TibrvMsg, TibrvMsg>()
	,rv_context_ptr_(new RvContext)
	,transport_ptr_(transport_ptr)
	,queue_ptr_(new TibrvQueue)
	,listener_ptr_(new TibrvListener)
	,callback_ptr_(new ReplyInfoRvCb)
{
	InitializeListen(recv_subject);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ReplyInfoRv::ReplyInfoRv(boost::shared_ptr<TibrvNetTransport> &transport_ptr,
	const std::string &recv_subject)
	:MLB::Utility::ReplyInfoUtility<TibrvMsg, TibrvMsg>()
	,rv_context_ptr_(new RvContext)
	,transport_ptr_(transport_ptr)
	,queue_ptr_(new TibrvQueue)
	,listener_ptr_(new TibrvListener)
	,callback_ptr_(new ReplyInfoRvCb)
{
	InitializeListen(recv_subject.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ReplyInfoRv::ReplyInfoRv(boost::shared_ptr<TibrvNetTransport> &transport_ptr,
	const TibrvMsg &msg_sent)
	:MLB::Utility::ReplyInfoUtility<TibrvMsg, TibrvMsg>()
	,rv_context_ptr_(new RvContext)
	,transport_ptr_(transport_ptr)
	,queue_ptr_(new TibrvQueue)
	,listener_ptr_(new TibrvListener)
	,callback_ptr_(new ReplyInfoRvCb)
{
	InitializeListen(MLB::RvUtilX::GetReplySubjectPtr(msg_sent));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ReplyInfoRv::ReplyInfoRv(const ReplyInfoRv &other)
	:MLB::Utility::ReplyInfoUtility<TibrvMsg, TibrvMsg>()
	,rv_context_ptr_(other.rv_context_ptr_)
	,transport_ptr_(other.transport_ptr_)
	,queue_ptr_(other.queue_ptr_)
	,listener_ptr_(other.listener_ptr_)
	,callback_ptr_(other.callback_ptr_)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ReplyInfoRv & ReplyInfoRv::operator = (const ReplyInfoRv &other)
{
	ReplyInfoRv tmp_datum(other);

	swap(tmp_datum);

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ReplyInfoRv::swap(ReplyInfoRv &other)
{
	rv_context_ptr_.swap(other.rv_context_ptr_);
	transport_ptr_.swap(other.transport_ptr_);
	queue_ptr_.swap(other.queue_ptr_);
	listener_ptr_.swap(other.listener_ptr_);
	callback_ptr_.swap(other.callback_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ReplyInfoRv::Check() const
{
	if (rv_context_ptr_ == NULL)
		throw RvException("The ReplyInfoRv 'rv_context_ptr_' member is 'NULL'.");
	else if (transport_ptr_ == NULL)
		throw RvException("The ReplyInfoRv 'transport_ptr_' member is 'NULL'.");
	else if (queue_ptr_ == NULL)
		throw RvException("The ReplyInfoRv 'queue_ptr_' member is 'NULL'.");
	else if (listener_ptr_ == NULL)
		throw RvException("The ReplyInfoRv 'listener_ptr_' member is 'NULL'.");
	else if (callback_ptr_ == NULL)
		throw RvException("The ReplyInfoRv 'callback_ptr_' member is 'NULL'.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ReplyInfoRv::GetDoneFlag() const
{
	Check();

	return(callback_ptr_->done_flag_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ReplyInfoRv::GetSuccessFlag() const
{
	Check();

	return(callback_ptr_->success_flag_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ReplyInfoRv::GetErrorText() const
{
	Check();

	return(callback_ptr_->error_text_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TibrvMsg &ReplyInfoRv::GetRvMessage(TibrvMsg &rv_msg) const
{
	Check();

	MLB::RvUtilX::CopyMsg(callback_ptr_->msg_recv_, rv_msg);

	return(rv_msg);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ReplyInfoRv::SendRvMessage(const TibrvMsg &rv_msg)
{
	RvUtilX_THROW_TIBRV_STATUS_IF(transport_ptr_->send,
		(rv_msg));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::shared_ptr<TibrvNetTransport> ReplyInfoRv::GetRvTransportSPtr() const
{
	return(transport_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::shared_ptr<TibrvQueue> ReplyInfoRv::GetRvQueueSPtr() const
{
	return(queue_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::shared_ptr<TibrvListener> ReplyInfoRv::GetRvListenerSPtr() const
{
	return(listener_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ReplyInfoRv::DoRecvImpl(TibrvMsg &recv_data, int wait_ms,
	bool throw_on_time_out_flag) const
{
	Check();

	if (callback_ptr_->done_flag_)
		throw RvException("Attempt to receive a single-shot reply failed "
			"because the callback instance indicates the reply has already "
			"been received.");

	double      time_out  = (wait_ms < 0) ? -1.0 :
		(static_cast<double>(wait_ms) / 1000.0);

	TibrvStatus rv_status = queue_ptr_->timedDispatch(time_out);

	if (callback_ptr_->done_flag_) {
		if (callback_ptr_->success_flag_) {
			RvUtilX_THROW_TIBRV_STATUS_IF(callback_ptr_->msg_recv_.createCopy,
				(recv_data));
			return(true);
		}
		throw RvException(callback_ptr_->error_text_);
	}
	else if (rv_status == TIBRV_TIMEOUT) {
		if (throw_on_time_out_flag)
			throw RvExceptionTimeOut(time_out, "Attempt to receive a single-"
				"shot reply failed");
	}
	else if (rv_status != TIBRV_OK)
		throw RvExceptionStatus(rv_status, "Attempt to receive a single-shot "
			"reply failed");

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ReplyInfoRv::ResetRecvImpl()
{
	Check();

	callback_ptr_->Reset();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ReplyInfoRv::InitializeListen(const char *recv_subject)
{
	try {
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_ptr_->create,
			());
		RvUtilX_THROW_TIBRV_STATUS_IF(listener_ptr_->create,
			(queue_ptr_.get(), callback_ptr_.get(), transport_ptr_.get(),
			recv_subject));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create an 'MLB::RvUtilX::"
			"ReplyInfoRv' instance for Tib/Rv reply name '" +
			std::string(recv_subject) + "': " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

