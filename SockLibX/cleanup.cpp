/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Closes the native operating system socket interface.

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
#ifdef _MSC_VER
int NPSL_SocketLibCleanUp(void * /* user_data_ptr */, char *error_text)
#else
int NPSL_SocketLibCleanUp(void * /* user_data_ptr */, char * /* error_text */)
#endif /* #ifdef _MSC_VER */
{
	int return_code = NPSL_SUCCESS;

#ifdef _Windows
	if (WSACleanup()) {
		if (error_text != NULL) {
			sprintf(error_text, "%s '%s'.",
				"Unable to cleanup Windows sockets DLL with", "WSACleanup()");
			NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
		}
		return_code = NPSL_SYSTEM_FAILURE;
	}
#endif /* #ifdef _Windows */

	return(return_code);
}
/* *********************************************************************** */

