// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for RvUtilX GetRvField() functions.

	Revision History	:	2007-02-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_GetRvField_hpp__HH

#define HH__MLB__RvUtilX_GetRvField_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
// Generic GetRvField() support
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TibrvMsgField &msg_field, tibrv_u16 field_id = 0, bool required_flag = true,
	tibrv_u8 required_type = TIBRVMSG_NO_TAG);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TibrvMsgField &msg_field, tibrv_u16 field_id = 0, bool required_flag = true,
	int required_type = TIBRVMSG_NO_TAG);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TibrvMsgField &msg_field, tibrv_u16 field_id = 0, bool required_flag = true,
	tibrv_u8 required_type = TIBRVMSG_NO_TAG);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TibrvMsgField &msg_field, tibrv_u16 field_id = 0, bool required_flag = true,
	int required_type = TIBRVMSG_NO_TAG);

// Get TibrvMsgDateTime
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TibrvMsgDateTime &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TibrvMsgDateTime &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
// Get tibrvMsgDateTime
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	tibrvMsgDateTime &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	tibrvMsgDateTime &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);

// Get MLB::Utility::TimeSpec
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	MLB::Utility::TimeSpec &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	MLB::Utility::TimeSpec &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);

// Get MLB::Utility::TimeVal
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	MLB::Utility::TimeVal &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	MLB::Utility::TimeVal &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);

// Get TibrvMsg
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TibrvMsg &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TibrvMsg &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
// Get tibrvMsg
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	tibrvMsg &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	tibrvMsg &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get tibrv_bool
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	tibrv_bool &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	tibrv_bool &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get bool
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	bool &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	bool &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get signed char
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	signed char &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	signed char &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
//	Get unsigned char
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned char &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned char &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get signed short
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	signed short &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	signed short &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
//	Get unsigned short
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned short &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned short &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get signed int
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	signed int &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	signed int &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
//	Get unsigned int
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned int &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned int &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

/*
//	Get signed long
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	signed long &out_datum, tibrv_u16 field_id, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	signed long &out_datum, tibrv_u16 field_id, bool required_flag = true,
	bool no_conversion_flag = true);
//	Get unsigned long
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned long &out_datum, tibrv_u16 field_id, bool required_flag = true,
	bool no_conversion_flag = true);
//	Get unsigned long
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned long &out_datum, tibrv_u16 field_id, bool required_flag = true,
	bool no_conversion_flag = true);
*/

//	Get signed 64-bit long
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	TIBRV_I64_TYPE &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	TIBRV_I64_TYPE &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
//	Get unsigned 64-bit long
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned TIBRV_I64_TYPE &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned TIBRV_I64_TYPE &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);

//	Get float
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	float &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	float &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get double
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	double &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	double &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

