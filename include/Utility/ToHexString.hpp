// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for hexadecimal string formatting.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__ToHexString_hpp__HH

#define HH__MLB__Utility__ToHexString_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file ToHexString.hpp

	\brief	Logic to support formatting of binary data in hexadecimal.
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

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY char        *ToHexString(std::size_t data_length,
	const char *data_ptr, char *out_ptr, int fill_char = ' ');
API_UTILITY std::string &ToHexString(std::size_t data_length,
	const char *data_ptr, std::string &out_string, int fill_char = ' ');
API_UTILITY std::string  ToHexString(std::size_t data_length,
	const char *data_ptr, int fill_char = ' ');

API_UTILITY char        *ToHexString(std::size_t data_length,
	const void *data_ptr, char *out_ptr, int fill_char = ' ');
API_UTILITY std::string &ToHexString(std::size_t data_length,
	const void *data_ptr, std::string &out_string, int fill_char = ' ');
API_UTILITY std::string  ToHexString(std::size_t data_length,
	const void *data_ptr, int fill_char = ' ');

API_UTILITY char        *ToHexString(const std::string &in_string,
	char *out_ptr, int fill_char = ' ');
API_UTILITY std::string &ToHexString(const std::string &in_string,
	std::string &out_string, int fill_char = ' ');
API_UTILITY std::string  ToHexString(const std::string &in_string,
	int fill_char = ' ');
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	char *DatumToHexString(const DataType &in_datum, char *out_ptr,
		int fill_char = ' ')
{
	return(ToHexString(sizeof(in_datum), &in_datum, out_ptr, fill_char));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string &DatumToHexString(const DataType &in_datum,
		std::string &out_string, int fill_char = ' ')
{
	return(ToHexString(sizeof(in_datum), &in_datum, out_string, fill_char));

}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string DatumToHexString(const DataType &in_datum, int fill_char = ' ')
{
	return(ToHexString(sizeof(in_datum), &in_datum, fill_char));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ToHexString_hpp__HH

