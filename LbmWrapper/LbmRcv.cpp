// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LbmRcv class.

	Revision History	:	2008-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmAttrSupport.hpp>

#include <ctime>

#ifdef __linux__
# include <memory.h>
#endif // #ifdef __linux__

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
LbmRcv::LbmRcv(LbmContext &context, LbmTopicRcv &topic,
	lbm_rcv_cb_proc call_back, void *user_data_ptr,
	lbm_event_queue_t *event_queue)
	:LbmObjectBase<lbm_rcv_t>()
{
	Initialize(context, topic, call_back, user_data_ptr, event_queue);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmRcv::LbmRcv(LbmContext &context, const char *topic_name,
	lbm_rcv_cb_proc call_back, void *user_data_ptr,
	lbm_event_queue_t *event_queue)
	:LbmObjectBase<lbm_rcv_t>()
{
	LbmTopicRcv topic(context, topic_name);

	Initialize(context, topic, call_back, user_data_ptr, event_queue);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmRcv::LbmRcv(LbmContext &context, const std::string &topic_name,
	lbm_rcv_cb_proc call_back, void *user_data_ptr,
	lbm_event_queue_t *event_queue)
	:LbmObjectBase<lbm_rcv_t>()
{
	LbmTopicRcv topic(context, topic_name);

	Initialize(context, topic, call_back, user_data_ptr, event_queue);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmRcv::Dump(int *size, lbm_config_option_t *opts)
{
	ConfigDumpHelper<lbm_rcv_t>::DumpConfig(::lbm_rcv_topic_dump,
		GetObjectPtr(), size, opts);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmConfigOptionList &LbmRcv::Dump(LbmConfigOptionList &out_list)
{
	return(ConfigDumpHelper<lbm_rcv_t>::DumpConfig(::lbm_rcv_topic_dump,
		GetObjectPtr(), out_list));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmConfigOptionList LbmRcv::Dump()
{
	LbmConfigOptionList out_list;

	return(Dump(out_list));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmRcv::Initialize(LbmContext &context, LbmTopicRcv &topic,
	lbm_rcv_cb_proc call_back, void *user_data_ptr,
	lbm_event_queue_t *event_queue)
{
	lbm_rcv_t *tmp_ptr;

	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_rcv_create,
		(&tmp_ptr, context.GetObjectPtr(), topic.GetObjectPtr(), call_back,
			user_data_ptr, event_queue));

	object_sptr_.reset(tmp_ptr, ::lbm_rcv_delete);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#ifdef TEST_MAIN

#include <LbmWrapper/LbmMiscSupport.hpp>
#include <LbmWrapper/LbmLogSupport.hpp>

#include <Utility/Sleep.hpp>
#include <Utility/HRTimer.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/OSSocketLibraryContext.hpp>

using namespace MLB::LbmWrapper;

//	////////////////////////////////////////////////////////////////////////////
struct MyLbmRcvCallback : public LbmRcvCallbackCRTP<MyLbmRcvCallback> {
	MyLbmRcvCallback()
		:total_msg_count_(0)
	{
	}

	int LbmRcvCallBackImpl(lbm_rcv_t * /* rcv */, lbm_msg_t *msg)
	{
		if (msg->type == LBM_MSG_DATA)
			++total_msg_count_;
		return(0);
	}

	unsigned long long total_msg_count_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct MyLbmEventQueueCallback :
	public LbmEventQueueCallbackCRTP<MyLbmEventQueueCallback> {
	MyLbmEventQueueCallback()
	{
	}

	int LbmEventQueueCallBackImpl(lbm_event_queue_t *evq, int event,
		std::size_t evq_size, lbm_ulong_t event_delay_usec)
	{
		std::cout << "Event queue=0x" << evq << ": Event=" << event <<
			"Event queue size=" << evq_size << ": Event delay usec=" <<
			event_delay_usec << std::endl;

		return(0);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct MyLbmTimerCallback : public LbmTimerCallbackCRTP<MyLbmTimerCallback> {
	MyLbmTimerCallback(MyLbmRcvCallback &msg_cb_ref,
		LbmEventQueue &event_queue_ref, unsigned long long max_msg_count,
		unsigned int max_seconds)
		:msg_rcv_cb_ref_(msg_cb_ref)
		,event_queue_ref_(event_queue_ref)
		,max_msg_count_(max_msg_count)
		,max_seconds_(max_seconds)
		,expiration_time_(0)
	{
	}

	int LbmTimerCallBackImpl(lbm_context_t * /* ctx */)
	{
		if (!expiration_time_)
			expiration_time_ = std::time(NULL) + max_seconds_;

		if (msg_rcv_cb_ref_.total_msg_count_ == max_msg_count_) {
			std::cerr << "The expected number of messages have been received (" <<
				max_msg_count_ << ")." << std::endl;
			UnblockEventQueueDispatch();
		}
		else if (expiration_time_ <= std::time(NULL)) {
			std::cerr << "The maximum number of seconds have been waited (" <<
				max_seconds_ << ")." << std::endl;
			UnblockEventQueueDispatch();
		}
		else if (MLB::Utility::CriticalEventTest()) {
			std::cerr << "A signal or other critical operating system event has "
				"been detected." << std::endl;
			UnblockEventQueueDispatch();
		}

		return(0);
	}

private:
	MyLbmRcvCallback   &msg_rcv_cb_ref_;
	LbmEventQueue      &event_queue_ref_;
	unsigned long long  max_msg_count_;
	unsigned int        max_seconds_;
	std::time_t         expiration_time_;

	void UnblockEventQueueDispatch()
	{
		std::cerr << "Unblocking event dispatch..." << std::endl;

		event_queue_ref_.EventDispatchUnblock();

		std::cerr << "Event dispatch is now unblocked." << std::endl;
	}

	MLB_Utility_NonCopyable_Macro(MyLbmTimerCallback);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_STRESS_MinRcv()
{
	int          second_count = 60;
	unsigned int iter_count   = 1000000;
	unsigned int this_iter    = 0;

	char message[64];

	::memset(message, ' ', sizeof(message) - 1);
	::memcpy(message, "A 64-byte message.", 18);
	message[sizeof(message) - 1] = '\0';

	try {
		MyLbmRcvCallback cb_functor;
		LbmAttrContext   context_attr;
#ifdef _MSC_VER
		context_attr.SetOpt("fd_management_type", "wincompport");
#else
		context_attr.SetOpt("fd_management_type", "epoll");
#endif // #ifdef _MSC_VER
		LbmContext       lbm_context(context_attr);
		LbmRcv           rcv(lbm_context, "Greeting", cb_functor);
		std::cerr << "Receiving up to " << iter_count << " messages within " <<
			second_count << " seconds on topic 'Greeting':" << std::flush;
		MLB::Utility::HRTimer hr_timer;
		MLB::Utility::TimeVal end_time;
		end_time.AddSeconds(second_count);
		hr_timer.Start();
		while ((cb_functor.total_msg_count_ < iter_count) &&
			(MLB::Utility::TimeVal() < end_time)) {
			if (MLB::Utility::CriticalEventTest()) {
				std::cerr << "Exiting due to detection of a signal or other "
					"critical operating system event." << std::endl;
				return;
			}
			MLB::Utility::SleepMilliSecs(1);
		}
		hr_timer.End();
		std::cerr << "Received a total of " << cb_functor.total_msg_count_ <<
			" message(s)." << std::endl << std::endl;
		std::cerr << " done in " << hr_timer.GetTickDiffInSeconds() <<
			" seconds." << std::endl;
		MLB::Utility::SleepSecs(2);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "MLB stress test analogue of the Minimal "
			"LBM Receiver Implementation minrcv.c failed on iteration " +
			MLB::Utility::AnyToString(this_iter) + " of " +
			MLB::Utility::AnyToString(iter_count) + " iterations: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_STRESS_MinRcvEventQueue()
{
	unsigned int second_count = 60;
	unsigned int iter_count   = 1000000;
	unsigned int this_iter    = 0;

	char message[64];

	::memset(message, ' ', sizeof(message) - 1);
	::memcpy(message, "A 64-byte message.", 18);
	message[sizeof(message) - 1] = '\0';

	try {
		MyLbmRcvCallback cb_rcv;
		LbmAttrContext   context_attr;
#ifdef _MSC_VER
		context_attr.SetOpt("fd_management_type", "wincompport");
#else
		context_attr.SetOpt("fd_management_type", "epoll");
#endif // #ifdef _MSC_VER
		LbmContext              lbm_context(context_attr);
		LbmRcv                  rcv(lbm_context, "Greeting", cb_rcv);
		MyLbmEventQueueCallback eq_cb_functor;
		LbmEventQueue           ev_queue(eq_cb_functor);
		MyLbmTimerCallback      cb_timer(cb_rcv, ev_queue, iter_count,
			second_count);
		lbm_context.ScheduleTimerRecurring(cb_timer, ev_queue, 100);
		std::cerr << "Receiving up to " << iter_count << " messages within " <<
			second_count << " seconds on topic 'Greeting':" << std::endl;
		MLB::Utility::HRTimer hr_timer;
		hr_timer.Start();
		ev_queue.EventDispatch(LBM_EVENT_QUEUE_BLOCK);
		hr_timer.End();
		std::cerr << "Received a total of " << cb_rcv.total_msg_count_ <<
			" message(s)." << std::endl << std::endl;
		std::cerr << " done in " << hr_timer.GetTickDiffInSeconds() <<
			" seconds." << std::endl;
		if (MLB::Utility::CriticalEventTest())
			std::cerr << "Exiting due to detection of a signal or other "
				"critical operating system event." << std::endl;
		MLB::Utility::SleepSecs(2);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "MLB stress test analogue of the Minimal "
			"LBM Receiver Implementation minrcv.c failed on iteration " +
			MLB::Utility::AnyToString(this_iter) + " of " +
			MLB::Utility::AnyToString(iter_count) + " iterations: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	This is the analogue of the Minimal LBM Receiver Implementation minrcv.c...
*/
bool TEST_MinRcv()
{
	try {
		MyLbmRcvCallback cb_functor;
		LbmAttrContext   context_attr;
#ifdef _MSC_VER
		context_attr.SetOpt("fd_management_type", "wincompport");
#else
		context_attr.SetOpt("fd_management_type", "epoll");
#endif // #ifdef _MSC_VER
		LbmContext       lbm_context(context_attr);
		LbmRcv           rcv(lbm_context, "Greeting", cb_functor);
		std::cerr << "Waiting for a message... " << std::flush;
		while (!cb_functor.total_msg_count_) {
			if (MLB::Utility::CriticalEventTest()) {
				std::cerr << "Exiting due to detection of a signal or other "
					"critical operating system event." << std::endl;
				return(false);
			}
			MLB::Utility::SleepMilliSecs(100);
		}
		std::cerr << "Received a total of " << cb_functor.total_msg_count_ <<
			" message(s)." << std::endl << std::endl;
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "MLB analogue of the Minimal LBM Receiver "
			"Implementation minrcv.c failed: " + std::string(except.what()));
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext   critical_event_context;
		MLB::Utility::OSSocketLibraryContext socket_library_context;
		LbmLoggerCout my_logger;
		LbmLog(my_logger);
		if (argc > 1)
			LbmConfig(argv[1]);
/*
		if (TEST_MinRcv())
			TEST_STRESS_MinRcv();
*/
		if (TEST_MinRcv())
			TEST_STRESS_MinRcvEventQueue();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

