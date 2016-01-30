/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Sets the blocking mode of a socket.

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

#ifndef _MSC_VER
# include <fcntl.h>
#endif // #ifndef _MSC_VER

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_SetBlockingModeOff(NPSL_SOCKET_HANDLE socket_handle, char *error_text)
{
	int            return_code;
	const char    *function_name;
#ifdef _Windows
	unsigned long  blocking_mode;
#else
	int            file_flags;
#endif /* #ifdef _Windows */

	if ((return_code = NPSL_CheckSocket(socket_handle, error_text)) ==
		NPSL_SUCCESS) {
#ifdef _Windows
		blocking_mode = 1;
		if (ioctlsocket(socket_handle, FIONBIO, &blocking_mode) != 0) {
			function_name = "ioctlsocket";
#else
		file_flags  = fcntl(socket_handle, F_GETFL);
		file_flags |= O_NONBLOCK;
		if (fcntl(socket_handle, F_SETFL, file_flags) == -1) {
			function_name  = "fcntl";
#endif /* #ifdef _Windows */
			if (error_text != NULL)
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
					strcat(strcat(strcpy(error_text,
					"Attempt to set the socket to non-blocking mode with '"),
					function_name), "()' failed: "));
			return_code = NPSL_SUPP_MapLastError();
		}
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_SetBlockingModeOn(NPSL_SOCKET_HANDLE socket_handle, char *error_text)
{
	int            return_code;
	const char    *function_name;
#ifdef _Windows
	unsigned long  blocking_mode;
#else
	int            file_flags;
#endif /* #ifdef _Windows */

	if ((return_code = NPSL_CheckSocket(socket_handle, error_text)) ==
		NPSL_SUCCESS) {
#ifdef _Windows
		blocking_mode = 0;
		if (ioctlsocket(socket_handle, FIONBIO, &blocking_mode) != 0) {
			function_name = "ioctlsocket";
#else
		file_flags  = fcntl(socket_handle, F_GETFL);
		file_flags &= ~O_NONBLOCK;
		if (fcntl(socket_handle, F_SETFL, file_flags) == -1) {
			function_name  = "fcntl";
#endif /* #ifdef _Windows */
			if (error_text != NULL)
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
					strcat(strcat(strcpy(error_text,
					"Attempt to set the socket to blocking mode with '"),
					function_name), "()' failed: "));
			return_code = NPSL_SUPP_MapLastError();
		}
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_SetBlockingMode(NPSL_SOCKET_HANDLE socket_handle,
	int blocking_mode_flag, char *error_text)
{
	return((blocking_mode_flag) ?
		NPSL_SetBlockingModeOn(socket_handle, error_text) :
		NPSL_SetBlockingModeOff(socket_handle, error_text));
}
/* *********************************************************************** */

