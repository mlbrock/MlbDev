// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB BoostX Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the Boost support library.

	Revision History	:	2005-08-23 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__BoostX_hpp__HH

#define HH__MLB__BoostX_hpp__HH	1

#define MLB__LIB__BoostX

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/TimeSupport.hpp>
#include <Utility/Utility_Exception.hpp>

//	Needed for DoubleScopedLockBase
#include <Utility/ValueToStringRadix.hpp>

#ifdef MLB__LIB__BoostX
# ifdef BOOSTX_DLL_EXPORTS
#  define API_BOOSTX		MB_LIB_EXPORT
# else
#  define API_BOOSTX		MB_LIB_IMPORT
# endif // #ifdef BOOSTX_DLL_EXPORTS
#else
#  define API_BOOSTX
#endif // #ifdef MLB__LIB__BoostX

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_BOOSTX

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4217 4275 4668 4625 4626)
# if (_MSC_VER >= 1500)
#  pragma warning(disable:4061 4365)
# endif // #if (_MSC_VER >= 1500)
#endif // #ifdef _MSC_VER

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION < 105000
namespace boost {
	enum xtime_compat {
		TIME_UTC_ = TIME_UTC
	};
}
#endif // #if BOOST_VERSION < 105000

#ifdef _MSC_VER
# pragma warning(pop)
#endif // #ifdef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace BoostX {

