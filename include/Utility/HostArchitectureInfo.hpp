// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the HostArchitectureInfo class.

	Revision History	:	1998-04-08 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__HostArchitectureInfo_hpp__HH

#define HH__MLB__Utility__HostArchitectureInfo_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file HostArchitectureInfo.hpp

	\brief	The HostArchitectureInfo header file.
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

// ////////////////////////////////////////////////////////////////////////////
struct API_UTILITY HostArchitectureInfo {
	HostArchitectureInfo();

	void swap(HostArchitectureInfo &other);

	bool operator  < (const HostArchitectureInfo &other) const;
	bool operator == (const HostArchitectureInfo &other) const;
	bool operator != (const HostArchitectureInfo &other) const;

	int Compare(const HostArchitectureInfo &other =
		HostArchitectureInfo()) const;

	std::string  ToString() const;
	std::string &ToString(std::string &out_string) const;
	std::string  ToStringLines(unsigned int padding = 22,
		const std::string &separator = ": ") const;
	std::string &ToStringLines(std::string &out_string,
		unsigned int padding = 22, const std::string &separator = ": ") const;

	unsigned int   char_bits_;
	unsigned int   sizeof_u_char_;
	unsigned int   sizeof_u_short_;
	unsigned int   sizeof_u_int_;
	unsigned int   sizeof_u_long_;
	unsigned int   sizeof_u_long_long_;
	unsigned int   sizeof_float_;
	unsigned int   sizeof_double_;
	unsigned int   sizeof_long_double_;
	bool           is_big_endian_;
	bool           is_network_byte_order_;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const HostArchitectureInfo &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__HostArchitectureInfo_hpp__HH

