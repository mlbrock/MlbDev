// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the TZSetInfo class.

	Revision History	:	2009-07-01 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2009 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__TZSetInfo_hpp__HH

#define HH__MLB__Utility__TZSetInfo_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file TZSetInfo.hpp

	\brief	Include file for the TZSetInfo class. Hacked from the old MLB
				DatFuncs library.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/TimeSupport.hpp>

#include <ctime>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
struct TZSetInfo {
public:
	TZSetInfo();
	TZSetInfo(const TZSetInfo &other);

	TZSetInfo & operator = (const TZSetInfo &other);

	std::time_t GetSecondsOffset(const struct tm *local_tm) const;
	std::time_t GetSecondsOffset(const struct tm &local_tm) const;
	std::time_t GetSecondsOffset(std::time_t in_time) const;
	std::time_t GetSecondsOffset(const TimeT &in_time) const;
	std::time_t GetSecondsOffset() const;

	std::time_t  timezone_;
	int          daylight_;
	const char  *tzname_[2];

	static std::time_t   GetTimezone();
	static int           GetDaylight();
	static char        **GetTZName();

	static std::time_t InvokeTZSet();
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__TZSetInfo_hpp__HH

