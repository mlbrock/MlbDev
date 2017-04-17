// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for support 29 West LBM statistics logic.

	Revision History	:	2010-07-31 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__LbmWrapper__LbmStatsSupport_hpp__HH

#define HH__MLB__LbmWrapper__LbmStatsSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmWrapper.hpp>

#include <Utility/StringSupport.hpp>

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable: 4512)
#endif // #ifdef _Windows

#include <boost/io/ios_state.hpp>

#ifdef _MSC_VER
# pragma warning(pop)
#endif // #ifdef _MSC_VER

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
template <typename MinType>
	inline MinType FixupMinStat(MinType min_value, bool fixup_min = true)
{
	return((fixup_min && (min_value == std::numeric_limits<MinType>::max())) ?
		0 : min_value);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void EmitStatName(const char *stat_name, unsigned int text_width,
	std::ostream &o_str)
{
	boost::io::ios_all_saver ios_saver(o_str);

	o_str
		<< std::left << MLB::Utility::PadRight(stat_name, text_width)
		<< ": "
			;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Default handling for numeric types...
template <typename StatType>
	inline void EmitStatValue(StatType stat_value, std::ostream &o_str)
{
	boost::io::ios_all_saver ios_saver(o_str);

	o_str
		<< std::right <<
		std::setw(static_cast<std::streamsize>(
			MLB::Utility::IntegralValueMaxLengthDec<StatType>())) <<
		stat_value
		;
}
//	----------------------------------------------------------------------------
template <>
	inline void EmitStatValue<const char *>(const char *stat_value,
		std::ostream &o_str)
{
	boost::io::ios_all_saver ios_saver(o_str);

	o_str
		<< std::left << stat_value
		;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Default handling for numeric types...
template <typename StatType>
	inline void EmitStatValueEndL(StatType stat_value, std::ostream &o_str)
{
	EmitStatValue(stat_value, o_str);

	o_str
		<< std::endl
		;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename StatType>
	inline void EmitStatNameValue(const char *stat_name, unsigned int text_width,
		StatType stat_value, std::ostream &o_str)
{
	EmitStatName(stat_name, text_width, o_str);
	EmitStatValue(stat_value, o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename StatType>
	inline void EmitStatNameValueEndL(const char *stat_name,
		unsigned int text_width, StatType stat_value, std::ostream &o_str)
{
	EmitStatNameValue(stat_name, text_width, stat_value, o_str);

	o_str
		<< std::endl
		;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename StatsType, typename ApiForType>
	struct GetStatsForAllAdapter {
	typedef            StatsType                    ThisStatsType;
	typedef typename   StatsType::MyLbmApiStatsType ThisLbmApiStatsType;
	typedef int      (*ThisFuncPtr)(ApiForType *, int *, int,
		typename StatsType::MyLbmApiStatsType *);

	GetStatsForAllAdapter(ApiForType *api_for_type, const char *stat_type_name,
		const char *api_for_type_name, const char *get_func_name,
		ThisFuncPtr get_func_ptr)
		:api_for_type_(api_for_type)
		,stat_type_name_(stat_type_name)
		,api_for_type_name_(api_for_type_name)
		,get_func_name_(get_func_name)
		,get_func_ptr_(get_func_ptr)
		,stat_list_()
	{
		std::vector<typename StatsType::MyLbmApiStatsType> tmp_list(1024);

		for ( ; ; ) {
			int tmp_size        = static_cast<int>(tmp_list.size());
			int lbm_return_code = (*get_func_ptr_)(api_for_type, &tmp_size,
				sizeof(tmp_list[0]), &(tmp_list[0]));
			if (!lbm_return_code) {
				if (static_cast<std::size_t>(tmp_size) < tmp_list.size())
					tmp_list.resize(static_cast<std::size_t>(tmp_size));
				break;
			}
			else if ((lbm_return_code == -1) && (::lbm_errnum() == LBM_EINVAL)) {
				/*
					Some kinda logic error if LBM returns an indication that there's
					not enough room for the list, but there actually is...
				*/
				if (static_cast<std::size_t>(tmp_size) <= tmp_list.size()) {
					std::ostringstream o_str;
					o_str << "An attempt to retrieve statistics information for "
						"all " << stat_type_name_ << " within a " <<
						api_for_type_name_ << " failed because the LBM function '" <<
						get_func_name_ << "()' indicated that a total of " <<
						tmp_list.size() << " elements was insufficient to store the "
						"data, but that " << tmp_size << "elements would suffice.";
					MLB::Utility::ThrowLogicError(o_str.str());
				}
				else
					tmp_list.resize(tmp_list.size() + 1024);	//	For an MT env.
			}
			else {
				WRAP29_LBM_THROW_STRING_NAME(get_func_name_);
			}
		}

		if (!tmp_list.empty()) {
			std::size_t                stat_count = tmp_list.size();
			const ThisLbmApiStatsType *stat_ptr   = &(tmp_list[0]);
			stat_list_.reserve(stat_count);
			for ( ; stat_count; --stat_count, ++stat_ptr)
				stat_list_.push_back(StatsType(*stat_ptr));
		}
	}

	ApiForType             *api_for_type_;
	const char             *stat_type_name_;
	const char             *api_for_type_name_;
	const char             *get_func_name_;
	ThisFuncPtr             get_func_ptr_;
	std::vector<StatsType>  stat_list_;
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__LbmWrapper__LbmStatsSupport_hpp__HH

