// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for functions to support replacement of
								characters in strings.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__ReplaceChars_hpp__HH

#define HH__MLB__Utility__ReplaceChars_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file ReplaceChars.hpp

	\brief	Logic to support replacement of characteras in strings.
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

//	//////////////////////////////////////////////////////////////////////////
API_UTILITY std::string &ReplaceCharsIn(const std::string &in_string,
	std::string &out_string, std::size_t from_char_count,
	const char *from_char_list, int to_char);
API_UTILITY std::string &ReplaceCharsIn(const std::string &in_string,
	std::string &out_string, const std::string &from_char_list, int to_char);
API_UTILITY std::string &ReplaceCharsIn(const std::string &in_string,
	std::string &out_string, int from_char, int to_char);
API_UTILITY std::string  ReplaceCharsIn(const std::string &in_string,
	const std::string &from_char_list, int to_char);
API_UTILITY std::string  ReplaceCharsIn(const std::string &in_string,
	int from_char, int to_char);

API_UTILITY std::string &ReplaceCharsNotIn(const std::string &in_string,
	std::string &out_string, std::size_t from_char_count,
	const char *from_char_list, int to_char);
API_UTILITY std::string &ReplaceCharsNotIn(const std::string &in_string,
	std::string &out_string, const std::string &from_char_list, int to_char);
API_UTILITY std::string &ReplaceCharsNotIn(const std::string &in_string,
	std::string &out_string, int from_char, int to_char);
API_UTILITY std::string  ReplaceCharsNotIn(const std::string &in_string,
	const std::string &from_char_list, int to_char);
API_UTILITY std::string  ReplaceCharsNotIn(const std::string &in_string,
	int from_char, int to_char);
//	//////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ReplaceChars_hpp__HH

