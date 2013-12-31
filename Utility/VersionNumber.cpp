// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the VersionNumber class.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/StringSupport.hpp>
#include <Utility/VersionNumber.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __GNUC__
# include <string.h>
# include <stdlib.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
VersionNumber::VersionNumber(unsigned int version_major,
	unsigned int version_minor, unsigned int version_release,
	unsigned int version_build)
{
	version_[VersionNumber::Major]   = version_major;
	version_[VersionNumber::Minor]   = version_minor;
	version_[VersionNumber::Release] = version_release;
	version_[VersionNumber::Build]   = version_build;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
VersionNumber::VersionNumber(const std::string &version_number)
{
	*this = FromString(version_number);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void VersionNumber::swap(VersionNumber &other)
{
	unsigned int tmp_version[sizeof(version_) / sizeof(version_[0])];

	::memcpy(tmp_version, version_, sizeof(version_));
	::memcpy(version_, other.version_, sizeof(version_));
	::memcpy(other.version_, tmp_version, sizeof(version_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int VersionNumber::GetMajor() const
{
	return(version_[VersionNumber::Major]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int VersionNumber::GetMinor() const
{
	return(version_[VersionNumber::Minor]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int VersionNumber::GetRelease() const
{
	return(version_[VersionNumber::Release]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int VersionNumber::GetBuild() const
{
	return(version_[VersionNumber::Build]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int VersionNumber::SetMajor(unsigned int version_major)
{
	unsigned old_value = version_[VersionNumber::Major];

	version_[VersionNumber::Major]   = version_major;

	return(old_value);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int VersionNumber::SetMinor(unsigned int version_minor)
{
	unsigned old_value = version_[VersionNumber::Minor];

	version_[VersionNumber::Minor]   = version_minor;

	return(old_value);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int VersionNumber::SetRelease(unsigned int version_release)
{
	unsigned old_value = version_[VersionNumber::Release];

	version_[VersionNumber::Release] = version_release;

	return(old_value);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int VersionNumber::SetBuild(unsigned int version_build)
{
	unsigned old_value = version_[VersionNumber::Build];

	version_[VersionNumber::Build]   = version_build;

	return(old_value);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber &VersionNumber::SetToMinimumValue()
{
	version_[VersionNumber::Major]   = 0;
	version_[VersionNumber::Minor]   = 0;
	version_[VersionNumber::Release] = 0;
	version_[VersionNumber::Build]   = 0;

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber &VersionNumber::SetToMaximumValue()
{
	version_[VersionNumber::Major]   =
		std::numeric_limits<unsigned int>::max();
	version_[VersionNumber::Minor]   =
		std::numeric_limits<unsigned int>::max();
	version_[VersionNumber::Release] =
		std::numeric_limits<unsigned int>::max();
	version_[VersionNumber::Build]   =
		std::numeric_limits<unsigned int>::max();

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VersionNumber::operator < (const VersionNumber &other) const
{
	return(::memcmp(version_, other.version_, sizeof(version_)) < 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VersionNumber::operator > (const VersionNumber &other) const
{
	return(::memcmp(version_, other.version_, sizeof(version_)) > 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VersionNumber::operator <= (const VersionNumber &other) const
{
	return(::memcmp(version_, other.version_, sizeof(version_)) <= 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VersionNumber::operator >= (const VersionNumber &other) const
{
	return(::memcmp(version_, other.version_, sizeof(version_)) >= 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VersionNumber::operator == (const VersionNumber &other) const
{
	return(::memcmp(version_, other.version_, sizeof(version_)) == 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool VersionNumber::operator != (const VersionNumber &other) const
{
	return(::memcmp(version_, other.version_, sizeof(version_)) != 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string VersionNumber::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &VersionNumber::ToString(std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		version_[VersionNumber::Major]   << "." <<
		version_[VersionNumber::Minor]   << "." <<
		version_[VersionNumber::Release] << "." <<
		version_[VersionNumber::Build];

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
VersionNumber VersionNumber::FromString(const std::string &in_datum,
	bool require_all_segments)
{
	return(FromString(in_datum.c_str(), require_all_segments));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
VersionNumber VersionNumber::FromString(const char *in_datum,
	bool require_all_segments)
{
	if (in_datum == NULL)
		ThrowInvalidArgument("Version string pointer is 'NULL'.");

	if (!(*in_datum))
		ThrowInvalidArgument("Version string is empty.");

	StringVector out_list;

	SplitString(in_datum, ".", out_list);

	if (out_list.size() > 4)
		ThrowInvalidArgument("Version string '" + std::string(in_datum) +
			"' number of elements (" + AnyToString(out_list.size()) +
			") exceeds the maximum (4).");

	if (require_all_segments && (out_list.size() < 4))
		ThrowInvalidArgument("Version string '" + std::string(in_datum) +
			"' number of elements (" + AnyToString(out_list.size()) +
			") is not equal to 4.");

	VersionNumber           out_datum;
	StringVector::size_type count_1;

	for (count_1 = 0; count_1 < out_list.size(); ++count_1) {
		char       *end_ptr;
		const char *start_ptr  = out_list[count_1].c_str();
		double      tmp_double = ::strtod(start_ptr, &end_ptr);
		if (end_ptr != (start_ptr + ::strlen(start_ptr)))
			ThrowInvalidArgument("Version string '" + std::string(in_datum) +
				"' " + GetElementName(static_cast<unsigned int>(count_1)) +
				" element contains at least one invalid character (ASCII " +
				AnyToString(static_cast<unsigned int>(*end_ptr)) + ").");
		else if ((tmp_double <
			static_cast<double>(std::numeric_limits<unsigned int>::min())) ||
			(tmp_double >
			static_cast<double>(std::numeric_limits<unsigned int>::max())))
			ThrowInvalidArgument("Version string '" + std::string(in_datum) +
				"' " + GetElementName(static_cast<unsigned int>(count_1)) +
				" element is outside of the permissible range (from " +
				AnyToString(std::numeric_limits<unsigned int>::min()) + " to " +
				AnyToString(std::numeric_limits<unsigned int>::max()) + " to " +
				", inclusive).");
		else if (tmp_double != ::floor(tmp_double))
			ThrowInvalidArgument("Version string '" + std::string(in_datum) +
				"' " + GetElementName(static_cast<unsigned int>(count_1)) +
				" element is not an integral value.");
		out_datum.version_[count_1] = static_cast<unsigned int>(tmp_double);
	}

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber VersionNumber::GetMinimumValue()
{
	return(VersionNumber().SetToMinimumValue());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber VersionNumber::GetMaximumValue()
{
	return(VersionNumber().SetToMaximumValue());
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string VersionNumber::GetElementName(unsigned int element_index)
{
	return((element_index == 0) ? "major" :
		((element_index == 1) ? "minor" :
		((element_index == 2) ? "release" :
		((element_index == 3) ? "build" : "*** INVALID ***"))));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const VersionNumber &datum)
{
	std::string out_string;

	o_str <<
		datum.ToString(out_string);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

