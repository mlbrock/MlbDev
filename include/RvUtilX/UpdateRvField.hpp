// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for RvUtilX UpdateRvField() functions.

	Revision History	:	2007-02-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_UpdateRvField_hpp__HH

#define HH__MLB__RvUtilX_UpdateRvField_hpp__HH	1

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
//	////////////////////////////////////////////////////////////////////////////
//	Support function for the 'UpdateRvField()' family of functions...
//	////////////////////////////////////////////////////////////////////////////
API_RVUTILX void ThrowIfUpdateRvFieldError(TibrvStatus rv_status,
	const char *field_name, tibrv_u16 field_id);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Update TibrvMsgDateTime
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const TibrvMsgDateTime &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const TibrvMsgDateTime &value, tibrv_u16 field_id = 0);
//	Update tibrvMsgDateTime
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const tibrvMsgDateTime &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const tibrvMsgDateTime &value, tibrv_u16 field_id = 0);

// Update MLB::Utility::TimeSpec
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const MLB::Utility::TimeSpec &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const MLB::Utility::TimeSpec &value, tibrv_u16 field_id = 0);

// Update MLB::Utility::TimeVal
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const MLB::Utility::TimeVal &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const MLB::Utility::TimeVal &value, tibrv_u16 field_id = 0);

//	Update TibrvMsg
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const TibrvMsg &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const TibrvMsg &value, tibrv_u16 field_id = 0);
//	Update tibrvMsg
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const tibrvMsg &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const tibrvMsg &value, tibrv_u16 field_id = 0);

//	Update tibrv_bool
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	tibrv_bool value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	tibrv_bool value, tibrv_u16 field_id = 0);

//	Update bool
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name, bool value,
	tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	bool value, tibrv_u16 field_id = 0);

//	Update signed char
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	signed char value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	signed char value, tibrv_u16 field_id = 0);
//	Update unsigned char
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	unsigned char value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned char value, tibrv_u16 field_id = 0);

//	Update signed short
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	signed short value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	signed short value, tibrv_u16 field_id = 0);
//	Update unsigned short
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	unsigned short value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned short value, tibrv_u16 field_id = 0);

//	Update signed int
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	signed int value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	signed int value, tibrv_u16 field_id = 0);
//	Update unsigned int
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	unsigned int value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned int value, tibrv_u16 field_id = 0);
/*
//	Update signed long
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	signed long value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	signed long value, tibrv_u16 field_id = 0);
//	Update unsigned long
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	unsigned long value, tibrv_u16 field_id = 0);
//	Update unsigned long
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned long value, tibrv_u16 field_id = 0);
*/

//	Update signed 64-bit long
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	TIBRV_I64_TYPE value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	TIBRV_I64_TYPE value, tibrv_u16 field_id = 0);
//	Update unsigned 64-bit long
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	unsigned TIBRV_I64_TYPE value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned TIBRV_I64_TYPE value, tibrv_u16 field_id = 0);

//	Update float
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name, float value,
	tibrv_u16 field_id = 0);
//	Update float
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	float value, tibrv_u16 field_id = 0);

//	Update double
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name, double value,
	tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	double value, tibrv_u16 field_id = 0);

//	Update tibrv_ipaddr32
API_RVUTILX void UpdateRvFieldIPUpdater32(TibrvMsg &msg, const char *field_name,
	const tibrv_ipaddr32 &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldIPUpdater32(TibrvMsg &msg, const std::string &field_name,
	const tibrv_ipaddr32 &value, tibrv_u16 field_id = 0);

//	Update tibrv_ipport16
API_RVUTILX void UpdateRvFieldIPPort16(TibrvMsg &msg, const char *field_name,
	const tibrv_ipport16 &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldIPPort16(TibrvMsg &msg, const std::string &field_name,
	const tibrv_ipport16 &value, tibrv_u16 field_id = 0);

//	Update char *
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const char *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const char *value, tibrv_u16 field_id = 0);
//	Update std::string
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const std::string &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id = 0);
//	Update std::ostringstream
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id = 0);

//	Update opaque byte string
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const char *field_name,
	unsigned int length, const void *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvField(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	unsigned int length, const void *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	const std::string &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldOpaque(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldOpaque(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id = 0);

//	Update XML byte string
API_RVUTILX void UpdateRvFieldXml(TibrvMsg &msg, const char *field_name,
	unsigned int length, const void *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	unsigned int length, const void *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldXml(TibrvMsg &msg, const char *field_name,
	const char *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const char *value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldXml(TibrvMsg &msg, const char *field_name,
	const std::string &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const std::string &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldXml(TibrvMsg &msg, const char *field_name,
	const std::ostringstream &value, tibrv_u16 field_id = 0);
API_RVUTILX void UpdateRvFieldXml(TibrvMsg &msg, const std::string &field_name,
	const std::ostringstream &value, tibrv_u16 field_id = 0);

//	Update string list
API_RVUTILX void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list);
API_RVUTILX void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list);
API_RVUTILX void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int in_count, char **in_list);
API_RVUTILX void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	unsigned int in_count, char **in_list);
API_RVUTILX void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	char *field_name_data, char **in_list);
API_RVUTILX void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	char **in_list);

