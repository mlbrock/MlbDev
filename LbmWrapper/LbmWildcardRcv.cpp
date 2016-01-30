// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LbmWildcardRcv class.

	Revision History	:	2008-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
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
LbmWildcardRcv::LbmWildcardRcv(LbmContext &context, const char *pattern_string,
	LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
	lbm_rcv_cb_proc call_back, void *user_data_ptr,
	lbm_event_queue_t *event_queue)
	:LbmObjectBase<lbm_wildcard_rcv_t>()
{
	Initialize(context, pattern_string, topic_rcv_attr, wildcard_rcv_attr,
		call_back, user_data_ptr, event_queue);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmWildcardRcv::LbmWildcardRcv(LbmContext &context,
	const std::string &pattern_string, LbmAttrTopicRcv &topic_rcv_attr,
	LbmAttrWildcardRcv &wildcard_rcv_attr, lbm_rcv_cb_proc call_back,
	void *user_data_ptr, lbm_event_queue_t *event_queue)
	:LbmObjectBase<lbm_wildcard_rcv_t>()
{
	Initialize(context, pattern_string.c_str(), topic_rcv_attr,
		wildcard_rcv_attr, call_back, user_data_ptr, event_queue);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmWildcardRcv::LbmWildcardRcv(LbmContext &context, const char *pattern_string,
	lbm_rcv_cb_proc call_back, void *user_data_ptr,
	lbm_event_queue_t *event_queue)
	:LbmObjectBase<lbm_wildcard_rcv_t>()
{
	LbmAttrTopicRcv    topic_rcv_attr;
	LbmAttrWildcardRcv wildcard_rcv_attr;

	Initialize(context, pattern_string, topic_rcv_attr, wildcard_rcv_attr,
		call_back, user_data_ptr, event_queue);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmWildcardRcv::LbmWildcardRcv(LbmContext &context,
	const std::string &pattern_string, lbm_rcv_cb_proc call_back,
	void *user_data_ptr, lbm_event_queue_t *event_queue)
	:LbmObjectBase<lbm_wildcard_rcv_t>()
{
	LbmAttrTopicRcv    topic_rcv_attr;
	LbmAttrWildcardRcv wildcard_rcv_attr;

	Initialize(context, pattern_string.c_str(), topic_rcv_attr,
		wildcard_rcv_attr, call_back, user_data_ptr, event_queue);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmWildcardRcv::Dump(int *size, lbm_config_option_t *opts)
{
	ConfigDumpHelper<lbm_wildcard_rcv_t>::DumpConfig(
		::lbm_wildcard_rcv_dump, GetObjectPtr(), size, opts);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmConfigOptionList &LbmWildcardRcv::Dump(LbmConfigOptionList &out_list)
{
	return(ConfigDumpHelper<lbm_wildcard_rcv_t>::DumpConfig(
		::lbm_wildcard_rcv_dump, GetObjectPtr(), out_list));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmConfigOptionList LbmWildcardRcv::Dump()
{
	LbmConfigOptionList out_list;

	return(Dump(out_list));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmWildcardRcv::Initialize(LbmContext &context, const char *pattern_string,
	LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
	lbm_rcv_cb_proc call_back, void *user_data_ptr,
	lbm_event_queue_t *event_queue)
{
	lbm_wildcard_rcv_t *tmp_ptr;

	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_wildcard_rcv_create,
		(&tmp_ptr, context.GetObjectPtr(), pattern_string,
			topic_rcv_attr.GetObjectPtr(), wildcard_rcv_attr.GetObjectPtr(),
			call_back, user_data_ptr, event_queue));

	object_sptr_.reset(tmp_ptr, ::lbm_wildcard_rcv_delete);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#ifdef TEST_MAIN

#include <LbmWrapper/LbmMiscSupport.hpp>
#include <LbmWrapper/LbmLogSupport.hpp>

#include <Utility/Sleep.hpp>
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
		LbmWildcardRcv   rcv(lbm_context, "Greeting*", cb_functor);
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
		TEST_MinRcv();
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

