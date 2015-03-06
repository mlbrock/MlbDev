// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File for C Language Support (memory.h)
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Memory functions source module for the C
								language support library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
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
const void *C_memchr(const void *buffer_ptr, int src_char, size_t data_length,
	const char *name_string)
{
	CheckCountTypeAndValue(data_length, name_string);

	if (buffer_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memchr()' failed: buffer pointer is 'NULL'.";
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

	return(::memchr(buffer_ptr, src_char, data_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int C_memcmp(const void *ptr_1, const void *ptr_2, size_t data_length,
	const char *name_string)
{
	CheckCountTypeAndValue(data_length, name_string);

	if (ptr_1 == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memcmp()' failed: first pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if (ptr_2 == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memcmp()' failed: second pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	return(::memcmp(ptr_1, ptr_2, data_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *C_memcpy(void *dst_ptr, const void *src_ptr, size_t data_length,
	const char *name_string)
{
	CheckCountTypeAndValue(data_length, name_string);

	if (dst_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memcpy()' failed: destination pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if (src_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memcpy()' failed: source pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	return(::memcpy(dst_ptr, src_ptr, data_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *C_memmove(void *dst_ptr, const void *src_ptr, size_t data_length,
	const char *name_string)
{
	CheckCountTypeAndValue(data_length, name_string);

	if (dst_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memmove()' failed: destination pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if (src_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memmove()' failed: source pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	return(::memmove(dst_ptr, src_ptr, data_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *C_memset(void *dst_ptr, int src_char, size_t data_length,
	const char *name_string)
{
	CheckCountTypeAndValue(data_length, name_string);

	if (dst_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memset()' failed: destination pointer is 'NULL'.";
		ThrowInvalidArgument(error_text);
	}

	if ((src_char < std::numeric_limits<unsigned char>::min()) ||
		(src_char > std::numeric_limits<unsigned char>::max())) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Invocation of 'memset()' failed: source character ('" << src_char <<
			"') is outside of the permissible range (" <<
			std::numeric_limits<unsigned char>::min() << " to " <<
			std::numeric_limits<unsigned char>::max() << ", inclusive).";
		ThrowInvalidArgument(error_text);
	}

	return(::memset(dst_ptr, src_char, data_length));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

