// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	PTypes Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for PTypes.

	Revision History	:	2004-12-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB___PTypesX_hpp__HH

#define HH__MLB___PTypesX_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
# include <boost/shared_ptr.hpp>
# include <boost/static_assert.hpp>
# include <boost/type_traits.hpp>
# pragma warning(default:4217 4668)
#else
# include <boost/shared_ptr.hpp>
# include <boost/static_assert.hpp>
# include <boost/type_traits.hpp>
#endif // #ifdef _Windows

#ifdef _Windows
# pragma warning(disable:4625 4626 4668)
# include <pasync.h>
//# include <pinet.h>
# include <pstreams.h>
# include <ptime.h>
# pragma warning(default:4625 4626 4668)
#else
# include <pasync.h>
//# include <pinet.h>
# include <pstreams.h>
# include <ptime.h>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace pt {

//	Must be in the pt namspace only. Well, std is possible, but even worse...
//	////////////////////////////////////////////////////////////////////////////
inline std::ostream & operator << (std::ostream &o_str, const pt::string &datum)
{
	o_str << static_cast<const char *>(datum);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace pt

namespace MLB {

namespace PTypesX {

//	////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
	//	This limitation is a result of the PTypes semaphore initialization,
	//	where 65535 is hard-coded as the maximum semaphore value.
const int MaximumSize_MsgQueue = 65535;
const int MaximumSize_JobQueue = 65535;
#else
const int MaximumSize_MsgQueue = std::numeric_limits<int>::max();
const int MaximumSize_JobQueue = std::numeric_limits<int>::max();
#endif // #ifdef WIN32
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Provides a way to kill pt::threads which won't die though the normal means.
//	This is more important under Windows, as PTypes doesn't provide a public
//	mechanism to get to the thread handle needed to invoke TerminateThread().
//
//	Nota Bene: Not tested, but something like this should work.
//	////////////////////////////////////////////////////////////////////////////
class ThreadKiller : public pt::thread {
public:
	ThreadKiller(pt::thread *pt_thread_ptr, unsigned int try_milliseconds = 0) :
		 pt::thread(false) {
		if (!pt_thread_ptr->get_finished()) {
			pt_thread_ptr->signal();
#ifdef WIN32
			Sleep(0);
			char thread_copy[sizeof(*this)];
			memcpy(thread_copy, this, sizeof(*this));
			memcpy(this, pt_thread_ptr, sizeof(*this));
			while (try_milliseconds && (!pt_thread_ptr->get_finished())) {
				Sleep(std::max(try_milliseconds, 10U));
				try_milliseconds -= std::max(try_milliseconds, 10U);
			}
			//	memcpy() gives us this...
			TerminateThread(this->handle, static_cast<DWORD>(-1));
			Sleep(10);
			memcpy(this, thread_copy, sizeof(*this));
#else
			if (sched_yield() == -1)
				usleep(1);
			while (try_milliseconds && (!pt_thread_ptr->get_finished())) {
				usleep((std::max(try_milliseconds, 10U)) * 1000);
				try_milliseconds -= std::max(try_milliseconds, 10U)
			}
			pthread_cancel(pt_thread_ptr->id);
			sched_yield();
			usleep(10000);
			if (!pt_thread_ptr->get_finished()) {
				pthread_kill(pt_thread_ptr->id, SIGKILL);
				if (sched_yield() == -1)
					usleep(1);
			}
#endif // #ifdef WIN32
		}
	}
private:
	MLB_Utility_NonCopyable_Macro(ThreadKiller);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class PTypesException : public MLB::Utility::ExceptionGeneral {
public:
	PTypesException() throw() :
		 ExceptionGeneral("Unspecified PTypes exception.") { }
	explicit PTypesException(const char *except_string) throw() :
		 ExceptionGeneral(except_string) { }
	explicit PTypesException(const std::string &except_string) throw() :
		 ExceptionGeneral(except_string.c_str()) { }
	explicit PTypesException(const std::ostringstream &except_string) throw() :
		 ExceptionGeneral(except_string.str().c_str()) { }
	explicit PTypesException(pt::exception *except) :
		 ExceptionGeneral(static_cast<const char *>(except->get_message())) { }
	~PTypesException() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		PTypesException tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void ThrowPTypesException(pt::exception *except)
{
	PTypesException ptypes_except(except);

	delete except;

	throw ptypes_except;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void ThrowPTypesException(const char *error_text)
{
	if ((error_text != NULL) && *error_text)
		throw PTypesException(error_text);

	throw PTypesException("Unspecified PTypes error.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void ThrowPTypesException(const std::string &error_text)
{
	ThrowPTypesException(error_text.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void ThrowPTypesException(const std::ostringstream &error_text)
{
	ThrowPTypesException(error_text.str().c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Type-safe pt::message cast for use in ''pt::msgqueue::msghandler()'...
//	////////////////////////////////////////////////////////////////////////////
template <typename MessageType>
	boost::shared_ptr<MessageType> MessageCast(pt::message *msg_ptr)
{
	//	Avoid a dynamic_cast<>...
	BOOST_STATIC_ASSERT((boost::is_same<MessageType, pt::message>::value) ||
		(boost::is_convertible<MessageType *, pt::message *>::value));

	//	Types are related, so we don't even need reinterpret_cast<>...
	return(boost::shared_ptr<MessageType>(
		static_cast<MessageType *>(msg_ptr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Type-safe wrapper for use in calls to 'pt::jobqueue::getmessage()'...
//	////////////////////////////////////////////////////////////////////////////
template <typename MessageType>
	boost::shared_ptr<MessageType> DequeMessage(pt::jobqueue *queue,
		int time_out = -1)
{
	try {
		return(boost::shared_ptr<MessageType>(
			MessageCast<MessageType>(queue->getmessage(time_out))));
	}
	catch (pt::exception *except) {
		std::ostringstream error_text;
		error_text << "Attempt to retrieve message from the message " <<
			"queue with 'getmessage(" << time_out << ")' failed: " <<
			except->get_message() << ".";
		delete except;
		throw MLB::PTypesX::PTypesException(error_text);
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace PTypesX

} // namespace MLB

#endif // #ifndef HH__MLB___PTypesX_hpp__HH

