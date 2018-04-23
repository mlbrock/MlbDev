/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Network Programming Support Library (NPSL) Source Module						*/
/*	***********************************************************************	*/
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Gets/sets the host name.

	Revision History	:	1993-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*		Necessary include files . . .														*/
/*	***********************************************************************	*/

#include <string.h>

#include "npsli.h"

/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_DetermineHostName(char *host_name, char *error_text)
{
	int return_code;

	try {
		MLB::Utility::GetHostName(host_name);
		return_code = NPSL_SUCCESS;
	}
	catch (const std::exception &except) {
		MLB::Utility::nstrcpy(error_text, except.what(), NPSL_MAX_ERROR_TEXT - 1);
		return_code = NPSL_SYSTEM_FAILURE;
	}

	return(return_code);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_GetHostName(char *host_name, unsigned int host_name_length)
{
	int return_code;

	try {
		MLB::Utility::GetHostName(host_name, host_name_length);
		return_code = NPSL_SUCCESS;
	}
	catch (...) {
		return_code = NPSL_SYSTEM_FAILURE;
	}

	return(return_code);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_SetHostName(const char *host_name)
{
	int return_code;

	try {
		MLB::Utility::SetHostName(host_name);
		return_code = NPSL_SUCCESS;
	}
	catch (...) {
		return_code = NPSL_SYSTEM_FAILURE;
	}

	return(return_code);
}
/*	***********************************************************************	*/

#ifdef TEST_MAIN

#include <stdio.h>

COMPAT_FN_DECL(int main, (void));

int main()
{
	int  return_code;
	char host_name[NPSL_MAX_HOST_NAME_LEN + 1];
	char error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test harness for function 'NPSL_GetHostName()'\n");
	fprintf(stderr, "---- ------- --- -------- --------------------\n\n");

	if ((return_code = NPSL_DetermineHostName(host_name, error_text)) ==
		NPSL_SUCCESS)
		printf("Host Name: %s\n", host_name);
	else
		fprintf(stderr, "ERROR: %s\n", error_text);

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

