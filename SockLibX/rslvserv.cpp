/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Resolves service information.

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
 
#include "npsli.h"

#include <stdlib.h>
#include <string.h>

/* *********************************************************************** */

/*	***********************************************************************	*/
int NPSL_ResolveServToPort(const char *serv_string, const char *serv_proto,
	int *out_port, char *error_text)
{
	return(NPSL_ResolveServToPortBasic(serv_string, serv_proto, out_port,
		NULL, NULL, error_text));
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_ResolveServToPortBasic(const char *serv_string, const char *serv_proto,
	int *out_port, char *out_proto_name, int *out_proto, char *error_text)
{
	int          return_code;
	int          lookup_done_flag = NPSL_FALSE;
	NPSL_SERVENT sent_data;
	char         buffer[8192];
	char         tmp_proto_name[127 + 1];

	out_proto_name = (out_proto_name != NULL) ? out_proto_name : tmp_proto_name;

	if (serv_string == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'serv_string' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!(*serv_string)) {
		if (error_text != NULL)
			strcpy(error_text,
				"The 'serv_string' parameter is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if (!isdigit(*serv_string)) {
		if ((return_code = NPSL_GetServByName(serv_string, serv_proto,
			&sent_data, buffer, sizeof(buffer), error_text)) == NPSL_SUCCESS) {
			if (out_port != NULL)
				*out_port =
					((int) ntohs(static_cast<unsigned short>(sent_data.s_port)));
			strcpy(out_proto_name, sent_data.s_proto);
			lookup_done_flag = NPSL_TRUE;
		}
	}
	else if (serv_proto != NULL) {
		if ((return_code = NPSL_GetServByPortString(serv_string, serv_proto,
			&sent_data, buffer, sizeof(buffer), error_text)) == NPSL_SUCCESS) {
			if (out_port != NULL)
				*out_port =
					((int) ntohs(static_cast<unsigned short>(sent_data.s_port)));
			strcpy(out_proto_name, sent_data.s_proto);
			lookup_done_flag = NPSL_TRUE;
		}
	}
	else if (MLB::NPSLI_Internal::str_digit(serv_string, 0) != NULL) {
		if (error_text != NULL)
			strcat(MLB::Utility::nstrcat(strcat(strcpy(error_text,
				"Non-numeric characters encountered in the 'serv_string' "),
				"parameter ('"), serv_string, NPSL_MAX_ERROR_TEXT - 100), "').");
		return_code = NPSL_FAILURE;
	}
/*
	CODE NOTE: 	To be removed. Port number should be a typdef'ed type; on
					systems where ints > 16 bits in length, an int will do,
					otherwise, must be an unsigned int.
	else if (atof(serv_string) > ((double) SHRT_MAX)) {
*/
	else if (atof(serv_string) > ((double) 65535)) {
		if (error_text != NULL)
			sprintf(error_text,
				"%s parameter ('%-.500s') %s %d and %d, inclusive.",
				"Invalid port number encountered in the 'serv_string'",
				serv_string, "port numbers must be between", 0, 65535);
		return_code = NPSL_FAILURE;
	}
	else {
		if (out_port != NULL)
			*out_port = atoi(serv_string);
		return_code = NPSL_SUCCESS;
	}

	if ((return_code == NPSL_SUCCESS) && lookup_done_flag &&
		(out_proto != NULL))
		return_code = NPSL_ResolveProtoToSint(out_proto_name, out_proto,
			error_text);

	return(return_code);
}
/*	***********************************************************************	*/

