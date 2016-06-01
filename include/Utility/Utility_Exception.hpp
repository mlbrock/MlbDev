// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for utility library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__Utility_Exception_hpp__HH

#define HH__MLB__Utility__Utility_Exception_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file Utility_Exception.hpp

	\brief	The MLB Utility exception header file.

	Contains classes and definitions for exception support.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <exception>
#include <stdexcept>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

/*
	CODE NOTE: Study for a future implementation.
// ////////////////////////////////////////////////////////////////////////////
template <typename BaseExceptionClass>
	class ExceptionBase : public BaseExceptionClass {
public:
	ExceptionBase(const char *except_string = NULL)
		:BaseExceptionClass()
		,what_string_((except_string == NULL) ? "" : except_string)
	{
	}

	~ExceptionBase() MBCOMPAT_EXCEPT_NOTHROW
	{
	}

	virtual const char *what() const
	{
		return(what_string_.c_str());
	}

	void SetWhat(const char *except_string = NULL)
	{
		std::string((except_string == NULL) ? "" : except_string).
			swap(what_string_);
	}
	void SetWhat(const std::string &except_string)
	{
		SetWhat(except_string.c_str());
	}
	void SetWhat(const std::ostringstream &except_string)
	{
		SetWhat(except_string.str().c_str());
	}

	template <typename ExceptionType>
		void Rethrow(const char *except_string = NULL) const
	{
		ExceptionType tmp_except(*this);

		tmp_except.SetWhat(except_string);

		throw tmp_except;
	}
	template <typename ExceptionType>
		void Rethrow(const std::string &except_string) const
	{
		Rethrow<ExceptionType>(except_string.c_str());
	}
	template <typename ExceptionType>
		void Rethrow(const std::ostringstream &except_string) const
	{
		Rethrow<ExceptionType>(except_string.str().c_str());
	}

protected:
	std::string what_string_;
};
// ////////////////////////////////////////////////////////////////////////////
*/

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY void ThrowErrorCode(long error_code, const char *error_text = NULL);
API_UTILITY void ThrowErrorCode(long error_code, const std::string &error_text);
API_UTILITY void ThrowErrorCode(long error_code,
	const std::ostringstream &error_text);
API_UTILITY void ThrowErrorCode(unsigned long error_code,
	const char *error_text = NULL);
API_UTILITY void ThrowErrorCode(unsigned long error_code,
	const std::string &error_text);
API_UTILITY void ThrowErrorCode(unsigned long error_code,
	const std::ostringstream &error_text);

API_UTILITY void ThrowStdException(const char *error_text = NULL);
API_UTILITY void ThrowStdException(const std::string &error_text);
API_UTILITY void ThrowStdException(const std::ostringstream &error_text);

API_UTILITY void ThrowDomainError(const char *error_text = NULL);
API_UTILITY void ThrowDomainError(const std::string &error_text);
API_UTILITY void ThrowDomainError(const std::ostringstream &error_text);

API_UTILITY void ThrowInvalidArgument(const char *error_text = NULL);
API_UTILITY void ThrowInvalidArgument(const std::string &error_text);
API_UTILITY void ThrowInvalidArgument(const std::ostringstream &error_text);

API_UTILITY void ThrowLengthError(const char *error_text = NULL);
API_UTILITY void ThrowLengthError(const std::string &error_text);
API_UTILITY void ThrowLengthError(const std::ostringstream &error_text);

API_UTILITY void ThrowLogicError(const char *error_text = NULL);
API_UTILITY void ThrowLogicError(const std::string &error_text);
API_UTILITY void ThrowLogicError(const std::ostringstream &error_text);

API_UTILITY void ThrowOutOfRange(const char *error_text = NULL);
API_UTILITY void ThrowOutOfRange(const std::string &error_text);
API_UTILITY void ThrowOutOfRange(const std::ostringstream &error_text);

API_UTILITY void ThrowOverflowError(const char *error_text = NULL);
API_UTILITY void ThrowOverflowError(const std::string &error_text);
API_UTILITY void ThrowOverflowError(const std::ostringstream &error_text);

API_UTILITY void ThrowRangeError(const char *error_text = NULL);
API_UTILITY void ThrowRangeError(const std::string &error_text);
API_UTILITY void ThrowRangeError(const std::ostringstream &error_text);

