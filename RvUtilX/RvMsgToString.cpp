// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts a message to a string.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
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

#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

namespace {
// ////////////////////////////////////////////////////////////////////////////
void RvMsgToStringInternal(const TibrvMsg &msg, unsigned int depth,
	std::ostringstream &out_string, RvMsgToStringFlags flags)
{
	tibrv_u32 field_count;

	RvUtilX_THROW_TIBRV_STATUS_IF(msg.getNumFields,
		(field_count));

	if (!(flags & RvMsgToStringFlags_NoMsgCBrackets)) {
		out_string << "{";
		if ((flags & RvMsgToStringFlags_WrapAndIndent) && (!depth))
			out_string << std::endl;
	}

	tibrv_u32 count_1;
	for (count_1 = 0; count_1 < field_count; count_1++) {
		TibrvMsgField msg_field;
		RvMsgSnapshotContext rvmsg_snapshot_context(msg);
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(msg).getFieldByIndex,
			(msg_field, count_1));
		if (count_1) {
			if (flags & RvMsgToStringFlags_FieldCommaSep)
				out_string << ", ";
			else
				out_string << " ";
		}

		if ((flags & RvMsgToStringFlags_WrapAndIndent) && (depth || count_1))
			out_string << std::endl;

		if (flags & RvMsgToStringFlags_WrapAndIndent)
			out_string << MLB::Utility::PadLeft("", depth + 1, '\t');

		if (flags & RvMsgToStringFlags_MsgDepth)
			out_string
				<< "{" << depth << "}";
		if (msg_field.getType() == TIBRVMSG_MSG) {
			RvMsgFieldHeader(msg_field, out_string, flags);
			RvMsgToStringInternal(TibrvMsg(msg_field.getData().msg, TIBRV_FALSE),
				depth + 1, out_string, flags);
		}
		else
			out_string
				<< RvMsgFieldDataToString(msg_field, flags);
	}

