// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the CompiledAlignmentInfo class.

	Revision History	:	2006-01-07 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/AlignmentSupport.hpp>
#include <Utility/CompiledAlignmentInfo.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
CompiledAlignmentInfo::CompiledAlignmentInfo()
	:align_u_char_(GetAlignmentRequirement<unsigned char>())
	,align_u_short_(GetAlignmentRequirement<unsigned short>())
	,align_u_int_(GetAlignmentRequirement<unsigned int>())
	,align_u_long_(GetAlignmentRequirement<unsigned long>())
	,align_u_long_long_(GetAlignmentRequirement<unsigned long long>())
	,align_float_(GetAlignmentRequirement<float>())
	,align_double_(GetAlignmentRequirement<double>())
	,align_long_double_(GetAlignmentRequirement<long double>())
	,align_structure_(GetAlignmentRequirementStructure())
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CompiledAlignmentInfo::swap(CompiledAlignmentInfo &other)
{
	std::swap(align_u_char_, other.align_u_char_);
	std::swap(align_u_short_, other.align_u_short_);
	std::swap(align_u_int_, other.align_u_int_);
	std::swap(align_u_long_, other.align_u_long_);
	std::swap(align_u_long_long_, other.align_u_long_long_);
	std::swap(align_float_, other.align_float_);
	std::swap(align_double_, other.align_double_);
	std::swap(align_long_double_, other.align_long_double_);
	std::swap(align_structure_, other.align_structure_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CompiledAlignmentInfo::operator < (
	const CompiledAlignmentInfo &other) const
{
	return(Compare(other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CompiledAlignmentInfo::operator == (
	const CompiledAlignmentInfo &other) const
{
	return(Compare(other) == 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CompiledAlignmentInfo::operator != (
	const CompiledAlignmentInfo &other) const
{
	return(!(*this == other));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int CompiledAlignmentInfo::Compare(const CompiledAlignmentInfo &other) const
{
	int cmp;

	if ((cmp = (static_cast<int>(align_u_char_) -
		static_cast<int>(other.align_u_char_))) == 0) {
		if ((cmp = (static_cast<int>(align_u_short_) -
			static_cast<int>(other.align_u_short_))) == 0) {
			if ((cmp = (static_cast<int>(align_u_int_) -
				static_cast<int>(other.align_u_int_))) == 0) {
				if ((cmp = (static_cast<int>(align_u_long_) -
					static_cast<int>(other.align_u_long_))) == 0) {
					if ((cmp = (static_cast<int>(align_u_long_long_) -
						static_cast<int>(other.align_u_long_long_))) == 0) {
						if ((cmp = (static_cast<int>(align_float_) -
							static_cast<int>(other.align_float_))) == 0) {
							if ((cmp = (static_cast<int>(align_double_) -
								static_cast<int>(other.align_double_))) == 0) {
								if ((cmp = (static_cast<int>(align_long_double_) -
									static_cast<int>(other.align_long_double_))) == 0) {
									cmp = static_cast<int>(align_structure_) -
										static_cast<int>(other.align_structure_);
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
std::string CompiledAlignmentInfo::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &CompiledAlignmentInfo::ToString(std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		"alignmentOfUnsignedChar = "     << align_u_char_      << ", " <<
		"alignmentOfUnsignedShort = "    << align_u_short_     << ", " <<
		"alignmentOfUnsignedInt = "      << align_u_int_       << ", " <<
		"alignmentOfUnsignedLong = "     << align_u_long_      << ", " <<
		"alignmentOfUnsignedLongLong = " << align_u_long_long_ << ", " <<
		"alignmentOfFloat = "            << align_float_       << ", " <<
		"alignmentOfDouble = "           << align_double_      << ", " <<
		"alignmentOfLongDouble = "       << align_long_double_ << ", " <<
		"alignmentOfStructure = "        << align_structure_;

	return(out_string.assign(o_str.str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CompiledAlignmentInfo::ToStringLines(unsigned int padding,
	const std::string &separator) const
{
	std::string out_string;

	return(ToStringLines(out_string, padding, separator));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &CompiledAlignmentInfo::ToStringLines(std::string &out_string,
	unsigned int padding, const std::string &separator) const
{
	std::ostringstream o_str;

	o_str <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfUnsignedChar"     <<
			std::right << separator << align_u_char_      << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfUnsignedShort"    <<
			std::right << separator << align_u_short_     << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfUnsignedInt"      <<
			std::right << separator << align_u_int_       << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfUnsignedLong"     <<
			std::right << separator << align_u_long_      << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfUnsignedLongLong" <<
			std::right << separator << align_u_long_long_ << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfFloat"            <<
			std::right << separator << align_float_       << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfDouble"           <<
			std::right << separator << align_double_      << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfLongDouble"       <<
			std::right << separator << align_long_double_ << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			"alignmentOfStructure" <<
			std::right << separator << align_structure_;

	return(out_string.assign(o_str.str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const CompiledAlignmentInfo &datum)
{
	o_str <<
		datum.ToString();

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

