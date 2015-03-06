/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Frees the memory associated with 'NPSL_PROTOENT'
								('struct protoent') structure.

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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_FreeProtoEnt

	SYNOPSIS		:	void NPSL_FreeProtoEnt(ptr);

						NPSL_PROTOENT *ptr;

	DESCRIPTION	:	Frees the members of a ''NPSL_PROTOENT'' structure.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``ptr`` points to the ''NPSL_PROTOENT'' structure
						to be freed by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_FreeProtoEntList
						NPSL_InitProtoEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2015 Michael L. Brock

	OUTPUT INDEX:	NPSL_FreeProtoEnt
						Memory Free Functions:Network Programming Support Functions:NPSL_FreeProtoEnt
						Network Programming Support Functions:Memory Free Functions:NPSL_FreeProtoEnt

	PUBLISH XREF:	NPSL_FreeProtoEnt

	PUBLISH NAME:	NPSL_FreeProtoEnt

	ENTRY CLASS	:	Memory Free Functions

EOH */
/*	***********************************************************************	*/
void NPSL_FreeProtoEnt(NPSL_PROTOENT *ptr)
{
	char **tmp_ptr;

	if (ptr->p_name != NULL)
		free(ptr->p_name);

	if (ptr->p_aliases != NULL) {
		tmp_ptr = ptr->p_aliases;
		while (*tmp_ptr != NULL) {
			free(*tmp_ptr);
			tmp_ptr++;
		}
		free(ptr->p_aliases);
	}

	NPSL_InitProtoEnt(ptr);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_FreeProtoEntList

	SYNOPSIS		:	void NPSL_FreeProtoEntList(in_count, in_list);

						unsigned int   *in_count;

						NPSL_PROTOENT **in_list;

	DESCRIPTION	:	Frees a list of ''NPSL_PROTOENT'' structures.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_count`` is a pointer to the ''unsigned int''
						which contains the number of elements in the list of
						''NPSL_PROTOENT'' structures to be freed by this function.

						(.) ``in_list`` is a pointer to the list of
						''NPSL_PROTOENT'' structures to be freed by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_FreeProtoEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2015 Michael L. Brock

	OUTPUT INDEX:	NPSL_FreeProtoEntList
						Memory Free Functions:Network Programming Support Functions:NPSL_FreeProtoEntList
						Network Programming Support Functions:Memory Free Functions:NPSL_FreeProtoEntList

	PUBLISH XREF:	NPSL_FreeProtoEntList

	PUBLISH NAME:	NPSL_FreeProtoEntList

	ENTRY CLASS	:	Memory Free Functions

EOH */
/*	***********************************************************************	*/
void NPSL_FreeProtoEntList(unsigned int *in_count, NPSL_PROTOENT **in_list)
{
	while (*in_count)
		NPSL_FreeProtoEnt(*in_list + --(*in_count));

	if (*in_list != NULL)
		free(*in_list);

	*in_count = 0;
	*in_list  = NULL;
}
/*	***********************************************************************	*/

