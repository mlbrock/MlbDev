// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts message field types to strings.

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

#include <RvUtilX/RvParseCmdLineArg.hpp>

#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
std::string MsgFieldTypeToString(const TibrvMsgField &datum)
{
	return(MsgFieldTypeToString(datum.getType()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string MsgFieldTypeToString(tibrv_u8 msg_field_type)
{
	std::string out_string;

	switch (msg_field_type) {
		case TIBRVMSG_NO_TAG		:
			out_string = "TIBRVMSG_NO_TAG";
			break;
		case TIBRVMSG_MSG			:
			out_string = "TIBRVMSG_MSG";
			break;
		case TIBRVMSG_DATETIME	:
			out_string = "TIBRVMSG_DATETIME";
			break;
		case TIBRVMSG_OPAQUE		:
			out_string = "TIBRVMSG_OPAQUE";
			break;
		case TIBRVMSG_STRING		:
			out_string = "TIBRVMSG_STRING";
			break;
		case TIBRVMSG_BOOL		:
			out_string = "TIBRVMSG_BOOL";
			break;
		case TIBRVMSG_I8			:
			out_string = "TIBRVMSG_I8";
			break;
		case TIBRVMSG_U8			:
			out_string = "TIBRVMSG_U8";
			break;
		case TIBRVMSG_I16			:
			out_string = "TIBRVMSG_I16";
			break;
		case TIBRVMSG_U16			:
			out_string = "TIBRVMSG_U16";
			break;
		case TIBRVMSG_I32			:
			out_string = "TIBRVMSG_I32";
			break;
		case TIBRVMSG_U32			:
			out_string = "TIBRVMSG_U32";
			break;
		case TIBRVMSG_I64			:
			out_string = "TIBRVMSG_I64";
			break;
		case TIBRVMSG_U64			:
			out_string = "TIBRVMSG_U64";
			break;
		case TIBRVMSG_F32			:
			out_string = "TIBRVMSG_F32";
			break;
		case TIBRVMSG_F64			:
			out_string = "TIBRVMSG_F64";
			break;
		case TIBRVMSG_IPPORT16	:
			out_string = "TIBRVMSG_IPPORT16";
			break;
		case TIBRVMSG_IPADDR32	:
			out_string = "TIBRVMSG_IPADDR32";
			break;
		case TIBRVMSG_ENCRYPTED	:
			out_string = "TIBRVMSG_ENCRYPTED";
			break;
		case TIBRVMSG_NONE		:
			out_string = "TIBRVMSG_NONE";
			break;
		case TIBRVMSG_I8ARRAY	:
			out_string = "TIBRVMSG_I8ARRAY";
			break;
		case TIBRVMSG_U8ARRAY	:
			out_string = "TIBRVMSG_U8ARRAY";
			break;
		case TIBRVMSG_I16ARRAY	:
			out_string = "TIBRVMSG_I16ARRAY";
			break;
		case TIBRVMSG_U16ARRAY	:
			out_string = "TIBRVMSG_U16ARRAY";
			break;
		case TIBRVMSG_I32ARRAY	:
			out_string = "TIBRVMSG_I32ARRAY";
			break;
		case TIBRVMSG_U32ARRAY	:
			out_string = "TIBRVMSG_U32ARRAY";
			break;
		case TIBRVMSG_I64ARRAY	:
			out_string = "TIBRVMSG_I64ARRAY";
			break;
		case TIBRVMSG_U64ARRAY	:
			out_string = "TIBRVMSG_U64ARRAY";
			break;
		case TIBRVMSG_F32ARRAY	:
			out_string = "TIBRVMSG_F32ARRAY";
			break;
		case TIBRVMSG_F64ARRAY	:
			out_string = "TIBRVMSG_F64ARRAY";
			break;
		case TIBRVMSG_XML			:
			out_string = "TIBRVMSG_XML";
			break;
		default						:
			std::ostringstream tmp_string;
			if ((msg_field_type >= TIBRVMSG_USER_FIRST) &&
				(msg_field_type <= TIBRVMSG_USER_LAST)) {
				tmp_string << "*User-defined TibRv Msg field type=" <<
					static_cast<unsigned int>(msg_field_type) << "*";
				out_string = tmp_string.str();
			}
			else {
				tmp_string << "*Invalid/unassigned TibRv Msg field type=" <<
					static_cast<unsigned int>(msg_field_type) << "*";
				out_string = tmp_string.str();
			}
			break;
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string MsgFieldTypeToDescription(const TibrvMsgField &datum)
{
	return(MsgFieldTypeToDescription(datum.getType()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string MsgFieldTypeToDescription(tibrv_u8 msg_field_type)
{
	std::ostringstream out_string;

	out_string << "Tib RV message field type " <<
		static_cast<unsigned int>(msg_field_type) << "=" <<
		MsgFieldTypeToString(msg_field_type);

	return(out_string.str());
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
tibrv_u8 StringToMsgFieldType(const std::string &msg_field_type_string)
{
	if (msg_field_type_string.empty())
		MLB::Utility::ThrowInvalidArgument("Tib/Rendezvous message field type "
			"string is empty.");

	if (std::isdigit(msg_field_type_string[0])) {
		tibrv_u8 msg_field_type = TIBRVMSG_NONE;
		try {
			RvParseCmdLineArg::ParseCmdLineNumeric<tibrv_u8>(
				msg_field_type_string, msg_field_type);
			//	We use 'MsgFieldTypeToString()' just to ensure the value is valid...
			MsgFieldTypeToString(msg_field_type);
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Unable to parse a Tib/Rendezvous "
				"message field type numeric value: " + std::string(except.what()));
		}
		return(msg_field_type);
	}
		
	std::string tmp_string(MLB::Utility::UpperCase(msg_field_type_string));

	if ((tmp_string == "TIBRVMSG_NO_TAG")         || (tmp_string == "NO_TAG"))
		return(TIBRVMSG_NO_TAG);
	else if ((tmp_string == "TIBRVMSG_MSG")       || (tmp_string == "MSG"))
		return(TIBRVMSG_MSG);
	else if ((tmp_string == "TIBRVMSG_DATETIME")  || (tmp_string == "DATETIME"))
		return(TIBRVMSG_DATETIME);
	else if ((tmp_string == "TIBRVMSG_OPAQUE")    || (tmp_string == "OPAQUE"))
		return(TIBRVMSG_OPAQUE);
	else if ((tmp_string == "TIBRVMSG_STRING")    || (tmp_string == "STRING"))
		return(TIBRVMSG_STRING);
	else if ((tmp_string == "TIBRVMSG_BOOL")      || (tmp_string == "BOOL"))
		return(TIBRVMSG_BOOL);
	else if ((tmp_string == "TIBRVMSG_I8")        || (tmp_string == "I8"))
		return(TIBRVMSG_I8);
	else if ((tmp_string == "TIBRVMSG_U8")        || (tmp_string == "U8"))
		return(TIBRVMSG_U8);
	else if ((tmp_string == "TIBRVMSG_I16")       || (tmp_string == "I16"))
		return(TIBRVMSG_I16);
	else if ((tmp_string == "TIBRVMSG_U16")       || (tmp_string == "U16"))
		return(TIBRVMSG_U16);
	else if ((tmp_string == "TIBRVMSG_I32")       || (tmp_string == "I32"))
		return(TIBRVMSG_I32);
	else if ((tmp_string == "TIBRVMSG_U32")       || (tmp_string == "U32"))
		return(TIBRVMSG_U32);
	else if ((tmp_string == "TIBRVMSG_I64")       || (tmp_string == "I64"))
		return(TIBRVMSG_I64);
	else if ((tmp_string == "TIBRVMSG_U64")       || (tmp_string == "U64"))
		return(TIBRVMSG_U64);
	else if ((tmp_string == "TIBRVMSG_F32")       || (tmp_string == "F32"))
		return(TIBRVMSG_F32);
	else if ((tmp_string == "TIBRVMSG_F64")       || (tmp_string == "F64"))
		return(TIBRVMSG_F64);
	else if ((tmp_string == "TIBRVMSG_IPPORT16")  || (tmp_string == "IPPORT16"))
		return(TIBRVMSG_IPPORT16);
	else if ((tmp_string == "TIBRVMSG_IPADDR32")  || (tmp_string == "IPADDR32"))
		return(TIBRVMSG_IPADDR32);
	else if ((tmp_string == "TIBRVMSG_ENCRYPTED") || (tmp_string == "ENCRYPTED"))
		return(TIBRVMSG_ENCRYPTED);
	else if ((tmp_string == "TIBRVMSG_NONE")      || (tmp_string == "NONE"))
		return(TIBRVMSG_NONE);
	else if ((tmp_string == "TIBRVMSG_I8ARRAY")   || (tmp_string == "I8ARRAY"))
		return(TIBRVMSG_I8ARRAY);
	else if ((tmp_string == "TIBRVMSG_U8ARRAY")   || (tmp_string == "U8ARRAY"))
		return(TIBRVMSG_U8ARRAY);
	else if ((tmp_string == "TIBRVMSG_I16ARRAY")  || (tmp_string == "I16ARRAY"))
		return(TIBRVMSG_I16ARRAY);
	else if ((tmp_string == "TIBRVMSG_U16ARRAY")  || (tmp_string == "U16ARRAY"))
		return(TIBRVMSG_U16ARRAY);
	else if ((tmp_string == "TIBRVMSG_I32ARRAY")  || (tmp_string == "I32ARRAY"))
		return(TIBRVMSG_I32ARRAY);
	else if ((tmp_string == "TIBRVMSG_U32ARRAY")  || (tmp_string == "U32ARRAY"))
		return(TIBRVMSG_U32ARRAY);
	else if ((tmp_string == "TIBRVMSG_I64ARRAY")  || (tmp_string == "I64ARRAY"))
		return(TIBRVMSG_I64ARRAY);
	else if ((tmp_string == "TIBRVMSG_U64ARRAY")  || (tmp_string == "U64ARRAY"))
		return(TIBRVMSG_U64ARRAY);
	else if ((tmp_string == "TIBRVMSG_F32ARRAY")  || (tmp_string == "F32ARRAY"))
		return(TIBRVMSG_F32ARRAY);
	else if ((tmp_string == "TIBRVMSG_F64ARRAY")  || (tmp_string == "F64ARRAY"))
		return(TIBRVMSG_F64ARRAY);
	else if ((tmp_string == "TIBRVMSG_XML")       || (tmp_string == "XML"))
		return(TIBRVMSG_XML);

	MLB::Utility::ThrowInvalidArgument("Unable to parse Tib/Rendezvous "
		"message field type string '" + msg_field_type_string + "'.");

	//	Prevent compiler complaints.
	return(TIBRVMSG_NONE);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

