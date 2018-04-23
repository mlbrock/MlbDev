/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Handles the high-level interface to the native
								operating system 'NPSL_SERVENT' functions.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2018.
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

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetServByPortString(const char *serv_port_string,
	const char *serv_proto, NPSL_SERVENT *serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	char *error_text)
{
	return(NPSL_GetServByPortStringBasic(serv_port_string, serv_proto,
		serv_ent_ptr, serv_ent_buffer_ptr, serv_ent_buffer_length, NULL,
		error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetServByPort(int serv_port, const char *serv_proto,
	NPSL_SERVENT *serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, char *error_text)
{
	return(NPSL_GetServByPortBasic(serv_port, serv_proto, serv_ent_ptr,
		serv_ent_buffer_ptr, serv_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetServByName(const char *serv_name, const char *serv_proto,
	NPSL_SERVENT *serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, char *error_text)
{
	return(NPSL_GetServByNameBasic(serv_name, serv_proto, serv_ent_ptr,
		serv_ent_buffer_ptr, serv_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetServByString(const char *serv_string, const char *serv_proto,
	NPSL_SERVENT *serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, char *error_text)
{
	return(NPSL_GetServByStringBasic(serv_string, serv_proto, serv_ent_ptr,
		serv_ent_buffer_ptr, serv_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

#ifdef TEST_MAIN

#include <stdio.h>

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

	fprintf(stderr, "Test routine for 'NPSL_GetServByPort()'\n");
	fprintf(stderr, "                 'NPSL_GetServByName()'\n");
	fprintf(stderr, "                 'NPSL_GetServByString()'\n");
	fprintf(stderr, "---- ------- --- ------------------------\n\n");

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
		if ((return_code = NPSL_GetServByString(argv[1], service_ptr,
			&servent_data, buffer, sizeof(buffer), error_text)) == NPSL_SUCCESS) {
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

