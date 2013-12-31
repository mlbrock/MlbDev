/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Internal Include File			*/
/* *********************************************************************** */
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the Network Programming Support
								Library (NPSL).

	Revision History	:	1996-04-10 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1996 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/* *********************************************************************** */

#ifndef h__NPSL_H__h

#define h__NPSL_H__h					1

/* *********************************************************************** */
/* *********************************************************************** */
/*		Include necessary header files . . .											*/
/* *********************************************************************** */

#ifdef _Windows
# ifdef _MSC_VER
#  if _MSC_VER >= 1310
#   pragma warning(disable:4668 4820)
#   include <windows.h>
#   pragma warning(default:4668)
#  elif _MSC_VER >= 1300
#   pragma warning(disable:4255 4668)
#   include <windows.h>
#   pragma warning(default:4255 4668)
#  elif _MSC_VER >= 1200
#   pragma warning(disable:4115)
#   include <windows.h>
#   pragma warning(default:4115)
#  else
#   pragma warning(disable:4115 4201 4214 4514)
#   include <windows.h>
#   pragma warning(default:4115 4201 4214)
# 	endif /* #  if _MSC_VER >= 1200 */
# else
#  include <windows.h>
# endif /* # ifdef _MSC_VER */
#else
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
#endif /* # ifdef _Windows */

/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

   BASE NAME   :  NPSL Truth Codes

   NAME        :  NPSL Truth Codes

   DESCRIPTION :  Manifest constants used within NPSL to specify the truth
						or falsity of expressions.

						(.) ''NPSL_TRUE''

						(.) ''NPSL_FALSE''

   NOTES       :  The manifest constant ''NPSL_TRUE'' should be defined as
						a non-zero value. Conversely, the manifest constant
						''NPSL_FALSE'' should be defined as zero (''0'').

   AUTHOR      :  Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2003 Michael L. Brock

   OUTPUT INDEX:  NPSL_TRUE
						NPSL_FALSE
						NPSL Truth Codes:NPSL_TRUE
						NPSL Truth Codes:NPSL_FALSE
						NPSL Manifest Constants:NPSL_TRUE
						NPSL Manifest Constants:NPSL_FALSE
						NPSL Defines:NPSL_TRUE
						NPSL Defines:NPSL_FALSE
						Defines:See Macros and Manifest Constants

   PUBLISH XREF:  NPSL Truth Codes
						NPSL Truth Code
						NPSL truth codes
						NPSL truth code
						NPSL_TRUE
						NPSL_FALSE

   PUBLISH NAME:	NPSL Truth Codes
						NPSL_TRUE
						NPSL_FALSE

	ENTRY CLASS	:	Macros and Manifest Constants

