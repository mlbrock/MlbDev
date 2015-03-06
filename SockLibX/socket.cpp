/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Create an endpoint for communication.

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
 
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_Socket(int socket_domain, int socket_type, int socket_protocol,
	NPSL_SOCKET_HANDLE *socket_handle, char *error_text)
{
	int return_code = NPSL_SUCCESS;

	*socket_handle = NPSL_INVALID_SOCKET;

	if ((*socket_handle = socket(socket_domain, socket_type,
		socket_protocol)) == NPSL_INVALID_SOCKET) {
		if (error_text != NULL)
			NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
				strcpy(error_text,
				"Attempt to create a socket with 'socket()' failed: "));
		return_code = NPSL_SUPP_MapLastError();
	}

	return(return_code);
}
/* *********************************************************************** */

