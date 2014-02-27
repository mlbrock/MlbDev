/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Handles basic address conversion.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2014.
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

#ifdef linux
# include <arpa/inet.h>
#endif // #ifdef linux

/* *********************************************************************** */

/* *********************************************************************** */
char *NPSL_AddrToINet(const char *in_addr, char *out_inet)
{
	return((NPSL_AddrToINetBasic(in_addr, out_inet, NULL) == NPSL_SUCCESS) ?
		out_inet : NULL);
}
/* *********************************************************************** */

/* *********************************************************************** */
char *NPSL_AddrStructToINet(const struct in_addr *in_addr, char *out_inet)
{
	return((NPSL_AddrStructToINetBasic(in_addr, out_inet, NULL) ==
		NPSL_SUCCESS) ? out_inet : NULL);
}
/* *********************************************************************** */

/* *********************************************************************** */
char *NPSL_AddrULongToINet(unsigned long in_addr, char *out_inet)
{
	return((NPSL_AddrULongToINetBasic(in_addr, out_inet, NULL) ==
		NPSL_SUCCESS) ? out_inet : NULL);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_AddrToINetBasic(const char *in_addr, char *out_inet, char *error_text)
{
	return(NPSL_AddrStructToINetBasic(((const struct in_addr *)
		((void *) in_addr)), out_inet, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_AddrStructToINetBasic(const struct in_addr *in_addr, char *out_inet,
	char *error_text)
{
	int   return_code = NPSL_SUCCESS;
	char *tmp_inet;

	if (in_addr == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'in_addr' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if ((tmp_inet = inet_ntoa(*((struct in_addr *) in_addr))) == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "Unable to convert Internet protocol address.");
		return_code = NPSL_FAILURE;
	}
	else if (strlen(tmp_inet) > NPSL_MAX_IPV4_ADDR_STR) {
		if (error_text != NULL)
			sprintf(error_text, "%s (%u) exceeds the maximum (%u).",
				"The length of the string form of the Internet protocol address",
				((unsigned int) strlen(tmp_inet)), NPSL_MAX_IPV4_ADDR_STR);
		return_code = NPSL_FAILURE;
	}
	else
		strcpy(out_inet, tmp_inet);

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_AddrULongToINetBasic(unsigned long in_addr, char *out_inet,
	char *error_text)
{
	return(NPSL_AddrStructToINetBasic(((const struct in_addr *) &in_addr),
		out_inet, error_text));
}
/* *********************************************************************** */

