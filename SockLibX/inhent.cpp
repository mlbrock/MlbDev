/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Initializes 'NPSL_HOSTENT' ('struct hostent')
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

	NAME			:	NPSL_InitHostEnt

	SYNOPSIS		:	void NPSL_InitHostEnt(ptr);

						NPSL_HOSTENT *ptr;

	DESCRIPTION	:	Initializes the members of a ''NPSL_HOSTENT'' structure
						to their default values.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``ptr`` points to the ''NPSL_HOSTENT'' structure
						to be initialized by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_InitHostEntList
						NPSL_CopyHostEnt
						NPSL_FreeHostEnt

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2015 Michael L. Brock

	OUTPUT INDEX:	NPSL_InitHostEnt
						Initialization Functions:Network Programming Support Functions:NPSL_InitHostEnt
						Network Programming Support Functions:Initialization Functions:NPSL_InitHostEnt

	PUBLISH XREF:	NPSL_InitHostEnt

	PUBLISH NAME:	NPSL_InitHostEnt

	ENTRY CLASS	:	Initialization Functions

EOH */
/*	***********************************************************************	*/
void NPSL_InitHostEnt(NPSL_HOSTENT *ptr)
{
	memset(ptr, '\0', sizeof(*ptr));

	ptr->h_name      = NULL;
	ptr->h_aliases   = NULL;
	ptr->h_addrtype  = 0;
	ptr->h_length    = 0;
	ptr->h_addr_list = NULL;
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	BOH

	NAME			:	NPSL_InitHostEntList

	SYNOPSIS		:	void NPSL_InitHostEntList(in_list);

						unsigned int  in_count;

						NPSL_HOSTENT *in_list;

	DESCRIPTION	:	Initializes the members of the elements of a list of
						''NPSL_HOSTENT'' structures to their default values.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_count`` is the ''unsigned int'' which contains the
						number of elements in the list of ''NPSL_HOSTENT''
						structures to be initialized by this function.

						(.) ``in_list`` is a the list of ''NPSL_HOSTENT''
						structures to be initialized by this function.

	RETURNS		:	Void.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_InitHostEnt
						NPSL_CopyHostEntList
						NPSL_FreeHostEntList

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2015 Michael L. Brock

	OUTPUT INDEX:	NPSL_InitHostEntList
						Initialization Functions:Network Programming Support Functions:NPSL_InitHostEntList
						Network Programming Support Functions:Initialization Functions:NPSL_InitHostEntList

	PUBLISH XREF:	NPSL_InitHostEntList

	PUBLISH NAME:	NPSL_InitHostEntList

	ENTRY CLASS	:	Initialization Functions

EOH */
/*	***********************************************************************	*/
void NPSL_InitHostEntList(unsigned int in_count, NPSL_HOSTENT *in_list)
{
	while (in_count)
		NPSL_InitHostEnt(in_list + --in_count);
}
/*	***********************************************************************	*/

