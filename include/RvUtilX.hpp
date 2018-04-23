// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for TibCo/Rendezvous 6 and above.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_hpp__HH

#define HH__MLB__RvUtilX_hpp__HH	1

#define MLB__LIB__RvUtilX

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/TimeSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <deque>

#ifdef MLB__LIB__RvUtilX
# ifdef RVUTILX_DLL_EXPORTS
#  define API_RVUTILX		MB_LIB_EXPORT
# else
#  define API_RVUTILX		MB_LIB_IMPORT
# endif // #ifdef RVUTILX_DLL_EXPORTS
#else
#  define API_RVUTILX
#endif // #ifdef MLB__LIB__RvUtilX

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_RVUTILX

//	We use the same header file as that used to compile the RV C++ library to
//	to prevent problems (such as that between:
//
//		tibrv/tibrvcpp.h:
//			const TibrvListener::TibrvTransport*  getTransport() const
//	and:
//		src/tibrvcpp.h:	
//			const TibrvListener::TibrvTransport*  getTransport() const
//		
//#include "tibrv/tibrvcpp.h"
#include "tibrvcpp.h"
#include "ftcpp.h"

#ifdef MLB_RvUtilX_VersionSupport
# if MLB_RvUtilX_VersionSupport >= 0700
#  undef  MLB_RvUtilX_Support_TransportSetBatchMode
#  define MLB_RvUtilX_Support_TransportSetBatchMode
# endif // # if MLB_RvUtilX_VersionSupport >= 0700
#endif // #ifdef MLB_RvUtilX_VersionSupport

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
typedef std::deque<TibrvMsg>         TibrvMsgList;
typedef TibrvMsgList::iterator       TibrvMsgListIter;
typedef TibrvMsgList::const_iterator TibrvMsgListIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	General support for formatting in human-readable form...
//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX std::string MsgFieldTypeToString(const TibrvMsgField &datum);
API_RVUTILX std::string MsgFieldTypeToString(tibrv_u8 msg_field_type);
API_RVUTILX std::string MsgFieldTypeToDescription(const TibrvMsgField &datum);
API_RVUTILX std::string MsgFieldTypeToDescription(tibrv_u8 msg_field_type);
API_RVUTILX tibrv_u8    StringToMsgFieldType(
	const std::string &msg_field_type_string);

API_RVUTILX std::string QueueLimitPolicyToString(
	tibrvQueueLimitPolicy queue_limit_policy);
API_RVUTILX std::string QueueLimitPolicyToDescription(
	tibrvQueueLimitPolicy queue_limit_policy);

API_RVUTILX std::string EventTypeToString(tibrvEventType event_type);
API_RVUTILX std::string EventTypeToDescription(tibrvEventType event_type);

API_RVUTILX std::string IOTypeToString(tibrvIOType io_type);
API_RVUTILX std::string IOTypeToDescription(tibrvIOType io_type);

API_RVUTILX std::string FtActionToString(tibrvftAction ft_action);
API_RVUTILX std::string FtActionToDescription(tibrvftAction ft_action);

API_RVUTILX std::string MsgDataTypeToString(tibrvMsgDataType msg_data_type);
API_RVUTILX std::string MsgDataTypeToDescription(
	tibrvMsgDataType msg_data_type);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX const char  *GetSendSubjectPtr(const TibrvMsg &msg);
API_RVUTILX const char  *GetReplySubjectPtr(const TibrvMsg &msg);
API_RVUTILX std::string &GetSendSubject(const TibrvMsg &msg,
	std::string &out_subject);
API_RVUTILX std::string &GetReplySubject(const TibrvMsg &msg,
	std::string &out_subject);
API_RVUTILX std::string  GetSendSubject(const TibrvMsg &msg);
API_RVUTILX std::string  GetReplySubject(const TibrvMsg &msg);
API_RVUTILX TibrvMsg    &SetReplySubject(TibrvMsg &msg,
	const char *subject_name);
API_RVUTILX TibrvMsg    &SetReplySubject(TibrvMsg &msg,
	const std::string &subject_name);
API_RVUTILX TibrvMsg    &SetSendSubject(TibrvMsg &msg,
	const char *subject_name);
API_RVUTILX TibrvMsg    &SetSendSubject(TibrvMsg &msg,
	const std::string &subject_name);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX void      CopyMsg(const TibrvMsg &src_msg, TibrvMsg &dst_msg);
API_RVUTILX void      CopyMsgNoSubjects(const TibrvMsg &src_msg,
	TibrvMsg &dst_msg);
