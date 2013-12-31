// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts message field data to strings.

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

#include <RvUtilX.hpp>
#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

namespace {
//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string RvMsgFieldDataArrayToString(tibrv_u32 in_count,
	const void *in_list, RvMsgToStringFlags flags)
{
	const DataType     *tmp_list = static_cast<const DataType *>(in_list);
	unsigned int        count_1;
	std::ostringstream  out_string;

	if (flags & RvMsgToStringFlags_ArrayCount)
		out_string << "<" << in_count << ">";

	out_string << "[";

	for (count_1 = 0; count_1 < in_count; ++count_1) {
		if (count_1)
			out_string
				<< ((flags & RvMsgToStringFlags_ArrayCommaSep) ? ", " : " ");
		if (flags & RvMsgToStringFlags_ArrayIndex)
			out_string
				<< "[" << count_1 << "]=";
		out_string
			<< tmp_list[count_1];
	}

	out_string << "]";

	return(out_string.str());
}
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostringstream &RvMsgFieldHeader(const TibrvMsgField &datum,
	std::ostringstream &o_str, RvMsgToStringFlags flags)
{
	if ((flags & RvMsgToStringFlags_FieldTypeId) ||
		 (flags & RvMsgToStringFlags_FieldTypeName)) {
		o_str << "[";
		if (flags & RvMsgToStringFlags_FieldTypeId) 
			o_str << static_cast<unsigned int>(datum.getType()) <<
				((flags & RvMsgToStringFlags_FieldTypeName) ? "=" : "");
		if (flags & RvMsgToStringFlags_FieldTypeName) 
			o_str << MsgFieldTypeToString(datum);
		o_str << "]";
	}

	if (flags & RvMsgToStringFlags_FieldId)
		o_str << datum.getName() << "(" << datum.getId() << ")=";
	else
		o_str << datum.getName() << "=";

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &RvMsgFieldHeader(const TibrvMsgField &datum,
	std::string &out_string, RvMsgToStringFlags flags)
{
	std::ostringstream o_str;

	return(out_string.assign(RvMsgFieldHeader(datum, o_str, flags).str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string RvMsgFieldHeader(const TibrvMsgField &datum,
	RvMsgToStringFlags flags)
{
	std::string out_string;

	return(RvMsgFieldHeader(datum, out_string, flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string RvMsgFieldDataArrayToString(const TibrvMsgField &datum,
	RvMsgToStringFlags flags)
{
	std::string out_string;

	switch (datum.getType()) {
		case TIBRVMSG_I8ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_i8>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_U8ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_u8>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_I16ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_i16>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_U16ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_u16>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_I32ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_i32>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_U32ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_u32>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_I64ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_i64>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_U64ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_u64>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_F32ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_f32>(
				datum.getCount(), datum.getData().array, flags);
			break;
		case TIBRVMSG_F64ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_f64>(
				datum.getCount(), datum.getData().array, flags);
			break;
		default						:
			std::ostringstream error_text;
			error_text << "Unable to convert the specified non-array type ('" <<
				MsgFieldTypeToString(datum) <<
				"' to string with function 'RvMsgFieldDataArrayToString()'";
			throw RvException(error_text);
			break;
	}

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string RvMsgFieldDataBytesToString(const TibrvMsgField &datum,
	RvMsgToStringFlags flags)
{
	std::ostringstream out_string;

	if (flags & RvMsgToStringFlags_ArrayCount)
		out_string << "<" << datum.getSize() << ">\"";

	if (datum.getSize()) {
		if (!memchr(datum.getData().buf, '\0', datum.getSize())) {
			std::string tmp_string(static_cast<const char *>(datum.getData().buf),
				datum.getSize());
			out_string << tmp_string;
		}
		else {
			unsigned int  tmp_len = datum.getSize();
			const char   *tmp_ptr = static_cast<const char *>(datum.getData().buf);
			unsigned int  count_1;
			for (count_1 = 0; count_1 < tmp_len; ++count_1) {
				if (std::isprint(static_cast<unsigned char>(*tmp_ptr)))
					out_string << *tmp_ptr;
				else
					out_string << "\\" << MLB::Utility::ValueToStringHex(
						static_cast<unsigned char>(*tmp_ptr));
				++tmp_ptr;
			}
		}
	}

	out_string << "\"";

	return(out_string.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string RvMsgFieldDataToString(const TibrvMsgField &datum,
	RvMsgToStringFlags flags)
{
	std::ostringstream out_string;

/*
	CODE NOTE: Obsoleted. To be removed.
	if ((flags & RvMsgToStringFlags_FieldTypeId) ||
		 (flags & RvMsgToStringFlags_FieldTypeName)) {
		out_string << "[";
		if (flags & RvMsgToStringFlags_FieldTypeId) 
			out_string << static_cast<unsigned int>(datum.getType()) <<
				((flags & RvMsgToStringFlags_FieldTypeName) ? "=" : "");
		if (flags & RvMsgToStringFlags_FieldTypeName) 
			out_string << MsgFieldTypeToString(datum);
		out_string << "]";
	}

	if (flags & RvMsgToStringFlags_FieldId)
		out_string << datum.getName() << "(" << datum.getId() << ")=";
	else
		out_string << datum.getName() << "=";
*/

	RvMsgFieldHeader(datum, out_string, flags);

	switch (datum.getType()) {
		case TIBRVMSG_NO_TAG		:
			out_string << MsgFieldTypeToString(datum);
			break;
		case TIBRVMSG_MSG			:
			out_string << "TIBRVMSG_MSG";
			break;
		case TIBRVMSG_DATETIME	:
			{
				MLB::Utility::TimeSpec tmp_time(
					static_cast<time_t>(datum.getData().date.sec),
					datum.getData().date.nsec);
				if (flags & RvMsgToStringFlags_DateFieldNSecs)
					out_string << tmp_time;
				else
					out_string << tmp_time.ToString(19) << "Z";
			}
			break;
		case TIBRVMSG_OPAQUE		:
			if (flags & RvMsgToStringFlags_ExpandOpaque)
				out_string << RvMsgFieldDataBytesToString(datum);
			else
				out_string << "[" << datum.getSize() << " opaque bytes]";
			break;
		case TIBRVMSG_STRING		:
			out_string << "\"" << datum.getData().str << "\"";
			break;
		case TIBRVMSG_BOOL		:
			out_string << ((datum.getData().boolean) ? "true" : "false");
			break;
		case TIBRVMSG_I8			:
			out_string << static_cast<int>(datum.getData().i8);
			break;
		case TIBRVMSG_U8			:
			out_string << static_cast<unsigned int>(datum.getData().u8);
			break;
		case TIBRVMSG_I16			:
			out_string << datum.getData().i16;
			break;
		case TIBRVMSG_U16			:
			out_string << datum.getData().u16;
			break;
		case TIBRVMSG_I32			:
			out_string << datum.getData().i32;
			break;
		case TIBRVMSG_U32			:
			out_string << datum.getData().u32;
			break;
		case TIBRVMSG_I64			:
			out_string << datum.getData().i64;
			break;
		case TIBRVMSG_U64			:
			out_string << datum.getData().u64;
			break;
		case TIBRVMSG_F32			:
			if (flags & RvMsgToStringFlags_DefaultFPPrec)
				out_string << datum.getData().f32;
			else {
				//	Tib/Rv code is C, this attempts to replicate it...
				char tmp_buffer[1024];
				::sprintf(tmp_buffer, "%.6f", datum.getData().f32);
				out_string << tmp_buffer;
			}
			break;
		case TIBRVMSG_F64			:
			if (flags & RvMsgToStringFlags_DefaultFPPrec)
				out_string << datum.getData().f64;
			else {
				//	Tib/Rv code is C, this attempts to replicate it...
				char tmp_buffer[1024];
				::sprintf(tmp_buffer, "%.6f", datum.getData().f64);
				out_string << tmp_buffer;
			}
			break;
		case TIBRVMSG_IPPORT16	:
			out_string << ntohs(datum.getData().ipport16);
			break;
		case TIBRVMSG_IPADDR32	:
			{
/*
				out_string <<
					static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
						datum.getData().ipaddr32)[0]) << "." <<
					static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
						datum.getData().ipaddr32)[1]) << "." <<
					static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
						datum.getData().ipaddr32)[2]) << "." <<
					static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
						datum.getData().ipaddr32)[3]);
*/
				tibrv_ipaddr32 ip_addr = datum.getData().ipaddr32;
				out_string <<
					static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
						&ip_addr)[0]) << "." <<
					static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
						&ip_addr)[1]) << "." <<
					static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
						&ip_addr)[2]) << "." <<
					static_cast<unsigned int>(reinterpret_cast<unsigned char *>(
						&ip_addr)[3]);
			}
			break;
		case TIBRVMSG_ENCRYPTED	:
// CODE NOTE: To be implemented.
			out_string << "TIBRVMSG_ENCRYPTED";
			break;
		case TIBRVMSG_NONE		:
			out_string << MsgFieldTypeToString(datum);
			break;
		case TIBRVMSG_I8ARRAY	:
		case TIBRVMSG_U8ARRAY	:
		case TIBRVMSG_I16ARRAY	:
		case TIBRVMSG_U16ARRAY	:
		case TIBRVMSG_I32ARRAY	:
		case TIBRVMSG_U32ARRAY	:
		case TIBRVMSG_I64ARRAY	:
		case TIBRVMSG_U64ARRAY	:
		case TIBRVMSG_F32ARRAY	:
		case TIBRVMSG_F64ARRAY	:
			out_string << RvMsgFieldDataArrayToString(datum, flags);
			break;
		case TIBRVMSG_XML			:
			if (flags & RvMsgToStringFlags_ExpandXml)
				out_string << RvMsgFieldDataBytesToString(datum);
			else
				out_string << "[XML document: " << datum.getSize() << " bytes]";
			break;
		default						:
			out_string << MsgFieldTypeToString(datum);
			break;
	}

	return(out_string.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const TibrvMsgField &datum)
{
	o_str << RvMsgFieldDataToString(datum, RvMsgToStringFlags_Default);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

