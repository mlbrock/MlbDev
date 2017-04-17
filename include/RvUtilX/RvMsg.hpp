// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for RvUtilX TibrvMsg wrapper functions.

	Revision History	:	2007-02-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_RvMsg_hpp__HH

#define HH__MLB__RvUtilX_RvMsg_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
API_RVUTILX const TibrvMsg   &MsgClearReferences(const TibrvMsg &msg);
API_RVUTILX std::string      &MsgConvertToString(const TibrvMsg &msg,
	std::string &out_string);
API_RVUTILX std::string       MsgConvertToString(const TibrvMsg &msg);
API_RVUTILX void              MsgCreateCopy(const TibrvMsg &src_msg,
	TibrvMsg &dst_msg);
API_RVUTILX TibrvMsg         *MsgDetach(TibrvMsg &msg);
API_RVUTILX void              MsgExpand(TibrvMsg &msg,
	unsigned int additional_storage);
API_RVUTILX const void       *MsgGetAsBytes(const TibrvMsg &msg);
API_RVUTILX const void       *MsgGetAsBytesCopy(const TibrvMsg &msg,
	void *msg_byte_ptr, unsigned int msg_byte_length);
API_RVUTILX unsigned int      MsgGetByteSize(const TibrvMsg &msg);
API_RVUTILX TibrvMsgDateTime &MsgGetCurrentTime(TibrvMsgDateTime &out_time);
API_RVUTILX TibrvMsgDateTime  MsgGetCurrentTime();
API_RVUTILX TibrvMsgField    &MsgGetField(const TibrvMsg &msg,
	const char *field_name, TibrvMsgField &msg_field, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsgField    &MsgGetField(const TibrvMsg &msg,
	const std::string &field_name, TibrvMsgField &msg_field,
	tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsgField    &MsgGetFieldByIndex(const TibrvMsg &msg,
	TibrvMsgField &msg_field, unsigned int field_index = 0);
API_RVUTILX TibrvMsgField    &MsgGetFieldInstance(const TibrvMsg &msg,
	const char *field_name, TibrvMsgField &msg_field,
	unsigned int field_instance);
API_RVUTILX TibrvMsgField    &MsgGetFieldInstance(const TibrvMsg &msg,
	const std::string &field_name, TibrvMsgField &msg_field,
	unsigned int field_instance );
API_RVUTILX tibrvMsg          MsgGetHandle(const TibrvMsg &msg);
API_RVUTILX TibrvMsg         &MsgGetMsg(const TibrvMsg &msg,
	const char *field_name, TibrvMsg &sub_msg, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg         &MsgGetMsg(const TibrvMsg &msg,
	const std::string &field_name, TibrvMsg &sub_msg, tibrv_u16 field_id = 0);
API_RVUTILX unsigned int      MsgGetNumFields(const TibrvMsg &msg);
API_RVUTILX const char       *MsgGetReplySubjectPtr(const TibrvMsg &msg);
API_RVUTILX std::string      &MsgGetReplySubject(const TibrvMsg &msg,
	std::string &subject_name);
API_RVUTILX std::string       MsgGetReplySubject(const TibrvMsg &msg);
API_RVUTILX const char       *MsgGetSendSubjectPtr(const TibrvMsg &msg);
API_RVUTILX std::string      &MsgGetSendSubject(const TibrvMsg &msg,
	std::string &subject_name);
API_RVUTILX std::string       MsgGetSendSubject(const TibrvMsg &msg);
API_RVUTILX TibrvStatus       MsgGetStatus(const TibrvMsg &msg);
API_RVUTILX bool              MsgIsDetached(const TibrvMsg &msg);
API_RVUTILX const TibrvMsg   &MsgMarkReferences(const TibrvMsg &msg);
API_RVUTILX TibrvStatus       MsgRemoveField(TibrvMsg &msg,
	const char *field_name, tibrv_u16 field_id = 0);
API_RVUTILX TibrvStatus       MsgRemoveField(TibrvMsg &msg,
	const std::string &field_name, tibrv_u16 field_id = 0);
API_RVUTILX bool              MsgRemoveFieldInstance(TibrvMsg &msg,
	const char *field_name, unsigned int field_instance);
API_RVUTILX bool              MsgRemoveFieldInstance(TibrvMsg &msg,
	const std::string &field_name, unsigned int field_instance);
API_RVUTILX void              MsgReset(TibrvMsg &msg);
API_RVUTILX TibrvMsg         &MsgSetReplySubject(TibrvMsg &msg,
	const char *subject_name);
API_RVUTILX TibrvMsg         &MsgSetReplySubject(TibrvMsg &msg,
	const std::string &subject_name);
API_RVUTILX TibrvMsg         &MsgSetSendSubject(TibrvMsg &msg,
	const char *subject_name);
API_RVUTILX TibrvMsg         &MsgSetSendSubject(TibrvMsg &msg,
	const std::string &subject_name);
API_RVUTILX std::string      &MsgToByteArray(const TibrvMsg &msg,
	std::string &out_string);
API_RVUTILX std::string       MsgToByteArray(const TibrvMsg &msg);
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvMsg_hpp__HH

