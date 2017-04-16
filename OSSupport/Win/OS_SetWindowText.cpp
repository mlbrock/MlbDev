// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	OS_SetWindowText.cpp

	File Description	:	Implementation of wrappers to the Windows function
								SetWindowText().

	Revision History	:	2016-04-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2017 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport/Win/OS_SetWindowText.hpp>

#include <Utility/Utility_Exception.hpp>

//#include <Winuser.h>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
int OS_SetWindowText(HWND window_handle, const char *window_text)
{
	int return_code;

	if ((return_code = ::SetWindowTextA(window_handle, window_text)) == 0) {
		std::ostringstream o_str;
		o_str << "Call to 'SetWindowTextA()' for window handle 0x" <<
			window_handle <<" failed";
		MLB::Utility::ThrowSystemError(o_str.str());
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int OS_SetWindowText(HWND window_handle, const std::string &window_text)
{
	return(OS_SetWindowText(window_handle, window_text.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

