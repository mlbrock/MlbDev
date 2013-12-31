// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for the RvAdvFlag class.

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

#include <Utility/EnumFlags.hpp>

//#include <VorParseCmdLineArg.h>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

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
typedef MLB::Utility::EnumFlags<RvAdvFlags> RvAdvFlagsEnumInfo;
typedef MLB::Utility::EnumFlags<CmAdvFlags> CmAdvFlagsEnumInfo;
typedef MLB::Utility::EnumFlags<FtAdvFlags> FtAdvFlagsEnumInfo;
//	////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::EnumFlagDatum<RvAdvFlags> RvEnumFlagList[] = {
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_None,                     "None"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_SlowConsumer,             "SlowConsumer"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_FastProducer,             "FastProducer"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_NoMemory,                 "NoMemory"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_LicenseExpire_Warn,       "LicenseExpire_Warn"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_LicenseExpire_Error,      "LicenseExpire_Error"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Dataloss_In_Ptp,          "Dataloss_In_Ptp"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Dataloss_In_BCast,        "Dataloss_In_BCast"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Dataloss_Out_Ptp,         "Dataloss_Out_Ptp"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Dataloss_Out_BCast,       "Dataloss_Out_BCast"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Rvd_Disconnected,         "Rvd_Disconnected"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Rvd_Connected,            "Rvd_Connected"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_UnreachableTransport,     "UnreachableTransport"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_QueueLimitExceeded,       "QueueLimitExceeded"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_DispatcherThreadExited,   "DispatcherThreadExited"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Vc_Connected,             "Vc_Connected"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Vc_Disconnected_Info,     "Vc_Disconnected_Info"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_Vc_Disconnected_Error,    "Vc_Disconnected_Error"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_HostStart,                "HostStart"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_HostStop,                 "HostStop"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_HostStatus,               "HostStatus"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_ListenStart,              "ListenStart"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_ListenStop,               "ListenStop"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_RetransmissionIn,         "RetransmissionIn"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_RetransmissionOut,        "RetransmissionOut"),
	MLB_UTILITY_ENUM_FLAG_DATUM(RvAdvFlags, Rv_DaemonRestarted,          "DaemonRestarted")
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
RvAdvFlagsEnumInfo RvAdvFlagsEnum("Rv advisory flag",
	sizeof(RvEnumFlagList) / sizeof(RvEnumFlagList[0]),
	RvEnumFlagList, "", "Mask");
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::EnumFlagDatum<CmAdvFlags> CmEnumFlagList[] = {
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_None,                     "None"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_DeliveryConfirm,          "DeliveryConfirm"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_DeliveryComplete,         "DeliveryComplete"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_DeliveryNoResponse,       "DeliveryNoResponse"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_DeliveryFailed,           "DeliveryFailed"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_DeliveryUnavailable,      "DeliveryUnavailable"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RegistrationRequest,      "RegistrationRequest"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RegistrationCertified,    "RegistrationCertified"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RegistrationNotCertified, "RegistrationNotCertified"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RegistrationNoResponse,   "RegistrationNoResponse"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RegistrationClosed,       "RegistrationClosed"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RegistrationDiscovery,    "RegistrationDiscovery"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RegistrationMoved,        "RegistrationMoved"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RegistrationCollision,    "RegistrationCollision"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RelayConnected,           "RelayConnected"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_RelayDisconnected,        "RelayDisconnected"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_QueueSchedulerActive,     "QueueSchedulerActive"),
	MLB_UTILITY_ENUM_FLAG_DATUM(CmAdvFlags, Cm_QueueSchedulerInactive,   "QueueSchedulerInactive")
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmAdvFlagsEnumInfo CmAdvFlagsEnum("Cm advisory flag",
	sizeof(CmEnumFlagList) / sizeof(CmEnumFlagList[0]),
	CmEnumFlagList, "", "Mask");
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::EnumFlagDatum<FtAdvFlags> FtEnumFlagList[] = {
	MLB_UTILITY_ENUM_FLAG_DATUM(FtAdvFlags, Ft_None,                     "None"),
	MLB_UTILITY_ENUM_FLAG_DATUM(FtAdvFlags, Ft_ParamMismatch_Warn,       "ParamMismatch_Warn"),
	MLB_UTILITY_ENUM_FLAG_DATUM(FtAdvFlags, Ft_ParamMismatch_Error,      "ParamMismatch_Error"),
	MLB_UTILITY_ENUM_FLAG_DATUM(FtAdvFlags, Ft_DisablingMember,          "DisablingMember"),
	MLB_UTILITY_ENUM_FLAG_DATUM(FtAdvFlags, Ft_TooManyActive,            "TooManyActive"),
	MLB_UTILITY_ENUM_FLAG_DATUM(FtAdvFlags, Ft_TooFewActive,             "TooFewActive")
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FtAdvFlagsEnumInfo FtAdvFlagsEnum("Ft advisory flag",
	sizeof(FtEnumFlagList) / sizeof(FtEnumFlagList[0]),
	FtEnumFlagList, "", "Mask");
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const RvAdvFlagsEnumInfo &GetRvAdvFlagsEnum()
{
	return(RvAdvFlagsEnum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmAdvFlagsEnumInfo &GetCmAdvFlagsEnum()
{
	return(CmAdvFlagsEnum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const FtAdvFlagsEnumInfo &GetFtAdvFlagsEnum()
{
	return(FtAdvFlagsEnum);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
struct TEST_Datum {
	unsigned int flag_evalue_;
	std::string  str_evalue_;
	std::string  alt_str_evalue_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TEST_Datum   TEST_RvStatusSingleList[] = {
	{	Rv_None,                     "None",                     "None"	},
	{	Rv_SlowConsumer,             "SlowConsumer",             "SlowConsumer"	},
	{	Rv_FastProducer,             "FastProducer",             "FastProducer"	},
	{	Rv_NoMemory,                 "NoMemory",                 "NoMemory"	},
	{	Rv_LicenseExpire_Warn,       "LicenseExpire_Warn",       "LicenseExpire_Warn"	},
	{	Rv_LicenseExpire_Error,      "LicenseExpire_Error",      "LicenseExpire_Error"	},
	{	Rv_Dataloss_In_Ptp,          "Dataloss_In_Ptp",          "Dataloss_In_Ptp"	},
	{	Rv_Dataloss_In_BCast,        "Dataloss_In_BCast",        "Dataloss_In_BCast"	},
	{	Rv_Dataloss_Out_Ptp,         "Dataloss_Out_Ptp",         "Dataloss_Out_Ptp"	},
	{	Rv_Dataloss_Out_BCast,       "Dataloss_Out_BCast",       "Dataloss_Out_BCast"	},
	{	Rv_Rvd_Disconnected,         "Rvd_Disconnected",         "Rvd_Disconnected"	},
	{	Rv_Rvd_Connected,            "Rvd_Connected",            "Rvd_Connected"	},
	{	Rv_UnreachableTransport,     "UnreachableTransport",     "UnreachableTransport"	},
	{	Rv_QueueLimitExceeded,       "QueueLimitExceeded",       "QueueLimitExceeded"	},
	{	Rv_DispatcherThreadExited,   "DispatcherThreadExited",   "DispatcherThreadExited"	},
	{	Rv_Vc_Connected,             "Vc_Connected",             "Vc_Connected"	},
	{	Rv_Vc_Disconnected_Info,     "Vc_Disconnected_Info",     "Vc_Disconnected_Info"	},
	{	Rv_Vc_Disconnected_Error,    "Vc_Disconnected_Error",    "Vc_Disconnected_Error"	},
	{	Rv_HostStart,                "HostStart",                "HostStart"	},
	{	Rv_HostStop,                 "HostStop",                 "HostStop"	},
	{	Rv_HostStatus,               "HostStatus",               "HostStatus"	},
	{	Rv_ListenStart,              "ListenStart",              "ListenStart"	},
	{	Rv_ListenStop,               "ListenStop",               "ListenStop"	},
	{	Rv_RetransmissionIn,         "RetransmissionIn",         "RetransmissionIn"	},
	{	Rv_RetransmissionOut,        "RetransmissionOut",        "RetransmissionOut"	},
	{	Rv_DaemonRestarted,          "DaemonRestarted",          "DaemonRestarted"	}
};
unsigned int TEST_RvStatusSingleCount  =
	sizeof(TEST_RvStatusSingleList) / sizeof(TEST_RvStatusSingleList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TEST_Datum   TEST_CmStatusSingleList[] = {
	{	Cm_None,                     "None",                     "None"	},
	{	Cm_DeliveryConfirm,          "DeliveryConfirm",          "DeliveryConfirm"	},
	{	Cm_DeliveryComplete,         "DeliveryComplete",         "DeliveryComplete"	},
	{	Cm_DeliveryNoResponse,       "DeliveryNoResponse",       "DeliveryNoResponse"	},
	{	Cm_DeliveryFailed,           "DeliveryFailed",           "DeliveryFailed"	},
	{	Cm_DeliveryUnavailable,      "DeliveryUnavailable",      "DeliveryUnavailable"	},
	{	Cm_RegistrationRequest,      "RegistrationRequest",      "RegistrationRequest"	},
	{	Cm_RegistrationCertified,    "RegistrationCertified",    "RegistrationCertified"	},
	{	Cm_RegistrationNotCertified, "RegistrationNotCertified", "RegistrationNotCertified"	},
	{	Cm_RegistrationNoResponse,   "RegistrationNoResponse",   "RegistrationNoResponse"	},
	{	Cm_RegistrationClosed,       "RegistrationClosed",       "RegistrationClosed"	},
	{	Cm_RegistrationDiscovery,    "RegistrationDiscovery",    "RegistrationDiscovery"	},
	{	Cm_RegistrationMoved,        "RegistrationMoved",        "RegistrationMoved"	},
	{	Cm_RegistrationCollision,    "RegistrationCollision",    "RegistrationCollision"	},
	{	Cm_RelayConnected,           "RelayConnected",           "RelayConnected"	},
	{	Cm_RelayDisconnected,        "RelayDisconnected",        "RelayDisconnected"	},
	{	Cm_QueueSchedulerActive,     "QueueSchedulerActive",     "QueueSchedulerActive"	},
	{	Cm_QueueSchedulerInactive,   "QueueSchedulerInactive",   "QueueSchedulerInactive"	}
};
unsigned int TEST_CmStatusSingleCount  =
	sizeof(TEST_CmStatusSingleList) / sizeof(TEST_CmStatusSingleList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TEST_Datum   TEST_FtStatusSingleList[] = {
	{	Ft_None,                     "None",                     "None"	},
	{	Ft_ParamMismatch_Warn,       "ParamMismatch_Warn",       "ParamMismatch_Warn"	},
	{	Ft_ParamMismatch_Error,      "ParamMismatch_Error",      "ParamMismatch_Error"	},
	{	Ft_DisablingMember,          "DisablingMember",          "DisablingMember"	},
	{	Ft_TooManyActive,            "TooManyActive",            "TooManyActive"	},
	{	Ft_TooFewActive,             "TooFewActive",             "TooFewActive"	}
};
unsigned int TEST_FtStatusSingleCount  =
	sizeof(TEST_FtStatusSingleList) / sizeof(TEST_FtStatusSingleList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TEST_Datum   TEST_RvStatusMultipleList[] = {
	{
		Rv_ActorSpeed,
		"SlowConsumer|FastProducer",
		"SlowConsumer|FastProducer"
	},
	{
		Rv_LicenseExpire,
		"LicenseExpire_Warn|LicenseExpire_Error",
		"LicenseExpire_Warn|LicenseExpire_Error"
	},
	{
		Rv_Dataloss_In,
		"Dataloss_In_Ptp|Dataloss_In_BCast",
		"Dataloss_In_Ptp|Dataloss_In_BCast"
	},
	{
		Rv_Dataloss_Out,
		"Dataloss_Out_Ptp|Dataloss_Out_BCast",
		"Dataloss_Out_Ptp|Dataloss_Out_BCast"
	},
	{
		Rv_Dataloss_Ptp,
		"Dataloss_In_Ptp|Dataloss_Out_Ptp",
		"Dataloss_In_Ptp|Dataloss_Out_Ptp"
	},
	{
		Rv_Dataloss_BCast,
		"Dataloss_In_BCast|Dataloss_Out_BCast",
		"Dataloss_In_BCast|Dataloss_Out_BCast"
	},
	{
		Rv_Dataloss,
		"Dataloss_In_Ptp|Dataloss_In_BCast|Dataloss_Out_Ptp|Dataloss_Out_BCast",
		"Dataloss_In_Ptp|Dataloss_In_BCast|Dataloss_Out_Ptp|Dataloss_Out_BCast"
	},
	{
		Rv_Rvd_Connection,
		"Rvd_Disconnected|Rvd_Connected",
		"Rvd_Disconnected|Rvd_Connected"
	},
	{
		Rv_Vc_Disconnection,
		"Vc_Disconnected_Info|Vc_Disconnected_Error",
		"Vc_Disconnected_Info|Vc_Disconnected_Error"
	},
	{
		Rv_Vc_Connection,
		"Vc_Connected|Vc_Disconnected_Info|Vc_Disconnected_Error",
		"Vc_Connected|Vc_Disconnected_Info|Vc_Disconnected_Error"
	},
	{
		static_cast<RvAdvFlags>(
			Rv_Rvd_Connection | Rv_Vc_Connection),
		"Rvd_Disconnected|Rvd_Connected|Vc_Connected|Vc_Disconnected_Info|Vc_Disconnected_Error",
		"Rvd_Disconnected|Rvd_Connected|Vc_Connected|Vc_Disconnected_Info|Vc_Disconnected_Error"
	},
	{
		Rv_HostStartStop,
		"HostStart|HostStop",
		"HostStart|HostStop"
	},
	{
		Rv_HostInfo,
		"HostStart|HostStop|HostStatus",
		"HostStart|HostStop|HostStatus"
	},
	{
		Rv_ListenStartStop,
		"ListenStart|ListenStop",
		"ListenStart|ListenStop"
	},
	{
		Rv_Retransmission,
		"RetransmissionIn|RetransmissionOut",
		"RetransmissionIn|RetransmissionOut"
	},
};
unsigned int TEST_RvStatusMultipleCount  =
	sizeof(TEST_RvStatusMultipleList) / sizeof(TEST_RvStatusMultipleList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TEST_Datum   TEST_CmStatusMultipleList[] = {
	{
		Cm_Delivery,
		"DeliveryConfirm|DeliveryComplete|DeliveryNoResponse|DeliveryFailed|DeliveryUnavailable",
		"DeliveryConfirm|DeliveryComplete|DeliveryNoResponse|DeliveryFailed|DeliveryUnavailable"
	},
	{
		Cm_Registration,
		"RegistrationRequest|RegistrationCertified|RegistrationNotCertified|RegistrationNoResponse|RegistrationClosed|RegistrationDiscovery|RegistrationMoved|RegistrationCollision",
		"RegistrationRequest|RegistrationCertified|RegistrationNotCertified|RegistrationNoResponse|RegistrationClosed|RegistrationDiscovery|RegistrationMoved|RegistrationCollision"
	},
	{
		Cm_Relay,
		"RelayConnected|RelayDisconnected",
		"RelayConnected|RelayDisconnected"
	},
	{
		Cm_QueueScheduler,
		"QueueSchedulerActive|QueueSchedulerInactive",
		"QueueSchedulerActive|QueueSchedulerInactive"
	},
};
unsigned int TEST_CmStatusMultipleCount  =
	sizeof(TEST_CmStatusMultipleList) / sizeof(TEST_CmStatusMultipleList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TEST_Datum   TEST_FtStatusMultipleList[] = {
	{
		Ft_ParamMismatch,
		"ParamMismatch_Warn|ParamMismatch_Error",
		"ParamMismatch_Warn|ParamMismatch_Error"
	},
	{
		Ft_InvalidActiveCount,
		"TooManyActive|TooFewActive",
		"TooManyActive|TooFewActive"
	},
};
unsigned int TEST_FtStatusMultipleCount  =
	sizeof(TEST_FtStatusMultipleList) / sizeof(TEST_FtStatusMultipleList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename EnumType>
	int TEST_DoThisTest(const MLB::Utility::EnumFlags<EnumType> &enum_ctl,
		unsigned int test_count, const TEST_Datum *test_list,
		const std::string &test_name, bool mask_flag, int &return_code)
{
	std::cout << "Test: " << test_name << std::endl;

	unsigned int count_1;
	for (count_1 = 0; count_1 < test_count; ++count_1) {
		try {
			//	Test conversion of an enumeration to a string...
			EnumType tmp_evalue_0 = static_cast<EnumType>(test_list[count_1].flag_evalue_);
			std::string tmp_string = (!mask_flag) ?
				enum_ctl.ToStringChecked(tmp_evalue_0) :
				enum_ctl.MaskToStringChecked(tmp_evalue_0);
			if (tmp_string != test_list[count_1].str_evalue_) {
				std::cout << "FAILED TEST NUMBER " << (count_1 + 1) << " OF " <<
					test_count << ": (action to string):" << std::endl;
				std::cout << "   EXPECTED=[" <<
					test_list[count_1].str_evalue_ << "]" << std::endl;
				std::cout << "   ACTUAL  =[" <<
					tmp_string << "]" << std::endl;
				return_code = EXIT_FAILURE;
			}
			//	Test conversion of a string to an enumeration...
			EnumType tmp_evalue_1 = static_cast<EnumType>(0);
			try {
				tmp_evalue_1 = (!mask_flag) ?
					enum_ctl.FromStringChecked(test_list[count_1].str_evalue_) :
					enum_ctl.MaskFromStringChecked(test_list[count_1].str_evalue_);
				if (tmp_evalue_1 != tmp_evalue_0) {
					std::cout << "FAILED TEST NUMBER " << (count_1 + 1) << " OF " <<
						test_count << ": (string to action #1): EXPECTED=[" <<
						test_list[count_1].flag_evalue_ << "], ACTUAL=[" <<
						tmp_evalue_1 << "]" << std::endl;
					return_code = EXIT_FAILURE;
				}
			}
			catch (const std::exception &except) {
				std::cout << "FAILED TEST NUMBER " << (count_1 + 1) << " OF " <<
					test_count << ": (string to action #2): " << except.what() <<
					std::endl;
				return_code = EXIT_FAILURE;
			}
			//	For masks, test conversion of an equivalent string to an enum,,,
			if (mask_flag) {
				EnumType tmp_evalue_2;
				try {
					tmp_evalue_2 = enum_ctl.MaskFromStringChecked(
						test_list[count_1].alt_str_evalue_);
					if (tmp_evalue_2 != tmp_evalue_0) {
						std::cout << "FAILED TEST NUMBER " << (count_1 + 1) <<
							" OF " << test_count <<
								": (string to action #2): EXPECTED=[" <<
							test_list[count_1].flag_evalue_ << "], ACTUAL=[" <<
							tmp_evalue_2 << "], FIRST RESULT=[" << tmp_evalue_1 <<
							"]" << std::endl;
						return_code = EXIT_FAILURE;
					}
				}
				catch (const std::exception &except) {
					std::cout << "FAILED TEST NUMBER " << (count_1 + 1) << " OF " <<
						test_count << ": (string to action #2): " << except.what() <<
						std::endl;
					return_code = EXIT_FAILURE;
				}
			}
		}
		catch (const std::exception &except) {
			std::cout << "FAILED TEST NUMBER " << (count_1 + 1) << " OF " <<
				test_count << ": (action to string): " << except.what() <<
				std::endl;
			return_code = EXIT_FAILURE;
		}
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	return_code = TEST_DoThisTest(GetRvAdvFlagsEnum(), TEST_RvStatusSingleCount,
		TEST_RvStatusSingleList, "Rv single flag", false, return_code);
	return_code = TEST_DoThisTest(GetRvAdvFlagsEnum(), TEST_RvStatusMultipleCount,
		TEST_RvStatusMultipleList, "Rv mask flag", true, return_code);

	return_code = TEST_DoThisTest(GetCmAdvFlagsEnum(), TEST_CmStatusSingleCount,
		TEST_CmStatusSingleList, "Cm single flag", false, return_code);
	return_code = TEST_DoThisTest(GetCmAdvFlagsEnum(), TEST_CmStatusMultipleCount,
		TEST_CmStatusMultipleList, "Cm mask flag", true, return_code);

	return_code = TEST_DoThisTest(GetFtAdvFlagsEnum(), TEST_FtStatusSingleCount,
		TEST_FtStatusSingleList, "Ft single flag", false, return_code);
	return_code = TEST_DoThisTest(GetFtAdvFlagsEnum(), TEST_FtStatusMultipleCount,
		TEST_FtStatusMultipleList, "Ft mask flag", true, return_code);

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

