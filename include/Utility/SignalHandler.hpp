// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the MLB signal support logic.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__SignalHandler_hpp__HH

#define HH__MLB__Utility__SignalHandler_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file SignalHandler.hpp

	\brief	The MLB signal handler support logic header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <signal.h>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
#ifdef __MSDOS__
typedef void (*SignalHandlerFunc)(int signal_number);
#elif _Windows
typedef void (*SignalHandlerFunc)(int signal_number);
#elif _MSC_VER
typedef void (*SignalHandlerFunc)(int signal_number);
#else
# ifdef __SVR4
typedef void (*SignalHandlerFunc)(int signal_number,
	siginfo_t *siginfo_ptr, void *signal_address);
# elif __GNUC__
typedef void (*SignalHandlerFunc)(int signal_number);
# else
typedef void (*SignalHandlerFunc)(int signal_number,
	int signal_code, struct sigcontext *signal_context, char *signal_address);
# endif // #ifndef __SVR4
#endif // ifdef __MSDOS__
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY bool SignalEventTest(int signal_id);
API_UTILITY void SignalEventClear(int signal_id);
API_UTILITY void SignalEventClearAll();
API_UTILITY void SignalHandlerInstall(unsigned int sig_count,
	const int *sig_list);
API_UTILITY void SignalHandlerInstall(int signal_id);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY SignalInstallData {
	SignalInstallData() :
		 signal_id_(0)
		,signal_func_(NULL) { }
	SignalInstallData(int signal_id, SignalHandlerFunc signal_func) :
		 signal_id_(signal_id)
		,signal_func_(signal_func) { }

	bool operator < (const SignalInstallData &other) const {
		return(signal_id_ < other.signal_id_);
	}
	bool operator == (const SignalInstallData &other) const {
		return(signal_id_ == other.signal_id_);
	}

	int               signal_id_;
	SignalHandlerFunc signal_func_;
};
//	////////////////////////////////////////////////////////////////////////////

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY SignalInstallDefaultData {
	SignalInstallDefaultData() :
		signal_func_list_() {
	}
	virtual ~SignalInstallDefaultData() {
		Uninstall();
	}

	void Install(int signal_id, SignalHandlerFunc signal_func) {
		Append(signal_id, signal(signal_id, signal_func));
	}
	void Append(int signal_id, SignalHandlerFunc signal_func) {
		signal_func_list_.push_back(SignalInstallData(signal_id, signal_func));
	}
	void Reset() {
		signal_func_list_.clear();
	}
	void Uninstall() {
		size_t signal_count = signal_func_list_.size();
		while (signal_count--)
			signal(signal_func_list_[signal_count].signal_id_,
				signal_func_list_[signal_count].signal_func_);
		Reset();
	}

	std::vector<SignalInstallData>  signal_func_list_;
};
//	////////////////////////////////////////////////////////////////////////////
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__SignalHandler_hpp__HH

