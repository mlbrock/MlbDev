//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Provides wide-to-MBCS (and vice versa) conversion.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/WideChar.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __GNUC__
# include <stdlib.h>
#endif // #ifdef __GNUC__

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	CODE NOTE: Not yet implemented for MinGW gcc 3.4.5
#ifndef __MINGW32__
//	////////////////////////////////////////////////////////////////////////////
CharWideToMultiByte_char::CharWideToMultiByte_char(
	const std::wostringstream &in_string)
	:string_datum(Convert(in_string.str().c_str()))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharWideToMultiByte_char::CharWideToMultiByte_char(
	const std::wstring &in_string)
	:string_datum(Convert(in_string.c_str()))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharWideToMultiByte_char::CharWideToMultiByte_char(const wchar_t *in_string)
	:string_datum(Convert(in_string))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharWideToMultiByte_char::~CharWideToMultiByte_char()
{
	if (string_datum != NULL) {
		free(string_datum);
		string_datum = NULL;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *CharWideToMultiByte_char::Get() const
{
	return(string_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *CharWideToMultiByte_char::Convert(const wchar_t *in_string)
{
	int in_length = static_cast<int>(wcstombs(NULL, in_string, 0));

	if (in_length == -1)
		ThrowInvalidArgument("Wide character string can not be converted: "
			"One or more invalid characters were encountered.");

	if (in_length < 0) {
		std::ostringstream error_text;
		error_text << "Wide character string can not be converted: The function "
			"'wcstombs()' returned an invalid value (" << in_length << ").";
		ThrowStdException(error_text);
	}

	//	Add space for the ASCII NUL...
	++in_length;

	char *out_string;

	out_string = static_cast<char *>(calloc(static_cast<std::size_t>(in_length),
		sizeof(char)));
	if (out_string == NULL)
		ThrowBadAlloc("Wide character string can not be converted: "
			"Couldn't allocate memory for the destination string.");

	if (in_length > 1)
		wcstombs(out_string, in_string, static_cast<std::size_t>(in_length));

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharWideToMultiByte_string::CharWideToMultiByte_string(
	const std::wostringstream &in_string)
	:string_datum(Convert(in_string.str().c_str()))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharWideToMultiByte_string::CharWideToMultiByte_string(
	const std::wstring &in_string)
	:string_datum(Convert(in_string.c_str()))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharWideToMultiByte_string::CharWideToMultiByte_string(const wchar_t *in_string)
	:string_datum(Convert(in_string))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharWideToMultiByte_string::~CharWideToMultiByte_string()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const std::string CharWideToMultiByte_string::Get() const
{
	return(string_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string CharWideToMultiByte_string::Convert(const wchar_t *in_string)
{
	return(CharWideToMultiByte_char(in_string).Get());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharMultiByteToWide_char::CharMultiByteToWide_char(
	const std::ostringstream &in_string)
	:string_datum(Convert(in_string.str().c_str()))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharMultiByteToWide_char::CharMultiByteToWide_char(
	const std::string &in_string)
	:string_datum(Convert(in_string.c_str()))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharMultiByteToWide_char::CharMultiByteToWide_char(const char *in_string)
	:string_datum(Convert(in_string))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharMultiByteToWide_char::~CharMultiByteToWide_char()
{
	if (string_datum != NULL) {
		free(string_datum);
		string_datum = NULL;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const wchar_t *CharMultiByteToWide_char::Get() const
{
	return(string_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
wchar_t *CharMultiByteToWide_char::Convert(const char *in_string)
{
	int      in_length = static_cast<int>(mbstowcs(NULL, in_string, 0));

	if (in_length == -1)
		ThrowInvalidArgument("MBCS character string can not be converted: "
			"One or more invalid characters were encountered.");
			
	if (in_length < 0) {
		std::ostringstream error_text;
		error_text << "MBCS character string can not be converted: The function "
			"'mbstowcs()' returned an invalid value (" << in_length << ").";
		ThrowStdException(error_text);
	}

	//	Add space for the ASCII NUL...
	++in_length;

	wchar_t *out_string;

	out_string = static_cast<wchar_t *>(
		calloc(static_cast<std::size_t>(in_length), sizeof(wchar_t)));
	if (out_string == NULL)
		ThrowBadAlloc("MBCS character string can not be converted: "
			"Couldn't allocate memory for the destination string.");

	if (in_length > 1)
		mbstowcs(out_string, in_string, static_cast<std::size_t>(in_length));

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharMultiByteToWide_string::CharMultiByteToWide_string(
	const std::ostringstream &in_string)
	:string_datum(Convert(in_string.str().c_str()))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharMultiByteToWide_string::CharMultiByteToWide_string(
	const std::string &in_string)
	:string_datum(Convert(in_string.c_str()))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharMultiByteToWide_string::CharMultiByteToWide_string(const char *in_string)
	:string_datum(Convert(in_string))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
CharMultiByteToWide_string::~CharMultiByteToWide_string()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const std::wstring CharMultiByteToWide_string::Get() const
{
	return(string_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::wstring CharMultiByteToWide_string::Convert(const char *in_string)
{
	return(CharMultiByteToWide_char(in_string).Get());
}
//	////////////////////////////////////////////////////////////////////////////
#endif // # ifndef __MINGW32__

} // namespace Utility

} // namespace MLB


#ifdef TEST_MAIN

#include <cstring>
#include <iomanip>

using namespace MLB::Utility;

//	////////////////////////////////////////////////////////////////////////////
static const wchar_t      *TEST_WideCharList[] = {
	L"Line one",
	L"Line two",
	L"Line three",
	L"Line four",
	L"Line five",
	L"Line six",
	L"Line seven",
};
static const unsigned int  TEST_WideCharCount  =
	sizeof(TEST_WideCharList) / sizeof(TEST_WideCharList[0]);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int TEST_WideChar()
{
	int          return_code = EXIT_SUCCESS;
	unsigned int count_1;

	for (count_1 = 0; count_1 < TEST_WideCharCount; count_1++) {
		const wchar_t     *src_ptr   = TEST_WideCharList[count_1];
		const std::string  dst_ptr_s = CharWideToMultiByte_string(src_ptr).Get();
		const std::wstring dst_ptr_w = CharMultiByteToWide_char(dst_ptr_s).Get();
		std::cout << std::setw(3) << std::right << count_1 << ":" << std::left <<
			"[" << dst_ptr_s << "] ---> ";
		if (wcslen(src_ptr) != dst_ptr_w.length()) {
			std::cout << "ERROR: Lengths differ (" <<
				static_cast<unsigned int>(wcslen(src_ptr)) << " versus " <<
				static_cast<unsigned int>(dst_ptr_w.length()) << ")";
			return_code = EXIT_FAILURE;
		}
		else if (memcmp(src_ptr, dst_ptr_w.c_str(),
			dst_ptr_w.length() * sizeof(wchar_t))) {
			std::cout << "ERROR: Contents differ.";
			return_code = EXIT_FAILURE;
		}
		else
			std::cout << "OK";
		std::cout << std::endl;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test routine for class 'CharWideToMultiByte_char'" << std::endl;
	std::cout << "Test routine for class 'CharMultiByteToWide_char'" << std::endl;
	std::cout << "---- ------- --- ----- --------------------------" << std::endl;

	try {
		if (TEST_WideChar() != EXIT_SUCCESS)
			return_code = EXIT_FAILURE;
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl;
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

