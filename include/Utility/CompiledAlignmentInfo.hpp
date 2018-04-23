// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the CompiledAlignmentInfo class.

	Revision History	:	1998-04-08 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__CompiledAlignmentInfo_hpp__HH

#define HH__MLB__Utility__CompiledAlignmentInfo_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file CompiledAlignmentInfo.hpp

	\brief	The CompiledAlignmentInfo header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY CompiledAlignmentInfo {
	CompiledAlignmentInfo();

	void swap(CompiledAlignmentInfo &other);

	bool operator  < (const CompiledAlignmentInfo &other) const;
	bool operator == (const CompiledAlignmentInfo &other) const;
	bool operator != (const CompiledAlignmentInfo &other) const;

	int Compare(const CompiledAlignmentInfo &other =
		CompiledAlignmentInfo()) const;

	std::string  ToString() const;
	std::string &ToString(std::string &out_string) const;
	std::string  ToStringLines(unsigned int padding = 27,
		const std::string &separator = ": ") const;
	std::string &ToStringLines(std::string &out_string,
		unsigned int padding = 27, const std::string &separator = ": ") const;

	unsigned int align_u_char_;
	unsigned int align_u_short_;
	unsigned int align_u_int_;
	unsigned int align_u_long_;
	unsigned int align_u_long_long_;
	unsigned int align_float_;
	unsigned int align_double_;
	unsigned int align_long_double_;
	unsigned int align_structure_;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const CompiledAlignmentInfo &datum);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__CompiledAlignmentInfo_hpp__HH

