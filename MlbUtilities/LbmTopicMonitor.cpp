// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Executable Module Source File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	A simple LBM topic monitor.

	Revision History	:	2010-01-10 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmAttrSupport.hpp>
#include <LbmWrapper/LbmMiscSupport.hpp>
#include <LbmWrapper/LbmStatsContext.hpp>
#include <LbmWrapper/LbmStatsRcv.hpp>

#include <Utility/StringSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/Sleep.hpp>
#include <Utility/TimeSupport.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/OSSocketLibraryContext.hpp>

#include <set>

//	////////////////////////////////////////////////////////////////////////////

namespace {
//	////////////////////////////////////////////////////////////////////////////
template <typename StatsType>
	void DumpStats(const StatsType &stats, const char *stat_type)
{
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "Stat Type: " << stat_type << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;

	std::cout << stats.ToString() << std::endl;

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

using namespace MLB::LbmWrapper;

#if 0
//	////////////////////////////////////////////////////////////////////////////
struct MyLbmTimerCallback : public LbmTimerCallbackCRTP<MyLbmTimerCallback> {
	MyLbmTimerCallback(LbmEventQueue &event_queue_ref,
		unsigned long long &curr_msg_count, unsigned long long max_msg_count,
		unsigned int max_seconds)
		:event_queue_ref_(event_queue_ref)
		,curr_msg_count_(curr_msg_count)
		,max_msg_count_(max_msg_count)
		,max_seconds_(max_seconds)
		,expiration_time_(0)
	{
	}

	int LbmTimerCallBackImpl(lbm_context_t * /* ctx */)
	{
		if (!expiration_time_)
			expiration_time_ = (max_seconds_) ? (::time(NULL) + max_seconds_) :
				std::numeric_limits<time_t>::max();

		if (curr_msg_count_ >= max_msg_count_) {
			std::cerr << "The expected number of messages have been received (" <<
				max_msg_count_ << ")." << std::endl;
			ShutdownEventQueueDispatch();
		}
		else if (expiration_time_ <= ::time(NULL)) {
			std::cerr << "The maximum number of seconds have been waited (" <<
				max_seconds_ << ")." << std::endl;
			ShutdownEventQueueDispatch();
		}
		else if (MLB::Utility::CriticalEventTest()) {
			std::cerr << "A signal or other critical operating system event has "
				"been detected." << std::endl;
			ShutdownEventQueueDispatch();
		}

		return(0);
	}

	void ShutdownEventQueueDispatch()
	{
		std::cerr << "Shutdown of event dispatch..." << std::endl;

		event_queue_ref_.EventQueueShutdown();

		std::cerr << "Event dispatch is now shutdown." << std::endl;
	}

private:
	LbmEventQueue      &event_queue_ref_;
	unsigned long long &curr_msg_count_;
	unsigned long long  max_msg_count_;
	unsigned int        max_seconds_;
	time_t              expiration_time_;

	void UnblockEventQueueDispatch()
	{
		std::cerr << "Unblocking event dispatch..." << std::endl;

		event_queue_ref_.EventDispatchUnblock();

		std::cerr << "Event dispatch is now unblocked." << std::endl;
	}

	MLB_Utility_NonCopyable_Macro(MyLbmTimerCallback);
};
//	////////////////////////////////////////////////////////////////////////////
#endif // #if 0

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

//	****************************************************************************
//	****************************************************************************
//	****************************************************************************

//	////////////////////////////////////////////////////////////////////////////
struct TopicDatum {
	TopicDatum(const std::string &topic_name, bool is_wildcard)
		:topic_name_(topic_name)
		,is_wildcard_(is_wildcard)
		,total_recv_count_(0)
		,total_recv_size_(0)
	{
	}

	bool operator < (const TopicDatum &other) const
	{
		return(topic_name_ < other.topic_name_);
	}

	std::string        topic_name_;
	bool               is_wildcard_;
	unsigned long long total_recv_count_;
	unsigned long long total_recv_size_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::multiset<TopicDatum>      TopicDatumMSet;
typedef TopicDatumMSet::iterator       TopicDatumMSetIter;
typedef TopicDatumMSet::const_iterator TopicDatumMSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmTopicMonitor :
	public MLB::LbmWrapper::LbmRcvCallbackCRTP<LbmTopicMonitor> {

public:
	LbmTopicMonitor(int argc, char **argv);

	virtual ~LbmTopicMonitor()
	{
	}

	virtual int LbmRcvCallBackImpl(lbm_rcv_t *rcv, lbm_msg_t *msg);

private:
	void ParseCmdLine(int argc, char **argv);
	void Run();
	void EmitMsgId(const lbm_msg_t *msg);

	std::string        lbm_config_file_;
	TopicDatumMSet     topic_mset_;
	unsigned long long curr_msg_count_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmTopicMonitor::LbmTopicMonitor(int argc, char **argv)
	:MLB::LbmWrapper::LbmRcvCallbackCRTP<LbmTopicMonitor>()
	,lbm_config_file_()
	,topic_mset_()
	,curr_msg_count_(0)
{
	MLB::Utility::OSSocketLibraryContext socket_library_context;

	ParseCmdLine(argc, argv);
	Run();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmTopicMonitor::ParseCmdLine(int argc, char **argv)
{
	bool is_wildcard = false;

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"<topic-name> [ <topic-name> ... ] " <<
 			"[ -wildcard <boolean> ] " <<
			"[ -lbm_config <LBM-configuration-file> ]" <<
			std::endl << std::endl <<
"\
   -help\n\
      Produces this output.\n\n\
   <topic-name> [ <topic-name> ... ]\n\
      Specifies a topic name which this program is to receive. Multiple of\n\
      such names may be specified, separated by spaces.\n\n\
      To specify a topic name containing spaces or shell meta-characters,\n\
      enclose the topic name in quotes and escape any meta-characters.\n\n\
   -wildcard <boolean>\n\
      Specifies whether any topic names which follow are wildcard names or\n\
      not. Multiple instances of this parameter may be interspersed through\n\
      a list of topic names to indicate which topic names are to be treated\n\
      as wildcard names and which are not.\n\n\
      The program begins in a state of '-wildcard " <<
			MLB::Utility::AnyToString(is_wildcard) << "'.\n\n\
   -lbm_config_file <file-name>\n\
      Specifies the name of the LBM configuration file to be used to perform\n\
      default initialization of the 29 West LBM library." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	unsigned int count_1;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (MLB::Utility::ParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-WILD_CARDS")
				("-WILDCARDS")
				("-WILD_CARD")
				("-WILDCARD")
				("-WILD")
					,
			count_1, argc, argv, is_wildcard))
				;
		else if (GetLbmConfigFileFromCmdLine(count_1, argc, argv,
			lbm_config_file_))
			;
		else
			topic_mset_.insert(TopicDatum(argv[count_1], is_wildcard));
	}

	if (topic_mset_.empty())
		MLB::Utility::ThrowInvalidArgument("No topic name names were specified "
			"on the command line.");

	if (!lbm_config_file_.empty()) {
		std::cout << "Loading LBM configuration file '" << lbm_config_file_ <<
			"'..." << std::endl;
		try {
			MLB::LbmWrapper::LbmConfig(lbm_config_file_);
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Attempt to load LBM configuration "
				"file '" + lbm_config_file_ + "' failed: " +
				std::string(except.what()));
		}
		std::cout << "The LBM configuration file '" << lbm_config_file_ <<
			"' has been loaded successfully." << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmTopicMonitor::Run()
{
	try {
		LbmAttrContext context_attr;
		context_attr.SetOpt("fd_management_type",
			GetOptimalLbmFdManagementType());
		LbmContext                      lbm_context(context_attr);
MyLbmEventQueueCallback eq_cb_functor;
LbmEventQueue           ev_queue(eq_cb_functor);
// MyLbmTimerCallback      cb_timer(cb_rcv, ev_queue, iter_count, second_count);
//	MyLbmTimerCallback      cb_timer(ev_queue, curr_msg_count_, 100, 0);
//	lbm_context.ScheduleTimerRecurring(cb_timer, ev_queue, 100);
		std::vector<LbmRcvSPtr>         rcv_list;
		std::vector<LbmWildcardRcvSPtr> wildcard_rcv_list;
		{
			TopicDatumMSetIterC             iter_b(topic_mset_.begin());
			TopicDatumMSetIterC             iter_e(topic_mset_.end());
			for ( ; iter_b != iter_e; ++iter_b) {
				if (!iter_b->is_wildcard_) {
/*
					LbmRcvSPtr rcv_sptr(
						new LbmRcv(lbm_context, iter_b->topic_name_, *this));
*/
					LbmRcvSPtr rcv_sptr(
						new LbmRcv(lbm_context, iter_b->topic_name_, *this, ev_queue));

					rcv_list.push_back(rcv_sptr);
					std::cout << "Subscribed to literal topic name '" <<
						iter_b->topic_name_ << "'" << std::endl;
				}
				else {
/*
					LbmWildcardRcvSPtr rcv_sptr(
						new LbmWildcardRcv(lbm_context, iter_b->topic_name_, *this));
*/
					LbmWildcardRcvSPtr rcv_sptr(
						new LbmWildcardRcv(lbm_context, iter_b->topic_name_, *this,
						ev_queue));

					wildcard_rcv_list.push_back(rcv_sptr);
					std::cout << "Subscribed to pattern topic name '" <<
						iter_b->topic_name_ << "'" << std::endl;
				}
			}
		}
		for ( ; ; ) {
			if (MLB::Utility::CriticalEventTest()) {
				std::cerr << "Exiting due to detection of a signal or other "
					"critical operating system event." << std::endl;
				break;
			}
//			MLB::Utility::SleepMilliSecs(100);
			ev_queue.EventDispatch(100);
		}
		DumpStats(LbmStatsContext(lbm_context), "LbmContext");
		LbmStatsRcvVector rcv_stat_list(
			LbmStatsRcv::GetStatsForContext(lbm_context));
		LbmStatsRcvVectorIterC iter_b(rcv_stat_list.begin());
		LbmStatsRcvVectorIterC iter_e(rcv_stat_list.end());
		for ( ; iter_b != iter_e; ++iter_b)
			DumpStats(*iter_b, "LbmRcv");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "LBM Topic Monitor failed while "
			"attempting to process messages received over LBM: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmTopicMonitor::EmitMsgId(const lbm_msg_t *msg)
{
	std::cout << "[" << msg->source << "][" << msg->topic_name << "]";
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int LbmTopicMonitor::LbmRcvCallBackImpl(lbm_rcv_t * /* rcv */, lbm_msg_t *msg)
{
	switch (msg->type) {
		case LBM_MSG_DATA								:
			EmitMsgId(msg);
			std::cout << ": Message received containing " << std::setw(10) <<
				msg->len << " bytes." << std::endl;
			break;
		case LBM_MSG_EOS								:
			EmitMsgId(msg);
			std::cout << ": Source reports end of the transport session." <<
				std::endl;
			break;
		case LBM_MSG_REQUEST							:
			EmitMsgId(msg);
			std::cout << ": Source request message." << std::endl;
			break;
		case LBM_MSG_RESPONSE						:
			EmitMsgId(msg);
			std::cout << ": Response message from requestee." << std::endl;
			break;
		case LBM_MSG_UNRECOVERABLE_LOSS			:
			EmitMsgId(msg);
			std::cout << ": Unrecoverable message loss." << std::endl;
			break;
		case LBM_MSG_UNRECOVERABLE_LOSS_BURST	:
			EmitMsgId(msg);
			std::cout << ": Unrecoverable burst of messages." << std::endl;
			break;
		case LBM_MSG_NO_SOURCE_NOTIFICATION		:
			EmitMsgId(msg);
			std::cout << ": Unable to locate topic source." << std::endl;
			break;
#if LBM_VERS_MAJOR >= 4
		case LBM_MSG_BOS								:
			EmitMsgId(msg);
			std::cout << ": Source reports beginning of the transport "
				"session." << std::endl;
			break;
#endif // #if LBM_VERS_MAJOR >= 4
		default											:
			EmitMsgId(msg);
			std::cout << ": Unhandled message type " << msg->type <<
				" encountered." << std::endl;
			break;
	}

	return(0);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		LbmTopicMonitor topic_monitor(argc, argv);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

