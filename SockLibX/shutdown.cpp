/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%


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
 
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_ShutDown(NPSL_SOCKET_HANDLE socket_handle, int shut_down_flag,
	char *error_text)
{
	int return_code;

	if (shutdown(socket_handle, shut_down_flag) == NPSL_SOCKET_ERROR) {
		if (error_text != NULL)
			NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
				strcpy(error_text,
				"Attempt to shutdown a socket with 'shutdown()' failed: "));
		return_code = NPSL_SUPP_MapLastError();
	}
	else
		return_code = NPSL_SUCCESS;

	return(return_code);
}
/* *********************************************************************** */