EOH */
/*	***********************************************************************	*/
#define NPSL_TRUE								1
#define NPSL_FALSE							0
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

   BASE NAME   :	NPSL Return Codes

   NAME        :  NPSL Return Codes

   DESCRIPTION :  The NPSL return codes have the following meanings:

						(.) ''NPSL_SUCCESS'' means that the function invoked
						completed without error. ''NPSL_SUCCESS'' is guaranteed to
						be equal to zero (''0'').

						(.) ''NPSL_FAILURE'' indicates that the function invoked
						encountered a general operation failure. This is the
						'catch-all' error code for those errors which do not fit
						into category of one the other NPSL error codes.

						(.) ''NPSL_BAD_ARGS_FAILURE'' indicates that the
						arguments passed to a function invoked were invalid.

						(.) ''NPSL_MEMORY_FAILURE'' indicates that the memory
						required for operation of the function invoked could not be
						allocated.

						(.) ''NPSL_SYSTEM_FAILURE'' indicates that the function
						invoked encountered a failure of a standard library
						function or a system call.

						(.) ''NPSL_SOCKET_FAILURE'' indicates a generalized failure
						not encompassed by one of the specific native socket
						libraries listed below.

						(.) ''NPSL_EACCES'' indicates n attempt was made to access
						a socket in a way forbidden by its access permissions. 

						(.) ''NPSL_EADDRINUSE'' indicates an application attempts to
						bind a socket to an IP address/port that has already been
						used for an existing socket, or a socket that wasn't closed
						properly, or one that is still in the process of closing. 

						(.) ''NPSL_EADDRNOTAVAIL'' indicates that the requested
						address is not valid in its context.

						(.) ''NPSL_EAFNOSUPPORT'' indicates that an address
						incompatible with the requested protocol was used. 

						(.) ''NPSL_EALREADY'' indicates that an operation was
						attempted on a nonblocking socket with an operation already
						in progress.

						(.) ''NPSL_ECONNREFUSED'' indicates that no connection could
						be made because the target machine actively refused it. 

						(.) ''NPSL_ECONNRESET'' indicates that an existing connection
						was forcibly closed by the remote host. 

						(.) ''NPSL_EDESTADDRREQ'' indicates that a required address
						was omitted from an operation on a socket. 

						(.) ''NPSL_EFAULT'' indicates that the operating system
						detected an invalid pointer address in attempting to use a
						pointer argument of a call. 

						(.) ''NPSL_EHOSTUNREACH'' indicates that a socket operation
						was attempted to an unreachable host.

						(.) ''NPSL_EINPROGRESS'' indicates that a blocking operation
						is currently executing and therefore the requested operation
						can not be performed.

						(.) ''NPSL_EINTR'' indicates that the operation was
						interrupted. Under Unix, this interruption was most likely
						the result of receipt of a signal.

						(.) ''NPSL_EINVAL'' indicates an invalid option was supplied
						to a socket library function.

						(.) ''NPSL_EISCONN'' indicates that a connect request was
						made on an already-connected socket.

						(.) ''NPSL_EMSGSIZE'' indicates a message sent on a datagram
						socket was larger than the internal message buffer or some
						other network limit, or the buffer used to receive a datagram
						was smaller than the datagram itself. 

						(.) ''NPSL_ENETDOWN'' indicates that a socket operation
						encountered a dead network. 

						(.) ''NPSL_ENETUNREACH'' indicates that a socket operation
						was attempted to an unreachable network. 

						(.) ''NPSL_ENOBUFS'' indicates that an operation on a socket
						could not be performed because the system lacked sufficient
						buffer space or because a queue was full. 

						(.) ''NPSL_ENOPROTOOPT'' indicates that an unknown, invalid
						or unsupported option or level was specified in an
						``NPSL_GetSockOpt`` or ``NPSL_SetSockOpt`` call.

						(.) ''NPSL_ENOTCONN'' indicates that a request to send or
						receive data was disallowed because the socket is not
						connected,

						(.) ''NPSL_ENOTSOCK'' indicates that an operation was
						attempted on something that is not a socket. 

						(.) ''NPSL_EOPNOTSUPP'' indicates that an operation was
						attempted which is not supported for the type of socket
						object specified.

						(.) ''NPSL_ETIMEDOUT'' indicates that a connection attempt
						failed because the connected party did not properly respond
						after a period of time, or the established connection failed
						because the connected host has failed to respond.

						(.) ''NPSL_EWOULDBLOCK'' indicates that an operation on a
						non-blocking sockets cannot be completed immediately.

						(-) Note that ''NPSL_EWOULDBLOCK'' is a non-fatal error, and
						the operation should be retried later. 

   AUTHOR      :  Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2003 Michael L. Brock

   OUTPUT INDEX:  NPSL_SUCCESS
						NPSL_FAILURE
						NPSL_BAD_ARGS_FAILURE
						NPSL_MEMORY_FAILURE
						NPSL_SYSTEM_FAILURE
						NPSL_SOCKET_FAILURE
						NPSL_EACCES
						NPSL_EADDRINUSE
						NPSL_EADDRNOTAVAIL
						NPSL_EAFNOSUPPORT
						NPSL_EALREADY
						NPSL_ECONNREFUSED
						NPSL_ECONNRESET
						NPSL_EDESTADDRREQ
						NPSL_EFAULT
						NPSL_EHOSTUNREACH
						NPSL_EINPROGRESS
						NPSL_EINTR
						NPSL_EINVAL
						NPSL_EISCONN
						NPSL_EMSGSIZE
						NPSL_ENETDOWN
						NPSL_ENETUNREACH
						NPSL_ENOBUFS
						NPSL_ENOPROTOOPT
						NPSL_ENOTCONN
						NPSL_ENOTSOCK
						NPSL_EOPNOTSUPP
						NPSL_ETIMEDOUT
						NPSL_EWOULDBLOCK
						NPSL Return Codes:NPSL_SUCCESS
						NPSL Return Codes:NPSL_FAILURE
						NPSL Return Codes:NPSL_BAD_ARGS_FAILURE
						NPSL Return Codes:NPSL_MEMORY_FAILURE
						NPSL Return Codes:NPSL_SYSTEM_FAILURE
						NPSL Return Codes:NPSL_SOCKET_FAILURE
						NPSL Return Codes:NPSL_EACCES
						NPSL Return Codes:NPSL_EADDRINUSE
						NPSL Return Codes:NPSL_EADDRNOTAVAIL
						NPSL Return Codes:NPSL_EAFNOSUPPORT
						NPSL Return Codes:NPSL_EALREADY
						NPSL Return Codes:NPSL_ECONNREFUSED
						NPSL Return Codes:NPSL_ECONNRESET
						NPSL Return Codes:NPSL_EDESTADDRREQ
						NPSL Return Codes:NPSL_EFAULT
						NPSL Return Codes:NPSL_EHOSTUNREACH
						NPSL Return Codes:NPSL_EINPROGRESS
						NPSL Return Codes:NPSL_EINTR
						NPSL Return Codes:NPSL_EINVAL
						NPSL Return Codes:NPSL_EISCONN
						NPSL Return Codes:NPSL_EMSGSIZE
						NPSL Return Codes:NPSL_ENETDOWN
						NPSL Return Codes:NPSL_ENETUNREACH
						NPSL Return Codes:NPSL_ENOBUFS
						NPSL Return Codes:NPSL_ENOPROTOOPT
						NPSL Return Codes:NPSL_ENOTCONN
						NPSL Return Codes:NPSL_ENOTSOCK
						NPSL Return Codes:NPSL_EOPNOTSUPP
						NPSL Return Codes:NPSL_ETIMEDOUT
						NPSL Return Codes:NPSL_EWOULDBLOCK
						NPSL Manifest Constants:NPSL_SUCCESS
						NPSL Manifest Constants:NPSL_FAILURE
						NPSL Manifest Constants:NPSL_BAD_ARGS_FAILURE
						NPSL Manifest Constants:NPSL_MEMORY_FAILURE
						NPSL Manifest Constants:NPSL_SYSTEM_FAILURE
						NPSL Manifest Constants:NPSL_SOCKET_FAILURE
						NPSL Manifest Constants:NPSL_EACCES
						NPSL Manifest Constants:NPSL_EADDRINUSE
						NPSL Manifest Constants:NPSL_EADDRNOTAVAIL
						NPSL Manifest Constants:NPSL_EAFNOSUPPORT
						NPSL Manifest Constants:NPSL_EALREADY
						NPSL Manifest Constants:NPSL_ECONNREFUSED
						NPSL Manifest Constants:NPSL_ECONNRESET
						NPSL Manifest Constants:NPSL_EDESTADDRREQ
						NPSL Manifest Constants:NPSL_EFAULT
						NPSL Manifest Constants:NPSL_EHOSTUNREACH
						NPSL Manifest Constants:NPSL_EINPROGRESS
						NPSL Manifest Constants:NPSL_EINTR
						NPSL Manifest Constants:NPSL_EINVAL
						NPSL Manifest Constants:NPSL_EISCONN
						NPSL Manifest Constants:NPSL_EMSGSIZE
						NPSL Manifest Constants:NPSL_ENETDOWN
						NPSL Manifest Constants:NPSL_ENETUNREACH
						NPSL Manifest Constants:NPSL_ENOBUFS
						NPSL Manifest Constants:NPSL_ENOPROTOOPT
						NPSL Manifest Constants:NPSL_ENOTCONN
						NPSL Manifest Constants:NPSL_ENOTSOCK
						NPSL Manifest Constants:NPSL_EOPNOTSUPP
						NPSL Manifest Constants:NPSL_ETIMEDOUT
						NPSL Manifest Constants:NPSL_EWOULDBLOCK
						NPSL Defines:NPSL_SOCKET_FAILURE
						NPSL Defines:NPSL_EACCES
						NPSL Defines:NPSL_EADDRINUSE
						NPSL Defines:NPSL_EADDRNOTAVAIL
						NPSL Defines:NPSL_EAFNOSUPPORT
						NPSL Defines:NPSL_EALREADY
						NPSL Defines:NPSL_ECONNREFUSED
						NPSL Defines:NPSL_ECONNRESET
						NPSL Defines:NPSL_EDESTADDRREQ
						NPSL Defines:NPSL_EFAULT
						NPSL Defines:NPSL_EHOSTUNREACH
						NPSL Defines:NPSL_EINPROGRESS
						NPSL Defines:NPSL_EINTR
						NPSL Defines:NPSL_EINVAL
						NPSL Defines:NPSL_EISCONN
						NPSL Defines:NPSL_EMSGSIZE
						NPSL Defines:NPSL_ENETDOWN
						NPSL Defines:NPSL_ENETUNREACH
						NPSL Defines:NPSL_ENOBUFS
						NPSL Defines:NPSL_ENOPROTOOPT
						NPSL Defines:NPSL_ENOTCONN
						NPSL Defines:NPSL_ENOTSOCK
						NPSL Defines:NPSL_EOPNOTSUPP
						NPSL Defines:NPSL_ETIMEDOUT
						NPSL Defines:NPSL_EWOULDBLOCK
						NPSL Defines:NPSL_SUCCESS
						NPSL Defines:NPSL_FAILURE
						NPSL Defines:NPSL_BAD_ARGS_FAILURE
						NPSL Defines:NPSL_MEMORY_FAILURE
						NPSL Defines:NPSL_SYSTEM_FAILURE
						Defines:See Macros and Manifest Constants

   PUBLISH XREF:  NPSL_SUCCESS
						NPSL_FAILURE
						NPSL_BAD_ARGS_FAILURE
						NPSL_MEMORY_FAILURE
						NPSL_SYSTEM_FAILURE
						NPSL_SOCKET_FAILURE
						NPSL_EACCES
						NPSL_EADDRINUSE
						NPSL_EADDRNOTAVAIL
						NPSL_EAFNOSUPPORT
						NPSL_EALREADY
						NPSL_ECONNREFUSED
						NPSL_ECONNRESET
						NPSL_EDESTADDRREQ
						NPSL_EFAULT
						NPSL_EHOSTUNREACH
						NPSL_EINPROGRESS
						NPSL_EINTR
						NPSL_EINVAL
						NPSL_EISCONN
						NPSL_EMSGSIZE
						NPSL_ENETDOWN
						NPSL_ENETUNREACH
						NPSL_ENOBUFS
						NPSL_ENOPROTOOPT
						NPSL_ENOTCONN
						NPSL_ENOTSOCK
						NPSL_EOPNOTSUPP
						NPSL_ETIMEDOUT
						NPSL_EWOULDBLOCK
						NPSL Return Codes
						NPSL Return Code
						NPSL return codes
						NPSL return code
						NPSL Status Codes
						NPSL Status Code
						NPSL status codes
						NPSL status code
						NPSL Error Codes
						NPSL Error Code
						NPSL error codes
						NPSL error code

   PUBLISH NAME:	NPSL Return Codes
						NPSL return codes
						NPSL_SUCCESS
						NPSL_FAILURE
						NPSL_BAD_ARGS_FAILURE
						NPSL_MEMORY_FAILURE
						NPSL_SYSTEM_FAILURE
						NPSL_SOCKET_FAILURE
						NPSL_EACCES
						NPSL_EADDRINUSE
						NPSL_EADDRNOTAVAIL
						NPSL_EAFNOSUPPORT
						NPSL_EALREADY
						NPSL_ECONNREFUSED
						NPSL_ECONNRESET
						NPSL_EDESTADDRREQ
						NPSL_EFAULT
						NPSL_EHOSTUNREACH
						NPSL_EINPROGRESS
						NPSL_EINTR
						NPSL_EINVAL
						NPSL_EISCONN
						NPSL_EMSGSIZE
						NPSL_ENETDOWN
						NPSL_ENETUNREACH
						NPSL_ENOBUFS
						NPSL_ENOPROTOOPT
						NPSL_ENOTCONN
						NPSL_ENOTSOCK
						NPSL_EOPNOTSUPP
						NPSL_ETIMEDOUT
						NPSL_EWOULDBLOCK

	ENTRY CLASS	:	Macros and Manifest Constants