// ////////////////////////////////////////////////////////////////////////////
struct NullDeleter {
	void operator()(void const *) const {}
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_BOOSTX MLB::Utility::TimeSpec
	&XTimeToTimeSpec(const boost::xtime &in_time,
	MLB::Utility::TimeSpec &out_time)
{
	out_time = MLB::Utility::TimeSpec(static_cast<time_t>(in_time.sec),
		in_time.nsec);

	return(out_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_BOOSTX MLB::Utility::TimeSpec
	XTimeToTimeSpec(const boost::xtime &in_time)
{
	return(MLB::Utility::TimeSpec(static_cast<time_t>(in_time.sec),
		in_time.nsec));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_BOOSTX std::string &XTimeToString(const boost::xtime &in_time,
	std::string &out_string)
{
	return(MLB::Utility::TimeSpec(static_cast<time_t>(in_time.sec),
		in_time.nsec).ToString(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_BOOSTX std::string &XTimeToString(const boost::xtime &in_time)
{
	std::string out_string;

	return(XTimeToString(in_time, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_BOOSTX) BoostException :
	public MLB::Utility::ExceptionGeneral {
public:
	BoostException() throw() :
		 ExceptionGeneral("Unspecified Boost library exception.") { }
	BoostException(const char *except_string) throw() :
		ExceptionGeneral(except_string) { }
	BoostException(const std::string &except_string) throw() :
		ExceptionGeneral(except_string) { }
	BoostException(const std::ostringstream &except_string) throw() :
		ExceptionGeneral(except_string) { }
	~BoostException() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		BoostException tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	It would have been possible to implement this as a class template with
//	specializations based upon the Boost lock type. This would have resulted in
// a substantial reduction in the number of constructors required. I didn't
//	do so because I wasn't sure exactly how it could be portably done under
//	both Windows (as a DLL) and in the new GCC 4.x shared library model for
//	exceptions.
//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_BOOSTX) BoostLockAcquisitionFailure :
	public BoostException {
public:
	//	Constructs try lock failures...
	explicit BoostLockAcquisitionFailure(const char *except_string) :
		 BoostException(GetFixedString(except_string))
		,is_time_out_failure_(false)
		,expiration_time_()
	{
	}
	//	Constructs try lock failures...
	explicit BoostLockAcquisitionFailure(const std::string &except_string) :
		 BoostException(GetFixedString(except_string.c_str()))
		,is_time_out_failure_(false)
		,expiration_time_()
	{
	}
	//	Constructs timed lock failures...
	BoostLockAcquisitionFailure(const boost::xtime &expiration_time,
		const char *except_string) :
		 BoostException(GetFixedString(expiration_time, except_string))
		,is_time_out_failure_(true)
		,expiration_time_(expiration_time)
	{
	}
	//	Constructs timed lock failures...
	BoostLockAcquisitionFailure(const boost::xtime &expiration_time,
		const std::string &except_string) :
		 BoostException(GetFixedString(expiration_time, except_string.c_str()))
		,is_time_out_failure_(true)
		,expiration_time_(expiration_time)
	{
	}
	~BoostLockAcquisitionFailure() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		BoostLockAcquisitionFailure tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}

	bool         IsTimedLockFailure() const {
		return(is_time_out_failure_);
	}
	boost::xtime GetExpirationTime() const {
		return(expiration_time_);
	}

private:
	bool         is_time_out_failure_;
	boost::xtime expiration_time_;

	static std::string GetFixedString(const char *except_string = NULL) {
		return(((except_string == NULL) || (!(*except_string))) ?
			 "Try-lock attempt failed." :
			(std::string("Try-lock attempt failed: ") + except_string));
	}
	static std::string GetFixedString(const boost::xtime &expiration_time,
		const char *except_string = NULL) {
		if ((except_string == NULL) || (!(*except_string)))
			return("Timed-lock attempt failed for expiration date/time " +
				XTimeToString(expiration_time) + ".");
		return("Timed-lock attempt failed for expiration date/time " +
			XTimeToString(expiration_time) + ": " + std::string(except_string));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ScopedLockType>
	void ThrowIfBoostLockAcquisitionFailure(const ScopedLockType &s_lock,
	const char *error_message = NULL)
{
	if (!s_lock.locked())
		throw BoostLockAcquisitionFailure(error_message);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ScopedLockType>
	void ThrowIfBoostLockAcquisitionFailure(const ScopedLockType &s_lock,
	const std::string &error_message)
{
	ThrowIfBoostLockAcquisitionFailure(s_lock, error_message.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ScopedLockType>
	void ThrowIfBoostLockAcquisitionFailure(const ScopedLockType &s_lock,
	const std::ostringstream &error_message)
{
	ThrowIfBoostLockAcquisitionFailure(s_lock, error_message.str().c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ScopedLockType>
	void ThrowIfBoostLockAcquisitionFailure(const ScopedLockType &s_lock,
	const boost::xtime &expiration_time, const char *error_message = NULL)
{
	if (!s_lock.locked())
		throw BoostLockAcquisitionFailure(expiration_time, error_message);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ScopedLockType>
	void ThrowIfBoostLockAcquisitionFailure(const ScopedLockType &s_lock,
	const boost::xtime &expiration_time, const std::string &error_message)
{
	ThrowIfBoostLockAcquisitionFailure(s_lock, expiration_time,
		error_message.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ScopedLockType>
	void ThrowIfBoostLockAcquisitionFailure(const ScopedLockType &s_lock,
	const boost::xtime &expiration_time, const std::ostringstream &error_message)
{
	ThrowIfBoostLockAcquisitionFailure(s_lock, expiration_time,
		error_message.str().c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
	//	Used to wait forever to acquire a lock...
const boost::xtime XTimeForever =
	{	std::numeric_limits<boost::xtime::xtime_sec_t>::max(),
		std::numeric_limits<boost::xtime::xtime_nsec_t>::max()
	};

	//	Used to try to acquire a lock and give up immediately if unsuccessful...
const boost::xtime XTimeOnce    =
	{ 0, 0 };
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline boost::xtime NanosecondsToXTimeCurrent(unsigned int in_nanoseconds)
{
	if (!in_nanoseconds)
		return(XTimeOnce);
	else if (in_nanoseconds == std::numeric_limits<unsigned int>::max())
		return(XTimeForever);

	boost::xtime out_datum;

	if (boost::xtime_get(&out_datum, boost::TIME_UTC_) != boost::TIME_UTC_)
		throw BoostException("Invocation of 'boost::xtime_get()' failed.");

	unsigned long long total_nsecs =
		(static_cast<unsigned long long>(out_datum.sec) *
		static_cast<unsigned long long>(1000000000)) +
		static_cast<unsigned long long>(out_datum.nsec) +
		static_cast<unsigned long long>(in_nanoseconds);

	out_datum.sec  = static_cast<boost::xtime::xtime_sec_t>(
		static_cast<unsigned long long>(total_nsecs) /
		static_cast<unsigned long long>(1000000000));
	out_datum.nsec = static_cast<boost::xtime::xtime_nsec_t>(
		static_cast<unsigned long long>(total_nsecs) %
		static_cast<unsigned long long>(1000000000));

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline boost::xtime MicrosecondsToXTimeCurrent(unsigned int in_microseconds)
{
	return(NanosecondsToXTimeCurrent(in_microseconds * 1000));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline boost::xtime MillisecondsToXTimeCurrent(unsigned int in_milliseconds)
{
	return(NanosecondsToXTimeCurrent(in_milliseconds * 1000000));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline boost::xtime SecondsToXTimeCurrent(unsigned int in_seconds)
{
	return(NanosecondsToXTimeCurrent(in_seconds * 1000000000));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename SomeLock>
	class NullScopedLockBase {
public:
	explicit NullScopedLockBase(SomeLock &lock_ref, bool = true) :
		 lock_ref_(lock_ref)
	{
	}
	
	void lock() {
	}
	void unlock() {
	}
	bool locked() {
		return(true);
	}

	SomeLock &lock_ref_;
private:
	MLB_Utility_NonCopyable_Macro(NullScopedLockBase);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename SomeLock>
	class NullScopedLockType : public NullScopedLockBase<SomeLock> {
public:
	explicit NullScopedLockType(SomeLock &lock_ref,
		bool initially_locked = true) :
		NullScopedLockBase<SomeLock>(lock_ref, initially_locked) { }
private:
	MLB_Utility_NonCopyable_Macro(NullScopedLockType);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename SomeLock>
	class NullScopedTryLockType : public NullScopedLockBase<SomeLock> {
public:
	explicit NullScopedTryLockType(SomeLock &lock_ref,
		bool initially_locked = true) :
		NullScopedLockBase<SomeLock>(lock_ref, initially_locked) { }
	bool try_lock() {
		return(true);
	}
private:
	MLB_Utility_NonCopyable_Macro(NullScopedTryLockType);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename SomeLock>
	class NullScopedTimedLockType : public NullScopedLockBase<SomeLock> {
public:
	explicit NullScopedTimedLockType(SomeLock &lock_ref,
		bool initially_locked = true) :
		NullScopedLockBase<SomeLock>(lock_ref, initially_locked) { }
	bool try_lock() {
		return(true);
	}
	bool timed_lock(const boost::xtime &) {
		return(true);
	}
private:
	MLB_Utility_NonCopyable_Macro(NullScopedTimedLockType);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_BOOSTX NullLockType {
public:
	NullLockType() :
		is_locked_flag_(false) { }
	~NullLockType() { }

	typedef int cv_state;

	typedef NullScopedLockType<NullLockType>      scoped_lock;
	typedef NullScopedTryLockType<NullLockType>   scoped_try_lock;
	typedef NullScopedTimedLockType<NullLockType> scoped_timed_lock;

	bool is_locked_flag_;
private:
	MLB_Utility_NonCopyable_Macro(NullLockType);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Empty base used just to catch the error condition of the same lock...
class API_BOOSTX DoubleScopedLockBase {
protected:
	DoubleScopedLockBase(const void *ptr_1, const void *ptr_2) {
		if (ptr_1 == ptr_2)
			MLB::Utility::ThrowInvalidArgument("MLB::BoostX::"
				"DoubleScopedLockBase: The first pointer is equal to the second "
				"pointer (address = " + MLB::Utility::ValueToStringHex(ptr_1) +
				").");
	}
private:
	MLB_Utility_NonCopyable_Macro(DoubleScopedLockBase);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename LockType>
	class DoubleScopedLock : public DoubleScopedLockBase {
	typedef typename LockType::scoped_lock ScopedLockType;
public:
	DoubleScopedLock(LockType &datum_1, LockType &datum_2) :
		 DoubleScopedLockBase(&datum_1, &datum_2)
		,lock_1(*std::min(&datum_1, &datum_2))
		,lock_2(*std::max(&datum_1, &datum_2)) { }
	~DoubleScopedLock() {
		unlock();
	}

	void unlock() {
		lock_2.unlock();
		lock_1.unlock();
	}

	bool locked() const {
		return(lock_1.locked() && lock_2.locked());
	}

	ScopedLockType lock_1;		
	ScopedLockType lock_2;

private:
	MLB_Utility_NonCopyable_Macro(DoubleScopedLock);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename LockType>
	class DoubleScopedTryLock : public DoubleScopedLockBase {
	typedef typename LockType::scoped_try_lock ScopedLockType;
public:
	DoubleScopedTryLock(LockType &datum_1, LockType &datum_2) :
		 DoubleScopedLockBase(&datum_1, &datum_2)
		,lock_1(*std::min(&datum_1, &datum_2))
		,lock_2(*std::max(&datum_1, &datum_2))
	{
		if (!locked()) {
			if (lock_2.locked())
				lock_2.unlock();
			if (lock_1.locked())
				lock_1.unlock();
		}
	}
	~DoubleScopedTryLock() {
		unlock();
	}

	void unlock() {
		lock_2.unlock();
		lock_1.unlock();
	}

	bool locked() const {
		return(lock_1.locked() && lock_2.locked());
	}

	ScopedLockType lock_1;		
	ScopedLockType lock_2;

private:
	MLB_Utility_NonCopyable_Macro(DoubleScopedTryLock);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename LockType>
	class DoubleScopedTimedLock : public DoubleScopedLockBase {
	typedef typename LockType::scoped_timed_lock ScopedLockType;
public:
	DoubleScopedTimedLock(LockType &datum_1, LockType &datum_2,
		const boost::xtime &wait_time) :
		 DoubleScopedLockBase(&datum_1, &datum_2)
		,lock_1(*std::min(&datum_1, &datum_2), wait_time)
		,lock_2(*std::max(&datum_1, &datum_2), wait_time)
	{
		if (!locked()) {
			if (lock_2.locked())
				lock_2.unlock();
			if (lock_1.locked())
				lock_1.unlock();
		}
	}
	~DoubleScopedTimedLock() {
		unlock();
	}

	void unlock() {
		lock_2.unlock();
		lock_1.unlock();
	}

	bool locked() const {
		return(lock_1.locked() && lock_2.locked());
	}

	ScopedLockType lock_1;		
	ScopedLockType lock_2;

private:
	MLB_Utility_NonCopyable_Macro(DoubleScopedTimedLock);
};
//	////////////////////////////////////////////////////////////////////////////
} // namespace BoostX

} // namespace MLB

#endif // #ifndef HH__MLB__BoostX_hpp__HH

