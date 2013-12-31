/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Initializes 'NPSL_SERVENT' ('struct servent')
								structures.

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

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_InitServEnt

	SYNOPSIS		:	void NPSL_InitServEnt(ptr);

						NPSL_SERVENT *ptr;

	DESCRIPTION	:	Initializes the members of a ''NPSL_SERVENT'' structure
						to their default values.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``ptr`` points to the ''NPSL_SERVENT'' structure
						to be initialized by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_InitServEntList
						NPSL_CopyServEnt
						NPSL_FreeServEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2003 Michael L. Brock

	OUTPUT INDEX:	NPSL_InitServEnt
						Initialization Functions:Network Programming Support Functions:NPSL_InitServEnt
						Network Programming Support Functions:Initialization Functions:NPSL_InitServEnt

	PUBLISH XREF:	NPSL_InitServEnt

	PUBLISH NAME:	NPSL_InitServEnt

	ENTRY CLASS	:	Initialization Functions

EOH */
/*	***********************************************************************	*/
void NPSL_InitServEnt(NPSL_SERVENT *ptr)
{
	memset(ptr, '\0', sizeof(*ptr));

	ptr->s_name    = NULL;
	ptr->s_aliases = NULL;
	ptr->s_port    = 0;
	ptr->s_proto   = NULL;
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_InitServEntList

	SYNOPSIS		:	void NPSL_InitServEntList(in_list);

						unsigned int  in_count;

						NPSL_SERVENT *in_list;

	DESCRIPTION	:	Initializes the members of the elements of a list of
						''NPSL_SERVENT'' structures to their default values.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_count`` is the ''unsigned int'' which contains the
						number of elements in the list of ''NPSL_SERVENT''
						structures to be initialized by this function.

						(.) ``in_list`` is a the list of ''NPSL_SERVENT''
						structures to be initialized by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_InitServEnt
						NPSL_CopyServEntList
						NPSL_FreeServEntList

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2003 Michael L. Brock

	OUTPUT INDEX:	NPSL_InitServEntList
						Initialization Functions:Network Programming Support Functions:NPSL_InitServEntList
						Network Programming Support Functions:Initialization Functions:NPSL_InitServEntList

	PUBLISH XREF:	NPSL_InitServEntList

	PUBLISH NAME:	NPSL_InitServEntList

	ENTRY CLASS	:	Initialization Functions

EOH */
/*	***********************************************************************	*/
void NPSL_InitServEntList(unsigned int in_count, NPSL_SERVENT *in_list)
{
	while (in_count)
		NPSL_InitServEnt(in_list + --in_count);
}
/*	***********************************************************************	*/

