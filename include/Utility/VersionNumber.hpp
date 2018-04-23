// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the VersionNumber class.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__VersionNumber_hpp__HH

#define HH__MLB__Utility__VersionNumber_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file VersionNumber.hpp

	\brief	The MLB VersionNumber header file.
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
struct API_UTILITY VersionNumber {
	enum VersionNumberIndex {
		Major   = 0,
		Minor   = 1,
		Release = 2,
		Build   = 3
	};

	explicit VersionNumber(unsigned int version_major = 0,
		unsigned int version_minor = 0, unsigned int version_release = 0,
		unsigned int version_build = 0);
	explicit VersionNumber(const std::string &version_number);

	void swap(VersionNumber &other);

	unsigned int GetMajor() const;
	unsigned int GetMinor() const;
	unsigned int GetRelease() const;
	unsigned int GetBuild() const;

	unsigned int SetMajor(unsigned int version_major);
	unsigned int SetMinor(unsigned int version_minor);
	unsigned int SetRelease(unsigned int version_release);
	unsigned int SetBuild(unsigned int version_build);

	VersionNumber &SetToMinimumValue();
	VersionNumber &SetToMaximumValue();

	bool operator <  (const VersionNumber &other) const;
	bool operator >  (const VersionNumber &other) const;
	bool operator <= (const VersionNumber &other) const;
	bool operator >= (const VersionNumber &other) const;
	bool operator == (const VersionNumber &other) const;
	bool operator != (const VersionNumber &other) const;

	std::string  ToString() const;
	std::string &ToString(std::string &out_string) const;

	static VersionNumber FromString(const std::string &in_datum,
		bool require_all_segments = true);
	static VersionNumber FromString(const char *in_datum,
		bool require_all_segments = true);

	static VersionNumber GetMinimumValue();
	static VersionNumber GetMaximumValue();

	static std::string GetElementName(unsigned int element_index);

	unsigned int version_[4];
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const VersionNumber &datum);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__VersionNumber_hpp__HH

