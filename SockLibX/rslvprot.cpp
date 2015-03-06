/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Resolves protocol information.

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
 
#include "npsli.h"

/* *********************************************************************** */

/*	***********************************************************************	*/
int NPSL_ResolveProtoToSint(const char *proto_string, int *out_proto,
	char *error_text)
{
	int           return_code;
	NPSL_PROTOENT pent_data;
	char          buffer[8192];

	if ((return_code = NPSL_GetProtoByString(proto_string, &pent_data, buffer,
		sizeof(buffer), error_text)) == NPSL_SUCCESS) {
		if (out_proto != NULL)
			*out_proto = pent_data.p_proto;
	}

	return(return_code);
}
/*	***********************************************************************	*/

