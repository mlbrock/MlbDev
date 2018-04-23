/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Performs a copy of a 'NPSL_PROTOENT' ('struct
								protoent') structure.

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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyProtoEntFlat

   SYNOPSIS    :	return_code = NPSL_CopyProtoEntFlat(in_proto_ent_ptr,
							out_proto_ent_ptr, proto_ent_buffer_ptr,
							proto_ent_buffer_length, error_text);

						int                  return_code;

						const NPSL_PROTOENT *in_proto_ent_ptr;

						NPSL_PROTOENT       *out_proto_ent_ptr;

						void                *proto_ent_buffer_ptr;

						unsigned int         proto_ent_buffer_length;

						char                *error_text;

   DESCRIPTION :	Copies a single ''NPSL_PROTOENT'' structures.

						Note that in contrast to the function ``NPSL_CopyProtoEnt``,
						this function performs a shallow-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_proto_ent_ptr`` is a pointer to the ''NPSL_PROTOENT''
						structure which is to be copied by this function.

						(.) ``out_proto_ent_ptr`` is a pointer to the ''NPSL_PROTOENT''
						structure into which the copy is to be performed.

						(.) ``proto_ent_buffer_ptr`` points to a buffer into which
						this function is to place the data contained in the string
						members of the ''NPSL_PROTOENT'' structure.

						(.) ``proto_ent_buffer_length`` is the length of the memory
						area to which the ``proto_ent_buffer_ptr`` parameter points.

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
						which the ``proto_ent_buffer_ptr`` parameter points to ensure
						that it is long enough for the copy. If it is not, the copy
						will not be performed and an error will be returned by this
						function.

   CAVEATS     :  

   SEE ALSO    :  NPSL_CopyProtoEnt
						NPSL_CopyProtoEntFlatBasic

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2018 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyProtoEntFlat
						Copy Functions:Network Programming Support Functions:NPSL_CopyProtoEntFlat
						Network Programming Support Functions:Copy Functions:NPSL_CopyProtoEntFlat

   PUBLISH XREF:	NPSL_CopyProtoEntFlat

   PUBLISH NAME:	NPSL_CopyProtoEntFlat

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyProtoEntFlat(const NPSL_PROTOENT *in_proto_ent_ptr,
	NPSL_PROTOENT *out_proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, char *error_text)
{
	return(NPSL_CopyProtoEntFlatBasic(in_proto_ent_ptr, out_proto_ent_ptr,
		proto_ent_buffer_ptr, proto_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyProtoEntFlatBasic

   SYNOPSIS    :	return_code = NPSL_CopyProtoEntFlatBasic(in_proto_ent_ptr,
							out_proto_ent_ptr, proto_ent_buffer_ptr,
							proto_ent_buffer_length, required_length, error_text);

						int                  return_code;

						const NPSL_PROTOENT *in_proto_ent_ptr;

						NPSL_PROTOENT       *out_proto_ent_ptr;

						void                *proto_ent_buffer_ptr;

						unsigned int         proto_ent_buffer_length;

						unsigned int        *required_length;

						char                *error_text;

   DESCRIPTION :	Copies a single ''NPSL_PROTOENT'' structures.

						Note that in contrast to the function ``NPSL_CopyProtoEnt``,
						this function performs a shallow-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_proto_ent_ptr`` is a pointer to the ''NPSL_PROTOENT''
						structure which is to be copied by this function.

						(.) ``out_proto_ent_ptr`` is a pointer to the ''NPSL_PROTOENT''
						structure into which the copy is to be performed.

						(.) ``proto_ent_buffer_ptr`` points to a buffer into which
						this function is to place the data contained in the string
						members of the ''NPSL_PROTOENT'' structure.

						(.) ``proto_ent_buffer_length`` is the length of the memory
						area to which the ``proto_ent_buffer_ptr`` parameter points.

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
						which the ``proto_ent_buffer_ptr`` parameter points to ensure
						that it is long enough for the copy. If it is not, the copy
						will not be performed and an error will be returned by this
						function.

   CAVEATS     :  

   SEE ALSO    :  NPSL_CopyProtoEnt
						NPSL_CopyProtoEntFlat

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2018 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyProtoEntFlatBasic
						Copy Functions:Network Programming Support Functions:NPSL_CopyProtoEntFlatBasic
						Network Programming Support Functions:Copy Functions:NPSL_CopyProtoEntFlatBasic

   PUBLISH XREF:	NPSL_CopyProtoEntFlatBasic

   PUBLISH NAME:	NPSL_CopyProtoEntFlatBasic

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyProtoEntFlatBasic(const NPSL_PROTOENT *in_proto_ent_ptr,
	NPSL_PROTOENT *out_proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int             return_code = NPSL_SUCCESS;
	unsigned int    count_1;
	unsigned int    required_size;
	const char    **in_alias;
	unsigned int    alias_count;
	unsigned int    alias_length;
	char           *str_ptr;
	unsigned int    str_length;
	NPSL_PROTOENT   tmp_proto_ent;

	if (required_length != NULL)
		*required_length = 0;

	if ((return_code = NPSL_CheckProtoEntParams(out_proto_ent_ptr,
		proto_ent_buffer_ptr, proto_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		goto EXIT_FUNCTION;

	in_alias       = ((const char **) in_proto_ent_ptr->p_aliases);
	alias_count    = 1;
	alias_length   = 0;

	while (*in_alias != NULL) {
		alias_length += COMPAT_CAST_static(unsigned int, strlen(*in_alias)) + 1;
		in_alias++;
		alias_count++;
	}

	required_size = sizeof(NPSL_PROTOENT) +
		COMPAT_CAST_static(unsigned int, strlen(in_proto_ent_ptr->p_name)) + 1 +
		(alias_count * sizeof(char *)) + alias_length;

	if (required_size > proto_ent_buffer_length) {
		if (error_text != NULL)
			sprintf(error_text, "%s %s (%u) %s (%u).",
				"The total length required to store the protocol entry",
				"'NPSL_PROTOENT' data", required_size,
				"is less than the allocated 'NPSL_PROTOENT' buffer length",
				proto_ent_buffer_length);
		if (required_length != NULL)
			*required_length = required_size;
		return_code = NPSL_FAILURE;
		goto EXIT_FUNCTION;
	}

	in_alias   = ((const char **) in_proto_ent_ptr->p_aliases);
	str_ptr    = ((char *) proto_ent_buffer_ptr) + sizeof(NPSL_PROTOENT) + 
		(alias_count * sizeof(char *));

	tmp_proto_ent           = *in_proto_ent_ptr;
	tmp_proto_ent.p_aliases = ((char **)
		(((char *) proto_ent_buffer_ptr) + sizeof(NPSL_PROTOENT)));

	tmp_proto_ent.p_name = str_ptr;
	memcpy(str_ptr, in_proto_ent_ptr->p_name,
		str_length =
			(COMPAT_CAST_static(unsigned int,
			strlen(in_proto_ent_ptr->p_name)) + 1));
	str_ptr += str_length;

	for (count_1 = 0; count_1 < alias_count; count_1++) {
		if (in_proto_ent_ptr->p_aliases[count_1] != NULL) {
			tmp_proto_ent.p_aliases[count_1] = str_ptr;
			memcpy(str_ptr, in_proto_ent_ptr->p_aliases[count_1],
				str_length = (COMPAT_CAST_static(unsigned int, 
					strlen(in_proto_ent_ptr->p_aliases[count_1])) + 1));
			str_ptr += str_length;
		}
		else
			tmp_proto_ent.p_aliases[count_1] = NULL;
	}

	*out_proto_ent_ptr                        = tmp_proto_ent;
	*((NPSL_PROTOENT *) proto_ent_buffer_ptr) = tmp_proto_ent;

EXIT_FUNCTION:

	return(return_code);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyProtoEnt

   SYNOPSIS    :	return_code = NPSL_CopyProtoEnt(in_ptr, out_ptr,
							error_text);

						int                  return_code;

						const NPSL_PROTOENT *in_ptr;

						NPSL_PROTOENT       *out_ptr;

						char                *error_text;

   DESCRIPTION :	Copies a single ''NPSL_PROTOENT'' structures.

						Note that in contrast to the functions
						``NPSL_CopyProtoEntFlat`` and ``NPSL_CopyProtoEntFlatBasic``,
						this function performs a deep-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_ptr`` is a pointer to the ''NPSL_PROTOENT''
						structure which is to be copied by this function.

						(.) ``out_ptr`` is a pointer to the ''NPSL_PROTOENT''
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

   SEE ALSO    :	NPSL_CopyProtoEntList
						NPSL_CopyProtoEntFlat
						NPSL_CopyProtoEntFlatBasic

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2018 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyProtoEnt
						Copy Functions:Network Programming Support Functions:NPSL_CopyProtoEnt
						Network Programming Support Functions:Copy Functions:NPSL_CopyProtoEnt

   PUBLISH XREF:	NPSL_CopyProtoEnt

   PUBLISH NAME:	NPSL_CopyProtoEnt

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyProtoEnt(const NPSL_PROTOENT *in_proto_ent_ptr,
	NPSL_PROTOENT *out_proto_ent_ptr, char *error_text)
{
	int            return_code       = NPSL_SUCCESS;
	unsigned int   tmp_alias_count   = 0;
	unsigned int   alias_count       = 1;
	unsigned int   alias_length      = 0;
	unsigned int   count_1;
	const char   **in_alias;

	in_alias = ((const char **) in_proto_ent_ptr->p_aliases);

	while (*in_alias != NULL) {
		alias_length += COMPAT_CAST_static(unsigned int, strlen(*in_alias)) + 1;
		in_alias++;
		alias_count++;
	}

	NPSL_InitProtoEnt(out_proto_ent_ptr);

	out_proto_ent_ptr->p_proto = in_proto_ent_ptr->p_proto;

	if ((in_proto_ent_ptr->p_name != NULL) && ((out_proto_ent_ptr->p_name =
		strdup(in_proto_ent_ptr->p_name)) == NULL)) {
		MLB::NPSLI_Internal::STR_AllocMsgItem(
			COMPAT_CAST_static(unsigned int, strlen(in_proto_ent_ptr->p_name)) + 1,
			error_text, "Unable to copy the 'NPSL_PROTOENT' member 'p_name'");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else if (MLB::NPSLI_Internal::meml_allocate(&tmp_alias_count,
		((void ***) &out_proto_ent_ptr->p_aliases), 0, alias_count) !=
		MLB::NPSLI_Internal::STRFUNCS_SUCCESS) {
		MLB::NPSLI_Internal::STR_AllocMsgPtrList(alias_count, alias_length,
			error_text,
			"Unable to copy the 'NPSL_PROTOENT' member 'p_aliases' pointer list");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else {
		for (count_1 = 0; count_1 < (alias_count - 1); count_1++) {
			if ((out_proto_ent_ptr->p_aliases[count_1] =
				strdup(in_proto_ent_ptr->p_aliases[count_1])) == NULL) {
				MLB::NPSLI_Internal::STR_AllocMsgPtrList(alias_count,
					alias_length, error_text,
					"Unable to copy the 'NPSL_PROTOENT' member 'p_aliases' list");
				return_code = NPSL_MEMORY_FAILURE;
				break;
			}
		}
	}

	if (return_code != NPSL_SUCCESS)
		NPSL_FreeProtoEnt(out_proto_ent_ptr);

	return(return_code);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyProtoEntList

   SYNOPSIS    :	return_code = NPSL_CopyProtoEntList(in_count, in_list,
							out_count, out_list);

						int                   return_code;

						unsigned int          in_count;

						const NPSL_PROTOENT  *in_list;

						unsigned int         *out_count;

						NPSL_PROTOENT       **out_list;

						char                 *error_text;

   DESCRIPTION :	Copies an array of ''NPSL_PROTOENT'' structures.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_count`` is the number of elements in the array
						``in_list``.

						(.) ``in_list`` is the array of ''NPSL_PROTOENT''
						structures which is to be copied by this function.

						(.) ``out_count`` is a pointer to an ''unsigned int'' into
						which this function will place the number of elements
						copied.

						(.) ``out_list`` is a pointer to the array of
						''NPSL_PROTOENT'' structures which will be allocated
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

   SEE ALSO    :	NPSL_CopyProtoEnt

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2018 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyProtoEntList
						Copy Functions:Network Programming Support Functions:NPSL_CopyProtoEntList
						Network Programming Support Functions:Copy Functions:NPSL_CopyProtoEntList

   PUBLISH XREF:	NPSL_CopyProtoEntList

   PUBLISH NAME:	NPSL_CopyProtoEntList

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyProtoEntList(unsigned int in_count, const NPSL_PROTOENT *in_list,
	unsigned int *out_count, NPSL_PROTOENT **out_list, char *error_text)
{
	int          return_code = NPSL_SUCCESS;
	unsigned int count_1;

	*out_count = 0;
	*out_list  = NULL;

	if (in_count) {
		if ((*out_list = ((NPSL_PROTOENT *) calloc(in_count,
			sizeof(**out_list)))) == NULL) {
			MLB::NPSLI_Internal::STR_AllocMsgList(in_count, sizeof(**out_list),
				error_text,
				"Unable to allocate memory for the 'NPSL_PROTOENT' list");
			return_code = NPSL_MEMORY_FAILURE;
		}
		else {
			*out_count = in_count;
			NPSL_InitProtoEntList(*out_count, *out_list);
			for (count_1 = 0; count_1 < in_count; count_1++) {
				if ((return_code = NPSL_CopyProtoEnt(in_list + count_1,
					*out_list + count_1, error_text)) != NPSL_SUCCESS) {
					NPSL_FreeProtoEntList(out_count, out_list);
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
	int             return_code = NPSL_FAILURE;
	unsigned int    count_1;
	unsigned int    count_2;
	char            buffer[1024];
	NPSL_PROTOENT   protoent_flat;
	NPSL_PROTOENT   protoent_copy;
	char          **tmp_list;
	char            error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test routine for 'NPSL_GetProtoByNumber()'\n");
	fprintf(stderr, "                 'NPSL_GetProtoByName()'\n");
	fprintf(stderr, "                 'NPSL_GetProtoByString()'\n");
	fprintf(stderr, "---- ------- --- -------------------------\n\n");

	if (argc == 1)
		sprintf(error_text, "USAGE: %s %s [%s . . .]\n", argv[0],
			"{<protocol-name> | <protocol-number>}",
			"{<protocol-name> | <protocol-number>}");
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			printf("PROTOCOL NAME OR NUMBER: %s\n", argv[count_1]);
			printf("                       : by %s ---> ",
				(!isdigit(*argv[count_1])) ? "name" : "number");
			if ((return_code = NPSL_GetProtoByString(argv[count_1],
				&protoent_flat, buffer, sizeof(buffer), error_text)) !=
				NPSL_SUCCESS)
				break;
			if ((return_code = NPSL_CopyProtoEnt(&protoent_flat, &protoent_copy,
				error_text)) != NPSL_SUCCESS)
				break;
			printf("DONE\n");
			printf("                       : Protocol Name    : %s\n",
				protoent_copy.p_name);
			printf("                       : Protocol Protocol: %d\n",
				protoent_copy.p_proto);
			if ((tmp_list = protoent_copy.p_aliases)[0] != NULL) {
				count_2 = 0;
				while (*tmp_list != NULL) {
					printf("                       : Alias    %8u: %s\n",
						count_2++, *tmp_list);
					tmp_list++;
				}
			}
		}
		NPSL_SocketLibCleanUp(NULL, NULL);
	}

	if (return_code != NPSL_SUCCESS)
		fprintf(stderr, "\n\nERROR: %s\n\n", error_text);

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

