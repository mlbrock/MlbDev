// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	OS_EnumThreadWindows.hpp

	File Description	:	Include file for the operating system support library
								for the Windows function EnumThreadWindows.

	Revision History	:	2016-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__OSSupport__OS_EnumThreadWindows_hpp___HH

#define HH___MLB__OSSupport__OS_EnumThreadWindows_hpp___HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport/OSSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
class OS_EnumThreadWindowsFunctorBase {
public:
	OS_EnumThreadWindowsFunctorBase()
	{
	}

	virtual ~OS_EnumThreadWindowsFunctorBase()
	{
	}

	virtual BOOL EnumFunction(HWND /* window_thread */)
	{
		return(TRUE);
	}

	static BOOL CALLBACK EnumThreadWindowFunc(HWND window_thread,
		LPARAM user_data);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	User32.lib/EnumThreadWindows()
API_OSSUPPORT bool                  OS_EnumThreadWindows(DWORD thread_id,
	WNDENUMPROC enum_func, LPARAM user_data);
API_OSSUPPORT bool                  OS_EnumThreadWindows(DWORD thread_id,
	OS_EnumThreadWindowsFunctorBase &enum_functor);
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#endif // #ifndef HH___MLB__OSSupport__OS_EnumThreadWindows_hpp___HH

