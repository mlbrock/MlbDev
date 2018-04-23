// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	StringRadixOps.cpp

	File Description	:	Implementation of the new, yet-to-be-fully-implemented,
								string <---> integer radix functions.

	Revision History	:	1993-10-10 --- Portions of original logic from
													MlbDev/Utility/IntToString.cpp.
									Michael L. Brock
								2016-08-07 --- Base-62 specific logic first-cut.
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/StringRadixOps.hpp>

#include <cstring>
#include <stdexcept>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <typename DstType, DstType SrcRadix>
	DstType StringToUIntRadix(const char *begin_ptr, const char *end_ptr,
		const char **last_ptr = NULL)
{
	const char *curr_ptr = begin_ptr;

	while ((curr_ptr < end_ptr) && (::isspace(*curr_ptr) || (*curr_ptr == '0')))
		++curr_ptr;

	DstType old_dst_value = 0;
	DstType dst_value     = 0;
	DstType tmp_value     = 0;

	while (curr_ptr < end_ptr) {
		old_dst_value  = dst_value;
		dst_value     *= static_cast<DstType>(SrcRadix);
		if (::isdigit(*curr_ptr))
			tmp_value += *curr_ptr - '0';
		else if (::isupper(*curr_ptr))
			tmp_value += (*curr_ptr - 'A') + 10;
		else if (::islower(*curr_ptr))
			tmp_value += (*curr_ptr - 'a') + 10 + 26;
		else
			break;
		if (tmp_value >= SrcRadix)
			break;
		dst_value += tmp_value;
		if (dst_value < old_dst_value) {
			/*
				We over-flowed.

				If last_ptr is not NULL, we'll set it to point to the offending
				character. Caller detects the overflow because that character is
				a valid character for the radix.

				Otherwise, we throw.
			*/
			if (last_ptr) {
				dst_value = old_dst_value;
				break;
			}
			std::ostringstream o_str;
			o_str << "Conversion of the string '" <<
				std::string(begin_ptr, end_ptr) << "' failed at index " <<
				(curr_ptr - begin_ptr) << " ('" << *curr_ptr << " = ASCII " <<
				static_cast<unsigned int>(static_cast<unsigned char>(*curr_ptr)) <<
				").";
			throw std::overflow_error(o_str.str());
		}
		++curr_ptr;
	}

	if (last_ptr)
		*last_ptr = curr_ptr;

	return(dst_value);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DstType, DstType SrcRadix>
	DstType StringToUIntRadix(std::size_t src_len, const char *src_ptr,
		const char **last_ptr = NULL)
{
	return(StringToUIntRadix<DstType, SrcRadix>(src_ptr,
		src_ptr + src_len, last_ptr));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DstType, DstType SrcRadix>
	DstType StringToUIntRadix(const char *src_ptr,
		const char **last_ptr = NULL)
{
	return(StringToUIntRadix<DstType, SrcRadix>(::strlen(src_ptr), src_ptr,
		last_ptr));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DstType, DstType SrcRadix>
	DstType StringToUIntRadix(const std::string &src,
		std::size_t *last_offset = NULL)
{
	DstType     return_value;
	const char *last_ptr;

	return_value = StringToUIntRadix<DstType, SrcRadix>(src.size(),
		src.c_str(), &last_ptr);

	if (last_offset)
		*last_offset = static_cast<std::size_t>(last_ptr - src.c_str());

	return(return_value);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous  namespace

// ////////////////////////////////////////////////////////////////////////////
Native_UInt64 StringToUIntRadix62(const char *begin_ptr, const char *end_ptr,
	const char **last_ptr)
{
	return(StringToUIntRadix<Native_UInt64, 62>(begin_ptr, end_ptr, last_ptr));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
Native_UInt64 StringToUIntRadix62(std::size_t src_len, const char *src_ptr,
	const char **last_ptr)
{
	return(StringToUIntRadix<Native_UInt64, 62>(src_len, src_ptr, last_ptr));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
Native_UInt64 StringToUIntRadix62(const char *src_ptr,
	const char **last_ptr)
{
	return(StringToUIntRadix<Native_UInt64, 62>(src_ptr, last_ptr));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
Native_UInt64 StringToUIntRadix62(const std::string &src,
	std::size_t *last_offset)
{
	return(StringToUIntRadix<Native_UInt64, 62>(src, last_offset));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <cstdlib>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << StringToUIntRadix62("47")          << "\n";
		std::cout << StringToUIntRadix62("H31")         << "\n";
		std::cout << StringToUIntRadix62("LygHa16AHYF") << "\n";
	}
	catch (const std::exception &except) {
		std::cout << std::endl;
		std::cout << "Regression test error: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

