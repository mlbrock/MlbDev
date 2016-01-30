// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the RvAdvMsg class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_RvAdvMsg_hpp__HH

#define HH__MLB__RvUtilX_RvAdvMsg_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RvParseCmdLineArg.hpp>
#include <Utility/LogLevel.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
enum RvAdvFlagType {
	RvAdvFlagType_Rv   = 0x0000,
	RvAdvFlagType_Cm   = 0x0001,
	RvAdvFlagType_Ft   = 0x0002
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
enum RvAdvFlags {
	//	/////////////////////////////////////////////////////////////////////
	//	No Rendezvous flags...
	Rv_None                     = 0x00000000,
	//	/////////////////////////////////////////////////////////////////////
	//	The basic Rendezvous advisory messages...
	Rv_SlowConsumer             = 0x00000001,
	Rv_FastProducer             = 0x00000002,
	Rv_NoMemory                 = 0x00000004,
	Rv_LicenseExpire_Warn       = 0x00000008,
	Rv_LicenseExpire_Error      = 0x00000010,
	Rv_Dataloss_In_Ptp          = 0x00000020,
	Rv_Dataloss_In_BCast        = 0x00000040,
	Rv_Dataloss_Out_Ptp         = 0x00000080,
	Rv_Dataloss_Out_BCast       = 0x00000100,
	Rv_Rvd_Disconnected         = 0x00000200,
	Rv_Rvd_Connected            = 0x00000400,
	Rv_UnreachableTransport     = 0x00000800,
	Rv_QueueLimitExceeded       = 0x00001000,
	Rv_DispatcherThreadExited   = 0x00002000,
	Rv_Vc_Connected             = 0x00004000,
	Rv_Vc_Disconnected_Info     = 0x00008000,
	Rv_Vc_Disconnected_Error    = 0x00010000,
	//	---------------------------------------------------------------------
	//	Some undocumented Rendezvous advisories...
	Rv_HostStart                = 0x00100000,
	Rv_HostStop                 = 0x00200000,
	Rv_HostStatus               = 0x00400000,
	Rv_ListenStart              = 0x00800000,
	Rv_ListenStop               = 0x01000000,
	Rv_RetransmissionIn         = 0x02000000,
	Rv_RetransmissionOut        = 0x04000000,
	Rv_DaemonRestarted          = 0x08000000,
	//	---------------------------------------------------------------------
	//	Useful masks of the basic Rendezvous advisory messages...
	//		Slow-consumer and fast-producer advisories...
	Rv_ActorSpeed               = Rv_SlowConsumer | Rv_FastProducer,
	//		License expiration...
	Rv_LicenseExpire            = Rv_LicenseExpire_Warn | Rv_LicenseExpire_Error,
	//		All in-bound dataloss...
	Rv_Dataloss_In              = Rv_Dataloss_In_Ptp | Rv_Dataloss_In_BCast,
	//		All out-bound dataloss...
	Rv_Dataloss_Out             = Rv_Dataloss_Out_Ptp | Rv_Dataloss_Out_BCast,
	//		All point-to-point protocol dataloss...
	Rv_Dataloss_Ptp             = Rv_Dataloss_In_Ptp | Rv_Dataloss_Out_Ptp,
	//		All broadcast protocol dataloss...
	Rv_Dataloss_BCast           = Rv_Dataloss_In_BCast | Rv_Dataloss_Out_BCast,
	//		All dataloss...
	Rv_Dataloss                 = Rv_Dataloss_In | Rv_Dataloss_Out,
	//		All Rvd connection/disconnection advisories...
	Rv_Rvd_Connection           = Rv_Rvd_Disconnected | Rv_Rvd_Connected,
	//		All virtual circuit disconnection advisories...
	Rv_Vc_Disconnection         = Rv_Vc_Disconnected_Info | Rv_Vc_Disconnected_Error,
	//		All virtual circuit connection/disconnection advisories...
	Rv_Vc_Connection            = Rv_Vc_Connected | Rv_Vc_Disconnection,
	//		Mask of the default basic Rendezvous advisories...
	Rv_DefaultBasic             = Rv_ActorSpeed | Rv_NoMemory | Rv_LicenseExpire | Rv_Dataloss | Rv_Rvd_Connection | Rv_QueueLimitExceeded | Rv_Vc_Disconnected_Error,
	//		Mask of all the basic Rendezvous advisories...
	Rv_MaskBasic                = Rv_SlowConsumer | Rv_FastProducer | Rv_NoMemory | Rv_LicenseExpire_Warn | Rv_LicenseExpire_Error | Rv_Dataloss_In_Ptp | Rv_Dataloss_In_BCast | Rv_Dataloss_Out_Ptp | Rv_Dataloss_Out_BCast | Rv_Rvd_Disconnected | Rv_Rvd_Connected | Rv_UnreachableTransport | Rv_QueueLimitExceeded | Rv_DispatcherThreadExited | Rv_Vc_Connected | Rv_Vc_Disconnected_Info | Rv_Vc_Disconnected_Error,
	//	---------------------------------------------------------------------
	//	Useful masks of the undocumented rendezvous advisories...
	//		Host-related advisories...
	Rv_HostStartStop            = Rv_HostStart | Rv_HostStop,
	Rv_HostInfo                 = Rv_HostStartStop | Rv_HostStatus,
	//		Subject listen start/stop advisories...
	Rv_ListenStartStop          = Rv_ListenStart | Rv_ListenStop,
	//		Retransmission-related advisiories...
	Rv_Retransmission           = Rv_RetransmissionIn | Rv_RetransmissionOut,
	//		A mask of the default undocumented Rendezvous advisories...
	Rv_DefaultOther             = Rv_DaemonRestarted,
	//		A mask of all of the undocumented Rendezvous advisories...
	Rv_MaskOther                = Rv_HostStart | Rv_HostStop | Rv_HostStatus | Rv_ListenStart | Rv_ListenStop | Rv_RetransmissionIn | Rv_RetransmissionOut | Rv_DaemonRestarted,
	//	---------------------------------------------------------------------
	//	Mask of the Rendezvous advisories...
	Rv_Default                  = Rv_DefaultBasic | Rv_DefaultOther,
	//		Mask of all Rendezvous advisories...
	Rv_Mask                     = Rv_MaskBasic | Rv_MaskOther
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
enum CmAdvFlags {
	//	/////////////////////////////////////////////////////////////////////
	//	No certified messaging flags...
	Cm_None                     = 0x00000000,
	//	/////////////////////////////////////////////////////////////////////
	//	The basic Rendezvous certified message delivery advisory messages...
	Cm_DeliveryConfirm          = 0x00000001,
	Cm_DeliveryComplete         = 0x00000002,
	Cm_DeliveryNoResponse       = 0x00000004,
	Cm_DeliveryFailed           = 0x00000008,
	Cm_DeliveryUnavailable      = 0x00000010,
	Cm_RegistrationRequest      = 0x00000020,
	Cm_RegistrationCertified    = 0x00000040,
	Cm_RegistrationNotCertified = 0x00000080,
	Cm_RegistrationNoResponse   = 0x00000100,
	Cm_RegistrationClosed       = 0x00000200,
	Cm_RegistrationDiscovery    = 0x00000400,
	Cm_RegistrationMoved        = 0x00000800,
	Cm_RegistrationCollision    = 0x00001000,
	Cm_RelayConnected           = 0x00002000,
	Cm_RelayDisconnected        = 0x00004000,
	Cm_QueueSchedulerActive     = 0x00008000,
	Cm_QueueSchedulerInactive   = 0x00010000,
	//	---------------------------------------------------------------------
	//	Useful masks of the basic Rendezvous certified message delivery
	//	advisory messages...
	//		All delivery-related advisories...
	Cm_Delivery                 = Cm_DeliveryConfirm | Cm_DeliveryComplete | Cm_DeliveryNoResponse | Cm_DeliveryFailed | Cm_DeliveryUnavailable,
	//		All registration-related advisories...
	Cm_Registration             = Cm_RegistrationRequest | Cm_RegistrationCertified | Cm_RegistrationNotCertified | Cm_RegistrationNoResponse | Cm_RegistrationClosed | Cm_RegistrationDiscovery | Cm_RegistrationMoved | Cm_RegistrationCollision,
	//		All relay connection/disconnection advisories...
	Cm_Relay                    = Cm_RelayConnected | Cm_RelayDisconnected,
	//		All queue scheduler advisories...
	Cm_QueueScheduler           = Cm_QueueSchedulerActive | Cm_QueueSchedulerInactive,
	//		Mask of the default certified message advisories...
	Cm_Default                  = Cm_None,
	//		Mask of all the certified message advisories...
	Cm_Mask                     = Cm_DeliveryConfirm | Cm_DeliveryComplete | Cm_DeliveryNoResponse | Cm_DeliveryFailed | Cm_DeliveryUnavailable | Cm_RegistrationRequest | Cm_RegistrationCertified | Cm_RegistrationNotCertified | Cm_RegistrationNoResponse | Cm_RegistrationClosed | Cm_RegistrationDiscovery | Cm_RegistrationMoved | Cm_RegistrationCollision | Cm_RelayConnected | Cm_RelayDisconnected | Cm_QueueSchedulerActive | Cm_QueueSchedulerInactive
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
enum FtAdvFlags {
	//	/////////////////////////////////////////////////////////////////////
	//	No fault tolerance flags...
	Ft_None                     = 0x00000000,
	//	/////////////////////////////////////////////////////////////////////
	//	The basic Rendezvous fault tolerance advisory messages...
	Ft_ParamMismatch_Warn       = 0x00000001,
	Ft_ParamMismatch_Error      = 0x00000002,
	Ft_DisablingMember          = 0x00000004,
	Ft_TooManyActive            = 0x00000008,
	Ft_TooFewActive             = 0x00000010,
	//	---------------------------------------------------------------------
	//	Useful masks of the basic Rendezvous fault tolerance advisory
	//	messages...
	//		All parameter mismatch advisories...
	Ft_ParamMismatch            = Ft_ParamMismatch_Warn | Ft_ParamMismatch_Error,
	//		All advisories describing an invalid active member count...
	Ft_InvalidActiveCount       = Ft_TooManyActive | Ft_TooFewActive,
	//		Mask of the default fault tolerance advisories...
	Ft_Default                  = Ft_ParamMismatch_Error | Ft_InvalidActiveCount,
	//		Mask of all fault tolerance advisories...
	Ft_Mask                     = Ft_ParamMismatch | Ft_DisablingMember | Ft_TooManyActive | Ft_TooFewActive
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvAdvFlagsAll {
	RvAdvFlagsAll(RvAdvFlags rv_flags = Rv_None, CmAdvFlags cm_flags = Cm_None,
		FtAdvFlags ft_flags = Ft_None)
		:rv_flags_(rv_flags)
		,cm_flags_(cm_flags)
		,ft_flags_(ft_flags)
	{
	}

	RvAdvFlags rv_flags_;
	CmAdvFlags cm_flags_;
	FtAdvFlags ft_flags_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvAdvMsg;
typedef RvAdvMsg    &(*GetAdvDataFunc)(const TibrvMsg &msg_recv,
	RvAdvMsg &out_contents);
typedef std::string &(*GetAdvTextFunc)(const TibrvMsg &msg_recv,
	std::string &out_string);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvAdvEventInfo {
	RvAdvEventInfo(RvAdvFlagType advisory_type, unsigned int advisory_flag,
		const std::string &subject_pattern)
		:advisory_type_(advisory_type)
		,advisory_flag_(advisory_flag)
		,subject_pattern_(subject_pattern)
		,subject_pattern_flat_(subject_pattern)
		,first_occurrence_time_(0, 0)
		,last_occurrence_time_(0, 0)
		,occurrence_count_(0)
	{
	}
	//	Used only for std::set::find()...
	RvAdvEventInfo(RvAdvFlagType advisory_type, unsigned int advisory_flag)
		:advisory_type_(advisory_type)
		,advisory_flag_(advisory_flag)
		,subject_pattern_()
		,subject_pattern_flat_()
		,first_occurrence_time_(0, 0)
		,last_occurrence_time_(0, 0)
		,occurrence_count_(0)
	{
	}

	bool operator < (const RvAdvEventInfo &other) const {
		return((advisory_type_ < other.advisory_type_) ? true :
			(advisory_flag_ < other.advisory_flag_));
	}

	void UpdateEventInfo(const MLB::Utility::TimeSpec &occurrence_time =
		MLB::Utility::TimeSpec()) {
		if (first_occurrence_time_.IsZero())
			first_occurrence_time_ = occurrence_time;
		last_occurrence_time_ = occurrence_time;
		MLB::Utility::IncrementToMax(occurrence_count_);
	}

	RvAdvFlagType          advisory_type_;
	unsigned int           advisory_flag_;
	std::string            subject_pattern_;
	SubjectNameFlat        subject_pattern_flat_;
	MLB::Utility::TimeSpec first_occurrence_time_;
	MLB::Utility::TimeSpec last_occurrence_time_;
	unsigned long long     occurrence_count_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<RvAdvEventInfo>          RvAdvEventInfoSet;
typedef RvAdvEventInfoSet::iterator       RvAdvEventInfoSetIter;
typedef RvAdvEventInfoSet::const_iterator RvAdvEventInfoSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvAdvFlagsMap {
	RvAdvFlagsMap(RvAdvFlagType advisory_type, unsigned int advisory_flag,
		GetAdvDataFunc get_contents_func, GetAdvTextFunc get_adv_text_func,
		const char *advisory_subject)
		:advisory_type_(advisory_type)
		,advisory_flag_(advisory_flag)
		,get_contents_func_(get_contents_func)
		,get_adv_text_func_(get_adv_text_func)
		,advisory_subject_(advisory_subject)
		,advisory_subject_flat_(advisory_subject)
	{
	}

	RvAdvFlagType    advisory_type_;
	unsigned int     advisory_flag_;
	GetAdvDataFunc   get_contents_func_;
	GetAdvTextFunc   get_adv_text_func_;
	const char      *advisory_subject_;
	SubjectNameFlat  advisory_subject_flat_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::pair<unsigned int, const RvAdvFlagsMap *> RvAdvFlagsMapList;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvAdvMsg {
	RvAdvMsg();
	explicit RvAdvMsg(const TibrvMsg &msg);
	virtual ~RvAdvMsg();
	RvAdvMsg(const RvAdvMsg &other);
	RvAdvMsg & operator = (const RvAdvMsg &other);
	RvAdvMsg & swap(RvAdvMsg &other);

	void              Clear();

	RvAdvMsg         &GetBasicAdv(const TibrvMsg &msg);
	const char       *CheckStringField(const char *field_ptr,
		const char *field_name) const;
	void              CheckAdvDesc() const;
	void              RethrowException(const std::exception &except,
		const char *advisory_name) const;

	std::string         MsgToString(const TibrvMsg &msg,
		bool throw_if_no_match = true);
	std::string        &MsgToString(const TibrvMsg &msg,
		std::string &out_string, bool throw_if_no_match = true);
	std::string         MsgToString(const RvAdvFlagsMap *map_ptr,
		const TibrvMsg &msg);
	std::string        &MsgToString(const RvAdvFlagsMap *map_ptr,
		const TibrvMsg &msg, std::string &out_string);

	static RvAdvMsg     ConvertMsgToContents(const TibrvMsg &msg,
		bool throw_if_no_match = true);
	static RvAdvMsg    &ConvertMsgToContents(const TibrvMsg &msg,
		RvAdvMsg &contents, bool throw_if_no_match = true);
	static RvAdvMsg     ConvertMsgToContents(const RvAdvFlagsMap *map_ptr,
		const TibrvMsg &msg);
	static RvAdvMsg    &ConvertMsgToContents(const RvAdvFlagsMap *map_ptr,
		const TibrvMsg &msg, RvAdvMsg &contents);


	static std::string  ConvertMsgToString(const TibrvMsg &msg,
		bool throw_if_no_match = true);
	static std::string &ConvertMsgToString(const TibrvMsg &msg,
		std::string &out_string, bool throw_if_no_match = true);
	static std::string  ConvertMsgToString(const RvAdvFlagsMap *map_ptr,
		const TibrvMsg &msg);
	static std::string &ConvertMsgToString(const RvAdvFlagsMap *map_ptr,
		const TibrvMsg &msg, std::string &out_string);

	static const RvAdvFlagsMap *SubjectToMapPtr(const char *subject,
		bool throw_if_no_match = true);
	static const RvAdvFlagsMap *MsgToMapPtr(const TibrvMsg &msg,
		bool throw_if_no_match = true);
	
	static bool                 IsValidMapPtr(const RvAdvFlagsMap *map_ptr);
	static bool                 IsValidMapPtr(const RvAdvFlagsMap *map_ptr,
		unsigned int map_count, const RvAdvFlagsMap *map_list);
	static const RvAdvFlagsMap *CheckIsValidMapPtr(const RvAdvFlagsMap *map_ptr);

	bool              initialized_from_message_;
	const char       *ADV_CLASS_;
	const char       *ADV_SOURCE_;
	const char       *ADV_NAME_;
	const char       *ADV_DESC_;
	tibrv_u32         waiting_;				//	SLOWCONSUMER, FASTPRODUCER
	tibrv_u32         dropped_;				//	SLOWCONSUMER
	tibrv_u64         bytes_dropped_;		//	SLOWCONSUMER
	TibrvMsgDateTime  expiretime_;			//	LICENSE.EXPIRE
	bool              license_expired_;		//	LICENSE.EXPIRE --- true if ERROR
	bool              dataloss_outbound_;	//	DATALOSS, false = in-bound
	bool              dataloss_ptp_;			//	DATALOSS, false = broadcast
	tibrv_ipaddr32    dataloss_host_;		//	DATALOSS
	tibrv_i32         dataloss_lost_;		//	DATALOSS
	const char       *tport_;
	const char       *descr_;
	tibrv_i32         dtexit_status_;		//	DISPATCHER.THREAD_EXITED
	const char       *dtexit_description_;	//	DISPATCHER.THREAD_EXITED
	bool              vc_disc_error_;		//	VC.DISCONNECTED -- true if error
	const char       *vc_disc_description_;//	VC.DISCONNECTED
	//	Following members are in the HOST.STATUS message...
	tibrv_ipaddr32    hostaddr_;				//	Host IP address
	tibrv_u32         sn_;						//	Serial number - mot present in 7.x
	bool              sn_flag_;				//	True if 'sn' field in message
	tibrv_u8          os_;						//	??? Rvd=2, Rvrd=130
	const char       *ver_;						//	Version 
	tibrv_u32         up_;						//	Uptime --- not present in 7.x
	bool              up_flag_;				//	True if 'up' field in message
	tibrv_ipaddr32    httpaddr_;				//	HTTP connection IP address
	tibrv_ipport16    httpport_;				//	HTTP connection IP port
	tibrv_u64         ms_;						//	Messages Sent
	tibrv_u64         bs_;						//	Bytes Sent
	tibrv_u64         mr_;						//	Messages Received
	tibrv_u64         br_;						//	Bytes Received
	tibrv_u64         ps_;						//	Packets Sent
	tibrv_u64         pr_;						//	Packets Received
	tibrv_u64         rx_;						//	Retransmission Requests
	tibrv_u64         pm_;						//	Packets Missed
	//	Additional HOST.STATUS fields from 7.5.x rvd instances...
	tibrv_ipaddr32    hsaddr_;					//	HTTPS connection IP address
	tibrv_ipport16    hsport_;					//	HTTPS connection IP port
	TibrvMsgDateTime  hstatus_time_;			//	GMT/UTC time of this status
	tibrv_u64         idl_;						//	In-bound data loss in packets
	tibrv_u64         odl_;						//	Out-bound data loss in packets
	tibrv_ipport16    ipport_;					//	Rvd connection port
	const char       *service_;				//	Rvd service
	const char       *network_;				//	Rvd network

	//	The following members are in the LISTEN.{START|STOP} messages...
	bool              listen_stop_;			//	If false, LISTEN.START message
	const char       *listen_sub_;
	const char       *listen_id_;
	tibrv_i32         listen_refcnt_;

	tibrv_ipaddr32    restart_host_;			//	DAEMON.RESTARTED

	//	The following members describe certified messaging advisories...
	const char       *cm_subject_;
	const char       *cm_listener_;
	tibrv_u64         cm_seqno_;
	tibrv_u64         cm_seqno_begin_;
	tibrv_u64         cm_seqno_end_;
	const char       *cm_sender_;
	tibrv_u64         cm_seqno_last_sent_;
	tibrv_u64         cm_seqno_last_confirmed_;
	const char       *cm_name_;
	//	The following members describe fault tolerance advisories...
	//		PARAM_MISMATCH advisories...
	bool              ft_active_goal_differs_;
	bool              ft_activation_interval_differs_;
	bool              ft_heartbeat_interval_differs_;
	bool              ft_other_member_prepares_before_;
	bool              ft_this_member_prepares_before_;
	bool              ft_other_param_mismatch_;

	MLB::Utility::LogLevel adv_class_log_;

	//		Basic advisories...
	RvAdvMsg &SlowConsumer(const TibrvMsg &msg);
	RvAdvMsg &FastProducer(const TibrvMsg &msg);
	RvAdvMsg &NoMemory(const TibrvMsg &msg);
	RvAdvMsg &LicenseExpire(const TibrvMsg &msg);
	RvAdvMsg &Dataloss(const TibrvMsg &msg);
	RvAdvMsg &RvdDisconnected(const TibrvMsg &msg);
	RvAdvMsg &RvdConnected(const TibrvMsg &msg);
	RvAdvMsg &UnreachableTransport(const TibrvMsg &msg);
	RvAdvMsg &QueueLimitExceeded(const TibrvMsg &msg);
	/**
		\note The dispatcher thread name can be extracted from the message as
		follows:
		\code std::string thread_name;
		\code SubjectNameFlat(ADV_NAME_).ToStringFromElements(thread_name, 2);
	*/
	RvAdvMsg &DispatcherThreadExited(const TibrvMsg &msg);
	RvAdvMsg &VcConnected(const TibrvMsg &msg);
	RvAdvMsg &VcDisconnected(const TibrvMsg &msg);
	//		Undocumented advisories...
	RvAdvMsg &HostStart(const TibrvMsg &msg);
	RvAdvMsg &HostStop(const TibrvMsg &msg);
	RvAdvMsg &HostStatus(const TibrvMsg &msg);
	RvAdvMsg &ListenStartStop(const TibrvMsg &msg);
	RvAdvMsg &Retransmission(const TibrvMsg &msg);
	RvAdvMsg &DaemonRestarted(const TibrvMsg &msg);
	//		Certified message advisories...
	RvAdvMsg &CmDeliveryConfirm(const TibrvMsg &msg);
	RvAdvMsg &CmDeliveryComplete(const TibrvMsg &msg);
	RvAdvMsg &CmDeliveryNoResponse(const TibrvMsg &msg);
	/**
		\note	The "_RV.ERROR.RVCM.DELIVERY.FAILED.>" and the
		"_RV.ERROR.RVCM.DELIVERY.CONFIRM.>" advisories can contain more
		than one listener, but this method retrieves only the first. To get the
		list of all listeners, use CmDeliveryFailed_GetListenerList().
	*/
	RvAdvMsg &CmDeliveryFailed(const TibrvMsg &msg);
	RvAdvMsg &CmDeliveryUnavailable(const TibrvMsg &msg);
	RvAdvMsg &CmRegistrationRequest(const TibrvMsg &msg);
	RvAdvMsg &CmRegistrationCertified(const TibrvMsg &msg);
	RvAdvMsg &CmRegistrationNotCertified(const TibrvMsg &msg);
	RvAdvMsg &CmRegistrationNoResponse(const TibrvMsg &msg);
	RvAdvMsg &CmRegistrationClosed(const TibrvMsg &msg);
	RvAdvMsg &CmRegistrationDiscovery(const TibrvMsg &msg);
	RvAdvMsg &CmRegistrationMoved(const TibrvMsg &msg);
	RvAdvMsg &CmRegistrationCollision(const TibrvMsg &msg);
	RvAdvMsg &CmRelayConnected(const TibrvMsg &msg);
	RvAdvMsg &CmRelayDisconnected(const TibrvMsg &msg);
	RvAdvMsg &CmQueueSchedulerActive(const TibrvMsg &msg);
	RvAdvMsg &CmQueueSchedulerInactive(const TibrvMsg &msg);
	//		Fault tolerance advisories...
	RvAdvMsg &FtParamMismatch(const TibrvMsg &msg);
	RvAdvMsg &FtDisablingMember(const TibrvMsg &msg);
	RvAdvMsg &FtTooManyActive(const TibrvMsg &msg);
	RvAdvMsg &FtTooFewActive(const TibrvMsg &msg);

	void              CmDeliveryFailed_GetListenerList(const TibrvMsg &msg,
		std::vector<std::string> &listener_list) const;
	bool              CmGetDataField(const TibrvMsg &msg,
		TibrvMsg &msg_data_field, bool field_is_required = true);

	//	----------------------------------------------------------------------
	static std::string SlowConsumerToString(const TibrvMsg &msg);
	static std::string FastProducerToString(const TibrvMsg &msg);
	static std::string NoMemoryToString(const TibrvMsg &msg);
	static std::string LicenseExpireToString(const TibrvMsg &msg);
	static std::string DatalossToString(const TibrvMsg &msg);
	static std::string RvdDisconnectedToString(const TibrvMsg &msg);
	static std::string RvdConnectedToString(const TibrvMsg &msg);
	static std::string UnreachableTransportToString(const TibrvMsg &msg);
	static std::string QueueLimitExceededToString(const TibrvMsg &msg);
	static std::string DispatcherThreadExitedToString(const TibrvMsg &msg);
	static std::string VcConnectedToString(const TibrvMsg &msg);
	static std::string VcDisconnectedToString(const TibrvMsg &msg);
	static std::string HostStartToString(const TibrvMsg &msg);
	static std::string HostStopToString(const TibrvMsg &msg);
	static std::string HostStatusToString(const TibrvMsg &msg);
	static std::string ListenStartStopToString(const TibrvMsg &msg);
	static std::string RetransmissionToString(const TibrvMsg &msg);
	static std::string DaemonRestartedToString(const TibrvMsg &msg);
	static std::string CmDeliveryConfirmToString(const TibrvMsg &msg);
	static std::string CmDeliveryCompleteToString(const TibrvMsg &msg);
	static std::string CmDeliveryNoResponseToString(const TibrvMsg &msg);
	static std::string CmDeliveryFailedToString(const TibrvMsg &msg);
	static std::string CmDeliveryUnavailableToString(const TibrvMsg &msg);
	static std::string CmRegistrationRequestToString(const TibrvMsg &msg);
	static std::string CmRegistrationCertifiedToString(const TibrvMsg &msg);
	static std::string CmRegistrationNotCertifiedToString(const TibrvMsg &msg);
	static std::string CmRegistrationNoResponseToString(const TibrvMsg &msg);
	static std::string CmRegistrationClosedToString(const TibrvMsg &msg);
	static std::string CmRegistrationDiscoveryToString(const TibrvMsg &msg);
	static std::string CmRegistrationMovedToString(const TibrvMsg &msg);
	static std::string CmRegistrationCollisionToString(const TibrvMsg &msg);
	static std::string CmRelayConnectedToString(const TibrvMsg &msg);
	static std::string CmRelayDisconnectedToString(const TibrvMsg &msg);
	static std::string CmQueueSchedulerActiveToString(const TibrvMsg &msg);
	static std::string CmQueueSchedulerInactiveToString(const TibrvMsg &msg);
	static std::string FtParamMismatchToString(const TibrvMsg &msg);
	static std::string FtDisablingMemberToString(const TibrvMsg &msg);
	static std::string FtTooManyActiveToString(const TibrvMsg &msg);
	static std::string FtTooFewActiveToString(const TibrvMsg &msg);
	static std::string DefaultAdvToString(const TibrvMsg &msg);
	//	----------------------------------------------------------------------
	static RvAdvMsg &SlowConsumer2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &FastProducer2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &NoMemory2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &LicenseExpire2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &Dataloss2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &RvdDisconnected2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &RvdConnected2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &UnreachableTransport2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &QueueLimitExceeded2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &DispatcherThreadExited2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &VcConnected2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &VcDisconnected2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	//		Undocumented advisories...
	static RvAdvMsg &HostStart2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &HostStop2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &HostStatus2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &ListenStartStop2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &Retransmission2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &DaemonRestarted2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	//		Certified message advisories...
	static RvAdvMsg &CmDeliveryConfirm2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmDeliveryComplete2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmDeliveryNoResponse2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmDeliveryFailed2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmDeliveryUnavailable2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRegistrationRequest2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRegistrationCertified2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRegistrationNotCertified2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRegistrationNoResponse2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRegistrationClosed2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRegistrationDiscovery2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRegistrationMoved2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRegistrationCollision2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRelayConnected2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmRelayDisconnected2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmQueueSchedulerActive2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &CmQueueSchedulerInactive2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	//		Fault tolerance advisories...
	static RvAdvMsg &FtParamMismatch2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &FtDisablingMember2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &FtTooManyActive2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	static RvAdvMsg &FtTooFewActive2(const TibrvMsg &msg,
		RvAdvMsg &contents);
	//	----------------------------------------------------------------------
	static std::string &SlowConsumerToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &FastProducerToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &NoMemoryToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &LicenseExpireToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &DatalossToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &RvdDisconnectedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &RvdConnectedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &UnreachableTransportToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &QueueLimitExceededToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &DispatcherThreadExitedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &VcConnectedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &VcDisconnectedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &HostStartToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &HostStopToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &HostStatusToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &ListenStartStopToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &RetransmissionToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &DaemonRestartedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmDeliveryConfirmToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmDeliveryCompleteToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmDeliveryNoResponseToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmDeliveryFailedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmDeliveryUnavailableToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRegistrationRequestToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRegistrationCertifiedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRegistrationNotCertifiedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRegistrationNoResponseToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRegistrationClosedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRegistrationDiscoveryToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRegistrationMovedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRegistrationCollisionToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRelayConnectedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmRelayDisconnectedToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmQueueSchedulerActiveToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &CmQueueSchedulerInactiveToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &FtParamMismatchToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &FtDisablingMemberToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &FtTooManyActiveToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &FtTooFewActiveToString2(const TibrvMsg &msg,
		std::string &out_string);
	static std::string &DefaultAdvToString2(const TibrvMsg &msg,
		std::string &out_string);
	//	----------------------------------------------------------------------

	static RvAdvFlagsMapList    GetFlagsMapRv();
	static RvAdvFlagsMapList    GetFlagsMapCm();
	static RvAdvFlagsMapList    GetFlagsMapFt();

	static const RvAdvFlagsMap *GetFlagsMapPtr(RvAdvFlagType advisory_type,
		unsigned int advisory_flag);
	static const RvAdvFlagsMap *GetFlagsMapPtr(unsigned int map_count,
		const RvAdvFlagsMap *map_list, unsigned int advisory_flag);

	static void CreateEventInfoSet(RvAdvEventInfoSet &event_set);

	static RvAdvEventInfo *LocateEventInfoInSet(const TibrvMsg &msg,
		RvAdvEventInfoSet &event_set, bool throw_if_no_match = true);
	static RvAdvEventInfo *LocateEventInfoInSet(const char *subject,
		RvAdvEventInfoSet &event_set, bool throw_if_no_match = true);
	static RvAdvEventInfo *LocateEventInfoInSet(const RvAdvFlagsMap *map_ptr,
		RvAdvEventInfoSet &event_set, bool throw_if_no_match = true);
	static RvAdvEventInfo *UpdateEventInfoSet(const TibrvMsg &msg,
		RvAdvEventInfoSet &event_set,
		const MLB::Utility::TimeSpec &event_time = MLB::Utility::TimeSpec(),
		bool throw_if_no_match = true);
	static RvAdvEventInfo *UpdateEventInfoSet(const char *subject,
		RvAdvEventInfoSet &event_set,
		const MLB::Utility::TimeSpec &event_time = MLB::Utility::TimeSpec(),
		bool throw_if_no_match = true);
	static RvAdvEventInfo *UpdateEventInfoSet(const RvAdvFlagsMap *map_ptr,
		RvAdvEventInfoSet &event_set,
		const MLB::Utility::TimeSpec &event_time = MLB::Utility::TimeSpec(),
		bool throw_if_no_match = true);

private:
	static const RvAdvFlagsMap *RvAdvMsg::SubjectToMapPtrInternal(
		const SubjectNameFlat subject, unsigned int map_count,
		const RvAdvFlagsMap *map_list);

	static void AppendEventInfoSet(unsigned int map_count,
		const RvAdvFlagsMap *map_list, RvAdvEventInfoSet &event_set);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_RVUTILX RvAdvMsgParseCmdLineArg : public RvParseCmdLineArg {
	static bool ParseCmdLineAdvMsgFlags(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		RvAdvFlags &rv_flags, CmAdvFlags &cm_flags, FtAdvFlags &ft_flags);
	static bool ParseCmdLineAdvMsgFlags(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		RvAdvFlagsAll &out_datum);
	static bool ParseCmdLineAdvMsgFlagsSpecDefault(unsigned int &current_index,
		int argc, char **argv, RvAdvFlags &rv_flags, CmAdvFlags &cm_flags,
		FtAdvFlags &ft_flags);
	static bool ParseCmdLineAdvMsgFlagsSpecDefault(unsigned int &current_index,
		int argc, char **argv, RvAdvFlagsAll &out_datum);
	template <typename  ContainerType>
		static bool ParseCmdLineAdvMsgFlagsSpec(const ContainerType &spec_cont,
		unsigned int &current_index, int argc, char **argv,
		RvAdvFlags &rv_flags, CmAdvFlags &cm_flags, FtAdvFlags &ft_flags) {
		ContainerType::const_iterator iter_b(spec_cont.begin());
		ContainerType::const_iterator iter_e(spec_cont.end());
		while (iter_b != iter_e) {
			if (ParseCmdLineAdvMsgFlags(*iter_b, current_index, argc, argv,
				rv_flags, cm_flags, ft_flags))
				return(true);
			++iter_b;
		}
		return(false);
	}
	template <typename  ContainerType>
		static bool ParseCmdLineAdvMsgFlagsSpec(const ContainerType &spec_cont,
		unsigned int &current_index, int argc, char **argv,
		RvAdvFlagsAll &out_datum) {
		return(ParseCmdLineAdvMsgFlagsSpec(spec_cont, current_index, argc,
			**argv, out_datum.rv_flags_, out_datum.cm_flags_,
			out_datum.ft_flags_));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void          StringToRvAdvMsgFlags(const std::string &in_string,
	RvAdvFlags &rv_flags, CmAdvFlags &cm_flags, FtAdvFlags &ft_flags);
RvAdvFlagsAll StringToRvAdvMsgFlags(const std::string &in_string);

std::string &RvAdvMsgFlagsToStringAppend(RvAdvFlagType advisory_type,
	unsigned int advisory_flag, std::string &out_string);
std::string  RvAdvMsgFlagsToStringAppend(RvAdvFlagType advisory_type,
	unsigned int advisory_flag);
std::string &RvAdvMsgFlagsToString(RvAdvFlagType advisory_type,
	unsigned int advisory_flag, std::string &out_string);
std::string  RvAdvMsgFlagsToString(RvAdvFlagType advisory_type,
	unsigned int advisory_flag);
std::string &RvAdvMsgFlagsToString(RvAdvFlags rv_flags,
	CmAdvFlags cm_flags, FtAdvFlags ft_flags, std::string &out_string);
std::string  RvAdvMsgFlagsToString(RvAdvFlags rv_flags, CmAdvFlags cm_flags,
	FtAdvFlags ft_flags);
std::string &RvAdvMsgFlagsToString(const RvAdvFlagsAll &flags,
	std::string &out_string);
std::string  RvAdvMsgFlagsToString(const RvAdvFlagsAll &flags);

std::string  GetRvAdvMsgFlagsHelpTextBrief(
	const std::string &param_name = "-advisory_flags");
std::string  GetRvAdvMsgFlagsHelpTextExtended(
	const std::string &param_name = "-advisory_flags",
	RvAdvFlags rv_flags = Rv_Mask, CmAdvFlags cm_flags = Cm_Mask,
	FtAdvFlags ft_flags = Ft_Mask);
std::ostream &EmitRvAdvMsgFlagsHelpTextBrief(std::ostream &o_str,
	const std::string &param_name = "-advisory_flags");
std::ostream &EmitRvAdvMsgFlagsHelpTextExtended(std::ostream &o_str,
	const std::string &param_name = "-advisory_flags",
	RvAdvFlags rv_flags = Rv_Mask, CmAdvFlags cm_flags = Cm_Mask,
	FtAdvFlags ft_flags = Ft_Mask);
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvAdvMsg_hpp__HH