	if (!(flags & RvMsgToStringFlags_NoMsgCBrackets)) {
		if (flags & RvMsgToStringFlags_WrapAndIndent) {
			out_string << std::endl;
			out_string << MLB::Utility::PadLeft("", depth, '\t');
		}
		out_string << "}";
	}
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void RvMsgToString(const TibrvMsg &msg, std::string &out_string,
	RvMsgToStringFlags flags)
{
	std::ostringstream tmp_string;

	RvMsgToStringInternal(msg, 0, tmp_string, flags);

	out_string = tmp_string.str();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string RvMsgToString(const TibrvMsg &msg, RvMsgToStringFlags flags)
{
	std::string out_string;

	RvMsgToString(msg, out_string, flags);

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const TibrvMsg &datum)
{
	o_str << RvMsgToString(datum, RvMsgToStringFlags_Default);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

#ifdef TEST_MAIN

#include <RvUtilX/AddRvField.hpp>
#include <RvUtilX/RvParseCmdLineArg.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/StringSupport.hpp>

using namespace MLB::RvUtilX;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (RvParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -subject <RV-subject-name> ] " <<
			"[ -service <RV-service> ] " <<
			"[ -network <RV-network> ] " <<
			"[ -daemon <RV-daemon> ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -subject <subject-name>\n\
      If specified, the test message will be sent on <subject-name>.\n\n\
   -service <RV-service>\n\
      The Tib/Rendezvous service for sending the message.\n\n\
   -network <RV-network>\n\
      The Tib/Rendezvous network for sending the message.\n\n\
   -daemon <RV-daemon>\n\
      The Tib/Rendezvous daemon for sending the message." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Get the command line parameters...
		// ////////////////////////////////////////////////////////////////
		unsigned int count_1;
		std::string  subject_name;
		SNDSpec      snd_spec;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (RvParseCmdLineArg::ParseCmdLineSubject("-SUBJECT",
            count_1, argc, argv, subject_name))
				;
			else if (snd_spec.ParseCmdLineServiceNetworkDaemon(count_1, argc,
				argv))
				;
			else
				RvParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		RvContext         rv_context;
		TibrvMsg          msg_base;
		TibrvMsg          msg_nested;
		TibrvMsg          msg_nested_2;
		unsigned int      int_array[] = { 1, 2, 3 };
		const char       *str         = "This is a string.";
		std::string       buf_xml     = "This is an XML string.";
		std::string       buf_opaque  = "This is an opaque string.";
//		const void       *array       = int_array;
		tibrv_bool        boolean     = TIBRV_TRUE;
		tibrv_i8          i8          = -8;
		tibrv_u8          u8          =  8;
		tibrv_i16         i16         = -16;
		tibrv_u16         u16         =  16;
		tibrv_i32         i32         = -32;
		tibrv_u32         u32         =  32;
		tibrv_i64         i64         = -64;
		tibrv_u64         u64         =  64;
		tibrv_f32         f32         = static_cast<float>(32.32);
		tibrv_f64         f64         = 64.64;
		tibrv_ipport16    ipport16    = ::htons(666);
		tibrv_ipaddr32    ipaddr32    = ::htonl(0x01020304);
		tibrvMsgDateTime  date;

		date.sec  = time(NULL);
		date.nsec = 0;

		AddRvField(msg_nested_2, "InnerNestedMsg",
			"The inner-most nested message field.");
		AddRvField(msg_nested, "NestedMsgField_1", "A nested message field.");
		AddRvField(msg_nested, "NestedMsgField_2", msg_nested_2);
		
		AddRvField(msg_base, "str", str);
		AddRvFieldXml(msg_base, "buf_xml", buf_xml);
		AddRvFieldOpaque(msg_base, "buf_opaque", buf_opaque);
//		AddRvField(msg_base, "array", array);
		// CODE NOTE: Need to add {Add|Get|Update}RvField() for arrays.
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_base.addU32Array,
			("u32_array", int_array, sizeof(int_array) / sizeof(int_array[0])));
		AddRvField(msg_base, "boolean", boolean);
		AddRvField(msg_base, "i8", i8);
		AddRvField(msg_base, "u8", u8);
		AddRvField(msg_base, "i16", i16);
		AddRvField(msg_base, "u16", u16);
		AddRvField(msg_base, "i32", i32);
		AddRvField(msg_base, "u32", u32);
		AddRvField(msg_base, "i64", i64);
		AddRvField(msg_base, "u64", u64);
		AddRvField(msg_base, "f32", f32);
		AddRvField(msg_base, "f64", f64);
		AddRvFieldIPPort16(msg_base, "ipport16", ipport16);
		AddRvFieldIPAddr32(msg_base, "ipaddr32", ipaddr32);
		AddRvField(msg_base, "date", date);
		AddRvField(msg_base, "msg_nested", msg_nested);

		std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
		std::string none_string(RvMsgToString(msg_base, RvMsgToStringFlags_None));
		std::cout << "TO STRING FLAGS=NONE: [" << none_string << "]" << std::endl;

		std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		const char *tib_string;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_base.convertToString,
			(tib_string));
		std::cout << "TO STRING TIBCO/RV  : [" << tib_string << "]" << std::endl;

		std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		std::cout << "COMPARE NONE TO TIB : Strings " <<
			((none_string == tib_string) ? "are identical." : "differ!!!") <<
			std::endl;
			
		std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		std::cout << "TO STRING FLAGS=MASK: [" <<
			RvMsgToString(msg_base, RvMsgToStringFlags_Mask) << "]" << std::endl;

		std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
		std::cout << std::endl;

		if (!subject_name.empty()) {
			std::cout << "Will send test message on subject " <<
				subject_name << "..." << std::flush;
			TibrvNetTransport transport;
			snd_spec.CreateTransport(transport);
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_base.setSendSubject,
				(subject_name.c_str()));
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_base.setReplySubject,
				("TEST.Reply.Subject"));
			RvUtilX_THROW_TIBRV_STATUS_IF(transport.send,
				(msg_base));
			std::cout << std::endl << std::endl;
		}
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

