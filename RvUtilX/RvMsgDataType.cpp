// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts message data (NOT field) types to strings.

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

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
std::string MsgDataTypeToString(tibrvMsgDataType msg_data_type)
{
	std::string out_string;

	switch (msg_data_type) {
		case tibrvMsgData_Primitive		:
			out_string = "tibrvMsgData_Primitive";
			break;
		case tibrvMsgData_MallocBlock		:
			out_string = "tibrvMsgData_MallocBlock";
			break;
		case tibrvMsgData_SubMessage		:
			out_string = "tibrvMsgData_SubMessage";
			break;
		case tibrvMsgData_WireReference	:
			out_string = "tibrvMsgData_WireReference";
			break;
		default									:
			std::ostringstream tmp_string;
			tmp_string << "*Invalid/unassigned TibRv message data type=" <<
				static_cast<int>(msg_data_type) << "*";
			out_string = tmp_string.str();
			break;
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string MsgDataTypeToDescription(tibrvMsgDataType msg_data_type)
{
	std::ostringstream out_string;

	out_string << "Tib RV message data type " <<
		static_cast<int>(msg_data_type) << "=" <<
		MsgDataTypeToString(msg_data_type);

	return(out_string.str());
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

