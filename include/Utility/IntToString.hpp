// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for formatting integral values as strings
								in radices from 2 to 64.

	Revision History	:	1995-10-10 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1995 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__IntToString_hpp__HH

#define HH__MLB__Utility__IntToString_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file 	IntToString.hpp

	\brief	Logic to support formatting of integral values as strings in
				radices from 2 to 64.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/ValueTraits.hpp>
#include <Utility/Utility_Exception.hpp>

#include <limits>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
	/*	*****************************************************************
		*****************************************************************
		Minimum and maximum length manifest constants for the 'IntToString()'
		family of functions . . .
		*****************************************************************	*/
#define IntToString_MinLen_Unsigned	\
	((sizeof(unsigned long) == 4) ? 6 :	\
	 ((sizeof(unsigned long) == 8) ? 11 : 22))
#define IntToString_MinLen_Signed		(1 + IntToString_MinLen_Unsigned)
#define IntToString_MinLen				IntToString_MinLen_Signed
#define IntToString_MaxLen_Unsigned	(sizeof(unsigned long) * CHAR_BIT)
#define IntToString_MaxLen_Signed		(1 + IntToString_MaxLen_Unsigned)
#define IntToString_MaxLen				IntToString_MaxLen_Signed
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Some other general maximums . . .
		*****************************************************************	*/
	//	Max lengths radix 2
#define IntToString_MaxLen_Unsigned_02	IntToString_MaxLen_Unsigned
#define IntToString_MaxLen_Signed_02		(IntToString_MaxLen_Unsigned_02 + 1)

	//	Max lengths radix 4
#define IntToString_MaxLen_Unsigned_04	\
	((sizeof(unsigned long) == 4) ? 16 :	\
	 ((sizeof(unsigned long) == 8) ? 32 : 64))
#define IntToString_MaxLen_Signed_04		(IntToString_MaxLen_Unsigned_04 + 1)

	//	Max lengths radix 8 (octal)
#define IntToString_MaxLen_Unsigned_08	\
	((sizeof(unsigned long) == 4) ? 11 :	\
	 ((sizeof(unsigned long) == 8) ? 22 : 44))
#define IntToString_MaxLen_Signed_08		(IntToString_MaxLen_Unsigned_08 + 1)

	//	Max lengths radix 10 (decimal)
#define IntToString_MaxLen_Unsigned_10	\
	((sizeof(unsigned long) == 4) ? 10 :	\
	 ((sizeof(unsigned long) == 8) ? 20 : 40))
#define IntToString_MaxLen_Signed_10		(IntToString_MaxLen_Unsigned_10 + 1)

	//	Max lengths radix 16 (hexadecimal)
#define IntToString_MaxLen_Unsigned_16	\
	((sizeof(unsigned long) == 4) ? 8 :	\
	 ((sizeof(unsigned long) == 8) ? 16 : 32))
#define IntToString_MaxLen_Signed_16		(IntToString_MaxLen_Unsigned_16 + 1)

	//	Max lengths radix 36
#define IntToString_MaxLen_Unsigned_36	\
	((sizeof(unsigned long) == 4) ? 7 :	\
	 ((sizeof(unsigned long) == 8) ? 13 : 25))
#define IntToString_MaxLen_Signed_36		(IntToString_MaxLen_Unsigned_36 + 1)

	//	Max lengths radix 64 (the maximum supported).
#define IntToString_MaxLen_Unsigned_64	IntToString_MinLen_Unsigned
#define IntToString_MaxLen_Signed_64		(IntToString_MaxLen_Unsigned_64 + 1)
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
extern unsigned int  IntToString_RadixCharCount;
extern const char   *IntToString_RadixCharList;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY char *IntToStringBasic(char *out_string, unsigned long in_value,
	int in_sign = 0, unsigned int in_radix = 10, const char *in_fill_char = NULL,
	unsigned int radix_char_count = 0, const char *radix_char_list = NULL);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename IntType> char *IntToString(char *out_string,
	IntType in_value, unsigned int in_radix = 10,
	const char *in_fill_char = NULL, unsigned int radix_char_count = 0,
	const char *radix_char_list = NULL)
{
	bool is_neg = IsValueNegative(in_value);

	if ((IntToStringBasic(out_string,
		static_cast<unsigned long>((is_neg) ? (0 - in_value) : in_value),
		(is_neg) ? -1 : 0, in_radix, in_fill_char, radix_char_count,
		radix_char_list)) == NULL) {
		std::ostringstream error_text;
		error_text << "IntToStringBasic() failed --- radix = " << in_radix << ".";
		ThrowInvalidArgument(error_text.str().c_str());
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename IntType> std::string IntToString(IntType in_value,
	unsigned int in_radix = 10, const char *in_fill_char = NULL,
	unsigned int radix_char_count = 0, const char *radix_char_list = NULL)
{
	char buffer[IntToString_MaxLen + 1];

	return(IntToString(buffer, in_value, in_radix, in_fill_char,
		radix_char_count, radix_char_list));
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename UIntType>
	inline char *UIntToString(char *out_string, UIntType in_value,
	unsigned int in_radix = 10, const char *in_fill_char = NULL,
	unsigned int radix_char_count = IntToString_RadixCharCount,
	const char *radix_char_list = IntToString_RadixCharList)
{
	UIntType  power_value;
	UIntType  old_power_value;
	char     *tmp_ptr_1;
	char     *tmp_ptr_2;
	char      buffer[(sizeof(UIntType) * CHAR_BIT) + 1 + 1];

	if (!radix_char_count) {
		radix_char_count = IntToString_RadixCharCount;
		radix_char_list  = IntToString_RadixCharList;
	}
	else if ((radix_char_count <= IntToString_RadixCharCount) &&
		(radix_char_list == NULL))
		radix_char_list = IntToString_RadixCharList;

	power_value     = static_cast<UIntType>(in_radix);
	old_power_value = 0;
	tmp_ptr_1       = out_string;
	tmp_ptr_2       = buffer + (sizeof(UIntType) * CHAR_BIT) + 1;

	if ((in_radix < 2) || (in_radix > radix_char_count))
		out_string = NULL;
	else {
		*tmp_ptr_2-- = '\0';
		*tmp_ptr_2-- = radix_char_list[((UIntType) (in_value %
			((UIntType) in_radix)))];
		in_fill_char = ((in_fill_char != NULL) && (!(*in_fill_char))) ?
			NULL : in_fill_char;
		while (power_value > old_power_value) {
			if (in_value >= power_value)
				*tmp_ptr_2-- =
					radix_char_list[((UIntType) ((in_value /
						power_value) % ((UIntType) in_radix)))];
			else if (in_fill_char != NULL)
				*tmp_ptr_2-- = *in_fill_char;
			else
				break;
			if ((std::numeric_limits<UIntType>::max() / power_value) <
				static_cast<UIntType>(in_radix))
         	break;
			old_power_value  = power_value;
			power_value     *= static_cast<UIntType>(in_radix);
		}
		tmp_ptr_2++;
		do { *tmp_ptr_1++ = *tmp_ptr_2; } while (*tmp_ptr_2++);
	}

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__IntToString_hpp__HH

