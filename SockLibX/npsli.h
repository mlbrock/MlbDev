/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Internal Include File			*/
/* *********************************************************************** */
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the Internal Network Programming
								Support Library (NPSL).

	Revision History	:	1996-04-10 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1996 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/* *********************************************************************** */

#ifndef h__NPSLI_H__h

#define h__NPSLI_H__h						1

/* *********************************************************************** */
/* *********************************************************************** */
/*		Include necessary header files . . .											*/
/* *********************************************************************** */

#include <Utility.hpp>

#include "npsl.h"

#include <stdarg.h>

/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	BASE NAME	:	NPSL Truth Code Mappings

   NAME        :	NPSL Truth Code Mappings

	SYNOPSIS		:	truth_code = NPSL_MAP_TRUTH_STR(str_code);

						truth_code = NPSL_MAP_TRUTH_GEN(gen_code);

	DESCRIPTION	:	The NPSL truth code mapping macros map truth codes from
						various low-level libraries to NPSL truth codes.

						(.) ''NPSL_MAP_TRUTH_STR'' maps a truth code
						from a STRFUNCS truth code to a NPSL truth code.

						(.) ''NPSL_MAP_TRUTH_GEN'' maps a truth code
						from a GENFUNCS truth code to a NPSL truth code.

	PARAMETERS	:	The single parameter to these macros is the value of the
						truth code to be mapped to its equivalent NPSL truth
						code.

   NOTES       :	To map return codes from the libraries, use the related
						NPSL return code mapping macros.

   AUTHOR      :	Michael L. Brock

	COPYRIGHT	:	Copyright 1996 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_MAP_TRUTH_STR
						NPSL_MAP_TRUTH_GEN
						Truth Code Mappings:NPSL_MAP_TRUTH_STR
						Truth Code Mappings:NPSL_MAP_TRUTH_GEN
						Macros and Manifest Constants:NPSL_MAP_TRUTH_STR
						Macros and Manifest Constants:NPSL_MAP_TRUTH_GEN
						Defines:See Macros and Manifest Constants

   PUBLISH XREF:	NPSL_MAP_TRUTH_STR
						NPSL_MAP_TRUTH_GEN
						NPSL Truth Code Mappings
						NPSL Truth Code Mapping
						NPSL truth code mappings
						NPSL truth code mapping

   PUBLISH NAME:	NPSL_MAP_TRUTH_STR
						NPSL_MAP_TRUTH_GEN

	ENTRY CLASS	:	Macros and Manifest Constants:Truth Code Mappings

EOH */
/*	***********************************************************************	*/
#define NPSL_MAP_TRUTH_STR(str_code)					\
	(((str_code) == STRFUNCS_TRUE)  ? NPSL_TRUE : NPSL_FALSE)
#define NPSL_MAP_TRUTH_GEN(gen_code)					\
	(((gen_code) == GENFUNCS_TRUE)  ? NPSL_TRUE : NPSL_FALSE)
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

	BASE NAME	:	NPSL Return Code Mappings

   NAME        :	NPSL Return Code Mappings

	SYNOPSIS		:	return_code = NPSL_MAP_ERROR_STR(str_code);

						return_code = NPSL_MAP_ERROR_GEN(gen_code);

	DESCRIPTION	:	The NPSL return code mapping macros map return codes from
						various low-level libraries to NPSL return codes.

						(.) ''NPSL_MAP_ERROR_STR'' maps a return code
						from a STRFUNCS return code to a NPSL return code.

						(.) ''NPSL_MAP_ERROR_GEN'' maps a return code
						from a GENFUNCS return code to a NPSL return code.

	PARAMETERS	:	The single parameter to these macros is the value of the
						return code to be mapped to its equivalent NPSL return
						code.

   NOTES       :	To map truth codes from the libraries, use the related
						NPSL truth code mapping macros.

   AUTHOR      :	Michael L. Brock

	COPYRIGHT	:	Copyright 1996 - 2017 Michael L. Brock

   OUTPUT INDEX:	NPSL_MAP_ERROR_STR
						NPSL_MAP_ERROR_GEN
						Return Code Mappings:NPSL_MAP_ERROR_STR
						Return Code Mappings:NPSL_MAP_ERROR_GEN
						Macros and Manifest Constants:NPSL_MAP_ERROR_STR
						Macros and Manifest Constants:NPSL_MAP_ERROR_GEN
						Defines:See Macros and Manifest Constants

   PUBLISH XREF:	NPSL_MAP_ERROR_STR
						NPSL_MAP_ERROR_GEN
						NPSL Return Code Mappings
						NPSL Return Code Mapping
						NPSL return code mappings
						NPSL return code mapping
						NPSL Status Code Mappings
						NPSL Status Code Mapping
						NPSL status code mappings
						NPSL status code mapping
						NPSL Error Code Mappings
						NPSL Error Code Mapping
						NPSL error code mappings
						NPSL error code mapping

   PUBLISH NAME:	NPSL_MAP_ERROR_STR
						NPSL_MAP_ERROR_GEN

	ENTRY CLASS	:	Macros and Manifest Constants:Return Code Mappings

