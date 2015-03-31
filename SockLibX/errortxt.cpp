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

		Copyright Michael L. Brock 1993 - 2015.
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

static const NPSL_SocketErrorSpec NPSL_ErrorList[] = {
#ifdef _Windows
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
	// ********** End of '#ifdef _Windows' **********
#elif __SVR4
const ErrnoSpec_Internal ErrnoList[] = {
	{  0,						"00000",
		"Error 0 / No error detected",
		"No error encountered."
	},
	{
		EPERM,	"EPERM",
		"Not super-user",
		"Typically this error indicates an attempt to modify a file in some \
way forbidden except to its owner or the super-user. It is also returned for \
attempts by ordinary users to do things allowed only to the super-user."
	},
	{
		ENOENT,	"ENOENT",
		"No such file or directory",
		"A file name is specified and the file should exist but doesn't, or \
one of the directories in a path name does not exist."
	},
	{
		ESRCH,	"ESRCH",
		"No such process, LWP, or thread",
		"No process can be found in the system that corresponds to the \
specified PID, LWPID_t, or thread_t."
	},
	{
		EINTR,	"EINTR",
		"Interrupted system call",
		"An asynchronous signal (such as interrupt or quit), which the user \
has elected to catch, occurred during a system service routine. If execution \
is resumed after processing the signal, it will appear as if the interrupted \
routine call returned this error condition."
	},
	{
		EIO,	"EIO",
		"I/O error",
		"Some physical I/O error has occurred. This error may in some cases \
occur on a call following the one to which it actually applies."
	},
	{
		ENXIO,	"ENXIO",
		"No such device or address",
		"I/O on a special file refers to a subdevice which does not exist, or \
exists beyond the limit of the device. It may also occur when, for example, \
a tape drive is not on-line or no disk pack is loaded on a drive."
	},
	{
		E2BIG,	"E2BIG",
		"Arg list too long",
		"An argument list longer than ARG_MAX bytes is presented to a member \
of the exec family of routines. The argument list limit is the sum of the \
size of the argument list plus the size of the environment's exported shell \
variables."
	},
	{
		ENOEXEC,	"ENOEXEC",
		"Exec format error",
		"A request is made to execute a file which, although it has the \
appropriate permissions, does not start with a valid format (see a.out(4))."
	},
	{
		EBADF,	"EBADF",
		"Bad file number",
		"Either a file descriptor refers to no open file, or a read \
(respectively, write) request is made to a file that is open only for \
writing (respectively, reading)."
	},
	{
		ECHILD,	"ECHILD",
		"No child processes",
		"A wait routine was executed by a process that had no existing or \
unwaited-for child processes."
	},
	{
		EAGAIN,	"EAGAIN",
		"No more processes, or no more LWPs",
		"For example, the fork routine failed because the system's process \
table is full or the user is not allowed to create any more processes, or a \
system call failed because of insufficient memory or swap space."
	},
	{
		ENOMEM,	"ENOMEM",
		"Not enough space",
		"During execution of an exec, brk, or sbrk routine, a program asks for \
more space than the system is able to supply. This is not a temporary \
condition; the maximum size is a system parameter. On some architectures, \
the error may also occur if the arrangement of text, data, and stack \
segments requires too many segmentation registers, or if there is not enough \
swap space during the fork routine. If this error occurs on a resource \
associated with Remote File Sharing (RFS), it indicates a memory depletion \
which may be temporary, dependent on system activity at the time the call \
was invoked."
	},
	{
		EACCES,	"EACCES",
		"Permission denied",
		"An attempt was made to access a file in a way forbidden by the \
protection system."
	},
	{
		EFAULT,	"EFAULT",
		"Bad address",
		"The system encountered a hardware fault in attempting to use an \
argument of a routine. For example, errno potentially may be set to EFAULT \
any time a routine that takes a pointer argument is passed an invalid \
address, if the system can detect the condition. Because systems will differ \
in their ability to reliably detect a bad address, on some implementations \
passing a bad address to a routine will result in undefined behavior."
	},
	{
		ENOTBLK,	"ENOTBLK",
		"Block device required",
		"A non-block device or file was mentioned where a block device was \
required (for example, in a call to the mount routine)."
	},
	{
		EBUSY,	"EBUSY",
		"Device busy",
		"An attempt was made to mount a device that was already mounted or an \
attempt was made to unmount a device on which there is an active file \
(open file, current directory, mounted-on file, active text segment). It \
will also occur if an attempt is made to enable accounting when it is \
already enabled. The device or resource is currently unavailable. EBUSY is \
also used by mutexs, semaphores, condition variables, and r/w locks, to \
indicate that a lock is held. And, EBUSY is also used by the processor \
control function P_ONLINE."
	},
	{
		EEXIST,	"EEXIST",
		"File exists",
		"An existing file was mentioned in an inappropriate context (for \
example, call to the link routine)."
	},
	{
		EXDEV,	"EXDEV",
		"Cross-device link",
		"A hard link to a file on another device was attempted."
	},
	{
		ENODEV,	"ENODEV",
		"No such device",
		"An attempt was made to apply an inappropriate operation to a device \
(for example, read a write-only device)."
	},
	{
		ENOTDIR,	"ENOTDIR",
		"Not a directory",
		"A non-directory was specified where a directory is required (for \
example, in a path prefix or as an argument to the chdir routine)."
	},
	{
		EISDIR,	"EISDIR",
		"Is a directory",
		"An attempt was made to write on a directory."
	},
	{
		EINVAL,	"EINVAL",
		"Invalid argument",
		"An invalid argument was specified (for example, unmounting a \
non-mounted device), mentioning an undefined signal in a call to the signal \
or kill routine."
	},
	{
		ENFILE,	"ENFILE",
		"File table overflow",
		"The system file table is full (that is, SYS_OPEN files are open, and \
temporarily no more files can be opened)."
	},
	{
		EMFILE,	"EMFILE",
		"Too many open files",
		"No process may have more than OPEN_MAX file descriptors open at a \
time."
	},
	{
		ENOTTY,	"ENOTTY",
		"Inappropriate ioctl for device",
		"A call was made to the ioctl routine specifying a file that is not a \
special character device."
	},
	{
		ETXTBSY,	"ETXTBSY",
		"Text file busy (obsolete)",
		"An attempt was made to execute a pure-procedure program that is \
currently open for writing. Also an attempt to open for writing or to remove \
a pure-procedure program that is being executed. (This message is obsolete.)"
	},
	{
		EFBIG,	"EFBIG",
		"File too large",
		"The size of the file exceeded the limit specified by resource \
RLIMIT_FSIZE; or, the file size exceeds the maximum supported by the file \
system."
	},
	{
		ENOSPC,	"ENOSPC",
		"No space left on device",
		"While writing an ordinary file or creating a directory entry, there \
is no free space left on the device. In the fcntl routine, the setting or \
removing of record locks on a file cannot be accomplished because there are \
no more record entries left on the system."
	},
	{
		ESPIPE,	"ESPIPE",
		"Illegal seek",
		"A call to the lseek routine was issued to a pipe."
	},
	{
		EROFS,	"EROFS",
		"Read-only file system",
		"An attempt to modify a file or directory was made on a device mounted \
read-only."
	},
	{
		EMLINK,	"EMLINK",
		"Too many links",
		"An attempt to make more than the maximum number of links, LINK_MAX, \
to a file."
	},
	{
		EPIPE,	"EPIPE",
		"Broken pipe",
		"A write on a pipe for which there is no process to read the data. \
This condition normally generates a signal; the error is returned if the \
signal is ignored."
	},
	{
		EDOM,	"EDOM",
		"Math argument out of domain of func",
		"The argument of a function in the math package (3M) is out of the \
domain of the function."
	},
	{
		ERANGE,	"ERANGE",
		"Math result not representable",
		"The value of a function in the math package (3M) is not representable \
within machine precision."
	},
	{
		ENOMSG,	"ENOMSG",
		"No message of desired type",
		"An attempt was made to receive a message of a type that does not \
exist on the specified message queue (see msgop(2))."
	},
	{
		EIDRM,	"EIDRM",
		"Identifier removed",
		"This error is returned to processes that resume execution due to the \
removal of an identifier from the file system's name space (see msgctl(2), \
semctl(2), and shmctl(2))."
	},
	{
		ECHRNG,	"ECHRNG",
		"Channel number out of range",
		"Channel number out of range."
	},
	{
		EL2NSYNC,	"EL2NSYNC",
		"Level 2 not synchronized",
		"Level 2 not synchronized."
	},
	{
		EL3HLT,	"EL3HLT",
		"Level 3 halted",
		"Level 3 halted."
	},
	{
		EL3RST,	"EL3RST",
		"Level 3 reset",
		"Level 3 reset."
	},
	{
		ELNRNG,	"ELNRNG",
		"Link number out of range",
		"Link number out of range."
	},
	{
		EUNATCH,	"EUNATCH",
		"Protocol driver not attached",
		"Protocol driver not attached."
	},
	{
		ENOCSI,	"ENOCSI",
		"No CSI structure available",
		"No CSI structure available."
	},
	{
		EL2HLT,	"EL2HLT",
		"Level 2 halted",
		"Level 2 halted."
	},
	{
		EDEADLK,	"EDEADLK",
		"Deadlock condition",
		"A deadlock situation was detected and avoided. This error pertains to \
file and record locking, and also applies to mutexs, semaphores, condition \
variables, and r/w locks."
	},
	{
		ENOLCK,	"ENOLCK",
		"No record locks available",
		"There are no more locks available. The system lock table is full (see \
fcntl(2))."
	},
	{
		ECANCELED,	"ECANCELED",
		"Operation canceled",
		"The associated asynchronous operation was canceled before completion."
	},
	{
		ENOTSUP,	"ENOTSUP",
		"Not supported",
		"This version of the system does not support this feature. Future \
versions of the system may provide support."
	},
	{
		49,	"Error 49",
		"Reserved",
		"Reserved."
	},
	{
		58,	"Error 58",
		"Reserved",
		"Reserved."
	},
	{
		59,	"Error 59",
		"Reserved",
		"Reserved."
	},
	{
		ENOSTR,	"ENOSTR",
		"Device not a stream",
		"A putmsg or getmsg system call was attempted on a file descriptor \
that is not a STREAMS device."
	},
	{
		ENODATA,	"ENODATA",
		"No data available",
		"No data available."
	},
	{
		ETIME,	"ETIME",
		"Timer expired",
		"The timer set for a STREAMS ioctl call has expired. The cause of this \
error is device specific and could indicate either a hardware or software \
failure, or perhaps a timeout value that is too short for the specific \
operation. The status of the ioctl operation is indeterminate. This is also \
returned in the case of _lwp_cond_timedwait() or cond_timedwait()."
	},
	{
		ENOSR,	"ENOSR",
		"Out of stream resources",
		"During a STREAMS open, either no STREAMS queues or no STREAMS head \
data structures were available. This is a temporary condition; one may \
recover from it if other processes release resources."
	},
	{
		ENONET,	"ENONET",
		"Machine is not on the network",
		"This error is Remote File Sharing (RFS) specific. It occurs when \
users try to advertise, unadvertise, mount, or unmount remote resources \
while the machine has not done the proper startup to connect to the network."
	},
	{
		ENOPKG,	"ENOPKG",
		"Package not installed",
		"This error occurs when users attempt to use a system call from a \
package which has not been installed."
	},
	{
		EREMOTE,	"EREMOTE",
		"Object is remote",
		"This error is RFS specific. It occurs when users try to advertise a \
resource which is not on the local machine, or try to mount/unmount a device \
(or pathname) that is on a remote machine."
	},
	{
		ENOLINK,	"ENOLINK",
		"Link has been severed",
		"This error is RFS specific. It occurs when the link (virtual circuit) \
connecting to a remote machine is gone."
	},
	{
		EADV,	"EADV",
		"Advertise error",
		"This error is RFS specific. It occurs when users try to advertise a \
resource which has been advertised already, or try to stop RFS while there \
are resources still advertised, or try to force unmount a resource when it \
is still advertised."
	},
	{
		ESRMNT,	"ESRMNT",
		"Srmount error",
		"This error is RFS specific. It occurs when an attempt is made to stop \
RFS while resources are still mounted by remote machines, or when a resource \
is readvertised with a client list that does not include a remote machine \
that currently has the resource mounted."
	},
	{
		ECOMM,	"ECOMM",
		"Communication error on send",
		"This error is RFS specific. It occurs when the current process is \
waiting for a message from a remote machine, and the virtual circuit fails."
	},
	{
		EPROTO,	"EPROTO",
		"Protocol error",
		"Some protocol error occurred. This error is device specific, but is \
generally not related to a hardware failure."
	},
	{
		EMULTIHOP,	"EMULTIHOP",
		"Multihop attempted",
		"This error is RFS specific. It occurs when users try to access remote \
resources which are not directly accessible."
	},
# ifdef EDOTDOT
	//	***** Unable to locate manifest constant 'EDOTDOT' Solaris 2.4 *****
	{
		EDOTDOT,	"EDOTDOT",
		"Error 76",
		"This error is RFS specific. A way for the server to tell the client \
that a process has transferred back from mount point."
	},
# endif // # ifdef EDOTDOT
	{
		EBADMSG,	"EBADMSG",
		"Not a data message",
		"During a read, getmsg, or ioctl I_RECVFD system call to a STREAMS \
device, something has come to the head of the queue that can't be processed. \
That something depends on the system call --- read: control information or \
passed file descriptor --- getmsg: passed file descriptor --- ioctl: control \
or data information."
	},
	{
		ENAMETOOLONG,	"ENAMETOOLONG",
		"File name too long",
		"The length of the path argument exceeds PATH_MAX, or the length of a \
path component exceeds NAME_MAX while _POSIX_NO_TRUNC is in effect; see \
limits(4)."
	},
	{
		EOVERFLOW,	"EOVERFLOW",
		"Value too large for defined data type",
		"Value too large for defined data type."
	},
	{
		ENOTUNIQ,	"ENOTUNIQ",
		"Name not unique on network",
		"Given log name not unique."
	},
	{
		EBADFD,	"EBADFD",
		"File descriptor in bad state",
		"Either a file descriptor refers to no open file or a read request was \
made to a file that is open only for writing."
	},
	{
		EREMCHG,	"EREMCHG",
		"Remote address changed",
		"Remote address changed."
	},
	{
		ELIBACC,	"ELIBACC",
		"Cannot access a needed shared library",
		"Trying to exec an a.out that requires a static shared library and the \
static shared library doesn't exist or the user doesn't have permission to \
use it."
	},
	{
		ELIBBAD,	"ELIBBAD",
		"Accessing a corrupted shared library",
		"Trying to exec an a.out that requires a static shared library (to be \
linked in) and exec could not load the static shared library. The static \
shared library is probably corrupted."
	},
	{
		ELIBSCN,	"ELIBSCN",
		"lib section in a.out corrupted",
		"Trying to exec an a.out that requires a static shared library (to be \
linked in) and there was erroneous data in the .lib section of the a.out. \
The .lib section tells exec what static shared libraries are needed. The \
a.out is probably corrupted."
	},
	{
		ELIBMAX,	"ELIBMAX",
		"Attempting to link in more shared libraries than system limit",
		"Trying to exec an a.out that requires more static shared libraries \
than is allowed on the current configuration of the system. See NFS \
Administration Guide."
	},
	{
		ELIBEXEC,	"ELIBEXEC",
		"Cannot exec a shared library directly",
		"Attempting to exec a shared library directly."
	},
	{
		EILSEQ,	"EILSEQ",
		"Wide character encoding error",
		"Illegal byte sequence. Cannot handle multiple characters as a single \
character."
	},
	{
		ENOSYS,	"ENOSYS",
		"Operation not applicable",
		"Operation not applicable."
	},
	{
		ELOOP,	"ELOOP",
		"Too many levels of symbolic links",
		"Number of symbolic links encountered during path name traversal \
exceeds MAXSYMLINKS"
	},
# ifdef ESTART
	//	***** Unable to locate manifest constant 'ESTART' Solaris 2.4 ******
	{
		ESTART,	"ESTART",
		"Restartable system call",
		"Interrupted system call should be restarted."
	},
# endif // # ifdef ESTART
	{
		ESTRPIPE,	"ESTRPIPE",
		"If pipe/FIFO, don't sleep in stream head",
		"Streams pipe error (not externally visible)."
	},
	{
		ENOTEMPTY,	"ENOTEMPTY",
		"Directory not empty",
		"Directory not empty."
	},
	{
		EUSERS,	"EUSERS",
		"Too many users",
		"Too many users."
	},
	{
		ENOTSOCK,	"ENOTSOCK",
		"Socket operation on non-socket",
		"Socket operation on non-socket."
	},
	{
		EDESTADDRREQ,	"EDESTADDRREQ",
		"Destination address required",
		"A required address was omitted from an operation on a transport \
endpoint. Destination address required."
	},
	{
		EMSGSIZE,	"EMSGSIZE",
		"Message too long",
		"A message sent on a transport provider was larger than the internal \
message buffer or some other network limit."
	},
	{
		EPROTOTYPE,	"EPROTOTYPE",
		"Protocol wrong type for socket",
		"A protocol was specified that does not support the semantics of the \
socket type requested."
	},
	{
		ENOPROTOOPT,	"ENOPROTOOPT",
		"Protocol not available",
		"A bad option or level was specified when getting or setting options \
for a protocol."
	},
	{
		EPROTONOSUPPORT,	"EPROTONOSUPPORT",
		"Protocol not supported",
		"The protocol has not been configured into the system or no \
implementation for it exists."
	},
	{
		ESOCKTNOSUPPORT,	"ESOCKTNOSUPPORT",
		"Socket type not supported",
		"The support for the socket type has not been configured into the \
system or no implementation for it exists."
	},
	{
		EOPNOTSUPP,	"EOPNOTSUPP",
		"Operation not supported on transport endpoint",
		"For example, trying to accept a connection on a datagram transport \
endpoint."
	},
	{
		EPFNOSUPPORT,	"EPFNOSUPPORT",
		"Protocol family not supported",
		"The protocol family has not been configured into the system or no \
implementation for it exists. Used for the Internet protocols."
	},
	{
		EAFNOSUPPORT,	"EAFNOSUPPORT",
		"Address family not supported by protocol family",
		"An address incompatible with the requested protocol was used."
	},
	{
		EADDRINUSE,	"EADDRINUSE",
		"Address already in use",
		"User attempted to use an address already in use, and the protocol \
does not allow this."
	},
	{
		EADDRNOTAVAIL,	"EADDRNOTAVAIL",
		"Cannot assign requested address",
		"Results from an attempt to create a transport endpoint with an \
address not on the current machine."
	},
	{
		ENETDOWN,	"ENETDOWN",
		"Network is down",
		"Operation encountered a dead network."
	},
	{
		ENETUNREACH,	"ENETUNREACH",
		"Network is unreachable",
		"Operation was attempted to an unreachable network."
	},
	{
		ENETRESET,	"ENETRESET",
		"Network dropped connection because of reset",
		"The host you were connected to crashed and rebooted."
	},
	{
		ECONNABORTED,	"ECONNABORTED",
		"Software caused connection abort",
		"A connection abort was caused internal to your host machine."
	},
	{
		ECONNRESET,	"ECONNRESET",
		"Connection reset by peer",
		"A connection was forcibly closed by a peer. This normally results \
from a loss of the connection on the remote host due to a timeout or a \
reboot."
	},
	{
		ENOBUFS,	"ENOBUFS",
		"No buffer space available",
		"An operation on a transport endpoint or pipe was not performed \
because the system lacked sufficient buffer space or because a queue was \
full."
	},
	{
		EISCONN,	"EISCONN",
		"Transport endpoint is already connected",
		"A connect request was made on an already connected transport \
endpoint; or, a sendto or sendmsg request on a connected transport endpoint \
specified a destination when already connected."
	},
	{
		ENOTCONN,	"ENOTCONN",
		"Transport endpoint is not connected",
		"A request to send or receive data was disallowed because the \
transport endpoint is not connected and (when sending a datagram) no address \
was supplied."
	},
	{
		ESHUTDOWN,	"ESHUTDOWN",
		"Cannot send after transport endpoint shutdown",
		"A request to send data was disallowed because the transport endpoint \
has already been shut down."
	},
	{
		ETOOMANYREFS,	"ETOOMANYREFS",
		"Too many references: cannot splice",
		"Too many references: cannot splice."
	},
	{
		ETIMEDOUT,	"ETIMEDOUT",
		"Connection timed out",
		"A connect or send request failed because the connected party did not \
properly respond after a period of time. (The timeout period is dependent on \
the communication protocol.)"
	},
	{
		ECONNREFUSED,	"ECONNREFUSED",
		"Connection refused",
		"No connection could be made because the target machine actively \
refused it. This usually results from trying to connect to a service that is \
inactive on the remote host."
	},
	{
		EHOSTDOWN,	"EHOSTDOWN",
		"Host is down",
		"A transport provider operation failed because the destination host \
was down."
	},
	{
		EHOSTUNREACH,	"EHOSTUNREACH",
		"No route to host",
		"A transport provider operation was attempted to an unreachable host."
	},
	{
		EALREADY,	"EALREADY",
		"Operation already in progress",
		"An operation was attempted on a non-blocking object that already had \
an operation in progress."
	},
	{
		EINPROGRESS,	"EINPROGRESS",
		"Operation now in progress",
		"An operation that takes a long time to complete (such as a connect) \
was attempted on a non-blocking object."
	},
	{
		ESTALE,	"ESTALE",
		"Stale NFS file handle",
		"Stale NFS file handle."
	}
};
	// ********** End of '#elif __SVR4' **********
#else
	{  0,						"00000",
		"Error 0 / No error detected",
		"No error encountered."
	},
	{  EPERM,				"EPERM",
		"Not owner",
		"Typically this error indicates an attempt to modify a file in some \
way forbidden except to its owner or super-user. It is also returned for \
attempts by ordinary users to do things allowed only to the super-user."
	},
	{  ENOENT,				"ENOENT",
		"No such file or directory",
		"This error occurs when a file name is specified and the file should \
exist but does not, or when one of the directories in a path name does not \
exist."
	},
	{  ESRCH,				"ESRCH",
		"No such process",
		"The process or process group whose number was given does not exist, \
or any such process is already dead."
	},
	{  EINTR,				"EINTR",
		"Interrupted system call",
		"An asynchronous signal (such as interrupt or quit) that the process \
has elected to catch occurred during a system call. If execution is resumed \
after processing the signal, and the system call is not restarted, it will \
appear as if the interrupted system call returned this error condition."
	},
	{  EIO,					"EIO",
		"I/O error",
		"Some physical I/O error occurred. This error may in some cases occur \
on a call following the one to which it actually applies."
	},
	{  ENXIO,				"ENXIO",
		"No such device or address",
		"I/O on a special file refers to a subdevice that does not exist, or \
beyond the limits of the device. It may also occur when, for example, a tape \
drive is not on-line or no disk pack is loaded on a drive."
	},
	{  E2BIG,				"E2BIG",
		"Arg list too long",
		"An argument list longer than the maximum number of bytes was presented \
to execve() or a routine that called execve()."
	},
	{  ENOEXEC,				"ENOEXEC",
		"Exec format error",
		"A request is made to execute a file which, although it has the \
appropriate permissions, does not start with a valid magic number \
(see 'man 5 a.out')."
	},
	{  EBADF,				"EBADF",
		"Bad file number",
		"Either a file descriptor refers to no open file, or a read \
(respectively, write) request is made to a file that is open only for \
writing (respectively, reading)."
	},
	{	ECHILD,				"ECHILD",
		"No children",
		"A wait() was executed by a process that had no existing or \
unwaited-for child processes."
	},
	{	EAGAIN,				"EAGAIN",
		"No more processes",
		"A fork() failed because the system's process table is full or the \
user is not allowed to create any more processes, or a system call failed \
because of insufficient resources."
	},
	{	ENOMEM,				"ENOMEM",
		"Not enough memory",
		"During an execve(), sbrk(), or brk(), a program asks for more address \
space or swap space than the system is able to supply, or a process size \
limit would be exceeded. A lack of swap space is normally a temporary \
condition; however, a lack of address space is not a temporary condition. \
The maximum size of the text, data, and stack segments is a system \
parameter.  Soft limits may be increased to their corresponding hard limits."
	},
	{	EACCES,				"EACCES",
		"Permission denied",
		"An attempt was made to access a file in a way forbidden by the \
protection system."
	},
	{	EFAULT,				"EFAULT",
		"Bad address",
		"The system encountered a hardware fault in attempting to access the \
arguments of a system call."
	},
	{	ENOTBLK,				"ENOTBLK",
		"Block device required",
		"A file that is not a block device was mentioned where a block device \
was required, for example, in mount()."
	},
	{	EBUSY,				"EBUSY",
		"Device busy",
		"An attempt was made to mount a file system that was already mounted \
or an attempt was made to dismount a file system on which there is an \
active file (open file, mapped file, current directory, or mounted-on \
directory)."
	},
	{	EEXIST,				"EEXIST",
		"File exists",
		"An existing file was mentioned in an inappropriate context, for \
example, link()."
	},
	{	EXDEV,				"EXDEV",
		"Cross-device link",
		"A hard link to a file on another file system was attempted."
	},
	{	ENODEV,				"ENODEV",
		"No such device",
		"An attempt was made to apply an inappropriate system call to a device \
(for example, an attempt to read a write-only device) or an attempt was made \
to use a device not configured by the system."
	},
	{	ENOTDIR,				"ENOTDIR",
		"Not a directory",
		"A non-directory was specified where a directory is required, for \
example, in a path prefix or as an argument to chdir()."
	},
	{	EISDIR,				"EISDIR",
		"Is a directory",
		"An attempt was made to write on a directory."
	},
	{	EINVAL,				"EINVAL",
		"Invalid argument",
		"A system call was made with an invalid argument; for example, \
dismounting a non-mounted file system, mentioning an unknown signal in \
sigvec() or kill(), reading or writing a file for which lseek() has \
generated a negative pointer, or some other argument inappropriate for the \
call. Also set by math functions, see 'man 3 intro'."
	},
	{	ENFILE,				"ENFILE",
		"File table overflow",
		"The system's table of open files is full, and temporarily no more \
open() calls can be accepted."
	},
	{	EMFILE,				"EMFILE",
		"Too many open files",
		"A process tried to have more open files than the system allows a \
process to have. The customary configuration limit is 64 per process."
	},
	{	ENOTTY,				"ENOTTY",
		"Inappropriate ioctl for device",
		"The code used in an ioctl() call is not supported by the object that \
the file descriptor in the call refers to."
	},
	{	ETXTBSY,				"ETXTBSY",
		"Text file busy",
		"An attempt was made to execute a pure-procedure program that is \
currently open for writing, or an attempt was made to open for writing a \
pure-procedure program that is being executed."
	},
	{	EFBIG,				"EFBIG",
		"File too large",
		"The size of a file exceeded the maximum file size (1,082,201,088 \
bytes)."
	},
	{	ENOSPC,				"ENOSPC",
		"No space left on device",
		"A write() to an ordinary file, the creation of a directory or \
symbolic link, or the creation of a directory entry failed because no more \
disk blocks are available on the file system, or the allocation of an inode \
for a newly created file failed because no more inodes are available on the \
file system."
	},
	{	ESPIPE,				"ESPIPE",
		"Illegal seek",
		"An lseek() was issued to a socket or pipe. This error may also be \
issued for other non-seekable devices."
	},
	{	EROFS,				"EROFS",
		"Read-only file system",
		"An attempt to modify a file or directory was made on a file system \
mounted read-only."
	},
	{	EMLINK,				"EMLINK",
		"Too many links",
		"An attempt was made to make more than 32767 hard links to a file."
	},
	{	EPIPE,				"EPIPE",
		"Broken pipe",
		"An attempt was made to write on a pipe or socket for which there is \
no process to read the data. This condition normally generates a signal; the \
error is returned if the signal is caught or ignored."
	},
	{	EDOM,					"EDOM",
		"Argument too large",
		"The argument of a function in the math library (as described in \
section 3M) is out of the domain of the function."
	},
	{	ERANGE,				"ERANGE",
		"Result too large",
		"The value of a function in the math library (as described in section \
3M) is unrepresentable within machine precision."
	},
	{	EWOULDBLOCK,		"EWOULDBLOCK",
		"Operation would block",
		"An operation that would cause a process to block was attempted on an \
object in non-blocking mode (see ioctl())."
	},
	{	EINPROGRESS,		"EINPROGRESS",
		"Operation now in progress",
		"An operation that takes a long time to complete (such as a connect()) \
was attempted on a non-blocking object (see ioctl())."
	},
	{	EALREADY,			"EALREADY",
		"Operation already in progress",
		"An operation was attempted on a non-blocking object that already had \
an operation in progress."
	},
	{	ENOTSOCK,			"ENOTSOCK",
		"Socket operation on non-socket",
		"Self-explanatory."
	},
	{	EDESTADDRREQ,		"EDESTADDRREQ",
		"Destination address required",
		"A required address was omitted from an operation on a socket."
	},
	{	EMSGSIZE,			"EMSGSIZE",
		"Message too long",
		"A message sent on a socket was larger than the internal message \
buffer."
	},
	{	EPROTOTYPE,			"EPROTOTYPE",
		"Protocol wrong type for socket",
		"A protocol was specified that does not support the semantics of the \
socket type requested. For example, you cannot use the ARPA Internet UDP \
protocol with type SOCK_STREAM."
	},
	{	ENOPROTOOPT,		"ENOPROTOOPT",
		"Option not supported by protocol",
		"A bad option was specified in a setsockopt() or getsockopt() call."
	},
	{	EPROTONOSUPPORT,	"EPROTONOSUPPORT",
		"Protocol not supported",
		"The protocol has not been configured into the system or no \
implementation for it exists."
	},
	{	ESOCKTNOSUPPORT,	"ESOCKTNOSUPPORT",
		"Socket type not supported",
		"The support for the socket type has not been configured into the \
system or no implementation for it exists."
	},
	{	EOPNOTSUPP,			"EOPNOTSUPP",
		"Operation not supported on socket",
		"For example, trying to accept a connection on a datagram socket."
	},
	{	EPFNOSUPPORT,		"EPFNOSUPPORT",
		"Protocol family not supported",
		"The protocol family has not been configured into the system or no \
implementation for it exists."
	},
	{	EAFNOSUPPORT,		"EAFNOSUPPORT",
		"Address family not supported by protocol family",
		"An address incompatible with the requested protocol was used. For \
example, you should not necessarily expect to be able to use PUP Internet \
addresses with ARPA Internet protocols."
	},
	{	EADDRINUSE,			"EADDRINUSE",
		"Address already in use",
		"Only one usage of each address is normally permitted."
	},
	{	EADDRNOTAVAIL,		"EADDRNOTAVAIL",
		"Can't assign requested address",
		"Normally results from an attempt to create a socket with an address \
not on this machine."
	},
	{	ENETDOWN,			"ENETDOWN",
		"Network is down",
		"A socket operation encountered a dead network."
	},
	{	ENETUNREACH,		"ENETUNREACH",
		"Network is unreachable",
		"A socket operation was attempted to an unreachable network."
	},
	{	ENETRESET,			"ENETRESET",
		"Network dropped connection on reset",
		"The host you were connected to crashed and rebooted."
	},
	{	ECONNABORTED,		"ECONNABORTED",
		"Software caused connection abort",
		"A connection abort was caused internal to your host machine."
	},
	{	ECONNRESET,			"ECONNRESET",
		"Connection reset by peer",
		"A connection was forcibly closed by a peer. This normally results \
from the peer executing a shutdown() call."
	},
	{	ENOBUFS,				"ENOBUFS",
		"No buffer space available",
		"An operation on a socket or pipe was not performed because the system \
lacked sufficient buffer space."
	},
	{	EISCONN,				"EISCONN",
		"Socket is already connected",
		"A connect() request was made on an already connected socket; or, a \
sendto() or sendmsg() request on a connected socket specified a destination \
other than the connected party."
	},
	{	ENOTCONN,			"ENOTCONN",
		"Socket is not connected",
		"An request to send or receive data was disallowed because the socket \
is not connected."
	},
	{	ESHUTDOWN,			"ESHUTDOWN",
		"Can't send after socket shutdown",
		"A request to send data was disallowed because the socket had already \
been shut down with a previous shutdown call."
	},
	{	ETOOMANYREFS,		"ETOOMANYREFS",
		"Too many references: can't splice",
		""
	},
	{	ETIMEDOUT,			"ETIMEDOUT",
		"Connection timed out",
		"A connect request or an NFS request failed because the party to which \
the request was made did not properly respond after a period of time. The \
timeout period is dependent on the communication protocol"
	},
	{	ECONNREFUSED,		"ECONNNREFUSED",
		"Connection refused",
		"No connection could be made because the target machine actively \
refused it. This usually results from trying to connect to a service that \
is inactive on the foreign host."
	},
	{	ELOOP,				"ELOOP",
		"Too many levels of symbolic links",
		"A path name lookup involved more than 20 symbolic links."
	},
	{	ENAMETOOLONG,		"ENAMETOOLONG",
		"File name too long",
		"A component of a path name exceeded 255 characters, or an entire path \
name exceeded 1024 characters."
	},
	{	EHOSTDOWN,			"EHOSTDOWN",
		"Host is down",
		"A socket operation failed because the destination host was down."
	},
	{	EHOSTUNREACH,		"EHOSTUNREACH",
		"Host is unreachable",
		"A socket operation was attempted to an unreachable host."
	},
	{	ENOTEMPTY,			"ENOTEMPTY",
		"Directory not empty",
		"An attempt was made to remove a directory with entries other than \
'&.' and '&.|.' by performing a rmdir() system call or a rename() system \
call with that directory specified as the target directory."
	},
#ifdef EPROCLIM
	{	EPROCLIM,			"EPROCLIM",
		"Too many processes",
		""
	},
#endif // #ifdef EPROCLIM
	{	EUSERS,				"EUSERS",
		"Too many users",
		"An operation to read disk quota information for the user failed \
because the system quota table was full."
	},
	{	EDQUOT,				"EDQUOT",
		"Disc quota exceeded",
		"A write() to an ordinary file, the creation of a directory or \
symbolic link, or the creation of a directory entry failed because the \
user's quota of disk blocks was exhausted, or the allocation of an inode \
for a newly created file failed because the user's quota of inodes was \
exhausted."
	},
	{	ESTALE,				"ESTALE",
		"Stale NFS file handle",
		"An NFS client referenced a file that it had opened but that had since \
been deleted."
	},
	{	EREMOTE,				"EREMOTE",
		"Too many levels of remote in path",
		"An attempt was made to remotely mount a file system into a path that \
already has a remotely mounted component."
	},
	{	ENOSTR,				"ENOSTR",
		"Not a stream device",
		"A putmsg() or getmsg() system call was attempted on a file descriptor \
that is not a STREAMS device."
	},
	{	ETIME,				"ETIME",
		"Timer expired",
		"The timer set for a STREAMS ioctl() call has expired. The cause of \
this error is device specific and could indicate either a hardware or \
software failure, or perhaps a timeout value that is too short for the \
specific operation. The status of the ioctl() operation is indeterminate."
	},
	{	ENOSR,				"ENOSR",
		"Out of stream resources",
		"During a STREAMS open(), either no STREAMS queues or no STREAMS head \
data structures were available."
	},
	{	ENOMSG,				"ENOMSG",
		"No message of desired type",
		"An attempt was made to receive a message of a type that does not \
exist on the specified message queue; see msgop()."
	},
	{	EBADMSG,				"EBADMSG",
		"Not a data message",
		"During a read(), getmsg(), or ioctl() I_RECVFD system call to a \
STREAMS device, something has come to the head of the queue that cannot be \
processed.  That something depends on the system call: read() control \
information or a passed file descriptor. getmsg() passed file descriptor. \
ioctl() control or data information."
	},
	{	EIDRM,				"EIDRM",
		"Identifier removed",
		"This error is returned to processes that resume execution due to the \
removal of an identifier from the QUERY"
	},
	{	EDEADLK,				"EDEADLK",
		"Deadlock situation detected/avoided",
		"An attempt was made to lock a system resource that would have \
resulted in a deadlock situation."
	},
	{	ENOLCK,				"ENOLCK",
		"No record locks available",
		"A system-imposed limit on the number of simultaneous file and record \
locks was reached and no more were available at that time."
	},
	{	ENONET,				"ENONET",
		"Machine is not on the network",
		"A attempt was made to advertise, unadvertise, mount, or unmount \
remote resources while the machine has not done the proper startup to \
connect to the network.  This error is Remote File Sharing (RFS) specific."
	},
#ifdef ERREMOTE
	{	ERREMOTE,			"ERREMOTE",
		"Object is remote",
		"An attempt was made to advertise a resource which is not on the local \
machine, or to mount/unmount a device (or pathname) that is on a remote \
machine. This error is RFS specific."
	},
#endif // #ifdef ERREMOTE
	{	ENOLINK,				"ENOLINK",
		"Link has been severed",
		"The link (virtual circuit) connecting to a remote machine is gone. \
This error is RFS specific."
	},
	{	EADV,					"EADV",
		"Advertise error ",
		"An attempt was made to advertise a resource which has been advertised \
already, or to stop the RFS while there ENOSYS are resources still \
advertised, or to force unmount a resource when it is still advertised. \
This error is RFS specific."
	},
	{	ESRMNT,				"ESRMNT",
		"Srmount error ",
		"An attempt was made to stop RFS while there are resources still \
mounted by remote machines. This error is RFS specific."
	},
	{	ECOMM,				"ECOMM",
		"Communication error on send",
		"An attempt was made to send messages to a remote machine when no \
virtual circuit could be found. This error is RFS specific."
	},
	{	EPROTO,				"EPROTO",
		"Protocol error",
		"Some protocol error occurred.  This error is device specific, but is \
generally not related to a hardware failure."
	},
	{	EMULTIHOP,			"EMULTIHOP",
		"Multihop attempted",
		"An attempt was made to access remote resources which are not directly \
accessible.  This error is RFS specific."
	},
	{	EDOTDOT,				"EDOTDOT",
		"EDOTDOT Should never occur",
		""
	},
	{	EREMCHG,				"EREMCHG",
		"Remote address changed",
		""
	},
	{	ENOSYS,				"ENOSYS",
		"Function not implemented",
		"An attempt was made to use a function that is not available in this \
implementation."
	}
	// ********** End of '#else' **********
#endif /* #ifdef _Windows */
};

