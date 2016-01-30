// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the parsing of strings as numeric
								quantities.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__ParseNumericString_hpp__HH

#define HH__MLB__Utility__ParseNumericString_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file		ParseNumericString.hpp

	\brief	Logic to support parsing of strings as numeric quantities.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/NumericSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <cstdlib>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
/*
	Default implementation for floating-point types...
*/
template <bool IsInteger, bool IsSigned> struct NumericTypeSpec {
	typedef double type;

	static type StrToTypeBased(const char *start_ptr, char **end_ptr, int base)
	{
		if (base != 10)
			ThrowInvalidArgument("Only base 10 is supported for the conversion "
				"of strings to a floating-point value.");

		return(StrToType(start_ptr, end_ptr));
	}

	static type StrToType(const char *start_ptr, char **end_ptr)
	{
		return(::strtod(start_ptr, end_ptr));
	}

	static const char *GetTypeName()
	{
		return("double");
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
	Implementation for signed integral types...
*/
template <> struct NumericTypeSpec<true, true> {
	typedef signed long long type;

	static type StrToTypeBased(const char *start_ptr, char **end_ptr, int base)
	{
#ifdef _MSC_VER
		return(::_strtoi64(start_ptr, end_ptr, base));
#else
		return(::strtoll(start_ptr, end_ptr, base));
#endif // #ifdef _MSC_VER
	}

	static type StrToType(const char *start_ptr, char **end_ptr)
	{
		return(StrToTypeBased(start_ptr, end_ptr, 10));
	}

	static const char *GetTypeName()
	{
		return("signed long long");
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
	Implementation for unsigned integral types...
*/
template <> struct NumericTypeSpec<true, false> {
	typedef unsigned long long type;

	static type StrToTypeBased(const char *start_ptr, char **end_ptr, int base)
	{
#ifdef _MSC_VER
		return(::_strtoui64(start_ptr, end_ptr, base));
#else
		return(::strtoull(start_ptr, end_ptr, base));
#endif // #ifdef _MSC_VER
	}

	static type StrToType(const char *start_ptr, char **end_ptr)
	{
		return(StrToTypeBased(start_ptr, end_ptr, 10));
	}

	static const char *GetTypeName()
	{
		return("unsigned long long");
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType> struct WidestType {
	typedef typename NumericTypeSpec<
		std::numeric_limits<DatumType>::is_integer,
		std::numeric_limits<DatumType>::is_signed>::type type;

	static type StrToTypeBased(const char *start_ptr, char **end_ptr, int base)
	{
		return(NumericTypeSpec<
			std::numeric_limits<DatumType>::is_integer,
			std::numeric_limits<DatumType>::is_signed>::StrToTypeBased(
			start_ptr, end_ptr, base));
	}

	static type StrToType(const char *start_ptr, char **end_ptr)
	{
		return(NumericTypeSpec<
			std::numeric_limits<DatumType>::is_integer,
			std::numeric_limits<DatumType>::is_signed>::StrToType(
			start_ptr, end_ptr));
	}

	static const char *GetTypeName()
	{
		return(NumericTypeSpec<
			std::numeric_limits<DatumType>::is_integer,
			std::numeric_limits<DatumType>::is_signed>::GetTypeName());
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ParseType>
	inline bool ParseNumericString(const char *in_value, ParseType &out_datum,
		bool throw_on_failure = false,
		ParseType min_value = NumericBounds<ParseType>::min(),
		ParseType max_value = NumericBounds<ParseType>::max())
{
	typedef typename WidestType<ParseType>::type WideType;

	char     *end_ptr;
	WideType  tmp_value = WidestType<ParseType>::StrToType(in_value, &end_ptr);

	if (end_ptr != (in_value + ::strlen(in_value))) {
		if (!throw_on_failure)
			return(false);
		std::ostringstream error_text;
		error_text << "Source string value '" << in_value << "' contains at "
			"least one invalid character (ASCII " <<
			static_cast<unsigned int>(static_cast<unsigned char>(*end_ptr)) <<
			").";
		ThrowInvalidArgument(error_text);
	}

	if ((tmp_value < static_cast<WideType>(min_value)) ||
		(tmp_value > static_cast<WideType>(max_value))) {
		if (!throw_on_failure)
			return(false);
		std::ostringstream error_text;
		error_text << "Source string value '" << in_value << "' is outside of "
			"the permissible range (from " << min_value << " to " << max_value <<
			", inclusive).";
		ThrowInvalidArgument(error_text);
	}

	out_datum = static_cast<ParseType>(tmp_value);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ParseType>
	inline bool ParseNumericString(const std::string &param_value,
		ParseType &out_datum, bool throw_on_failure = false,
		ParseType min_value = NumericBounds<ParseType>::min(),
		ParseType max_value = NumericBounds<ParseType>::max())
{
	return(ParseNumericString(param_value.c_str(), out_datum, throw_on_failure,
		min_value, max_value));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ParseType>
	inline bool IsNumericString(const char *param_value)
{
	ParseType tmp_datum;

	return(ParseNumericString(param_value, tmp_datum, false));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ParseType>
	inline bool IsNumericString(const std::string &param_value)
{
	return(IsNumericString<ParseType>(param_value.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ParseType>
	inline ParseType CheckIsNumericString(const char *param_value,
		ParseType min_value = NumericBounds<ParseType>::min(),
		ParseType max_value = NumericBounds<ParseType>::max())
{
	ParseType out_datum;

	ParseNumericString(param_value, out_datum, true, min_value, max_value);

	return(out_datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ParseType>
	inline ParseType CheckIsNumericString(const std::string &param_value,
		ParseType min_value = NumericBounds<ParseType>::min(),
		ParseType max_value = NumericBounds<ParseType>::max())
{
	return(CheckIsNumericString(param_value.c_str(), min_value, max_value));
}
// ////////////////////////////////////////////////////////////////////////////

#ifdef PARSE_NUMERIC_STRING_TEST

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	void TEST_WidestTypeInfoSingle(const char *in_type_name)
{
	std::cerr << std::left << std::setw(18) << in_type_name << std::right <<
		" -> " << std::setw(5) <<
		sizeof(MLB::Utility::WidestType<DatumType>::type) << " -> " <<
		MLB::Utility::WidestType<DatumType>::GetTypeName() << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_WidestTypeInfo()
{
	TEST_WidestTypeInfoSingle<signed char>("signed char");
	TEST_WidestTypeInfoSingle<unsigned char>("unsigned char");
	TEST_WidestTypeInfoSingle<signed short>("signed short");
	TEST_WidestTypeInfoSingle<unsigned short>("unsigned short");
	TEST_WidestTypeInfoSingle<signed int>("signed int");
	TEST_WidestTypeInfoSingle<unsigned int>("unsigned int");
	TEST_WidestTypeInfoSingle<signed long>("signed long");
	TEST_WidestTypeInfoSingle<unsigned long>("unsigned long");
	TEST_WidestTypeInfoSingle<signed long long>("signed long long");
	TEST_WidestTypeInfoSingle<unsigned long long>("unsigned long long");
	TEST_WidestTypeInfoSingle<float>("float");
	TEST_WidestTypeInfoSingle<double>("double");
	TEST_WidestTypeInfoSingle<long double>("long double");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	inline void TEST_ParseTypeString(const char *in_type_name,
		const char *in_value)
{
	try {
		DatumType out_datum;
		std::cerr << std::left << std::setw(18) << in_type_name << " -> " <<
			std::setw(22) << in_value << std::right << ": " << std::flush;
		MLB::Utility::ParseNumericString(in_value, out_datum, true);
		std::cerr << std::setw(22) <<
			static_cast<MLB::Utility::WidestType<DatumType>::type>(out_datum) <<
			std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "FAILED: " << except.what() << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	inline void TEST_ParseTypeValue(const char *in_type_name, DatumType in_value)
{
	std::ostringstream o_str;

	o_str <<
		static_cast<MLB::Utility::WidestType<DatumType>::type>(in_value);

	TEST_ParseTypeString<DatumType>(in_type_name, o_str.str().c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline void TEST_ParseType()
{
	TEST_ParseTypeValue<signed char>("signed char",               std::numeric_limits<signed char>::min());
	TEST_ParseTypeValue<signed char>("signed char",               std::numeric_limits<signed char>::max());
	TEST_ParseTypeValue<unsigned char>("unsigned char",           std::numeric_limits<unsigned char>::min());
	TEST_ParseTypeValue<unsigned char>("unsigned char",           std::numeric_limits<unsigned char>::max());
	TEST_ParseTypeValue<signed short>("signed short",             std::numeric_limits<signed short>::min());
	TEST_ParseTypeValue<signed short>("signed short",             std::numeric_limits<signed short>::max());
	TEST_ParseTypeValue<unsigned short>("unsigned short",         std::numeric_limits<unsigned short>::min());
	TEST_ParseTypeValue<unsigned short>("unsigned short",         std::numeric_limits<unsigned short>::max());
	TEST_ParseTypeValue<signed int>("signed int",                 std::numeric_limits<signed int>::min());
	TEST_ParseTypeValue<signed int>("signed int",                 std::numeric_limits<signed int>::max());
	TEST_ParseTypeValue<unsigned int>("unsigned int",             std::numeric_limits<unsigned int>::min());
	TEST_ParseTypeValue<unsigned int>("unsigned int",             std::numeric_limits<unsigned int>::max());
	TEST_ParseTypeValue<signed long>("signed long",               std::numeric_limits<signed long>::min());
	TEST_ParseTypeValue<signed long>("signed long",               std::numeric_limits<signed long>::max());
	TEST_ParseTypeValue<unsigned long>("unsigned long",           std::numeric_limits<unsigned long>::min());
	TEST_ParseTypeValue<unsigned long>("unsigned long",           std::numeric_limits<unsigned long>::max());
	TEST_ParseTypeValue<signed long long>("signed long long",     std::numeric_limits<signed long long>::min());
	TEST_ParseTypeValue<signed long long>("signed long long",     std::numeric_limits<signed long long>::max());
	TEST_ParseTypeValue<unsigned long long>("unsigned long long", std::numeric_limits<unsigned long long>::min());
	TEST_ParseTypeValue<unsigned long long>("unsigned long long", std::numeric_limits<unsigned long long>::max());
	TEST_ParseTypeValue<float>("float",                           std::numeric_limits<float>::min());
	TEST_ParseTypeValue<float>("float",                           std::numeric_limits<float>::max());
	TEST_ParseTypeValue<double>("double",                         std::numeric_limits<double>::min());
	TEST_ParseTypeValue<double>("double",                         std::numeric_limits<double>::max());
	TEST_ParseTypeValue<long double>("long double",               std::numeric_limits<long double>::min());
	TEST_ParseTypeValue<long double>("long double",               std::numeric_limits<long double>::max());
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef PARSE_NUMERIC_STRING_TEST

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ParseNumericString_hpp__HH