EOH */
/*	***********************************************************************	*/
#define NPSL_MAP_ERROR_STR(str_code)										 			\
	(((str_code) == STRFUNCS_SUCCESS)          ? NPSL_SUCCESS          :	\
	 ((str_code) == STRFUNCS_FAILURE)          ? NPSL_FAILURE          :	\
	 ((str_code) == STRFUNCS_BAD_ARGS_FAILURE) ? NPSL_BAD_ARGS_FAILURE :	\
	 ((str_code) == STRFUNCS_MEMORY_FAILURE)   ? NPSL_MEMORY_FAILURE   :	\
	 ((str_code) == STRFUNCS_SYSTEM_FAILURE)   ? NPSL_SYSTEM_FAILURE   :	\
	 NPSL_FAILURE)
#define NPSL_MAP_ERROR_GEN(gen_code)													\
	(((gen_code) == GENFUNCS_SUCCESS)          ? NPSL_SUCCESS          :	\
	 ((gen_code) == GENFUNCS_FAILURE)          ? NPSL_FAILURE          :	\
	 ((gen_code) == GENFUNCS_BAD_ARGS_FAILURE) ? NPSL_BAD_ARGS_FAILURE :	\
	 ((gen_code) == GENFUNCS_MEMORY_FAILURE)   ? NPSL_MEMORY_FAILURE   :	\
	 ((gen_code) == GENFUNCS_SYSTEM_FAILURE)   ? NPSL_SYSTEM_FAILURE   :	\
	 NPSL_FAILURE)
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
typedef int NPSL_SEND_RECV_LEN_T;
#elif defined(__linux__)
typedef ssize_t NPSL_SEND_RECV_LEN_T;
#else
typedef size_t NPSL_SEND_RECV_LEN_T;
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/* *********************************************************************** */
/* *********************************************************************** */
/* Function prototypes for internal functions defined within 'siidr.a'.		*/
/* *********************************************************************** */

	/*	*****************************************************************
		*****************************************************************
		Error description function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(const NPSL_SocketErrorSpec *NPSL_GetErrorPtr,
	(int errno_code));
COMPAT_FN_DECL(const NPSL_SocketErrorSpec *NPSL_GetLastErrorPtr,
	(void));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Miscellaneous function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_CheckHostEntParams,
	(const NPSL_HOSTENT *host_ent_ptr, const void *host_ent_buffer_ptr,
	unsigned int host_ent_buffer_length, char *error_text));
COMPAT_FN_DECL(int NPSL_CheckProtoEntParams,
	(const NPSL_PROTOENT *proto_ent_ptr, const void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, char *error_text));
COMPAT_FN_DECL(int NPSL_CheckServEntParams,
	(const NPSL_SERVENT *serv_ent_ptr, const void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, char *error_text));
	/*	*****************************************************************	*/

/* *********************************************************************** */

