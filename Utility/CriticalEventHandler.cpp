//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Provides an interface for the handling of critical
								process events.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/CriticalEventHandler.hpp>
#include <Utility/Utility_Exception.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Storage for the critical event flag...
// ////////////////////////////////////////////////////////////////////////////
static volatile AtomicFlag CriticalEventFlagStorage = static_cast<AtomicFlag>(0);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventClear()
{
	AtomicFlagClear(&CriticalEventFlagStorage);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventSet()
{
	AtomicFlagSet(&CriticalEventFlagStorage);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CriticalEventTest()
{
	return(AtomicFlagTest(&CriticalEventFlagStorage));
}
// ////////////////////////////////////////////////////////////////////////////

#ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////
static BOOL WINAPI Win32_CriticalEventlHandler(DWORD control_event_type)
{
	switch (control_event_type) {
		// Control-C signal event.
		case CTRL_C_EVENT:
			CriticalEventSet();
			return(TRUE);
		// Console close event.
		case CTRL_CLOSE_EVENT:
			CriticalEventSet();
			return(TRUE);
		// Control-Break event.
		case CTRL_BREAK_EVENT:
			CriticalEventSet();
			return(TRUE);
		//	User logoff event.
		case CTRL_LOGOFF_EVENT:
			CriticalEventSet();
			return(TRUE);
		//	System shutdown event.
		case CTRL_SHUTDOWN_EVENT:
			CriticalEventSet();
			return(TRUE);
		//	Some other event, handled (one hopes) by another handler function...
		default:
			break;
	}

	return(FALSE);
}
// ////////////////////////////////////////////////////////////////////////////
#endif // #ifdef _Windows

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(disable:4555)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
// ////////////////////////////////////////////////////////////////////////////
#ifdef __MSDOS__
static void CriticalEventSignalHandler(int /* signal_id */)
#elif _Windows
static void CriticalEventSignalHandler(int /* signal_id */)
#elif _MSC_VER
static void CriticalEventSignalHandler(int /* signal_id */)
#else
# ifdef __SVR4
static void CriticalEventSignalHandler(int signal_id, siginfo_t *siginfo_ptr,
	void *signal_address)
# elif defined(__GNUC__)
static void CriticalEventSignalHandler(int /* signal_id */)
# else
static void CriticalEventSignalHandler(int signal_id, int signal_code,
	struct sigcontext *signal_context, char *signal_address)
# endif /* # ifdef __SVR4 */
#endif /* ifdef __MSDOS__ */
{
	AtomicFlagSet(&CriticalEventFlagStorage);
}
// ////////////////////////////////////////////////////////////////////////////
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(default:4555)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventInstallSignal(unsigned int sig_count, const int *sig_list)
{
	while (sig_count--)
		::signal(sig_list[sig_count], CriticalEventSignalHandler);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventInstallSignal(unsigned int sig_count, const int *sig_list,
	SignalInstallDefaultData &signal_data)
{
	while (sig_count--)
		signal_data.Install(sig_list[sig_count], CriticalEventSignalHandler);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventInstallSignal(int signal_id)
{
	::signal(signal_id, CriticalEventSignalHandler);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventInstallSignal(int signal_id,
	SignalInstallDefaultData &signal_data)
{
	signal_data.Install(signal_id, CriticalEventSignalHandler);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventInstallDefault()
{
	SignalInstallDefaultData signal_data;

	CriticalEventInstall(signal_data);

	signal_data.Reset();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventInstall(SignalInstallDefaultData &signal_data)
{
#ifdef _Windows
	CriticalEventInstallSignal(SIGINT, signal_data);
//	signal(SIGABRT, SIG_IGN, signal_data);
	CriticalEventInstallSignal(SIGTERM, signal_data);
	if (!::SetConsoleCtrlHandler(
		reinterpret_cast<PHANDLER_ROUTINE>(Win32_CriticalEventlHandler), TRUE))
		ThrowSystemError("Call to 'SetConsoleCtrlHandler()' failed");
#else
	CriticalEventInstallSignal(SIGHUP, signal_data);
	CriticalEventInstallSignal(SIGINT, signal_data);
	CriticalEventInstallSignal(SIGQUIT, signal_data);
//	signal(SIGABRT, SIG_IGN, signal_data);
	CriticalEventInstallSignal(SIGTERM, signal_data);
//	signal(SIGUSR1, SIG_IGN, signal_data);
//	signal(SIGUSR2, SIG_IGN, signal_data);
//	signal(SIGCHLD, SIG_IGN, signal_data);
//	signal(SIGPWR, SIG_IGN, signal_data);
//	signal(SIGWINCH, SIG_IGN, signal_data);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CriticalEventUninstall(SignalInstallDefaultData &signal_data)
{
	signal_data.Uninstall();

#ifdef _Windows
	if (!::SetConsoleCtrlHandler(
		reinterpret_cast<PHANDLER_ROUTINE>(Win32_CriticalEventlHandler), FALSE))
		ThrowSystemError("Call to 'SetConsoleCtrlHandler()' failed");
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <string>
#include <sstream>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	std::cout << "Test harness for CriticalEventHandler" << std::endl;
	std::cout << "---- ------- --- --------------------" << std::endl <<
		std::endl;

	{
		CriticalEventContext critical_event_context;
		std::cout << "Installed a critical event handler for this process" <<
			std::endl;
		std::cout << "   Send a SIGINT, or close the window, or whatever..." <<
			std::endl;
		bool no_signal = true;
		while (no_signal) {
			if (CriticalEventTest())
				break;
			Sleep(10);
		}
	}

	std::cout << std::endl << std::endl <<
		"DONE: Now exiting program..." << std::endl << std::endl;
	Sleep(3000);

	return(EXIT_SUCCESS);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

