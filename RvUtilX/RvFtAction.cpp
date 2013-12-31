// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts fault tolerance actions to strings.

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
std::string FtActionToString(tibrvftAction ft_action)
{
	std::string out_string;

	switch (ft_action) {
		case TIBRVFT_PREPARE_TO_ACTIVATE	:
			out_string = "TIBRVFT_PREPARE_TO_ACTIVATE";
			break;
		case TIBRVFT_ACTIVATE				:
			out_string = "TIBRVFT_ACTIVATE";
			break;
		case TIBRVFT_DEACTIVATE				:
			out_string = "TIBRVFT_DEACTIVATE";
			break;
		default									:
			std::ostringstream tmp_string;
			tmp_string << "*Invalid/unassigned TibRv fault tolerance action=" <<
				static_cast<int>(ft_action) << "*";
			out_string = tmp_string.str();
			break;
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string FtActionToDescription(tibrvftAction ft_action)
{
	std::ostringstream out_string;

	out_string << "Tib RV fault tolerance action " <<
		static_cast<int>(ft_action) << "=" << FtActionToString(ft_action);

	return(out_string.str());
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

