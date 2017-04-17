// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File for C Language Support (stdlib.h)
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Stdlib functions source module for the C language
								support library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2017.
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

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
void *C_calloc(size_t number_of_elements, size_t size_of_element,
	const char *name_string)
{
	if ((number_of_elements < 1) || (size_of_element < 1)) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Attempt to invoke calloc(" <<
			static_cast<unsigned int>(number_of_elements) << ", " <<
			static_cast<unsigned int>(size_of_element) <<
			") with invalid arguments.";
		ThrowInvalidArgument(error_text);
	}

	void *result_ptr = ::calloc(number_of_elements, size_of_element);

	if (result_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Attempt to invoke calloc(" <<
			static_cast<unsigned int>(number_of_elements) << ", " <<
			static_cast<unsigned int>(size_of_element) << ") for a total of " <<
			static_cast<unsigned int>(number_of_elements * size_of_element) <<
			" bytes failed.";
		ThrowBadAlloc(error_text);
	}

	return(result_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void C_free(void *mem_ptr, const char * /* name_string */)
{
	if (mem_ptr != NULL)
		::free(mem_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *C_malloc(size_t alloc_size, const char *name_string)
{
	if (alloc_size < 1) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Attempt to invoke malloc(" <<
			static_cast<unsigned int>(alloc_size) << ") with invalid arguments.";
		ThrowInvalidArgument(error_text);
	}

	void *result_ptr = ::malloc(alloc_size);

	if (result_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Attempt to invoke malloc(" <<
			static_cast<unsigned int>(alloc_size) << ") for a total of " <<
			static_cast<unsigned int>(alloc_size) << " bytes failed.";
		ThrowBadAlloc(error_text);
	}

	return(result_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *C_realloc(void *mem_ptr, size_t alloc_size,
	const char *name_string)
{
	//	Behavior of realloc() as specified by the standard.
	if ((!alloc_size) && (mem_ptr != NULL)) {
		::free(mem_ptr);
		return(NULL);
	}

	if ((alloc_size < 1) || (mem_ptr == NULL)) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Attempt to invoke realloc(0x" << std::hex << mem_ptr << std::dec <<
				", " << static_cast<unsigned int>(alloc_size) <<
				") with invalid arguments.";
		ThrowInvalidArgument(error_text);
	}

	void *result_ptr = ::realloc(mem_ptr, alloc_size);

	if (result_ptr == NULL) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"Attempt to invoke realloc(0x" << std::hex << mem_ptr << std::dec <<
				", " << static_cast<unsigned int>(alloc_size) <<
				") for a total of " << static_cast<unsigned int>(alloc_size) <<
			" bytes failed.";
		ThrowBadAlloc(error_text);
	}

	return(result_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

