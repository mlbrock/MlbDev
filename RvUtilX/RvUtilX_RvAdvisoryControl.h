// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the RvUtilX callback template classes.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_RvAdvisory_h__HH

#define HH__MLB__RvUtilX_RvAdvisory_h__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RvCallback.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {
namespace RvUtilX {
//	////////////////////////////////////////////////////////////////////////////
TibrvQueue *GetDefaultQueue()
{
	TibrvQueue *return_ptr = Tibrv::defaultQueue();

	if (return_ptr == NULL)
		throw RvException("Tib/Rv static method 'Tibrv::defaultQueue()' "
			"returned 'NULL'.");

	return(return_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvProcessTransport *GetProcessTransportPtr()
{
	TibrvProcessTransport *return_ptr = Tibrv::processTransport();

	if (return_ptr == NULL)
		throw RvException("Tib/Rv static method 'Tibrv::processTransport()' "
			"returned 'NULL'.");

	return(return_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

/*
	CODE NOTE: Now in RvTransport.cpp. To be removed.
//	////////////////////////////////////////////////////////////////////////////
tibrvTransport GetHandle(const TibrvTransport &transport_ref)
{
	return(transport_ref.getHandle());
}
//	////////////////////////////////////////////////////////////////////////////
*/

/*
	CODE NOTE: Now in RvTransport.cpp. To be removed.
//	////////////////////////////////////////////////////////////////////////////
std::string &GetDescription(const tibrvTransport &transport_ref,
	std::string &out_string)
{
	const char *tmp_ptr = NULL;

	RvUtilX_THROW_TIBRV_STATUS_IF(tibrvTransport_GetDescription,
		(transport_ref, &tmp_ptr));

	return(out_string.assign((tmp_ptr == NULL) ? "" : tmp_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GetDescription(const tibrvTransport &transport_ref)
{
	std::string out_string;

	return(GetDescription(transport_ref, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &GetDescription(const TibrvTransport &transport_ref,
	std::string &out_string)
{
	return(GetDescription(GetHandle(transport_ref), std::string &out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GetDescription(const TibrvTransport &transport_ref)
{
	std::string out_string;

	return(GetDescription(transport_ref, out_string));
}
//	////////////////////////////////////////////////////////////////////////////
*/

//	////////////////////////////////////////////////////////////////////////////
TibrvListener &CreateListener(TibrvQueue &queue_ref,
	TibrvCallback &callback_ref, TibrvTransport &transport_ref,
	TibrvListener &listener_ref, const char *subject_name,
	const void *closure = NULL)
{
	SubjectNameWork::SubjectNameWork(subject_name);

	RvUtilX_THROW_TIBRV_STATUS_IF(listener_ref.create,
		(&queue_ref, &callback_ref, &transport_ref, subject_name, closure));

	return(listener_ref);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvListener &CreateListener(TibrvQueue &queue_ref,
	TibrvCallback &callback_ref, TibrvTransport &transport_ref,
	TibrvListener &listener_ref, const std::string &subject_name,
	const void *closure = NULL)
{
	return(CreateListener(queue_ref, callback_ref, transport_ref, listener_ref,
		subject_name.c_str(), closure));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvQueueGroup &AddToQueueGroup(TibrvQueue &queue_ref)
{
}
//	////////////////////////////////////////////////////////////////////////////
} // namespace RvUtilx
} // namespace MLB

namespace MLB {
namespace RvUtilX {
//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Add to RVUtilx_GetRvField.h
API_RVUTILX bool GetRvFieldIPAddr32(const TibrvMsg &msg, const char *field_name,
	tibrv_ipaddr32 &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldIPAddr32(const TibrvMsg &msg,
	const std::string &field_name, tibrv_ipaddr32 &out_datum,
	tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldIPAddr32(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldIPAddr32(const TibrvMsg &msg,
	const std::string &field_name, std::string &out_datum,
	tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Add to GetRvField.cpp
// Get tibrv_ipaddr32
bool GetRvFieldIPAddr32(const TibrvMsg &msg, const char *field_name,
	tibrv_ipaddr32 &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (GetRvField(msg, field_name, msg_field, field_id, required_flag,
		TIBRVMSG_IPADDR32))
		return(false);

	out_datum = msg_field.getData().ipaddr32;
	
	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Add to GetRvField.cpp
// Get tibrv_ipaddr32
bool GetRvFieldIPAddr32(const TibrvMsg &msg, const std::string &field_name,
	tibrv_ipaddr32 &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Add to GetRvField.cpp
// Get tibrv_ipaddr32 as a std::string
bool GetRvFieldIPAddr32(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	tibrv_ipaddr32 ip_addr;

	if (!GetRvFieldIPAddr32(msg, field_name, ip_addr, field_id, required_flag,
		no_conversion_flag)) {
		ip_addr = 0;
		return(false);
	}

	std::ostringstring o_str;

	o_str <<
		static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
			&ip_addr)[0]) << "." <<
		static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
			&ip_addr)[1]) << "." <<
		static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
			&ip_addr)[2]) << "." <<
		static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
			&ip_addr)[3]);

	out_datum.swap(o_str.str());

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Add to GetRvField.cpp
// Get tibrv_ipaddr32 as a std::string
bool GetRvFieldIPAddr32(const TibrvMsg &msg, const std::string &field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvFieldIPAddr32(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////
} // namespace RvUtilx
} // namespace MLB

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
class RvAdvisoryAction {
	RvAdvisoryAction();
	virtual ~RvAdvisoryAction();

	virtual void ProcessRvAdvisory(TibrvListener *listener_ptr,
		const TibrvMsg &msg_recv, const std::string &send_subject,
		const std::string &msg_class_name, bool msg_class_known,
		MLB::Utility::LogLevel msg_class_log,
		const MLB::RvUtilX::SubjectNameFlat &flat_name);

	static void GetMsgContents_SlowConsumer(const TibrvMsg &msg_recv,
		unsigned int &waiting, unsigned int &dropped,
		unsigned long long &bytes_dropped);
	static unsigned int GetMsgContents_FastProducer(const TibrvMsg &msg_recv);
	static MLB::Utility::TimeSpec GetMsgContents_LicenseExpire(
		const TibrvMsg &msg_recv);
	static std::string GetMsgContents_OtherHost(const TibrvMsg &msg_recv);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryAction::RvAdvisoryAction()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryAction::~RvAdvisoryAction()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvisoryAction::ProcessRvAdvisory(TibrvListener *listener_ptr,
	const TibrvMsg &msg_recv, const std::string &send_subject,
	const std::string &msg_class_name, bool msg_class_known,
	MLB::Utility::LogLevel msg_class_log,
	const MLB::RvUtilX::SubjectNameFlat &flat_name)
{
	std::string out_text;

	if (send_subject == "_RV.ERROR.SYSTEM.CLIENT.SLOWCONSUMER") {
		unsigned int       waiting;
		unsigned int       dropped;
		unsigned long long bytes_dropped;
		GetMsgContents_SlowConsumer(msg_recv, waiting, dropped, bytes_dropped);
		out_text = "Slow consumption detected (messages waiting = " +
			MLB::Utility::AnyToString(waiting) + ", messages dropped = " +
			MLB::Utility::AnyToString(dropped) + ", bytes dropped = " +
			MLB::Utility::AnyToString(bytes_dropped) + ")";
	}
	else if (send_subject == "_RV.WARN.SYSTEM.CLIENT.FASTPRODUCER") {
		unsigned int waiting = GetMsgContents_FastProducer(msg_recv);
		out_text = "Fast production detected (messages waiting = " +
			MLB::Utility::AnyToString(waiting) + ")";
	}
	else if ((send_subject == "_RV.ERROR.SYSTEM.LICENSE.EXPIRE") ||
		(send_subject == "_RV.WARN.SYSTEM.LICENSE.EXPIRE")) {
		MLB::Utility::TimeSpec expiration(GetMsgContents_LicenseExpire(msg_recv));
		out_text = "Tib/Rv license " +
			((msg_class_log == MLB::Utility::LogLevel_Warning) ? "will expire" :
			"expired") + " at " + expiration.ToString();
	}
	else if (send_subject == "_RV.WARN.SYSTEM.RVD.DISCONNECTED")
		out_text = "The Tib/Rv daemon has disconnected";
	else if (send_subject == "_RV.WARN.SYSTEM.RVD.CONNECTED")
		out_text = "The Tib/Rv daemon has (re-)connected";
	else if (send_subject == "_RV.ERROR.SYSTEM.CLIENT.NOMEMORY")
		out_text = "The Tib/Rv library is unable to allocate memory";
	else if ((flat_name.GetElementCount() > 4) &&
		(!strcmp(flat_name.element_list_[3], "DATALOSS"))) {
		std::string direction;
		std::string proto_type;
		std::string other_host(GetMsgContents_OtherHost(msg_recv));
		if (!strcmp(flat_name.element_list_[4], "INBOUND"))
			direction = "in-bound";
		else if (!strcmp(flat_name.element_list_[4], "OUTBOUND"))
			direction = "out-bound";
		else
			direction = "unknown direction";
		if (!strcmp(flat_name.element_list_[5], "BCAST"))
			proto_type = "broadcast";
		else if (!strcmp(flat_name.element_list_[5], "PTP"))
			proto_type = "point-to-point";
		else
			proto_type = "unknown";
		out_text = "An " + direction + " message data-loss was detected for a "
			 proto_type + " protocol in communication with host " +
			other_host;
	}
	else
		out_text = "Tib/Rv advisory message received";

	//	Get a description of the transport...
	std::string transport_string;
	try {
		TibrvTransport transport_ptr    = GetTransport(listener_ptr);
		tibrvTransport transport_handle = TransportGetHandle(*transport_ptr);
		std::string    tmp_string(TransportGetDescription(*transport_ptr));
		transport_string = "Tib/Rv transport " +
			MLB::Utility::ValueToStringHex(transport_handle) + " ('" +
			tmp_string + "')";
	}
	catch (const std::exception &) {
		transport_string = "*NO TRANSPORT DESCRIPTION*";
	}

	std::cout << MLB::Utility::TimeSpec() << " "
		MLB::Utility::ConvertLogLevelToText() << " ????? " <<
		out_text << " on transport " << transport_string << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvisoryAction::GetMsgContents_SlowConsumer(const TibrvMsg &msg_recv,
	unsigned int &waiting, unsigned int &dropped,
	unsigned long long &bytes_dropped)
{
	GetRvField(msg_recv, "waiting", waiting, 0, false, true);
	GetRvField(msg_recv, "dropped", dropped, 0, false, true);
	GetRvField(msg_recv, "bytes_dropped", bytes_dropped, 0, false, true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int RvAdvisoryAction::GetMsgContents_FastProducer(
	const TibrvMsg &msg_recv)
{
	unsigned int waiting = 0;

	GetRvField(msg_recv, "waiting", waiting, 0, false, true);

	return(waiting);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::TimeSpec RvAdvisoryAction::GetMsgContents_LicenseExpire(
	const TibrvMsg &msg_recv)
{
	MLB::Utility::TimeSpec expiretime(0, 0);

	GetRvField(msg_recv, "expiretime", expiretime, 0, false, true);

	return(expiretime);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvisoryAction::GetMsgContents_OtherHost(const TibrvMsg &msg_recv)
{
	std::string host;

	if (GetRvFieldIPAddr32(msg_recv, "host", host, 0, false, true))
		return(host);

	return("");
}
//	////////////////////////////////////////////////////////////////////////////

namespace {
//	////////////////////////////////////////////////////////////////////////////
const char         *RvAdvisorySubjectList[] = {
	"_RV.ERROR.SYSTEM.CLIENT.NOMEMORY",				
	"_RV.ERROR.SYSTEM.CLIENT.SLOWCONSUMER",			
	"_RV.ERROR.SYSTEM.DATALOSS.INBOUND.BCAST",		
	"_RV.ERROR.SYSTEM.DATALOSS.INBOUND.PTP ",		
	"_RV.ERROR.SYSTEM.DATALOSS.OUTBOUND.BCAST ",	
	"_RV.ERROR.SYSTEM.DATALOSS.OUTBOUND.PTP",		
	"_RV.ERROR.SYSTEM.LICENSE.EXPIRE",			
	"_RV.INFO.SYSTEM.RVD.CONNECTED",				
	"_RV.WARN.SYSTEM.CLIENT.FASTPRODUCER",		
	"_RV.WARN.SYSTEM.LICENSE.EXPIRE",				
	"_RV.WARN.SYSTEM.RVD.DISCONNECTED",			
};
const unsigned int  RvAdvisorySubjectCount  =
	sizeof(RvAdvisorySubjectList) / sizeof(RvAdvisorySubjectList[0]);
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
struct RvAdvisoryEventInfo {
	RvAdvisoryEventInfo(const std::string &subject_pattern)
		:subject_pattern_(subject_pattern)
		,first_occurrence_time_(0, 0)
		,last_occurrence_time_(0, 0)
		,occurrence_count_(0)
	{
	}

	bool operator < (const RvAdvisoryEventInfo &other) const {
		return(subject_pattern_ < other.subject_pattern_);
	}

	void AddOccurrence(const MLB::Utility::TimeSpec &occurrence_time =
		MLB::Utility::TimeSpec()) {
		if (first_occurrence_time_.IsZero())
			first_occurrence_time_ = occurrence_time;
		last_occurrence_time_ = occurrence_time;
		MLB::Utility::IncrementToMax(occurrence_count_);
	}

	std::string            subject_pattern_;
	MLB::Utility::TimeSpec first_occurrence_time_;
	MLB::Utility::TimeSpec last_occurrence_time_;
	unsigned long long     occurrence_count_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<RvAdvisoryEventInfo>          RvAdvisoryEventInfoSet;
typedef RvAdvisoryEventInfoSet::iterator       RvAdvisoryEventInfoSetIter;
typedef RvAdvisoryEventInfoSet::const_iterator RvAdvisoryEventInfoSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvAdvisoryControl {
	typedef RvCallbackMsgEvent<RvAdvisory>       MyMsgCallbackType;
	typedef boost::shared_ptr<MyMsgCallbackType> MyMsgCallbackTypeSPtr;
	typedef boost::shared_ptr<TibrvListener>     MyListenerSPtr;

	struct MyTransportInfo {
		MyTransportInfo(TibrvQueue *queue_ptr, TibrvCallback *callback_ptr,
			TibrvTransport *transport_ptr, const void *closure = NULL)
			:transport_ptr_(transport_ptr)
			,listener_list_()
		{
			unsigned int count_1;
			for (count_1 = 0; count_1 < RvAdvisorySubjectCount; ++count_1) {
				MyListenerSPtr listener_ptr(new TibrvListener);
				listener_list_.push_back(CreateListener(*queue_ptr, *callback_ptr,
					*transport_ptr, *listener_ptr, RvAdvisorySubject[count_1],
					closure_ptr));
			}
		}
		//	Used only for std::set::find()...
		MyTransportInfo(TibrvTransport *transport_ptr, bool)
			:transport_ptr_(transport_ptr)
			,listener_list_()
		{
		}

		bool operator < (const MyTransportInfo &other) const {
			return(transport_ptr_ < other.transport_ptr_);
		}

		TibrvTransport              *transport_ptr_;
		std::vector<MyListenerSPtr>  listener_ptr_;
	};

	typedef std::set<MyTransportInfo>            MyTransportSet;

public:
	RvAdvisoryControl()
		:rv_context_()
		,transport_set_()
		,queue_ptr_(MLB::RvUtilX::GetDefaultQueue(), MLB::BoostX::NullDeleter())
		,callback_(GetThisRef())
	{
	}
	explicit RvAdvisoryControl(TibrvQueue &queue_ref)
		:rv_context_()
		,transport_set_()
		,queue_ptr_(*queue_ref, MLB::BoostX::NullDeleter())
		,callback_(GetThisRef())
	{
	}
	explicit RvAdvisoryControl(TibrvQueueGroup &queue_group)
		:rv_context_()
		,transport_set_()
		,queue_ptr_(new TibrvQueue)
		,callback_(GetThisRef())
	{
		MLB::RvUtilX::AddToQueueGroup(MLB::RvUtilX::QueueCreate(queue_ptr_.get()));
	}

	~RvAdvisoryControl()
	{
	}

	bool AddDefaultTransport();
	bool AddTransport(TibrvTransport *transport_ptr);

	void ProcessEventOnMsg(TibrvMsgCallback *callback_ptr,
		TibrvListener *listener_ptr, TibrvMsg &msg_recv);
	void ReportException(const std::exception *except_ptr) {
		LogError << "Exception encountered in Tib/Rendezvous callback: " <<
			((except_ptr != NULL) ? except_ptr->what() : "*UNSPECIFIED*") <<
			std::endl;
	}

	RvContext                     rv_context_;
	MyTransportSet                transport_set_;
	boost::shared_ptr<TibrvQueue> queue_ptr_;
	MyMsgCallbackType             callback_;

private:
	MLB_Utility_NonCopyable_Macro(RvAdvisoryControl);

	RvAdvisoryControl &GetThisRef() {
		return(*this);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddDefaultTransport()
{
	return(AddTransport(MLB::RvUtilX::GetProcessTransportPtr()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddTransport(TibrvTransport *transport_ptr)
{
	if (transport_ptr == NULL)
		MLB::Utility::ThrowInvalidArgument("Invocation of '"
			"RvAdvisoryControl::AddTransport()' with a 'NULL' transport pointer.");

	if (transport_set_.find(MyTransportInfo(transport_ptr, false)) ==
		transport_set_.end())
		return(false);

	transport_set_.insert(MyTransportInfo(queue_ptr_.get(), &callback_,
		transport_ptr));

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvisoryControl::ProcessEventOnMsg(TibrvMsgCallback *callback_ptr,
	TibrvListener *listener_ptr, TibrvMsg &msg_recv)
{
	std::string                   s_name(MLB::RvUtilX::GetSendSubject(msg_recv));
	std::string                   msg_class_name;
	bool                          msg_class_known = false;
	MLB::Utility::LogLevel        msg_class_log   = LogLevel_Notice;
	MLB::RvUtilX::SubjectNameFlat flat_name(send_subject);

	if (flat_name.GetElementCount() > 1) {
		msg_class_name  = flat_name.element_list_[1];
		msg_class_known = true;
		if (msg_class_name == "INFO")
			msg_class_log  = LogLevel_Info;
		else if (msg_class_name == "WARN")
			msg_class_log  = LogLevel_Warning;
		else if (msg_class_name == "ERROR")
			msg_class_log  = LogLevel_Error;
		else
			msg_class_known = false;
	}

	RvAdvisoryAction().ProcessRvAdvisory(listener_ptr, msg_recv, s_name,
		msg_class_name, msg_class_known, msg_class_log, flat_name);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvAdvisory_h__HH

