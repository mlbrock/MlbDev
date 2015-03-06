// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for 29 West LBM attribute and configuration
								logic.

	Revision History	:	2008-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmAttrSupport.hpp>

#ifdef __linux__
# include <string.h>
#endif // #ifdef __linux__

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
bool LbmConfigOptionLessThan::operator () (const lbm_config_option_t &lhs,
	const lbm_config_option_t &rhs) const
{
	int cmp;

	if ((cmp = ::strcmp(lhs.type, rhs.type)) == 0)
		cmp = ::strcmp(lhs.oname, rhs.oname);

	return(cmp < 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::size_t GetMaxOptionNameLength(const LbmConfigOptionList &opt_list)
{
	std::size_t                     max_opt_name_len = 0;
	LbmConfigOptionList::size_type  opt_count        = opt_list.size();
	const lbm_config_option_t      *opt_ptr          = &(opt_list[0]);

	while (opt_count--) {
		max_opt_name_len = std::max(max_opt_name_len, ::strlen(opt_ptr->oname));
		++opt_ptr;
	}

	return(max_opt_name_len);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GetFormattedString(const LbmConfigOptionList &opt_list)
{
	std::streamsize                 max_opt_name_len =
		static_cast<std::streamsize>(GetMaxOptionNameLength(opt_list));
	LbmConfigOptionList::size_type  opt_count = opt_list.size();
	const lbm_config_option_t      *opt_ptr   = &(opt_list[0]);
	std::ostringstream              o_str;

	o_str << std::left;

	while (opt_count--) {
		o_str
			<< std::setw(10)               << opt_ptr->type  << ":"
			<< std::setw(max_opt_name_len) << opt_ptr->oname << ":"
			                               << opt_ptr->val
				;
		if (opt_count)
			o_str << std::endl;
		++opt_ptr;
	}

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const LbmConfigOptionList &datum)
{
	o_str
		<< GetFormattedString(datum)
			;

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

