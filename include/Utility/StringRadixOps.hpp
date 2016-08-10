// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	StringRadixOps.hpp

	File Description	:	Include file for the new, yet-to-be-fully-implemented,
								string <---> integer radix functions.

	Revision History	:	1995-10-10 --- Portions of original logic from
													MlbDev/Utility/IntToString.cpp.
									Michael L. Brock
								2016-08-07 --- Base-62 specific logic first-cut.
									Michael L. Brock

		Copyright Michael L. Brock 1995 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__StringRadixOps_hpp__HH

#define HH__MLB__Utility__StringRadixOps_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file 	StringRadixOps.hpp

	\brief	String <--> radix functions.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <mbtypes.h>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
Native_UInt64 StringToUIntRadix62(const char *begin_ptr, const char *end_ptr,
	const char **last_ptr = NULL);
Native_UInt64 StringToUIntRadix62(std::size_t src_len, const char *src_ptr,
	const char **last_ptr = NULL);
Native_UInt64 StringToUIntRadix62(const char *src_ptr,
	const char **last_ptr = NULL);
Native_UInt64 StringToUIntRadix62(const std::string &src,
	std::size_t *last_offset = NULL);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__StringRadixOps_hpp__HH
