// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LbmStatsContext class.

	Revision History	:	2010-07-31 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmStatsContext.hpp>
#include <LbmWrapper/LbmStatsSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
LbmStatsContext::LbmStatsContext(MyDatumType &datum)
	:LbmStatsBase<MyDatumType>(datum.GetObjectPtr())
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsContext::LbmStatsContext(MyLbmApiType *datum)
	:LbmStatsBase<MyDatumType>(datum)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string LbmStatsContext::ToString(unsigned int text_width,
	bool /* fixup_min */) const
{
	using namespace MLB::Utility;

	text_width = (text_width) ? text_width : MaxStatsTextWidth;

	std::ostringstream o_str;

	EmitStatNameValueEndL("Topic resolution datagrams sent", text_width,
		stats_datum_.tr_dgrams_sent, o_str);
	EmitStatNameValueEndL("Topic resolution datagram bytes sent", text_width,
		stats_datum_.tr_bytes_sent, o_str);
	EmitStatNameValueEndL("Topic resolution datagrams received", text_width,
		stats_datum_.tr_dgrams_rcved, o_str);
	EmitStatNameValueEndL("Topic resolution datagram bytes received", text_width,
		stats_datum_.tr_bytes_rcved, o_str);
	EmitStatNameValueEndL("Topic resolution datagrams dropped due to incorrect version", text_width,
		stats_datum_.tr_dgrams_dropped_ver, o_str);
	EmitStatNameValueEndL("Topic resolution datagrams dropped due to incorrect type", text_width,
		stats_datum_.tr_dgrams_dropped_type, o_str);
	EmitStatNameValueEndL("Topic resolution datagrams dropped due to being malformed", text_width,
		stats_datum_.tr_dgrams_dropped_malformed, o_str);
	EmitStatNameValueEndL("Topic resolution datagram sends which failed", text_width,
		stats_datum_.tr_dgrams_send_failed, o_str);
	EmitStatNameValueEndL("Topics in the source topic map", text_width,
		stats_datum_.tr_src_topics, o_str);
	EmitStatNameValueEndL("Topics in the receiver topic map", text_width,
		stats_datum_.tr_rcv_topics, o_str);
	EmitStatNameValueEndL("Unresolved topics in the receiver topic map", text_width,
		stats_datum_.tr_rcv_unresolved_topics, o_str);
	EmitStatNameValueEndL("LBT-RM datagrams received not belonging to any transport session", text_width,
		stats_datum_.lbtrm_unknown_msgs_rcved, o_str);
	EmitStatNameValueEndL("LBT-RU datagrams received not belonging to any transport session", text_width,
		stats_datum_.lbtru_unknown_msgs_rcved, o_str);
	EmitStatNameValueEndL("LBM send calls which blocked", text_width,
		stats_datum_.send_blocked, o_str);
	EmitStatNameValueEndL("LBM send calls which returned EWOULDBLOCK", text_width,
		stats_datum_.send_would_block, o_str);
	EmitStatNameValueEndL("LBM send response calls which blocked", text_width,
		stats_datum_.resp_blocked, o_str);
	EmitStatNameValue("LBM send response calls which returned EWOULDBLOCK", text_width,
		stats_datum_.resp_would_block, o_str);

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmStatsContext::Initialize()
{
	WRAP29_LBM_THROW_IF_NEGATIVE_ONE(
		::lbm_context_retrieve_stats,
		(datum_ptr_, &stats_datum_));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#ifdef TEST_MAIN

#include <LbmWrapper/LbmStatsRcv.hpp>
#include <LbmWrapper/LbmStatsSrc.hpp>
#include <LbmWrapper/LbmStatsEventQueue.hpp>
#include <LbmWrapper/LbmAttrSupport.hpp>
#include <LbmWrapper/LbmMiscSupport.hpp>

#include <Utility/Sleep.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/OSSocketLibraryContext.hpp>

using namespace MLB::LbmWrapper;

//	////////////////////////////////////////////////////////////////////////////
//	Define by uncommenting to run the source RDMA tests.
//	#define TEST_SRC_TRANSPORT_RDMA
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename StatsType>
	void TEST_DumpStats(const StatsType &stats, const char *test_type,
	const char *test_name, bool is_scalar = true, std::size_t this_index = 0,
	std::size_t total_count = 0)
{
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "Test Type: " << test_type << std::endl;
	std::cout << "Test Name: " << test_name << std::endl;
	if (is_scalar)
		std::cout << "Element  : N/A" << std::endl;
	else
		std::cout << "Element  : Number " << (this_index + 1) << " of " <<
			total_count << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;

	std::cout << stats.ToString() << std::endl;

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	if (is_scalar)
		std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename StatsTypeList>
	void TEST_DumpStatsList(const StatsTypeList &stats_list,
		const char *test_type, const char *test_name)
{
	std::cout << MLB::Utility::PadLeft("", 79, '+') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '+') << std::endl;

	std::cout << "Test Type: " << test_type         << std::endl;
	std::cout << "Test Name: " << test_name         << std::endl;
	std::cout << "Total    : " << stats_list.size() << std::endl;

	std::cout << MLB::Utility::PadLeft("", 79, '+') << std::endl;

	for (std::size_t count_1 = 0; count_1 < stats_list.size(); ++count_1)
		TEST_DumpStats(stats_list[count_1], test_type, test_name, false,
			count_1, stats_list.size());
		
	std::cout << MLB::Utility::PadLeft("", 79, '+') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '+') << std::endl;
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_LbmContextStats(LbmContext &datum, const char *test_name)
{
	LbmStatsContext stats(datum);

	TEST_DumpStats(stats, "LbmContext", test_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_LbmSrcStats(LbmSrc &datum, const char *test_name)
{
	LbmStatsSrc stats(datum);

	TEST_DumpStats(stats, "LbmSrc", test_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_LbmContextDefault()
{
	try {
		LbmContext lbm_context;
		TEST_LbmContextStats(lbm_context, "Default-constructed context");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failure in "
			"TEST_LbmContextDefault() test: " + std::string(except.what()));
	}

	std::cout << "Done." << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_LbmContextWithSrcTopics()
{
	try {
		char message[64];
		::memset(message, ' ', sizeof(message) - 1);
		::memcpy(message, "A 64-byte message.", 18);
		message[sizeof(message) - 1] = '\0';

		LbmAttrContext context_attr;
#ifdef _MSC_VER
		context_attr.SetOpt("fd_management_type", "wincompport");
#else
		context_attr.SetOpt("fd_management_type", "epoll");
#endif // #ifdef _MSC_VER
		LbmContext      lbm_context(context_attr);
		LbmAttrTopicSrc topic_src_attr;
		topic_src_attr.SetOpt("implicit_batching_minimum_length", "8192");
		//	----------------------------------------------------------------
		topic_src_attr.SetOpt("transport", "tcp");
		LbmTopicSrc topic_src_tcp(lbm_context, "StatsTest.001.TCP", topic_src_attr);
		LbmSrc      src_tcp(lbm_context, topic_src_tcp);
		src_tcp.Send(message, sizeof(message), LBM_SRC_NONBLOCK | LBM_MSG_FLUSH);
		//	----------------------------------------------------------------
		topic_src_attr.SetOpt("transport", "lbtrm");
		LbmTopicSrc topic_src_rm(lbm_context, "StatsTest.001.RM", topic_src_attr);
		LbmSrc      src_rm(lbm_context, topic_src_rm);
		src_rm.Send(message, sizeof(message), LBM_SRC_NONBLOCK | LBM_MSG_FLUSH);
		//	----------------------------------------------------------------
		topic_src_attr.SetOpt("transport", "lbtru");
		LbmTopicSrc topic_src_ru(lbm_context, "StatsTest.001.RU", topic_src_attr);
		LbmSrc      src_ru(lbm_context, topic_src_ru);
		src_ru.Send(message, sizeof(message), LBM_SRC_NONBLOCK | LBM_MSG_FLUSH);
		//	----------------------------------------------------------------
#if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 5))
		topic_src_attr.SetOpt("transport", "lbtipc");
		LbmTopicSrc topic_src_ipc(lbm_context, "StatsTest.001.IPC", topic_src_attr);
		LbmSrc      src_ipc(lbm_context, topic_src_ipc);
		src_ipc.Send(message, sizeof(message), LBM_SRC_NONBLOCK | LBM_MSG_FLUSH);
#endif // #if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 5))
		//	----------------------------------------------------------------
#if defined(TEST_SRC_TRANSPORT_RDMA)
# if (LBM_VERS_MAJOR > 4) || ((LBM_VERS_MAJOR == 4) && (LBM_VERS_MINOR >= 1))
		topic_src_attr.SetOpt("transport", "LBT-RDMA");
		LbmTopicSrc topic_src_rdma(lbm_context, "StatsTest.001.RMDA", topic_src_attr);
		LbmSrc      src_rdma(lbm_context, topic_src_rdma);
		src_rdma.Send(message, sizeof(message), LBM_SRC_NONBLOCK | LBM_MSG_FLUSH);
# endif // #if (LBM_VERS_MAJOR > 4) || ((LBM_VERS_MAJOR == 4) && (LBM_VERS_MINOR >= 1))
#endif // #if defined(TEST_SRC_TRANSPORT_RDMA)
		TEST_LbmContextStats(lbm_context, "Multiple transport sends");
		TEST_LbmSrcStats(src_tcp, "Single send on TCP topic");
		TEST_LbmSrcStats(src_rm,  "Single send on LBT-RM topic");
		TEST_LbmSrcStats(src_ru,  "Single send on LBT-RU topic");
#if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 5))
		TEST_LbmSrcStats(src_ipc, "Single send on LBT-IPC topic");
#endif // #if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 5))
#if defined(TEST_SRC_TRANSPORT_RDMA)
# if (LBM_VERS_MAJOR > 4) || ((LBM_VERS_MAJOR == 4) && (LBM_VERS_MINOR >= 1))
		TEST_LbmSrcStats(src_rdma, "Single send on LBT-RDMA topic");
# endif // #if (LBM_VERS_MAJOR > 4) || ((LBM_VERS_MAJOR == 4) && (LBM_VERS_MINOR >= 1))
#endif // #if defined(TEST_SRC_TRANSPORT_RDMA)
		//	----------------------------------------------------------------
		TEST_DumpStatsList(LbmStatsSrc::GetStatsForContext(lbm_context),
			"LbmSrc", "From 'GetStatsForContext()'");
		TEST_DumpStatsList(LbmStatsSrc::GetImStatsForContext(lbm_context),
			"LbmSrc", "From 'GetImStatsForContext()'");
		//	----------------------------------------------------------------
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failure in "
			"TEST_LbmContextWithSrcTopics() test: " + std::string(except.what()));
	}

	std::cout << "Done." << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

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
void TEST_LbmContextWithRcvTopics()
{
	try {
		MyLbmRcvCallback cb_functor;
		LbmAttrContext   context_attr;
#ifdef _MSC_VER
		context_attr.SetOpt("fd_management_type", "wincompport");
#else
		context_attr.SetOpt("fd_management_type", "epoll");
#endif // #ifdef _MSC_VER
		LbmContext      lbm_context(context_attr);
		LbmAttrTopicRcv topic_rcv_attr;
		//	----------------------------------------------------------------
//		LbmTopicRcv topic_rcv_001(lbm_context, "StatsTest.002", topic_rcv_attr);
LbmTopicRcv topic_rcv_001(lbm_context, "Greeting", topic_rcv_attr);
		LbmRcv      rcv_001(lbm_context, topic_rcv_001, cb_functor);
		for (int count_1 = 0; (count_1 < 1000) && (!cb_functor.total_msg_count_);
			++count_1) {
			if (MLB::Utility::CriticalEventTest())
				throw MLB::Utility::ExceptionCriticalEvent();
			MLB::Utility::SleepMilliSecs(100);
		}
		std::cerr << "Received a total of " << cb_functor.total_msg_count_ <<
			" message(s)." << std::endl << std::endl;
		//	----------------------------------------------------------------
		LbmTopicRcv topic_rcv_002(lbm_context, "StatsTest.003", topic_rcv_attr);
		LbmRcv      rcv_002(lbm_context, topic_rcv_002, cb_functor);
		for (int count_1 = 0; (count_1 < 10) && (!cb_functor.total_msg_count_);
			++count_1) {
			if (MLB::Utility::CriticalEventTest())
				throw MLB::Utility::ExceptionCriticalEvent();
			MLB::Utility::SleepMilliSecs(100);
		}
		std::cerr << "Received a total of " << cb_functor.total_msg_count_ <<
			" message(s)." << std::endl << std::endl;
		//	----------------------------------------------------------------
		TEST_LbmContextStats(lbm_context, "Multiple transport receives");
		//	----------------------------------------------------------------
		TEST_DumpStatsList(LbmStatsRcv::GetStatsForContext(lbm_context),
			"LbmRcv", "From 'GetStatsForContext()'");
		TEST_DumpStatsList(LbmStatsRcv::GetStatsForRcv(rcv_001),
			"LbmRcv", "From 'GetStatsForRcv()'");
		TEST_DumpStatsList(LbmStatsRcv::GetImStatsForContext(lbm_context),
			"LbmRcv", "From 'GetImStatsForContext()'");
		//	----------------------------------------------------------------
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failure in "
			"TEST_LbmContextWithRcvTopics() test: " + std::string(except.what()));
	}


	std::cout << "Done." << std::endl;
}
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
void TEST_LbmContextWithEventQueue()
{
	try {
		LbmContext              lbm_context;
		MyLbmEventQueueCallback eq_cb_functor;
		LbmEventQueue           event_queue(eq_cb_functor);
		LbmStatsEventQueue      event_queue_stats(event_queue);
		TEST_LbmContextStats(lbm_context, "Context with event queue");
		TEST_DumpStats(event_queue_stats, "LbmStatsEventQueue",
			"Context with event queue");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failure in "
			"TEST_LbmContextWithEventQueue() test: " + std::string(except.what()));
	}

	std::cout << "Done." << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext   critical_event_context;
		MLB::Utility::OSSocketLibraryContext socket_library_context;
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << "LBM Library Version    : " << ::lbm_version() << std::endl;
		std::cout << "LBM Header File Version: " <<
			LbmGetIncludeFileVersion() << " (numeric version = " <<
			LbmGetIncludeFileVersionCombined() << ")" << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		TEST_LbmContextDefault();
		TEST_LbmContextWithSrcTopics();
		TEST_LbmContextWithRcvTopics();
		TEST_LbmContextWithEventQueue();
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << std::endl;
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

