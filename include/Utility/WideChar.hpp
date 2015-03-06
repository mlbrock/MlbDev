// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the wide-character support.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__WideChar_hpp__HH

#define HH__MLB__Utility__WideChar_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file WideChar.hpp

	\brief	The MLB wide-character support header file.
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

#ifndef __MINGW32__
//	//////////////////////////////////////////////////////////////////////////
struct API_UTILITY CharWideToMultiByte_char {
	explicit CharWideToMultiByte_char(const std::wostringstream &in_string);
	explicit CharWideToMultiByte_char(const std::wstring &in_string);
	explicit CharWideToMultiByte_char(const wchar_t *in_string);
	~CharWideToMultiByte_char();

	const char *Get() const;

	char *string_datum;

	static char *Convert(const wchar_t *in_string);
};
//	//////////////////////////////////////////////////////////////////////////
#endif // #ifndef __MINGW32__

#ifndef __MINGW32__
//	//////////////////////////////////////////////////////////////////////////
struct API_UTILITY CharWideToMultiByte_string {
	explicit CharWideToMultiByte_string(const std::wostringstream &in_string);
	explicit CharWideToMultiByte_string(const std::wstring &in_string);
	explicit CharWideToMultiByte_string(const wchar_t *in_string);
	~CharWideToMultiByte_string();

	const std::string Get() const;

	std::string string_datum;

	static std::string Convert(const wchar_t *in_string);
};
//	//////////////////////////////////////////////////////////////////////////
#endif // #ifndef __MINGW32__

#ifndef __MINGW32__
//	//////////////////////////////////////////////////////////////////////////
struct API_UTILITY CharMultiByteToWide_char {
	explicit CharMultiByteToWide_char(const std::ostringstream &in_string);
	explicit CharMultiByteToWide_char(const std::string &in_string);
	explicit CharMultiByteToWide_char(const char *in_string);
	~CharMultiByteToWide_char();

	const wchar_t *Get() const;

	wchar_t *string_datum;

	static wchar_t *Convert(const char *in_string);
};
//	//////////////////////////////////////////////////////////////////////////
#endif // #ifndef __MINGW32__

//	CODE NOTE: Not yet implemented for MinGW gcc 3.4.5
#ifndef __MINGW32__
//	//////////////////////////////////////////////////////////////////////////
struct API_UTILITY CharMultiByteToWide_string {
	explicit CharMultiByteToWide_string(const std::ostringstream &in_string);
	explicit CharMultiByteToWide_string(const std::string &in_string);
	explicit CharMultiByteToWide_string(const char *in_string);
	~CharMultiByteToWide_string();

	const std::wstring Get() const;

	std::wstring string_datum;

	static std::wstring Convert(const char *in_string);
};
//	//////////////////////////////////////////////////////////////////////////
#endif // #ifndef __MINGW32__

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__WideChar_hpp__HH

