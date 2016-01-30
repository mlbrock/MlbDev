// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the MACAddress class.

	Revision History	:	1998-04-08 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__MACAddress_hpp__HH

#define HH__MLB__Utility__MACAddress_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file MACAddress.hpp

	\brief	The MACAddress header file.
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
	// Number of bytes in an Ethernet address...
const unsigned int  MACAddress_Length       = 6;

	//	Length of an Ethernet address formatted as a string...
const unsigned int  MACAddress_StringLength =
	(MACAddress_Length * 2) + (MACAddress_Length - 1);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief A class to support retrieval and format conversions of Ethernet
	Media Access Control (MAC) addresses.
*/
struct API_UTILITY MACAddress {
	MACAddress();
	explicit MACAddress(const unsigned char *ethernet_address);

	bool operator <  (const MACAddress &other) const;
	bool operator >  (const MACAddress &other) const;
	bool operator == (const MACAddress &other) const;
	bool operator != (const MACAddress &other) const;
	bool operator <= (const MACAddress &other) const;
	bool operator >= (const MACAddress &other) const;

	int         Compare(const MACAddress &other) const;
	bool 			IsZero() const;

	MACAddress &SetToMinimumValue();
	MACAddress &SetToMaximumValue();

	void Get();
	void Clear();

	char        *ToString(char *buffer, int separator = ':') const;
	std::string  ToString(int separator = ':') const;

	void swap(MACAddress &other);

	static MACAddress GetMyMACAddress();
	static int        Compare(const MACAddress *lhs, const MACAddress *rhs);
	static MACAddress GetMinimumValue();
	static MACAddress GetMaximumValue();

	unsigned char octet[MACAddress_Length];
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const MACAddress &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__MACAddress_hpp__HH

