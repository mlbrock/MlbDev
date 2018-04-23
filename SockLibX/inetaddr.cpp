/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Handles basic Internet address conversion.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2018.
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

#ifdef __linux__
# include <arpa/inet.h>
#endif // #ifdef __linux__

/* *********************************************************************** */

/* *********************************************************************** */
unsigned long NPSL_INetToAddr(const char *in_inet)
{
	unsigned long tmp_addr;

	return((NPSL_INetToAddrBasic(in_inet, &tmp_addr, NULL) == NPSL_SUCCESS) ?
		tmp_addr : ((unsigned long) NPSL_INADDR_NONE));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_INetToAddrBasic(const char *in_inet, unsigned long *out_addr,
	char *error_text)
{
	int           return_code = NPSL_SUCCESS;
	unsigned long tmp_addr    = ((unsigned long) NPSL_INADDR_NONE);

	if (in_inet == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'in_inet' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!(*in_inet)) {
		if (error_text != NULL)
			strcpy(error_text, "The 'in_inet' parameter is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if (!isdigit(*in_inet)) {
		if (error_text != NULL)
			sprintf(error_text, "%s ('%c') %s.",
				"The first character of the 'in_inet' parameter", *in_inet,
				"is not a digit");
		return_code = NPSL_FAILURE;
	}
	else if ((tmp_addr = inet_addr(in_inet)) == NPSL_INADDR_NONE) {
		if (error_text != NULL)
			strcat(MLB::Utility::nstrcat(strcpy(error_text,
				"Invalid Internet protocol dotted address encountered ('"),
				in_inet, NPSL_MAX_ERROR_TEXT - 100), "').");
		return_code = NPSL_FAILURE;
	}
	else if (out_addr != NULL)
		*out_addr = tmp_addr;

	return(return_code);
}
/* *********************************************************************** */

