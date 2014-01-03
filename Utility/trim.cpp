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

		Copyright Michael L. Brock 1986 - 2014.
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

   COPYRIGHT   :  Copyright 1986 - 2014 Michael L. Brock

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

	while (*temp_ptr_1) {
		if (!isspace(*((unsigned char *) temp_ptr_1))) {
			while (*temp_ptr_1)
				*in_text++ = *temp_ptr_1++;
			*in_text = '\0';
			return(temp_ptr_2);
		}
		temp_ptr_1++;
	}

	return(temp_ptr_2);
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

   COPYRIGHT   :  Copyright 1986 - 2014 Michael L. Brock

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

   COPYRIGHT   :  Copyright 1986 - 2014 Michael L. Brock

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

