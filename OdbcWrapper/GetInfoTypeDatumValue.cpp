// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the GetInfoTypeDatumValue class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
GetInfoTypeDatumValue::GetInfoTypeDatumValue()
	:info_type_(0)
	,is_string_(false)
	,is_small_(false)
	,is_mask_(false)
	,is_flag_(false)
	,is_handle_(false)
	,is_handle_2way_(false)
	,value_string_()
	,value_USMALLINT_(0)
	,value_UINTEGER_(0)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
GetInfoTypeDatumValue::GetInfoTypeDatumValue(
	const GetInfoTypeDatumRaw &raw_descriptor)
	:info_type_(raw_descriptor.info_type_)
	,is_string_(raw_descriptor.is_string_)
	,is_small_(raw_descriptor.is_small_)
	,is_mask_(raw_descriptor.is_mask_)
	,is_flag_(raw_descriptor.is_flag_)
	,is_handle_(raw_descriptor.is_handle_)
	,is_handle_2way_(raw_descriptor.is_handle_2way_)
	,value_string_()
	,value_USMALLINT_(0)
	,value_UINTEGER_(0)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
GetInfoTypeDatumValue::~GetInfoTypeDatumValue()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool GetInfoTypeDatumValue::operator < (
	const GetInfoTypeDatumValue &other) const
{
	return(info_type_ < other.info_type_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
GetInfoTypeDatumValue &GetInfoTypeDatumValue::swap(
	GetInfoTypeDatumValue &other)
{
	std::swap(info_type_, other.info_type_);
	std::swap(is_string_, other.is_string_);
	std::swap(is_small_, other.is_small_);
	std::swap(is_mask_, other.is_mask_);
	std::swap(is_flag_, other.is_flag_);
	std::swap(is_handle_, other.is_handle_);
	std::swap(is_handle_2way_, other.is_handle_2way_);
	value_string_.swap(other.value_string_);
	std::swap(value_USMALLINT_, other.value_USMALLINT_);
	std::swap(value_UINTEGER_, other.value_UINTEGER_);

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &GetInfoTypeDatumValue::ToString(
	std::string &out_string) const
{
	if (is_string_)
		out_string.assign(value_string_);
	else {
		std::ostringstream o_str;
		if (is_small_)
			o_str << value_USMALLINT_;
		else if (is_mask_)
			o_str << MLB::Utility::ValueToStringBin(value_UINTEGER_);
		else if (is_flag_)
			o_str <<
				MLB::Utility::AnyToString((value_UINTEGER_) ? true : false);
		else if (is_handle_ || is_handle_2way_)
			o_str << MLB::Utility::ValueToStringHex(value_UINTEGER_);
		else
			o_str << value_UINTEGER_;
		out_string = o_str.str();
	}

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetInfoTypeDatumValue::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &GetInfoTypeDatumValue::ToStringPadded(
	std::string &out_string) const
{
	if (is_string_)
		out_string.assign(value_string_);
	else {
		std::ostringstream o_str;
		if (is_small_)
			o_str << std::setw(
				MLB::Utility::IntegralValueMaxLengthDecUnsigned<SQLUSMALLINT>()) <<
				value_USMALLINT_;
		else if (is_mask_)
			o_str << MLB::Utility::ValueToStringBin(value_UINTEGER_);
		else if (is_flag_)
			o_str <<
				MLB::Utility::AnyToString((value_UINTEGER_) ? true : false);
		else if (is_handle_ || is_handle_2way_)
			o_str << MLB::Utility::ValueToStringHex(value_UINTEGER_);
		else
			o_str << std::setw(
				MLB::Utility::IntegralValueMaxLengthDecUnsigned<SQLUINTEGER>()) <<
				value_UINTEGER_;
		out_string = o_str.str();
	}

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetInfoTypeDatumValue::ToStringPadded() const
{
	std::string out_string;

	return(ToStringPadded(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const GetInfoTypeDatumValue &datum)
{
	std::string out_string;

	o_str
		<< datum.ToString(out_string);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

