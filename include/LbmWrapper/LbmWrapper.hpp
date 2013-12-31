// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Wrapper include file for the 29 West LBM product.

	Revision History	:	2008-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__LbmWrapper__LbmWrapper_hpp__HH

#define HH__MLB__LbmWrapper__LbmWrapper_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <mbcompat.h>

#if defined(_MSC_VER)
# pragma warning(disable:4820)
/* Windows-only includes */
#include <winsock.h>
#else
/* Unix-only includes */
#include <stdlib.h>
#include <unistd.h>
#endif

#if defined(linux)
# include <sys/types.h>
#endif // #if defined(linux)

#include <lbm/lbm.h>

#include <Utility/Utility_Exception.hpp>

#include <boost/shared_ptr.hpp>

// ////////////////////////////////////////////////////////////////////////////

/**
	\namespace MLB

	\brief The corporate namespace.
*/
namespace MLB {

/**
	\namespace LbmWrapper

	\brief The 29 West LBM library wrapper project namespace.
*/
namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
typedef signed short int   LbmOpt_SShortInt;
typedef unsigned short int LbmOpt_UShortInt;
typedef signed int         LbmOpt_SInt;
typedef unsigned int       LbmOpt_UInt;
typedef signed long int    LbmOpt_SLongInt;
typedef unsigned long int  LbmOpt_ULongInt;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename CBBase>
	struct LbmLogCallbackCRTP {
protected:
	LbmLogCallbackCRTP()
	{
	}

public:
	static int LbmLogCallBack(int level, const char *message, void *clientd)
	{
		return(reinterpret_cast<CBBase *>(clientd)->
			LbmLogCallBackImpl(level, message));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename CBBase>
	struct LbmTimerCallbackCRTP {
protected:
	LbmTimerCallbackCRTP()
	{
	}

public:
	static int LbmTimerCallBack(lbm_context_t *ctx, const void *clientd)
	{
		return(const_cast<CBBase *>(reinterpret_cast<const CBBase *>(clientd))->
			LbmTimerCallBackImpl(ctx));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename CBBase>
	struct LbmSrcCallbackCRTP {
protected:
	LbmSrcCallbackCRTP()
	{
	}

public:
	static int LbmSrcCallBack(lbm_src_t *src, int event, void *ed, void *clientd)
	{
		return(reinterpret_cast<CBBase *>(clientd)->
			LbmSrcCallBackImpl(src, event, ed));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename CBBase>
	struct LbmRcvCallbackCRTP {
protected:
	LbmRcvCallbackCRTP()
	{
	}

public:
	static int LbmRcvCallBack(lbm_rcv_t *rcv, lbm_msg_t *msg, void *clientd)
	{
		return(reinterpret_cast<CBBase *>(clientd)->
			LbmRcvCallBackImpl(rcv, msg));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename CBBase>
	struct LbmEventQueueCallbackCRTP {
protected:
	LbmEventQueueCallbackCRTP()
	{
	}

public:
	static int LbmEventQueueCallBack(lbm_event_queue_t *evq, int event,
		std::size_t evq_size, lbm_ulong_t event_delay_usec, void *clientd)
	{
		return(reinterpret_cast<CBBase *>(clientd)->
			LbmEventQueueCallBackImpl(evq, event, evq_size, event_delay_usec));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmException :
	public MLB::Utility::ExceptionGeneral {
public:
	LbmException();
	explicit LbmException(const char *except_string);
	explicit LbmException(const std::string &except_string);
	explicit LbmException(const std::ostringstream &except_string);
	~LbmException() MBCOMPAT_EXCEPT_NOTHROW;

	virtual void Rethrow(const char *except_string = NULL) const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmExceptionStatus :
	public LbmException {
public:
	//	Default constructor.
	LbmExceptionStatus();
	//	String-only constructors...
	explicit LbmExceptionStatus(const char *except_string);
	explicit LbmExceptionStatus(const std::string &except_string);
	explicit LbmExceptionStatus(const std::ostringstream &except_string);
	~LbmExceptionStatus() MBCOMPAT_EXCEPT_NOTHROW;

	int GetStatus() const;

	virtual void Rethrow(const char *except_string = NULL) const;

	static std::string GetStatusString(const char *other_text = NULL);

protected:
	int lbm_error_code_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#define WRAP29_LBM_THROW_STRING_NAME(method_name)						\
	{																					\
		std::ostringstream INTERNAL_error_text;							\
		INTERNAL_error_text << "Invocation of '" << method_name <<	\
			"()' failed";															\
		throw LbmExceptionStatus(INTERNAL_error_text.str());			\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#define WRAP29_LBM_THROW(method_name)										\
	WRAP29_LBM_THROW_STRING_NAME(#method_name)
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#define WRAP29_LBM_THROW_IF_NE_ZERO(method_name, method_args)			\
	{																						\
		int INTERNAL_lbm_error_code = method_name method_args ;			\
		if (INTERNAL_lbm_error_code != 0) {										\
			WRAP29_LBM_THROW(method_name)											\
		}																					\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#define WRAP29_LBM_THROW_IF_LT_ZERO(method_name, method_args)			\
	{																						\
		int INTERNAL_lbm_error_code = method_name method_args ;			\
		if (INTERNAL_lbm_error_code < 0) {										\
			WRAP29_LBM_THROW(method_name)											\
		}																					\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#define WRAP29_LBM_THROW_IF_NEGATIVE_ONE(method_name, method_args)	\
	{																						\
		int INTERNAL_lbm_error_code = method_name method_args ;			\
		if (INTERNAL_lbm_error_code == -1) {									\
			WRAP29_LBM_THROW(method_name)											\
		}																					\
	}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename LbmApiType>
	class LbmObjectBase {

protected:
	struct MyNullDeleter {
		void operator()(void const *) const {}
	};

	LbmObjectBase()
		:object_sptr_()
	{
	}
	explicit LbmObjectBase(LbmApiType *api_type_ptr)
		:object_sptr_(api_type_ptr, MyNullDeleter())
	{
	}
	virtual ~LbmObjectBase()
	{
	}

	boost::shared_ptr<LbmApiType> object_sptr_;

public:
	typedef LbmApiType MyLbmApiType;

	const LbmApiType *GetObjectPtr() const
	{
		return(object_sptr_.get());
	}
	LbmApiType *GetObjectPtr()
	{
		return(object_sptr_.get());
	}

	const LbmApiType *GetValidObjectPtr() const
	{
		ThrowIfEmptySPtr();

		return(GetObjectPtr());
	}
	LbmApiType *GetValidObjectPtr()
	{
		ThrowIfEmptySPtr();

		return(GetObjectPtr());
	}

private:
	void ThrowIfEmptySPtr() const
	{
		if (object_sptr_.get())
			MLB::Utility::ThrowLogicError("The LBM object pointer is 'NULL'.");
	}
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	Forward declare the attribute template class (and instantiations). The real
	work is done within LbmAttribute.hpp...
*/
template <typename LbmApiType> class LbmAttributeBase;

//	////////////////////////////////////////////////////////////////////////////
typedef LbmAttributeBase<lbm_context_attr_t>      LbmAttrContext;
typedef LbmAttributeBase<lbm_rcv_topic_attr_t>    LbmAttrTopicRcv;
typedef LbmAttributeBase<lbm_src_topic_attr_t>    LbmAttrTopicSrc;
typedef LbmAttributeBase<lbm_wildcard_rcv_attr_t> LbmAttrWildcardRcv;
typedef LbmAttributeBase<lbm_event_queue_attr_t>  LbmAttrEventQueue;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::vector<lbm_config_option_t>    LbmConfigOptionList;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmEventQueue : public LbmObjectBase<lbm_event_queue_t> {

	void Initialize(lbm_event_queue_monitor_proc call_back, void *user_data_ptr,
		LbmAttrEventQueue &attr_ref);
	void Initialize(lbm_event_queue_monitor_proc call_back, void *user_data_ptr,
		lbm_event_queue_attr_t *attr_ptr);

public:
	typedef lbm_event_queue_stats_t MyLbmApiStatsType;

	explicit LbmEventQueue(lbm_event_queue_t *event_queue_ptr = NULL);
	explicit LbmEventQueue(lbm_event_queue_monitor_proc call_back,
		void *user_data_ptr = NULL, lbm_event_queue_attr_t *attr_ptr = NULL);
	LbmEventQueue(lbm_event_queue_monitor_proc call_back,
		void *user_data_ptr, LbmAttrEventQueue &attr);

	template <typename FunctorType>
		explicit LbmEventQueue(FunctorType &cb_functor,
			lbm_event_queue_attr_t *attr_ptr = NULL)
			:LbmObjectBase<lbm_event_queue_t>()
	{
		Initialize(FunctorType::LbmEventQueueCallBack, &cb_functor, attr_ptr);
	}
	template <typename FunctorType>
		LbmEventQueue(FunctorType &cb_functor, LbmAttrEventQueue &attr_ref)
			:LbmObjectBase<lbm_event_queue_t>()
	{
		Initialize(FunctorType::LbmEventQueueCallBack, &cb_functor, attr_ref);
	}

	int  EventDispatch(lbm_ulong_t tmo);
	void EventDispatchUnblock();
	int  EventQueueSize();
	void EventQueueShutdown();

	void                 Dump(int *size, lbm_config_option_t *opts);
	LbmConfigOptionList &Dump(LbmConfigOptionList &out_list);
	LbmConfigOptionList  Dump();
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmContext : public LbmObjectBase<lbm_context_t>  {

	void Initialize(lbm_context_attr_t *attr_ptr);

public:
	typedef lbm_context_stats_t MyLbmApiStatsType;

	         LbmContext();
	explicit LbmContext(LbmAttrContext &context_attr);
	explicit LbmContext(lbm_context_t *context_attr);

	int ScheduleTimer(lbm_timer_cb_proc proc, void *clientd,
		LbmEventQueue &evq, lbm_ulong_t delay);
	int ScheduleTimer(lbm_timer_cb_proc proc, void *clientd,
		lbm_event_queue_t *evq, lbm_ulong_t delay);
	template <typename FunctorType>
		int ScheduleTimer(FunctorType &cb_functor, LbmEventQueue &evq,
		lbm_ulong_t delay)
	{
		return(ScheduleTimer(&cb_functor, evq.GetObjectPtr(), delay));
	}
	template <typename FunctorType>
		int ScheduleTimer(FunctorType &cb_functor, lbm_event_queue_t *evq,
		lbm_ulong_t delay)
	{
		return(ScheduleTimer(FunctorType::LbmTimerCallBack, &cb_functor,
			evq, delay));
	}

	int ScheduleTimerRecurring(lbm_timer_cb_proc proc, void *clientd,
		LbmEventQueue &evq, lbm_ulong_t delay);
	int ScheduleTimerRecurring(lbm_timer_cb_proc proc, void *clientd,
		lbm_event_queue_t *evq, lbm_ulong_t delay);
	template <typename FunctorType>
		int ScheduleTimerRecurring(FunctorType &cb_functor,
		lbm_event_queue_t *evq, lbm_ulong_t delay)
	{
		return(ScheduleTimerRecurring(FunctorType::LbmTimerCallBack,
			&cb_functor, evq, delay));
	}
	template <typename FunctorType>
		int ScheduleTimerRecurring(FunctorType &cb_functor, LbmEventQueue &evq,
		lbm_ulong_t delay)
	{
		return(ScheduleTimerRecurring(cb_functor, evq.GetObjectPtr(), delay));
	}

	void                 Dump(int *size, lbm_config_option_t *opts);
	LbmConfigOptionList &Dump(LbmConfigOptionList &out_list);
	LbmConfigOptionList  Dump();
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmTopicSrc : public LbmObjectBase<lbm_topic_t> {

	std::string topic_name_;

	void Initialize(LbmContext &context, lbm_src_topic_attr_t *topic_attr);

public:

	LbmTopicSrc(LbmContext &context, const char *topic_name,
		lbm_src_topic_attr_t *topic_attr = NULL);
	LbmTopicSrc(LbmContext &context, const std::string &topic_name,
		lbm_src_topic_attr_t *topic_attr = NULL);
	LbmTopicSrc(LbmContext &context, const char *topic_name,
		LbmAttrTopicSrc &topic_attr);
	LbmTopicSrc(LbmContext &context, const std::string &topic_name,
		LbmAttrTopicSrc &topic_attr);
	explicit LbmTopicSrc(lbm_topic_t *topic_attr);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmTopicRcv : public LbmObjectBase<lbm_topic_t> {

	std::string topic_name_;

	void Initialize(LbmContext &context, lbm_rcv_topic_attr_t *attr_ptr);

public:

	LbmTopicRcv(LbmContext &context, const char *topic_name,
		lbm_rcv_topic_attr_t *topic_attr = NULL);
	LbmTopicRcv(LbmContext &context, const std::string &topic_name,
		lbm_rcv_topic_attr_t *topic_attr = NULL);
	LbmTopicRcv(LbmContext &context, const char *topic_name,
		LbmAttrTopicRcv &topic_attr);
	LbmTopicRcv(LbmContext &context, const std::string &topic_name,
		LbmAttrTopicRcv &topic_attr);
	explicit LbmTopicRcv(lbm_topic_t *topic_attr);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmSrc : public LbmObjectBase<lbm_src_t> {
	void Initialize(LbmContext &context, LbmTopicSrc &topic,
		lbm_src_cb_proc call_back, void *user_data_ptr,
		lbm_event_queue_t *event_queue);

public:

	typedef lbm_src_transport_stats_t MyLbmApiStatsType;

	LbmSrc(LbmContext &context, LbmTopicSrc &topic);
	LbmSrc(LbmContext &context, const char *topic_name);
	LbmSrc(LbmContext &context, const std::string &topic_name);

	template <typename FunctorType>
		LbmSrc(LbmContext &context, LbmTopicSrc &topic, FunctorType &cb_functor,
			lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_src_t>()
	{
		Initialize(context, topic, FunctorType::LbmSrcCallBack, &cb_functor,
			event_queue);
	}
	template <typename FunctorType>
		LbmSrc(LbmContext &context, const char *topic_name,
			FunctorType &cb_functor, lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_src_t>()
	{
		LbmTopicSrc topic(context, topic_name);

		Initialize(context, topic, FunctorType::LbmSrcCallBack, &cb_functor,
			event_queue);
	}
	template <typename FunctorType>
		LbmSrc(LbmContext &context, const std::string &topic_name,
			FunctorType &cb_functor, lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_src_t>()
	{
		LbmTopicSrc topic(context, topic_name);

		Initialize(context, topic, FunctorType::LbmSrcCallBack, &cb_functor,
			event_queue);
	}

	template <typename FunctorType>
		LbmSrc(LbmContext &context, LbmTopicSrc &topic, FunctorType &cb_functor,
			LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_src_t>()
	{
		Initialize(context, topic, FunctorType::LbmSrcCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}
	template <typename FunctorType>
		LbmSrc(LbmContext &context, const char *topic_name,
			FunctorType &cb_functor, LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_src_t>()
	{
		LbmTopicSrc topic(context, topic_name);

		Initialize(context, topic, FunctorType::LbmSrcCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}
	template <typename FunctorType>
		LbmSrc(LbmContext &context, const std::string &topic_name,
			FunctorType &cb_functor, LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_src_t>()
	{
		LbmTopicSrc topic(context, topic_name);

		Initialize(context, topic, FunctorType::LbmSrcCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}

	void Send(const std::string &msg, int flags = 0);
	void Send(const void *msg_ptr, std::size_t msg_length, int flags = 0);
	void Send(const char *msg_ptr, std::size_t msg_length, int flags = 0);
	void SendAsciiZ(const char *msg_ptr, int flags = 0);

	void                 Dump(int *size, lbm_config_option_t *opts);
	LbmConfigOptionList &Dump(LbmConfigOptionList &out_list);
	LbmConfigOptionList  Dump();
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmRcv : public LbmObjectBase<lbm_rcv_t> {
	void Initialize(LbmContext &context, LbmTopicRcv &topic,
		lbm_rcv_cb_proc call_back, void *user_data_ptr,
		lbm_event_queue_t *event_queue);

public:

	typedef lbm_rcv_transport_stats_t MyLbmApiStatsType;

	LbmRcv(LbmContext &context, LbmTopicRcv &topic, lbm_rcv_cb_proc call_back,
		void *user_data_ptr = NULL, lbm_event_queue_t *event_queue = NULL);
	LbmRcv(LbmContext &context, const char *topic_name,
		lbm_rcv_cb_proc call_back, void *user_data_ptr = NULL,
		lbm_event_queue_t *event_queue = NULL);
	LbmRcv(LbmContext &context, const std::string &topic_name,
		lbm_rcv_cb_proc call_back, void *user_data_ptr = NULL,
		lbm_event_queue_t *event_queue = NULL);

	template <typename FunctorType>
		LbmRcv(LbmContext &context, LbmTopicRcv &topic, FunctorType &cb_functor,
			lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_rcv_t>()
	{
		Initialize(context, topic, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue);
	}
	template <typename FunctorType>
		LbmRcv(LbmContext &context, const char *topic_name,
			FunctorType &cb_functor, lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_rcv_t>()
	{
		LbmTopicRcv topic(context, topic_name);

		Initialize(context, topic, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue);
	}
	template <typename FunctorType>
		LbmRcv(LbmContext &context, const std::string &topic_name,
			FunctorType &cb_functor, lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_rcv_t>()
	{
		LbmTopicRcv topic(context, topic_name);

		Initialize(context, topic, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue);
	}

	template <typename FunctorType>
		LbmRcv(LbmContext &context, LbmTopicRcv &topic, FunctorType &cb_functor,
			LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_rcv_t>()
	{
		Initialize(context, topic, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}
	template <typename FunctorType>
		LbmRcv(LbmContext &context, const char *topic_name,
			FunctorType &cb_functor, LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_rcv_t>()
	{
		LbmTopicRcv topic(context, topic_name);

		Initialize(context, topic, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}
	template <typename FunctorType>
		LbmRcv(LbmContext &context, const std::string &topic_name,
			FunctorType &cb_functor, LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_rcv_t>()
	{
		LbmTopicRcv topic(context, topic_name);

		Initialize(context, topic, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}

	void                 Dump(int *size, lbm_config_option_t *opts);
	LbmConfigOptionList &Dump(LbmConfigOptionList &out_list);
	LbmConfigOptionList  Dump();
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LbmWildcardRcv : public LbmObjectBase<lbm_wildcard_rcv_t> {
	void Initialize(LbmContext &context, const char *pattern_string,
		LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
		lbm_rcv_cb_proc call_back, void *user_data_ptr,
		lbm_event_queue_t *event_queue);

public:
	LbmWildcardRcv(LbmContext &context, const char *pattern_string,
		LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
      lbm_rcv_cb_proc call_back, void *user_data_ptr = NULL,
		lbm_event_queue_t *event_queue = NULL);
	LbmWildcardRcv(LbmContext &context, const std::string &pattern_string,
		LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
      lbm_rcv_cb_proc call_back, void *user_data_ptr = NULL,
		lbm_event_queue_t *event_queue = NULL);
	LbmWildcardRcv(LbmContext &context, const char *pattern_string,
		lbm_rcv_cb_proc call_back, void *user_data_ptr = NULL,
		lbm_event_queue_t *event_queue = NULL);
	LbmWildcardRcv(LbmContext &context, const std::string &pattern_string,
		lbm_rcv_cb_proc call_back, void *user_data_ptr = NULL,
		lbm_event_queue_t *event_queue = NULL);

	template <typename FunctorType>
		LbmWildcardRcv(LbmContext &context, const char *pattern_string,
			LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
			FunctorType &cb_functor, lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_wildcard_rcv_t>()
	{
		Initialize(context, pattern_string, topic_rcv_attr, wildcard_rcv_attr,
			FunctorType::LbmRcvCallBack, &cb_functor, event_queue);
	}
	template <typename FunctorType>
		LbmWildcardRcv(LbmContext &context, const std::string &pattern_string,
			LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
			FunctorType &cb_functor, lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_wildcard_rcv_t>()
	{
		Initialize(context, pattern_string.c_str(), topic_rcv_attr,
			wildcard_rcv_attr, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue);
	}
	template <typename FunctorType>
		LbmWildcardRcv(LbmContext &context, const char *pattern_string,
			FunctorType &cb_functor, lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_wildcard_rcv_t>()
	{
		LbmAttrTopicRcv    topic_rcv_attr;
		LbmAttrWildcardRcv wildcard_rcv_attr;

		Initialize(context, pattern_string, topic_rcv_attr, wildcard_rcv_attr,
			FunctorType::LbmRcvCallBack, &cb_functor, event_queue);
	}
	template <typename FunctorType>
		LbmWildcardRcv(LbmContext &context, const std::string &pattern_string,
			FunctorType &cb_functor, lbm_event_queue_t *event_queue = NULL)
			:LbmObjectBase<lbm_wildcard_rcv_t>()
	{
		LbmAttrTopicRcv    topic_rcv_attr;
		LbmAttrWildcardRcv wildcard_rcv_attr;

		Initialize(context, pattern_string.c_str(), topic_rcv_attr,
			wildcard_rcv_attr, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue);
	}

	template <typename FunctorType>
		LbmWildcardRcv(LbmContext &context, const char *pattern_string,
			LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
			FunctorType &cb_functor, LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_wildcard_rcv_t>()
	{
		Initialize(context, pattern_string, topic_rcv_attr, wildcard_rcv_attr,
			FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}
	template <typename FunctorType>
		LbmWildcardRcv(LbmContext &context, const std::string &pattern_string,
			LbmAttrTopicRcv &topic_rcv_attr, LbmAttrWildcardRcv &wildcard_rcv_attr,
			FunctorType &cb_functor, LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_wildcard_rcv_t>()
	{
		Initialize(context, pattern_string.c_str(), topic_rcv_attr,
			wildcard_rcv_attr, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}
	template <typename FunctorType>
		LbmWildcardRcv(LbmContext &context, const char *pattern_string,
			FunctorType &cb_functor, LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_wildcard_rcv_t>()
	{
		LbmAttrTopicRcv    topic_rcv_attr;
		LbmAttrWildcardRcv wildcard_rcv_attr;

		Initialize(context, pattern_string, topic_rcv_attr, wildcard_rcv_attr,
			FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}
	template <typename FunctorType>
		LbmWildcardRcv(LbmContext &context, const std::string &pattern_string,
			FunctorType &cb_functor, LbmEventQueue &event_queue)
			:LbmObjectBase<lbm_wildcard_rcv_t>()
	{
		LbmAttrTopicRcv    topic_rcv_attr;
		LbmAttrWildcardRcv wildcard_rcv_attr;

		Initialize(context, pattern_string.c_str(), topic_rcv_attr,
			wildcard_rcv_attr, FunctorType::LbmRcvCallBack, &cb_functor,
			event_queue.GetObjectPtr());
	}

	void                 Dump(int *size, lbm_config_option_t *opts);
	LbmConfigOptionList &Dump(LbmConfigOptionList &out_list);
	LbmConfigOptionList  Dump();
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<LbmEventQueue>  LbmEventQueueSPtr;
typedef boost::shared_ptr<LbmContext>     LbmContextSPtr;
typedef boost::shared_ptr<LbmTopicSrc>    LbmTopicSrcSPtr;
typedef boost::shared_ptr<LbmTopicRcv>    LbmTopicRcvSPtr;
typedef boost::shared_ptr<LbmSrc>         LbmSrcSPtr;
typedef boost::shared_ptr<LbmRcv>         LbmRcvSPtr;
typedef boost::shared_ptr<LbmWildcardRcv> LbmWildcardRcvSPtr;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<LbmAttrContext>     LbmAttrContextSPtr;
typedef boost::shared_ptr<LbmAttrTopicRcv>    LbmAttrTopicRcvSPtr;
typedef boost::shared_ptr<LbmAttrTopicSrc>    LbmAttrTopicSrcSPtr;
typedef boost::shared_ptr<LbmAttrWildcardRcv> LbmAttrWildcardRcvSPtr;
typedef boost::shared_ptr<LbmAttrEventQueue>  LbmAttrEventQueueSPtr;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DatumType> class LbmStatsBase {
public:
	typedef          DatumType                    MyDatumType;
	typedef typename DatumType::MyLbmApiType      MyLbmApiType;
	typedef typename DatumType::MyLbmApiStatsType MyLbmApiStatsType;

	const MyLbmApiType *GetObjectPtr() const
	{
		return(*datum_ptr_);
	}

	const MyLbmApiStatsType &GetStatsRef() const
	{
		return(stats_datum_);
	}

protected:
	explicit LbmStatsBase(MyLbmApiType *datum)
		:datum_ptr_(datum)
		,stats_datum_()
	{
	}

	explicit LbmStatsBase(const MyLbmApiStatsType &stats)
		:datum_ptr_()
		,stats_datum_(stats)
	{
	}

	MyLbmApiType      *datum_ptr_;
	MyLbmApiStatsType  stats_datum_;
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__LbmWrapper__LbmWrapper_hpp__HH