//	Update XML list
API_RVUTILX void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list);
API_RVUTILX void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	const MLB::Utility::C_ListTypeString<unsigned int> &string_list);
API_RVUTILX void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, unsigned int in_count, char **in_list);
API_RVUTILX void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	unsigned int in_count, char **in_list);
API_RVUTILX void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	char *field_name_data, char **in_list);
API_RVUTILX void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	char **in_list);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update enumeration
template <typename EnumType> void UpdateRvFieldEnum(TibrvMsg &msg,
	const char *field_name, EnumType value, tibrv_u16 field_id = 0)
{
	TibrvStatus rv_status;

	if ((rv_status = msg.updateI32(field_name, value, field_id)) != TIBRV_OK)
		ThrowIfUpdateRvFieldError(rv_status, field_name, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Update enumeration
template <typename EnumType> void UpdateRvFieldEnum(TibrvMsg &msg,
	const std::string &field_name, EnumType value, tibrv_u16 field_id = 0)
{
	UpdateRvFieldEnum(msg, field_name.c_str(), value, field_id);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, const ContainerType &container)
{
	try {
		UpdateRvFieldPackedList(false, msg, field_name_count, field_name_data,
			container);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failure in UpdateRvFieldStringList(): " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void UpdateRvFieldStringList(TibrvMsg &msg, const char *field_name_base,
	const ContainerType &container)
{
	UpdateRvFieldStringList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), container);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_count,
	const char *field_name_data, const ContainerType &container)
{
	try {
		UpdateRvFieldPackedList(true, msg, field_name_count, field_name_data,
			container);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failure in UpdateRvFieldXmlList(): " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void UpdateRvFieldXmlList(TibrvMsg &msg, const char *field_name_base,
	const ContainerType &container)
{
	UpdateRvFieldXmlList(msg, ListBaseNameToCountName(field_name_base).c_str(),
		ListBaseNameToDataName(field_name_base).c_str(), container);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Not using boost::shared_array<> to avoid bringing in the includes...
template <typename ContainerType>
	void UpdateRvFieldPackedList(bool is_xml_field, TibrvMsg &msg,
	const char *field_name_count, const char *field_name_data,
	const ContainerType &container)
{
	unsigned int *length_list = NULL;
	char         *string_data = NULL;

	try {
		unsigned int in_count     = static_cast<unsigned int>(container.size());
		unsigned int total_length = 1;
		unsigned int count_1      = 0;
		length_list = static_cast<unsigned int *>(
			MLB::Utility::C_calloc(in_count + 1, sizeof(unsigned int)));
		length_list[0] = in_count;
		ContainerType::const_iterator iter_b(container.begin());
		ContainerType::const_iterator iter_e(container.end());
		count_1 = 0;
		while (iter_b != iter_e) {
			length_list[count_1 + 1]  = static_cast<unsigned int>(iter_b->size());
			total_length             += length_list[count_1 + 1] + 1;
			++count_1;
			++iter_b;
		}
		string_data = static_cast<char *>(MLB::Utility::C_calloc(total_length,
			sizeof(char)));
		unsigned int this_offset = 0;
		count_1 = 0;
		iter_b  = container.begin();
		iter_e  = container.end();
		while (iter_b != iter_e) {
			strcpy(string_data + this_offset, iter_b->c_str());
			this_offset += length_list[count_1 + 1] + 1;
			++count_1;
			++iter_b;
			//	If it's a list of XML strings, we terminate all but the last one
			//	with a line-feed. Otherwise, the elements implicitly get an ASCII
			// NUL as a result of the memory being calloc()ed...
			if (is_xml_field && (count_1 < in_count))
				string_data[this_offset - 1] = '\n';
		}
		//	CODE NOTE: To be replaced with the array func TBI.
		RvUtilX_THROW_TIBRV_STATUS_IF(msg.updateU32Array,
			(field_name_count, length_list, in_count + 1));
		try {
			if (is_xml_field)
				UpdateRvFieldXml(msg, field_name_data, string_data);
			else
				UpdateRvFieldOpaque(msg, field_name_data, total_length, string_data);
		}
		catch (const std::exception &) {
			msg.removeField(field_name_count);
			throw;
		}
		free(length_list);
		free(string_data);
	}
	catch (const std::exception &except) {
		if (length_list != NULL)
			free(length_list);
		if (string_data != NULL)
			free(string_data);
		MLB::Utility::Rethrow(except, "Unable to insert a packed " +
			std::string((is_xml_field) ? "XML" : "string") +
			" list into an Rv message: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_UpdateRvField_hpp__HH