API_UTILITY void ThrowRuntimeError(const char *error_text = NULL);
API_UTILITY void ThrowRuntimeError(const std::string &error_text);
API_UTILITY void ThrowRuntimeError(const std::ostringstream &error_text);

API_UTILITY void ThrowUnderflowError(const char *error_text = NULL);
API_UTILITY void ThrowUnderflowError(const std::string &error_text);
API_UTILITY void ThrowUnderflowError(const std::ostringstream &error_text);

API_UTILITY void ThrowBadAlloc(const char *error_text = NULL);
API_UTILITY void ThrowBadAlloc(const std::string &error_text);
API_UTILITY void ThrowBadAlloc(const std::ostringstream &error_text);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_UTILITY) ExceptionGeneral :
	public std::exception {
public:
	ExceptionGeneral(const char *except_string = NULL);
	ExceptionGeneral(const std::string &except_string);
	ExceptionGeneral(const std::ostringstream &except_string);

	~ExceptionGeneral() MBCOMPAT_EXCEPT_NOTHROW;

	const char *what() const throw();

	void SetWhat(const char *except_string = NULL);
	void SetWhat(const std::string &except_string);
	void SetWhat(const std::ostringstream &except_string);

	virtual void Rethrow(const char *except_string = NULL) const;
	//	Needn't be virtual.
	void Rethrow(const std::string &except_string) const;
	//	Needn't be virtual.
	void Rethrow(const std::ostringstream &except_string) const;


protected:
	std::string except_string_;

private:
	static const char *GetFixedString(const char *except_string = NULL);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY void ThrowExceptionGeneral(const char *error_text = NULL);
API_UTILITY void ThrowExceptionGeneral(const std::string &error_text);
API_UTILITY void ThrowExceptionGeneral(const std::ostringstream &error_text);
//	Shorter-named wrappers for the functions above...
API_UTILITY void ThrowException(const char *error_text = NULL);
API_UTILITY void ThrowException(const std::string &error_text);
API_UTILITY void ThrowException(const std::ostringstream &error_text);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ExceptType>
	void RethrowExceptionGeneral(const ExceptType &except,
	const char *except_string = NULL)
{
	except.Rethrow(except_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ExceptType> void Rethrow(const ExceptType &,
	const char *except_string)
{
	// Cheap way of disambiguating std::exception without a dynamic_cast<>.
	try {
		throw;
	}
	catch (const ExceptionGeneral &tmp_except) {
		RethrowExceptionGeneral(tmp_except, except_string);
	}
	catch (const std::domain_error &) {
		throw std::domain_error(except_string);
	}
	catch (const std::invalid_argument &) {
		throw std::invalid_argument(except_string);
	}
	catch (const std::length_error &) {
		throw std::length_error(except_string);
	}
	catch (const std::out_of_range &) {
		throw std::out_of_range(except_string);
	}
	catch (const std::logic_error &) {
		throw std::logic_error(except_string);
	}
	catch (const std::overflow_error &) {
		throw std::overflow_error(except_string);
	}
	catch (const std::underflow_error &) {
		throw std::underflow_error(except_string);
	}
	catch (const std::range_error &) {
		throw std::range_error(except_string);
	}
	catch (const std::runtime_error &) {
		throw std::runtime_error(except_string);
	}
	catch (const std::bad_alloc &) {
#if defined(_MSC_VER) && !defined(__MINGW32__) && (_MSC_VER < 1700)
		// Because P. J. and Pete provided a ctor taking a const char *...
		throw std::bad_alloc(except_string);
#else
		throw;
#endif // #if defined(_MSC_VER) && !defined(__MINGW32__) && (_MSC_VER < 1700)
	}
	catch (const std::bad_exception &) {
#if defined(_MSC_VER) && !defined(__MINGW32__) && (_MSC_VER < 1900)
		// Because P. J. and Pete provided a ctor taking a const char *...
		throw std::bad_exception(except_string);
#else
		throw;
#endif // #if defined(_MSC_VER) && !defined(__MINGW32__) && (_MSC_VER < 1900)
	}
	catch (const std::exception &) {
#if defined(_MSC_VER) && !defined(__MINGW32__)
		// Because P. J. and Pete provided a ctor taking a const char *...
		throw std::exception(except_string);
#else
		throw;
#endif // #if defined(_MSC_VER) && !defined(__MINGW32__)
	}
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
# pragma warning(push)
# pragma warning(disable: 4571)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1500)
	catch (...) {
		throw;
	}
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
# pragma warning(pop)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1500)
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ExceptType> void Rethrow(const ExceptType &except,
	const std::string &except_string)
{
	Rethrow(except, except_string.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ExceptType> void Rethrow(const ExceptType &except,
	const std::ostringstream &except_string)
{
	Rethrow(except, except_string.str().c_str());
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_UTILITY) ExceptionErrno :
	public ExceptionGeneral {
public:
	//	Default constructor.
	ExceptionErrno();
	//	String-only constructors...
	ExceptionErrno(const char *except_string);
	ExceptionErrno(const std::string &except_string);
	ExceptionErrno(const std::ostringstream &except_string);

	//	int - based constructors...
	ExceptionErrno(int error_code);
	ExceptionErrno(int error_code, const char *except_string);
	ExceptionErrno(int error_code, const std::string &except_string);
	ExceptionErrno(int error_code, const std::ostringstream &except_string);

	~ExceptionErrno() MBCOMPAT_EXCEPT_NOTHROW;

	int whatcode() const;

	virtual void Rethrow(const char *except_string = NULL) const;

protected:
	int error_code_;

	static std::string GetStatusString(int error_code,
		const char *other_text = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_UTILITY) ExceptionSystemError :
	public ExceptionGeneral {
public:
	//	Default constructor.
	ExceptionSystemError();
	//	String-only constructors...
	ExceptionSystemError(const char *except_string);
	ExceptionSystemError(const std::string &except_string);
	ExceptionSystemError(const std::ostringstream &except_string);

	//	SystemErrorCode - based constructors...
	ExceptionSystemError(SystemErrorCode error_code);
	ExceptionSystemError(SystemErrorCode error_code,
		const char *except_string);
	ExceptionSystemError(SystemErrorCode error_code,
		const std::string &except_string);
	ExceptionSystemError(SystemErrorCode error_code,
		const std::ostringstream &except_string);
	ExceptionSystemError(bool, SystemErrorCode error_code,
		const std::string &except_string);

	~ExceptionSystemError() MBCOMPAT_EXCEPT_NOTHROW;

	SystemErrorCode whatcode() const;

	virtual void Rethrow(const char *except_string = NULL) const;

protected:
	SystemErrorCode error_code_;

	static std::string GetStatusString(SystemErrorCode error_code,
		const char *other_text = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY void ThrowErrno(int error_code, const char *error_text = NULL);
API_UTILITY void ThrowErrno(int error_code, const std::string &error_text);
API_UTILITY void ThrowErrno(int error_code,
	const std::ostringstream &error_text);
API_UTILITY void ThrowErrno(const char *error_text = NULL);
API_UTILITY void ThrowErrno(const std::string &error_text);
API_UTILITY void ThrowErrno(const std::ostringstream &error_text);

API_UTILITY void ThrowSystemError(SystemErrorCode error_code,
	const char *error_text = NULL);
API_UTILITY void ThrowSystemError(SystemErrorCode error_code,
	const std::string &error_text);
API_UTILITY void ThrowSystemError(SystemErrorCode error_code,
	const std::ostringstream &error_text);
API_UTILITY void ThrowSystemError(const char *error_text = NULL);
API_UTILITY void ThrowSystemError(const std::string &error_text);
API_UTILITY void ThrowSystemError(const std::ostringstream &error_text);
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_UTILITY) ExceptionCriticalEvent :
	public ExceptionGeneral {
public:
	//	Default constructor.
	ExceptionCriticalEvent();
	//	String-only constructors...
	ExceptionCriticalEvent(const char *except_string);
	ExceptionCriticalEvent(const std::string &except_string);
	ExceptionCriticalEvent(const std::ostringstream &except_string);

	virtual void Rethrow(const char *except_string = NULL) const;

private:
	static const char *GetFixedString(const char *except_string = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_UTILITY) ExceptionMMapVMFailure :
	public ExceptionGeneral {
public:
	//	Default constructor.
	ExceptionMMapVMFailure();
	//	String-only constructors...
	ExceptionMMapVMFailure(const char *except_string);
	ExceptionMMapVMFailure(const std::string &except_string);
	ExceptionMMapVMFailure(const std::ostringstream &except_string);

	virtual void Rethrow(const char *except_string = NULL) const;

private:
	static const char *GetFixedString(const char *except_string = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility_Exception_hpp__HH