EOH */
/*	***********************************************************************	*/
#define NPSL_SUCCESS						 	 0
#define NPSL_FAILURE							-1
#define NPSL_BAD_ARGS_FAILURE				-2
#define NPSL_MEMORY_FAILURE				-3
#define NPSL_SYSTEM_FAILURE				-4
#define NPSL_SOCKET_FAILURE				-10000
#define NPSL_EACCES							(NPSL_SOCKET_FAILURE +  -1)
#define NPSL_EADDRINUSE						(NPSL_SOCKET_FAILURE +  -2)
#define NPSL_EADDRNOTAVAIL					(NPSL_SOCKET_FAILURE +  -3)
#define NPSL_EAFNOSUPPORT					(NPSL_SOCKET_FAILURE +  -4)
#define NPSL_EALREADY						(NPSL_SOCKET_FAILURE +  -5)
#define NPSL_ECONNREFUSED					(NPSL_SOCKET_FAILURE +  -6)
#define NPSL_ECONNRESET						(NPSL_SOCKET_FAILURE +  -7)
#define NPSL_EDESTADDRREQ					(NPSL_SOCKET_FAILURE +  -8)
#define NPSL_EFAULT							(NPSL_SOCKET_FAILURE +  -9)
#define NPSL_EHOSTUNREACH					(NPSL_SOCKET_FAILURE + -10)
#define NPSL_EINPROGRESS					(NPSL_SOCKET_FAILURE + -11)
#define NPSL_EINTR							(NPSL_SOCKET_FAILURE + -12)
#define NPSL_EINVAL							(NPSL_SOCKET_FAILURE + -13)
#define NPSL_EISCONN							(NPSL_SOCKET_FAILURE + -14)
#define NPSL_EMSGSIZE						(NPSL_SOCKET_FAILURE + -15)
#define NPSL_ENETDOWN						(NPSL_SOCKET_FAILURE + -16)
#define NPSL_ENETUNREACH					(NPSL_SOCKET_FAILURE + -17)
#define NPSL_ENOBUFS							(NPSL_SOCKET_FAILURE + -18)
#define NPSL_ENOPROTOOPT					(NPSL_SOCKET_FAILURE + -19)
#define NPSL_ENOTCONN						(NPSL_SOCKET_FAILURE + -20)
#define NPSL_ENOTSOCK						(NPSL_SOCKET_FAILURE + -21)
#define NPSL_EOPNOTSUPP						(NPSL_SOCKET_FAILURE + -22)
#define NPSL_ETIMEDOUT						(NPSL_SOCKET_FAILURE + -23)
#define NPSL_EWOULDBLOCK					(NPSL_SOCKET_FAILURE + -24)
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	BOH

   NAME        :  NPSL_MAX_ERROR_TEXT

   DESCRIPTION :  The manifest constant ''NPSL_MAX_ERROR_TEXT'' defines
						the minimum amount of storage which should be allocated to
						hold an error message returned by a NPSL function.

   CAVEATS     :  NPSL error messages can be quite lengthy. Therefore,
						attempting to save space by allocating less than
						''NPSL_MAX_ERROR_TEXT'' bytes to error message buffers
						passed to NPSL functions can lead to overwriting of the
						heap or the stack.

   AUTHOR      :  Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2003 Michael L. Brock

   OUTPUT INDEX:  NPSL_MAX_ERROR_TEXT
						NPSL Manifest Constants:NPSL_MAX_ERROR_TEXT
						NPSL Defines:NPSL_MAX_ERROR_TEXT
						Defines:See Macros and Manifest Constants

   PUBLISH XREF:	NPSL_MAX_ERROR_TEXT

   PUBLISH NAME:	NPSL_MAX_ERROR_TEXT

	ENTRY CLASS	:	Macros and Manifest Constants

