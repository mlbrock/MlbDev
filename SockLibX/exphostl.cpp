/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Expands a list of host names to a list of host names
								including any aliases.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/*    Include necessary header files . . .                                 */
/* *********************************************************************** */
 
#include <mbcompat.h>

#include <stdio.h>
#include <string.h>

#ifndef __MSDOS__
# ifndef _Windows
#  include <netdb.h>
#  include <sys/types.h>
#  include <sys/socket.h>
# endif /* # ifndef _Windows */
#endif /* #ifndef __MSDOS__ */ 

#include <Utility/C_StringSupport.hpp>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  NPSL_ExpandHostNameList

   SYNOPSIS    :  return_code = NPSL_ExpandHostNameList(in_host_count,
							in_host_list, out_host_count, out_host_list, error_text);

						unsigned int    in_host_count;

						char          **in_host_list;

						unsigned int   *out_host_count;

						char         ***out_host_list;

						char           *error_text;

   DESCRIPTION :  Expands the list of ``in_host_count`` host names listed in
						the array ``in_host_list`` to ``out_host_count`` host names
						and aliases in the array ``out_host_list``. Alises for
						the host names are procured by way of a call to the
						function ``gethostbyname``.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``in_host_count`` is the number of number of host names
						listed in the array ``in_host_list``.

						(.) ``in_host_list`` is a pointer to an array of char which
						contains the host names.

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

   SEE ALSO    :  NPSL_ExpandHostNameString

   EXAMPLES    :  

   AUTHOR      :	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2015 Michael L. Brock

	OUTPUT INDEX:	NPSL_ExpandHostNameList
						Host Name Functions:NPSL_ExpandHostNameList
						NPSL:Functions:NPSL_ExpandHostNameList
						NPSL:Host Name Functions:NPSL_ExpandHostNameList
						Network Programming Support Functions:See NPSL

	PUBLISH NAME:	NPSL_ExpandHostNameList

	PUBLISH XREF:	NPSL_ExpandHostNameList

	ENTRY CLASS	:	Host Name Functions

EOH */
/* *********************************************************************** */
int NPSL_ExpandHostNameList(unsigned int in_host_count, char **in_host_list,
	unsigned int *out_host_count, char ***out_host_list, char *error_text)
{
	int            return_code = NPSL_SUCCESS;
	unsigned int   count_1;

	*out_host_count = 0;
	*out_host_list  = NULL;

	try {
		MLB::Utility::StringSet tmp_set;
		for (count_1 = 0; count_1 < in_host_count; count_1++) {
			NPSL_HOSTENT hent_data;
			char         hent_buffer[2048];
			char         tmp_error_text[NPSL_MAX_ERROR_TEXT];
			if ((return_code = NPSL_GetHostByString(in_host_list[count_1],
				&hent_data, hent_buffer, sizeof(hent_buffer), tmp_error_text)) !=
				NPSL_SUCCESS) {
				sprintf(error_text,
					"Unable to get the host information for '%-.300s': %s",
					in_host_list[count_1], tmp_error_text);
				break;
			}
			tmp_set.insert(hent_data.h_name);
			char **this_ptr = hent_data.h_aliases;
			while (*this_ptr != NULL) {
				tmp_set.insert(*this_ptr);
				this_ptr++;
			}
		}
		if (return_code == NPSL_SUCCESS)
			MLB::Utility::StringContainerToCharPtrArray(tmp_set,
				*out_host_count, *out_host_list, "NPSL_ExpandHostNameList");
	}
	catch (const std::exception &except) {
		MLB::NPSLI_Internal::NPSLI_GetErrorFromException(except, error_text);
		return_code = NPSL_FAILURE;
	}

	return(return_code);
}
/* *********************************************************************** */

#ifdef TEST_MAIN

COMPAT_FN_DECL(int main, (int argc, char **argv));

int main(int argc, char **argv)
{
	int            return_code = NPSL_FAILURE;
	unsigned int   count_1;
	unsigned int   count_2;
	unsigned int   host_count  = 0;
	char         **host_list   = NULL;
	char           error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test routine for 'NPSL_ExpandHostNameList()'\n");
	fprintf(stderr, "---- ------- --- ---------------------------\n\n");

	if (argc == 1)
		fprintf(stderr, "USAGE: %s <host-name> [<host-name> . . .]\n", argv[0]);
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			printf("HOSTNAME: %s ---> ", argv[count_1]);
			if ((return_code = NPSL_ExpandHostNameList(1, argv + count_1,
				&host_count, &host_list, error_text)) != NPSL_SUCCESS) {
				fprintf(stderr, "Error on call to '%s': %s\n",
					"NPSL_ExpandHostNameList()", error_text);
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

