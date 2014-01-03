// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Limited-length string concatenation.

	Revision History	:	1986-10-27 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1986 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/* 	Include necessary header files . . .											*/
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

   NAME        :  nstrcat

   SYNOPSIS    :  char *nstrcat(to_string, from_string, string_length);

						char         *to_string;

						const char   *from_string;

						size_t        string_length;

   DESCRIPTION :  Copies characters from the area to which the ``from_string``
						parameter points to the end of the null-terminated area to
						which the ``to_string`` parameter points until one of the
						follow conditions is met:

						(.) an ASCII 0 is encountered, in which case it is copied
						to the appropriate position in the area to which the
						``to_string`` parameter points and the function returns; or,

						(.) ``string_length`` characters have copied from ``from_string``
						to ``to_string`` in which case a terminating ASCII 0 is
						appended to the appropriate position in the area to which the
						``to_string`` parameter points and the function returns.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``to_string`` points to the area to receive the string.
						This area must have at least ``string_length`` bytes plus
						one byte (for the terminating ASCII 0) allocated to it in
						order to handle the general case (which might include
						``from_string`` strings longer than ``string_length``
						bytes).

						(.) ``from_string`` points to the area from which the
						string is to be copied.

						(.) ``string_length`` is the number of bytes to be copied
						from ``from_string`` to ``to_string``.

   RETURNS     :  A pointer to the destination string (that is,
						``to_string``).

   NOTES       :  

   CAVEATS     :  It is important to remember that ``string_length`` is the
						maximum length of the C string NOT including the ASCII 0
						termination byte.

						For example, if you were to perform the concatenation from
						a string which is 20 characters in length and the
						``string_length`` parameter were set to 10, the first 10
						characters would be concatenated to the ``to_string`` and
						an ASCII 0 placed immediately following those 10
						characters.

   SEE ALSO    :  nstrcpy

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1986 - 2014 Michael L. Brock

   OUTPUT INDEX:  nstrcat
						Miscellaneous Functions:nstrcat
						STRFUNCS:Miscellaneous Functions:nstrcat
						String Functions:See STRFUNCS

   PUBLISH XREF:  nstrcat

   PUBLISH NAME:	nstrcat

	ENTRY CLASS	:	Miscellaneous Functions

EOH */
// ////////////////////////////////////////////////////////////////////////////
#ifndef NARGS
char *nstrcat(char *to_string, const char *from_string,
	unsigned int string_length)
#else
char *nstrcat(to_string, from_string, string_length)
char         *to_string;
const char   *from_string;
unsigned int  string_length;
#endif /* #ifndef NARGS */
{
	char *tmp_ptr = to_string;

	tmp_ptr += ::strlen(to_string);

	while (*from_string && (string_length > 0)) {
		*tmp_ptr++ = *from_string++;
		string_length--;
	}

	*tmp_ptr = '\0';

	return(to_string);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <stdio.h>

using namespace MLB::Utility;

#define TEST_STRING_SIZE	10

COMPAT_FN_DECL(int main, (int argc, char **argv));

// ////////////////////////////////////////////////////////////////////////////
#ifndef NARGS
int main(int argc, char **argv)
#else
int main(argc, argv)
int    argc;
char **argv;
#endif /* #ifndef NARGS */
{
	unsigned int  count_1;
	char         *prog;
	char          buffer[TEST_STRING_SIZE + 1];

	fprintf(stderr, "Test routine for 'nstrcat()'\n");
	fprintf(stderr, "---- ------- --- -----------\n\n");

	fprintf(stderr, ">>> Original = TEST_\n>>> size = %u\n\n",
		TEST_STRING_SIZE);

	if (argc < 2) {
		prog = (strchr(argv[0], '/')) ? strchr(argv[0], '/') + 1: argv[0];
		fprintf(stderr, "%s\n\nUSAGE: %s <string> [<string> . . . ]\n",
			"ERROR: Invalid invocation attempt", prog);
		return(-1);
	}
	else {
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			strcpy(buffer, "TEST_");
			printf("[String %05u] [%s] -> [%s]\n", count_1,
				argv[count_1], nstrcat(buffer, argv[count_1], TEST_STRING_SIZE));
		}
	}

	return(0);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

