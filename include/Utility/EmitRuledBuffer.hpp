// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	EmitRuledBuffer.hpp

	File Description	:	Header file for the EmitRuledBuffer functions.

	Revision History	:	2017-10-13 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2017 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__EmitRuledBuffer_hpp__HH

#define HH__MLB__Utility__EmitRuledBuffer_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
const int ErbFlag_None         = 0x00;
const int ErbFlag_NoCEscSeqs   = 0x01;
const int ErbFlag_UseHexNul    = 0x02;
const int ErbFlag_Use8BitAscii = 0x04;
const int ErbFlag_HexRule      = 0x08;
const int ErbFlag_RuleOnTop    = 0x10;
const int ErbFlag_Default      = ErbFlag_None;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EmitRuledBuffer(std::size_t src_length,
	const char *src_ptr, unsigned long long start_offset = 0,
	int flags = ErbFlag_Default);
std::vector<std::string> EmitRuledBuffer(const char *src_ptr,
	unsigned long long start_offset = 0, int flags = ErbFlag_Default);
std::vector<std::string> EmitRuledBuffer(const char *begin_ptr,
	const char *end_ptr, unsigned long long start_offset = 0,
	int flags = ErbFlag_Default);
std::vector<std::string> EmitRuledBuffer(const std::string &src,
	unsigned long long start_offset = 0, int flags = ErbFlag_Default);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__EmitRuledBuffer_hpp__HH

