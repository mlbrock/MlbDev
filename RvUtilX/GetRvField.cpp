// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Get message fields.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/GetRvField.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TibrvMsgField &msg_field, tibrv_u16 field_id, bool required_flag,
	tibrv_u8 required_type)
{
	// Although we don't modify the message, the method is not const. So...
	TibrvStatus rv_status =
		const_cast<TibrvMsg &>(msg).getField(field_name, msg_field, field_id);

	if (rv_status == TIBRV_NOT_FOUND) {
		if (required_flag) {
			std::ostringstream error_text;
			error_text << "Unable to locate a required field (" <<
				FieldNameAndIdToString(field_name, field_id) << ")";
			MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
		}
		return(false);
	}
	else if (rv_status == TIBRV_ID_CONFLICT) {
		std::ostringstream error_text;
		error_text << "Attempt to locate a field (" <<
			FieldNameAndIdToString(field_name, field_id) <<
			") resulted in a name/id conflict";
		MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
	}
	else if (rv_status != TIBRV_OK) {
		std::ostringstream error_text;
		error_text << "Invocation of 'TibrvMsg::getField()' for " <<
			FieldNameAndIdToString(field_name, field_id) << " failed.";
		MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
	}
	else if ((required_type != TIBRVMSG_NO_TAG) &&
		(msg_field.getType() != required_type)) {
		std::ostringstream error_text;
		error_text << "Attempt to locate a field (" <<
			FieldNameAndIdToString(field_name, field_id) <<
			") succeeded, but the field type (" <<
			MLB::RvUtilX::MsgFieldTypeToDescription(msg_field.getType()) <<
			") was not the type expected (" <<
			MLB::RvUtilX::MsgFieldTypeToDescription(required_type) << ").";
		throw RvException(error_text);
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TibrvMsgField &msg_field, tibrv_u16 field_id, bool required_flag,
	int required_type)
{
	return(GetRvField(msg, field_name, msg_field, field_id, required_flag,
		static_cast<tibrv_u8>(required_type)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TibrvMsgField &msg_field, tibrv_u16 field_id, bool required_flag,
	tibrv_u8 required_type)
{
	return(GetRvField(msg, field_name.c_str(), msg_field, field_id,
		required_flag, required_type));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TibrvMsgField &msg_field, tibrv_u16 field_id, bool required_flag,
	int required_type)
{
	return(GetRvField(msg, field_name.c_str(), msg_field, field_id,
		required_flag, required_type));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get TibrvMsgDateTime
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TibrvMsgDateTime &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	tibrvMsgDateTime c_out_datum;

	if (!GetRvField(msg, field_name, c_out_datum, field_id, required_flag,
		no_conversion_flag))
		return(false);

	out_datum = TibrvMsgDateTime(c_out_datum);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get TibrvMsgDateTime
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TibrvMsgDateTime &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get tibrvMsgDateTime
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	tibrvMsgDateTime &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_DATETIME))
		return(false);

	if (msg_field.getType() == TIBRVMSG_DATETIME)
		out_datum = msg_field.getData().date;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(tibrvMsg_GetDateTimeEx,
			(msg.getHandle(), field_name, &out_datum, field_id));
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get tibrvMsgDateTime
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	tibrvMsgDateTime &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get MLB::Utility::TimeSpec
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	MLB::Utility::TimeSpec &out_datum, tibrv_u16 field_id,
	bool required_flag, bool no_conversion_flag)
{
	TibrvMsgDateTime tmp_datum;

	if (!GetRvField(msg, field_name, tmp_datum, field_id, required_flag,
		no_conversion_flag))
		return(false);

	FromTibDateTime(tmp_datum, out_datum);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get MLB::Utility::TimeSpec
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	MLB::Utility::TimeSpec &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get MLB::Utility::TimeVal
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	MLB::Utility::TimeVal &out_datum, tibrv_u16 field_id,
	bool required_flag, bool no_conversion_flag)
{
//	TibrvMsgField          msg_field;
	MLB::Utility::TimeSpec tmp_out_datum;

	if (!GetRvField(msg, field_name, tmp_out_datum, field_id,
		required_flag, no_conversion_flag))
		return(false);

	out_datum = tmp_out_datum.ToTimeVal();

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get MLB::Utility::TimeVal
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	MLB::Utility::TimeVal &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get TibrvMsg
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TibrvMsg &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	tibrvMsg c_out_datum;

	if (!GetRvField(msg, field_name, c_out_datum, field_id, required_flag,
		no_conversion_flag))
		return(false);

	TibrvStatus rv_status = out_datum.reset();
	if (rv_status != TIBRV_OK) {
		std::ostringstream error_text;
		error_text << "Attempt to get a field (" <<
			FieldNameAndIdToString(field_name, field_id) <<
			") succeeded, but the attempt to 'reset()' the destination of the " <<
			"nested message failed.";
		throw RvExceptionStatus(rv_status, error_text);
	}

	TibrvMsg tmp_msg(c_out_datum, TIBRV_FALSE);
	rv_status = tmp_msg.createCopy(out_datum);
	if (rv_status != TIBRV_OK) {
		std::ostringstream error_text;
		error_text << "Attempt to get a field (" <<
			FieldNameAndIdToString(field_name, field_id) <<
			") succeeded, but the attempt to 'createCopy()' of the " <<
			"nested message failed.";
		throw RvExceptionStatus(rv_status, error_text);
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get TibrvMsg
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TibrvMsg &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get tibrvMsg
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	tibrvMsg &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_MSG))
		return(false);

	out_datum = msg_field.getData().msg;

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get tibrvMsg
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	tibrvMsg &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get tibrv_bool
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	tibrv_bool &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_BOOL))
		return(false);

	if (msg_field.getType() == TIBRVMSG_BOOL)
		out_datum = msg_field.getData().boolean;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getBool ,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get tibrv_bool
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	tibrv_bool &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get bool
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	bool &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	tibrv_bool tmp_datum;

	if (!GetRvField(msg, field_name, tmp_datum, field_id, required_flag,
		no_conversion_flag))
		return(false);

	out_datum = (tmp_datum != TIBRV_FALSE) ? true : false;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get bool
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	bool &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get signed char
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	signed char &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_I8))
		return(false);

	if (msg_field.getType() == TIBRVMSG_I8)
		out_datum = msg_field.getData().i8;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getI8,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get signed char
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	signed char &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned char
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned char &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_U8))
		return(false);

	if (msg_field.getType() == TIBRVMSG_U8)
		out_datum = msg_field.getData().u8;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getU8,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned char
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned char &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get signed short
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	signed short &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_I16))
		return(false);

	if (msg_field.getType() == TIBRVMSG_I16)
		out_datum = msg_field.getData().i16;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getI16,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get signed short
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	signed short &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned short
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned short &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_U16))
		return(false);

	if (msg_field.getType() == TIBRVMSG_U16)
		out_datum = msg_field.getData().u16;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getU16,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned short
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned short &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get signed int
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	signed int &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_I32))
		return(false);

	if (msg_field.getType() == TIBRVMSG_I32)
		out_datum = msg_field.getData().i32;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getI32,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get signed int
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	signed int &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned int
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned int &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_U32))
		return(false);

	if (msg_field.getType() == TIBRVMSG_U32)
		out_datum = msg_field.getData().u32;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getU32,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned int
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned int &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
//	Get signed long
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	signed long &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_I32))
		return(false);

	out_datum = msg_field.getData().i32;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get signed long
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	signed long &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned long
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned long &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_U32))
		return(false);

	out_datum = msg_field.getData().u32;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned long
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned long &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
//	Get signed 64-bit long
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TIBRV_I64_TYPE &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_I64))
		return(false);

	if (msg_field.getType() == TIBRVMSG_I64)
		out_datum = msg_field.getData().i64;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getI64,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get signed 64-bit long
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TIBRV_I64_TYPE &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned 64-bit long
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned TIBRV_I64_TYPE &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_U64))
		return(false);

	if (msg_field.getType() == TIBRVMSG_U64)
		out_datum = msg_field.getData().u64;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getU64,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get unsigned 64-bit long
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned TIBRV_I64_TYPE &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get float
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	float &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_F32))
		return(false);

	if (msg_field.getType() == TIBRVMSG_F32)
		out_datum = msg_field.getData().f32;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getF32,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get float
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	float &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get double
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	double &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_F64))
		return(false);

	if (msg_field.getType() == TIBRVMSG_F64)
		out_datum = msg_field.getData().f64;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getF64,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get double
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	double &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get tibrv_ipaddr32
bool GetRvFieldIPAddr32(const TibrvMsg &msg, const char *field_name,
	tibrv_ipaddr32 &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_IPADDR32))
		return(false);

	if (msg_field.getType() == TIBRVMSG_IPADDR32)
		out_datum = msg_field.getData().ipaddr32;
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getIPAddr32,
			(field_name, out_datum, field_id));
	}
	
	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
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

	std::ostringstream o_str;

	o_str <<
		static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
			&ip_addr)[0]) << "." <<
		static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
			&ip_addr)[1]) << "." <<
		static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
			&ip_addr)[2]) << "." <<
		static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
			&ip_addr)[3]);

	o_str.str().swap(out_datum);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get tibrv_ipaddr32 as a std::string
