/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Resolves host/service/protocol information.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2015.
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

#include <stdio.h>
#include <string.h>

/* *********************************************************************** */

/*	***********************************************************************	*/
int NPSL_ResolveHostServProtoString(const char *in_string, char *out_host,
	int *out_port, int *out_proto, char *error_text)
{
	return(NPSL_ResolveHostServProtoStringBasic(NPSL_FALSE, in_string, out_host,
		out_port, out_proto, error_text));
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_ResolveProtoHostServString(const char *in_string, char *out_host,
	int *out_port, int *out_proto, char *error_text)
{
	return(NPSL_ResolveHostServProtoStringBasic(NPSL_TRUE, in_string, out_host,
		out_port, out_proto, error_text));
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_ResolveHostServProtoStringBasic(int phs_flag, const char *in_string,
	char *out_host, int *out_port, int *out_proto, char *error_text)
{
	int            return_code = NPSL_SUCCESS;
	unsigned int   tmp_count;
	char         **tmp_list;
	const char    *string_type;
	unsigned int   count_1;
	char           tmp_error_text[NPSL_MAX_ERROR_TEXT];

	string_type = (phs_flag) ? "protocol/host/service" : "host/service/protocol";

	if ((in_string == NULL) || (!(*in_string))) {
		strcpy(tmp_error_text,
			(in_string == NULL) ? "Specification string is 'NULL'." :
			"Specification string is empty.");
		return_code = NPSL_FAILURE;
	}
	else if (MLB::NPSLI_Internal::chrcnt(in_string, ':') > 2) {
		sprintf(tmp_error_text, "%s (%u) --- the maximum is 3.",
			"Too many components encountered in specification string",
			((unsigned int) MLB::NPSLI_Internal::chrcnt(in_string, ':') + 1));
		return_code = NPSL_FAILURE;
	}
	else if (MLB::NPSLI_Internal::sepwordx_keep(in_string, ":", &tmp_count,
		&tmp_list) != MLB::NPSLI_Internal::STRFUNCS_SUCCESS) {
		strcpy(tmp_error_text,
			"Attempt to parse the specification string failed.");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else {
		for (count_1 = 0; count_1 < tmp_count; ++count_1) {
			if (!tmp_list[count_1][0]) {
				strcpy(tmp_error_text,
					"Empty components encountered in specification string.");
				return_code = NPSL_FAILURE;
				break;
			}
		}
		if (return_code == NPSL_SUCCESS) {
			if (phs_flag)
				return_code = NPSL_ResolveHostServProto(
					(tmp_count > 1) ? tmp_list[1] : NULL,
					(tmp_count > 2) ? tmp_list[2] : NULL,
					tmp_list[0],
					out_host, out_port, out_proto, tmp_error_text);
			else
				return_code = NPSL_ResolveHostServProto(
					tmp_list[0],
					(tmp_count > 1) ? tmp_list[1] : NULL,
					(tmp_count > 2) ? tmp_list[2] : NULL,
					out_host, out_port, out_proto, tmp_error_text);
		}
		MLB::NPSLI_Internal::strl_remove_all(&tmp_count, &tmp_list);
	}
	

	if ((return_code != NPSL_SUCCESS) && (error_text != NULL))
		strcat(strcat(MLB::Utility::nstrcat(strcat(strcat(strcpy(error_text,
			"Unable to resolve the "), string_type), " specification string '"),
			(in_string != NULL) ? in_string : "NULL", 500), "': "),
			tmp_error_text);

	return(return_code);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_ResolveHostServProto(const char *host_string, const char *serv_string,
	const char *proto_string, char *out_host, int *out_port, int *out_proto,
	char *error_text)
{
	int  return_code = NPSL_SUCCESS;
	char tmp_error_text[NPSL_MAX_ERROR_TEXT];

	if (out_host != NULL)
		*out_host = '\0';
	if (out_port != NULL)
		*out_port = 0;
	if (out_proto != NULL)
		*out_proto = 0;

	/*	*****************************************************************
		*****************************************************************
		Resolve the host name to its canonical form . . .
		*****************************************************************	*/
	if ((host_string != NULL) &&
		((return_code = NPSL_ResolveHostToName(host_string, out_host,
		tmp_error_text)) != NPSL_SUCCESS))
		goto EXIT_FUNCTION;
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Resolve the protocol name/number to its numeric equivalent . . .
		*****************************************************************	*/
	if ((proto_string != NULL) &&
		((return_code = NPSL_ResolveProtoToSint(proto_string, out_proto,
		tmp_error_text)) != NPSL_SUCCESS))
		goto EXIT_FUNCTION;
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Resolve the service name/number to its numeric equivalent . . .

		Note that 
		*****************************************************************	*/
	if (serv_string != NULL) {
		if ((proto_string != NULL) || (out_proto == NULL))
			return_code = NPSL_ResolveServToPort(serv_string, proto_string,
				out_port, tmp_error_text);
		else
			return_code = NPSL_ResolveServToPortBasic(serv_string, proto_string,
				out_port, NULL, out_proto, tmp_error_text);
	}
	/*	*****************************************************************	*/

EXIT_FUNCTION:

	if ((return_code != NPSL_SUCCESS) && (error_text != NULL))
		sprintf(error_text, "%s '%-.100s', %s '%-.100s', %s '%-.100s': %s",
			"Unable to resolve combination of host",
			(host_string != NULL) ? host_string   : "NULL",
			"service", (serv_string != NULL)  ? serv_string  : "NULL",
			"protocol", (proto_string != NULL) ? proto_string : "NULL",
			tmp_error_text);

	return(return_code);
}
/*	***********************************************************************	*/


#ifdef TEST_MAIN

#include <Utility/C_StringSupport.hpp>

COMPAT_FN_DECL(int main, (int argc, char **argv));

int main(int argc, char **argv)
{
	int            return_code = NPSL_SUCCESS;
	int            phs_flag    = NPSL_FALSE;
	unsigned int   count_1;
	char           out_host[NPSL_MAX_HOST_NAME_LEN + 1];
	int            out_port;
	int            out_proto;
	char           error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr,
		"Test routine for 'NPSL_ResolveHostServProtoString()'\n");
	fprintf(stderr,
		"                 'NPSL_ResolveProtoHostServString()'\n");
	fprintf(stderr,
		"                 'NPSL_ResolveHostServProtoStringBasic()'\n");
	fprintf(stderr,
		"                 'NPSL_ResolveHostServProto()'\n");
	fprintf(stderr,
		"---- ------- --- ----------------------------------------\n\n");

	for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
		if ((!MLB::Utility::Utility_stricmp("-h", argv[count_1])) ||
			(!MLB::Utility::Utility_stricmp("-help", argv[count_1]))) {
			fprintf(stderr, "USAGE:\n   %s %s [ %s . . . ]\n",
				MLB::NPSLI_Internal::extract_file_name(argv[0]),
				"[ -hsp ] <host-service-protocol-string> . . .",
				"-phs <protocol-host-service-string> . . .");
			goto EXIT_FUNCTION;
		}
	}

	if (argc < 2) {
		sprintf(error_text, "Invalid command line --- use '-h' for help");
		return_code = NPSL_FAILURE;
		goto EXIT_FUNCTION;
	}

	if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			if (!MLB::Utility::Utility_stricmp("-hsp", argv[count_1]))
				phs_flag = NPSL_FALSE;
			else if (!MLB::Utility::Utility_stricmp("-phs", argv[count_1]))
				phs_flag = NPSL_TRUE;
			else {
				printf("[%s]:", argv[count_1]);
				if (NPSL_ResolveHostServProtoStringBasic(phs_flag, argv[count_1],
					out_host, &out_port, &out_proto, error_text) == NPSL_SUCCESS)
					printf("[%s][%d][%d]\n", out_host, out_port, out_proto);
				else {
					printf("ERROR: %s\n", error_text);
					return_code = NPSL_FAILURE;
				}
			}
		}
		NPSL_SocketLibCleanUp(NULL, NULL);
	}

EXIT_FUNCTION:

	if (return_code != NPSL_SUCCESS)
		fprintf(stderr, "\n\nERROR: %s\n\n", error_text);

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

