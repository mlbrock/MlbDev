//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of various string support logic.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/StringSupport.hpp>

#include <algorithm>
#include <deque>

#ifndef _Windows
# include <unistd.h>
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

//	//////////////////////////////////////////////////////////////////////////
struct PRIVATE_toupper {
	std::string::value_type operator () (std::string::value_type in_char) const {
		return(static_cast<std::string::value_type>(std::toupper(in_char)));
	}
};
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
struct PRIVATE_tolower {
	std::string::value_type operator () (std::string::value_type in_char) const {
		return(static_cast<std::string::value_type>(std::tolower(in_char)));
	}
};
//	//////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	//////////////////////////////////////////////////////////////////////////
std::string &UpperCase(const std::string &in_string,
	std::string &out_string)
{
	out_string = in_string;

	std::transform(out_string.begin(), out_string.end(),
		out_string.begin(), PRIVATE_toupper());

	return(out_string);
}
//	//////////////////////////////////////////////////////////////////////////

/*
	IMPLEMENTATION NOTE: An alternative approach.
	std::transform(element_name_normal_.begin(), element_name_normal_.end(),
		element_name_normal_.begin(), static_cast<int (*)(int)>(std::tolower));
*/
//	//////////////////////////////////////////////////////////////////////////
std::string &LowerCase(const std::string &in_string,
	std::string &out_string)
{
	out_string = in_string;

	std::transform(out_string.begin(), out_string.end(),
		out_string.begin(), PRIVATE_tolower());

	return(out_string);
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string UpperCase(const std::string &in_string)
{
	std::string out_string;

	return(UpperCase(in_string, out_string));
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string LowerCase(const std::string &in_string)
{
	std::string out_string;

	return(LowerCase(in_string, out_string));
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string &TrimRightString(const std::string &in_string,
	std::string &out_string, const std::string &trim_char_list)
{
	out_string = in_string;

	return(out_string.erase(out_string.find_last_not_of(trim_char_list) + 1));
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string &TrimLeftString(const std::string &in_string,
	std::string &out_string, const std::string &trim_char_list)
{
	out_string = in_string;

	return(out_string.erase(0, out_string.find_first_not_of(trim_char_list)));
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string &TrimString(const std::string &in_string,
	std::string &out_string, const std::string &trim_char_list)
{
	out_string = in_string;

	out_string.erase(out_string.find_last_not_of(trim_char_list) + 1);

	return(out_string.erase(0, out_string.find_first_not_of(trim_char_list)));
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string TrimRight(const std::string &in_string,
	const std::string &trim_char_list)
{
	std::string out_string;

	return(TrimRightString(in_string, out_string, trim_char_list));
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string TrimLeft(const std::string &in_string,
	const std::string &trim_char_list)
{
	std::string out_string;

	return(TrimLeftString(in_string, out_string, trim_char_list));
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string Trim(const std::string &in_string,
	const std::string &trim_char_list)
{
	std::string out_string;

	return(TrimString(in_string, out_string, trim_char_list));
}
//	//////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &PadLeft(const std::string &in_string, std::string &out_string,
	unsigned int pad_length, char pad_char)
{
	if (in_string.size() >= static_cast<std::string::size_type>(pad_length))
		out_string.assign(in_string, 0, pad_length);
	else {
		std::ostringstream o_str;
		o_str << std::right << std::setfill(pad_char) <<
			std::setw(static_cast<std::streamsize>(pad_length)) <<
			in_string << std::setfill(' ');
		out_string = o_str.str();
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string PadLeft(const std::string &in_string, unsigned int pad_length,
	char pad_char)
{
	std::string out_string;

	return(PadLeft(in_string, out_string, pad_length, pad_char));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &PadRight(const std::string &in_string, std::string &out_string,
	unsigned int pad_length, char pad_char)
{
	if (in_string.size() >= static_cast<std::string::size_type>(pad_length))
		out_string.assign(in_string, 0, pad_length);
	else {
		std::ostringstream o_str;
		o_str << std::left << std::setfill(pad_char) <<
			std::setw(static_cast<std::streamsize>(pad_length)) <<
			in_string << std::right << std::setfill(' ');
		out_string = o_str.str();
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string PadRight(const std::string &in_string,
	unsigned int pad_length, char pad_char)
{
	std::string out_string;

	return(PadRight(in_string, out_string, pad_length, pad_char));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
StringVector &SplitString(const std::string &in_string,
	const std::string &find_string, StringVector &out_list,
	std::string::size_type start_pos, bool discard_empty_sections)
{
	std::deque<std::string> tmp_list;
	std::string::size_type  in_length   = in_string.size();
	std::string::size_type  find_length = find_string.size();

	if (!find_length) {
		out_list.clear();
		return(out_list);
	}

	//	Append to the list any separator-delimited sub-string sections.
	while (start_pos < in_length) {
		std::string::size_type found_pos = in_string.find(find_string, start_pos);
		if (found_pos == std::string::npos)
			break;
		if ((!discard_empty_sections) || (found_pos != start_pos))
			tmp_list.push_back(in_string.substr(start_pos, found_pos - start_pos));
		start_pos = found_pos + find_length;
	}

	//	There may be a trailing sub-string; if so, grab it...
	if ((start_pos <= in_length) &&
		((!discard_empty_sections) || (start_pos < in_length)))
		tmp_list.push_back(in_string.substr(start_pos));

	out_list.assign(tmp_list.begin(), tmp_list.end());

	return(out_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
StringVector GetSplit(const std::string &in_string,
	const std::string &find_string, std::string::size_type start_pos,
	bool discard_empty_sections_flag)
{
	StringVector out_list;

	return(SplitString(in_string, find_string, out_list, start_pos,
		discard_empty_sections_flag));
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &MultipleCharsToOneChar(const std::string &in_string,
	std::string &out_string, const char *multiple_chars,
	std::string::value_type single_char)
{
	return(MultipleCharsToOneChar(in_string, out_string,
		MultipleCharsToOneCharGenericFunctor(multiple_chars), single_char));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string MultipleCharsToOneChar(const std::string &in_string,
	const char *multiple_chars, std::string::value_type single_char)
{
	return(MultipleCharsToOneChar(in_string,
		MultipleCharsToOneCharGenericFunctor(multiple_chars), single_char));
}
// ////////////////////////////////////////////////////////////////////////////

namespace {
// ////////////////////////////////////////////////////////////////////////////
struct OneSpaceFunctor {
	bool operator () (std::string::value_type in_char) const
	{
		return(::isspace(in_char) != 0);
	}
};
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::string &OneSpace(const std::string &in_string, std::string &out_string)
{
	return(MultipleCharsToOneChar(in_string, out_string, OneSpaceFunctor(),' '));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OneSpace(const std::string &in_string)
{
	std::string out_string;

	return(OneSpace(in_string, out_string));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

