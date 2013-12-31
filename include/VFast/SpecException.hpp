// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The VFAST specification exception class include file.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__SpecException_hpp__HH

#define HH__MLB__VFast__SpecException_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file SpecException.hpp

	\brief	The VFAST specification exception class include file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/SpecExceptionBase.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	A class template for the reportable exceptions as specified by the \e FAST
	\e Specification \e 1.1 dated \e 2006-12-20, \e Appendix \e 4 \e Summary
	\e of \e Error \e Codes , pages 42 - 43.
	
	The template is specialized with a character which depends upon the type
	of the exception to be reported:

		- D = Dynamic
		- R = Reportable
		- S = Static
*/
template <char ExceptionType>
class MB_LIB_EXCEPTION_CLASS(API_VFAST) SpecException :
	public SpecExceptionBase {
public:
	explicit SpecException(unsigned int exception_number,
		const char *except_string = NULL)
		:SpecExceptionBase(ExceptionType, exception_number, except_string)
	{
	}
	SpecException(unsigned int exception_number,
		const std::string &except_string)
		:SpecExceptionBase(ExceptionType, exception_number, except_string)
	{
	}
	SpecException(unsigned int exception_number,
		const std::ostringstream &except_string)
		:SpecExceptionBase(ExceptionType, exception_number, except_string)
	{
	}

	~SpecException() MBCOMPAT_EXCEPT_NOTHROW
	{
	}

	virtual void Rethrow(const char *except_string = NULL) const
	{
		SpecException<ExceptionType> tmp_except(*this);

		tmp_except.SetWhat(except_string);

		throw tmp_except;
	}

	static void ThrowException(unsigned int exception_number,
		const char *except_string = NULL)
	{
		throw	SpecException<ExceptionType>(exception_number, except_string);
	}
	static void ThrowException(unsigned int exception_number,
		const std::string &except_string)
	{
		ThrowException(exception_number, except_string.c_str());
	}
	static void ThrowException(unsigned int exception_number,
		const std::ostringstream &except_string)
	{
		ThrowException(exception_number, except_string.str().c_str());
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef SpecException<'D'> SpecExceptionDynamic;
typedef SpecException<'R'> SpecExceptionReportable;
typedef SpecException<'S'> SpecExceptionStatic;
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__SpecException_hpp__HH

