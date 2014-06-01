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

		Copyright Michael L. Brock 2001 - 2014.
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

#include <BoostX.hpp>

#include <RvUtilX/RvAdvMsg.hpp>
#include <RvUtilX/Cm.hpp>
#include <RvUtilX/RvCallback.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

//	CODE NOTE: Needed only for the GetRvField() functions. To be removed.
#include <RvUtilX/GetRvField.hpp>

// CODE NOTE: Needed only within the cpp... To be removed.
#include <Utility/StringSupport.hpp>
#include <Utility/LogManager.hpp>
#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
typedef boost::function6<
	void,
	TibrvListener *,
	const TibrvMsg &,
	const char *,
	const char *,
	bool,
	MLB::Utility::LogLevel> RvAdvActionType;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvAdvAction {
public:
	RvAdvAction();
	~RvAdvAction();

	void operator () (TibrvListener *listener_ptr,
		const TibrvMsg &msg_recv, const char *subject_ptr,
		const char *msg_class_name, bool msg_class_known,
		MLB::Utility::LogLevel msg_class_log)
	{
		ProcessRvAdvisory(listener_ptr, msg_recv, subject_ptr, msg_class_name,
			msg_class_known, msg_class_log);
	}

	void        ProcessRvAdvisory(TibrvListener *listener_ptr,
		const TibrvMsg &msg_recv, const char *subject_ptr,
		const char *msg_class_name, bool msg_class_known,
		MLB::Utility::LogLevel msg_class_log);
	std::string CreateAdvisoryDescription(TibrvListener *listener_ptr,
		const TibrvMsg &msg_recv, const char *subject_ptr);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvAction::RvAdvAction()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvAction::~RvAdvAction()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvAction::ProcessRvAdvisory(TibrvListener *listener_ptr,
	const TibrvMsg &msg_recv, const char *subject_ptr,
	const char *msg_class_name, bool msg_class_known,
	MLB::Utility::LogLevel msg_class_log)
{
try {
std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::string string_1(
		CreateAdvisoryDescription(listener_ptr, msg_recv, subject_ptr));
std::cout << "[" << subject_ptr << "]" << std::endl;
	std::cout << MLB::Utility::TimeSpec::Now() << " " <<
		MLB::Utility::ConvertLogLevelToText(msg_class_log) <<
		std::setw(10) << MLB::Utility::CurrentProcessId() << " " <<
		string_1 << std::endl;
std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
	std::string string_2;
	RvAdvMsg::ConvertMsgToString(msg_recv, string_2, true);
std::cout
	<< MLB::Utility::PadLeft("", MLB::Utility::Length_TimeSpec) << " "
	<< MLB::Utility::PadLeft("", 8) << " "
	<< MLB::Utility::PadLeft("", 10) << " "
	<< string_2 << std::endl;
if (string_1.find(string_2) == std::string::npos)
	throw RvException("Unable to locate the 'RvAdvMsg::ConvertMsgToString()' "
		"string within the 'RvAdvAction::CreateAdvisoryDescription()' "
		"string.");

}
catch (const std::exception &except) {
	std::cout << "********** " << except.what() << std::endl;
std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
std::cout << RvMsgToString(msg_recv, RvMsgToStringFlags_Mask) << std::endl;
}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvAction::CreateAdvisoryDescription(
	TibrvListener *listener_ptr, const TibrvMsg &msg_recv,
	const char *subject_ptr)
{
	std::string out_text;

	((listener_ptr->getClosure() == NULL) ?
		RvAdvMsg::DefaultAdvToString2 :
		reinterpret_cast<GetAdvTextFunc>(listener_ptr->getClosure()))
		(msg_recv, out_text);

	//	Get a description of the transport...
	std::string transport_string;
	try {
		TibrvTransport *transport_ptr    = GetTransport(listener_ptr);
		tibrvTransport  transport_handle = TransportGetHandle(*transport_ptr);
		std::string    tmp_string(TransportGetDescription(*transport_ptr));
		transport_string = "Tib/Rv transport " +
			MLB::Utility::ValueToStringHex(transport_handle) + " ('" +
			tmp_string + "')";
		if (TransportIsProcessTransport(*transport_ptr))
			transport_string += " [default transport]";
		if (listener_ptr->getQueue() == GetDefaultQueue())
			transport_string += " [default queue]";
	}
	catch (const std::exception &) {
		transport_string = "*NO TRANSPORT DESCRIPTION*";
	}

	return(out_text + " on transport " + transport_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvAdvisoryControl {
private:
	typedef RvCallbackMsgEvent<RvAdvisoryControl> MyMsgCallbackType;
	typedef boost::shared_ptr<MyMsgCallbackType>  MyMsgCallbackTypeSPtr;

	struct MyListenerInfo {
		MyListenerInfo(RvAdvFlagType advisory_type, unsigned int advisory_flag,
			boost::shared_ptr<TibrvListener> &listener_sptr);
		//	Used only for std::set::find()...
		MyListenerInfo(RvAdvFlagType advisory_type, unsigned int advisory_flag);

		bool operator < (const MyListenerInfo &other) const;

		RvAdvFlagType                    advisory_type_;
		unsigned int                     advisory_flag_;
		boost::shared_ptr<TibrvListener> listener_sptr_;
	};

	typedef std::set<MyListenerInfo>      MyListenerSet;
	typedef MyListenerSet::iterator       MyListenerSetIter;
	typedef MyListenerSet::const_iterator MyListenerSetIterC;

	struct MyTransportInfo {
		MyTransportInfo(TibrvQueue *queue_ptr, TibrvCallback *callback_ptr,
			TibrvTransport *transport_ptr, RvAdvFlags rv_flags = Rv_Default,
			CmAdvFlags cm_flags = Cm_Default, FtAdvFlags ft_flags = Ft_Default);
		//	Used only for std::set::find()...
		explicit MyTransportInfo(TibrvTransport *transport_ptr);

		bool operator < (const MyTransportInfo &other) const;

		TibrvTransport    *transport_ptr_;
		MyListenerSet      listener_list_;
		RvAdvEventInfoSet  event_set_;

		void SubscribeToAdvisories(unsigned int flags,
			const RvAdvFlagsMapList &map_list, TibrvQueue *queue_ptr,
			TibrvCallback *callback_ptr, TibrvTransport *transport_ptr);
		bool AddListener(TibrvQueue *queue_ptr, TibrvCallback *callback_ptr,
			const RvAdvFlagsMap &map_ref);
		bool RemoveListener(const RvAdvFlagsMap &map_ref);
	};

	typedef std::set<MyTransportInfo>      MyTransportSet;
	typedef MyTransportSet::iterator       MyTransportSetIter;
	typedef MyTransportSet::const_iterator MyTransportSetIterC;

public:
	explicit RvAdvisoryControl(const RvAdvActionType &advisory_action =
		RvAdvAction());
	explicit RvAdvisoryControl(TibrvQueue &queue_ref,
		const RvAdvActionType &advisory_action = RvAdvAction());
	explicit RvAdvisoryControl(TibrvQueueGroup &queue_group,
		const RvAdvActionType &advisory_action = RvAdvAction());

	~RvAdvisoryControl();

	bool AddDefaultTransport(RvAdvFlags rv_flags = Rv_Default);
	bool AddTransport(TibrvTransport *transport_ptr,
		RvAdvFlags rv_flags = Rv_Default, CmAdvFlags cm_flags = Cm_None,
		FtAdvFlags ft_flags = Ft_None);
	bool AddTransport(TibrvCmTransport *transport_ptr,
		RvAdvFlags rv_flags = Rv_Default, CmAdvFlags cm_flags = Cm_Default);
	bool AddTransport(TibrvCmQueueTransport *transport_ptr,
		RvAdvFlags rv_flags = Rv_Default, CmAdvFlags cm_flags = Cm_Default);
	bool AddTransport(TibrvFtMember *transport_ptr,
		RvAdvFlags rv_flags = Rv_Default, FtAdvFlags ft_flags = Ft_Default);
	bool AddTransport(TibrvFtMonitor *transport_ptr,
		RvAdvFlags rv_flags = Rv_Default, FtAdvFlags ft_flags = Ft_Default);
	bool AddTransportCm(TibrvTransport *transport_ptr,
		RvAdvFlags rv_flags = Rv_Default, CmAdvFlags cm_flags = Cm_Default);
	bool AddTransportFt(TibrvTransport *transport_ptr,
		RvAdvFlags rv_flags = Rv_Default, FtAdvFlags ft_flags = Ft_Default);
	bool RemoveTransport(TibrvTransport *transport_ptr);

	bool AddListener(TibrvTransport *transport_ptr,
		RvAdvFlags rv_flags);
	bool AddListener(TibrvTransport *transport_ptr,
		CmAdvFlags cm_flags);
	bool AddListener(TibrvTransport *transport_ptr,
		FtAdvFlags ft_flags);
	bool AddListener(TibrvTransport *transport_ptr, RvAdvFlagType advisory_type,
		unsigned int advisory_flag);
	bool RemoveListener(TibrvTransport *transport_ptr,
		RvAdvFlags rv_flags);
	bool RemoveListener(TibrvTransport *transport_ptr,
		CmAdvFlags cm_flags);
	bool RemoveListener(TibrvTransport *transport_ptr,
		FtAdvFlags ft_flags);
	bool RemoveListener(TibrvTransport *transport_ptr,
		RvAdvFlagType advisory_type, unsigned int advisory_flag);

	boost::shared_ptr<TibrvQueue> GetQueuePtr();
	
	void ProcessEventOnMsg(TibrvMsgCallback *callback_ptr,
		TibrvListener *listener_ptr, TibrvMsg &msg_recv);
	void ReportException(const std::exception *except_ptr);

	std::vector<std::string> GetEventInfoStringList() const;

	RvContext                     rv_context_;
	RvAdvActionType               advisory_action_;
	MyTransportSet                transport_set_;
	boost::shared_ptr<TibrvQueue> queue_ptr_;
	MyMsgCallbackType             callback_;

private:
	MLB_Utility_NonCopyable_Macro(RvAdvisoryControl);

	RvAdvisoryControl &GetThisRef();
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl::MyListenerInfo::MyListenerInfo(RvAdvFlagType advisory_type,
	unsigned int advisory_flag, boost::shared_ptr<TibrvListener> &listener_sptr)
	:advisory_type_(advisory_type)
	,advisory_flag_(advisory_flag)
	,listener_sptr_(listener_sptr)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl::MyListenerInfo::MyListenerInfo(RvAdvFlagType advisory_type,
	unsigned int advisory_flag)
	:advisory_type_(advisory_type)
	,advisory_flag_(advisory_flag)
	,listener_sptr_()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::MyListenerInfo::operator < (const MyListenerInfo &other)
	const
{
	return((advisory_type_ < other.advisory_type_) ? true :
		(advisory_flag_ < other.advisory_flag_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl::MyTransportInfo::MyTransportInfo(TibrvQueue *queue_ptr,
	TibrvCallback *callback_ptr, TibrvTransport *transport_ptr,
	RvAdvFlags rv_flags,
	CmAdvFlags cm_flags,
	FtAdvFlags ft_flags)
	:transport_ptr_(transport_ptr)
	,listener_list_()
	,event_set_()
{
	RvAdvMsg::CreateEventInfoSet(event_set_);

	SubscribeToAdvisories(rv_flags, RvAdvMsg::GetFlagsMapRv(),
		queue_ptr, callback_ptr, transport_ptr);
	SubscribeToAdvisories(cm_flags, RvAdvMsg::GetFlagsMapCm(),
		queue_ptr, callback_ptr, transport_ptr);
	SubscribeToAdvisories(ft_flags, RvAdvMsg::GetFlagsMapFt(),
		queue_ptr, callback_ptr, transport_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl::MyTransportInfo::MyTransportInfo(
	TibrvTransport *transport_ptr)
	:transport_ptr_(transport_ptr)
	,listener_list_()
	,event_set_()
{
	RvAdvMsg::CreateEventInfoSet(event_set_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::MyTransportInfo::operator < (
	const MyTransportInfo &other) const
{
	return(transport_ptr_ < other.transport_ptr_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvisoryControl::MyTransportInfo::SubscribeToAdvisories(
	unsigned int flags, const RvAdvFlagsMapList &map_list, TibrvQueue *queue_ptr,
	TibrvCallback *callback_ptr, TibrvTransport *transport_ptr)
{
	if (flags) {
		unsigned int count_1;
		for (count_1 = 0; count_1 < map_list.first; ++count_1) {
			if (flags & map_list.second[count_1].advisory_flag_)
				AddListener(queue_ptr, callback_ptr, map_list.second[count_1]);
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::MyTransportInfo::AddListener(TibrvQueue *queue_ptr,
	TibrvCallback *callback_ptr, const RvAdvFlagsMap &map_ref)
{
	if (listener_list_.find(MyListenerInfo(map_ref.advisory_type_,
		map_ref.advisory_flag_)) != listener_list_.end())
		return(false);

	boost::shared_ptr<TibrvListener> listener_sptr(new TibrvListener);

	CreateListener(*queue_ptr, *callback_ptr, *transport_ptr_, *listener_sptr,
		map_ref.advisory_subject_, map_ref.get_adv_text_func_);

	listener_list_.insert(MyListenerInfo(map_ref.advisory_type_,
		map_ref.advisory_flag_, listener_sptr));

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::MyTransportInfo::RemoveListener(
	const RvAdvFlagsMap &map_ref)
{
	MyListenerSetIter iter_f(listener_list_.find(MyListenerInfo(
		map_ref.advisory_type_, map_ref.advisory_flag_)));

	if (iter_f == listener_list_.end())
		return(false);

	iter_f->listener_sptr_->destroy();

	listener_list_.erase(iter_f);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl::RvAdvisoryControl(const RvAdvActionType &advisory_action)
	:rv_context_()
	,advisory_action_(advisory_action)
	,transport_set_()
	,queue_ptr_(GetDefaultQueue(), MLB::BoostX::NullDeleter())
	,callback_(GetThisRef())
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl::RvAdvisoryControl(TibrvQueue &queue_ref,
	const RvAdvActionType &advisory_action)
	:rv_context_()
	,advisory_action_(advisory_action)
	,transport_set_()
	,queue_ptr_(&queue_ref, MLB::BoostX::NullDeleter())
	,callback_(GetThisRef())
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl::RvAdvisoryControl(TibrvQueueGroup &queue_group,
	const RvAdvActionType &advisory_action)
	:rv_context_()
	,advisory_action_(advisory_action)
	,transport_set_()
	,queue_ptr_(new TibrvQueue)
	,callback_(GetThisRef())
{
	AddToQueueGroup(queue_group, QueueCreate(*queue_ptr_.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl::~RvAdvisoryControl()
{
	while (!transport_set_.empty())
		RemoveTransport(transport_set_.begin()->transport_ptr_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddDefaultTransport(RvAdvFlags rv_flags)
{
	return(AddTransport(GetProcessTransportPtr(), rv_flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddTransport(TibrvTransport *transport_ptr,
	RvAdvFlags rv_flags, CmAdvFlags cm_flags, FtAdvFlags ft_flags)
{
	if (transport_ptr == NULL)
		MLB::Utility::ThrowInvalidArgument("Invocation of '"
			"RvAdvisoryControl::AddTransport()' with a 'NULL' transport pointer.");

	if (transport_set_.find(MyTransportInfo(transport_ptr)) !=
		transport_set_.end())
		return(false);

	transport_set_.insert(MyTransportInfo(queue_ptr_.get(), &callback_,
		transport_ptr, rv_flags, cm_flags, ft_flags));

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddTransport(TibrvCmTransport *transport_ptr,
	RvAdvFlags rv_flags, CmAdvFlags cm_flags)
{
	return(AddTransportCm(GetTransport(*transport_ptr), rv_flags, cm_flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddTransport(TibrvCmQueueTransport *transport_ptr,
	RvAdvFlags rv_flags, CmAdvFlags cm_flags)
{
	return(AddTransportCm(GetTransport(*transport_ptr), rv_flags, cm_flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddTransport(TibrvFtMember *transport_ptr,
	RvAdvFlags rv_flags, FtAdvFlags ft_flags)
{
	return(AddTransportFt(GetTransport(*transport_ptr), rv_flags, ft_flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddTransport(TibrvFtMonitor *transport_ptr,
	RvAdvFlags rv_flags, FtAdvFlags ft_flags)
{
	return(AddTransportFt(GetTransport(*transport_ptr), rv_flags, ft_flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddTransportCm(TibrvTransport *transport_ptr,
	RvAdvFlags rv_flags, CmAdvFlags cm_flags)
{
	return(AddTransport(transport_ptr, rv_flags, cm_flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddTransportFt(TibrvTransport *transport_ptr,
	RvAdvFlags rv_flags, FtAdvFlags ft_flags)
{
	return(AddTransport(transport_ptr, rv_flags, Cm_None, ft_flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::RemoveTransport(TibrvTransport *transport_ptr)
{
	MyTransportSetIter iter_f(transport_set_.find(
		MyTransportInfo(transport_ptr)));

	if (iter_f == transport_set_.end())
		return(false);

	MyListenerSetIter iter_b(iter_f->listener_list_.begin());
	MyListenerSetIter iter_e(iter_f->listener_list_.end());

	while (iter_b != iter_e) {
		iter_b->listener_sptr_->destroy();
		++iter_b;
	}

	transport_set_.erase(iter_f);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddListener(TibrvTransport *transport_ptr,
	RvAdvFlags rv_flags)
{
	return(AddListener(transport_ptr, RvAdvFlagType_Rv, rv_flags & Rv_Mask));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddListener(TibrvTransport *transport_ptr,
	CmAdvFlags cm_flags)
{
	return(AddListener(transport_ptr, RvAdvFlagType_Cm, cm_flags & Cm_Mask));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddListener(TibrvTransport *transport_ptr,
	FtAdvFlags ft_flags)
{
	return(AddListener(transport_ptr, RvAdvFlagType_Ft, ft_flags & Ft_Mask));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::AddListener(TibrvTransport *transport_ptr,
	RvAdvFlagType advisory_type, unsigned int advisory_flag)
{
	const RvAdvFlagsMap *map_ptr =
		RvAdvMsg::GetFlagsMapPtr(advisory_type, advisory_flag);

	if (map_ptr == NULL)
		return(false);

	MyTransportSetIter iter_f(transport_set_.find(
		MyTransportInfo(transport_ptr)));

	return((iter_f == transport_set_.end()) ? false :
		const_cast<MyTransportInfo *>(&(*iter_f))->AddListener(
		queue_ptr_.get(), &callback_, *map_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::RemoveListener(TibrvTransport *transport_ptr,
	RvAdvFlags rv_flags)
{
	return(RemoveListener(transport_ptr, RvAdvFlagType_Rv, rv_flags & Rv_Mask));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::RemoveListener(TibrvTransport *transport_ptr,
	CmAdvFlags cm_flags)
{
	return(RemoveListener(transport_ptr, RvAdvFlagType_Cm, cm_flags & Cm_Mask));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::RemoveListener(TibrvTransport *transport_ptr,
	FtAdvFlags ft_flags)
{
	return(RemoveListener(transport_ptr, RvAdvFlagType_Ft, ft_flags & Ft_Mask));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvisoryControl::RemoveListener(TibrvTransport *transport_ptr,
	RvAdvFlagType advisory_type, unsigned int advisory_flag)
{
	const RvAdvFlagsMap *map_ptr =
		RvAdvMsg::GetFlagsMapPtr(advisory_type, advisory_flag);

	if (map_ptr == NULL)
		return(false);

	MyTransportSetIter iter_f(transport_set_.find(
		MyTransportInfo(transport_ptr)));

	return((iter_f == transport_set_.end()) ? false :
		const_cast<MyTransportInfo *>(&(*iter_f))->RemoveListener(*map_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
boost::shared_ptr<TibrvQueue> RvAdvisoryControl::GetQueuePtr()
{
	return(queue_ptr_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvisoryControl::ProcessEventOnMsg(TibrvMsgCallback * /* callback_ptr */,
	TibrvListener *listener_ptr, TibrvMsg &msg_recv)
{
	const char *subject_ptr(GetSendSubjectPtr(msg_recv));

	if (subject_ptr == NULL)
		throw RvException("The send subject name of a Tib/Rv message received "
			"in a TibrvMsgCallback is 'NULL'.");

	TibrvTransport *transport_ptr = GetTransport(listener_ptr);

	MyTransportSetIter transport_iter_f(transport_set_.find(
		MyTransportInfo(transport_ptr)));

	//	If we can't find the transport, it must have been removed after the
	//	message arrived but before we dequeued it...
	if (transport_iter_f == transport_set_.end())
		return;

	SubjectNameFlat        flat_name(subject_ptr);
	char                   msg_class_name[MLB::Utility::LogLevelTextMaxLength+1];
	bool                   msg_class_known;
	MLB::Utility::LogLevel msg_class_log;

	strcpy(msg_class_name, (flat_name.GetElementCount() < 2) ? "" :
		flat_name.element_list_[1]);
	msg_class_known = true;

	if (!strcmp(msg_class_name, "INFO"))
		msg_class_log  = MLB::Utility::LogLevel_Info;
	else if (!strcmp(msg_class_name, "WARN"))
		msg_class_log  = MLB::Utility::LogLevel_Warning;
	else if (!strcmp(msg_class_name, "ERROR"))
		msg_class_log  = MLB::Utility::LogLevel_Error;
	else {
		strcpy(msg_class_name, "NOTICE");
		msg_class_known = false;
		msg_class_log   = MLB::Utility::LogLevel_Notice;
	}

	RvAdvMsg::UpdateEventInfoSet(msg_recv,
		const_cast<MyTransportInfo *>(&(*transport_iter_f))->event_set_,
		MLB::Utility::TimeSpec(), false);

	advisory_action_(listener_ptr, msg_recv, subject_ptr,
		msg_class_name, msg_class_known, msg_class_log);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvisoryControl::ReportException(const std::exception *except_ptr)
{
	std::cerr << "Exception encountered in Tib/Rendezvous callback: " <<
		((except_ptr != NULL) ? except_ptr->what() : "*UNSPECIFIED*") <<
		std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> RvAdvisoryControl::GetEventInfoStringList() const
{
	std::vector<std::string> tmp_list;

	MyTransportSetIterC iter_b(transport_set_.begin());
	MyTransportSetIterC iter_e(transport_set_.end());

	while (iter_b != iter_e) {
		tmp_list.push_back(TransportIsProcessTransport(*iter_b->transport_ptr_) ?
			"*PROCESS TRANSPORT*" :
			SNDSpec(*iter_b->transport_ptr_).ToStringSimple());
		RvAdvEventInfoSetIterC iter_ei_b(iter_b->event_set_.begin());
		RvAdvEventInfoSetIterC iter_ei_e(iter_b->event_set_.end());
		while (iter_ei_b != iter_ei_e) {
			std::ostringstream o_str;
			o_str
				<< "   " << iter_ei_b->advisory_type_ << ":0x"
				<< std::setfill('0') << std::hex << std::setw(8) <<
					iter_ei_b->advisory_flag_ << std::setfill(' ') << std::dec <<
					" "
				<< std::left << std::setw(48) << iter_ei_b->subject_pattern_ <<
					std::right << " "
				<< std::setw(22) << iter_ei_b->occurrence_count_ << " "
				<< iter_ei_b->first_occurrence_time_ << " "
				<< iter_ei_b->last_occurrence_time_;
			tmp_list.push_back(o_str.str());
			++iter_ei_b;
		}
		++iter_b;
	}

	return(tmp_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvisoryControl &RvAdvisoryControl::GetThisRef()
{
	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvAdvisory_h__HH


#ifdef TEST_MAIN

#include <RvUtilX/AddRvField.hpp>
#include <RvUtilX/RvParseCmdLineArg.hpp>
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <RvUtilX\RvCallbackFt.hpp>

using namespace MLB::RvUtilX;

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const std::string &test_name, const char *result)
{
	std::cout << "   : " << std::left << std::setw(24) << test_name <<
		std::right << ": " << result << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const std::string &test_name,
	const std::string &result)
{
	TEST_EmitResults(test_name, result.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const std::string &test_name,
	const std::exception &except)
{
	TEST_EmitResults(test_name, "ERROR: " + std::string(except.what()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Test code for RvTransport.cpp...
void TEST_TibrvTransportSupport_OneTransport(TibrvTransport *transport_ptr,
	const std::string &transport_name, int &return_code)
{
	std::string test_name("Regression Test: TibrvTransportSupport.cpp:[" +
		transport_name + "]");

	std::cout << test_name << std::endl;
	std::cout << MLB::Utility::PadLeft("",
		static_cast<unsigned int>(test_name.size()), '-') << std::endl;

	try {
		bool result = TransportCheckIsValid(*transport_ptr);
		TEST_EmitResults("TransportCheckIsValid()",
			MLB::Utility::AnyToString(result));
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportCheckIsValid()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		bool result = TransportIsProcessTransport(*transport_ptr);
		TEST_EmitResults("TransportIsProcessTransport()",
			MLB::Utility::AnyToString(result));
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportIsProcessTransport()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		std::string result(TransportGetDescription(*transport_ptr));
		TEST_EmitResults("TransportGetDescription()", result);
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportGetDescription()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		tibrvTransport result = TransportGetHandle(*transport_ptr);
		TEST_EmitResults("TransportGetHandle()", MLB::Utility::AnyToString(result));
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportGetHandle()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		std::string result(TransportCreateInbox(*transport_ptr));
		TEST_EmitResults("TransportCreateInbox()", result);
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportCreateInbox()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		TibrvMsg out_msg;
		AddRvField(out_msg, "FieldName", "FieldValue");
		SetSendSubject(out_msg, "Please.Ignore.This.Message");
		TransportSend(*transport_ptr, out_msg);
		TEST_EmitResults("TransportSend()", "(done)");
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportSend()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		TibrvMsg request_msg;
		TibrvMsg reply_msg;
		AddRvField(reply_msg, "FieldName", "FieldValue");
		SetSendSubject(request_msg, "Please.Ignore.This.Message");
		SetReplySubject(request_msg, "Please.Dont.Reply.To.This.Message");
		TransportSendReply(*transport_ptr, reply_msg, request_msg);
		TEST_EmitResults("TransportSendReply()", "(done)");
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportSendReply()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		TibrvMsg request_msg;
		TibrvMsg reply_msg;
		AddRvField(request_msg, "FieldName", "FieldValue");
		SetSendSubject(request_msg, "Please.Ignore.This.Message");
		SetReplySubject(request_msg, "Please.Dont.Reply.To.This.Message");
		TransportSendRequest(*transport_ptr, request_msg, reply_msg, 0.1);
		TEST_EmitResults("TransportSendRequest()", "(done)");
	}
	catch (const RvExceptionTimeOut &) {
		TEST_EmitResults("TransportSendRequest()", "Expected time-out occurred.");
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportSendRequest()", except);
		return_code = EXIT_FAILURE;
	}

	if (!TransportIsProcessTransport(*transport_ptr)) {
		try {
			std::string description("Description=" + test_name);
			TransportSetDescription(*transport_ptr, description);
			TEST_EmitResults("TransportSetDescription()", "(done)");
		}
		catch (const std::exception &except) {
		TEST_EmitResults("TransportSetDescription()", except);
			return_code = EXIT_FAILURE;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Test code for RvTransportNet.cpp...
void TEST_TibrvTransportSupport_OneTransport(TibrvNetTransport *transport_ptr,
	const std::string &transport_name, int &return_code)
{
	TEST_TibrvTransportSupport_OneTransport(
		static_cast<TibrvTransport *>(transport_ptr), transport_name,
		return_code);

	try {
		std::string result(TransportGetService(*transport_ptr));
		TEST_EmitResults("TransportGetService()", result);
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportGetService()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		std::string result(TransportGetNetwork(*transport_ptr));
		TEST_EmitResults("TransportGetNetwork()", result);
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportGetNetwork()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		std::string result(TransportGetDaemon(*transport_ptr));
		TEST_EmitResults("TransportGetDaemon()", result);
	}
	catch (const std::exception &except) {
		TEST_EmitResults("TransportGetDaemon()", except);
		return_code = EXIT_FAILURE;
	}

	try {
		SNDSpec result(GetSNDSpec(*transport_ptr));
		TEST_EmitResults("GetSNDSpec()", result.ToStringSimple());
	}
	catch (const std::exception &except) {
		TEST_EmitResults("GetSNDSpec()", except);
		return_code = EXIT_FAILURE;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Test code for RvTransportCm.cpp...
void TEST_TibrvTransportSupport_OneTransport(TibrvCmTransport *transport_ptr,
	const std::string &transport_name, int &return_code)
{
	TEST_TibrvTransportSupport_OneTransport(GetTransport(*transport_ptr),
		transport_name, return_code);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Test code for RvTransport.cpp...
void TEST_TibrvTransportSupport()
{
	RvContext rv_context;

	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	int return_code = EXIT_SUCCESS;

	//	TibrvTransport test...
	{
		//	Default transport test...
		TEST_TibrvTransportSupport_OneTransport(GetProcessTransportPtr(),
			"Tibrv::processTransport()", return_code);
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
	}

	//	TibrvNetTransport test...
	{
		std::string transport_name("TibrvNetTransport(Service=12345)");
		try {
			boost::shared_ptr<TibrvNetTransport> trans_ptr(new TibrvNetTransport);
			CreateTransport(*trans_ptr, "12345", "", "");
			TEST_TibrvTransportSupport_OneTransport(trans_ptr.get(),
				transport_name, return_code);
		}
		catch (const std::exception &except) {
			std::cerr << "Regression Test: RvTransport.cpp:[" <<
				transport_name << "]: " << except.what() << std::endl;
			return_code = EXIT_FAILURE;
		}
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
	}

	{
		std::string transport_name("TibrvCmTransport(Service=12345) #1");
		try {
			boost::shared_ptr<TibrvNetTransport> trans_ptr(new TibrvNetTransport);
			CreateTransport(*trans_ptr, "12345", "", "");
			boost::shared_ptr<TibrvCmTransport> cmtrans_ptr(new TibrvCmTransport);
			CreateTransport(*cmtrans_ptr, *trans_ptr);
/*
			TEST_TibrvTransportSupport_OneTransport(trans_ptr.get(),
				transport_name, return_code);
*/
			TEST_TibrvTransportSupport_OneTransport(cmtrans_ptr.get(),
				transport_name, return_code);
		}
		catch (const std::exception &except) {
			std::cerr << "Regression Test: RvTransport.cpp:[" <<
				transport_name << "]: " << except.what() << std::endl;
			return_code = EXIT_FAILURE;
		}
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
	}

	{
		std::string transport_name("TibrvCmTransport(Service=12345) #2");
		try {
			boost::shared_ptr<TibrvNetTransport> trans_ptr(new TibrvNetTransport);
			CreateTransport(*trans_ptr, "12345", "", "");
			boost::shared_ptr<TibrvCmTransport> cmtrans_ptr(new TibrvCmTransport);
			CreateTransport(*cmtrans_ptr, *trans_ptr, "MyCmName", false,
				"MyLedgerName", false);
			TEST_TibrvTransportSupport_OneTransport(trans_ptr.get(),
				transport_name, return_code);
		}
		catch (const std::exception &except) {
			std::cerr << "Regression Test: RvTransport.cpp:[" <<
				transport_name << "]: " << except.what() << std::endl;
			return_code = EXIT_FAILURE;
		}
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
	}

	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
	std::cout << std::endl;

	if (return_code != EXIT_SUCCESS)
		MLB::Utility::ThrowException("Errors occurred in the "
			"RvTransport.cpp' regression test.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct FtAdvisoriesTest {
	FtAdvisoriesTest(RvAdvisoryControl &advisory_control,
		const RvAdvFlagsAll &adv_flags,
		boost::shared_ptr<TibrvTransport> &source_trans_sptr,
		unsigned int weight, unsigned int active_goal, double heartbeat_interval,
		double preparation_interval, double activation_interval)
		:advisory_control_(advisory_control)
		,transport_sptr_(new TibrvNetTransport)
		,callback_ft_(GetThisRef())
		,ft_spec_("Test.Ft.Group", weight, active_goal, heartbeat_interval,
			preparation_interval, activation_interval)
		,ft_member_()
	{
		//	Create a copy of the transport...
		SNDSpec(*source_trans_sptr).CreateTransport(*transport_sptr_);
		//	Join the fault tolerance group...
		ft_spec_.CreateFtMember(ft_member_, *advisory_control.GetQueuePtr().get(),
			callback_ft_, *transport_sptr_);
		advisory_control.AddTransportFt(transport_sptr_.get(),
			Rv_Default, adv_flags.ft_flags_);
	}

	void ProcessEventOnFtAction(TibrvFtMemberCallback * /* callback_ptr */,
		TibrvFtMember * /* ftMember */, const char *group_name, tibrvftAction action) {
		std::cout << "RvFT callback: Action = '" <<
			FtActionToDescription(action) << "', Group Name = '" <<
			group_name << "'." << std::endl;
	}
	void ReportException(const std::exception *except_ptr) {
		std::cout << "Exception encountered in Tib/Rendezvous callback: " <<
			((except_ptr != NULL) ? except_ptr->what() : "*UNSPECIFIED*") <<
			std::endl;
	}

	RvAdvisoryControl                         &advisory_control_;
	boost::shared_ptr<TibrvNetTransport>       transport_sptr_;
	RvCallbackFtActionEvent<FtAdvisoriesTest>  callback_ft_;
	FtSpec                                     ft_spec_;
	TibrvFtMember                              ft_member_;

private:
	FtAdvisoriesTest &GetThisRef() {
		return(*this);
	}

	MLB_Utility_NonCopyable_Macro(FtAdvisoriesTest);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_DispatchEvents(RvAdvisoryControl &advisory_control)
{
	TibrvStatus rv_status;

	rv_status = advisory_control.GetQueuePtr()->timedDispatch(0.1);

	if (MLB::Utility::CriticalEventTest())
		throw MLB::Utility::ExceptionCriticalEvent("Performing an asynchronous "
			"exit due to receipt of a signal.");

	if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
		throw RvExceptionStatus(rv_status,
			"Error occurred while dispatching from a Rendezvous queue.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_DispatchEventsLoop(RvAdvisoryControl &advisory_control,
	unsigned int seconds_to_dispatch)
{
	std::cout << "Will dispatch events for " << seconds_to_dispatch <<
		" seconds..." << std::endl;

	MLB::Utility::TimeT end_time;

	end_time.AddSeconds(seconds_to_dispatch);

	while (MLB::Utility::TimeT() < end_time) {
		if (MLB::Utility::CriticalEventTest())
			throw MLB::Utility::ExceptionCriticalEvent();
		::Sleep(10);
		TEST_DispatchEvents(advisory_control);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<TibrvTransport> TransportSPtr;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_AdvisoryManagement(std::vector<TransportSPtr> &transport_list)
{
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << "Advisory Subscription Management Test"  << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;

	RvAdvisoryControl advisory_control;

	advisory_control.AddTransport(transport_list[0].get(), Rv_Mask);
	std::cout << "Added transport " <<
		SNDSpec::FromTransport(*transport_list[0]) << std::endl;

	if (transport_list.size() > 1) {
		advisory_control.AddTransport(transport_list[1].get(), Rv_Mask);
		std::cout << "Added transport " <<
			SNDSpec::FromTransport(*transport_list[1]).ToStringSimple() <<
			std::endl;
		advisory_control.RemoveTransport(GetProcessTransportPtr());
		std::cout << "Removed transport " <<
			SNDSpec::FromTransport(*transport_list[1]) << std::endl;
	}

	const RvAdvFlagsMapList &map_list(RvAdvMsg::GetFlagsMapRv());

	size_t count_1;

	for (count_1 = 0; count_1 < map_list.first; ++count_1) {
		advisory_control.RemoveListener(transport_list[0].get(),
		RvAdvFlagType_Rv, map_list.second[count_1].advisory_flag_);
		std::cout << "Removed listener from transport " <<
			SNDSpec::FromTransport(*transport_list[0]).ToStringSimple() <<
			": " << map_list.second[count_1].advisory_subject_ << std::endl;
	}

	unsigned int random_index = rand() % map_list.first;

	advisory_control.AddListener(transport_list[0].get(),
		static_cast<RvAdvFlags>(map_list.second[random_index].advisory_flag_));
	std::cout << "Re-added listener to transport " <<
		SNDSpec::FromTransport(*transport_list[0]).ToStringSimple() << ": " <<
		map_list.second[random_index].advisory_subject_ << std::endl;
	
	TEST_DispatchEventsLoop(advisory_control, 60);

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "Advisory Subscription Management Test Complete"  << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_RvAdvisories(std::vector<TransportSPtr> &transport_list,
	const RvAdvFlagsAll &adv_flags)
{
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << "Comprehensive Advisories Test"  << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;

	RvAdvisoryControl advisory_control;
	size_t            count_1;

	for (count_1 = 0; count_1 < transport_list.size(); ++count_1)
		advisory_control.AddTransport(transport_list[count_1].get(),
			adv_flags.rv_flags_, adv_flags.cm_flags_, adv_flags.ft_flags_);

	advisory_control.AddDefaultTransport(adv_flags.rv_flags_);

	TEST_DispatchEventsLoop(advisory_control, 120);

	std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
	std::vector<std::string> out_list(advisory_control.GetEventInfoStringList());
	std::copy(out_list.begin(), out_list.end(),
		std::ostream_iterator<MLB::Utility::StringVector::value_type>(
			std::cout, "\n"));

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "Comprehensive Advisories Test Complete"  << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_FtAdvisories(boost::shared_ptr<TibrvTransport> &source_trans_sptr,
	const RvAdvFlagsAll &adv_flags)
{
	RvAdvisoryControl advisory_control;

	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << "Fault Tolerance Advisories Test"  << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;

	std::cout << "Creating the first FT member..." << std::endl;
	FtAdvisoriesTest ft_test_good_1(advisory_control, adv_flags,
		source_trans_sptr, 1, 1, 1.0, 0.0, 30.0);

	std::cout << "Creating an FT member with a different activation "
		"interval..." << std::endl;
	FtAdvisoriesTest ft_test_bad_1(advisory_control, adv_flags,
		source_trans_sptr, 1, 1, 1.0, 0.0, 30.1);
	TEST_DispatchEvents(advisory_control);

	std::cout << std::endl;

	TEST_DispatchEvents(advisory_control);

	std::cout << "Creating an FT member with a different preparation "
		"interval..." << std::endl;
	FtAdvisoriesTest ft_test_bad_2(advisory_control, adv_flags,
		source_trans_sptr, 1, 1, 1.0, 2.0, 30.0);
	TEST_DispatchEvents(advisory_control);

	std::cout << std::endl;

	TEST_DispatchEvents(advisory_control);

	std::cout << "Creating an FT member with a different heartbeat "
		"interval..." << std::endl;
	FtAdvisoriesTest ft_test_bad_3(advisory_control, adv_flags,
		source_trans_sptr, 1, 1, 2.0, 0.0, 30.0);
	TEST_DispatchEvents(advisory_control);

	std::cout << std::endl;

	TEST_DispatchEvents(advisory_control);

	std::cout << "Creating an FT member with a different active goal ..." <<
		std::endl;
	FtAdvisoriesTest ft_test_bad_4(advisory_control, adv_flags,
		source_trans_sptr, 1, 2, 1.0, 0.0, 30.0);
	TEST_DispatchEvents(advisory_control);

	std::cout << std::endl;

	TEST_DispatchEventsLoop(advisory_control, 120);

	std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
	std::vector<std::string> out_list(advisory_control.GetEventInfoStringList());
	std::copy(out_list.begin(), out_list.end(),
		std::ostream_iterator<MLB::Utility::StringVector::value_type>(
			std::cout, "\n"));

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "Fault Tolerance Advisories Test Complete"  << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

	int         return_code = EXIT_SUCCESS;
	std::string my_name     = MLB::Utility::GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (RvParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout
			<< "USAGE: " << std::endl
			<< "   " << my_name << " "
			<< "[ " << GetRvAdvMsgFlagsHelpTextBrief() << " ] "
			<< "<snd-spec> [ <snd-spec> ... ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n" <<
	GetRvAdvMsgFlagsHelpTextExtended() << "\n\
   <snd-spec>\n\
      Is a combined specification of a Tib/Rv service, network and daemon\n\
      described in the following format:\n\
\n\
         [<service>][/[<network>][/[<daemon>]]]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		//	================================================================
		//	Construct the list of transports...
		//	----------------------------------------------------------------
//TEST_TibrvTransportSupport();
		MLB::Utility::CriticalEventContext critical_event_context;
		RvContext                          rv_context;
		std::set<SNDSpec>                  snd_set;
		std::vector<TransportSPtr>         transport_list;
		unsigned int                       count_1;
		RvAdvFlagsAll                      adv_flags(Rv_Mask, Cm_Mask, Ft_Mask);
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (RvAdvMsgParseCmdLineArg::ParseCmdLineAdvMsgFlagsSpecDefault(
				count_1, argc, argv, adv_flags))
				;
			else {
				SNDSpec this_snd(SNDSpec::FromDelimitedString(argv[count_1], "/"));
				std::cout << "Service " << std::left << std::setw(48) <<
					this_snd.ToStringSimple() << std::right << ": " << std::flush;
				if (!snd_set.insert(this_snd).second) {
					std::cout << "Already created." << std::endl;
					continue;
				}
				boost::shared_ptr<TibrvNetTransport> xport(new TibrvNetTransport);
				this_snd.CreateTransport(*xport);
				TransportSetDescription(*xport, "MLB::RvUtilX::RvAdvisoryControl test "
					"program on host " + MLB::Utility::GetHostNameCanonical() +
					", process id " +
					MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()) +
					".");
				transport_list.push_back(xport);
				std::cout << "Created." << std::endl;
			}
		}
		if (transport_list.empty())
			MLB::Utility::ThrowInvalidArgument("Expected at least one service/"
				"network/daemon parameter --- use '-help' for help.");
		std::cout << "Using flags: " << RvAdvMsgFlagsToString(adv_flags) <<
			std::endl;
		std::cout << std::endl;
	//	================================================================
		//	Comprehensive advisories test...
		//	----------------------------------------------------------------
		{
			TEST_AdvisoryManagement(transport_list);
		}
		//	================================================================
		//	Comprehensive advisories test...
		//	----------------------------------------------------------------
		{
			TEST_RvAdvisories(transport_list, adv_flags);
		}
		//	================================================================
		//	================================================================
		//	Fault tolerance advisories test...
		//	----------------------------------------------------------------
		{
			TEST_FtAdvisories(transport_list[0], adv_flags);
		}
		//	================================================================

	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);		
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

