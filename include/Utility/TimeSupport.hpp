// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the date/time  support logic.

	Revision History	:	1998-04-08 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__TimeSupport_hpp__HH

#define HH__MLB__Utility__TimeSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file TimeSupport.hpp

	\brief	The date/time support logic header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <ctime>

#ifdef _Windows
# if !defined(__MINGW32__)
#  if defined(_MSC_VER) && (_MSC_VER >= 1400)
#   pragma warning(disable:4548)
#  endif // #if defined(_MSC_VER) && (_MSC_VER >= 1400)
#  pragma warning(push)
#  pragma warning(disable:4018 4100 4146 4244 4290 4511 4512 4663)
# endif // #if !defined(__MINGW32__)
# include <time.h>
# if !defined(__MINGW32__)
#  pragma warning(pop)
# endif // #if !defined(__MINGW32__)
// No struct timespec in Windows
struct timespec {
	time_t tv_sec;
	long   tv_nsec;
};
#else
# include <time.h>
# include <unistd.h>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
	//	Unadorned time:
	//		hh:mm:ss
const unsigned int Length_Time_Basic	      = 2 + 1 + 2 + 1 + 2;

	//	Simplest of all:
	//		YYYY-MM-DD
const unsigned int Length_Date_Basic         = 4 + 1 + 2 + 1 + 2;

	//		dddddd
const unsigned int Length_Days_Basic         = 6;

	//	Same as above plus a space followed by the eight-character time:
	//		YYYY-MM-DD hh:mm:ss
const unsigned int Length_TimeTM             = Length_Date_Basic + 1 + Length_Time_Basic;

	//	The result of a call to asctime() without the trailing line-feed:
	//		Www Mmm dd hh::mm::ss YYYY
const unsigned int Length_TimeTM_AscTimeSane = 3 + 1 + 3 + 1 + 2 + 1 + Length_Time_Basic + 1 + 4;

	//	The result of a call to asctime() with the trailing line-feed:
	//		Www Mmm dd hh::mm::ss YYYY\n
const unsigned int Length_TimeTM_AscTime     = Length_TimeTM_AscTimeSane + 1;

	//	Same as above, no difference:
	//		YYYY-MM-DD hh:mm:ss
const unsigned int Length_TimeT              = Length_TimeTM;
	//		YYYY-MM-DD hh:mm:ss
const unsigned int LengthInterval_TimeT      = Length_Days_Basic + 1 + Length_Time_Basic;

	//	Same as above plus a period followed by the number of milliseconds:
	//		YYYY-MM-DD hh:mm:ss.mmm
const unsigned int Length_TimeB              = Length_TimeTM + 1 + 3;
	//		dddddd hh:mm:ss.mmm
const unsigned int LengthInterval_TimeB      = Length_Days_Basic + 1 + Length_Time_Basic + 1 + 3;

	//	Same as above plus a period followed by the number of microseconds:
	//		YYYY-MM-DD hh:mm:ss.uuuuuu
const unsigned int Length_TimeVal		      = Length_TimeTM + 1 + 6;
	//		dddddd hh:mm:ss.uuuuuu
const unsigned int LengthInterval_TimeVal		= Length_Days_Basic + 1 + Length_Time_Basic + 1 + 6;

	//	As above, but with a period followed by the number of nanoseconds instead:
	//		YYYY-MM-DD hh:mm:ss.nnnnnnnnn
const unsigned int Length_TimeSpec	         = Length_TimeTM + 1 + 9;
	//		dddddd hh:mm:ss.nnnnnnnnn
const unsigned int LengthInterval_TimeSpec	= Length_Days_Basic + 1 + Length_Time_Basic + 1 + 9;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseFromString(const char *in_date, time_t &out_secs,
	long &out_fractional, long fractional_places);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/**
	\brief A wrapper class around \c struct \c tm.

	The wrapper is implemented by public inheritance from \c struct \c tm ,
	so substitutability is supported.
*/
struct API_UTILITY TimeTM : public tm {
	TimeTM();
	explicit TimeTM(const tm &in_time);
	~TimeTM();

