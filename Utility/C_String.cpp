// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File for C Language Support (string.h)
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	String functions source module for the C language
								support library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __GNUC__
# include <string.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
char *C_strcat(char *dst_string, const char *src_string,
	const char *name_string)
{
	if (dst_string == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'strcat()' failed: destination string is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if (src_string == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'strcat()' failed: source string is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	return(::strcpy(dst_string, src_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *C_strcpy(char *dst_string, const char *src_string,
	const char *name_string)
{
	if (dst_string == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'strcpy()' failed: destination string is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if (src_string == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'strcpy()' failed: souce string is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	return(::strcpy(dst_string, src_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *C_strdup(const char *src_string, const char *name_string)
{
	if (src_string == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'strdup()' failed: source string is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	char *result_ptr = ::strdup(src_string);

	if (result_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'strdup()' failed: source string of length " <<
			(static_cast<unsigned int>(strlen(src_string)) + 1) <<
			" bytes (including NUL terminator) couldn't be copied.";
		ThrowBadAlloc(error_text);
	}

	return(result_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int C_stricmp(char *ptr_1, const char *ptr_2, const char *name_string)
{
	if (ptr_1 == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'stricmp()' failed: destination string is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if (ptr_2 == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'stricmp()' failed: souce string is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	return(Utility_stricmp(ptr_1, ptr_2));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Utility_stricmp(const char *ptr_1, const char *ptr_2)
{
#ifndef __GNUC__
	return(::stricmp(ptr_1, ptr_2));
#else
	return(::strcasecmp(ptr_1, ptr_2));
#endif // #ifndef __GNUC__
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Utility_strnicmp(const char *ptr_1, const char *ptr_2, size_t data_length)
{
#ifndef __GNUC__
	return(::strnicmp(ptr_1, ptr_2, data_length));
#else
	return(::strncasecmp(ptr_1, ptr_2, data_length));
#endif // #ifndef __GNUC__
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

