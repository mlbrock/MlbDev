// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	OS_EnumWindows.cpp

	File Description	:	Implementation of wrappers to the Windows function
								EnumWindows().

	Revision History	:	2017-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2017 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport/Win/OS_EnumWindows.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
bool OS_EnumWindows(WNDENUMPROC enum_func, LPARAM user_data)
{
	return(::EnumWindows(enum_func, user_data) != 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool OS_EnumWindows(OS_EnumWindowsFunctorBase &enum_functor)
{
	return(::EnumWindows(OS_EnumWindowsFunctorBase::EnumWindowFunc,
		reinterpret_cast<LPARAM>(&enum_functor)) != 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK OS_EnumWindowsFunctorBase::EnumWindowFunc(
	HWND window_handle, LPARAM user_data)
{
	OS_EnumWindowsFunctorBase *base_ptr =
		reinterpret_cast<OS_EnumWindowsFunctorBase *>(user_data);

	if (!base_ptr)
		MLB::Utility::ThrowLogicError("Attempt to cast a user data pointer "
			"to an instance of the hierarchy 'MLB::OSSupport::"
			"OS_EnumWindowsFunctorBase' resulted in a dynamic_cast<> "
			"failure.");

	return(base_ptr->EnumFunction(window_handle));
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
class MyEnumFunc : public OS_EnumWindowsFunctorBase {
	virtual BOOL EnumFunction(HWND window_handle)
	{
		std::string class_name(OS_GetClassName(window_handle));

		std::cout << std::setw(8) << window_handle << ": " <<
			std::left << std::setw(19) << class_name.substr(0, 19) <<
			std::right << ((class_name.size() > 19) ? '>' : ' ') << ": " <<
			OS_GetWindowText(window_handle) << std::endl;

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
		MyEnumFunc my_func;
		OS_EnumWindows(my_func);
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_EnumWindows.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

