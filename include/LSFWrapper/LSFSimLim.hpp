// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Simulation LIM Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the LSFWrapper simulation LIM.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>
#include <RvUtilX.hpp>
#include <LSFWrapper/LSFWrapper.hpp>

#ifdef _Windows
# pragma warning(disable:4018 4100 4146 4244 4290 4511 4512 4663)
# include <set>
# include <vector>
# pragma warning(default:4018 4100 4146 4244 4290 4511 4512 4663)
#else
# include <set>
# include <vector>
#endif // #ifdef _Windows

#include <PTypesX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

// ////////////////////////////////////////////////////////////////////////////
static const char SubjectNameBase_Lim[]        = "LIM.REQ";
static const char SubjectNameBase_LimReply[]   = "LIM.REP";
static const char SubjectNameBase_ClientExit[] = "LIM.CLIENT_EXIT";
static const char SubjectNameBase_LimFt[]      = "LIM.FT.REQ";
static const char SubjectNameBase_MLim[]       = "MLIM.REQ";
static const char SubjectNameBase_MLimPub[]    = "MLIM.PUB";
// ////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB


