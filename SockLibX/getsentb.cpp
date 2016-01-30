/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Handles the low-level interface to the native
								operating system 'NPSL_SERVENT' functions.

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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetServByPortStringBasic(const char *serv_port_string,
	const char *serv_proto, NPSL_SERVENT *serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	unsigned int *required_length, char *error_text)
{
	int    return_code;
	double tmp_double;

	if (required_length != NULL)
		*required_length = 0;

	if (serv_port_string == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'serv_port_string' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!(*serv_port_string)) {
		if (error_text != NULL)
			strcpy(error_text,
				"The 'serv_port_string' parameter is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if (MLB::NPSLI_Internal::str_digit(serv_port_string, 0) != NULL) {
		if (error_text != NULL)
			strcat(MLB::Utility::nstrcat(strcat(strcpy(error_text,
				"Non-numeric characters encountered in the 'serv_port_string' "),
				"parameter ('"), serv_port_string, NPSL_MAX_ERROR_TEXT - 100),
				"').");
		return_code = NPSL_FAILURE;
	}
/*
	CODE NOTE: 	To be removed. Port number should be a typdef'ed type; on
					systems where ints > 16 bits in length, an int will do,
					otherwise, must be an unsigned int.
	else if (((tmp_double = atof(serv_port_string)) < 0.0) ||
		(tmp_double > ((double) SHRT_MAX))) {
*/
	else if (((tmp_double = atof(serv_port_string)) < 0.0) ||
		(tmp_double > ((double) 65535))) {
		if (error_text != NULL)
			sprintf(error_text,
				"%s parameter ('%-.500s') %s %d and %d, inclusive.",
				"Invalid port number encountered in the 'serv_port_string'",
				serv_port_string, "port numbers must be between", 0, 65535);
		return_code = NPSL_FAILURE;
	}
	else
		return_code = NPSL_GetServByPortBasic(atoi(serv_port_string), serv_proto,
			serv_ent_ptr, serv_ent_buffer_ptr, serv_ent_buffer_length,
			required_length, error_text);

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetServByPortBasic(int serv_port, const char *serv_proto,
	NPSL_SERVENT *serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int           return_code = NPSL_SUCCESS;
	NPSL_SERVENT *tmp_ent_ptr;

	if (required_length != NULL)
		*required_length = 0;

	if ((serv_proto != NULL) && (!(*serv_proto))) {
		if (error_text != NULL)
			strcpy(error_text,
		"The 'serv_proto' parameter is not 'NULL', but is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if ((return_code = NPSL_CheckServEntParams(serv_ent_ptr,
		serv_ent_buffer_ptr, serv_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		;
	else {
		int tmp_port = COMPAT_CAST_static(int,
			ntohs(COMPAT_CAST_static(unsigned short, serv_port)));
#if NPSL_HAS_GETSERVBYPORT_R
		if ((tmp_ent_ptr = getservbyport_r(tmp_port, serv_proto,
			serv_ent_ptr, serv_ent_buffer_ptr,
			((int) serv_ent_buffer_length))) == NULL) {
			if (error_text != NULL) {
				sprintf(error_text,
					"%s '%s()' for service port '%d', protocol '%-.127s': ",
					"Unable to get service entry with", "getservbyport_r",
					serv_port, (serv_proto != NULL) ? serv_proto : "*NULL*");
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
			}
			return_code = NPSL_SUPP_MapLastError();
		}
#else
		if ((tmp_ent_ptr = getservbyport(tmp_port, serv_proto)) == NULL) {
			if (error_text != NULL) {
				sprintf(error_text,
					"%s '%s()' for service port '%d', protocol '%-.127s': ",
					"Unable to get service entry with", "getservbyport",
					serv_port, (serv_proto != NULL) ? serv_proto : "*NULL*");
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
			}
			return_code = NPSL_SUPP_MapLastError();
		}
		else
			return_code = NPSL_CopyServEntFlatBasic(tmp_ent_ptr, serv_ent_ptr,
				serv_ent_buffer_ptr, serv_ent_buffer_length, required_length,
				error_text);
#endif // #if NPSL_HAS_GETSERVBYPORT_R
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetServByNameBasic(const char *serv_name, const char *serv_proto,
	NPSL_SERVENT *serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int           return_code = NPSL_SUCCESS;
	NPSL_SERVENT *tmp_ent_ptr;

	if (required_length != NULL)
		*required_length = 0;

	if (serv_name == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'serv_name' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!(*serv_name)) {
		if (error_text != NULL)
			strcpy(error_text, "The 'serv_name' parameter is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if ((serv_proto != NULL) && (!(*serv_proto))) {
		if (error_text != NULL)
			strcpy(error_text,
		"The 'serv_proto' parameter is not 'NULL', but is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if ((return_code = NPSL_CheckServEntParams(serv_ent_ptr,
		serv_ent_buffer_ptr, serv_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		;
	else {
#if NPSL_HAS_GETSERVBYNAME_R
		if ((tmp_ent_ptr = getservbyname_r(serv_name, serv_proto,
			serv_ent_ptr, serv_ent_buffer_ptr,
			((int) serv_ent_buffer_length))) == NULL) {
			if (error_text != NULL) {
				sprintf(error_text,
					"%s '%s()' for service name '%-.127s', protocol '%-.127s': ",
					"Unable to get service entry with", "getservbyname_r",
					serv_name, (serv_proto != NULL) ? serv_proto : "*NULL*");
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
			}
			return_code = NPSL_SUPP_MapLastError();
		}
#else
		if ((tmp_ent_ptr = getservbyname(serv_name, serv_proto)) == NULL) {
			if (error_text != NULL) {
				sprintf(error_text,
					"%s '%s()' for service name '%-.127s', protocol '%-.127s': ",
					"Unable to get service entry with", "getservbyname",
					serv_name, (serv_proto != NULL) ? serv_proto : "*NULL*");
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
			}
			return_code = NPSL_SUPP_MapLastError();
		}
		else
			return_code = NPSL_CopyServEntFlatBasic(tmp_ent_ptr, serv_ent_ptr,
				serv_ent_buffer_ptr, serv_ent_buffer_length, required_length,
				error_text);
#endif // #if NPSL_HAS_GETSERVBYNAME_R
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetServByStringBasic(const char *serv_string, const char *serv_proto,
	NPSL_SERVENT *serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int return_code;

	if (required_length != NULL)
		*required_length = 0;

	if (serv_string == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'serv_string' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!(*serv_string)) {
		if (error_text != NULL)
			strcpy(error_text, "The 'serv_string' parameter is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if (!isdigit(*serv_string))
		return_code = NPSL_GetServByNameBasic(serv_string, serv_proto,
			serv_ent_ptr, serv_ent_buffer_ptr, serv_ent_buffer_length,
			required_length, error_text);
	else
		return_code = NPSL_GetServByPortStringBasic(serv_string, serv_proto,
			serv_ent_ptr, serv_ent_buffer_ptr, serv_ent_buffer_length,
			required_length, error_text);

	return(return_code);
}
/* *********************************************************************** */

#ifdef TEST_MAIN

COMPAT_FN_DECL(int main, (int argc, char **argv));

int main(int argc, char **argv)
{
	int            return_code = NPSL_FAILURE;
	unsigned int   count_1;
	char           buffer[1024];
	NPSL_SERVENT   servent_data;
	const char    *service_ptr;
	char         **tmp_list;
	char           error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test routine for 'NPSL_GetServByPortBasic()'\n");
	fprintf(stderr, "                 'NPSL_GetServByNameBasic()'\n");
	fprintf(stderr, "                 'NPSL_GetServByStringBasic()'\n");
	fprintf(stderr, "---- ------- --- -----------------------------\n\n");

	if ((argc < 2) || (argc > 3))
		sprintf(error_text, "USAGE: %s %s\n", argv[0],
			"{ <service-name> | <service-port> } [ <service-protocol> ]");
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		service_ptr = (argc < 3) ? NULL : argv[2];
		printf("SERVICE NAME OR PORT: %s\n", argv[1]);
		printf("SERVICE PROTOCOL    : %s\n",
			(service_ptr != NULL) ? service_ptr : "*** UNSPECIFIED ***");
		printf("                    : by %s ---> ",
			(!isdigit(*argv[1])) ? "name" : "port");
		if ((return_code = NPSL_GetServByStringBasic(argv[1], service_ptr,
			&servent_data, buffer, sizeof(buffer), NULL, error_text)) ==
			NPSL_SUCCESS) {
			printf("DONE\n");
			printf("                    : Service Name    : %s\n",
				servent_data.s_name);
			printf("                    : Service Port    : %d\n",
				((int) ntohs(servent_data.s_port)));
			printf("                    : Service Protocol: %s\n",
				servent_data.s_proto);
			if ((tmp_list = servent_data.s_aliases)[0] != NULL) {
				count_1 = 0;
				while (*tmp_list != NULL) {
					printf("                    : Alias %10u: %s\n", count_1++,
						*tmp_list);
					tmp_list++;
				}
			}
		}
		NPSL_SocketLibCleanUp(NULL, NULL);
	}

	if (return_code != NPSL_SUCCESS)
		fprintf(stderr, "\n\nERROR: %s\n\n", error_text);

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

