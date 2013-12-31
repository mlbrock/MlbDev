/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Frees the memory associated with 'NPSL_HOSTENT'
								('struct hostent') structure.

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

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_FreeHostEnt

	SYNOPSIS		:	void NPSL_FreeHostEnt(ptr);

						NPSL_HOSTENT *ptr;

	DESCRIPTION	:	Frees the members of a ''NPSL_HOSTENT'' structure.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``ptr`` points to the ''NPSL_HOSTENT'' structure
						to be freed by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_FreeHostEntList
						NPSL_InitHostEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2003 Michael L. Brock

	OUTPUT INDEX:	NPSL_FreeHostEnt
						Memory Free Functions:Network Programming Support Functions:NPSL_FreeHostEnt
						Network Programming Support Functions:Memory Free Functions:NPSL_FreeHostEnt

	PUBLISH XREF:	NPSL_FreeHostEnt

	PUBLISH NAME:	NPSL_FreeHostEnt

	ENTRY CLASS	:	Memory Free Functions

EOH */
/*	***********************************************************************	*/
void NPSL_FreeHostEnt(NPSL_HOSTENT *ptr)
{
	char **tmp_ptr;

	if (ptr->h_name != NULL)
		free(ptr->h_name);

	if (ptr->h_aliases != NULL) {
		tmp_ptr = ptr->h_aliases;
		while (*tmp_ptr != NULL) {
			free(*tmp_ptr);
			tmp_ptr++;
		}
		free(ptr->h_aliases);
	}

	if (ptr->h_addr_list != NULL) {
		tmp_ptr = ptr->h_addr_list;
		while (*tmp_ptr != NULL) {
			free(*tmp_ptr);
			tmp_ptr++;
		}
		free(ptr->h_addr_list);
	}

	NPSL_InitHostEnt(ptr);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_FreeHostEntList

	SYNOPSIS		:	void NPSL_FreeHostEntList(in_count, in_list);

						unsigned int  *in_count;

						NPSL_HOSTENT **in_list;

	DESCRIPTION	:	Frees a list of ''NPSL_HOSTENT'' structures.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_count`` is a pointer to the ''unsigned int''
						which contains the number of elements in the list of
						''NPSL_HOSTENT'' structures to be freed by this function.

						(.) ``in_list`` is a pointer to the list of
						''NPSL_HOSTENT'' structures to be freed by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_FreeHostEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2003 Michael L. Brock

	OUTPUT INDEX:	NPSL_FreeHostEntList
						Memory Free Functions:Network Programming Support Functions:NPSL_FreeHostEntList
						Network Programming Support Functions:Memory Free Functions:NPSL_FreeHostEntList

	PUBLISH XREF:	NPSL_FreeHostEntList

	PUBLISH NAME:	NPSL_FreeHostEntList

	ENTRY CLASS	:	Memory Free Functions

EOH */
/*	***********************************************************************	*/
void NPSL_FreeHostEntList(unsigned int *in_count, NPSL_HOSTENT **in_list)
{
	while (*in_count)
		NPSL_FreeHostEnt(*in_list + --(*in_count));

	if (*in_list != NULL)
		free(*in_list);

	*in_count = 0;
	*in_list  = NULL;
}
/*	***********************************************************************	*/

