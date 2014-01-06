// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for the RvAdvMsg class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/InlineContainer.hpp>
#include <RvUtilX/RvAdvMsg.hpp>
#include <RvUtilX/GetRvField.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

namespace {

//	////////////////////////////////////////////////////////////////////////////
const TibrvMsgDateTime EmptyTibRvDateTimeField;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvFlagsMap RvAdvisoryMapList[] = {
	//	Basic advisories...
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_SlowConsumer,             RvAdvMsg::SlowConsumer2,               RvAdvMsg::SlowConsumerToString2,               "_RV.ERROR.SYSTEM.CLIENT.SLOWCONSUMER"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_FastProducer,             RvAdvMsg::FastProducer2,               RvAdvMsg::FastProducerToString2,               "_RV.WARN.SYSTEM.CLIENT.FASTPRODUCER"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_NoMemory,                 RvAdvMsg::NoMemory2,                   RvAdvMsg::NoMemoryToString2,                   "_RV.ERROR.SYSTEM.CLIENT.NOMEMORY"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_LicenseExpire_Warn,       RvAdvMsg::LicenseExpire2,              RvAdvMsg::LicenseExpireToString2,              "_RV.WARN.SYSTEM.LICENSE.EXPIRE"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_LicenseExpire_Error,      RvAdvMsg::LicenseExpire2,              RvAdvMsg::LicenseExpireToString2,              "_RV.ERROR.SYSTEM.LICENSE.EXPIRE"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Dataloss_In_Ptp,          RvAdvMsg::Dataloss2,                   RvAdvMsg::DatalossToString2,                   "_RV.ERROR.SYSTEM.DATALOSS.INBOUND.PTP"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Dataloss_In_BCast,        RvAdvMsg::Dataloss2,                   RvAdvMsg::DatalossToString2,                   "_RV.ERROR.SYSTEM.DATALOSS.INBOUND.BCAST"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Dataloss_Out_Ptp,         RvAdvMsg::Dataloss2,                   RvAdvMsg::DatalossToString2,                   "_RV.ERROR.SYSTEM.DATALOSS.OUTBOUND.PTP"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Dataloss_Out_BCast,       RvAdvMsg::Dataloss2,                   RvAdvMsg::DatalossToString2,                   "_RV.ERROR.SYSTEM.DATALOSS.OUTBOUND.BCAST"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Rvd_Disconnected,         RvAdvMsg::RvdDisconnected2,            RvAdvMsg::RvdDisconnectedToString2,            "_RV.WARN.SYSTEM.RVD.DISCONNECTED"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Rvd_Connected,            RvAdvMsg::RvdConnected2,               RvAdvMsg::RvdConnectedToString2,               "_RV.INFO.SYSTEM.RVD.CONNECTED"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_UnreachableTransport,     RvAdvMsg::UnreachableTransport2,       RvAdvMsg::UnreachableTransportToString2,       "_RV.INFO.SYSTEM.UNREACHABLE.TRANSPORT.>"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_QueueLimitExceeded,       RvAdvMsg::QueueLimitExceeded2,         RvAdvMsg::QueueLimitExceededToString2,         "_RV.WARN.SYSTEM.QUEUE.LIMIT_EXCEEDED"),	// Docs say _RV.WARN.SYSTEM.QUEUE.LIMIT_EXCEEDED.>, but this has not been observed.
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_DispatcherThreadExited,   RvAdvMsg::DispatcherThreadExited2,     RvAdvMsg::DispatcherThreadExitedToString2,     "_RV.INFO.SYSTEM.DISPATCHER.THREAD_EXITED.>"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Vc_Connected,             RvAdvMsg::VcConnected2,                RvAdvMsg::VcConnectedToString2,                "_RV.INFO.SYSTEM.VC.CONNECTED"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Vc_Disconnected_Info,     RvAdvMsg::VcDisconnected2,             RvAdvMsg::VcDisconnectedToString2,             "_RV.INFO.SYSTEM.VC.DISCONNECTED"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_Vc_Disconnected_Error,    RvAdvMsg::VcDisconnected2,             RvAdvMsg::VcDisconnectedToString2,             "_RV.ERROR.SYSTEM.VC.DISCONNECTED"),
	//	Undocumented advisories...
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_HostStart,                RvAdvMsg::HostStart2,                  RvAdvMsg::HostStartToString2,                  "_RV.INFO.SYSTEM.HOST.START.>"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_HostStop,                 RvAdvMsg::HostStop2,                   RvAdvMsg::HostStopToString2,                   "_RV.INFO.SYSTEM.HOST.STOP.>"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_HostStatus,               RvAdvMsg::HostStatus2,                 RvAdvMsg::HostStatusToString2,                 "_RV.INFO.SYSTEM.HOST.STATUS.>"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_ListenStart,              RvAdvMsg::ListenStartStop2,            RvAdvMsg::ListenStartStopToString2,            "_RV.INFO.SYSTEM.LISTEN.START.>"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_ListenStop,               RvAdvMsg::ListenStartStop2,            RvAdvMsg::ListenStartStopToString2,            "_RV.INFO.SYSTEM.LISTEN.STOP.>"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_RetransmissionIn,         RvAdvMsg::Retransmission2,             RvAdvMsg::RetransmissionToString2,             "_RV.INFO.SYSTEM.RETRANSMISSION.INBOUND.EXPECTED"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_RetransmissionOut,        RvAdvMsg::ListenStartStop2,            RvAdvMsg::ListenStartStopToString2,            "_RV.INFO.SYSTEM.RETRANSMISSION.OUTBOUND.SENT"),
	RvAdvFlagsMap(RvAdvFlagType_Rv,	Rv_DaemonRestarted,          RvAdvMsg::DaemonRestarted2,            RvAdvMsg::DaemonRestartedToString2,            "_RV.INFO.SYSTEM.DAEMON.RESTARTED")
};
const unsigned int  RvAdvisoryMapCount       =
	sizeof(RvAdvisoryMapList) / sizeof(RvAdvisoryMapList[0]);
//	----------------------------------------------------------------------------
const RvAdvFlagsMap CmAdvisoryMapList[] = {
	//	Certified message advisories...
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_DeliveryConfirm,          RvAdvMsg::CmDeliveryConfirm2,          RvAdvMsg::CmDeliveryConfirmToString2,          "_RV.INFO.RVCM.DELIVERY.CONFIRM.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_DeliveryComplete,         RvAdvMsg::CmDeliveryComplete2,         RvAdvMsg::CmDeliveryCompleteToString2,         "_RV.INFO.RVCM.DELIVERY.COMPLETE.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_DeliveryNoResponse,       RvAdvMsg::CmDeliveryNoResponse2,       RvAdvMsg::CmDeliveryNoResponseToString2,       "_RV.WARN.RVCM.DELIVERY.NO_RESPONSE.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_DeliveryFailed,           RvAdvMsg::CmDeliveryFailed2,           RvAdvMsg::CmDeliveryFailedToString2,           "_RV.ERROR.RVCM.DELIVERY.FAILED.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_DeliveryUnavailable,      RvAdvMsg::CmDeliveryUnavailable2,      RvAdvMsg::CmDeliveryUnavailableToString2,      "_RV.ERROR.RVCM.DELIVERY.UNAVAILABLE.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RegistrationRequest,      RvAdvMsg::CmRegistrationRequest2,      RvAdvMsg::CmRegistrationRequestToString2,      "_RV.INFO.RVCM.REGISTRATION.REQUEST.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RegistrationCertified,    RvAdvMsg::CmRegistrationCertified2,    RvAdvMsg::CmRegistrationCertifiedToString2,    "_RV.INFO.RVCM.REGISTRATION.CERTIFIED.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RegistrationNotCertified, RvAdvMsg::CmRegistrationNotCertified2, RvAdvMsg::CmRegistrationNotCertifiedToString2, "_RV.WARN.RVCM.REGISTRATION.NOT_CERTIFIED.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RegistrationNoResponse,   RvAdvMsg::CmRegistrationNoResponse2,   RvAdvMsg::CmRegistrationNoResponseToString2,   "_RV.WARN.RVCM.REGISTRATION.NO_RESPONSE.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RegistrationClosed,       RvAdvMsg::CmRegistrationClosed2,       RvAdvMsg::CmRegistrationClosedToString2,       "_RV.INFO.RVCM.REGISTRATION.CLOSED.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RegistrationDiscovery,    RvAdvMsg::CmRegistrationDiscovery2,    RvAdvMsg::CmRegistrationDiscoveryToString2,    "_RV.INFO.RVCM.REGISTRATION.DISCOVERY.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RegistrationMoved,        RvAdvMsg::CmRegistrationMoved2,        RvAdvMsg::CmRegistrationMovedToString2,        "_RV.INFO.RVCM.REGISTRATION.MOVED.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RegistrationCollision,    RvAdvMsg::CmRegistrationCollision2,    RvAdvMsg::CmRegistrationCollisionToString2,    "_RV.ERROR.RVCM.REGISTRATION.COLLISION.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RelayConnected,           RvAdvMsg::CmRelayConnected2,           RvAdvMsg::CmRelayConnectedToString2,           "_RV.INFO.RVCM.RELAY.CONNECTED.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_RelayDisconnected,        RvAdvMsg::CmRelayDisconnected2,        RvAdvMsg::CmRelayDisconnectedToString2,        "_RV.INFO.RVCM.RELAY.DISCONNECTED.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_QueueSchedulerActive,     RvAdvMsg::CmQueueSchedulerActive2,     RvAdvMsg::CmQueueSchedulerActiveToString2,     "_RV.INFO.RVCM.QUEUE.SCHEDULER.ACTIVE.>"),
	RvAdvFlagsMap(RvAdvFlagType_Cm,	Cm_QueueSchedulerInactive,   RvAdvMsg::CmQueueSchedulerInactive2,   RvAdvMsg::CmQueueSchedulerInactiveToString2,   "_RV.INFO.RVCM.QUEUE.SCHEDULER.INACTIVE.>")
};
const unsigned int  CmAdvisoryMapCount       =
	sizeof(CmAdvisoryMapList) / sizeof(CmAdvisoryMapList[0]);