// Get tibrv_ipaddr32
API_RVUTILX bool GetRvFieldIPAddr32(const TibrvMsg &msg, const char *field_name,
	tibrv_ipaddr32 &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldIPAddr32(const TibrvMsg &msg,
	const std::string &field_name, tibrv_ipaddr32 &out_datum,
	tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldIPAddr32(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldIPAddr32(const TibrvMsg &msg,
	const std::string &field_name, std::string &out_datum,
	tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

// Get tibrv_ipport16
API_RVUTILX bool GetRvFieldIPPort16(const TibrvMsg &msg, const char *field_name,
	tibrv_ipport16 &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldIPPort16(const TibrvMsg &msg,
	const std::string &field_name, tibrv_ipport16 &out_datum,
	tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get char *
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	const char *&out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	const char *&out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get std::string
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	std::string &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get opaque byte string
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const char *field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvField(const TibrvMsg &msg, const std::string &field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldOpaque(const TibrvMsg &msg, const char *field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldOpaque(const TibrvMsg &msg, const std::string &field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldOpaque(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldOpaque(const TibrvMsg &msg, const std::string &field_name,
	std::string &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get XML byte string
API_RVUTILX bool GetRvFieldXml(const TibrvMsg &msg, const char *field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldXml(const TibrvMsg &msg, const std::string &field_name,
	unsigned int &out_length, const void *&out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldXml(const TibrvMsg &msg, const char *field_name,
	std::string &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true);
API_RVUTILX bool GetRvFieldXml(const TibrvMsg &msg, const std::string &field_name,
	std::string &out_datum, tibrv_u16 field_id = 0, bool required_flag = true,
	bool no_conversion_flag = true);

//	Get string list
API_RVUTILX void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	MLB::Utility::C_ListTypeString<unsigned int> &string_list);
API_RVUTILX void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	MLB::Utility::C_ListTypeString<unsigned int> &string_list);
API_RVUTILX void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int *out_count, char ***out_list);
API_RVUTILX void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	unsigned int *out_count, char ***out_list);
API_RVUTILX void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, char ***out_list);
API_RVUTILX void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	char ***out_list);

//	Get XML list
API_RVUTILX void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	MLB::Utility::C_ListTypeString<unsigned int> &string_list);
API_RVUTILX void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	MLB::Utility::C_ListTypeString<unsigned int> &string_list);
API_RVUTILX void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int *out_count, char ***out_list);
API_RVUTILX void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	unsigned int *out_count, char ***out_list);
API_RVUTILX void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, char ***out_list);
API_RVUTILX void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	char ***out_list);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get enumeration
template <typename EnumType> bool GetRvFieldEnum(const TibrvMsg &msg,
	const char *field_name, EnumType &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true)
{
	TibrvMsgField msg_field;

	if (!GetRvField(msg, field_name, msg_field, field_id, required_flag,
		(!no_conversion_flag) ? TIBRVMSG_NO_TAG : TIBRVMSG_I32))
		return(false);

	if (msg_field.getType() == TIBRVMSG_BOOL)
		out_datum = static_cast<EnumType>(msg_field.getData().i32);
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(msg.getI32,
			(field_name, out_datum, field_id));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Get enumeration
template <typename EnumType> bool GetRvFieldEnum(const TibrvMsg &msg,
	const std::string &field_name, EnumType &out_datum, tibrv_u16 field_id = 0,
	bool required_flag = true, bool no_conversion_flag = true)
{
	return(GetRvFieldEnum(msg, field_name.c_str(), out_datum, field_id,
		required_flag, no_conversion_flag));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, ContainerType &container)
{
	try {
		GetRvFieldPackedList(false, msg, field_name_count, field_name_data,
			container);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failure in GetRvFieldStringList(): " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void GetRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	ContainerType &container)
{
	GetRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), container);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, ContainerType &container)
{
	try {
		GetRvFieldPackedList(true, msg, field_name_count, field_name_data,
			container);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failure in GetRvFieldXmlList(): " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void GetRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	ContainerType &container)
{
	GetRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), container);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void GetRvFieldPackedList(bool is_xml_field, TibrvMsg &msg,
	const char *field_name_count, const char *field_name_data,
	ContainerType &container)
{
	try {
		RvMsgSnapshotContext rvmsg_snapshot_context(msg);
		//	Get the array of lengths...
		const tibrv_u32 *length_list;
		unsigned int     length_count;
		//	CODE NOTE: To be replaced with the array func TBI.
		RvUtilX_THROW_TIBRV_STATUS_IF(msg.getU32Array,
			(field_name_count, length_list, length_count));
		//	Check the array of lengths...
		if (!length_count)
			throw RvException("The length array contains no elements.");
		if (length_count != (length_list[0] + 1)) {
			std::ostringstream error_text;
			error_text << "The number of elements in the length array (" <<
				length_count << ") is not equal to the value of the first " <<
				"element of the length array + 1 (" << length_list[0] + " + 1 = " <<
				(length_list[0] + 1) << ").";
			throw RvException(error_text);
		}
		//	Get the data...
		const void   *string_data;
		unsigned int  string_length;
		if (is_xml_field)
			GetRvFieldXml(msg, field_name_data, string_length, string_data);
		else
			GetRvFieldOpaque(msg, field_name_data, string_length, string_data);
		//	Insert the data into the container...
		const char    *tmp_ptr = static_cast<const char *>(string_data);
		unsigned int   count_1;
		ContainerType  tmp_container;
		for (count_1 = 0; count_1 < length_list[0]; ++count_1) {
			tmp_container.push_back(std::string(tmp_ptr,
				length_list[count_1 + 1]));
			tmp_ptr += length_list[count_1 + 1] + 1;
		}
		//	Swap the data from the message into the destination container...
		std::swap(container, tmp_container);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to extract a packed " +
			std::string((is_xml_field) ? "XML" : "string") +
			" list from an Rv message: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_GetRvField_hpp__HH