namespace MLB {

namespace NPSLI_Internal {

//	////////////////////////////////////////////////////////////////////////////
const int STRFUNCS_SUCCESS = 0;
const int STRFUNCS_FAILURE = -1;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *NPSLI_GetErrorFromException(const std::exception &except,
	char *error_text);

size_t chrcnt(const char *search_string, int target_char);
char *trim(char *in_text);
char *ltrim(char *in_text);
char *rtrim(char *in_text);
char *oneblank(char *text);
char *nocontrl(char *text);
char *str_digit(const char *in_string, int sense_flag);

int meml_allocate(unsigned int *data_count, void ***data_list,
	size_t data_length, unsigned int element_count);
int meml_append(unsigned int *data_count, void ***data_list,
	const void *in_data, size_t data_length);
int meml_remove(unsigned int *data_count, void ***data_list,
	unsigned int first_element, unsigned int element_count);
int meml_remove_all(unsigned int *data_count, void ***data_list);

int strl_append(unsigned int *string_count, char ***string_list,
	const char *in_string);
int strl_remove(unsigned int *string_count, char ***string_list,
	unsigned int first_element, unsigned int element_count);
int strl_remove_all(unsigned int *string_count, char ***string_list);
int strln_append(unsigned int *string_count, char ***string_list,
	const char *in_string, size_t string_length);

int sepwordl(const char *in_string, unsigned int *string_count,
	char ***string_list);
int sepwordc_keep(const char *in_string, int separator_char,
	unsigned int *string_count, char ***string_list);
int sepwordx_keep(const char *in_string, const char *separator_list,
	unsigned int *string_count, char ***string_list);

	/*	*****************************************************************
		*****************************************************************
		Allocation error message function prototypes . . .
		*****************************************************************	*/
#ifdef __GNUC__
COMPAT_FN_DECL(char *STR_AllocMsgItem, (unsigned int size, char *buffer,
	const char *in_format, ...) __attribute__ ((format (printf, 3, 4))));
COMPAT_FN_DECL(char *STR_AllocMsgList, (unsigned int element_count,
	unsigned int element_size, char *buffer, const char *in_format, ...)
	__attribute__ ((format (printf, 4, 5))));
COMPAT_FN_DECL(char *STR_AllocMsgPtrList, (unsigned int element_count,
	unsigned int total_size, char *buffer, const char *in_format, ...)
	__attribute__ ((format (printf, 4, 5))));
#else
# ifndef NO_STDARGS
COMPAT_FN_DECL(char *STR_AllocMsgItem, (unsigned int size, char *buffer,
	const char *in_format, ...));
COMPAT_FN_DECL(char *STR_AllocMsgList, (unsigned int element_count,
	unsigned int element_size, char *buffer, const char *in_format, ...));
COMPAT_FN_DECL(char *STR_AllocMsgPtrList, (unsigned int element_count,
	unsigned int total_size, char *buffer, const char *in_format, ...));
# else
COMPAT_FN_DECL(char *STR_AllocMsgItem, (unsigned int size, char *buffer,
	const char *in_format, va_alist));
COMPAT_FN_DECL(char *STR_AllocMsgList, (unsigned int element_count,
	unsigned int element_size, char *buffer, const char *in_format, va_alist));
COMPAT_FN_DECL(char *STR_AllocMsgPtrList, (unsigned int element_count,
	unsigned int total_size, char *buffer, const char *in_format, va_alist));
# endif /* # ifndef NO_STDARGS */
#endif /* #ifdef __GNUC__ */
COMPAT_FN_DECL(char *STR_AllocMsgItemVA, (unsigned int size, char *buffer,
	const char *in_format, va_list arg_list));
COMPAT_FN_DECL(char *STR_AllocMsgListVA, (unsigned int element_count,
	unsigned int element_size, char *buffer, const char *in_format,
	va_list arg_list));
COMPAT_FN_DECL(char *STR_AllocMsgPtrListVA, (unsigned int element_count,
	unsigned int total_size, char *buffer, const char *in_format,
	va_list arg_list));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Miscellaneous
		*****************************************************************	*/
COMPAT_FN_DECL(const char *extract_file_name, (const char *in_path));
	/*	*****************************************************************	*/
//	////////////////////////////////////////////////////////////////////////////

} // namespace NPSLI_Internal

} // namespace MLB

#endif /* #ifndef h__NPSLI_H__h */

