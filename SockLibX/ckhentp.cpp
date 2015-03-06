/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Checks the parameters used in the manipulation of a
								'NPSL_HOSTENT' ('struct hostent') structure.

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
 
#include <stdio.h>
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_CheckHostEntParams(const NPSL_HOSTENT *host_ent_ptr,
	const void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	char *error_text)
{
	int return_code = NPSL_SUCCESS;

	if (host_ent_ptr == NULL) {
		if (error_text != NULL)
			strcpy(error_text,
				"The destination 'NPSL_HOSTENT' pointer is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (host_ent_buffer_ptr == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'NPSL_HOSTENT' buffer pointer is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (host_ent_buffer_length < sizeof(NPSL_HOSTENT)) {
		if (error_text != NULL)
			sprintf(error_text, "%s (%u) is less than the minimum (%u).",
				"The 'NPSL_HOSTENT' buffer length", host_ent_buffer_length,
				((unsigned int) sizeof(NPSL_HOSTENT)));
		return_code = NPSL_FAILURE;
	}

	return(return_code);
}
/* *********************************************************************** */

