/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Get the name of the peer socket.

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
int NPSL_GetPeerName(NPSL_SOCKET_HANDLE socket_handle, NPSL_SOCKADDR *name,
	NPSL_SOCKLEN_T *addrlen, char *error_text)
{
	int return_code;

	if (getpeername(socket_handle, name, addrlen) != NPSL_SOCKET_OK) {
		if (error_text != NULL)
			NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
				strcpy(error_text,
"Attempt to get the name of the peer socket with 'getpeername()' failed: "));
		return_code = NPSL_SUPP_MapLastError();
	}
	else
		return_code = NPSL_SUCCESS;

	return(return_code);
}
/* *********************************************************************** */

