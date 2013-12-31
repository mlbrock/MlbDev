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
								operating system 'NPSL_PROTOENT' functions.

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
#include <stdlib.h>
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetProtoByNumberStringBasic(const char *proto_number_string,
	NPSL_PROTOENT *proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int    return_code;
	double tmp_double;

	if (required_length != NULL)
		*required_length = 0;

	if (proto_number_string == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'proto_number_string' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!(*proto_number_string)) {
		if (error_text != NULL)
			strcpy(error_text,
				"The 'proto_number_string' parameter is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if (MLB::NPSLI_Internal::str_digit(proto_number_string, 0) != NULL) {
		if (error_text != NULL)
			strcat(MLB::Utility::nstrcat(strcat(strcpy(error_text,
				"Non-numeric characters encountered in the 'proto_number_string' "),
				"parameter ('"), proto_number_string, NPSL_MAX_ERROR_TEXT - 100),
				"').");
		return_code = NPSL_FAILURE;
	}
	else if (((tmp_double = atof(proto_number_string)) < 0.0) ||
		(tmp_double > ((double) SHRT_MAX))) {
		if (error_text != NULL)
			sprintf(error_text,
				"%s parameter ('%-.500s') %s %d and %d, inclusive.",
				"Invalid protocol number encountered in the 'proto_number_string'",
				proto_number_string, "protocol numbers must be between", 0,
				SHRT_MAX);
		return_code = NPSL_FAILURE;
	}
	else
		return_code = NPSL_GetProtoByNumberBasic(atoi(proto_number_string),
			proto_ent_ptr, proto_ent_buffer_ptr, proto_ent_buffer_length,
			required_length, error_text);

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetProtoByNumberBasic(int proto_number, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	unsigned int *required_length, char *error_text)
{
	int            return_code = NPSL_SUCCESS;
	NPSL_PROTOENT *tmp_ent_ptr;

	if (required_length != NULL)
		*required_length = 0;

	if ((return_code = NPSL_CheckProtoEntParams(proto_ent_ptr,
		proto_ent_buffer_ptr, proto_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		;
	else {
#if NPSL_HAS_GETPROTOBYPORT_R
		if ((tmp_ent_ptr = getprotobynumber_r(proto_number, proto_ent_ptr,
			proto_ent_buffer_ptr, ((int) proto_ent_buffer_length))) == NULL) {
			if (error_text != NULL) {
				sprintf(error_text,
					"%s '%s()' for protocol number '%d': ",
					"Unable to get protocol entry with", "getprotobynumber_r",
					proto_number);
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
			}
			return_code = NPSL_SUPP_MapLastError();
		}
#else
		if ((tmp_ent_ptr = getprotobynumber(proto_number)) == NULL) {
			if (error_text != NULL) {
				sprintf(error_text,
					"%s '%s()' for protocol number '%d': ",
					"Unable to get protocol entry with", "getprotobynumber",
					proto_number);
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
			}
			return_code = NPSL_SUPP_MapLastError();
		}
		else
			return_code = NPSL_CopyProtoEntFlatBasic(tmp_ent_ptr, proto_ent_ptr,
				proto_ent_buffer_ptr, proto_ent_buffer_length, required_length,
				error_text);
#endif // #if NPSL_HAS_GETPROTOBYPORT_R
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetProtoByNameBasic(const char *proto_name,
	NPSL_PROTOENT *proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int            return_code = NPSL_SUCCESS;
	NPSL_PROTOENT *tmp_ent_ptr;

	if (required_length != NULL)
		*required_length = 0;

	if (proto_name == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'proto_name' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!(*proto_name)) {
		if (error_text != NULL)
			strcpy(error_text, "The 'proto_name' parameter is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if ((return_code = NPSL_CheckProtoEntParams(proto_ent_ptr,
		proto_ent_buffer_ptr, proto_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		;
	else {
#if NPSL_HAS_GETPROTOBYNAME_R
		if ((tmp_ent_ptr = getprotobyname_r(proto_name,
			proto_ent_ptr, proto_ent_buffer_ptr,
			((int) proto_ent_buffer_length))) == NULL) {
			if (error_text != NULL) {
				sprintf(error_text,
					"%s '%s()' for protocol name '%-.127s': ",
					"Unable to get protocol entry with", "getprotobyname_r",
					proto_name);
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
			}
			return_code = NPSL_SUPP_MapLastError();
		}
#else
		if ((tmp_ent_ptr = getprotobyname(proto_name)) == NULL) {
			if (error_text != NULL) {
				sprintf(error_text,
					"%s '%s()' for protocol name '%-.127s': ",
					"Unable to get protocol entry with", "getprotobyname",
					proto_name);
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT, error_text);
			}
			return_code = NPSL_SUPP_MapLastError();
		}
		else
			return_code = NPSL_CopyProtoEntFlatBasic(tmp_ent_ptr, proto_ent_ptr,
				proto_ent_buffer_ptr, proto_ent_buffer_length, required_length,
				error_text);
#endif // #if NPSL_HAS_GETPROTOBYNAME_R
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetProtoByStringBasic(const char *proto_string,
	NPSL_PROTOENT *proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int return_code;

	if (required_length != NULL)
		*required_length = 0;

	if (proto_string == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'proto_string' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!isdigit(*proto_string))
		return_code = NPSL_GetProtoByNameBasic(proto_string, proto_ent_ptr,
			proto_ent_buffer_ptr, proto_ent_buffer_length, required_length,
			error_text);
	else
		return_code = NPSL_GetProtoByNumberStringBasic(proto_string,
			proto_ent_ptr, proto_ent_buffer_ptr, proto_ent_buffer_length,
			required_length, error_text);

	return(return_code);
}
/* *********************************************************************** */

#ifdef TEST_MAIN

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

	fprintf(stderr, "Test routine for 'NPSL_GetProtoByNumberBasic()'\n");
	fprintf(stderr, "                 'NPSL_GetProtoByNameBasic()'\n");
	fprintf(stderr, "                 'NPSL_GetProtoByStringBasic()'\n");
	fprintf(stderr, "---- ------- --- ------------------------------\n\n");

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
			if (NPSL_GetProtoByStringBasic(argv[count_1], &protoent_data, buffer,
				sizeof(buffer), NULL, error_text) == NPSL_SUCCESS) {
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

