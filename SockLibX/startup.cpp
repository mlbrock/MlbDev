/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Starts the native operating system socket interface.

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
 
#include <stdio.h>
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
#ifdef _MSC_VER
int NPSL_SocketLibStartUp(void * /* user_data_ptr */, char *error_text)
#else
int NPSL_SocketLibStartUp(void * /* user_data_ptr */, char * /* error_text */)
#endif /* #ifdef _MSC_VER */
{
	int return_code = NPSL_SUCCESS;

#ifdef _Windows
	int     error_code;
	WORD    wsa_version;
	WSADATA wsa_data;
	char    tmp_error_text[NPSL_MAX_ERROR_TEXT];

	wsa_version = MAKEWORD(2, 2);
	if ((error_code = WSAStartup(wsa_version, &wsa_data)) != 0) {
		if (error_text != NULL) {
			sprintf(tmp_error_text, "Function '%s' has an error code of %d: ",
				"WSAStartup()", error_code);
			NPSL_AppendErrorString(error_code, 0, NPSL_MAX_ERROR_TEXT,
				tmp_error_text);
		}
		return_code = NPSL_SYSTEM_FAILURE;
	}
	else if ((LOBYTE(wsa_data.wVersion) != LOBYTE(wsa_version)) ||
		(HIBYTE(wsa_data.wVersion) != HIBYTE(wsa_version))) {
		if (error_text != NULL)
			sprintf(tmp_error_text, "Function '%s' %s %s %u.%u.",
				"WSAStartup()", "returned a 'WSADATA' structure with a",
				"'wVersion' member having a value of",
				((unsigned int) LOBYTE(wsa_data.wVersion)),
				((unsigned int) HIBYTE(wsa_data.wVersion)));
		WSACleanup();
		return_code = NPSL_SYSTEM_FAILURE;
	}
	if ((return_code != NPSL_SUCCESS) && (error_text != NULL)) {
		sprintf(error_text, "%s '%s' for version %u.%u: ",
			"Unable to initialize Windows sockets DLL with", "WSAStartup()",
			((unsigned int) LOBYTE(wsa_version)),
			((unsigned int) HIBYTE(wsa_version)));
		MLB::Utility::nstrcat(error_text, tmp_error_text,
			NPSL_MAX_ERROR_TEXT - 100);
	}
#endif /* #ifdef _Windows */

	return(return_code);
}
/* *********************************************************************** */

