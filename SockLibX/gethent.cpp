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
								operating system 'NPSL_HOSTENT' functions.

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
int NPSL_GetHostByAddr(const char *host_addr, int host_addr_len,
	int host_addr_type, NPSL_HOSTENT *host_ent_ptr, void *host_ent_buffer_ptr,
	unsigned int host_ent_buffer_length, char *error_text)
{
	return(NPSL_GetHostByAddrBasic(host_addr, host_addr_len, host_addr_type,
		host_ent_ptr, host_ent_buffer_ptr, host_ent_buffer_length, NULL,
		error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetHostByINet(const char *host_inet, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	char *error_text)
{
	return(NPSL_GetHostByINetBasic(host_inet, host_ent_ptr, host_ent_buffer_ptr,
		host_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetHostByName(const char *host_name, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	char *error_text)
{
	return(NPSL_GetHostByNameBasic(host_name, host_ent_ptr, host_ent_buffer_ptr,
		host_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetHostByString(const char *host_string, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	char *error_text)
{
	return(NPSL_GetHostByStringBasic(host_string, host_ent_ptr,
		host_ent_buffer_ptr, host_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

#ifdef TEST_MAIN

#include <stdio.h>

COMPAT_FN_DECL(int main, (int argc, char **argv));

int main(int argc, char **argv)
{
	int            return_code = NPSL_FAILURE;
	unsigned int   count_1;
	unsigned int   count_2;
	char           buffer[1024];
	NPSL_HOSTENT   hostent_data;
	char         **tmp_list;
	char           inet_buffer[NPSL_MAX_IPV4_ADDR_STR + 1];
	char           error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test routine for 'NPSL_GetHostByAddr()'\n");
	fprintf(stderr, "                 'NPSL_GetHostByINet()'\n");
	fprintf(stderr, "                 'NPSL_GetHostByName()'\n");
	fprintf(stderr, "                 'NPSL_GetHostByString()'\n");
	fprintf(stderr, "---- ------- --- ------------------------\n\n");

	if (argc == 1)
		sprintf(error_text, "USAGE: %s { %s } [ { %s } . . . ]\n", argv[0],
			"<host-name> | <IP-address>", "<host-name> | <IP-address>");
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			printf("HOSTNAME: %s ---> ", argv[count_1]);
			printf("by %s ---> ",
				(!isdigit(*argv[count_1])) ? "name" : "address");
			if ((return_code = NPSL_GetHostByString(argv[count_1],
				&hostent_data, buffer, sizeof(buffer), error_text)) !=
				NPSL_SUCCESS)
				break;
			printf("DONE\n");
			printf("        : Host Name     : %s\n",
				hostent_data.h_name);
			printf("        : Address Type  : %d\n",
				hostent_data.h_addrtype);
			printf("        : Address Length: %d\n",
				hostent_data.h_length);
			if ((tmp_list = hostent_data.h_aliases)[0] != NULL) {
				count_2 = 0;
				while (*tmp_list != NULL) {
					printf("        : Alias %8u: %s\n", count_2++,
						*tmp_list);
					tmp_list++;
				}
			}
			if (((tmp_list = hostent_data.h_addr_list)[0] != NULL) &&
				(hostent_data.h_addrtype == AF_INET)) {
				count_2 = 0;
				while (*tmp_list != NULL) {
					printf("        : Address %6u: %s\n", count_2++,
						NPSL_AddrToINet(*tmp_list, inet_buffer));
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

