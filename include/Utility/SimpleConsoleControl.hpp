//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for SimpleConsoleControl class.

	Revision History	:	1996-02-21 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1996 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__SimpleConsoleControl_hpp__HH

#define HH__MLB__Utility__SimpleConsoleControl_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
/**
	\file SimpleConsoleControl.hpp

	\brief	The SimpleConsoleControl header file.
*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
struct SimpleConsoleControl {
	SimpleConsoleControl()
		:output_handle_(::GetStdHandle(STD_OUTPUT_HANDLE))
	{
	}

	void SetTopLeft() const
	{
		COORD top_left;

		top_left.X = 0;
		top_left.Y = 0;

		if (!::SetConsoleCursorPosition(output_handle_, top_left))
			ThrowSystemError("Call to ::SetConsoleCursorPosition() failed");
	}

	void ClearScreen() const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		if (!::GetConsoleScreenBufferInfo(output_handle_, &csbi))
			ThrowSystemError("Call to ::GetConsoleScreenBufferInfo() failed");

		DWORD dwConSize = static_cast<DWORD>(csbi.dwSize.X * csbi.dwSize.Y);
		DWORD cCharsWritten;
		COORD top_left;

		top_left.X = 0;
		top_left.Y = 0;

		if (!::FillConsoleOutputCharacter(output_handle_, (TCHAR) ' ',
			dwConSize, top_left, &cCharsWritten))
			ThrowSystemError("Call to ::FillConsoleOutputCharacter() failed");

		SetTopLeft();
	}

	HANDLE output_handle_;
};
#else
struct SimpleConsoleControl {
	void SetTopLeft() const
	{
		std::cout << "\x1b[1;1H" << std::flush;
	}

	void ClearScreen() const
	{
		std::cout << "\x1b[2J" << std::flush;
	}
};
#endif // #ifdef _MSC_VER
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__SimpleConsoleControl_hpp__HH

