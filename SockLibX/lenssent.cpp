/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Determines the number and length of 'NPSL_SERVENT'
								('struct servent') structure 'p_aliases' members.

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

#include <string.h>
 
#include "npsli.h"

/* *********************************************************************** */

/*	***********************************************************************	*/
unsigned int NPSL_GetServEntAliasesCount(const NPSL_SERVENT *sent_ptr)
{
	return(NPSL_GetServEntAliasesCountAndLength(sent_ptr, NULL));
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
unsigned int NPSL_GetServEntAliasesLength(const NPSL_SERVENT *sent_ptr)
{
	unsigned int alias_length;

	NPSL_GetServEntAliasesCountAndLength(sent_ptr, &alias_length);

	return(alias_length);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
unsigned int NPSL_GetServEntAliasesCountAndLength(const NPSL_SERVENT *sent_ptr,
	unsigned int *alias_length)
{
	const char   **in_alias;
	unsigned int   alias_count;
	unsigned int   tmp_length;

	alias_length  = (alias_length != NULL) ? alias_length : &tmp_length;
	*alias_length = 0;
	in_alias      = ((const char **) sent_ptr->s_aliases);
	alias_count   = 1;

	while (*in_alias != NULL) {
		*alias_length += COMPAT_CAST_static(unsigned int, strlen(*in_alias)) + 1;
		in_alias++;
		alias_count++;
	}

	return(alias_count);
}
/*	***********************************************************************	*/

