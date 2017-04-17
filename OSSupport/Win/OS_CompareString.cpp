// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	OS_CompareString.cpp

	File Description	:	Implementation of wrappers to the Windows function
								CompareString().

	Revision History	:	2016-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport/Win/OS_CompareString.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
int OS_CompareString(LCID locale_name, DWORD compare_flags,
	LPCSTR string_ptr_1, int string_len_1, LPCSTR string_ptr_2,
	int string_len_2)
{
	int return_code = ::CompareStringA(locale_name, compare_flags, string_ptr_1,
		string_len_1, string_ptr_2, string_len_2);

	if (!return_code)
		MLB::Utility::ThrowSystemError("Call to CompareStringA(" +
			MLB::Utility::ValueToStringHex(locale_name) + ", " +
			MLB::Utility::ValueToStringHex(compare_flags, false) + ", " +
			MLB::Utility::ValueToStringHex(string_ptr_1, true) + ", " +
			MLB::Utility::AnyToString(string_len_1) + ", " +
			MLB::Utility::ValueToStringHex(string_ptr_2, true) + ", " +
			MLB::Utility::AnyToString(string_len_2) + ") failed");

	return(return_code - 2);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int OS_CompareString(LCID locale_name, DWORD compare_flags,
	const std::string &string_1, const std::string &string_2)
{
	return(OS_CompareString(locale_name, compare_flags, string_1.c_str(),
		static_cast<int>(string_1.size()), string_2.c_str(),
		static_cast<int>(string_2.size())));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

