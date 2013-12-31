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
								operating system 'NPSL_HOSTENT' functions.

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
int NPSL_GetHostByAddrBasic(const char *host_addr, int host_addr_len,
	int host_addr_type, NPSL_HOSTENT *host_ent_ptr, void *host_ent_buffer_ptr,
	unsigned int host_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int           return_code = NPSL_SUCCESS;
	NPSL_HOSTENT *tmp_ent_ptr;
#if NPSL_HAS_GETHOSTBYADDR_R
	int           local_errno;
#endif // #if NPSL_HAS_GETHOSTBYADDR_R

	if (required_length != NULL)
		*required_length = 0;

	if (host_addr == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'host_addr' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if ((return_code = NPSL_CheckHostEntParams(host_ent_ptr,
		host_ent_buffer_ptr, host_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		;
	else {
#if NPSL_HAS_GETHOSTBYADDR_R
		if ((tmp_ent_ptr = gethostbyaddr_r(host_addr, host_addr_len,
			host_addr_type, host_ent_ptr, host_ent_buffer_ptr,
			((int) host_ent_buffer_length), &local_errno)) == NULL) {
			if (error_text != NULL)
				NPSL_GetErrorString(local_errno, 0, NPSL_MAX_ERROR_TEXT,
					strcpy(error_text,
					"Unable to get host entry with 'gethostbyaddr_r()' failed: "));
			return_code = NPSL_SUPP_MapLastError();
		}
#else
		if ((tmp_ent_ptr = gethostbyaddr(host_addr, host_addr_len,
			host_addr_type)) == NULL) {
			if (error_text != NULL)
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
					strcpy(error_text,
					"Unable to get host entry with 'gethostbyaddr()' failed: "));
			return_code = NPSL_SUPP_MapLastError();
		}
		else
			return_code = NPSL_CopyHostEntFlatBasic(tmp_ent_ptr, host_ent_ptr,
				host_ent_buffer_ptr, host_ent_buffer_length, required_length,
				error_text);
#endif // #if NPSL_HAS_GETHOSTBYADDR_R
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetHostByINetBasic(const char *host_inet, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	unsigned int *required_length, char *error_text)
{
	int           return_code;
	unsigned long inet_addr;

	if (required_length != NULL)
		*required_length = 0;

	if ((return_code = NPSL_INetToAddrBasic(host_inet, &inet_addr,
		error_text)) == NPSL_SUCCESS)	 
		return_code = NPSL_GetHostByAddrBasic(((const char *) &inet_addr),
			4, AF_INET, host_ent_ptr, host_ent_buffer_ptr,
			host_ent_buffer_length, required_length, error_text);

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetHostByNameBasic(const char *host_name, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	unsigned int *required_length, char *error_text)
{
	int           return_code = NPSL_SUCCESS;
	NPSL_HOSTENT *tmp_ent_ptr;
#if NPSL_HAS_GETHOSTBYNAME_R
	int           local_errno;
#endif // #if NPSL_HAS_GETHOSTBYNAME_R

	if (required_length != NULL)
		*required_length = 0;

	if (host_name == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'host_name' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!(*host_name)) {
		if (error_text != NULL)
			strcpy(error_text, "The 'host_name' parameter is an empty string.");
		return_code = NPSL_FAILURE;
	}
	else if ((return_code = NPSL_CheckHostEntParams(host_ent_ptr,
		host_ent_buffer_ptr, host_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		;
	else {
#if NPSL_HAS_GETHOSTBYNAME_R
		if ((tmp_ent_ptr = gethostbyname_r(host_name, host_ent_ptr,
			host_ent_buffer_ptr, ((int) host_ent_buffer_length),
			&local_errno)) == NULL) {
			if (error_text != NULL)
				NPSL_GetErrorString(local_errno, 0, NPSL_MAX_ERROR_TEXT,
					strcat(nstrcat(strcpy(error_text,
					"Unable to get host entry with 'gethostbyname_r()' for host '"),
					host_name, NPSL_MAX_ERROR_TEXT - 100), "'."));
			return_code = NPSL_SUPP_MapLastError();
		}
#else
		if ((tmp_ent_ptr = gethostbyname(host_name)) == NULL) {
			if (error_text != NULL)
				NPSL_AppendLastErrorString(0, NPSL_MAX_ERROR_TEXT,
					strcat(MLB::Utility::nstrcat(strcpy(error_text,
					"Unable to get host entry with 'gethostbyname()' for host '"),
					host_name, NPSL_MAX_ERROR_TEXT - 100), "'."));
			return_code = NPSL_SUPP_MapLastError();
		}
		else
			return_code = NPSL_CopyHostEntFlatBasic(tmp_ent_ptr, host_ent_ptr,
				host_ent_buffer_ptr, host_ent_buffer_length, required_length,
				error_text);
#endif // #if NPSL_HAS_GETHOSTBYNAME_R
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_GetHostByStringBasic(const char *host_string,
	NPSL_HOSTENT *host_ent_ptr, void *host_ent_buffer_ptr,
	unsigned int host_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int return_code;

	if (host_string == NULL) {
		if (error_text != NULL)
			strcpy(error_text, "The 'host_string' parameter is 'NULL'.");
		return_code = NPSL_FAILURE;
	}
	else if (!isdigit(*host_string))
		return_code = NPSL_GetHostByNameBasic(host_string, host_ent_ptr,
			host_ent_buffer_ptr, host_ent_buffer_length, required_length,
			error_text);
	else
		return_code = NPSL_GetHostByINetBasic(host_string, host_ent_ptr,
			host_ent_buffer_ptr, host_ent_buffer_length, required_length,
			error_text);

	return(return_code);
}
/* *********************************************************************** */

#ifdef TEST_MAIN

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

	fprintf(stderr, "Test routine for 'NPSL_GetHostByAddrBasic()'\n");
	fprintf(stderr, "                 'NPSL_GetHostByINetBasic()'\n");
	fprintf(stderr, "                 'NPSL_GetHostByNameBasic()'\n");
	fprintf(stderr, "                 'NPSL_GetHostByStringBasic()'\n");
	fprintf(stderr, "---- ------- --- -----------------------------\n\n");

	if (argc == 1)
		sprintf(error_text, "USAGE: %s { %s } [ { %s } . . . ]\n", argv[0],
			"<host-name> | <IP-address>", "<host-name> | <IP-address>");
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			printf("HOSTNAME: %s ---> ", argv[count_1]);
			printf("by %s ---> ",
				(!isdigit(*argv[count_1])) ? "name" : "address");
			if ((return_code = NPSL_GetHostByStringBasic(argv[count_1],
				&hostent_data, buffer, sizeof(buffer), NULL, error_text)) !=
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

