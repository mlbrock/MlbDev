//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Converts an integral value to a string in the specified
								radix.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/IntToString.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
unsigned int  IntToString_RadixCharCount = 64;
const char   *IntToString_RadixCharList  =
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@#";
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
/* BOH

	NAME			:	IntToStringBasic

	SYNOPSIS		:	return_ptr = IntToStringBasic(char *out_string,
							unsigned long in_value, int in_sign = 0,
							unsigned int in_radix = 10,
							const char *in_fill_char = NULL,
							unsigned int radix_char_count = IntToString_RadixCharCount,
							const char *radix_char_list = IntToString_RadixCharList)

						char          *return_ptr;

						char          *out_string;

						unsigned long  in_value;

						int            in_sign;

						unsigned int   in_radix;

						const char    *in_fill_char;

						unsigned int   radix_char_count;

						const char    *radix_char_list;

	DESCRIPTION	:	Converts an ``unsigned long`` into a string using the
						specified radix.

						The resulting string is placed into the buffer to which the
						``out_string`` parameter points.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``out_string`` points to the area into which the
						resulting string will be placed. Its minimum length to
						support an arbitrary ``in_radix`` and ``in_sign`` should be:

						(--) 1 + (sizeof(unsigned long) * CHAR_BIT) + 1

						(-) This permits the storage of the longest string returned
						by this function, including ASCII NULL-terminator.

						(.) ``in_value`` is the value to be converted. It is
						always passed as an ``unsigned long``, even for negative
						values.

						(.) ``in_sign`` is the sign to be used in the resulting
						conversion. If less than zero, then a minus sign (''-'')
						will be prepended to the resulting string.

						(.) ``in_radix`` is the radix to be used for the resulting
						string. It must be in the range of ''2'' through the value
						if the ``radix_char_count`` parameter, inclusive.

						(.) ``in_fill_char`` points to the character to be used to
						left-pad the resulting string. If this parameter is ''NULL''
						or points to a zero-length string, the string will not be
						padded.

						(.) ``radix_char_count`` is the number of characters in the
						array ``radix_char_list``. It represents the maximum radix
						which can be converted by this function.

						(-) If ``radix_char_count`` is ''0'', the value ''64'' will
						be used by this function.

						(.) ``radix_char_list`` is the list of characters to be
						used in the conversion.

						(-) If the ``radix_char_count`` parameter is ''0'', this
						function will use the following string:

{|-|}"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@#"

						(-) If the ``radix_char_count`` parameter is less than or
						equal to ''64'' and ``radix_char_list`` is ''NULL'',
						``radix_char_list`` will be set to the string above.

   RETURNS     :  Returns from this function are as follow:

						(.) If no error occurs, a pointer to the string
						``out_string`` is returned.

						(.) If an error occurs, a ''NULL'' pointer is returned.

	NOTES			:	The only error which can occur in the operation of this
						function is the passing of an invalid ``in_radix``
						parameter.

	CAVEATS		:	

	SEE ALSO		:	val2str
						IntToStringBasic_cat

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2014 Michael L. Brock

	OUTPUT INDEX:	IntToStringBasic
						Miscellaneous Functions:IntToStringBasic
						STRFUNCS:Miscellaneous Functions:IntToStringBasic
						String Functions:See STRFUNCS

	PUBLISH XREF:	IntToStringBasic

	PUBLISH NAME:	IntToStringBasic

	ENTRY CLASS	:	Miscellaneous Functions

EOH */
//	////////////////////////////////////////////////////////////////////////////
char *IntToStringBasic(char *out_string, unsigned long in_value, int in_sign,
	unsigned int in_radix, const char *in_fill_char,
	unsigned int radix_char_count, const char *radix_char_list)
{
	unsigned long  power_value;
	unsigned long  old_power_value;
	char          *tmp_ptr_1;
	char          *tmp_ptr_2;
	char           buffer[IntToString_MaxLen + 1];

	if (!radix_char_count) {
		radix_char_count = IntToString_RadixCharCount;
		radix_char_list  = IntToString_RadixCharList;
	}
	else if ((radix_char_count <= IntToString_RadixCharCount) &&
		(radix_char_list == NULL))
		radix_char_list = IntToString_RadixCharList;

	power_value     = in_radix;
	old_power_value = 0;
	tmp_ptr_1       = out_string;
	tmp_ptr_2       = buffer + (sizeof(unsigned long) * CHAR_BIT) + 1;

	if ((in_radix < 2) || (in_radix > radix_char_count))
		out_string = NULL;
	else {
		*tmp_ptr_2-- = '\0';
		*tmp_ptr_2-- = radix_char_list[((unsigned int) (in_value %
			((unsigned long) in_radix)))];
		in_fill_char = ((in_fill_char != NULL) && (!(*in_fill_char))) ?
			NULL : in_fill_char;
		while (power_value > old_power_value) {
			if (in_value >= power_value)
				*tmp_ptr_2-- =
					radix_char_list[((unsigned int) ((in_value /
						power_value) % ((unsigned long) in_radix)))];
			else if (in_fill_char != NULL)
				*tmp_ptr_2-- = *in_fill_char;
			else
				break;
			if ((ULONG_MAX / power_value) < in_radix)
         	break;
			old_power_value  = power_value;
			power_value     *= in_radix;
		}
		if (in_sign < 0)
			*tmp_ptr_2-- = '-';
		tmp_ptr_2++;
		do { *tmp_ptr_1++ = *tmp_ptr_2; } while (*tmp_ptr_2++);
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB


#ifdef TEST_MAIN

#include <Utility/HRTimer.hpp>
#include <Utility/StringSupport.hpp>

#include <mbtypes.h>

#include <iomanip>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
template <typename UIntType>
	void TEST_EmitRadixLength(std::ostream &o_str, unsigned int radix)
{
	char buffer[(sizeof(Native_UInt64) * CHAR_BIT) + 1 + 1];

	o_str << " " << std::setw(7) <<
		::strlen(UIntToString(buffer, std::numeric_limits<UIntType>::max(),
		radix))
		;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_EmitRadixLengthInfo()
{
	std::cout << "===== ======= ======= ======= =======" << std::endl;
	std::cout << "Radix UInt  8 UInt 16 UInt 32 UInt 64" << std::endl;
	std::cout << "----- ------- ------- ------- -------" << std::endl;

	for (unsigned int count_1 = 2; count_1 <=64; ++count_1) {
		std::cout << std::right << std::setw(2) << count_1 << "   ";
		TEST_EmitRadixLength<Native_UInt8>(std::cout, count_1);
		TEST_EmitRadixLength<Native_UInt16>(std::cout, count_1);
		TEST_EmitRadixLength<Native_UInt32>(std::cout, count_1);
		TEST_EmitRadixLength<Native_UInt64>(std::cout, count_1);
		std::cout << std::endl;
	}

	std::cout << "===== ======= ======= ======= =======" << std::endl <<
		std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_UIntToStringStressTest()
{
	std::cout << PadRight("", 79, '=') << std::endl;
	std::cout << "TEST_UIntToStringStressTest():" << std::endl;
	std::cout << PadRight("", 79, '-') << std::endl;

	Native_UInt32 test_value = 666;
	HRTimer       hr_timer;
	char          buffer[(sizeof(Native_UInt64) * CHAR_BIT) + 1 + 1];

	hr_timer.Start();

	for (int count_1 = 0; count_1 < 1000000; ++count_1)
		UIntToString(buffer, test_value);

	hr_timer.End();

	std::cout << "Elapsed time    = " << std::setprecision(16) <<
		std::setw(20) << hr_timer.GetTickDiffInSeconds() << std::endl;
	std::cout << "Time per string = " <<
		(hr_timer.GetTickDiffInSeconds() /
		static_cast<double>(1000000)) << std::endl;

	std::cout << PadRight("", 79, '=') << std::endl << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename IntType> void TestType(const std::string &type_name,
	IntType &the_value)
{

	the_value = 0;

	std::cout << std::setw(16) << type_name << ": Minimum Value = " <<
		std::right << std::setw(IntToString_MaxLen_Signed_10) <<
		IntToString(std::numeric_limits<IntType>::min(), 10, " ") <<
		std::left << std::endl;
	std::cout << std::setw(16) << type_name << ": Maximum Value = " <<
		std::right << std::setw(IntToString_MaxLen_Signed_10) <<
		IntToString(std::numeric_limits<IntType>::max(), 10, " ") <<
		std::left << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	unsigned int count_1;

	std::cout << "Test routine for function 'IntToString()'" << std::endl;
	std::cout << "---- ------- --- -------- ---------------" << std::endl;

	TEST_EmitRadixLengthInfo();
	TEST_UIntToStringStressTest();

	std::cout <<
		std::setw( 6) << "Radix" << " " <<
		std::setw( 9) << "Value 255" << " " <<
		std::setw(10) << "Length 255" << " " <<
		std::setw(32) << "Value 4294967295" << " " <<
		std::setw(17) << "Length 4294967295" << " " <<
		std::endl;
	std::cout <<
		std::setw( 6) << "Number" << " " <<
		std::setw( 9) << "For Radix" << " " <<
		std::setw(10) << "For Radix" << " " <<
		std::setw(32) << "For Radix" << " " <<
		std::setw(17) << "For Radix" << " " <<
		std::endl;
	std::cout << std::setfill('=') <<
		std::setw( 6) << "" << " " <<
		std::setw( 9) << "" << " " <<
		std::setw(10) << "" << " " <<
		std::setw(32) << "" << " " <<
		std::setw(17) << "" << " " <<
		std::setfill(' ') << std::endl;

	for (count_1 = 2; count_1 <= IntToString_RadixCharCount; count_1++) {
		std::string the_string;
		the_string = IntToString(static_cast<unsigned long>(
			std::numeric_limits<unsigned char>::max()), count_1);
		std::cout <<
			std::right << std::setw(6) << count_1 << " " <<
			std::right << std::setw(9) << the_string << " " <<
			std::setw(10) << static_cast<unsigned int>(the_string.size()) <<
			std::left << " ";

		the_string = IntToString(static_cast<unsigned long>(
			std::numeric_limits<unsigned long>::max()), count_1);
		std::cout <<
			std::right << std::setw(32) << the_string << " " <<
			std::setw(17) << static_cast<unsigned int>(the_string.size()) <<
			std::left << std::endl;
	}

	std::cout << std::setfill('=') <<
		std::setw( 6) << "" << " " <<
		std::setw( 9) << "" << " " <<
		std::setw(10) << "" << " " <<
		std::setw(32) << "" << " " <<
		std::setw(17) << "" << " " <<
		std::setfill(' ') << std::endl;

	std::cout << std::setfill('=') << std::setw(79) << "" << std::setfill(' ') <<
		std::endl;

	//	Macro to test the individual types...
	#define TheTest(some_type)		\
		{									\
			some_type some_value;	\
			TestType(#some_type, some_value);	\
		}									\

	TheTest(signed char)
	TheTest(unsigned char)
	TheTest(signed short)
	TheTest(unsigned short)
	TheTest(signed int)
	TheTest(unsigned int)
	TheTest(signed long)
	TheTest(unsigned long)

	std::cout << std::setfill('=') << std::setw(79) << "" << std::setfill(' ') <<
		std::endl;

	std::cout << std::endl;

	return(EXIT_SUCCESS);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

