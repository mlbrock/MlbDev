//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for log level support.

	Revision History	:	2005-01-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__Utility__LogLevel_hpp__HH

#define HH__MLB__Utility__Utility__LogLevel_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
enum LogLevel {
	LogLevel_Literal   =  0,
	LogLevel_Spam      =  1,
	LogLevel_Minutiae  =  2,
	LogLevel_Debug     =  3,
	LogLevel_Detail    =  4,
	LogLevel_Info      =  5,
	LogLevel_Notice    =  6,
	LogLevel_Warning   =  7,
	LogLevel_Error     =  8,
	LogLevel_Critical  =  9,
	LogLevel_Alert     = 10,
	LogLevel_Emergency = 11,
	LogLevel_Fatal     = 12
};
//	----------------------------------------------------------------------------
enum LogLevelFlag {
	LogFlag_Literal   = (1 << LogLevel_Literal  ),
	LogFlag_Spam      = (1 << LogLevel_Spam     ),
	LogFlag_Minutiae  = (1 << LogLevel_Minutiae ),
	LogFlag_Debug     = (1 << LogLevel_Debug    ),
	LogFlag_Detail    = (1 << LogLevel_Detail   ),
	LogFlag_Info      = (1 << LogLevel_Info     ),
	LogFlag_Notice    = (1 << LogLevel_Notice   ),
	LogFlag_Warning   = (1 << LogLevel_Warning  ),
	LogFlag_Error     = (1 << LogLevel_Error    ),
	LogFlag_Critical  = (1 << LogLevel_Critical ),
	LogFlag_Alert     = (1 << LogLevel_Alert    ),
	LogFlag_Emergency = (1 << LogLevel_Emergency),
	LogFlag_Fatal     = (1 << LogLevel_Fatal    ),
	LogFlag_Mask      =	(1 << LogLevel_Literal  ) |
								(1 << LogLevel_Spam     ) |
								(1 << LogLevel_Minutiae ) |
								(1 << LogLevel_Debug    ) |
								(1 << LogLevel_Detail   ) |
								(1 << LogLevel_Info     ) |
								(1 << LogLevel_Notice   ) |
								(1 << LogLevel_Warning  ) |
								(1 << LogLevel_Error    ) |
								(1 << LogLevel_Critical ) |
								(1 << LogLevel_Alert    ) |
								(1 << LogLevel_Emergency) |
								(1 << LogLevel_Fatal    )
};
//	----------------------------------------------------------------------------
const LogLevel LogLevel_Minimum = LogLevel_Literal;
const LogLevel LogLevel_Maximum = LogLevel_Fatal;
const LogLevel LogLevel_Invalid = static_cast<LogLevel>(-1);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogLevel_hpp__HH

