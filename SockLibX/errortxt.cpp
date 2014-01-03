/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Retrieves networking sub-system error strings.

	Revision History	:	1993-06-09 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/*	***********************************************************************	*/

/* *********************************************************************** */
/* *********************************************************************** */
/* 	Include necessary header files . . .											*/
/* *********************************************************************** */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "npsli.h"

#include <Utility/IntToString.hpp>

/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/*		The list of error descriptions for various operating systems . . .	*/
/* *********************************************************************** */

#ifdef _Windows
static const NPSL_SocketErrorSpec NPSL_ErrorList[] = {
	{	WSAEACCES,	"WSAEACCES",
		"Permission denied",
		"An attempt was made to access a socket in a way forbidden by its \
access permissions. An example is using a broadcast address for sendto \
without broadcast permission being set using setsockopt(SO_BROADCAST)."
	},

	{	WSAEADDRINUSE,	"WSAEADDRINUSE",
		"Address already in use",
		"Typically, only one usage of each socket address (protocol/IP \
address/port) is permitted. This error occurs if an application attempts \
to bind a socket to an IP address/port that has already been used for an \
existing socket, or a socket that wasn't closed properly, or one that is \
still in the process of closing. For server applications that need to bind \
multiple sockets to the same port number, consider using \
setsockopt(SO_REUSEADDR). Client applications usually need not call bind \
at all connect chooses an unused port automatically. When bind is called \
with a wildcard address (involving ADDR_ANY), a WSAEADDRINUSE error could \
be delayed until the specific address is committed. This could happen with \
a call to another function later, including connect, listen, WSAConnect, \
or WSAJoinLeaf."
	},
	{	WSAEADDRNOTAVAIL,	"WSAEADDRNOTAVAIL",
		"Cannot assign requested address",
		"The requested address is not valid in its context. This normally \
results from an attempt to bind to an address that is not valid for the \
local machine. This can also result from connect, sendto, WSAConnect, \
WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for \
a remote machine (for example, address or port 0)."
	},
	{	WSAEAFNOSUPPORT,	"WSAEAFNOSUPPORT",
		"Address family not supported by protocol family",
		"An address incompatible with the requested protocol was used. All \
sockets are created with an associated address family (that is, AF_INET for \
Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). \
This error is returned if an incorrect protocol is explicitly requested in \
the socket call, or if an address of the wrong family is used for a socket, \
for example, in sendto."
	},
	{	WSAEALREADY,	"WSAEALREADY",
		"Operation already in progress",
		"An operation was attempted on a nonblocking socket with an operation \
already in progress that is, calling connect a second time on a nonblocking \
socket that is already connecting, or canceling an asynchronous request \
(WSAAsyncGetXbyY) that has already been canceled or completed."
	},
	{	WSAECONNABORTED,	"WSAECONNABORTED",
		"Software caused connection abort",
		"An established connection was aborted by the software in your host \
machine, possibly due to a data transmission time-out or protocol error."
	},
	{	WSAECONNREFUSED,	"WSAECONNREFUSED",
		"Connection refused",
		"No connection could be made because the target machine actively \
refused it. This usually results from trying to connect to a service that \
is inactive on the foreign host that is, one with no server application \
running."
	},
	{	WSAECONNRESET,	"WSAECONNRESET",
		"Connection reset by peer",
		"An existing connection was forcibly closed by the remote host. This \
normally results if the peer application on the remote host is suddenly \
stopped, the host is rebooted, or the remote host uses a hard close (see \
setsockopt for more information on the SO_LINGER option on the remote \
socket.) This error may also result if a connection was broken due to \
keep-alive activity detecting a failure while one or more operations are \
in progress. Operations that were in progress fail with WSAENETRESET. \
Subsequent operations fail with WSAECONNRESET."
	},
	{	WSAEDESTADDRREQ,	"WSAEDESTADDRREQ",
		"Destination address required",
		"A required address was omitted from an operation on a socket. For \
example, this error is returned if sendto is called with the remote address \
of ADDR_ANY."
	},
	{	WSAEFAULT,	"WSAEFAULT",
		"Bad address",
		"The system detected an invalid pointer address in attempting to use \
a pointer argument of a call. This error occurs if an application passes an \
invalid pointer value, or if the length of the buffer is too small. For \
instance, if the length of an argument, which is a SOCKADDR structure, is \
smaller than the sizeof(SOCKADDR)."
	},
	{	WSAEHOSTDOWN,	"WSAEHOSTDOWN",
		"Host is down",
		"A socket operation failed because the destination host is down. A \
socket operation encountered a dead host. Networking activity on the local \
host has not been initiated. These conditions are more likely to be \
indicated by the error WSAETIMEDOUT."
	},
	{	WSAEHOSTUNREACH,	"WSAEHOSTUNREACH",
		"No route to host",
		"A socket operation was attempted to an unreachable host. See \
WSAENETUNREACH."
	},
	{	WSAEINPROGRESS,	"WSAEINPROGRESS",
		"Operation now in progress",
		"A blocking operation is currently executing. Windows Sockets only \
allows a single blocking operation per-task or thread to be outstanding, \
and if any other function call is made (whether or not it references that \
or any other socket) the function fails with the WSAEINPROGRESS error."
	},
	{	WSAEINTR,	"WSAEINTR",
		"Interrupted function call",
		"A blocking operation was interrupted by a call to \
WSACancelBlockingCall."
	},
	{	WSAEINVAL,	"WSAEINVAL",
		"Invalid argument",
		"Some invalid argument was supplied (for example, specifying an \
invalid level to the setsockopt function). In some instances, it also \
refers to the current state of the socket for instance, calling accept \
on a socket that is not listening."
	},
	{	WSAEISCONN,	"WSAEISCONN",
		"Socket is already connected",
		"A connect request was made on an already-connected socket. Some \
implementations also return this error if sendto is called on a connected \
SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter in sendto is \
ignored) although other implementations treat this as a legal occurrence."
	},
	{	WSAEMFILE,	"WSAEMFILE",
		"Too many open files",
		"Too many open sockets. Each implementation may have a maximum number \
of socket handles available, either globally, per process, or per thread."
	},
	{	WSAEMSGSIZE,	"WSAEMSGSIZE",
		"Message too long",
		"A message sent on a datagram socket was larger than the internal \
message buffer or some other network limit, or the buffer used to receive a \
datagram was smaller than the datagram itself."
	},
	{	WSAENETDOWN,	"WSAENETDOWN",
		"Network is down",
		"A socket operation encountered a dead network. This could indicate \
a serious failure of the network system (that is, the protocol stack that \
the Windows Sockets DLL runs over), the network interface, or the local \
network itself."
	},
	{	WSAENETRESET,	"WSAENETRESET",
		"Network dropped connection on reset",
		"The connection has been broken due to keep-alive activity detecting \
a failure while the operation was in progress. It can also be returned by \
setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that \
has already failed."
	},
	{	WSAENETUNREACH,	"WSAENETUNREACH",
		"Network is unreachable",
		"A socket operation was attempted to an unreachable network. This \
usually means the local software knows no route to reach the remote host."
	},
	{	WSAENOBUFS,	"WSAENOBUFS",
		"No buffer space available",
		"An operation on a socket could not be performed because the system \
lacked sufficient buffer space or because a queue was full."
	},
	{	WSAENOPROTOOPT,	"WSAENOPROTOOPT",
		"Bad protocol option",
		"An unknown, invalid or unsupported option or level was specified in \
a getsockopt or setsockopt call."
	},
	{	WSAENOTCONN,	"WSAENOTCONN",
		"Socket is not connected",
		"A request to send or receive data was disallowed because the socket \
is not connected and (when sending on a datagram socket using sendto) no \
address was supplied. Any other type of operation might also return this \
error --- for example, setsockopt setting SO_KEEPALIVE if the connection \
has been reset."
	},
	{	WSAENOTSOCK,	"WSAENOTSOCK",
		"Socket operation on nonsocket",
		"An operation was attempted on something that is not a socket. Either \
the socket handle parameter did not reference a valid socket, or for select, \
a member of an fd_set was not valid."
	},
	{	WSAEOPNOTSUPP,	"WSAEOPNOTSUPP",
		"Operation not supported",
		"The attempted operation is not supported for the type of object \
referenced. Usually this occurs when a socket descriptor to a socket that \
cannot support this operation is trying to accept a connection on a datagram \
socket."
	},
	{	WSAEPFNOSUPPORT,	"WSAEPFNOSUPPORT",
		"Protocol family not supported",
		"The protocol family has not been configured into the system or no \
implementation for it exists. This message has a slightly different meaning \
from WSAEAFNOSUPPORT. However, it is interchangeable in most cases, and all \
Windows Sockets functions that return one of these messages also specify \
WSAEAFNOSUPPORT."
	},
	{	WSAEPROCLIM,	"WSAEPROCLIM",
		"Too many processes",
		"A Windows Sockets implementation may have a limit on the number of \
applications that can use it simultaneously. WSAStartup may fail with this \
error if the limit has been reached."
	},
	{	WSAEPROTONOSUPPORT,	"WSAEPROTONOSUPPORT",
		"Protocol not supported",
		"The requested protocol has not been configured into the system, or \
no implementation for it exists. For example, a socket call requests a \
SOCK_DGRAM socket, but specifies a stream protocol."
	},
	{	WSAEPROTOTYPE,	"WSAEPROTOTYPE",
		"Protocol wrong type for socket",
		"A protocol was specified in the socket function call that does not \
support the semantics of the socket type requested. For example, the ARPA \
Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM."
	},
	{	WSAESHUTDOWN,	"WSAESHUTDOWN",
		"Cannot send after socket shutdown",
		"A request to send or receive data was disallowed because the socket \
had already been shut down in that direction with a previous shutdown call. \
By calling shutdown a partial close of a socket is requested, which is a \
signal that sending or receiving, or both have been discontinued."
	},
	{	WSAESOCKTNOSUPPORT,	"WSAESOCKTNOSUPPORT",
		"Socket type not supported",
		"The support for the specified socket type does not exist in this \
address family. For example, the optional type SOCK_RAW might be selected \
in a socket call, and the implementation does not support SOCK_RAW sockets \
at all."
	},
	{	WSAETIMEDOUT,	"WSAETIMEDOUT",
		"Connection timed out",
		"A connection attempt failed because the connected party did not \
properly respond after a period of time, or the established connection \
failed because the connected host has failed to respond."
	},
# ifdef WSATYPE_NOT_FOUND
	{	WSATYPE_NOT_FOUND,	"WSATYPE_NOT_FOUND",
		"Class type not found",
		"The specified class was not found."
	},
# endif /* #ifdef WSATYPE_NOT_FOUND */
	{	WSAEWOULDBLOCK,	"WSAEWOULDBLOCK",
		"Resource temporarily unavailable",
		"This error is returned from operations on nonblocking sockets that \
cannot be completed immediately, for example recv when no data is queued \
to be read from the socket. It is a nonfatal error, and the operation \
should be retried later. It is normal for WSAEWOULDBLOCK to be reported as \
the result from calling connect on a nonblocking SOCK_STREAM socket, since \
some time must elapse for the connection to be established."
	},
	{	WSAHOST_NOT_FOUND,	"WSAHOST_NOT_FOUND",
		"Host not found",
		"No such host is known. The name is not an official host name or \
alias, or it cannot be found in the database(s) being queried. This error \
may also be returned for protocol and service queries, and means that the \
specified name could not be found in the relevant database."
	},
# ifdef WSA_INVALID_HANDLE
	{	WSA_INVALID_HANDLE,	"WSA_INVALID_HANDLE",
		"Specified event object handle is invalid",
		"An application attempts to use an event object, but the specified \
handle is not valid."
	},
# endif /* #ifdef WSA_INVALID_HANDLE */
# ifdef WSA_INVALID_PARAMETER
	{	WSA_INVALID_PARAMETER,	"WSA_INVALID_PARAMETER",
		"One or more parameters are invalid",
		"An application used a Windows Sockets function which directly maps \
to a Win32 function. The Win32 function is indicating a problem with one or \
more parameters."
	},
# endif /* #ifdef WSA_INVALID_PARAMETER */
# ifdef WSAINVALIDPROCTABLE
	{	WSAINVALIDPROCTABLE,	"WSAINVALIDPROCTABLE",
		"Invalid procedure table from service provider",
		"A service provider returned a bogus procedure table to Ws2_32.dll. \
(Usually caused by one or more of the function pointers being null.)"
	},
# endif /* #ifdef WSAINVALIDPROCTABLE */
# ifdef WSAINVALIDPROVIDER
	{	WSAINVALIDPROVIDER,	"WSAINVALIDPROVIDER",
		"Invalid service provider version number",
		"A service provider returned a version number other than 2.0."
	},
# endif /* #ifdef WSAINVALIDPROVIDER */
# ifdef WSA_IO_INCOMPLETE
	{	WSA_IO_INCOMPLETE,	"WSA_IO_INCOMPLETE",
		"Overlapped I/O event object not in signaled state",
		"The application has tried to determine the status of an overlapped \
operation which is not yet completed. Applications that use \
WSAGetOverlappedResult (with the fWait flag set to FALSE) in a polling \
mode to determine when an overlapped operation has completed, get this \
error code until the operation is complete."
	},
# endif /* #ifdef WSA_IO_INCOMPLETE */
# ifdef WSA_IO_PENDING
	{	WSA_IO_PENDING,	"WSA_IO_PENDING",
		"Overlapped operations will complete later",
		"The application has initiated an overlapped operation that cannot \
be completed immediately. A completion indication will be given later when \
the operation has been completed."
	},
# endif /* #ifdef WSA_IO_PENDING */
# ifdef WSA_NOT_ENOUGH_MEMORY
		{	WSA_NOT_ENOUGH_MEMORY,	"WSA_NOT_ENOUGH_MEMORY",
		"Insufficient memory available",
		"An application used a Windows Sockets function that directly maps to \
a Win32 function. The Win32 function is indicating a lack of required \
memory resources."
	},
# endif /* #ifdef WSA_NOT_ENOUGH_MEMORY */
	{	WSANOTINITIALISED,	"WSANOTINITIALISED",
		"Successful WSAStartup not yet performed",
		"Either the application hasn't called WSAStartup or WSAStartup \
failed. The application may be accessing a socket that the current active \
task does not own (that is, trying to share a socket between tasks), or \
WSACleanup has been called too many times."
	},
	{	WSANO_DATA,	"WSANO_DATA",
		"Valid name, no data record of requested type",
		"The requested name is valid and was found in the database, but it \
does not have the correct associated data being resolved for. The usual \
example for this is a host name-to-address translation attempt (using \
gethostbyname or WSAAsyncGetHostByName) which uses the DNS (Domain Name \
Server). An MX record is returned but no A record indicating the host itself \
exists, but is not directly reachable."
	},
	{	WSANO_RECOVERY,	"WSANO_RECOVERY",
		"This is a nonrecoverable error",
		"This indicates some sort of nonrecoverable error occurred during a \
database lookup. This may be because the database files (for example, \
BSD-compatible HOSTS, SERVICES, or PROTOCOLS files) could not be found, \
or a DNS request was returned by the server with a severe error."
	},
# ifdef WSAPROVIDERFAILEDINIT
	{	WSAPROVIDERFAILEDINIT,	"WSAPROVIDERFAILEDINIT",
		"Unable to initialize a service provider",
		"Either a service provider's DLL could not be loaded (LoadLibrary \
failed) or the provider's WSPStartup/NSPStartup function failed."
	},
# endif /* #ifdef WSAPROVIDERFAILEDINIT */
# ifdef WSASYSCALLFAILURE
	{	WSASYSCALLFAILURE,	"WSASYSCALLFAILURE",
		"System call failure",
		"Returned when a system call that should never fail does. For example, \
if a call to WaitForMultipleObjects fails or one of the registry functions \
fails trying to manipulate the protocol/name space catalogs."
	},
# endif /* #ifdef WSASYSCALLFAILURE */
	{	WSASYSNOTREADY,	"WSASYSNOTREADY",
		"Network subsystem is unavailable",
		"This error is returned by WSAStartup if the Windows Sockets \
implementation cannot function at this time because the underlying system \
it uses to provide network services is currently unavailable."
	},
	{	WSATRY_AGAIN,	"WSATRY_AGAIN",
		"Nonauthoritative host not found",
		"This is usually a temporary error during host name resolution and \
means that the local server did not receive a response from an authoritative \
server. A retry at some time later may be successful."
	},
	{	WSAVERNOTSUPPORTED,	"WSAVERNOTSUPPORTED",
		"Winsock.dll version out of range",
		"The current Windows Sockets implementation does not support the \
Windows Sockets specification version requested by the application. Check \
that no old Windows Sockets DLL files are being accessed."
	},
	{	WSAEDISCON,	"WSAEDISCON",
		"Graceful shutdown in progress",
		"Returned by WSARecv and WSARecvFrom to indicate that the remote \
party has initiated a graceful shutdown sequence."
	},
# ifdef WSA_OPERATION_ABORTED
	{	WSA_OPERATION_ABORTED,	"WSA_OPERATION_ABORTED",
		"Overlapped operation aborted",
		"An overlapped operation was canceled due to the closure of the \
socket, or the execution of the SIO_FLUSH command in WSAIoctl."
	}
# endif /* #ifdef WSA_OPERATION_ABORTED */
};

