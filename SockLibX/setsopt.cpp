/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Set socket options.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2016.
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
/* *********************************************************************** */
/* Function prototypes for functions defined within this module...			*/
/* *********************************************************************** */
COMPAT_FN_DECL_STATIC(int NPSL_SetSockOpt_BufferSize,
	(NPSL_SOCKET_HANDLE socket_handle, unsigned int buffer_size, int opt_name,
	char *error_text));
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_SetSockOpt(NPSL_SOCKET_HANDLE socket_handle, int opt_level,
	int opt_name, const void *opt_value, int opt_length, char *error_text)
{
	int return_code;

	if (setsockopt(socket_handle, opt_level, opt_name,
		((const NPSL_NATIVE_SocketOptionValueType) opt_value), opt_length) !=
		NPSL_SOCKET_OK) {
		if (error_text != NULL)
			NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
				strcpy(error_text,
"Attempt to set the socket options with 'setsockopt()' failed: "));
		return_code = NPSL_SUPP_MapLastError();
	}
	else
		return_code = NPSL_SUCCESS;

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_SetSockOpt_RcvBuf(NPSL_SOCKET_HANDLE socket_handle,
	unsigned int buffer_size, char *error_text)
{
	return(NPSL_SetSockOpt_BufferSize(socket_handle, buffer_size,
		SO_RCVBUF, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_SetSockOpt_SndBuf(NPSL_SOCKET_HANDLE socket_handle,
	unsigned int buffer_size, char *error_text)
{
	return(NPSL_SetSockOpt_BufferSize(socket_handle, buffer_size,
		SO_SNDBUF, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
static int NPSL_SetSockOpt_BufferSize(NPSL_SOCKET_HANDLE socket_handle,
	unsigned int buffer_size, int opt_name, char *error_text)
{
	int  return_code;
	char tmp_error_text[NPSL_MAX_ERROR_TEXT + 1];

	if ((return_code = NPSL_SetSockOpt(socket_handle, SOL_SOCKET, opt_name,
		&buffer_size, sizeof(buffer_size),
		(error_text != NULL) ? tmp_error_text : NULL)) != NPSL_SUCCESS) {
		if (error_text != NULL)
			strcat(strcat(strcat(strcpy(error_text, "Unable to set the "),
				(opt_name == SO_RCVBUF) ? "receive" : "send"), " buffer size: "),
				tmp_error_text);
	}

	return(return_code);
}
/* *********************************************************************** */

