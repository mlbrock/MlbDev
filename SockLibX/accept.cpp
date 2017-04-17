/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Accept a communication on a socket.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2017.
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
int NPSL_Accept(NPSL_SOCKET_HANDLE in_socket_handle,
	NPSL_SOCKET_HANDLE *out_socket_handle, NPSL_SOCKADDR *addr,
	NPSL_SOCKLEN_T *addrlen, char *error_text)
{
	int            return_code;
	NPSL_SOCKADDR  tmp_addr;
	NPSL_SOCKLEN_T tmp_addrlen;

	*out_socket_handle = NPSL_INVALID_SOCKET;

	/*
		Permits the caller to pass NULLs in cases where they're not
		interested in the client socket address --- which is retrievable
		using the NPSL_GetPeerName() interface anyway.

		In general, the 'addr' and 'addrlen' parameters should both be 'NULL'
		or both be non 'NULL'.
	*/
	if ((addr == NULL) && (addrlen != NULL)) {
		if (error_text != NULL)
			strcpy(error_text, "Invalid call to 'NPSL_Accept()': The 'addr' "
				"parameter is 'NULL', but the 'addrlen' parameter is non-"
				"NULL'.");
		return_code = NPSL_SUPP_MapLastError();
	}
	else if ((addr != NULL) && (addrlen == NULL)) {
		if (error_text != NULL)
			strcpy(error_text, "Invalid call to 'NPSL_Accept()': The 'addr' "
				"parameter is not 'NULL', but the 'addrlen' parameter is 'NULL'.");
		return_code = NPSL_SUPP_MapLastError();
	}
	else {
		if ((addr == NULL) && (addrlen == NULL)) {
			addr    = &tmp_addr;
			addrlen = &tmp_addrlen;
		}
		if ((*out_socket_handle = accept(in_socket_handle, addr,
			addrlen)) == NPSL_INVALID_SOCKET) {
			if (error_text != NULL)
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
					strcpy(error_text,
			"Attempt to accept a socket connection with 'accept()' failed: "));
			return_code = NPSL_SUPP_MapLastError();
		}
		else
			return_code = NPSL_SUCCESS;
	}

	return(return_code);
}
/* *********************************************************************** */

