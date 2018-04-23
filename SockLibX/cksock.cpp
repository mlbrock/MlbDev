/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Checks a socket handle.

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

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_CheckSocket(NPSL_SOCKET_HANDLE socket_handle, char *error_text)
{
	int return_code;

	if (socket_handle != NPSL_INVALID_SOCKET)
		return_code = NPSL_SUCCESS;
	else {
		if (error_text != NULL)
			sprintf(error_text, "Invalid socket handle (%d).", socket_handle);
		return_code = NPSL_FAILURE;
	}

	return(return_code);
}
/* *********************************************************************** */