EOH */
/*	***********************************************************************	*/
#define NPSL_MAX_ERROR_TEXT				2048
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#define NPSL_MAX_IPV4_ADDR_STR			((4 * 4) + 3)
#define NPSL_MAX_IPV6_ADDR_STR			((4 * 8) * 7)
#define NPSL_MAX_INET_ADDR_STR			NPSL_MAX_IPV6_ADDR_STR
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef __SVR4
# define NPSL_HAS_GETHOSTBYADDR_R		NPSL_TRUE
# define NPSL_HAS_GETHOSTBYNAME_R		NPSL_TRUE
# define NPSL_HAS_GETSERVBYPORT_R		NPSL_TRUE
# define NPSL_HAS_GETSERVBYNAME_R		NPSL_TRUE
# define NPSL_HAS_GETPROTOBYPORT_R		NPSL_TRUE
# define NPSL_HAS_GETPROTOBYNAME_R		NPSL_TRUE
#else
# define NPSL_HAS_GETHOSTBYADDR_R		NPSL_FALSE
# define NPSL_HAS_GETHOSTBYNAME_R		NPSL_FALSE
# define NPSL_HAS_GETSERVBYPORT_R		NPSL_FALSE
# define NPSL_HAS_GETSERVBYNAME_R		NPSL_FALSE
# define NPSL_HAS_GETPROTOBYPORT_R		NPSL_FALSE
# define NPSL_HAS_GETPROTOBYNAME_R		NPSL_FALSE
#endif /* #ifdef __SVR4 */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
typedef SOCKET NPSL_SOCKET_HANDLE;
#else
typedef int NPSL_SOCKET_HANDLE;
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
# define NPSL_INVALID_SOCKET				INVALID_SOCKET
#else
# define NPSL_INVALID_SOCKET				(-1)
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
# define NPSL_INADDR_NONE					INADDR_NONE
#else
# define NPSL_INADDR_NONE					(-1)
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
# define NPSL_SOCKET_OK						0
# define NPSL_SOCKET_ERROR					SOCKET_ERROR
#else
# define NPSL_SOCKET_OK						0
# define NPSL_SOCKET_ERROR					(-1)
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
typedef int NPSL_SOCKLEN_T;
#elif defined(linux)
typedef socklen_t NPSL_SOCKLEN_T;
#else
typedef int NPSL_SOCKLEN_T;
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
typedef SOCKADDR NPSL_SOCKADDR;
#else
typedef struct sockaddr NPSL_SOCKADDR;
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
typedef SOCKADDR_IN NPSL_SOCKADDR_IN;
#else
typedef struct sockaddr_in NPSL_SOCKADDR_IN;
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
typedef struct hostent NPSL_HOSTENT;
#else
typedef struct hostent NPSL_HOSTENT;
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
typedef SERVENT NPSL_SERVENT;
#else
typedef struct servent NPSL_SERVENT;
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
typedef PROTOENT NPSL_PROTOENT;
#else
typedef struct protoent NPSL_PROTOENT;
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#ifdef _Windows
# ifdef SD_BOTH
#  define NPSL_SHUTDOWN_FLAG_RECV		SD_RECV
#  define NPSL_SHUTDOWN_FLAG_SEND		SD_SEND
#  define NPSL_SHUTDOWN_FLAG_BOTH		SD_BOTH
# else
#  define NPSL_SHUTDOWN_FLAG_RECV		0
#  define NPSL_SHUTDOWN_FLAG_SEND		1
#  define NPSL_SHUTDOWN_FLAG_BOTH		2
# endif /* # ifdef SD_BOTH */
#else
# define NPSL_SHUTDOWN_FLAG_RECV			0
# define NPSL_SHUTDOWN_FLAG_SEND			1
# define NPSL_SHUTDOWN_FLAG_BOTH			2
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
typedef fd_set NPSL_FD_SET_T;
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#define NPSL_FD_CLR(socket_handle, fd_set)	FD_CLR(socket_handle, fd_set)
#define NPSL_FD_ISSET(socket_handle, fd_set)	FD_ISSET(socket_handle, fd_set)
#define NPSL_FD_SET(socket_handle, fd_set)	FD_SET(socket_handle, fd_set)
#define NPSL_FD_ZERO(fd_set)						FD_ZERO(fd_set)
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
typedef struct {
	unsigned char octet[6];
} NPSL_ADDR_MAC;
/*	***********************************************************************	*/

