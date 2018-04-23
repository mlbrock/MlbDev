// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the HostVirtualMemoryPageInfo class.

	Revision History	:	1998-04-08 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__HostVirtualMemoryPageInfo_hpp__HH

#define HH__MLB__Utility__HostVirtualMemoryPageInfo_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file HostVirtualMemoryPageInfo.hpp

	\brief	The HostVirtualMemoryPageInfo header file.
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
struct API_UTILITY HostVirtualMemoryPageInfo {
	HostVirtualMemoryPageInfo();

	void swap(HostVirtualMemoryPageInfo &other);

	bool operator  < (const HostVirtualMemoryPageInfo &other) const;
	bool operator == (const HostVirtualMemoryPageInfo &other) const;
	bool operator != (const HostVirtualMemoryPageInfo &other) const;

	int Compare(const HostVirtualMemoryPageInfo &other =
		HostVirtualMemoryPageInfo()) const;

	std::string  ToString() const;
	std::string &ToString(std::string &out_string) const;
	std::string  ToStringLines(unsigned int padding = 28,
		const std::string &separator = ": ") const;
	std::string &ToStringLines(std::string &out_string,
		unsigned int padding = 28, const std::string &separator = ": ") const;

	unsigned int page_size_;
	unsigned int page_alloc_granularity_size_;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const HostVirtualMemoryPageInfo &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__HostVirtualMemoryPageInfo_hpp__HH