bool GetRvFieldIPAddr32(const TibrvMsg &msg, const std::string &field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvFieldIPAddr32(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get tibrv_ipport16
bool GetRvFieldIPPort16(const TibrvMsg &msg, const char *field_name,
	tibrv_ipport16 &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_IPPORT16))
		return(false);

	if (msg_field.getType() == TIBRVMSG_IPPORT16)
		out_datum = ::ntohs(msg_field.getData().ipport16);
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getIPPort16,
			(field_name, out_datum, field_id));
		out_datum = ::ntohs(out_datum);
	}
	
	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Get tibrv_ipport16
bool GetRvFieldIPPort16(const TibrvMsg &msg, const std::string &field_name,
	tibrv_ipport16 &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get char *
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	const char *&out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_STRING))
		return(false);

	if (msg_field.getType() == TIBRVMSG_STRING)
		out_datum = msg_field.getData().str;
	else if (msg_field.getType() == TIBRVMSG_XML)
		out_datum = static_cast<const char *>(msg_field.getData().buf);
	else if (msg_field.getType() == TIBRVMSG_OPAQUE)
		out_datum = static_cast<const char *>(msg_field.getData().buf);
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getString,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get char *
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	const char *&out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get std::string
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	RvMsgSnapshotContext rvmsg_snapshot_context(msg);

	const char *tmp_datum;

	if (!GetRvField(msg, field_name, tmp_datum, field_id, required_flag,
		no_conversion_flag))
		return(false);

	out_datum = tmp_datum;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get std::string
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvField(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get opaque byte string
bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id,
	bool required_flag, bool no_conversion_flag)
{
	return(GetRvFieldOpaque(msg, field_name, out_length, out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get opaque byte string
bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id,
	bool required_flag, bool no_conversion_flag)
{
	return(GetRvFieldOpaque(msg, field_name.c_str(), out_length, out_datum,
		field_id, required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get opaque byte string
bool GetRvFieldOpaque(const TibrvMsg &msg, const char *field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id,
	bool required_flag, bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_OPAQUE))
		return(false);

	out_length = msg_field.getSize();
	out_datum  = msg_field.getData().buf;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get opaque byte string
bool GetRvFieldOpaque(const TibrvMsg &msg, const std::string &field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id,
	bool required_flag, bool no_conversion_flag)
{
	return(GetRvFieldOpaque(msg, field_name.c_str(), out_length, out_datum,
		field_id, required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get opaque byte string
bool GetRvFieldOpaque(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	RvMsgSnapshotContext rvmsg_snapshot_context(msg);

	unsigned int  tmp_length;
	const void   *tmp_datum;

	if (!GetRvFieldOpaque(msg, field_name, tmp_length, tmp_datum, field_id,
		required_flag, no_conversion_flag))
		return(false);

	std::string tmp_string(static_cast<const char *>(tmp_datum), tmp_length);

	out_datum.swap(tmp_string);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get opaque byte string
bool GetRvFieldOpaque(const TibrvMsg &msg, const std::string &field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvFieldOpaque(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get XML byte string
bool GetRvFieldXml(const TibrvMsg &msg, const char *field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id,
	bool required_flag, bool no_conversion_flag)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_XML))
		return(false);

	out_length = msg_field.getSize();
	out_datum  = msg_field.getData().buf;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get XML byte string
bool GetRvFieldXml(const TibrvMsg &msg, const std::string &field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id,
	bool required_flag, bool no_conversion_flag)
{
	return(GetRvFieldXml(msg, field_name.c_str(), out_length, out_datum,
		field_id, required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get XML byte string
bool GetRvFieldXml(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	RvMsgSnapshotContext rvmsg_snapshot_context(msg);

	unsigned int  tmp_length;
	const void   *tmp_datum;

	if (!GetRvFieldXml(msg, field_name, tmp_length, tmp_datum, field_id,
		required_flag, no_conversion_flag))
		return(false);

	std::string tmp_string(static_cast<const char *>(tmp_datum), tmp_length);

	out_datum.swap(tmp_string);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get XML byte string
bool GetRvFieldXml(const TibrvMsg &msg, const std::string &field_name,
	std::string &out_datum, tibrv_u16 field_id, bool required_flag,
	bool no_conversion_flag)
{
	return(GetRvFieldXml(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	std::deque<std::string> tmp_deque;
	GetRvFieldStringList(msg, field_name_count, field_name_data,
		tmp_deque);
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;
	tmp_list.FromContainer(tmp_deque, "Failure in GetRvFieldStringList()");
	std::swap(string_list, tmp_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	GetRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), string_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int *out_count, char ***out_list)
{
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;

	GetRvFieldStringList(msg, field_name_count, field_name_data, tmp_list);

	*out_count = tmp_list.GetCount();
	*out_list  = tmp_list.GetList();

	//	Mustn't try to free the data when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	unsigned int *out_count, char ***out_list)
{
	GetRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), out_count, out_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, char ***out_list)
{
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;

	GetRvFieldStringList(msg, field_name_count, field_name_data, tmp_list);

	//	Extend the list by one in order to add the 'NULL' pointer...
	tmp_list.Realloc(tmp_list.GetCount() + 1,
		"Failure in GetRvFieldStringList()");
	tmp_list.GetList()[tmp_list.GetCount() - 1] = NULL;

	*out_list = tmp_list.GetList();

	//	Mustn't try to free the data when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	char ***out_list)
{
	GetRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), out_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	std::deque<std::string> tmp_deque;
	GetRvFieldXmlList(msg, field_name_count, field_name_data,
		tmp_deque);
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;
	tmp_list.FromContainer(tmp_deque, "Failure in GetRvFieldXmlList()");
	std::swap(string_list, tmp_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	GetRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), string_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int *out_count, char ***out_list)
{
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;

	GetRvFieldXmlList(msg, field_name_count, field_name_data, tmp_list);

	*out_count = tmp_list.GetCount();
	*out_list  = tmp_list.GetList();

	//	Mustn't try to free the data when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	unsigned int *out_count, char ***out_list)
{
	GetRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), out_count, out_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, char ***out_list)
{
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;

	GetRvFieldXmlList(msg, field_name_count, field_name_data, tmp_list);

	//	Extend the list by one in order to add the 'NULL' pointer...
	tmp_list.Realloc(tmp_list.GetCount() + 1,
		"Failure in GetRvFieldXmlList()");
	tmp_list.GetList()[tmp_list.GetCount() - 1] = NULL;

	*out_list = tmp_list.GetList();

	//	Mustn't try to free the data when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	char ***out_list)
{
	GetRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), out_list);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

#ifdef TEST_MAIN

#include <RvUtilX/AddRvField.hpp>

#include <Utility/StringSupport.hpp>

#include <iterator>

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
int TEST_LargeXmlField(int &return_code, unsigned int test_size = 50000000,
	unsigned int test_count = 100)
{
	try {
		{
			std::cout << "TEST_LargeXmlField(): Size = " << test_size <<
				", iterations = " << test_count << std::endl;
			TibrvMsg the_msg;
			std::cout << "TEST_LargeXmlField(): Created message." << std::endl;
			{
				std::string test_buffer(test_size, 'X');
				std::cout << "TEST_LargeXmlField(): Created test buffer." <<
					std::endl;
				AddRvFieldXml(the_msg, "xmlData", test_buffer, 0);
				std::cout << "TEST_LargeXmlField(): Inserted test buffer into "
					"message." << std::endl;
			}
			std::cout << "TEST_LargeXmlField(): Released test buffer." <<
				std::endl;
			std::cout << "Retrieving field " << test_count << " times... " <<
				std::setw(10) << "" << std::flush;
			unsigned int count_1;
			for (count_1 = 0; count_1 < test_count; ++count_1) {
				std::string xml_data;
				GetRvFieldXml(the_msg, "xmlData", xml_data, 0, true, true);
				std::cout << "\b\b\b\b\b\b\b\b\b\b" << std::setw(10) <<
					(count_1 + 1) << std::flush;
			}
			std::cout << std::endl;
			std::cout << "TEST_LargeXmlField(): Loop completed." << std::endl;
		}
		std::cout << "TEST_LargeXmlField(): Test done!!!" << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl <<
			"ERROR: TEST_LargeXmlField() failed: " << except.what() <<
			std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const char *test_type, const char *test_op,
	const std::string &results)
{
	std::cout << "   " << test_type << ": " << test_op << ": " << results <<
		std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(bool ok_flag, const char *test_type, const char *test_op,
	const std::string &results)
{
	TEST_EmitResults(test_type, test_op, "REGRESSION TEST SUCCEEDED: " <<
		results << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const char *test_type, const char *test_op,
	const std::exception &except)
{
	TEST_EmitResults(test_type, test_op, "REGRESSION TEST FAILED   : " +
		std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const char *test_type, const char *test_op)
{
	TEST_EmitResults(test_type, test_op, "REGRESSION TEST SUCCEEDED");
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitResultsImpl(const char *test_op, const std::string &result_type,
	const std::string &results)
{
	std::cout << "   " << std::left << std::setw(26) << test_op << ": " <<
		std::setw(9) << result_type << ": " << results << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const char *test_op, const std::string &results = "")
{
/*
	std::cout << "   " << test_type << ": " << test_op << ": " <<
		"REGRESSION TEST SUCCEEDED: " << results << std::endl;
*/
	TEST_EmitResultsImpl(test_op, "SUCCEEDED", results);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const char *test_op, const char *results)
{
	TEST_EmitResultsImpl(test_op, "SUCCEEDED", results);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitResults(const char *test_op, const std::exception &except)
{
/*
	std::cout << "   " << test_type << ": " << test_op << ": " <<
		"REGRESSION TEST FAILED   : "  << except.what() << endl;
*/
	TEST_EmitResultsImpl(test_op, "FAILED", except.what());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_TypeConversion_TibrvMsgDateTime()
{
	const char *test_type = "TibrvMsgDateTime";

	std::cout << test_type << ":" << std::endl;
	std::cout << MLB::Utility::PadLeft("",
		static_cast<unsigned int>(strlen(test_type)), '-') << std::endl;

	const char *test_op   = "Add";

	try {
		TibrvMsg msg;
		{
			MLB::Utility::TimeSpec in_datum;
			AddRvField(msg, "my_field_name",
				TibrvMsgDateTime(ToTibDateTime(in_datum)));
			TEST_EmitResults(test_op, in_datum.ToString());
		}
		{
			test_op = "Get TibrvMsgDateTime";
			TibrvMsgDateTime out_datum;
			GetRvField(msg, "my_field_name", out_datum, 0, true, true);
			TEST_EmitResults(test_op, FromTibDateTime(out_datum).ToString());
		}
		{
			test_op = "Get tibrvMsgDateTime";
			tibrvMsgDateTime out_datum;
			GetRvField(msg, "my_field_name", out_datum, 0, true, true);
			TEST_EmitResults(test_op, FromTibDateTime(out_datum).ToString());
		}
		{
			test_op = "Get MLB::Utility::TimeSpec";
			MLB::Utility::TimeSpec out_datum;
			GetRvField(msg, "my_field_name", out_datum, 0, true, true);
			TEST_EmitResults(test_op, out_datum.ToString());
		}
		{
			test_op = "Get MLB::Utility::TimeVal";
			MLB::Utility::TimeVal out_datum;
			GetRvField(msg, "my_field_name", out_datum, 0, true, true);
			TEST_EmitResults(test_op,
				MLB::Utility::TimeSpec(out_datum).ToString());
		}
		{
			test_op = "Get std::string";
			std::string out_datum;
			GetRvField(msg, "my_field_name", out_datum, 0, true, false);
			TEST_EmitResults(test_op, out_datum);
		}
	}
	catch (const std::exception &except) {
		TEST_EmitResults(test_op, except);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_TypeConversion_charstring()
{
	const char *test_type = "char *";

	std::cout << test_type << ":" << std::endl;
	std::cout << MLB::Utility::PadLeft("",
		static_cast<unsigned int>(strlen(test_type)), '-') << std::endl;

	const char *test_op   = "";

	try {
		{
			TibrvMsg msg;
			{
				test_op = "Add char *";
				const char *in_datum = "MyStringFieldValue";
				AddRvField(msg, "my_field_name", in_datum);
				TEST_EmitResults(test_op, in_datum);
			}
			{
				test_op = "Get char *";
				const char *out_datum;
				GetRvField(msg, "my_field_name", out_datum, 0, true, true);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get std::string";
				std::string out_datum;
				GetRvField(msg, "my_field_name", out_datum, 0, true, true);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get XML";
				std::string out_datum;
				GetRvFieldXml(msg, "my_field_name", out_datum, 0, true, false);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get Opaque";
				std::string out_datum;
				GetRvFieldOpaque(msg, "my_field_name", out_datum, 0, true, false);
				TEST_EmitResults(test_op, out_datum);
			}
		}
		{
			TibrvMsg msg;
			{
				test_op = "Add XML";
				std::string in_datum("MyStringFieldValue",
					strlen("MyStringFieldValue") + 1);
				AddRvFieldXml(msg, "my_field_name", in_datum);
				TEST_EmitResults(test_op, in_datum);
			}
			{
				test_op = "Get char *";
				const char *out_datum;
				GetRvField(msg, "my_field_name", out_datum, 0, true, false);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get std::string";
				std::string out_datum;
				GetRvField(msg, "my_field_name", out_datum, 0, true, false);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get XML";
				std::string out_datum;
				GetRvFieldXml(msg, "my_field_name", out_datum, 0, true, true);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get Opaque";
				std::string out_datum;
				GetRvFieldOpaque(msg, "my_field_name", out_datum, 0, true, false);
				TEST_EmitResults(test_op, out_datum);
			}
		}
		{
			TibrvMsg msg;
			{
				test_op = "Add Opaque";
				std::string in_datum("MyStringFieldValue",
					strlen("MyStringFieldValue") + 1);
				AddRvFieldOpaque(msg, "my_field_name", in_datum);
				TEST_EmitResults(test_op, in_datum);
			}
			{
				test_op = "Get char *";
				const char *out_datum;
				GetRvField(msg, "my_field_name", out_datum, 0, true, false);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get std::string";
				std::string out_datum;
				GetRvField(msg, "my_field_name", out_datum, 0, true, false);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get XML";
				std::string out_datum;
				GetRvFieldXml(msg, "my_field_name", out_datum, 0, true, false);
				TEST_EmitResults(test_op, out_datum);
			}
			{
				test_op = "Get Opaque";
				std::string out_datum;
				GetRvFieldOpaque(msg, "my_field_name", out_datum, 0, true, true);
				TEST_EmitResults(test_op, out_datum);
			}
		}
	}
	catch (const std::exception &except) {
		TEST_EmitResults(test_op, except);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_TypeConversion()
{
	TEST_TypeConversion_TibrvMsgDateTime();
	TEST_TypeConversion_charstring();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

//	TEST_LargeXmlField(return_code);

TEST_TypeConversion();

	//	String list test.
	try {
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		std::cout << "String List Test" << std::endl;
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		TibrvMsg                   msg;
		unsigned int               count_1;
		MLB::Utility::StringVector src_list;
		for (count_1 = 0; count_1 < 10; ++count_1)
			src_list.push_back(std::string("Test string element number " +
				MLB::Utility::AnyToString(count_1 + 1) + "."));
		AddRvFieldStringList(msg, "stringField", src_list);
		std::cout << "TO STRING: [" << RvMsgToString(msg) << "]" << std::endl;
		MLB::Utility::StringVector dst_list;
		GetRvFieldStringList(msg, "stringField", dst_list);
		std::cout << "FINAL LIST:" << std::endl;
		std::cout << "----- -----" << std::endl;
		std::copy(dst_list.begin(), dst_list.end(),
			std::ostream_iterator<MLB::Utility::StringVector::value_type>(
				std::cout, "\n"));
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl <<
			"ERROR: [Add|Get]RvFieldStringList() failed: " << except.what() <<
			std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	//	XML list test.
	try {
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		std::cout << "XML List Test" << std::endl;
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		TibrvMsg                   msg;
		unsigned int               count_1;
		MLB::Utility::StringVector src_list;
		for (count_1 = 0; count_1 < 10; ++count_1) {
			std::string da_number(MLB::Utility::AnyToString(count_1 + 1));
			src_list.push_back(std::string("\
<TestXml_" + da_number + ">\n\
   <element number " + da_number + ">\n\
      SomeDataHere\n\
   </element number " + da_number + ">\n\
</TestXml_" + da_number + ">"));
		}
		AddRvFieldXmlList(msg, "xmlField", src_list);
		std::cout << "TO STRING: [" << RvMsgToString(msg) << "]" << std::endl;
		MLB::Utility::StringVector dst_list;
		GetRvFieldXmlList(msg, "xmlField", dst_list);
		std::cout << "FINAL LIST:" << std::endl;
		std::cout << "----- -----" << std::endl;
		std::copy(dst_list.begin(), dst_list.end(),
			std::ostream_iterator<MLB::Utility::StringVector::value_type>(
				std::cout, "\n"));
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl <<
			"ERROR: [Add|Get]RvFieldXmlList() failed: " << except.what() <<
			std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

