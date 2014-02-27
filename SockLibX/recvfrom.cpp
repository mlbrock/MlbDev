/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Receive a datagram message from a socket.

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

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_RecvFrom(NPSL_SOCKET_HANDLE socket_handle, char *buffer,
	unsigned int buffer_length, int flags, NPSL_SOCKADDR *addr,
	NPSL_SOCKLEN_T *addrlen, unsigned int *recv_count, char *error_text)
{
	int                  return_code;
	NPSL_SEND_RECV_LEN_T data_length;

	*recv_count = 0;

	if (buffer_length > ((unsigned int) INT_MAX)) {
		if (error_text != NULL)
			sprintf(error_text, "%s (%u) %s (%d).",
				"The specified buffer length", buffer_length,
				"exceeds the maximum permissible signed integer value", INT_MAX);
		return_code = NPSL_FAILURE;
	}
	else {
		if ((data_length = recvfrom(socket_handle, buffer, ((int) buffer_length),
			flags, addr, addrlen)) != NPSL_SOCKET_ERROR) {
			*recv_count  = ((unsigned int) data_length);
			 return_code = NPSL_SUCCESS;
		}
		else {
			if (error_text != NULL)
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
					strcpy(error_text,
"Attempt to receive a datagram message from a socket with 'recvfrom()' failed: "));
			return_code = NPSL_SUPP_MapLastError();
		}
	}

	return(return_code);
}
/* *********************************************************************** */

