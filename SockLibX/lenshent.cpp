/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Determines the number and length of 'NPSL_HOSTENT'
								('struct hostent') structure 'h_aliases'
								and 'h_addr_list' members.

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
 
#include <string.h>
 
#include "npsli.h"

/* *********************************************************************** */

/*	***********************************************************************	*/
unsigned int NPSL_GetHostEntAddressCount(const NPSL_HOSTENT *hent_ptr)
{
	return(NPSL_GetHostEntAddressCountAndLength(hent_ptr, NULL));
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
unsigned int NPSL_GetHostEntAddressLength(const NPSL_HOSTENT *hent_ptr)
{
	unsigned int address_length;

	NPSL_GetHostEntAddressCountAndLength(hent_ptr, &address_length);

	return(address_length);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
unsigned int NPSL_GetHostEntAddressCountAndLength(const NPSL_HOSTENT *hent_ptr,
	unsigned int *address_length)
{
	const char   **in_address;
	unsigned int   address_count;
	unsigned int   tmp_length;

	address_length  = (address_length != NULL) ? address_length : &tmp_length;
	*address_length = 0;
	in_address      = ((const char **) hent_ptr->h_addr_list);
	address_count   = 1;

	while (*in_address != NULL) {
		in_address++;
		address_count++;
	}

	*address_length = (address_count - 1) * hent_ptr->h_length;

	return(address_count);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
unsigned int NPSL_GetHostEntAliasesCount(const NPSL_HOSTENT *hent_ptr)
{
	return(NPSL_GetHostEntAliasesCountAndLength(hent_ptr, NULL));
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
unsigned int NPSL_GetHostEntAliasesLength(const NPSL_HOSTENT *hent_ptr)
{
	unsigned int alias_length;

	NPSL_GetHostEntAliasesCountAndLength(hent_ptr, &alias_length);

	return(alias_length);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
unsigned int NPSL_GetHostEntAliasesCountAndLength(const NPSL_HOSTENT *hent_ptr,
	unsigned int *alias_length)
{
	const char   **in_alias;
	unsigned int   alias_count;
	unsigned int   tmp_length;

	alias_length  = (alias_length != NULL) ? alias_length : &tmp_length;
	*alias_length = 0;
	in_alias      = ((const char **) hent_ptr->h_aliases);
	alias_count   = 1;

	while (*in_alias != NULL) {
		*alias_length += COMPAT_CAST_static(unsigned int, strlen(*in_alias)) + 1;
		in_alias++;
		alias_count++;
	}

	return(alias_count);
}
/*	***********************************************************************	*/

