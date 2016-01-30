/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Internal Module File			*/
/* *********************************************************************** */
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Shim file needed to support NPSL code which old MLB
								logic from the obsolete C libraries strfuncs and
								genfuncs.

	Revision History	:	1996-04-10 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1996 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/*		Include necessary header files . . .											*/
/* *********************************************************************** */

#include "npsli.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* *********************************************************************** */

namespace MLB {

namespace NPSLI_Internal {

//	////////////////////////////////////////////////////////////////////////////
char *NPSLI_GetErrorFromException(const std::exception &except,
	char *error_text)
{
	return(MLB::Utility::nstrcpy(error_text, except.what(),
		NPSL_MAX_ERROR_TEXT - 1));
}
//	////////////////////////////////////////////////////////////////////////////

/*	*********************************************************************** */
#ifndef NARGS
size_t chrcnt(const char *search_string, int target_char)
#else
size_t chrcnt(search_string, target_char)
const char *search_string;
int         target_char;
#endif /* #ifndef NARGS */
{
	unsigned int  count = 0;
	const char   *tmp_ptr;

	while ((tmp_ptr = strchr(search_string, target_char)) != NULL) {
		count++;
		search_string = tmp_ptr + 1;
	}

	return(count);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

	NAME			:	trim

	SYNOPSIS		:	string_ptr = trim(in_text);

						char *string_ptr;

						char *in_text;

	DESCRIPTION	:	Removes all leading and trailing whitespace from the
						string to which the ``in_text`` parameter points.

						Whitespace is defined as any character for which the
						standard library macro ``isspace`` returns true.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``in_text`` is the string to be trimmed.

	RETURNS		: 	A pointer to the string used as input. This is, ``in_text``
						is returned.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  trim_basic
						trim_basic_mem
						ltrim
						rtrim
						trimx

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1986 - 2016 Michael L. Brock

   OUTPUT INDEX:  trim
						String Justification Functions:trim
						STRFUNCS:String Justification Functions:trim
						String Functions:See STRFUNCS

   PUBLISH XREF:  trim

   PUBLISH NAME:	trim

	ENTRY CLASS	:	String Justification Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
char *trim(char *in_text)
#else
char *trim(in_text)
char *in_text;
#endif /* #ifndef NARGS */
{
	return(ltrim(rtrim(in_text)));
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

	NAME			:	ltrim

	SYNOPSIS		:	string_ptr = ltrim(in_text);

						char *string_ptr;

						char *in_text;

	DESCRIPTION	:	Removes all leading whitespace from the string to which
						the ``in_text`` parameter points.  That is, the string is
						trimmed from the left. 

						Whitespace is defined as any character for which the
						standard library macro ``isspace`` returns true.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``in_text`` is the string from which whitespace on the
						left is to be removed.

	RETURNS		: 	A pointer to the string used as input. This is, ``in_text``
						is returned.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  ltrim_basic
						ltrim_basic_mem
						ltrimx
						rtrim
						trim

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1986 - 2016 Michael L. Brock

   OUTPUT INDEX:  ltrim
						String Justification Functions:ltrim
						STRFUNCS:String Justification Functions:ltrim
						String Functions:See STRFUNCS

   PUBLISH XREF:  ltrim

   PUBLISH NAME:	ltrim

	ENTRY CLASS	:	String Justification Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
char *ltrim(char *in_text)
#else
char *ltrim(in_text)
char *in_text;
#endif /* #ifndef NARGS */
{
	char *temp_ptr_1 = in_text;
	char *temp_ptr_2 = in_text;

	while (*temp_ptr_1) {
		if (!isspace(*((unsigned char *) temp_ptr_1))) {
			while (*temp_ptr_1)
				*in_text++ = *temp_ptr_1++;
			*in_text = '\0';
			return(temp_ptr_2);
		}
		temp_ptr_1++;
	}

	return(temp_ptr_2);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

	NAME			: 	rtrim

	SYNOPSIS		:	string_ptr = rtrim(in_text);

						char *string_ptr;

						char *in_text;

	DESCRIPTION	:	Removes all trailing whitespace from the string to which
						the ``in_text`` parameter points.  That is, the string is
						trimmed from the right. 

						Whitespace is defined as any character for which the
						standard library macro ``isspace`` returns true.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``in_text`` is the string from which whitespace on the
						right is to be removed.

	RETURNS		: 	A pointer to the string used as input. This is, ``in_text``
						is returned.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  rtrim_basic
						rtrim_basic_mem
						rtrimx
						ltrim
						trim

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1986 - 2016 Michael L. Brock

   OUTPUT INDEX:  rtrim
						String Justification Functions:rtrim
						STRFUNCS:String Justification Functions:rtrim
						String Functions:See STRFUNCS

   PUBLISH XREF:  rtrim

   PUBLISH NAME:	rtrim

	ENTRY CLASS	:	String Justification Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
char *rtrim(char *in_text)
#else
char *rtrim(in_text)
char *in_text;
#endif /* #ifndef NARGS */
{
	unsigned int  count;
	char         *temp_ptr;

	count = COMPAT_CAST_static(unsigned int, strlen(in_text));

	if (count) {
		temp_ptr = in_text + count - 1;
		while (count-- && isspace(*((unsigned char *) temp_ptr)))
			*temp_ptr-- = 0;
	}

	return(in_text);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

	NAME			: 	oneblank

	SYNOPSIS		:	char *oneblank(text);

						char *text;

	DESCRIPTION	:	Removes multiple consecutive whitespace characters from the
						string to which the ``text`` parameter points, replacing
						them with a single space (ASCII 32).

						Whitespace is defined as any character for which the
						standard library macro ``isspace`` returns true.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``text`` is the string in which multiple whitespace
						characters are to be reduced to a single space.

	RETURNS		:	A pointer to the string used as input.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  nospace
						noblank

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1986 - 2016 Michael L. Brock

   OUTPUT INDEX:  oneblank
						String Justification Functions:oneblank
						STRFUNCS:String Justification Functions:oneblank
						String Functions:See STRFUNCS

   PUBLISH XREF:  oneblank

   PUBLISH NAME:	oneblank

	ENTRY CLASS	:	String Justification Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
char *oneblank(char *text)
#else
char *oneblank(text)
char *text;
#endif /* #ifndef NARGS */
{
	char         *temp_ptr_1 = text;
	char         *temp_ptr_2 = text;
	unsigned int  length;

	length = COMPAT_CAST_static(unsigned int, strlen(text));

	while (*temp_ptr_1) {
		if (isspace(*((unsigned char *) temp_ptr_1))) {
			*text++ = ' ';
			temp_ptr_1++;
			while (isspace(*temp_ptr_1)) {
				temp_ptr_1++;
				length--;
			}
		}
		else
			*text++ = *temp_ptr_1++;
	}

	temp_ptr_2[length] = '\0';

	return(temp_ptr_2);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	nocontrl

   SYNOPSIS    :	return_ptr = nocontrl(text);

						char *return_ptr;

						char *text;

   DESCRIPTION :	Removes non-printable characters from a string.

   PARAMETERS  :	Parameters to this function are as follow:

   					(.) ``text`` is the string on which the function is to
						operate.

	RETURNS		:	A pointer to the string used as input.

   NOTES       :  Control characters are defined as any character for which
						the standard library macro ``isprint`` returns zero.

   CAVEATS     :	

   SEE ALSO    :	

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1986 - 2016 Michael L. Brock

   OUTPUT INDEX:	nocontrl
						Miscellaneous Functions:nocontrl
						STRFUNCS:Miscellaneous Functions:nocontrl
						String Functions:See STRFUNCS

   PUBLISH XREF:	nocontrl

   PUBLISH NAME:	nocontrl

	ENTRY CLASS	:	Miscellaneous Functions

EOH */
/*	***********************************************************************	*/
#ifndef NARGS
char *nocontrl(char *text)
#else
char *nocontrl(text)
char *text;
#endif /* #ifndef NARGS */
{
	register char *temp_ptr_1 = text;
	register char *temp_ptr_2 = text;

	while (*temp_ptr_1) {
		if (isprint(*temp_ptr_1))
			*temp_ptr_2++ = *temp_ptr_1++;
		else
			temp_ptr_1++;
	}

	*temp_ptr_2 = '\0';

	return(text);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	NAME			:	str_digit

	SYNOPSIS		:	return_ptr = str_digit(in_string, sense_flag);

						char       *return_ptr;

						const char *in_string;

						int         sense_flag;

	DESCRIPTION	:	Returns a pointer to the first (non-) numeric character
						within a string.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_string`` is the string in which the search is
						to be performed.

						(.) ``sense_flag`` specifies the sense of the search to
						be performed.

						(..) If ``sense_flag`` is non-zero, then this function will
						locate the first numeric character in ``in_string``.

						(..) If ``sense_flag`` is zero, then this function will
						locate the first non-numeric character in
						``in_string``.

	RETURNS		:	Returns from this function are as follow:

						(.) If a character of the desired type is within the string,
						a pointer to the first character of that type is returned.

						(.) Otherwise, a ''NULL'' pointer is returned.

	NOTES			:	A numeric character is one for which the macro
						``isdigit`` returns a non-zero value.

	CAVEATS		:	

	SEE ALSO		:	strr_digit
						idx_digit
						str_alnum
						str_alpha
						str_ascii
						str_bdigit
						str_cntrl
						str_csym
						str_csymf
						str_graph
						str_lower
						str_odigit
						str_print
						str_punct
						str_space
						str_upper
						str_xdigit

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1986 - 2016 Michael L. Brock

	OUTPUT INDEX:	str_digit
						Character Class Functions:Decimal Digit Character Class Functions:str_digit
						STRFUNCS:Character Class Functions:Decimal Digit Character Class Functions:str_digit
						Character Class Functions:Search Functions/Pointer:str_digit
						STRFUNCS:Character Class Functions:Search Functions/Pointer:str_digit
						String Functions:See STRFUNCS

	PUBLISH XREF:	str_digit

	PUBLISH NAME:	str_digit

	ENTRY CLASS	:	Character Class Functions:Decimal Digit Character Class Functions
						Character Class Functions:Search Functions/Pointer

EOH */
/*	***********************************************************************	*/
#ifndef NARGS
char *str_digit(const char *in_string, int sense_flag)
#else
char *str_digit(in_string, sense_flag)
const char *in_string;
int         sense_flag;
#endif /* #ifndef NARGS */
{
	sense_flag = (sense_flag) ? 1 : 0;

	while (*in_string) {
		if (((isdigit(*in_string)) ? 1 : 0) == sense_flag)
			return(((char *) in_string));
		in_string++;
	}

	return(NULL);
}
/*	***********************************************************************	*/

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  meml_allocate

   SYNOPSIS    :  return_code = meml_allocate(data_count, data_list,
							data_length, element_count);

						int             return_code;

						unsigned int   *data_count;

						void         ***data_list;

						size_t          data_length;

						unsigned int    element_count;

   DESCRIPTION :  Allocates memory for array elements.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``data_count`` points to integer which contains the
						number of elements in the array to which ``data_list``
						points.

						(-) If the function succeeds in allocating elements for the
						array then that integer will be incremented.

						(.) ``data_list`` is a triply-indirect pointer to ``void``
						(``void ***``) which contains the array. If the function
						succeeds allocating elements for the array then the memory
						pointed to by ``data_list`` will be re-allocated to hold
						the newly-added elements.

						(.) ``data_length`` is the length of the area of memory for
						which memory is to be allocaed for each of the elements
						added.

						(-) You may pass a ``data_length`` parameter of ''0'', in
						which case memory will be allocated for the specified
						number of elements in ``element_count``, but the pointer
						in each element will be set to ''NULL''.

						(.) ``element_count`` is the number of elements to add to
						the array.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  meml_append

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1991 - 2016 Michael L. Brock

   OUTPUT INDEX:  meml_allocate
						Memory List Functions:meml_allocate
						STRFUNCS:Memory List Functions:meml_allocate
						String Functions:See STRFUNCS

   PUBLISH XREF:  meml_allocate

   PUBLISH NAME:	meml_allocate

	ENTRY CLASS	:	Memory List Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int meml_allocate(unsigned int *data_count, void ***data_list,
	size_t data_length, unsigned int element_count)
#else
int meml_allocate(data_count, data_list, data_length, element_count)
unsigned int   *data_count;
void         ***data_list;
size_t          data_length;
unsigned int    element_count;
#endif /* #ifndef NARGS */
{
	int            return_code = STRFUNCS_FAILURE;
	void          *tmp_data    = NULL;
	unsigned int   old_data_count;
	unsigned int   count_1;
	void         **tmp_list;

	if (!element_count)
		return_code = STRFUNCS_SUCCESS;
	else {
#ifdef __MSDOS__
		if ((*data_count == UINT_MAX) || ((((unsigned long)
			(*data_count + element_count)) *
			((unsigned long) sizeof(char *))) > ((unsigned long) (UINT_MAX - 15))))
			goto EXIT_FUNCTION;
#endif /* #ifdef __MSDOS__ */
		if (((!(*data_count)) && (*data_list == NULL)) ||
			(*data_count && (*data_list != NULL))) {
			if (data_length) {
				if ((tmp_data = ((void *) calloc(data_length, sizeof(char)))) ==
            	NULL)
					goto EXIT_FUNCTION;
			}
			if (!(*data_list))
				tmp_list = (void **) calloc(element_count, sizeof(void *));
			else
				tmp_list = (void **) realloc(*data_list,
					(*data_count + element_count) * sizeof(void *));
			if (tmp_list == NULL) {
				if (tmp_data)
					free(tmp_data);
			}
			else if (!data_length) {
				memset(((char *) tmp_list) + (*data_count * sizeof(void **)),
					'\0', element_count * sizeof(void **));
				*data_list   = tmp_list;
				*data_count += element_count;
				return_code  = STRFUNCS_SUCCESS;
			}
			else {
				return_code                   = STRFUNCS_SUCCESS;
				old_data_count                = *data_count;
				*data_list                    = tmp_list;
				(*data_list)[(*data_count)++] = tmp_data;
				for (count_1 = 1; count_1 < element_count; count_1++) {
					if ((return_code = meml_append(data_count, data_list,
						tmp_data, data_length)) != STRFUNCS_SUCCESS) {
						meml_remove(data_count, data_list, old_data_count, count_1);
						break;
					}
				}
			}
		}
	}

EXIT_FUNCTION:

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  meml_append

   SYNOPSIS    :  return_code = meml_append(data_count, data_list, in_data,
							data_length);

						int            return_code;

						unsigned int   *data_count;

						void         ***data_list;

						const void     *in_data;

						size_t          data_length;

   DESCRIPTION :  Appends an array element to an array of pointers.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``data_count`` points to integer which contains the
						number of elements in the array to which ``data_list``
						points.

						(-) If the function succeeds in appending the element
						``in_data`` then that integer will be incremented.

						(.) ``data_list`` is a triply-indirect pointer to ``void``
						(``void ***``) which contains the array. If the function
						succeeds in appending the element ``in_data`` then the
						memory pointed to by ``data_list`` will be re-allocated to
						hold the newly-added element.

						(.) ``in_data`` points to an area of memory of
						``data_length`` size. The contents of that area of memory
						will be appended to the array pointed to by ``data_list``.

						(.) ``data_length`` is the length of the area of memory to
						which the ``in_data`` parameter points.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  meml_add
						meml_copy
						meml_insert
						meml_remove
						meml_append

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1991 - 2016 Michael L. Brock

   OUTPUT INDEX:  meml_append
						Memory List Functions:meml_append
						STRFUNCS:Memory List Functions:meml_append
						String Functions:See STRFUNCS

   PUBLISH XREF:  meml_append

   PUBLISH NAME:	meml_append

	ENTRY CLASS	:	Memory List Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int meml_append(unsigned int *data_count, void ***data_list,
	const void *in_data, size_t data_length)
#else
int meml_append(data_count, data_list, in_data, data_length)
unsigned int   *data_count;
void         ***data_list;
const void     *in_data;
size_t          data_length;
#endif /* #ifndef NARGS */
{
	int    return_code = STRFUNCS_FAILURE;
	void  *tmp_data    = NULL;
	void **tmp_list;

	if (((!(*data_count)) && (*data_list == NULL)) ||
		(*data_count && (*data_list != NULL))) {
		if (data_length > 0) {
			if ((tmp_data = ((void *) calloc(data_length, sizeof(char)))) == NULL)
				goto EXIT_FUNCTION;
			memcpy(tmp_data, in_data, data_length);
		}
		if (*data_list == NULL)
			tmp_list = (void **) calloc(1, sizeof(char *));
		else
			tmp_list = (void **) realloc(*data_list,
				(*data_count + 1) * sizeof(char *));
		if (tmp_list != NULL) {
			return_code           = STRFUNCS_SUCCESS;
			tmp_list[*data_count] = tmp_data;
			*data_list            = tmp_list;
			(*data_count)++;
		}
		else if (tmp_data != NULL)
			free(tmp_data);
	}

EXIT_FUNCTION:

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  meml_remove

   SYNOPSIS    :  return_code = meml_remove(data_count, data_list,
							first_element, element_count);

						int             return_code;

						unsigned int   *data_count;

						void         ***data_list;

						unsigned int    first_element;

						unsigned int    element_count;

   DESCRIPTION :  Deletes an array element from an array of pointers.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``data_count`` points to integer which contains the
						number of elements in the array to which ``data_list``
						points.

						(-) If the function succeeds in deleting the specified
						elements then that integer will have subtracted from it
						the number of elements deleted.

						(.) ``data_list`` is a triply-indirect pointer to ``void``
						(``void ***``) which contains the array. If the function
						succeeds in deleting elements then the memory pointed to
						by ``data_list`` will be re-allocated to hold the new size
						of the array.

						(.) ``data_length`` is the length of the array elements.

						(.) ``first_element`` is the array index of the first
						element to be deleted.

						(-) If ``first_element`` exceeds the array index of the
						last array element (that is, if ``first_element`` is
						greater that or equal to ``*data_count``) no work will
						be performed by this function.

						(.) ``element_count`` is the number of elements to be
						deleted.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  meml_add
						meml_append
						meml_copy
						meml_insert
						meml_remove_all
						mema_remove

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1991 - 2016 Michael L. Brock

   OUTPUT INDEX:  meml_remove
						Memory List Functions:meml_remove
						STRFUNCS:Memory List Functions:meml_remove
						String Functions:See STRFUNCS

   PUBLISH XREF:  meml_remove

   PUBLISH NAME:	meml_remove

	ENTRY CLASS	:	Memory List Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int meml_remove(unsigned int *data_count, void ***data_list,
	unsigned int first_element, unsigned int element_count)
#else
int meml_remove(data_count, data_list, first_element, element_count)
unsigned int   *data_count;
void         ***data_list;
unsigned int    first_element;
unsigned int    element_count;
#endif /* #ifndef NARGS */
{
	int            return_code = STRFUNCS_FAILURE;
	unsigned int   count_1;
	unsigned int   data_index  = 0;
	void         **tmp_list    = NULL;

	if (((!(*data_count)) && (*data_list == NULL)) ||
		(*data_count && (*data_list != NULL))) {
		if ((!(*data_count)) || (!element_count) ||
			(first_element >= *data_count))
			return_code = STRFUNCS_SUCCESS;
		else {
			if ((first_element + element_count) > *data_count)
				element_count = *data_count - first_element;
			if (element_count < *data_count) {
				if ((tmp_list = ((void **)
					calloc(*data_count - element_count, sizeof(void *)))) == NULL)
					goto EXIT_FUNCTION;
				for (count_1 = 0; count_1 < *data_count; count_1++) {
					if ((count_1 < first_element) ||
						(count_1 >= (first_element + element_count)))
						tmp_list[data_index++] = (*data_list)[count_1];
				}
			}
			for (count_1 = 0; count_1 < element_count; count_1++) {
				if ((*data_list)[first_element + count_1])
					free((*data_list)[first_element + count_1]);
			}
			free(*data_list);
			*data_list  = tmp_list;
			*data_count = *data_count - element_count;
			return_code = STRFUNCS_SUCCESS;
		}
	}

EXIT_FUNCTION:

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  meml_remove_all

   SYNOPSIS    :  return_code = meml_remove_all(data_count, data_list);

						int             return_code;

						unsigned int   *data_count;

						void         ***data_list;

   DESCRIPTION :  Deletes all elements from an array of pointers.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``data_count`` points to integer which contains the
						number of elements in the array to which ``data_list``
						points.

						(.) ``data_list`` is a triply-indirect pointer to ``void``
						(``void ***``) which contains the array to be deleted.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  meml_add
						meml_append
						meml_copy
						meml_insert
						meml_remove
						mema_remove_all

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1991 - 2016 Michael L. Brock

   OUTPUT INDEX:  meml_remove_all
						Memory List Functions:meml_remove_all
						STRFUNCS:Memory List Functions:meml_remove_all
						String Functions:See STRFUNCS

   PUBLISH XREF:  meml_remove_all

   PUBLISH NAME:	meml_remove_all

	ENTRY CLASS	:	Memory List Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int meml_remove_all(unsigned int *data_count, void ***data_list)
#else
int meml_remove_all(data_count, data_list)
unsigned int   *data_count;
void         ***data_list;
#endif /* #ifndef NARGS */
{
	return(meml_remove(data_count, data_list, 0, *data_count));
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  strl_append

   SYNOPSIS    :  return_code = strl_append(string_count, string_list,
							in_string);

						int              return_code;

						unsigned int    *string_count;

						char          ***string_list;

						const char      *in_string;

   DESCRIPTION :  Appends an array element to an array of pointers to strings.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``string_count`` points to integer which contains the
						number of elements in the array to which ``string_list``
						points.

						(-) If the function succeeds in appending the element
						``in_string`` then that integer will be incremented.

						(.) ``string_list`` is a triply-indirect pointer to ``char``
						(``char ***``) which contains the array. If the function
						succeeds in appending the element ``in_string`` then the
						memory pointed to by ``string_list`` will be re-allocated to
						hold the newly-added element.

						(.) ``in_string`` points to the area of memory to be
						appended to the array pointed to by ``string_list``.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  strln_append
						strl_add
						strl_copy
						strl_insert
						strl_remove
						meml_append
						mema_append

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1991 - 2016 Michael L. Brock

   OUTPUT INDEX:  strl_append
						String List Functions:strl_append
						STRFUNCS:String List Functions:strl_append
						String Functions:See STRFUNCS

   PUBLISH XREF:  strl_append

   PUBLISH NAME:	strl_append

	ENTRY CLASS	:	String List Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int strl_append(unsigned int *string_count, char ***string_list,
	const char *in_string)
#else
int strl_append(string_count, string_list, in_string)
unsigned int    *string_count;
char          ***string_list;
const char      *in_string;
#endif /* #ifndef NARGS */
{
	return(meml_append(string_count, ((void ***) string_list),
		((const void *) in_string), strlen(in_string) + 1));
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  strl_remove

   SYNOPSIS    :  return_code = strl_remove(string_count, string_list,
							first_element, element_count);

						int             return_code;

						unsigned int   *string_count;

						char         ***string_list;

						unsigned int    first_element;

						unsigned int    element_count;

   DESCRIPTION :  Deletes an array element from an array of pointers to
						strings.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``string_count`` points to integer which contains the
						number of elements in the array to which ``string_list``
						points.

						(-) If the function succeeds in deleting the specified
						elements then that integer will have subtracted from it
						the number of elements deleted.

						(.) ``string_list`` is a triply-indirect pointer to ``char``
						(``char ***``) which contains the array. If the function
						succeeds in deleting elements then the memory pointed to
						by ``string_list`` will be re-allocated to hold the new size
						of the array.

						(.) ``first_element`` is the array index of the first
						element to be deleted.

						(-) If ``first_element`` exceeds the array index of the
						last array element (that is, if ``first_element`` is
						greater that or equal to ``*string_count``) no work will
						be performed by this function.

						(.) ``element_count`` is the number of elements to be
						deleted.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  strl_remove_all
						meml_remove

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1991 - 2016 Michael L. Brock

   OUTPUT INDEX:  strl_remove
						String List Functions:strl_remove
						STRFUNCS:String List Functions:strl_remove
						String Functions:See STRFUNCS

   PUBLISH XREF:  strl_remove

   PUBLISH NAME:	strl_remove

	ENTRY CLASS	:	String List Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int strl_remove(unsigned int *string_count, char ***string_list,
	unsigned int first_element, unsigned int element_count)
#else
int strl_remove(string_count, string_list, first_element, element_count)
unsigned int   *string_count;
char         ***string_list;
unsigned int    first_element;
unsigned int    element_count;
#endif /* #ifndef NARGS */
{
	return(meml_remove(string_count, ((void ***) string_list), first_element,
		element_count));
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  strl_remove_all

   SYNOPSIS    :  return_code = strl_remove_all(string_count, string_list);

						int             return_code;

						unsigned int   *string_count;

						char         ***string_list;

   DESCRIPTION :  Deletes all elements from an array of pointers to strings.

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``string_count`` points to integer which contains the
						number of elements in the array to which ``string_list``
						points.

						(.) ``string_list`` is a triply-indirect pointer to ``char``
						(``char ***``) which contains the array to be deleted.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  strl_remove
						meml_remove_all

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1991 - 2016 Michael L. Brock

   OUTPUT INDEX:  strl_remove_all
						String List Functions:strl_remove_all
						STRFUNCS:String List Functions:strl_remove_all
						String Functions:See STRFUNCS

   PUBLISH XREF:  strl_remove_all

   PUBLISH NAME:	strl_remove_all

	ENTRY CLASS	:	String List Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int strl_remove_all(unsigned int *string_count, char ***string_list)
#else
int strl_remove_all(string_count, string_list)
unsigned int   *string_count;
char         ***string_list;
#endif /* #ifndef NARGS */
{
	return(meml_remove_all(string_count, ((void ***) string_list)));
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  strln_append

   SYNOPSIS    :  return_code = strln_append(string_count, string_list,
							in_string, string_length);

						int             return_code;

						unsigned int   *string_count;

						char         ***string_list;

						char           *in_string;

						size_t          string_length;

   DESCRIPTION :  Appends an array element to an array of pointers to strings.
						The string copied from ``in_string`` will be truncated to a
						maximum length of ``string_length`` bytes (excluding the
						terminating ASCII NUL byte).

   PARAMETERS  :  Parameters to this function are as follow:

						(.) ``string_count`` points to integer which contains the
						number of elements in the array to which ``string_list``
						points.

						(-) If the function succeeds in appending the element
						``in_string`` then that integer will be incremented.

						(.) ``string_list`` is a triply-indirect pointer to ``char``
						(``char ***``) which contains the array. If the function
						succeeds in appending the element ``in_string`` then the
						memory pointed to by ``string_list`` will be re-allocated to
						hold the newly-added element.

						(.) ``in_string`` points to the area of memory to be
						appended to the array pointed to by ``string_list``.

						(.) ``string_length`` is the maximum length of ``in_string``
						to be inserted into the array, not including the terminating
						ASCII NUL byte.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  

   CAVEATS     :  

   SEE ALSO    :  strl_append
						strln_add
						strl_copy
						strln_insert
						strl_remove
						meml_append
						mema_append

   EXAMPLES    :  

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1991 - 2016 Michael L. Brock

   OUTPUT INDEX:  strln_append
						String List Functions:strln_append
						STRFUNCS:String List Functions:strln_append
						String Functions:See STRFUNCS

   PUBLISH XREF:  strln_append

   PUBLISH NAME:	strln_append

	ENTRY CLASS	:	String List Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int strln_append(unsigned int *string_count, char ***string_list,
	const char *in_string, size_t string_length)
#else
int strln_append(string_count, string_list, in_string, string_length)
unsigned int   *string_count;
char         ***string_list;
const char     *in_string;
size_t          string_length;
#endif /* #ifndef NARGS */
{
	int return_code;

	if ((return_code = meml_append(string_count, ((void ***) string_list),
		((const void *) in_string), string_length + 1)) == STRFUNCS_SUCCESS) {
		if (string_length < strlen(in_string))
			(*string_list)[*string_count - 1][string_length] = '\0';
	}

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  sepwordl

   SYNOPSIS    :  return_code = sepwordl(in_string, string_count,
							string_list);

						unsigned int    return_code;

						const char     *in_string;

						unsigned int   *string_count;

						char         ***string_list;

   DESCRIPTION :  Transforms a string into an array of pointers to strings.
						The transformation is effected by examining the string for
						the presence of separation characters. Strings composed of
						characters not in the set of separation characters will
						become elements in the array of pointers to strings created
						by this function.

						Separation characters are specified as being white-space
						characters (as defined by the C standard library macro
						``isspace``).

						Multiple instances of the separation characters are
						swallowed by this function.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_string`` is the string to be transformed into an
						array of pointers to strings.

						(.) ``string_count`` points to an integer which will
						contain the number of records in the destination list
						``string_list`` if this function is successful.

						(.) ``string_list`` points to the list to which string
						segments from ``in_string`` are to be copied. Memory will
						be allocated for this list by this function.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  To separate a string by an arbitrary set of characters,
						use the function ``sepwordx`` instead.

   CAVEATS     :  

   SEE ALSO    :  sepwordc
						sepwordc_keep
						sepwords
						sepwordx
						sepwordx_keep
						

	EXAMPLES    :

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1992 - 2016 Michael L. Brock

	OUTPUT INDEX:	sepwordl
						String Splitting Functions:sepwordl
						STRFUNCS:String Splitting Functions:sepwordl
						String Functions:See STRFUNCS

	PUBLISH XREF:	sepwordl

	PUBLISH NAME:	sepwordl

	ENTRY CLASS	:	String Splitting Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int sepwordl(const char *in_string, unsigned int *string_count,
	char ***string_list)
#else
int sepwordl(in_string, string_count, string_list)
const char     *in_string;
unsigned int   *string_count;
char         ***string_list;
#endif /* #ifndef NARGS */
{
	int   return_code = STRFUNCS_SUCCESS;
	char *tmp_ptr_1;

	*string_count = 0;
	*string_list  = NULL;

	while (*in_string) {
		if (!isspace(*in_string)) {
			tmp_ptr_1 = ((char*) in_string);
			while (*tmp_ptr_1 && (!isspace(*tmp_ptr_1)))
				tmp_ptr_1++;
			if ((return_code = strln_append(string_count, string_list, in_string,
				((unsigned int) (tmp_ptr_1 - ((char *) in_string))))) != 0)
				break;
			if (!(*(in_string = tmp_ptr_1)))
				break;
		}
		in_string++;
	}

	if (return_code)
		strl_remove(string_count, string_list, 0, *string_count);

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  sepwordc_keep

   SYNOPSIS    :  return_code = sepwordc_keep(in_string, separator_char,
							string_count, string_list);

						int             return_code;

						unsigned int   *in_string;

						int             separator_char;

						unsigned int   *string_count;

						char         ***string_list;

   DESCRIPTION :  Transforms a string into an array of pointers to strings.
						The transformation is effected by examining the string for
						the presence of separation characters. Strings composed of
						characters not in the set of separation characters will
						become elements in the array of pointers to strings created
						by this function.

						The separation character is specified as being the
						character in the parameter ``separator_char``.

						Multiple instances of the separation characters are
						propagated by this function into the array ``string_list``
						as empty strings.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_string`` is the string to be transformed into an
						array of pointers to strings.

						(.) ``separator_char`` is the character which separates
						portions of the string ``in_string`` to be transformed into
						array elements.

						(-) If ``separator_char`` is ASCII ''0'' (''\0''),
						``in_string`` will be copied in its entirety to
						the array ``string_list``.

						(.) ``string_count`` points to an integer which will
						contain the number of records in the destination list
						``string_list`` if this function is successful.

						(.) ``string_list`` points to the list to which string
						segments from ``in_string`` are to be copied. Memory will
						be allocated for this list by this function.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  If ``separator_char`` is ''NULL'' or is a zero-length
						string, ``in_string`` will be copied in its entirety to
						the array ``string_list``.

						Multiple instances of the separation characters are
						propagated by this function into the array ``string_list``
						as empty strings. If you do not want to keep them as
						empty strings in the resulting list, use the associated
						function ``sepwordc``.

   CAVEATS     :  

   SEE ALSO    :  sepwordl
						sepwords
						sepwordc
						sepwordx_keep

	EXAMPLES    :

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1992 - 2016 Michael L. Brock

	OUTPUT INDEX:	sepwordc_keep
						String Splitting Functions:sepwordc_keep
						STRFUNCS:String Splitting Functions:sepwordc_keep
						String Functions:See STRFUNCS

	PUBLISH XREF:	sepwordc_keep

	PUBLISH NAME:	sepwordc_keep

	ENTRY CLASS	:	String Splitting Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int sepwordc_keep(const char *in_string, int separator_char,
	unsigned int *string_count, char ***string_list)
#else
int sepwordc_keep(in_string, separator_char, string_count, string_list)
const char     *in_string;
int             separator_char;
unsigned int   *string_count;
char         ***string_list;
#endif /* #ifndef NARGS */
{
	int           return_code = STRFUNCS_SUCCESS;
	unsigned int  leading_count;
	char         *tmp_ptr_1;

	*string_count = 0;
	*string_list  = NULL;

	if ((in_string != NULL) && *in_string) {
		if (((char) separator_char) == '\0')
			return_code = strl_append(string_count, string_list, in_string);
		else {
			leading_count = 0;
			while (*in_string && (((char) separator_char) == *in_string)) {
				in_string++;
				leading_count++;
			}
			if (leading_count && ((return_code = meml_allocate(string_count,
				((void ***) string_list), 1,
				leading_count + ((*in_string) ? 0 : 1))) != STRFUNCS_SUCCESS))
				goto EXIT_FUNCTION;
			while (*in_string) {
				if (((char) separator_char) != *in_string) {
					tmp_ptr_1 = ((char *) in_string);
					while (*tmp_ptr_1 && (((char) separator_char) != *tmp_ptr_1))
						tmp_ptr_1++;
					if ((return_code = meml_append(string_count,
						((void ***) string_list), in_string, ((unsigned int)
						(tmp_ptr_1 - ((char *) in_string))) + 1)) !=
						STRFUNCS_SUCCESS)
						goto EXIT_FUNCTION;
					(*string_list)[*string_count - 1]
						[((unsigned int) (tmp_ptr_1 - ((char *) in_string)))] =
						'\0';
					if (!(*(in_string = tmp_ptr_1)))
						break;
				}
				if ((((char) separator_char) == *(in_string + 1)) ||
					(!(*(in_string + 1)))) {
					if ((return_code = strl_append(string_count, string_list,
						"")) != STRFUNCS_SUCCESS)
						break;
				}
				in_string++;
			}
		}
	}

EXIT_FUNCTION:

	if (return_code != STRFUNCS_SUCCESS)
		strl_remove(string_count, string_list, 0, *string_count);

	return(return_code);
}
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* BOH

   NAME        :  sepwordx_keep

   SYNOPSIS    :  return_code = sepwordx_keep(in_string, separator_list,
							string_count, string_list);

						unsigned int    return_code;

						const char     *in_string;

						const char     *separator_list;

						unsigned int   *string_count;

						char         ***string_list;

   DESCRIPTION :  Transforms a string into an array of pointers to strings.
						The transformation is effected by examining the string for
						the presence of separation characters. Strings composed of
						characters not in the set of separation characters will
						become elements in the array of pointers to strings created
						by this function.

						Separation characters are specified as being one of the
						characters in the parameter ``separator_list``.

						Multiple instances of the separation characters are
						propagated by this function into the array ``string_list``
						as empty strings.

	PARAMETERS	:	Parameters to this function are as follow:

						(.) ``in_string`` is the string to be transformed into an
						array of pointers to strings.

						(.) ``separator_list`` is the ASCII NUL terminated string
						which contains the characters which separate portions of
						the string ``in_string`` to be transformed into array
						elements.

						(-) If ``separator_list`` is ''NULL'' or is a zero-length
						string, ``in_string`` will be copied in its entirety to
						the array ``string_list``.

						(.) ``string_count`` points to an integer which will
						contain the number of records in the destination list
						``string_list`` if this function is successful.

						(.) ``string_list`` points to the list to which string
						segments from ``in_string`` are to be copied. Memory will
						be allocated for this list by this function.

   RETURNS     :  Returns from this function are as follow:

						(.) ''STRFUNCS_SUCCESS'' if no error occurred.

						(.) One of the non-zero STRFUNCS return codes if an error
						occurred.

   NOTES       :  If ``separator_list`` is ''NULL'' or is a zero-length
						string, ``in_string`` will be copied in its entirety to
						the array ``string_list``.

						Multiple instances of the separation characters are
						propagated by this function into the array ``string_list``
						as empty strings. If you do not want to keep them as
						empty strings in the resulting list, use the associated
						function ``sepwordx``.

   CAVEATS     :  

   SEE ALSO    :  sepwordl
						sepwords
						sepwordx
						sepwordc_keep

	EXAMPLES    :

   AUTHOR      :  Michael L. Brock

   COPYRIGHT   :  Copyright 1992 - 2016 Michael L. Brock

	OUTPUT INDEX:	sepwordx_keep
						String Splitting Functions:sepwordx_keep
						STRFUNCS:String Splitting Functions:sepwordx_keep
						String Functions:See STRFUNCS

	PUBLISH XREF:	sepwordx_keep

	PUBLISH NAME:	sepwordx_keep

	ENTRY CLASS	:	String Splitting Functions

EOH */
/* *********************************************************************** */
#ifndef NARGS
int sepwordx_keep(const char *in_string, const char *separator_list,
	unsigned int *string_count, char ***string_list)
#else
int sepwordx_keep(in_string, separator_list, string_count, string_list)
const char     *in_string;
const char     *separator_list;
unsigned int   *string_count;
char         ***string_list;
#endif /* #ifndef NARGS */
{
	int           return_code = STRFUNCS_SUCCESS;
	unsigned int  leading_count;
	char         *tmp_ptr_1;

	*string_count = 0;
	*string_list  = NULL;

	if ((in_string != NULL) && *in_string) {
		if ((separator_list == NULL) || (!*separator_list))
			return_code = strl_append(string_count, string_list, in_string);
		else if (!(*(separator_list + 1)))
			return_code = sepwordc_keep(in_string, *separator_list, string_count,
				string_list);
		else {
			leading_count = 0;
			while (*in_string && (strchr(separator_list, *in_string) != NULL)) {
				in_string++;
				leading_count++;
			}
			if (leading_count && ((return_code = meml_allocate(string_count,
				((void ***) string_list), 1,
				leading_count + ((*in_string) ? 0 : 1))) != STRFUNCS_SUCCESS))
				goto EXIT_FUNCTION;
			while (*in_string) {
				if (strchr(separator_list, *in_string) == NULL) {
					tmp_ptr_1 = ((char *) in_string);
					while (*tmp_ptr_1 && (!strchr(separator_list, *tmp_ptr_1)))
						tmp_ptr_1++;
					if ((return_code = meml_append(string_count,
						((void ***) string_list), in_string, ((unsigned int)
						(tmp_ptr_1 - ((char *) in_string))) + 1)) !=
						STRFUNCS_SUCCESS)
						goto EXIT_FUNCTION;
					(*string_list)[*string_count - 1]
						[((unsigned int) (tmp_ptr_1 - ((char *) in_string)))] =
						'\0';
					if (!(*(in_string = tmp_ptr_1)))
						break;
				}
				if ((strchr(separator_list, *(in_string + 1)) != NULL) ||
					(!(*(in_string + 1)))) {
					if ((return_code = strl_append(string_count, string_list,
						"")) != STRFUNCS_SUCCESS)
						break;
				}
				in_string++;
			}
		}
	}

EXIT_FUNCTION:

	if (return_code != STRFUNCS_SUCCESS)
		strl_remove(string_count, string_list, 0, *string_count);

	return(return_code);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	STR_AllocMsgItem

	SYNOPSIS		:	return_ptr = STR_AllocMsgItem(size, buffer, in_format, ...);

						char         *return_ptr;

						unsigned int  size;

						char         *buffer;

						const char   *in_format;

						...

	DESCRIPTION	:	Constructs an error message describing a memory allocation
						error for a scalar.

						Following the string constructed using the ``in_format``
						parameter, this function uses the ``size`` parameter to
						construct a string describing the size of the failed
						allocation attempt.

	PARAMETERS  :  Parameters to this function are as follows:

						(.) ``size`` is the amount of memory which could not be
						allocated.

						(.) ``buffer`` is the string in which the error message
						constructed is to be stored.

						(.) ``in_format`` is a ``sprintf``-type format to be used
						in formatting the subsequent function arguments represented
						by ``...``.

						(.) ``...`` represents a variable argument list to be used,
						in conjunction with the ``in_format`` parameter, to format
						the error message.

	RETURNS     :  Returns the ``buffer`` parameter.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	STR_AllocMsgItemVA
						STR_AllocMsgList
						STR_AllocMsgPtrList

	EXAMPLES		:	If this function is invoked as follows:

{|@|}STR_AllocMsgItem(100, error_text, "%s initialization memory",
	"Unable to allocate");

						Printing the contents of ``error_text`` upon return yields:

<|@|>Unable to allocate initialization memory --- 100 bytes required.

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1995 - 2016 Michael L. Brock

	OUTPUT INDEX:	STR_AllocMsgItem
						Error Description Functions:STR_AllocMsgItem
						STRFUNCS:Functions:STR_AllocMsgItem
						STRFUNCS:Error Description Functions:STR_AllocMsgItem
						String Functions:See STRFUNCS

	PUBLISH XREF:	STR_AllocMsgItem

	PUBLISH NAME:	STR_AllocMsgItem

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
/*VARARGS2*/
#ifndef NO_STDARGS
char *STR_AllocMsgItem(unsigned int size, char *buffer,
	const char *in_format, ...)
#else
char *STR_AllocMsgItem(size, buffer, in_format, va_list)
unsigned int  size;
char         *buffer;
const char   *in_format;
va_dcl
#endif /* #ifndef NO_STDARGS */
{
	va_list argument_ptr;

#ifndef NO_STDARGS
	va_start(argument_ptr, in_format);
#else
	va_start(argument_ptr);
#endif /* #ifndef NO_STDARGS */

	STR_AllocMsgItemVA(size, buffer, in_format, argument_ptr);

	va_end(argument_ptr);

	return(buffer);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	STR_AllocMsgList

	SYNOPSIS		:	return_ptr = STR_AllocMsgList(element_count, element_size,
							buffer, in_format, ...);

						char         *return_ptr;

						unsigned int  element_count;

						unsigned int  element_size;

						char         *buffer;

						const char   *in_format;

						...

	DESCRIPTION	:	Constructs an error message describing a memory allocation
						error for an array.

						Following the string constructed using the ``in_format``
						parameter, this function uses the ``element_count`` and
						``element_size`` parameters to construct a string
						describing the size of the failed allocation attempt.

	PARAMETERS  :  Parameters to this function are as follows:

						(.) ``element_count`` is the number of elements in the
						array for which the allocation attempt failed.

						(.) ``element_size`` is the size of the individual array
						elements for which the allocation attempt failed.

						(.) ``buffer`` is the string in which the error message
						constructed is to be stored.

						(.) ``in_format`` is a ``sprintf``-type format to be used
						in formatting the subsequent function arguments represented
						by ``...``.

						(.) ``...`` represents a variable argument list to be used,
						in conjunction with the ``in_format`` parameter, to format
						the error message.

	RETURNS     :  Returns the ``buffer`` parameter.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	STR_AllocMsgListVA
						STR_AllocMsgItem
						STR_AllocMsgPtrList

	EXAMPLES		:	If this function is invoked as follows:

{|@|}STR_AllocMsgList(10, 35, error_text, "%s the storage array",
	"Unable to allocate");

						Printing the contents of ``error_text`` upon return yields:

<|@|>Unable to allocate the storage array --- 10 elements * 35 size = 350
bytes required.

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1995 - 2016 Michael L. Brock

	OUTPUT INDEX:	STR_AllocMsgList
						Error Description Functions:STR_AllocMsgList
						STRFUNCS:Functions:STR_AllocMsgList
						STRFUNCS:Error Description Functions:STR_AllocMsgList
						String Functions:See STRFUNCS

	PUBLISH XREF:	STR_AllocMsgList

	PUBLISH NAME:	STR_AllocMsgList

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
/*VARARGS2*/
#ifndef NO_STDARGS
char *STR_AllocMsgList(unsigned int element_count, unsigned int element_size,
	char *buffer, const char *in_format, ...)
#else
char *STR_AllocMsgList(element_count, element_size, buffer, in_format,
	va_list)
unsigned int  element_count;
unsigned int  element_size;
char         *buffer;
const char   *in_format;
va_dcl
#endif /* #ifndef NO_STDARGS */
{
	va_list argument_ptr;

#ifndef NO_STDARGS
	va_start(argument_ptr, in_format);
#else
	va_start(argument_ptr);
#endif /* #ifndef NO_STDARGS */

	STR_AllocMsgListVA(element_count, element_size, buffer, in_format,
		argument_ptr);

	va_end(argument_ptr);

	return(buffer);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	STR_AllocMsgPtrList

	SYNOPSIS		:	return_ptr = STR_AllocMsgPtrList(element_count, total_size,
							buffer, in_format, ...);

						char         *return_ptr;

						unsigned int  element_count;

						unsigned int  total_size;

						char         *buffer;

						const char   *in_format;

						...

	DESCRIPTION	:	Constructs an error message describing a memory allocation
						error for an array of pointers to data, such as an array
						of pointers to strings.

						Following the string constructed using the ``in_format``
						parameter, this function uses the ``element_count``,
						``total_size`` parameters, and the size of a ``void *`` to
						construct a string describing the size of the failed
						allocation attempt.

	PARAMETERS  :  Parameters to this function are as follows:

						(.) ``element_count`` is the number of elements in the
						array for which the allocation attempt failed.

						(.) ``total_size`` is the total size of all elements in the
						array for which the allocation attempt failed.

						(.) ``buffer`` is the string in which the error message
						constructed is to be stored.

						(.) ``in_format`` is a ``sprintf``-type format to be used
						in formatting the subsequent function arguments represented
						by ``...``.

						(.) ``...`` represents a variable argument list to be used,
						in conjunction with the ``in_format`` parameter, to format
						the error message.

	RETURNS     :  Returns the ``buffer`` parameter.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	STR_AllocMsgPtrList
						STR_AllocMsgItem
						STR_AllocMsgList

	EXAMPLES		:	If this function is invoked as follows:

{|@|}STR_AllocMsgPtrList(10, 1024, error_text, "%s the string list",
	"Unable to allocate");

						Printing the contents of ``error_text`` upon return yields
						(where ``sizeof(void *)`` = ''4''):

<|@|>Unable to allocate the string list --- (10 pointer elements * 4
(sizeof(void *))) + 1024 memory pointed to = 40 + 1024 = 1064 bytes required.

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1995 - 2016 Michael L. Brock

	OUTPUT INDEX:	STR_AllocMsgPtrList
						Error Description Functions:STR_AllocMsgPtrList
						STRFUNCS:Functions:STR_AllocMsgPtrList
						STRFUNCS:Error Description Functions:STR_AllocMsgPtrList
						String Functions:See STRFUNCS

	PUBLISH XREF:	STR_AllocMsgPtrList

	PUBLISH NAME:	STR_AllocMsgPtrList

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
/*VARARGS2*/
#ifndef NO_STDARGS
char *STR_AllocMsgPtrList(unsigned int element_count,
	unsigned int total_size, char *buffer, const char *in_format, ...)
#else
char *STR_AllocMsgPtrList(element_count, total_size, buffer, in_format,
	va_list)
unsigned int  element_count;
unsigned int  total_size;
char         *buffer;
const char   *in_format;
va_dcl
#endif /* #ifndef NO_STDARGS */
{
	va_list argument_ptr;

#ifndef NO_STDARGS
	va_start(argument_ptr, in_format);
#else
	va_start(argument_ptr);
#endif /* #ifndef NO_STDARGS */

	STR_AllocMsgPtrListVA(element_count, total_size, buffer, in_format,
		argument_ptr);

	va_end(argument_ptr);

	return(buffer);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	STR_AllocMsgItemVA

	SYNOPSIS		:	return_ptr = STR_AllocMsgItemVA(size, buffer, arg_list);

						char         *return_ptr;

						unsigned int  size;

						char         *buffer;

						va_list       arg_list;

	DESCRIPTION	:	Constructs an error message describing a memory allocation
						error for a scalar.

						Following the string constructed using the ``in_format``
						parameter, this function uses the ``size`` parameter to
						construct a string describing the size of the failed
						allocation attempt.

	PARAMETERS  :  Parameters to this function are as follows:

						(.) ``size`` is the amount of memory which could not be
						allocated.

						(.) ``buffer`` is the string in which the error message
						constructed is to be stored.

						(.) ``in_format`` is a ``sprintf``-type format to be used
						in formatting the subsequent function arguments represented
						by the ``arg_list`` parameter.

						(.) ``arg_list`` contains subsequent arguments to this
						function. They will be output in accordance with the
						specifications embedded in the ``in_format`` parameter.

	RETURNS     :  Returns the ``buffer`` parameter.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	STR_AllocMsgItem
						STR_AllocMsgListVA
						STR_AllocMsgPtrListVA

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1995 - 2016 Michael L. Brock

	OUTPUT INDEX:	STR_AllocMsgItemVA
						Error Description Functions:STR_AllocMsgItemVA
						STRFUNCS:Functions:STR_AllocMsgItemVA
						STRFUNCS:Error Description Functions:STR_AllocMsgItemVA
						String Functions:See STRFUNCS

	PUBLISH XREF:	STR_AllocMsgItemVA

	PUBLISH NAME:	STR_AllocMsgItemVA

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
#ifndef NARGS
char *STR_AllocMsgItemVA(unsigned int size, char *buffer,
	const char *in_format, va_list arg_list)
#else
char *STR_AllocMsgItemVA(size, buffer, in_format, arg_list)
unsigned int  size;
char         *buffer;
const char   *in_format;
va_list       arg_list;
#endif /* #ifndef NARGS */
{
	vsprintf(buffer, in_format, arg_list);
	sprintf(buffer + strlen(buffer), " --- %u bytes required.", size);

	return(buffer);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	STR_AllocMsgListVA

	SYNOPSIS		:	return_ptr = STR_AllocMsgListVA(element_count, element_size,
							buffer, in_format, arg_list);

						char         *return_ptr;

						unsigned int  element_count;

						unsigned int  element_size;

						char         *buffer;

						const char   *in_format;

						va_list       arg_list;

	DESCRIPTION	:	Constructs an error message describing a memory allocation
						error for an array.

						Following the string constructed using the ``in_format``
						parameter, this function uses the ``element_count`` and
						``element_size`` parameters to construct a string
						describing the size of the failed allocation attempt.

	PARAMETERS  :  Parameters to this function are as follows:

						(.) ``element_count`` is the number of elements in the
						array for which the allocation attempt failed.

						(.) ``element_size`` is the size of the individual array
						elements for which the allocation attempt failed.

						(.) ``buffer`` is the string in which the error message
						constructed is to be stored.

						(.) ``in_format`` is a ``sprintf``-type format to be used
						in formatting the subsequent function arguments represented
						by the ``arg_list`` parameter.

						(.) ``arg_list`` contains subsequent arguments to this
						function. They will be output in accordance with the
						specifications embedded in the ``in_format`` parameter.

	RETURNS     :  Returns the ``buffer`` parameter.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	STR_AllocMsgList
						STR_AllocMsgItemVA
						STR_AllocMsgPtrListVA

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1995 - 2016 Michael L. Brock

	OUTPUT INDEX:	STR_AllocMsgListVA
						Error Description Functions:STR_AllocMsgListVA
						STRFUNCS:Functions:STR_AllocMsgListVA
						STRFUNCS:Error Description Functions:STR_AllocMsgListVA
						String Functions:See STRFUNCS

	PUBLISH XREF:	STR_AllocMsgListVA

	PUBLISH NAME:	STR_AllocMsgListVA

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
/*VARARGS2*/
#ifndef NARGS
char *STR_AllocMsgListVA(unsigned int element_count, unsigned int element_size,
	char *buffer, const char *in_format, va_list arg_list)
#else
char *STR_AllocMsgListVA(element_count, element_size, buffer, in_format,
	arg_list)
unsigned int  element_count;
unsigned int  element_size;
char         *buffer;
const char   *in_format;
va_list       arg_list;
#endif /* #ifndef NARGS */
{
	vsprintf(buffer, in_format, arg_list);
	sprintf(buffer + strlen(buffer),
		" --- %u elements * %u size = %u bytes required.",
		element_count, element_size, element_count * element_size);

	return(buffer);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	STR_AllocMsgPtrListVA

	SYNOPSIS		:	return_ptr = STR_AllocMsgPtrListVA(element_count, total_size,
							buffer, in_format, arg_list);

						char         *return_ptr;

						unsigned int  element_count;

						unsigned int  total_size;

						char         *buffer;

						const char   *in_format;

						va_list       arg_list;

	DESCRIPTION	:	Constructs an error message describing a memory allocation
						error for an array of pointers to data, such as an array
						of pointers to strings.

						Following the string constructed using the ``in_format``
						parameter, this function uses the ``element_count``,
						``total_size`` parameters, and the size of a ``void *`` to
						construct a string describing the size of the failed
						allocation attempt.

	PARAMETERS  :  Parameters to this function are as follows:

						(.) ``element_count`` is the number of elements in the
						array for which the allocation attempt failed.

						(.) ``total_size`` is the total size of all elements in the
						array for which the allocation attempt failed.

						(.) ``buffer`` is the string in which the error message
						constructed is to be stored.

						(.) ``in_format`` is a ``sprintf``-type format to be used
						in formatting the subsequent function arguments represented
						by the ``arg_list`` parameter.

						(.) ``arg_list`` contains subsequent arguments to this
						function. They will be output in accordance with the
						specifications embedded in the ``in_format`` parameter.

	RETURNS     :  Returns the ``buffer`` parameter.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	STR_AllocMsgPtrList
						STR_AllocMsgItemVA
						STR_AllocMsgListVA

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1995 - 2016 Michael L. Brock

	OUTPUT INDEX:	STR_AllocMsgPtrListVA
						Error Description Functions:STR_AllocMsgPtrListVA
						STRFUNCS:Functions:STR_AllocMsgPtrListVA
						STRFUNCS:Error Description Functions:STR_AllocMsgPtrListVA
						String Functions:See STRFUNCS

	PUBLISH XREF:	STR_AllocMsgPtrListVA

	PUBLISH NAME:	STR_AllocMsgPtrListVA

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
#ifndef NARGS
char *STR_AllocMsgPtrListVA(unsigned int element_count, unsigned int total_size,
	char *buffer, const char *in_format, va_list arg_list)
#else
char *STR_AllocMsgPtrListVA(element_count, total_size, buffer, in_format,
	arg_list)
unsigned int  element_count;
unsigned int  total_size;
char         *buffer;
const char   *in_format;
va_list       arg_list;
#endif /* #ifndef NARGS */
{
	vsprintf(buffer, in_format, arg_list);
	sprintf(buffer + strlen(buffer),
		" --- (%u %s * %u %s) + %u %s = %u + %u = %u bytes required.",
		element_count, "pointer elements", ((unsigned int) sizeof(void *)),
		"(sizeof(void *))", total_size, "memory pointed to",
		((unsigned int) (element_count * sizeof(void *))),
		total_size,
		((unsigned int) ((element_count * sizeof(void *)) + total_size)));

	return(buffer);
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :	extract_file_name

   SYNOPSIS    :	return_ptr = extract_file_name(in_path);

						const char *return_ptr;

						const char *in_path;


   DESCRIPTION :	Locates the file name portion of a path name.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``in_path`` is the path name in which the file name
						portion is to be located.

   RETURNS     :	A pointer to the file-name portion of ``in_path``.

						If ``in_path`` does not contain a file name, a pointer to
						terminating ASCII 0 of ``in_path`` is returned by this
						function.

   NOTES       :	

   CAVEATS     :	

   SEE ALSO    :	

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

   COPYRIGHT   :	Copyright 1996 - 2016 Michael L. Brock

   OUTPUT INDEX:	extract_file_name
						Miscellaneous Functions:extract_file_name
						STRFUNCS:Miscellaneous Functions:extract_file_name
						String Functions:See STRFUNCS

   PUBLISH XREF:	extract_file_name

   PUBLISH NAME:	extract_file_name

	ENTRY CLASS	:	Miscellaneous Functions

EOH */
/*	***********************************************************************	*/
#ifndef NARGS
const char *extract_file_name(const char *in_path)
#else
const char *extract_file_name(in_path)
const char *in_path;
#endif /* #ifndef NARGS */
{
	const char *ptr;

	/*	*****************************************************************
		*****************************************************************
		We need to get the file name without such things as the drive
		specification or directory hierarchy. Different for each OS . . .
		*****************************************************************	*/
#ifdef __MSDOS__
		in_path = ((ptr = strrchr(in_path, ':'))  != NULL) ? (ptr + 1) : in_path;
		in_path = ((ptr = strrchr(in_path, '\\')) != NULL) ? (ptr + 1) : in_path;
		in_path = ((ptr = strrchr(in_path, '/'))  != NULL) ? (ptr + 1) : in_path;
#else
# ifdef _Windows
		in_path = ((ptr = strrchr(in_path, ':'))  != NULL) ? (ptr + 1) : in_path;
		in_path = ((ptr = strrchr(in_path, '\\')) != NULL) ? (ptr + 1) : in_path;
		in_path = ((ptr = strrchr(in_path, '/'))  != NULL) ? (ptr + 1) : in_path;
# else
#  ifdef _VMS_
		in_path = ((ptr = strrchr(in_path, ']'))  != NULL) ? (ptr + 1) : in_path;
		in_path = ((ptr = strrchr(in_path, ':'))  != NULL) ? (ptr + 1) : in_path;
#  else
		in_path = ((ptr = strrchr(in_path, '/'))  != NULL) ? (ptr + 1) : in_path;
#  endif /* # ifdef _VMS_ */
# endif /* # ifdef _Windows */
#endif /* #ifdef __MSDOS__ */
	/*	*****************************************************************	*/

	return(in_path);
}
/*	***********************************************************************	*/

} // namespace NPSLI_Internal

} // namespace MLB

