/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Get socket options.

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
COMPAT_FN_DECL_STATIC(int NPSL_GetSockOpt_BufferSize,
	(NPSL_SOCKET_HANDLE socket_handle, unsigned int *buffer_size, int opt_name,
	char *error_text));
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetSockOpt(NPSL_SOCKET_HANDLE socket_handle, int opt_level,
	int opt_name, void *opt_value, int *opt_length, char *error_text)
{
	int return_code;

#ifdef __linux__
	if (getsockopt(socket_handle, opt_level, opt_name,
		((NPSL_NATIVE_SocketOptionValueType) opt_value),
		reinterpret_cast<socklen_t *>(opt_length)) != NPSL_SOCKET_OK) {
#else
	if (getsockopt(socket_handle, opt_level, opt_name,
		((NPSL_NATIVE_SocketOptionValueType) opt_value), opt_length) !=
		NPSL_SOCKET_OK) {
#endif // #ifdef __linux__
		if (error_text != NULL)
			NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
				strcpy(error_text,
"Attempt to get the socket options with 'getsockopt()' failed: "));
		return_code = NPSL_SUPP_MapLastError();
	}
	else
		return_code = NPSL_SUCCESS;

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetSockOpt_RcvBuf(NPSL_SOCKET_HANDLE socket_handle,
	unsigned int *buffer_size, char *error_text)
{
	return(NPSL_GetSockOpt_BufferSize(socket_handle, buffer_size,
		SO_RCVBUF, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetSockOpt_SndBuf(NPSL_SOCKET_HANDLE socket_handle,
	unsigned int *buffer_size, char *error_text)
{
	return(NPSL_GetSockOpt_BufferSize(socket_handle, buffer_size,
		SO_SNDBUF, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
static int NPSL_GetSockOpt_BufferSize(NPSL_SOCKET_HANDLE socket_handle,
	unsigned int *buffer_size, int opt_name, char *error_text)
{
	int  return_code;
	int  tmp_buffer_length = sizeof(*buffer_size);
	char tmp_error_text[NPSL_MAX_ERROR_TEXT + 1];

	if ((return_code = NPSL_GetSockOpt(socket_handle, SOL_SOCKET, opt_name,
		buffer_size, &tmp_buffer_length,
		(error_text != NULL) ? tmp_error_text : NULL)) != NPSL_SUCCESS) {
		if (error_text != NULL)
			strcat(strcat(strcat(strcpy(error_text, "Unable to get the "),
				(opt_name == SO_RCVBUF) ? "receive" : "send"), " buffer size: "),
				tmp_error_text);
	}

	return(return_code);
}
/* *********************************************************************** */

