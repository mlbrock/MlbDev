// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The VFAST basic specification exception class include
								file.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__SpecExceptionBase_hpp__HH

#define HH__MLB__VFast__SpecExceptionBase_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file SpecExceptionBase.hpp

	\brief	The VFAST basic specification exception class include file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFastException.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	The VFast specification exception base class.
*/
class MB_LIB_EXCEPTION_CLASS(API_VFAST) SpecExceptionBase :
	public VFastException {
public:
	SpecExceptionBase(int exception_type, unsigned int exception_number,
		const char *except_string = NULL);
	SpecExceptionBase(int exception_type, unsigned int exception_number,
		const std::string &except_string);
	SpecExceptionBase(int exception_type, unsigned int exception_number,
		const std::ostringstream &except_string);

	~SpecExceptionBase() MBCOMPAT_EXCEPT_NOTHROW;

	/**
		Returns the specification type of the exception:

		- D = Dynamic
		- R = Reportable
		- S = Static
	*/
	char         WhatExceptionType() const;

	/**
		Returns the specification number of the exception. For example,
		the dynamic specification exception type has an exception number \e 6
		which means that a mandatory field is not present in the stream and has
		an empty previous value.
	*/
	unsigned int WhatExceptionNumber() const;

	/**
		Returns an analog of the \c std::exception method \c what along with
		the specification type and number.
	*/
	std::string  WhatExceptionIdString() const;

	/**
		Rethrows the exception.

		\param except_string , if not \c NULL , points to the new string with
		which any current \c what string will be replaced. If \e except_string
		is \c NULL , a default string will be used to replace the \c what
		string instead.
	*/
	virtual void Rethrow(const char *except_string = NULL) const;

protected:
	int          exception_type_;
	unsigned int exception_number_;
	std::string  exception_id_string_;

private:
	static std::string GetFixedString(int exception_type,
		unsigned int exception_number, const char *except_string);
	static std::string GetFixedIdString(int exception_type,
		unsigned int exception_number);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__SpecExceptionBase_hpp__HH