/*
	ADF NOTE:	No ADF.
*/
/*	***********************************************************************	*/
#define NPSL_MAX_HOST_NAME_LEN			4096
/*	***********************************************************************	*/

/* *********************************************************************** */
/* *********************************************************************** */
/* Type used by the native socket library as the {get|set}sockopt() data	*/
/* value parameter.																			*/
/* *********************************************************************** */
typedef char *NPSL_NATIVE_SocketOptionValueType;
/* *********************************************************************** */

/* *********************************************************************** */
typedef struct {
	int   error_code;
	char *error_name;
	char *short_description;
	char *long_description;
} NPSL_SocketErrorSpec;
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/* 	Function prototypes for functions defined within 'npsl.a' . . .		*/
/* *********************************************************************** */

	/*	*****************************************************************
		*****************************************************************
		Core socket support function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_Accept,
	(NPSL_SOCKET_HANDLE in_socket_handle, NPSL_SOCKET_HANDLE *out_socket_handle,
	NPSL_SOCKADDR *addr, NPSL_SOCKLEN_T *addrlen, char *error_text));
COMPAT_FN_DECL(int NPSL_Bind,
	(NPSL_SOCKET_HANDLE socket_handle, const NPSL_SOCKADDR *name,
	NPSL_SOCKLEN_T addrlen, char *error_text));
COMPAT_FN_DECL(int NPSL_CloseSocket,
	(NPSL_SOCKET_HANDLE socket_handle, char *error_text));
COMPAT_FN_DECL(int NPSL_Connect,
	(NPSL_SOCKET_HANDLE socket_handle, const NPSL_SOCKADDR *name,
	NPSL_SOCKLEN_T addrlen, char *error_text));
COMPAT_FN_DECL(int NPSL_GetPeerName,
	(NPSL_SOCKET_HANDLE socket_handle, NPSL_SOCKADDR *name,
	NPSL_SOCKLEN_T *addrlen, char *error_text));
COMPAT_FN_DECL(int NPSL_GetSockName,
	(NPSL_SOCKET_HANDLE socket_handle, NPSL_SOCKADDR *name,
	NPSL_SOCKLEN_T *addrlen, char *error_text));
COMPAT_FN_DECL(int NPSL_GetSockOpt,
	(NPSL_SOCKET_HANDLE socket_handle, int opt_level, int opt_name,
	void *opt_value, int *opt_length, char *error_text));
COMPAT_FN_DECL(int NPSL_Listen,
	(NPSL_SOCKET_HANDLE socket_handle, int backlog, char *error_text));
COMPAT_FN_DECL(int NPSL_Recv,
	(NPSL_SOCKET_HANDLE socket_handle, char *buffer, unsigned int buffer_length,
	int flags, unsigned int *recv_count, char *error_text));
COMPAT_FN_DECL(int NPSL_RecvFrom,
	(NPSL_SOCKET_HANDLE socket_handle, char *buffer, unsigned int buffer_length,
	int flags, NPSL_SOCKADDR *addr, NPSL_SOCKLEN_T *addrlen,
	unsigned int *recv_count, char *error_text));
COMPAT_FN_DECL(int NPSL_Select,
	(int fd_set_size, NPSL_FD_SET_T *read_fds, NPSL_FD_SET_T *write_fds,
	NPSL_FD_SET_T *error_fds, const struct timeval *time_out,
	unsigned int *ready_count, char *error_text));
COMPAT_FN_DECL(int NPSL_Send,
	(NPSL_SOCKET_HANDLE socket_handle, const char *buffer,
	unsigned int buffer_length, int flags, unsigned int *send_count,
	char *error_text));
COMPAT_FN_DECL(int NPSL_SendTo,
	(NPSL_SOCKET_HANDLE socket_handle, const char *buffer,
	unsigned int buffer_length, int flags, const NPSL_SOCKADDR *addr,
	NPSL_SOCKLEN_T addrlen, unsigned int *send_count, char *error_text));
COMPAT_FN_DECL(int NPSL_SetSockOpt,
	(NPSL_SOCKET_HANDLE socket_handle, int opt_level, int opt_name,
	const void *opt_value, int opt_length, char *error_text));
COMPAT_FN_DECL(int NPSL_ShutDown,
	(NPSL_SOCKET_HANDLE socket_handle, int shut_down_flag, char *error_text));
COMPAT_FN_DECL(int NPSL_Socket,
	(int socket_domain, int socket_type, int socket_protocol,
	NPSL_SOCKET_HANDLE *socket_handle, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Some specialized {get|set} socket option function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_GetSockOpt_RcvBuf, (NPSL_SOCKET_HANDLE socket_handle,
	unsigned int *buffer_size, char *error_text));
COMPAT_FN_DECL(int NPSL_GetSockOpt_SndBuf, (NPSL_SOCKET_HANDLE socket_handle,
	unsigned int *buffer_size, char *error_text));
COMPAT_FN_DECL(int NPSL_SetSockOpt_RcvBuf, (NPSL_SOCKET_HANDLE socket_handle,
	unsigned int buffer_size, char *error_text));
COMPAT_FN_DECL(int NPSL_SetSockOpt_SndBuf, (NPSL_SOCKET_HANDLE socket_handle,
	unsigned int buffer_size, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Some supporting socket function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_CheckSocket,
	(NPSL_SOCKET_HANDLE socket_handle, char *error_text));
COMPAT_FN_DECL(int NPSL_SetBlockingModeOff,
	(NPSL_SOCKET_HANDLE socket_handle, char *error_text));
COMPAT_FN_DECL(int NPSL_SetBlockingModeOn,
	(NPSL_SOCKET_HANDLE socket_handle, char *error_text));
COMPAT_FN_DECL(int NPSL_SetBlockingMode,
	(NPSL_SOCKET_HANDLE socket_handle, int blocking_mode_flag, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Various NPSL resolution function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_ResolveHostToName,
	(const char *host_string, char *out_host, char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveHostToAddrStruct,
	(const char *host_string, struct in_addr *out_host, char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveHostToINet,
	(const char *host_string, char *out_host, char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveHostToULong,
	(const char *host_string, unsigned long *out_host, char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveHostBasic,
	(const char *host_string, char *out_host, struct in_addr *out_host_in_addr,
	char *out_host_inet, unsigned long *out_host_ulong, char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveProtoToSint,
	(const char *proto_string, int *out_proto, char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveServToPort,
	(const char *serv_string, const char *serv_proto, int *out_port,
	char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveServToPortBasic,
	(const char *serv_string, const char *serv_proto, int *out_port,
	char *out_proto_name, int *out_proto, char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveHostServProtoString,
	(const char *in_string, char *out_host, int *out_port, int *out_proto,
	char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveProtoHostServString,
	(const char *in_string, char *out_host, int *out_port, int *out_proto,
	char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveHostServProtoStringBasic,
	(int phs_flag, const char *in_string, char *out_host, int *out_port,
	int *out_proto, char *error_text));
COMPAT_FN_DECL(int NPSL_ResolveHostServProto,
	(const char *host_string, const char *serv_string, const char *proto_string,
	char *out_host, int *out_port, int *out_proto, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Get host entry function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_GetHostByAddr,
	(const char *host_addr, int host_addr_len, int host_addr_type,
	NPSL_HOSTENT *host_ent_ptr, void *host_ent_buffer_ptr,
	unsigned int host_ent_buffer_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetHostByINet,
	(const char *host_inet, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetHostByName,
	(const char *host_name, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetHostByString,
	(const char *host_string, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Basic get host entry function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_GetHostByAddrBasic,
	(const char *host_addr, int host_addr_len, int host_addr_type,
	NPSL_HOSTENT *host_ent_ptr, void *host_ent_buffer_ptr,
	unsigned int host_ent_buffer_length, unsigned int *required_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetHostByINetBasic,
	(const char *host_inet, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetHostByNameBasic,
	(const char *host_name, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetHostByStringBasic,
	(const char *host_string, NPSL_HOSTENT *host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	unsigned int *required_length, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Host entry structure management function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_CopyHostEntFlat,
	(const NPSL_HOSTENT *in_host_ent_ptr, NPSL_HOSTENT *out_host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_CopyHostEntFlatBasic,
	(const NPSL_HOSTENT *in_host_ent_ptr, NPSL_HOSTENT *out_host_ent_ptr,
	void *host_ent_buffer_ptr, unsigned int host_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int  NPSL_CopyHostEnt,
	(const NPSL_HOSTENT *in_host_ent_ptr, NPSL_HOSTENT *out_host_ent_ptr,
	char *error_text));
COMPAT_FN_DECL(int NPSL_CopyHostEntList,
	(unsigned int in_count, const NPSL_HOSTENT *in_list,
	unsigned int *out_count, NPSL_HOSTENT **out_list, char *error_text));
COMPAT_FN_DECL(void NPSL_FreeHostEnt,
	(NPSL_HOSTENT *ptr));
COMPAT_FN_DECL(void NPSL_FreeHostEntList,
	(unsigned int *in_count, NPSL_HOSTENT **in_list));
COMPAT_FN_DECL(void NPSL_InitHostEnt,
	(NPSL_HOSTENT *ptr));
COMPAT_FN_DECL(void NPSL_InitHostEntList,
	(unsigned int in_count, NPSL_HOSTENT *in_list));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Get protocol entry function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_GetProtoByNumberString,
	(const char *proto_number_string, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetProtoByNumber,
	(int proto_number, NPSL_PROTOENT *proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetProtoByName,
	(const char *proto_name, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetProtoByString,
	(const char *proto_string, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Basic get protocol entry function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_GetProtoByNumberStringBasic,
	(const char *proto_number_string, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetProtoByNumberBasic,
	(int proto_number, NPSL_PROTOENT *proto_ent_ptr, void *proto_ent_buffer_ptr,
	unsigned int proto_ent_buffer_length, unsigned int *required_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetProtoByNameBasic,
	(const char *proto_name, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetProtoByStringBasic,
	(const char *proto_string, NPSL_PROTOENT *proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	unsigned int *required_length, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Protocol entry structure management function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_CopyProtoEntFlat,
	(const NPSL_PROTOENT *in_proto_ent_ptr, NPSL_PROTOENT *out_proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_CopyProtoEntFlatBasic,
	(const NPSL_PROTOENT *in_proto_ent_ptr, NPSL_PROTOENT *out_proto_ent_ptr,
	void *proto_ent_buffer_ptr, unsigned int proto_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int  NPSL_CopyProtoEnt,
	(const NPSL_PROTOENT *in_proto_ent_ptr, NPSL_PROTOENT *out_proto_ent_ptr,
	char *error_text));
COMPAT_FN_DECL(int NPSL_CopyProtoEntList,
	(unsigned int in_count, const NPSL_PROTOENT *in_list,
	unsigned int *out_count, NPSL_PROTOENT **out_list, char *error_text));
COMPAT_FN_DECL(void NPSL_FreeProtoEnt,
	(NPSL_PROTOENT *ptr));
COMPAT_FN_DECL(void NPSL_FreeProtoEntList,
	(unsigned int *in_count, NPSL_PROTOENT **in_list));
COMPAT_FN_DECL(void NPSL_InitProtoEnt,
	(NPSL_PROTOENT *ptr));
COMPAT_FN_DECL(void NPSL_InitProtoEntList,
	(unsigned int in_count, NPSL_PROTOENT *in_list));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Get service entry function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_GetServByPortString,
	(const char *serv_port_string, const char *serv_proto,
	NPSL_SERVENT *serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetServByPort,
	(int serv_port, const char *serv_proto, NPSL_SERVENT *serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetServByName,
	(const char *serv_name, const char *serv_proto, NPSL_SERVENT *serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetServByString,
	(const char *serv_string, const char *serv_proto, NPSL_SERVENT *serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Basic get service entry function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_GetServByPortStringBasic,
	(const char *serv_port_string, const char *serv_proto,
	NPSL_SERVENT *serv_ent_ptr, void *serv_ent_buffer_ptr,
	unsigned int serv_ent_buffer_length, unsigned int *required_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_GetServByPortBasic,
	(int serv_port, const char *serv_proto, NPSL_SERVENT *serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetServByNameBasic,
	(const char *serv_name, const char *serv_proto, NPSL_SERVENT *serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int NPSL_GetServByStringBasic,
	(const char *serv_string, const char *serv_proto, NPSL_SERVENT *serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	unsigned int *required_length, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Service entry structure management function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_CopyServEntFlat,
	(const NPSL_SERVENT *in_serv_ent_ptr, NPSL_SERVENT *out_serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	char *error_text));
COMPAT_FN_DECL(int NPSL_CopyServEntFlatBasic,
	(const NPSL_SERVENT *in_serv_ent_ptr, NPSL_SERVENT *out_serv_ent_ptr,
	void *serv_ent_buffer_ptr, unsigned int serv_ent_buffer_length,
	unsigned int *required_length, char *error_text));
COMPAT_FN_DECL(int  NPSL_CopyServEnt,
	(const NPSL_SERVENT *in_serv_ent_ptr, NPSL_SERVENT *out_serv_ent_ptr,
	char *error_text));
COMPAT_FN_DECL(int NPSL_CopyServEntList,
	(unsigned int in_count, const NPSL_SERVENT *in_list,
	unsigned int *out_count, NPSL_SERVENT **out_list, char *error_text));
COMPAT_FN_DECL(void NPSL_FreeServEnt,
	(NPSL_SERVENT *ptr));
COMPAT_FN_DECL(void NPSL_FreeServEntList,
	(unsigned int *in_count, NPSL_SERVENT **in_list));
COMPAT_FN_DECL(void NPSL_InitServEnt,
	(NPSL_SERVENT *ptr));
COMPAT_FN_DECL(void NPSL_InitServEntList,
	(unsigned int in_count, NPSL_SERVENT *in_list));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Socket library start-up and clean-up function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_SocketLibStartUp, (void *user_data_ptr,
	char *error_text));
COMPAT_FN_DECL(int NPSL_SocketLibCleanUp, (void *user_data_ptr,
	char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Host name expansion functions . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_ExpandHostNameList,
	(unsigned int in_host_count, char **in_host_list,
	unsigned int *out_host_count, char ***out_host_list, char *error_text));
COMPAT_FN_DECL(int NPSL_ExpandHostNameString,
	(const char *in_host_string, unsigned int *out_host_count,
	char ***out_host_list, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Host name function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_DetermineHostName,
	(char *host_name, char *error_text));
COMPAT_FN_DECL(int NPSL_GetHostName,
	(char *host_name, unsigned int host_name_length));
COMPAT_FN_DECL(int NPSL_SetHostName,
	(const char *host_name));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Network address function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_GetMAC,
	(NPSL_ADDR_MAC *ethernet_address, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Error description function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(void NPSL_GetErrorList,
	(unsigned int *out_count, const NPSL_SocketErrorSpec **out_list));
COMPAT_FN_DECL(char *NPSL_AppendErrorString,
	(int errno_code, int long_flag, unsigned int max_text, char *error_text));
COMPAT_FN_DECL(char *NPSL_AppendLastErrorString,
	(int long_flag, unsigned int max_text, char *error_text));
COMPAT_FN_DECL(char *NPSL_GetErrorString,
	(int errno_code, int long_flag, unsigned int max_text, char *error_text));
COMPAT_FN_DECL(char *NPSL_GetLastErrorString,
	(int long_flag, unsigned int max_text, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Address conversion function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(char          *NPSL_AddrToINet,
	(const char *in_addr, char *out_inet));
COMPAT_FN_DECL(char          *NPSL_AddrStructToINet,
	(const struct in_addr *in_addr, char *out_inet));
COMPAT_FN_DECL(char          *NPSL_AddrULongToINet,
	(unsigned long in_addr, char *out_inet));
COMPAT_FN_DECL(unsigned long  NPSL_INetToAddr,
	(const char *in_inet));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Basic address conversion function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_AddrToINetBasic,
	(const char *in_addr, char *out_inet, char *error_text));
COMPAT_FN_DECL(int NPSL_AddrStructToINetBasic,
	(const struct in_addr *in_addr, char *out_inet, char *error_text));
COMPAT_FN_DECL(int NPSL_AddrULongToINetBasic,
	(unsigned long in_addr, char *out_inet, char *error_text));
COMPAT_FN_DECL(int NPSL_INetToAddrBasic,
	(const char *in_inet, unsigned long *out_addr, char *error_text));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Determine lengths and counts for NPSL_HOSTENT function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(unsigned int NPSL_GetHostEntAddressCount,
	(const NPSL_HOSTENT *hent_ptr));
COMPAT_FN_DECL(unsigned int NPSL_GetHostEntAddressLength,
	(const NPSL_HOSTENT *hent_ptr));
COMPAT_FN_DECL(unsigned int NPSL_GetHostEntAddressCountAndLength,
	(const NPSL_HOSTENT *hent_ptr,
	unsigned int *address_length));
COMPAT_FN_DECL(unsigned int NPSL_GetHostEntAliasesCount,
	(const NPSL_HOSTENT *hent_ptr));
COMPAT_FN_DECL(unsigned int NPSL_GetHostEntAliasesLength,
	(const NPSL_HOSTENT *hent_ptr));
COMPAT_FN_DECL(unsigned int NPSL_GetHostEntAliasesCountAndLength,
	(const NPSL_HOSTENT *hent_ptr, unsigned int *alias_length));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Determine lengths and counts for NPSL_PROTOENT function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(unsigned int NPSL_GetProtoEntAliasesCount,
	(const NPSL_PROTOENT *pent_ptr));
COMPAT_FN_DECL(unsigned int NPSL_GetProtoEntAliasesLength,
	(const NPSL_PROTOENT *pent_ptr));
COMPAT_FN_DECL(unsigned int NPSL_GetProtoEntAliasesCountAndLength,
	(const NPSL_PROTOENT *pent_ptr, unsigned int *alias_length));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Determine lengths and counts for NPSL_SERVENT function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(unsigned int NPSL_GetServEntAliasesCount,
	(const NPSL_SERVENT *sent_ptr));
COMPAT_FN_DECL(unsigned int NPSL_GetServEntAliasesLength,
	(const NPSL_SERVENT *sent_ptr));
COMPAT_FN_DECL(unsigned int NPSL_GetServEntAliasesCountAndLength,
	(const NPSL_SERVENT *sent_ptr, unsigned int *alias_length));
	/*	*****************************************************************	*/

	/*	*****************************************************************
		*****************************************************************
		Miscellaneous supporting function prototypes . . .
		*****************************************************************	*/
COMPAT_FN_DECL(int NPSL_SUPP_MapLastError,
	(void));
COMPAT_FN_DECL(int NPSL_SUPP_MapError,
	(int native_error));
	/*	*****************************************************************	*/

/* *********************************************************************** */

#endif /* #ifndef h__NPSL_H__h */

