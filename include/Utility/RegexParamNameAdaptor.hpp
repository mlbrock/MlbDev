// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for functions which provide regex-based
								adaptors for command line parameter parsing.

	Revision History	:	2006-10-25 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility_RegexParamNameAdaptor_hpp__HH

#define HH__MLB__Utility_RegexParamNameAdaptor_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file RegexParamNameAdaptor.hpp
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class PosixCRegexWrapper;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY bool RegexParamName(const PosixCRegexWrapper &param_pattern,
	const char *this_param);
API_UTILITY bool RegexParamName(const PosixCRegexWrapper &param_pattern,
	const std::string &this_param);
API_UTILITY bool RegexParamName(const char *param_pattern,
	const char *this_param);
API_UTILITY bool RegexParamName(const std::string &param_pattern,
	const std::string &this_param);
API_UTILITY bool RegexParamName(const char *param_pattern,
	unsigned int &current_index, int argc, char **argv);
API_UTILITY bool RegexParamName(const std::string &param_pattern,
	unsigned int &current_index, int argc, char **argv);

API_UTILITY bool RegexParamName(const std::vector<std::string> &pattern_list,
	const char *this_param);
API_UTILITY bool RegexParamName(const std::vector<std::string> &pattern_list,
	const std::string &this_param);
API_UTILITY bool RegexParamName(const std::vector<std::string> &pattern_list,
	unsigned int &current_index, int argc, char **argv);

API_UTILITY const char *RegexParamNameAdaptor(
	const PosixCRegexWrapper &param_pattern, const char *this_param);
API_UTILITY const char *RegexParamNameAdaptor(
	const PosixCRegexWrapper &param_pattern, const std::string &this_param);
API_UTILITY const char *RegexParamNameAdaptor(const char *param_pattern,
	const char *this_param);
API_UTILITY const char *RegexParamNameAdaptor(const char *param_pattern,
	const std::string &this_param);
API_UTILITY const char *RegexParamNameAdaptor(const char *param_pattern,
	unsigned int &current_index, int argc, char **argv);
API_UTILITY const char *RegexParamNameAdaptor(const std::string &param_pattern,
	unsigned int &current_index, int argc, char **argv);

API_UTILITY const char *RegexParamNameAdaptor(
	const std::vector<std::string> &pattern_list, const char *this_param);
API_UTILITY const char *RegexParamNameAdaptor(
	const std::vector<std::string> &pattern_list,
	const std::string &this_param);
API_UTILITY const char *RegexParamNameAdaptor(
	const std::vector<std::string> &pattern_list,
	unsigned int &current_index, int argc, char **argv);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility_RegexParamNameAdaptor_hpp__HH