API_RVUTILX TibrvMsg *DetachTibrvMsg(TibrvMsg &src_msg);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_RVUTILX void Swap(TibrvMsg &lhs, TibrvMsg &rhs)
{
	TibrvMsg tmp_msg;

	CopyMsg(lhs, tmp_msg);
	CopyMsg(rhs, lhs);
	CopyMsg(tmp_msg, rhs);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_RVUTILX MLB::Utility::TimeSpec &FromTibDateTime(
	const tibrvMsgDateTime &src_dt, MLB::Utility::TimeSpec &dst_dt);
API_RVUTILX MLB::Utility::TimeSpec  FromTibDateTime(
	const tibrvMsgDateTime &src_dt);
API_RVUTILX MLB::Utility::TimeVal  &FromTibDateTime(
	const tibrvMsgDateTime &src_dt, MLB::Utility::TimeVal &dst_dt);

API_RVUTILX tibrvMsgDateTime       &ToTibDateTime(
	const MLB::Utility::TimeSpec &src_dt, tibrvMsgDateTime &dst_dt);
API_RVUTILX tibrvMsgDateTime       &ToTibDateTime(
	const MLB::Utility::TimeSpec &src_dt);
API_RVUTILX tibrvMsgDateTime       &ToTibDateTime(
	const MLB::Utility::TimeVal &src_dt, tibrvMsgDateTime &dst_dt);
API_RVUTILX tibrvMsgDateTime       &ToTibDateTime(
	const MLB::Utility::TimeVal &src_dt);
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_RVUTILX std::string ListBaseNameToCountName(
	const char *field_name_base)
{
	return(std::string(field_name_base) += "_COUNT");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_RVUTILX std::string ListBaseNameToDataName(
	const char *field_name_base)
{
	return(std::string(field_name_base) += "_DATA");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const unsigned int MaxSubjectSize  = TIBRV_SUBJECT_MAX;
const unsigned int MaxElementSize  = TIBRV_SUBJECT_TOKEN_MAX;

	//	The smallest subject name element is a single character, which is
	//	separated by a period ('.'). Thus the maximum number of elements in
	//	a subject name is the maximum length of a subject name divided by 2...
const unsigned int MaxElementCount = MaxSubjectSize / 2;

	//	This maximum was observed to be operative under Rendezvous 6.9. An
	//	attempt to set the reply subject to a subject name larger than this
	//	using the 'setReplySubject()' method doesn't result in an error
	//	TibrvStatus, but the reply subject is set to the empty string.
const unsigned int MaxReplySubjectSize = 118;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char ElementSeparatorChar      = '.';
const char ElementAnyChar            = '*';
const char ElementLastChar           = '>';

const char ElementSeparatorCharStr[] = ".";
const char ElementAnyCharStr[]       = "*";
const char ElementLastCharStr[]      = ">";

const char ElementPatternCharStr[]   = "*>";
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_RVUTILX) RvException :
	public MLB::Utility::ExceptionGeneral {
public:
	RvException();
	RvException(const char *except_string);
	RvException(const std::string &except_string);
	RvException(const std::ostringstream &except_string);
	~RvException() MBCOMPAT_EXCEPT_NOTHROW;

	virtual void Rethrow(const char *except_string = NULL) const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_RVUTILX) RvExceptionStatus :
	public RvException {
public:
	//	Default constructor.
	RvExceptionStatus();
	//	String-only constructors...
	RvExceptionStatus(const char *except_string);
	RvExceptionStatus(const std::string &except_string);
	RvExceptionStatus(const std::ostringstream &except_string);
	//	C tibrv_status - based constructors...
	RvExceptionStatus(tibrv_status rv_status);
	RvExceptionStatus(tibrv_status rv_status, const char *except_string);
	RvExceptionStatus(tibrv_status rv_status, const std::string &except_string);
	RvExceptionStatus(tibrv_status rv_status,
		const std::ostringstream &except_string);
	//	C++ TibrvStatus - based constructors...
	RvExceptionStatus(const TibrvStatus &rv_status);
	RvExceptionStatus(const TibrvStatus &rv_status, const char *except_string);
	RvExceptionStatus(const TibrvStatus &rv_status,
		const std::string &except_string);
	RvExceptionStatus(const TibrvStatus &rv_status,
		const std::ostringstream &except_string);
	~RvExceptionStatus() MBCOMPAT_EXCEPT_NOTHROW;

	tibrv_status GetStatus() const;

	virtual void Rethrow(const char *except_string = NULL) const;

	static std::string GetStatusString(const TibrvStatus &rv_status,
		const char *other_text = NULL);
	static std::string GetStatusString(tibrv_status rv_status,
		const char *other_text = NULL);

protected:
	tibrv_status rv_status_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_RVUTILX) RvExceptionTimeOut :
	public RvException {
public:
	//	Default constructor.
	RvExceptionTimeOut();
	//	String-only constructors...
	RvExceptionTimeOut(const char *except_string);
	RvExceptionTimeOut(const std::string &except_string);
	RvExceptionTimeOut(const std::ostringstream &except_string);
	// Other constructors...
	RvExceptionTimeOut(double time_out);
	RvExceptionTimeOut(double time_out, const char *except_string);
	RvExceptionTimeOut(double time_out, const std::string &except_string);
	RvExceptionTimeOut(double time_out, const std::ostringstream &except_string);
	~RvExceptionTimeOut() MBCOMPAT_EXCEPT_NOTHROW;

	double GetTimeOut() const;

	virtual void Rethrow(const char *except_string = NULL) const;

	static std::string GetTimeOutString(double time_out,
		const char *other_text = NULL);

private:
	double time_out_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX void ThrowTibrvStatus(TibrvStatus error_code,
	const char *error_text);
API_RVUTILX void ThrowTibrvStatus(TibrvStatus error_code,
	const std::string &error_text);
API_RVUTILX void ThrowTibrvStatus(TibrvStatus error_code,
	const std::ostringstream &error_text);
API_RVUTILX void ThrowTibrvTimeOut(double time_out,
	const char *error_text = NULL);
API_RVUTILX void ThrowTibrvTimeOut(double time_out,
	const std::string &error_text);
API_RVUTILX void ThrowTibrvTimeOut(double time_out,
	const std::ostringstream &error_text);
API_RVUTILX void ThrowTibrvTimeOutIf(TibrvStatus rv_status,
	double time_out, const char *error_text = NULL);
API_RVUTILX void ThrowTibrvTimeOutIf(TibrvStatus rv_status,
	double time_out, const std::string &error_text);
API_RVUTILX void ThrowTibrvTimeOutIf(TibrvStatus rv_status,
	double time_out, const std::ostringstream &error_text);
API_RVUTILX void ThrowTibrvTimeOut(const char *function_name, double time_out);
API_RVUTILX void ThrowTibrvTimeOut(const std::string &function_name,
	double time_out);
API_RVUTILX void ThrowTibrvTimeOut(const std::ostringstream &function_name,
	double time_out);
API_RVUTILX void ThrowTibrvTimeOutIf(TibrvStatus rv_status,
	const char *function_name, double time_out);
API_RVUTILX void ThrowTibrvTimeOutIf(TibrvStatus rv_status,
	const std::string &function_name, double time_out);
API_RVUTILX void ThrowTibrvTimeOutIf(TibrvStatus rv_status,
	const std::ostringstream &function_name, double time_out);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	For use with the Tib/Rv C++ API.

	Should be invoked as:

		RvUtilX_THROW_TIBRV_STATUS_IF(Tibrv::open, ())

	or:

		TibrvNetTransport my_transport;
		RvUtilX_THROW_TIBRV_STATUS_IF(my_transport.create, (serv, net, daemon));
*/
#define RvUtilX_THROW_TIBRV_STATUS_IF(method_name, method_args)		\
	{																						\
		TibrvStatus INTERNAL_rv_status = method_name method_args ;		\
		if (INTERNAL_rv_status != TIBRV_OK) {									\
			std::ostringstream INTERNAL_error_text;							\
			INTERNAL_error_text << "Invocation of '" << #method_name <<	\
				"()' failed";															\
			MLB::RvUtilX::ThrowTibrvStatus(INTERNAL_rv_status,				\
				INTERNAL_error_text.str());												\
		}																					\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	For use with the Tib/Rv C++ API.

	Should be invoked as (note that 'time_out' appears twice in the example ---
	once as a parameter to the Rendezvous function and once as a parameter to
	be used to report the error):

		TibrvNetTransport my_transport;
		RvUtilX_THROW_TIBRV_TIMEOUT_IF(my_transport.sendRequest,
			(send_msg, reply_msg, time_out), time_out);
*/
#define RvUtilX_THROW_TIBRV_TIMEOUT_IF(method_name, method_args, time_out)	\
	{																								\
		TibrvStatus INTERNAL_rv_status = method_name method_args ;				\
		if (INTERNAL_rv_status == TIBRV_TIMEOUT) {									\
			MLB::RvUtilX::ThrowTibrvTimeOut(#method_name, time_out);				\
		}																							\
		else if (INTERNAL_rv_status != TIBRV_OK) {									\
			std::ostringstream INTERNAL_error_text;									\
			INTERNAL_error_text << "Invocation of '" << #method_name <<			\
				"()' failed";																	\
			MLB::RvUtilX::ThrowTibrvStatus(INTERNAL_rv_status,						\
				INTERNAL_error_text.str());														\
		}																							\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	For use with the Tib/Rv C API.

	Should be invoked as:

		RvUtilX_THROW_TIBRV_STATUS_C_IF(tibrv_open, ())

	or:

		tibrvTransport my_transport;
		RvUtilX_THROW_TIBRV_STATUS_C_IF(tibrvTransport_Create,
			(&my_transport, serv, net, daemon));
*/
#define RvUtilX_THROW_TIBRV_STATUS_C_IF(method_name, method_args)		\
	{																						\
		tibrv_status INTERNAL_rv_status = method_name method_args ;		\
		if (INTERNAL_rv_status != TIBRV_OK) {									\
			std::ostringstream INTERNAL_error_text;							\
			INTERNAL_error_text << "Invocation of '" << #method_name <<	\
				"()' failed";															\
			MLB::RvUtilX::ThrowTibrvStatus(INTERNAL_rv_status,				\
				INTERNAL_error_text.str());												\
		}																					\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	For use with the Tib/Rv C API.

	Should be invoked as (note that 'time_out' appears twice in the example ---
	once as a parameter to the Rendezvous function and once as a parameter to
	be used to report the error):

		tibrvTransport my_transport;
		RvUtilX_THROW_TIBRV_TIMEOUT_C_IF(tibrvTransport_SendRequest,
			(my_transport, send_msg, reply_msg, time_out), time_out);
*/
#define RvUtilX_THROW_TIBRV_TIMEOUT_C_IF(method_name, method_args, time_out)	\
	{																									\
		tibrv_status INTERNAL_rv_status = method_name method_args ;					\
		if (INTERNAL_rv_status == TIBRV_TIMEOUT) {										\
			MLB::RvUtilX::ThrowTibrvTimeOut(#method_name, time_out);					\
		}																								\
		else if (INTERNAL_rv_status != TIBRV_OK) {										\
			std::ostringstream INTERNAL_error_text;										\
			INTERNAL_error_text << "Invocation of '" << #method_name <<				\
				"()' failed";																		\
			MLB::RvUtilX::ThrowTibrvStatus(INTERNAL_rv_status,							\
				INTERNAL_error_text.str());															\
		}																								\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/**
	\enum Specifies the flags which may be used to format Rendezvous messages
	as human-readable strings.
*/
enum RvMsgToStringFlags {
	/** No flags set. */
	RvMsgToStringFlags_None           = 0x0000,
	/** Includes the message nesting depth in the output. */
	RvMsgToStringFlags_MsgDepth       = 0x0001,
	/** Includes the message field index in the output. */
	RvMsgToStringFlags_MsgFieldIndex  = 0x0002,
	/** Includes the message field type id in the output. */
	RvMsgToStringFlags_FieldTypeId    = 0x0004,
	/** Includes the message field type name in the output. */
	RvMsgToStringFlags_FieldTypeName  = 0x0008,
	/** Includes the message field id in the output. */
	RvMsgToStringFlags_FieldId        = 0x0010,
	/** Includes the array field element count in the output. */
	RvMsgToStringFlags_ArrayCount     = 0x0020,
	/** Inclues the array field element index in the output. */
	RvMsgToStringFlags_ArrayIndex     = 0x0040,
	/** Separates message fields with a comma. */
	RvMsgToStringFlags_FieldCommaSep  = 0x0080,
	/** Separates message field array elements with a comma. */
	RvMsgToStringFlags_ArrayCommaSep  = 0x0100,
	/** Formats date/time fields with fractional nanoseconds. */
	RvMsgToStringFlags_DateFieldNSecs = 0x0200,
	/** Expands XML message fields to their contents in the output. */
	RvMsgToStringFlags_ExpandXml      = 0x0400,
	/** Expands opaque message fields to their contents in the output. */
	RvMsgToStringFlags_ExpandOpaque   = 0x0800,
	/** Uses the default precision for floating-point fields in the output. */
	RvMsgToStringFlags_DefaultFPPrec  = 0x1000,
	/** Does not surround messages with curly brackets in the output. */
	RvMsgToStringFlags_NoMsgCBrackets = 0x2000,
	/** Provide line wrapping and depth-dependent indentation in the output. */
	RvMsgToStringFlags_WrapAndIndent  = 0x4000,
	/** The settings used as the dfault parameter value. */
	RvMsgToStringFlags_Default        = RvMsgToStringFlags_None,
	/** Flag setting to use for compatibility with tibrvlisten. */
	RvMsgToStringFlags_Compatibility  = RvMsgToStringFlags_None,
	/** The mask of all valid flags. */
	RvMsgToStringFlags_Mask           = RvMsgToStringFlags_MsgDepth | RvMsgToStringFlags_MsgFieldIndex | RvMsgToStringFlags_FieldTypeId | RvMsgToStringFlags_FieldTypeName | RvMsgToStringFlags_FieldId | RvMsgToStringFlags_ArrayCount | RvMsgToStringFlags_ArrayIndex | RvMsgToStringFlags_FieldCommaSep | RvMsgToStringFlags_ArrayCommaSep | RvMsgToStringFlags_DateFieldNSecs | RvMsgToStringFlags_ExpandXml | RvMsgToStringFlags_ExpandOpaque | RvMsgToStringFlags_DefaultFPPrec | RvMsgToStringFlags_NoMsgCBrackets | RvMsgToStringFlags_WrapAndIndent
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX std::ostringstream &RvMsgFieldHeader(const TibrvMsgField &datum,
	std::ostringstream &o_str,
	RvMsgToStringFlags flags = RvMsgToStringFlags_Default);
API_RVUTILX std::string  &RvMsgFieldHeader(const TibrvMsgField &datum,
	std::string &out_string,
	RvMsgToStringFlags flags = RvMsgToStringFlags_Default);
API_RVUTILX std::string   RvMsgFieldHeader(const TibrvMsgField &datum,
	RvMsgToStringFlags flags = RvMsgToStringFlags_Default);

API_RVUTILX std::string RvMsgFieldDataArrayToString(const TibrvMsgField &datum,
	RvMsgToStringFlags flags = RvMsgToStringFlags_Default);
API_RVUTILX std::string RvMsgFieldDataBytesToString(const TibrvMsgField &datum,
	RvMsgToStringFlags flags = RvMsgToStringFlags_Default);
API_RVUTILX std::string RvMsgFieldDataToString(const TibrvMsgField &datum,
	RvMsgToStringFlags flags = RvMsgToStringFlags_Default);

API_RVUTILX std::ostream & operator << (std::ostream &o_str,
	const TibrvMsgField &datum);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_RVUTILX void        RvMsgToString(const TibrvMsg &msg,
	std::string &out_string,
	RvMsgToStringFlags flags = RvMsgToStringFlags_Default);
API_RVUTILX std::string RvMsgToString(const TibrvMsg &msg,
	RvMsgToStringFlags flags = RvMsgToStringFlags_Default);

API_RVUTILX std::ostream & operator << (std::ostream &o_str,
	const TibrvMsg &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX const TibrvMsg &MarkReferencesInMsg(const TibrvMsg &msg);
API_RVUTILX const TibrvMsg &ClearReferencesInMsg(const TibrvMsg &msg);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_RVUTILX std::string FieldNameAndIdToString(const char *field_name,
	tibrv_u16 field_id = 0)
{
  	std::ostringstream tmp_text;

	tmp_text << "RV field name = " <<
		((field_name != NULL) ? "'" : "") <<
		((field_name != NULL) ? field_name : "NULL") <<
		((field_name != NULL) ? "'" : "") << ", RV field id = " << field_id;

	return(tmp_text.str());
}
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
inline API_RVUTILX std::string FieldNameAndIdToString(
	const std::string &field_name, tibrv_u16 field_id = 0)
{
	return(FieldNameAndIdToString(field_name.c_str(), field_id));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Some Tib/Rv environment functions in RvEnvironment.cpp...
API_RVUTILX void                   OpenRvEnv();
API_RVUTILX void                   CloseRvEnv();
API_RVUTILX TibrvQueue            *GetDefaultQueue();
API_RVUTILX TibrvProcessTransport *GetProcessTransportPtr();
API_RVUTILX const char            *GetRvEnvVersionPtr();
API_RVUTILX std::string           &GetRvEnvVersion(std::string &out_string);
API_RVUTILX std::string            GetRvEnvVersion();
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Functions in RvTransport.cpp...
API_RVUTILX std::string        TransportCreateInbox(
	TibrvTransport &transport_ref);
API_RVUTILX std::string       &TransportCreateInbox(
	TibrvTransport &transport_ref, std::string &subject_name);
API_RVUTILX void               TransportDestroy(TibrvTransport &transport_ref);
API_RVUTILX void               TransportDestroy(TibrvTransport *transport_ref);
API_RVUTILX std::string       &TransportGetDescription(
	const tibrvTransport &transport_ref, std::string &description);
API_RVUTILX std::string        TransportGetDescription(
	const tibrvTransport &transport_ref);
API_RVUTILX std::string       &TransportGetDescription(
	const TibrvTransport &transport_ref, std::string &description);
API_RVUTILX std::string        TransportGetDescription(
	const TibrvTransport &transport_ref);
API_RVUTILX tibrvTransport     TransportGetHandle(
	const TibrvTransport &transport_ref);
API_RVUTILX bool               TransportIsValid(
	const TibrvTransport &transport_ref);
API_RVUTILX void               TransportSend(TibrvTransport &transport_ref,
	const TibrvMsg &msg);
API_RVUTILX void               TransportSendReply(TibrvTransport &transport_ref,
	const TibrvMsg &reply_msg, const TibrvMsg &request_msg);
API_RVUTILX void               TransportSendRequest(
	TibrvTransport &transport_ref, const TibrvMsg &request_msg,
	TibrvMsg &reply_msg, double time_out);
API_RVUTILX void               TransportSetDescription
	(TibrvTransport &transport_ref, const char *description);
API_RVUTILX void               TransportSetDescription(
	TibrvTransport &transport_ref, const std::string &description);
API_RVUTILX bool               TransportIsProcessTransport(
	const TibrvTransport &transport_ref);
API_RVUTILX bool               TransportCheckIsValid(
	const TibrvTransport &transport_ref);
API_RVUTILX bool               TransportCheckIsNotValid(
	const TibrvTransport &transport_ref);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Handy class for managing ++/-- of Rendezvous library reference count...
class API_RVUTILX RvContext {
public:
	RvContext() {
		//	Perform the static open...
		RvUtilX_THROW_TIBRV_STATUS_IF(Tibrv::open, ());
	}
	RvContext(const RvContext &) {
		//	Perform the static open for the destination of the copy...
		RvUtilX_THROW_TIBRV_STATUS_IF(Tibrv::open, ());
	}
	virtual ~RvContext() {
		//	Tibrv::close() doesn't throw as of Rv 7.3, but this is future-proof.
		try {
			//	Perform the static close...
			Tibrv::close();
		}
		catch (...) { }
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_RVUTILX RvMsgSnapshotContext {
public:
	RvMsgSnapshotContext(const TibrvMsg &msg_ref);
	virtual ~RvMsgSnapshotContext();

private:
	const TibrvMsg &msg_ref_;

	MLB_Utility_NonCopyable_Macro(RvMsgSnapshotContext);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_RVUTILX SubjectNameWork {
	SubjectNameWork();
	SubjectNameWork(const std::string &subject_name);
	SubjectNameWork(const char *subject_name);

	SubjectNameWork(const SubjectNameWork &other);

	SubjectNameWork & operator = (const SubjectNameWork &other);

	void swap(SubjectNameWork &other);

	void SetSubjectName(const char *subject_name);
	void SetSubjectName(const std::string &subject_name);

	std::string  ToString() const;
	std::string &ToString(std::string &out_string) const;
	char        *ToString(char *out_string) const;

	std::string  ToStringFromElements(unsigned int element_start = 0,
		unsigned int element_end = MaxElementCount) const;
	std::string &ToStringFromElements(std::string &out_string,
		unsigned int element_start = 0,
		unsigned int element_end = MaxElementCount) const;
	char        *ToStringFromElements(char *out_string,
		unsigned int element_start = 0,
		unsigned int element_end = MaxElementCount) const;

	bool operator < (const SubjectNameWork &other) const;

	char          subject_name_[MaxSubjectSize + 1];
	unsigned int  element_count_;
	char         *element_list_[MaxElementCount];
	bool          is_pattern_;
};
API_RVUTILX std::ostream & operator << (std::ostream &o_str,
	const SubjectNameWork &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<SubjectNameWork>          SubjectNameWorkSet;
typedef SubjectNameWorkSet::iterator       SubjectNameWorkSetIter;
typedef SubjectNameWorkSet::const_iterator SubjectNameWorkSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_RVUTILX SubjectNameFlat {
	SubjectNameFlat();
	SubjectNameFlat(const std::string &subject_name);
	SubjectNameFlat(const char *subject_name);
	SubjectNameFlat(const SubjectNameWork &subject_name);

	void SetSubjectName(const std::string &subject_name);
	void SetSubjectName(const char *subject_name);
	void SetSubjectName(const SubjectNameWork &subject_name);

	bool MatchSubjectName(const std::string &subject_name,
		unsigned int *failed_element = NULL) const;
	bool MatchSubjectName(const char *subject_name,
		unsigned int *failed_element = NULL) const;
	bool MatchSubjectName(const SubjectNameFlat &subject_name,
		unsigned int *failed_element = NULL) const;

	bool IsPattern() const;
	bool IsFullName() const;
	bool IsReplyName() const;
	unsigned int GetElementCount() const;

	void CheckPattern();
	void CheckFullName();
	void CheckReplyName();

	std::string  ToString() const;
	std::string &ToString(std::string &out_string) const;
	char        *ToString(char *out_string) const;

	std::string  ToStringFromElements(unsigned int element_start = 0,
		unsigned int element_end = MaxElementCount) const;
	std::string &ToStringFromElements(std::string &out_string,
		unsigned int element_start = 0,
		unsigned int element_end = MaxElementCount) const;
	char        *ToStringFromElements(char *out_string,
		unsigned int element_start = 0,
		unsigned int element_end = MaxElementCount) const;

	bool operator < (const SubjectNameFlat &other) const;

	static bool IsPatternSubjectName(const char *subject_name);
	static bool IsPatternSubjectName(const std::string &subject_name);
	static bool IsPatternSubjectName(const std::ostringstream &subject_name);

	static bool IsFullNameSubjectName(const char *subject_name);
	static bool IsFullNameSubjectName(const std::string &subject_name);
	static bool IsFullNameSubjectName(const std::ostringstream &subject_name);

	static void CheckPatternSubjectName(const char *subject_name);
	static void CheckPatternSubjectName(const std::string &subject_name);
	static void CheckPatternSubjectName(const std::ostringstream &subject_name);

	static void CheckFullNameSubjectName(const char *subject_name);
	static void CheckFullNameSubjectName(const std::string &subject_name);
	static void CheckFullNameSubjectName(const std::ostringstream &subject_name);

	static void CheckReplySubjectName(const char *subject_name);
	static void CheckReplySubjectName(const std::string &subject_name);
	static void CheckReplySubjectName(const std::ostringstream &subject_name);

	unsigned int element_count_;
	char         element_list_[MaxElementCount][MaxElementSize + 1];
	bool         is_pattern_;
};
API_RVUTILX std::ostream & operator << (std::ostream &o_str,
	const SubjectNameFlat &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<SubjectNameFlat>          SubjectNameFlatSet;
typedef SubjectNameFlatSet::iterator       SubjectNameFlatSetIter;
typedef SubjectNameFlatSet::const_iterator SubjectNameFlatSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX bool IsPatternSubjectName(const char *subject_name);
API_RVUTILX bool IsPatternSubjectName(const std::string &subject_name);
API_RVUTILX bool IsPatternSubjectName(const std::ostringstream &subject_name);
API_RVUTILX bool IsFullNameSubjectName(const char *subject_name);
API_RVUTILX bool IsFullNameSubjectName(const std::string &subject_name);
API_RVUTILX bool IsFullNameSubjectName(const std::ostringstream &subject_name);
API_RVUTILX void CheckPatternSubjectName(const char *subject_name);
API_RVUTILX void CheckPatternSubjectName(const std::string &subject_name);
API_RVUTILX void CheckPatternSubjectName(const std::ostringstream &subject_name);
API_RVUTILX void CheckFullNameSubjectName(const char *subject_name);
API_RVUTILX void CheckFullNameSubjectName(const std::string &subject_name);
API_RVUTILX void CheckFullNameSubjectName(const std::ostringstream &subject_name);
API_RVUTILX void CheckReplySubjectName(const char *subject_name);
API_RVUTILX void CheckReplySubjectName(const std::string &subject_name);
API_RVUTILX void CheckReplySubjectName(const std::ostringstream &subject_name);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX void AppendSubjectNameToMultiPartList(
	const SubjectNameFlat &subject_name, MLB::Utility::StringVector &out_list);
API_RVUTILX void AppendSubjectNameToMultiPartList(
	const SubjectNameWork &subject_name, MLB::Utility::StringVector &out_list);
API_RVUTILX void AppendSubjectNameToMultiPartList(
	const std::string &subject_name, MLB::Utility::StringVector &out_list);
API_RVUTILX void SubjectNameToMultiPartList(
	const SubjectNameFlat &subject_name, MLB::Utility::StringVector &out_list);
API_RVUTILX void SubjectNameToMultiPartList(
	const SubjectNameWork &subject_name, MLB::Utility::StringVector &out_list);
API_RVUTILX void SubjectNameToMultiPartList(
	const std::string &subject_name, MLB::Utility::StringVector &out_list);
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_RVUTILX SNDSpec {
public:
	SNDSpec(const char *service = NULL, const char *network = NULL,
		const char *daemon = NULL);
	SNDSpec(const std::string &service, const std::string &network,
		const std::string &daemon);
	SNDSpec(tibrvTransport &source_transport);
	SNDSpec(TibrvTransport &source_transport);
	SNDSpec(const MLB::Utility::StringVector &string_list);

	void SetService(const std::string &datum);
	void SetNetwork(const std::string &datum);
	void SetDaemon(const std::string &datum);
	void SetService(const char *datum = NULL);
	void SetNetwork(const char *datum = NULL);
	void SetDaemon(const char *datum = NULL);

	void SetServiceNetworkDaemon(const std::string &service,
		const std::string &network, const std::string &daemon);
	void SetServiceNetworkDaemon(const char *service = NULL,
		const char *network = NULL, const char *daemon = NULL);

	bool ParseCmdLineServiceNetworkDaemon(unsigned int &current_index,
		int argc, char **argv);
	bool ParseCmdLineServiceNetworkDaemonFT(unsigned int &current_index,
		int argc, char **argv);

	std::string GetService() const;
	std::string GetNetwork() const;
	std::string GetDaemon() const;
	std::string GetServiceDescription() const;
	std::string GetNetworkDescription() const;
	std::string GetDaemonDescription() const;

	void CreateTransport(TibrvNetTransport &transport) const;

	std::string  ToString() const;
	std::string &ToString(std::string &out_string) const;
	std::string  ToStringSimple() const;
	std::string &ToStringSimple(std::string &out_string,
		const std::string &delimiter = "/") const;
	std::string  ToStringLines(unsigned int padding = 7,
		const std::string &separator = ": ",
		const std::string &preface = "") const;
	std::string &ToStringLines(std::string &out_string,
		unsigned int padding = 7, const std::string &separator = ": ",
		const std::string &preface = "") const;

	MLB::Utility::StringVector ToArgvList(const std::string &quote_char = "\"",
		const std::string &prefix = "-") const;

	void swap(SNDSpec &other);

	bool operator < (const SNDSpec &other) const;
	bool operator == (const SNDSpec &other) const;
	bool operator != (const SNDSpec &other) const;

	int CompareServiceAndNetwork(const std::string &service,
		const std::string &network) const;
	int CompareServiceAndNetwork(const SNDSpec &other) const;

	template <typename ContainerType>
		void AppendArgvList(ContainerType &argv_list,
		const std::string &quote_char = "\"",
		const std::string &prefix = "-") const {
		AppendArgvList(*this, argv_list, quote_char, prefix);
	}

	static MLB::Utility::StringVector ToArgvList(const SNDSpec &snd_spec,
		const std::string &quote_char = "\"", const std::string &prefix = "-");
	template <typename ContainerType>
		static void AppendArgvList(const SNDSpec &snd_spec,
		ContainerType &argv_list, const std::string &quote_char = "\"",
		const std::string &prefix = "-") {
		MLB::Utility::StringVector            tmp_argv_list(
			ToArgvList(snd_spec, quote_char, prefix));
		MLB::Utility::StringVector::size_type count_1;
		for (count_1 = 0; count_1 < tmp_argv_list.size(); ++count_1)
         argv_list.push_back(tmp_argv_list[count_1]);
	}

	static SNDSpec FromDelimitedString(const std::string &in_string,
		const std::string &delimiter);
	static SNDSpec FromTransport(TibrvTransport &source_transport);
	static SNDSpec FromTransport(tibrvTransport source_transport);

	std::string service_;
	std::string network_;
	std::string daemon_;

private:
	std::string ToStringItem(const std::string &item_ref) const;
};
API_RVUTILX std::ostream & operator << (std::ostream &o_str,
	const SNDSpec &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_RVUTILX FtSpec {
public:
	FtSpec();
	FtSpec(const std::string &group_name, unsigned int weight,
		unsigned int active_goal, double heartbeat_interval,
		double preparation_interval, double activation_interval);
	FtSpec(const MLB::Utility::StringVector &string_list);
	virtual ~FtSpec() { }

	void swap(FtSpec &other);

	std::string  GetGroupName() const;
	tibrv_u16    GetWeight() const;
	tibrv_u16    GetActiveGoal() const;
	double       GetHeartbeatInterval() const;
	double       GetPreparationInterval() const;
	double       GetActivationInterval() const;

	void SetGroupName(const std::string &group_name);
	void SetWeight(unsigned int weight);
	void SetActiveGoal(unsigned int active_goal);
	void SetHeartbeatInterval(double heartbeat_interval);
	void SetPreparationInterval(double preparation_interval);
	void SetActivationInterval(double activation_interval);

	void SetWeight(const std::string &weight);
	void SetActiveGoal(const std::string &active_goal);
	void SetHeartbeatInterval(const std::string &heartbeat_interval);
	void SetPreparationInterval(const std::string &preparation_interval);
	void SetActivationInterval(const std::string &activation_interval);

	void CreateFtMember(TibrvFtMember &ft_member, TibrvQueue &queue,
		TibrvFtMemberCallback &callback, TibrvTransport &transport) const;

	virtual bool ParseCmdLine(unsigned int &current_index, int argc,
		char **argv);

	bool operator < (const FtSpec &other) const;
	bool operator == (const FtSpec &other) const;
	bool operator != (const FtSpec &other) const;

	int Compare(const FtSpec &other) const;

	std::string  ToString() const;
	std::string &ToString(std::string &out_string) const;
	std::string  ToStringLines(unsigned int padding = 20,
		const std::string &separator = ": ",
		const std::string &preface = "") const;
	std::string &ToStringLines(std::string &out_string,
		unsigned int padding = 20, const std::string &separator = ": ",
		const std::string &preface = "") const;

	virtual MLB::Utility::StringVector ToArgvList(
		const std::string &quote_char = "\"",
		const std::string &prefix = "-") const;

	template <typename ContainerType>
		void AppendArgvList(ContainerType &argv_list,
		const std::string &quote_char = "\"",
		const std::string &prefix = "-") const {
		AppendArgvList(*this, argv_list, quote_char, prefix);
	}

	static MLB::Utility::StringVector ToArgvList(const FtSpec &ft_spec,
		const std::string &quote_char = "\"", const std::string &prefix = "-");

	template <typename ContainerType>
		static void AppendArgvList(const FtSpec &ft_spec,
		ContainerType &argv_list, const std::string &quote_char = "\"",
		const std::string &prefix = "-") {
		MLB::Utility::StringVector            tmp_argv_list(
			ToArgvList(ft_spec, quote_char, prefix));
		MLB::Utility::StringVector::size_type count_1;
		for (count_1 = 0; count_1 < tmp_argv_list.size(); ++count_1)
         argv_list.push_back(tmp_argv_list[count_1]);
	}

	static FtSpec FromDelimitedString(const std::string &in_string,
		const std::string &delimiter);

	std::string  group_name_;
	unsigned int weight_;
	unsigned int active_goal_;
	double       heartbeat_interval_;
	double       preparation_interval_;
	double       activation_interval_;
};
API_RVUTILX std::ostream & operator << (std::ostream &o_str,
	const FtSpec &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Functions in RvTransportNet.cpp...
API_RVUTILX TibrvNetTransport &CreateTransport(TibrvNetTransport &transport,
	const std::string &service = "", const std::string &network = "",
	const std::string &daemon = "");
API_RVUTILX TibrvNetTransport &CreateLicensedTransport(
	TibrvNetTransport &transport, const std::string &service,
	const std::string &network, const std::string &daemon,
	const std::string &license_ticket);
API_RVUTILX std::string &TransportGetService(
	const TibrvNetTransport &transport_ref, std::string &out_string);
API_RVUTILX std::string  TransportGetService(
	const TibrvNetTransport &transport_ref);
API_RVUTILX std::string &TransportGetNetwork(
	const TibrvNetTransport &transport_ref, std::string &out_string);
API_RVUTILX std::string  TransportGetNetwork(
	const TibrvNetTransport &transport_ref);
API_RVUTILX std::string &TransportGetDaemon(
	const TibrvNetTransport &transport_ref, std::string &out_string);
API_RVUTILX std::string  TransportGetDaemon(
	const TibrvNetTransport &transport_ref);
API_RVUTILX SNDSpec     &GetSNDSpec(const TibrvNetTransport &transport_ref,
	SNDSpec &out_spec);
API_RVUTILX SNDSpec      GetSNDSpec(const TibrvNetTransport &transport_ref);

#ifdef MLB_RvUtilX_Support_Rv7X
API_RVUTILX void         TransportSetBatchMode(TibrvNetTransport &transport_ref,
	tibrvTransportBatchMode batch_mode);
#endif // #ifdef MLB_RvUtilX_Support_Rv7X
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Some TibrvListener wrapper functions in RvListener.cpp...
API_RVUTILX TibrvListener  &CreateListener(TibrvQueue &queue_ref,
	TibrvCallback &callback_ref, TibrvTransport &transport_ref,
	TibrvListener &listener_ref, const char *subject_name,
	const void *closure = NULL);
API_RVUTILX TibrvListener  &CreateListener(TibrvQueue &queue_ref,
	TibrvCallback &callback_ref, TibrvTransport &transport_ref,
	TibrvListener &listener_ref, const std::string &subject_name,
	const void *closure = NULL);
API_RVUTILX const char     *GetSubjectPtr(const TibrvListener &listener);
API_RVUTILX std::string    &GetSubject(const TibrvListener &listener,
	std::string &out_subject);
API_RVUTILX std::string     GetSubject(const TibrvListener &listener);
API_RVUTILX TibrvTransport *GetTransport(TibrvListener *listener);
API_RVUTILX TibrvTransport *GetTransport(TibrvListener &listener);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Functions in RvFtMember.cpp...
API_RVUTILX TibrvNetTransport *GetTransport(const TibrvFtMember &ft_member);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Functions in RvFtMonitor.cpp...
API_RVUTILX TibrvNetTransport *GetTransport(const TibrvFtMonitor &ft_monitor);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Functions in RvQueue.cpp...
API_RVUTILX TibrvQueue &QueueCreate(TibrvQueue &queue_ref);
API_RVUTILX void QueueDestroy(TibrvQueue &queue_ref);
API_RVUTILX void QueueDispatch(TibrvQueue &queue_ref);
API_RVUTILX unsigned int QueueGetCount(TibrvQueue &queue_ref);
API_RVUTILX tibrvQueue QueueGetHandle(TibrvQueue &queue_ref);
API_RVUTILX void QueueGetLimitPolicy(TibrvQueue &queue_ref,
	tibrvQueueLimitPolicy &policy, unsigned int &max_events,
	unsigned int &discard_amount);
API_RVUTILX const char *QueueGetNamePtr(TibrvQueue &queue_ref);
API_RVUTILX std::string QueueGetName(TibrvQueue &queue_ref);
API_RVUTILX unsigned int QueueGetPriority(TibrvQueue &queue_ref);
API_RVUTILX bool QueueIsDefault(const TibrvQueue *queue_ptr);
API_RVUTILX bool QueueIsDefault(const TibrvQueue &queue_ref);
API_RVUTILX bool QueuePoll(TibrvQueue &queue_ref);
API_RVUTILX void QueueSetLimitPolicy(TibrvQueue &queue_ref,
	tibrvQueueLimitPolicy policy, unsigned int max_events,
	unsigned int discard_amount);
API_RVUTILX void QueueSetName(TibrvQueue &queue_ref, const char *queue_name);
API_RVUTILX void QueueSetName(TibrvQueue &queue_ref,
	const std::string &queue_name);
API_RVUTILX void QueueSetPriority(TibrvQueue &queue_ref, unsigned int priority);
API_RVUTILX bool QueueTimedDispatch(TibrvQueue &queue_ref, double time_out);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Functions in RvQueueGroup.cpp...
API_RVUTILX TibrvQueueGroup &AddToQueueGroup(TibrvQueueGroup &queue_group_ref,
	TibrvQueue &queue_ref);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX TibrvStatus SendRequestSpecificReply(TibrvTransport &transport,
	const TibrvMsg &msg_send, TibrvMsg &msg_reply, double time_out);
API_RVUTILX void        SendRequestSpecificReplyThrow(TibrvTransport &transport,
	const TibrvMsg &msg_send, TibrvMsg &msg_reply, double time_out);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX TibrvStatus WaitForOneShotMessage(TibrvTransport &transport,
	const std::string subject_recv, TibrvMsg &msg_recv, double time_out);
API_RVUTILX TibrvStatus WaitForOneShotMessage(const SNDSpec &snd_spec,
	const std::string subject_recv, TibrvMsg &msg_recv, double time_out);
API_RVUTILX TibrvStatus WaitForOneShotMessage(const std::string &service,
	const std::string &network, const std::string &daemon,
	const std::string subject_recv, TibrvMsg &msg_recv, double time_out);
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_hpp__HH

