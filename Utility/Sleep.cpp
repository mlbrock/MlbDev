// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the portable sleep functions.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Sleep.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
void SleepSecs(unsigned long seconds)
{
#ifdef _Windows
	::Sleep(static_cast<unsigned long>(seconds) * 1000L);
#else
	SleepMicroSecs(static_cast<unsigned long>(seconds) * 1000000);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SleepMilliSecs(unsigned long milliseconds)
{
#ifdef _Windows
	::Sleep(static_cast<unsigned long>(milliseconds));
#else
	SleepMicroSecs(static_cast<unsigned int>(milliseconds) * 1000);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SleepMicroSecs(unsigned long microseconds)
{
#ifdef __MSDOS__
# if __BORLANDC__ || __BCPLUSPLUS
#  include <dos.h>
	::delay(((unsigned int) (microseconds / 1000L)));
# else
	::sleep(((unsigned int) (microseconds / 1000000L)));
# endif /* # if __BORLANDC__  || __BCPLUSPLUS */
#elif _Windows
	::Sleep(microseconds / 1000L);
#elif __SVR4
/*
	CODE NOTE:	I really should eventually implement code equivalent to BSD
					'usleep()' here.
*/
	struct timeval tmp_timeval;

	tmp_timeval.tv_sec  = ((long) (microseconds / 1000000L));
	tmp_timeval.tv_usec = ((long) (microseconds % 1000000L));

	::select(0, NULL, NULL, NULL, &tmp_timeval);
#else
	::usleep(microseconds);
#endif // #ifdef __MSDOS__
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SleepNanoSecs(unsigned long nanoseconds)
{
	SleepNanoSecs(static_cast<unsigned long long>(nanoseconds));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SleepNanoSecs(unsigned long long nanoseconds)
{
#ifdef __MSDOS__
# if __BORLANDC__ || __BCPLUSPLUS
#  include <dos.h>
	::delay(((unsigned int)
		(nanoseconds / static_cast<unsigned long long>(1000000))));
# else
	::sleep(((unsigned int)
		(nanoseconds / static_cast<unsigned long long>(1000000000))));
# endif /* # if __BORLANDC__  || __BCPLUSPLUS */
#elif _Windows
	::Sleep(((DWORD) (nanoseconds / static_cast<unsigned long long>(1000000))));
#elif _MSC_VER
	::Sleep(((DWORD) (nanoseconds / static_cast<unsigned long long>(1000000))));
#elif defined(__linux__)
	struct timespec tmp_timespec;

	tmp_timespec.tv_sec  =
		((long) (nanoseconds / static_cast<unsigned long long>(1000000000)));
	tmp_timespec.tv_nsec =
		((long) (nanoseconds % static_cast<unsigned long long>(1000000000)));

	::nanosleep(&tmp_timespec, NULL);
#elif __SVR4
	struct timespec tmp_timespec;

	tmp_timespec.tv_sec  =
		((long) (nanoseconds / static_cast<unsigned long long>(1000000000)));
	tmp_timespec.tv_nsec =
		((long) (nanoseconds % static_cast<unsigned long long>(1000000000)));

	::nanosleep(&tmp_timespec, NULL);
#else
	::usleep(static_cast<unsigned long>(
		nanoseconds / static_cast<unsigned long long>(1000)));
#endif /* #ifdef __MSDOS__ */
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TimeSpec NanoSleep(const TimeSpec &request)
{
	TimeSpec remainder(0, 0);

#ifdef __SVR4
	if (::nanosleep(&request, &remainder))
		ThrowErrno("Invocation of 'nanosleep()' failed");
#elif defined(__linux__)
	if (::nanosleep(&request, &remainder))
		ThrowErrno("Invocation of 'nanosleep()' failed");
#else
	if ((request.tv_sec < 0L) || (request.tv_nsec < 0L) ||
		(request.tv_nsec > 1000000000L)) {
		std::ostringstream error_text;
		error_text << "Invalid TimeSpec specified (tv_sec = " <<
			static_cast<int>(request.tv_sec) << ", tv_nsec = " <<
			request.tv_nsec << ")";
		ThrowErrno(EINVAL, error_text);
	}
	else {
		SleepNanoSecs((static_cast<unsigned long>(request.tv_sec) * 1000000000L) +
			request.tv_nsec);
		remainder.tv_sec  = 0;
		remainder.tv_nsec = 0L;
	}
#endif // #ifdef __SVR4

	return(remainder);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void YieldThreadQuantum()
{
#ifdef _Windows
	::Sleep(0);
#else
	SleepMicroSecs(0);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

