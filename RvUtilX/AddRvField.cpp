// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Add message fields.

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

#include <RvUtilX/AddRvField.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
void ThrowIfAddRvFieldError(TibrvStatus rv_status, const char *field_name,
	tibrv_u16 field_id)
{
	if (rv_status == TIBRV_ID_CONFLICT) {
		std::ostringstream error_text;
		error_text << "Attempt to add a field (" <<
			FieldNameAndIdToString(field_name, field_id) <<
			") resulted in a name/id conflict";
		MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
	}
	else if (rv_status == TIBRV_ID_IN_USE) {
		std::ostringstream error_text;
		error_text << "Attempt to add a field (" <<
			FieldNameAndIdToString(field_name, field_id) <<
			") resulted in the determination that a field with those " <<
			"indentifiers already exists within the message.";
		MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
	}
	else if (rv_status != TIBRV_OK) {
		std::ostringstream error_text;
		error_text << "Invocation of 'TibrvMsg::addField()' for " <<
			FieldNameAndIdToString(field_name, field_id) << " failed.";
		MLB::RvUtilX::ThrowTibrvStatus(rv_status, error_text);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add TibrvMsgDateTime
void AddRvField(TibrvMsg &msg, const char *field_name,
	const TibrvMsgDateTime &value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addDateTime(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add TibrvMsgDateTime
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	const TibrvMsgDateTime &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrvMsgDateTime
void AddRvField(TibrvMsg &msg, const char *field_name,
	const tibrvMsgDateTime &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name, TibrvMsgDateTime(value), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrvMsgDateTime
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	const tibrvMsgDateTime &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add MLB::Utility::TimeSpec
void AddRvField(TibrvMsg &msg, const char *field_name,
	const MLB::Utility::TimeSpec &value, tibrv_u16 field_id)
{
	TibrvMsgDateTime tmp_value;

	AddRvField(msg, field_name, ToTibDateTime(value, tmp_value), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add MLB::Utility::TimeSpec
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	const MLB::Utility::TimeSpec &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add MLB::Utility::TimeVal
void AddRvField(TibrvMsg &msg, const char *field_name,
	const MLB::Utility::TimeVal &value, tibrv_u16 field_id)
{
	TibrvMsgDateTime tmp_value;

	AddRvField(msg, field_name, ToTibDateTime(value, tmp_value), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add MLB::Utility::TimeVal
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	const MLB::Utility::TimeVal &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add TibrvMsg
void AddRvField(TibrvMsg &msg, const char *field_name, const TibrvMsg &value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addMsg(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add TibrvMsg
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	const TibrvMsg &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrvMsg
void AddRvField(TibrvMsg &msg, const char *field_name, const tibrvMsg &value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name, TibrvMsg(value, TIBRV_FALSE), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrvMsg
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	const tibrvMsg &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrv_bool
void AddRvField(TibrvMsg &msg, const char *field_name, tibrv_bool value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addBool(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrv_bool
void AddRvField(TibrvMsg &msg, const std::string &field_name, tibrv_bool value,
	tibrv_u16 field_id)
{
	return(AddRvField(msg, field_name.c_str(), value, field_id));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add bool
void AddRvField(TibrvMsg &msg, const char *field_name, bool value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addBool(field_name,
		((value) ? TIBRV_TRUE : TIBRV_FALSE), field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add bool
void AddRvField(TibrvMsg &msg, const std::string &field_name, bool value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add signed char
void AddRvField(TibrvMsg &msg, const char *field_name, signed char value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addI8(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add signed char
void AddRvField(TibrvMsg &msg, const std::string &field_name, signed char value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned char
void AddRvField(TibrvMsg &msg, const char *field_name, unsigned char value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addU8(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned char
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned char value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add signed short
void AddRvField(TibrvMsg &msg, const char *field_name, signed short value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addI16(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add signed short
void AddRvField(TibrvMsg &msg, const std::string &field_name, signed short value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned short
void AddRvField(TibrvMsg &msg, const char *field_name, unsigned short value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addU16(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned short
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned short value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add signed int
void AddRvField(TibrvMsg &msg, const char *field_name, signed int value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addI32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add signed int
void AddRvField(TibrvMsg &msg, const std::string &field_name, signed int value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned int
void AddRvField(TibrvMsg &msg, const char *field_name, unsigned int value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addU32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned int
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned int value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
//	Add signed long
void AddRvField(TibrvMsg &msg, const char *field_name, signed long value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addI32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add signed long
void AddRvField(TibrvMsg &msg, const std::string &field_name, signed long value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned long
void AddRvField(TibrvMsg &msg, const char *field_name, unsigned long value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addU32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned long
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned long value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
//	Add signed 64-bit long
void AddRvField(TibrvMsg &msg, const char *field_name, TIBRV_I64_TYPE value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addI64(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add signed 64-bit long
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	TIBRV_I64_TYPE value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned 64-bit long
void AddRvField(TibrvMsg &msg, const char *field_name,
	unsigned TIBRV_I64_TYPE value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addU64(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add unsigned 64-bit long
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned TIBRV_I64_TYPE value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add float
void AddRvField(TibrvMsg &msg, const char *field_name, float value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addF32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add float
void AddRvField(TibrvMsg &msg, const std::string &field_name, float value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add double
void AddRvField(TibrvMsg &msg, const char *field_name, double value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addF64(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add double
void AddRvField(TibrvMsg &msg, const std::string &field_name, double value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrv_ipaddr32
void AddRvFieldIPAddr32(TibrvMsg &msg, const char *field_name,
	const tibrv_ipaddr32 &value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addIPAddr32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrv_ipaddr32
void AddRvFieldIPAddr32(TibrvMsg &msg, const std::string &field_name,
	const tibrv_ipaddr32 &value, tibrv_u16 field_id)
{
	AddRvFieldIPAddr32(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrv_ipport16
void AddRvFieldIPPort16(TibrvMsg &msg, const char *field_name,
	const tibrv_ipport16 &value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addIPPort16 (field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add tibrv_ipport16
void AddRvFieldIPPort16(TibrvMsg &msg, const std::string &field_name,
	const tibrv_ipport16 &value, tibrv_u16 field_id)
{
	AddRvFieldIPPort16(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add char *
void AddRvField(TibrvMsg &msg, const char *field_name, const char *value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addString(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add char *
void AddRvField(TibrvMsg &msg, const std::string &field_name, const char *value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add std::string
void AddRvField(TibrvMsg &msg, const char *field_name, const std::string &value,
	tibrv_u16 field_id)
{
	AddRvField(msg, field_name, value.c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add std::string
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value.c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add std::ostringstream
void AddRvField(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name, value.str().c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add std::ostringstream
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), value.str().c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add opaque byte string
void AddRvField(TibrvMsg &msg, const char *field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	AddRvFieldOpaque(msg, field_name, length, value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add opaque byte string
void AddRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	AddRvField(msg, field_name.c_str(), length, value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add opaque byte string
void AddRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addOpaque(field_name, value, length, field_id)) !=
		TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add opaque byte string
void AddRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	AddRvFieldOpaque(msg, field_name.c_str(), length, value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add opaque byte string
void AddRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	const std::string &value, tibrv_u16 field_id)
{
	AddRvFieldOpaque(msg, field_name, static_cast<unsigned int>(value.size()),
		value.c_str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add opaque byte string
void AddRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id)
{
	AddRvFieldOpaque(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add opaque byte string
void AddRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	AddRvFieldOpaque(msg, field_name, value.str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add opaque byte string
void AddRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	AddRvFieldOpaque(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add XML byte string
void AddRvFieldXml(TibrvMsg &msg, const char *field_name, unsigned int length,
	const void *value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addXml(field_name, value, length, field_id)) !=
		TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add XML byte string
void AddRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id)
{
	AddRvFieldXml(msg, field_name.c_str(), length, value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add XML byte string
void AddRvFieldXml(TibrvMsg &msg, const char *field_name, const char *value,
	tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addXml(field_name, value,
		static_cast<unsigned int>(strlen(value)), field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add XML byte string
void AddRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const char *value, tibrv_u16 field_id)
{
	AddRvFieldXml(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add XML byte string
void AddRvFieldXml(TibrvMsg &msg, const char *field_name,
	const std::string &value, tibrv_u16 field_id)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.addXml(field_name, value.c_str(),
		static_cast<unsigned int>(value.size()), field_id)) != TIBRV_OK)
		ThrowIfAddRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add XML byte string
void AddRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id)
{
	AddRvFieldXml(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add XML byte string
void AddRvFieldXml(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	AddRvFieldXml(msg, field_name, value.str(), field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Add XML byte string
void AddRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id)
{
	AddRvFieldXml(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	std::deque<std::string> tmp_deque;
	string_list.ToContainer(tmp_deque, "Failure in AddRvFieldStringList()");
	AddRvFieldStringList(msg, field_name_count, field_name_data, tmp_deque);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	AddRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), string_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int in_count, char **in_list)
{
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;

	tmp_list.SetCount(in_count);
	tmp_list.SetList(in_list);
	AddRvFieldStringList(msg, field_name_count, field_name_data, tmp_list);

	//	Mustn't try to free the data when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	unsigned int in_count, char **in_list)
{
	AddRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), in_count, in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, char **in_list)
{
	unsigned int   in_count = 0;
	char         **tmp_ptr  = in_list;

	while (*tmp_ptr != NULL) {
		++in_count;
		++tmp_ptr;
	}

	AddRvFieldStringList(msg, field_name_count, field_name_data, in_count,
		in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	char **in_list)
{
	AddRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	std::deque<std::string> tmp_deque;
	string_list.ToContainer(tmp_deque, "Failure in AddRvFieldXmlList()");
	AddRvFieldXmlList(msg, field_name_count, field_name_data, tmp_deque);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list)
{
	AddRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), string_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int in_count, char **in_list)
{
	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;

	tmp_list.SetCount(in_count);
	tmp_list.SetList(in_list);
	AddRvFieldXmlList(msg, field_name_count, field_name_data, tmp_list);

	//	Mustn't try to free the data when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	unsigned int in_count, char **in_list)
{
	AddRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), in_count, in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, char **in_list)
{
	unsigned int   in_count = 0;
	char         **tmp_ptr  = in_list;

	while (*tmp_ptr != NULL) {
		++in_count;
		++tmp_ptr;
	}

	AddRvFieldXmlList(msg, field_name_count, field_name_data, in_count,
		in_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void AddRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	char **in_list)
{
	AddRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), in_list);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

#ifdef TEST_MAIN

#include <RvUtilX/RvMsg.hpp>

using namespace MLB::RvUtilX;

//	////////////////////////////////////////////////////////////////////////////
char         *TEST_StringList[] = {
	"The first string.",
	"The second string.",
	"The last of three strings."
};
const unsigned int  TEST_StringCount = sizeof(TEST_StringList) /
	sizeof(TEST_StringList);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TibrvMsg msg;
		bool     a_bool = true;
		AddRvField(msg, "A", a_bool, 1);
		AddRvField(msg, "A", a_bool, 2);
		AddRvFieldStringList(msg, "TestCountField", "TestDataField",
			TEST_StringCount, TEST_StringList);
		const char *to_string;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg.convertToString,
			(to_string));
		std::cout << "TO STRING A: [" << to_string << "]" << std::endl;
		std::cout << "TO STRING B: [" << RvMsgToString(msg) << std::endl;
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

