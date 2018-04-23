// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	OS_EnumThreadWindows.cpp

	File Description	:	Implementation of wrappers to the Windows function
								EnumThreadWindows().

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

#include <OSSupport/Win/OS_EnumThreadWindows.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
bool OS_EnumThreadWindows(DWORD thread_id, WNDENUMPROC enum_func,
	LPARAM user_data)
{
	return(::EnumThreadWindows(thread_id, enum_func, user_data) != 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OS_EnumThreadWindows(DWORD thread_id,
	OS_EnumThreadWindowsFunctorBase &enum_functor)
{
	return(::EnumThreadWindows(thread_id,
		OS_EnumThreadWindowsFunctorBase::EnumThreadWindowFunc,
		reinterpret_cast<LPARAM>(&enum_functor)) != 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK OS_EnumThreadWindowsFunctorBase::EnumThreadWindowFunc(
	HWND window_thread, LPARAM user_data)
{
	OS_EnumThreadWindowsFunctorBase *base_ptr =
		reinterpret_cast<OS_EnumThreadWindowsFunctorBase *>(user_data);

	if (!base_ptr)
		MLB::Utility::ThrowLogicError("Attempt to cast a user data pointer "
			"to an instance of the hierarchy 'MLB::OSSupport::"
			"OS_EnumThreadWindowsFunctorBase' resulted in a dynamic_cast<> "
			"failure.");

	return(base_ptr->EnumFunction(window_thread));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

#include <OSSupport/Win/OS_GetClassName.hpp>
#include <OSSupport/Win/OS_GetWindowText.hpp>

using namespace MLB::OSSupport;

namespace {

// ////////////////////////////////////////////////////////////////////////////
class MyEnumFunc : public OS_EnumThreadWindowsFunctorBase {
	virtual BOOL EnumFunction(HWND window_thread)
	{
		std::string class_name(OS_GetClassName(window_thread));

		std::cout << std::setw(8) << window_thread << ": " <<
			std::left << std::setw(19) << class_name.substr(0, 19) <<
			std::right << ((class_name.size() > 19) ? '>' : ' ') << ": " <<
			OS_GetWindowText(window_thread) << std::endl;

		return(TRUE);
	}
};
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		HWND       fg_window   = ::GetForegroundWindow();
		DWORD      fg_win_tid = ::GetWindowThreadProcessId(fg_window, NULL);
		MyEnumFunc my_func;
		OS_EnumThreadWindows(fg_win_tid, my_func);
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_EnumThreadWindows.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

