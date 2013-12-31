// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File for C Language Support (extended memory.h)
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Memory extension functions source module for the C
								language support library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2014.
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
# include <memory.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
void *C_memccpy(void *dst_ptr, const void *src_ptr, int src_char,
	size_t data_length, const char *name_string)
{
	CheckCountTypeAndValue(data_length, name_string);

	if (dst_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memccpy()' failed: destination pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if (src_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memccpy()' failed: source pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if ((src_char < std::numeric_limits<unsigned char>::min()) ||
		(src_char > std::numeric_limits<unsigned char>::max())) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memchr()' failed: search character ('" << src_char <<
			"') is outside of the permissible range (" <<
			std::numeric_limits<unsigned char>::min() << " to " <<
			std::numeric_limits<unsigned char>::max() << ", inclusive).";
		ThrowInvalidArgument(error_text);
	}

	return(::memccpy(dst_ptr, src_ptr, src_char, data_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *C_memdup(const void *src_ptr, size_t src_length,
	const char *name_string)
{
	CheckCountTypeAndValue(src_length, name_string);

	if (src_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memdup()' failed: source pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	void *result_ptr = ::malloc(src_length);

	if (result_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memdup()' failed: source memory area of length " <<
			static_cast<unsigned int>(src_length) << " bytes couldn't be copied.";
		ThrowBadAlloc(error_text);
	}

	return(::memcpy(result_ptr, src_ptr, src_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int C_memicmp(const void *ptr_1, const void *ptr_2, size_t data_length,
	const char *name_string)
{
	CheckCountTypeAndValue(data_length, name_string);

	if (ptr_1 == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memicmp()' failed: first pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if (ptr_2 == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memicmp()' failed: second pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	return(Utility_memicmp(ptr_1, ptr_2, data_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Utility_memicmp(const void *ptr_1, const void *ptr_2, size_t data_length)
{
#ifndef __GNUC__
	return(::memicmp(ptr_1, ptr_2, data_length));
#else
	const signed char *tp_1 = static_cast<const signed char *>(ptr_1);
	const signed char *tp_2 = static_cast<const signed char *>(ptr_2);

	while (data_length--) {
		int cmp = ::toupper(*tp_1) - ::toupper(*tp_2);
		if (cmp)
			return(cmp);
		++tp_1;
		++tp_2;
	}

	return(0);
#endif // #ifndef __GNUC__
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

