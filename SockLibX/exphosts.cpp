/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Expands a string containing host names to a list of
								host names including any aliases.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/*    Include necessary header files . . .                                 */
/* *********************************************************************** */
 
#include <stdlib.h>
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  NPSL_ExpandHostNameString

   SYNOPSIS    :  return_code = NPSL_ExpandHostNameString(in_host_string,
							out_host_count, out_host_list, error_text);

						const char     *in_host_string;

						unsigned int   *out_host_count;

						char         ***out_host_list;

						char           *error_text;

   DESCRIPTION :  Expands the host names listed in the string
						``in_host_string`` to ``out_host_count`` host names
						and aliases in the array ``out_host_list``.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``in_host_string`` is a string which contains the host
						names to be expanded separated by white-space or commas.

						(.) ``out_host_count`` is a pointer to an unsigned int into
						which will be placed the number of host names (and aliases)
						allocated by the function.

						(.) ``out_host_list`` is a pointer to a pointer to an array
						of char which will be allocated to store the host names and
						aliases resulting from the operation of the function.

						(.) ``error_text`` points to a string into which this
						function will place text descriptive of any error which
						might occur.

						(-) The string allocated for this purpose should be at least
						''NPSL_MAX_ERROR_TEXT'' characters in length.

   RETURNS     :  Returns from this function are as follow:

						(.) ''NPSL_SUCCESS'' if the function completes without
						error. ''NPSL_SUCCESS'' is guaranteed to be equal to
						zero (0).

						(.) One of the defined non-zero NPSL return codes
						indicative of error as follows:

						(..) ''NPSL_SYSTEM_FAILURE'' indicates that a standard
						library 	function or a system call failed.

						(..) ''NPSL_MEMORY_FAILURE'' indicates that the memory
						required for operation of the function could not be
						allocated.

   NOTES       :  The array ``out_host_list`` is in sorted order and contains
						no duplicates.

						The use of the term ``aliases`` in this description includes
						the primary host name as listed in ''/etc/hosts''.

   CAVEATS     :  

   SEE ALSO    :  NPSL_ExpandHostNameList

   EXAMPLES    :  

   AUTHOR      :	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2016 Michael L. Brock

	OUTPUT INDEX:	NPSL_ExpandHostNameString
						Host Name Functions:NPSL_ExpandHostNameString
						NPSL:Functions:NPSL_ExpandHostNameString
						NPSL:Host Name Functions:NPSL_ExpandHostNameString
						Network Programming Support Functions:See NPSL

	PUBLISH NAME:	NPSL_ExpandHostNameString

	PUBLISH XREF:	NPSL_ExpandHostNameString

	ENTRY CLASS	:	Host Name Functions

EOH */
/* *********************************************************************** */
int NPSL_ExpandHostNameString(const char *in_host_string,
	unsigned int *out_host_count, char ***out_host_list, char *error_text)
{
	int            return_code = NPSL_SUCCESS;
	unsigned int   tmp_count   = 0;
	char         **tmp_list    = NULL;
	char          *tmp_string;
	char          *tmp_ptr;

	*out_host_count = 0;
	*out_host_list  = NULL;

	if ((in_host_string != NULL) && *in_host_string) {
		if ((tmp_string = strdup(in_host_string)) == NULL) {
			strcpy(error_text, "Unable to copy the host name string.");
			return_code = NPSL_MEMORY_FAILURE;
		}
		else {
			while((tmp_ptr = strchr(tmp_string, ',')) != NULL)
				*tmp_ptr = ' ';
			if (MLB::NPSLI_Internal::sepwordl(MLB::NPSLI_Internal::trim(
				MLB::NPSLI_Internal::oneblank(
				MLB::NPSLI_Internal::nocontrl(tmp_string))), &tmp_count, &tmp_list) !=
				MLB::NPSLI_Internal::STRFUNCS_SUCCESS) {
				strcpy(error_text, "Unable to create a temporary host name list.");
				return_code = NPSL_MEMORY_FAILURE;
			}
			else {
				return_code = NPSL_ExpandHostNameList(tmp_count, tmp_list,
					out_host_count, out_host_list, error_text);
				MLB::NPSLI_Internal::strl_remove_all(&tmp_count, &tmp_list);
			}
			free(tmp_string);
		}
	}

	return(return_code);
}
/* *********************************************************************** */

#ifdef TEST_MAIN

#include <stdio.h>

COMPAT_FN_DECL(int main, (int argc, char **argv));

int main(int argc, char **argv)
{
	int            return_code = NPSL_FAILURE;
	unsigned int   count_1;
	unsigned int   count_2;
	unsigned int   host_count  = 0;
	char         **host_list   = NULL;
	char           error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test routine for 'NPSL_ExpandHostNameString()'\n");
	fprintf(stderr, "---- ------- --- ----------------------------\n\n");

	if (argc == 1)
		fprintf(stderr, "USAGE: %s <host-name> [<host-name> . . .]\n", argv[0]);
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			printf("HOSTNAME: %s ---> ", argv[count_1]);
			if ((return_code = NPSL_ExpandHostNameString(argv[count_1],
				&host_count, &host_list, error_text)) != NPSL_SUCCESS) {
				fprintf(stderr, "Error on call to '%s': %s\n",
					"NPSL_ExpandHostNameString()", error_text);
				break;
			}
			printf("%u aliases\n>>> ", host_count);
			for (count_2 = 0; count_2 < host_count; count_2++)
				printf("%s%s", host_list[count_2],
					(count_2 < (host_count - 1)) ? "," : "\n");
			printf("-----------------------------------------------------\n");
			MLB::NPSLI_Internal::strl_remove_all(&host_count, &host_list);
		}
		NPSL_SocketLibCleanUp(NULL, NULL);
	}

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

