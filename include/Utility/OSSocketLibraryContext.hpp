// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Ensures that the operating system socket library has
								been initialized.

	Revision History	:	1992-09-13 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1992 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__OSSocketLibraryContext_hpp__HH

#define HH__MLB__Utility__OSSocketLibraryContext_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file 	OSSocketLibraryContext.hpp

	\brief	Logic to ensure that the operating system socket library has been
				initialized for use by the application.

	\note		Note that it is the application programmer's responsibility to
				include the correct Winsock include file before this include file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
/**
	Class which ensures that the operating system socket library has been
	initialized for use by the application.

	Useful only under Windows of course. On other operating systems, this
	class and its members resolve to an elaborate series of noops.

	Copied from the the old implementation in MLB::SockLibX , which in turn
	depends upon code from the Network Programming Support Library (NPSL).
*/
class OSSocketLibraryContext {
public:
	OSSocketLibraryContext()
	{
		StartSocketLibrary();
	}

	OSSocketLibraryContext(const OSSocketLibraryContext &)
	{
		StartSocketLibrary();
	}

	/**
		Virtual to permit lifetime guarantees for derived class instances to be
		provided by the language.
	*/
	virtual ~OSSocketLibraryContext()
	{
#ifdef _MSC_VER
		::WSACleanup();
#endif // #ifdef _MSC_VER
	}

private:
	void StartSocketLibrary()
	{
#ifdef _MSC_VER
		int     error_code;
		WORD    wsa_version;
		WSADATA wsa_data;

		wsa_version = MAKEWORD(2, 2);

		if ((error_code = ::WSAStartup(wsa_version, &wsa_data)) != 0) {
			std::ostringstream o_str;
			o_str << "Function 'WSAStartup()' had an error code of " <<
				error_code << ".";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
#endif // #ifdef _MSC_VER
	}
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__OSSocketLibraryContext_hpp__HH

