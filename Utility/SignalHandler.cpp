//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Signal handling.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/AtomicTypes.hpp>
#include <Utility/SignalHandler.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
struct SignalDef {
	SignalDef()
		:signal_id_(-1)
		,signal_name_(NULL)
		,signal_description_(NULL) { }
	SignalDef(int signal_id)
		:signal_id_(signal_id)
		,signal_name_()
		,signal_description_() { }
	SignalDef(int signal_id, const char *signal_name,
		const char *signal_description)
		:signal_id_(signal_id)
		,signal_name_(signal_name)
		,signal_description_(signal_description) { }

	bool operator < (const SignalDef &other) const {
		return(signal_id_ < other.signal_id_);
	}

	int         signal_id_;
	std::string signal_name_;
	std::string signal_description_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct SignalDef_Internal {
	SignalDef_Internal()
		:signal_id_(-1)
		,signal_name_(NULL)
		,signal_description_(NULL) { }

	void SetDatum(int signal_id, const char *signal_name,
		const char *signal_description) {
		signal_id_          = signal_id;
		signal_name_        = signal_name;
		signal_description_ = signal_description;
	}

	int         signal_id_;
	const char *signal_name_;
	const char *signal_description_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
static const unsigned int SignalDefCount_Internal = 256;
static SignalDef_Internal SignalDefList_Internal[SignalDefCount_Internal];
// ////////////////////////////////////////////////////////////////////////////
/*
#define SET_SIGNAL_DEF(signal_id, signal_description)	\
	SignalDefList_Internal[signal_id].SetDatum(signal_id, #signal_id, signal_description);
#ifdef _MSDOS_
	SET_SIGNAL_DEF(SIGINT,		"CTRL+C signal")
	SET_SIGNAL_DEF(SIGABRT,		"abnormal termination")
	SET_SIGNAL_DEF(SIGFPE,		"floating-point error")
	SET_SIGNAL_DEF(SIGILL,		"illegal instruction")
	SET_SIGNAL_DEF(SIGSEGV,		"illegal storage access")
	SET_SIGNAL_DEF(SIGTERM		"termination request")
#elif _Windows
	SET_SIGNAL_DEF(SIGABRT,		"abnormal termination")
	SET_SIGNAL_DEF(SIGFPE,		"floating-point error")
	SET_SIGNAL_DEF(SIGILL,		"illegal instruction")
	SET_SIGNAL_DEF(SIGINT,		"CTRL+C signal")
	SET_SIGNAL_DEF(SIGSEGV,		"illegal storage access")
	SET_SIGNAL_DEF(SIGTERM		"termination request")
#elif __SVR4
	SET_SIGNAL_DEF(SIGHUP,		"controlling terminal hangup")
	SET_SIGNAL_DEF(SIGINT,		"interactive interrupt")
	SET_SIGNAL_DEF(SIGQUIT,		"interactive process abnormal termination")
	SET_SIGNAL_DEF(SIGILL,		"illegal instruction exception")
	SET_SIGNAL_DEF(SIGTRAP,		"trace trap (not reset when caught)")
	SET_SIGNAL_DEF(SIGIOT,		"IOT instruction ('abort()' calls this SIGABRT)")
	SET_SIGNAL_DEF(SIGEMT,		"EMT instruction")
	SET_SIGNAL_DEF(SIGFPE,		"floating point exception")
	SET_SIGNAL_DEF(SIGKILL,		"kill (unstoppable termination)")
	SET_SIGNAL_DEF(SIGBUS,		"bus error")
	SET_SIGNAL_DEF(SIGSEGV,		"memory access / segmentation violation")
	SET_SIGNAL_DEF(SIGSYS,		"bad argument to system call")
	SET_SIGNAL_DEF(SIGPIPE,		"write on a pipe with no one to read it")
	SET_SIGNAL_DEF(SIGALRM,		"alarm clock expired")
	SET_SIGNAL_DEF(SIGTERM,		"software termination signal from kill")
	SET_SIGNAL_DEF(SIGUSR1,		"user defined signal number 1")
	SET_SIGNAL_DEF(SIGUSR2,		"user defined signal number 2")
	SET_SIGNAL_DEF(SIGCHLD,		"to parent on child stop or exit (System V SIGCLD)")
	SET_SIGNAL_DEF(SIGPWR,		"power fail/re-start")
	SET_SIGNAL_DEF(SIGWINCH,	"window changed")
	SET_SIGNAL_DEF(SIGURG,		"urgent condition on IO channel")
	SET_SIGNAL_DEF(SIGIO,		"input/output possible signal (System V SIGPOLL)")
	SET_SIGNAL_DEF(SIGSTOP,		"sendable stop signal not from tty")
	SET_SIGNAL_DEF(SIGTSTP,		"stop signal from tty")
	SET_SIGNAL_DEF(SIGCONT,		"continue a stopped process")
	SET_SIGNAL_DEF(SIGTTIN,		"to readers pgrp upon background tty read")
	SET_SIGNAL_DEF(SIGTTOU,		"like TTIN for output if (tp->t_local&LTOSTOP)")
	SET_SIGNAL_DEF(SIGVTALRM,	"virtual time alarm")
	SET_SIGNAL_DEF(SIGPROF,		"profiling time alarm")
	SET_SIGNAL_DEF(SIGXCPU,		"exceeded CPU time limit")
	SET_SIGNAL_DEF(SIGXFSZ,		"exceeded file size limit")
	SET_SIGNAL_DEF(SIGWAITING,	"process light-weight processes are blocked")
	SET_SIGNAL_DEF(SIGLWP,		"thread library special exit signal")
	SET_SIGNAL_DEF(SIGFREEZE,	"check point freeze")
	SET_SIGNAL_DEF(SIGTHAW,		"check point thaw")
	SET_SIGNAL_DEF(SIGCANCEL,	"cancellation signal reserved by the threads library")
	SET_SIGNAL_DEF(SIGLOST,		"resource lost")
#else
	SET_SIGNAL_DEF(SIGHUP,		"controlling terminal hangup")
	SET_SIGNAL_DEF(SIGINT,		"interactive interrupt")
	SET_SIGNAL_DEF(SIGQUIT,		"interactive process abnormal termination")
	SET_SIGNAL_DEF(SIGILL,		"illegal instruction exception")
	SET_SIGNAL_DEF(SIGTRAP,		"trace trap (not reset when caught)")
	SET_SIGNAL_DEF(SIGIOT,		"IOT instruction ('abort()' calls this SIGABRT)")
	SET_SIGNAL_DEF(SIGEMT,		"EMT instruction")
	SET_SIGNAL_DEF(SIGFPE,		"floating point exception")
	SET_SIGNAL_DEF(SIGKILL,		"kill (unstoppable termination)")
	SET_SIGNAL_DEF(SIGBUS,		"bus error")
	SET_SIGNAL_DEF(SIGSEGV,		"memory access / segmentation violation")
	SET_SIGNAL_DEF(SIGSYS,		"bad argument to system call")
	SET_SIGNAL_DEF(SIGPIPE,		"write on a pipe with no one to read it")
	SET_SIGNAL_DEF(SIGALRM,		"alarm clock expired")
	SET_SIGNAL_DEF(SIGTERM,		"software termination signal from kill")
	SET_SIGNAL_DEF(SIGURG,		"urgent condition on IO channel")
	SET_SIGNAL_DEF(SIGSTOP,		"sendable stop signal not from tty")
	SET_SIGNAL_DEF(SIGTSTP,		"stop signal from tty")
	SET_SIGNAL_DEF(SIGCONT,		"continue a stopped process")
	SET_SIGNAL_DEF(SIGCHLD,		"to parent on child stop or exit (System V SIGCLD)")
	SET_SIGNAL_DEF(SIGTTIN,		"to readers pgrp upon background tty read")
	SET_SIGNAL_DEF(SIGTTOU,		"like TTIN for output if (tp->t_local&LTOSTOP)")
	SET_SIGNAL_DEF(SIGIO,		"input/output possible signal (System V SIGPOLL)")
	SET_SIGNAL_DEF(SIGXCPU,		"exceeded CPU time limit")
	SET_SIGNAL_DEF(SIGXFSZ,		"exceeded file size limit")
	SET_SIGNAL_DEF(SIGVTALRM,	"virtual time alarm")
	SET_SIGNAL_DEF(SIGPROF,		"profiling time alarm")
	SET_SIGNAL_DEF(SIGWINCH,	"window changed")
	SET_SIGNAL_DEF(SIGLOST,		"resource lost (e.g., record-lock lost)")
	SET_SIGNAL_DEF(SIGUSR1,		"user defined signal number 1")
	SET_SIGNAL_DEF(SIGUSR2,		"user defined signal number 2")
#endif // _MSDOS_
};
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
static volatile AtomicFlag SignalEventList[SignalDefCount_Internal] = { 0 };
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
static bool IsSignalMapped(int signal_id)
{
	return((signal_id > 0) &&
		(static_cast<unsigned int>(signal_id) < SignalDefCount_Internal));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef __MSDOS__
static void SignalHandler(int signal_id)
#elif _Windows
static void SignalHandler(int signal_id)
#elif _MSC_VER
static void SignalHandler(int signal_id)
#else
# ifdef __SVR4
static void SignalHandler(int signal_id, siginfo_t *siginfo_ptr,
	void *signal_address)
# elif defined(__GNUC__)
static void SignalHandler(int signal_id)
# else
static void SignalHandler(int signal_id, int signal_code,
	struct sigcontext *signal_context, char *signal_address)
# endif /* # ifdef __SVR4 */
#endif /* ifdef __MSDOS__ */
{
	if (IsSignalMapped(signal_id))
		AtomicFlagSet(SignalEventList + signal_id);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
bool SignalEventTest(int signal_id)
{
	return((IsSignalMapped(signal_id)) ?
		AtomicFlagTest(SignalEventList + signal_id) : false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SignalEventClear(int signal_id)
{
	if (IsSignalMapped(signal_id))
		AtomicFlagClear(SignalEventList + signal_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SignalEventClearAll()
{
	unsigned int              count_1;
	const SignalDef_Internal *sig_ptr;

	for (count_1 = 0, sig_ptr = SignalDefList_Internal;
		count_1 < SignalDefCount_Internal; ++count_1, ++sig_ptr)
		AtomicFlagClear(SignalEventList + count_1);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SignalHandlerInstall(unsigned int sig_count, const int *sig_list)
{
	while (sig_count--) {
		if (IsSignalMapped(sig_list[sig_count]))
			::signal(sig_list[sig_count], SignalHandler);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SignalHandlerInstall(int signal_id)
{
	if (IsSignalMapped(signal_id))
		::signal(signal_id, SignalHandler);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <cstdlib>
#include <string>
#include <sstream>

#include <Utility/Sleep.hpp>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	std::cout << "Test harness for SignalHandler.cpp" << std::endl;
	std::cout << "---- ------- --- -----------------" << std::endl << std::endl;

	SignalHandlerInstall(SIGINT);

	std::cout << "Installed signal handler for SIGINT" << std::endl;
	std::cout << "   Send a SIGINT to this process to test." << std::endl;

	bool no_signal = true;
	while (no_signal) {
		if (SignalEventTest(SIGINT))
			break;
		SleepSecs(10);
	}

	std::cout << std::endl << std::endl <<
		"DONE: Now exiting program..." << std::endl << std::endl;
	SleepSecs(3000);

	return(EXIT_SUCCESS);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