static const unsigned int NPSL_ErrorCount =
	(sizeof(NPSL_ErrorList) / sizeof(NPSL_ErrorList[0]));

#endif /* #ifdef _Windows */

/* *********************************************************************** */

/*	***********************************************************************	*/
void NPSL_GetErrorList(unsigned int *out_count,
	const NPSL_SocketErrorSpec **out_list)
{
#ifdef _Windows
	*out_count = NPSL_ErrorCount;
	*out_list  = NPSL_ErrorList;
#else
	GEN_GetErrorList(out_count, out_list);
#endif /* #ifdef _Windows */
}
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	NPSL_AppendErrorString

	SYNOPSIS		:	return_ptr = char *NPSL_AppendErrorString(errno_code,
							long_flag, max_text, error_text);

						char         *return_ptr;

						int           errno_code;

						int           long_flag;

						unsigned int  max_text;

						char         *error_text;

	DESCRIPTION	:	Appends text descriptive of the error corresponding to the
						specified networking sub-system error code to a string.

   PARAMETERS  :  Parameters to this function are as follows:

						(.) ``errno_code`` is the error code for which the function
						is to append a text description.

						(.) ``long_flag`` indicates to the function whether short
						or long error descriptions are to be used. If ``long_flag``
						is zero then only the short error description will be used.
						If ``long_flag`` is non-zero, then the long error
						description will be appended after the short error
						description.

						(-) Under SunOS 4.1.3 for example, if ``errno_code`` is
						equal to the manifest constant ''ENOENT'' and ``long_flag``
						is ''0'', the function will append the string:

						<|-|> Error code 2 (ENOENT --- No such file or directory)

						(-) If instead ``long_flag`` is non-zero, the function will
						append the string:

						<|-|> Error code 2 (ENOENT --- No such file or directory)
						This error occurs when a file name is specified and the
						file should exist but does not, or when one of the
						directories in a path name does not exist.

						(.) ``max_text`` is the maximum length to which the function
						is to permit the appended string ``error_text`` to grow.

						(.) ``error_text`` is a pointer to a string to which the
						function is to append text descriptive of the error.

   RETURNS     :	The ``error_text`` parameter is returned.

	NOTES			:	If ``errno_code`` is not a valid error code, the function
						will append the error description as follows:

						<|-|> Invalid error code (``error-code-number``)

	CAVEATS		:	

	SEE ALSO		:	NPSL_AppendLastErrorString
						NPSL_GetErrorString

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2014 Michael L. Brock

	OUTPUT INDEX:	NPSL_AppendErrorString
						Error Description Functions:NPSL_AppendErrorString
						NPSL:Functions:NPSL_AppendErrorString
						NPSL:Error Description Functions:NPSL_AppendErrorString
						Network Programming Support Functions:See NPSL

	PUBLISH XREF:	NPSL_AppendErrorString

	PUBLISH NAME:	NPSL_AppendErrorString

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
char *NPSL_AppendErrorString(int errno_code, int long_flag,
	unsigned int max_text, char *error_text)
{
	unsigned int error_length;

	if ((error_length = COMPAT_CAST_static(unsigned int, strlen(error_text))) <
		max_text)
		NPSL_GetErrorString(errno_code, long_flag, max_text - error_length,
			error_text + error_length);

	return(error_text);
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	NPSL_AppendLastErrorString

	SYNOPSIS		:	return_ptr = NPSL_AppendLastErrorString(long_flag, max_text,
							error_text);

						char         *return_ptr;

						int           long_flag;

						unsigned int  max_text;

						char         *error_text;

	DESCRIPTION	:	Appends text descriptive of the error corresponding to the
						last networking sub-system error (as returned by the function
						''WSAGetLastError()'').

   PARAMETERS  :  Parameters to this function are as follows:

						(.) ``long_flag`` indicates to the function whether short
						or long error descriptions are to be used. If ``long_flag``
						is zero then only the short error description will be used.
						If ``long_flag`` is non-zero, then the long error
						description will be appended after the short error
						description.

						(-) Under SunOS 4.1.3 for example, if ``errno`` is equal to
						the manifest constant ''ENOENT'' and ``long_flag`` is ''0'',
						the function will append the string:

						<|-|> Error code 2 (ENOENT --- No such file or directory)

						(-) If instead ``long_flag`` is non-zero, the function will
						append the string:

						<|-|> Error code 2 (ENOENT --- No such file or directory)
						This error occurs when a file name is specified and the
						file should exist but does not, or when one of the
						directories in a path name does not exist.

						(.) ``max_text`` is the maximum length to which the function
						is to permit the appended string ``error_text`` to grow.

						(.) ``error_text`` is a pointer to a string to which the
						function is to append text descriptive of the error.

   RETURNS     :	The ``error_text`` parameter is returned.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_AppendErrorString
						NPSL_GetLastErrorString

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2014 Michael L. Brock

	OUTPUT INDEX:	NPSL_AppendLastErrorString
						Error Description Functions:NPSL_AppendLastErrorString
						NPSL:Functions:NPSL_AppendLastErrorString
						NPSL:Error Description Functions:NPSL_AppendLastErrorString
						Network Programming Support Functions:See NPSL

	PUBLISH XREF:	NPSL_AppendLastErrorString

	PUBLISH NAME:	NPSL_AppendLastErrorString

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
char *NPSL_AppendLastErrorString(int long_flag, unsigned int max_text,
	char *error_text)
{
#ifdef _Windows
	return(NPSL_AppendErrorString(WSAGetLastError(), long_flag, max_text,
		error_text));
#else
	return(GEN_AppendErrorString(errno, long_flag, max_text, error_text));
#endif /* #ifdef _Windows */
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	NPSL_GetErrorString

	SYNOPSIS		:	return_ptr = NPSL_GetErrorString(errno_code, long_flag,
							max_text, error_text);

						char         *return_ptr;

						int           errno_code;

						int           long_flag;

						unsigned int  max_text;

						char         *error_text;

	DESCRIPTION	:	Copies text descriptive of the error corresponding to the
						specified networking sub-system error code to a string.

   PARAMETERS  :  Parameters to this function are as follows:

						(.) ``errno_code`` is the error code for which the function
						is to copy a text description.

						(.) ``long_flag`` indicates to the function whether short
						or long error descriptions are to be used. If ``long_flag``
						is zero then only the short error description will be used.
						If ``long_flag`` is non-zero, then the long error
						description will be copied after the short error
						description.

						(-) Under SunOS 4.1.3 for example, if ``errno_code`` is
						equal to the manifest constant ''ENOENT'' and ``long_flag``
						is ''0'', the function will copy the string:

						<|-|> Error code 2 (ENOENT --- No such file or directory)

						(-) If instead ``long_flag`` is non-zero, the function will
						copy the string:

						<|-|> Error code 2 (ENOENT --- No such file or directory)
						This error occurs when a file name is specified and the
						file should exist but does not, or when one of the
						directories in a path name does not exist.

						(.) ``max_text`` is the maximum length to which the function
						is to permit the copied string ``error_text`` to grow.

						(.) ``error_text`` is a pointer to the string into which the
						function is to copy text descriptive of the error.

   RETURNS     :	The ``error_text`` parameter is returned.

	NOTES			:	If ``errno_code`` is not a valid error code, the function
						will copy the error description as follows:

						<|-|> Invalid error code (``error-code-number``)

	CAVEATS		:	

	SEE ALSO		:	NPSL_GetLastErrorString
						NPSL_AppendErrorString

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2014 Michael L. Brock

	OUTPUT INDEX:	NPSL_GetErrorString
						Error Description Functions:NPSL_GetErrorString
						NPSL:Functions:NPSL_GetErrorString
						NPSL:Error Description Functions:NPSL_GetErrorString
						Network Programming Support Functions:See NPSL

	PUBLISH XREF:	NPSL_GetErrorString

	PUBLISH NAME:	NPSL_GetErrorString

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
char *NPSL_GetErrorString(int errno_code, int long_flag, unsigned int max_text,
	char *error_text)
{
#ifdef _Windows
	const NPSL_SocketErrorSpec *errno_ptr;
	char                        buffer[4196];
	char                        number_buffer[IntToString_MaxLen + 1];

	if ((errno_ptr = NPSL_GetErrorPtr(errno_code)) != NULL) {
		strcat(strcat(strcat(strcat(strcat(strcat(strcat(strcat(strcpy(buffer,
			"Error code "),
			MLB::Utility::IntToString(number_buffer, errno_code)),
			" ("), errno_ptr->error_name), " --- "),
			errno_ptr->short_description), ") "),
			(long_flag && *errno_ptr->long_description) ? " " : ""),
			(long_flag && *errno_ptr->long_description) ?
			errno_ptr->long_description : "");
	}
	else
		MLB::NPSLI_Internal::oneblank(strcat(strcat(strcpy(buffer,
			"Invalid error code ("),
			MLB::Utility::IntToString(number_buffer, errno_code)), ")"));

	if ((!max_text) || (max_text >= strlen(buffer)))
		strcpy(error_text, MLB::NPSLI_Internal::oneblank(buffer));
	else
		MLB::Utility::nstrcpy(error_text, MLB::NPSLI_Internal::oneblank(buffer),
			max_text);

	return(error_text);
#else
	return(GEN_GetErrorString(errno_code, long_flag, max_text, error_text));
#endif /* #ifdef _Windows */
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	NPSL_GetLastErrorString

	SYNOPSIS		:	return_ptr = NPSL_GetLastErrorString(long_flag, max_text,
							error_text);

						char         *return_ptr;

						int           long_flag;

						unsigned int  max_text;

						char         *error_text;

	DESCRIPTION	:	Copies text descriptive of the error corresponding to the
						last networking sub-system error (as returned by the function
						''WSAGetLastError()'').

   PARAMETERS  :  Parameters to this function are as follows:

						(.) ``long_flag`` indicates to the function whether short
						or long error descriptions are to be used. If ``long_flag``
						is zero then only the short error description will be used.
						If ``long_flag`` is non-zero, then the long error
						description will be copied after the short error
						description.

						(-) Under SunOS 4.1.3 for example, if ``errno`` is equal to
						the manifest constant ''ENOENT'' and ``long_flag`` is ''0'',
						the function will copy the string:

						<|-|> Error code 2 (ENOENT --- No such file or directory)

						(-) If instead ``long_flag`` is non-zero, the function will
						copy the string:

						<|-|> Error code 2 (ENOENT --- No such file or directory)
						This error occurs when a file name is specified and the
						file should exist but does not, or when one of the
						directories in a path name does not exist.

						(.) ``max_text`` is the maximum length to which the function
						is to permit the copied string ``error_text`` to grow.

						(.) ``error_text`` is a pointer to the string into which the
						function is to copy text descriptive of the error.

   RETURNS     :	The ``error_text`` parameter is returned.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_GetErrorString
						NPSL_AppendLastErrorString

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2014 Michael L. Brock

	OUTPUT INDEX:	NPSL_GetLastErrorString
						Error Description Functions:NPSL_GetLastErrorString
						NPSL:Functions:NPSL_GetLastErrorString
						NPSL:Error Description Functions:NPSL_GetLastErrorString
						Network Programming Support Functions:See NPSL

	PUBLISH XREF:	NPSL_GetLastErrorString

	PUBLISH NAME:	NPSL_GetLastErrorString

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
char *NPSL_GetLastErrorString(int long_flag, unsigned int max_text,
	char *error_text)
{
#ifdef _Windows
	return(NPSL_GetErrorString(WSAGetLastError(), long_flag, max_text,
		error_text));
#else
	return(GEN_GetErrorString(errno, long_flag, max_text, error_text));
#endif /* #ifdef _Windows */
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	NPSL_GetErrorPtr

	SYNOPSIS		:	return_ptr = NPSL_GetErrorPtr(errno_code);

						const NPSL_SocketErrorSpec *return_ptr;

						int                         errno_code;

	DESCRIPTION	:	Gets a pointer the ''GEN_NPSL_ERROR_SPEC'' error description
						structure corresponding to the specified networking
						sub-system error code.

   PARAMETERS  :  Parameters to this function are as follows:

						(.) ``errno_code`` is the error code for which the function
						is to return a pointer to the corresponding error
						description structure.

   RETURNS     :  Returns from this function are as follow:

						(.) If the error code specified by the ``errno_code``
						parameter is valid, a pointer to the ''GEN_NPSL_ERROR_SPEC''
						error description structure corresponding to that error
						code is returned.

						(.) Otherwise, a ''NULL'' pointer is returned.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_GetLastErrorPtr

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2014 Michael L. Brock

	OUTPUT INDEX:	NPSL_GetErrorPtr
						Error Description Functions:NPSL_GetErrorPtr
						NPSL:Functions:NPSL_GetErrorPtr
						NPSL:Error Description Functions:NPSL_GetErrorPtr
						Network Programming Support Functions:See NPSL

	PUBLISH XREF:	NPSL_GetErrorPtr

	PUBLISH NAME:	NPSL_GetErrorPtr

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
const NPSL_SocketErrorSpec *NPSL_GetErrorPtr(int errno_code)
{
#ifdef _Windows
	unsigned int                count_1;
	const NPSL_SocketErrorSpec *errno_ptr = NULL;

	for (count_1 = 0; count_1 < NPSL_ErrorCount; count_1++) {
		if (errno_code == ((int) NPSL_ErrorList[count_1].error_code)) {
			errno_ptr = NPSL_ErrorList + count_1;
			break;
		}
	}

	return(errno_ptr);
#else
	return(GEN_GetErrorPtr(errno_code));
#endif /* #ifdef _Windows */
}
/* *********************************************************************** */

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* BOH

	NAME			:	NPSL_GetLastErrorPtr

	SYNOPSIS		:	return_ptr = NPSL_GetLastErrorPtr();

						const GEN_NPSL_ERROR_SPEC *return_ptr;

	DESCRIPTION	:	Gets a pointer the ''GEN_NPSL_ERROR_SPEC'' error description
						structure corresponding to the last networking sub-system
						error (as returned by the function ''WSAGetLastError()'').

   PARAMETERS  :  Void.

   RETURNS     :  Returns a pointer to the ''NPSL_SocketErrorSpec'' error
						description structure corresponding to the current value
						of the global variable ``errno``.

	NOTES			:	

	CAVEATS		:	

	SEE ALSO		:	NPSL_GetErrorPtr

	EXAMPLES		:	

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1993 - 2014 Michael L. Brock

	OUTPUT INDEX:	NPSL_GetLastErrorPtr
						Error Description Functions:NPSL_GetLastErrorPtr
						NPSL:Functions:NPSL_GetLastErrorPtr
						NPSL:Error Description Functions:NPSL_GetLastErrorPtr
						Network Programming Support Functions:See NPSL

	PUBLISH XREF:	NPSL_GetLastErrorPtr

	PUBLISH NAME:	NPSL_GetLastErrorPtr

	ENTRY CLASS	:	Error Description Functions

EOH */
/*	***********************************************************************	*/
#ifndef NARGS
const NPSL_SocketErrorSpec *NPSL_GetLastErrorPtr(void)
#else
const NPSL_SocketErrorSpec *NPSL_GetLastErrorPtr()
#endif /* #ifndef NARGS */
{
#ifdef _Windows
	return(NPSL_GetErrorPtr(WSAGetLastError()));
#else
	return(GEN_GetErrorPtr(errno));
#endif /* #ifdef _Windows */
}
/* *********************************************************************** */

#ifdef TEST_MAIN

#include <stdio.h>

#ifdef __MSDOS__
COMPAT_EXTERN_DATA_DECL unsigned int _stklen = 16384;
#endif /* __MSDOS__ */

COMPAT_FN_DECL(       int  main,
	(int argc, char **argv));
COMPAT_FN_DECL_STATIC(void DoUsage,
	(char *prog, char *exit_text, int exit_code));

int main(int argc, char **argv)
{
	unsigned int                count_1   = 0;
	unsigned int                count_2   = 0;
	int                         long_flag = 0;
	unsigned int                max_text  = 0;
	unsigned int                longest_1 = 0;
	unsigned int                longest_2 = 0;
	unsigned int                error_count;
	const NPSL_SocketErrorSpec *error_list;
	unsigned int                length;
	char                        error_text[4096];

	fprintf(stderr, "Test routine for 'NPSL_GetErrorString()'\n");
	fprintf(stderr, "---- ------- --- -----------------------\n\n");

	NPSL_GetErrorList(&error_count, &error_list);

	for (count_1 = 0; count_1 < error_count; count_1++) {
		length    = COMPAT_CAST_static(unsigned int,
			strlen(NPSL_GetErrorString(error_list[count_1].error_code,
			0, 0, error_text)));
		longest_1 = (longest_1 > length) ? longest_1 : length;
		length    = COMPAT_CAST_static(unsigned int,
			strlen(NPSL_GetErrorString(error_list[count_1].error_code,
			1, 0, error_text)));
		longest_2 = (longest_2 > length) ? longest_2 : length;
		count_1++;
	}

	printf("Longest Short Message: %u bytes\n", longest_1);
	printf("Longest Long Message : %u bytes\n", longest_2);

	for (count_1 = 1; count_1 < ((unsigned int) argc); count_1++) {
		if (!strnicmp("-LONG=", argv[count_1], 6)) {
			if ((!stricmp(argv[count_1] + 6, "ON")) ||
				(!stricmp(argv[count_1] + 6, "YES")) ||
				(!stricmp(argv[count_1] + 6, "TRUE")))
				long_flag = 1;
			else if ((!stricmp(argv[count_1] + 6, "OFF")) ||
				(!stricmp(argv[count_1] + 6, "NO")) ||
				(!stricmp(argv[count_1] + 6, "FALSE")))
				long_flag = 0;
			else
				DoUsage(argv[0], "Invalid '-LONG=' toggle", 22);
		}
		else if ((!strnicmp("-MAX=", argv[count_1], 5)) &&
			(strlen(argv[count_1]) > 5) && isdigit(argv[count_1][5]))
			max_text = atoi(argv[count_1] + 5);
		else if (!stricmp("-ALL", argv[count_1])) {
			printf("-----------------------------------------------------\n");
			for (count_2 = 0; count_2 < error_count; count_2++)
				printf("[%05u]:%s\n", count_2,
					NPSL_GetErrorString(error_list[count_2].error_code,
					long_flag, max_text, error_text));
			printf("-----------------------------------------------------\n");
		}
		else if ((!stricmp("-HELP", argv[count_1])) ||
			(!stricmp("-H", argv[count_1])))
			DoUsage(argv[0], "Help request noted.", 0);
		else
			printf("%d ---> %s\n", atoi(argv[count_1]),
				NPSL_GetErrorString(atoi(argv[count_1]), long_flag,
				max_text, error_text));
	}

	return(0);
}

static void DoUsage(char *prog, char *exit_text, int exit_code)
{
	prog = (strchr(prog, '\\')) ? strchr(prog, '\\') + 1 : prog;

	if (*exit_text)
		fprintf(stderr, "%s\n\n", exit_text);

	fprintf(stderr, "USAGE: %s %s %s %s [%s . . .]\n", prog,
		"[-LONG={ON|OFF|YES|NO|TRUE|FALSE}]", "[-MAX=<max-length-of-string>]",
		"<errno-number>", "<errno-number>");

	exit(exit_code);
}

#endif /* #ifdef TEST_MAIN */

