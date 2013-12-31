// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts queue limit policy types to strings.

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

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
std::string QueueLimitPolicyToString(
	tibrvQueueLimitPolicy queue_limit_policy)
{
	std::string out_string;

	switch (queue_limit_policy) {
		case TIBRVQUEUE_DISCARD_NONE	:
			out_string = "TIBRVQUEUE_DISCARD_NONE";
			break;
		case TIBRVQUEUE_DISCARD_NEW	:
			out_string = "TIBRVQUEUE_DISCARD_NEW";
			break;
		case TIBRVQUEUE_DISCARD_FIRST	:
			out_string = "TIBRVQUEUE_DISCARD_FIRST";
			break;
		case TIBRVQUEUE_DISCARD_LAST	:
			out_string = "TIBRVQUEUE_DISCARD_LAST";
			break;
		default								:
			std::ostringstream tmp_string;
			tmp_string << "*Invalid/unassigned TibRv queue limit policy=" <<
				static_cast<int>(queue_limit_policy) << "*";
			out_string = tmp_string.str();
			break;
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string QueueLimitPolicyToDescription(
	tibrvQueueLimitPolicy queue_limit_policy)
{
	std::ostringstream out_string;

	out_string << "Tib RV queue limit policy " <<
		static_cast<int>(queue_limit_policy) << "=" <<
		QueueLimitPolicyToString(queue_limit_policy);

	return(out_string.str());
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