//	----------------------------------------------------------------------------
const RvAdvFlagsMap FtAdvisoryMapList[] = {
	//	Fault tolerance advisories...
	RvAdvFlagsMap(RvAdvFlagType_Ft,	Ft_ParamMismatch_Warn,       RvAdvMsg::FtParamMismatch2,            RvAdvMsg::FtParamMismatchToString2,            "_RV.WARN.RVFT.PARAM_MISMATCH.>"),
	RvAdvFlagsMap(RvAdvFlagType_Ft,	Ft_ParamMismatch_Error,      RvAdvMsg::FtParamMismatch2,            RvAdvMsg::FtParamMismatchToString2,            "_RV.ERROR.RVFT.PARAM_MISMATCH.>"),
	RvAdvFlagsMap(RvAdvFlagType_Ft,	Ft_DisablingMember,          RvAdvMsg::FtDisablingMember2,          RvAdvMsg::FtDisablingMemberToString2,          "_RV.ERROR.RVFT.DISABLING_MEMBER.>"),
	RvAdvFlagsMap(RvAdvFlagType_Ft,	Ft_TooManyActive,            RvAdvMsg::FtTooManyActive2,            RvAdvMsg::FtTooManyActiveToString2,            "_RV.WARN.RVFT.TOO_MANY_ACTIVE.>"),
	RvAdvFlagsMap(RvAdvFlagType_Ft,	Ft_TooFewActive,             RvAdvMsg::FtTooFewActive2,             RvAdvMsg::FtTooFewActiveToString2,             "_RV.WARN.RVFT.TOO_FEW_ACTIVE.>")
};
const unsigned int  FtAdvisoryMapCount       =
	sizeof(FtAdvisoryMapList) / sizeof(FtAdvisoryMapList[0]);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvAdvMsgFlagString {
	RvAdvFlagType  advisory_type_;
	unsigned int   advisory_flag_;
	const char    *advisory_string_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvMsgFlagString RvAdvMsgFlagStringList[] = {
	//	First, all of the canonical names...
	{ RvAdvFlagType_Rv, Rv_SlowConsumer,             "SlowConsumer"                },
	{ RvAdvFlagType_Rv, Rv_FastProducer,             "FastProducer"                },
	{ RvAdvFlagType_Rv, Rv_NoMemory,                 "NoMemory"                    },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Warn,       "LicenseExpireWarning"        },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Error,      "LicenseExpireError"          },
	{ RvAdvFlagType_Rv, Rv_Dataloss_In_Ptp,          "DatalossInboundPtp"          },
	{ RvAdvFlagType_Rv, Rv_Dataloss_In_BCast,        "DatalossInboundBCast"        },
	{ RvAdvFlagType_Rv, Rv_Dataloss_Out_Ptp,         "DatalossOutboundPtp"         },
	{ RvAdvFlagType_Rv, Rv_Dataloss_Out_BCast,       "DatalossOutboundBCast"       },
	{ RvAdvFlagType_Rv, Rv_Rvd_Disconnected,         "RvdDisconnected"             },
	{ RvAdvFlagType_Rv, Rv_Rvd_Connected,            "RvdConnected"                },
	{ RvAdvFlagType_Rv, Rv_UnreachableTransport,     "UnreachableTransport"        },
	{ RvAdvFlagType_Rv, Rv_QueueLimitExceeded,       "QueueLimitExceeded"          },
	{ RvAdvFlagType_Rv, Rv_DispatcherThreadExited,   "DispatcherThreadExited"      },
	{ RvAdvFlagType_Rv, Rv_Vc_Connected,             "VcConnected"                 },
	{ RvAdvFlagType_Rv, Rv_Vc_Disconnected_Info,     "VcDisconnectedInfo"          },
	{ RvAdvFlagType_Rv, Rv_Vc_Disconnected_Error,    "VcDisconnectedError"         },
	{ RvAdvFlagType_Rv, Rv_HostStart,                "HostStart"                   },
	{ RvAdvFlagType_Rv, Rv_HostStop,                 "HostStop"                    },
	{ RvAdvFlagType_Rv, Rv_HostStatus,               "HostStatus"                  },
	{ RvAdvFlagType_Rv, Rv_ListenStart,              "ListenStart"                 },
	{ RvAdvFlagType_Rv, Rv_ListenStop,               "ListenStop"                  },
	{ RvAdvFlagType_Rv, Rv_RetransmissionIn,         "RetransmissionInbound"       },
	{ RvAdvFlagType_Rv, Rv_RetransmissionOut,        "RetransmissionOutbound"      },
	{ RvAdvFlagType_Rv, Rv_DaemonRestarted,          "DaemonRestarted"             },
	{ RvAdvFlagType_Cm, Cm_DeliveryConfirm,          "DeliveryConfirm"             },
	{ RvAdvFlagType_Cm, Cm_DeliveryComplete,         "DeliveryComplete"            },
	{ RvAdvFlagType_Cm, Cm_DeliveryNoResponse,       "DeliveryNoResponse"          },
	{ RvAdvFlagType_Cm, Cm_DeliveryFailed,           "DeliveryFailed"              },
	{ RvAdvFlagType_Cm, Cm_DeliveryUnavailable,      "DeliveryUnavailable"         },
	{ RvAdvFlagType_Cm, Cm_RegistrationRequest,      "RegistrationRequest"         },
	{ RvAdvFlagType_Cm, Cm_RegistrationCertified,    "RegistrationCertified"       },
	{ RvAdvFlagType_Cm, Cm_RegistrationNotCertified, "RegistrationNotCertified"    },
	{ RvAdvFlagType_Cm, Cm_RegistrationNoResponse,   "RegistrationNoResponse"      },
	{ RvAdvFlagType_Cm, Cm_RegistrationClosed,       "RegistrationClosed"          },
	{ RvAdvFlagType_Cm, Cm_RegistrationDiscovery,    "RegistrationDiscovery"       },
	{ RvAdvFlagType_Cm, Cm_RegistrationMoved,        "RegistrationMoved"           },
	{ RvAdvFlagType_Cm, Cm_RegistrationCollision,    "RegistrationCollision"       },
	{ RvAdvFlagType_Cm, Cm_RelayConnected,           "RelayConnected"              },
	{ RvAdvFlagType_Cm, Cm_RelayDisconnected,        "RelayDisconnected"           },
	{ RvAdvFlagType_Cm, Cm_QueueSchedulerActive,     "QueueSchedulerActive"        },
	{ RvAdvFlagType_Cm, Cm_QueueSchedulerInactive,   "QueueSchedulerInactive"      },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "ParameterMismatchWarning"    },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Error,      "ParameterMismatchError"      },
	{ RvAdvFlagType_Ft, Ft_DisablingMember,          "DisablingMember"             },
	{ RvAdvFlagType_Ft, Ft_TooManyActive,            "TooManyActive"               },
	{ RvAdvFlagType_Ft, Ft_TooFewActive,             "TooFewActive"                },
	//	Now the equivalents which aree the same text as the enumerations...
	{ RvAdvFlagType_Rv, Rv_SlowConsumer,             "Rv_SlowConsumer"             },
	{ RvAdvFlagType_Rv, Rv_FastProducer,             "Rv_FastProducer"             },
	{ RvAdvFlagType_Rv, Rv_NoMemory,                 "Rv_NoMemory"                 },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Warn,       "Rv_LicenseExpire_Warn"       },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Error,      "Rv_LicenseExpire_Error"      },
	{ RvAdvFlagType_Rv, Rv_Dataloss_In_Ptp,          "Rv_Dataloss_In_Ptp"          },
	{ RvAdvFlagType_Rv, Rv_Dataloss_In_BCast,        "Rv_Dataloss_In_BCast"        },
	{ RvAdvFlagType_Rv, Rv_Dataloss_Out_Ptp,         "Rv_Dataloss_Out_Ptp"         },
	{ RvAdvFlagType_Rv, Rv_Dataloss_Out_BCast,       "Rv_Dataloss_Out_BCast"       },
	{ RvAdvFlagType_Rv, Rv_Rvd_Disconnected,         "Rv_Rvd_Disconnected"         },
	{ RvAdvFlagType_Rv, Rv_Rvd_Connected,            "Rv_Rvd_Connected"            },
	{ RvAdvFlagType_Rv, Rv_UnreachableTransport,     "Rv_UnreachableTransport"     },
	{ RvAdvFlagType_Rv, Rv_QueueLimitExceeded,       "Rv_QueueLimitExceeded"       },
	{ RvAdvFlagType_Rv, Rv_DispatcherThreadExited,   "Rv_DispatcherThreadExited"   },
	{ RvAdvFlagType_Rv, Rv_Vc_Connected,             "Rv_Vc_Connected"             },
	{ RvAdvFlagType_Rv, Rv_Vc_Disconnected_Info,     "Rv_Vc_Disconnected_Info"     },
	{ RvAdvFlagType_Rv, Rv_Vc_Disconnected_Error,    "Rv_Vc_Disconnected_Error"    },
	{ RvAdvFlagType_Rv, Rv_HostStart,                "Rv_HostStart"                },
	{ RvAdvFlagType_Rv, Rv_HostStop,                 "Rv_HostStop"                 },
	{ RvAdvFlagType_Rv, Rv_HostStatus,               "Rv_HostStatus"               },
	{ RvAdvFlagType_Rv, Rv_ListenStart,              "Rv_ListenStart"              },
	{ RvAdvFlagType_Rv, Rv_ListenStop,               "Rv_ListenStop"               },
	{ RvAdvFlagType_Rv, Rv_RetransmissionIn,         "Rv_RetransmissionIn"         },
	{ RvAdvFlagType_Rv, Rv_RetransmissionOut,        "Rv_RetransmissionOut"        },
	{ RvAdvFlagType_Rv, Rv_DaemonRestarted,          "Rv_DaemonRestarted"          },
	{ RvAdvFlagType_Cm, Cm_DeliveryConfirm,          "Cm_DeliveryConfirm"          },
	{ RvAdvFlagType_Cm, Cm_DeliveryComplete,         "Cm_DeliveryComplete"         },
	{ RvAdvFlagType_Cm, Cm_DeliveryNoResponse,       "Cm_DeliveryNoResponse"       },
	{ RvAdvFlagType_Cm, Cm_DeliveryFailed,           "Cm_DeliveryFailed"           },
	{ RvAdvFlagType_Cm, Cm_DeliveryUnavailable,      "Cm_DeliveryUnavailable"      },
	{ RvAdvFlagType_Cm, Cm_RegistrationRequest,      "Cm_RegistrationRequest"      },
	{ RvAdvFlagType_Cm, Cm_RegistrationCertified,    "Cm_RegistrationCertified"    },
	{ RvAdvFlagType_Cm, Cm_RegistrationNotCertified, "Cm_RegistrationNotCertified" },
	{ RvAdvFlagType_Cm, Cm_RegistrationNoResponse,   "Cm_RegistrationNoResponse"   },
	{ RvAdvFlagType_Cm, Cm_RegistrationClosed,       "Cm_RegistrationClosed"       },
	{ RvAdvFlagType_Cm, Cm_RegistrationDiscovery,    "Cm_RegistrationDiscovery"    },
	{ RvAdvFlagType_Cm, Cm_RegistrationMoved,        "Cm_RegistrationMoved"        },
	{ RvAdvFlagType_Cm, Cm_RegistrationCollision,    "Cm_RegistrationCollision"    },
	{ RvAdvFlagType_Cm, Cm_RelayConnected,           "Cm_RelayConnected"           },
	{ RvAdvFlagType_Cm, Cm_RelayDisconnected,        "Cm_RelayDisconnected"        },
	{ RvAdvFlagType_Cm, Cm_QueueSchedulerActive,     "Cm_QueueSchedulerActive"     },
	{ RvAdvFlagType_Cm, Cm_QueueSchedulerInactive,   "Cm_QueueSchedulerInactive"   },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "Ft_ParamMismatch_Warn"       },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Error,      "Ft_ParamMismatch_Error"      },
	{ RvAdvFlagType_Ft, Ft_DisablingMember,          "Ft_DisablingMember"          },
	{ RvAdvFlagType_Ft, Ft_TooManyActive,            "Ft_TooManyActive"            },
	{ RvAdvFlagType_Ft, Ft_TooFewActive,             "Ft_TooFewActive"             },
	//	Finally, a bunch of synonyms...
	{ RvAdvFlagType_Rv, Rv_SlowConsumer,             "Slow_Consumer"               },
	{ RvAdvFlagType_Rv, Rv_FastProducer,             "Fast_Producer"               },
	{ RvAdvFlagType_Rv, Rv_NoMemory,                 "No_Memory"                   },
	{ RvAdvFlagType_Rv, Rv_NoMemory,                 "No_Mem"                      },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Warn,       "LicenseExpireWarn"           },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Warn,       "LicenseExpire_Warn"          },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Warn,       "LicenseExpire_Warning"       },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Warn,       "License_Expire_Warn"         },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Warn,       "License_Expire_Warning"      },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Error,      "LicenseExpire_Error"         },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Error,      "LicenseExpire_Error"         },
	{ RvAdvFlagType_Rv, Rv_LicenseExpire_Error,      "License_Expire_Error"        },
	{ RvAdvFlagType_Rv, Rv_Dataloss_In_Ptp,          "Dataloss_In_Ptp"             },
	{ RvAdvFlagType_Rv, Rv_Dataloss_In_Ptp,          "Dataloss_Inbound_Ptp"        },
	{ RvAdvFlagType_Rv, Rv_Dataloss_In_BCast,        "Dataloss_In_BCast"           },
	{ RvAdvFlagType_Rv, Rv_Dataloss_In_BCast,        "Dataloss_Inbound_BCast"      },
	{ RvAdvFlagType_Rv, Rv_Dataloss_Out_Ptp,         "Dataloss_Out_Ptp"            },
	{ RvAdvFlagType_Rv, Rv_Dataloss_Out_Ptp,         "Dataloss_Outbound_Ptp"       },
	{ RvAdvFlagType_Rv, Rv_Dataloss_Out_BCast,       "Dataloss_Out_BCast"          },
	{ RvAdvFlagType_Rv, Rv_Dataloss_Out_BCast,       "Dataloss_Outbound_BCast"     },
	{ RvAdvFlagType_Rv, Rv_Rvd_Disconnected,         "Rvd_Disconnected"            },
	{ RvAdvFlagType_Rv, Rv_Rvd_Connected,            "Rvd_Connected"               },
	{ RvAdvFlagType_Rv, Rv_UnreachableTransport,     "Unreachable_Transport"       },
	{ RvAdvFlagType_Rv, Rv_QueueLimitExceeded,       "Queue_Limit_Exceeded"        },
	{ RvAdvFlagType_Rv, Rv_DispatcherThreadExited,   "Dispatcher_Thread_Exited"    },
	{ RvAdvFlagType_Rv, Rv_Vc_Connected,             "Vc_Connected"                },
	{ RvAdvFlagType_Rv, Rv_Vc_Disconnected_Info,     "Vc_Disconnected_Info"        },
	{ RvAdvFlagType_Rv, Rv_Vc_Disconnected_Error,    "Vc_Disconnected_Error"       },
	{ RvAdvFlagType_Rv, Rv_HostStart,                "Host_Start"                  },
	{ RvAdvFlagType_Rv, Rv_HostStop,                 "Host_Stop"                   },
	{ RvAdvFlagType_Rv, Rv_HostStatus,               "Host_Status"                 },
	{ RvAdvFlagType_Rv, Rv_ListenStart,              "Listen_Start"                },
	{ RvAdvFlagType_Rv, Rv_ListenStop,               "Listen_Stop"                 },
	{ RvAdvFlagType_Rv, Rv_RetransmissionIn,         "Retransmission_In"           },
	{ RvAdvFlagType_Rv, Rv_RetransmissionIn,         "Retransmission_Inbound"      },
	{ RvAdvFlagType_Rv, Rv_RetransmissionOut,        "Retransmission_Out"          },
	{ RvAdvFlagType_Rv, Rv_RetransmissionOut,        "Retransmission_Outbound"     },
	{ RvAdvFlagType_Rv, Rv_DaemonRestarted,          "Daemon_Restarted"            },
	{ RvAdvFlagType_Cm, Cm_DeliveryConfirm,          "Delivery_Confirm"            },
	{ RvAdvFlagType_Cm, Cm_DeliveryComplete,         "Delivery_Complete"           },
	{ RvAdvFlagType_Cm, Cm_DeliveryNoResponse,       "Delivery_No_Response"        },
	{ RvAdvFlagType_Cm, Cm_DeliveryFailed,           "Delivery_Failed"             },
	{ RvAdvFlagType_Cm, Cm_DeliveryUnavailable,      "Delivery_Unavailable"        },
	{ RvAdvFlagType_Cm, Cm_RegistrationRequest,      "Registration_Request"        },
	{ RvAdvFlagType_Cm, Cm_RegistrationCertified,    "Registration_Certified"      },
	{ RvAdvFlagType_Cm, Cm_RegistrationNotCertified, "Registration_Not_Certified"  },
	{ RvAdvFlagType_Cm, Cm_RegistrationNoResponse,   "Registration_No_Response"    },
	{ RvAdvFlagType_Cm, Cm_RegistrationClosed,       "Registration_Closed"         },
	{ RvAdvFlagType_Cm, Cm_RegistrationDiscovery,    "Registration_Discovery"      },
	{ RvAdvFlagType_Cm, Cm_RegistrationMoved,        "Registration_Moved"          },
	{ RvAdvFlagType_Cm, Cm_RegistrationCollision,    "Registration_Collision"      },
	{ RvAdvFlagType_Cm, Cm_RelayConnected,           "Relay_Connected"             },
	{ RvAdvFlagType_Cm, Cm_RelayDisconnected,        "Relay_Disconnected"          },
	{ RvAdvFlagType_Cm, Cm_QueueSchedulerActive,     "Queue_Scheduler_Active"      },
	{ RvAdvFlagType_Cm, Cm_QueueSchedulerInactive,   "Queue_Scheduler_Inactive"    },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "ParamMismatch_Warn"          },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "ParamMismatch_Warning"       },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "Param_Mismatch_Warn"         },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "Param_Mismatch_Warning"      },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "ParameterMismatch_Warn"      },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "ParameterMismatch_Warning"   },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "Parameter_Mismatch_Warn"     },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Warn,       "Parameter_Mismatch_Warning"  },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Error,      "ParamMismatch_Error"         },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Error,      "ParameterMismatch_Error"     },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Error,      "Param_Mismatch_Error"        },
	{ RvAdvFlagType_Ft, Ft_ParamMismatch_Error,      "Parameter_Mismatch_Error"    },
	{ RvAdvFlagType_Ft, Ft_DisablingMember,          "Disabling_Member"            },
	{ RvAdvFlagType_Ft, Ft_TooManyActive,            "Too_Many_Active"             },
	{ RvAdvFlagType_Ft, Ft_TooFewActive,             "Too_Few_Active"              }
};
const unsigned int       RvAdvMsgFlagStringCount  =
	sizeof(RvAdvMsgFlagStringList) / sizeof(RvAdvMsgFlagStringList[0]);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvMsgFlagString RvAdvMsgFlagStringExtList[] = {
	{ RvAdvFlagType_Rv, Rv_None,                     "RvNone"                      },
	{ RvAdvFlagType_Cm, Cm_None,                     "CmNone"                      },
	{ RvAdvFlagType_Ft, Ft_None,                     "FtNone"                      },
	{ RvAdvFlagType_Rv, Rv_None,                     "Rv_None"                     },
	{ RvAdvFlagType_Cm, Cm_None,                     "Cm_None"                     },
	{ RvAdvFlagType_Ft, Ft_None,                     "Ft_None"                     },
	{ RvAdvFlagType_Rv, Rv_Default,                  "RvDefault"                   },
	{ RvAdvFlagType_Cm, Cm_Default,                  "CmDefault"                   },
	{ RvAdvFlagType_Ft, Ft_Default,                  "FtDefault"                   },
	{ RvAdvFlagType_Rv, Rv_Default,                  "Rv_Default"                  },
	{ RvAdvFlagType_Cm, Cm_Default,                  "Cm_Default"                  },
	{ RvAdvFlagType_Ft, Ft_Default,                  "Ft_Default"                  },
	{ RvAdvFlagType_Rv, Rv_Mask,                     "RvMask"                      },
	{ RvAdvFlagType_Cm, Cm_Mask,                     "CmMask"                      },
	{ RvAdvFlagType_Ft, Ft_Mask,                     "FtMask"                      },
	{ RvAdvFlagType_Rv, Rv_Mask,                     "Rv_Mask"                     },
	{ RvAdvFlagType_Cm, Cm_Mask,                     "Cm_Mask"                     },
	{ RvAdvFlagType_Ft, Ft_Mask,                     "Ft_Mask"                     },
	{ RvAdvFlagType_Rv, Rv_Mask,                     "RvAll"                       },
	{ RvAdvFlagType_Cm, Cm_Mask,                     "CmAll"                       },
	{ RvAdvFlagType_Ft, Ft_Mask,                     "FtAll"                       },
	{ RvAdvFlagType_Rv, Rv_Mask,                     "Rv_All"                      },
	{ RvAdvFlagType_Cm, Cm_Mask,                     "Cm_All"                      },
	{ RvAdvFlagType_Ft, Ft_Mask,                     "Ft_All"                      }

};
const unsigned int       RvAdvMsgFlagStringExtCount  =
	sizeof(RvAdvMsgFlagStringExtList) / sizeof(RvAdvMsgFlagStringExtList[0]);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvMsgFlagsHelpTextAppend(RvAdvFlagType advisory_type,
	unsigned int advisory_flag, std::ostream &o_str)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < RvAdvMsgFlagStringCount; ++count_1) {
		if ((advisory_type == RvAdvMsgFlagStringList[count_1].advisory_type_) &&
			 (advisory_flag  & RvAdvMsgFlagStringList[count_1].advisory_flag_)) {
			o_str
				<< "         " << RvAdvMsgFlagStringList[count_1].advisory_string_
				<< std::endl;
			//	Don't display following synonyms...
			advisory_flag &= ~RvAdvMsgFlagStringList[count_1].advisory_flag_;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool StringToRvAdvMsgFlagsAppend(const std::string &in_string,
	unsigned int strings_count, const RvAdvMsgFlagString *strings_list,
	RvAdvFlags &rv_flags, CmAdvFlags &cm_flags, FtAdvFlags &ft_flags)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < strings_count; ++count_1) {
		if (!stricmp(in_string.c_str(), strings_list[count_1].advisory_string_)){
			if (strings_list[count_1].advisory_type_ == RvAdvFlagType_Rv)
				rv_flags = static_cast<RvAdvFlags>(rv_flags |
					strings_list[count_1].advisory_flag_);
			else if (strings_list[count_1].advisory_type_ == RvAdvFlagType_Cm)
				cm_flags = static_cast<CmAdvFlags>(cm_flags |
					strings_list[count_1].advisory_flag_);
			else
				ft_flags = static_cast<FtAdvFlags>(ft_flags |
					strings_list[count_1].advisory_flag_);
			return(true);
		}
	}

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	Copied from MLB::SockLibX::AddressIP::GetHostIPAddress() so that the
	SockLibX DLL doesn't become a dependency when building RvUtilX as a
	DLL (as is necessary when producing RvUtilX Python).
*/
std::string MLB_SockLibX_AddressIP_IPAddressToString(tibrv_ipaddr32 ip_address)
{
	using namespace MLB::Utility;

	return(
		AnyToString(reinterpret_cast<unsigned char *>(&ip_address)[0]) + "." +
		AnyToString(reinterpret_cast<unsigned char *>(&ip_address)[1]) + "." +
		AnyToString(reinterpret_cast<unsigned char *>(&ip_address)[2]) + "." +
		AnyToString(reinterpret_cast<unsigned char *>(&ip_address)[3]));
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg::RvAdvMsg()
	:initialized_from_message_(false)
	,ADV_CLASS_(NULL)
	,ADV_SOURCE_(NULL)
	,ADV_NAME_(NULL)
	,ADV_DESC_(NULL)
	,waiting_(0)
	,dropped_(0)
	,bytes_dropped_(0)
	,expiretime_()
	,license_expired_(false)
	,dataloss_outbound_(false)
	,dataloss_ptp_(false)
	,dataloss_host_(0)
	,dataloss_lost_(0)
	,tport_(NULL)
	,descr_(NULL)
	,dtexit_status_(0)
	,dtexit_description_(NULL)
	,vc_disc_error_(false)
	,vc_disc_description_(NULL)
	,hostaddr_(0)
	,sn_(0)
	,sn_flag_(false)
	,os_(0)
	,ver_(NULL)
	,up_(0)
	,up_flag_(false)
	,httpaddr_(0)
	,httpport_(0)
	,ms_(0)
	,bs_(0)
	,mr_(0)
	,br_(0)
	,ps_(0)
	,pr_(0)
	,rx_(0)
	,pm_(0)
	,hsaddr_(0)
	,hsport_(0)
	,hstatus_time_()
	,idl_(0)
	,odl_(0)
	,ipport_(0)
	,service_(NULL)
	,network_(NULL)
	,listen_stop_(false)
	,listen_sub_(NULL)
	,listen_id_(NULL)
	,listen_refcnt_(0)
	,restart_host_(0)
	,cm_subject_(NULL)
	,cm_listener_(NULL)
	,cm_seqno_(0)
	,cm_seqno_begin_(0)
	,cm_seqno_end_(0)
	,cm_sender_(NULL)
	,cm_seqno_last_sent_(0)
	,cm_seqno_last_confirmed_(0)
	,cm_name_(NULL)
	,ft_active_goal_differs_(false)
	,ft_activation_interval_differs_(false)
	,ft_heartbeat_interval_differs_(false)
	,ft_other_member_prepares_before_(false)
	,ft_this_member_prepares_before_(false)
	,ft_other_param_mismatch_(false)
	,adv_class_log_(MLB::Utility::LogLevel_Notice)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg::RvAdvMsg(const TibrvMsg &msg)
	:initialized_from_message_(false)
	,ADV_CLASS_(NULL)
	,ADV_SOURCE_(NULL)
	,ADV_NAME_(NULL)
	,ADV_DESC_(NULL)
	,waiting_(0)
	,dropped_(0)
	,bytes_dropped_(0)
	,expiretime_()
	,license_expired_(false)
	,dataloss_outbound_(false)
	,dataloss_ptp_(false)
	,dataloss_host_(0)
	,dataloss_lost_(0)
	,tport_(NULL)
	,descr_(NULL)
	,dtexit_status_(0)
	,dtexit_description_(NULL)
	,vc_disc_error_(false)
	,vc_disc_description_(NULL)
	,hostaddr_(0)
	,sn_(0)
	,sn_flag_(false)
	,os_(0)
	,ver_(NULL)
	,up_(0)
	,up_flag_(false)
	,httpaddr_(0)
	,httpport_(0)
	,ms_(0)
	,bs_(0)
	,mr_(0)
	,br_(0)
	,ps_(0)
	,pr_(0)
	,rx_(0)
	,pm_(0)
	,hsaddr_(0)
	,hsport_(0)
	,hstatus_time_()
	,idl_(0)
	,odl_(0)
	,ipport_(0)
	,service_(NULL)
	,network_(NULL)
	,listen_stop_(false)
	,listen_sub_(NULL)
	,listen_id_(NULL)
	,listen_refcnt_(0)
	,restart_host_(0)
	,cm_subject_(NULL)
	,cm_listener_(NULL)
	,cm_seqno_(0)
	,cm_seqno_begin_(0)
	,cm_seqno_end_(0)
	,cm_sender_(NULL)
	,cm_seqno_last_sent_(0)
	,cm_seqno_last_confirmed_(0)
	,cm_name_(NULL)
	,ft_active_goal_differs_(false)
	,ft_activation_interval_differs_(false)
	,ft_heartbeat_interval_differs_(false)
	,ft_other_member_prepares_before_(false)
	,ft_this_member_prepares_before_(false)
	,ft_other_param_mismatch_(false)
	,adv_class_log_(MLB::Utility::LogLevel_Notice)
{
	GetBasicAdv(msg);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg::~RvAdvMsg()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg::RvAdvMsg(const RvAdvMsg &other)
	:initialized_from_message_(other.initialized_from_message_)
	,ADV_CLASS_(other.ADV_CLASS_)
	,ADV_SOURCE_(other.ADV_SOURCE_)
	,ADV_NAME_(other.ADV_NAME_)
	,ADV_DESC_(other.ADV_DESC_)
	,waiting_(other.waiting_)
	,dropped_(other.dropped_)
	,bytes_dropped_(other.bytes_dropped_)
	,expiretime_(other.expiretime_)
	,license_expired_(other.license_expired_)
	,dataloss_outbound_(other.dataloss_outbound_)
	,dataloss_ptp_(other.dataloss_ptp_)
	,dataloss_host_(other.dataloss_host_)
	,dataloss_lost_(other.dataloss_lost_)
	,tport_(other.tport_)
	,descr_(other.descr_)
	,dtexit_status_(other.dtexit_status_)
	,dtexit_description_(other.dtexit_description_)
	,vc_disc_error_(other.vc_disc_error_)
	,vc_disc_description_(other.vc_disc_description_)
	,hostaddr_(other.hostaddr_)
	,sn_(other.sn_)
	,sn_flag_(other.sn_flag_)
	,os_(other.os_)
	,ver_(other.ver_)
	,up_(other.up_)
	,up_flag_(other.up_flag_)
	,httpaddr_(other.httpaddr_)
	,httpport_(other.httpport_)
	,ms_(other.ms_)
	,bs_(other.bs_)
	,mr_(other.mr_)
	,br_(other.br_)
	,ps_(other.ps_)
	,pr_(other.pr_)
	,rx_(other.rx_)
	,pm_(other.pm_)
	,hsaddr_(other.hsaddr_)
	,hsport_(other.hsport_)
	,hstatus_time_(other.hstatus_time_)
	,idl_(other.idl_)
	,odl_(other.odl_)
	,ipport_(other.ipport_)
	,service_(other.service_)
	,network_(other.network_)
	,listen_stop_(other.listen_stop_)
	,listen_sub_(other.listen_sub_)
	,listen_id_(other.listen_id_)
	,listen_refcnt_(other.listen_refcnt_)
	,restart_host_(other.restart_host_)
	,cm_subject_(other.cm_subject_)
	,cm_listener_(other.cm_listener_)
	,cm_seqno_(other.cm_seqno_)
	,cm_seqno_begin_(other.cm_seqno_begin_)
	,cm_seqno_end_(other.cm_seqno_end_)
	,cm_sender_(other.cm_sender_)
	,cm_seqno_last_sent_(other.cm_seqno_last_sent_)
	,cm_seqno_last_confirmed_(other.cm_seqno_last_confirmed_)
	,cm_name_(other.cm_name_)
	,ft_active_goal_differs_(other.ft_active_goal_differs_)
	,ft_activation_interval_differs_(other.ft_activation_interval_differs_)
	,ft_heartbeat_interval_differs_(other.ft_heartbeat_interval_differs_)
	,ft_other_member_prepares_before_(other.ft_other_member_prepares_before_)
	,ft_this_member_prepares_before_(other.ft_this_member_prepares_before_)
	,ft_other_param_mismatch_(other.ft_other_param_mismatch_)
	,adv_class_log_(other.adv_class_log_)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg & RvAdvMsg::operator = (const RvAdvMsg &other)
{
	RvAdvMsg tmp(other);

	return(swap(tmp));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::swap(RvAdvMsg &other)
{
	std::swap(initialized_from_message_,        other.initialized_from_message_);
	std::swap(ADV_CLASS_,                       other.ADV_CLASS_);
	std::swap(ADV_SOURCE_,                      other.ADV_SOURCE_);
	std::swap(ADV_NAME_,                        other.ADV_NAME_);
	std::swap(ADV_DESC_,                        other.ADV_DESC_);
	std::swap(waiting_,                         other.waiting_);
	std::swap(dropped_,                         other.dropped_);
	std::swap(bytes_dropped_,                   other.bytes_dropped_);
	std::swap(expiretime_,                      other.expiretime_);
	std::swap(license_expired_,                 other.license_expired_);
	std::swap(dataloss_outbound_,               other.dataloss_outbound_);
	std::swap(dataloss_ptp_,                    other.dataloss_ptp_);
	std::swap(dataloss_host_,                   other.dataloss_host_);
	std::swap(dataloss_lost_,                   other.dataloss_lost_);
	std::swap(tport_,                           other.tport_);
	std::swap(descr_,                           other.descr_);
	std::swap(dtexit_status_,                   other.dtexit_status_);
	std::swap(dtexit_description_,              other.dtexit_description_);
	std::swap(vc_disc_error_,                   other.vc_disc_error_);
	std::swap(vc_disc_description_,             other.vc_disc_description_);
	std::swap(hostaddr_,                        other.hostaddr_);
	std::swap(sn_,                              other.sn_);
	std::swap(sn_flag_,                         other.sn_flag_);
	std::swap(os_,                              other.os_);
	std::swap(ver_,                             other.ver_);
	std::swap(up_,                              other.up_);
	std::swap(up_flag_,                         other.up_flag_);
	std::swap(httpaddr_,                        other.httpaddr_);
	std::swap(httpport_,                        other.httpport_);
	std::swap(ms_,                              other.ms_);
	std::swap(bs_,                              other.bs_);
	std::swap(mr_,                              other.mr_);
	std::swap(br_,                              other.br_);
	std::swap(ps_,                              other.ps_);
	std::swap(pr_,                              other.pr_);
	std::swap(rx_,                              other.rx_);
	std::swap(pm_,                              other.pm_);
	std::swap(hsaddr_,                          other.hsaddr_);
	std::swap(hsport_,                          other.hsport_);
	std::swap(hstatus_time_,                    other.hstatus_time_);
	std::swap(idl_,                             other.idl_);
	std::swap(odl_,                             other.odl_);
	std::swap(ipport_,                          other.ipport_);
	std::swap(service_,                         other.service_);
	std::swap(network_,                         other.network_);
	std::swap(listen_stop_,                     other.listen_stop_);
	std::swap(listen_sub_,                      other.listen_sub_);
	std::swap(listen_id_,                       other.listen_id_);
	std::swap(listen_refcnt_,                   other.listen_refcnt_);
	std::swap(restart_host_,                    other.restart_host_);
	std::swap(cm_subject_,                      other.cm_subject_);
	std::swap(cm_listener_,                     other.cm_listener_);
	std::swap(cm_seqno_,                        other.cm_seqno_);
	std::swap(cm_seqno_begin_,                  other.cm_seqno_begin_);
	std::swap(cm_seqno_end_,                    other.cm_seqno_end_);
	std::swap(cm_sender_,                       other.cm_sender_);
	std::swap(cm_seqno_last_sent_,              other.cm_seqno_last_sent_);
	std::swap(cm_seqno_last_confirmed_,         other.cm_seqno_last_confirmed_);
	std::swap(cm_name_,                         other.cm_name_);
	std::swap(ft_active_goal_differs_,          other.ft_active_goal_differs_);
	std::swap(ft_activation_interval_differs_,  other.ft_activation_interval_differs_);
	std::swap(ft_heartbeat_interval_differs_,   other.ft_heartbeat_interval_differs_);
	std::swap(ft_other_member_prepares_before_, other.ft_other_member_prepares_before_);
	std::swap(ft_this_member_prepares_before_,  other.ft_this_member_prepares_before_);
	std::swap(ft_other_param_mismatch_,         other.ft_other_param_mismatch_);
	std::swap(adv_class_log_,                   other.adv_class_log_);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvMsg::Clear()
{
	initialized_from_message_        = false;
	ADV_CLASS_                       = NULL;
	ADV_SOURCE_                      = NULL;
	ADV_NAME_                        = NULL;
	ADV_DESC_                        = NULL;
	waiting_                         = 0;
	dropped_                         = 0;
	bytes_dropped_                   = 0;
	expiretime_                      = EmptyTibRvDateTimeField;
	license_expired_                 = false;
	dataloss_outbound_               = false;
	dataloss_ptp_                    = false;
	dataloss_host_                   = 0;
	dataloss_lost_                   = 0;
	tport_                           = NULL;
	descr_                           = NULL;
	dtexit_status_                   = 0;
	dtexit_description_              = NULL;
	vc_disc_error_                   = false;
	vc_disc_description_             = NULL;
	hostaddr_                        = 0;
	sn_                              = 0;
	sn_flag_                         = false;
	os_                              = 0;
	ver_                             = NULL;
	up_                              = 0;
	up_flag_                         = false;
	httpaddr_                        = 0;
	httpport_                        = 0;
	ms_                              = 0;
	bs_                              = 0;
	mr_                              = 0;
	br_                              = 0;
	ps_                              = 0;
	pr_                              = 0;
	rx_                              = 0;
	pm_                              = 0;
	hsaddr_                          = 0;
	hsport_                          = 0;
	hstatus_time_                    = EmptyTibRvDateTimeField;
	idl_                             = 0;
	odl_                             = 0;
	ipport_                          = 0;
	service_                         = NULL;
	network_                         = NULL;
	listen_stop_                     = false;
	listen_sub_                      = NULL;
	listen_id_                       = NULL;
	listen_refcnt_                   = 0;
	restart_host_                    = 0;
	cm_subject_                      = NULL;
	cm_listener_                     = NULL;
	cm_seqno_                        = 0;
	cm_seqno_begin_                  = 0;
	cm_seqno_end_                    = 0;
	cm_sender_                       = NULL;
	cm_seqno_last_sent_              = 0;
	cm_seqno_last_confirmed_         = 0;
	cm_name_                         = NULL;
	ft_active_goal_differs_          = false;
	ft_activation_interval_differs_  = false;
	ft_heartbeat_interval_differs_   = false;
	ft_other_member_prepares_before_ = false;
	ft_this_member_prepares_before_  = false;
	ft_other_param_mismatch_         = false;
	adv_class_log_                   = MLB::Utility::LogLevel_Notice;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::GetBasicAdv(const TibrvMsg &msg)
{
	try {
		Clear();
		GetRvField(msg, "ADV_CLASS",  ADV_CLASS_,  0, true,  true);
		GetRvField(msg, "ADV_SOURCE", ADV_SOURCE_, 0, true,  true);
		GetRvField(msg, "ADV_NAME",   ADV_NAME_,   0, true,  true);
		GetRvField(msg, "ADV_DESC",   ADV_DESC_,   0, false, true);
		CheckStringField("ADV_CLASS",  ADV_CLASS_);
		CheckStringField("ADV_SOURCE", ADV_SOURCE_);
		CheckStringField("ADV_NAME",   ADV_NAME_);
	}
	catch (const std::exception &except) {
		const char *tmp_ptr = NULL;
		try {
			tmp_ptr = GetSendSubjectPtr(msg);
		}
		catch (const std::exception &) {
		}
		MLB::Utility::Rethrow(except, "Unable to process the Tib/Rv advisory "
			"received on subject '" +
			std::string((tmp_ptr != NULL) ? "*NO SEND SUBJECT*" : tmp_ptr) +
			"': " + std::string(except.what()));
	}

	if (!strcmp(ADV_CLASS_, "INFO"))
		adv_class_log_ = MLB::Utility::LogLevel_Info;
	else if (!strcmp(ADV_CLASS_, "WARN"))
		adv_class_log_ = MLB::Utility::LogLevel_Warning;
	else if (!strcmp(ADV_CLASS_, "ERROR"))
		adv_class_log_ = MLB::Utility::LogLevel_Error;
	else
		adv_class_log_ = MLB::Utility::LogLevel_Notice;

	initialized_from_message_ = true;

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RvAdvMsg::CheckStringField(const char *field_name,
	const char *field_ptr) const
{
	if (field_ptr == NULL)
		throw RvException("Field '" + std::string(field_name) + "' is 'NULL'.");

	return(field_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvMsg::CheckAdvDesc() const
{
	if (ADV_DESC_ == NULL)
		throw RvException("Received a Tib/Rv advisory which did not contain an "
			"expected 'ADV_DESC' field.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvMsg::RethrowException(const std::exception &except,
	const char *advisory_name) const
{
	MLB::Utility::Rethrow(except, "Error occurred in processing Tib/Rv "
		"advisory message for " + std::string(advisory_name) + ": " +
		std::string(except.what()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::MsgToString(const TibrvMsg &msg, bool throw_if_no_match)
{
	std::string out_string;

	return(MsgToString(msg, out_string, throw_if_no_match));

}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::MsgToString(const TibrvMsg &msg,
	std::string &out_string, bool throw_if_no_match)
{
	const RvAdvFlagsMap *map_ptr = RvAdvMsg::MsgToMapPtr(msg, throw_if_no_match);

	return((map_ptr == NULL) ? out_string.assign(DefaultAdvToString(msg)) :
		MsgToString(map_ptr, msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::MsgToString(const RvAdvFlagsMap *map_ptr,
	const TibrvMsg &msg)
{
	std::string out_string;

	return(MsgToString(map_ptr, msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::MsgToString(const RvAdvFlagsMap *map_ptr,
	const TibrvMsg &msg, std::string &out_string)
{
	CheckIsValidMapPtr(map_ptr);

	return((*map_ptr->get_adv_text_func_)(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg RvAdvMsg::ConvertMsgToContents(const TibrvMsg &msg,
	bool throw_if_no_match)
{
	RvAdvMsg contents;

	return(ConvertMsgToContents(msg, contents, throw_if_no_match));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::ConvertMsgToContents(const TibrvMsg &msg,
	RvAdvMsg &contents, bool throw_if_no_match)
{
	const RvAdvFlagsMap *map_ptr = RvAdvMsg::MsgToMapPtr(msg, throw_if_no_match);

	if (map_ptr == NULL) {
		RvAdvMsg tmp_contents(msg);
		return(contents.swap(tmp_contents));
	}
	return(ConvertMsgToContents(map_ptr, msg, contents));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg RvAdvMsg::ConvertMsgToContents(const RvAdvFlagsMap *map_ptr,
	const TibrvMsg &msg)
{
	RvAdvMsg contents;

	return(ConvertMsgToContents(map_ptr, msg, contents));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::ConvertMsgToContents(const RvAdvFlagsMap *map_ptr,
	const TibrvMsg &msg, RvAdvMsg &contents)
{
	CheckIsValidMapPtr(map_ptr);

	return((*map_ptr->get_contents_func_)(msg, contents));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::ConvertMsgToString(const TibrvMsg &msg,
	bool throw_if_no_match)
{
	RvAdvMsg contents;

	return(contents.MsgToString(msg, throw_if_no_match));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::ConvertMsgToString(const TibrvMsg &msg,
	std::string &out_string, bool throw_if_no_match)
{
	RvAdvMsg contents;

	return(contents.MsgToString(msg, out_string, throw_if_no_match));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::ConvertMsgToString(const RvAdvFlagsMap *map_ptr,
	const TibrvMsg &msg)
{
	RvAdvMsg contents;

	return(contents.MsgToString(map_ptr, msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::ConvertMsgToString(const RvAdvFlagsMap *map_ptr,
	const TibrvMsg &msg, std::string &out_string)
{
	RvAdvMsg contents;

	return(contents.MsgToString(map_ptr, msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvFlagsMap *RvAdvMsg::SubjectToMapPtr(const char *subject,
	bool throw_if_no_match)
{
	if ((subject == NULL) || (!(*subject)))
		MLB::Utility::ThrowInvalidArgument("The specified subject name is " +
			std::string((subject == NULL) ? "'NULL'." : "empty."));

	//	CODE NOTE: Lookup by subject name
	const RvAdvFlagsMap *map_ptr = NULL;
	SubjectNameFlat      flat_subject(subject);

	if ((map_ptr = SubjectToMapPtrInternal(flat_subject,
		RvAdvisoryMapCount, RvAdvisoryMapList)) == NULL) {
		if ((map_ptr = SubjectToMapPtrInternal(flat_subject,
			CmAdvisoryMapCount, CmAdvisoryMapList)) == NULL)
			map_ptr = SubjectToMapPtrInternal(flat_subject,
				FtAdvisoryMapCount, FtAdvisoryMapList);
	}

	if ((map_ptr == NULL) && throw_if_no_match)
		throw RvException("Unable to resolve a message with subject '" +
			std::string(subject) + "' to an Tib/Rv advisory subject name "
			"pattern.");

	return(map_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvFlagsMap *RvAdvMsg::MsgToMapPtr(const TibrvMsg &msg,
	bool throw_if_no_match)
{

	return(SubjectToMapPtr(GetSendSubjectPtr(msg), throw_if_no_match));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvMsg::IsValidMapPtr(const RvAdvFlagsMap *map_ptr)
{
	return(IsValidMapPtr(map_ptr, RvAdvisoryMapCount, RvAdvisoryMapList) ||
      IsValidMapPtr(map_ptr, CmAdvisoryMapCount, CmAdvisoryMapList) ||
		IsValidMapPtr(map_ptr, FtAdvisoryMapCount, FtAdvisoryMapList));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvMsg::IsValidMapPtr(const RvAdvFlagsMap *map_ptr,
	unsigned int map_count, const RvAdvFlagsMap *map_list)
{
	return(((!map_count) || (map_ptr < map_list) ||
		(map_ptr > (map_list + (map_count - 1))) ||
		((reinterpret_cast<const char *>(map_ptr) -
		reinterpret_cast<const char *>(map_list)) %
		sizeof(*map_ptr))) ? false : true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvFlagsMap *RvAdvMsg::CheckIsValidMapPtr(const RvAdvFlagsMap *map_ptr)
{
	if (!IsValidMapPtr(map_ptr))
		MLB::Utility::ThrowInvalidArgument("The pointer " +
			MLB::Utility::ValueToStringHex(map_ptr) + " which purports to be a "
			"pointer to an element of a list of 'RvAdvFlagsMap' structures is "
			"not valid.");

	return(map_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::SlowConsumer(const TibrvMsg &msg)
{
	GetRvField(msg, "waiting",       waiting_,       0, false, true);
	GetRvField(msg, "dropped",       dropped_,       0, false, true);
	GetRvField(msg, "bytes_dropped", bytes_dropped_, 0, false, true);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FastProducer(const TibrvMsg &msg)
{
	GetRvField(msg, "waiting", waiting_, 0, false, true);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::NoMemory(const TibrvMsg &msg)
{
	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::LicenseExpire(const TibrvMsg &msg)
{
	GetRvField(msg, "expiretime", expiretime_, 0, false, true);

	license_expired_ = (adv_class_log_ == MLB::Utility::LogLevel_Error) ?
		true : false;

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::Dataloss(const TibrvMsg &msg)
{
	SubjectNameFlat flat_name(ADV_NAME_);

	if ((flat_name.GetElementCount() != 3) ||
		strcmp(flat_name.element_list_[0], "DATALOSS") ||
		(strcmp(flat_name.element_list_[1], "INBOUND") &&
		 strcmp(flat_name.element_list_[1], "OUTBOUND")) ||
		(strcmp(flat_name.element_list_[2], "BCAST") &&
		 strcmp(flat_name.element_list_[2], "PTP")))
		throw RvException("The advisory message 'ADV_NAME_' field contents "
			"do not conform to the pattern expected for Tib/RV dataloss "
			"messages " + std::string(ADV_NAME_));

	dataloss_outbound_ = (!strcmp(flat_name.element_list_[1], "INBOUND")) ?
		false : true;
	dataloss_ptp_      = (!strcmp(flat_name.element_list_[2], "BCAST"))   ?
		false : true;

	GetRvFieldIPAddr32(msg, "host", dataloss_host_, 0, true, false);
	GetRvField(msg,         "lost", dataloss_lost_, 0, true, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::RvdDisconnected(const TibrvMsg &msg)
{
	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::RvdConnected(const TibrvMsg &msg)
{
	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::UnreachableTransport(const TibrvMsg &msg)
{
	GetRvField(msg, "tport", tport_, 0, true,  true);
	GetRvField(msg, "descr", descr_, 0, false, true);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::QueueLimitExceeded(const TibrvMsg &msg)
{
	GetRvField(msg, "descr", descr_, 0, false, true);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::DispatcherThreadExited(const TibrvMsg &msg)
{
	GetRvField(msg, "status",      dtexit_status_, 0, false, true);
	GetRvField(msg, "description", dtexit_description_, 0, false, true);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::VcConnected(const TibrvMsg &msg)
{
	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::VcDisconnected(const TibrvMsg &msg)
{
	GetRvField(msg, "description", vc_disc_description_, 0, false, true);

	vc_disc_error_ = (adv_class_log_ == MLB::Utility::LogLevel_Error) ?
		true : false;

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::HostStart(const TibrvMsg &msg)
{
	try {
		GetRvFieldIPAddr32(msg,    "hostaddr", hostaddr_, 0, true,  false);
		sn_flag_ = GetRvField(msg, "sn",       sn_,       0, false, false);
		GetRvField(msg,            "os",       os_,       0, true,  false);
		GetRvField(msg,            "ver",      ver_,      0, true,  false);
		up_flag_ = GetRvField(msg, "up",       up_,       0, false, false);
		GetRvFieldIPAddr32(msg,    "httpaddr", httpaddr_, 0, true,  false);
		GetRvFieldIPPort16(msg,    "httpport", httpport_, 0, true,  false);
	}
	catch (const std::exception &except) {
		RethrowException(except, "host start message");
	}

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::HostStop(const TibrvMsg &msg)
{
	GetRvFieldIPAddr32(msg, "hostaddr", hostaddr_, 0, true,  false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::HostStatus(const TibrvMsg &msg)
{
/*
===============================================================================
[_RV.INFO.SYSTEM.HOST.STATUS.9F37F632]
[2007-12-14 11:38:32]:
	subject=_RV.INFO.SYSTEM.HOST.STATUS.9F37F632,
	message={
		{0}[8=TIBRVMSG_STRING]ADV_CLASS(0)="INFO",
		{0}[8=TIBRVMSG_STRING]ADV_SOURCE(0)="SYSTEM",
		{0}[8=TIBRVMSG_STRING]ADV_NAME(0)="HOST.STATUS.9F37F632",
		{0}[27=TIBRVMSG_IPADDR32]hostaddr(0)=159.55.246.50,
		{0}[19=TIBRVMSG_U32]sn(0)=222638,
		{0}[15=TIBRVMSG_U8]os(0)=2,
		{0}[8=TIBRVMSG_STRING]ver(0)="6.9.8",
		{0}[19=TIBRVMSG_U32]up(0)=159840,
		{0}[27=TIBRVMSG_IPADDR32]httpaddr(0)=159.55.246.50,
		{0}[26=TIBRVMSG_IPPORT16]httpport(0)=7580,
		{0}[21=TIBRVMSG_U64]ms(0)=14504,
		{0}[21=TIBRVMSG_U64]bs(0)=4594796,
		{0}[21=TIBRVMSG_U64]mr(0)=8506740,
		{0}[21=TIBRVMSG_U64]br(0)=2448756583,
		{0}[21=TIBRVMSG_U64]ps(0)=45766,
		{0}[21=TIBRVMSG_U64]pr(0)=10441784,
		{0}[21=TIBRVMSG_U64]rx(0)=0,
		{0}[21=TIBRVMSG_U64]pm(0)=1703
	}
===============================================================================

===============================================================================
[_RV.INFO.SYSTEM.HOST.STATUS.9F37F63A]
[2007-12-14 13:29:54]: subject=_RV.INFO.SYSTEM.HOST.STATUS.9F37F63A,
	message={
		{0}[8=TIBRVMSG_STRING]ADV_CLASS(0)="INFO",
		{0}[8=TIBRVMSG_STRING]ADV_SOURCE(0)="SYSTEM",
		{0}[8=TIBRVMSG_STRING]ADV_NAME(0)="HOST.STATUS.9F37F63A",
		{0}[27=TIBRVMSG_IPADDR32]hostaddr(0)=159.55.246.58,
		{0}[15=TIBRVMSG_U8]os(0)=2,
		{0}[8=TIBRVMSG_STRING]ver(0)="7.5.2",
		{0}[27=TIBRVMSG_IPADDR32]httpaddr(0)=159.55.246.58,
		{0}[26=TIBRVMSG_IPPORT16]httpport(0)=1092,
		{0}[3=TIBRVMSG_DATETIME]time(0)=2007-12-14 13:29:54.371000000,
		{0}[19=TIBRVMSG_U32]up(0)=599877,
		{0}[21=TIBRVMSG_U64]ms(0)=1198745,
		{0}[21=TIBRVMSG_U64]bs(0)=434530770,
		{0}[21=TIBRVMSG_U64]mr(0)=1110219,
		{0}[21=TIBRVMSG_U64]br(0)=221904954,
		{0}[21=TIBRVMSG_U64]ps(0)=1866690,
		{0}[21=TIBRVMSG_U64]pr(0)=2087492,
		{0}[21=TIBRVMSG_U64]rx(0)=5019,
		{0}[21=TIBRVMSG_U64]pm(0)=0,
		{0}[21=TIBRVMSG_U64]idl(0)=0,
		{0}[21=TIBRVMSG_U64]odl(0)=0,
		{0}[26=TIBRVMSG_IPPORT16]ipport(0)=7500,
		{0}[8=TIBRVMSG_STRING]service(0)="7500",
		{0}[8=TIBRVMSG_STRING]network(0)=""
	}
===============================================================================

===============================================================================
Version 7.5.3
-------------
[_RV.INFO.SYSTEM.HOST.STATUS.9F37F60B]
[2007-12-14 11:34:40]:
	subject=_RV.INFO.SYSTEM.HOST.STATUS.9F37F60B,
	message={
		{0}[8=TIBRVMSG_STRING]ADV_CLASS(0)="INFO",
		{0}[8=TIBRVMSG_STRING]ADV_SOURCE(0)="SYSTEM",
		{0}[8=TIBRVMSG_STRING]ADV_NAME(0)="HOST.STATUS.9F37F60B",
		{0}[27=TIBRVMSG_IPADDR32]hostaddr(0)=159.55.246.11,
		{0}[15=TIBRVMSG_U8]os(0)=2,
		{0}[8=TIBRVMSG_STRING]ver(0)="7.5.3",
		{0}[27=TIBRVMSG_IPADDR32]httpaddr(0)=159.55.246.11,
		{0}[26=TIBRVMSG_IPPORT16]httpport(0)=1086,
		{0}[3=TIBRVMSG_DATETIME]time(0)=2007-12-14 11:34:42.202000000,
		{0}[19=TIBRVMSG_U32]up(0)=1755824,
		{0}[21=TIBRVMSG_U64]ms(0)=2985580,
		{0}[21=TIBRVMSG_U64]bs(0)=763249855,
		{0}[21=TIBRVMSG_U64]mr(0)=3168567,
		{0}[21=TIBRVMSG_U64]br(0)=730846308,
		{0}[21=TIBRVMSG_U64]ps(0)=5094546,
		{0}[21=TIBRVMSG_U64]pr(0)=5774828,
		{0}[21=TIBRVMSG_U64]rx(0)=9265,
		{0}[21=TIBRVMSG_U64]pm(0)=0,
		{0}[21=TIBRVMSG_U64]idl(0)=0,
		{0}[21=TIBRVMSG_U64]odl(0)=0,
		{0}[26=TIBRVMSG_IPPORT16]ipport(0)=7500,
		{0}[8=TIBRVMSG_STRING]service(0)="7500",
		{0}[8=TIBRVMSG_STRING]network(0)=""
	}
===============================================================================
*/

	try {
		GetRvFieldIPAddr32(msg,    "hostaddr", hostaddr_,     0, true,   false);
		sn_flag_ = GetRvField(msg, "sn",       sn_,       0, false, false);
		GetRvField(msg,            "os",       os_,           0, true,   false);
		GetRvField(msg,            "ver",      ver_,          0, true,   false);
		up_flag_ = GetRvField(msg, "up",       up_,       0, false, false);
		GetRvFieldIPAddr32(msg,    "httpaddr", httpaddr_,     0, true,   false);
		GetRvFieldIPPort16(msg,    "httpport", httpport_,     0, true,   false);
		GetRvField(msg,            "ms",       ms_,           0, true,   false);
		GetRvField(msg,            "bs",       bs_,           0, true,   false);
		GetRvField(msg,            "mr",       mr_,           0, true,   false);
		GetRvField(msg,            "br",       br_,           0, true,   false);
		GetRvField(msg,            "ps",       ps_,           0, true,   false);
		GetRvField(msg,            "pr",       pr_,           0, true,   false);
		GetRvField(msg,            "rx",       rx_,           0, true,   false);
		GetRvField(msg,            "pm",       pm_,           0, true,   false);
		//	Following fields not in Tib/Rv 6.9.8...
		GetRvFieldIPAddr32(msg,    "hsaddr",   hsaddr_,       0, false,  false);
		GetRvFieldIPPort16(msg,    "hsport",   hsport_,       0, false,  false);
		GetRvField(msg,            "time",     hstatus_time_, 0, false,  false);
		GetRvField(msg,            "idl",      idl_,          0, false,  false);
		GetRvField(msg,            "odl",      odl_,          0, false,  false);
		GetRvFieldIPPort16(msg,    "ipport",   ipport_,       0, false,  false);
		GetRvField(msg,            "service",  service_,      0, false,  false);
		GetRvField(msg,            "network",  network_,      0, false,  false);
	}
	catch (const std::exception &except) {
		RethrowException(except, "host status message");
	}

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::ListenStartStop(const TibrvMsg &msg)
{
	try {
		SubjectNameFlat tmp_adv_name(ADV_NAME_);
		if (tmp_adv_name.GetElementCount() < 3)
			throw RvException("Invalid number of elements in the 'ADV_NAME' "
				"field (" +
				MLB::Utility::AnyToString(tmp_adv_name.GetElementCount()) +
				") --- expected at least three.");
		if (!strcmp(tmp_adv_name.element_list_[1], "START"))
			listen_stop_ = false;
		else if (!strcmp(tmp_adv_name.element_list_[1], "STOP"))
			listen_stop_ = true;
		else
			throw RvException("Invalid string in the second element of the "
				"'ADV_NAME' field ('" + std::string(tmp_adv_name.element_list_[1]) +
				"') expected either or 'START' or 'STOP'.");
		GetRvField(msg, "id",     listen_id_,     0, true, true);
		GetRvField(msg, "sub",    listen_sub_,    0, true, true);
		GetRvField(msg, "refcnt", listen_refcnt_, 0, true, true);
	}
	catch (const std::exception &except) {
		RethrowException(except, "listen start/stop message");
	}

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::Retransmission(const TibrvMsg &msg)
{
	try {
		if (!strcmp(ADV_NAME_, "RETRANSMISSION.INBOUND.EXPECTED"))
			dataloss_outbound_ = false;
		else if (!strcmp(ADV_NAME_, "RETRANSMISSION.OUTBOUND.SENT"))
			dataloss_outbound_ = true;
		else 
			throw RvException("Invalid string in the 'ADV_NAME' field '" + 
				std::string(ADV_NAME_) + "') expected either "
				"'RETRANSMISSION.INBOUND.EXPECTED' or "
				"'RETRANSMISSION.OUTBOUND.SENT'.");
		GetRvFieldIPAddr32(msg, "host", dataloss_host_, 0, true, false);
		GetRvField(msg,         "lost", dataloss_lost_, 0, true, false);

	}
	catch (const std::exception &except) {
		RethrowException(except, "retransmission informational message");
	}

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::DaemonRestarted(const TibrvMsg &msg)
{
	GetRvFieldIPAddr32(msg, "host", restart_host_, 0, true, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryConfirm(const TibrvMsg &msg)
{
/*
const char     **cm_listener_list_;

cm_listener_list_
	_RV.ERROR.RVCM.DELIVERY.FAILED.>
cm_subject_
	_RV.INFO.RVCM.DELIVERY.CONFIRM.>
	_RV.INFO.RVCM.DELIVERY.COMPLETE.>
	_RV.ERROR.RVCM.DELIVERY.FAILED.>
	_RV.ERROR.RVCM.DELIVERY.UNAVAILABLE.>
	_RV.INFO.RVCM.REGISTRATION.REQUEST.>
	_RV.INFO.RVCM.REGISTRATION.CERTIFIED.>
	_RV.WARN.RVCM.REGISTRATION.NOT_CERTIFIED.>
	_RV.WARN.RVCM.REGISTRATION.NO_RESPONSE.>
	_RV.INFO.RVCM.REGISTRATION.CLOSED.>
	_RV.INFO.RVCM.REGISTRATION.DISCOVERY.>
cm_listener_
	_RV.INFO.RVCM.DELIVERY.CONFIRM.>
	_RV.WARN.RVCM.DELIVERY.NO_RESPONSE.>
	_RV.INFO.RVCM.REGISTRATION.REQUEST.>
	_RV.INFO.RVCM.REGISTRATION.CLOSED.>
cm_seqno_
	_RV.INFO.RVCM.DELIVERY.CONFIRM.>
	_RV.INFO.RVCM.DELIVERY.COMPLETE.>
	_RV.ERROR.RVCM.DELIVERY.FAILED.>
cm_data_
	_RV.ERROR.RVCM.DELIVERY.FAILED.>
cm_seqno_begin_
	_RV.ERROR.RVCM.DELIVERY.UNAVAILABLE.>
	_RV.INFO.RVCM.REGISTRATION.CERTIFIED.>
cm_seqno_end_
	_RV.ERROR.RVCM.DELIVERY.UNAVAILABLE.>
cm_sender_
	_RV.ERROR.RVCM.DELIVERY.UNAVAILABLE.>
	_RV.INFO.RVCM.REGISTRATION.CERTIFIED.>
	_RV.WARN.RVCM.REGISTRATION.NOT_CERTIFIED.>
	_RV.WARN.RVCM.REGISTRATION.NO_RESPONSE.>
	_RV.INFO.RVCM.REGISTRATION.DISCOVERY.>
cm_seqno_last_sent_
	_RV.INFO.RVCM.REGISTRATION.CLOSED.>
cm_seqno_last_confirmed_
	_RV.INFO.RVCM.REGISTRATION.CLOSED.>
cm_name_
	_RV.INFO.RVCM.REGISTRATION.MOVED.>
	_RV.ERROR.RVCM.REGISTRATION.COLLISION.>
	_RV.INFO.RVCM.RELAY.CONNECTED.>
	_RV.INFO.RVCM.RELAY.DISCONNECTED.>
	_RV.INFO.RVCM.QUEUE.SCHEDULER.ACTIVE.>
	_RV.INFO.RVCM.QUEUE.SCHEDULER.INACTIVE.>
*/

	GetRvField(msg, "subject",    cm_subject_,     0, false, false);
	GetRvField(msg, "listener",   cm_listener_,    0, false, false);
	GetRvField(msg, "seqno",      cm_seqno_,       0, false, false);
	GetRvField(msg, "seqno_begin",cm_seqno_begin_, 0, false, false);
	GetRvField(msg, "seqno_end",  cm_seqno_end_,   0, false, false);
	GetRvField(msg, "sender",     cm_sender_,      0, false, false);
	GetRvField(msg, "seqno_last_sent",
											cm_seqno_last_sent_,      0, false, false);
	GetRvField(msg, "seqno_last_confirmed",
											cm_seqno_last_confirmed_, 0, false, false);
	GetRvField(msg, "name",       cm_name_,     0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryComplete(const TibrvMsg &msg)
{
	GetRvField(msg, "subject", cm_subject_, 0, false, false);
	GetRvField(msg, "seqno",   cm_seqno_,   0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryNoResponse(const TibrvMsg &msg)
{
	GetRvField(msg, "listener",   cm_listener_,  0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryFailed(const TibrvMsg &msg)
{
	GetRvField(msg, "subject",    cm_subject_,   0, false, false);
	GetRvField(msg, "seqno",      cm_seqno_,     0, false, false);

	GetRvField(msg, "listener",   cm_listener_,  0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryUnavailable(const TibrvMsg &msg)
{
	GetRvField(msg, "subject",     cm_subject_,     0, false, false);
	GetRvField(msg, "seqno_begin", cm_seqno_begin_, 0, false, false);
	GetRvField(msg, "seqno_end",   cm_seqno_end_,   0, false, false);
	GetRvField(msg, "sender",      cm_sender_,      0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationRequest(const TibrvMsg &msg)
{
	GetRvField(msg, "subject",    cm_subject_,   0, false, false);
	GetRvField(msg, "listener",   cm_listener_,  0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationCertified(const TibrvMsg &msg)
{
	GetRvField(msg, "subject",    cm_subject_,     0, false, false);
	GetRvField(msg, "sender",     cm_sender_,      0, false, false);
	GetRvField(msg, "seqno_begin",cm_seqno_begin_, 0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationNotCertified(const TibrvMsg &msg)
{
	GetRvField(msg, "subject",    cm_subject_,   0, false, false);
	GetRvField(msg, "sender",     cm_sender_,    0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationNoResponse(const TibrvMsg &msg)
{
	GetRvField(msg, "subject",    cm_subject_,   0, false, false);
	GetRvField(msg, "sender",     cm_sender_,    0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationClosed(const TibrvMsg &msg)
{
	GetRvField(msg, "subject",    cm_subject_,   0, false, false);
	GetRvField(msg, "listener",   cm_listener_,  0, false, false);
	GetRvField(msg, "seqno_last_sent",      cm_seqno_last_sent_,      0, false, false);
	GetRvField(msg, "seqno_last_confirmed", cm_seqno_last_confirmed_, 0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationDiscovery(const TibrvMsg &msg)
{
	GetRvField(msg, "subject",    cm_subject_,   0, false, false);
	GetRvField(msg, "sender",     cm_sender_,    0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationMoved(const TibrvMsg &msg)
{
	GetRvField(msg, "name",       cm_name_,   0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationCollision(const TibrvMsg &msg)
{
	GetRvField(msg, "name",       cm_name_,   0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRelayConnected(const TibrvMsg &msg)
{
	GetRvField(msg, "name",       cm_name_,   0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRelayDisconnected(const TibrvMsg &msg)
{
	GetRvField(msg, "name",       cm_name_,   0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmQueueSchedulerActive(const TibrvMsg &msg)
{
	GetRvField(msg, "name",       cm_name_,   0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmQueueSchedulerInactive(const TibrvMsg &msg)
{
	GetRvField(msg, "name",       cm_name_,   0, false, false);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FtParamMismatch(const TibrvMsg &msg)
{
	CheckAdvDesc();

	if (!strcmp(ADV_DESC_, "Active goal differs"))
		ft_active_goal_differs_ = true;
	else if (!strcmp(ADV_DESC_, "Activation interval differs"))
		ft_activation_interval_differs_ = true;
	else if (!strcmp(ADV_DESC_, "Heartbeat interval differs"))
		ft_heartbeat_interval_differs_ = true;
	else if (!strcmp(ADV_DESC_, "Other prepares before this member's heartbeat"))
		ft_other_member_prepares_before_ = true;
	else if (!strcmp(ADV_DESC_, "This member prepares before other's heartbeat"))
		ft_this_member_prepares_before_ = true;
	else
		ft_other_param_mismatch_ = true;

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FtDisablingMember(const TibrvMsg &msg)
{
	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FtTooManyActive(const TibrvMsg &msg)
{
	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FtTooFewActive(const TibrvMsg &msg)
{
	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvMsg::CmDeliveryFailed_GetListenerList(const TibrvMsg &msg,
	std::vector<std::string> &listener_list) const
{
	try {
		tibrv_u32 field_count;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg.getNumFields,
			(field_count));
		tibrv_u32                count_1;
		std::vector<std::string> tmp_listener_list;
		for (count_1 = 0; count_1 < field_count; count_1++) {
			RvMsgSnapshotContext rvmsg_snapshot_context(msg);
			TibrvMsgField        msg_field;
			RvUtilX_THROW_TIBRV_STATUS_IF(
				const_cast<TibrvMsg &>(msg).getFieldByIndex,
				(msg_field, count_1));
			if (!strcmp(msg_field.getName(), "listener")) {
				if (msg_field.getType() != TIBRVMSG_STRING)
					throw RvException("Attempt to locate field name 'listener' "
						"succeded, but the field type (" +
						MsgFieldTypeToDescription(msg_field.getType()) +
						") was not of the expected type (" +
						MsgFieldTypeToDescription(TIBRVMSG_STRING) + ").");
				else if (msg_field.getData().str)
					throw RvException("Attempt to locate field name 'listener' "
						"succeded, but 'TibrvMsgField::getData().str' returns "
						"'NULL'.");
				tmp_listener_list.push_back(msg_field.getData().str);
			}
		}
		if (tmp_listener_list.empty())
			throw RvException("No fields named 'listener' were located within "
				"message.");
		listener_list.swap(tmp_listener_list);
	}
	catch (const std::exception &except) {
		RethrowException(except, "certified message delivery failed message");
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvMsg::CmGetDataField(const TibrvMsg &msg,
	TibrvMsg &msg_data_field, bool field_is_required)
{
	return(GetRvField(msg, "data", msg_data_field, 0, field_is_required, true));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::SlowConsumer2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.SlowConsumer(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FastProducer2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.FastProducer(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::NoMemory2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.NoMemory(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::LicenseExpire2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.LicenseExpire(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::Dataloss2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.Dataloss(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::RvdDisconnected2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.RvdDisconnected(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::RvdConnected2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.RvdConnected(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::UnreachableTransport2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.UnreachableTransport(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::QueueLimitExceeded2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.QueueLimitExceeded(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::DispatcherThreadExited2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.DispatcherThreadExited(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::VcConnected2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.VcConnected(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::VcDisconnected2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.VcDisconnected(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::HostStart2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.HostStart(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::HostStop2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.HostStop(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::HostStatus2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.HostStatus(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::ListenStartStop2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.ListenStartStop(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::Retransmission2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.Retransmission(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::DaemonRestarted2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.DaemonRestarted(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryConfirm2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmDeliveryConfirm(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryComplete2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmDeliveryComplete(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryNoResponse2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmDeliveryNoResponse(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryFailed2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmDeliveryFailed(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmDeliveryUnavailable2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmDeliveryUnavailable(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationRequest2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRegistrationRequest(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationCertified2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRegistrationCertified(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationNotCertified2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRegistrationNotCertified(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationNoResponse2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRegistrationNoResponse(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationClosed2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRegistrationClosed(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationDiscovery2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRegistrationDiscovery(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationMoved2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRegistrationMoved(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRegistrationCollision2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRegistrationCollision(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRelayConnected2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRelayConnected(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmRelayDisconnected2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmRelayDisconnected(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmQueueSchedulerActive2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmQueueSchedulerActive(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::CmQueueSchedulerInactive2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.CmQueueSchedulerInactive(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FtParamMismatch2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.FtParamMismatch(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FtDisablingMember2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.FtDisablingMember(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FtTooManyActive2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.FtTooManyActive(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvMsg &RvAdvMsg::FtTooFewActive2(const TibrvMsg &msg,
	RvAdvMsg &contents)
{
	return(contents.FtTooFewActive(msg));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::SlowConsumerToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(SlowConsumerToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::FastProducerToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(FastProducerToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::NoMemoryToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(NoMemoryToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::LicenseExpireToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(LicenseExpireToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::DatalossToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(DatalossToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::RvdDisconnectedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(RvdDisconnectedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::RvdConnectedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(RvdConnectedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::UnreachableTransportToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(UnreachableTransportToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::QueueLimitExceededToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(QueueLimitExceededToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::DispatcherThreadExitedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(DispatcherThreadExitedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::VcConnectedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(VcConnectedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::VcDisconnectedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(VcDisconnectedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::HostStartToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(HostStartToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::HostStopToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(HostStopToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::HostStatusToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(HostStatusToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::ListenStartStopToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(ListenStartStopToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::RetransmissionToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(RetransmissionToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::DaemonRestartedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(DaemonRestartedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmDeliveryConfirmToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmDeliveryConfirmToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmDeliveryCompleteToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmDeliveryCompleteToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmDeliveryNoResponseToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmDeliveryNoResponseToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmDeliveryFailedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmDeliveryFailedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmDeliveryUnavailableToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmDeliveryUnavailableToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRegistrationRequestToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRegistrationRequestToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRegistrationCertifiedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRegistrationCertifiedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRegistrationNotCertifiedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRegistrationNotCertifiedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRegistrationNoResponseToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRegistrationNoResponseToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRegistrationClosedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRegistrationClosedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRegistrationDiscoveryToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRegistrationDiscoveryToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRegistrationMovedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRegistrationMovedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRegistrationCollisionToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRegistrationCollisionToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRelayConnectedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRelayConnectedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmRelayDisconnectedToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmRelayDisconnectedToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmQueueSchedulerActiveToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmQueueSchedulerActiveToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::CmQueueSchedulerInactiveToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(CmQueueSchedulerInactiveToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::FtParamMismatchToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(FtParamMismatchToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::FtDisablingMemberToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(FtDisablingMemberToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::FtTooManyActiveToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(FtTooManyActiveToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::FtTooFewActiveToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(FtTooFewActiveToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsg::DefaultAdvToString(const TibrvMsg &msg)
{
	std::string out_string;

	return(DefaultAdvToString2(msg, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::SlowConsumerToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.SlowConsumer(msg);

	std::ostringstream o_str;

	o_str
		<< "Slow consumption detected (messages waiting = " << contents.waiting_
		<< ", messages dropped = " << contents.dropped_ << ", bytes dropped = "
		<< contents.bytes_dropped_ << ")";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::FastProducerToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.FastProducer(msg);

	std::ostringstream o_str;

	o_str
		<< "Fast production detected (messages waiting = " << contents.waiting_
		<< ")";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::NoMemoryToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.NoMemory(msg);

	return(out_string.assign("The Tib/Rv library is unable to allocate memory"));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::LicenseExpireToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.LicenseExpire(msg);

	std::ostringstream o_str;

	o_str
		<< "Tib/Rv license "
		<< ((contents.license_expired_) ? "expired" : "will expire")
		<< " at " << FromTibDateTime(contents.expiretime_);

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::DatalossToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.Dataloss(msg);

	std::ostringstream o_str;

	o_str
		<< ((contents.dataloss_outbound_) ? "Out" : "In") << "-bound message "
			"data-loss was detected for a "
		<< ((contents.dataloss_ptp_) ? "point-to-point" : "broadcast")
		<< " protocol in communication with host "
		<< MLB_SockLibX_AddressIP_IPAddressToString(contents.dataloss_host_)
		<< " and a resulting loss of " << contents.dataloss_lost_ << " messages";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::RvdDisconnectedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.RvdDisconnected(msg);

	return(out_string.assign("The Tib/Rv daemon has disconnected"));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::RvdConnectedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.RvdConnected(msg);

	return(out_string.assign("The Tib/Rv daemon has (re-)connected"));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::UnreachableTransportToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.UnreachableTransport(msg);

	std::ostringstream o_str;

	o_str
		<< "Tib/Rv transport object (transport id = "
		<< MLB::Utility::NullToString(contents.tport_, "*UNKNOWN TRANSPORT ID*")
		<< ", transport description = "
		<< MLB::Utility::NullToString(contents.descr_,
			"*NO TRANSPORT DESCRIPTION*") << ") has terminated";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::QueueLimitExceededToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.QueueLimitExceeded(msg);

	std::ostringstream o_str;

	o_str
		<< "A Tib/Rv queue limit was exceeded on the queue named '"
		<< MLB::Utility::NullToString(contents.descr_, "*NO QUEUE DESCRIPTION*")
		<< "'";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::DispatcherThreadExitedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.DispatcherThreadExited(msg);

	std::ostringstream o_str;
	std::string        thread_name;

	SubjectNameFlat(contents.ADV_NAME_).ToStringFromElements(thread_name, 2);

	o_str
		<< "The Tib/Rv dispatcher thread named '"
		<< SubjectNameFlat(contents.ADV_NAME_).
			ToStringFromElements(thread_name, 2)
		<< "' has exited with status code = " << contents.dtexit_status_ << " ("
		<< MLB::Utility::NullToString(::tibrvStatus_GetText(
			static_cast<tibrv_status>(contents.dtexit_status_))) << "): "
		<< MLB::Utility::NullToString(contents.dtexit_description_);

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::VcConnectedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.VcConnected(msg);

	return(out_string.assign("Tib/Rv virtual circuit connected"));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::VcDisconnectedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.VcDisconnected(msg);

	std::ostringstream o_str;

	o_str
		<< "Tib/Rv virtual circuit disconnected "
		<< ((contents.vc_disc_error_) ? " in error" : "") << ": "
		<< MLB::Utility::NullToString(contents.vc_disc_description_);

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::HostStartToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.HostStart(msg);

	std::ostringstream o_str;

	o_str
		<< "Tib/Rv host stop message received for host "
		<< MLB_SockLibX_AddressIP_IPAddressToString(contents.hostaddr_)
		<< ", serial number ";

	if (contents.sn_flag_)
		o_str
			<< contents.sn_;
	else
		o_str
			<< "N/A";

	o_str
		<< " running Tib/Rv software "
		<< ((contents.os_ & 0x80) ? "rvrd" : "rvrd")
		<< " version " << MLB::Utility::NullToString(contents.ver_)
		<< ", uptime "
		<< ((contents.up_flag_) ?
			MLB::Utility::TimeT(contents.up_).ToStringInterval() : "N/A")
		<< " with HTTP access at http://"
		<< MLB_SockLibX_AddressIP_IPAddressToString(contents.httpaddr_) << ":"
		<< contents.httpport_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::HostStopToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.HostStop(msg);

	std::ostringstream o_str;

	o_str
		<< "Tib/Rv host stop message received for host "
		<< MLB_SockLibX_AddressIP_IPAddressToString(contents.hostaddr_);

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::HostStatusToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.HostStatus(msg);

	std::ostringstream o_str;

//	CODE NOTE: Indicate whether 7.5.X fields were in the message.
	o_str
		<< "Tib/Rv host status message received for host "
		<< MLB_SockLibX_AddressIP_IPAddressToString(contents.hostaddr_)
		<< ", serial number ";

	if (contents.sn_flag_)
		o_str
			<< contents.sn_;
	else
		o_str
			<<"N/A";

	o_str
		<< " running Tib/Rv software "
		<< ((contents.os_ & 0x80) ? "rvrd" : "rvrd")
		<< " version " << MLB::Utility::NullToString(contents.ver_)
		<< ", uptime "
		<< ((contents.up_flag_) ?
			MLB::Utility::TimeT(contents.up_).ToStringInterval() : "N/A")
		<< " with HTTP access at http://"
		<< MLB_SockLibX_AddressIP_IPAddressToString(contents.httpaddr_) << ":"
		<< contents.httpport_ << ":"
		<< " Messages sent="           << contents.ms_
		<< " Bytes sent="              << contents.bs_
		<< " Messages received="       << contents.mr_
		<< " Bytes received="          << contents.br_
		<< " Packets sent="            << contents.ps_
		<< " Packets received="        << contents.pr_
		<< " Retransmission requests=" << contents.rx_
		<< " Packets missed="          << contents.pm_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::ListenStartStopToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.ListenStartStop(msg);

	std::ostringstream o_str;

	o_str
		<< "Listen " << ((contents.listen_stop_) ? "stop" : "start")
		<< " message on subject '"
		<< MLB::Utility::NullToString(contents.listen_sub_)
		<< "' from transport "
		<< MLB::Utility::NullToString(contents.listen_id_)
		<< " with refcount = " << contents.listen_refcnt_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::RetransmissionToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.Retransmission(msg);

	std::ostringstream o_str;

	if (contents.dataloss_outbound_)
		o_str
			<< "Advisory message indicates that an out-bound retransmission has "
				"been sent to correspondent host "
			<< MLB_SockLibX_AddressIP_IPAddressToString(contents.dataloss_host_)
			<< " due to " << contents.dataloss_lost_ << " packets";
	else
		o_str
			<< "Advisory message indicates that an in-bound retransmission is "
				"expected from correspondent host "
			<< MLB_SockLibX_AddressIP_IPAddressToString(contents.dataloss_host_)
			<< " due to " << contents.dataloss_lost_ << " packets";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::DaemonRestartedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.DaemonRestarted(msg);

	std::ostringstream o_str;

	o_str
		<< "Tib/Rv daemon on host "
		<< MLB_SockLibX_AddressIP_IPAddressToString(contents.dataloss_host_)
		<< " has been restarted";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmDeliveryConfirmToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmDeliveryConfirm(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmDeliveryConfirm()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmDeliveryCompleteToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmDeliveryComplete(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmDeliveryComplete()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmDeliveryNoResponseToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmDeliveryNoResponse(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmDeliveryNoResponse()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmDeliveryFailedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmDeliveryFailed(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmDeliveryFailed()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmDeliveryUnavailableToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmDeliveryUnavailable(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmDeliveryUnavailable()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRegistrationRequestToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRegistrationRequest(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRegistrationRequest()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRegistrationCertifiedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRegistrationCertified(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRegistrationCertified()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRegistrationNotCertifiedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRegistrationNotCertified(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRegistrationNotCertified()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRegistrationNoResponseToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRegistrationNoResponse(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRegistrationNoResponse()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRegistrationClosedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRegistrationClosed(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRegistrationClosed()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRegistrationDiscoveryToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRegistrationDiscovery(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRegistrationDiscovery()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRegistrationMovedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRegistrationMoved(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRegistrationMoved()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRegistrationCollisionToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRegistrationCollision(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRegistrationCollision()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRelayConnectedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRelayConnected(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRelayConnected()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmRelayDisconnectedToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmRelayDisconnected(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmRelayDisconnected()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmQueueSchedulerActiveToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmQueueSchedulerActive(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmQueueSchedulerActive()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::CmQueueSchedulerInactiveToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.CmQueueSchedulerInactive(msg);

	std::ostringstream o_str;

	//	CODE NOTE: To be implemented.
	o_str
		<< "Certified message advisory in CmQueueSchedulerInactive()";

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::FtParamMismatchToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.FtParamMismatch(msg);

	contents.CheckAdvDesc();

	std::ostringstream o_str;

	o_str
		<< "Fault tolerance parameter mismatch: " << contents.ADV_DESC_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::FtDisablingMemberToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.FtDisablingMember(msg);

	contents.CheckAdvDesc();

	std::ostringstream o_str;

	o_str
		<< "Fault tolerance library is disabling this member: "
		<< contents.ADV_DESC_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::FtTooManyActiveToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.FtTooManyActive(msg);

	contents.CheckAdvDesc();

	std::ostringstream o_str;

	o_str
		<< "Too many fault tolerance active members: " << contents.ADV_DESC_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::FtTooFewActiveToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	RvAdvMsg contents(msg);

	contents.FtTooFewActive(msg);

	contents.CheckAdvDesc();

	std::ostringstream o_str;

	o_str
		<< "Too few fault tolerance active members: " << contents.ADV_DESC_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsg::DefaultAdvToString2(const TibrvMsg &msg,
	std::string &out_string)
{
	const char *subject_ptr(GetSendSubjectPtr(msg));

	if (subject_ptr == NULL)
		throw RvException("The send subject name of a Tib/Rv message received "
			"in a TibrvMsgCallback is 'NULL'.");

	return(out_string.assign("Tib/Rv advisory message received on subject "
		"name '" + std::string(subject_ptr) + "'"));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvFlagsMapList RvAdvMsg::GetFlagsMapRv()
{
	return(RvAdvFlagsMapList(RvAdvisoryMapCount, RvAdvisoryMapList));
}
//	////////////////////////////////////////////////////////////////////////////

//	x////////////////////////////////////////////////////////////////////////////
RvAdvFlagsMapList RvAdvMsg::GetFlagsMapCm()
{
	return(RvAdvFlagsMapList(CmAdvisoryMapCount, CmAdvisoryMapList));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvFlagsMapList RvAdvMsg::GetFlagsMapFt()
{
	return(RvAdvFlagsMapList(FtAdvisoryMapCount, FtAdvisoryMapList));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvFlagsMap *RvAdvMsg::GetFlagsMapPtr(RvAdvFlagType advisory_type,
	unsigned int advisory_flag)
{
	if (advisory_type == RvAdvFlagType_Rv)
		return(GetFlagsMapPtr(RvAdvisoryMapCount, RvAdvisoryMapList,
			advisory_flag));
	else if (advisory_type == RvAdvFlagType_Cm)
		return(GetFlagsMapPtr(CmAdvisoryMapCount, CmAdvisoryMapList,
			advisory_flag));
	else if (advisory_type == RvAdvFlagType_Ft)
		return(GetFlagsMapPtr(FtAdvisoryMapCount, FtAdvisoryMapList,
			advisory_flag));
	else
		return(NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvFlagsMap *RvAdvMsg::GetFlagsMapPtr(unsigned int map_count,
	const RvAdvFlagsMap *map_list, unsigned int advisory_flag)
{
	while (map_count--) {
		if (map_list[map_count].advisory_flag_ == advisory_flag)
			return(map_list + map_count);
	}

	return(NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvMsg::CreateEventInfoSet(RvAdvEventInfoSet &event_set)
{
	RvAdvEventInfoSet tmp_event_set;

	AppendEventInfoSet(RvAdvisoryMapCount, RvAdvisoryMapList, tmp_event_set);
	AppendEventInfoSet(CmAdvisoryMapCount, CmAdvisoryMapList, tmp_event_set);
	AppendEventInfoSet(FtAdvisoryMapCount, FtAdvisoryMapList, tmp_event_set);

	event_set.swap(tmp_event_set);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvEventInfo *RvAdvMsg::LocateEventInfoInSet(const TibrvMsg &msg,
	RvAdvEventInfoSet &event_set, bool throw_if_no_match)
{
	return(LocateEventInfoInSet(GetSendSubjectPtr(msg), event_set,
		throw_if_no_match));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvEventInfo *RvAdvMsg::LocateEventInfoInSet(const char *subject,
	RvAdvEventInfoSet &event_set, bool throw_if_no_match)
{
	return(LocateEventInfoInSet(SubjectToMapPtr(subject, throw_if_no_match),
		event_set, throw_if_no_match));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvEventInfo *RvAdvMsg::LocateEventInfoInSet(const RvAdvFlagsMap *map_ptr,
	RvAdvEventInfoSet &event_set, bool throw_if_no_match)
{
	RvAdvEventInfo *return_ptr = NULL;

	if (map_ptr != NULL) {
		RvAdvEventInfoSetIter iter_f(event_set.find(
			RvAdvEventInfo(map_ptr->advisory_type_, map_ptr->advisory_flag_)));
		if (iter_f != event_set.end())
			return_ptr = &(*iter_f);
	}

	if ((return_ptr == NULL) && throw_if_no_match)
		throw RvException("Unable to resolve a Tib/Rv advisory message map "
			"pointer.");

	return(return_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvEventInfo *RvAdvMsg::UpdateEventInfoSet(const TibrvMsg &msg,
	RvAdvEventInfoSet &event_set, const MLB::Utility::TimeSpec &event_time,
	bool throw_if_no_match)
{
	return(UpdateEventInfoSet(MsgToMapPtr(msg, throw_if_no_match),
		event_set, event_time, throw_if_no_match));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvEventInfo *RvAdvMsg::UpdateEventInfoSet(const char *subject,
	RvAdvEventInfoSet &event_set, const MLB::Utility::TimeSpec &event_time,
	bool throw_if_no_match)
{
	return(UpdateEventInfoSet(SubjectToMapPtr(subject, throw_if_no_match),
		event_set, event_time, throw_if_no_match));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvEventInfo *RvAdvMsg::UpdateEventInfoSet(const RvAdvFlagsMap *map_ptr,
	RvAdvEventInfoSet &event_set, const MLB::Utility::TimeSpec &event_time,
	bool throw_if_no_match)
{
	RvAdvEventInfo *event_ptr =
		LocateEventInfoInSet(map_ptr, event_set, throw_if_no_match);

	if (event_ptr != NULL)
		event_ptr->UpdateEventInfo(event_time);

	return(event_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RvAdvFlagsMap *RvAdvMsg::SubjectToMapPtrInternal(
	const SubjectNameFlat subject, unsigned int map_count,
	const RvAdvFlagsMap *map_list)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < map_count; ++count_1) {
		if (map_list[count_1].advisory_subject_flat_.MatchSubjectName(subject))
			return(map_list + count_1);
	}

	return(NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvAdvMsg::AppendEventInfoSet(unsigned int map_count,
	const RvAdvFlagsMap *map_list, RvAdvEventInfoSet &event_set)
{
	while (map_count--)
		event_set.insert(RvAdvEventInfo(map_list[map_count].advisory_type_,
			map_list[map_count].advisory_flag_,
			map_list[map_count].advisory_subject_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void StringToRvAdvMsgFlags(const std::string &in_string,
	RvAdvFlags &rv_flags, CmAdvFlags &cm_flags, FtAdvFlags &ft_flags)
{
	try {
		RvAdvFlags tmp_rv_flags = Rv_None;
		CmAdvFlags tmp_cm_flags = Cm_None;
		FtAdvFlags tmp_ft_flags = Ft_None;
		if (!in_string.empty()) {
			std::vector<std::string> tmp_list;
			MLB::Utility::SplitString(in_string, "|", tmp_list, 0, false);
			std::vector<std::string>::const_iterator iter_b(tmp_list.begin());
			std::vector<std::string>::const_iterator iter_e(tmp_list.end());
			while (iter_b != iter_e) {
				std::string this_element(MLB::Utility::Trim(*iter_b));
				if (this_element.empty())
					MLB::Utility::ThrowInvalidArgument("Empty Tib/Rv advisory flag "
						"'OR' section encountered.");
				if (!StringToRvAdvMsgFlagsAppend(*iter_b,
					RvAdvMsgFlagStringExtCount, RvAdvMsgFlagStringExtList,
					tmp_rv_flags, tmp_cm_flags, tmp_ft_flags)) {
					if (!StringToRvAdvMsgFlagsAppend(*iter_b,
						RvAdvMsgFlagStringCount, RvAdvMsgFlagStringList,
						tmp_rv_flags, tmp_cm_flags, tmp_ft_flags))
						MLB::Utility::ThrowInvalidArgument("Invalid Tib/Rv advisory "
							"flag section encountered ('" + *iter_b + "').");
				}
				++iter_b;
			}
		}
		rv_flags = tmp_rv_flags;
		cm_flags = tmp_cm_flags;
		ft_flags = tmp_ft_flags;
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to parse the Tib/Rv advisory "
			"message flags failed: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvAdvFlagsAll StringToRvAdvMsgFlags(const std::string &in_string)
{
	RvAdvFlagsAll out_flags;

	StringToRvAdvMsgFlags(in_string, out_flags.rv_flags_, out_flags.cm_flags_,
		out_flags.ft_flags_);

	return(out_flags);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsgFlagsToStringAppend(RvAdvFlagType advisory_type,
	unsigned int advisory_flag, std::string &out_string)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < RvAdvMsgFlagStringCount; ++count_1) {
		if ((advisory_type == RvAdvMsgFlagStringList[count_1].advisory_type_) &&
			 (advisory_flag  & RvAdvMsgFlagStringList[count_1].advisory_flag_)) {
			if (!out_string.empty())
				out_string += "|";
			out_string    +=  RvAdvMsgFlagStringList[count_1].advisory_string_;
			//	Don't display following synonyms...
			advisory_flag &= ~RvAdvMsgFlagStringList[count_1].advisory_flag_;
		}
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsgFlagsToStringAppend(RvAdvFlagType advisory_type,
	unsigned int advisory_flag)
{
	std::string out_string;

	return(RvAdvMsgFlagsToStringAppend(advisory_type, advisory_flag,
		out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsgFlagsToString(RvAdvFlagType advisory_type,
	unsigned int advisory_flag, std::string &out_string)
{
	std::string tmp_string;

	RvAdvMsgFlagsToStringAppend(advisory_type, advisory_flag, tmp_string);

	out_string.swap(tmp_string);

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsgFlagsToString(RvAdvFlagType advisory_type,
	unsigned int advisory_flag)
{
	std::string out_string;

	return(RvAdvMsgFlagsToString(advisory_type, advisory_flag, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsgFlagsToString(RvAdvFlags rv_flags,
	CmAdvFlags cm_flags, FtAdvFlags ft_flags, std::string &out_string)
{
	std::string tmp_string;

	RvAdvMsgFlagsToStringAppend(RvAdvFlagType_Rv, rv_flags, tmp_string);
	RvAdvMsgFlagsToStringAppend(RvAdvFlagType_Cm, cm_flags, tmp_string);
	RvAdvMsgFlagsToStringAppend(RvAdvFlagType_Ft, ft_flags, tmp_string);

	out_string.swap(tmp_string);

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsgFlagsToString(RvAdvFlags rv_flags, CmAdvFlags cm_flags,
	FtAdvFlags ft_flags)
{
	std::string out_string;

	return(RvAdvMsgFlagsToString(rv_flags, cm_flags, ft_flags, out_string));

}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvAdvMsgFlagsToString(const RvAdvFlagsAll &flags,
	std::string &out_string)
{
	return(RvAdvMsgFlagsToString(flags.rv_flags_, flags.cm_flags_,
		flags.ft_flags_, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvAdvMsgFlagsToString(const RvAdvFlagsAll &flags)
{
	std::string out_string;

	return(RvAdvMsgFlagsToString(flags, out_string));

}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GetRvAdvMsgFlagsHelpTextBrief(const std::string &param_name)
{
	std::ostringstream o_str;

	EmitRvAdvMsgFlagsHelpTextBrief(o_str, param_name);

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GetRvAdvMsgFlagsHelpTextExtended(const std::string &param_name,
	RvAdvFlags rv_flags, CmAdvFlags cm_flags, FtAdvFlags ft_flags)
{
	std::ostringstream o_str;

	EmitRvAdvMsgFlagsHelpTextExtended(o_str, param_name, rv_flags, cm_flags,
		ft_flags);

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream &EmitRvAdvMsgFlagsHelpTextBrief(std::ostream &o_str,
	const std::string &param_name)
{
	o_str
		<< param_name << " <advisory-flag> [ | <advisory-flag> ... ]";

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream &EmitRvAdvMsgFlagsHelpTextExtended(std::ostream &o_str,
	const std::string &param_name, RvAdvFlags rv_flags, CmAdvFlags cm_flags,
	FtAdvFlags ft_flags)
{
	o_str
		<< "   " << param_name << "\n\
      Specifies the Tib/Rv advisory message flags to be used. Multiple flags\n\
      may be separated with the vertical bar character ('|'). Valid flags are:"
		"\n";

	RvAdvMsgFlagsHelpTextAppend(RvAdvFlagType_Rv, rv_flags, o_str);
	RvAdvMsgFlagsHelpTextAppend(RvAdvFlagType_Cm, cm_flags, o_str);
	RvAdvMsgFlagsHelpTextAppend(RvAdvFlagType_Ft, ft_flags, o_str);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvMsgParseCmdLineArg::ParseCmdLineAdvMsgFlags(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, RvAdvFlags &rv_flags, CmAdvFlags &cm_flags,
	FtAdvFlags &ft_flags)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, tmp_string))
		return(false);

	try {
		StringToRvAdvMsgFlags(tmp_string, rv_flags, cm_flags, ft_flags);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "The specified value for parameter '" +
			std::string(argv[current_index - 1]) + "' is in error: " +
			std::string(except.what()));
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvMsgParseCmdLineArg::ParseCmdLineAdvMsgFlags(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, RvAdvFlagsAll &out_datum)
{
	return(ParseCmdLineAdvMsgFlags(param_name, current_index, argc, argv,
		out_datum.rv_flags_, out_datum.cm_flags_, out_datum.ft_flags_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvMsgParseCmdLineArg::ParseCmdLineAdvMsgFlagsSpecDefault(
	unsigned int &current_index, int argc, char **argv, RvAdvFlags &rv_flags,
	CmAdvFlags &cm_flags, FtAdvFlags &ft_flags)
{
	return(ParseCmdLineAdvMsgFlagsSpec(
		MLB::Utility::MakeInlineVector<std::string>
			("-RV_ADVISORY_FLAGS")
			("-RV_ADVISORYFLAGS")
			("-RVADVISORY_FLAGS")
			("-RVADVISORYFLAGS")
			("-RV_ADVISORY_FLAG")
			("-RV_ADVISORYFLAG")
			("-RVADVISORY_FLAG")
			("-RVADVISORYFLAG")
			("-RV_ADV_FLAGS")
			("-RV_ADVFLAGS")
			("-RVADV_FLAGS")
			("-RVADVFLAGS")
			("-RV_ADV_FLAG")
			("-RV_ADVFLAG")
			("-RVADV_FLAG")
			("-RVADVFLAG")
			("-ADVISORY_FLAGS")
			("-ADVISORYFLAGS")
			("-ADVISORY_FLAG")
			("-ADVISORYFLAG")
			("-ADV_FLAGS")
			("-ADVFLAGS")
			("-ADV_FLAG")
			("-ADVFLAG"),
			current_index, argc, argv, rv_flags, cm_flags, ft_flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvAdvMsgParseCmdLineArg::ParseCmdLineAdvMsgFlagsSpecDefault(
	unsigned int &current_index, int argc, char **argv, RvAdvFlagsAll &out_datum)
{
	return(ParseCmdLineAdvMsgFlagsSpecDefault(current_index, argc, argv,
		out_datum.rv_flags_, out_datum.cm_flags_, out_datum.ft_flags_));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#ifdef TEST_MAIN

#include <RvUtilX/AddRvField.hpp>
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_ptr.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

using namespace MLB::RvUtilX;

namespace {
//	////////////////////////////////////////////////////////////////////////////
class TEST_CallbackMsgEvent : public TibrvMsgCallback {
public:
	TEST_CallbackMsgEvent()
	{
	}
	virtual ~TEST_CallbackMsgEvent()
	{
	}

	virtual void onMsg(TibrvListener *, TibrvMsg &msg_recv) {
		try {
			std::cout << MLB::Utility::TimeSpec() << " Tib/Rv Advisory: " <<
				RvAdvMsg::ConvertMsgToString(msg_recv,  true) << std::endl;
		}
		catch (const std::exception &except) {
			std::cout << MLB::Utility::TimeSpec() << " ERROR          : " <<
				except.what() << std::endl;
		}
	}

private:
	MLB_Utility_NonCopyable_Macro(TEST_CallbackMsgEvent);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TEST_CallbackMsgEvent MyCallBack;
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
void TEST_DispatchEvents()
{
	TibrvStatus rv_status;

	rv_status = GetDefaultQueue()->timedDispatch(0.1);

	if (MLB::Utility::CriticalEventTest())
		throw MLB::Utility::ExceptionCriticalEvent("Performing an asynchronous "
			"exit due to receipt of a signal.");

	if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
		throw RvExceptionStatus(rv_status,
			"Error occurred while dispatching from a Rendezvous queue.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_DispatchEventsLoop(unsigned int seconds_to_dispatch)
{
	std::cout << "Will dispatch events for " << seconds_to_dispatch <<
		" seconds..." << std::endl;

	MLB::Utility::TimeT end_time;

	end_time.AddSeconds(seconds_to_dispatch);

	while (MLB::Utility::TimeT() < end_time) {
		if (MLB::Utility::CriticalEventTest())
			throw MLB::Utility::ExceptionCriticalEvent();
		::Sleep(10);
		TEST_DispatchEvents();
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<TibrvTransport> TransportSPtr;
typedef boost::shared_ptr<TibrvListener>  ListenerSPtr;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_SubscribeToAdvisoriesType(unsigned int flags,
	const RvAdvFlagsMapList &map_list, TibrvTransport *transport_ptr,
	std::vector<ListenerSPtr> &listener_list)
{
	if (flags) {
		unsigned int count_1;
		for (count_1 = 0; count_1 < map_list.first; ++count_1) {
			if (flags & map_list.second[count_1].advisory_flag_) {
				ListenerSPtr listener_ptr(new TibrvListener);
				CreateListener(*GetDefaultQueue(), MyCallBack, *transport_ptr,
					*listener_ptr, map_list.second[count_1].advisory_subject_,
					map_list.second[count_1].get_adv_text_func_);
				listener_list.push_back(listener_ptr);
			}
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_SubscribeToAdvisories(RvAdvFlags rv_flags, CmAdvFlags cm_flags,
	FtAdvFlags ft_flags, TibrvTransport *transport_ptr,
	std::vector<ListenerSPtr> &listener_list)
{
	TEST_SubscribeToAdvisoriesType(rv_flags, RvAdvMsg::GetFlagsMapRv(),
		transport_ptr, listener_list);
	TEST_SubscribeToAdvisoriesType(cm_flags, RvAdvMsg::GetFlagsMapCm(),
		transport_ptr, listener_list);
	TEST_SubscribeToAdvisoriesType(ft_flags, RvAdvMsg::GetFlagsMapFt(),
		transport_ptr, listener_list);
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

	size_t                    count_1;
	std::vector<ListenerSPtr> listener_list;

	for (count_1 = 0; count_1 < transport_list.size(); ++count_1)
		TEST_SubscribeToAdvisories(adv_flags.rv_flags_, adv_flags.cm_flags_,
			adv_flags.ft_flags_, transport_list[count_1].get(), listener_list);

	TEST_SubscribeToAdvisories(adv_flags.rv_flags_, adv_flags.cm_flags_,
			adv_flags.ft_flags_, GetProcessTransportPtr(), listener_list);

	TEST_DispatchEventsLoop(120);

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "Comprehensive Advisories Test Complete"  << std::endl;
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
			TEST_RvAdvisories(transport_list, adv_flags);
		}

	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

