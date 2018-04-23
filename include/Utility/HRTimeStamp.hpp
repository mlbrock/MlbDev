// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the HRTimeStamp class.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__HRTimeStamp_hpp__HH

#define HH__MLB__Utility__HRTimeStamp_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file HRTimeStamp.hpp

	\brief	The MLB HRTimeStamp header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/HRTimer.hpp>
#include <Utility/TimeSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
const unsigned int HRTimeStampLength = Length_TimeSpec + 1 + 20;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief A portable high-resolution time stamp.
*/
struct API_UTILITY HRTimeStamp {
	HRTimeStamp(bool init_flag = true);
	HRTimeStamp(const timespec &in_time,
		HRTimerTick in_ticks = HRTimer::GetTicksCurrentFromSystem());

	bool operator <  (const HRTimeStamp &other) const;
	bool operator >  (const HRTimeStamp &other) const;
	bool operator == (const HRTimeStamp &other) const;
	bool operator != (const HRTimeStamp &other) const;
	bool operator <= (const HRTimeStamp &other) const;
	bool operator >= (const HRTimeStamp &other) const;

	HRTimeStamp &SetToNow();
	HRTimeStamp &SetToMinimumValue();
	HRTimeStamp &SetToMaximumValue();

	char        *ToString(char *buffer,
		unsigned int max_length = HRTimeStampLength) const;
	std::string  ToString(unsigned int max_length = HRTimeStampLength) const;
	std::string &ToString(std::string &out_string,
		unsigned int max_length = HRTimeStampLength) const;

	void swap(HRTimeStamp &other);

	static int Compare(const HRTimeStamp *lhs, const HRTimeStamp *rhs);
	
	static HRTimeStamp GetMinimumValue();
	static HRTimeStamp GetMaximumValue();

	TimeSpec    time_utc_;
	HRTimerTick time_tick_;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const HRTimeStamp &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__HRTimeStamp_hpp__HH

