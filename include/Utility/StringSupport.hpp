// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File for C Language Support
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for C++ string supporty library functions.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__StringSupport_hpp__HH

#define HH__MLB__StringSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file 	StringSupport.hpp

	\brief	The MLB Utility C++ string support functions header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>

#if defined(__linux__)
# include <string.h>
#endif // #if defined(__linux__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string  &UpperCase(const std::string &in_string,
	std::string &out_string);
API_UTILITY std::string  &LowerCase(const std::string &in_string,
	std::string &out_string);
API_UTILITY  std::string  UpperCase(const std::string &in_string);
API_UTILITY  std::string  LowerCase(const std::string &in_string);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string &TrimRightString(const std::string &in_string,
	std::string &out_string, const std::string &trim_char_list = " \t\r\n\v\f");
API_UTILITY std::string &TrimLeftString(const std::string &in_string,
	std::string &out_string, const std::string &trim_char_list = " \t\r\n\v\f");
API_UTILITY std::string &TrimString(const std::string &in_string,
	std::string &out_string, const std::string &trim_char_list = " \t\r\n\v\f");
API_UTILITY std::string  TrimRight(const std::string &in_string,
	const std::string &trim_char_list = " \t\r\n\v\f");
API_UTILITY std::string  TrimLeft(const std::string &in_string,
	const std::string &trim_char_list = " \t\r\n\v\f");
API_UTILITY std::string  Trim(const std::string &in_string,
	const std::string &trim_char_list = " \t\r\n\v\f");
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string &PadLeft(const std::string &in_string,
	std::string &out_string, unsigned int pad_length, char pad_char = ' ');
API_UTILITY std::string PadLeft(const std::string &in_string,
	unsigned int pad_length, char pad_char = ' ');

API_UTILITY std::string &PadRight(const std::string &in_string,
	std::string &out_string, unsigned int pad_length, char pad_char = ' ');
API_UTILITY std::string PadRight(const std::string &in_string,
	unsigned int pad_length, char pad_char = ' ');
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string ZeroFill(const DataType &in_datum, unsigned int pad_length)
{
	return(ZeroFill(AnyToString(in_datum), pad_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string &ZeroFill(const DataType &in_datum, std::string &out_string,
	unsigned int pad_length)
{
	return(ZeroFill(AnyToString(in_datum), out_string, pad_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <>
	inline std::string &ZeroFill<std::string>(const std::string &in_datum,
		std::string &out_string, unsigned int pad_length)
{
	return(PadLeft(in_datum, out_string, pad_length, '0'));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <>
	inline std::string ZeroFill<std::string>(const std::string &in_datum,
		unsigned int pad_length)
{
	std::string out_string;

	return(ZeroFill(in_datum, out_string, pad_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Specialization for bool because AnyToString<bool>() returns the text string
//	'true' or 'false'.
template <>
	inline std::string &ZeroFill<bool>(const bool &in_datum,
		std::string &out_string, unsigned int pad_length)
{
	return(ZeroFill(AnyToString(static_cast<int>(in_datum)), out_string,
		pad_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Specialization for bool because AnyToString<bool>() returns the text string
//	'true' or 'false'.
template <>
	inline std::string ZeroFill<bool>(const bool &in_datum,
		unsigned int pad_length)
{
	return(ZeroFill(AnyToString(static_cast<int>(in_datum)), pad_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY StringVector &SplitString(const std::string &in_string,
	const std::string &find_string, StringVector &out_list,
	std::string::size_type start_pos = 0, bool discard_empty_sections = false);
API_UTILITY StringVector  GetSplit(const std::string &in_string,
	const std::string &find_string, std::string::size_type start_pos = 0,
	bool discard_empty_sections_flag = false);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	std::string &JoinString(std::string &out_string,
	const ContainerType &container, const std::string separator_string = "")
{
	std::string                            tmp_string;
	typename ContainerType::const_iterator iter_b(container.begin());
	typename ContainerType::const_iterator iter_e(container.end());

	while (iter_b != iter_e) {
		tmp_string += *iter_b;
		++iter_b;
		if ((!separator_string.empty()) && (iter_b != iter_e))
			tmp_string += separator_string;
	}

	return(out_string.assign(tmp_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	std::string JoinString(const ContainerType &container,
	const std::string separator_string = "")
{
	std::string out_string;

	return(JoinString(out_string, container, separator_string.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct MultipleCharsToOneCharGenericFunctor {
	explicit MultipleCharsToOneCharGenericFunctor(const char *multi_chars,
		std::size_t multi_chars_length = 0)
		:multi_chars_(multi_chars)
		,multi_chars_length_(multi_chars_length)
	{
		if (!multi_chars_length_)
			multi_chars_length_ = ::strlen(multi_chars_);
	}

	bool operator () (std::string::value_type in_char) const
	{
		return(::memchr(multi_chars_, in_char, multi_chars_length_) != NULL);
	}

	const char  *multi_chars_;
	std::size_t  multi_chars_length_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ResolutionFunctor>
	std::string &MultipleCharsToOneChar(const std::string &in_string,
	std::string &out_string, const ResolutionFunctor &resolver,
	std::string::value_type single_char)
{
	if (in_string.empty())
		out_string.clear();
	else {
		bool                        in_multi_flag = false;
		std::string                 tmp_string(in_string.size(), '*');
		std::string::const_iterator iter_b(in_string.begin());
		std::string::const_iterator iter_e(in_string.end());
		std::string::iterator       iter_o(tmp_string.begin());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (!resolver(*iter_b)) {
				*iter_o++ = *iter_b;
				in_multi_flag = false;
			}
			else if (!in_multi_flag) {
				*iter_o++ = single_char;
				in_multi_flag = true;
			}
		}
		out_string.assign(tmp_string, 0,
			static_cast<std::size_t>(iter_o - tmp_string.begin()));
	}

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ResolutionFunctor>
	std::string MultipleCharsToOneChar(const std::string &in_string,
		const ResolutionFunctor &resolver, std::string::value_type single_char)
{
	std::string out_string;

	return(MultipleCharsToOneChar(in_string, out_string, resolver, single_char));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &MultipleCharsToOneChar(const std::string &in_string,
	std::string &out_string, const char *multiple_chars,
	std::string::value_type single_char);
std::string  MultipleCharsToOneChar(const std::string &in_string,
	const char *multiple_chars, std::string::value_type single_char);

std::string &OneSpace(const std::string &in_string, std::string &out_string);
std::string  OneSpace(const std::string &in_string);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline int CompareStringIgnoreCase(const std::string &lhs,
	const std::string &rhs)
{
	return(Utility_stricmp(lhs.c_str(), rhs.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct StringIsLessThanIgnoreCase {
	inline bool operator () (const std::string &lhs,
		const std::string &rhs) const
	{
		return(CompareStringIgnoreCase(lhs, rhs) < 0);
	}
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__StringSupport_hpp__HH

