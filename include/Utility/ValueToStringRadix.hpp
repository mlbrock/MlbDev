// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for formatting integral values as unsigned
								strings in radices 2, 8, 10 and 16.
								in various radices.

	Revision History	:	1995-10-10 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1995 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__ValueToStringRadix_hpp__HH

#define HH__MLB__Utility__ValueToStringRadix_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file 	ValueToStringRadix.hpp

	\brief	Logic to support formatting of integral values as unsigned strings
				in radices 2, 8, 10 and 16.
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

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4826)
# endif // # if _MSC_VER < 1400

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string ValueToStringHex(const DataType &datum, bool prefix_flag = true,
	char pad_char = '0')
{
	//	Yep, that's a C-style cast. The other functions do the same...
	const unsigned long long tmp_datum = ((const unsigned long long) datum);
	std::ostringstream       tmp_text;

	tmp_text << ((prefix_flag) ? "0x" : "") << std::hex <<
		std::setfill(pad_char) <<
		std::setw(sizeof(datum) * 2) << tmp_datum << std::setfill(' ');

	return(tmp_text.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string ValueToStringDec(const DataType &datum, bool prefix_flag = true,
	char pad_char = '0')
{
	const unsigned long long tmp_datum = ((const unsigned long long) datum);
	std::ostringstream       tmp_text;

	tmp_text << ((prefix_flag) ? "0d" : "") << std::dec <<
		std::setfill(pad_char) <<
		std::setw(static_cast<std::streamsize>(
			IntegralValueMaxLengthDec<DataType>())) <<
		tmp_datum << std::setfill(' ');

	return(tmp_text.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string ValueToStringOct(const DataType &datum, bool prefix_flag = true,
	char pad_char = '0')
{
	const unsigned long long tmp_datum = ((const unsigned long long) datum);
	std::ostringstream       tmp_text;

	tmp_text <<  ((prefix_flag) ? "0o" : "") << std::oct <<
		std::setfill(pad_char) <<
		std::setw(sizeof(datum) * 3) << tmp_datum << std::setfill(' ');

	return(tmp_text.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string ValueToStringBin(const DataType &datum, bool prefix_flag = true,
	char pad_char = '0')
{
	const unsigned long long tmp_datum = ((const unsigned long long) datum);
	std::ostringstream       tmp_text;

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4244)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	tmp_text <<  ((prefix_flag) ? "0b" : "") <<
		std::setfill(pad_char) <<
		std::setw(sizeof(datum) * CHAR_BIT) <<
		std::bitset<sizeof(datum) * CHAR_BIT>(tmp_datum) << std::setfill(' ');
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

	return(tmp_text.str());
}
// ////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
# pragma warning(pop)
# endif // # if _MSC_VER < 1400

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ValueToStringRadix_hpp__HH