static const unsigned int NPSL_ErrorCount =
	(sizeof(NPSL_ErrorList) / sizeof(NPSL_ErrorList[0]));

/* *********************************************************************** */

/*	***********************************************************************	*/
void NPSL_GetErrorList(unsigned int *out_count,
	const NPSL_SocketErrorSpec **out_list)
{
	*out_count = NPSL_ErrorCount;
	*out_list  = NPSL_ErrorList;
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

	COPYRIGHT	:	Copyright 1993 - 2015 Michael L. Brock

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

	COPYRIGHT	:	Copyright 1993 - 2015 Michael L. Brock

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
	return(NPSL_AppendErrorString(errno, long_flag, max_text, error_text));
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

	COPYRIGHT	:	Copyright 1993 - 2015 Michael L. Brock

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

	COPYRIGHT	:	Copyright 1993 - 2015 Michael L. Brock

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
	return(NPSL_GetErrorString(errno, long_flag, max_text, error_text));
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

	COPYRIGHT	:	Copyright 1993 - 2015 Michael L. Brock

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
	unsigned int                count_1;
	const NPSL_SocketErrorSpec *errno_ptr = NULL;

	for (count_1 = 0; count_1 < NPSL_ErrorCount; count_1++) {
		if (errno_code == ((int) NPSL_ErrorList[count_1].error_code)) {
			errno_ptr = NPSL_ErrorList + count_1;
			break;
		}
	}

	return(errno_ptr);
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

	COPYRIGHT	:	Copyright 1993 - 2015 Michael L. Brock

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
	return(NPSL_GetErrorPtr(errno));
#endif /* #ifdef _Windows */
}
/* *********************************************************************** */

