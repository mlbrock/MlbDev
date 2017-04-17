/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Performs a copy of a 'NPSL_SERVENT' ('struct
								servent') structure.

	Revision History	:	1993-04-12 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2017.
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

   NAME        :	NPSL_CopyServEntFlat

   SYNOPSIS    :	return_code = NPSL_CopyServEntFlat(in_serv_ent_ptr,
							out_serv_ent_ptr, serv_ent_buffer_ptr,
							serv_ent_buffer_length, error_text);

						int                 return_code;

						const NPSL_SERVENT *in_serv_ent_ptr;

						NPSL_SERVENT       *out_serv_ent_ptr;

						void               *serv_ent_buffer_ptr;

						unsigned int        serv_ent_buffer_length;

						char               *error_text;

   DESCRIPTION :	Copies a single ''NPSL_SERVENT'' structures.

						Note that in contrast to the function ``NPSL_CopyServEnt``,
						this function performs a shallow-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_serv_ent_ptr`` is a pointer to the ''NPSL_SERVENT''
						structure which is to be copied by this function.

						(.) ``out_serv_ent_ptr`` is a pointer to the ''NPSL_SERVENT''
						structure into which the copy is to be performed.

						(.) ``serv_ent_buffer_ptr`` points to a buffer into which
						this function is to place the data contained in the string
						members of the ''NPSL_SERVENT'' structure.

						(.) ``serv_ent_buffer_length`` is the length of the memory
						area to which the ``serv_ent_buffer_ptr`` parameter points.

						(.) ``error_text`` points to a string into which this
						function will place text descriptive of any error which
						might occur.

						(-) The string allocated for this purpose should be at least
						''NPSL_MAX_ERROR_TEXT'' characters in length.

   RETURNS     :	Returns from this function are as follow:

						(.) ''NPSL_SUCCESS'' if the function completes without
						error. ''NPSL_SUCCESS'' is guaranteed to be equal to
						zero (0).

						(.) One of the defined non-zero NPSL return codes
						indicative of error.

   NOTES       :	This function calculates the total length of the buffer to
						which the ``serv_ent_buffer_ptr`` parameter points to ensure
						that it is long enough for the copy. If it is not, the copy
						will not be performed and an error will be returned by this
						function.

   CAVEATS     :  

   SEE ALSO    :  NPSL_CopyServEnt
						NPSL_CopyServEntFlatBasic

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyServEntFlat
						Copy Functions:Network Programming Support Functions:NPSL_CopyServEntFlat
						Network Programming Support Functions:Copy Functions:NPSL_CopyServEntFlat

   PUBLISH XREF:	NPSL_CopyServEntFlat

   PUBLISH NAME:	NPSL_CopyServEntFlat

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyServEntFlat(const NPSL_SERVENT *in_serv_ent_ptr,
	NPSL_SERVENT *out_serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, char *error_text)
{
	return(NPSL_CopyServEntFlatBasic(in_serv_ent_ptr, out_serv_ent_ptr,
		serv_ent_buffer_ptr, serv_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyServEntFlatBasic

   SYNOPSIS    :	return_code = NPSL_CopyServEntFlatBasic(in_serv_ent_ptr,
							out_serv_ent_ptr, serv_ent_buffer_ptr,
							serv_ent_buffer_length, required_length, error_text);

						int                 return_code;

						const NPSL_SERVENT *in_serv_ent_ptr;

						NPSL_SERVENT       *out_serv_ent_ptr;

						void               *serv_ent_buffer_ptr;

						unsigned int        serv_ent_buffer_length;

						unsigned int       *required_length;

						char               *error_text;

   DESCRIPTION :	Copies a single ''NPSL_SERVENT'' structures.

						Note that in contrast to the function ``NPSL_CopyServEnt``,
						this function performs a shallow-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_serv_ent_ptr`` is a pointer to the ''NPSL_SERVENT''
						structure which is to be copied by this function.

						(.) ``out_serv_ent_ptr`` is a pointer to the ''NPSL_SERVENT''
						structure into which the copy is to be performed.

						(.) ``serv_ent_buffer_ptr`` points to a buffer into which
						this function is to place the data contained in the string
						members of the ''NPSL_SERVENT'' structure.

						(.) ``serv_ent_buffer_length`` is the length of the memory
						area to which the ``serv_ent_buffer_ptr`` parameter points.

						(.) ``required_length`` is a pointer to an ''unsigned int''
						into which this function will place the total length
						required to construct a flat copy of the structure.

						(-) Note that if this parameter is ''NULL'', it will not
						bedereferenced by this function.

						(-) If this parameter is not ''NULL'', the length required
						for the copy will be placed into the ''unsigned int'' into
						which it points regardless of the success or failure of
						the copy operation.

						(.) ``error_text`` points to a string into which this
						function will place text descriptive of any error which
						might occur.

						(-) The string allocated for this purpose should be at least
						''NPSL_MAX_ERROR_TEXT'' characters in length.

   RETURNS     :	Returns from this function are as follow:

						(.) ''NPSL_SUCCESS'' if the function completes without
						error. ''NPSL_SUCCESS'' is guaranteed to be equal to
						zero (0).

						(.) One of the defined non-zero NPSL return codes
						indicative of error.

   NOTES       :	This function calculates the total length of the buffer to
						which the ``serv_ent_buffer_ptr`` parameter points to ensure
						that it is long enough for the copy. If it is not, the copy
						will not be performed and an error will be returned by this
						function.

   CAVEATS     :  

   SEE ALSO    :  NPSL_CopyServEnt
						NPSL_CopyServEntFlat

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyServEntFlatBasic
						Copy Functions:Network Programming Support Functions:NPSL_CopyServEntFlatBasic
						Network Programming Support Functions:Copy Functions:NPSL_CopyServEntFlatBasic

   PUBLISH XREF:	NPSL_CopyServEntFlatBasic

   PUBLISH NAME:	NPSL_CopyServEntFlatBasic

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyServEntFlatBasic(const NPSL_SERVENT *in_serv_ent_ptr,
	NPSL_SERVENT *out_serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int            return_code = NPSL_SUCCESS;
	unsigned int   count_1;
	unsigned int   required_size;
	const char   **in_alias;
	unsigned int   alias_count;
	unsigned int   alias_length;
	char          *str_ptr;
	unsigned int   str_length;
	NPSL_SERVENT   tmp_serv_ent;

	if (required_length != NULL)
		*required_length = 0;

	if ((return_code = NPSL_CheckServEntParams(out_serv_ent_ptr,
		serv_ent_buffer_ptr, serv_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		goto EXIT_FUNCTION;

	in_alias       = ((const char **) in_serv_ent_ptr->s_aliases);
	alias_count    = 1;
	alias_length   = 0;

	while (*in_alias != NULL) {
		alias_length += COMPAT_CAST_static(unsigned int, strlen(*in_alias)) + 1;
		in_alias++;
		alias_count++;
	}

	required_size = sizeof(NPSL_SERVENT) +
		COMPAT_CAST_static(unsigned int, strlen(in_serv_ent_ptr->s_name)) + 1 +
		(alias_count * sizeof(char *)) +
		COMPAT_CAST_static(unsigned int, strlen(in_serv_ent_ptr->s_proto)) + 1 +
		alias_length;

	if (required_size > serv_ent_buffer_length) {
		if (error_text != NULL)
			sprintf(error_text, "%s %s (%u) %s (%u).",
				"The total length required to store the serv entry",
				"'NPSL_SERVENT' data", required_size,
				"is less than the allocated 'NPSL_SERVENT' buffer length",
				serv_ent_buffer_length);
		if (required_length != NULL)
			*required_length = required_size;
		return_code = NPSL_FAILURE;
		goto EXIT_FUNCTION;
	}

	in_alias   = ((const char **) in_serv_ent_ptr->s_aliases);
	str_ptr    = ((char *) serv_ent_buffer_ptr) + sizeof(NPSL_SERVENT) + 
		(alias_count * sizeof(char *));

	tmp_serv_ent           = *in_serv_ent_ptr;
	tmp_serv_ent.s_aliases = ((char **)
		(((char *) serv_ent_buffer_ptr) + sizeof(NPSL_SERVENT)));

	tmp_serv_ent.s_name = str_ptr;
	memcpy(str_ptr, in_serv_ent_ptr->s_name,
		str_length =
			(COMPAT_CAST_static(unsigned int,
			strlen(in_serv_ent_ptr->s_name)) + 1));
	str_ptr += str_length;

	tmp_serv_ent.s_proto = str_ptr;
	memcpy(str_ptr, in_serv_ent_ptr->s_proto,
		str_length =
			(COMPAT_CAST_static(unsigned int,
			strlen(in_serv_ent_ptr->s_proto)) + 1));
	str_ptr += str_length;

	for (count_1 = 0; count_1 < alias_count; count_1++) {
		if (in_serv_ent_ptr->s_aliases[count_1] != NULL) {
			tmp_serv_ent.s_aliases[count_1] = str_ptr;
			memcpy(str_ptr, in_serv_ent_ptr->s_aliases[count_1],
				str_length = (COMPAT_CAST_static(unsigned int,
					strlen(in_serv_ent_ptr->s_aliases[count_1])) + 1));
			str_ptr += str_length;
		}
		else
			tmp_serv_ent.s_aliases[count_1] = NULL;
	}

	*out_serv_ent_ptr                       = tmp_serv_ent;
	*((NPSL_SERVENT *) serv_ent_buffer_ptr) = tmp_serv_ent;

EXIT_FUNCTION:

	return(return_code);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyServEnt

   SYNOPSIS    :	return_code = NPSL_CopyServEnt(in_ptr, out_ptr,
							error_text);

						int                 return_code;

						const NPSL_SERVENT *in_ptr;

						NPSL_SERVENT       *out_ptr;

						char               *error_text;

   DESCRIPTION :	Copies a single ''NPSL_SERVENT'' structures.

						Note that in contrast to the functions
						``NPSL_CopyServEntFlat`` and ``NPSL_CopyServEntFlatBasic``,
						this function performs a deep-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_ptr`` is a pointer to the ''NPSL_SERVENT''
						structure which is to be copied by this function.

						(.) ``out_ptr`` is a pointer to the ''NPSL_SERVENT''
						structure into which the copy is to be performed.

						(.) ``error_text`` points to a string into which this
						function will place text descriptive of any error which
						might occur.

						(-) The string allocated for this purpose should be at least
						''NPSL_MAX_ERROR_TEXT'' characters in length.

   RETURNS     :	Returns from this function are as follow:

						(.) ''NPSL_SUCCESS'' if the function completes without
						error. ''NPSL_SUCCESS'' is guaranteed to be equal to
						zero (0).

						(.) One of the defined non-zero NPSL return codes
						indicative of error.

   NOTES       :	

   CAVEATS     :  

   SEE ALSO    :	NPSL_CopyServEntList
						NPSL_CopyServEntFlat
						NPSL_CopyServEntFlatBasic

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyServEnt
						Copy Functions:Network Programming Support Functions:NPSL_CopyServEnt
						Network Programming Support Functions:Copy Functions:NPSL_CopyServEnt

   PUBLISH XREF:	NPSL_CopyServEnt

   PUBLISH NAME:	NPSL_CopyServEnt

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyServEnt(const NPSL_SERVENT *in_serv_ent_ptr,
	NPSL_SERVENT *out_serv_ent_ptr, char *error_text)
{
	int            return_code       = NPSL_SUCCESS;
	unsigned int   tmp_alias_count   = 0;
	unsigned int   alias_count       = 1;
	unsigned int   alias_length      = 0;
	unsigned int   count_1;
	const char   **in_alias;

	in_alias = ((const char **) in_serv_ent_ptr->s_aliases);

	while (*in_alias != NULL) {
		alias_length += COMPAT_CAST_static(unsigned int, strlen(*in_alias)) + 1;
		in_alias++;
		alias_count++;
	}

	NPSL_InitServEnt(out_serv_ent_ptr);

	out_serv_ent_ptr->s_port = in_serv_ent_ptr->s_port;

	if ((in_serv_ent_ptr->s_name != NULL) && ((out_serv_ent_ptr->s_name =
		strdup(in_serv_ent_ptr->s_name)) == NULL)) {
		MLB::NPSLI_Internal::STR_AllocMsgItem(
			COMPAT_CAST_static(unsigned int, strlen(in_serv_ent_ptr->s_name)) + 1,
			error_text, "Unable to copy the 'NPSL_SERVENT' member 's_name'");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else if ((in_serv_ent_ptr->s_proto != NULL) && ((out_serv_ent_ptr->s_proto =
		strdup(in_serv_ent_ptr->s_proto)) == NULL)) {
		MLB::NPSLI_Internal::STR_AllocMsgItem(
			COMPAT_CAST_static(unsigned int, strlen(in_serv_ent_ptr->s_proto)) + 1,
			error_text, "Unable to copy the 'NPSL_SERVENT' member 's_proto'");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else if (MLB::NPSLI_Internal::meml_allocate(&tmp_alias_count,
		((void ***) &out_serv_ent_ptr->s_aliases), 0, alias_count) !=
		MLB::NPSLI_Internal::STRFUNCS_SUCCESS) {
		MLB::NPSLI_Internal::STR_AllocMsgPtrList(alias_count, alias_length,
			error_text,
			"Unable to copy the 'NPSL_SERVENT' member 's_aliases' pointer list");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else {
		for (count_1 = 0; count_1 < (alias_count - 1); count_1++) {
			if ((out_serv_ent_ptr->s_aliases[count_1] =
				strdup(in_serv_ent_ptr->s_aliases[count_1])) == NULL) {
				MLB::NPSLI_Internal::STR_AllocMsgPtrList(alias_count,
					alias_length, error_text,
					"Unable to copy the 'NPSL_SERVENT' member 's_aliases' list");
				return_code = NPSL_MEMORY_FAILURE;
				break;
			}
		}
	}

	if (return_code != NPSL_SUCCESS)
		NPSL_FreeServEnt(out_serv_ent_ptr);

	return(return_code);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyServEntList

   SYNOPSIS    :	return_code = NPSL_CopyServEntList(in_count, in_list,
							out_count, out_list);

						int                  return_code;

						unsigned int         in_count;

						const NPSL_SERVENT  *in_list;

						unsigned int        *out_count;

						NPSL_SERVENT       **out_list;

						char                *error_text;

   DESCRIPTION :	Copies an array of ''NPSL_SERVENT'' structures.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_count`` is the number of elements in the array
						``in_list``.

						(.) ``in_list`` is the array of ''NPSL_SERVENT''
						structures which is to be copied by this function.

						(.) ``out_count`` is a pointer to an ''unsigned int'' into
						which this function will place the number of elements
						copied.

						(.) ``out_list`` is a pointer to the array of
						''NPSL_SERVENT'' structures which will be allocated
						and copied by this function.

						(.) ``error_text`` points to a string into which this
						function will place text descriptive of any error which
						might occur.

						(-) The string allocated for this purpose should be at least
						''NPSL_MAX_ERROR_TEXT'' characters in length.

   RETURNS     :	Returns from this function are as follow:

						(.) ''NPSL_SUCCESS'' if the function completes without
						error. ''NPSL_SUCCESS'' is guaranteed to be equal to
						zero (0).

						(.) One of the defined non-zero NPSL return codes
						indicative of error.

   NOTES       :	

   CAVEATS     :  

   SEE ALSO    :	NPSL_CopyServEnt

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyServEntList
						Copy Functions:Network Programming Support Functions:NPSL_CopyServEntList
						Network Programming Support Functions:Copy Functions:NPSL_CopyServEntList

   PUBLISH XREF:	NPSL_CopyServEntList

   PUBLISH NAME:	NPSL_CopyServEntList

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyServEntList(unsigned int in_count, const NPSL_SERVENT *in_list,
	unsigned int *out_count, NPSL_SERVENT **out_list, char *error_text)
{
	int          return_code = NPSL_SUCCESS;
	unsigned int count_1;

	*out_count = 0;
	*out_list  = NULL;

	if (in_count) {
		if ((*out_list = ((NPSL_SERVENT *) calloc(in_count,
			sizeof(**out_list)))) == NULL) {
			MLB::NPSLI_Internal::STR_AllocMsgList(in_count, sizeof(**out_list),
				error_text,
				"Unable to allocate memory for the 'NPSL_SERVENT' list");
			return_code = NPSL_MEMORY_FAILURE;
		}
		else {
			*out_count = in_count;
			NPSL_InitServEntList(*out_count, *out_list);
			for (count_1 = 0; count_1 < in_count; count_1++) {
				if ((return_code = NPSL_CopyServEnt(in_list + count_1,
					*out_list + count_1, error_text)) != NPSL_SUCCESS) {
					NPSL_FreeServEntList(out_count, out_list);
					break;
				}
			}
		}
	}

	return(return_code);
}
/*	***********************************************************************	*/

#ifdef TEST_MAIN

COMPAT_FN_DECL(int main, (int argc, char **argv));

int main(int argc, char **argv)
{
	int            return_code = NPSL_FAILURE;
	unsigned int   count_1;
	char           buffer[1024];
	NPSL_SERVENT   servent_flat;
	NPSL_SERVENT   servent_copy;
	char         **tmp_list;
	char           error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test routine for 'NPSL_CopyServEnt()'\n");
	fprintf(stderr, "---- ------- --- --------------------\n\n");

	if (argc != 3)
		sprintf(error_text, "USAGE: %s %s\n", argv[0],
			"{<service-name> | <service-port>} <service-protocol>");
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		printf("SERVICE NAME OR PORT: %s\n", argv[1]);
		printf("SERVICE PROTOCOL    : %s\n", argv[2]);
		printf("                    : by %s ---> ",
			(!isdigit(*argv[1])) ? "name" : "port");
		if ((return_code = NPSL_GetServByString(argv[1], argv[2],
			&servent_flat, buffer, sizeof(buffer), error_text)) == NPSL_SUCCESS) {
			if ((return_code = NPSL_CopyServEnt(&servent_flat, &servent_copy,
				error_text)) == NPSL_SUCCESS) {
				printf("DONE\n");
				printf("                    : Service Name    : %s\n",
					servent_copy.s_name);
				printf("                    : Service Port    : %d\n",
					((int) ntohs(servent_copy.s_port)));
				printf("                    : Service Protocol: %s\n",
					servent_copy.s_proto);
				if ((tmp_list = servent_copy.s_aliases)[0] != NULL) {
					count_1 = 0;
					while (*tmp_list != NULL) {
						printf("                    : Alias %8u: %s\n", count_1++,
							*tmp_list);
						tmp_list++;
					}
				}
				NPSL_FreeServEnt(&servent_copy);
			}
		}
		NPSL_SocketLibCleanUp(NULL, NULL);
	}


	if (return_code != NPSL_SUCCESS)
		fprintf(stderr, "\n\nERROR: %s\n\n", error_text);

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

