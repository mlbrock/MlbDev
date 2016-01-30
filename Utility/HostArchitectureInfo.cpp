// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the HostArchitectureInfo class.

	Revision History	:	2006-01-07 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/HostArchitectureInfo.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
HostArchitectureInfo::HostArchitectureInfo()
	:char_bits_(CHAR_BIT)
	,sizeof_u_char_(sizeof(unsigned char))
	,sizeof_u_short_(sizeof(unsigned short))
	,sizeof_u_int_(sizeof(unsigned int))
	,sizeof_u_long_(sizeof(unsigned long))
	,sizeof_u_long_long_(sizeof(unsigned long long))
	,sizeof_float_(sizeof(float))
	,sizeof_double_(sizeof(double))
	,sizeof_long_double_(sizeof(long double))
	,is_big_endian_(IsBigEndianArchitecture())
	,is_network_byte_order_(HostByteOrderEqualNetworkByteOrder())
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void HostArchitectureInfo::swap(HostArchitectureInfo &other)
{
	std::swap(char_bits_, other.char_bits_);
	std::swap(sizeof_u_char_, other.sizeof_u_char_);
	std::swap(sizeof_u_short_, other.sizeof_u_short_);
	std::swap(sizeof_u_int_, other.sizeof_u_int_);
	std::swap(sizeof_u_long_, other.sizeof_u_long_);
	std::swap(sizeof_u_long_long_, other.sizeof_u_long_long_);
	std::swap(sizeof_float_, other.sizeof_float_);
	std::swap(sizeof_double_, other.sizeof_double_);
	std::swap(sizeof_long_double_, other.sizeof_long_double_);
	std::swap(is_big_endian_, other.is_big_endian_);
	std::swap(is_network_byte_order_, other.is_network_byte_order_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool HostArchitectureInfo::operator < (const HostArchitectureInfo &other) const
{
	return(Compare(other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool HostArchitectureInfo::operator == (const HostArchitectureInfo &other) const
{
	return(Compare(other) == 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool HostArchitectureInfo::operator != (const HostArchitectureInfo &other) const
{
	return(!(*this == other));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int HostArchitectureInfo::Compare(const HostArchitectureInfo &other) const
{
	int cmp;

	if ((cmp = (static_cast<int>(char_bits_) -
		static_cast<int>(other.char_bits_))) == 0) {
		if ((cmp = (static_cast<int>(sizeof_u_char_) -
			static_cast<int>(other.sizeof_u_char_))) == 0) {
			if ((cmp = (static_cast<int>(sizeof_u_short_) -
				static_cast<int>(other.sizeof_u_short_))) == 0) {
				if ((cmp = (static_cast<int>(sizeof_u_int_) -
					static_cast<int>(other.sizeof_u_int_))) == 0) {
					if ((cmp = (static_cast<int>(sizeof_u_long_) -
						static_cast<int>(other.sizeof_u_long_))) == 0) {
						if ((cmp = (static_cast<int>(sizeof_u_long_long_) -
							static_cast<int>(other.sizeof_u_long_long_))) == 0) {
							if ((cmp = (static_cast<int>(sizeof_float_) -
								static_cast<int>(other.sizeof_float_))) == 0) {
								if ((cmp = (static_cast<int>(sizeof_double_) -
									static_cast<int>(other.sizeof_double_))) == 0) {
									if ((cmp = (static_cast<int>(sizeof_long_double_) -
										static_cast<int>(other.sizeof_long_double_))) == 0) {
										if ((cmp = (static_cast<int>(is_big_endian_) -
											static_cast<int>(other.is_big_endian_))) == 0) {
											cmp = static_cast<int>(is_network_byte_order_) -
												static_cast<int>(other.is_network_byte_order_);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return(cmp);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string HostArchitectureInfo::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &HostArchitectureInfo::ToString(std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		"charBits = "               << char_bits_          << ", " <<
		"sizeOfUnsignedChar = "     << sizeof_u_char_      << ", " <<
		"sizeOfUnsignedShort = "    << sizeof_u_short_     << ", " <<
		"sizeOfUnsignedInt = "      << sizeof_u_int_       << ", " <<
		"sizeOfUnsignedLong = "     << sizeof_u_long_      << ", " <<
		"sizeOfUnsignedLongLong = " << sizeof_u_long_long_ << ", " <<
		"sizeOfFloat = "            << sizeof_float_       << ", " <<
		"sizeOfDouble = "           << sizeof_double_      << ", " <<
		"sizeOfLongDouble = "       << sizeof_long_double_ << ", " <<
		"isBigEndian = "            << is_big_endian_      << ", " <<
		"isNetworkByteOrder = "     << is_network_byte_order_;

	return(out_string.assign(o_str.str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string HostArchitectureInfo::ToStringLines(unsigned int padding,
	const std::string &separator) const
{
	std::string out_string;

	return(ToStringLines(out_string, padding, separator));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &HostArchitectureInfo::ToStringLines(std::string &out_string,
	unsigned int padding, const std::string &separator) const
{
	std::ostringstream o_str;

	o_str <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"charBits"               <<
			std::right << separator << char_bits_          << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"sizeOfUnsignedChar"     <<
			std::right << separator << sizeof_u_char_      << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"sizeOfUnsignedShort"    <<
			std::right << separator << sizeof_u_short_     << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"sizeOfUnsignedInt"      <<
			std::right << separator << sizeof_u_int_       << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"sizeOfUnsignedLong"     <<
			std::right << separator << sizeof_u_long_      << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"sizeOfUnsignedLongLong" <<
			std::right << separator << sizeof_u_long_long_ << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"sizeOfFloat"            <<
			std::right << separator << sizeof_float_       << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"sizeOfDouble"           <<
			std::right << separator << sizeof_double_      << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"sizeOfLongDouble"       <<
			std::right << separator << sizeof_long_double_ << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"isBigEndian"            <<
			std::right << separator << is_big_endian_      << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"isNetworkByteOrder"     <<
			std::right << separator << is_network_byte_order_;

	return(out_string.assign(o_str.str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const HostArchitectureInfo &datum)
{
	o_str <<
		datum.ToString();

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

