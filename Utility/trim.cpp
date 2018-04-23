// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Removes white-space from the left, right or both sides
								of a string.

	Revision History	:	1986-10-27 --- Creation
									Michael L. Brock

								Ported from the MLB Strfuncs library source modules
								ltrim.c, rtrim.c and trim.c.

		Copyright Michael L. Brock 1986 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#ifdef __GNUC__
# include <string.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/* BOH

	NAME			:	ltrim

	SYNOPSIS		:	string_ptr = ltrim(in_text);

						char *string_ptr;

						char *in_text;

	DESCRIPTION	:	Removes all leading whitespace from the string to which
						the ``in_text`` parameter points.  That is, the string is
						trimmed from the left. 

						Whitespace is defined as any character for which the
						standard library macro ``isspace`` returns true.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``in_text`` is the string from which whitespace on the
						left is to be removed.

	RETURNS		: 	A pointer to the string used as input. This is, ``in_text``
						is returned.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  ltrim_basic
						ltrim_basic_mem
						ltrimx
						rtrim
						trim

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1986 - 2018 Michael L. Brock

   OUTPUT INDEX:  ltrim
						String Justification Functions:ltrim
						STRFUNCS:String Justification Functions:ltrim
						String Functions:See STRFUNCS

   PUBLISH XREF:  ltrim

   PUBLISH NAME:	ltrim

	ENTRY CLASS	:	String Justification Functions

