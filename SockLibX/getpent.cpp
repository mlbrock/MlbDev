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
								operating system 'NPSL_PROTOENT' functions.

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
 
#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetProtoByNumberString(const char *proto_number_string,
	NPSL_PROTOENT *proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, char *error_text)
{
	return(NPSL_GetProtoByNumberStringBasic(proto_number_string, proto_ent_ptr,
		proto_ent_buffer_ptr, proto_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetProtoByNumber(int proto_number, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	char *error_text)
{
	return(NPSL_GetProtoByNumberBasic(proto_number, proto_ent_ptr,
		proto_ent_buffer_ptr, proto_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetProtoByName(const char *proto_name, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	char *error_text)
{
	return(NPSL_GetProtoByNameBasic(proto_name, proto_ent_ptr,
		proto_ent_buffer_ptr, proto_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetProtoByString(const char *proto_string,
	NPSL_PROTOENT *proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, char *error_text)
{
	return(NPSL_GetProtoByStringBasic(proto_string, proto_ent_ptr,
		proto_ent_buffer_ptr, proto_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

#ifdef TEST_MAIN

#include <stdio.h>

COMPAT_FN_DECL(int main, (int argc, char **argv));

int main(int argc, char **argv)
{
	int             return_code = NPSL_FAILURE;
	unsigned int    count_1;
	unsigned int    count_2;
	char            buffer[1024];
	NPSL_PROTOENT   protoent_data;
	char          **tmp_list;
	char            error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test routine for 'NPSL_GetProtoByNumber()'\n");
	fprintf(stderr, "                 'NPSL_GetProtoByName()'\n");
	fprintf(stderr, "                 'NPSL_GetProtoByString()'\n");
	fprintf(stderr, "---- ------- --- -------------------------\n\n");

	if (argc == 1)
		sprintf(error_text, "USAGE: %s %s [%s . . .]\n", argv[0],
			"{ <protocol-name> | <protocol-number> }",
			"{ <protocol-name> | <protocol-number> }");
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		return_code = NPSL_SUCCESS;
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			printf("PROTOCOL NAME OR NUMBER: %s\n", argv[count_1]);
			printf("                       : by %s ---> ",
				(!isdigit(*argv[count_1])) ? "name" : "number");
			if (NPSL_GetProtoByString(argv[count_1], &protoent_data, buffer,
				sizeof(buffer), error_text) == NPSL_SUCCESS) {
				printf("DONE\n");
				printf("                       : Protocol Name    : %s\n",
					protoent_data.p_name);
				printf("                       : Protocol Protocol: %d\n",
					protoent_data.p_proto);
				if ((tmp_list = protoent_data.p_aliases)[0] != NULL) {
					count_2 = 0;
					while (*tmp_list != NULL) {
						printf("                       : Alias    %8u: %s\n",
							count_2++, *tmp_list);
						tmp_list++;
					}
				}
			}
			else
				printf("                       : ERROR: %s\n", error_text);
		}
		NPSL_SocketLibCleanUp(NULL, NULL);
	}

	if (return_code != NPSL_SUCCESS)
		fprintf(stderr, "\n\nERROR: %s\n\n", error_text);

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

