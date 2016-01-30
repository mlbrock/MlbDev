/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Resolves host information.

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
 
#include "npsli.h"

#include <memory.h>
#include <string.h>

/* *********************************************************************** */

/*	***********************************************************************	*/
int NPSL_ResolveHostToName(const char *host_string, char *out_host,
	char *error_text)
{
	int          return_code;
	NPSL_HOSTENT hent_data;
	char         buffer[8192];

	if ((return_code = NPSL_GetHostByString(host_string, &hent_data, buffer,
		sizeof(buffer), error_text)) == NPSL_SUCCESS) {
		if (out_host != NULL)
			strcpy(out_host, hent_data.h_name);
	}

	return(return_code);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_ResolveHostToAddrStruct(const char *host_string,
	struct in_addr *out_host, char *error_text)
{
	int          return_code;
	NPSL_HOSTENT hent_data;
	char         buffer[8192];

	if ((return_code = NPSL_GetHostByString(host_string, &hent_data, buffer,
		sizeof(buffer), error_text)) == NPSL_SUCCESS) {
		if (out_host != NULL)
			*out_host = *((struct in_addr *) hent_data.h_addr_list[0]);
	}

	return(return_code);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_ResolveHostToINet(const char *host_string, char *out_host,
	char *error_text)
{
	int          return_code;
	NPSL_HOSTENT hent_data;
	char         buffer[8192];

	if ((return_code = NPSL_GetHostByString(host_string, &hent_data, buffer,
		sizeof(buffer), error_text)) == NPSL_SUCCESS) {
		if (out_host != NULL)
			NPSL_AddrStructToINetBasic(((struct in_addr *)
				hent_data.h_addr_list[0]), out_host, error_text);
	}

	return(return_code);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_ResolveHostToULong(const char *host_string, unsigned long *out_host,
	char *error_text)
{
	int          return_code;
	NPSL_HOSTENT hent_data;
	char         buffer[8192];

	if ((return_code = NPSL_GetHostByString(host_string, &hent_data, buffer,
		sizeof(buffer), error_text)) == NPSL_SUCCESS) {
		if (out_host != NULL) {
			memset(out_host, '\0', sizeof(*out_host));
			memcpy(out_host, hent_data.h_addr_list[0], 4);
		}
	}

	return(return_code);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_ResolveHostBasic(const char *host_string, char *out_host_name,
	struct in_addr *out_host_in_addr, char *out_host_inet,
	unsigned long *out_host_ulong, char *error_text)
{
	int          return_code;
	NPSL_HOSTENT hent_data;
	char         buffer[8192];

	if ((return_code = NPSL_GetHostByString(host_string, &hent_data, buffer,
		sizeof(buffer), error_text)) == NPSL_SUCCESS) {
		if (out_host_name != NULL)
			strcpy(out_host_name, hent_data.h_name);
		if (out_host_in_addr != NULL)
			*out_host_in_addr = *((struct in_addr *) hent_data.h_addr_list[0]);
		if (out_host_inet != NULL)
			NPSL_AddrStructToINetBasic(((struct in_addr *)
				hent_data.h_addr_list[0]), out_host_inet, error_text);
		if (out_host_ulong != NULL) {
			memset(out_host_ulong, '\0', sizeof(*out_host_ulong));
			memcpy(out_host_ulong, hent_data.h_addr_list[0], 4);
		}
	}

	return(return_code);
}
/*	***********************************************************************	*/