	bool operator <  (const TimeTM &other) const;
	bool operator >  (const TimeTM &other) const;
	bool operator <= (const TimeTM &other) const;
	bool operator >= (const TimeTM &other) const;
	bool operator == (const TimeTM &other) const;
	bool operator != (const TimeTM &other) const;

	char        *AscTime(char *buffer) const;
	std::string  AscTime() const;
	char        *AscTimeSane(char *buffer) const;
	std::string  AscTimeSane() const;

	char        *ToString(char *buffer,
		unsigned int max_length = Length_TimeTM) const;
	std::string  ToString(unsigned int max_length = Length_TimeTM) const;
	std::string &ToString(std::string &out_string,
		unsigned int max_length = Length_TimeTM) const;

	const struct tm &GetUnderlyingRef() const
	{
		return(*this);
	}
	struct tm &GetUnderlyingRef()
	{
		return(*this);
	}
	struct tm GetUnderlying() const
	{
		return(*this);
	}

	static TimeTM Now();

	static TimeTM TimeUTC(const time_t in_time = time(NULL));
	static TimeTM TimeLocal(const time_t in_time = time(NULL));

	//	Used to support a C-style interface...
	static int    Compare(const TimeTM *lhs, const TimeTM *rhs);
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const TimeTM &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/**
	\brief A wrapper class around \c time_t.
*/
struct API_UTILITY TimeT {
	         TimeT();
	explicit TimeT(const time_t &in_time);
	explicit TimeT(const std::string &in_date);

	~TimeT();

	bool operator <  (const TimeT &other) const;
	bool operator >  (const TimeT &other) const;
	bool operator <= (const TimeT &other) const;
	bool operator >= (const TimeT &other) const;
	bool operator == (const TimeT &other) const;
	bool operator != (const TimeT &other) const;

	int  Compare(const TimeT &other) const;

	TimeT &SetToNow();
	TimeT &SetToMinimumValue();
	TimeT &SetToMaximumValue();

	bool IsZero() const;

	TimeT &AddSeconds(int secs_to_add);
	TimeT &AddMilliseconds(int msecs_to_add);
	TimeT &AddMicroseconds(int usecs_to_add);
	TimeT &AddNanoseconds(int nsecs_to_add);

	char        *ToString(char *buffer,
		unsigned int max_length = Length_TimeT) const;
	std::string  ToString(unsigned int max_length = Length_TimeT) const;
	std::string &ToString(std::string &out_string,
		unsigned int max_length = Length_TimeT) const;

	char        *ToStringLocal(char *buffer,
		unsigned int max_length = Length_TimeT) const;
	std::string  ToStringLocal(unsigned int max_length = Length_TimeT) const;
	std::string &ToStringLocal(std::string &out_string,
		unsigned int max_length = Length_TimeT) const;

	char        *ToStringInterval(char *buffer,
		unsigned int max_length = LengthInterval_TimeT) const;
	std::string  ToStringInterval(
		unsigned int max_length = LengthInterval_TimeT) const;
	std::string &ToStringInterval(std::string &out_string,
		unsigned int max_length = LengthInterval_TimeT) const;

	const time_t &GetUnderlyingRef() const
	{
		return(datum_);
	}
	time_t &GetUnderlyingRef()
	{
		return(datum_);
	}
	time_t GetUnderlying() const
	{
		return(datum_);
	}

	static TimeT FromString(const std::string &in_date);
	static TimeT FromString(const char *in_date);

	static TimeT Now();

	//	Used to support a C-style interface...
	static int   Compare(const TimeT *lhs, const TimeT *rhs);

	static TimeT GetMinimumValue();
	static TimeT GetMaximumValue();

