// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	OS_GetWindowText.cpp

	File Description	:	Implementation of wrappers to the Windows function
								GetWindowText().

	Revision History	:	2016-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport/Win/OS_GetWindowText.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
int OS_GetWindowText(HWND window_handle, LPTSTR class_name_ptr,
	int class_name_length)
{
	if (class_name_length < 1)
		MLB::Utility::ThrowInvalidArgument("The length of the buffer in which "
			"to store the window text is less than 1.");

	int return_code = ::GetWindowText(window_handle, class_name_ptr,
		class_name_length);

	if (!return_code) {
		if (::GetLastError())
			MLB::Utility::ThrowSystemError("Call to GetWindowText(" +
				MLB::Utility::AnyToString(window_handle) + ", " +
				MLB::Utility::ValueToStringHex(class_name_ptr) + ", " +
				MLB::Utility::AnyToString(class_name_length) + ") failed");
		*class_name_ptr   = '\0';
		class_name_length = 0;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int OS_GetWindowText(HWND window_handle, char *class_name_ptr,
	std::size_t class_name_length)
{
	return(OS_GetWindowText(window_handle, class_name_ptr,
		static_cast<int>(class_name_length)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &OS_GetWindowText(HWND window_handle, std::string &class_name)
{
	std::size_t tmp_length = 256;
	std::string tmp_name;

	for ( ; ; ) {
		std::string(tmp_length, '\0').swap(tmp_name);
		int return_length = OS_GetWindowText(window_handle,
			const_cast<char *>(tmp_name.c_str()), tmp_name.size());
		if (static_cast<std::size_t>(return_length) < (tmp_name.size() - 1)) {
			class_name =
				tmp_name.substr(0, static_cast<std::size_t>(return_length));
			break;
		}
		tmp_length += 256;
	}

	return(class_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OS_GetWindowText(HWND window_handle)
{
	std::string class_name;

	return(OS_GetWindowText(window_handle, class_name));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

