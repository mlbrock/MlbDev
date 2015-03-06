// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The basic VFast exception class include file.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__VFastException_hpp__HH

#define HH__MLB__VFast__VFastException_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file VFastException.hpp

	\brief	The basic VFast exception class header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	The base class for exceptions related to the VFast library.
*/
class MB_LIB_EXCEPTION_CLASS(API_VFAST) VFastException :
	public MLB::Utility::ExceptionGeneral {
public:
	explicit VFastException(const char *except_string = NULL);
	explicit VFastException(const std::string &except_string);
	explicit VFastException(const std::ostringstream &except_string);

	~VFastException() MBCOMPAT_EXCEPT_NOTHROW;

	virtual void Rethrow(const char *except_string = NULL) const;

private:
	static std::string GetFixedString(const char *except_string);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__VFastException_hpp__HH