	time_t datum_;

private:
	char *FormatString(const TimeTM &in_tm, char *buffer,
		unsigned int max_length = Length_TimeT) const;
	char *FormatStringInterval(char *buffer,
		unsigned int max_length = LengthInterval_TimeT) const;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const TimeT &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/**
	\brief A wrapper class around \c struct \c timeval.

	The wrapper is implemented by public inheritance from \c struct \c timeval ,
	so substitutability is supported.
*/
struct API_UTILITY TimeVal : public timeval {
	         TimeVal();
	explicit TimeVal(time_t in_secs, long in_usecs = 0L);
	explicit TimeVal(const timeval &in_time);
	explicit TimeVal(const std::string &in_date);

	~TimeVal();

	bool operator <  (const TimeVal &other) const;
	bool operator >  (const TimeVal &other) const;
	bool operator <= (const TimeVal &other) const;
	bool operator >= (const TimeVal &other) const;
	bool operator == (const TimeVal &other) const;
	bool operator != (const TimeVal &other) const;

	int  Compare(const TimeVal &other) const;

	TimeVal &SetToNow();
	TimeVal &SetToMinimumValue();
	TimeVal &SetToMaximumValue();

	bool IsZero() const;

	TimeVal &AddSeconds(int secs_to_add);
	TimeVal &AddMilliseconds(int msecs_to_add);
	TimeVal &AddMicroseconds(int usecs_to_add);
	TimeVal &AddNanoseconds(int nsecs_to_add);

	void swap(TimeVal &other);

	char        *ToString(char *buffer,
		unsigned int max_length = Length_TimeVal) const;
	std::string  ToString(unsigned int max_length = Length_TimeVal) const;
	std::string &ToString(std::string &out_string,
		unsigned int max_length = Length_TimeVal) const;

	char        *ToStringLocal(char *buffer,
		unsigned int max_length = Length_TimeVal) const;
	std::string  ToStringLocal(unsigned int max_length = Length_TimeVal) const;
	std::string &ToStringLocal(std::string &out_string,
		unsigned int max_length = Length_TimeVal) const;

	char        *ToStringInterval(char *buffer,
		unsigned int max_length = LengthInterval_TimeVal) const;
	std::string  ToStringInterval(
		unsigned int max_length = LengthInterval_TimeVal) const;
	std::string &ToStringInterval(std::string &out_string,
		unsigned int max_length = LengthInterval_TimeVal) const;

	TimeT   ToTimeT() const;

	unsigned long long ToTicks() const;
	unsigned long long ToMicroseconds() const;

	double GetDoubleEquivalent() const;
	double GetDoubleTicks() const;

	static signed long long GetDifferenceTicks(const TimeVal &time_1,
		const TimeVal &time_2);

	static double GetDifferenceTicksDouble(const TimeVal &time_1,
		const TimeVal &time_2);
	static double GetDifferenceDouble(const TimeVal &time_1,
		const TimeVal &time_2);

	static TimeVal GetDifference(const TimeVal &time_1,
		const TimeVal &time_2);
	static TimeVal GetDifferenceAbs(const TimeVal &time_1,
		const TimeVal &time_2);

	const struct timeval &GetUnderlyingRef() const
	{
		return(*this);
	}
	struct timeval &GetUnderlyingRef()
	{
		return(*this);
	}
	struct timeval GetUnderlying() const
	{
		return(*this);
	}

	static TimeVal FromString(const std::string &in_date);
	static TimeVal FromString(const char *in_date);
	static TimeVal FromMicroseconds(unsigned long long usecs);

	static TimeVal Now();

	//	Used to support a C-style interface...
	static int     Compare(const TimeVal *lhs, const TimeVal *rhs);

	static TimeVal GetMinimumValue();
	static TimeVal GetMaximumValue();

private:
	char *FormatString(const TimeTM &in_tm, char *buffer,
		unsigned int max_length = Length_TimeVal) const;
	char *FormatStringInterval(char *buffer,
		unsigned int max_length = LengthInterval_TimeVal) const;
	char *AppendFractionalPortion(unsigned int idx, char *buffer) const;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const TimeVal &datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/**
	\brief A wrapper class around \c struct \c timespec.

	The wrapper is implemented by public inheritance from \c struct \c timespec ,
	so substitutability is supported.
*/
struct API_UTILITY TimeSpec : public timespec {
	TimeSpec();
	explicit TimeSpec(time_t in_secs, long in_nsecs = 0L);
	explicit TimeSpec(const timespec &in_time);
	explicit TimeSpec(const timeval &in_time);
	explicit TimeSpec(const std::string &in_date);
	~TimeSpec();

