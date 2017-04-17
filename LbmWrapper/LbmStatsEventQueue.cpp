// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LbmStatsEventQueue class.

	Revision History	:	2008-08-05 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmStatsEventQueue.hpp>
#include <LbmWrapper/LbmStatsSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
LbmStatsEventQueue::LbmStatsEventQueue(MyDatumType &datum)
	:LbmStatsBase<MyDatumType>(datum.GetObjectPtr())
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsEventQueue::LbmStatsEventQueue(MyLbmApiType *datum)
	:LbmStatsBase<MyDatumType>(datum)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string LbmStatsEventQueue::ToString(unsigned int text_width,
	bool fixup_min) const
{
	using namespace MLB::Utility;

	text_width = (text_width) ? text_width : MaxStatsTextWidth;

	std::ostringstream o_str;

	EmitStatNameValueEndL("Number of data messages currently enqueued", text_width,
		stats_datum_.data_msgs, o_str);
	EmitStatNameValueEndL("Total number of data messages enqueued", text_width,
		stats_datum_.data_msgs_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for data messages", text_width,
		FixupMinStat(stats_datum_.data_msgs_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for data messages", text_width,
		stats_datum_.data_msgs_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for data messages", text_width,
		stats_datum_.data_msgs_svc_max, o_str);
	EmitStatNameValueEndL("Number of response messages currently enqueued", text_width,
		stats_datum_.resp_msgs, o_str);
	EmitStatNameValueEndL("Total number of response messages enqueued.", text_width,
		stats_datum_.resp_msgs_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for response messages", text_width,
		FixupMinStat(stats_datum_.resp_msgs_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for response messages", text_width,
		stats_datum_.resp_msgs_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for response messages", text_width,
		stats_datum_.resp_msgs_svc_max, o_str);
	EmitStatNameValueEndL("Number of topicless immediate messages currently enqueued", text_width,
		stats_datum_.topicless_im_msgs, o_str);
	EmitStatNameValueEndL("Total number of topicless immediate messages enqueued", text_width,
		stats_datum_.topicless_im_msgs_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for topicless immediate messages", text_width,
		FixupMinStat(stats_datum_.topicless_im_msgs_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for topicless immediate messages", text_width,
		stats_datum_.topicless_im_msgs_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for topicless immediate messages", text_width,
		stats_datum_.topicless_im_msgs_svc_max, o_str);
	EmitStatNameValueEndL("Number of wildcard receiver messages currently enqueued", text_width,
		stats_datum_.wrcv_msgs, o_str);
	EmitStatNameValueEndL("Total number of wildcard receiver messages enqueued", text_width,
		stats_datum_.wrcv_msgs_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for wildcard receiver messages", text_width,
		FixupMinStat(stats_datum_.wrcv_msgs_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for wildcard receiver messages", text_width,
		stats_datum_.wrcv_msgs_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for wildcard receiver messages", text_width,
		stats_datum_.wrcv_msgs_svc_max, o_str);
	EmitStatNameValueEndL("Number of I/O events currently enqueued", text_width,
		stats_datum_.io_events, o_str);
	EmitStatNameValueEndL("Total number of I/O events enqueued", text_width,
		stats_datum_.io_events_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for I/O events", text_width,
		FixupMinStat(stats_datum_.io_events_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for I/O events", text_width,
		stats_datum_.io_events_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for I/O events", text_width,
		stats_datum_.io_events_svc_max, o_str);
	EmitStatNameValueEndL("Number of timer events currently enqueued", text_width,
		stats_datum_.timer_events, o_str);
	EmitStatNameValueEndL("Total number of timer events enqueued", text_width,
		stats_datum_.timer_events_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for timer events", text_width,
		FixupMinStat(stats_datum_.timer_events_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for timer events", text_width,
		stats_datum_.timer_events_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for timer events", text_width,
		stats_datum_.timer_events_svc_max, o_str);
	EmitStatNameValueEndL("Number of source events currently enqueued", text_width,
		stats_datum_.source_events, o_str);
	EmitStatNameValueEndL("Total number of source events enqueued", text_width,
		stats_datum_.source_events_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for source events", text_width,
		FixupMinStat(stats_datum_.source_events_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for source events", text_width,
		stats_datum_.source_events_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for source events", text_width,
		stats_datum_.source_events_svc_max, o_str);
	EmitStatNameValueEndL("Number of unblock events currently enqueued", text_width,
		stats_datum_.unblock_events, o_str);
	EmitStatNameValueEndL("Total number of unblock events enqueued", text_width,
		stats_datum_.unblock_events_tot, o_str);
	EmitStatNameValueEndL("Number of cancel events currently enqueued", text_width,
		stats_datum_.cancel_events, o_str);
	EmitStatNameValueEndL("Total number of cancel events enqueued", text_width,
		stats_datum_.cancel_events_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for cancel events", text_width,
		FixupMinStat(stats_datum_.cancel_events_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for cancel events", text_width,
		stats_datum_.cancel_events_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for cancel events", text_width,
		stats_datum_.cancel_events_svc_max, o_str);
	EmitStatNameValueEndL("Number of context source events currently enqueued", text_width,
		stats_datum_.context_source_events, o_str);
	EmitStatNameValueEndL("Total number of context source events enqueued", text_width,
		stats_datum_.context_source_events_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for context source events", text_width,
		FixupMinStat(stats_datum_.context_source_events_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for context source events", text_width,
		stats_datum_.context_source_events_svc_mean, o_str);
	EmitStatNameValueEndL("Maximum service time for context source events", text_width,
		stats_datum_.context_source_events_svc_max, o_str);
	EmitStatNameValueEndL("Total number of events currently enqueued", text_width,
		stats_datum_.events, o_str);
	EmitStatNameValueEndL("Total number of events enqueued", text_width,
		stats_datum_.events_tot, o_str);
	EmitStatNameValueEndL("Minimum age of event queue entry when dequeued", text_width,
		FixupMinStat(stats_datum_.age_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean age of event queue entry when dequeued", text_width,
		stats_datum_.age_mean, o_str);
	EmitStatNameValue("Maximum age of event queue entry when dequeued", text_width,
		stats_datum_.age_max, o_str);
//	This is a guess... I know these members weren't in 3.5.0 and were in 3.6.5. So this may need fine-tuning --- MLB
#if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 6))
	o_str << std::endl;
	EmitStatNameValueEndL("Number of callback events currently enqueued", text_width,
		stats_datum_.callback_events, o_str);
	EmitStatNameValueEndL("Number of callback events enqueued", text_width,
		stats_datum_.callback_events_tot, o_str);
	EmitStatNameValueEndL("Minimum service time for callback events", text_width,
		FixupMinStat(stats_datum_.callback_events_svc_min, fixup_min), o_str);
	EmitStatNameValueEndL("Mean service time for callback events", text_width,
		stats_datum_.callback_events_svc_mean, o_str);
	EmitStatNameValue("Maximum service time for callback events", text_width,
		stats_datum_.callback_events_svc_max, o_str);
#endif // #if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR => 6))

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmStatsEventQueue::Initialize()
{
	WRAP29_LBM_THROW_IF_NEGATIVE_ONE(
		::lbm_event_queue_retrieve_stats,
		(datum_ptr_, &stats_datum_));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

