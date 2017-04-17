// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Copies a TibrvMsg.

	Revision History	:	2004-12-30 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RvMsg.hpp>

#include <Utility/stringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
void CopyMsg(const TibrvMsg &src_msg, TibrvMsg &dst_msg)
{
	try {
		const char *send_subject  = GetSendSubjectPtr(src_msg);
		const char *reply_subject = GetReplySubjectPtr(src_msg);
		RvUtilX_THROW_TIBRV_STATUS_IF(dst_msg.reset,
			());
		RvUtilX_THROW_TIBRV_STATUS_IF(const_cast<TibrvMsg &>(src_msg).createCopy,
			(dst_msg));
		if (send_subject != NULL) {
			RvUtilX_THROW_TIBRV_STATUS_IF(dst_msg.setSendSubject,
				(send_subject));
		}
		if (reply_subject != NULL) {
			RvUtilX_THROW_TIBRV_STATUS_IF(dst_msg.setReplySubject,
				(reply_subject));
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except,
			std::string("Attempt to copy a TibrvMsg object failed: ") +
			except.what());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CopyMsgNoSubjects(const TibrvMsg &src_msg, TibrvMsg &dst_msg)
{
	MsgCreateCopy(src_msg, dst_msg);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