#ifdef TEST_MAIN

#include <Utility/C_StringSupport.hpp>

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
		if (!MLB::Utility::Utility_strnicmp("-LONG=", argv[count_1], 6)) {
			if ((!MLB::Utility::Utility_stricmp(argv[count_1] + 6, "ON")) ||
				(!MLB::Utility::Utility_stricmp(argv[count_1] + 6, "YES")) ||
				(!MLB::Utility::Utility_stricmp(argv[count_1] + 6, "TRUE")))
				long_flag = 1;
			else if ((!MLB::Utility::Utility_stricmp(argv[count_1] + 6, "OFF")) ||
				(!MLB::Utility::Utility_stricmp(argv[count_1] + 6, "NO")) ||
				(!MLB::Utility::Utility_stricmp(argv[count_1] + 6, "FALSE")))
				long_flag = 0;
			else
				DoUsage(argv[0], "Invalid '-LONG=' toggle", 22);
		}
		else if ((!MLB::Utility::Utility_strnicmp("-MAX=", argv[count_1], 5)) &&
			(strlen(argv[count_1]) > 5) && isdigit(argv[count_1][5]))
			max_text = atoi(argv[count_1] + 5);
		else if (!MLB::Utility::Utility_stricmp("-ALL", argv[count_1])) {
			printf("-----------------------------------------------------\n");
			for (count_2 = 0; count_2 < error_count; count_2++)
				printf("[%05u]:%s\n", count_2,
					NPSL_GetErrorString(error_list[count_2].error_code,
					long_flag, max_text, error_text));
			printf("-----------------------------------------------------\n");
		}
		else if ((!MLB::Utility::Utility_stricmp("-HELP", argv[count_1])) ||
			(!MLB::Utility::Utility_stricmp("-H", argv[count_1])))
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

