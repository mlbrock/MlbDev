// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for miscellaneous numeric type support.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__NumericSupport_hpp__HH

#define HH__MLB__Utility__NumericSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file		NumericSupport.hpp

	\brief	Miscellaneous numeric type support.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <limits>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
//	Default implementation for integral types...
template <typename DataType, bool> struct NumericBoundsHelper {
	static DataType min() { return(std::numeric_limits<DataType>::min()); }
	static DataType max() { return(std::numeric_limits<DataType>::max()); }
};
//	----------------------------------------------------------------------------
//	Implementation for floating-point types...
template <typename DataType> struct NumericBoundsHelper<DataType, false> {
	static DataType min() { return(-std::numeric_limits<DataType>::max()); }
	static DataType max() { return( std::numeric_limits<DataType>::max()); }
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> struct NumericBounds :
	public NumericBoundsHelper<DataType, true> {
};
//	----------------------------------------------------------------------------
template <> struct NumericBounds<float> :
	public NumericBoundsHelper<float, false> {
};
//	----------------------------------------------------------------------------
template <> struct NumericBounds<double> :
	public NumericBoundsHelper<double, false> {
};
//	----------------------------------------------------------------------------
template <> struct NumericBounds<long double> :
	public NumericBoundsHelper<long double, false> {
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> struct PrintableType {
	typedef DataType type;
};
//	----------------------------------------------------------------------------
//	g++ requires a separate specialization for char...
template <> struct PrintableType<char> {
	typedef signed int type;
};
//	----------------------------------------------------------------------------
template <> struct PrintableType<signed char> {
	typedef signed int type;
};
//	----------------------------------------------------------------------------
template <> struct PrintableType<unsigned char> {
	typedef unsigned int type;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	inline typename PrintableType<DataType>::type
		GetPrintableType(DataType value)
{
	return(static_cast<typename PrintableType<DataType>::type>(value));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <unsigned long long UIntNumber, unsigned long long UIntPower>
	struct UIntegerPower {
	static const unsigned long long value =
		UIntNumber * UIntegerPower<UIntNumber, UIntPower - 1>::value;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <unsigned long long UIntNumber>
	struct UIntegerPower<UIntNumber, 0> {
	static const unsigned long long value = 1;
};
// ////////////////////////////////////////////////////////////////////////////

#ifdef MLB_UTILITY_NUMERIC_SUPPORT_TEST

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	inline void TEST_NumericBounds(const char *in_type_name,
		DatumType in_value_1, DatumType in_value_2)
{
	std::cout
		<< std::left << std::setw(18) << in_type_name << std::right << ": "
		<< std::setw(23) << GetPrintableType(in_value_1) << " ---> "
		<< std::setw(23) << GetPrintableType(in_value_2) << std::endl
			;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline void TEST_NumericType()
{
	TEST_NumericBounds<signed char>("signed char",               std::numeric_limits<signed char>::min(),        NumericBounds<signed char>::min());
	TEST_NumericBounds<signed char>("signed char",               std::numeric_limits<signed char>::max(),        NumericBounds<signed char>::max());
	TEST_NumericBounds<unsigned char>("unsigned char",           std::numeric_limits<unsigned char>::min(),      NumericBounds<unsigned char>::min());
	TEST_NumericBounds<unsigned char>("unsigned char",           std::numeric_limits<unsigned char>::max(),      NumericBounds<unsigned char>::max());
	TEST_NumericBounds<signed short>("signed short",             std::numeric_limits<signed short>::min(),       NumericBounds<signed short>::min());
	TEST_NumericBounds<signed short>("signed short",             std::numeric_limits<signed short>::max(),       NumericBounds<signed short>::max());
	TEST_NumericBounds<unsigned short>("unsigned short",         std::numeric_limits<unsigned short>::min(),     NumericBounds<unsigned short>::min());
	TEST_NumericBounds<unsigned short>("unsigned short",         std::numeric_limits<unsigned short>::max(),     NumericBounds<unsigned short>::max());
	TEST_NumericBounds<signed int>("signed int",                 std::numeric_limits<signed int>::min(),         NumericBounds<signed int>::min());
	TEST_NumericBounds<signed int>("signed int",                 std::numeric_limits<signed int>::max(),         NumericBounds<signed int>::max());
	TEST_NumericBounds<unsigned int>("unsigned int",             std::numeric_limits<unsigned int>::min(),       NumericBounds<unsigned int>::min());
	TEST_NumericBounds<unsigned int>("unsigned int",             std::numeric_limits<unsigned int>::max(),       NumericBounds<unsigned int>::max());
	TEST_NumericBounds<signed long>("signed long",               std::numeric_limits<signed long>::min(),        NumericBounds<signed long>::min());
	TEST_NumericBounds<signed long>("signed long",               std::numeric_limits<signed long>::max(),        NumericBounds<signed long>::max());
	TEST_NumericBounds<unsigned long>("unsigned long",           std::numeric_limits<unsigned long>::min(),      NumericBounds<unsigned long>::min());
	TEST_NumericBounds<unsigned long>("unsigned long",           std::numeric_limits<unsigned long>::max(),      NumericBounds<unsigned long>::max());
	TEST_NumericBounds<signed long long>("signed long long",     std::numeric_limits<signed long long>::min(),   NumericBounds<signed long long>::min());
	TEST_NumericBounds<signed long long>("signed long long",     std::numeric_limits<signed long long>::max(),   NumericBounds<signed long long>::max());
	TEST_NumericBounds<unsigned long long>("unsigned long long", std::numeric_limits<unsigned long long>::min(), NumericBounds<unsigned long long>::min());
	TEST_NumericBounds<unsigned long long>("unsigned long long", std::numeric_limits<unsigned long long>::max(), NumericBounds<unsigned long long>::max());
	TEST_NumericBounds<float>("float",                           std::numeric_limits<float>::min(),              NumericBounds<float>::min());
	TEST_NumericBounds<float>("float",                           std::numeric_limits<float>::max(),              NumericBounds<float>::max());
	TEST_NumericBounds<double>("double",                         std::numeric_limits<double>::min(),             NumericBounds<double>::min());
	TEST_NumericBounds<double>("double",                         std::numeric_limits<double>::max(),             NumericBounds<double>::max());
	TEST_NumericBounds<long double>("long double",               std::numeric_limits<long double>::min(),        NumericBounds<long double>::min());
	TEST_NumericBounds<long double>("long double",               std::numeric_limits<long double>::max(),        NumericBounds<long double>::max());
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef MLB_UTILITY_NUMERIC_SUPPORT_TEST

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__NumericSupport_hpp__HH

