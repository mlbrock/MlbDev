// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Detaches a TibrvMsg.

	Revision History	:	2004-12-30 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2018.
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

#include <Utility/stringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
TibrvMsg *DetachTibrvMsg(TibrvMsg &src_msg)
{
	TibrvStatus rv_status;

	if ((rv_status = src_msg.getStatus()) != TIBRV_OK)
		ThrowTibrvStatus(rv_status,
			"Unable to detach a TibrvMsg due to its invalid status");
		
	if (src_msg.isDetached())
		throw RvException("Unable to detach a TibrvMsg because it is already "
			"detached.");

	TibrvMsg *dst_msg_ptr;

	if ((dst_msg_ptr = src_msg.detach()) == NULL)
		throw RvException("Unable to detach a TibrvMsg because the 'detach()' "
			"method of the message to be copied returned 'NULL'.");

	return(dst_msg_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

