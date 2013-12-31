/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Frees the memory associated with 'NPSL_SERVENT'
								('struct servent') structure.

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

	NAME			:	NPSL_FreeServEnt

	SYNOPSIS		:	void NPSL_FreeServEnt(ptr);

						NPSL_SERVENT *ptr;

	DESCRIPTION	:	Frees the members of a ''NPSL_SERVENT'' structure.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``ptr`` points to the ''NPSL_SERVENT'' structure
						to be freed by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_FreeServEntList
						NPSL_InitServEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2003 Michael L. Brock

	OUTPUT INDEX:	NPSL_FreeServEnt
						Memory Free Functions:Network Programming Support Functions:NPSL_FreeServEnt
						Network Programming Support Functions:Memory Free Functions:NPSL_FreeServEnt

	PUBLISH XREF:	NPSL_FreeServEnt

	PUBLISH NAME:	NPSL_FreeServEnt

	ENTRY CLASS	:	Memory Free Functions

EOH */
/*	***********************************************************************	*/
void NPSL_FreeServEnt(NPSL_SERVENT *ptr)
{
	char **tmp_ptr;

	if (ptr->s_name != NULL)
		free(ptr->s_name);

	if (ptr->s_aliases != NULL) {
		tmp_ptr = ptr->s_aliases;
		while (*tmp_ptr != NULL) {
			free(*tmp_ptr);
			tmp_ptr++;
		}
		free(ptr->s_aliases);
	}

	if (ptr->s_proto != NULL)
		free(ptr->s_proto);

	NPSL_InitServEnt(ptr);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_FreeServEntList

	SYNOPSIS		:	void NPSL_FreeServEntList(in_count, in_list);

						unsigned int  *in_count;

						NPSL_SERVENT **in_list;

	DESCRIPTION	:	Frees a list of ''NPSL_SERVENT'' structures.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_count`` is a pointer to the ''unsigned int''
						which contains the number of elements in the list of
						''NPSL_SERVENT'' structures to be freed by this function.

						(.) ``in_list`` is a pointer to the list of
						''NPSL_SERVENT'' structures to be freed by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_FreeServEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2003 Michael L. Brock

	OUTPUT INDEX:	NPSL_FreeServEntList
						Memory Free Functions:Network Programming Support Functions:NPSL_FreeServEntList
						Network Programming Support Functions:Memory Free Functions:NPSL_FreeServEntList

	PUBLISH XREF:	NPSL_FreeServEntList

	PUBLISH NAME:	NPSL_FreeServEntList

	ENTRY CLASS	:	Memory Free Functions

EOH */
/*	***********************************************************************	*/
void NPSL_FreeServEntList(unsigned int *in_count, NPSL_SERVENT **in_list)
{
	while (*in_count)
		NPSL_FreeServEnt(*in_list + --(*in_count));

	if (*in_list != NULL)
		free(*in_list);

	*in_count = 0;
	*in_list  = NULL;
}
/*	***********************************************************************	*/

