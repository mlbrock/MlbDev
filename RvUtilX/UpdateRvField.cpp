// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Update message fields.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/UpdateRvField.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
void ThrowIfUpdateRvFieldError(TibrvStatus rv_status, const char *field_name,
	tibrv_u16 field_id)
{
	if (rv_status == TIBRV_ID_CONFLICT) {
		std::ostringstream error_text;
		error_text << "Attempt to update a field (" <<
			FieldNameAndIdToString(field_name, field_id) <<
			") resulted in a name/id conflict";
		MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
	}
	else if (rv_status == TIBRV_ID_IN_USE) {
		std::ostringstream error_text;
		error_text << "Attempt to update a field (" <<
			FieldNameAndIdToString(field_name, field_id) <<
			") resulted in the determination that a field with those " <<
			"indentifiers already exists within the message.";
		MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
	}
	else if (rv_status != TIBRV_OK) {
		std::ostringstream error_text;
		error_text << "Invocation of 'TibrvMsg::updateField()' for " <<
			FieldNameAndIdToString(field_name, field_id) << " failed.";
		MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update TibrvMsgDateTime
void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const TibrvMsgDateTime &value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateDateTime(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update TibrvMsgDateTime
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const TibrvMsgDateTime &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrvMsgDateTime
void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const tibrvMsgDateTime &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name, TibrvMsgDateTime(value), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrvMsgDateTime
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const tibrvMsgDateTime &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update MLB::Utility::TimeSpec
void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const MLB::Utility::TimeSpec &value, tibrv_u16 field_id)
{
	TibrvMsgDateTime tmp_value;

	UpdateRvField(msg, field_name, ToTibDateTime(value, tmp_value), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update MLB::Utility::TimeSpec
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const MLB::Utility::TimeSpec &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update MLB::Utility::TimeVal
void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const MLB::Utility::TimeVal &value, tibrv_u16 field_id)
{
	TibrvMsgDateTime tmp_value;

	UpdateRvField(msg, field_name, ToTibDateTime(value, tmp_value), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update MLB::Utility::TimeVal
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const MLB::Utility::TimeVal &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update TibrvMsg
void UpdateRvField(TibrvMsg &msg, const char *field_name, const TibrvMsg &value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateMsg(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update TibrvMsg
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const TibrvMsg &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrvMsg
void UpdateRvField(TibrvMsg &msg, const char *field_name, const tibrvMsg &value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name, TibrvMsg(value, TIBRV_FALSE), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrvMsg
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const tibrvMsg &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrv_bool
void UpdateRvField(TibrvMsg &msg, const char *field_name, tibrv_bool value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateBool(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrv_bool
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, tibrv_bool value,
	tibrv_u16 field_id)
{
	return(UpdateRvField(msg, field_name.c_str(), value, field_id));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update bool
void UpdateRvField(TibrvMsg &msg, const char *field_name, bool value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateBool(field_name,
		((value) ? TIBRV_TRUE : TIBRV_FALSE), field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update bool
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, bool value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update signed char
void UpdateRvField(TibrvMsg &msg, const char *field_name, signed char value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateI8(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update signed char
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, signed char value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned char
void UpdateRvField(TibrvMsg &msg, const char *field_name, unsigned char value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateU8(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned char
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned char value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update signed short
void UpdateRvField(TibrvMsg &msg, const char *field_name, signed short value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateI16(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update signed short
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, signed short value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned short
void UpdateRvField(TibrvMsg &msg, const char *field_name, unsigned short value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateU16(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned short
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned short value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update signed int
void UpdateRvField(TibrvMsg &msg, const char *field_name, signed int value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateI32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update signed int
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, signed int value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned int
void UpdateRvField(TibrvMsg &msg, const char *field_name, unsigned int value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateU32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned int
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned int value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
//	Update signed long
void UpdateRvField(TibrvMsg &msg, const char *field_name, signed long value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateI32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update signed long
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, signed long value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned long
void UpdateRvField(TibrvMsg &msg, const char *field_name, unsigned long value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateU32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned long
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned long value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
//	Update signed 64-bit long
void UpdateRvField(TibrvMsg &msg, const char *field_name, TIBRV_I64_TYPE value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateI64(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update signed 64-bit long
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	TIBRV_I64_TYPE value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned 64-bit long
void UpdateRvField(TibrvMsg &msg, const char *field_name,
	unsigned TIBRV_I64_TYPE value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateU64(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update unsigned 64-bit long
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned TIBRV_I64_TYPE value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update float
void UpdateRvField(TibrvMsg &msg, const char *field_name, float value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateF32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update float
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, float value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update double
void UpdateRvField(TibrvMsg &msg, const char *field_name, double value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateF64(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update double
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, double value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrv_ipaddr32
void UpdateRvFieldIPAddr32(TibrvMsg &msg, const char *field_name,
	const tibrv_ipaddr32 &value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateIPAddr32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrv_ipaddr32
void UpdateRvFieldIPAddr32(TibrvMsg &msg, const std::string &field_name,
	const tibrv_ipaddr32 &value, tibrv_u16 field_id)
{
	UpdateRvFieldIPAddr32(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrv_ipport16
void UpdateRvFieldIPPort16(TibrvMsg &msg, const char *field_name,
	const tibrv_ipport16 &value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateIPPort16 (field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update tibrv_ipport16
void UpdateRvFieldIPPort16(TibrvMsg &msg, const std::string &field_name,
	const tibrv_ipport16 &value, tibrv_u16 field_id)
{
	UpdateRvFieldIPPort16(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update char *
void UpdateRvField(TibrvMsg &msg, const char *field_name, const char *value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateString(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update char *
void UpdateRvField(TibrvMsg &msg, const std::string &field_name, const char *value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update std::string
void UpdateRvField(TibrvMsg &msg, const char *field_name, const std::string &value,
	tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name, value.c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update std::string
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value.c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update std::ostringstream
void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name, value.str().c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update std::ostringstream
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), value.str().c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update opaque byte string
void UpdateRvField(TibrvMsg &msg, const char *field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	UpdateRvFieldOpaque(msg, field_name, length, value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update opaque byte string
void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	UpdateRvField(msg, field_name.c_str(), length, value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update opaque byte string
void UpdateRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateOpaque(field_name, value, length, field_id)) !=
		TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update opaque byte string
void UpdateRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	UpdateRvFieldOpaque(msg, field_name.c_str(), length, value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update opaque byte string
void UpdateRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	const std::string &value, tibrv_u16 field_id)
{
	UpdateRvFieldOpaque(msg, field_name, static_cast<unsigned int>(value.size()),
		value.c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update opaque byte string
void UpdateRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id)
{
	UpdateRvFieldOpaque(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update opaque byte string
void UpdateRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	UpdateRvFieldOpaque(msg, field_name, value.str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update opaque byte string
void UpdateRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	UpdateRvFieldOpaque(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update XML byte string
void UpdateRvFieldXml(TibrvMsg &msg, const char *field_name, unsigned int length,
	const void *value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateXml(field_name, value, length, field_id)) !=
		TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update XML byte string
void UpdateRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	UpdateRvFieldXml(msg, field_name.c_str(), length, value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update XML byte string
void UpdateRvFieldXml(TibrvMsg &msg, const char *field_name, const char *value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateXml(field_name, value,
		static_cast<unsigned int>(strlen(value)), field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update XML byte string
void UpdateRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const char *value, tibrv_u16 field_id)
{
	UpdateRvFieldXml(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update XML byte string
void UpdateRvFieldXml(TibrvMsg &msg, const char *field_name,
	const std::string &value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateXml(field_name, value.c_str(),
		static_cast<unsigned int>(value.size()), field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update XML byte string
void UpdateRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id)
{
	UpdateRvFieldXml(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update XML byte string
void UpdateRvFieldXml(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	UpdateRvFieldXml(msg, field_name, value.str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update XML byte string
void UpdateRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	UpdateRvFieldXml(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	std::deque<std::string> tmp_deque;
	string_list.ToContainer(tmp_deque, "Failure in UpdateRvFieldStringList()");
	UpdateRvFieldStringList(msg, field_name_count, field_name_data, tmp_deque);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	UpdateRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), string_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int in_count, char **in_list)
{
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;

	tmp_list.SetCount(in_count);
	tmp_list.SetList(in_list);
	UpdateRvFieldStringList(msg, field_name_count, field_name_data, tmp_list);

	//	Mustn't try to free the data when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	unsigned int in_count, char **in_list)
{
	UpdateRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), in_count, in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, char **in_list)
{
	unsigned int   in_count = 0;
	char         **tmp_ptr  = in_list;

	while (*tmp_ptr != NULL) {
		++in_count;
		++tmp_ptr;
	}

	UpdateRvFieldStringList(msg, field_name_count, field_name_data, in_count,
		in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	char **in_list)
{
	UpdateRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	std::deque<std::string> tmp_deque;
	string_list.ToContainer(tmp_deque, "Failure in UpdateRvFieldXmlList()");
	UpdateRvFieldXmlList(msg, field_name_count, field_name_data, tmp_deque);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	UpdateRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), string_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int in_count, char **in_list)
{
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;

	tmp_list.SetCount(in_count);
	tmp_list.SetList(in_list);
	UpdateRvFieldXmlList(msg, field_name_count, field_name_data, tmp_list);

	//	Mustn't try to free the data when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	unsigned int in_count, char **in_list)
{
	UpdateRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), in_count, in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, char **in_list)
{
	unsigned int   in_count = 0;
	char         **tmp_ptr  = in_list;

	while (*tmp_ptr != NULL) {
		++in_count;
		++tmp_ptr;
	}

	UpdateRvFieldXmlList(msg, field_name_count, field_name_data, in_count,
		in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	char **in_list)
{
	UpdateRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), in_list);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::RvUtilX;

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TibrvMsg msg;
		bool     a_bool = true;
		UpdateRvField(msg, "A", a_bool, 1);
		UpdateRvField(msg, "A", a_bool, 1);
		const char *to_string;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg.convertToString,
			(to_string));
		std::cout << "TO STRING: [" << to_string << "]" << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl <<
			"ERROR: " << except.what() << std::endl << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

