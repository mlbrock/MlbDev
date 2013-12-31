//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string RvMsgFieldDataArrayToString(tibrv_u32 in_count,
	const void *in_list)
{
	const DataType     *tmp_list = static_cast<const DataType *>(in_list);
	unsigned int        count_1;
	std::ostringstream  out_string;

	out_string << "<" << in_count << ">";

	for (count_1 = 0; count_1 < in_count; ++count_1)
		out_string << ((count_1) ? ", " : "") <<
			"[" << count_1 << "]=" << tmp_list[count_1];

	return(out_string.str());
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string RvMsgFieldDataArrayToString(const TibrvMsgField &datum)
{
	std::string out_string;

	switch (datum.getType()) {
		case TIBRVMSG_I8ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_i8>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_U8ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_u8>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_I16ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_i16>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_U16ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_u16>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_I32ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_i32>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_U32ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_u32>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_I64ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_i64>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_U64ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_u64>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_F32ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_f32>(
				datum.getCount(), datum.getData().array);
			break;
		case TIBRVMSG_F64ARRAY	:
			out_string = RvMsgFieldDataArrayToString<tibrv_f64>(
				datum.getCount(), datum.getData().array);
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
std::string RvMsgFieldDataBytesToString(const TibrvMsgField &datum)
{
	std::ostringstream out_string;

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
std::string RvMsgFieldDataToString(const TibrvMsgField &datum)
{
	std::ostringstream out_string;

	out_string << datum.getName() << "[" << MsgFieldTypeToString(datum) <<
		"](" << datum.getId() << ")=";

	switch (datum.getType()) {
		case TIBRVMSG_NO_TAG		:
			out_string << MsgFieldTypeToString(datum);
			break;
		case TIBRVMSG_MSG			:
			out_string << "TIBRVMSG_MSG";
			break;
		case TIBRVMSG_DATETIME	:
			out_string << MLB::Utility::TimeSpec(
				static_cast<time_t>(datum.getData().date.sec),
				datum.getData().date.nsec);
			break;
		case TIBRVMSG_OPAQUE		:
			out_string << RvMsgFieldDataBytesToString(datum);
			break;
		case TIBRVMSG_STRING		:
			out_string << "\"" << datum.getData().str << "\"";
			break;
		case TIBRVMSG_BOOL		:
			out_string << ((datum.getData().boolean) ? "TRUE" : "FALSE");
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
			out_string << datum.getData().f32;
			break;
		case TIBRVMSG_F64			:
			out_string << datum.getData().f64;
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
			out_string << RvMsgFieldDataArrayToString(datum);
			break;
		case TIBRVMSG_XML			:
			out_string << RvMsgFieldDataBytesToString(datum);
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
	o_str << RvMsgFieldDataToString(datum);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

//	CODE NOTE: From RvMsgToString.cpp
// ////////////////////////////////////////////////////////////////////////////
static void RvMsgToStringInternal(const TibrvMsg &msg, unsigned int depth,
	std::ostringstream &out_string)
{
	tibrv_u32 field_count;

	RvUtilX_THROW_TIBRV_STATUS_IF(msg.getNumFields,
		(field_count));

	tibrv_u32 count_1;
	for (count_1 = 0; count_1 < field_count; count_1++) {
		TibrvMsgField        msg_field;
		RvMsgSnapshotContext rvmsg_snapshot_context(msg);
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getFieldByIndex,
			(msg_field, count_1));
		if (count_1)
			out_string << ", ";
		if (msg_field.getType() == TIBRVMSG_MSG)
			RvMsgToStringInternal(TibrvMsg(msg_field.getData().msg,
				TIBRV_FALSE), depth + 1, out_string);
		else
			out_string << "{" << depth << "}" <<
				RvMsgFieldDataToString(msg_field);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RvMsgToString(const TibrvMsg &msg, std::string &out_string)
{
	std::ostringstream tmp_string;

	RvMsgToStringInternal(msg, 0, tmp_string);

	out_string = tmp_string.str();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string RvMsgToString(const TibrvMsg &msg)
{
	std::string out_string;

	RvMsgToString(msg, out_string);

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const TibrvMsg &datum)
{
	o_str << RvMsgToString(datum);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