	bool operator <  (const TimeSpec &other) const;
	bool operator >  (const TimeSpec &other) const;
	bool operator <= (const TimeSpec &other) const;
	bool operator >= (const TimeSpec &other) const;
	bool operator == (const TimeSpec &other) const;
	bool operator != (const TimeSpec &other) const;

	int  Compare(const TimeSpec &other) const;

	TimeSpec &SetToNow();
	TimeSpec &SetToMinimumValue();
	TimeSpec &SetToMaximumValue();

	bool IsZero() const;

	TimeSpec &AddSeconds(int secs_to_add);
	TimeSpec &AddMilliseconds(int msecs_to_add);
	TimeSpec &AddMicroseconds(int usecs_to_add);
	TimeSpec &AddNanoseconds(int nsecs_to_add);

	void swap(TimeSpec &other);

	char        *ToString(char *buffer,
		unsigned int max_length = Length_TimeSpec) const;
	std::string  ToString(unsigned int max_length = Length_TimeSpec) const;
	std::string &ToString(std::string &out_string,
		unsigned int max_length = Length_TimeSpec) const;

	char        *ToStringLocal(char *buffer,
		unsigned int max_length = Length_TimeSpec) const;
	std::string  ToStringLocal(unsigned int max_length = Length_TimeSpec) const;
	std::string &ToStringLocal(std::string &out_string,
		unsigned int max_length = Length_TimeSpec) const;

	char        *ToStringInterval(char *buffer,
		unsigned int max_length = LengthInterval_TimeSpec) const;
	std::string  ToStringInterval(
		unsigned int max_length = LengthInterval_TimeSpec) const;
	std::string &ToStringInterval(std::string &out_string,
		unsigned int max_length = LengthInterval_TimeSpec) const;

	TimeT   ToTimeT() const;
	TimeVal ToTimeVal() const;

	unsigned long long ToTicks() const;
	unsigned long long ToNanoseconds() const;

	double GetDoubleEquivalent() const;
	double GetDoubleTicks() const;

	static signed long long GetDifferenceTicks(const TimeSpec &time_1,
		const TimeSpec &time_2);

	static double GetDifferenceTicksDouble(const TimeSpec &time_1,
		const TimeSpec &time_2);
	static double GetDifferenceDouble(const TimeSpec &time_1,
		const TimeSpec &time_2);

	static TimeSpec GetDifference(const TimeSpec &time_1,
		const TimeSpec &time_2);
	static TimeSpec GetDifferenceAbs(const TimeSpec &time_1,
		const TimeSpec &time_2);

	const struct timespec &GetUnderlyingRef() const
	{
		return(*this);
	}
	struct timespec &GetUnderlyingRef()
	{
		return(*this);
	}
	struct timespec GetUnderlying() const
	{
		return(*this);
	}

	static TimeSpec FromString(const std::string &in_date);
	static TimeSpec FromString(const char *in_date);
	static TimeSpec FromNanoseconds(unsigned long long nsecs);

	static TimeSpec Now();

	//	Used to support a C-style interface...
	static int      Compare(const TimeSpec *lhs, const TimeSpec *rhs);

	static TimeSpec GetMinimumValue();
	static TimeSpec GetMaximumValue();

private:
	char *FormatString(const TimeTM &in_tm, char *buffer,
		unsigned int max_length = Length_TimeSpec) const;
	char *FormatStringInterval(char *buffer,
		unsigned int max_length = LengthInterval_TimeSpec) const;
	char *AppendFractionalPortion(unsigned int idx, char *buffer) const;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const TimeSpec &datum);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__TimeSupport_hpp__HH

