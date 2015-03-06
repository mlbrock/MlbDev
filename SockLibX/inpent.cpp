/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Initializes 'NPSL_PROTOENT' ('struct protoent')
								structures.

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
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_InitProtoEnt

	SYNOPSIS		:	void NPSL_InitProtoEnt(ptr);

						NPSL_PROTOENT *ptr;

	DESCRIPTION	:	Initializes the members of a ''NPSL_PROTOENT'' structure
						to their default values.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``ptr`` points to the ''NPSL_PROTOENT'' structure
						to be initialized by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_InitProtoEntList
						NPSL_CopyProtoEnt
						NPSL_FreeProtoEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2015 Michael L. Brock

	OUTPUT INDEX:	NPSL_InitProtoEnt
						Initialization Functions:Network Programming Support Functions:NPSL_InitProtoEnt
						Network Programming Support Functions:Initialization Functions:NPSL_InitProtoEnt

	PUBLISH XREF:	NPSL_InitProtoEnt

	PUBLISH NAME:	NPSL_InitProtoEnt

	ENTRY CLASS	:	Initialization Functions

EOH */
/*	***********************************************************************	*/
void NPSL_InitProtoEnt(NPSL_PROTOENT *ptr)
{
	memset(ptr, '\0', sizeof(*ptr));

	ptr->p_name    = NULL;
	ptr->p_aliases = NULL;
	ptr->p_proto   = 0;
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_InitProtoEntList

	SYNOPSIS		:	void NPSL_InitProtoEntList(in_list);

						unsigned int   in_count;

						NPSL_PROTOENT *in_list;

	DESCRIPTION	:	Initializes the members of the elements of a list of
						''NPSL_PROTOENT'' structures to their default values.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_count`` is the ''unsigned int'' which contains the
						number of elements in the list of ''NPSL_PROTOENT''
						structures to be initialized by this function.

						(.) ``in_list`` is a the list of ''NPSL_PROTOENT''
						structures to be initialized by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_InitProtoEnt
						NPSL_CopyProtoEntList
						NPSL_FreeProtoEntList

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2015 Michael L. Brock

	OUTPUT INDEX:	NPSL_InitProtoEntList
						Initialization Functions:Network Programming Support Functions:NPSL_InitProtoEntList
						Network Programming Support Functions:Initialization Functions:NPSL_InitProtoEntList

	PUBLISH XREF:	NPSL_InitProtoEntList

	PUBLISH NAME:	NPSL_InitProtoEntList

	ENTRY CLASS	:	Initialization Functions

EOH */
/*	***********************************************************************	*/
void NPSL_InitProtoEntList(unsigned int in_count, NPSL_PROTOENT *in_list)
{
	while (in_count)
		NPSL_InitProtoEnt(in_list + --in_count);
}
/*	***********************************************************************	*/