EOH */
// ////////////////////////////////////////////////////////////////////////////
#ifndef NARGS
char *ltrim(char *in_text)
#else
char *ltrim(in_text)
char *in_text;
#endif /* #ifndef NARGS */
{
	char *temp_ptr_1 = in_text;
	char *temp_ptr_2 = in_text;

	if (isspace(*((unsigned char *) temp_ptr_1++))) {
		while (isspace(*((unsigned char *) temp_ptr_1)))
			++temp_ptr_1;
		do {
			*temp_ptr_2++ = *temp_ptr_1;
		} while (*temp_ptr_1++);
	}

	return(in_text);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/* BOH

	NAME			: 	rtrim

	SYNOPSIS		:	string_ptr = rtrim(in_text);

						char *string_ptr;

						char *in_text;

	DESCRIPTION	:	Removes all trailing whitespace from the string to which
						the ``in_text`` parameter points.  That is, the string is
						trimmed from the right. 

						Whitespace is defined as any character for which the
						standard library macro ``isspace`` returns true.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``in_text`` is the string from which whitespace on the
						right is to be removed.

	RETURNS		: 	A pointer to the string used as input. This is, ``in_text``
						is returned.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  rtrim_basic
						rtrim_basic_mem
						rtrimx
						ltrim
						trim

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1986 - 2018 Michael L. Brock

   OUTPUT INDEX:  rtrim
						String Justification Functions:rtrim
						STRFUNCS:String Justification Functions:rtrim
						String Functions:See STRFUNCS

   PUBLISH XREF:  rtrim

   PUBLISH NAME:	rtrim

	ENTRY CLASS	:	String Justification Functions

EOH */
// ////////////////////////////////////////////////////////////////////////////
#ifndef NARGS
char *rtrim(char *in_text)
#else
char *rtrim(in_text)
char *in_text;
#endif /* #ifndef NARGS */
{
	unsigned int  count;
	char         *temp_ptr;

	count = strlen(in_text);

	if (count) {
		temp_ptr = in_text + count - 1;
		while (count-- && isspace(*((unsigned char *) temp_ptr)))
			*temp_ptr-- = 0;
	}

	return(in_text);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/* BOH

	NAME			:	trim

	SYNOPSIS		:	string_ptr = trim(in_text);

						char *string_ptr;

						char *in_text;

	DESCRIPTION	:	Removes all leading and trailing whitespace from the
						string to which the ``in_text`` parameter points.

						Whitespace is defined as any character for which the
						standard library macro ``isspace`` returns true.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``in_text`` is the string to be trimmed.

	RETURNS		: 	A pointer to the string used as input. This is, ``in_text``
						is returned.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  trim_basic
						trim_basic_mem
						ltrim
						rtrim
						trimx

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1986 - 2018 Michael L. Brock

   OUTPUT INDEX:  trim
						String Justification Functions:trim
						STRFUNCS:String Justification Functions:trim
						String Functions:See STRFUNCS

   PUBLISH XREF:  trim

   PUBLISH NAME:	trim

	ENTRY CLASS	:	String Justification Functions

EOH */
// ////////////////////////////////////////////////////////////////////////////
#ifndef NARGS
char *trim(char *in_text)
#else
char *trim(in_text)
char *in_text;
#endif /* #ifndef NARGS */
{
	return(ltrim(rtrim(in_text)));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <cstdlib>

using namespace MLB::Utility;

namespace {

// ////////////////////////////////////////////////////////////////////////////
struct TEST_Element {
	TEST_Element(int ltrim_delta, int rtrim_delta, const char *str)
		:str_(str)
		,ltrim_len_((ltrim_delta < 0) ? 0 :
			(str_.size() - static_cast<std::size_t>(ltrim_delta)))
		,rtrim_len_((rtrim_delta < 0) ? 0 :
			(str_.size() - static_cast<std::size_t>(rtrim_delta)))
		,trim_len_((ltrim_len_ || rtrim_len_) ? (str_.size() -
			(((ltrim_len_) ? (str_.size() - ltrim_len_) : 0) +
			 ((rtrim_len_) ? (str_.size() - rtrim_len_) : 0))) : 0)
	{
	}

	std::string str_;
	std::size_t ltrim_len_;
	std::size_t rtrim_len_;
	std::size_t trim_len_;
};
//	----------------------------------------------------------------------------
const TEST_Element TEST_TrimList[] = {
	TEST_Element( 0,  0, ""),
	TEST_Element( 0,  0, "no-trim"),
	TEST_Element(-1, -1, "       "),
	TEST_Element(-1, -1, " \t\n\f\v\r "),
	TEST_Element( 1,  0, " ltrim-1"),
	TEST_Element( 2,  0, "  ltrim-2"),
	TEST_Element( 3,  0, "   ltrim-3"),
	TEST_Element( 0,  1, "rtrim-1 "),
	TEST_Element( 0,  2, "rtrim-2  "),
	TEST_Element( 0,  3, "rtrim-3   "),
	TEST_Element( 1,  1, " trim-1 "),
	TEST_Element( 2,  2, "  trim-2  "),
	TEST_Element( 3,  3, "   trim-3   "),
	TEST_Element( 0,  0, "internal space-0"),
	TEST_Element( 1,  1, " internal space-1 "),
	TEST_Element( 1,  1, " internal  space-2 "),
	TEST_Element( 1,  1, " internal   space-3 ")
};
const std::size_t  TEST_TrimCount  = sizeof(TEST_TrimList) /
	sizeof(TEST_TrimList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string TEST_EmitString(const char *in_string, std::size_t padding = 20)
{
	std::size_t        in_length = ::strlen(in_string);
	std::ostringstream o_str;

	o_str << "[";

	while (*in_string) {
		o_str << ((::strchr("\t\v\f\n\r", *in_string)) ? '*' : *in_string);
		++in_string;
	}

	o_str << "]";

	if (in_length < padding)
		o_str <<
			std::setw(static_cast<std::streamsize>(padding - in_length)) << " ";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_OneFunction(int &return_code, const char *func_name,
	char * (*func_ptr)(char *), std::size_t (TEST_Element::*func_len_ptr))
{
	for (std::size_t count_1 = 0; count_1 < TEST_TrimCount; ++count_1) {
		std::string tmp_string(TEST_TrimList[count_1].str_.c_str());
		std::cout << std::left << std::setw(6) << func_name << std::right <<
			": " << TEST_EmitString(tmp_string.c_str()) << " ---> ";
		std::size_t result_len = ::strlen((*func_ptr)(
			const_cast<char *>(tmp_string.c_str())));
		std::cout << TEST_EmitString(tmp_string.c_str()) << ": ";
		if (TEST_TrimList[count_1].*func_len_ptr == result_len)
			std::cout << "OK" << std::endl;
		else {
			std::cout << "FAILED: Chars expected=" <<
				TEST_TrimList[count_1].*func_len_ptr << ", actual=" <<
				result_len << std::endl;
			return_code = EXIT_FAILURE;
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#define TEST_MAIN_RUN_FUNC_BASIC(return_code, func_name, mdata_name)	\
	TEST_OneFunction(return_code, #func_name, func_name,					\
		&TEST_Element:: mdata_name)
#define TEST_MAIN_RUN_FUNC(return_code, func_name)							\
	TEST_MAIN_RUN_FUNC_BASIC(return_code, func_name, func_name##_len_)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_AllFunctions(int &return_code)
{
	TEST_MAIN_RUN_FUNC(return_code, ltrim);
	TEST_MAIN_RUN_FUNC(return_code, rtrim);
	TEST_MAIN_RUN_FUNC(return_code,  trim);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_AllFunctions(return_code);
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
	}

	if (return_code != EXIT_SUCCESS)
		std::cout << "One or more tests failed." << std::endl;

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

