// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts event types to strings.

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

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
std::string EventTypeToString(tibrvEventType event_type)
{
	std::string out_string;

	switch (event_type) {
		case TIBRV_LISTEN_EVENT	:
			out_string = "TIBRV_LISTEN_EVENT";
			break;
		case TIBRV_TIMER_EVENT	:
			out_string = "TIBRV_TIMER_EVENT";
			break;
		case TIBRV_IO_EVENT		:
			out_string = "TIBRV_IO_EVENT";
			break;
		default						:
			std::ostringstream tmp_string;
			tmp_string << "*Invalid/unassigned TibRv event type=" <<
				static_cast<unsigned int>(event_type) << "*";
			out_string = tmp_string.str();
			break;
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string EventTypeToDescription(tibrvEventType event_type)
{
	std::ostringstream out_string;

	out_string << "Tib RV event type " <<
		static_cast<unsigned int>(event_type) << "=" <<
		EventTypeToString(event_type);

	return(out_string.str());
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

