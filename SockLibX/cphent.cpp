/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Performs a copy of a 'NPSL_HOSTENT' ('struct
								hostent') structure.

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
#include <string.h>

#include <Utility/C_StringSupport.hpp>

#include "npsli.h"

/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyHostEntFlat

   SYNOPSIS    :	return_code = NPSL_CopyHostEntFlat(in_host_ent_ptr,
							out_host_ent_ptr, host_ent_buffer_ptr,
							host_ent_buffer_length, error_text);

						int                 return_code;

						const NPSL_HOSTENT *in_host_ent_ptr;

						NPSL_HOSTENT       *out_host_ent_ptr;

						void               *host_ent_buffer_ptr;

						unsigned int        host_ent_buffer_length;

						char               *error_text;

   DESCRIPTION :	Copies a single ''NPSL_HOSTENT'' structures.

						Note that in contrast to the function ``NPSL_CopyHostEnt``,
						this function performs a shallow-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_host_ent_ptr`` is a pointer to the ''NPSL_HOSTENT''
						structure which is to be copied by this function.

						(.) ``out_host_ent_ptr`` is a pointer to the ''NPSL_HOSTENT''
						structure into which the copy is to be performed.

						(.) ``host_ent_buffer_ptr`` points to a buffer into which
						this function is to place the data contained in the string
						members of the ''NPSL_HOSTENT'' structure.

						(.) ``host_ent_buffer_length`` is the length of the memory
						area to which the ``host_ent_buffer_ptr`` parameter points.

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
						which the ``host_ent_buffer_ptr`` parameter points to ensure
						that it is long enough for the copy. If it is not, the copy
						will not be performed and an error will be returned by this
						function.

   CAVEATS     :  

   SEE ALSO    :  NPSL_CopyHostEnt
						NPSL_CopyHostEntFlatBasic

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyHostEntFlat
						Copy Functions:Network Programming Support Functions:NPSL_CopyHostEntFlat
						Network Programming Support Functions:Copy Functions:NPSL_CopyHostEntFlat

   PUBLISH XREF:	NPSL_CopyHostEntFlat

   PUBLISH NAME:	NPSL_CopyHostEntFlat

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyHostEntFlat(const NPSL_HOSTENT *in_host_ent_ptr,
	NPSL_HOSTENT *out_host_ent_ptr, void *host_ent_buffer_ptr,
	unsigned int host_ent_buffer_length, char *error_text)
{
	return(NPSL_CopyHostEntFlatBasic(in_host_ent_ptr, out_host_ent_ptr,
		host_ent_buffer_ptr, host_ent_buffer_length, NULL, error_text));
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyHostEntFlatBasic

   SYNOPSIS    :	return_code = NPSL_CopyHostEntFlatBasic(in_host_ent_ptr,
							out_host_ent_ptr, host_ent_buffer_ptr,
							host_ent_buffer_length, required_length, error_text);

						int                 return_code;

						const NPSL_HOSTENT *in_host_ent_ptr;

						NPSL_HOSTENT       *out_host_ent_ptr;

						void               *host_ent_buffer_ptr;

						unsigned int        host_ent_buffer_length;

						unsigned int       *required_length;

						char               *error_text;

   DESCRIPTION :	Copies a single ''NPSL_HOSTENT'' structures.

						Note that in contrast to the function ``NPSL_CopyHostEnt``,
						this function performs a shallow-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_host_ent_ptr`` is a pointer to the ''NPSL_HOSTENT''
						structure which is to be copied by this function.

						(.) ``out_host_ent_ptr`` is a pointer to the ''NPSL_HOSTENT''
						structure into which the copy is to be performed.

						(.) ``host_ent_buffer_ptr`` points to a buffer into which
						this function is to place the data contained in the string
						members of the ''NPSL_HOSTENT'' structure.

						(.) ``host_ent_buffer_length`` is the length of the memory
						area to which the ``host_ent_buffer_ptr`` parameter points.

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
						which the ``host_ent_buffer_ptr`` parameter points to ensure
						that it is long enough for the copy. If it is not, the copy
						will not be performed and an error will be returned by this
						function.

   CAVEATS     :  

   SEE ALSO    :  NPSL_CopyHostEnt
						NPSL_CopyHostEntFlat

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyHostEntFlatBasic
						Copy Functions:Network Programming Support Functions:NPSL_CopyHostEntFlatBasic
						Network Programming Support Functions:Copy Functions:NPSL_CopyHostEntFlatBasic

   PUBLISH XREF:	NPSL_CopyHostEntFlatBasic

   PUBLISH NAME:	NPSL_CopyHostEntFlatBasic

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyHostEntFlatBasic(const NPSL_HOSTENT *in_host_ent_ptr,
	NPSL_HOSTENT *out_host_ent_ptr, void *host_ent_buffer_ptr,
	unsigned int host_ent_buffer_length, unsigned int *required_length,
	char *error_text)
{
	int            return_code = NPSL_SUCCESS;
	unsigned int   count_1;
	unsigned int   required_size;
	const char   **in_alias;
	const char   **in_address;
	unsigned int   alias_count;
	unsigned int   alias_length;
	unsigned int   address_count;
	unsigned int   address_length;
	char          *str_ptr;
	unsigned int   str_length;
	NPSL_HOSTENT   tmp_host_ent;

	if (required_length != NULL)
		*required_length = 0;

	if ((return_code = NPSL_CheckHostEntParams(out_host_ent_ptr,
		host_ent_buffer_ptr, host_ent_buffer_length, error_text)) !=
		NPSL_SUCCESS)
		goto EXIT_FUNCTION;

	in_alias       = ((const char **) in_host_ent_ptr->h_aliases);
	in_address     = ((const char **) in_host_ent_ptr->h_addr_list);
	alias_count    = 1;
	alias_length   = 0;
	address_count  = 1;
	address_length = 0;

	while (*in_alias != NULL) {
		alias_length += COMPAT_CAST_static(unsigned int, strlen(*in_alias)) + 1;
		in_alias++;
		alias_count++;
	}

	while (*in_address != NULL) {
		in_address++;
		address_count++;
	}
	address_length = (address_count - 1) * in_host_ent_ptr->h_length;

	required_size = sizeof(NPSL_HOSTENT) +
		COMPAT_CAST_static(unsigned int, strlen(in_host_ent_ptr->h_name)) + 1 +
		(alias_count * sizeof(char *)) + (address_count * sizeof(char *)) +
		alias_length + address_length;

	if (required_size > host_ent_buffer_length) {
		if (error_text != NULL)
			sprintf(error_text, "%s %s (%u) %s (%u).",
				"The total length required to store the host entry",
				"'NPSL_HOSTENT' data", required_size,
				"is less than the allocated 'NPSL_HOSTENT' buffer length",
				host_ent_buffer_length);
		if (required_length != NULL)
			*required_length = required_size;
		return_code = NPSL_FAILURE;
		goto EXIT_FUNCTION;
	}

	in_alias   = ((const char **) in_host_ent_ptr->h_aliases);
	in_address = ((const char **) in_host_ent_ptr->h_addr_list);
	str_ptr    = ((char *) host_ent_buffer_ptr) + sizeof(NPSL_HOSTENT) + 
		(alias_count * sizeof(char *)) +
		(address_count * sizeof(char *));

	tmp_host_ent             = *in_host_ent_ptr;
	tmp_host_ent.h_aliases   = ((char **)
		(((char *) host_ent_buffer_ptr) + sizeof(NPSL_HOSTENT)));
	tmp_host_ent.h_addr_list = ((char **)
		((((char *) host_ent_buffer_ptr) + sizeof(NPSL_HOSTENT)) +
		(alias_count * sizeof(char *))));

	tmp_host_ent.h_name = str_ptr;
	memcpy(str_ptr, in_host_ent_ptr->h_name,
		str_length = (COMPAT_CAST_static(unsigned int,
		strlen(in_host_ent_ptr->h_name)) + 1));
	str_ptr += str_length;

	for (count_1 = 0; count_1 < alias_count; count_1++) {
		if (in_host_ent_ptr->h_aliases[count_1] != NULL) {
			tmp_host_ent.h_aliases[count_1] = str_ptr;
			memcpy(str_ptr, in_host_ent_ptr->h_aliases[count_1],
				str_length = (COMPAT_CAST_static(unsigned int,
				strlen(in_host_ent_ptr->h_aliases[count_1])) + 1));
			str_ptr += str_length;
		}
		else
			tmp_host_ent.h_aliases[count_1] = NULL;
	}

	for (count_1 = 0; count_1 < address_count; count_1++) {
		if (in_host_ent_ptr->h_addr_list[count_1] != NULL) {
			tmp_host_ent.h_addr_list[count_1] = str_ptr;
			memcpy(str_ptr, in_host_ent_ptr->h_addr_list[count_1],
				in_host_ent_ptr->h_length * sizeof(char));
			str_ptr += in_host_ent_ptr->h_length * sizeof(char);
		}
		else
			tmp_host_ent.h_addr_list[count_1] = NULL;
	}

	*out_host_ent_ptr                       = tmp_host_ent;
	*((NPSL_HOSTENT *) host_ent_buffer_ptr) = tmp_host_ent;

EXIT_FUNCTION:

	return(return_code);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyHostEnt

   SYNOPSIS    :	return_code = NPSL_CopyHostEnt(in_ptr, out_ptr,
							error_text);

						int                 return_code;

						const NPSL_HOSTENT *in_ptr;

						NPSL_HOSTENT       *out_ptr;

						char               *error_text;

   DESCRIPTION :	Copies a single ''NPSL_HOSTENT'' structures.

						Note that in contrast to the functions
						``NPSL_CopyHostEntFlat`` and ``NPSL_CopyHostEntFlatBasic``,
						this function performs a deep-copy.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_ptr`` is a pointer to the ''NPSL_HOSTENT''
						structure which is to be copied by this function.

						(.) ``out_ptr`` is a pointer to the ''NPSL_HOSTENT''
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

   SEE ALSO    :	NPSL_CopyHostEntList
						NPSL_CopyHostEntFlat
						NPSL_CopyHostEntFlatBasic

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyHostEnt
						Copy Functions:Network Programming Support Functions:NPSL_CopyHostEnt
						Network Programming Support Functions:Copy Functions:NPSL_CopyHostEnt

   PUBLISH XREF:	NPSL_CopyHostEnt

   PUBLISH NAME:	NPSL_CopyHostEnt

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyHostEnt(const NPSL_HOSTENT *in_host_ent_ptr,
	NPSL_HOSTENT *out_host_ent_ptr, char *error_text)
{
	int            return_code       = NPSL_SUCCESS;
	unsigned int   tmp_alias_count   = 0;
	unsigned int   tmp_address_count = 0;
	unsigned int   alias_count       = 1;
	unsigned int   address_count     = 1;
	unsigned int   alias_length      = 0;
	unsigned int   address_length    = 0;
	unsigned int   count_1;
	const char   **in_alias;
	const char   **in_address;

	in_alias   = ((const char **) in_host_ent_ptr->h_aliases);
	in_address = ((const char **) in_host_ent_ptr->h_addr_list);

	while (*in_alias != NULL) {
		alias_length += COMPAT_CAST_static(unsigned int, strlen(*in_alias)) + 1;
		in_alias++;
		alias_count++;
	}

	while (*in_address != NULL) {
		in_address++;
		address_count++;
	}
	address_length = (address_count - 1) * in_host_ent_ptr->h_length;

	NPSL_InitHostEnt(out_host_ent_ptr);

	out_host_ent_ptr->h_addrtype = in_host_ent_ptr->h_addrtype;
	out_host_ent_ptr->h_length   = in_host_ent_ptr->h_length;

	if ((in_host_ent_ptr->h_name != NULL) && ((out_host_ent_ptr->h_name =
		strdup(in_host_ent_ptr->h_name)) == NULL)) {
		MLB::NPSLI_Internal::STR_AllocMsgItem(
			COMPAT_CAST_static(unsigned int, strlen(in_host_ent_ptr->h_name)) + 1,
			error_text, "Unable to copy the 'NPSL_HOSTENT' member 'h_name'");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else if (MLB::NPSLI_Internal::meml_allocate(&tmp_address_count,
		((void ***) &out_host_ent_ptr->h_addr_list), 0, address_count) !=
      MLB::NPSLI_Internal::STRFUNCS_SUCCESS) {
		MLB::NPSLI_Internal::STR_AllocMsgPtrList(address_count, address_length,
			error_text,
			"Unable to copy the 'NPSL_HOSTENT' member 'h_addr_list' pointer list");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else if (MLB::NPSLI_Internal::meml_allocate(&tmp_alias_count,
		((void ***) &out_host_ent_ptr->h_aliases), 0, alias_count) !=
		MLB::NPSLI_Internal::STRFUNCS_SUCCESS) {
		MLB::NPSLI_Internal::STR_AllocMsgPtrList(alias_count, alias_length,
			error_text,
			"Unable to copy the 'NPSL_HOSTENT' member 'h_aliases' pointer list");
		return_code = NPSL_MEMORY_FAILURE;
	}
	else {
		for (count_1 = 0; count_1 < (address_count - 1); count_1++) {
			try {
				out_host_ent_ptr->h_addr_list[count_1] = COMPAT_CAST_static(char *,
					MLB::Utility::C_memdup(in_host_ent_ptr->h_addr_list[count_1],
					((unsigned int) in_host_ent_ptr->h_length), "NPSL_CopyHostEnt"));
			}
			catch (const std::exception &except) {
				char tmp_error_text[NPSL_MAX_ERROR_TEXT];
				MLB::NPSLI_Internal::NPSLI_GetErrorFromException(except,
					tmp_error_text);
				MLB::Utility::nstrcat(strcpy(error_text,
					"Unable to copy the 'NPSL_HOSTENT' member 'h_addr_list' list: "),
					tmp_error_text, NPSL_MAX_ERROR_TEXT - 100);
				return_code = NPSL_MEMORY_FAILURE;
				break;
			}
		}
		if (return_code == NPSL_SUCCESS) {
			for (count_1 = 0; count_1 < (alias_count - 1); count_1++) {
				if ((out_host_ent_ptr->h_aliases[count_1] =
					strdup(in_host_ent_ptr->h_aliases[count_1])) == NULL) {
					MLB::NPSLI_Internal::STR_AllocMsgPtrList(alias_count,
						alias_length, error_text,
						"Unable to copy the 'NPSL_HOSTENT' member 'h_aliases' list");
					return_code = NPSL_MEMORY_FAILURE;
					break;
				}
			}
		}
	}

	if (return_code != NPSL_SUCCESS)
		NPSL_FreeHostEnt(out_host_ent_ptr);

	return(return_code);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	NPSL_CopyHostEntList

   SYNOPSIS    :	return_code = NPSL_CopyHostEntList(in_count, in_list,
							out_count, out_list);

						int                  return_code;

						unsigned int         in_count;

						const NPSL_HOSTENT  *in_list;

						unsigned int        *out_count;

						NPSL_HOSTENT       **out_list;

						char                *error_text;

   DESCRIPTION :	Copies an array of ''NPSL_HOSTENT'' structures.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_count`` is the number of elements in the array
						``in_list``.

						(.) ``in_list`` is the array of ''NPSL_HOSTENT''
						structures which is to be copied by this function.

						(.) ``out_count`` is a pointer to an ''unsigned int'' into
						which this function will place the number of elements
						copied.

						(.) ``out_list`` is a pointer to the array of
						''NPSL_HOSTENT'' structures which will be allocated
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

   SEE ALSO    :	NPSL_CopyHostEnt

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1993 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_CopyHostEntList
						Copy Functions:Network Programming Support Functions:NPSL_CopyHostEntList
						Network Programming Support Functions:Copy Functions:NPSL_CopyHostEntList

   PUBLISH XREF:	NPSL_CopyHostEntList

   PUBLISH NAME:	NPSL_CopyHostEntList

	ENTRY CLASS	:	Copy Functions

EOH */
/*	***********************************************************************	*/
int NPSL_CopyHostEntList(unsigned int in_count, const NPSL_HOSTENT *in_list,
	unsigned int *out_count, NPSL_HOSTENT **out_list, char *error_text)
{
	int          return_code = NPSL_SUCCESS;
	unsigned int count_1;

	*out_count = 0;
	*out_list  = NULL;

	if (in_count) {
		if ((*out_list = ((NPSL_HOSTENT *) calloc(in_count,
			sizeof(**out_list)))) == NULL) {
			MLB::NPSLI_Internal::STR_AllocMsgList(in_count, sizeof(**out_list),
				error_text,
				"Unable to allocate memory for the 'NPSL_HOSTENT' list");
			return_code = NPSL_MEMORY_FAILURE;
		}
		else {
			*out_count = in_count;
			NPSL_InitHostEntList(*out_count, *out_list);
			for (count_1 = 0; count_1 < in_count; count_1++) {
				if ((return_code = NPSL_CopyHostEnt(in_list + count_1,
					*out_list + count_1, error_text)) != NPSL_SUCCESS) {
					NPSL_FreeHostEntList(out_count, out_list);
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
	unsigned int   count_2;
	char           buffer[1024];
	NPSL_HOSTENT   hostent_flat;
	NPSL_HOSTENT   hostent_copy;
	char         **tmp_list;
	char           inet_buffer[NPSL_MAX_IPV4_ADDR_STR + 1];
	char           error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test routine for 'NPSL_CopyHostEnt()'\n");
	fprintf(stderr, "---- ------- --- --------------------\n\n");

	if (argc == 1)
		sprintf(error_text, "USAGE: %s %s [ %s . . . ]\n",
			"<host-name> | IP-address", "<host-name> | IP-address", argv[0]);
	else if ((return_code = NPSL_SocketLibStartUp(NULL, error_text)) ==
		NPSL_SUCCESS) {
		for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
			printf("HOSTNAME: %s ---> ", argv[count_1]);
			printf("by %s ---> ",
				(!isdigit(*argv[count_1])) ? "name" : "address");
			if ((return_code = NPSL_GetHostByString(argv[count_1],
				&hostent_flat, buffer, sizeof(buffer), error_text)) !=
				NPSL_SUCCESS)
				break;
			if ((return_code = NPSL_CopyHostEnt(&hostent_flat, &hostent_copy,
				error_text)) != NPSL_SUCCESS)
				break;
			printf("DONE\n");
			printf("        : Host Name     : %s\n",
				hostent_copy.h_name);
			printf("        : Address Type  : %d\n",
				hostent_copy.h_addrtype);
			printf("        : Address Length: %d\n",
				hostent_copy.h_length);
			if ((tmp_list = hostent_copy.h_aliases)[0] != NULL) {
				count_2 = 0;
				while (*tmp_list != NULL) {
					printf("        : Alias %8u: %s\n", count_2++,
						*tmp_list);
					tmp_list++;
				}
			}
			if (((tmp_list = hostent_copy.h_addr_list)[0] != NULL) &&
				(hostent_copy.h_addrtype == AF_INET)) {
				count_2 = 0;
				while (*tmp_list != NULL) {
					printf("        : Address %6u: %s\n", count_2++,
						NPSL_AddrToINet(*tmp_list, inet_buffer));
					tmp_list++;
				}
			}
			NPSL_FreeHostEnt(&hostent_copy);
		}
		NPSL_SocketLibCleanUp(NULL, NULL);
	}

	if (return_code != NPSL_SUCCESS)
		fprintf(stderr, "\n\nERROR: %s\n\n", error_text);

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

