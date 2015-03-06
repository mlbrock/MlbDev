//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB RvUtilX Boost Python Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the Boost::Python interface to the
								Tib/Rv Environment class.

	Revision History	:	2008-06-24 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <BoostX/BoostPython.hpp>

#include <RvUtilX/RvMsgAdd.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
// CODE NOTE: To be added to RvMsgAdd.cpp
TibrvMsg &MsgAddString(TibrvMsg &msg, const char *field_name,
	const char *field_value, tibrv_u16 field_id = 0)
{
	RvUtilX_THROW_TIBRV_STATUS_IF(msg.addString,
		(field_name, field_value, field_id));

	return(msg);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// CODE NOTE: To be added to RvMsgAdd.cpp
TibrvMsg &MsgAddString(TibrvMsg &msg, const std::string &field_name,
	const std::string &field_value, tibrv_u16 field_id = 0)
{
	return(MsgAddString(msg, field_name.c_str(), field_value.c_str(), field_id));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct LimitPolicyPython {
	LimitPolicyPython(tibrvQueueLimitPolicy policy = TIBRVQUEUE_DISCARD_NONE,
		unsigned int max_events = 0, unsigned int discard_amount = 0)
		:policy_(policy)
		,max_events_(max_events)
		,discard_amount_(discard_amount)
	{
	}

	std::string ToString() const
	{
		std::ostringstream o_str;

		o_str
			<< "LimitPolicy{"
			<< policy_ << "/"
			<< max_events_ << "/"
			<< discard_amount_ << "}";

		return(o_str.str());
	}

	tibrvQueueLimitPolicy policy_;
	unsigned int          max_events_;
	unsigned int          discard_amount_;
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class QueuePython {
public:
	QueuePython()
		:queue_sptr_(new TibrvQueue)
	{
		QueueCreate(*queue_sptr_);
	}

	explicit QueuePython(TibrvQueue *queue_ptr)
		:queue_sptr_(queue_ptr, MLB::BoostX::NullDeleter())
	{
	}

	~QueuePython()
	{
		DestroyInternal(true);
	}

	void Destroy()
	{
		DestroyInternal(false);
	}

	void Dispatch()
	{
		QueueDispatch(*queue_sptr_);
	}

	unsigned int GetCount() const
	{
		return(QueueGetCount(*queue_sptr_));
	}

	tibrvQueue GetHandle() const
	{
		return(QueueGetHandle(*queue_sptr_));
	}

	LimitPolicyPython GetLimitPolicy() const
	{
		LimitPolicyPython limit_policy;

		QueueGetLimitPolicy(*queue_sptr_, limit_policy.policy_,
			limit_policy.max_events_, limit_policy.discard_amount_);

		return(limit_policy);
	}

	void SetLimitPolicy(LimitPolicyPython &limit_policy)
	{
		QueueSetLimitPolicy(*queue_sptr_, limit_policy.policy_,
			limit_policy.max_events_, limit_policy.discard_amount_);
	}

	std::string GetName() const
	{
		return(QueueGetName(*queue_sptr_));
	}

	void SetName(const std::string &queue_name)
	{
		QueueSetName(*queue_sptr_, queue_name);
	}

	unsigned int GetPriority() const
	{
		return(QueueGetPriority(*queue_sptr_));
	}

	void SetPriority(unsigned int queue_priority)
	{
		QueueSetPriority(*queue_sptr_, queue_priority);
	}

	bool Poll()
	{

		return(QueuePoll(*queue_sptr_));
	}

	bool TimedDispatch(double time_out)
	{
		return(QueueTimedDispatch(*queue_sptr_,time_out));
	}

	bool IsDefaultQueue() const
	{
		return(QueueIsDefault(queue_sptr_.get()));
	}

	std::string ToString() const
	{
		return("TibrvQueue{" + MLB::Utility::AnyToString(GetHandle()) + "}");
	}

	static QueuePython Default()
	{
		return(QueuePython(GetDefaultQueue()));
	}

private:

	void DestroyInternal(bool already_destroyed_is_ok)
	{
		if (queue_sptr_.get() == NULL) {
			if (already_destroyed_is_ok)
				return;
//			throw ***
		}

		if (!IsDefaultQueue())
			QueueDestroy(*queue_sptr_);

		queue_sptr_.reset();
	}

	boost::shared_ptr<TibrvQueue> queue_sptr_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MessagePython {
public:
	MessagePython()
		:msg_sptr_(new TibrvMsg)
	{
	}
	explicit MessagePython(unsigned int initial_size)
		:msg_sptr_(new TibrvMsg(initial_size))
	{
	}
	explicit MessagePython(const std::string &msg_as_bytes)
		:msg_sptr_(new TibrvMsg(msg_as_bytes.data()))
	{
	}
	explicit MessagePython(const TibrvMsg &src_msg)
		:msg_sptr_(new TibrvMsg)
	{
		CopyMsgNoSubjects(src_msg, *msg_sptr_);
	}

	unsigned int GetFieldCount() const
	{
		return(MsgGetNumFields(*msg_sptr_));
	}
	//	Compatibility with the Tib/Rv .NET implementation with VB.NET support.
	int GetFieldCountAsInt() const
	{
		return(static_cast<int>(MsgGetNumFields(*msg_sptr_)));
	}
	tibrvMsg GetHandle() const
	{
		return(MsgGetHandle(*msg_sptr_));
	}
	std::string GetReplySubject() const
	{
		return(MsgGetReplySubject(*msg_sptr_));
	}
	void SetReplySubject(const std::string &subject_name) const
	{
		MsgSetReplySubject(*msg_sptr_, subject_name);
	}
	std::string GetSendSubject() const
	{
		return(MsgGetSendSubject(*msg_sptr_));
	}
	void SetSendSubject(const std::string &subject_name) const
	{
		MsgSetSendSubject(*msg_sptr_, subject_name);
	}
	unsigned int GetSize() const
	{
		return(MsgGetByteSize(*msg_sptr_));
	}

	void AddBool(const std::string &field_name, bool field_value,
		unsigned int field_id = 0)
	{
		MsgAddBool(*msg_sptr_, field_name, field_value, field_id);
	}
	void AddString(const std::string &field_name, const std::string &field_value,
		unsigned int field_id = 0)
	{
		MsgAddString(*msg_sptr_, field_name, field_value, field_id);
	}

	void Expand(unsigned int additional_storage)
	{
		MsgExpand(*msg_sptr_, additional_storage);
	}
	void Reset()
	{
		MsgReset(*msg_sptr_);
	}

	std::string ToByteArray() const
	{
		return(MsgToByteArray(*msg_sptr_));
	}

	// Class properties...
	unsigned int GetMinimumCustomDataTypeID()
	{
		return(TIBRVMSG_USER_FIRST);
	}
	unsigned int GetMaximumCustomDataTypeID()
	{
		return(TIBRVMSG_USER_LAST);
	}

	std::string ToString() const
	{
		std::ostringstream o_str;

		o_str
			<< "TibrvMessage{" << GetHandle() << "}";

		return(o_str.str());
	}

	TibrvMsg &GetMsgRef()
	{
		return(*msg_sptr_);
	}
	TibrvMsg &GetMsgRef() const
	{
		return(*msg_sptr_);
	}

private:
	boost::shared_ptr<TibrvMsg> msg_sptr_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class TransportPython {
protected:
	TransportPython()
		:transport_sptr_()
	{
	}
	explicit TransportPython(TibrvProcessTransport *transport_ptr)
		:transport_sptr_(transport_ptr, MLB::BoostX::NullDeleter())
	{
	}
	explicit TransportPython(TibrvNetTransport *transport_ptr)
		:transport_sptr_(transport_ptr, DestroyInternal)
	{
	}

	virtual ~TransportPython()
	{
	}

	virtual const char *GetTypeName() const
	{
		return("abstract");
	}

	boost::shared_ptr<TibrvTransport> transport_sptr_;

public:

	std::string    CreateInbox();
	void           Destroy();
	void           Send(const MessagePython &msg);
	void           SendReply(const MessagePython &reply_msg,
		const MessagePython &request_msg);
	void           SendRequest(const MessagePython &reply_msg,
		const MessagePython &request_msg, double time_out);

	tibrvTransport GetHandle() const;

	std::string ToString() const
	{
		return(std::string("Tib/Rv ") + GetTypeName() + " transport");
	}

private:

	static void DestroyInternal(TibrvTransport *transport_ptr)
	{
		TransportDestroy(transport_ptr);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string TransportPython::CreateInbox()
{
	return(TransportCreateInbox(*transport_sptr_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportPython::Destroy()
{
	TransportDestroy(*transport_sptr_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
tibrvTransport TransportPython::GetHandle() const
{
	return(TransportGetHandle(*transport_sptr_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportPython::Send(const MessagePython &msg)
{
	TransportSend(*transport_sptr_, msg.GetMsgRef());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportPython::SendReply(const MessagePython &reply_msg,
	const MessagePython &request_msg)
{
	TransportSendReply(*transport_sptr_, reply_msg.GetMsgRef(),
		request_msg.GetMsgRef());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void  TransportPython::SendRequest(const MessagePython &reply_msg,
	const MessagePython &request_msg, double time_out)
{
	TransportSendRequest(*transport_sptr_, reply_msg.GetMsgRef(),
		request_msg.GetMsgRef(), time_out);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class IntraProcessTransportPython :
	public TransportPython {
public:
	IntraProcessTransportPython()
		:TransportPython(GetProcessTransportPtr())
	{
	}

	static IntraProcessTransportPython UniqueInstance()
	{
		return(IntraProcessTransportPython());
	}

protected:
	virtual const char *GetTypeName() const
	{
		return("intra-process");
	}

protected:
	explicit IntraProcessTransportPython(TibrvProcessTransport *transport_ptr)
		:TransportPython(transport_ptr)
	{
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class NetTransportPython :
	public TransportPython {
public:
	NetTransportPython()
		:TransportPython(new TibrvNetTransport)
	{
	}
	NetTransportPython(const std::string &service, const std::string &network,
		const std::string &daemon)
		:TransportPython(new TibrvNetTransport)
	{
		CreateTransport(*static_cast<TibrvNetTransport *>(transport_sptr_.get()),
			service, network, daemon);
	}
	NetTransportPython(const std::string &service, const std::string &network,
		const std::string &daemon, const std::string &license_ticket)
		:TransportPython(new TibrvNetTransport)
	{
		CreateLicensedTransport(*static_cast<TibrvNetTransport *>(transport_sptr_.get()),
			service, network, daemon, license_ticket);
	}

	void GetBatchMode()
	{
	}

#ifdef MLB_RvUtilX_Support_TransportSetBatchMode
	void SetBatchMode(tibrvTransportBatchMode batch_mode)
	{
		TransportSetBatchMode(*GetPtrAsType(), batch_mode);
	}
#endif // #ifdef MLB_RvUtilX_Support_TransportSetBatchMode

	std::string GetDaemon() const
	{
		return(TransportGetDaemon(*GetPtrAsType()));
	}

	std::string GetDescription() const
	{
		return(TransportGetDescription(*GetPtrAsType()));
	}
	void SetDescription(const std::string &description)
	{
		return(TransportSetDescription(*GetPtrAsType(), description));
	}

	std::string GetNetwork() const
	{
		return(TransportGetNetwork(*GetPtrAsType()));
	}

	std::string GetService() const
	{
		return(TransportGetService(*GetPtrAsType()));
	}

protected:
	virtual const char *GetTypeName() const
	{
		return("network");
	}

private:
	TibrvNetTransport *GetPtrAsType()
	{
		return(static_cast<TibrvNetTransport *>(transport_sptr_.get()));
	}
	const TibrvNetTransport *GetPtrAsType() const
	{
		return(static_cast<const TibrvNetTransport *>(transport_sptr_.get()));
	}
};
//	////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
class EnvironmentPython {
public:
	static void Close()
	{
		//	Perform the static open...
		RvUtilX_THROW_TIBRV_STATUS_IF(Tibrv::close,
			());
	}

	static QueuePython DefaultQueue()
	{
		return(QueuePython::Default());
	}

	static IntraProcessTransportPython IntraProcessTransport()
	{
		return(IntraProcessTransportPython());
	}

	static void Open()
	{
		//	Perform the static open...
		RvUtilX_THROW_TIBRV_STATUS_IF(Tibrv::open,
			());
	}

	static std::string Version()
	{
		return(GetRvEnvVersion());
	}
};
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void MLB_RvUtilX_EnvironmentPython()
{
	boost::python::class_<EnvironmentPython>("Environment",
		"The Tib/Rendezvous environment.",
		boost::python::no_init)

		// Static properties...
		.add_static_property("DefaultQueue",
			EnvironmentPython::DefaultQueue)
		.add_static_property("IntraProcessTransport",
			EnvironmentPython::IntraProcessTransport)
		.add_static_property("Version",
			EnvironmentPython::Version)

		// Static member functions...
		.def("Close",
			EnvironmentPython::Close,
			"Decrements the Tib/Rendezvous library reference count for each\n"
			"Open() call that was made. The final invocation will free the\n"
			"Tib/Rendezvous internal machinery initialized by the first Open()\n"
			"call.")
		.staticmethod("Close")
		.def("Open",
			EnvironmentPython::Open,
			"Upon the first call to this static method, the Tib/Rendezvous\n"
			"internal machinery is initialized.\n"
			"Subsequent calls increment an internal reference count.")
		.staticmethod("Open")
		;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MLB_RvUtilX_LimitPolicyPython()
{
	boost::python::class_<LimitPolicyPython>("LimitPolicy",
		"A Tib/Rendezvous queue limit policy class. This specifies whether\n"
		"limits to Queue growth are to be applied and the policy to be used\n"
		"when a limit (if specified) is reached.")

		.def(boost::python::init<
			boost::python::optional<tibrvQueueLimitPolicy, unsigned int,
			unsigned int> >())

		.def("__str__",
			&LimitPolicyPython::ToString)
		;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MLB_RvUtilX_MessagePython()
{
	boost::python::class_<MessagePython>("Message",
		"A Tib/Rendezvous message.")
  		.def(boost::python::init<>())

		.def("__str__",
			MessagePython::ToString)

		// Instance properties...
		.add_property("FieldCount",
			MessagePython::GetFieldCount
#if BOOST_VERSION >= 103300
			,"The number of field contained within the message.")
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("FieldCount",
			MessagePython::GetFieldCountAsInt
#if BOOST_VERSION >= 103300
			,"The number of field contained within the message as a signed int.")
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("ReplySubject",
			MessagePython::GetReplySubject,
			MessagePython::SetReplySubject
#if BOOST_VERSION >= 103300
			,"The reply subject of the message."
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("SendSubject",
			MessagePython::GetSendSubject,
			MessagePython::SetSendSubject
#if BOOST_VERSION >= 103300
			,"The reply subject of the message."
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("Size",
			MessagePython::GetSize
#if BOOST_VERSION >= 103300
			,"The size of the message.")
#endif // #if BOOST_VERSION >= 103300
		)

		// Instance methods --- general purpose...
		.def("Expand",
			MessagePython::Expand,
			"Enlarges the allocated size of the message.")
		.def("Reset",
			MessagePython::Reset,
			"Resets the message state, making it equivalent to an empty message.")
		.def("ToByteArray",
			MessagePython::ToByteArray,
			"Returns the message as an array of bytes.")

		// Instance methods --- for adding fields...
		.def("AddBool",
			MessagePython::AddBool,
			"Adds a boolean field to the message.")
		.def("AddString",
			MessagePython::AddString,
			"Adds a string field to the message.")

		// Class properties...
		.add_static_property("MinimumCustomDataTypeID",
			MessagePython::GetMinimumCustomDataTypeID)
		.add_static_property("MaximumCustomDataTypeID",
			MessagePython::GetMaximumCustomDataTypeID)
		;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MLB_RvUtilX_QueuePython()
{
	boost::python::class_<QueuePython>("Queue",
		"A Tib/Rendezvous event queue.")
  		.def(boost::python::init<>())

		.def("__str__",
			QueuePython::ToString)

		// Instance properties...
		.add_property("Count",
			QueuePython::GetCount
#if BOOST_VERSION >= 103300
			,"The number of events pending within the queue.")
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("Handle",
			QueuePython::GetHandle
#if BOOST_VERSION >= 103300
			,"The underlying queue handle from the low-level C API."
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("IsDefaultQueue",
			QueuePython::IsDefaultQueue
#if BOOST_VERSION >= 103300
			,"True if this queue is the default queue."
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("LimitPolicy",
			QueuePython::GetLimitPolicy,
			QueuePython::SetLimitPolicy
#if BOOST_VERSION >= 103300
			,"The limit policy object for the queue."
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("Name",
			QueuePython::GetName,
			QueuePython::SetName
#if BOOST_VERSION >= 103300
			,"The descriptive name of the queue."
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("Priority",
			QueuePython::GetPriority,
			QueuePython::SetPriority
#if BOOST_VERSION >= 103300
			,"The priority of this queue vis-a-vis other queues when in a\n"
			"queue group."
#endif // #if BOOST_VERSION >= 103300
		)

		// Instance methods...
		.def("Destroy",
			QueuePython::Destroy,
			"Destroys the queue, rendering it unusable for further operation.")
		.def("Dispatch",
			QueuePython::Dispatch,
			"Dispatches the first event in the queue and returns. If there are\n"
			"no events in the queue, this method blocks until there is one.")
		.def("Poll",
			QueuePython::Poll,
			"Dispatches the first event in the queue and returns true. If there\n"
			"are no events in the queue, this method returns false.")
		.def("TimedDispatch",
			QueuePython::TimedDispatch,
			"Dispatches the first event in the queue and returns true. If there\n"
			"are no events in the queue this method will wait for up to as\n"
			"long as the time_out parameter specifies before returning. If no\n"
			"event has been dispatched, this method returns false.")

		// Class properties...
		.add_static_property("Default",
			QueuePython::Default)
		;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MLB_RvUtilX_TransportPython()
{
	boost::python::class_<TransportPython, boost::noncopyable>("Transport",
		"A Tib/Rendezvous transport on which messages may be sent and/or\n"
		"received.",
		boost::python::no_init)

		.def("__str__",
			TransportPython::ToString)

		// Instance properties...
		.add_property("Handle",
			TransportPython::GetHandle
#if BOOST_VERSION >= 103300
			,"The underlying transport handle from the low-level C API."
#endif // #if BOOST_VERSION >= 103300
		)

		// Instance methods...
		.def("CreateInbox",
			TransportPython::CreateInbox,
			"Creates a unique inbox name for the transport.")
		.def("Destroy",
			TransportPython::Destroy,
			"Destroys the transport, rendering it unusable for further operation.")
		.def("Send",
			TransportPython::Send,
			"Sends the specified message.")
		.def("SendReply",
			TransportPython::SendReply,
			"Sends the reply message to the subject name specified by the reply\n"
			"subject name of the request message.")
		.def("SendRequest",
			TransportPython::SendRequest,
			"Sends the request message message and waits for the specified\n"
			"time-out period for a reply.")
		;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MLB_RvUtilX_IntraProcessTransportPython()
{
	boost::python::class_<IntraProcessTransportPython,
		boost::python::bases<TransportPython> >("IntraProcessTransport",
		"The Tib/Rendezvous intra-process transport.")
  		.def(boost::python::init<>())

		.add_static_property("UniqueInstance",
			IntraProcessTransportPython::UniqueInstance)
		;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MLB_RvUtilX_NetTransportPython()
{
	boost::python::class_<NetTransportPython,
		boost::python::bases<TransportPython> >("NetTransport",
		"A Tib/Rendezvous network transport.")

  		.def(boost::python::init<>())
  		.def(boost::python::init<std::string, std::string, std::string>())
  		.def(boost::python::init<std::string, std::string, std::string,
			std::string>())

		.add_property("Daemon",
			NetTransportPython::GetDaemon
#if BOOST_VERSION >= 103300
			,"The daemon to which the transport is bound."
#endif // #if BOOST_VERSION >= 103300
		)
#ifdef MLB_RvUtilX_Support_TransportSetBatchMode
		// CODE NOTE: The BatchMode property in Tib/Rv 7.0 and above only.
		.add_property("BatchMode",
			NetTransportPython::GetBatchMode,
			NetTransportPython::SetBatchMode
# if BOOST_VERSION >= 103300
			,"The transport batch mode in use with the transport."
# endif // #if BOOST_VERSION >= 103300
		)
#endif // #ifdef MLB_RvUtilX_Support_TransportSetBatchMode
		.add_property("Description",
			NetTransportPython::GetDescription,
			NetTransportPython::SetDescription
#if BOOST_VERSION >= 103300
			,"The descriptive string associated with the transport."
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("Network",
			NetTransportPython::GetNetwork
#if BOOST_VERSION >= 103300
			,"The network to which the transport is bound."
#endif // #if BOOST_VERSION >= 103300
		)
		.add_property("Service",
			NetTransportPython::GetService
#if BOOST_VERSION >= 103300
			,"The service to which the transport is bound."
#endif // #if BOOST_VERSION >= 103300
		)
		;
}
// ////////////////////////////////////////////////////////////////////////////

#define MLB_RVUTILX_PYTHON_USE_HACK_ENUM_DOCSTRING	1

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(MLBRvUtilXPython)
{
#ifdef MLB_RvUtilX_Support_TransportSetBatchMode
# ifndef MLB_RVUTILX_PYTHON_USE_HACK_ENUM_DOCSTRING
	boost::python::enum_<tibrvTransportBatchMode>("TransportBatchMode")
		.value("Default",    TIBRV_TRANSPORT_DEFAULT_BATCH)
		.value("TimerBatch", TIBRV_TRANSPORT_TIMER_BATCH)
		;
# else
	{
		boost::python::enum_<tibrvTransportBatchMode> tbm("TransportBatchMode");
		tbm.value("Default",    TIBRV_TRANSPORT_DEFAULT_BATCH);
		tbm.value("TimerBatch", TIBRV_TRANSPORT_TIMER_BATCH);
		PyTypeObject *obj_ptr = reinterpret_cast<PyTypeObject *>(tbm.ptr());
		PyDict_SetItemString(obj_ptr->tp_dict, "__doc__",
			PyString_FromString(
			"An enumeration of the permissible values to which the BatchMode\n"
			"member of a NetTransport may be set."));
	}
# endif // #ifndef MLB_RVUTILX_PYTHON_USE_HACK_ENUM_DOCSTRING
#endif // #ifdef MLB_RvUtilX_Support_TransportSetBatchMode

	boost::python::enum_<tibrvQueueLimitPolicy>("LimitPolicyStrategy")
		.value("DiscardNew",   TIBRVQUEUE_DISCARD_NEW)
		.value("DiscardNone",  TIBRVQUEUE_DISCARD_NONE)
		.value("DiscardFirst", TIBRVQUEUE_DISCARD_FIRST)
		.value("DiscardLast",  TIBRVQUEUE_DISCARD_LAST)
		;

	MLB_RvUtilX_EnvironmentPython();
	MLB_RvUtilX_MessagePython();
	MLB_RvUtilX_LimitPolicyPython();
	MLB_RvUtilX_QueuePython();
	MLB_RvUtilX_TransportPython();
	MLB_RvUtilX_IntraProcessTransportPython();
	MLB_RvUtilX_NetTransportPython();
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

