/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for synchronous I/O multiplexing.

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
int NPSL_Select(int fd_set_size, NPSL_FD_SET_T *read_fds,
	NPSL_FD_SET_T *write_fds, NPSL_FD_SET_T *error_fds,
	const struct timeval *time_out, unsigned int *ready_count, char *error_text)
{
	int return_code;
	int select_count;

	*ready_count = 0;

	/*
		Cast needed to support benighted declarations of 'select()'...
	*/
	if ((select_count = select(fd_set_size, read_fds, write_fds, error_fds,
		const_cast<struct timeval *>(time_out))) != NPSL_SOCKET_ERROR) {
		*ready_count = ((unsigned int) select_count);
		 return_code = NPSL_SUCCESS;
	}
	else {
		if (error_text != NULL)
			NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
				strcpy(error_text,
				"Attempt to perform a select with 'select()' failed: "));
		return_code = NPSL_SUPP_MapLastError();
	}

	return(return_code);
}
/* *********************************************************************** */

