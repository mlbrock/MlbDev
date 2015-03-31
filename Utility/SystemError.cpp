// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Utility Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the system error functions.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/IntToString.hpp>
#include <Utility/Utility_Exception.hpp>

#include <sstream>

#ifdef __GNUC__
# include <string.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
typedef struct {
	int         error_code;
	const char *error_name;
	const char *short_description;
	const char *long_description;
} ErrnoSpec_Internal;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
typedef struct {
	SystemErrorCode  error_code;
	const char      *error_name;
	const char      *short_description;
	const char      *long_description;
} SystemErrorSpec_Internal;
#else
typedef ErrnoSpec_Internal SystemErrorSpec_Internal;
#endif // #ifdef _MSC_VER
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Functions private to this module...
// ////////////////////////////////////////////////////////////////////////////
const ErrnoSpec_Internal       *GetErrnoPtr(int errno_code = errno);
#ifdef _Windows
const SystemErrorSpec_Internal *GetSystemErrorPtr(
	SystemErrorCode errno_code = GetLastSystemErrorCode());
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef __MSDOS__
const ErrnoSpec_Internal ErrnoList[] = {
	{  EZERO,				"EZERO",
		"Error 0 / No error detected",
		"No error encountered."
	},
	{	EINVFNC,				"EINVFNC",
		"Invalid function number",
		"Invalid function number."
	},
	{	ENOFILE,				"ENOFILE",
		"File not found",
		"File not found."
	},
	{	ENOPATH,				"ENOPATH",
		"Path not found",
		"Path not found."
	},
	{	ECONTR,				"ECONTR",
		"Memory blocks destroyed",
		"Memory blocks destroyed."
	},
	{	EINVMEM,				"EINVMEM",
		"Invalid memory block address",
		"Invalid memory block address."
	},
	{	EINVENV,				"EINVENV",
		"Invalid environment",
		"Invalid environment."
	},
	{	EINVFMT,				"EINVFMT",
		"Invalid format",
		"Invalid format."
	},
	{	EINVACC,				"EINVACC",
		"Invalid access code",
		"Invalid access code."
	},
	{	EINVDAT,				"EINVDAT",
		"Invalid data",
		"Invalid data."
	},
	{	EINVDRV,				"EINVDRV",
		"Invalid drive specified",
		"Invalid drive specified."
	},
	{	ECURDIR,				"ECURDIR",
		"Attempt to remove current directory",
		"Attempt to remove current directory."
	},
	{	ENOTSAM,				"ENOTSAM",
		"Not same device",
		"Not same device."
	},
	{	ENMFILE,				"ENMFILE",
		"No more files",
		"No more files."
	},
	{  ENOENT,				"ENOENT",
		"No such file or directory",
		"This error occurs when a file name is specified and the file should \
exist but does not, or when one of the directories in a path name does not \
exist."
	},
	{	EMFILE,				"EMFILE",
		"Too many open files",
		"A process tried to have more open files than the system allows a \
process to have. The customary configuration limit is 64 per process."
	},
	{	EACCES,				"EACCES",
		"Permission denied",
		"An attempt was made to access a file in a way forbidden by the \
protection system."
	},
	{  EBADF,				"EBADF",
		"Bad file number",
		"Either a file descriptor refers to no open file, or a read \
(respectively, write) request is made to a file that is open only for \
writing (respectively, reading)."
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
	{	EFAULT,				"EFAULT",
		"Bad address",
		"The system encountered a hardware fault in attempting to access the \
arguments of a system call."
	},
	{	ENODEV,				"ENODEV",
		"No such device",
		"An attempt was made to apply an inappropriate system call to a device \
(for example, an attempt to read a write-only device) or an attempt was made \
to use a device not configured by the system."
	},
	{	EINVAL,				"EINVAL",
		"Invalid argument",
		"A system call was made with an invalid argument; for example, \
dismounting a non-mounted file system, mentioning an unknown signal in \
sigvec() or kill(), reading or writing a file for which lseek() has \
generated a negative pointer, or some other argument inappropriate for the \
call. Also set by math functions, see 'man 3 intro'."
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
	{	EXDEV,				"EXDEV",
		"Cross-device link",
		"A hard link to a file on another file system was attempted."
	},
	{	ENFILE,				"ENFILE",
		"File table overflow",
		"The system's table of open files is full, and temporarily no more \
open() calls can be accepted."
	},
	{	ECHILD,				"ECHILD",
		"No children",
		"A wait() was executed by a process that had no existing or \
unwaited-for child processes."
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
	{	EEXIST,				"EEXIST",
		"File exists",
		"An existing file was mentioned in an inappropriate context, for \
example, link()."
	},
	{	EDEADLOCK,			"EDEADLOCK",
		"Deadlock situation detected/avoided",
		"An attempt was made to lock a system resource that would have \
resulted in a deadlock situation."
	},
	{  EPERM,				"EPERM",
		"Not owner",
		"Typically this error indicates an attempt to modify a file in some \
way forbidden except to its owner or super-user. It is also returned for \
attempts by ordinary users to do things allowed only to the super-user."
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
	{	EAGAIN,				"EAGAIN",
		"No more processes",
		"A fork() failed because the system's process table is full or the \
user is not allowed to create any more processes, or a system call failed \
because of insufficient resources."
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
	{	ENOTDIR,				"ENOTDIR",
		"Not a directory",
		"A non-directory was specified where a directory is required, for \
example, in a path prefix or as an argument to chdir()."
	},
	{	EISDIR,				"EISDIR",
		"Is a directory",
		"An attempt was made to write on a directory."
	},
	{	EUCLEAN,				"EUCLEAN",
		"*** UNKNOWN ***",
		"Unknown error."
	}
};
	// ********** End of '#ifdef __MSDOS__' **********
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
#endif // # ifdef EDOTDOT
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
#endif // # ifdef ESTART
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
#elif _MSC_VER
const ErrnoSpec_Internal ErrnoList[] = {
/*
	{  EZERO,				"EZERO",
		"Error 0 / No error detected",
		"No error encountered."
	},
	{	EINVFNC,				"EINVFNC",
		"Invalid function number",
		"Invalid function number."
	},
	{	ENOFILE,				"ENOFILE",
		"File not found",
		"File not found."
	},
	{	ENOPATH,				"ENOPATH",
		"Path not found",
		"Path not found."
	},
	{	ECONTR,				"ECONTR",
		"Memory blocks destroyed",
		"Memory blocks destroyed."
	},
	{	EINVMEM,				"EINVMEM",
		"Invalid memory block address",
		"Invalid memory block address."
	},
	{	EINVENV,				"EINVENV",
		"Invalid environment",
		"Invalid environment."
	},
	{	EINVFMT,				"EINVFMT",
		"Invalid format",
		"Invalid format."
	},
	{	EINVACC,				"EINVACC",
		"Invalid access code",
		"Invalid access code."
	},
	{	EINVDAT,				"EINVDAT",
		"Invalid data",
		"Invalid data."
	},
	{	EINVDRV,				"EINVDRV",
		"Invalid drive specified",
		"Invalid drive specified."
	},
	{	ECURDIR,				"ECURDIR",
		"Attempt to remove current directory",
		"Attempt to remove current directory."
	},
	{	ENOTSAM,				"ENOTSAM",
		"Not same device",
		"Not same device."
	},
	{	ENMFILE,				"ENMFILE",
		"No more files",
		"No more files."
	},
*/
	{  ENOENT,				"ENOENT",
		"No such file or directory",
		"This error occurs when a file name is specified and the file should \
exist but does not, or when one of the directories in a path name does not \
exist."
	},
	{	EMFILE,				"EMFILE",
		"Too many open files",
		"A process tried to have more open files than the system allows a \
process to have. The customary configuration limit is 64 per process."
	},
	{	EACCES,				"EACCES",
		"Permission denied",
		"An attempt was made to access a file in a way forbidden by the \
protection system."
	},
	{  EBADF,				"EBADF",
		"Bad file number",
		"Either a file descriptor refers to no open file, or a read \
(respectively, write) request is made to a file that is open only for \
writing (respectively, reading)."
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
	{	EFAULT,				"EFAULT",
		"Bad address",
		"The system encountered a hardware fault in attempting to access the \
arguments of a system call."
	},
	{	ENODEV,				"ENODEV",
		"No such device",
		"An attempt was made to apply an inappropriate system call to a device \
(for example, an attempt to read a write-only device) or an attempt was made \
to use a device not configured by the system."
	},
	{	EINVAL,				"EINVAL",
		"Invalid argument",
		"A system call was made with an invalid argument; for example, \
dismounting a non-mounted file system, mentioning an unknown signal in \
sigvec() or kill(), reading or writing a file for which lseek() has \
generated a negative pointer, or some other argument inappropriate for the \
call. Also set by math functions, see 'man 3 intro'."
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
	{	EXDEV,				"EXDEV",
		"Cross-device link",
		"A hard link to a file on another file system was attempted."
	},
	{	ENFILE,				"ENFILE",
		"File table overflow",
		"The system's table of open files is full, and temporarily no more \
open() calls can be accepted."
	},
	{	ECHILD,				"ECHILD",
		"No children",
		"A wait() was executed by a process that had no existing or \
unwaited-for child processes."
	},
	{	ENOTTY,				"ENOTTY",
		"Inappropriate ioctl for device",
		"The code used in an ioctl() call is not supported by the object that \
the file descriptor in the call refers to."
	},
/*
	{	ETXTBSY,				"ETXTBSY",
		"Text file busy",
		"An attempt was made to execute a pure-procedure program that is \
currently open for writing, or an attempt was made to open for writing a \
pure-procedure program that is being executed."
	},
*/
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
	{	EEXIST,				"EEXIST",
		"File exists",
		"An existing file was mentioned in an inappropriate context, for \
example, link()."
	},
	{	EDEADLOCK,			"EDEADLOCK",
		"Deadlock situation detected/avoided",
		"An attempt was made to lock a system resource that would have \
resulted in a deadlock situation."
	},
	{  EPERM,				"EPERM",
		"Not owner",
		"Typically this error indicates an attempt to modify a file in some \
way forbidden except to its owner or super-user. It is also returned for \
attempts by ordinary users to do things allowed only to the super-user."
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
	{	EAGAIN,				"EAGAIN",
		"No more processes",
		"A fork() failed because the system's process table is full or the \
user is not allowed to create any more processes, or a system call failed \
because of insufficient resources."
	},
/*
	{	ENOTBLK,				"ENOTBLK",
		"Block device required",
		"A file that is not a block device was mentioned where a block device \
was required, for example, in mount()."
	},
*/
	{	EBUSY,				"EBUSY",
		"Device busy",
		"An attempt was made to mount a file system that was already mounted \
or an attempt was made to dismount a file system on which there is an \
active file (open file, mapped file, current directory, or mounted-on \
directory)."
	},
	{	ENOTDIR,				"ENOTDIR",
		"Not a directory",
		"A non-directory was specified where a directory is required, for \
example, in a path prefix or as an argument to chdir()."
	},
	{	EISDIR,				"EISDIR",
		"Is a directory",
		"An attempt was made to write on a directory."
	}
/*
	,
	{	EUCLEAN,				"EUCLEAN",
		"*** UNKNOWN ***",
		"Unknown error."
	}
*/
};
#elif __MWERKS__
const ErrnoSpec_Internal ErrnoList[] = {
	{  ENOERR,				"ENOERR",
		"Error 0 / No error detected",
		"No error detected."
	},
# ifdef EINVFNC
   {	EINVFNC,				"EINVFNC",
		"Invalid function number",
		"Invalid function number."
	},
# endif // # ifdef EINVFNC
# ifdef ENOFILE
	{	ENOFILE,				"ENOFILE",
		"File not found",
		"File not found."
	},
# endif // # ifdef ENOFILE
# ifdef ENOPATH
	{	ENOPATH,				"ENOPATH",
		"Path not found",
		"Path not found."
	},
# endif // # ifdef ENOPATH
# ifdef ECONTR
	{	ECONTR,				"ECONTR",
		"Memory blocks destroyed",
		"Memory blocks destroyed."
	},
# endif // # ifdef ECONTR
# ifdef EINVMEM
	{	EINVMEM,				"EINVMEM",
		"Invalid memory block address",
		"Invalid memory block address."
	},
# endif // # ifdef EINVMEM
# ifdef EINVENV
	{	EINVENV,				"EINVENV",
		"Invalid environment",
		"Invalid environment."
	},
# endif // # ifdef EINVENV
# ifdef EINVFMT
	{	EINVFMT,				"EINVFMT",
		"Invalid format",
		"Invalid format."
	},
# endif // # ifdef EINVFMT
# ifdef EINVACC
	{	EINVACC,				"EINVACC",
		"Invalid access code",
		"Invalid access code."
	},
# endif // # ifdef EINVACC
# ifdef EINVDAT
	{	EINVDAT,				"EINVDAT",
		"Invalid data",
		"Invalid data."
	},
# endif // # ifdef EINVDAT
# ifdef EINVDRV
	{	EINVDRV,				"EINVDRV",
		"Invalid drive specified",
		"Invalid drive specified."
	},
# endif // # ifdef EINVDRV
# ifdef ECURDIR
	{	ECURDIR,				"ECURDIR",
		"Attempt to remove current directory",
		"Attempt to remove current directory."
	},
# endif // # ifdef ECURDIR
# ifdef ENOTSAM
	{	ENOTSAM,				"ENOTSAM",
		"Not same device",
		"Not same device."
	},
# endif // # ifdef ENOTSAM
# ifdef ENMFILE
	{	ENMFILE,				"ENMFILE",
		"No more files",
		"No more files."
	},
# endif // # ifdef ENMFILE
	{  ENOENT,				"ENOENT",
		"No such file or directory",
		"This error occurs when a file name is specified and the file should \
exist but does not, or when one of the directories in a path name does not \
exist."
	},
	{	EMFILE,				"EMFILE",
		"Too many open files",
		"A process tried to have more open files than the system allows a \
process to have. The customary configuration limit is 64 per process."
	},
	{	EACCES,				"EACCES",
		"Permission denied",
		"An attempt was made to access a file in a way forbidden by the \
protection system."
	},
	{  EBADF,				"EBADF",
		"Bad file descriptor",
		"Either a file descriptor refers to no open file, or a read \
(respectively, write) request is made to a file that is open only for \
writing (respectively, reading)."
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
	{	EFAULT,				"EFAULT",
		"Bad address",
		"The system encountered a hardware fault in attempting to access the \
arguments of a system call."
	},
	{	ENODEV,				"ENODEV",
		"Operation not supported by device / No such device",
		"An attempt was made to apply an inappropriate system call to a device \
(for example, an attempt to read a write-only device) or an attempt was made \
to use a device not configured by the system."
	},
	{	EINVAL,				"EINVAL",
		"Invalid argument",
		"A system call was made with an invalid argument; for example, \
dismounting a non-mounted file system, mentioning an unknown signal in \
sigvec() or kill(), reading or writing a file for which lseek() has \
generated a negative pointer, or some other argument inappropriate for the \
call. Also set by math functions, see 'man 3 intro'."
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
	{	EXDEV,				"EXDEV",
		"Cross-device link",
		"A hard link to a file on another file system was attempted."
	},
	{	ENFILE,				"ENFILE",
		"Too many open files in system / File table overflow",
		"The system's table of open files is full, and temporarily no more \
open() calls can be accepted."
	},
	{	ECHILD,				"ECHILD",
		"No child processes",
		"A wait() was executed by a process that had no existing or \
unwaited-for child processes."
	},
	{	ENOTTY,				"ENOTTY",
		"Inappropriate ioctl for device",
		"The code used in an ioctl() call is not supported by the object that \
the file descriptor in the call refers to."
	},
# ifdef ETXTBSY
	{	ETXTBSY,				"ETXTBSY",
		"Text file busy",
		"An attempt was made to execute a pure-procedure program that is \
currently open for writing, or an attempt was made to open for writing a \
pure-procedure program that is being executed."
	},
# endif // # ifdef ETXTBSY
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
		"Numerical argument too large",
		"The argument of a function in the math library (as described in \
section 3M) is out of the domain of the function."
	},
	{	ERANGE,				"ERANGE",
		"Result too large",
		"The value of a function in the math library (as described in section \
3M) is unrepresentable within machine precision."
	},
	{	EEXIST,				"EEXIST",
		"File exists",
		"An existing file was mentioned in an inappropriate context, for \
example, link()."
	},
	{	EDEADLOCK,			"EDEADLOCK",
		"Deadlock situation detected/avoided",
		"An attempt was made to lock a system resource that would have \
resulted in a deadlock situation."
	},
	{  EPERM,				"EPERM",
		"Operation not permitted / Not owner",
		"Typically this error indicates an attempt to modify a file in some \
way forbidden except to its owner or super-user. It is also returned for \
attempts by ordinary users to do things allowed only to the super-user."
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
		"Device not configured / No such device or address",
		"I/O on a special file refers to a subdevice that does not exist, or \
beyond the limits of the device. It may also occur when, for example, a tape \
drive is not on-line or no disk pack is loaded on a drive."
	},
	{	EAGAIN,				"EAGAIN",
		"Resource temporarily unavailable / No more processes",
		"A fork() failed because the system's process table is full or the \
user is not allowed to create any more processes, or a system call failed \
because of insufficient resources."
	},
# ifdef ENOTBLK
	,{	ENOTBLK,				"ENOTBLK",
		"Block device required",
		"A file that is not a block device was mentioned where a block device \
was required, for example, in mount()."
	}
# endif // # ifdef ENOTBLK
	{	EBUSY,				"EBUSY",
		"Device busy",
		"An attempt was made to mount a file system that was already mounted \
or an attempt was made to dismount a file system on which there is an \
active file (open file, mapped file, current directory, or mounted-on \
directory)."
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
# ifdef EUCLEAN
	,{	EUCLEAN,				"EUCLEAN",
		"*** UNKNOWN ***",
		"Unknown error."
	}
# endif // # ifdef EUCLEAN
	{
		EILSEQ,	"EILSEQ",
		"Wide character encoding error",
		"Illegal byte sequence. Cannot handle multiple characters as a single \
character."
	}
# ifdef ENAMETOOLONG
	,{
		ENAMETOOLONG,	"ENAMETOOLONG",
		"File name too long",
		"The length of the path argument exceeds PATH_MAX, or the length of a \
path component exceeds NAME_MAX while _POSIX_NO_TRUNC is in effect; see \
limits(4)."
	}
# endif // # ifdef ENAMETOOLONG
# ifdef ENOSYS
	,{
		ENOSYS,	"ENOSYS",
		"Function not implemented / Operation not applicable",
		"Operation not applicable."
	}
# endif // # ifdef ENOSYS
# ifdef ENOTEMPTY
	,{
		ENOTEMPTY,	"ENOTEMPTY",
		"Directory not empty",
		"Directory not empty."
	}
# endif // # ifdef ENOTEMPTY
# ifdef ENOLCK
	,{
		ENOLCK,	"ENOLCK",
		"No record locks available",
		"There are no more locks available. The system lock table is full (see \
fcntl(2))."
	}
# endif // # ifdef ENOLCK
# ifdef EFPOS
	,{	EFPOS,	"EFPOS",
		"File position error",
		"File position error."
	}
# endif // # ifdef EFPOS
# ifdef EMACOSERR
	,{	EMACOSERR,	"EMACOSERR",
		"Mac OS error",
		"Mac OS error."
	}
# endif // # ifdef EMACOSERR
# ifdef ESIGPARM
	,{	ESIGPARM,	"ESIGPARM",
		"Signal error",
		"Signal error."
	}
# endif // # ifdef ESIGPARM
# ifdef EUNKNOWN
	,{	EUNKNOWN,	"EUNKNOWN",
		"Unknown error",
		"Unknown error."
	}
# endif // # ifdef EUNKNOWN
};
#elif _Windows
const ErrnoSpec_Internal ErrnoList[] = {
	{  EZERO,				"EZERO",
		"Error 0 / No error detected",
		"No error encountered."
	},
	{	EINVFNC,				"EINVFNC",
		"Invalid function number",
		"Invalid function number."
	},
	{	ENOFILE,				"ENOFILE",
		"File not found",
		"File not found."
	},
	{	ENOPATH,				"ENOPATH",
		"Path not found",
		"Path not found."
	},
	{	ECONTR,				"ECONTR",
		"Memory blocks destroyed",
		"Memory blocks destroyed."
	},
	{	EINVMEM,				"EINVMEM",
		"Invalid memory block address",
		"Invalid memory block address."
	},
	{	EINVENV,				"EINVENV",
		"Invalid environment",
		"Invalid environment."
	},
	{	EINVFMT,				"EINVFMT",
		"Invalid format",
		"Invalid format."
	},
	{	EINVACC,				"EINVACC",
		"Invalid access code",
		"Invalid access code."
	},
	{	EINVDAT,				"EINVDAT",
		"Invalid data",
		"Invalid data."
	},
	{	EINVDRV,				"EINVDRV",
		"Invalid drive specified",
		"Invalid drive specified."
	},
	{	ECURDIR,				"ECURDIR",
		"Attempt to remove current directory",
		"Attempt to remove current directory."
	},
	{	ENOTSAM,				"ENOTSAM",
		"Not same device",
		"Not same device."
	},
	{	ENMFILE,				"ENMFILE",
		"No more files",
		"No more files."
	},
	{  ENOENT,				"ENOENT",
		"No such file or directory",
		"This error occurs when a file name is specified and the file should \
exist but does not, or when one of the directories in a path name does not \
exist."
	},
	{	EMFILE,				"EMFILE",
		"Too many open files",
		"A process tried to have more open files than the system allows a \
process to have. The customary configuration limit is 64 per process."
	},
	{	EACCES,				"EACCES",
		"Permission denied",
		"An attempt was made to access a file in a way forbidden by the \
protection system."
	},
	{  EBADF,				"EBADF",
		"Bad file number",
		"Either a file descriptor refers to no open file, or a read \
(respectively, write) request is made to a file that is open only for \
writing (respectively, reading)."
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
	{	EFAULT,				"EFAULT",
		"Bad address",
		"The system encountered a hardware fault in attempting to access the \
arguments of a system call."
	},
	{	ENODEV,				"ENODEV",
		"No such device",
		"An attempt was made to apply an inappropriate system call to a device \
(for example, an attempt to read a write-only device) or an attempt was made \
to use a device not configured by the system."
	},
	{	EINVAL,				"EINVAL",
		"Invalid argument",
		"A system call was made with an invalid argument; for example, \
dismounting a non-mounted file system, mentioning an unknown signal in \
sigvec() or kill(), reading or writing a file for which lseek() has \
generated a negative pointer, or some other argument inappropriate for the \
call. Also set by math functions, see 'man 3 intro'."
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
	{	EXDEV,				"EXDEV",
		"Cross-device link",
		"A hard link to a file on another file system was attempted."
	},
	{	ENFILE,				"ENFILE",
		"File table overflow",
		"The system's table of open files is full, and temporarily no more \
open() calls can be accepted."
	},
	{	ECHILD,				"ECHILD",
		"No children",
		"A wait() was executed by a process that had no existing or \
unwaited-for child processes."
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
	{	EEXIST,				"EEXIST",
		"File exists",
		"An existing file was mentioned in an inappropriate context, for \
example, link()."
	},
	{	EDEADLOCK,			"EDEADLOCK",
		"Deadlock situation detected/avoided",
		"An attempt was made to lock a system resource that would have \
resulted in a deadlock situation."
	},
	{  EPERM,				"EPERM",
		"Not owner",
		"Typically this error indicates an attempt to modify a file in some \
way forbidden except to its owner or super-user. It is also returned for \
attempts by ordinary users to do things allowed only to the super-user."
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
	{	EAGAIN,				"EAGAIN",
		"No more processes",
		"A fork() failed because the system's process table is full or the \
user is not allowed to create any more processes, or a system call failed \
because of insufficient resources."
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
	{	ENOTDIR,				"ENOTDIR",
		"Not a directory",
		"A non-directory was specified where a directory is required, for \
example, in a path prefix or as an argument to chdir()."
	},
	{	EISDIR,				"EISDIR",
		"Is a directory",
		"An attempt was made to write on a directory."
	},
	{	EUCLEAN,				"EUCLEAN",
		"*** UNKNOWN ***",
		"Unknown error."
	}
};
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
const ErrnoSpec_Internal ErrnoList[] = {
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
};
#endif // #ifdef __MSDOS__

const unsigned int ErrnoCount = (sizeof(ErrnoList) / sizeof(ErrnoList[0]));
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
const SystemErrorSpec_Internal SystemErrorList[] = {
#ifdef ERROR_SUCCESS
	{	ERROR_SUCCESS,                                            "ERROR_SUCCESS",
		"The operation completed successfully.",
		"The operation completed successfully."
	},
#endif // #ifdef ERROR_SUCCESS
#ifdef ERROR_INVALID_FUNCTION
	{	ERROR_INVALID_FUNCTION,                                   "ERROR_INVALID_FUNCTION",
		"Incorrect function.",
		"Incorrect function."
	},
#endif // #ifdef ERROR_INVALID_FUNCTION
#ifdef ERROR_FILE_NOT_FOUND
	{	ERROR_FILE_NOT_FOUND,                                     "ERROR_FILE_NOT_FOUND",
		"The system cannot find the file specified.",
		"The system cannot find the file specified."
	},
#endif // #ifdef ERROR_FILE_NOT_FOUND
#ifdef ERROR_PATH_NOT_FOUND
	{	ERROR_PATH_NOT_FOUND,                                     "ERROR_PATH_NOT_FOUND",
		"The system cannot find the path specified.",
		"The system cannot find the path specified."
	},
#endif // #ifdef ERROR_PATH_NOT_FOUND
#ifdef ERROR_TOO_MANY_OPEN_FILES
	{	ERROR_TOO_MANY_OPEN_FILES,                                "ERROR_TOO_MANY_OPEN_FILES",
		"The system cannot open the file.",
		"The system cannot open the file."
	},
#endif // #ifdef ERROR_TOO_MANY_OPEN_FILES
#ifdef ERROR_ACCESS_DENIED
	{	ERROR_ACCESS_DENIED,                                      "ERROR_ACCESS_DENIED",
		"Access is denied.",
		"Access is denied."
	},
#endif // #ifdef ERROR_ACCESS_DENIED
#ifdef ERROR_INVALID_HANDLE
	{	ERROR_INVALID_HANDLE,                                     "ERROR_INVALID_HANDLE",
		"The handle is invalid.",
		"The handle is invalid."
	},
#endif // #ifdef ERROR_INVALID_HANDLE
#ifdef ERROR_ARENA_TRASHED
	{	ERROR_ARENA_TRASHED,                                      "ERROR_ARENA_TRASHED",
		"The storage control blocks were destroyed.",
		"The storage control blocks were destroyed."
	},
#endif // #ifdef ERROR_ARENA_TRASHED
#ifdef ERROR_NOT_ENOUGH_MEMORY
	{	ERROR_NOT_ENOUGH_MEMORY,                                  "ERROR_NOT_ENOUGH_MEMORY",
		"Not enough storage is available to process this command.",
		"Not enough storage is available to process this command."
	},
#endif // #ifdef ERROR_NOT_ENOUGH_MEMORY
#ifdef ERROR_INVALID_BLOCK
	{	ERROR_INVALID_BLOCK,                                      "ERROR_INVALID_BLOCK",
		"The storage control block address is invalid.",
		"The storage control block address is invalid."
	},
#endif // #ifdef ERROR_INVALID_BLOCK
#ifdef ERROR_BAD_ENVIRONMENT
	{	ERROR_BAD_ENVIRONMENT,                                    "ERROR_BAD_ENVIRONMENT",
		"The environment is incorrect.",
		"The environment is incorrect."
	},
#endif // #ifdef ERROR_BAD_ENVIRONMENT
#ifdef ERROR_BAD_FORMAT
	{	ERROR_BAD_FORMAT,                                         "ERROR_BAD_FORMAT",
		"An attempt was made to load a program with an incorrect format.",
		"An attempt was made to load a program with an incorrect format."
	},
#endif // #ifdef ERROR_BAD_FORMAT
#ifdef ERROR_INVALID_ACCESS
	{	ERROR_INVALID_ACCESS,                                     "ERROR_INVALID_ACCESS",
		"The access code is invalid.",
		"The access code is invalid."
	},
#endif // #ifdef ERROR_INVALID_ACCESS
#ifdef ERROR_INVALID_DATA
	{	ERROR_INVALID_DATA,                                       "ERROR_INVALID_DATA",
		"The data is invalid.",
		"The data is invalid."
	},
#endif // #ifdef ERROR_INVALID_DATA
#ifdef ERROR_OUTOFMEMORY
	{	ERROR_OUTOFMEMORY,                                        "ERROR_OUTOFMEMORY",
		"Not enough storage is available to complete this operation.",
		"Not enough storage is available to complete this operation."
	},
#endif // #ifdef ERROR_OUTOFMEMORY
#ifdef ERROR_INVALID_DRIVE
	{	ERROR_INVALID_DRIVE,                                      "ERROR_INVALID_DRIVE",
		"The system cannot find the drive specified.",
		"The system cannot find the drive specified."
	},
#endif // #ifdef ERROR_INVALID_DRIVE
#ifdef ERROR_CURRENT_DIRECTORY
	{	ERROR_CURRENT_DIRECTORY,                                  "ERROR_CURRENT_DIRECTORY",
		"The directory cannot be removed.",
		"The directory cannot be removed."
	},
#endif // #ifdef ERROR_CURRENT_DIRECTORY
#ifdef ERROR_NOT_SAME_DEVICE
	{	ERROR_NOT_SAME_DEVICE,                                    "ERROR_NOT_SAME_DEVICE",
		"The system cannot move the file to a different disk drive.",
		"The system cannot move the file to a different disk drive."
	},
#endif // #ifdef ERROR_NOT_SAME_DEVICE
#ifdef ERROR_NO_MORE_FILES
	{	ERROR_NO_MORE_FILES,                                      "ERROR_NO_MORE_FILES",
		"There are no more files.",
		"There are no more files."
	},
#endif // #ifdef ERROR_NO_MORE_FILES
#ifdef ERROR_WRITE_PROTECT
	{	ERROR_WRITE_PROTECT,                                      "ERROR_WRITE_PROTECT",
		"The media is write protected.",
		"The media is write protected."
	},
#endif // #ifdef ERROR_WRITE_PROTECT
#ifdef ERROR_BAD_UNIT
	{	ERROR_BAD_UNIT,                                           "ERROR_BAD_UNIT",
		"The system cannot find the device specified.",
		"The system cannot find the device specified."
	},
#endif // #ifdef ERROR_BAD_UNIT
#ifdef ERROR_NOT_READY
	{	ERROR_NOT_READY,                                          "ERROR_NOT_READY",
		"The device is not ready.",
		"The device is not ready."
	},
#endif // #ifdef ERROR_NOT_READY
#ifdef ERROR_BAD_COMMAND
	{	ERROR_BAD_COMMAND,                                        "ERROR_BAD_COMMAND",
		"The device does not recognize the command.",
		"The device does not recognize the command."
	},
#endif // #ifdef ERROR_BAD_COMMAND
#ifdef ERROR_CRC
	{	ERROR_CRC,                                                "ERROR_CRC",
		"Data error (cyclic redundancy check).",
		"Data error (cyclic redundancy check)."
	},
#endif // #ifdef ERROR_CRC
#ifdef ERROR_BAD_LENGTH
	{	ERROR_BAD_LENGTH,                                         "ERROR_BAD_LENGTH",
		"The program issued a command but the command length is incorrect.",
		"The program issued a command but the command length is incorrect."
	},
#endif // #ifdef ERROR_BAD_LENGTH
#ifdef ERROR_SEEK
	{	ERROR_SEEK,                                               "ERROR_SEEK",
		"The drive cannot locate a specific area or track on the disk.",
		"The drive cannot locate a specific area or track on the disk."
	},
#endif // #ifdef ERROR_SEEK
#ifdef ERROR_NOT_DOS_DISK
	{	ERROR_NOT_DOS_DISK,                                       "ERROR_NOT_DOS_DISK",
		"The specified disk or diskette cannot be accessed.",
		"The specified disk or diskette cannot be accessed."
	},
#endif // #ifdef ERROR_NOT_DOS_DISK
#ifdef ERROR_SECTOR_NOT_FOUND
	{	ERROR_SECTOR_NOT_FOUND,                                   "ERROR_SECTOR_NOT_FOUND",
		"The drive cannot find the sector requested.",
		"The drive cannot find the sector requested."
	},
#endif // #ifdef ERROR_SECTOR_NOT_FOUND
#ifdef ERROR_OUT_OF_PAPER
	{	ERROR_OUT_OF_PAPER,                                       "ERROR_OUT_OF_PAPER",
		"The printer is out of paper.",
		"The printer is out of paper."
	},
#endif // #ifdef ERROR_OUT_OF_PAPER
#ifdef ERROR_WRITE_FAULT
	{	ERROR_WRITE_FAULT,                                        "ERROR_WRITE_FAULT",
		"The system cannot write to the specified device.",
		"The system cannot write to the specified device."
	},
#endif // #ifdef ERROR_WRITE_FAULT
#ifdef ERROR_READ_FAULT
	{	ERROR_READ_FAULT,                                         "ERROR_READ_FAULT",
		"The system cannot read from the specified device.",
		"The system cannot read from the specified device."
	},
#endif // #ifdef ERROR_READ_FAULT
#ifdef ERROR_GEN_FAILURE
	{	ERROR_GEN_FAILURE,                                        "ERROR_GEN_FAILURE",
		"A device attached to the system is not functioning.",
		"A device attached to the system is not functioning."
	},
#endif // #ifdef ERROR_GEN_FAILURE
#ifdef ERROR_SHARING_VIOLATION
	{	ERROR_SHARING_VIOLATION,                                  "ERROR_SHARING_VIOLATION",
		"The process cannot access the file because it is being used by another process.",
		"The process cannot access the file because it is being used by another process."
	},
#endif // #ifdef ERROR_SHARING_VIOLATION
#ifdef ERROR_LOCK_VIOLATION
	{	ERROR_LOCK_VIOLATION,                                     "ERROR_LOCK_VIOLATION",
		"The process cannot access the file because another process has locked a portion of the file.",
		"The process cannot access the file because another process has locked a portion of the file."
	},
#endif // #ifdef ERROR_LOCK_VIOLATION
#ifdef ERROR_WRONG_DISK
	{	ERROR_WRONG_DISK,                                         "ERROR_WRONG_DISK",
		"The wrong diskette is in the drive.",
		"The wrong diskette is in the drive. Insert the correct volume into the drive."
	},
#endif // #ifdef ERROR_WRONG_DISK
#ifdef ERROR_SHARING_BUFFER_EXCEEDED
	{	ERROR_SHARING_BUFFER_EXCEEDED,                            "ERROR_SHARING_BUFFER_EXCEEDED",
		"Too many files opened for sharing.",
		"Too many files opened for sharing."
	},
#endif // #ifdef ERROR_SHARING_BUFFER_EXCEEDED
#ifdef ERROR_HANDLE_EOF
	{	ERROR_HANDLE_EOF,                                         "ERROR_HANDLE_EOF",
		"Reached the end of the file.",
		"Reached the end of the file."
	},
#endif // #ifdef ERROR_HANDLE_EOF
#ifdef ERROR_HANDLE_DISK_FULL
	{	ERROR_HANDLE_DISK_FULL,                                   "ERROR_HANDLE_DISK_FULL",
		"The disk is full.",
		"The disk is full."
	},
#endif // #ifdef ERROR_HANDLE_DISK_FULL
#ifdef ERROR_NOT_SUPPORTED
	{	ERROR_NOT_SUPPORTED,                                      "ERROR_NOT_SUPPORTED",
		"The request is not supported.",
		"The request is not supported."
	},
#endif // #ifdef ERROR_NOT_SUPPORTED
#ifdef ERROR_REM_NOT_LIST
	{	ERROR_REM_NOT_LIST,                                       "ERROR_REM_NOT_LIST",
		"The remote computer network path is not available.",
		"Windows cannot find the network path. Verify that the network path is correct and the destination computer is not busy or turned off. If Windows still cannot find the network path, contact your network administrator."
	},
#endif // #ifdef ERROR_REM_NOT_LIST
#ifdef ERROR_DUP_NAME
	{	ERROR_DUP_NAME,                                           "ERROR_DUP_NAME",
		"A duplicate name exists on the network.",
		"You were not connected because a duplicate name exists on the network. Go to System in the Control Panel to change the computer name and try again."
	},
#endif // #ifdef ERROR_DUP_NAME
#ifdef ERROR_BAD_NETPATH
	{	ERROR_BAD_NETPATH,                                        "ERROR_BAD_NETPATH",
		"The network path was not found.",
		"The network path was not found."
	},
#endif // #ifdef ERROR_BAD_NETPATH
#ifdef ERROR_NETWORK_BUSY
	{	ERROR_NETWORK_BUSY,                                       "ERROR_NETWORK_BUSY",
		"The network is busy.",
		"The network is busy."
	},
#endif // #ifdef ERROR_NETWORK_BUSY
#ifdef ERROR_DEV_NOT_EXIST
	{	ERROR_DEV_NOT_EXIST,                                      "ERROR_DEV_NOT_EXIST",
		"The specified network resource or device is no longer available.",
		"The specified network resource or device is no longer available."
	},
#endif // #ifdef ERROR_DEV_NOT_EXIST
#ifdef ERROR_TOO_MANY_CMDS
	{	ERROR_TOO_MANY_CMDS,                                      "ERROR_TOO_MANY_CMDS",
		"The network BIOS command limit has been reached.",
		"The network BIOS command limit has been reached."
	},
#endif // #ifdef ERROR_TOO_MANY_CMDS
#ifdef ERROR_ADAP_HDW_ERR
	{	ERROR_ADAP_HDW_ERR,                                       "ERROR_ADAP_HDW_ERR",
		"A network adapter hardware error occurred.",
		"A network adapter hardware error occurred."
	},
#endif // #ifdef ERROR_ADAP_HDW_ERR
#ifdef ERROR_BAD_NET_RESP
	{	ERROR_BAD_NET_RESP,                                       "ERROR_BAD_NET_RESP",
		"The specified server cannot perform the requested operation.",
		"The specified server cannot perform the requested operation."
	},
#endif // #ifdef ERROR_BAD_NET_RESP
#ifdef ERROR_UNEXP_NET_ERR
	{	ERROR_UNEXP_NET_ERR,                                      "ERROR_UNEXP_NET_ERR",
		"An unexpected network error occurred.",
		"An unexpected network error occurred."
	},
#endif // #ifdef ERROR_UNEXP_NET_ERR
#ifdef ERROR_BAD_REM_ADAP
	{	ERROR_BAD_REM_ADAP,                                       "ERROR_BAD_REM_ADAP",
		"The remote adapter is not compatible.",
		"The remote adapter is not compatible."
	},
#endif // #ifdef ERROR_BAD_REM_ADAP
#ifdef ERROR_PRINTQ_FULL
	{	ERROR_PRINTQ_FULL,                                        "ERROR_PRINTQ_FULL",
		"The printer queue is full.",
		"The printer queue is full."
	},
#endif // #ifdef ERROR_PRINTQ_FULL
#ifdef ERROR_NO_SPOOL_SPACE
	{	ERROR_NO_SPOOL_SPACE,                                     "ERROR_NO_SPOOL_SPACE",
		"Space to store the file waiting to be printed is not available on the server.",
		"Space to store the file waiting to be printed is not available on the server."
	},
#endif // #ifdef ERROR_NO_SPOOL_SPACE
#ifdef ERROR_PRINT_CANCELLED
	{	ERROR_PRINT_CANCELLED,                                    "ERROR_PRINT_CANCELLED",
		"Your file waiting to be printed was deleted.",
		"Your file waiting to be printed was deleted."
	},
#endif // #ifdef ERROR_PRINT_CANCELLED
#ifdef ERROR_NETNAME_DELETED
	{	ERROR_NETNAME_DELETED,                                    "ERROR_NETNAME_DELETED",
		"The specified network name is no longer available.",
		"The specified network name is no longer available."
	},
#endif // #ifdef ERROR_NETNAME_DELETED
#ifdef ERROR_NETWORK_ACCESS_DENIED
	{	ERROR_NETWORK_ACCESS_DENIED,                              "ERROR_NETWORK_ACCESS_DENIED",
		"Network access is denied.",
		"Network access is denied."
	},
#endif // #ifdef ERROR_NETWORK_ACCESS_DENIED
#ifdef ERROR_BAD_DEV_TYPE
	{	ERROR_BAD_DEV_TYPE,                                       "ERROR_BAD_DEV_TYPE",
		"The network resource type is not correct.",
		"The network resource type is not correct."
	},
#endif // #ifdef ERROR_BAD_DEV_TYPE
#ifdef ERROR_BAD_NET_NAME
	{	ERROR_BAD_NET_NAME,                                       "ERROR_BAD_NET_NAME",
		"The network name cannot be found.",
		"The network name cannot be found."
	},
#endif // #ifdef ERROR_BAD_NET_NAME
#ifdef ERROR_TOO_MANY_NAMES
	{	ERROR_TOO_MANY_NAMES,                                     "ERROR_TOO_MANY_NAMES",
		"The name limit for the local computer network adapter card was exceeded.",
		"The name limit for the local computer network adapter card was exceeded."
	},
#endif // #ifdef ERROR_TOO_MANY_NAMES
#ifdef ERROR_TOO_MANY_SESS
	{	ERROR_TOO_MANY_SESS,                                      "ERROR_TOO_MANY_SESS",
		"The network BIOS session limit was exceeded.",
		"The network BIOS session limit was exceeded."
	},
#endif // #ifdef ERROR_TOO_MANY_SESS
#ifdef ERROR_SHARING_PAUSED
	{	ERROR_SHARING_PAUSED,                                     "ERROR_SHARING_PAUSED",
		"The remote server has been paused or is in the process of being started.",
		"The remote server has been paused or is in the process of being started."
	},
#endif // #ifdef ERROR_SHARING_PAUSED
#ifdef ERROR_REQ_NOT_ACCEP
	{	ERROR_REQ_NOT_ACCEP,                                      "ERROR_REQ_NOT_ACCEP",
		"No more connections can be made to this remote computer at this time because there are already as many connections as the computer can accept.",
		"No more connections can be made to this remote computer at this time because there are already as many connections as the computer can accept."
	},
#endif // #ifdef ERROR_REQ_NOT_ACCEP
#ifdef ERROR_REDIR_PAUSED
	{	ERROR_REDIR_PAUSED,                                       "ERROR_REDIR_PAUSED",
		"The specified printer or disk device has been paused.",
		"The specified printer or disk device has been paused."
	},
#endif // #ifdef ERROR_REDIR_PAUSED
#ifdef ERROR_FILE_EXISTS
	{	ERROR_FILE_EXISTS,                                        "ERROR_FILE_EXISTS",
		"The file exists.",
		"The file exists."
	},
#endif // #ifdef ERROR_FILE_EXISTS
#ifdef ERROR_CANNOT_MAKE
	{	ERROR_CANNOT_MAKE,                                        "ERROR_CANNOT_MAKE",
		"The directory or file cannot be created.",
		"The directory or file cannot be created."
	},
#endif // #ifdef ERROR_CANNOT_MAKE
#ifdef ERROR_FAIL_I24
	{	ERROR_FAIL_I24,                                           "ERROR_FAIL_I24",
		"Fail on INT 24.",
		"Fail on INT 24."
	},
#endif // #ifdef ERROR_FAIL_I24
#ifdef ERROR_OUT_OF_STRUCTURES
	{	ERROR_OUT_OF_STRUCTURES,                                  "ERROR_OUT_OF_STRUCTURES",
		"Storage to process this request is not available.",
		"Storage to process this request is not available."
	},
#endif // #ifdef ERROR_OUT_OF_STRUCTURES
#ifdef ERROR_ALREADY_ASSIGNED
	{	ERROR_ALREADY_ASSIGNED,                                   "ERROR_ALREADY_ASSIGNED",
		"The local device name is already in use.",
		"The local device name is already in use."
	},
#endif // #ifdef ERROR_ALREADY_ASSIGNED
#ifdef ERROR_INVALID_PASSWORD
	{	ERROR_INVALID_PASSWORD,                                   "ERROR_INVALID_PASSWORD",
		"The specified network password is not correct.",
		"The specified network password is not correct."
	},
#endif // #ifdef ERROR_INVALID_PASSWORD
#ifdef ERROR_INVALID_PARAMETER
	{	ERROR_INVALID_PARAMETER,                                  "ERROR_INVALID_PARAMETER",
		"The parameter is incorrect.",
		"The parameter is incorrect."
	},
#endif // #ifdef ERROR_INVALID_PARAMETER
#ifdef ERROR_NET_WRITE_FAULT
	{	ERROR_NET_WRITE_FAULT,                                    "ERROR_NET_WRITE_FAULT",
		"A write fault occurred on the network.",
		"A write fault occurred on the network."
	},
#endif // #ifdef ERROR_NET_WRITE_FAULT
#ifdef ERROR_NO_PROC_SLOTS
	{	ERROR_NO_PROC_SLOTS,                                      "ERROR_NO_PROC_SLOTS",
		"The system cannot start another process at this time.",
		"The system cannot start another process at this time."
	},
#endif // #ifdef ERROR_NO_PROC_SLOTS
#ifdef ERROR_TOO_MANY_SEMAPHORES
	{	ERROR_TOO_MANY_SEMAPHORES,                                "ERROR_TOO_MANY_SEMAPHORES",
		"Cannot create another system semaphore.",
		"Cannot create another system semaphore."
	},
#endif // #ifdef ERROR_TOO_MANY_SEMAPHORES
#ifdef ERROR_EXCL_SEM_ALREADY_OWNED
	{	ERROR_EXCL_SEM_ALREADY_OWNED,                             "ERROR_EXCL_SEM_ALREADY_OWNED",
		"The exclusive semaphore is owned by another process.",
		"The exclusive semaphore is owned by another process."
	},
#endif // #ifdef ERROR_EXCL_SEM_ALREADY_OWNED
#ifdef ERROR_SEM_IS_SET
	{	ERROR_SEM_IS_SET,                                         "ERROR_SEM_IS_SET",
		"The semaphore is set and cannot be closed.",
		"The semaphore is set and cannot be closed."
	},
#endif // #ifdef ERROR_SEM_IS_SET
#ifdef ERROR_TOO_MANY_SEM_REQUESTS
	{	ERROR_TOO_MANY_SEM_REQUESTS,                              "ERROR_TOO_MANY_SEM_REQUESTS",
		"The semaphore cannot be set again.",
		"The semaphore cannot be set again."
	},
#endif // #ifdef ERROR_TOO_MANY_SEM_REQUESTS
#ifdef ERROR_INVALID_AT_INTERRUPT_TIME
	{	ERROR_INVALID_AT_INTERRUPT_TIME,                          "ERROR_INVALID_AT_INTERRUPT_TIME",
		"Cannot request exclusive semaphores at interrupt time.",
		"Cannot request exclusive semaphores at interrupt time."
	},
#endif // #ifdef ERROR_INVALID_AT_INTERRUPT_TIME
#ifdef ERROR_SEM_OWNER_DIED
	{	ERROR_SEM_OWNER_DIED,                                     "ERROR_SEM_OWNER_DIED",
		"The previous ownership of this semaphore has ended.",
		"The previous ownership of this semaphore has ended."
	},
#endif // #ifdef ERROR_SEM_OWNER_DIED
#ifdef ERROR_SEM_USER_LIMIT
	{	ERROR_SEM_USER_LIMIT,                                     "ERROR_SEM_USER_LIMIT",
		"Insert the diskette for drive.",
		"The diskette must be inserted in the drive."
	},
#endif // #ifdef ERROR_SEM_USER_LIMIT
#ifdef ERROR_DISK_CHANGE
	{	ERROR_DISK_CHANGE,                                        "ERROR_DISK_CHANGE",
		"The program stopped because an alternate diskette was not inserted.",
		"The program stopped because an alternate diskette was not inserted."
	},
#endif // #ifdef ERROR_DISK_CHANGE
#ifdef ERROR_DRIVE_LOCKED
	{	ERROR_DRIVE_LOCKED,                                       "ERROR_DRIVE_LOCKED",
		"The disk is in use or locked by another process.",
		"The disk is in use or locked by another process."
	},
#endif // #ifdef ERROR_DRIVE_LOCKED
#ifdef ERROR_BROKEN_PIPE
	{	ERROR_BROKEN_PIPE,                                        "ERROR_BROKEN_PIPE",
		"The pipe has been ended.",
		"The pipe has been ended."
	},
#endif // #ifdef ERROR_BROKEN_PIPE
#ifdef ERROR_OPEN_FAILED
	{	ERROR_OPEN_FAILED,                                        "ERROR_OPEN_FAILED",
		"The system cannot open the device or file specified.",
		"The system cannot open the device or file specified."
	},
#endif // #ifdef ERROR_OPEN_FAILED
#ifdef ERROR_BUFFER_OVERFLOW
	{	ERROR_BUFFER_OVERFLOW,                                    "ERROR_BUFFER_OVERFLOW",
		"The file name is too long.",
		"The file name is too long."
	},
#endif // #ifdef ERROR_BUFFER_OVERFLOW
#ifdef ERROR_DISK_FULL
	{	ERROR_DISK_FULL,                                          "ERROR_DISK_FULL",
		"There is not enough space on the disk.",
		"There is not enough space on the disk."
	},
#endif // #ifdef ERROR_DISK_FULL
#ifdef ERROR_NO_MORE_SEARCH_HANDLES
	{	ERROR_NO_MORE_SEARCH_HANDLES,                             "ERROR_NO_MORE_SEARCH_HANDLES",
		"No more internal file identifiers available.",
		"No more internal file identifiers available."
	},
#endif // #ifdef ERROR_NO_MORE_SEARCH_HANDLES
#ifdef ERROR_INVALID_TARGET_HANDLE
	{	ERROR_INVALID_TARGET_HANDLE,                              "ERROR_INVALID_TARGET_HANDLE",
		"The target internal file identifier is incorrect.",
		"The target internal file identifier is incorrect."
	},
#endif // #ifdef ERROR_INVALID_TARGET_HANDLE
#ifdef ERROR_INVALID_CATEGORY
	{	ERROR_INVALID_CATEGORY,                                   "ERROR_INVALID_CATEGORY",
		"The IOCTL call made by the application program is not correct.",
		"The IOCTL call made by the application program is not correct."
	},
#endif // #ifdef ERROR_INVALID_CATEGORY
#ifdef ERROR_INVALID_VERIFY_SWITCH
	{	ERROR_INVALID_VERIFY_SWITCH,                              "ERROR_INVALID_VERIFY_SWITCH",
		"The verify-on-write switch parameter value is not correct.",
		"The verify-on-write switch parameter value is not correct."
	},
#endif // #ifdef ERROR_INVALID_VERIFY_SWITCH
#ifdef ERROR_BAD_DRIVER_LEVEL
	{	ERROR_BAD_DRIVER_LEVEL,                                   "ERROR_BAD_DRIVER_LEVEL",
		"The system does not support the command requested.",
		"The system does not support the command requested."
	},
#endif // #ifdef ERROR_BAD_DRIVER_LEVEL
#ifdef ERROR_CALL_NOT_IMPLEMENTED
	{	ERROR_CALL_NOT_IMPLEMENTED,                               "ERROR_CALL_NOT_IMPLEMENTED",
		"This function is not supported on this system.",
		"This function is not supported on this system."
	},
#endif // #ifdef ERROR_CALL_NOT_IMPLEMENTED
#ifdef ERROR_SEM_TIMEOUT
	{	ERROR_SEM_TIMEOUT,                                        "ERROR_SEM_TIMEOUT",
		"The semaphore timeout period has expired.",
		"The semaphore timeout period has expired."
	},
#endif // #ifdef ERROR_SEM_TIMEOUT
#ifdef ERROR_INSUFFICIENT_BUFFER
	{	ERROR_INSUFFICIENT_BUFFER,                                "ERROR_INSUFFICIENT_BUFFER",
		"The data area passed to a system call is too small.",
		"The data area passed to a system call is too small."
	},
#endif // #ifdef ERROR_INSUFFICIENT_BUFFER
#ifdef ERROR_INVALID_NAME
	{	ERROR_INVALID_NAME,                                       "ERROR_INVALID_NAME",
		"The filename, directory name, or volume label syntax is incorrect.",
		"The filename, directory name, or volume label syntax is incorrect."
	},
#endif // #ifdef ERROR_INVALID_NAME
#ifdef ERROR_INVALID_LEVEL
	{	ERROR_INVALID_LEVEL,                                      "ERROR_INVALID_LEVEL",
		"The system call level is not correct.",
		"The system call level is not correct."
	},
#endif // #ifdef ERROR_INVALID_LEVEL
#ifdef ERROR_NO_VOLUME_LABEL
	{	ERROR_NO_VOLUME_LABEL,                                    "ERROR_NO_VOLUME_LABEL",
		"The disk has no volume label.",
		"The disk has no volume label."
	},
#endif // #ifdef ERROR_NO_VOLUME_LABEL
#ifdef ERROR_MOD_NOT_FOUND
	{	ERROR_MOD_NOT_FOUND,                                      "ERROR_MOD_NOT_FOUND",
		"The specified module could not be found.",
		"The specified module could not be found."
	},
#endif // #ifdef ERROR_MOD_NOT_FOUND
#ifdef ERROR_PROC_NOT_FOUND
	{	ERROR_PROC_NOT_FOUND,                                     "ERROR_PROC_NOT_FOUND",
		"The specified procedure could not be found.",
		"The specified procedure could not be found."
	},
#endif // #ifdef ERROR_PROC_NOT_FOUND
#ifdef ERROR_WAIT_NO_CHILDREN
	{	ERROR_WAIT_NO_CHILDREN,                                   "ERROR_WAIT_NO_CHILDREN",
		"There are no child processes to wait for.",
		"There are no child processes to wait for."
	},
#endif // #ifdef ERROR_WAIT_NO_CHILDREN
#ifdef ERROR_CHILD_NOT_COMPLETE
	{	ERROR_CHILD_NOT_COMPLETE,                                 "ERROR_CHILD_NOT_COMPLETE",
		"The application cannot be run in Win32 mode.",
		"The application cannot be run in Win32 mode."
	},
#endif // #ifdef ERROR_CHILD_NOT_COMPLETE
#ifdef ERROR_DIRECT_ACCESS_HANDLE
	{	ERROR_DIRECT_ACCESS_HANDLE,                               "ERROR_DIRECT_ACCESS_HANDLE",
		"Attempt to use a file handle to an open disk partition for an operation other than raw disk I/O.",
		"Attempt to use a file handle to an open disk partition for an operation other than raw disk I/O."
	},
#endif // #ifdef ERROR_DIRECT_ACCESS_HANDLE
#ifdef ERROR_NEGATIVE_SEEK
	{	ERROR_NEGATIVE_SEEK,                                      "ERROR_NEGATIVE_SEEK",
		"An attempt was made to move the file pointer before the beginning of the file.",
		"An attempt was made to move the file pointer before the beginning of the file."
	},
#endif // #ifdef ERROR_NEGATIVE_SEEK
#ifdef ERROR_SEEK_ON_DEVICE
	{	ERROR_SEEK_ON_DEVICE,                                     "ERROR_SEEK_ON_DEVICE",
		"The file pointer cannot be set on the specified device or file.",
		"The file pointer cannot be set on the specified device or file."
	},
#endif // #ifdef ERROR_SEEK_ON_DEVICE
#ifdef ERROR_IS_JOIN_TARGET
	{	ERROR_IS_JOIN_TARGET,                                     "ERROR_IS_JOIN_TARGET",
		"A JOIN or SUBST command cannot be used for a drive that contains previously joined drives.",
		"A JOIN or SUBST command cannot be used for a drive that contains previously joined drives."
	},
#endif // #ifdef ERROR_IS_JOIN_TARGET
#ifdef ERROR_IS_JOINED
	{	ERROR_IS_JOINED,                                          "ERROR_IS_JOINED",
		"An attempt was made to use a JOIN or SUBST command on a drive that has already been joined.",
		"An attempt was made to use a JOIN or SUBST command on a drive that has already been joined."
	},
#endif // #ifdef ERROR_IS_JOINED
#ifdef ERROR_IS_SUBSTED
	{	ERROR_IS_SUBSTED,                                         "ERROR_IS_SUBSTED",
		"An attempt was made to use a JOIN or SUBST command on a drive that has already been substituted.",
		"An attempt was made to use a JOIN or SUBST command on a drive that has already been substituted."
	},
#endif // #ifdef ERROR_IS_SUBSTED
#ifdef ERROR_NOT_JOINED
	{	ERROR_NOT_JOINED,                                         "ERROR_NOT_JOINED",
		"The system tried to delete the JOIN of a drive that is not joined.",
		"The system tried to delete the JOIN of a drive that is not joined."
	},
#endif // #ifdef ERROR_NOT_JOINED
#ifdef ERROR_NOT_SUBSTED
	{	ERROR_NOT_SUBSTED,                                        "ERROR_NOT_SUBSTED",
		"The system tried to delete the substitution of a drive that is not substituted.",
		"The system tried to delete the substitution of a drive that is not substituted."
	},
#endif // #ifdef ERROR_NOT_SUBSTED
#ifdef ERROR_JOIN_TO_JOIN
	{	ERROR_JOIN_TO_JOIN,                                       "ERROR_JOIN_TO_JOIN",
		"The system tried to join a drive to a directory on a joined drive.",
		"The system tried to join a drive to a directory on a joined drive."
	},
#endif // #ifdef ERROR_JOIN_TO_JOIN
#ifdef ERROR_SUBST_TO_SUBST
	{	ERROR_SUBST_TO_SUBST,                                     "ERROR_SUBST_TO_SUBST",
		"The system tried to substitute a drive to a directory on a substituted drive.",
		"The system tried to substitute a drive to a directory on a substituted drive."
	},
#endif // #ifdef ERROR_SUBST_TO_SUBST
#ifdef ERROR_JOIN_TO_SUBST
	{	ERROR_JOIN_TO_SUBST,                                      "ERROR_JOIN_TO_SUBST",
		"The system tried to join a drive to a directory on a substituted drive.",
		"The system tried to join a drive to a directory on a substituted drive."
	},
#endif // #ifdef ERROR_JOIN_TO_SUBST
#ifdef ERROR_SUBST_TO_JOIN
	{	ERROR_SUBST_TO_JOIN,                                      "ERROR_SUBST_TO_JOIN",
		"The system tried to SUBST a drive to a directory on a joined drive.",
		"The system tried to SUBST a drive to a directory on a joined drive."
	},
#endif // #ifdef ERROR_SUBST_TO_JOIN
#ifdef ERROR_BUSY_DRIVE
	{	ERROR_BUSY_DRIVE,                                         "ERROR_BUSY_DRIVE",
		"The system cannot perform a JOIN or SUBST at this time.",
		"The system cannot perform a JOIN or SUBST at this time."
	},
#endif // #ifdef ERROR_BUSY_DRIVE
#ifdef ERROR_SAME_DRIVE
	{	ERROR_SAME_DRIVE,                                         "ERROR_SAME_DRIVE",
		"The system cannot join or substitute a drive to or for a directory on the same drive.",
		"The system cannot join or substitute a drive to or for a directory on the same drive."
	},
#endif // #ifdef ERROR_SAME_DRIVE
#ifdef ERROR_DIR_NOT_ROOT
	{	ERROR_DIR_NOT_ROOT,                                       "ERROR_DIR_NOT_ROOT",
		"The directory is not a subdirectory of the root directory.",
		"The directory is not a subdirectory of the root directory."
	},
#endif // #ifdef ERROR_DIR_NOT_ROOT
#ifdef ERROR_DIR_NOT_EMPTY
	{	ERROR_DIR_NOT_EMPTY,                                      "ERROR_DIR_NOT_EMPTY",
		"The directory is not empty.",
		"The directory is not empty."
	},
#endif // #ifdef ERROR_DIR_NOT_EMPTY
#ifdef ERROR_IS_SUBST_PATH
	{	ERROR_IS_SUBST_PATH,                                      "ERROR_IS_SUBST_PATH",
		"The path specified is being used in a substitute.",
		"The path specified is being used in a substitute."
	},
#endif // #ifdef ERROR_IS_SUBST_PATH
#ifdef ERROR_IS_JOIN_PATH
	{	ERROR_IS_JOIN_PATH,                                       "ERROR_IS_JOIN_PATH",
		"Not enough resources are available to process this command.",
		"Not enough resources are available to process this command."
	},
#endif // #ifdef ERROR_IS_JOIN_PATH
#ifdef ERROR_PATH_BUSY
	{	ERROR_PATH_BUSY,                                          "ERROR_PATH_BUSY",
		"The path specified cannot be used at this time.",
		"The path specified cannot be used at this time."
	},
#endif // #ifdef ERROR_PATH_BUSY
#ifdef ERROR_IS_SUBST_TARGET
	{	ERROR_IS_SUBST_TARGET,                                    "ERROR_IS_SUBST_TARGET",
		"An attempt was made to join or substitute a drive for which a directory on the drive is the target of a previous substitute.",
		"An attempt was made to join or substitute a drive for which a directory on the drive is the target of a previous substitute."
	},
#endif // #ifdef ERROR_IS_SUBST_TARGET
#ifdef ERROR_SYSTEM_TRACE
	{	ERROR_SYSTEM_TRACE,                                       "ERROR_SYSTEM_TRACE",
		"System trace information was not specified in your CONFIG.SYS file, or tracing is disallowed.",
		"System trace information was not specified in your CONFIG.SYS file, or tracing is disallowed."
	},
#endif // #ifdef ERROR_SYSTEM_TRACE
#ifdef ERROR_INVALID_EVENT_COUNT
	{	ERROR_INVALID_EVENT_COUNT,                                "ERROR_INVALID_EVENT_COUNT",
		"The number of specified semaphore events for DosMuxSemWait is not correct.",
		"The number of specified semaphore events for DosMuxSemWait is not correct."
	},
#endif // #ifdef ERROR_INVALID_EVENT_COUNT
#ifdef ERROR_TOO_MANY_MUXWAITERS
	{	ERROR_TOO_MANY_MUXWAITERS,                                "ERROR_TOO_MANY_MUXWAITERS",
		"DosMuxSemWait did not execute; too many semaphores are already set.",
		"DosMuxSemWait did not execute; too many semaphores are already set."
	},
#endif // #ifdef ERROR_TOO_MANY_MUXWAITERS
#ifdef ERROR_INVALID_LIST_FORMAT
	{	ERROR_INVALID_LIST_FORMAT,                                "ERROR_INVALID_LIST_FORMAT",
		"The DosMuxSemWait list is not correct.",
		"The DosMuxSemWait list is not correct."
	},
#endif // #ifdef ERROR_INVALID_LIST_FORMAT
#ifdef ERROR_LABEL_TOO_LONG
	{	ERROR_LABEL_TOO_LONG,                                     "ERROR_LABEL_TOO_LONG",
		"The volume label you entered exceeds the label character limit of the target file system.",
		"The volume label you entered exceeds the label character limit of the target file system."
	},
#endif // #ifdef ERROR_LABEL_TOO_LONG
#ifdef ERROR_TOO_MANY_TCBS
	{	ERROR_TOO_MANY_TCBS,                                      "ERROR_TOO_MANY_TCBS",
		"Cannot create another thread.",
		"Cannot create another thread."
	},
#endif // #ifdef ERROR_TOO_MANY_TCBS
#ifdef ERROR_SIGNAL_REFUSED
	{	ERROR_SIGNAL_REFUSED,                                     "ERROR_SIGNAL_REFUSED",
		"The recipient process has refused the signal.",
		"The recipient process has refused the signal."
	},
#endif // #ifdef ERROR_SIGNAL_REFUSED
#ifdef ERROR_DISCARDED
	{	ERROR_DISCARDED,                                          "ERROR_DISCARDED",
		"The segment is already discarded and cannot be locked.",
		"The segment is already discarded and cannot be locked."
	},
#endif // #ifdef ERROR_DISCARDED
#ifdef ERROR_NOT_LOCKED
	{	ERROR_NOT_LOCKED,                                         "ERROR_NOT_LOCKED",
		"The segment is already unlocked.",
		"The segment is already unlocked."
	},
#endif // #ifdef ERROR_NOT_LOCKED
#ifdef ERROR_BAD_THREADID_ADDR
	{	ERROR_BAD_THREADID_ADDR,                                  "ERROR_BAD_THREADID_ADDR",
		"The address for the thread ID is not correct.",
		"The address for the thread ID is not correct."
	},
#endif // #ifdef ERROR_BAD_THREADID_ADDR
#ifdef ERROR_BAD_ARGUMENTS
	{	ERROR_BAD_ARGUMENTS,                                      "ERROR_BAD_ARGUMENTS",
		"The argument string passed to DosExecPgm is not correct.",
		"The argument string passed to DosExecPgm is not correct."
	},
#endif // #ifdef ERROR_BAD_ARGUMENTS
#ifdef ERROR_BAD_PATHNAME
	{	ERROR_BAD_PATHNAME,                                       "ERROR_BAD_PATHNAME",
		"The specified path is invalid.",
		"The specified path is invalid."
	},
#endif // #ifdef ERROR_BAD_PATHNAME
#ifdef ERROR_SIGNAL_PENDING
	{	ERROR_SIGNAL_PENDING,                                     "ERROR_SIGNAL_PENDING",
		"A signal is already pending.",
		"A signal is already pending."
	},
#endif // #ifdef ERROR_SIGNAL_PENDING
#ifdef ERROR_MAX_THRDS_REACHED
	{	ERROR_MAX_THRDS_REACHED,                                  "ERROR_MAX_THRDS_REACHED",
		"No more threads can be created in the system.",
		"No more threads can be created in the system."
	},
#endif // #ifdef ERROR_MAX_THRDS_REACHED
#ifdef ERROR_LOCK_FAILED
	{	ERROR_LOCK_FAILED,                                        "ERROR_LOCK_FAILED",
		"Unable to lock a region of a file.",
		"Unable to lock a region of a file."
	},
#endif // #ifdef ERROR_LOCK_FAILED
#ifdef ERROR_BUSY
	{	ERROR_BUSY,                                               "ERROR_BUSY",
		"The requested resource is in use.",
		"The requested resource is in use."
	},
#endif // #ifdef ERROR_BUSY
#ifdef ERROR_CANCEL_VIOLATION
	{	ERROR_CANCEL_VIOLATION,                                   "ERROR_CANCEL_VIOLATION",
		"A lock request was not outstanding for the supplied cancel region.",
		"A lock request was not outstanding for the supplied cancel region."
	},
#endif // #ifdef ERROR_CANCEL_VIOLATION
#ifdef ERROR_ATOMIC_LOCKS_NOT_SUPPORTED
	{	ERROR_ATOMIC_LOCKS_NOT_SUPPORTED,                         "ERROR_ATOMIC_LOCKS_NOT_SUPPORTED",
		"The file system does not support atomic changes to the lock type.",
		"The file system does not support atomic changes to the lock type."
	},
#endif // #ifdef ERROR_ATOMIC_LOCKS_NOT_SUPPORTED
#ifdef ERROR_INVALID_SEGMENT_NUMBER
	{	ERROR_INVALID_SEGMENT_NUMBER,                             "ERROR_INVALID_SEGMENT_NUMBER",
		"The system detected a segment number that was not correct.",
		"The system detected a segment number that was not correct."
	},
#endif // #ifdef ERROR_INVALID_SEGMENT_NUMBER
#ifdef ERROR_INVALID_ORDINAL
	{	ERROR_INVALID_ORDINAL,                                    "ERROR_INVALID_ORDINAL",
		"The operating system cannot run the program.",
		"The operating system cannot run the program."
	},
#endif // #ifdef ERROR_INVALID_ORDINAL
#ifdef ERROR_ALREADY_EXISTS
	{	ERROR_ALREADY_EXISTS,                                     "ERROR_ALREADY_EXISTS",
		"Cannot create a file when that file already exists.",
		"Cannot create a file when that file already exists."
	},
#endif // #ifdef ERROR_ALREADY_EXISTS
#ifdef ERROR_INVALID_FLAG_NUMBER
	{	ERROR_INVALID_FLAG_NUMBER,                                "ERROR_INVALID_FLAG_NUMBER",
		"The flag passed is not correct.",
		"The flag passed is not correct."
	},
#endif // #ifdef ERROR_INVALID_FLAG_NUMBER
#ifdef ERROR_SEM_NOT_FOUND
	{	ERROR_SEM_NOT_FOUND,                                      "ERROR_SEM_NOT_FOUND",
		"The specified system semaphore name was not found.",
		"The specified system semaphore name was not found."
	},
#endif // #ifdef ERROR_SEM_NOT_FOUND
#ifdef ERROR_INVALID_STARTING_CODESEG
	{	ERROR_INVALID_STARTING_CODESEG,                           "ERROR_INVALID_STARTING_CODESEG",
		"The operating system cannot run the program.",
		"The operating system cannot run the requested resource (invalid starting code seg)."
	},
#endif // #ifdef ERROR_INVALID_STARTING_CODESEG
#ifdef ERROR_INVALID_STACKSEG
	{	ERROR_INVALID_STACKSEG,                                   "ERROR_INVALID_STACKSEG",
		"The operating system cannot run the program.",
		"The operating system cannot run the requested resource (invalid starting stack seg)."
	},
#endif // #ifdef ERROR_INVALID_STACKSEG
#ifdef ERROR_INVALID_MODULETYPE
	{	ERROR_INVALID_MODULETYPE,                                 "ERROR_INVALID_MODULETYPE",
		"The operating system cannot run the program.",
		"The operating system cannot run the requested resource (invalid module type)."
	},
#endif // #ifdef ERROR_INVALID_MODULETYPE
#ifdef ERROR_INVALID_EXE_SIGNATURE
	{	ERROR_INVALID_EXE_SIGNATURE,                              "ERROR_INVALID_EXE_SIGNATURE",
		"Cannot run EXE file in Win32 mode.",
		"Cannot run EXE file in Win32 mode."
	},
#endif // #ifdef ERROR_INVALID_EXE_SIGNATURE
#ifdef ERROR_EXE_MARKED_INVALID
	{	ERROR_EXE_MARKED_INVALID,                                 "ERROR_EXE_MARKED_INVALID",
		"The operating system cannot run the program.",
		"The operating system cannot run the requested resource (EXE marked as invalid)."
	},
#endif // #ifdef ERROR_EXE_MARKED_INVALID
#ifdef ERROR_BAD_EXE_FORMAT
	{	ERROR_BAD_EXE_FORMAT,                                     "ERROR_BAD_EXE_FORMAT",
		"The file has a bad EXE format --- not a valid Win32 application.",
		"The file has a bad EXE format --- not a valid Win32 application."
	},
#endif // #ifdef ERROR_BAD_EXE_FORMAT
#ifdef ERROR_ITERATED_DATA_EXCEEDS_64k
	{	ERROR_ITERATED_DATA_EXCEEDS_64k,                          "ERROR_ITERATED_DATA_EXCEEDS_64k",
		"The operating system cannot run the requested resource (iterated data exceeds 64K).",
		"The operating system cannot run the requested resource (iterated data exceeds 64K)."
	},
#endif // #ifdef ERROR_ITERATED_DATA_EXCEEDS_64k
#ifdef ERROR_INVALID_MINALLOCSIZE
	{	ERROR_INVALID_MINALLOCSIZE,                               "ERROR_INVALID_MINALLOCSIZE",
		"The operating system cannot run the requested resource (invalid min alloc size).",
		"The operating system cannot run the requested resource (invalid min alloc size)."
	},
#endif // #ifdef ERROR_INVALID_MINALLOCSIZE
#ifdef ERROR_DYNLINK_FROM_INVALID_RING
	{	ERROR_DYNLINK_FROM_INVALID_RING,                          "ERROR_DYNLINK_FROM_INVALID_RING",
		"The operating system cannot run this application program.",
		"The operating system cannot run this application program."
	},
#endif // #ifdef ERROR_DYNLINK_FROM_INVALID_RING
#ifdef ERROR_IOPL_NOT_ENABLED
	{	ERROR_IOPL_NOT_ENABLED,                                   "ERROR_IOPL_NOT_ENABLED",
		"The operating system is not presently configured to run this application.",
		"The operating system is not presently configured to run this application."
	},
#endif // #ifdef ERROR_IOPL_NOT_ENABLED
#ifdef ERROR_INVALID_SEGDPL
	{	ERROR_INVALID_SEGDPL,                                     "ERROR_INVALID_SEGDPL",
		"The operating system cannot run the requested resource (invalid seg DPL).",
		"The operating system cannot run the requested resource (invalid seg DPL)."
	},
#endif // #ifdef ERROR_INVALID_SEGDPL
#ifdef ERROR_AUTODATASEG_EXCEEDS_64k
	{	ERROR_AUTODATASEG_EXCEEDS_64k,                            "ERROR_AUTODATASEG_EXCEEDS_64k",
		"The operating system cannot run this application program.",
		"The operating system cannot run this application program."
	},
#endif // #ifdef ERROR_AUTODATASEG_EXCEEDS_64k
#ifdef ERROR_RING2SEG_MUST_BE_MOVABLE
	{	ERROR_RING2SEG_MUST_BE_MOVABLE,                           "ERROR_RING2SEG_MUST_BE_MOVABLE",
		"The code segment cannot be greater than or equal to 64K.",
		"The code segment cannot be greater than or equal to 64K."
	},
#endif // #ifdef ERROR_RING2SEG_MUST_BE_MOVABLE
#ifdef ERROR_RELOC_CHAIN_XEEDS_SEGLIM
	{	ERROR_RELOC_CHAIN_XEEDS_SEGLIM,                           "ERROR_RELOC_CHAIN_XEEDS_SEGLIM",
		"The operating system cannot run the requested resource (reloc chain needs seg lim).",
		"The operating system cannot run the requested resource (reloc chain needs seg lim)."
	},
#endif // #ifdef ERROR_RELOC_CHAIN_XEEDS_SEGLIM
#ifdef ERROR_INFLOOP_IN_RELOC_CHAIN
	{	ERROR_INFLOOP_IN_RELOC_CHAIN,                             "ERROR_INFLOOP_IN_RELOC_CHAIN",
		"The operating system cannot run the requested resource (INFLOOP in reloc chain).",
		"The operating system cannot run the requested resource (INFLOOP in reloc chain)."
	},
#endif // #ifdef ERROR_INFLOOP_IN_RELOC_CHAIN
#ifdef ERROR_ENVVAR_NOT_FOUND
	{	ERROR_ENVVAR_NOT_FOUND,                                   "ERROR_ENVVAR_NOT_FOUND",
		"The system could not find the environment option that was entered.",
		"The system could not find the environment option that was entered."
	},
#endif // #ifdef ERROR_ENVVAR_NOT_FOUND
#ifdef ERROR_NO_SIGNAL_SENT
	{	ERROR_NO_SIGNAL_SENT,                                     "ERROR_NO_SIGNAL_SENT",
		"No process in the command subtree has a signal handler.",
		"No process in the command subtree has a signal handler."
	},
#endif // #ifdef ERROR_NO_SIGNAL_SENT
#ifdef ERROR_FILENAME_EXCED_RANGE
	{	ERROR_FILENAME_EXCED_RANGE,                               "ERROR_FILENAME_EXCED_RANGE",
		"The filename or extension is too long.",
		"The filename or extension is too long."
	},
#endif // #ifdef ERROR_FILENAME_EXCED_RANGE
#ifdef ERROR_RING2_STACK_IN_USE
	{	ERROR_RING2_STACK_IN_USE,                                 "ERROR_RING2_STACK_IN_USE",
		"The ring 2 stack is in use.",
		"The ring 2 stack is in use."
	},
#endif // #ifdef ERROR_RING2_STACK_IN_USE
#ifdef ERROR_META_EXPANSION_TOO_LONG
	{	ERROR_META_EXPANSION_TOO_LONG,                            "ERROR_META_EXPANSION_TOO_LONG",
		"The global filename characters, * or ?, are entered incorrectly or too many global filename characters are specified.",
		"The global filename characters, * or ?, are entered incorrectly or too many global filename characters are specified."
	},
#endif // #ifdef ERROR_META_EXPANSION_TOO_LONG
#ifdef ERROR_INVALID_SIGNAL_NUMBER
	{	ERROR_INVALID_SIGNAL_NUMBER,                              "ERROR_INVALID_SIGNAL_NUMBER",
		"The signal being posted is not correct.",
		"The signal being posted is not correct."
	},
#endif // #ifdef ERROR_INVALID_SIGNAL_NUMBER
#ifdef ERROR_THREAD_1_INACTIVE
	{	ERROR_THREAD_1_INACTIVE,                                  "ERROR_THREAD_1_INACTIVE",
		"The signal handler cannot be set.",
		"The signal handler cannot be set."
	},
#endif // #ifdef ERROR_THREAD_1_INACTIVE
#ifdef ERROR_LOCKED
	{	ERROR_LOCKED,                                             "ERROR_LOCKED",
		"The segment is locked and cannot be reallocated.",
		"The segment is locked and cannot be reallocated."
	},
#endif // #ifdef ERROR_LOCKED
#ifdef ERROR_TOO_MANY_MODULES
	{	ERROR_TOO_MANY_MODULES,                                   "ERROR_TOO_MANY_MODULES",
		"Too many dynamic-link modules are attached to this program or dynamic-link module.",
		"Too many dynamic-link modules are attached to this program or dynamic-link module."
	},
#endif // #ifdef ERROR_TOO_MANY_MODULES
#ifdef ERROR_NESTING_NOT_ALLOWED
	{	ERROR_NESTING_NOT_ALLOWED,                                "ERROR_NESTING_NOT_ALLOWED",
		"Cannot nest calls to LoadModule.",
		"Cannot nest calls to LoadModule."
	},
#endif // #ifdef ERROR_NESTING_NOT_ALLOWED
#ifdef ERROR_EXE_MACHINE_TYPE_MISMATCH
	{	ERROR_EXE_MACHINE_TYPE_MISMATCH,                          "ERROR_EXE_MACHINE_TYPE_MISMATCH",
		"The image file is valid, but is for a machine type other than the current machine.",
		"The image file is valid, but is for a machine type other than the current machine."
	},
#endif // #ifdef ERROR_EXE_MACHINE_TYPE_MISMATCH
#ifdef ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY
	{	ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY,                    "ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY",
		"The image file %1 is signed, unable to modify.",
		"The image file %1 is signed, unable to modify."
	},
#endif // #ifdef ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY
#ifdef ERRO_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY
	{	ERRO_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY,              "ERRO_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY",
		"The image file %1 is strong signed, unable to modify.",
		"The image file %1 is strong signed, unable to modify."
	},
#endif // #ifdef ERRO_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY
#ifdef ERROR_BAD_PIPE
	{	ERROR_BAD_PIPE,                                           "ERROR_BAD_PIPE",
		"The pipe state is invalid.",
		"The pipe state is invalid."
	},
#endif // #ifdef ERROR_BAD_PIPE
#ifdef ERROR_PIPE_BUSY
	{	ERROR_PIPE_BUSY,                                          "ERROR_PIPE_BUSY",
		"All pipe instances are busy.",
		"All pipe instances are busy."
	},
#endif // #ifdef ERROR_PIPE_BUSY
#ifdef ERROR_NO_DATA
	{	ERROR_NO_DATA,                                            "ERROR_NO_DATA",
		"The pipe is being closed.",
		"The pipe is being closed."
	},
#endif // #ifdef ERROR_NO_DATA
#ifdef ERROR_PIPE_NOT_CONNECTED
	{	ERROR_PIPE_NOT_CONNECTED,                                 "ERROR_PIPE_NOT_CONNECTED",
		"No process is on the other end of the pipe.",
		"No process is on the other end of the pipe."
	},
#endif // #ifdef ERROR_PIPE_NOT_CONNECTED
#ifdef ERROR_MORE_DATA
	{	ERROR_MORE_DATA,                                          "ERROR_MORE_DATA",
		"More data is available.",
		"More data is available."
	},
#endif // #ifdef ERROR_MORE_DATA
#ifdef ERROR_VC_DISCONNECTED
	{	ERROR_VC_DISCONNECTED,                                    "ERROR_VC_DISCONNECTED",
		"The session was cancelled.",
		"The session was cancelled."
	},
#endif // #ifdef ERROR_VC_DISCONNECTED
#ifdef ERROR_INVALID_EA_NAME
	{	ERROR_INVALID_EA_NAME,                                    "ERROR_INVALID_EA_NAME",
		"The specified extended attribute name was invalid.",
		"The specified extended attribute name was invalid."
	},
#endif // #ifdef ERROR_INVALID_EA_NAME
#ifdef ERROR_EA_LIST_INCONSISTENT
	{	ERROR_EA_LIST_INCONSISTENT,                               "ERROR_EA_LIST_INCONSISTENT",
		"The extended attributes are inconsistent.",
		"The extended attributes are inconsistent."
	},
#endif // #ifdef ERROR_EA_LIST_INCONSISTENT
#ifdef WAIT_TIMEOUT
	{	WAIT_TIMEOUT,                                             "WAIT_TIMEOUT",
		"The wait operation timed out.",
		"The wait operation timed out."
	},
#endif // #ifdef WAIT_TIMEOUT
#ifdef ERROR_NO_MORE_ITEMS
	{	ERROR_NO_MORE_ITEMS,                                      "ERROR_NO_MORE_ITEMS",
		"No more data is available.",
		"No more data is available."
	},
#endif // #ifdef ERROR_NO_MORE_ITEMS
#ifdef ERROR_CANNOT_COPY
	{	ERROR_CANNOT_COPY,                                        "ERROR_CANNOT_COPY",
		"The copy functions cannot be used.",
		"The copy functions cannot be used."
	},
#endif // #ifdef ERROR_CANNOT_COPY
#ifdef ERROR_DIRECTORY
	{	ERROR_DIRECTORY,                                          "ERROR_DIRECTORY",
		"The directory name is invalid.",
		"The directory name is invalid."
	},
#endif // #ifdef ERROR_DIRECTORY
#ifdef ERROR_EAS_DIDNT_FIT
	{	ERROR_EAS_DIDNT_FIT,                                      "ERROR_EAS_DIDNT_FIT",
		"The extended attributes did not fit in the buffer.",
		"The extended attributes did not fit in the buffer."
	},
#endif // #ifdef ERROR_EAS_DIDNT_FIT
#ifdef ERROR_EA_FILE_CORRUPT
	{	ERROR_EA_FILE_CORRUPT,                                    "ERROR_EA_FILE_CORRUPT",
		"The extended attribute file on the mounted file system is corrupt.",
		"The extended attribute file on the mounted file system is corrupt."
	},
#endif // #ifdef ERROR_EA_FILE_CORRUPT
#ifdef ERROR_EA_TABLE_FULL
	{	ERROR_EA_TABLE_FULL,                                      "ERROR_EA_TABLE_FULL",
		"The extended attribute table file is full.",
		"The extended attribute table file is full."
	},
#endif // #ifdef ERROR_EA_TABLE_FULL
#ifdef ERROR_INVALID_EA_HANDLE
	{	ERROR_INVALID_EA_HANDLE,                                  "ERROR_INVALID_EA_HANDLE",
		"The specified extended attribute handle is invalid.",
		"The specified extended attribute handle is invalid."
	},
#endif // #ifdef ERROR_INVALID_EA_HANDLE
#ifdef ERROR_EAS_NOT_SUPPORTED
	{	ERROR_EAS_NOT_SUPPORTED,                                  "ERROR_EAS_NOT_SUPPORTED",
		"The mounted file system does not support extended attributes.",
		"The mounted file system does not support extended attributes."
	},
#endif // #ifdef ERROR_EAS_NOT_SUPPORTED
#ifdef ERROR_NOT_OWNER
	{	ERROR_NOT_OWNER,                                          "ERROR_NOT_OWNER",
		"Attempt to release mutex not owned by caller.",
		"Attempt to release mutex not owned by caller."
	},
#endif // #ifdef ERROR_NOT_OWNER
#ifdef ERROR_TOO_MANY_POSTS
	{	ERROR_TOO_MANY_POSTS,                                     "ERROR_TOO_MANY_POSTS",
		"Too many posts were made to a semaphore.",
		"Too many posts were made to a semaphore."
	},
#endif // #ifdef ERROR_TOO_MANY_POSTS
#ifdef ERROR_PARTIAL_COPY
	{	ERROR_PARTIAL_COPY,                                       "ERROR_PARTIAL_COPY",
		"Only part of a ReadProcessMemory or WriteProcessMemory request was completed.",
		"Only part of a ReadProcessMemory or WriteProcessMemory request was completed."
	},
#endif // #ifdef ERROR_PARTIAL_COPY
#ifdef ERROR_OPLOCK_NOT_GRANTED
	{	ERROR_OPLOCK_NOT_GRANTED,                                 "ERROR_OPLOCK_NOT_GRANTED",
		"The oplock request is denied.",
		"The oplock request is denied."
	},
#endif // #ifdef ERROR_OPLOCK_NOT_GRANTED
#ifdef ERROR_INVALID_OPLOCK_PROTOCOL
	{	ERROR_INVALID_OPLOCK_PROTOCOL,                            "ERROR_INVALID_OPLOCK_PROTOCOL",
		"An invalid oplock acknowledgment was received by the system.",
		"An invalid oplock acknowledgment was received by the system."
	},
#endif // #ifdef ERROR_INVALID_OPLOCK_PROTOCOL
#ifdef ERROR_DISK_TOO_FRAGMENTED
	{	ERROR_DISK_TOO_FRAGMENTED,                                "ERROR_DISK_TOO_FRAGMENTED",
		"The volume is too fragmented to complete this operation.",
		"The volume is too fragmented to complete this operation."
	},
#endif // #ifdef ERROR_DISK_TOO_FRAGMENTED
#ifdef ERROR_DELETE_PENDING
	{	ERROR_DELETE_PENDING,                                     "ERROR_DELETE_PENDING",
		"The file cannot be opened because it is in the process of being deleted.",
		"The file cannot be opened because it is in the process of being deleted."
	},
#endif // #ifdef ERROR_DELETE_PENDING
#ifdef ERROR_MR_MID_NOT_FOUND
	{	ERROR_MR_MID_NOT_FOUND,                                   "ERROR_MR_MID_NOT_FOUND",
		"The system cannot find message text for message number in the message file.",
		"The system cannot find message text for message number in the message file."
	},
#endif // #ifdef ERROR_MR_MID_NOT_FOUND
#ifdef ERROR_SCOPE_NOT_FOUND
	{	ERROR_SCOPE_NOT_FOUND,                                    "ERROR_SCOPE_NOT_FOUND",
		"The scope specified was not found.",
		"The scope specified was not found."
	},
#endif // #ifdef ERROR_SCOPE_NOT_FOUND
#ifdef ERROR_INVALID_ADDRESS
	{	ERROR_INVALID_ADDRESS,                                    "ERROR_INVALID_ADDRESS",
		"Attempt to access invalid address.",
		"Attempt to access invalid address."
	},
#endif // #ifdef ERROR_INVALID_ADDRESS
#ifdef ERROR_ARITHMETIC_OVERFLOW
	{	ERROR_ARITHMETIC_OVERFLOW,                                "ERROR_ARITHMETIC_OVERFLOW",
		"Arithmetic result exceeded 32 bits.",
		"Arithmetic result exceeded 32 bits."
	},
#endif // #ifdef ERROR_ARITHMETIC_OVERFLOW
#ifdef ERROR_PIPE_CONNECTED
	{	ERROR_PIPE_CONNECTED,                                     "ERROR_PIPE_CONNECTED",
		"There is a process on other end of the pipe.",
		"There is a process on other end of the pipe."
	},
#endif // #ifdef ERROR_PIPE_CONNECTED
#ifdef ERROR_PIPE_LISTENING
	{	ERROR_PIPE_LISTENING,                                     "ERROR_PIPE_LISTENING",
		"Waiting for a process to open the other end of the pipe.",
		"Waiting for a process to open the other end of the pipe."
	},
#endif // #ifdef ERROR_PIPE_LISTENING
#ifdef ERROR_EA_ACCESS_DENIED
	{	ERROR_EA_ACCESS_DENIED,                                   "ERROR_EA_ACCESS_DENIED",
		"Access to the extended attribute was denied.",
		"Access to the extended attribute was denied."
	},
#endif // #ifdef ERROR_EA_ACCESS_DENIED
#ifdef ERROR_OPERATION_ABORTED
	{	ERROR_OPERATION_ABORTED,                                  "ERROR_OPERATION_ABORTED",
		"The I/O operation has been aborted because of either a thread exit or an application request.",
		"The I/O operation has been aborted because of either a thread exit or an application request."
	},
#endif // #ifdef ERROR_OPERATION_ABORTED
#ifdef ERROR_IO_INCOMPLETE
	{	ERROR_IO_INCOMPLETE,                                      "ERROR_IO_INCOMPLETE",
		"Overlapped I/O event is not in a signaled state.",
		"Overlapped I/O event is not in a signaled state."
	},
#endif // #ifdef ERROR_IO_INCOMPLETE
#ifdef ERROR_IO_PENDING
	{	ERROR_IO_PENDING,                                         "ERROR_IO_PENDING",
		"Overlapped I/O operation is in progress.",
		"Overlapped I/O operation is in progress."
	},
#endif // #ifdef ERROR_IO_PENDING
#ifdef ERROR_NOACCESS
	{	ERROR_NOACCESS,                                           "ERROR_NOACCESS",
		"Invalid access to memory location.",
		"Invalid access to memory location."
	},
#endif // #ifdef ERROR_NOACCESS
#ifdef ERROR_SWAPERROR
	{	ERROR_SWAPERROR,                                          "ERROR_SWAPERROR",
		"Error performing inpage operation.",
		"Error performing inpage operation."
	},
#endif // #ifdef ERROR_SWAPERROR
#ifdef ERROR_STACK_OVERFLOW
	{	ERROR_STACK_OVERFLOW,                                     "ERROR_STACK_OVERFLOW",
		"Recursion too deep; the stack overflowed.",
		"Recursion too deep; the stack overflowed."
	},
#endif // #ifdef ERROR_STACK_OVERFLOW
#ifdef ERROR_INVALID_MESSAGE
	{	ERROR_INVALID_MESSAGE,                                    "ERROR_INVALID_MESSAGE",
		"The window cannot act on the sent message.",
		"The window cannot act on the sent message."
	},
#endif // #ifdef ERROR_INVALID_MESSAGE
#ifdef ERROR_CAN_NOT_COMPLETE
	{	ERROR_CAN_NOT_COMPLETE,                                   "ERROR_CAN_NOT_COMPLETE",
		"Cannot complete this function.",
		"Cannot complete this function."
	},
#endif // #ifdef ERROR_CAN_NOT_COMPLETE
#ifdef ERROR_INVALID_FLAGS
	{	ERROR_INVALID_FLAGS,                                      "ERROR_INVALID_FLAGS",
		"Invalid flags.",
		"Invalid flags."
	},
#endif // #ifdef ERROR_INVALID_FLAGS
#ifdef ERROR_UNRECOGNIZED_VOLUME
	{	ERROR_UNRECOGNIZED_VOLUME,                                "ERROR_UNRECOGNIZED_VOLUME",
		"The volume does not contain a recognized file system. Please make sure that all required file system drivers are loaded and that the volume is not corrupted.",
		"The volume does not contain a recognized file system. Please make sure that all required file system drivers are loaded and that the volume is not corrupted."
	},
#endif // #ifdef ERROR_UNRECOGNIZED_VOLUME
#ifdef ERROR_FILE_INVALID
	{	ERROR_FILE_INVALID,                                       "ERROR_FILE_INVALID",
		"The volume for a file has been externally altered so that the opened file is no longer valid.",
		"The volume for a file has been externally altered so that the opened file is no longer valid."
	},
#endif // #ifdef ERROR_FILE_INVALID
#ifdef ERROR_FULLSCREEN_MODE
	{	ERROR_FULLSCREEN_MODE,                                    "ERROR_FULLSCREEN_MODE",
		"The requested operation cannot be performed in full-screen mode.",
		"The requested operation cannot be performed in full-screen mode."
	},
#endif // #ifdef ERROR_FULLSCREEN_MODE
#ifdef ERROR_NO_TOKEN
	{	ERROR_NO_TOKEN,                                           "ERROR_NO_TOKEN",
		"An attempt was made to reference a token that does not exist.",
		"An attempt was made to reference a token that does not exist."
	},
#endif // #ifdef ERROR_NO_TOKEN
#ifdef ERROR_BADDB
	{	ERROR_BADDB,                                              "ERROR_BADDB",
		"The configuration registry database is corrupt.",
		"The configuration registry database is corrupt."
	},
#endif // #ifdef ERROR_BADDB
#ifdef ERROR_BADKEY
	{	ERROR_BADKEY,                                             "ERROR_BADKEY",
		"The configuration registry key is invalid.",
		"The configuration registry key is invalid."
	},
#endif // #ifdef ERROR_BADKEY
#ifdef ERROR_CANTOPEN
	{	ERROR_CANTOPEN,                                           "ERROR_CANTOPEN",
		"The configuration registry key could not be opened.",
		"The configuration registry key could not be opened."
	},
#endif // #ifdef ERROR_CANTOPEN
#ifdef ERROR_CANTREAD
	{	ERROR_CANTREAD,                                           "ERROR_CANTREAD",
		"The configuration registry key could not be read.",
		"The configuration registry key could not be read."
	},
#endif // #ifdef ERROR_CANTREAD
#ifdef ERROR_CANTWRITE
	{	ERROR_CANTWRITE,                                          "ERROR_CANTWRITE",
		"The configuration registry key could not be written.",
		"The configuration registry key could not be written."
	},
#endif // #ifdef ERROR_CANTWRITE
#ifdef ERROR_REGISTRY_RECOVERED
	{	ERROR_REGISTRY_RECOVERED,                                 "ERROR_REGISTRY_RECOVERED",
		"One of the files in the registry database had to be recovered by use of a log or alternate copy. The recovery was successful.",
		"One of the files in the registry database had to be recovered by use of a log or alternate copy. The recovery was successful."
	},
#endif // #ifdef ERROR_REGISTRY_RECOVERED
#ifdef ERROR_REGISTRY_CORRUPT
	{	ERROR_REGISTRY_CORRUPT,                                   "ERROR_REGISTRY_CORRUPT",
		"The registry is corrupted. The structure of one of the files containing registry data is corrupted, or the system's memory image of the file is corrupted, or the file could not be recovered because the alternate copy or log was absent or corrupted.",
		"The registry is corrupted. The structure of one of the files containing registry data is corrupted, or the system's memory image of the file is corrupted, or the file could not be recovered because the alternate copy or log was absent or corrupted."
	},
#endif // #ifdef ERROR_REGISTRY_CORRUPT
#ifdef ERROR_REGISTRY_IO_FAILED
	{	ERROR_REGISTRY_IO_FAILED,                                 "ERROR_REGISTRY_IO_FAILED",
		"An I/O operation initiated by the registry failed unrecoverably. The registry could not read in, or write out, or flush, one of the files that contain the system's image of the registry.",
		"An I/O operation initiated by the registry failed unrecoverably. The registry could not read in, or write out, or flush, one of the files that contain the system's image of the registry."
	},
#endif // #ifdef ERROR_REGISTRY_IO_FAILED
#ifdef ERROR_NOT_REGISTRY_FILE
	{	ERROR_NOT_REGISTRY_FILE,                                  "ERROR_NOT_REGISTRY_FILE",
		"The system has attempted to load or restore a file into the registry, but the specified file is not in a registry file format.",
		"The system has attempted to load or restore a file into the registry, but the specified file is not in a registry file format."
	},
#endif // #ifdef ERROR_NOT_REGISTRY_FILE
#ifdef ERROR_KEY_DELETED
	{	ERROR_KEY_DELETED,                                        "ERROR_KEY_DELETED",
		"Illegal operation attempted on a registry key that has been marked for deletion.",
		"Illegal operation attempted on a registry key that has been marked for deletion."
	},
#endif // #ifdef ERROR_KEY_DELETED
#ifdef ERROR_NO_LOG_SPACE
	{	ERROR_NO_LOG_SPACE,                                       "ERROR_NO_LOG_SPACE",
		"System could not allocate the required space in a registry log.",
		"System could not allocate the required space in a registry log."
	},
#endif // #ifdef ERROR_NO_LOG_SPACE
#ifdef ERROR_KEY_HAS_CHILDREN
	{	ERROR_KEY_HAS_CHILDREN,                                   "ERROR_KEY_HAS_CHILDREN",
		"Cannot create a symbolic link in a registry key that already has subkeys or values.",
		"Cannot create a symbolic link in a registry key that already has subkeys or values."
	},
#endif // #ifdef ERROR_KEY_HAS_CHILDREN
#ifdef ERROR_CHILD_MUST_BE_VOLATILE
	{	ERROR_CHILD_MUST_BE_VOLATILE,                             "ERROR_CHILD_MUST_BE_VOLATILE",
		"Cannot create a stable subkey under a volatile parent key.",
		"Cannot create a stable subkey under a volatile parent key."
	},
#endif // #ifdef ERROR_CHILD_MUST_BE_VOLATILE
#ifdef ERROR_NOTIFY_ENUM_DIR
	{	ERROR_NOTIFY_ENUM_DIR,                                    "ERROR_NOTIFY_ENUM_DIR",
		"A notify change request is being completed and the information is not being returned in the caller's buffer. The caller now needs to enumerate the files to find the changes.",
		"A notify change request is being completed and the information is not being returned in the caller's buffer. The caller now needs to enumerate the files to find the changes."
	},
#endif // #ifdef ERROR_NOTIFY_ENUM_DIR
#ifdef ERROR_DEPENDENT_SERVICES_RUNNING
	{	ERROR_DEPENDENT_SERVICES_RUNNING,                         "ERROR_DEPENDENT_SERVICES_RUNNING",
		"A stop control has been sent to a service that other running services are dependent on.",
		"A stop control has been sent to a service that other running services are dependent on."
	},
#endif // #ifdef ERROR_DEPENDENT_SERVICES_RUNNING
#ifdef ERROR_INVALID_SERVICE_CONTROL
	{	ERROR_INVALID_SERVICE_CONTROL,                            "ERROR_INVALID_SERVICE_CONTROL",
		"The requested control is not valid for this service.",
		"The requested control is not valid for this service."
	},
#endif // #ifdef ERROR_INVALID_SERVICE_CONTROL
#ifdef ERROR_SERVICE_REQUEST_TIMEOUT
	{	ERROR_SERVICE_REQUEST_TIMEOUT,                            "ERROR_SERVICE_REQUEST_TIMEOUT",
		"The service did not respond to the start or control request in a timely fashion.",
		"The service did not respond to the start or control request in a timely fashion."
	},
#endif // #ifdef ERROR_SERVICE_REQUEST_TIMEOUT
#ifdef ERROR_SERVICE_NO_THREAD
	{	ERROR_SERVICE_NO_THREAD,                                  "ERROR_SERVICE_NO_THREAD",
		"A thread could not be created for the service.",
		"A thread could not be created for the service."
	},
#endif // #ifdef ERROR_SERVICE_NO_THREAD
#ifdef ERROR_SERVICE_DATABASE_LOCKED
	{	ERROR_SERVICE_DATABASE_LOCKED,                            "ERROR_SERVICE_DATABASE_LOCKED",
		"The service database is locked.",
		"The service database is locked."
	},
#endif // #ifdef ERROR_SERVICE_DATABASE_LOCKED
#ifdef ERROR_SERVICE_ALREADY_RUNNING
	{	ERROR_SERVICE_ALREADY_RUNNING,                            "ERROR_SERVICE_ALREADY_RUNNING",
		"An instance of the service is already running.",
		"An instance of the service is already running."
	},
#endif // #ifdef ERROR_SERVICE_ALREADY_RUNNING
#ifdef ERROR_INVALID_SERVICE_ACCOUNT
	{	ERROR_INVALID_SERVICE_ACCOUNT,                            "ERROR_INVALID_SERVICE_ACCOUNT",
		"The account name is invalid or does not exist.",
		"The account name is invalid or does not exist, or the password is invalid for the account name specified."
	},
#endif // #ifdef ERROR_INVALID_SERVICE_ACCOUNT
#ifdef ERROR_SERVICE_DISABLED
	{	ERROR_SERVICE_DISABLED,                                   "ERROR_SERVICE_DISABLED",
		"The specified service is disabled and cannot be started.",
		"The service cannot be started, either because it is disabled or because it has no enabled devices associated with it."
	},
#endif // #ifdef ERROR_SERVICE_DISABLED
#ifdef ERROR_CIRCULAR_DEPENDENCY
	{	ERROR_CIRCULAR_DEPENDENCY,                                "ERROR_CIRCULAR_DEPENDENCY",
		"Circular service dependency was specified.",
		"Circular service dependency was specified."
	},
#endif // #ifdef ERROR_CIRCULAR_DEPENDENCY
#ifdef ERROR_SERVICE_DOES_NOT_EXIST
	{	ERROR_SERVICE_DOES_NOT_EXIST,                             "ERROR_SERVICE_DOES_NOT_EXIST",
		"The specified service does not exist as an installed service.",
		"The specified service does not exist as an installed service."
	},
#endif // #ifdef ERROR_SERVICE_DOES_NOT_EXIST
#ifdef ERROR_SERVICE_CANNOT_ACCEPT_CTRL
	{	ERROR_SERVICE_CANNOT_ACCEPT_CTRL,                         "ERROR_SERVICE_CANNOT_ACCEPT_CTRL",
		"The service cannot accept control messages at this time.",
		"The service cannot accept control messages at this time."
	},
#endif // #ifdef ERROR_SERVICE_CANNOT_ACCEPT_CTRL
#ifdef ERROR_SERVICE_NOT_ACTIVE
	{	ERROR_SERVICE_NOT_ACTIVE,                                 "ERROR_SERVICE_NOT_ACTIVE",
		"The service has not been started.",
		"The service has not been started."
	},
#endif // #ifdef ERROR_SERVICE_NOT_ACTIVE
#ifdef ERROR_FAILED_SERVICE_CONTROLLER_CONNECT
	{	ERROR_FAILED_SERVICE_CONTROLLER_CONNECT,                  "ERROR_FAILED_SERVICE_CONTROLLER_CONNECT",
		"The service process could not connect to the service controller.",
		"The service process could not connect to the service controller."
	},
#endif // #ifdef ERROR_FAILED_SERVICE_CONTROLLER_CONNECT
#ifdef ERROR_EXCEPTION_IN_SERVICE
	{	ERROR_EXCEPTION_IN_SERVICE,                               "ERROR_EXCEPTION_IN_SERVICE",
		"An exception occurred in the service when handling the control request.",
		"An exception occurred in the service when handling the control request."
	},
#endif // #ifdef ERROR_EXCEPTION_IN_SERVICE
#ifdef ERROR_DATABASE_DOES_NOT_EXIST
	{	ERROR_DATABASE_DOES_NOT_EXIST,                            "ERROR_DATABASE_DOES_NOT_EXIST",
		"The database specified does not exist.",
		"The database specified does not exist."
	},
#endif // #ifdef ERROR_DATABASE_DOES_NOT_EXIST
#ifdef ERROR_SERVICE_SPECIFIC_ERROR
	{	ERROR_SERVICE_SPECIFIC_ERROR,                             "ERROR_SERVICE_SPECIFIC_ERROR",
		"The service has returned a service-specific error code.",
		"The service has returned a service-specific error code."
	},
#endif // #ifdef ERROR_SERVICE_SPECIFIC_ERROR
#ifdef ERROR_PROCESS_ABORTED
	{	ERROR_PROCESS_ABORTED,                                    "ERROR_PROCESS_ABORTED",
		"The process terminated unexpectedly.",
		"The process terminated unexpectedly."
	},
#endif // #ifdef ERROR_PROCESS_ABORTED
#ifdef ERROR_SERVICE_DEPENDENCY_FAIL
	{	ERROR_SERVICE_DEPENDENCY_FAIL,                            "ERROR_SERVICE_DEPENDENCY_FAIL",
		"The dependency service or group failed to start.",
		"The dependency service or group failed to start."
	},
#endif // #ifdef ERROR_SERVICE_DEPENDENCY_FAIL
#ifdef ERROR_SERVICE_LOGON_FAILED
	{	ERROR_SERVICE_LOGON_FAILED,                               "ERROR_SERVICE_LOGON_FAILED",
		"The service did not start due to a logon failure.",
		"The service did not start due to a logon failure."
	},
#endif // #ifdef ERROR_SERVICE_LOGON_FAILED
#ifdef ERROR_SERVICE_START_HANG
	{	ERROR_SERVICE_START_HANG,                                 "ERROR_SERVICE_START_HANG",
		"After starting, the service hung in a start-pending state.",
		"After starting, the service hung in a start-pending state."
	},
#endif // #ifdef ERROR_SERVICE_START_HANG
#ifdef ERROR_INVALID_SERVICE_LOCK
	{	ERROR_INVALID_SERVICE_LOCK,                               "ERROR_INVALID_SERVICE_LOCK",
		"The specified service database lock is invalid.",
		"The specified service database lock is invalid."
	},
#endif // #ifdef ERROR_INVALID_SERVICE_LOCK
#ifdef ERROR_SERVICE_MARKED_FOR_DELETE
	{	ERROR_SERVICE_MARKED_FOR_DELETE,                          "ERROR_SERVICE_MARKED_FOR_DELETE",
		"The specified service has been marked for deletion.",
		"The specified service has been marked for deletion."
	},
#endif // #ifdef ERROR_SERVICE_MARKED_FOR_DELETE
#ifdef ERROR_SERVICE_EXISTS
	{	ERROR_SERVICE_EXISTS,                                     "ERROR_SERVICE_EXISTS",
		"The specified service already exists.",
		"The specified service already exists."
	},
#endif // #ifdef ERROR_SERVICE_EXISTS
#ifdef ERROR_ALREADY_RUNNING_LKG
	{	ERROR_ALREADY_RUNNING_LKG,                                "ERROR_ALREADY_RUNNING_LKG",
		"The system is currently running with the last-known-good configuration.",
		"The system is currently running with the last-known-good configuration."
	},
#endif // #ifdef ERROR_ALREADY_RUNNING_LKG
#ifdef ERROR_SERVICE_DEPENDENCY_DELETED
	{	ERROR_SERVICE_DEPENDENCY_DELETED,                         "ERROR_SERVICE_DEPENDENCY_DELETED",
		"The dependency service does not exist or has been marked for deletion.",
		"The dependency service does not exist or has been marked for deletion."
	},
#endif // #ifdef ERROR_SERVICE_DEPENDENCY_DELETED
#ifdef ERROR_BOOT_ALREADY_ACCEPTED
	{	ERROR_BOOT_ALREADY_ACCEPTED,                              "ERROR_BOOT_ALREADY_ACCEPTED",
		"The current boot has already been accepted for use as the last-known-good control set.",
		"The current boot has already been accepted for use as the last-known-good control set."
	},
#endif // #ifdef ERROR_BOOT_ALREADY_ACCEPTED
#ifdef ERROR_SERVICE_NEVER_STARTED
	{	ERROR_SERVICE_NEVER_STARTED,                              "ERROR_SERVICE_NEVER_STARTED",
		"No attempts to start the service have been made since the last boot.",
		"No attempts to start the service have been made since the last boot."
	},
#endif // #ifdef ERROR_SERVICE_NEVER_STARTED
#ifdef ERROR_DUPLICATE_SERVICE_NAME
	{	ERROR_DUPLICATE_SERVICE_NAME,                             "ERROR_DUPLICATE_SERVICE_NAME",
		"The name is already in use as either a service name or a service display name.",
		"The name is already in use as either a service name or a service display name."
	},
#endif // #ifdef ERROR_DUPLICATE_SERVICE_NAME
#ifdef ERROR_DIFFERENT_SERVICE_ACCOUNT
	{	ERROR_DIFFERENT_SERVICE_ACCOUNT,                          "ERROR_DIFFERENT_SERVICE_ACCOUNT",
		"The account specified for this service is different from the account specified for other services running in the same process.",
		"The account specified for this service is different from the account specified for other services running in the same process."
	},
#endif // #ifdef ERROR_DIFFERENT_SERVICE_ACCOUNT
#ifdef ERROR_CANNOT_DETECT_DRIVER_FAILURE
	{	ERROR_CANNOT_DETECT_DRIVER_FAILURE,                       "ERROR_CANNOT_DETECT_DRIVER_FAILURE",
		"Failure actions can only be set for Win32 services, not for drivers.",
		"Failure actions can only be set for Win32 services, not for drivers."
	},
#endif // #ifdef ERROR_CANNOT_DETECT_DRIVER_FAILURE
#ifdef ERROR_CANNOT_DETECT_PROCESS_ABORT
	{	ERROR_CANNOT_DETECT_PROCESS_ABORT,                        "ERROR_CANNOT_DETECT_PROCESS_ABORT",
		"This service runs in the same process as the service control manager. Therefore, the service control manager cannot take action if this service's process terminates unexpectedly.",
		"This service runs in the same process as the service control manager. Therefore, the service control manager cannot take action if this service's process terminates unexpectedly."
	},
#endif // #ifdef ERROR_CANNOT_DETECT_PROCESS_ABORT
#ifdef ERROR_NO_RECOVERY_PROGRAM
	{	ERROR_NO_RECOVERY_PROGRAM,                                "ERROR_NO_RECOVERY_PROGRAM",
		"No recovery program has been configured for this service.",
		"No recovery program has been configured for this service."
	},
#endif // #ifdef ERROR_NO_RECOVERY_PROGRAM
#ifdef ERROR_SERVICE_NOT_IN_EXE
	{	ERROR_SERVICE_NOT_IN_EXE,                                 "ERROR_SERVICE_NOT_IN_EXE",
		"The executable program that this service is configured to run in does not implement the service.",
		"The executable program that this service is configured to run in does not implement the service."
	},
#endif // #ifdef ERROR_SERVICE_NOT_IN_EXE
#ifdef ERROR_NOT_SAFEBOOT_SERVICE
	{	ERROR_NOT_SAFEBOOT_SERVICE,                               "ERROR_NOT_SAFEBOOT_SERVICE",
		"This service cannot be started in Safe Mode.",
		"This service cannot be started in Safe Mode."
	},
#endif // #ifdef ERROR_NOT_SAFEBOOT_SERVICE
#ifdef ERROR_END_OF_MEDIA
	{	ERROR_END_OF_MEDIA,                                       "ERROR_END_OF_MEDIA",
		"The physical end of the tape has been reached.",
		"The physical end of the tape has been reached."
	},
#endif // #ifdef ERROR_END_OF_MEDIA
#ifdef ERROR_FILEMARK_DETECTED
	{	ERROR_FILEMARK_DETECTED,                                  "ERROR_FILEMARK_DETECTED",
		"A tape access reached a filemark.",
		"A tape access reached a filemark."
	},
#endif // #ifdef ERROR_FILEMARK_DETECTED
#ifdef ERROR_BEGINNING_OF_MEDIA
	{	ERROR_BEGINNING_OF_MEDIA,                                 "ERROR_BEGINNING_OF_MEDIA",
		"The beginning of the tape or a partition was encountered.",
		"The beginning of the tape or a partition was encountered."
	},
#endif // #ifdef ERROR_BEGINNING_OF_MEDIA
#ifdef ERROR_SETMARK_DETECTED
	{	ERROR_SETMARK_DETECTED,                                   "ERROR_SETMARK_DETECTED",
		"A tape access reached the end of a set of files.",
		"A tape access reached the end of a set of files."
	},
#endif // #ifdef ERROR_SETMARK_DETECTED
#ifdef ERROR_NO_DATA_DETECTED
	{	ERROR_NO_DATA_DETECTED,                                   "ERROR_NO_DATA_DETECTED",
		"No more data is on the tape.",
		"No more data is on the tape."
	},
#endif // #ifdef ERROR_NO_DATA_DETECTED
#ifdef ERROR_PARTITION_FAILURE
	{	ERROR_PARTITION_FAILURE,                                  "ERROR_PARTITION_FAILURE",
		"Tape could not be partitioned.",
		"Tape could not be partitioned."
	},
#endif // #ifdef ERROR_PARTITION_FAILURE
#ifdef ERROR_INVALID_BLOCK_LENGTH
	{	ERROR_INVALID_BLOCK_LENGTH,                               "ERROR_INVALID_BLOCK_LENGTH",
		"When accessing a new tape of a multivolume partition, the current block size is incorrect.",
		"When accessing a new tape of a multivolume partition, the current block size is incorrect."
	},
#endif // #ifdef ERROR_INVALID_BLOCK_LENGTH
#ifdef ERROR_DEVICE_NOT_PARTITIONED
	{	ERROR_DEVICE_NOT_PARTITIONED,                             "ERROR_DEVICE_NOT_PARTITIONED",
		"Tape partition information could not be found when loading a tape.",
		"Tape partition information could not be found when loading a tape."
	},
#endif // #ifdef ERROR_DEVICE_NOT_PARTITIONED
#ifdef ERROR_UNABLE_TO_LOCK_MEDIA
	{	ERROR_UNABLE_TO_LOCK_MEDIA,                               "ERROR_UNABLE_TO_LOCK_MEDIA",
		"Unable to lock the media eject mechanism.",
		"Unable to lock the media eject mechanism."
	},
#endif // #ifdef ERROR_UNABLE_TO_LOCK_MEDIA
#ifdef ERROR_UNABLE_TO_UNLOAD_MEDIA
	{	ERROR_UNABLE_TO_UNLOAD_MEDIA,                             "ERROR_UNABLE_TO_UNLOAD_MEDIA",
		"Unable to unload the media.",
		"Unable to unload the media."
	},
#endif // #ifdef ERROR_UNABLE_TO_UNLOAD_MEDIA
#ifdef ERROR_MEDIA_CHANGED
	{	ERROR_MEDIA_CHANGED,                                      "ERROR_MEDIA_CHANGED",
		"The media in the drive may have changed.",
		"The media in the drive may have changed."
	},
#endif // #ifdef ERROR_MEDIA_CHANGED
#ifdef ERROR_BUS_RESET
	{	ERROR_BUS_RESET,                                          "ERROR_BUS_RESET",
		"The I/O bus was reset.",
		"The I/O bus was reset."
	},
#endif // #ifdef ERROR_BUS_RESET
#ifdef ERROR_NO_MEDIA_IN_DRIVE
	{	ERROR_NO_MEDIA_IN_DRIVE,                                  "ERROR_NO_MEDIA_IN_DRIVE",
		"No media in drive.",
		"No media in drive."
	},
#endif // #ifdef ERROR_NO_MEDIA_IN_DRIVE
#ifdef ERROR_NO_UNICODE_TRANSLATION
	{	ERROR_NO_UNICODE_TRANSLATION,                             "ERROR_NO_UNICODE_TRANSLATION",
		"No mapping for the Unicode character exists in the target multi-byte code page.",
		"No mapping for the Unicode character exists in the target multi-byte code page."
	},
#endif // #ifdef ERROR_NO_UNICODE_TRANSLATION
#ifdef ERROR_DLL_INIT_FAILED
	{	ERROR_DLL_INIT_FAILED,                                    "ERROR_DLL_INIT_FAILED",
		"A dynamic link library (DLL) initialization routine failed.",
		"A dynamic link library (DLL) initialization routine failed."
	},
#endif // #ifdef ERROR_DLL_INIT_FAILED
#ifdef ERROR_SHUTDOWN_IN_PROGRESS
	{	ERROR_SHUTDOWN_IN_PROGRESS,                               "ERROR_SHUTDOWN_IN_PROGRESS",
		"A system shutdown is in progress.",
		"A system shutdown is in progress."
	},
#endif // #ifdef ERROR_SHUTDOWN_IN_PROGRESS
#ifdef ERROR_NO_SHUTDOWN_IN_PROGRESS
	{	ERROR_NO_SHUTDOWN_IN_PROGRESS,                            "ERROR_NO_SHUTDOWN_IN_PROGRESS",
		"Unable to abort the system shutdown because no shutdown was in progress.",
		"Unable to abort the system shutdown because no shutdown was in progress."
	},
#endif // #ifdef ERROR_NO_SHUTDOWN_IN_PROGRESS
#ifdef ERROR_IO_DEVICE
	{	ERROR_IO_DEVICE,                                          "ERROR_IO_DEVICE",
		"The request could not be performed because of an I/O device error.",
		"The request could not be performed because of an I/O device error."
	},
#endif // #ifdef ERROR_IO_DEVICE
#ifdef ERROR_SERIAL_NO_DEVICE
	{	ERROR_SERIAL_NO_DEVICE,                                   "ERROR_SERIAL_NO_DEVICE",
		"No serial device was successfully initialized. The serial driver will unload.",
		"No serial device was successfully initialized. The serial driver will unload."
	},
#endif // #ifdef ERROR_SERIAL_NO_DEVICE
#ifdef ERROR_IRQ_BUSY
	{	ERROR_IRQ_BUSY,                                           "ERROR_IRQ_BUSY",
		"Unable to open a device that was sharing an interrupt request (IRQ) with other devices. At least one other device that uses that IRQ was already opened.",
		"Unable to open a device that was sharing an interrupt request (IRQ) with other devices. At least one other device that uses that IRQ was already opened."
	},
#endif // #ifdef ERROR_IRQ_BUSY
#ifdef ERROR_MORE_WRITES
	{	ERROR_MORE_WRITES,                                        "ERROR_MORE_WRITES",
		"A serial I/O operation was completed by another write to the serial port. (The IOCTL_SERIAL_XOFF_COUNTER reached zero.)",
		"A serial I/O operation was completed by another write to the serial port. (The IOCTL_SERIAL_XOFF_COUNTER reached zero.)"
	},
#endif // #ifdef ERROR_MORE_WRITES
#ifdef ERROR_COUNTER_TIMEOUT
	{	ERROR_COUNTER_TIMEOUT,                                    "ERROR_COUNTER_TIMEOUT",
		"A serial I/O operation completed because the timeout period expired. (The IOCTL_SERIAL_XOFF_COUNTER did not reach zero.)",
		"A serial I/O operation completed because the timeout period expired. (The IOCTL_SERIAL_XOFF_COUNTER did not reach zero.)"
	},
#endif // #ifdef ERROR_COUNTER_TIMEOUT
#ifdef ERROR_FLOPPY_ID_MARK_NOT_FOUND
	{	ERROR_FLOPPY_ID_MARK_NOT_FOUND,                           "ERROR_FLOPPY_ID_MARK_NOT_FOUND",
		"No ID address mark was found on the floppy disk.",
		"No ID address mark was found on the floppy disk."
	},
#endif // #ifdef ERROR_FLOPPY_ID_MARK_NOT_FOUND
#ifdef ERROR_FLOPPY_WRONG_CYLINDER
	{	ERROR_FLOPPY_WRONG_CYLINDER,                              "ERROR_FLOPPY_WRONG_CYLINDER",
		"Mismatch between the floppy disk sector ID field and the floppy disk controller track address.",
		"Mismatch between the floppy disk sector ID field and the floppy disk controller track address."
	},
#endif // #ifdef ERROR_FLOPPY_WRONG_CYLINDER
#ifdef ERROR_FLOPPY_UNKNOWN_ERROR
	{	ERROR_FLOPPY_UNKNOWN_ERROR,                               "ERROR_FLOPPY_UNKNOWN_ERROR",
		"The floppy disk controller reported an error that is not recognized by the floppy disk driver.",
		"The floppy disk controller reported an error that is not recognized by the floppy disk driver."
	},
#endif // #ifdef ERROR_FLOPPY_UNKNOWN_ERROR
#ifdef ERROR_FLOPPY_BAD_REGISTERS
	{	ERROR_FLOPPY_BAD_REGISTERS,                               "ERROR_FLOPPY_BAD_REGISTERS",
		"The floppy disk controller returned inconsistent results in its registers.",
		"The floppy disk controller returned inconsistent results in its registers."
	},
#endif // #ifdef ERROR_FLOPPY_BAD_REGISTERS
#ifdef ERROR_DISK_RECALIBRATE_FAILED
	{	ERROR_DISK_RECALIBRATE_FAILED,                            "ERROR_DISK_RECALIBRATE_FAILED",
		"While accessing the hard disk, a recalibrate operation failed, even after retries.",
		"While accessing the hard disk, a recalibrate operation failed, even after retries."
	},
#endif // #ifdef ERROR_DISK_RECALIBRATE_FAILED
#ifdef ERROR_DISK_OPERATION_FAILED
	{	ERROR_DISK_OPERATION_FAILED,                              "ERROR_DISK_OPERATION_FAILED",
		"While accessing the hard disk, a disk operation failed even after retries.",
		"While accessing the hard disk, a disk operation failed even after retries."
	},
#endif // #ifdef ERROR_DISK_OPERATION_FAILED
#ifdef ERROR_DISK_RESET_FAILED
	{	ERROR_DISK_RESET_FAILED,                                  "ERROR_DISK_RESET_FAILED",
		"While accessing the hard disk, a disk controller reset was needed, but even that failed.",
		"While accessing the hard disk, a disk controller reset was needed, but even that failed."
	},
#endif // #ifdef ERROR_DISK_RESET_FAILED
#ifdef ERROR_EOM_OVERFLOW
	{	ERROR_EOM_OVERFLOW,                                       "ERROR_EOM_OVERFLOW",
		"Physical end of tape encountered.",
		"Physical end of tape encountered."
	},
#endif // #ifdef ERROR_EOM_OVERFLOW
#ifdef ERROR_NOT_ENOUGH_SERVER_MEMORY
	{	ERROR_NOT_ENOUGH_SERVER_MEMORY,                           "ERROR_NOT_ENOUGH_SERVER_MEMORY",
		"Not enough server storage is available to process this command.",
		"Not enough server storage is available to process this command."
	},
#endif // #ifdef ERROR_NOT_ENOUGH_SERVER_MEMORY
#ifdef ERROR_POSSIBLE_DEADLOCK
	{	ERROR_POSSIBLE_DEADLOCK,                                  "ERROR_POSSIBLE_DEADLOCK",
		"A potential deadlock condition has been detected.",
		"A potential deadlock condition has been detected."
	},
#endif // #ifdef ERROR_POSSIBLE_DEADLOCK
#ifdef ERROR_MAPPED_ALIGNMENT
	{	ERROR_MAPPED_ALIGNMENT,                                   "ERROR_MAPPED_ALIGNMENT",
		"The base address or the file offset specified does not have the proper alignment.",
		"The base address or the file offset specified does not have the proper alignment."
	},
#endif // #ifdef ERROR_MAPPED_ALIGNMENT
#ifdef ERROR_SET_POWER_STATE_VETOED
	{	ERROR_SET_POWER_STATE_VETOED,                             "ERROR_SET_POWER_STATE_VETOED",
		"An attempt to change the system power state was vetoed by another application or driver.",
		"An attempt to change the system power state was vetoed by another application or driver."
	},
#endif // #ifdef ERROR_SET_POWER_STATE_VETOED
#ifdef ERROR_SET_POWER_STATE_FAILED
	{	ERROR_SET_POWER_STATE_FAILED,                             "ERROR_SET_POWER_STATE_FAILED",
		"The system BIOS failed an attempt to change the system power state.",
		"The system BIOS failed an attempt to change the system power state."
	},
#endif // #ifdef ERROR_SET_POWER_STATE_FAILED
#ifdef ERROR_TOO_MANY_LINKS
	{	ERROR_TOO_MANY_LINKS,                                     "ERROR_TOO_MANY_LINKS",
		"An attempt was made to create more links on a file than the file system supports.",
		"An attempt was made to create more links on a file than the file system supports."
	},
#endif // #ifdef ERROR_TOO_MANY_LINKS
#ifdef ERROR_OLD_WIN_VERSION
	{	ERROR_OLD_WIN_VERSION,                                    "ERROR_OLD_WIN_VERSION",
		"The specified program requires a newer version of Windows.",
		"The specified program requires a newer version of Windows."
	},
#endif // #ifdef ERROR_OLD_WIN_VERSION
#ifdef ERROR_APP_WRONG_OS
	{	ERROR_APP_WRONG_OS,                                       "ERROR_APP_WRONG_OS",
		"The specified program is not a Windows or MS-DOS program.",
		"The specified program is not a Windows or MS-DOS program."
	},
#endif // #ifdef ERROR_APP_WRONG_OS
#ifdef ERROR_SINGLE_INSTANCE_APP
	{	ERROR_SINGLE_INSTANCE_APP,                                "ERROR_SINGLE_INSTANCE_APP",
		"Cannot start more than one instance of the specified program.",
		"Cannot start more than one instance of the specified program."
	},
#endif // #ifdef ERROR_SINGLE_INSTANCE_APP
#ifdef ERROR_RMODE_APP
	{	ERROR_RMODE_APP,                                          "ERROR_RMODE_APP",
		"The specified program was written for an earlier version of Windows.",
		"The specified program was written for an earlier version of Windows."
	},
#endif // #ifdef ERROR_RMODE_APP
#ifdef ERROR_INVALID_DLL
	{	ERROR_INVALID_DLL,                                        "ERROR_INVALID_DLL",
		"One of the library files needed to run this application is damaged.",
		"One of the library files needed to run this application is damaged."
	},
#endif // #ifdef ERROR_INVALID_DLL
#ifdef ERROR_NO_ASSOCIATION
	{	ERROR_NO_ASSOCIATION,                                     "ERROR_NO_ASSOCIATION",
		"No application is associated with the specified file for this operation.",
		"No application is associated with the specified file for this operation."
	},
#endif // #ifdef ERROR_NO_ASSOCIATION
#ifdef ERROR_DDE_FAIL
	{	ERROR_DDE_FAIL,                                           "ERROR_DDE_FAIL",
		"An error occurred in sending the command to the application.",
		"An error occurred in sending the command to the application."
	},
#endif // #ifdef ERROR_DDE_FAIL
#ifdef ERROR_DLL_NOT_FOUND
	{	ERROR_DLL_NOT_FOUND,                                      "ERROR_DLL_NOT_FOUND",
		"One of the library files needed to run this application cannot be found.",
		"One of the library files needed to run this application cannot be found."
	},
#endif // #ifdef ERROR_DLL_NOT_FOUND
#ifdef ERROR_NO_MORE_USER_HANDLES
	{	ERROR_NO_MORE_USER_HANDLES,                               "ERROR_NO_MORE_USER_HANDLES",
		"The current process has used all of its system allowance of handles for Window Manager objects.",
		"The current process has used all of its system allowance of handles for Window Manager objects."
	},
#endif // #ifdef ERROR_NO_MORE_USER_HANDLES
#ifdef ERROR_MESSAGE_SYNC_ONLY
	{	ERROR_MESSAGE_SYNC_ONLY,                                  "ERROR_MESSAGE_SYNC_ONLY",
		"The message can be used only with synchronous operations.",
		"The message can be used only with synchronous operations."
	},
#endif // #ifdef ERROR_MESSAGE_SYNC_ONLY
#ifdef ERROR_SOURCE_ELEMENT_EMPTY
	{	ERROR_SOURCE_ELEMENT_EMPTY,                               "ERROR_SOURCE_ELEMENT_EMPTY",
		"The indicated source element has no media.",
		"The indicated source element has no media."
	},
#endif // #ifdef ERROR_SOURCE_ELEMENT_EMPTY
#ifdef ERROR_DESTINATION_ELEMENT_FULL
	{	ERROR_DESTINATION_ELEMENT_FULL,                           "ERROR_DESTINATION_ELEMENT_FULL",
		"The indicated destination element already contains media.",
		"The indicated destination element already contains media."
	},
#endif // #ifdef ERROR_DESTINATION_ELEMENT_FULL
#ifdef ERROR_ILLEGAL_ELEMENT_ADDRESS
	{	ERROR_ILLEGAL_ELEMENT_ADDRESS,                            "ERROR_ILLEGAL_ELEMENT_ADDRESS",
		"The indicated element does not exist.",
		"The indicated element does not exist."
	},
#endif // #ifdef ERROR_ILLEGAL_ELEMENT_ADDRESS
#ifdef ERROR_MAGAZINE_NOT_PRESENT
	{	ERROR_MAGAZINE_NOT_PRESENT,                               "ERROR_MAGAZINE_NOT_PRESENT",
		"The indicated element is part of a magazine that is not present.",
		"The indicated element is part of a magazine that is not present."
	},
#endif // #ifdef ERROR_MAGAZINE_NOT_PRESENT
#ifdef ERROR_DEVICE_REINITIALIZATION_NEEDED
	{	ERROR_DEVICE_REINITIALIZATION_NEEDED,                     "ERROR_DEVICE_REINITIALIZATION_NEEDED",
		"The indicated device requires reinitialization due to hardware errors.",
		"The indicated device requires reinitialization due to hardware errors."
	},
#endif // #ifdef ERROR_DEVICE_REINITIALIZATION_NEEDED
#ifdef ERROR_DEVICE_REQUIRES_CLEANING
	{	ERROR_DEVICE_REQUIRES_CLEANING,                           "ERROR_DEVICE_REQUIRES_CLEANING",
		"The device has indicated that cleaning is required before further operations are attempted.",
		"The device has indicated that cleaning is required before further operations are attempted."
	},
#endif // #ifdef ERROR_DEVICE_REQUIRES_CLEANING
#ifdef ERROR_DEVICE_DOOR_OPEN
	{	ERROR_DEVICE_DOOR_OPEN,                                   "ERROR_DEVICE_DOOR_OPEN",
		"The device has indicated that its door is open.",
		"The device has indicated that its door is open."
	},
#endif // #ifdef ERROR_DEVICE_DOOR_OPEN
#ifdef ERROR_DEVICE_NOT_CONNECTED
	{	ERROR_DEVICE_NOT_CONNECTED,                               "ERROR_DEVICE_NOT_CONNECTED",
		"The device is not connected.",
		"The device is not connected."
	},
#endif // #ifdef ERROR_DEVICE_NOT_CONNECTED
#ifdef ERROR_NOT_FOUND
	{	ERROR_NOT_FOUND,                                          "ERROR_NOT_FOUND",
		"Element not found.",
		"Element not found."
	},
#endif // #ifdef ERROR_NOT_FOUND
#ifdef ERROR_NO_MATCH
	{	ERROR_NO_MATCH,                                           "ERROR_NO_MATCH",
		"There was no match for the specified key in the index.",
		"There was no match for the specified key in the index."
	},
#endif // #ifdef ERROR_NO_MATCH
#ifdef ERROR_SET_NOT_FOUND
	{	ERROR_SET_NOT_FOUND,                                      "ERROR_SET_NOT_FOUND",
		"The property set specified does not exist on the object.",
		"The property set specified does not exist on the object."
	},
#endif // #ifdef ERROR_SET_NOT_FOUND
#ifdef ERROR_POINT_NOT_FOUND
	{	ERROR_POINT_NOT_FOUND,                                    "ERROR_POINT_NOT_FOUND",
		"The point passed to GetMouseMovePointsEx is not in the buffer.",
		"The point passed to GetMouseMovePointsEx is not in the buffer."
	},
#endif // #ifdef ERROR_POINT_NOT_FOUND
#ifdef ERROR_NO_TRACKING_SERVICE
	{	ERROR_NO_TRACKING_SERVICE,                                "ERROR_NO_TRACKING_SERVICE",
		"The tracking (workstation) service is not running.",
		"The tracking (workstation) service is not running."
	},
#endif // #ifdef ERROR_NO_TRACKING_SERVICE
#ifdef ERROR_NO_VOLUME_ID
	{	ERROR_NO_VOLUME_ID,                                       "ERROR_NO_VOLUME_ID",
		"The Volume ID could not be found.",
		"The Volume ID could not be found."
	},
#endif // #ifdef ERROR_NO_VOLUME_ID
#ifdef ERROR_UNABLE_TO_REMOVE_REPLACED
	{	ERROR_UNABLE_TO_REMOVE_REPLACED,                          "ERROR_UNABLE_TO_REMOVE_REPLACED",
		"Unable to remove the file to be replaced.",
		"Unable to remove the file to be replaced."
	},
#endif // #ifdef ERROR_UNABLE_TO_REMOVE_REPLACED
#ifdef ERROR_UNABLE_TO_MOVE_REPLACEMENT
	{	ERROR_UNABLE_TO_MOVE_REPLACEMENT,                         "ERROR_UNABLE_TO_MOVE_REPLACEMENT",
		"Unable to move the replacement file to the file to be replaced. The file to be replaced has retained its original name.",
		"Unable to move the replacement file to the file to be replaced. The file to be replaced has retained its original name."
	},
#endif // #ifdef ERROR_UNABLE_TO_MOVE_REPLACEMENT
#ifdef ERROR_UNABLE_TO_MOVE_REPLACEMENT_2
	{	ERROR_UNABLE_TO_MOVE_REPLACEMENT_2,                       "ERROR_UNABLE_TO_MOVE_REPLACEMENT_2",
		"Unable to move the replacement file to the file to be replaced. The file to be replaced has been renamed using the backup name.",
		"Unable to move the replacement file to the file to be replaced. The file to be replaced has been renamed using the backup name."
	},
#endif // #ifdef ERROR_UNABLE_TO_MOVE_REPLACEMENT_2
#ifdef ERROR_JOURNAL_DELETE_IN_PROGRESS
	{	ERROR_JOURNAL_DELETE_IN_PROGRESS,                         "ERROR_JOURNAL_DELETE_IN_PROGRESS",
		"The volume change journal is being deleted.",
		"The volume change journal is being deleted."
	},
#endif // #ifdef ERROR_JOURNAL_DELETE_IN_PROGRESS
#ifdef ERROR_JOURNAL_NOT_ACTIVE
	{	ERROR_JOURNAL_NOT_ACTIVE,                                 "ERROR_JOURNAL_NOT_ACTIVE",
		"The volume change journal is not active.",
		"The volume change journal is not active."
	},
#endif // #ifdef ERROR_JOURNAL_NOT_ACTIVE
#ifdef ERROR_POTENTIAL_FILE_FOUND
	{	ERROR_POTENTIAL_FILE_FOUND,                               "ERROR_POTENTIAL_FILE_FOUND",
		"A file was found, but it may not be the correct file.",
		"A file was found, but it may not be the correct file."
	},
#endif // #ifdef ERROR_POTENTIAL_FILE_FOUND
#ifdef ERROR_JOURNAL_ENTRY_DELETED
	{	ERROR_JOURNAL_ENTRY_DELETED,                              "ERROR_JOURNAL_ENTRY_DELETED",
		"The journal entry has been deleted from the journal.",
		"The journal entry has been deleted from the journal."
	},
#endif // #ifdef ERROR_JOURNAL_ENTRY_DELETED
#ifdef ERROR_BAD_DEVICE
	{	ERROR_BAD_DEVICE,                                         "ERROR_BAD_DEVICE",
		"The specified device name is invalid.",
		"The specified device name is invalid."
	},
#endif // #ifdef ERROR_BAD_DEVICE
#ifdef ERROR_CONNECTION_UNAVAIL
	{	ERROR_CONNECTION_UNAVAIL,                                 "ERROR_CONNECTION_UNAVAIL",
		"The device is not currently connected but it is a remembered connection.",
		"The device is not currently connected but it is a remembered connection."
	},
#endif // #ifdef ERROR_CONNECTION_UNAVAIL
#ifdef ERROR_DEVICE_ALREADY_REMEMBERED
	{	ERROR_DEVICE_ALREADY_REMEMBERED,                          "ERROR_DEVICE_ALREADY_REMEMBERED",
		"The local device name has a remembered connection to another network resource.",
		"The local device name has a remembered connection to another network resource."
	},
#endif // #ifdef ERROR_DEVICE_ALREADY_REMEMBERED
#ifdef ERROR_NO_NET_OR_BAD_PATH
	{	ERROR_NO_NET_OR_BAD_PATH,                                 "ERROR_NO_NET_OR_BAD_PATH",
		"No network provider accepted the given network path.",
		"No network provider accepted the given network path."
	},
#endif // #ifdef ERROR_NO_NET_OR_BAD_PATH
#ifdef ERROR_BAD_PROVIDER
	{	ERROR_BAD_PROVIDER,                                       "ERROR_BAD_PROVIDER",
		"The specified network provider name is invalid.",
		"The specified network provider name is invalid."
	},
#endif // #ifdef ERROR_BAD_PROVIDER
#ifdef ERROR_CANNOT_OPEN_PROFILE
	{	ERROR_CANNOT_OPEN_PROFILE,                                "ERROR_CANNOT_OPEN_PROFILE",
		"Unable to open the network connection profile.",
		"Unable to open the network connection profile."
	},
#endif // #ifdef ERROR_CANNOT_OPEN_PROFILE
#ifdef ERROR_BAD_PROFILE
	{	ERROR_BAD_PROFILE,                                        "ERROR_BAD_PROFILE",
		"The network connection profile is corrupted.",
		"The network connection profile is corrupted."
	},
#endif // #ifdef ERROR_BAD_PROFILE
#ifdef ERROR_NOT_CONTAINER
	{	ERROR_NOT_CONTAINER,                                      "ERROR_NOT_CONTAINER",
		"Cannot enumerate a noncontainer.",
		"Cannot enumerate a noncontainer."
	},
#endif // #ifdef ERROR_NOT_CONTAINER
#ifdef ERROR_EXTENDED_ERROR
	{	ERROR_EXTENDED_ERROR,                                     "ERROR_EXTENDED_ERROR",
		"An extended error has occurred.",
		"An extended error has occurred."
	},
#endif // #ifdef ERROR_EXTENDED_ERROR
#ifdef ERROR_INVALID_GROUPNAME
	{	ERROR_INVALID_GROUPNAME,                                  "ERROR_INVALID_GROUPNAME",
		"The format of the specified group name is invalid.",
		"The format of the specified group name is invalid."
	},
#endif // #ifdef ERROR_INVALID_GROUPNAME
#ifdef ERROR_INVALID_COMPUTERNAME
	{	ERROR_INVALID_COMPUTERNAME,                               "ERROR_INVALID_COMPUTERNAME",
		"The format of the specified computer name is invalid.",
		"The format of the specified computer name is invalid."
	},
#endif // #ifdef ERROR_INVALID_COMPUTERNAME
#ifdef ERROR_INVALID_EVENTNAME
	{	ERROR_INVALID_EVENTNAME,                                  "ERROR_INVALID_EVENTNAME",
		"The format of the specified event name is invalid.",
		"The format of the specified event name is invalid."
	},
#endif // #ifdef ERROR_INVALID_EVENTNAME
#ifdef ERROR_INVALID_DOMAINNAME
	{	ERROR_INVALID_DOMAINNAME,                                 "ERROR_INVALID_DOMAINNAME",
		"The format of the specified domain name is invalid.",
		"The format of the specified domain name is invalid."
	},
#endif // #ifdef ERROR_INVALID_DOMAINNAME
#ifdef ERROR_INVALID_SERVICENAME
	{	ERROR_INVALID_SERVICENAME,                                "ERROR_INVALID_SERVICENAME",
		"The format of the specified service name is invalid.",
		"The format of the specified service name is invalid."
	},
#endif // #ifdef ERROR_INVALID_SERVICENAME
#ifdef ERROR_INVALID_NETNAME
	{	ERROR_INVALID_NETNAME,                                    "ERROR_INVALID_NETNAME",
		"The format of the specified network name is invalid.",
		"The format of the specified network name is invalid."
	},
#endif // #ifdef ERROR_INVALID_NETNAME
#ifdef ERROR_INVALID_SHARENAME
	{	ERROR_INVALID_SHARENAME,                                  "ERROR_INVALID_SHARENAME",
		"The format of the specified share name is invalid.",
		"The format of the specified share name is invalid."
	},
#endif // #ifdef ERROR_INVALID_SHARENAME
#ifdef ERROR_INVALID_PASSWORDNAME
	{	ERROR_INVALID_PASSWORDNAME,                               "ERROR_INVALID_PASSWORDNAME",
		"The format of the specified password is invalid.",
		"The format of the specified password is invalid."
	},
#endif // #ifdef ERROR_INVALID_PASSWORDNAME
#ifdef ERROR_INVALID_MESSAGENAME
	{	ERROR_INVALID_MESSAGENAME,                                "ERROR_INVALID_MESSAGENAME",
		"The format of the specified message name is invalid.",
		"The format of the specified message name is invalid."
	},
#endif // #ifdef ERROR_INVALID_MESSAGENAME
#ifdef ERROR_INVALID_MESSAGEDEST
	{	ERROR_INVALID_MESSAGEDEST,                                "ERROR_INVALID_MESSAGEDEST",
		"The format of the specified message destination is invalid.",
		"The format of the specified message destination is invalid."
	},
#endif // #ifdef ERROR_INVALID_MESSAGEDEST
#ifdef ERROR_SESSION_CREDENTIAL_CONFLICT
	{	ERROR_SESSION_CREDENTIAL_CONFLICT,                        "ERROR_SESSION_CREDENTIAL_CONFLICT",
		"The credentials supplied conflict with an existing set of credentials.",
		"Multiple connections to a server or shared resource by the same user, using more than one user name, are not allowed. Disconnect all previous connections to the server or shared resource and try again."
	},
#endif // #ifdef ERROR_SESSION_CREDENTIAL_CONFLICT
#ifdef ERROR_REMOTE_SESSION_LIMIT_EXCEEDED
	{	ERROR_REMOTE_SESSION_LIMIT_EXCEEDED,                      "ERROR_REMOTE_SESSION_LIMIT_EXCEEDED",
		"An attempt was made to establish a session to a network server, but there are already too many sessions established to that server.",
		"An attempt was made to establish a session to a network server, but there are already too many sessions established to that server."
	},
#endif // #ifdef ERROR_REMOTE_SESSION_LIMIT_EXCEEDED
#ifdef ERROR_DUP_DOMAINNAME
	{	ERROR_DUP_DOMAINNAME,                                     "ERROR_DUP_DOMAINNAME",
		"The workgroup or domain name is already in use by another computer on the network.",
		"The workgroup or domain name is already in use by another computer on the network."
	},
#endif // #ifdef ERROR_DUP_DOMAINNAME
#ifdef ERROR_NO_NETWORK
	{	ERROR_NO_NETWORK,                                         "ERROR_NO_NETWORK",
		"The network is not present or not started.",
		"The network is not present or not started."
	},
#endif // #ifdef ERROR_NO_NETWORK
#ifdef ERROR_CANCELLED
	{	ERROR_CANCELLED,                                          "ERROR_CANCELLED",
		"The operation was canceled by the user.",
		"The operation was canceled by the user."
	},
#endif // #ifdef ERROR_CANCELLED
#ifdef ERROR_USER_MAPPED_FILE
	{	ERROR_USER_MAPPED_FILE,                                   "ERROR_USER_MAPPED_FILE",
		"The requested operation cannot be performed on a file with a user-mapped section open.",
		"The requested operation cannot be performed on a file with a user-mapped section open."
	},
#endif // #ifdef ERROR_USER_MAPPED_FILE
#ifdef ERROR_CONNECTION_REFUSED
	{	ERROR_CONNECTION_REFUSED,                                 "ERROR_CONNECTION_REFUSED",
		"The remote system refused the network connection.",
		"The remote system refused the network connection."
	},
#endif // #ifdef ERROR_CONNECTION_REFUSED
#ifdef ERROR_GRACEFUL_DISCONNECT
	{	ERROR_GRACEFUL_DISCONNECT,                                "ERROR_GRACEFUL_DISCONNECT",
		"The network connection was gracefully closed.",
		"The network connection was gracefully closed."
	},
#endif // #ifdef ERROR_GRACEFUL_DISCONNECT
#ifdef ERROR_ADDRESS_ALREADY_ASSOCIATED
	{	ERROR_ADDRESS_ALREADY_ASSOCIATED,                         "ERROR_ADDRESS_ALREADY_ASSOCIATED",
		"The network transport endpoint already has an address associated with it.",
		"The network transport endpoint already has an address associated with it."
	},
#endif // #ifdef ERROR_ADDRESS_ALREADY_ASSOCIATED
#ifdef ERROR_ADDRESS_NOT_ASSOCIATED
	{	ERROR_ADDRESS_NOT_ASSOCIATED,                             "ERROR_ADDRESS_NOT_ASSOCIATED",
		"An address has not yet been associated with the network endpoint.",
		"An address has not yet been associated with the network endpoint."
	},
#endif // #ifdef ERROR_ADDRESS_NOT_ASSOCIATED
#ifdef ERROR_CONNECTION_INVALID
	{	ERROR_CONNECTION_INVALID,                                 "ERROR_CONNECTION_INVALID",
		"An operation was attempted on a nonexistent network connection.",
		"An operation was attempted on a nonexistent network connection."
	},
#endif // #ifdef ERROR_CONNECTION_INVALID
#ifdef ERROR_CONNECTION_ACTIVE
	{	ERROR_CONNECTION_ACTIVE,                                  "ERROR_CONNECTION_ACTIVE",
		"An invalid operation was attempted on an active network connection.",
		"An invalid operation was attempted on an active network connection."
	},
#endif // #ifdef ERROR_CONNECTION_ACTIVE
#ifdef ERROR_NETWORK_UNREACHABLE
	{	ERROR_NETWORK_UNREACHABLE,                                "ERROR_NETWORK_UNREACHABLE",
		"The remote network is not reachable by the transport.",
		"The network location cannot be reached. For information about network troubleshooting, see Windows Help."
	},
#endif // #ifdef ERROR_NETWORK_UNREACHABLE
#ifdef ERROR_HOST_UNREACHABLE
	{	ERROR_HOST_UNREACHABLE,                                   "ERROR_HOST_UNREACHABLE",
		"The remote system is not reachable by the transport.",
		"The network location cannot be reached. For information about network troubleshooting, see Windows Help."
	},
#endif // #ifdef ERROR_HOST_UNREACHABLE
#ifdef ERROR_PROTOCOL_UNREACHABLE
	{	ERROR_PROTOCOL_UNREACHABLE,                               "ERROR_PROTOCOL_UNREACHABLE",
		"The remote system does not support the transport protocol.",
		"The network location cannot be reached. For information about network troubleshooting, see Windows Help."
	},
#endif // #ifdef ERROR_PROTOCOL_UNREACHABLE
#ifdef ERROR_PORT_UNREACHABLE
	{	ERROR_PORT_UNREACHABLE,                                   "ERROR_PORT_UNREACHABLE",
		"No service is operating at the destination network endpoint on the remote system.",
		"No service is operating at the destination network endpoint on the remote system."
	},
#endif // #ifdef ERROR_PORT_UNREACHABLE
#ifdef ERROR_REQUEST_ABORTED
	{	ERROR_REQUEST_ABORTED,                                    "ERROR_REQUEST_ABORTED",
		"The request was aborted.",
		"The request was aborted."
	},
#endif // #ifdef ERROR_REQUEST_ABORTED
#ifdef ERROR_CONNECTION_ABORTED
	{	ERROR_CONNECTION_ABORTED,                                 "ERROR_CONNECTION_ABORTED",
		"The network connection was aborted by the local system.",
		"The network connection was aborted by the local system."
	},
#endif // #ifdef ERROR_CONNECTION_ABORTED
#ifdef ERROR_RETRY
	{	ERROR_RETRY,                                              "ERROR_RETRY",
		"The operation could not be completed. A retry should be performed.",
		"The operation could not be completed. A retry should be performed."
	},
#endif // #ifdef ERROR_RETRY
#ifdef ERROR_CONNECTION_COUNT_LIMIT
	{	ERROR_CONNECTION_COUNT_LIMIT,                             "ERROR_CONNECTION_COUNT_LIMIT",
		"A connection to the server could not be made because the limit on the number of concurrent connections for this account has been reached.",
		"A connection to the server could not be made because the limit on the number of concurrent connections for this account has been reached."
	},
#endif // #ifdef ERROR_CONNECTION_COUNT_LIMIT
#ifdef ERROR_LOGIN_TIME_RESTRICTION
	{	ERROR_LOGIN_TIME_RESTRICTION,                             "ERROR_LOGIN_TIME_RESTRICTION",
		"Attempting to log in during an unauthorized time of day for this account.",
		"Attempting to log in during an unauthorized time of day for this account."
	},
#endif // #ifdef ERROR_LOGIN_TIME_RESTRICTION
#ifdef ERROR_LOGIN_WKSTA_RESTRICTION
	{	ERROR_LOGIN_WKSTA_RESTRICTION,                            "ERROR_LOGIN_WKSTA_RESTRICTION",
		"The account is not authorized to log in from this station.",
		"The account is not authorized to log in from this station."
	},
#endif // #ifdef ERROR_LOGIN_WKSTA_RESTRICTION
#ifdef ERROR_INCORRECT_ADDRESS
	{	ERROR_INCORRECT_ADDRESS,                                  "ERROR_INCORRECT_ADDRESS",
		"The network address could not be used for the operation requested.",
		"The network address could not be used for the operation requested."
	},
#endif // #ifdef ERROR_INCORRECT_ADDRESS
#ifdef ERROR_ALREADY_REGISTERED
	{	ERROR_ALREADY_REGISTERED,                                 "ERROR_ALREADY_REGISTERED",
		"The service is already registered.",
		"The service is already registered."
	},
#endif // #ifdef ERROR_ALREADY_REGISTERED
#ifdef ERROR_SERVICE_NOT_FOUND
	{	ERROR_SERVICE_NOT_FOUND,                                  "ERROR_SERVICE_NOT_FOUND",
		"The specified service does not exist.",
		"The specified service does not exist."
	},
#endif // #ifdef ERROR_SERVICE_NOT_FOUND
#ifdef ERROR_NOT_AUTHENTICATED
	{	ERROR_NOT_AUTHENTICATED,                                  "ERROR_NOT_AUTHENTICATED",
		"The operation being requested was not performed because the user has not been authenticated.",
		"The operation being requested was not performed because the user has not been authenticated."
	},
#endif // #ifdef ERROR_NOT_AUTHENTICATED
#ifdef ERROR_NOT_LOGGED_ON
	{	ERROR_NOT_LOGGED_ON,                                      "ERROR_NOT_LOGGED_ON",
		"The operation being requested was not performed because the user has not logged on to the network. The specified service does not exist.",
		"The operation being requested was not performed because the user has not logged on to the network. The specified service does not exist."
	},
#endif // #ifdef ERROR_NOT_LOGGED_ON
#ifdef ERROR_CONTINUE
	{	ERROR_CONTINUE,                                           "ERROR_CONTINUE",
		"Continue with work in progress.",
		"Continue with work in progress."
	},
#endif // #ifdef ERROR_CONTINUE
#ifdef ERROR_ALREADY_INITIALIZED
	{	ERROR_ALREADY_INITIALIZED,                                "ERROR_ALREADY_INITIALIZED",
		"An attempt was made to perform an initialization operation when initialization has already been completed.",
		"An attempt was made to perform an initialization operation when initialization has already been completed."
	},
#endif // #ifdef ERROR_ALREADY_INITIALIZED
#ifdef ERROR_NO_MORE_DEVICES
	{	ERROR_NO_MORE_DEVICES,                                    "ERROR_NO_MORE_DEVICES",
		"No more local devices.",
		"No more local devices."
	},
#endif // #ifdef ERROR_NO_MORE_DEVICES
#ifdef ERROR_NO_SUCH_SITE
	{	ERROR_NO_SUCH_SITE,                                       "ERROR_NO_SUCH_SITE",
		"The specified site does not exist.",
		"The specified site does not exist."
	},
#endif // #ifdef ERROR_NO_SUCH_SITE
#ifdef ERROR_DOMAIN_CONTROLLER_EXISTS
	{	ERROR_DOMAIN_CONTROLLER_EXISTS,                           "ERROR_DOMAIN_CONTROLLER_EXISTS",
		"A domain controller with the specified name already exists.",
		"A domain controller with the specified name already exists."
	},
#endif // #ifdef ERROR_DOMAIN_CONTROLLER_EXISTS
#ifdef ERROR_ONLY_IF_CONNECTED
	{	ERROR_ONLY_IF_CONNECTED,                                  "ERROR_ONLY_IF_CONNECTED",
		"This operation is supported only when you are connected to the server.",
		"This operation is supported only when you are connected to the server."
	},
#endif // #ifdef ERROR_ONLY_IF_CONNECTED
#ifdef ERROR_OVERRIDE_NOCHANGES
	{	ERROR_OVERRIDE_NOCHANGES,                                 "ERROR_OVERRIDE_NOCHANGES",
		"The group policy framework should call the extension even if there are no changes.",
		"The group policy framework should call the extension even if there are no changes."
	},
#endif // #ifdef ERROR_OVERRIDE_NOCHANGES
#ifdef ERROR_BAD_USER_PROFILE
	{	ERROR_BAD_USER_PROFILE,                                   "ERROR_BAD_USER_PROFILE",
		"The specified user does not have a valid profile.",
		"The specified user does not have a valid profile."
	},
#endif // #ifdef ERROR_BAD_USER_PROFILE
#ifdef ERROR_NOT_SUPPORTED_ON_SBS
	{	ERROR_NOT_SUPPORTED_ON_SBS,                               "ERROR_NOT_SUPPORTED_ON_SBS",
		"This operation is not supported on a Microsoft Small Business Server.",
		"This operation is not supported on a Microsoft Small Business Server."
	},
#endif // #ifdef ERROR_NOT_SUPPORTED_ON_SBS
#ifdef ERROR_SERVER_SHUTDOWN_IN_PROGRESS
	{	ERROR_SERVER_SHUTDOWN_IN_PROGRESS,                        "ERROR_SERVER_SHUTDOWN_IN_PROGRESS",
		"The server machine is shutting down.",
		"The server machine is shutting down."
	},
#endif // #ifdef ERROR_SERVER_SHUTDOWN_IN_PROGRESS
#ifdef ERROR_HOST_DOWN
	{	ERROR_HOST_DOWN,                                          "ERROR_HOST_DOWN",
		"The remote system is not available. For information about network troubleshooting, see Windows Help.",
		"The remote system is not available. For information about network troubleshooting, see Windows Help."
	},
#endif // #ifdef ERROR_HOST_DOWN
#ifdef ERROR_NON_ACCOUNT_SID
	{	ERROR_NON_ACCOUNT_SID,                                    "ERROR_NON_ACCOUNT_SID",
		"The security identifier provided is not from an account domain.",
		"The security identifier provided is not from an account domain."
	},
#endif // #ifdef ERROR_NON_ACCOUNT_SID
#ifdef ERROR_NON_DOMAIN_SID
	{	ERROR_NON_DOMAIN_SID,                                     "ERROR_NON_DOMAIN_SID",
		"The security identifier provided does not have a domain component.",
		"The security identifier provided does not have a domain component."
	},
#endif // #ifdef ERROR_NON_DOMAIN_SID
#ifdef ERROR_APPHELP_BLOCK
	{	ERROR_APPHELP_BLOCK,                                      "ERROR_APPHELP_BLOCK",
		"AppHelp dialog canceled thus preventing the application from starting.",
		"AppHelp dialog canceled thus preventing the application from starting."
	},
#endif // #ifdef ERROR_APPHELP_BLOCK
#ifdef ERROR_ACCESS_DISABLED_BY_POLICY
	{	ERROR_ACCESS_DISABLED_BY_POLICY,                          "ERROR_ACCESS_DISABLED_BY_POLICY",
		"Windows cannot open this program because it has been prevented by a software restriction policy. For more information, open Event Viewer or contact your system administrator.",
		"Windows cannot open this program because it has been prevented by a software restriction policy. For more information, open Event Viewer or contact your system administrator."
	},
#endif // #ifdef ERROR_ACCESS_DISABLED_BY_POLICY
#ifdef ERROR_REG_NAT_CONSUMPTION
	{	ERROR_REG_NAT_CONSUMPTION,                                "ERROR_REG_NAT_CONSUMPTION",
		"A program attempt to use an invalid register value. Normally caused by an uninitialized register. This error is Itanium specific.",
		"A program attempt to use an invalid register value. Normally caused by an uninitialized register. This error is Itanium specific."
	},
#endif // #ifdef ERROR_REG_NAT_CONSUMPTION
#ifdef ERROR_CSCSHARE_OFFLINE
	{	ERROR_CSCSHARE_OFFLINE,                                   "ERROR_CSCSHARE_OFFLINE",
		"The share is currently offline or does not exist.",
		"The share is currently offline or does not exist."
	},
#endif // #ifdef ERROR_CSCSHARE_OFFLINE
#ifdef ERROR_PKINIT_FAILURE
	{	ERROR_PKINIT_FAILURE,                                     "ERROR_PKINIT_FAILURE",
		"The kerberos protocol encountered an error while validating the KDC certificate during smartcard logon.",
		"The kerberos protocol encountered an error while validating the KDC certificate during smartcard logon."
	},
#endif // #ifdef ERROR_PKINIT_FAILURE
#ifdef ERROR_SMARTCARD_SUBSYSTEM_FAILURE
	{	ERROR_SMARTCARD_SUBSYSTEM_FAILURE,                        "ERROR_SMARTCARD_SUBSYSTEM_FAILURE",
		"The kerberos protocol encountered an error while attempting to utilize the smartcard subsystem.",
		"The kerberos protocol encountered an error while attempting to utilize the smartcard subsystem."
	},
#endif // #ifdef ERROR_SMARTCARD_SUBSYSTEM_FAILURE
#ifdef ERROR_DOWNGRADE_DETECTED
	{	ERROR_DOWNGRADE_DETECTED,                                 "ERROR_DOWNGRADE_DETECTED",
		"The system detected a possible attempt to compromise security. Please ensure that you can contact the server that authenticated you.",
		"The system detected a possible attempt to compromise security. Please ensure that you can contact the server that authenticated you."
	},
#endif // #ifdef ERROR_DOWNGRADE_DETECTED
#ifdef SEC_E_SMARTCARD_CERT_REVOKED
	{	static_cast<SystemErrorCode>(SEC_E_SMARTCARD_CERT_REVOKED),                             "SEC_E_SMARTCARD_CERT_REVOKED",
		"The smartcard certificate used for authentication has been revoked. Please contact your system administrator. There may be additional information in the event log.",
		"The smartcard certificate used for authentication has been revoked. Please contact your system administrator. There may be additional information in the event log."
	},
#endif // #ifdef SEC_E_SMARTCARD_CERT_REVOKED
#ifdef SEC_E_ISSUING_CA_UNTRUSTED
	{	static_cast<SystemErrorCode>(SEC_E_ISSUING_CA_UNTRUSTED),                               "SEC_E_ISSUING_CA_UNTRUSTED",
		"An untrusted certificate authority was detected while processing the smartcard certificate used for authentication. Please contact your system administrator.",
		"An untrusted certificate authority was detected while processing the smartcard certificate used for authentication. Please contact your system administrator."
	},
#endif // #ifdef SEC_E_ISSUING_CA_UNTRUSTED
#ifdef SEC_E_REVOCATION_OFFLINE_C
	{	static_cast<SystemErrorCode>(SEC_E_REVOCATION_OFFLINE_C),                               "SEC_E_REVOCATION_OFFLINE_C",
		"The revocation status of the smartcard certificate used for authentication could not be determined. Please contact your system administrator.",
		"The revocation status of the smartcard certificate used for authentication could not be determined. Please contact your system administrator."
	},
#endif // #ifdef SEC_E_REVOCATION_OFFLINE_C
#ifdef SEC_E_PKINIT_CLIENT_FAILUR
	{	static_cast<SystemErrorCode>(SEC_E_PKINIT_CLIENT_FAILUR),                               "SEC_E_PKINIT_CLIENT_FAILUR",
		"The smartcard certificate used for authentication was not trusted. Please contact your system administrator.",
		"The smartcard certificate used for authentication was not trusted. Please contact your system administrator."
	},
#endif // #ifdef SEC_E_PKINIT_CLIENT_FAILUR
#ifdef SEC_E_SMARTCARD_CERT_EXPIRED
	{	static_cast<SystemErrorCode>(SEC_E_SMARTCARD_CERT_EXPIRED),                             "SEC_E_SMARTCARD_CERT_EXPIRED",
		"The smartcard certificate used for authentication has expired. Please contact your system administrator.",
		"The smartcard certificate used for authentication has expired. Please contact your system administrator."
	},
#endif // #ifdef SEC_E_SMARTCARD_CERT_EXPIRED
#ifdef ERROR_MACHINE_LOCKED
	{	ERROR_MACHINE_LOCKED,                                     "ERROR_MACHINE_LOCKED",
		"The machine is locked and cannot be shut down without the force option.",
		"The machine is locked and cannot be shut down without the force option."
	},
#endif // #ifdef ERROR_MACHINE_LOCKED
#ifdef ERROR_CALLBACK_SUPPLIED_INVALID_DATA
	{	ERROR_CALLBACK_SUPPLIED_INVALID_DATA,                     "ERROR_CALLBACK_SUPPLIED_INVALID_DATA",
		"An application-defined callback gave invalid data when called.",
		"An application-defined callback gave invalid data when called."
	},
#endif // #ifdef ERROR_CALLBACK_SUPPLIED_INVALID_DATA
#ifdef ERROR_SYNC_FOREGROUND_REFRESH_REQUIRED
	{	ERROR_SYNC_FOREGROUND_REFRESH_REQUIRED,                   "ERROR_SYNC_FOREGROUND_REFRESH_REQUIRED",
		"The group policy framework should call the extension in the synchronous foreground policy refresh.",
		"The group policy framework should call the extension in the synchronous foreground policy refresh."
	},
#endif // #ifdef ERROR_SYNC_FOREGROUND_REFRESH_REQUIRED
#ifdef ERROR_DRIVER_BLOCKED
	{	ERROR_DRIVER_BLOCKED,                                     "ERROR_DRIVER_BLOCKED",
		"This driver has been blocked from loading.",
		"This driver has been blocked from loading."
	},
#endif // #ifdef ERROR_DRIVER_BLOCKED
#ifdef ERROR_INVALID_IMPORT_OF_NON_DLL
	{	ERROR_INVALID_IMPORT_OF_NON_DLL,                          "ERROR_INVALID_IMPORT_OF_NON_DLL",
		"A dynamic link library (DLL) referenced a module that was neither a DLL nor the process's executable image.",
		"A dynamic link library (DLL) referenced a module that was neither a DLL nor the process's executable image."
	},
#endif // #ifdef ERROR_INVALID_IMPORT_OF_NON_DLL
#ifdef ERROR_ACCESS_DISABLED_WEBBLADE
	{	ERROR_ACCESS_DISABLED_WEBBLADE,                           "ERROR_ACCESS_DISABLED_WEBBLADE",
		"Windows cannot open this program since it has been disabled.",
		"Windows cannot open this program since it has been disabled."
	},
#endif // #ifdef ERROR_ACCESS_DISABLED_WEBBLADE
#ifdef ERROR_ACCESS_DISABLED_WEBBLADE_TAMPER
	{	ERROR_ACCESS_DISABLED_WEBBLADE_TAMPER,                    "ERROR_ACCESS_DISABLED_WEBBLADE_TAMPER",
		"Windows cannot open this program because the license enforcement system has been tampered with or become corrupted.",
		"Windows cannot open this program because the license enforcement system has been tampered with or become corrupted."
	},
#endif // #ifdef ERROR_ACCESS_DISABLED_WEBBLADE_TAMPER
#ifdef ERROR_RECOVERY_FAILURE
	{	ERROR_RECOVERY_FAILURE,                                   "ERROR_RECOVERY_FAILURE",
		"A transaction recovery failed.",
		"A transaction recovery failed."
	},
#endif // #ifdef ERROR_RECOVERY_FAILURE
#ifdef ERROR_ALREADY_FIBER
	{	ERROR_ALREADY_FIBER,                                      "ERROR_ALREADY_FIBER",
		"The current thread has already been converted to a fiber.",
		"The current thread has already been converted to a fiber."
	},
#endif // #ifdef ERROR_ALREADY_FIBER
#ifdef ERROR_ALREADY_THREAD
	{	ERROR_ALREADY_THREAD,                                     "ERROR_ALREADY_THREAD",
		"The current thread has already been converted from a fiber.",
		"The current thread has already been converted from a fiber."
	},
#endif // #ifdef ERROR_ALREADY_THREAD
#ifdef ERROR_STACK_BUFFER_OVERRUN
	{	ERROR_STACK_BUFFER_OVERRUN,                               "ERROR_STACK_BUFFER_OVERRUN",
		"The system detected an overrun of a stack-based buffer in this application. This overrun could potentially allow a malicious user to gain control of this application.",
		"The system detected an overrun of a stack-based buffer in this application. This overrun could potentially allow a malicious user to gain control of this application."
	},
#endif // #ifdef ERROR_STACK_BUFFER_OVERRUN
#ifdef ERROR_PARAMETER_QUOTA_EXCEEDED
	{	ERROR_PARAMETER_QUOTA_EXCEEDED,                           "ERROR_PARAMETER_QUOTA_EXCEEDED",
		"Data present in one of the parameters is more than the function can operate on.",
		"Data present in one of the parameters is more than the function can operate on."
	},
#endif // #ifdef ERROR_PARAMETER_QUOTA_EXCEEDED
#ifdef ERROR_DEBUGGER_INACTIVE
	{	ERROR_DEBUGGER_INACTIVE,                                  "ERROR_DEBUGGER_INACTIVE",
		"An attempt to do an operation on a debug object failed because the object is in the process of being deleted.",
		"An attempt to do an operation on a debug object failed because the object is in the process of being deleted."
	},
#endif // #ifdef ERROR_DEBUGGER_INACTIVE
#ifdef ERROR_NOT_ALL_ASSIGNED
	{	ERROR_NOT_ALL_ASSIGNED,                                   "ERROR_NOT_ALL_ASSIGNED",
		"Not all privileges referenced are assigned to the caller.",
		"Not all privileges referenced are assigned to the caller."
	},
#endif // #ifdef ERROR_NOT_ALL_ASSIGNED
#ifdef ERROR_SOME_NOT_MAPPED
	{	ERROR_SOME_NOT_MAPPED,                                    "ERROR_SOME_NOT_MAPPED",
		"Some mapping between account names and security IDs was not done.",
		"Some mapping between account names and security IDs was not done."
	},
#endif // #ifdef ERROR_SOME_NOT_MAPPED
#ifdef ERROR_NO_QUOTAS_FOR_ACCOUNT
	{	ERROR_NO_QUOTAS_FOR_ACCOUNT,                              "ERROR_NO_QUOTAS_FOR_ACCOUNT",
		"No system quota limits are specifically set for this account.",
		"No system quota limits are specifically set for this account."
	},
#endif // #ifdef ERROR_NO_QUOTAS_FOR_ACCOUNT
#ifdef ERROR_LOCAL_USER_SESSION_KEY
	{	ERROR_LOCAL_USER_SESSION_KEY,                             "ERROR_LOCAL_USER_SESSION_KEY",
		"No encryption key is available. A well-known encryption key was returned.",
		"No encryption key is available. A well-known encryption key was returned."
	},
#endif // #ifdef ERROR_LOCAL_USER_SESSION_KEY
#ifdef ERROR_NULL_LM_PASSWORD
	{	ERROR_NULL_LM_PASSWORD,                                   "ERROR_NULL_LM_PASSWORD",
		"The password is too complex to be converted to a LAN Manager password. The LAN Manager password returned is a NULL string.",
		"The password is too complex to be converted to a LAN Manager password. The LAN Manager password returned is a NULL string."
	},
#endif // #ifdef ERROR_NULL_LM_PASSWORD
#ifdef ERROR_UNKNOWN_REVISION
	{	ERROR_UNKNOWN_REVISION,                                   "ERROR_UNKNOWN_REVISION",
		"The revision level is unknown.",
		"The revision level is unknown."
	},
#endif // #ifdef ERROR_UNKNOWN_REVISION
#ifdef ERROR_REVISION_MISMATCH
	{	ERROR_REVISION_MISMATCH,                                  "ERROR_REVISION_MISMATCH",
		"Indicates two revision levels are incompatible.",
		"Indicates two revision levels are incompatible."
	},
#endif // #ifdef ERROR_REVISION_MISMATCH
#ifdef ERROR_INVALID_OWNER
	{	ERROR_INVALID_OWNER,                                      "ERROR_INVALID_OWNER",
		"This security ID may not be assigned as the owner of this object.",
		"This security ID may not be assigned as the owner of this object."
	},
#endif // #ifdef ERROR_INVALID_OWNER
#ifdef ERROR_INVALID_PRIMARY_GROUP
	{	ERROR_INVALID_PRIMARY_GROUP,                              "ERROR_INVALID_PRIMARY_GROUP",
		"This security ID may not be assigned as the primary group of an object.",
		"This security ID may not be assigned as the primary group of an object."
	},
#endif // #ifdef ERROR_INVALID_PRIMARY_GROUP
#ifdef ERROR_NO_IMPERSONATION_TOKEN
	{	ERROR_NO_IMPERSONATION_TOKEN,                             "ERROR_NO_IMPERSONATION_TOKEN",
		"An attempt has been made to operate on an impersonation token by a thread that is not currently impersonating a client.",
		"An attempt has been made to operate on an impersonation token by a thread that is not currently impersonating a client."
	},
#endif // #ifdef ERROR_NO_IMPERSONATION_TOKEN
#ifdef ERROR_CANT_DISABLE_MANDATORY
	{	ERROR_CANT_DISABLE_MANDATORY,                             "ERROR_CANT_DISABLE_MANDATORY",
		"The group may not be disabled.",
		"The group may not be disabled."
	},
#endif // #ifdef ERROR_CANT_DISABLE_MANDATORY
#ifdef ERROR_NO_LOGON_SERVERS
	{	ERROR_NO_LOGON_SERVERS,                                   "ERROR_NO_LOGON_SERVERS",
		"There are currently no logon servers available to service the logon request.",
		"There are currently no logon servers available to service the logon request."
	},
#endif // #ifdef ERROR_NO_LOGON_SERVERS
#ifdef ERROR_NO_SUCH_LOGON_SESSION
	{	ERROR_NO_SUCH_LOGON_SESSION,                              "ERROR_NO_SUCH_LOGON_SESSION",
		"A specified logon session does not exist. It may already have been terminated.",
		"A specified logon session does not exist. It may already have been terminated."
	},
#endif // #ifdef ERROR_NO_SUCH_LOGON_SESSION
#ifdef ERROR_NO_SUCH_PRIVILEGE
	{	ERROR_NO_SUCH_PRIVILEGE,                                  "ERROR_NO_SUCH_PRIVILEGE",
		"A specified privilege does not exist.",
		"A specified privilege does not exist."
	},
#endif // #ifdef ERROR_NO_SUCH_PRIVILEGE
#ifdef ERROR_PRIVILEGE_NOT_HELD
	{	ERROR_PRIVILEGE_NOT_HELD,                                 "ERROR_PRIVILEGE_NOT_HELD",
		"A required privilege is not held by the client.",
		"A required privilege is not held by the client."
	},
#endif // #ifdef ERROR_PRIVILEGE_NOT_HELD
#ifdef ERROR_INVALID_ACCOUNT_NAME
	{	ERROR_INVALID_ACCOUNT_NAME,                               "ERROR_INVALID_ACCOUNT_NAME",
		"The name provided is not a properly formed account name.",
		"The name provided is not a properly formed account name."
	},
#endif // #ifdef ERROR_INVALID_ACCOUNT_NAME
#ifdef ERROR_USER_EXISTS
	{	ERROR_USER_EXISTS,                                        "ERROR_USER_EXISTS",
		"The specified user already exists.",
		"The specified user already exists."
	},
#endif // #ifdef ERROR_USER_EXISTS
#ifdef ERROR_NO_SUCH_USER
	{	ERROR_NO_SUCH_USER,                                       "ERROR_NO_SUCH_USER",
		"The specified user does not exist.",
		"The specified user does not exist."
	},
#endif // #ifdef ERROR_NO_SUCH_USER
#ifdef ERROR_GROUP_EXISTS
	{	ERROR_GROUP_EXISTS,                                       "ERROR_GROUP_EXISTS",
		"The specified group already exists.",
		"The specified group already exists."
	},
#endif // #ifdef ERROR_GROUP_EXISTS
#ifdef ERROR_NO_SUCH_GROUP
	{	ERROR_NO_SUCH_GROUP,                                      "ERROR_NO_SUCH_GROUP",
		"The specified group does not exist.",
		"The specified group does not exist."
	},
#endif // #ifdef ERROR_NO_SUCH_GROUP
#ifdef ERROR_MEMBER_IN_GROUP
	{	ERROR_MEMBER_IN_GROUP,                                    "ERROR_MEMBER_IN_GROUP",
		"Either the specified user account is already a member of the specified group, or the specified group cannot be deleted because it contains a member.",
		"Either the specified user account is already a member of the specified group, or the specified group cannot be deleted because it contains a member."
	},
#endif // #ifdef ERROR_MEMBER_IN_GROUP
#ifdef ERROR_MEMBER_NOT_IN_GROUP
	{	ERROR_MEMBER_NOT_IN_GROUP,                                "ERROR_MEMBER_NOT_IN_GROUP",
		"The specified user account is not a member of the specified group account.",
		"The specified user account is not a member of the specified group account."
	},
#endif // #ifdef ERROR_MEMBER_NOT_IN_GROUP
#ifdef ERROR_LAST_ADMIN
	{	ERROR_LAST_ADMIN,                                         "ERROR_LAST_ADMIN",
		"The last remaining administration account cannot be disabled or deleted.",
		"The last remaining administration account cannot be disabled or deleted."
	},
#endif // #ifdef ERROR_LAST_ADMIN
#ifdef ERROR_WRONG_PASSWORD
	{	ERROR_WRONG_PASSWORD,                                     "ERROR_WRONG_PASSWORD",
		"Unable to update the password. The value provided as the current password is incorrect.",
		"Unable to update the password. The value provided as the current password is incorrect."
	},
#endif // #ifdef ERROR_WRONG_PASSWORD
#ifdef ERROR_ILL_FORMED_PASSWORD
	{	ERROR_ILL_FORMED_PASSWORD,                                "ERROR_ILL_FORMED_PASSWORD",
		"Unable to update the password. The value provided for the new password contains values that are not allowed in passwords.",
		"Unable to update the password. The value provided for the new password contains values that are not allowed in passwords."
	},
#endif // #ifdef ERROR_ILL_FORMED_PASSWORD
#ifdef ERROR_PASSWORD_RESTRICTION
	{	ERROR_PASSWORD_RESTRICTION,                               "ERROR_PASSWORD_RESTRICTION",
		"Unable to update the password. The value provided for the new password does not meet the length, complexity, or history requirement of the domain.",
		"Unable to update the password. The value provided for the new password does not meet the length, complexity, or history requirement of the domain."
	},
#endif // #ifdef ERROR_PASSWORD_RESTRICTION
#ifdef ERROR_LOGON_FAILURE
	{	ERROR_LOGON_FAILURE,                                      "ERROR_LOGON_FAILURE",
		"Logon failure: unknown user name or bad password.",
		"Logon failure: unknown user name or bad password."
	},
#endif // #ifdef ERROR_LOGON_FAILURE
#ifdef ERROR_ACCOUNT_RESTRICTION
	{	ERROR_ACCOUNT_RESTRICTION,                                "ERROR_ACCOUNT_RESTRICTION",
		"Logon failure: user account restriction.",
		"Logon failure: user account restriction. Possible reasons are blank passwords not allowed, logon hour restrictions, or a policy restriction has been enforced."
	},
#endif // #ifdef ERROR_ACCOUNT_RESTRICTION
#ifdef ERROR_INVALID_LOGON_HOURS
	{	ERROR_INVALID_LOGON_HOURS,                                "ERROR_INVALID_LOGON_HOURS",
		"Logon failure: account logon time restriction violation.",
		"Logon failure: account logon time restriction violation."
	},
#endif // #ifdef ERROR_INVALID_LOGON_HOURS
#ifdef ERROR_INVALID_WORKSTATION
	{	ERROR_INVALID_WORKSTATION,                                "ERROR_INVALID_WORKSTATION",
		"Logon failure: user not allowed to log on to this computer.",
		"Logon failure: user not allowed to log on to this computer."
	},
#endif // #ifdef ERROR_INVALID_WORKSTATION
#ifdef ERROR_PASSWORD_EXPIRED
	{	ERROR_PASSWORD_EXPIRED,                                   "ERROR_PASSWORD_EXPIRED",
		"Logon failure: the specified account password has expired.",
		"Logon failure: the specified account password has expired."
	},
#endif // #ifdef ERROR_PASSWORD_EXPIRED
#ifdef ERROR_ACCOUNT_DISABLED
	{	ERROR_ACCOUNT_DISABLED,                                   "ERROR_ACCOUNT_DISABLED",
		"Logon failure: account currently disabled.",
		"Logon failure: account currently disabled."
	},
#endif // #ifdef ERROR_ACCOUNT_DISABLED
#ifdef ERROR_NONE_MAPPED
	{	ERROR_NONE_MAPPED,                                        "ERROR_NONE_MAPPED",
		"No mapping between account names and security IDs was done.",
		"No mapping between account names and security IDs was done."
	},
#endif // #ifdef ERROR_NONE_MAPPED
#ifdef ERROR_TOO_MANY_LUIDS_REQUESTED
	{	ERROR_TOO_MANY_LUIDS_REQUESTED,                           "ERROR_TOO_MANY_LUIDS_REQUESTED",
		"Too many local user identifiers (LUIDs) were requested at one time.",
		"Too many local user identifiers (LUIDs) were requested at one time."
	},
#endif // #ifdef ERROR_TOO_MANY_LUIDS_REQUESTED
#ifdef ERROR_LUIDS_EXHAUSTED
	{	ERROR_LUIDS_EXHAUSTED,                                    "ERROR_LUIDS_EXHAUSTED",
		"No more local user identifiers (LUIDs) are available.",
		"No more local user identifiers (LUIDs) are available."
	},
#endif // #ifdef ERROR_LUIDS_EXHAUSTED
#ifdef ERROR_INVALID_SUB_AUTHORITY
	{	ERROR_INVALID_SUB_AUTHORITY,                              "ERROR_INVALID_SUB_AUTHORITY",
		"The subauthority part of a security ID is invalid for this particular use.",
		"The subauthority part of a security ID is invalid for this particular use."
	},
#endif // #ifdef ERROR_INVALID_SUB_AUTHORITY
#ifdef ERROR_INVALID_ACL
	{	ERROR_INVALID_ACL,                                        "ERROR_INVALID_ACL",
		"The access control list (ACL) structure is invalid.",
		"The access control list (ACL) structure is invalid."
	},
#endif // #ifdef ERROR_INVALID_ACL
#ifdef ERROR_INVALID_SID
	{	ERROR_INVALID_SID,                                        "ERROR_INVALID_SID",
		"The security ID structure is invalid.",
		"The security ID structure is invalid."
	},
#endif // #ifdef ERROR_INVALID_SID
#ifdef ERROR_INVALID_SECURITY_DESCR
	{	ERROR_INVALID_SECURITY_DESCR,                             "ERROR_INVALID_SECURITY_DESCR",
		"The security descriptor structure is invalid.",
		"The security descriptor structure is invalid."
	},
#endif // #ifdef ERROR_INVALID_SECURITY_DESCR
#ifdef ERROR_BAD_INHERITANCE_ACL
	{	ERROR_BAD_INHERITANCE_ACL,                                "ERROR_BAD_INHERITANCE_ACL",
		"The inherited access control list (ACL) or access control entry (ACE) could not be built.",
		"The inherited access control list (ACL) or access control entry (ACE) could not be built."
	},
#endif // #ifdef ERROR_BAD_INHERITANCE_ACL
#ifdef ERROR_SERVER_DISABLED
	{	ERROR_SERVER_DISABLED,                                    "ERROR_SERVER_DISABLED",
		"The server is currently disabled.",
		"The server is currently disabled."
	},
#endif // #ifdef ERROR_SERVER_DISABLED
#ifdef ERROR_SERVER_NOT_DISABLED
	{	ERROR_SERVER_NOT_DISABLED,                                "ERROR_SERVER_NOT_DISABLED",
		"The server is currently enabled.",
		"The server is currently enabled."
	},
#endif // #ifdef ERROR_SERVER_NOT_DISABLED
#ifdef ERROR_INVALID_ID_AUTHORITY
	{	ERROR_INVALID_ID_AUTHORITY,                               "ERROR_INVALID_ID_AUTHORITY",
		"The value provided was an invalid value for an identifier authority.",
		"The value provided was an invalid value for an identifier authority."
	},
#endif // #ifdef ERROR_INVALID_ID_AUTHORITY
#ifdef ERROR_ALLOTTED_SPACE_EXCEEDED
	{	ERROR_ALLOTTED_SPACE_EXCEEDED,                            "ERROR_ALLOTTED_SPACE_EXCEEDED",
		"No more memory is available for security information updates.",
		"No more memory is available for security information updates."
	},
#endif // #ifdef ERROR_ALLOTTED_SPACE_EXCEEDED
#ifdef ERROR_INVALID_GROUP_ATTRIBUTES
	{	ERROR_INVALID_GROUP_ATTRIBUTES,                           "ERROR_INVALID_GROUP_ATTRIBUTES",
		"The specified attributes are invalid, or incompatible with the attributes for the group as a whole.",
		"The specified attributes are invalid, or incompatible with the attributes for the group as a whole."
	},
#endif // #ifdef ERROR_INVALID_GROUP_ATTRIBUTES
#ifdef ERROR_BAD_IMPERSONATION_LEVEL
	{	ERROR_BAD_IMPERSONATION_LEVEL,                            "ERROR_BAD_IMPERSONATION_LEVEL",
		"Either a required impersonation level was not provided, or the provided impersonation level is invalid.",
		"Either a required impersonation level was not provided, or the provided impersonation level is invalid."
	},
#endif // #ifdef ERROR_BAD_IMPERSONATION_LEVEL
#ifdef ERROR_CANT_OPEN_ANONYMOUS
	{	ERROR_CANT_OPEN_ANONYMOUS,                                "ERROR_CANT_OPEN_ANONYMOUS",
		"Cannot open an anonymous level security token.",
		"Cannot open an anonymous level security token."
	},
#endif // #ifdef ERROR_CANT_OPEN_ANONYMOUS
#ifdef ERROR_BAD_VALIDATION_CLASS
	{	ERROR_BAD_VALIDATION_CLASS,                               "ERROR_BAD_VALIDATION_CLASS",
		"The validation information class requested was invalid.",
		"The validation information class requested was invalid."
	},
#endif // #ifdef ERROR_BAD_VALIDATION_CLASS
#ifdef ERROR_BAD_TOKEN_TYPE
	{	ERROR_BAD_TOKEN_TYPE,                                     "ERROR_BAD_TOKEN_TYPE",
		"The type of the token is inappropriate for its attempted use.",
		"The type of the token is inappropriate for its attempted use."
	},
#endif // #ifdef ERROR_BAD_TOKEN_TYPE
#ifdef ERROR_NO_SECURITY_ON_OBJECT
	{	ERROR_NO_SECURITY_ON_OBJECT,                              "ERROR_NO_SECURITY_ON_OBJECT",
		"Unable to perform a security operation on an object that has no associated security.",
		"Unable to perform a security operation on an object that has no associated security."
	},
#endif // #ifdef ERROR_NO_SECURITY_ON_OBJECT
#ifdef ERROR_CANT_ACCESS_DOMAIN_INFO
	{	ERROR_CANT_ACCESS_DOMAIN_INFO,                            "ERROR_CANT_ACCESS_DOMAIN_INFO",
		"Configuration information could not be read from the domain controller, either because the machine is unavailable, or access has been denied.",
		"Configuration information could not be read from the domain controller, either because the machine is unavailable, or access has been denied."
	},
#endif // #ifdef ERROR_CANT_ACCESS_DOMAIN_INFO
#ifdef ERROR_INVALID_SERVER_STATE
	{	ERROR_INVALID_SERVER_STATE,                               "ERROR_INVALID_SERVER_STATE",
		"The security account manager (SAM) or local security authority (LSA) server was in the wrong state to perform the security operation.",
		"The security account manager (SAM) or local security authority (LSA) server was in the wrong state to perform the security operation."
	},
#endif // #ifdef ERROR_INVALID_SERVER_STATE
#ifdef ERROR_INVALID_DOMAIN_STATE
	{	ERROR_INVALID_DOMAIN_STATE,                               "ERROR_INVALID_DOMAIN_STATE",
		"The domain was in the wrong state to perform the security operation.",
		"The domain was in the wrong state to perform the security operation."
	},
#endif // #ifdef ERROR_INVALID_DOMAIN_STATE
#ifdef ERROR_INVALID_DOMAIN_ROLE
	{	ERROR_INVALID_DOMAIN_ROLE,                                "ERROR_INVALID_DOMAIN_ROLE",
		"This operation is only allowed for the Primary Domain Controller of the domain.",
		"This operation is only allowed for the Primary Domain Controller of the domain."
	},
#endif // #ifdef ERROR_INVALID_DOMAIN_ROLE
#ifdef ERROR_NO_SUCH_DOMAIN
	{	ERROR_NO_SUCH_DOMAIN,                                     "ERROR_NO_SUCH_DOMAIN",
		"The specified domain either does not exist or could not be contacted.",
		"The specified domain either does not exist or could not be contacted."
	},
#endif // #ifdef ERROR_NO_SUCH_DOMAIN
#ifdef ERROR_DOMAIN_EXISTS
	{	ERROR_DOMAIN_EXISTS,                                      "ERROR_DOMAIN_EXISTS",
		"The specified domain already exists.",
		"The specified domain already exists."
	},
#endif // #ifdef ERROR_DOMAIN_EXISTS
#ifdef ERROR_DOMAIN_LIMIT_EXCEEDED
	{	ERROR_DOMAIN_LIMIT_EXCEEDED,                              "ERROR_DOMAIN_LIMIT_EXCEEDED",
		"An attempt was made to exceed the limit on the number of domains per server.",
		"An attempt was made to exceed the limit on the number of domains per server."
	},
#endif // #ifdef ERROR_DOMAIN_LIMIT_EXCEEDED
#ifdef ERROR_INTERNAL_DB_CORRUPTION
	{	ERROR_INTERNAL_DB_CORRUPTION,                             "ERROR_INTERNAL_DB_CORRUPTION",
		"Unable to complete the requested operation because of either a catastrophic media failure or a data structure corruption on the disk.",
		"Unable to complete the requested operation because of either a catastrophic media failure or a data structure corruption on the disk."
	},
#endif // #ifdef ERROR_INTERNAL_DB_CORRUPTION
#ifdef ERROR_INTERNAL_ERROR
	{	ERROR_INTERNAL_ERROR,                                     "ERROR_INTERNAL_ERROR",
		"An internal error occurred.",
		"An internal error occurred."
	},
#endif // #ifdef ERROR_INTERNAL_ERROR
#ifdef ERROR_GENERIC_NOT_MAPPED
	{	ERROR_GENERIC_NOT_MAPPED,                                 "ERROR_GENERIC_NOT_MAPPED",
		"Generic access types were contained in an access mask which should already be mapped to nongeneric types.",
		"Generic access types were contained in an access mask which should already be mapped to nongeneric types."
	},
#endif // #ifdef ERROR_GENERIC_NOT_MAPPED
#ifdef ERROR_BAD_DESCRIPTOR_FORMAT
	{	ERROR_BAD_DESCRIPTOR_FORMAT,                              "ERROR_BAD_DESCRIPTOR_FORMAT",
		"A security descriptor is not in the right format (absolute or self-relative).",
		"A security descriptor is not in the right format (absolute or self-relative)."
	},
#endif // #ifdef ERROR_BAD_DESCRIPTOR_FORMAT
#ifdef ERROR_NOT_LOGON_PROCESS
	{	ERROR_NOT_LOGON_PROCESS,                                  "ERROR_NOT_LOGON_PROCESS",
		"The requested action is restricted for use by logon processes only. The calling process has not registered as a logon process.",
		"The requested action is restricted for use by logon processes only. The calling process has not registered as a logon process."
	},
#endif // #ifdef ERROR_NOT_LOGON_PROCESS
#ifdef ERROR_LOGON_SESSION_EXISTS
	{	ERROR_LOGON_SESSION_EXISTS,                               "ERROR_LOGON_SESSION_EXISTS",
		"Cannot start a new logon session with an ID that is already in use.",
		"Cannot start a new logon session with an ID that is already in use."
	},
#endif // #ifdef ERROR_LOGON_SESSION_EXISTS
#ifdef ERROR_NO_SUCH_PACKAGE
	{	ERROR_NO_SUCH_PACKAGE,                                    "ERROR_NO_SUCH_PACKAGE",
		"A specified authentication package is unknown.",
		"A specified authentication package is unknown."
	},
#endif // #ifdef ERROR_NO_SUCH_PACKAGE
#ifdef ERROR_BAD_LOGON_SESSION_STATE
	{	ERROR_BAD_LOGON_SESSION_STATE,                            "ERROR_BAD_LOGON_SESSION_STATE",
		"The logon session is not in a state that is consistent with the requested operation.",
		"The logon session is not in a state that is consistent with the requested operation."
	},
#endif // #ifdef ERROR_BAD_LOGON_SESSION_STATE
#ifdef ERROR_LOGON_SESSION_COLLISION
	{	ERROR_LOGON_SESSION_COLLISION,                            "ERROR_LOGON_SESSION_COLLISION",
		"The logon session ID is already in use.",
		"The logon session ID is already in use."
	},
#endif // #ifdef ERROR_LOGON_SESSION_COLLISION
#ifdef ERROR_INVALID_LOGON_TYPE
	{	ERROR_INVALID_LOGON_TYPE,                                 "ERROR_INVALID_LOGON_TYPE",
		"A logon request contained an invalid logon type value.",
		"A logon request contained an invalid logon type value."
	},
#endif // #ifdef ERROR_INVALID_LOGON_TYPE
#ifdef ERROR_CANNOT_IMPERSONATE
	{	ERROR_CANNOT_IMPERSONATE,                                 "ERROR_CANNOT_IMPERSONATE",
		"Unable to impersonate using a named pipe until data has been read from that pipe.",
		"Unable to impersonate using a named pipe until data has been read from that pipe."
	},
#endif // #ifdef ERROR_CANNOT_IMPERSONATE
#ifdef ERROR_RXACT_INVALID_STATE
	{	ERROR_RXACT_INVALID_STATE,                                "ERROR_RXACT_INVALID_STATE",
		"The transaction state of a registry subtree is incompatible with the requested operation.",
		"The transaction state of a registry subtree is incompatible with the requested operation."
	},
#endif // #ifdef ERROR_RXACT_INVALID_STATE
#ifdef ERROR_RXACT_COMMIT_FAILURE
	{	ERROR_RXACT_COMMIT_FAILURE,                               "ERROR_RXACT_COMMIT_FAILURE",
		"An internal security database corruption has been encountered.",
		"An internal security database corruption has been encountered."
	},
#endif // #ifdef ERROR_RXACT_COMMIT_FAILURE
#ifdef ERROR_SPECIAL_ACCOUNT
	{	ERROR_SPECIAL_ACCOUNT,                                    "ERROR_SPECIAL_ACCOUNT",
		"Cannot perform this operation on built-in accounts.",
		"Cannot perform this operation on built-in accounts."
	},
#endif // #ifdef ERROR_SPECIAL_ACCOUNT
#ifdef ERROR_SPECIAL_GROUP
	{	ERROR_SPECIAL_GROUP,                                      "ERROR_SPECIAL_GROUP",
		"Cannot perform this operation on this built-in special group.",
		"Cannot perform this operation on this built-in special group."
	},
#endif // #ifdef ERROR_SPECIAL_GROUP
#ifdef ERROR_SPECIAL_USER
	{	ERROR_SPECIAL_USER,                                       "ERROR_SPECIAL_USER",
		"Cannot perform this operation on this built-in special user.",
		"Cannot perform this operation on this built-in special user."
	},
#endif // #ifdef ERROR_SPECIAL_USER
#ifdef ERROR_MEMBERS_PRIMARY_GROUP
	{	ERROR_MEMBERS_PRIMARY_GROUP,                              "ERROR_MEMBERS_PRIMARY_GROUP",
		"The user cannot be removed from a group because the group is currently the user's primary group.",
		"The user cannot be removed from a group because the group is currently the user's primary group."
	},
#endif // #ifdef ERROR_MEMBERS_PRIMARY_GROUP
#ifdef ERROR_TOKEN_ALREADY_IN_USE
	{	ERROR_TOKEN_ALREADY_IN_USE,                               "ERROR_TOKEN_ALREADY_IN_USE",
		"The token is already in use as a primary token.",
		"The token is already in use as a primary token."
	},
#endif // #ifdef ERROR_TOKEN_ALREADY_IN_USE
#ifdef ERROR_NO_SUCH_ALIAS
	{	ERROR_NO_SUCH_ALIAS,                                      "ERROR_NO_SUCH_ALIAS",
		"The specified local group does not exist.",
		"The specified local group does not exist."
	},
#endif // #ifdef ERROR_NO_SUCH_ALIAS
#ifdef ERROR_MEMBER_NOT_IN_ALIAS
	{	ERROR_MEMBER_NOT_IN_ALIAS,                                "ERROR_MEMBER_NOT_IN_ALIAS",
		"The specified account name is not a member of the local group.",
		"The specified account name is not a member of the local group."
	},
#endif // #ifdef ERROR_MEMBER_NOT_IN_ALIAS
#ifdef ERROR_MEMBER_IN_ALIAS
	{	ERROR_MEMBER_IN_ALIAS,                                    "ERROR_MEMBER_IN_ALIAS",
		"The specified account name is already a member of the local group.",
		"The specified account name is already a member of the local group."
	},
#endif // #ifdef ERROR_MEMBER_IN_ALIAS
#ifdef ERROR_ALIAS_EXISTS
	{	ERROR_ALIAS_EXISTS,                                       "ERROR_ALIAS_EXISTS",
		"The specified local group already exists.",
		"The specified local group already exists."
	},
#endif // #ifdef ERROR_ALIAS_EXISTS
#ifdef ERROR_LOGON_NOT_GRANTED
	{	ERROR_LOGON_NOT_GRANTED,                                  "ERROR_LOGON_NOT_GRANTED",
		"Logon failure: the user has not been granted the requested logon type at this computer.",
		"Logon failure: the user has not been granted the requested logon type at this computer."
	},
#endif // #ifdef ERROR_LOGON_NOT_GRANTED
#ifdef ERROR_TOO_MANY_SECRETS
	{	ERROR_TOO_MANY_SECRETS,                                   "ERROR_TOO_MANY_SECRETS",
		"The maximum number of secrets that may be stored in a single system has been exceeded.",
		"The maximum number of secrets that may be stored in a single system has been exceeded."
	},
#endif // #ifdef ERROR_TOO_MANY_SECRETS
#ifdef ERROR_SECRET_TOO_LONG
	{	ERROR_SECRET_TOO_LONG,                                    "ERROR_SECRET_TOO_LONG",
		"The length of a secret exceeds the maximum length allowed.",
		"The length of a secret exceeds the maximum length allowed."
	},
#endif // #ifdef ERROR_SECRET_TOO_LONG
#ifdef ERROR_INTERNAL_DB_ERROR
	{	ERROR_INTERNAL_DB_ERROR,                                  "ERROR_INTERNAL_DB_ERROR",
		"The local security authority database contains an internal inconsistency.",
		"The local security authority database contains an internal inconsistency."
	},
#endif // #ifdef ERROR_INTERNAL_DB_ERROR
#ifdef ERROR_TOO_MANY_CONTEXT_IDS
	{	ERROR_TOO_MANY_CONTEXT_IDS,                               "ERROR_TOO_MANY_CONTEXT_IDS",
		"During a logon attempt, the user's security context accumulated too many security IDs.",
		"During a logon attempt, the user's security context accumulated too many security IDs."
	},
#endif // #ifdef ERROR_TOO_MANY_CONTEXT_IDS
#ifdef ERROR_LOGON_TYPE_NOT_GRANTED
	{	ERROR_LOGON_TYPE_NOT_GRANTED,                             "ERROR_LOGON_TYPE_NOT_GRANTED",
		"Logon failure: the user has not been granted the requested logon type at this computer.",
		"Logon failure: the user has not been granted the requested logon type at this computer."
	},
#endif // #ifdef ERROR_LOGON_TYPE_NOT_GRANTED
#ifdef ERROR_NT_CROSS_ENCRYPTION_REQUIRED
	{	ERROR_NT_CROSS_ENCRYPTION_REQUIRED,                       "ERROR_NT_CROSS_ENCRYPTION_REQUIRED",
		"A cross-encrypted password is necessary to change a user password.",
		"A cross-encrypted password is necessary to change a user password."
	},
#endif // #ifdef ERROR_NT_CROSS_ENCRYPTION_REQUIRED
#ifdef ERROR_NO_SUCH_MEMBER
	{	ERROR_NO_SUCH_MEMBER,                                     "ERROR_NO_SUCH_MEMBER",
		"A new member could not be added to or removed from the local group because the member does not exist.",
		"A new member could not be added to or removed from the local group because the member does not exist."
	},
#endif // #ifdef ERROR_NO_SUCH_MEMBER
#ifdef ERROR_INVALID_MEMBER
	{	ERROR_INVALID_MEMBER,                                     "ERROR_INVALID_MEMBER",
		"A new member could not be added to a local group because the member has the wrong account type.",
		"A new member could not be added to a local group because the member has the wrong account type."
	},
#endif // #ifdef ERROR_INVALID_MEMBER
#ifdef ERROR_TOO_MANY_SIDS
	{	ERROR_TOO_MANY_SIDS,                                      "ERROR_TOO_MANY_SIDS",
		"Too many security IDs have been specified.",
		"Too many security IDs have been specified."
	},
#endif // #ifdef ERROR_TOO_MANY_SIDS
#ifdef ERROR_LM_CROSS_ENCRYPTION_REQUIRED
	{	ERROR_LM_CROSS_ENCRYPTION_REQUIRED,                       "ERROR_LM_CROSS_ENCRYPTION_REQUIRED",
		"A cross-encrypted password is necessary to change this user password.",
		"A cross-encrypted password is necessary to change this user password."
	},
#endif // #ifdef ERROR_LM_CROSS_ENCRYPTION_REQUIRED
#ifdef ERROR_NO_INHERITANCE
	{	ERROR_NO_INHERITANCE,                                     "ERROR_NO_INHERITANCE",
		"Indicates an ACL contains no inheritable components.",
		"Indicates an ACL contains no inheritable components."
	},
#endif // #ifdef ERROR_NO_INHERITANCE
#ifdef ERROR_FILE_CORRUPT
	{	ERROR_FILE_CORRUPT,                                       "ERROR_FILE_CORRUPT",
		"The file or directory is corrupted and unreadable.",
		"The file or directory is corrupted and unreadable."
	},
#endif // #ifdef ERROR_FILE_CORRUPT
#ifdef ERROR_DISK_CORRUPT
	{	ERROR_DISK_CORRUPT,                                       "ERROR_DISK_CORRUPT",
		"The disk structure is corrupted and unreadable.",
		"The disk structure is corrupted and unreadable."
	},
#endif // #ifdef ERROR_DISK_CORRUPT
#ifdef ERROR_NO_USER_SESSION_KEY
	{	ERROR_NO_USER_SESSION_KEY,                                "ERROR_NO_USER_SESSION_KEY",
		"There is no user session key for the specified logon session.",
		"There is no user session key for the specified logon session."
	},
#endif // #ifdef ERROR_NO_USER_SESSION_KEY
#ifdef ERROR_LICENSE_QUOTA_EXCEEDED
	{	ERROR_LICENSE_QUOTA_EXCEEDED,                             "ERROR_LICENSE_QUOTA_EXCEEDED",
		"The service being accessed is licensed for a particular number of connections. No more connections can be made to the service at this time because there are already as many connections as the service can accept.",
		"The service being accessed is licensed for a particular number of connections. No more connections can be made to the service at this time because there are already as many connections as the service can accept."
	},
#endif // #ifdef ERROR_LICENSE_QUOTA_EXCEEDED
#ifdef ERROR_WRONG_TARGET_NAME
	{	ERROR_WRONG_TARGET_NAME,                                  "ERROR_WRONG_TARGET_NAME",
		"Logon Failure: The target account name is incorrect.",
		"Logon Failure: The target account name is incorrect."
	},
#endif // #ifdef ERROR_WRONG_TARGET_NAME
#ifdef ERROR_MUTUAL_AUTH_FAILED
	{	ERROR_MUTUAL_AUTH_FAILED,                                 "ERROR_MUTUAL_AUTH_FAILED",
		"Mutual Authentication failed. The server's password is out of date at the domain controller.",
		"Mutual Authentication failed. The server's password is out of date at the domain controller."
	},
#endif // #ifdef ERROR_MUTUAL_AUTH_FAILED
#ifdef ERROR_TIME_SKEW
	{	ERROR_TIME_SKEW,                                          "ERROR_TIME_SKEW",
		"There is a time and/or date difference between the client and server.",
		"There is a time and/or date difference between the client and server."
	},
#endif // #ifdef ERROR_TIME_SKEW
#ifdef ERROR_CURRENT_DOMAIN_NOT_ALLOWED
	{	ERROR_CURRENT_DOMAIN_NOT_ALLOWED,                         "ERROR_CURRENT_DOMAIN_NOT_ALLOWED",
		"This operation cannot be performed on the current domain.",
		"This operation cannot be performed on the current domain."
	},
#endif // #ifdef ERROR_CURRENT_DOMAIN_NOT_ALLOWED
#ifdef ERROR_INVALID_WINDOW_HANDLE
	{	ERROR_INVALID_WINDOW_HANDLE,                              "ERROR_INVALID_WINDOW_HANDLE",
		"Invalid window handle.",
		"Invalid window handle."
	},
#endif // #ifdef ERROR_INVALID_WINDOW_HANDLE
#ifdef ERROR_INVALID_MENU_HANDLE
	{	ERROR_INVALID_MENU_HANDLE,                                "ERROR_INVALID_MENU_HANDLE",
		"Invalid menu handle.",
		"Invalid menu handle."
	},
#endif // #ifdef ERROR_INVALID_MENU_HANDLE
#ifdef ERROR_INVALID_CURSOR_HANDLE
	{	ERROR_INVALID_CURSOR_HANDLE,                              "ERROR_INVALID_CURSOR_HANDLE",
		"Invalid cursor handle.",
		"Invalid cursor handle."
	},
#endif // #ifdef ERROR_INVALID_CURSOR_HANDLE
#ifdef ERROR_INVALID_ACCEL_HANDLE
	{	ERROR_INVALID_ACCEL_HANDLE,                               "ERROR_INVALID_ACCEL_HANDLE",
		"Invalid accelerator table handle.",
		"Invalid accelerator table handle."
	},
#endif // #ifdef ERROR_INVALID_ACCEL_HANDLE
#ifdef ERROR_INVALID_HOOK_HANDLE
	{	ERROR_INVALID_HOOK_HANDLE,                                "ERROR_INVALID_HOOK_HANDLE",
		"Invalid hook handle.",
		"Invalid hook handle."
	},
#endif // #ifdef ERROR_INVALID_HOOK_HANDLE
#ifdef ERROR_INVALID_DWP_HANDLE
	{	ERROR_INVALID_DWP_HANDLE,                                 "ERROR_INVALID_DWP_HANDLE",
		"Invalid handle to a multiple-window position structure.",
		"Invalid handle to a multiple-window position structure."
	},
#endif // #ifdef ERROR_INVALID_DWP_HANDLE
#ifdef ERROR_TLW_WITH_WSCHILD
	{	ERROR_TLW_WITH_WSCHILD,                                   "ERROR_TLW_WITH_WSCHILD",
		"Cannot create a top-level child window.",
		"Cannot create a top-level child window."
	},
#endif // #ifdef ERROR_TLW_WITH_WSCHILD
#ifdef ERROR_CANNOT_FIND_WND_CLASS
	{	ERROR_CANNOT_FIND_WND_CLASS,                              "ERROR_CANNOT_FIND_WND_CLASS",
		"Cannot find window class.",
		"Cannot find window class."
	},
#endif // #ifdef ERROR_CANNOT_FIND_WND_CLASS
#ifdef ERROR_WINDOW_OF_OTHER_THREAD
	{	ERROR_WINDOW_OF_OTHER_THREAD,                             "ERROR_WINDOW_OF_OTHER_THREAD",
		"Invalid window; it belongs to other thread.",
		"Invalid window; it belongs to other thread."
	},
#endif // #ifdef ERROR_WINDOW_OF_OTHER_THREAD
#ifdef ERROR_HOTKEY_ALREADY_REGISTERED
	{	ERROR_HOTKEY_ALREADY_REGISTERED,                          "ERROR_HOTKEY_ALREADY_REGISTERED",
		"Hot key is already registered.",
		"Hot key is already registered."
	},
#endif // #ifdef ERROR_HOTKEY_ALREADY_REGISTERED
#ifdef ERROR_CLASS_ALREADY_EXISTS
	{	ERROR_CLASS_ALREADY_EXISTS,                               "ERROR_CLASS_ALREADY_EXISTS",
		"Class already exists.",
		"Class already exists."
	},
#endif // #ifdef ERROR_CLASS_ALREADY_EXISTS
#ifdef ERROR_CLASS_DOES_NOT_EXIST
	{	ERROR_CLASS_DOES_NOT_EXIST,                               "ERROR_CLASS_DOES_NOT_EXIST",
		"Class does not exist.",
		"Class does not exist."
	},
#endif // #ifdef ERROR_CLASS_DOES_NOT_EXIST
#ifdef ERROR_CLASS_HAS_WINDOWS
	{	ERROR_CLASS_HAS_WINDOWS,                                  "ERROR_CLASS_HAS_WINDOWS",
		"Class still has open windows.",
		"Class still has open windows."
	},
#endif // #ifdef ERROR_CLASS_HAS_WINDOWS
#ifdef ERROR_INVALID_INDEX
	{	ERROR_INVALID_INDEX,                                      "ERROR_INVALID_INDEX",
		"Invalid index.",
		"Invalid index."
	},
#endif // #ifdef ERROR_INVALID_INDEX
#ifdef ERROR_INVALID_ICON_HANDLE
	{	ERROR_INVALID_ICON_HANDLE,                                "ERROR_INVALID_ICON_HANDLE",
		"Invalid icon handle.",
		"Invalid icon handle."
	},
#endif // #ifdef ERROR_INVALID_ICON_HANDLE
#ifdef ERROR_PRIVATE_DIALOG_INDEX
	{	ERROR_PRIVATE_DIALOG_INDEX,                               "ERROR_PRIVATE_DIALOG_INDEX",
		"Using private DIALOG window words.",
		"Using private DIALOG window words."
	},
#endif // #ifdef ERROR_PRIVATE_DIALOG_INDEX
#ifdef ERROR_LISTBOX_ID_NOT_FOUND
	{	ERROR_LISTBOX_ID_NOT_FOUND,                               "ERROR_LISTBOX_ID_NOT_FOUND",
		"The list box identifier was not found.",
		"The list box identifier was not found."
	},
#endif // #ifdef ERROR_LISTBOX_ID_NOT_FOUND
#ifdef ERROR_NO_WILDCARD_CHARACTERS
	{	ERROR_NO_WILDCARD_CHARACTERS,                             "ERROR_NO_WILDCARD_CHARACTERS",
		"No wildcards were found.",
		"No wildcards were found."
	},
#endif // #ifdef ERROR_NO_WILDCARD_CHARACTERS
#ifdef ERROR_CLIPBOARD_NOT_OPEN
	{	ERROR_CLIPBOARD_NOT_OPEN,                                 "ERROR_CLIPBOARD_NOT_OPEN",
		"Thread does not have a clipboard open.",
		"Thread does not have a clipboard open."
	},
#endif // #ifdef ERROR_CLIPBOARD_NOT_OPEN
#ifdef ERROR_HOTKEY_NOT_REGISTERED
	{	ERROR_HOTKEY_NOT_REGISTERED,                              "ERROR_HOTKEY_NOT_REGISTERED",
		"Hot key is not registered.",
		"Hot key is not registered."
	},
#endif // #ifdef ERROR_HOTKEY_NOT_REGISTERED
#ifdef ERROR_WINDOW_NOT_DIALOG
	{	ERROR_WINDOW_NOT_DIALOG,                                  "ERROR_WINDOW_NOT_DIALOG",
		"The window is not a valid dialog window.",
		"The window is not a valid dialog window."
	},
#endif // #ifdef ERROR_WINDOW_NOT_DIALOG
#ifdef ERROR_CONTROL_ID_NOT_FOUND
	{	ERROR_CONTROL_ID_NOT_FOUND,                               "ERROR_CONTROL_ID_NOT_FOUND",
		"Control ID not found.",
		"Control ID not found."
	},
#endif // #ifdef ERROR_CONTROL_ID_NOT_FOUND
#ifdef ERROR_INVALID_COMBOBOX_MESSAGE
	{	ERROR_INVALID_COMBOBOX_MESSAGE,                           "ERROR_INVALID_COMBOBOX_MESSAGE",
		"Invalid message for a combo box because it does not have an edit control.",
		"Invalid message for a combo box because it does not have an edit control."
	},
#endif // #ifdef ERROR_INVALID_COMBOBOX_MESSAGE
#ifdef ERROR_WINDOW_NOT_COMBOBOX
	{	ERROR_WINDOW_NOT_COMBOBOX,                                "ERROR_WINDOW_NOT_COMBOBOX",
		"The window is not a combo box.",
		"The window is not a combo box."
	},
#endif // #ifdef ERROR_WINDOW_NOT_COMBOBOX
#ifdef ERROR_INVALID_EDIT_HEIGHT
	{	ERROR_INVALID_EDIT_HEIGHT,                                "ERROR_INVALID_EDIT_HEIGHT",
		"Height must be less than 256.",
		"Height must be less than 256."
	},
#endif // #ifdef ERROR_INVALID_EDIT_HEIGHT
#ifdef ERROR_DC_NOT_FOUND
	{	ERROR_DC_NOT_FOUND,                                       "ERROR_DC_NOT_FOUND",
		"Invalid device context (DC) handle.",
		"Invalid device context (DC) handle."
	},
#endif // #ifdef ERROR_DC_NOT_FOUND
#ifdef ERROR_INVALID_HOOK_FILTER
	{	ERROR_INVALID_HOOK_FILTER,                                "ERROR_INVALID_HOOK_FILTER",
		"Invalid hook procedure type.",
		"Invalid hook procedure type."
	},
#endif // #ifdef ERROR_INVALID_HOOK_FILTER
#ifdef ERROR_INVALID_FILTER_PROC
	{	ERROR_INVALID_FILTER_PROC,                                "ERROR_INVALID_FILTER_PROC",
		"Invalid hook procedure.",
		"Invalid hook procedure."
	},
#endif // #ifdef ERROR_INVALID_FILTER_PROC
#ifdef ERROR_HOOK_NEEDS_HMOD
	{	ERROR_HOOK_NEEDS_HMOD,                                    "ERROR_HOOK_NEEDS_HMOD",
		"Cannot set nonlocal hook without a module handle.",
		"Cannot set nonlocal hook without a module handle."
	},
#endif // #ifdef ERROR_HOOK_NEEDS_HMOD
#ifdef ERROR_GLOBAL_ONLY_HOOK
	{	ERROR_GLOBAL_ONLY_HOOK,                                   "ERROR_GLOBAL_ONLY_HOOK",
		"This hook procedure can only be set globally.",
		"This hook procedure can only be set globally."
	},
#endif // #ifdef ERROR_GLOBAL_ONLY_HOOK
#ifdef ERROR_JOURNAL_HOOK_SET
	{	ERROR_JOURNAL_HOOK_SET,                                   "ERROR_JOURNAL_HOOK_SET",
		"The journal hook procedure is already installed.",
		"The journal hook procedure is already installed."
	},
#endif // #ifdef ERROR_JOURNAL_HOOK_SET
#ifdef ERROR_HOOK_NOT_INSTALLED
	{	ERROR_HOOK_NOT_INSTALLED,                                 "ERROR_HOOK_NOT_INSTALLED",
		"The hook procedure is not installed.",
		"The hook procedure is not installed."
	},
#endif // #ifdef ERROR_HOOK_NOT_INSTALLED
#ifdef ERROR_INVALID_LB_MESSAGE
	{	ERROR_INVALID_LB_MESSAGE,                                 "ERROR_INVALID_LB_MESSAGE",
		"Invalid message for single-selection list box.",
		"Invalid message for single-selection list box."
	},
#endif // #ifdef ERROR_INVALID_LB_MESSAGE
#ifdef ERROR_SETCOUNT_ON_BAD_LB
	{	ERROR_SETCOUNT_ON_BAD_LB,                                 "ERROR_SETCOUNT_ON_BAD_LB",
		"LB_SETCOUNT sent to non-lazy list box.",
		"LB_SETCOUNT sent to non-lazy list box."
	},
#endif // #ifdef ERROR_SETCOUNT_ON_BAD_LB
#ifdef ERROR_LB_WITHOUT_TABSTOPS
	{	ERROR_LB_WITHOUT_TABSTOPS,                                "ERROR_LB_WITHOUT_TABSTOPS",
		"This list box does not support tab stops.",
		"This list box does not support tab stops."
	},
#endif // #ifdef ERROR_LB_WITHOUT_TABSTOPS
#ifdef ERROR_DESTROY_OBJECT_OF_OTHER_THREAD
	{	ERROR_DESTROY_OBJECT_OF_OTHER_THREAD,                     "ERROR_DESTROY_OBJECT_OF_OTHER_THREAD",
		"Cannot destroy object created by another thread.",
		"Cannot destroy object created by another thread."
	},
#endif // #ifdef ERROR_DESTROY_OBJECT_OF_OTHER_THREAD
#ifdef ERROR_CHILD_WINDOW_MENU
	{	ERROR_CHILD_WINDOW_MENU,                                  "ERROR_CHILD_WINDOW_MENU",
		"Child windows cannot have menus.",
		"Child windows cannot have menus."
	},
#endif // #ifdef ERROR_CHILD_WINDOW_MENU
#ifdef ERROR_NO_SYSTEM_MENU
	{	ERROR_NO_SYSTEM_MENU,                                     "ERROR_NO_SYSTEM_MENU",
		"The window does not have a system menu.",
		"The window does not have a system menu."
	},
#endif // #ifdef ERROR_NO_SYSTEM_MENU
#ifdef ERROR_INVALID_MSGBOX_STYLE
	{	ERROR_INVALID_MSGBOX_STYLE,                               "ERROR_INVALID_MSGBOX_STYLE",
		"Invalid message box style.",
		"Invalid message box style."
	},
#endif // #ifdef ERROR_INVALID_MSGBOX_STYLE
#ifdef ERROR_INVALID_SPI_VALUE
	{	ERROR_INVALID_SPI_VALUE,                                  "ERROR_INVALID_SPI_VALUE",
		"Invalid system-wide (SPI_*) parameter.",
		"Invalid system-wide (SPI_*) parameter."
	},
#endif // #ifdef ERROR_INVALID_SPI_VALUE
#ifdef ERROR_SCREEN_ALREADY_LOCKED
	{	ERROR_SCREEN_ALREADY_LOCKED,                              "ERROR_SCREEN_ALREADY_LOCKED",
		"Screen already locked.",
		"Screen already locked."
	},
#endif // #ifdef ERROR_SCREEN_ALREADY_LOCKED
#ifdef ERROR_HWNDS_HAVE_DIFF_PARENT
	{	ERROR_HWNDS_HAVE_DIFF_PARENT,                             "ERROR_HWNDS_HAVE_DIFF_PARENT",
		"All handles to windows in a multiple-window position structure must have the same parent.",
		"All handles to windows in a multiple-window position structure must have the same parent."
	},
#endif // #ifdef ERROR_HWNDS_HAVE_DIFF_PARENT
#ifdef ERROR_NOT_CHILD_WINDOW
	{	ERROR_NOT_CHILD_WINDOW,                                   "ERROR_NOT_CHILD_WINDOW",
		"The window is not a child window.",
		"The window is not a child window."
	},
#endif // #ifdef ERROR_NOT_CHILD_WINDOW
#ifdef ERROR_INVALID_GW_COMMAND
	{	ERROR_INVALID_GW_COMMAND,                                 "ERROR_INVALID_GW_COMMAND",
		"Invalid GW_* command.",
		"Invalid GW_* command."
	},
#endif // #ifdef ERROR_INVALID_GW_COMMAND
#ifdef ERROR_INVALID_THREAD_ID
	{	ERROR_INVALID_THREAD_ID,                                  "ERROR_INVALID_THREAD_ID",
		"Invalid thread identifier.",
		"Invalid thread identifier."
	},
#endif // #ifdef ERROR_INVALID_THREAD_ID
#ifdef ERROR_NON_MDICHILD_WINDOW
	{	ERROR_NON_MDICHILD_WINDOW,                                "ERROR_NON_MDICHILD_WINDOW",
		"Cannot process a message from a window that is not a multiple document interface (MDI) window.",
		"Cannot process a message from a window that is not a multiple document interface (MDI) window."
	},
#endif // #ifdef ERROR_NON_MDICHILD_WINDOW
#ifdef ERROR_POPUP_ALREADY_ACTIVE
	{	ERROR_POPUP_ALREADY_ACTIVE,                               "ERROR_POPUP_ALREADY_ACTIVE",
		"Popup menu already active.",
		"Popup menu already active."
	},
#endif // #ifdef ERROR_POPUP_ALREADY_ACTIVE
#ifdef ERROR_NO_SCROLLBARS
	{	ERROR_NO_SCROLLBARS,                                      "ERROR_NO_SCROLLBARS",
		"The window does not have scroll bars.",
		"The window does not have scroll bars."
	},
#endif // #ifdef ERROR_NO_SCROLLBARS
#ifdef ERROR_INVALID_SCROLLBAR_RANGE
	{	ERROR_INVALID_SCROLLBAR_RANGE,                            "ERROR_INVALID_SCROLLBAR_RANGE",
		"Scroll bar range cannot be greater than MAXLONG.",
		"Scroll bar range cannot be greater than MAXLONG."
	},
#endif // #ifdef ERROR_INVALID_SCROLLBAR_RANGE
#ifdef ERROR_INVALID_SHOWWIN_COMMAND
	{	ERROR_INVALID_SHOWWIN_COMMAND,                            "ERROR_INVALID_SHOWWIN_COMMAND",
		"Cannot show or remove the window in the way specified.",
		"Cannot show or remove the window in the way specified."
	},
#endif // #ifdef ERROR_INVALID_SHOWWIN_COMMAND
#ifdef ERROR_NO_SYSTEM_RESOURCES
	{	ERROR_NO_SYSTEM_RESOURCES,                                "ERROR_NO_SYSTEM_RESOURCES",
		"Insufficient system resources exist to complete the requested service.",
		"Insufficient system resources exist to complete the requested service."
	},
#endif // #ifdef ERROR_NO_SYSTEM_RESOURCES
#ifdef ERROR_NONPAGED_SYSTEM_RESOURCES
	{	ERROR_NONPAGED_SYSTEM_RESOURCES,                          "ERROR_NONPAGED_SYSTEM_RESOURCES",
		"Insufficient system resources exist to complete the requested service.",
		"Insufficient system resources exist to complete the requested service."
	},
#endif // #ifdef ERROR_NONPAGED_SYSTEM_RESOURCES
#ifdef ERROR_PAGED_SYSTEM_RESOURCES
	{	ERROR_PAGED_SYSTEM_RESOURCES,                             "ERROR_PAGED_SYSTEM_RESOURCES",
		"Insufficient system resources exist to complete the requested service.",
		"Insufficient system resources exist to complete the requested service."
	},
#endif // #ifdef ERROR_PAGED_SYSTEM_RESOURCES
#ifdef ERROR_WORKING_SET_QUOTA
	{	ERROR_WORKING_SET_QUOTA,                                  "ERROR_WORKING_SET_QUOTA",
		"Insufficient quota to complete the requested service.",
		"Insufficient quota to complete the requested service."
	},
#endif // #ifdef ERROR_WORKING_SET_QUOTA
#ifdef ERROR_PAGEFILE_QUOTA
	{	ERROR_PAGEFILE_QUOTA,                                     "ERROR_PAGEFILE_QUOTA",
		"Insufficient quota to complete the requested service.",
		"Insufficient quota to complete the requested service."
	},
#endif // #ifdef ERROR_PAGEFILE_QUOTA
#ifdef ERROR_COMMITMENT_LIMIT
	{	ERROR_COMMITMENT_LIMIT,                                   "ERROR_COMMITMENT_LIMIT",
		"The paging file is too small for this operation to complete.",
		"The paging file is too small for this operation to complete."
	},
#endif // #ifdef ERROR_COMMITMENT_LIMIT
#ifdef ERROR_MENU_ITEM_NOT_FOUND
	{	ERROR_MENU_ITEM_NOT_FOUND,                                "ERROR_MENU_ITEM_NOT_FOUND",
		"A menu item was not found.",
		"A menu item was not found."
	},
#endif // #ifdef ERROR_MENU_ITEM_NOT_FOUND
#ifdef ERROR_INVALID_KEYBOARD_HANDLE
	{	ERROR_INVALID_KEYBOARD_HANDLE,                            "ERROR_INVALID_KEYBOARD_HANDLE",
		"Invalid keyboard layout handle.",
		"Invalid keyboard layout handle."
	},
#endif // #ifdef ERROR_INVALID_KEYBOARD_HANDLE
#ifdef ERROR_HOOK_TYPE_NOT_ALLOWED
	{	ERROR_HOOK_TYPE_NOT_ALLOWED,                              "ERROR_HOOK_TYPE_NOT_ALLOWED",
		"Hook type not allowed.",
		"Hook type not allowed."
	},
#endif // #ifdef ERROR_HOOK_TYPE_NOT_ALLOWED
#ifdef ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION
	{	ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION,                 "ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION",
		"This operation requires an interactive window station.",
		"This operation requires an interactive window station."
	},
#endif // #ifdef ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION
#ifdef ERROR_TIMEOUT
	{	ERROR_TIMEOUT,                                            "ERROR_TIMEOUT",
		"This operation returned because the timeout period expired.",
		"This operation returned because the timeout period expired."
	},
#endif // #ifdef ERROR_TIMEOUT
#ifdef ERROR_INVALID_MONITOR_HANDLE
	{	ERROR_INVALID_MONITOR_HANDLE,                             "ERROR_INVALID_MONITOR_HANDLE",
		"Invalid monitor handle.",
		"Invalid monitor handle."
	},
#endif // #ifdef ERROR_INVALID_MONITOR_HANDLE
#ifdef ERROR_EVENTLOG_FILE_CORRUPT
	{	ERROR_EVENTLOG_FILE_CORRUPT,                              "ERROR_EVENTLOG_FILE_CORRUPT",
		"The event log file is corrupted.",
		"The event log file is corrupted."
	},
#endif // #ifdef ERROR_EVENTLOG_FILE_CORRUPT
#ifdef ERROR_EVENTLOG_CANT_START
	{	ERROR_EVENTLOG_CANT_START,                                "ERROR_EVENTLOG_CANT_START",
		"No event log file could be opened, so the event logging service did not start.",
		"No event log file could be opened, so the event logging service did not start."
	},
#endif // #ifdef ERROR_EVENTLOG_CANT_START
#ifdef ERROR_LOG_FILE_FULL
	{	ERROR_LOG_FILE_FULL,                                      "ERROR_LOG_FILE_FULL",
		"The event log file is full.",
		"The event log file is full."
	},
#endif // #ifdef ERROR_LOG_FILE_FULL
#ifdef ERROR_EVENTLOG_FILE_CHANGED
	{	ERROR_EVENTLOG_FILE_CHANGED,                              "ERROR_EVENTLOG_FILE_CHANGED",
		"The event log file has changed between read operations.",
		"The event log file has changed between read operations."
	},
#endif // #ifdef ERROR_EVENTLOG_FILE_CHANGED
#ifdef ERROR_INSTALL_SERVICE_FAILURE
	{	ERROR_INSTALL_SERVICE_FAILURE,                            "ERROR_INSTALL_SERVICE_FAILURE",
		"The Windows Installer service could not be accessed. This can occur if you are running Windows in safe mode, or if the Windows Installer is not correctly installed. Contact your support personnel for assistance.",
		"The Windows Installer service could not be accessed. This can occur if you are running Windows in safe mode, or if the Windows Installer is not correctly installed. Contact your support personnel for assistance."
	},
#endif // #ifdef ERROR_INSTALL_SERVICE_FAILURE
#ifdef ERROR_INSTALL_USEREXIT
	{	ERROR_INSTALL_USEREXIT,                                   "ERROR_INSTALL_USEREXIT",
		"User cancelled installation.",
		"User cancelled installation."
	},
#endif // #ifdef ERROR_INSTALL_USEREXIT
#ifdef ERROR_INSTALL_FAILURE
	{	ERROR_INSTALL_FAILURE,                                    "ERROR_INSTALL_FAILURE",
		"Fatal error during installation.",
		"Fatal error during installation."
	},
#endif // #ifdef ERROR_INSTALL_FAILURE
#ifdef ERROR_INSTALL_SUSPEND
	{	ERROR_INSTALL_SUSPEND,                                    "ERROR_INSTALL_SUSPEND",
		"Installation suspended, incomplete.",
		"Installation suspended, incomplete."
	},
#endif // #ifdef ERROR_INSTALL_SUSPEND
#ifdef ERROR_UNKNOWN_PRODUCT
	{	ERROR_UNKNOWN_PRODUCT,                                    "ERROR_UNKNOWN_PRODUCT",
		"Product code not registered.",
		"This action is only valid for products that are currently installed."
	},
#endif // #ifdef ERROR_UNKNOWN_PRODUCT
#ifdef ERROR_UNKNOWN_FEATURE
	{	ERROR_UNKNOWN_FEATURE,                                    "ERROR_UNKNOWN_FEATURE",
		"Feature ID not registered.",
		"Feature ID not registered."
	},
#endif // #ifdef ERROR_UNKNOWN_FEATURE
#ifdef ERROR_UNKNOWN_COMPONENT
	{	ERROR_UNKNOWN_COMPONENT,                                  "ERROR_UNKNOWN_COMPONENT",
		"Component ID not registered.",
		"Component ID not registered."
	},
#endif // #ifdef ERROR_UNKNOWN_COMPONENT
#ifdef ERROR_UNKNOWN_PROPERTY
	{	ERROR_UNKNOWN_PROPERTY,                                   "ERROR_UNKNOWN_PROPERTY",
		"Unknown property.",
		"Unknown property."
	},
#endif // #ifdef ERROR_UNKNOWN_PROPERTY
#ifdef ERROR_INVALID_HANDLE_STATE
	{	ERROR_INVALID_HANDLE_STATE,                               "ERROR_INVALID_HANDLE_STATE",
		"Handle is in an invalid state.",
		"Handle is in an invalid state."
	},
#endif // #ifdef ERROR_INVALID_HANDLE_STATE
#ifdef ERROR_BAD_CONFIGURATION
	{	ERROR_BAD_CONFIGURATION,                                  "ERROR_BAD_CONFIGURATION",
		"Configuration data corrupt.",
		"The configuration data for this product is corrupt. Contact your support personnel."
	},
#endif // #ifdef ERROR_BAD_CONFIGURATION
#ifdef ERROR_INDEX_ABSENT
	{	ERROR_INDEX_ABSENT,                                       "ERROR_INDEX_ABSENT",
		"Component qualifier not present.",
		"Component qualifier not present."
	},
#endif // #ifdef ERROR_INDEX_ABSENT
#ifdef ERROR_INSTALL_SOURCE_ABSENT
	{	ERROR_INSTALL_SOURCE_ABSENT,                              "ERROR_INSTALL_SOURCE_ABSENT",
		"Install source unavailable.",
		"The installation source for this product is not available. Verify that the source exists and that you can access it."
	},
#endif // #ifdef ERROR_INSTALL_SOURCE_ABSENT
#ifdef ERROR_INSTALL_PACKAGE_VERSION
	{	ERROR_INSTALL_PACKAGE_VERSION,                            "ERROR_INSTALL_PACKAGE_VERSION",
		"This installation package cannot be installed by the Windows Installer service. You must install a Windows service pack that contains a newer version of the Windows Installer service.",
		"This installation package cannot be installed by the Windows Installer service. You must install a Windows service pack that contains a newer version of the Windows Installer service."
	},
#endif // #ifdef ERROR_INSTALL_PACKAGE_VERSION
#ifdef ERROR_PRODUCT_UNINSTALLED
	{	ERROR_PRODUCT_UNINSTALLED,                                "ERROR_PRODUCT_UNINSTALLED",
		"Product is uninstalled.",
		"Product is uninstalled."
	},
#endif // #ifdef ERROR_PRODUCT_UNINSTALLED
#ifdef ERROR_BAD_QUERY_SYNTAX
	{	ERROR_BAD_QUERY_SYNTAX,                                   "ERROR_BAD_QUERY_SYNTAX",
		"SQL query syntax invalid or unsupported.",
		"SQL query syntax invalid or unsupported."
	},
#endif // #ifdef ERROR_BAD_QUERY_SYNTAX
#ifdef ERROR_INVALID_FIELD
	{	ERROR_INVALID_FIELD,                                      "ERROR_INVALID_FIELD",
		"Record field does not exist.",
		"Record field does not exist."
	},
#endif // #ifdef ERROR_INVALID_FIELD
#ifdef ERROR_DEVICE_REMOVED
	{	ERROR_DEVICE_REMOVED,                                     "ERROR_DEVICE_REMOVED",
		"The device has been removed.",
		"The device has been removed."
	},
#endif // #ifdef ERROR_DEVICE_REMOVED
#ifdef ERROR_INSTALL_ALREADY_RUNNING
	{	ERROR_INSTALL_ALREADY_RUNNING,                            "ERROR_INSTALL_ALREADY_RUNNING",
		"Another installation is already in progress. Complete that installation before proceeding with this install.",
		"Another installation is already in progress. Complete that installation before proceeding with this install."
	},
#endif // #ifdef ERROR_INSTALL_ALREADY_RUNNING
#ifdef ERROR_INSTALL_PACKAGE_OPEN_FAILED
	{	ERROR_INSTALL_PACKAGE_OPEN_FAILED,                        "ERROR_INSTALL_PACKAGE_OPEN_FAILED",
		"This installation package could not be opened. Verify that the package exists and that you can access it, or contact the application vendor to verify that this is a valid Windows Installer package.",
		"This installation package could not be opened. Verify that the package exists and that you can access it, or contact the application vendor to verify that this is a valid Windows Installer package."
	},
#endif // #ifdef ERROR_INSTALL_PACKAGE_OPEN_FAILED
#ifdef ERROR_INSTALL_PACKAGE_INVALID
	{	ERROR_INSTALL_PACKAGE_INVALID,                            "ERROR_INSTALL_PACKAGE_INVALID",
		"This installation package could not be opened. Contact the application vendor to verify that this is a valid Windows Installer package.",
		"This installation package could not be opened. Contact the application vendor to verify that this is a valid Windows Installer package."
	},
#endif // #ifdef ERROR_INSTALL_PACKAGE_INVALID
#ifdef ERROR_INSTALL_UI_FAILURE
	{	ERROR_INSTALL_UI_FAILURE,                                 "ERROR_INSTALL_UI_FAILURE",
		"There was an error starting the Windows Installer service user interface. Contact your support personnel.",
		"There was an error starting the Windows Installer service user interface. Contact your support personnel."
	},
#endif // #ifdef ERROR_INSTALL_UI_FAILURE
#ifdef ERROR_INSTALL_LOG_FAILURE
	{	ERROR_INSTALL_LOG_FAILURE,                                "ERROR_INSTALL_LOG_FAILURE",
		"Error opening installation log file. Verify that the specified log file location exists and that you can write to it.",
		"Error opening installation log file. Verify that the specified log file location exists and that you can write to it."
	},
#endif // #ifdef ERROR_INSTALL_LOG_FAILURE
#ifdef ERROR_INSTALL_LANGUAGE_UNSUPPORTED
	{	ERROR_INSTALL_LANGUAGE_UNSUPPORTED,                       "ERROR_INSTALL_LANGUAGE_UNSUPPORTED",
		"The language of this installation package is not supported by your system.",
		"The language of this installation package is not supported by your system."
	},
#endif // #ifdef ERROR_INSTALL_LANGUAGE_UNSUPPORTED
#ifdef ERROR_INSTALL_TRANSFORM_FAILURE
	{	ERROR_INSTALL_TRANSFORM_FAILURE,                          "ERROR_INSTALL_TRANSFORM_FAILURE",
		"Error applying transforms. Verify that the specified transform paths are valid.",
		"Error applying transforms. Verify that the specified transform paths are valid."
	},
#endif // #ifdef ERROR_INSTALL_TRANSFORM_FAILURE
#ifdef ERROR_INSTALL_PACKAGE_REJECTED
	{	ERROR_INSTALL_PACKAGE_REJECTED,                           "ERROR_INSTALL_PACKAGE_REJECTED",
		"This installation is forbidden by system policy. Contact your system administrator.",
		"This installation is forbidden by system policy. Contact your system administrator."
	},
#endif // #ifdef ERROR_INSTALL_PACKAGE_REJECTED
#ifdef ERROR_FUNCTION_NOT_CALLED
	{	ERROR_FUNCTION_NOT_CALLED,                                "ERROR_FUNCTION_NOT_CALLED",
		"Function could not be executed.",
		"Function could not be executed."
	},
#endif // #ifdef ERROR_FUNCTION_NOT_CALLED
#ifdef ERROR_FUNCTION_FAILED
	{	ERROR_FUNCTION_FAILED,                                    "ERROR_FUNCTION_FAILED",
		"Function failed during execution.",
		"Function failed during execution."
	},
#endif // #ifdef ERROR_FUNCTION_FAILED
#ifdef ERROR_INVALID_TABLE
	{	ERROR_INVALID_TABLE,                                      "ERROR_INVALID_TABLE",
		"Invalid or unknown table specified.",
		"Invalid or unknown table specified."
	},
#endif // #ifdef ERROR_INVALID_TABLE
#ifdef ERROR_DATATYPE_MISMATCH
	{	ERROR_DATATYPE_MISMATCH,                                  "ERROR_DATATYPE_MISMATCH",
		"Data supplied is of wrong type.",
		"Data supplied is of wrong type."
	},
#endif // #ifdef ERROR_DATATYPE_MISMATCH
#ifdef ERROR_UNSUPPORTED_TYPE
	{	ERROR_UNSUPPORTED_TYPE,                                   "ERROR_UNSUPPORTED_TYPE",
		"Data of this type is not supported.",
		"Data of this type is not supported."
	},
#endif // #ifdef ERROR_UNSUPPORTED_TYPE
#ifdef ERROR_CREATE_FAILED
	{	ERROR_CREATE_FAILED,                                      "ERROR_CREATE_FAILED",
		"The Windows Installer service failed to start. Contact your support personnel.",
		"The Windows Installer service failed to start. Contact your support personnel."
	},
#endif // #ifdef ERROR_CREATE_FAILED
#ifdef ERROR_INSTALL_TEMP_UNWRITABLE
	{	ERROR_INSTALL_TEMP_UNWRITABLE,                            "ERROR_INSTALL_TEMP_UNWRITABLE",
		"The Temp folder is on a drive that is full or inaccessible. Free up space on the drive or verify that you have write permission on the Temp folder.",
		"The Temp folder is on a drive that is full or inaccessible. Free up space on the drive or verify that you have write permission on the Temp folder."
	},
#endif // #ifdef ERROR_INSTALL_TEMP_UNWRITABLE
#ifdef ERROR_INSTALL_PLATFORM_UNSUPPORTED
	{	ERROR_INSTALL_PLATFORM_UNSUPPORTED,                       "ERROR_INSTALL_PLATFORM_UNSUPPORTED",
		"This installation package is not supported by this processor type. Contact your product vendor.",
		"This installation package is not supported by this processor type. Contact your product vendor."
	},
#endif // #ifdef ERROR_INSTALL_PLATFORM_UNSUPPORTED
#ifdef ERROR_INSTALL_NOTUSED
	{	ERROR_INSTALL_NOTUSED,                                    "ERROR_INSTALL_NOTUSED",
		"Component not used on this computer.",
		"Component not used on this computer."
	},
#endif // #ifdef ERROR_INSTALL_NOTUSED
#ifdef ERROR_PATCH_PACKAGE_OPEN_FAILED
	{	ERROR_PATCH_PACKAGE_OPEN_FAILED,                          "ERROR_PATCH_PACKAGE_OPEN_FAILED",
		"This patch package could not be opened. Verify that the patch package exists and that you can access it, or contact the application vendor to verify that this is a valid Windows Installer patch package.",
		"This patch package could not be opened. Verify that the patch package exists and that you can access it, or contact the application vendor to verify that this is a valid Windows Installer patch package."
	},
#endif // #ifdef ERROR_PATCH_PACKAGE_OPEN_FAILED
#ifdef ERROR_PATCH_PACKAGE_INVALID
	{	ERROR_PATCH_PACKAGE_INVALID,                              "ERROR_PATCH_PACKAGE_INVALID",
		"This patch package could not be opened. Contact the application vendor to verify that this is a valid Windows Installer patch package.",
		"This patch package could not be opened. Contact the application vendor to verify that this is a valid Windows Installer patch package."
	},
#endif // #ifdef ERROR_PATCH_PACKAGE_INVALID
#ifdef ERROR_PATCH_PACKAGE_UNSUPPORTED
	{	ERROR_PATCH_PACKAGE_UNSUPPORTED,                          "ERROR_PATCH_PACKAGE_UNSUPPORTED",
		"This patch package cannot be processed by the Windows Installer service. You must install a Windows service pack that contains a newer version of the Windows Installer service.",
		"This patch package cannot be processed by the Windows Installer service. You must install a Windows service pack that contains a newer version of the Windows Installer service."
	},
#endif // #ifdef ERROR_PATCH_PACKAGE_UNSUPPORTED
#ifdef ERROR_PRODUCT_VERSION
	{	ERROR_PRODUCT_VERSION,                                    "ERROR_PRODUCT_VERSION",
		"Another version of this product is already installed. Installation of this version cannot continue. To configure or remove the existing version of this product, use Add/Remove Programs on the Control Panel.",
		"Another version of this product is already installed. Installation of this version cannot continue. To configure or remove the existing version of this product, use Add/Remove Programs on the Control Panel."
	},
#endif // #ifdef ERROR_PRODUCT_VERSION
#ifdef ERROR_INVALID_COMMAND_LINE
	{	ERROR_INVALID_COMMAND_LINE,                               "ERROR_INVALID_COMMAND_LINE",
		"Invalid command line argument. Consult the Windows Installer SDK for detailed command line help.",
		"Invalid command line argument. Consult the Windows Installer SDK for detailed command line help."
	},
#endif // #ifdef ERROR_INVALID_COMMAND_LINE
#ifdef ERROR_INSTALL_REMOTE_DISALLOWED
	{	ERROR_INSTALL_REMOTE_DISALLOWED,                          "ERROR_INSTALL_REMOTE_DISALLOWED",
		"Only administrators have permission to add, remove, or configure server software during a Terminal Services remote session. If you want to install or configure software on the server, contact your network administrator.",
		"Only administrators have permission to add, remove, or configure server software during a Terminal Services remote session. If you want to install or configure software on the server, contact your network administrator."
	},
#endif // #ifdef ERROR_INSTALL_REMOTE_DISALLOWED
#ifdef ERROR_SUCCESS_REBOOT_INITIATED
	{	ERROR_SUCCESS_REBOOT_INITIATED,                           "ERROR_SUCCESS_REBOOT_INITIATED",
		"The requested operation completed successfully. The system will be restarted so the changes can take effect.",
		"The requested operation completed successfully. The system will be restarted so the changes can take effect."
	},
#endif // #ifdef ERROR_SUCCESS_REBOOT_INITIATED
#ifdef ERROR_PATCH_TARGET_NOT_FOUND
	{	ERROR_PATCH_TARGET_NOT_FOUND,                             "ERROR_PATCH_TARGET_NOT_FOUND",
		"The upgrade patch cannot be installed by the Windows Installer service because the program to be upgraded may be missing, or the upgrade patch may update a different version of the program. Verify that the program to be upgraded exists on your computer and that you have the correct upgrade patch.",
		"The upgrade patch cannot be installed by the Windows Installer service because the program to be upgraded may be missing, or the upgrade patch may update a different version of the program. Verify that the program to be upgraded exists on your computer and that you have the correct upgrade patch."
	},
#endif // #ifdef ERROR_PATCH_TARGET_NOT_FOUND
#ifdef ERROR_PATCH_PACKAGE_REJECTED
	{	ERROR_PATCH_PACKAGE_REJECTED,                             "ERROR_PATCH_PACKAGE_REJECTED",
		"The patch package is not permitted by software restriction policy.",
		"The patch package is not permitted by software restriction policy."
	},
#endif // #ifdef ERROR_PATCH_PACKAGE_REJECTED
#ifdef ERROR_INSTALL_TRANSFORM_REJECTED
	{	ERROR_INSTALL_TRANSFORM_REJECTED,                         "ERROR_INSTALL_TRANSFORM_REJECTED",
		"One or more customizations are not permitted by software restriction policy.",
		"One or more customizations are not permitted by software restriction policy."
	},
#endif // #ifdef ERROR_INSTALL_TRANSFORM_REJECTED
#ifdef ERROR_INSTALL_REMOTE_PROHIBITED
	{	ERROR_INSTALL_REMOTE_PROHIBITED,                          "ERROR_INSTALL_REMOTE_PROHIBITED",
		"The Windows Installer does not permit installation from a Remote Desktop Connection.",
		"The Windows Installer does not permit installation from a Remote Desktop Connection."
	},
#endif // #ifdef ERROR_INSTALL_REMOTE_PROHIBITED
#ifdef RPC_S_INVALID_STRING_BINDING
	{	RPC_S_INVALID_STRING_BINDING,                             "RPC_S_INVALID_STRING_BINDING",
		"The string binding is invalid.",
		"The string binding is invalid."
	},
#endif // #ifdef RPC_S_INVALID_STRING_BINDING
#ifdef RPC_S_WRONG_KIND_OF_BINDING
	{	RPC_S_WRONG_KIND_OF_BINDING,                              "RPC_S_WRONG_KIND_OF_BINDING",
		"The binding handle is not the correct type.",
		"The binding handle is not the correct type."
	},
#endif // #ifdef RPC_S_WRONG_KIND_OF_BINDING
#ifdef RPC_S_INVALID_BINDING
	{	RPC_S_INVALID_BINDING,                                    "RPC_S_INVALID_BINDING",
		"The binding handle is invalid.",
		"The binding handle is invalid."
	},
#endif // #ifdef RPC_S_INVALID_BINDING
#ifdef RPC_S_PROTSEQ_NOT_SUPPORTED
	{	RPC_S_PROTSEQ_NOT_SUPPORTED,                              "RPC_S_PROTSEQ_NOT_SUPPORTED",
		"The RPC protocol sequence is not supported.",
		"The RPC protocol sequence is not supported."
	},
#endif // #ifdef RPC_S_PROTSEQ_NOT_SUPPORTED
#ifdef RPC_S_INVALID_RPC_PROTSEQ
	{	RPC_S_INVALID_RPC_PROTSEQ,                                "RPC_S_INVALID_RPC_PROTSEQ",
		"The RPC protocol sequence is invalid.",
		"The RPC protocol sequence is invalid."
	},
#endif // #ifdef RPC_S_INVALID_RPC_PROTSEQ
#ifdef RPC_S_INVALID_STRING_UUID
	{	RPC_S_INVALID_STRING_UUID,                                "RPC_S_INVALID_STRING_UUID",
		"The string universal unique identifier (UUID) is invalid.",
		"The string universal unique identifier (UUID) is invalid."
	},
#endif // #ifdef RPC_S_INVALID_STRING_UUID
#ifdef RPC_S_INVALID_ENDPOINT_FORMAT
	{	RPC_S_INVALID_ENDPOINT_FORMAT,                            "RPC_S_INVALID_ENDPOINT_FORMAT",
		"The endpoint format is invalid.",
		"The endpoint format is invalid."
	},
#endif // #ifdef RPC_S_INVALID_ENDPOINT_FORMAT
#ifdef RPC_S_INVALID_NET_ADDR
	{	RPC_S_INVALID_NET_ADDR,                                   "RPC_S_INVALID_NET_ADDR",
		"The network address is invalid.",
		"The network address is invalid."
	},
#endif // #ifdef RPC_S_INVALID_NET_ADDR
#ifdef RPC_S_NO_ENDPOINT_FOUND
	{	RPC_S_NO_ENDPOINT_FOUND,                                  "RPC_S_NO_ENDPOINT_FOUND",
		"No endpoint was found.",
		"No endpoint was found."
	},
#endif // #ifdef RPC_S_NO_ENDPOINT_FOUND
#ifdef RPC_S_INVALID_TIMEOUT
	{	RPC_S_INVALID_TIMEOUT,                                    "RPC_S_INVALID_TIMEOUT",
		"The timeout value is invalid.",
		"The timeout value is invalid."
	},
#endif // #ifdef RPC_S_INVALID_TIMEOUT
#ifdef RPC_S_OBJECT_NOT_FOUND
	{	RPC_S_OBJECT_NOT_FOUND,                                   "RPC_S_OBJECT_NOT_FOUND",
		"The object universal unique identifier (UUID) was not found.",
		"The object universal unique identifier (UUID) was not found."
	},
#endif // #ifdef RPC_S_OBJECT_NOT_FOUND
#ifdef RPC_S_ALREADY_REGISTERED
	{	RPC_S_ALREADY_REGISTERED,                                 "RPC_S_ALREADY_REGISTERED",
		"The object universal unique identifier (UUID) has already been registered.",
		"The object universal unique identifier (UUID) has already been registered."
	},
#endif // #ifdef RPC_S_ALREADY_REGISTERED
#ifdef RPC_S_TYPE_ALREADY_REGISTERED
	{	RPC_S_TYPE_ALREADY_REGISTERED,                            "RPC_S_TYPE_ALREADY_REGISTERED",
		"The type universal unique identifier (UUID) has already been registered.",
		"The type universal unique identifier (UUID) has already been registered."
	},
#endif // #ifdef RPC_S_TYPE_ALREADY_REGISTERED
#ifdef RPC_S_ALREADY_LISTENING
	{	RPC_S_ALREADY_LISTENING,                                  "RPC_S_ALREADY_LISTENING",
		"The RPC server is already listening.",
		"The RPC server is already listening."
	},
#endif // #ifdef RPC_S_ALREADY_LISTENING
#ifdef RPC_S_NO_PROTSEQS_REGISTERED
	{	RPC_S_NO_PROTSEQS_REGISTERED,                             "RPC_S_NO_PROTSEQS_REGISTERED",
		"No protocol sequences have been registered.",
		"No protocol sequences have been registered."
	},
#endif // #ifdef RPC_S_NO_PROTSEQS_REGISTERED
#ifdef RPC_S_NOT_LISTENING
	{	RPC_S_NOT_LISTENING,                                      "RPC_S_NOT_LISTENING",
		"The RPC server is not listening.",
		"The RPC server is not listening."
	},
#endif // #ifdef RPC_S_NOT_LISTENING
#ifdef RPC_S_UNKNOWN_MGR_TYPE
	{	RPC_S_UNKNOWN_MGR_TYPE,                                   "RPC_S_UNKNOWN_MGR_TYPE",
		"The manager type is unknown.",
		"The manager type is unknown."
	},
#endif // #ifdef RPC_S_UNKNOWN_MGR_TYPE
#ifdef RPC_S_UNKNOWN_IF
	{	RPC_S_UNKNOWN_IF,                                         "RPC_S_UNKNOWN_IF",
		"The interface is unknown.",
		"The interface is unknown."
	},
#endif // #ifdef RPC_S_UNKNOWN_IF
#ifdef RPC_S_NO_BINDINGS
	{	RPC_S_NO_BINDINGS,                                        "RPC_S_NO_BINDINGS",
		"There are no bindings.",
		"There are no bindings."
	},
#endif // #ifdef RPC_S_NO_BINDINGS
#ifdef RPC_S_NO_PROTSEQS
	{	RPC_S_NO_PROTSEQS,                                        "RPC_S_NO_PROTSEQS",
		"There are no protocol sequences.",
		"There are no protocol sequences."
	},
#endif // #ifdef RPC_S_NO_PROTSEQS
#ifdef RPC_S_CANT_CREATE_ENDPOINT
	{	RPC_S_CANT_CREATE_ENDPOINT,                               "RPC_S_CANT_CREATE_ENDPOINT",
		"The endpoint cannot be created.",
		"The endpoint cannot be created."
	},
#endif // #ifdef RPC_S_CANT_CREATE_ENDPOINT
#ifdef RPC_S_OUT_OF_RESOURCES
	{	RPC_S_OUT_OF_RESOURCES,                                   "RPC_S_OUT_OF_RESOURCES",
		"Not enough resources are available to complete this operation.",
		"Not enough resources are available to complete this operation."
	},
#endif // #ifdef RPC_S_OUT_OF_RESOURCES
#ifdef RPC_S_SERVER_UNAVAILABLE
	{	RPC_S_SERVER_UNAVAILABLE,                                 "RPC_S_SERVER_UNAVAILABLE",
		"The RPC server is unavailable.",
		"The RPC server is unavailable."
	},
#endif // #ifdef RPC_S_SERVER_UNAVAILABLE
#ifdef RPC_S_SERVER_TOO_BUSY
	{	RPC_S_SERVER_TOO_BUSY,                                    "RPC_S_SERVER_TOO_BUSY",
		"The RPC server is too busy to complete this operation.",
		"The RPC server is too busy to complete this operation."
	},
#endif // #ifdef RPC_S_SERVER_TOO_BUSY
#ifdef RPC_S_INVALID_NETWORK_OPTIONS
	{	RPC_S_INVALID_NETWORK_OPTIONS,                            "RPC_S_INVALID_NETWORK_OPTIONS",
		"The network options are invalid.",
		"The network options are invalid."
	},
#endif // #ifdef RPC_S_INVALID_NETWORK_OPTIONS
#ifdef RPC_S_NO_CALL_ACTIVE
	{	RPC_S_NO_CALL_ACTIVE,                                     "RPC_S_NO_CALL_ACTIVE",
		"There are no remote procedure calls active on this thread.",
		"There are no remote procedure calls active on this thread."
	},
#endif // #ifdef RPC_S_NO_CALL_ACTIVE
#ifdef RPC_S_CALL_FAILED
	{	RPC_S_CALL_FAILED,                                        "RPC_S_CALL_FAILED",
		"The remote procedure call failed.",
		"The remote procedure call failed."
	},
#endif // #ifdef RPC_S_CALL_FAILED
#ifdef RPC_S_CALL_FAILED_DNE
	{	RPC_S_CALL_FAILED_DNE,                                    "RPC_S_CALL_FAILED_DNE",
		"The remote procedure call failed and did not execute.",
		"The remote procedure call failed and did not execute."
	},
#endif // #ifdef RPC_S_CALL_FAILED_DNE
#ifdef RPC_S_PROTOCOL_ERROR
	{	RPC_S_PROTOCOL_ERROR,                                     "RPC_S_PROTOCOL_ERROR",
		"A remote procedure call (RPC) protocol error occurred.",
		"A remote procedure call (RPC) protocol error occurred."
	},
#endif // #ifdef RPC_S_PROTOCOL_ERROR
#ifdef RPC_S_UNSUPPORTED_TRANS_SYN
	{	RPC_S_UNSUPPORTED_TRANS_SYN,                              "RPC_S_UNSUPPORTED_TRANS_SYN",
		"The transfer syntax is not supported by the RPC server.",
		"The transfer syntax is not supported by the RPC server."
	},
#endif // #ifdef RPC_S_UNSUPPORTED_TRANS_SYN
#ifdef RPC_S_UNSUPPORTED_TYPE
	{	RPC_S_UNSUPPORTED_TYPE,                                   "RPC_S_UNSUPPORTED_TYPE",
		"The universal unique identifier (UUID) type is not supported.",
		"The universal unique identifier (UUID) type is not supported."
	},
#endif // #ifdef RPC_S_UNSUPPORTED_TYPE
#ifdef RPC_S_INVALID_TAG
	{	RPC_S_INVALID_TAG,                                        "RPC_S_INVALID_TAG",
		"The tag is invalid.",
		"The tag is invalid."
	},
#endif // #ifdef RPC_S_INVALID_TAG
#ifdef RPC_S_INVALID_BOUND
	{	RPC_S_INVALID_BOUND,                                      "RPC_S_INVALID_BOUND",
		"The array bounds are invalid.",
		"The array bounds are invalid."
	},
#endif // #ifdef RPC_S_INVALID_BOUND
#ifdef RPC_S_NO_ENTRY_NAME
	{	RPC_S_NO_ENTRY_NAME,                                      "RPC_S_NO_ENTRY_NAME",
		"The binding does not contain an entry name.",
		"The binding does not contain an entry name."
	},
#endif // #ifdef RPC_S_NO_ENTRY_NAME
#ifdef RPC_S_INVALID_NAME_SYNTAX
	{	RPC_S_INVALID_NAME_SYNTAX,                                "RPC_S_INVALID_NAME_SYNTAX",
		"The name syntax is invalid.",
		"The name syntax is invalid."
	},
#endif // #ifdef RPC_S_INVALID_NAME_SYNTAX
#ifdef RPC_S_UNSUPPORTED_NAME_SYNTAX
	{	RPC_S_UNSUPPORTED_NAME_SYNTAX,                            "RPC_S_UNSUPPORTED_NAME_SYNTAX",
		"The name syntax is not supported.",
		"The name syntax is not supported."
	},
#endif // #ifdef RPC_S_UNSUPPORTED_NAME_SYNTAX
#ifdef RPC_S_UUID_NO_ADDRESS
	{	RPC_S_UUID_NO_ADDRESS,                                    "RPC_S_UUID_NO_ADDRESS",
		"No network address is available to use to construct a universal unique identifier (UUID).",
		"No network address is available to use to construct a universal unique identifier (UUID)."
	},
#endif // #ifdef RPC_S_UUID_NO_ADDRESS
#ifdef RPC_S_DUPLICATE_ENDPOINT
	{	RPC_S_DUPLICATE_ENDPOINT,                                 "RPC_S_DUPLICATE_ENDPOINT",
		"The endpoint is a duplicate.",
		"The endpoint is a duplicate."
	},
#endif // #ifdef RPC_S_DUPLICATE_ENDPOINT
#ifdef RPC_S_UNKNOWN_AUTHN_TYPE
	{	RPC_S_UNKNOWN_AUTHN_TYPE,                                 "RPC_S_UNKNOWN_AUTHN_TYPE",
		"The authentication type is unknown.",
		"The authentication type is unknown."
	},
#endif // #ifdef RPC_S_UNKNOWN_AUTHN_TYPE
#ifdef RPC_S_MAX_CALLS_TOO_SMALL
	{	RPC_S_MAX_CALLS_TOO_SMALL,                                "RPC_S_MAX_CALLS_TOO_SMALL",
		"The maximum number of calls is too small.",
		"The maximum number of calls is too small."
	},
#endif // #ifdef RPC_S_MAX_CALLS_TOO_SMALL
#ifdef RPC_S_STRING_TOO_LONG
	{	RPC_S_STRING_TOO_LONG,                                    "RPC_S_STRING_TOO_LONG",
		"The string is too long.",
		"The string is too long."
	},
#endif // #ifdef RPC_S_STRING_TOO_LONG
#ifdef RPC_S_PROTSEQ_NOT_FOUND
	{	RPC_S_PROTSEQ_NOT_FOUND,                                  "RPC_S_PROTSEQ_NOT_FOUND",
		"The RPC protocol sequence was not found.",
		"The RPC protocol sequence was not found."
	},
#endif // #ifdef RPC_S_PROTSEQ_NOT_FOUND
#ifdef RPC_S_PROCNUM_OUT_OF_RANGE
	{	RPC_S_PROCNUM_OUT_OF_RANGE,                               "RPC_S_PROCNUM_OUT_OF_RANGE",
		"The procedure number is out of range.",
		"The procedure number is out of range."
	},
#endif // #ifdef RPC_S_PROCNUM_OUT_OF_RANGE
#ifdef RPC_S_BINDING_HAS_NO_AUTH
	{	RPC_S_BINDING_HAS_NO_AUTH,                                "RPC_S_BINDING_HAS_NO_AUTH",
		"The binding does not contain any authentication information.",
		"The binding does not contain any authentication information."
	},
#endif // #ifdef RPC_S_BINDING_HAS_NO_AUTH
#ifdef RPC_S_UNKNOWN_AUTHN_SERVICE
	{	RPC_S_UNKNOWN_AUTHN_SERVICE,                              "RPC_S_UNKNOWN_AUTHN_SERVICE",
		"The authentication service is unknown.",
		"The authentication service is unknown."
	},
#endif // #ifdef RPC_S_UNKNOWN_AUTHN_SERVICE
#ifdef RPC_S_UNKNOWN_AUTHN_LEVEL
	{	RPC_S_UNKNOWN_AUTHN_LEVEL,                                "RPC_S_UNKNOWN_AUTHN_LEVEL",
		"The authentication level is unknown.",
		"The authentication level is unknown."
	},
#endif // #ifdef RPC_S_UNKNOWN_AUTHN_LEVEL
#ifdef RPC_S_INVALID_AUTH_IDENTITY
	{	RPC_S_INVALID_AUTH_IDENTITY,                              "RPC_S_INVALID_AUTH_IDENTITY",
		"The security context is invalid.",
		"The security context is invalid."
	},
#endif // #ifdef RPC_S_INVALID_AUTH_IDENTITY
#ifdef RPC_S_UNKNOWN_AUTHZ_SERVICE
	{	RPC_S_UNKNOWN_AUTHZ_SERVICE,                              "RPC_S_UNKNOWN_AUTHZ_SERVICE",
		"The authorization service is unknown.",
		"The authorization service is unknown."
	},
#endif // #ifdef RPC_S_UNKNOWN_AUTHZ_SERVICE
#ifdef EPT_S_INVALID_ENTRY
	{	EPT_S_INVALID_ENTRY,                                      "EPT_S_INVALID_ENTRY",
		"The entry is invalid.",
		"The entry is invalid."
	},
#endif // #ifdef EPT_S_INVALID_ENTRY
#ifdef EPT_S_CANT_PERFORM_OP
	{	EPT_S_CANT_PERFORM_OP,                                    "EPT_S_CANT_PERFORM_OP",
		"The server endpoint cannot perform the operation.",
		"The server endpoint cannot perform the operation."
	},
#endif // #ifdef EPT_S_CANT_PERFORM_OP
#ifdef EPT_S_NOT_REGISTERED
	{	EPT_S_NOT_REGISTERED,                                     "EPT_S_NOT_REGISTERED",
		"There are no more endpoints available from the endpoint mapper.",
		"There are no more endpoints available from the endpoint mapper."
	},
#endif // #ifdef EPT_S_NOT_REGISTERED
#ifdef RPC_S_NOTHING_TO_EXPORT
	{	RPC_S_NOTHING_TO_EXPORT,                                  "RPC_S_NOTHING_TO_EXPORT",
		"No interfaces have been exported.",
		"No interfaces have been exported."
	},
#endif // #ifdef RPC_S_NOTHING_TO_EXPORT
#ifdef RPC_S_INCOMPLETE_NAME
	{	RPC_S_INCOMPLETE_NAME,                                    "RPC_S_INCOMPLETE_NAME",
		"The entry name is incomplete.",
		"The entry name is incomplete."
	},
#endif // #ifdef RPC_S_INCOMPLETE_NAME
#ifdef RPC_S_INVALID_VERS_OPTION
	{	RPC_S_INVALID_VERS_OPTION,                                "RPC_S_INVALID_VERS_OPTION",
		"The version option is invalid.",
		"The version option is invalid."
	},
#endif // #ifdef RPC_S_INVALID_VERS_OPTION
#ifdef RPC_S_NO_MORE_MEMBERS
	{	RPC_S_NO_MORE_MEMBERS,                                    "RPC_S_NO_MORE_MEMBERS",
		"There are no more members.",
		"There are no more members."
	},
#endif // #ifdef RPC_S_NO_MORE_MEMBERS
#ifdef RPC_S_NOT_ALL_OBJS_UNEXPORTED
	{	RPC_S_NOT_ALL_OBJS_UNEXPORTED,                            "RPC_S_NOT_ALL_OBJS_UNEXPORTED",
		"There is nothing to unexport.",
		"There is nothing to unexport."
	},
#endif // #ifdef RPC_S_NOT_ALL_OBJS_UNEXPORTED
#ifdef RPC_S_INTERFACE_NOT_FOUND
	{	RPC_S_INTERFACE_NOT_FOUND,                                "RPC_S_INTERFACE_NOT_FOUND",
		"The interface was not found.",
		"The interface was not found."
	},
#endif // #ifdef RPC_S_INTERFACE_NOT_FOUND
#ifdef RPC_S_ENTRY_ALREADY_EXISTS
	{	RPC_S_ENTRY_ALREADY_EXISTS,                               "RPC_S_ENTRY_ALREADY_EXISTS",
		"The entry already exists.",
		"The entry already exists."
	},
#endif // #ifdef RPC_S_ENTRY_ALREADY_EXISTS
#ifdef RPC_S_ENTRY_NOT_FOUND
	{	RPC_S_ENTRY_NOT_FOUND,                                    "RPC_S_ENTRY_NOT_FOUND",
		"The entry is not found.",
		"The entry is not found."
	},
#endif // #ifdef RPC_S_ENTRY_NOT_FOUND
#ifdef RPC_S_NAME_SERVICE_UNAVAILABLE
	{	RPC_S_NAME_SERVICE_UNAVAILABLE,                           "RPC_S_NAME_SERVICE_UNAVAILABLE",
		"The name service is unavailable.",
		"The name service is unavailable."
	},
#endif // #ifdef RPC_S_NAME_SERVICE_UNAVAILABLE
#ifdef RPC_S_INVALID_NAF_ID
	{	RPC_S_INVALID_NAF_ID,                                     "RPC_S_INVALID_NAF_ID",
		"The network address family is invalid.",
		"The network address family is invalid."
	},
#endif // #ifdef RPC_S_INVALID_NAF_ID
#ifdef RPC_S_CANNOT_SUPPORT
	{	RPC_S_CANNOT_SUPPORT,                                     "RPC_S_CANNOT_SUPPORT",
		"The requested operation is not supported.",
		"The requested operation is not supported."
	},
#endif // #ifdef RPC_S_CANNOT_SUPPORT
#ifdef RPC_S_NO_CONTEXT_AVAILABLE
	{	RPC_S_NO_CONTEXT_AVAILABLE,                               "RPC_S_NO_CONTEXT_AVAILABLE",
		"No security context is available to allow impersonation.",
		"No security context is available to allow impersonation."
	},
#endif // #ifdef RPC_S_NO_CONTEXT_AVAILABLE
#ifdef RPC_S_INTERNAL_ERROR
	{	RPC_S_INTERNAL_ERROR,                                     "RPC_S_INTERNAL_ERROR",
		"An internal error occurred in a remote procedure call (RPC).",
		"An internal error occurred in a remote procedure call (RPC)."
	},
#endif // #ifdef RPC_S_INTERNAL_ERROR
#ifdef RPC_S_ZERO_DIVIDE
	{	RPC_S_ZERO_DIVIDE,                                        "RPC_S_ZERO_DIVIDE",
		"The RPC server attempted an integer division by zero.",
		"The RPC server attempted an integer division by zero."
	},
#endif // #ifdef RPC_S_ZERO_DIVIDE
#ifdef RPC_S_ADDRESS_ERROR
	{	RPC_S_ADDRESS_ERROR,                                      "RPC_S_ADDRESS_ERROR",
		"An addressing error occurred in the RPC server.",
		"An addressing error occurred in the RPC server."
	},
#endif // #ifdef RPC_S_ADDRESS_ERROR
#ifdef RPC_S_FP_DIV_ZERO
	{	RPC_S_FP_DIV_ZERO,                                        "RPC_S_FP_DIV_ZERO",
		"A floating-point operation at the RPC server caused a division by zero.",
		"A floating-point operation at the RPC server caused a division by zero."
	},
#endif // #ifdef RPC_S_FP_DIV_ZERO
#ifdef RPC_S_FP_UNDERFLOW
	{	RPC_S_FP_UNDERFLOW,                                       "RPC_S_FP_UNDERFLOW",
		"A floating-point underflow occurred at the RPC server.",
		"A floating-point underflow occurred at the RPC server."
	},
#endif // #ifdef RPC_S_FP_UNDERFLOW
#ifdef RPC_S_FP_OVERFLOW
	{	RPC_S_FP_OVERFLOW,                                        "RPC_S_FP_OVERFLOW",
		"A floating-point overflow occurred at the RPC server.",
		"A floating-point overflow occurred at the RPC server."
	},
#endif // #ifdef RPC_S_FP_OVERFLOW
#ifdef RPC_X_NO_MORE_ENTRIES
	{	RPC_X_NO_MORE_ENTRIES,                                    "RPC_X_NO_MORE_ENTRIES",
		"The list of RPC servers available for the binding of auto handles has been exhausted.",
		"The list of RPC servers available for the binding of auto handles has been exhausted."
	},
#endif // #ifdef RPC_X_NO_MORE_ENTRIES
#ifdef RPC_X_SS_CHAR_TRANS_OPEN_FAIL
	{	RPC_X_SS_CHAR_TRANS_OPEN_FAIL,                            "RPC_X_SS_CHAR_TRANS_OPEN_FAIL",
		"Unable to open the character translation table file.",
		"Unable to open the character translation table file."
	},
#endif // #ifdef RPC_X_SS_CHAR_TRANS_OPEN_FAIL
#ifdef RPC_X_SS_CHAR_TRANS_SHORT_FILE
	{	RPC_X_SS_CHAR_TRANS_SHORT_FILE,                           "RPC_X_SS_CHAR_TRANS_SHORT_FILE",
		"The file containing the character translation table has fewer than 512 bytes.",
		"The file containing the character translation table has fewer than 512 bytes."
	},
#endif // #ifdef RPC_X_SS_CHAR_TRANS_SHORT_FILE
#ifdef RPC_X_SS_IN_NULL_CONTEXT
	{	RPC_X_SS_IN_NULL_CONTEXT,                                 "RPC_X_SS_IN_NULL_CONTEXT",
		"A null context handle was passed from the client to the host during a remote procedure call.",
		"A null context handle was passed from the client to the host during a remote procedure call."
	},
#endif // #ifdef RPC_X_SS_IN_NULL_CONTEXT
#ifdef RPC_X_SS_CONTEXT_DAMAGED
	{	RPC_X_SS_CONTEXT_DAMAGED,                                 "RPC_X_SS_CONTEXT_DAMAGED",
		"The context handle changed during a remote procedure call.",
		"The context handle changed during a remote procedure call."
	},
#endif // #ifdef RPC_X_SS_CONTEXT_DAMAGED
#ifdef RPC_X_SS_HANDLES_MISMATCH
	{	RPC_X_SS_HANDLES_MISMATCH,                                "RPC_X_SS_HANDLES_MISMATCH",
		"The binding handles passed to a remote procedure call do not match.",
		"The binding handles passed to a remote procedure call do not match."
	},
#endif // #ifdef RPC_X_SS_HANDLES_MISMATCH
#ifdef RPC_X_SS_CANNOT_GET_CALL_HANDLE
	{	RPC_X_SS_CANNOT_GET_CALL_HANDLE,                          "RPC_X_SS_CANNOT_GET_CALL_HANDLE",
		"The stub is unable to get the remote procedure call handle.",
		"The stub is unable to get the remote procedure call handle."
	},
#endif // #ifdef RPC_X_SS_CANNOT_GET_CALL_HANDLE
#ifdef RPC_X_NULL_REF_POINTER
	{	RPC_X_NULL_REF_POINTER,                                   "RPC_X_NULL_REF_POINTER",
		"A null reference pointer was passed to the stub.",
		"A null reference pointer was passed to the stub."
	},
#endif // #ifdef RPC_X_NULL_REF_POINTER
#ifdef RPC_X_ENUM_VALUE_OUT_OF_RANGE
	{	RPC_X_ENUM_VALUE_OUT_OF_RANGE,                            "RPC_X_ENUM_VALUE_OUT_OF_RANGE",
		"The enumeration value is out of range.",
		"The enumeration value is out of range."
	},
#endif // #ifdef RPC_X_ENUM_VALUE_OUT_OF_RANGE
#ifdef RPC_X_BYTE_COUNT_TOO_SMALL
	{	RPC_X_BYTE_COUNT_TOO_SMALL,                               "RPC_X_BYTE_COUNT_TOO_SMALL",
		"The byte count is too small.",
		"The byte count is too small."
	},
#endif // #ifdef RPC_X_BYTE_COUNT_TOO_SMALL
#ifdef RPC_X_BAD_STUB_DATA
	{	RPC_X_BAD_STUB_DATA,                                      "RPC_X_BAD_STUB_DATA",
		"The stub received bad data.",
		"The stub received bad data."
	},
#endif // #ifdef RPC_X_BAD_STUB_DATA
#ifdef ERROR_INVALID_USER_BUFFER
	{	ERROR_INVALID_USER_BUFFER,                                "ERROR_INVALID_USER_BUFFER",
		"The supplied user buffer is not valid for the requested operation.",
		"The supplied user buffer is not valid for the requested operation."
	},
#endif // #ifdef ERROR_INVALID_USER_BUFFER
#ifdef ERROR_UNRECOGNIZED_MEDIA
	{	ERROR_UNRECOGNIZED_MEDIA,                                 "ERROR_UNRECOGNIZED_MEDIA",
		"The disk media is not recognized. It may not be formatted.",
		"The disk media is not recognized. It may not be formatted."
	},
#endif // #ifdef ERROR_UNRECOGNIZED_MEDIA
#ifdef ERROR_NO_TRUST_LSA_SECRET
	{	ERROR_NO_TRUST_LSA_SECRET,                                "ERROR_NO_TRUST_LSA_SECRET",
		"The workstation does not have a trust secret.",
		"The workstation does not have a trust secret."
	},
#endif // #ifdef ERROR_NO_TRUST_LSA_SECRET
#ifdef ERROR_NO_TRUST_SAM_ACCOUNT
	{	ERROR_NO_TRUST_SAM_ACCOUNT,                               "ERROR_NO_TRUST_SAM_ACCOUNT",
		"The security database on the server does not have a computer account for this workstation trust relationship.",
		"The security database on the server does not have a computer account for this workstation trust relationship."
	},
#endif // #ifdef ERROR_NO_TRUST_SAM_ACCOUNT
#ifdef ERROR_TRUSTED_DOMAIN_FAILURE
	{	ERROR_TRUSTED_DOMAIN_FAILURE,                             "ERROR_TRUSTED_DOMAIN_FAILURE",
		"The trust relationship between the primary domain and the trusted domain failed.",
		"The trust relationship between the primary domain and the trusted domain failed."
	},
#endif // #ifdef ERROR_TRUSTED_DOMAIN_FAILURE
#ifdef ERROR_TRUSTED_RELATIONSHIP_FAILURE
	{	ERROR_TRUSTED_RELATIONSHIP_FAILURE,                       "ERROR_TRUSTED_RELATIONSHIP_FAILURE",
		"The trust relationship between this workstation and the primary domain failed.",
		"The trust relationship between this workstation and the primary domain failed."
	},
#endif // #ifdef ERROR_TRUSTED_RELATIONSHIP_FAILURE
#ifdef ERROR_TRUST_FAILURE
	{	ERROR_TRUST_FAILURE,                                      "ERROR_TRUST_FAILURE",
		"The network logon failed.",
		"The network logon failed."
	},
#endif // #ifdef ERROR_TRUST_FAILURE
#ifdef RPC_S_CALL_IN_PROGRESS
	{	RPC_S_CALL_IN_PROGRESS,                                   "RPC_S_CALL_IN_PROGRESS",
		"A remote procedure call is already in progress for this thread.",
		"A remote procedure call is already in progress for this thread."
	},
#endif // #ifdef RPC_S_CALL_IN_PROGRESS
#ifdef ERROR_NETLOGON_NOT_STARTED
	{	ERROR_NETLOGON_NOT_STARTED,                               "ERROR_NETLOGON_NOT_STARTED",
		"An attempt was made to logon, but the network logon service was not started.",
		"An attempt was made to logon, but the network logon service was not started."
	},
#endif // #ifdef ERROR_NETLOGON_NOT_STARTED
#ifdef ERROR_ACCOUNT_EXPIRED
	{	ERROR_ACCOUNT_EXPIRED,                                    "ERROR_ACCOUNT_EXPIRED",
		"The user's account has expired.",
		"The user's account has expired."
	},
#endif // #ifdef ERROR_ACCOUNT_EXPIRED
#ifdef ERROR_REDIRECTOR_HAS_OPEN_HANDLES
	{	ERROR_REDIRECTOR_HAS_OPEN_HANDLES,                        "ERROR_REDIRECTOR_HAS_OPEN_HANDLES",
		"The redirector is in use and cannot be unloaded.",
		"The redirector is in use and cannot be unloaded."
	},
#endif // #ifdef ERROR_REDIRECTOR_HAS_OPEN_HANDLES
#ifdef ERROR_PRINTER_DRIVER_ALREADY_INSTALLED
	{	ERROR_PRINTER_DRIVER_ALREADY_INSTALLED,                   "ERROR_PRINTER_DRIVER_ALREADY_INSTALLED",
		"The specified printer driver is already installed.",
		"The specified printer driver is already installed."
	},
#endif // #ifdef ERROR_PRINTER_DRIVER_ALREADY_INSTALLED
#ifdef ERROR_UNKNOWN_PORT
	{	ERROR_UNKNOWN_PORT,                                       "ERROR_UNKNOWN_PORT",
		"The specified port is unknown.",
		"The specified port is unknown."
	},
#endif // #ifdef ERROR_UNKNOWN_PORT
#ifdef ERROR_UNKNOWN_PRINTER_DRIVER
	{	ERROR_UNKNOWN_PRINTER_DRIVER,                             "ERROR_UNKNOWN_PRINTER_DRIVER",
		"The printer driver is unknown.",
		"The printer driver is unknown."
	},
#endif // #ifdef ERROR_UNKNOWN_PRINTER_DRIVER
#ifdef ERROR_UNKNOWN_PRINTPROCESSOR
	{	ERROR_UNKNOWN_PRINTPROCESSOR,                             "ERROR_UNKNOWN_PRINTPROCESSOR",
		"The print processor is unknown.",
		"The print processor is unknown."
	},
#endif // #ifdef ERROR_UNKNOWN_PRINTPROCESSOR
#ifdef ERROR_INVALID_SEPARATOR_FILE
	{	ERROR_INVALID_SEPARATOR_FILE,                             "ERROR_INVALID_SEPARATOR_FILE",
		"The specified separator file is invalid.",
		"The specified separator file is invalid."
	},
#endif // #ifdef ERROR_INVALID_SEPARATOR_FILE
#ifdef ERROR_INVALID_PRIORITY
	{	ERROR_INVALID_PRIORITY,                                   "ERROR_INVALID_PRIORITY",
		"The specified priority is invalid.",
		"The specified priority is invalid."
	},
#endif // #ifdef ERROR_INVALID_PRIORITY
#ifdef ERROR_INVALID_PRINTER_NAME
	{	ERROR_INVALID_PRINTER_NAME,                               "ERROR_INVALID_PRINTER_NAME",
		"The printer name is invalid.",
		"The printer name is invalid."
	},
#endif // #ifdef ERROR_INVALID_PRINTER_NAME
#ifdef ERROR_PRINTER_ALREADY_EXISTS
	{	ERROR_PRINTER_ALREADY_EXISTS,                             "ERROR_PRINTER_ALREADY_EXISTS",
		"The printer already exists.",
		"The printer already exists."
	},
#endif // #ifdef ERROR_PRINTER_ALREADY_EXISTS
#ifdef ERROR_INVALID_PRINTER_COMMAND
	{	ERROR_INVALID_PRINTER_COMMAND,                            "ERROR_INVALID_PRINTER_COMMAND",
		"The printer command is invalid.",
		"The printer command is invalid."
	},
#endif // #ifdef ERROR_INVALID_PRINTER_COMMAND
#ifdef ERROR_INVALID_DATATYPE
	{	ERROR_INVALID_DATATYPE,                                   "ERROR_INVALID_DATATYPE",
		"The specified datatype is invalid.",
		"The specified datatype is invalid."
	},
#endif // #ifdef ERROR_INVALID_DATATYPE
#ifdef ERROR_INVALID_ENVIRONMENT
	{	ERROR_INVALID_ENVIRONMENT,                                "ERROR_INVALID_ENVIRONMENT",
		"The environment specified is invalid.",
		"The environment specified is invalid."
	},
#endif // #ifdef ERROR_INVALID_ENVIRONMENT
#ifdef RPC_S_NO_MORE_BINDINGS
	{	RPC_S_NO_MORE_BINDINGS,                                   "RPC_S_NO_MORE_BINDINGS",
		"There are no more bindings.",
		"There are no more bindings."
	},
#endif // #ifdef RPC_S_NO_MORE_BINDINGS
#ifdef ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT
	{	ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT,                  "ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT",
		"The account used is an interdomain trust account. Use your global user account or local user account to access this server.",
		"The account used is an interdomain trust account. Use your global user account or local user account to access this server."
	},
#endif // #ifdef ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT
#ifdef ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT
	{	ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT,                  "ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT",
		"The account used is a computer account. Use your global user account or local user account to access this server.",
		"The account used is a computer account. Use your global user account or local user account to access this server."
	},
#endif // #ifdef ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT
#ifdef ERROR_NOLOGON_SERVER_TRUST_ACCOUNT
	{	ERROR_NOLOGON_SERVER_TRUST_ACCOUNT,                       "ERROR_NOLOGON_SERVER_TRUST_ACCOUNT",
		"The account used is a server trust account. Use your global user account or local user account to access this server.",
		"The account used is a server trust account. Use your global user account or local user account to access this server."
	},
#endif // #ifdef ERROR_NOLOGON_SERVER_TRUST_ACCOUNT
#ifdef ERROR_DOMAIN_TRUST_INCONSISTENT
	{	ERROR_DOMAIN_TRUST_INCONSISTENT,                          "ERROR_DOMAIN_TRUST_INCONSISTENT",
		"The name or security ID (SID) of the domain specified is inconsistent with the trust information for that domain.",
		"The name or security ID (SID) of the domain specified is inconsistent with the trust information for that domain."
	},
#endif // #ifdef ERROR_DOMAIN_TRUST_INCONSISTENT
#ifdef ERROR_SERVER_HAS_OPEN_HANDLES
	{	ERROR_SERVER_HAS_OPEN_HANDLES,                            "ERROR_SERVER_HAS_OPEN_HANDLES",
		"The server is in use and cannot be unloaded.",
		"The server is in use and cannot be unloaded."
	},
#endif // #ifdef ERROR_SERVER_HAS_OPEN_HANDLES
#ifdef ERROR_RESOURCE_DATA_NOT_FOUND
	{	ERROR_RESOURCE_DATA_NOT_FOUND,                            "ERROR_RESOURCE_DATA_NOT_FOUND",
		"The specified image file did not contain a resource section.",
		"The specified image file did not contain a resource section."
	},
#endif // #ifdef ERROR_RESOURCE_DATA_NOT_FOUND
#ifdef ERROR_RESOURCE_TYPE_NOT_FOUND
	{	ERROR_RESOURCE_TYPE_NOT_FOUND,                            "ERROR_RESOURCE_TYPE_NOT_FOUND",
		"The specified resource type cannot be found in the image file.",
		"The specified resource type cannot be found in the image file."
	},
#endif // #ifdef ERROR_RESOURCE_TYPE_NOT_FOUND
#ifdef ERROR_RESOURCE_NAME_NOT_FOUND
	{	ERROR_RESOURCE_NAME_NOT_FOUND,                            "ERROR_RESOURCE_NAME_NOT_FOUND",
		"The specified resource name cannot be found in the image file.",
		"The specified resource name cannot be found in the image file."
	},
#endif // #ifdef ERROR_RESOURCE_NAME_NOT_FOUND
#ifdef ERROR_RESOURCE_LANG_NOT_FOUND
	{	ERROR_RESOURCE_LANG_NOT_FOUND,                            "ERROR_RESOURCE_LANG_NOT_FOUND",
		"The specified resource language ID cannot be found in the image file.",
		"The specified resource language ID cannot be found in the image file."
	},
#endif // #ifdef ERROR_RESOURCE_LANG_NOT_FOUND
#ifdef ERROR_NOT_ENOUGH_QUOTA
	{	ERROR_NOT_ENOUGH_QUOTA,                                   "ERROR_NOT_ENOUGH_QUOTA",
		"Not enough quota is available to process this command.",
		"Not enough quota is available to process this command."
	},
#endif // #ifdef ERROR_NOT_ENOUGH_QUOTA
#ifdef RPC_S_NO_INTERFACES
	{	RPC_S_NO_INTERFACES,                                      "RPC_S_NO_INTERFACES",
		"No interfaces have been registered.",
		"No interfaces have been registered."
	},
#endif // #ifdef RPC_S_NO_INTERFACES
#ifdef RPC_S_CALL_CANCELLED
	{	RPC_S_CALL_CANCELLED,                                     "RPC_S_CALL_CANCELLED",
		"The remote procedure call was cancelled.",
		"The remote procedure call was cancelled."
	},
#endif // #ifdef RPC_S_CALL_CANCELLED
#ifdef RPC_S_BINDING_INCOMPLETE
	{	RPC_S_BINDING_INCOMPLETE,                                 "RPC_S_BINDING_INCOMPLETE",
		"The binding handle does not contain all required information.",
		"The binding handle does not contain all required information."
	},
#endif // #ifdef RPC_S_BINDING_INCOMPLETE
#ifdef RPC_S_COMM_FAILURE
	{	RPC_S_COMM_FAILURE,                                       "RPC_S_COMM_FAILURE",
		"A communications failure occurred during a remote procedure call.",
		"A communications failure occurred during a remote procedure call."
	},
#endif // #ifdef RPC_S_COMM_FAILURE
#ifdef RPC_S_UNSUPPORTED_AUTHN_LEVEL
	{	RPC_S_UNSUPPORTED_AUTHN_LEVEL,                            "RPC_S_UNSUPPORTED_AUTHN_LEVEL",
		"The requested authentication level is not supported.",
		"The requested authentication level is not supported."
	},
#endif // #ifdef RPC_S_UNSUPPORTED_AUTHN_LEVEL
#ifdef RPC_S_NO_PRINC_NAME
	{	RPC_S_NO_PRINC_NAME,                                      "RPC_S_NO_PRINC_NAME",
		"No principal name registered.",
		"No principal name registered."
	},
#endif // #ifdef RPC_S_NO_PRINC_NAME
#ifdef RPC_S_NOT_RPC_ERROR
	{	RPC_S_NOT_RPC_ERROR,                                      "RPC_S_NOT_RPC_ERROR",
		"The error specified is not a valid Windows RPC error code.",
		"The error specified is not a valid Windows RPC error code."
	},
#endif // #ifdef RPC_S_NOT_RPC_ERROR
#ifdef RPC_S_UUID_LOCAL_ONLY
	{	RPC_S_UUID_LOCAL_ONLY,                                    "RPC_S_UUID_LOCAL_ONLY",
		"A UUID that is valid only on this computer has been allocated.",
		"A UUID that is valid only on this computer has been allocated."
	},
#endif // #ifdef RPC_S_UUID_LOCAL_ONLY
#ifdef RPC_S_SEC_PKG_ERROR
	{	RPC_S_SEC_PKG_ERROR,                                      "RPC_S_SEC_PKG_ERROR",
		"A security package specific error occurred.",
		"A security package specific error occurred."
	},
#endif // #ifdef RPC_S_SEC_PKG_ERROR
#ifdef RPC_S_NOT_CANCELLED
	{	RPC_S_NOT_CANCELLED,                                      "RPC_S_NOT_CANCELLED",
		"Thread is not canceled.",
		"Thread is not canceled."
	},
#endif // #ifdef RPC_S_NOT_CANCELLED
#ifdef RPC_X_INVALID_ES_ACTION
	{	RPC_X_INVALID_ES_ACTION,                                  "RPC_X_INVALID_ES_ACTION",
		"Invalid operation on the encoding/decoding handle.",
		"Invalid operation on the encoding/decoding handle."
	},
#endif // #ifdef RPC_X_INVALID_ES_ACTION
#ifdef RPC_X_WRONG_ES_VERSION
	{	RPC_X_WRONG_ES_VERSION,                                   "RPC_X_WRONG_ES_VERSION",
		"Incompatible version of the serializing package.",
		"Incompatible version of the serializing package."
	},
#endif // #ifdef RPC_X_WRONG_ES_VERSION
#ifdef RPC_X_WRONG_STUB_VERSION
	{	RPC_X_WRONG_STUB_VERSION,                                 "RPC_X_WRONG_STUB_VERSION",
		"Incompatible version of the RPC stub.",
		"Incompatible version of the RPC stub."
	},
#endif // #ifdef RPC_X_WRONG_STUB_VERSION
#ifdef RPC_X_INVALID_PIPE_OBJECT
	{	RPC_X_INVALID_PIPE_OBJECT,                                "RPC_X_INVALID_PIPE_OBJECT",
		"The RPC pipe object is invalid or corrupted.",
		"The RPC pipe object is invalid or corrupted."
	},
#endif // #ifdef RPC_X_INVALID_PIPE_OBJECT
#ifdef RPC_X_WRONG_PIPE_ORDER
	{	RPC_X_WRONG_PIPE_ORDER,                                   "RPC_X_WRONG_PIPE_ORDER",
		"An invalid operation was attempted on an RPC pipe object.",
		"An invalid operation was attempted on an RPC pipe object."
	},
#endif // #ifdef RPC_X_WRONG_PIPE_ORDER
#ifdef RPC_X_WRONG_PIPE_VERSION
	{	RPC_X_WRONG_PIPE_VERSION,                                 "RPC_X_WRONG_PIPE_VERSION",
		"Unsupported RPC pipe version.",
		"Unsupported RPC pipe version."
	},
#endif // #ifdef RPC_X_WRONG_PIPE_VERSION
#ifdef RPC_S_GROUP_MEMBER_NOT_FOUND
	{	RPC_S_GROUP_MEMBER_NOT_FOUND,                             "RPC_S_GROUP_MEMBER_NOT_FOUND",
		"The group member was not found.",
		"The group member was not found."
	},
#endif // #ifdef RPC_S_GROUP_MEMBER_NOT_FOUND
#ifdef EPT_S_CANT_CREATE
	{	EPT_S_CANT_CREATE,                                        "EPT_S_CANT_CREATE",
		"The endpoint mapper database entry could not be created.",
		"The endpoint mapper database entry could not be created."
	},
#endif // #ifdef EPT_S_CANT_CREATE
#ifdef RPC_S_INVALID_OBJECT
	{	RPC_S_INVALID_OBJECT,                                     "RPC_S_INVALID_OBJECT",
		"The object universal unique identifier (UUID) is the nil UUID.",
		"The object universal unique identifier (UUID) is the nil UUID."
	},
#endif // #ifdef RPC_S_INVALID_OBJECT
#ifdef ERROR_INVALID_TIME
	{	ERROR_INVALID_TIME,                                       "ERROR_INVALID_TIME",
		"The specified time is invalid.",
		"The specified time is invalid."
	},
#endif // #ifdef ERROR_INVALID_TIME
#ifdef ERROR_INVALID_FORM_NAME
	{	ERROR_INVALID_FORM_NAME,                                  "ERROR_INVALID_FORM_NAME",
		"The specified form name is invalid.",
		"The specified form name is invalid."
	},
#endif // #ifdef ERROR_INVALID_FORM_NAME
#ifdef ERROR_INVALID_FORM_SIZE
	{	ERROR_INVALID_FORM_SIZE,                                  "ERROR_INVALID_FORM_SIZE",
		"The specified form size is invalid.",
		"The specified form size is invalid."
	},
#endif // #ifdef ERROR_INVALID_FORM_SIZE
#ifdef ERROR_ALREADY_WAITING
	{	ERROR_ALREADY_WAITING,                                    "ERROR_ALREADY_WAITING",
		"The specified printer handle is already being waited on",
		"The specified printer handle is already being waited on"
	},
#endif // #ifdef ERROR_ALREADY_WAITING
#ifdef ERROR_PRINTER_DELETED
	{	ERROR_PRINTER_DELETED,                                    "ERROR_PRINTER_DELETED",
		"The specified printer has been deleted.",
		"The specified printer has been deleted."
	},
#endif // #ifdef ERROR_PRINTER_DELETED
#ifdef ERROR_INVALID_PRINTER_STATE
	{	ERROR_INVALID_PRINTER_STATE,                              "ERROR_INVALID_PRINTER_STATE",
		"The state of the printer is invalid.",
		"The state of the printer is invalid."
	},
#endif // #ifdef ERROR_INVALID_PRINTER_STATE
#ifdef ERROR_PASSWORD_MUST_CHANGE
	{	ERROR_PASSWORD_MUST_CHANGE,                               "ERROR_PASSWORD_MUST_CHANGE",
		"The user's password must be changed before logging on the first time.",
		"The user's password must be changed before logging on the first time."
	},
#endif // #ifdef ERROR_PASSWORD_MUST_CHANGE
#ifdef ERROR_DOMAIN_CONTROLLER_NOT_FOUND
	{	ERROR_DOMAIN_CONTROLLER_NOT_FOUND,                        "ERROR_DOMAIN_CONTROLLER_NOT_FOUND",
		"Could not find the domain controller for this domain.",
		"Could not find the domain controller for this domain."
	},
#endif // #ifdef ERROR_DOMAIN_CONTROLLER_NOT_FOUND
#ifdef ERROR_ACCOUNT_LOCKED_OUT
	{	ERROR_ACCOUNT_LOCKED_OUT,                                 "ERROR_ACCOUNT_LOCKED_OUT",
		"The referenced account is currently locked out and may not be logged on to.",
		"The referenced account is currently locked out and may not be logged on to."
	},
#endif // #ifdef ERROR_ACCOUNT_LOCKED_OUT
#ifdef OR_INVALID_OXID
	{	OR_INVALID_OXID,                                          "OR_INVALID_OXID",
		"The object exporter specified was not found.",
		"The object exporter specified was not found."
	},
#endif // #ifdef OR_INVALID_OXID
#ifdef OR_INVALID_OID
	{	OR_INVALID_OID,                                           "OR_INVALID_OID",
		"The object specified was not found.",
		"The object specified was not found."
	},
#endif // #ifdef OR_INVALID_OID
#ifdef OR_INVALID_SET
	{	OR_INVALID_SET,                                           "OR_INVALID_SET",
		"The object resolver set specified was not found.",
		"The object resolver set specified was not found."
	},
#endif // #ifdef OR_INVALID_SET
#ifdef RPC_S_SEND_INCOMPLETE
	{	RPC_S_SEND_INCOMPLETE,                                    "RPC_S_SEND_INCOMPLETE",
		"Some data remains to be sent in the request buffer.",
		"Some data remains to be sent in the request buffer."
	},
#endif // #ifdef RPC_S_SEND_INCOMPLETE
#ifdef RPC_S_INVALID_ASYNC_HANDLE
	{	RPC_S_INVALID_ASYNC_HANDLE,                               "RPC_S_INVALID_ASYNC_HANDLE",
		"Invalid asynchronous remote procedure call handle.",
		"Invalid asynchronous remote procedure call handle."
	},
#endif // #ifdef RPC_S_INVALID_ASYNC_HANDLE
#ifdef RPC_S_INVALID_ASYNC_CALL
	{	RPC_S_INVALID_ASYNC_CALL,                                 "RPC_S_INVALID_ASYNC_CALL",
		"Invalid asynchronous RPC call handle for this operation.",
		"Invalid asynchronous RPC call handle for this operation."
	},
#endif // #ifdef RPC_S_INVALID_ASYNC_CALL
#ifdef RPC_X_PIPE_CLOSED
	{	RPC_X_PIPE_CLOSED,                                        "RPC_X_PIPE_CLOSED",
		"The RPC pipe object has already been closed.",
		"The RPC pipe object has already been closed."
	},
#endif // #ifdef RPC_X_PIPE_CLOSED
#ifdef RPC_X_PIPE_DISCIPLINE_ERROR
	{	RPC_X_PIPE_DISCIPLINE_ERROR,                              "RPC_X_PIPE_DISCIPLINE_ERROR",
		"The RPC call completed before all pipes were processed.",
		"The RPC call completed before all pipes were processed."
	},
#endif // #ifdef RPC_X_PIPE_DISCIPLINE_ERROR
#ifdef RPC_X_PIPE_EMPTY
	{	RPC_X_PIPE_EMPTY,                                         "RPC_X_PIPE_EMPTY",
		"No more data is available from the RPC pipe.",
		"No more data is available from the RPC pipe."
	},
#endif // #ifdef RPC_X_PIPE_EMPTY
#ifdef ERROR_NO_SITENAME
	{	ERROR_NO_SITENAME,                                        "ERROR_NO_SITENAME",
		"No site name is available for this machine.",
		"No site name is available for this machine."
	},
#endif // #ifdef ERROR_NO_SITENAME
#ifdef ERROR_CANT_ACCESS_FILE
	{	ERROR_CANT_ACCESS_FILE,                                   "ERROR_CANT_ACCESS_FILE",
		"The file cannot be accessed by the system.",
		"The file cannot be accessed by the system."
	},
#endif // #ifdef ERROR_CANT_ACCESS_FILE
#ifdef ERROR_CANT_RESOLVE_FILENAME
	{	ERROR_CANT_RESOLVE_FILENAME,                              "ERROR_CANT_RESOLVE_FILENAME",
		"The name of the file cannot be resolved by the system.",
		"The name of the file cannot be resolved by the system."
	},
#endif // #ifdef ERROR_CANT_RESOLVE_FILENAME
#ifdef RPC_S_ENTRY_TYPE_MISMATCH
	{	RPC_S_ENTRY_TYPE_MISMATCH,                                "RPC_S_ENTRY_TYPE_MISMATCH",
		"The entry is not of the expected type.",
		"The entry is not of the expected type."
	},
#endif // #ifdef RPC_S_ENTRY_TYPE_MISMATCH
#ifdef RPC_S_NOT_ALL_OBJS_EXPORTED
	{	RPC_S_NOT_ALL_OBJS_EXPORTED,                              "RPC_S_NOT_ALL_OBJS_EXPORTED",
		"Not all object UUIDs could be exported to the specified entry.",
		"Not all object UUIDs could be exported to the specified entry."
	},
#endif // #ifdef RPC_S_NOT_ALL_OBJS_EXPORTED
#ifdef RPC_S_INTERFACE_NOT_EXPORTED
	{	RPC_S_INTERFACE_NOT_EXPORTED,                             "RPC_S_INTERFACE_NOT_EXPORTED",
		"Interface could not be exported to the specified entry.",
		"Interface could not be exported to the specified entry."
	},
#endif // #ifdef RPC_S_INTERFACE_NOT_EXPORTED
#ifdef RPC_S_PROFILE_NOT_ADDED
	{	RPC_S_PROFILE_NOT_ADDED,                                  "RPC_S_PROFILE_NOT_ADDED",
		"The specified profile entry could not be added.",
		"The specified profile entry could not be added."
	},
#endif // #ifdef RPC_S_PROFILE_NOT_ADDED
#ifdef RPC_S_PRF_ELT_NOT_ADDED
	{	RPC_S_PRF_ELT_NOT_ADDED,                                  "RPC_S_PRF_ELT_NOT_ADDED",
		"The specified profile element could not be added.",
		"The specified profile element could not be added."
	},
#endif // #ifdef RPC_S_PRF_ELT_NOT_ADDED
#ifdef RPC_S_PRF_ELT_NOT_REMOVED
	{	RPC_S_PRF_ELT_NOT_REMOVED,                                "RPC_S_PRF_ELT_NOT_REMOVED",
		"The specified profile element could not be removed.",
		"The specified profile element could not be removed."
	},
#endif // #ifdef RPC_S_PRF_ELT_NOT_REMOVED
#ifdef RPC_S_GRP_ELT_NOT_ADDED
	{	RPC_S_GRP_ELT_NOT_ADDED,                                  "RPC_S_GRP_ELT_NOT_ADDED",
		"The group element could not be added.",
		"The group element could not be added."
	},
#endif // #ifdef RPC_S_GRP_ELT_NOT_ADDED
#ifdef RPC_S_GRP_ELT_NOT_REMOVED
	{	RPC_S_GRP_ELT_NOT_REMOVED,                                "RPC_S_GRP_ELT_NOT_REMOVED",
		"The group element could not be removed.",
		"The group element could not be removed."
	},
#endif // #ifdef RPC_S_GRP_ELT_NOT_REMOVED
#ifdef ERROR_KM_DRIVER_BLOCKED
	{	ERROR_KM_DRIVER_BLOCKED,                                  "ERROR_KM_DRIVER_BLOCKED",
		"The printer driver is not compatible with a policy enabled on your computer that blocks NT 4.0 drivers.",
		"The printer driver is not compatible with a policy enabled on your computer that blocks NT 4.0 drivers."
	},
#endif // #ifdef ERROR_KM_DRIVER_BLOCKED
#ifdef ERROR_CONTEXT_EXPIRED
	{	ERROR_CONTEXT_EXPIRED,                                    "ERROR_CONTEXT_EXPIRED",
		"The context has expired and can no longer be used.",
		"The context has expired and can no longer be used."
	},
#endif // #ifdef ERROR_CONTEXT_EXPIRED
#ifdef ERROR_PER_USER_TRUST_QUOTA_EXCEEDED
	{	ERROR_PER_USER_TRUST_QUOTA_EXCEEDED,                      "ERROR_PER_USER_TRUST_QUOTA_EXCEEDED",
		"The current user's delegated trust creation quota has been exceeded.",
		"The current user's delegated trust creation quota has been exceeded."
	},
#endif // #ifdef ERROR_PER_USER_TRUST_QUOTA_EXCEEDED
#ifdef ERROR_ALL_USER_TRUST_QUOTA_EXCEEDED
	{	ERROR_ALL_USER_TRUST_QUOTA_EXCEEDED,                      "ERROR_ALL_USER_TRUST_QUOTA_EXCEEDED",
		"The total delegated trust creation quota has been exceeded.",
		"The total delegated trust creation quota has been exceeded."
	},
#endif // #ifdef ERROR_ALL_USER_TRUST_QUOTA_EXCEEDED
#ifdef ERROR_USER_DELETE_TRUST_QUOTA_EXCEEDED
	{	ERROR_USER_DELETE_TRUST_QUOTA_EXCEEDED,                   "ERROR_USER_DELETE_TRUST_QUOTA_EXCEEDED",
		"The current user's delegated trust deletion quota has been exceeded.",
		"The current user's delegated trust deletion quota has been exceeded."
	},
#endif // #ifdef ERROR_USER_DELETE_TRUST_QUOTA_EXCEEDED
#ifdef ERROR_INVALID_PIXEL_FORMAT
	{	ERROR_INVALID_PIXEL_FORMAT,                               "ERROR_INVALID_PIXEL_FORMAT",
		"The pixel format is invalid.",
		"The pixel format is invalid."
	},
#endif // #ifdef ERROR_INVALID_PIXEL_FORMAT
#ifdef ERROR_BAD_DRIVER
	{	ERROR_BAD_DRIVER,                                         "ERROR_BAD_DRIVER",
		"The specified driver is invalid.",
		"The specified driver is invalid."
	},
#endif // #ifdef ERROR_BAD_DRIVER
#ifdef ERROR_INVALID_WINDOW_STYLE
	{	ERROR_INVALID_WINDOW_STYLE,                               "ERROR_INVALID_WINDOW_STYLE",
		"The window style or class attribute is invalid for this operation.",
		"The window style or class attribute is invalid for this operation."
	},
#endif // #ifdef ERROR_INVALID_WINDOW_STYLE
#ifdef ERROR_METAFILE_NOT_SUPPORTED
	{	ERROR_METAFILE_NOT_SUPPORTED,                             "ERROR_METAFILE_NOT_SUPPORTED",
		"The requested metafile operation is not supported.",
		"The requested metafile operation is not supported."
	},
#endif // #ifdef ERROR_METAFILE_NOT_SUPPORTED
#ifdef ERROR_TRANSFORM_NOT_SUPPORTED
	{	ERROR_TRANSFORM_NOT_SUPPORTED,                            "ERROR_TRANSFORM_NOT_SUPPORTED",
		"The requested transformation operation is not supported.",
		"The requested transformation operation is not supported."
	},
#endif // #ifdef ERROR_TRANSFORM_NOT_SUPPORTED
#ifdef ERROR_CLIPPING_NOT_SUPPORTED
	{	ERROR_CLIPPING_NOT_SUPPORTED,                             "ERROR_CLIPPING_NOT_SUPPORTED",
		"The requested clipping operation is not supported.",
		"The requested clipping operation is not supported."
	},
#endif // #ifdef ERROR_CLIPPING_NOT_SUPPORTED
#ifdef ERROR_INVALID_CMM
	{	ERROR_INVALID_CMM,                                        "ERROR_INVALID_CMM",
		"The specified color management module is invalid.",
		"The specified color management module is invalid."
	},
#endif // #ifdef ERROR_INVALID_CMM
#ifdef ERROR_INVALID_PROFILE
	{	ERROR_INVALID_PROFILE,                                    "ERROR_INVALID_PROFILE",
		"The specified color profile is invalid.",
		"The specified color profile is invalid."
	},
#endif // #ifdef ERROR_INVALID_PROFILE
#ifdef ERROR_TAG_NOT_FOUND
	{	ERROR_TAG_NOT_FOUND,                                      "ERROR_TAG_NOT_FOUND",
		"The specified tag was not found.",
		"The specified tag was not found."
	},
#endif // #ifdef ERROR_TAG_NOT_FOUND
#ifdef ERROR_TAG_NOT_PRESENT
	{	ERROR_TAG_NOT_PRESENT,                                    "ERROR_TAG_NOT_PRESENT",
		"A required tag is not present.",
		"A required tag is not present."
	},
#endif // #ifdef ERROR_TAG_NOT_PRESENT
#ifdef ERROR_DUPLICATE_TAG
	{	ERROR_DUPLICATE_TAG,                                      "ERROR_DUPLICATE_TAG",
		"The specified tag is already present.",
		"The specified tag is already present."
	},
#endif // #ifdef ERROR_DUPLICATE_TAG
#ifdef ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE
	{	ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE,                 "ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE",
		"The specified color profile is not associated with any device.",
		"The specified color profile is not associated with any device."
	},
#endif // #ifdef ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE
#ifdef ERROR_PROFILE_NOT_FOUND
	{	ERROR_PROFILE_NOT_FOUND,                                  "ERROR_PROFILE_NOT_FOUND",
		"The specified color profile was not found.",
		"The specified color profile was not found."
	},
#endif // #ifdef ERROR_PROFILE_NOT_FOUND
#ifdef ERROR_INVALID_COLORSPACE
	{	ERROR_INVALID_COLORSPACE,                                 "ERROR_INVALID_COLORSPACE",
		"The specified color space is invalid.",
		"The specified color space is invalid."
	},
#endif // #ifdef ERROR_INVALID_COLORSPACE
#ifdef ERROR_ICM_NOT_ENABLED
	{	ERROR_ICM_NOT_ENABLED,                                    "ERROR_ICM_NOT_ENABLED",
		"Image Color Management is not enabled.",
		"Image Color Management is not enabled."
	},
#endif // #ifdef ERROR_ICM_NOT_ENABLED
#ifdef ERROR_DELETING_ICM_XFORM
	{	ERROR_DELETING_ICM_XFORM,                                 "ERROR_DELETING_ICM_XFORM",
		"There was an error while deleting the color transform.",
		"There was an error while deleting the color transform."
	},
#endif // #ifdef ERROR_DELETING_ICM_XFORM
#ifdef ERROR_INVALID_TRANSFORM
	{	ERROR_INVALID_TRANSFORM,                                  "ERROR_INVALID_TRANSFORM",
		"The specified color transform is invalid.",
		"The specified color transform is invalid."
	},
#endif // #ifdef ERROR_INVALID_TRANSFORM
#ifdef ERROR_COLORSPACE_MISMATCH
	{	ERROR_COLORSPACE_MISMATCH,                                "ERROR_COLORSPACE_MISMATCH",
		"The specified transform does not match the bitmap's color space.",
		"The specified transform does not match the bitmap's color space."
	},
#endif // #ifdef ERROR_COLORSPACE_MISMATCH
#ifdef ERROR_INVALID_COLORINDEX
	{	ERROR_INVALID_COLORINDEX,                                 "ERROR_INVALID_COLORINDEX",
		"The specified named color index is not present in the profile.",
		"The specified named color index is not present in the profile."
	},
#endif // #ifdef ERROR_INVALID_COLORINDEX
#ifdef ERROR_CONNECTED_OTHER_PASSWORD
	{	ERROR_CONNECTED_OTHER_PASSWORD,                           "ERROR_CONNECTED_OTHER_PASSWORD",
		"The network connection was made successfully, but the user had to be prompted for a password other than the one originally specified.",
		"The network connection was made successfully, but the user had to be prompted for a password other than the one originally specified."
	},
#endif // #ifdef ERROR_CONNECTED_OTHER_PASSWORD
#ifdef ERROR_CONNECTED_OTHER_PASSWORD_DEFAULT
	{	ERROR_CONNECTED_OTHER_PASSWORD_DEFAULT,                   "ERROR_CONNECTED_OTHER_PASSWORD_DEFAULT",
		"The network connection was made successfully using default credentials.",
		"The network connection was made successfully using default credentials."
	},
#endif // #ifdef ERROR_CONNECTED_OTHER_PASSWORD_DEFAULT
#ifdef ERROR_BAD_USERNAME
	{	ERROR_BAD_USERNAME,                                       "ERROR_BAD_USERNAME",
		"The specified username is invalid.",
		"The specified username is invalid."
	},
#endif // #ifdef ERROR_BAD_USERNAME
#ifdef ERROR_NOT_CONNECTED
	{	ERROR_NOT_CONNECTED,                                      "ERROR_NOT_CONNECTED",
		"This network connection does not exist.",
		"This network connection does not exist."
	},
#endif // #ifdef ERROR_NOT_CONNECTED
#ifdef ERROR_OPEN_FILES
	{	ERROR_OPEN_FILES,                                         "ERROR_OPEN_FILES",
		"This network connection has files open or requests pending.",
		"This network connection has files open or requests pending."
	},
#endif // #ifdef ERROR_OPEN_FILES
#ifdef ERROR_ACTIVE_CONNECTIONS
	{	ERROR_ACTIVE_CONNECTIONS,                                 "ERROR_ACTIVE_CONNECTIONS",
		"Active connections still exist.",
		"Active connections still exist."
	},
#endif // #ifdef ERROR_ACTIVE_CONNECTIONS
#ifdef ERROR_DEVICE_IN_USE
	{	ERROR_DEVICE_IN_USE,                                      "ERROR_DEVICE_IN_USE",
		"The device is in use by an active process and cannot be disconnected.",
		"The device is in use by an active process and cannot be disconnected."
	},
#endif // #ifdef ERROR_DEVICE_IN_USE
#ifdef ERROR_UNKNOWN_PRINT_MONITOR
	{	ERROR_UNKNOWN_PRINT_MONITOR,                              "ERROR_UNKNOWN_PRINT_MONITOR",
		"The specified print monitor is unknown.",
		"The specified print monitor is unknown."
	},
#endif // #ifdef ERROR_UNKNOWN_PRINT_MONITOR
#ifdef ERROR_PRINTER_DRIVER_IN_USE
	{	ERROR_PRINTER_DRIVER_IN_USE,                              "ERROR_PRINTER_DRIVER_IN_USE",
		"The specified printer driver is currently in use.",
		"The specified printer driver is currently in use."
	},
#endif // #ifdef ERROR_PRINTER_DRIVER_IN_USE
#ifdef ERROR_SPOOL_FILE_NOT_FOUND
	{	ERROR_SPOOL_FILE_NOT_FOUND,                               "ERROR_SPOOL_FILE_NOT_FOUND",
		"The spool file was not found.",
		"The spool file was not found."
	},
#endif // #ifdef ERROR_SPOOL_FILE_NOT_FOUND
#ifdef ERROR_SPL_NO_STARTDOC
	{	ERROR_SPL_NO_STARTDOC,                                    "ERROR_SPL_NO_STARTDOC",
		"A StartDocPrinter call was not issued.",
		"A StartDocPrinter call was not issued."
	},
#endif // #ifdef ERROR_SPL_NO_STARTDOC
#ifdef ERROR_SPL_NO_ADDJOB
	{	ERROR_SPL_NO_ADDJOB,                                      "ERROR_SPL_NO_ADDJOB",
		"An AddJob call was not issued.",
		"An AddJob call was not issued."
	},
#endif // #ifdef ERROR_SPL_NO_ADDJOB
#ifdef ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED
	{	ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED,                  "ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED",
		"The specified print processor has already been installed.",
		"The specified print processor has already been installed."
	},
#endif // #ifdef ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED
#ifdef ERROR_PRINT_MONITOR_ALREADY_INSTALLED
	{	ERROR_PRINT_MONITOR_ALREADY_INSTALLED,                    "ERROR_PRINT_MONITOR_ALREADY_INSTALLED",
		"The specified print monitor has already been installed.",
		"The specified print monitor has already been installed."
	},
#endif // #ifdef ERROR_PRINT_MONITOR_ALREADY_INSTALLED
#ifdef ERROR_INVALID_PRINT_MONITOR
	{	ERROR_INVALID_PRINT_MONITOR,                              "ERROR_INVALID_PRINT_MONITOR",
		"The specified print monitor does not have the required functions.",
		"The specified print monitor does not have the required functions."
	},
#endif // #ifdef ERROR_INVALID_PRINT_MONITOR
#ifdef ERROR_PRINT_MONITOR_IN_USE
	{	ERROR_PRINT_MONITOR_IN_USE,                               "ERROR_PRINT_MONITOR_IN_USE",
		"The specified print monitor is currently in use.",
		"The specified print monitor is currently in use."
	},
#endif // #ifdef ERROR_PRINT_MONITOR_IN_USE
#ifdef ERROR_PRINTER_HAS_JOBS_QUEUED
	{	ERROR_PRINTER_HAS_JOBS_QUEUED,                            "ERROR_PRINTER_HAS_JOBS_QUEUED",
		"The requested operation is not allowed when there are jobs queued to the printer.",
		"The requested operation is not allowed when there are jobs queued to the printer."
	},
#endif // #ifdef ERROR_PRINTER_HAS_JOBS_QUEUED
#ifdef ERROR_SUCCESS_REBOOT_REQUIRED
	{	ERROR_SUCCESS_REBOOT_REQUIRED,                            "ERROR_SUCCESS_REBOOT_REQUIRED",
		"The requested operation is successful. Changes will not be effective until the system is rebooted.",
		"The requested operation is successful. Changes will not be effective until the system is rebooted."
	},
#endif // #ifdef ERROR_SUCCESS_REBOOT_REQUIRED
#ifdef ERROR_SUCCESS_RESTART_REQUIRED
	{	ERROR_SUCCESS_RESTART_REQUIRED,                           "ERROR_SUCCESS_RESTART_REQUIRED",
		"The requested operation is successful. Changes will not be effective until the service is restarted.",
		"The requested operation is successful. Changes will not be effective until the service is restarted."
	},
#endif // #ifdef ERROR_SUCCESS_RESTART_REQUIRED
#ifdef ERROR_PRINTER_NOT_FOUND
	{	ERROR_PRINTER_NOT_FOUND,                                  "ERROR_PRINTER_NOT_FOUND",
		"No printers were found.",
		"No printers were found."
	},
#endif // #ifdef ERROR_PRINTER_NOT_FOUND
#ifdef ERROR_PRINTER_DRIVER_WARNED
	{	ERROR_PRINTER_DRIVER_WARNED,                              "ERROR_PRINTER_DRIVER_WARNED",
		"The printer driver is known to be unreliable.",
		"The printer driver is known to be unreliable."
	},
#endif // #ifdef ERROR_PRINTER_DRIVER_WARNED
#ifdef ERROR_PRINTER_DRIVER_BLOCKED
	{	ERROR_PRINTER_DRIVER_BLOCKED,                             "ERROR_PRINTER_DRIVER_BLOCKED",
		"The printer driver is known to harm the system.",
		"The printer driver is known to harm the system."
	},
#endif // #ifdef ERROR_PRINTER_DRIVER_BLOCKED
#ifdef ERROR_WINS_INTERNAL
	{	ERROR_WINS_INTERNAL,                                      "ERROR_WINS_INTERNAL",
		"WINS encountered an error while processing the command.",
		"WINS encountered an error while processing the command."
	},
#endif // #ifdef ERROR_WINS_INTERNAL
#ifdef ERROR_CAN_NOT_DEL_LOCAL_WINS
	{	ERROR_CAN_NOT_DEL_LOCAL_WINS,                             "ERROR_CAN_NOT_DEL_LOCAL_WINS",
		"The local WINS cannot be deleted.",
		"The local WINS cannot be deleted."
	},
#endif // #ifdef ERROR_CAN_NOT_DEL_LOCAL_WINS
#ifdef ERROR_STATIC_INIT
	{	ERROR_STATIC_INIT,                                        "ERROR_STATIC_INIT",
		"The importation from the file failed.",
		"The importation from the file failed."
	},
#endif // #ifdef ERROR_STATIC_INIT
#ifdef ERROR_INC_BACKUP
	{	ERROR_INC_BACKUP,                                         "ERROR_INC_BACKUP",
		"The backup failed. Was a full backup done before?",
		"The backup failed. Was a full backup done before?"
	},
#endif // #ifdef ERROR_INC_BACKUP
#ifdef ERROR_FULL_BACKUP
	{	ERROR_FULL_BACKUP,                                        "ERROR_FULL_BACKUP",
		"The backup failed. Check the directory to which you are backing the database.",
		"The backup failed. Check the directory to which you are backing the database."
	},
#endif // #ifdef ERROR_FULL_BACKUP
#ifdef ERROR_REC_NON_EXISTENT
	{	ERROR_REC_NON_EXISTENT,                                   "ERROR_REC_NON_EXISTENT",
		"The name does not exist in the WINS database.",
		"The name does not exist in the WINS database."
	},
#endif // #ifdef ERROR_REC_NON_EXISTENT
#ifdef ERROR_RPL_NOT_ALLOWED
	{	ERROR_RPL_NOT_ALLOWED,                                    "ERROR_RPL_NOT_ALLOWED",
		"Replication with a nonconfigured partner is not allowed.",
		"Replication with a nonconfigured partner is not allowed."
	},
#endif // #ifdef ERROR_RPL_NOT_ALLOWED
#ifdef ERROR_DHCP_ADDRESS_CONFLICT
	{	ERROR_DHCP_ADDRESS_CONFLICT,                              "ERROR_DHCP_ADDRESS_CONFLICT",
		"The DHCP client has obtained an IP address that is already in use on the network. The local interface will be disabled until the DHCP client can obtain a new address.",
		"The DHCP client has obtained an IP address that is already in use on the network. The local interface will be disabled until the DHCP client can obtain a new address."
	},
#endif // #ifdef ERROR_DHCP_ADDRESS_CONFLICT
#ifdef ERROR_WMI_GUID_NOT_FOUND
	{	ERROR_WMI_GUID_NOT_FOUND,                                 "ERROR_WMI_GUID_NOT_FOUND",
		"The GUID passed was not recognized as valid by a WMI data provider.",
		"The GUID passed was not recognized as valid by a WMI data provider."
	},
#endif // #ifdef ERROR_WMI_GUID_NOT_FOUND
#ifdef ERROR_WMI_INSTANCE_NOT_FOUND
	{	ERROR_WMI_INSTANCE_NOT_FOUND,                             "ERROR_WMI_INSTANCE_NOT_FOUND",
		"The instance name passed was not recognized as valid by a WMI data provider.",
		"The instance name passed was not recognized as valid by a WMI data provider."
	},
#endif // #ifdef ERROR_WMI_INSTANCE_NOT_FOUND
#ifdef ERROR_WMI_ITEMID_NOT_FOUND
	{	ERROR_WMI_ITEMID_NOT_FOUND,                               "ERROR_WMI_ITEMID_NOT_FOUND",
		"The data item ID passed was not recognized as valid by a WMI data provider.",
		"The data item ID passed was not recognized as valid by a WMI data provider."
	},
#endif // #ifdef ERROR_WMI_ITEMID_NOT_FOUND
#ifdef ERROR_WMI_TRY_AGAIN
	{	ERROR_WMI_TRY_AGAIN,                                      "ERROR_WMI_TRY_AGAIN",
		"The WMI request could not be completed and should be retried.",
		"The WMI request could not be completed and should be retried."
	},
#endif // #ifdef ERROR_WMI_TRY_AGAIN
#ifdef ERROR_WMI_DP_NOT_FOUND
	{	ERROR_WMI_DP_NOT_FOUND,                                   "ERROR_WMI_DP_NOT_FOUND",
		"The WMI data provider could not be located.",
		"The WMI data provider could not be located."
	},
#endif // #ifdef ERROR_WMI_DP_NOT_FOUND
#ifdef ERROR_WMI_UNRESOLVED_INSTANCE_REF
	{	ERROR_WMI_UNRESOLVED_INSTANCE_REF,                        "ERROR_WMI_UNRESOLVED_INSTANCE_REF",
		"The WMI data provider references an instance set that has not been registered.",
		"The WMI data provider references an instance set that has not been registered."
	},
#endif // #ifdef ERROR_WMI_UNRESOLVED_INSTANCE_REF
#ifdef ERROR_WMI_ALREADY_ENABLED
	{	ERROR_WMI_ALREADY_ENABLED,                                "ERROR_WMI_ALREADY_ENABLED",
		"The WMI data block or event notification has already been enabled.",
		"The WMI data block or event notification has already been enabled."
	},
#endif // #ifdef ERROR_WMI_ALREADY_ENABLED
#ifdef ERROR_WMI_GUID_DISCONNECTED
	{	ERROR_WMI_GUID_DISCONNECTED,                              "ERROR_WMI_GUID_DISCONNECTED",
		"The WMI data block is no longer available.",
		"The WMI data block is no longer available."
	},
#endif // #ifdef ERROR_WMI_GUID_DISCONNECTED
#ifdef ERROR_WMI_SERVER_UNAVAILABLE
	{	ERROR_WMI_SERVER_UNAVAILABLE,                             "ERROR_WMI_SERVER_UNAVAILABLE",
		"The WMI data service is not available.",
		"The WMI data service is not available."
	},
#endif // #ifdef ERROR_WMI_SERVER_UNAVAILABLE
#ifdef ERROR_WMI_DP_FAILED
	{	ERROR_WMI_DP_FAILED,                                      "ERROR_WMI_DP_FAILED",
		"The WMI data provider failed to carry out the request.",
		"The WMI data provider failed to carry out the request."
	},
#endif // #ifdef ERROR_WMI_DP_FAILED
#ifdef ERROR_WMI_INVALID_MOF
	{	ERROR_WMI_INVALID_MOF,                                    "ERROR_WMI_INVALID_MOF",
		"The WMI MOF information is not valid.",
		"The WMI MOF information is not valid."
	},
#endif // #ifdef ERROR_WMI_INVALID_MOF
#ifdef ERROR_WMI_INVALID_REGINFO
	{	ERROR_WMI_INVALID_REGINFO,                                "ERROR_WMI_INVALID_REGINFO",
		"The WMI registration information is not valid.",
		"The WMI registration information is not valid."
	},
#endif // #ifdef ERROR_WMI_INVALID_REGINFO
#ifdef ERROR_WMI_ALREADY_DISABLED
	{	ERROR_WMI_ALREADY_DISABLED,                               "ERROR_WMI_ALREADY_DISABLED",
		"The WMI data block or event notification has already been disabled.",
		"The WMI data block or event notification has already been disabled."
	},
#endif // #ifdef ERROR_WMI_ALREADY_DISABLED
#ifdef ERROR_WMI_READ_ONLY
	{	ERROR_WMI_READ_ONLY,                                      "ERROR_WMI_READ_ONLY",
		"The WMI data item or data block is read only.",
		"The WMI data item or data block is read only."
	},
#endif // #ifdef ERROR_WMI_READ_ONLY
#ifdef ERROR_WMI_SET_FAILURE
	{	ERROR_WMI_SET_FAILURE,                                    "ERROR_WMI_SET_FAILURE",
		"The WMI data item or data block could not be changed.",
		"The WMI data item or data block could not be changed."
	},
#endif // #ifdef ERROR_WMI_SET_FAILURE
#ifdef ERROR_INVALID_MEDIA
	{	ERROR_INVALID_MEDIA,                                      "ERROR_INVALID_MEDIA",
		"The media identifier does not represent a valid medium.",
		"The media identifier does not represent a valid medium."
	},
#endif // #ifdef ERROR_INVALID_MEDIA
#ifdef ERROR_INVALID_LIBRARY
	{	ERROR_INVALID_LIBRARY,                                    "ERROR_INVALID_LIBRARY",
		"The library identifier does not represent a valid library.",
		"The library identifier does not represent a valid library."
	},
#endif // #ifdef ERROR_INVALID_LIBRARY
#ifdef ERROR_INVALID_MEDIA_POOL
	{	ERROR_INVALID_MEDIA_POOL,                                 "ERROR_INVALID_MEDIA_POOL",
		"The media pool identifier does not represent a valid media pool.",
		"The media pool identifier does not represent a valid media pool."
	},
#endif // #ifdef ERROR_INVALID_MEDIA_POOL
#ifdef ERROR_DRIVE_MEDIA_MISMATCH
	{	ERROR_DRIVE_MEDIA_MISMATCH,                               "ERROR_DRIVE_MEDIA_MISMATCH",
		"The drive and medium are not compatible or exist in different libraries.",
		"The drive and medium are not compatible or exist in different libraries."
	},
#endif // #ifdef ERROR_DRIVE_MEDIA_MISMATCH
#ifdef ERROR_MEDIA_OFFLINE
	{	ERROR_MEDIA_OFFLINE,                                      "ERROR_MEDIA_OFFLINE",
		"The medium currently exists in an offline library and must be online to perform this operation.",
		"The medium currently exists in an offline library and must be online to perform this operation."
	},
#endif // #ifdef ERROR_MEDIA_OFFLINE
#ifdef ERROR_LIBRARY_OFFLINE
	{	ERROR_LIBRARY_OFFLINE,                                    "ERROR_LIBRARY_OFFLINE",
		"The operation cannot be performed on an offline library.",
		"The operation cannot be performed on an offline library."
	},
#endif // #ifdef ERROR_LIBRARY_OFFLINE
#ifdef ERROR_EMPTY
	{	ERROR_EMPTY,                                              "ERROR_EMPTY",
		"The library, drive, or media pool is empty.",
		"The library, drive, or media pool is empty."
	},
#endif // #ifdef ERROR_EMPTY
#ifdef ERROR_NOT_EMPTY
	{	ERROR_NOT_EMPTY,                                          "ERROR_NOT_EMPTY",
		"The library, drive, or media pool must be empty to perform this operation.",
		"The library, drive, or media pool must be empty to perform this operation."
	},
#endif // #ifdef ERROR_NOT_EMPTY
#ifdef ERROR_MEDIA_UNAVAILABLE
	{	ERROR_MEDIA_UNAVAILABLE,                                  "ERROR_MEDIA_UNAVAILABLE",
		"No media is currently available in this media pool or library.",
		"No media is currently available in this media pool or library."
	},
#endif // #ifdef ERROR_MEDIA_UNAVAILABLE
#ifdef ERROR_RESOURCE_DISABLED
	{	ERROR_RESOURCE_DISABLED,                                  "ERROR_RESOURCE_DISABLED",
		"A resource required for this operation is disabled.",
		"A resource required for this operation is disabled."
	},
#endif // #ifdef ERROR_RESOURCE_DISABLED
#ifdef ERROR_INVALID_CLEANER
	{	ERROR_INVALID_CLEANER,                                    "ERROR_INVALID_CLEANER",
		"The media identifier does not represent a valid cleaner.",
		"The media identifier does not represent a valid cleaner."
	},
#endif // #ifdef ERROR_INVALID_CLEANER
#ifdef ERROR_UNABLE_TO_CLEAN
	{	ERROR_UNABLE_TO_CLEAN,                                    "ERROR_UNABLE_TO_CLEAN",
		"The drive cannot be cleaned or does not support cleaning.",
		"The drive cannot be cleaned or does not support cleaning."
	},
#endif // #ifdef ERROR_UNABLE_TO_CLEAN
#ifdef ERROR_OBJECT_NOT_FOUND
	{	ERROR_OBJECT_NOT_FOUND,                                   "ERROR_OBJECT_NOT_FOUND",
		"The object identifier does not represent a valid object.",
		"The object identifier does not represent a valid object."
	},
#endif // #ifdef ERROR_OBJECT_NOT_FOUND
#ifdef ERROR_DATABASE_FAILURE
	{	ERROR_DATABASE_FAILURE,                                   "ERROR_DATABASE_FAILURE",
		"Unable to read from or write to the database.",
		"Unable to read from or write to the database."
	},
#endif // #ifdef ERROR_DATABASE_FAILURE
#ifdef ERROR_DATABASE_FULL
	{	ERROR_DATABASE_FULL,                                      "ERROR_DATABASE_FULL",
		"The database is full.",
		"The database is full."
	},
#endif // #ifdef ERROR_DATABASE_FULL
#ifdef ERROR_MEDIA_INCOMPATIBLE
	{	ERROR_MEDIA_INCOMPATIBLE,                                 "ERROR_MEDIA_INCOMPATIBLE",
		"The medium is not compatible with the device or media pool.",
		"The medium is not compatible with the device or media pool."
	},
#endif // #ifdef ERROR_MEDIA_INCOMPATIBLE
#ifdef ERROR_RESOURCE_NOT_PRESENT
	{	ERROR_RESOURCE_NOT_PRESENT,                               "ERROR_RESOURCE_NOT_PRESENT",
		"The resource required for this operation does not exist.",
		"The resource required for this operation does not exist."
	},
#endif // #ifdef ERROR_RESOURCE_NOT_PRESENT
#ifdef ERROR_INVALID_OPERATION
	{	ERROR_INVALID_OPERATION,                                  "ERROR_INVALID_OPERATION",
		"The operation identifier is not valid.",
		"The operation identifier is not valid."
	},
#endif // #ifdef ERROR_INVALID_OPERATION
#ifdef ERROR_MEDIA_NOT_AVAILABLE
	{	ERROR_MEDIA_NOT_AVAILABLE,                                "ERROR_MEDIA_NOT_AVAILABLE",
		"The media is not mounted or ready for use.",
		"The media is not mounted or ready for use."
	},
#endif // #ifdef ERROR_MEDIA_NOT_AVAILABLE
#ifdef ERROR_DEVICE_NOT_AVAILABLE
	{	ERROR_DEVICE_NOT_AVAILABLE,                               "ERROR_DEVICE_NOT_AVAILABLE",
		"The device is not ready for use.",
		"The device is not ready for use."
	},
#endif // #ifdef ERROR_DEVICE_NOT_AVAILABLE
#ifdef ERROR_REQUEST_REFUSED
	{	ERROR_REQUEST_REFUSED,                                    "ERROR_REQUEST_REFUSED",
		"The operator or administrator has refused the request.",
		"The operator or administrator has refused the request."
	},
#endif // #ifdef ERROR_REQUEST_REFUSED
#ifdef ERROR_INVALID_DRIVE_OBJECT
	{	ERROR_INVALID_DRIVE_OBJECT,                               "ERROR_INVALID_DRIVE_OBJECT",
		"The drive identifier does not represent a valid drive.",
		"The drive identifier does not represent a valid drive."
	},
#endif // #ifdef ERROR_INVALID_DRIVE_OBJECT
#ifdef ERROR_LIBRARY_FULL
	{	ERROR_LIBRARY_FULL,                                       "ERROR_LIBRARY_FULL",
		"Library is full. No slot is available for use.",
		"Library is full. No slot is available for use."
	},
#endif // #ifdef ERROR_LIBRARY_FULL
#ifdef ERROR_MEDIUM_NOT_ACCESSIBLE
	{	ERROR_MEDIUM_NOT_ACCESSIBLE,                              "ERROR_MEDIUM_NOT_ACCESSIBLE",
		"The transport cannot access the medium.",
		"The transport cannot access the medium."
	},
#endif // #ifdef ERROR_MEDIUM_NOT_ACCESSIBLE
#ifdef ERROR_UNABLE_TO_LOAD_MEDIUM
	{	ERROR_UNABLE_TO_LOAD_MEDIUM,                              "ERROR_UNABLE_TO_LOAD_MEDIUM",
		"Unable to load the medium into the drive.",
		"Unable to load the medium into the drive."
	},
#endif // #ifdef ERROR_UNABLE_TO_LOAD_MEDIUM
#ifdef ERROR_UNABLE_TO_INVENTORY_DRIVE
	{	ERROR_UNABLE_TO_INVENTORY_DRIVE,                          "ERROR_UNABLE_TO_INVENTORY_DRIVE",
		"Unable to retrieve status about the drive.",
		"Unable to retrieve status about the drive."
	},
#endif // #ifdef ERROR_UNABLE_TO_INVENTORY_DRIVE
#ifdef ERROR_UNABLE_TO_INVENTORY_SLOT
	{	ERROR_UNABLE_TO_INVENTORY_SLOT,                           "ERROR_UNABLE_TO_INVENTORY_SLOT",
		"Unable to retrieve status about the slot.",
		"Unable to retrieve status about the slot."
	},
#endif // #ifdef ERROR_UNABLE_TO_INVENTORY_SLOT
#ifdef ERROR_UNABLE_TO_INVENTORY_TRANSPORT
	{	ERROR_UNABLE_TO_INVENTORY_TRANSPORT,                      "ERROR_UNABLE_TO_INVENTORY_TRANSPORT",
		"Unable to retrieve status about the transport.",
		"Unable to retrieve status about the transport."
	},
#endif // #ifdef ERROR_UNABLE_TO_INVENTORY_TRANSPORT
#ifdef ERROR_TRANSPORT_FULL
	{	ERROR_TRANSPORT_FULL,                                     "ERROR_TRANSPORT_FULL",
		"Cannot use the transport because it is already in use.",
		"Cannot use the transport because it is already in use."
	},
#endif // #ifdef ERROR_TRANSPORT_FULL
#ifdef ERROR_CONTROLLING_IEPORT
	{	ERROR_CONTROLLING_IEPORT,                                 "ERROR_CONTROLLING_IEPORT",
		"Unable to open or close the inject/eject port.",
		"Unable to open or close the inject/eject port."
	},
#endif // #ifdef ERROR_CONTROLLING_IEPORT
#ifdef ERROR_UNABLE_TO_EJECT_MOUNTED_MEDIA
	{	ERROR_UNABLE_TO_EJECT_MOUNTED_MEDIA,                      "ERROR_UNABLE_TO_EJECT_MOUNTED_MEDIA",
		"Unable to eject the media because it is in a drive.",
		"Unable to eject the media because it is in a drive."
	},
#endif // #ifdef ERROR_UNABLE_TO_EJECT_MOUNTED_MEDIA
#ifdef ERROR_CLEANER_SLOT_SET
	{	ERROR_CLEANER_SLOT_SET,                                   "ERROR_CLEANER_SLOT_SET",
		"A cleaner slot is already reserved.",
		"A cleaner slot is already reserved."
	},
#endif // #ifdef ERROR_CLEANER_SLOT_SET
#ifdef ERROR_CLEANER_SLOT_NOT_SET
	{	ERROR_CLEANER_SLOT_NOT_SET,                               "ERROR_CLEANER_SLOT_NOT_SET",
		"A cleaner slot is not reserved.",
		"A cleaner slot is not reserved."
	},
#endif // #ifdef ERROR_CLEANER_SLOT_NOT_SET
#ifdef ERROR_CLEANER_CARTRIDGE_SPENT
	{	ERROR_CLEANER_CARTRIDGE_SPENT,                            "ERROR_CLEANER_CARTRIDGE_SPENT",
		"The cleaner cartridge has performed the maximum number of drive cleanings.",
		"The cleaner cartridge has performed the maximum number of drive cleanings."
	},
#endif // #ifdef ERROR_CLEANER_CARTRIDGE_SPENT
#ifdef ERROR_UNEXPECTED_OMID
	{	ERROR_UNEXPECTED_OMID,                                    "ERROR_UNEXPECTED_OMID",
		"Unexpected on-medium identifier.",
		"Unexpected on-medium identifier."
	},
#endif // #ifdef ERROR_UNEXPECTED_OMID
#ifdef ERROR_CANT_DELETE_LAST_ITEM
	{	ERROR_CANT_DELETE_LAST_ITEM,                              "ERROR_CANT_DELETE_LAST_ITEM",
		"The last remaining item in this group or resource cannot be deleted.",
		"The last remaining item in this group or resource cannot be deleted."
	},
#endif // #ifdef ERROR_CANT_DELETE_LAST_ITEM
#ifdef ERROR_MESSAGE_EXCEEDS_MAX_SIZE
	{	ERROR_MESSAGE_EXCEEDS_MAX_SIZE,                           "ERROR_MESSAGE_EXCEEDS_MAX_SIZE",
		"The message provided exceeds the maximum size allowed for this parameter.",
		"The message provided exceeds the maximum size allowed for this parameter."
	},
#endif // #ifdef ERROR_MESSAGE_EXCEEDS_MAX_SIZE
#ifdef ERROR_VOLUME_CONTAINS_SYS_FILES
	{	ERROR_VOLUME_CONTAINS_SYS_FILES,                          "ERROR_VOLUME_CONTAINS_SYS_FILES",
		"The volume contains system or paging files.",
		"The volume contains system or paging files."
	},
#endif // #ifdef ERROR_VOLUME_CONTAINS_SYS_FILES
#ifdef ERROR_INDIGENOUS_TYPE
	{	ERROR_INDIGENOUS_TYPE,                                    "ERROR_INDIGENOUS_TYPE",
		"The media type cannot be removed from this library since at least one drive in the library reports it can support this media type.",
		"The media type cannot be removed from this library since at least one drive in the library reports it can support this media type."
	},
#endif // #ifdef ERROR_INDIGENOUS_TYPE
#ifdef ERROR_NO_SUPPORTING_DRIVES
	{	ERROR_NO_SUPPORTING_DRIVES,                               "ERROR_NO_SUPPORTING_DRIVES",
		"This offline media cannot be mounted on this system since no enabled drives are present which can be used.",
		"This offline media cannot be mounted on this system since no enabled drives are present which can be used."
	},
#endif // #ifdef ERROR_NO_SUPPORTING_DRIVES
#ifdef ERROR_CLEANER_CARTRIDGE_INSTALLED
	{	ERROR_CLEANER_CARTRIDGE_INSTALLED,                        "ERROR_CLEANER_CARTRIDGE_INSTALLED",
		"A cleaner cartridge is present in the tape library.",
		"A cleaner cartridge is present in the tape library."
	},
#endif // #ifdef ERROR_CLEANER_CARTRIDGE_INSTALLED
#ifdef ERROR_FILE_OFFLINE
	{	ERROR_FILE_OFFLINE,                                       "ERROR_FILE_OFFLINE",
		"The remote storage service was not able to recall the file.",
		"The remote storage service was not able to recall the file."
	},
#endif // #ifdef ERROR_FILE_OFFLINE
#ifdef ERROR_REMOTE_STORAGE_NOT_ACTIVE
	{	ERROR_REMOTE_STORAGE_NOT_ACTIVE,                          "ERROR_REMOTE_STORAGE_NOT_ACTIVE",
		"The remote storage service is not operational at this time.",
		"The remote storage service is not operational at this time."
	},
#endif // #ifdef ERROR_REMOTE_STORAGE_NOT_ACTIVE
#ifdef ERROR_REMOTE_STORAGE_MEDIA_ERROR
	{	ERROR_REMOTE_STORAGE_MEDIA_ERROR,                         "ERROR_REMOTE_STORAGE_MEDIA_ERROR",
		"The remote storage service encountered a media error.",
		"The remote storage service encountered a media error."
	},
#endif // #ifdef ERROR_REMOTE_STORAGE_MEDIA_ERROR
#ifdef ERROR_NOT_A_REPARSE_POINT
	{	ERROR_NOT_A_REPARSE_POINT,                                "ERROR_NOT_A_REPARSE_POINT",
		"The file or directory is not a reparse point.",
		"The file or directory is not a reparse point."
	},
#endif // #ifdef ERROR_NOT_A_REPARSE_POINT
#ifdef ERROR_REPARSE_ATTRIBUTE_CONFLICT
	{	ERROR_REPARSE_ATTRIBUTE_CONFLICT,                         "ERROR_REPARSE_ATTRIBUTE_CONFLICT",
		"The reparse point attribute cannot be set because it conflicts with an existing attribute.",
		"The reparse point attribute cannot be set because it conflicts with an existing attribute."
	},
#endif // #ifdef ERROR_REPARSE_ATTRIBUTE_CONFLICT
#ifdef ERROR_INVALID_REPARSE_DATA
	{	ERROR_INVALID_REPARSE_DATA,                               "ERROR_INVALID_REPARSE_DATA",
		"The data present in the reparse point buffer is invalid.",
		"The data present in the reparse point buffer is invalid."
	},
#endif // #ifdef ERROR_INVALID_REPARSE_DATA
#ifdef ERROR_REPARSE_TAG_INVALID
	{	ERROR_REPARSE_TAG_INVALID,                                "ERROR_REPARSE_TAG_INVALID",
		"The tag present in the reparse point buffer is invalid.",
		"The tag present in the reparse point buffer is invalid."
	},
#endif // #ifdef ERROR_REPARSE_TAG_INVALID
#ifdef ERROR_REPARSE_TAG_MISMATCH
	{	ERROR_REPARSE_TAG_MISMATCH,                               "ERROR_REPARSE_TAG_MISMATCH",
		"There is a mismatch between the tag specified in the request and the tag present in the reparse point.",
		"There is a mismatch between the tag specified in the request and the tag present in the reparse point."
	},
#endif // #ifdef ERROR_REPARSE_TAG_MISMATCH
#ifdef ERROR_VOLUME_NOT_SIS_ENABLED
	{	ERROR_VOLUME_NOT_SIS_ENABLED,                             "ERROR_VOLUME_NOT_SIS_ENABLED",
		"Single Instance Storage is not available on this volume.",
		"Single Instance Storage is not available on this volume."
	},
#endif // #ifdef ERROR_VOLUME_NOT_SIS_ENABLED
#ifdef ERROR_DEPENDENT_RESOURCE_EXISTS
	{	ERROR_DEPENDENT_RESOURCE_EXISTS,                          "ERROR_DEPENDENT_RESOURCE_EXISTS",
		"The cluster resource cannot be moved to another group because other resources are dependent on it.",
		"The cluster resource cannot be moved to another group because other resources are dependent on it."
	},
#endif // #ifdef ERROR_DEPENDENT_RESOURCE_EXISTS
#ifdef ERROR_DEPENDENCY_NOT_FOUND
	{	ERROR_DEPENDENCY_NOT_FOUND,                               "ERROR_DEPENDENCY_NOT_FOUND",
		"The cluster resource dependency cannot be found.",
		"The cluster resource dependency cannot be found."
	},
#endif // #ifdef ERROR_DEPENDENCY_NOT_FOUND
#ifdef ERROR_DEPENDENCY_ALREADY_EXISTS
	{	ERROR_DEPENDENCY_ALREADY_EXISTS,                          "ERROR_DEPENDENCY_ALREADY_EXISTS",
		"The cluster resource cannot be made dependent on the specified resource because it is already dependent.",
		"The cluster resource cannot be made dependent on the specified resource because it is already dependent."
	},
#endif // #ifdef ERROR_DEPENDENCY_ALREADY_EXISTS
#ifdef ERROR_RESOURCE_NOT_ONLINE
	{	ERROR_RESOURCE_NOT_ONLINE,                                "ERROR_RESOURCE_NOT_ONLINE",
		"The cluster resource is not online.",
		"The cluster resource is not online."
	},
#endif // #ifdef ERROR_RESOURCE_NOT_ONLINE
#ifdef ERROR_HOST_NODE_NOT_AVAILABLE
	{	ERROR_HOST_NODE_NOT_AVAILABLE,                            "ERROR_HOST_NODE_NOT_AVAILABLE",
		"A cluster node is not available for this operation.",
		"A cluster node is not available for this operation."
	},
#endif // #ifdef ERROR_HOST_NODE_NOT_AVAILABLE
#ifdef ERROR_RESOURCE_NOT_AVAILABLE
	{	ERROR_RESOURCE_NOT_AVAILABLE,                             "ERROR_RESOURCE_NOT_AVAILABLE",
		"The cluster resource is not available.",
		"The cluster resource is not available."
	},
#endif // #ifdef ERROR_RESOURCE_NOT_AVAILABLE
#ifdef ERROR_RESOURCE_NOT_FOUND
	{	ERROR_RESOURCE_NOT_FOUND,                                 "ERROR_RESOURCE_NOT_FOUND",
		"The cluster resource could not be found.",
		"The cluster resource could not be found."
	},
#endif // #ifdef ERROR_RESOURCE_NOT_FOUND
#ifdef ERROR_SHUTDOWN_CLUSTER
	{	ERROR_SHUTDOWN_CLUSTER,                                   "ERROR_SHUTDOWN_CLUSTER",
		"The cluster is being shut down.",
		"The cluster is being shut down."
	},
#endif // #ifdef ERROR_SHUTDOWN_CLUSTER
#ifdef ERROR_CANT_EVICT_ACTIVE_NODE
	{	ERROR_CANT_EVICT_ACTIVE_NODE,                             "ERROR_CANT_EVICT_ACTIVE_NODE",
		"A cluster node cannot be evicted from the cluster unless the node is down.",
		"A cluster node cannot be evicted from the cluster unless the node is down."
	},
#endif // #ifdef ERROR_CANT_EVICT_ACTIVE_NODE
#ifdef ERROR_OBJECT_ALREADY_EXISTS
	{	ERROR_OBJECT_ALREADY_EXISTS,                              "ERROR_OBJECT_ALREADY_EXISTS",
		"The object already exists.",
		"The object already exists."
	},
#endif // #ifdef ERROR_OBJECT_ALREADY_EXISTS
#ifdef ERROR_OBJECT_IN_LIST
	{	ERROR_OBJECT_IN_LIST,                                     "ERROR_OBJECT_IN_LIST",
		"The object is already in the list.",
		"The object is already in the list."
	},
#endif // #ifdef ERROR_OBJECT_IN_LIST
#ifdef ERROR_GROUP_NOT_AVAILABLE
	{	ERROR_GROUP_NOT_AVAILABLE,                                "ERROR_GROUP_NOT_AVAILABLE",
		"The cluster group is not available for any new requests.",
		"The cluster group is not available for any new requests."
	},
#endif // #ifdef ERROR_GROUP_NOT_AVAILABLE
#ifdef ERROR_GROUP_NOT_FOUND
	{	ERROR_GROUP_NOT_FOUND,                                    "ERROR_GROUP_NOT_FOUND",
		"The cluster group could not be found.",
		"The cluster group could not be found."
	},
#endif // #ifdef ERROR_GROUP_NOT_FOUND
#ifdef ERROR_GROUP_NOT_ONLINE
	{	ERROR_GROUP_NOT_ONLINE,                                   "ERROR_GROUP_NOT_ONLINE",
		"The operation could not be completed because the cluster group is not online.",
		"The operation could not be completed because the cluster group is not online."
	},
#endif // #ifdef ERROR_GROUP_NOT_ONLINE
#ifdef ERROR_HOST_NODE_NOT_RESOURCE_OWNER
	{	ERROR_HOST_NODE_NOT_RESOURCE_OWNER,                       "ERROR_HOST_NODE_NOT_RESOURCE_OWNER",
		"The cluster node is not the owner of the resource.",
		"The cluster node is not the owner of the resource."
	},
#endif // #ifdef ERROR_HOST_NODE_NOT_RESOURCE_OWNER
#ifdef ERROR_HOST_NODE_NOT_GROUP_OWNER
	{	ERROR_HOST_NODE_NOT_GROUP_OWNER,                          "ERROR_HOST_NODE_NOT_GROUP_OWNER",
		"The cluster node is not the owner of the group.",
		"The cluster node is not the owner of the group."
	},
#endif // #ifdef ERROR_HOST_NODE_NOT_GROUP_OWNER
#ifdef ERROR_RESMON_CREATE_FAILED
	{	ERROR_RESMON_CREATE_FAILED,                               "ERROR_RESMON_CREATE_FAILED",
		"The cluster resource could not be created in the specified resource monitor.",
		"The cluster resource could not be created in the specified resource monitor."
	},
#endif // #ifdef ERROR_RESMON_CREATE_FAILED
#ifdef ERROR_RESMON_ONLINE_FAILED
	{	ERROR_RESMON_ONLINE_FAILED,                               "ERROR_RESMON_ONLINE_FAILED",
		"The cluster resource could not be brought online by the resource monitor.",
		"The cluster resource could not be brought online by the resource monitor."
	},
#endif // #ifdef ERROR_RESMON_ONLINE_FAILED
#ifdef ERROR_RESOURCE_ONLINE
	{	ERROR_RESOURCE_ONLINE,                                    "ERROR_RESOURCE_ONLINE",
		"The operation could not be completed because the cluster resource is online.",
		"The operation could not be completed because the cluster resource is online."
	},
#endif // #ifdef ERROR_RESOURCE_ONLINE
#ifdef ERROR_QUORUM_RESOURCE
	{	ERROR_QUORUM_RESOURCE,                                    "ERROR_QUORUM_RESOURCE",
		"The cluster resource could not be deleted or brought offline because it is the quorum resource.",
		"The cluster resource could not be deleted or brought offline because it is the quorum resource."
	},
#endif // #ifdef ERROR_QUORUM_RESOURCE
#ifdef ERROR_NOT_QUORUM_CAPABLE
	{	ERROR_NOT_QUORUM_CAPABLE,                                 "ERROR_NOT_QUORUM_CAPABLE",
		"The cluster could not make the specified resource a quorum resource because it is not capable of being a quorum resource.",
		"The cluster could not make the specified resource a quorum resource because it is not capable of being a quorum resource."
	},
#endif // #ifdef ERROR_NOT_QUORUM_CAPABLE
#ifdef ERROR_CLUSTER_SHUTTING_DOWN
	{	ERROR_CLUSTER_SHUTTING_DOWN,                              "ERROR_CLUSTER_SHUTTING_DOWN",
		"The cluster software is shutting down.",
		"The cluster software is shutting down."
	},
#endif // #ifdef ERROR_CLUSTER_SHUTTING_DOWN
#ifdef ERROR_INVALID_STATE
	{	ERROR_INVALID_STATE,                                      "ERROR_INVALID_STATE",
		"The group or resource is not in the correct state to perform the requested operation.",
		"The group or resource is not in the correct state to perform the requested operation."
	},
#endif // #ifdef ERROR_INVALID_STATE
#ifdef ERROR_RESOURCE_PROPERTIES_STORED
	{	ERROR_RESOURCE_PROPERTIES_STORED,                         "ERROR_RESOURCE_PROPERTIES_STORED",
		"The properties were stored but not all changes will take effect until the next time the resource is brought online.",
		"The properties were stored but not all changes will take effect until the next time the resource is brought online."
	},
#endif // #ifdef ERROR_RESOURCE_PROPERTIES_STORED
#ifdef ERROR_NOT_QUORUM_CLASS
	{	ERROR_NOT_QUORUM_CLASS,                                   "ERROR_NOT_QUORUM_CLASS",
		"The cluster could not make the specified resource a quorum resource because it does not belong to a shared storage class.",
		"The cluster could not make the specified resource a quorum resource because it does not belong to a shared storage class."
	},
#endif // #ifdef ERROR_NOT_QUORUM_CLASS
#ifdef ERROR_CORE_RESOURCE
	{	ERROR_CORE_RESOURCE,                                      "ERROR_CORE_RESOURCE",
		"The cluster resource could not be deleted since it is a core resource.",
		"The cluster resource could not be deleted since it is a core resource."
	},
#endif // #ifdef ERROR_CORE_RESOURCE
#ifdef ERROR_QUORUM_RESOURCE_ONLINE_FAILED
	{	ERROR_QUORUM_RESOURCE_ONLINE_FAILED,                      "ERROR_QUORUM_RESOURCE_ONLINE_FAILED",
		"The quorum resource failed to come online.",
		"The quorum resource failed to come online."
	},
#endif // #ifdef ERROR_QUORUM_RESOURCE_ONLINE_FAILED
#ifdef ERROR_QUORUMLOG_OPEN_FAILED
	{	ERROR_QUORUMLOG_OPEN_FAILED,                              "ERROR_QUORUMLOG_OPEN_FAILED",
		"The quorum log could not be created or mounted successfully.",
		"The quorum log could not be created or mounted successfully."
	},
#endif // #ifdef ERROR_QUORUMLOG_OPEN_FAILED
#ifdef ERROR_CLUSTERLOG_CORRUPT
	{	ERROR_CLUSTERLOG_CORRUPT,                                 "ERROR_CLUSTERLOG_CORRUPT",
		"The cluster log is corrupt.",
		"The cluster log is corrupt."
	},
#endif // #ifdef ERROR_CLUSTERLOG_CORRUPT
#ifdef ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE
	{	ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE,                  "ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE",
		"The record could not be written to the cluster log since it exceeds the maximum size.",
		"The record could not be written to the cluster log since it exceeds the maximum size."
	},
#endif // #ifdef ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE
#ifdef ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE
	{	ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE,                         "ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE",
		"The cluster log exceeds its maximum size.",
		"The cluster log exceeds its maximum size."
	},
#endif // #ifdef ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE
#ifdef ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND
	{	ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND,                      "ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND",
		"No checkpoint record was found in the cluster log.",
		"No checkpoint record was found in the cluster log."
	},
#endif // #ifdef ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND
#ifdef ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE
	{	ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE,                        "ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE",
		"The minimum required disk space needed for logging is not available.",
		"The minimum required disk space needed for logging is not available."
	},
#endif // #ifdef ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE
#ifdef ERROR_QUORUM_OWNER_ALIVE
	{	ERROR_QUORUM_OWNER_ALIVE,                                 "ERROR_QUORUM_OWNER_ALIVE",
		"The cluster node failed to take control of the quorum resource because the resource is owned by another active node.",
		"The cluster node failed to take control of the quorum resource because the resource is owned by another active node."
	},
#endif // #ifdef ERROR_QUORUM_OWNER_ALIVE
#ifdef ERROR_NETWORK_NOT_AVAILABLE
	{	ERROR_NETWORK_NOT_AVAILABLE,                              "ERROR_NETWORK_NOT_AVAILABLE",
		"A cluster network is not available for this operation.",
		"A cluster network is not available for this operation."
	},
#endif // #ifdef ERROR_NETWORK_NOT_AVAILABLE
#ifdef ERROR_NODE_NOT_AVAILABLE
	{	ERROR_NODE_NOT_AVAILABLE,                                 "ERROR_NODE_NOT_AVAILABLE",
		"A cluster node is not available for this operation.",
		"A cluster node is not available for this operation."
	},
#endif // #ifdef ERROR_NODE_NOT_AVAILABLE
#ifdef ERROR_ALL_NODES_NOT_AVAILABLE
	{	ERROR_ALL_NODES_NOT_AVAILABLE,                            "ERROR_ALL_NODES_NOT_AVAILABLE",
		"All cluster nodes must be running to perform this operation.",
		"All cluster nodes must be running to perform this operation."
	},
#endif // #ifdef ERROR_ALL_NODES_NOT_AVAILABLE
#ifdef ERROR_RESOURCE_FAILED
	{	ERROR_RESOURCE_FAILED,                                    "ERROR_RESOURCE_FAILED",
		"A cluster resource failed.",
		"A cluster resource failed."
	},
#endif // #ifdef ERROR_RESOURCE_FAILED
#ifdef ERROR_CLUSTER_INVALID_NODE
	{	ERROR_CLUSTER_INVALID_NODE,                               "ERROR_CLUSTER_INVALID_NODE",
		"The cluster node is not valid.",
		"The cluster node is not valid."
	},
#endif // #ifdef ERROR_CLUSTER_INVALID_NODE
#ifdef ERROR_CLUSTER_NODE_EXISTS
	{	ERROR_CLUSTER_NODE_EXISTS,                                "ERROR_CLUSTER_NODE_EXISTS",
		"The cluster node already exists.",
		"The cluster node already exists."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_EXISTS
#ifdef ERROR_CLUSTER_JOIN_IN_PROGRESS
	{	ERROR_CLUSTER_JOIN_IN_PROGRESS,                           "ERROR_CLUSTER_JOIN_IN_PROGRESS",
		"A node is in the process of joining the cluster.",
		"A node is in the process of joining the cluster."
	},
#endif // #ifdef ERROR_CLUSTER_JOIN_IN_PROGRESS
#ifdef ERROR_CLUSTER_NODE_NOT_FOUND
	{	ERROR_CLUSTER_NODE_NOT_FOUND,                             "ERROR_CLUSTER_NODE_NOT_FOUND",
		"The cluster node was not found.",
		"The cluster node was not found."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_NOT_FOUND
#ifdef ERROR_CLUSTER_LOCAL_NODE_NOT_FOUND
	{	ERROR_CLUSTER_LOCAL_NODE_NOT_FOUND,                       "ERROR_CLUSTER_LOCAL_NODE_NOT_FOUND",
		"The cluster local node information was not found.",
		"The cluster local node information was not found."
	},
#endif // #ifdef ERROR_CLUSTER_LOCAL_NODE_NOT_FOUND
#ifdef ERROR_CLUSTER_NETWORK_EXISTS
	{	ERROR_CLUSTER_NETWORK_EXISTS,                             "ERROR_CLUSTER_NETWORK_EXISTS",
		"The cluster network already exists.",
		"The cluster network already exists."
	},
#endif // #ifdef ERROR_CLUSTER_NETWORK_EXISTS
#ifdef ERROR_CLUSTER_NETWORK_NOT_FOUND
	{	ERROR_CLUSTER_NETWORK_NOT_FOUND,                          "ERROR_CLUSTER_NETWORK_NOT_FOUND",
		"The cluster network was not found.",
		"The cluster network was not found."
	},
#endif // #ifdef ERROR_CLUSTER_NETWORK_NOT_FOUND
#ifdef ERROR_CLUSTER_NETINTERFACE_EXISTS
	{	ERROR_CLUSTER_NETINTERFACE_EXISTS,                        "ERROR_CLUSTER_NETINTERFACE_EXISTS",
		"The cluster network interface already exists.",
		"The cluster network interface already exists."
	},
#endif // #ifdef ERROR_CLUSTER_NETINTERFACE_EXISTS
#ifdef ERROR_CLUSTER_NETINTERFACE_NOT_FOUND
	{	ERROR_CLUSTER_NETINTERFACE_NOT_FOUND,                     "ERROR_CLUSTER_NETINTERFACE_NOT_FOUND",
		"The cluster network interface was not found.",
		"The cluster network interface was not found."
	},
#endif // #ifdef ERROR_CLUSTER_NETINTERFACE_NOT_FOUND
#ifdef ERROR_CLUSTER_INVALID_REQUEST
	{	ERROR_CLUSTER_INVALID_REQUEST,                            "ERROR_CLUSTER_INVALID_REQUEST",
		"The cluster request is not valid for this object.",
		"The cluster request is not valid for this object."
	},
#endif // #ifdef ERROR_CLUSTER_INVALID_REQUEST
#ifdef ERROR_CLUSTER_INVALID_NETWORK_PROVIDER
	{	ERROR_CLUSTER_INVALID_NETWORK_PROVIDER,                   "ERROR_CLUSTER_INVALID_NETWORK_PROVIDER",
		"The cluster network provider is not valid.",
		"The cluster network provider is not valid."
	},
#endif // #ifdef ERROR_CLUSTER_INVALID_NETWORK_PROVIDER
#ifdef ERROR_CLUSTER_NODE_DOWN
	{	ERROR_CLUSTER_NODE_DOWN,                                  "ERROR_CLUSTER_NODE_DOWN",
		"The cluster node is down.",
		"The cluster node is down."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_DOWN
#ifdef ERROR_CLUSTER_NODE_UNREACHABLE
	{	ERROR_CLUSTER_NODE_UNREACHABLE,                           "ERROR_CLUSTER_NODE_UNREACHABLE",
		"The cluster node is not reachable.",
		"The cluster node is not reachable."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_UNREACHABLE
#ifdef ERROR_CLUSTER_NODE_NOT_MEMBER
	{	ERROR_CLUSTER_NODE_NOT_MEMBER,                            "ERROR_CLUSTER_NODE_NOT_MEMBER",
		"The cluster node is not a member of the cluster.",
		"The cluster node is not a member of the cluster."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_NOT_MEMBER
#ifdef ERROR_CLUSTER_JOIN_NOT_IN_PROGRESS
	{	ERROR_CLUSTER_JOIN_NOT_IN_PROGRESS,                       "ERROR_CLUSTER_JOIN_NOT_IN_PROGRESS",
		"A cluster join operation is not in progress.",
		"A cluster join operation is not in progress."
	},
#endif // #ifdef ERROR_CLUSTER_JOIN_NOT_IN_PROGRESS
#ifdef ERROR_CLUSTER_INVALID_NETWORK
	{	ERROR_CLUSTER_INVALID_NETWORK,                            "ERROR_CLUSTER_INVALID_NETWORK",
		"The cluster network is not valid.",
		"The cluster network is not valid."
	},
#endif // #ifdef ERROR_CLUSTER_INVALID_NETWORK
#ifdef ERROR_CLUSTER_NODE_UP
	{	ERROR_CLUSTER_NODE_UP,                                    "ERROR_CLUSTER_NODE_UP",
		"The cluster node is up.",
		"The cluster node is up."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_UP
#ifdef ERROR_CLUSTER_IPADDR_IN_USE
	{	ERROR_CLUSTER_IPADDR_IN_USE,                              "ERROR_CLUSTER_IPADDR_IN_USE",
		"The cluster IP address is already in use.",
		"The cluster IP address is already in use."
	},
#endif // #ifdef ERROR_CLUSTER_IPADDR_IN_USE
#ifdef ERROR_CLUSTER_NODE_NOT_PAUSED
	{	ERROR_CLUSTER_NODE_NOT_PAUSED,                            "ERROR_CLUSTER_NODE_NOT_PAUSED",
		"The cluster node is not paused.",
		"The cluster node is not paused."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_NOT_PAUSED
#ifdef ERROR_CLUSTER_NO_SECURITY_CONTEXT
	{	ERROR_CLUSTER_NO_SECURITY_CONTEXT,                        "ERROR_CLUSTER_NO_SECURITY_CONTEXT",
		"No cluster security context is available.",
		"No cluster security context is available."
	},
#endif // #ifdef ERROR_CLUSTER_NO_SECURITY_CONTEXT
#ifdef ERROR_CLUSTER_NETWORK_NOT_INTERNAL
	{	ERROR_CLUSTER_NETWORK_NOT_INTERNAL,                       "ERROR_CLUSTER_NETWORK_NOT_INTERNAL",
		"The cluster network is not configured for internal cluster communication.",
		"The cluster network is not configured for internal cluster communication."
	},
#endif // #ifdef ERROR_CLUSTER_NETWORK_NOT_INTERNAL
#ifdef ERROR_CLUSTER_NODE_ALREADY_UP
	{	ERROR_CLUSTER_NODE_ALREADY_UP,                            "ERROR_CLUSTER_NODE_ALREADY_UP",
		"The cluster node is already up.",
		"The cluster node is already up."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_ALREADY_UP
#ifdef ERROR_CLUSTER_NODE_ALREADY_DOWN
	{	ERROR_CLUSTER_NODE_ALREADY_DOWN,                          "ERROR_CLUSTER_NODE_ALREADY_DOWN",
		"The cluster node is already down.",
		"The cluster node is already down."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_ALREADY_DOWN
#ifdef ERROR_CLUSTER_NETWORK_ALREADY_ONLINE
	{	ERROR_CLUSTER_NETWORK_ALREADY_ONLINE,                     "ERROR_CLUSTER_NETWORK_ALREADY_ONLINE",
		"The cluster network is already online.",
		"The cluster network is already online."
	},
#endif // #ifdef ERROR_CLUSTER_NETWORK_ALREADY_ONLINE
#ifdef ERROR_CLUSTER_NETWORK_ALREADY_OFFLINE
	{	ERROR_CLUSTER_NETWORK_ALREADY_OFFLINE,                    "ERROR_CLUSTER_NETWORK_ALREADY_OFFLINE",
		"The cluster network is already offline.",
		"The cluster network is already offline."
	},
#endif // #ifdef ERROR_CLUSTER_NETWORK_ALREADY_OFFLINE
#ifdef ERROR_CLUSTER_NODE_ALREADY_MEMBER
	{	ERROR_CLUSTER_NODE_ALREADY_MEMBER,                        "ERROR_CLUSTER_NODE_ALREADY_MEMBER",
		"The cluster node is already a member of the cluster.",
		"The cluster node is already a member of the cluster."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_ALREADY_MEMBER
#ifdef ERROR_CLUSTER_LAST_INTERNAL_NETWORK
	{	ERROR_CLUSTER_LAST_INTERNAL_NETWORK,                      "ERROR_CLUSTER_LAST_INTERNAL_NETWORK",
		"The cluster network is the only one configured for internal cluster communication between two or more active cluster nodes. The internal communication capability cannot be removed from the network.",
		"The cluster network is the only one configured for internal cluster communication between two or more active cluster nodes. The internal communication capability cannot be removed from the network."
	},
#endif // #ifdef ERROR_CLUSTER_LAST_INTERNAL_NETWORK
#ifdef ERROR_CLUSTER_NETWORK_HAS_DEPENDENTS
	{	ERROR_CLUSTER_NETWORK_HAS_DEPENDENTS,                     "ERROR_CLUSTER_NETWORK_HAS_DEPENDENTS",
		"One or more cluster resources depend on the network to provide service to clients. The client access capability cannot be removed from the network.",
		"One or more cluster resources depend on the network to provide service to clients. The client access capability cannot be removed from the network."
	},
#endif // #ifdef ERROR_CLUSTER_NETWORK_HAS_DEPENDENTS
#ifdef ERROR_INVALID_OPERATION_ON_QUORUM
	{	ERROR_INVALID_OPERATION_ON_QUORUM,                        "ERROR_INVALID_OPERATION_ON_QUORUM",
		"This operation cannot be performed on the cluster resource as it the quorum resource. You may not bring the quorum resource offline or modify its possible owners list.",
		"This operation cannot be performed on the cluster resource as it the quorum resource. You may not bring the quorum resource offline or modify its possible owners list."
	},
#endif // #ifdef ERROR_INVALID_OPERATION_ON_QUORUM
#ifdef ERROR_DEPENDENCY_NOT_ALLOWED
	{	ERROR_DEPENDENCY_NOT_ALLOWED,                             "ERROR_DEPENDENCY_NOT_ALLOWED",
		"The cluster quorum resource is not allowed to have any dependencies.",
		"The cluster quorum resource is not allowed to have any dependencies."
	},
#endif // #ifdef ERROR_DEPENDENCY_NOT_ALLOWED
#ifdef ERROR_CLUSTER_NODE_PAUSED
	{	ERROR_CLUSTER_NODE_PAUSED,                                "ERROR_CLUSTER_NODE_PAUSED",
		"The cluster node is paused.",
		"The cluster node is paused."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_PAUSED
#ifdef ERROR_NODE_CANT_HOST_RESOURCE
	{	ERROR_NODE_CANT_HOST_RESOURCE,                            "ERROR_NODE_CANT_HOST_RESOURCE",
		"The cluster resource cannot be brought online. The owner node cannot run this resource.",
		"The cluster resource cannot be brought online. The owner node cannot run this resource."
	},
#endif // #ifdef ERROR_NODE_CANT_HOST_RESOURCE
#ifdef ERROR_CLUSTER_NODE_NOT_READY
	{	ERROR_CLUSTER_NODE_NOT_READY,                             "ERROR_CLUSTER_NODE_NOT_READY",
		"The cluster node is not ready to perform the requested operation.",
		"The cluster node is not ready to perform the requested operation."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_NOT_READY
#ifdef ERROR_CLUSTER_NODE_SHUTTING_DOWN
	{	ERROR_CLUSTER_NODE_SHUTTING_DOWN,                         "ERROR_CLUSTER_NODE_SHUTTING_DOWN",
		"The cluster node is shutting down.",
		"The cluster node is shutting down."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_SHUTTING_DOWN
#ifdef ERROR_CLUSTER_JOIN_ABORTED
	{	ERROR_CLUSTER_JOIN_ABORTED,                               "ERROR_CLUSTER_JOIN_ABORTED",
		"The cluster join operation was aborted.",
		"The cluster join operation was aborted."
	},
#endif // #ifdef ERROR_CLUSTER_JOIN_ABORTED
#ifdef ERROR_CLUSTER_INCOMPATIBLE_VERSIONS
	{	ERROR_CLUSTER_INCOMPATIBLE_VERSIONS,                      "ERROR_CLUSTER_INCOMPATIBLE_VERSIONS",
		"The cluster join operation failed due to incompatible software versions between the joining node and its sponsor.",
		"The cluster join operation failed due to incompatible software versions between the joining node and its sponsor."
	},
#endif // #ifdef ERROR_CLUSTER_INCOMPATIBLE_VERSIONS
#ifdef ERROR_CLUSTER_MAXNUM_OF_RESOURCES_EXCEEDED
	{	ERROR_CLUSTER_MAXNUM_OF_RESOURCES_EXCEEDED,               "ERROR_CLUSTER_MAXNUM_OF_RESOURCES_EXCEEDED",
		"This resource cannot be created because the cluster has reached the limit on the number of resources it can monitor.",
		"This resource cannot be created because the cluster has reached the limit on the number of resources it can monitor."
	},
#endif // #ifdef ERROR_CLUSTER_MAXNUM_OF_RESOURCES_EXCEEDED
#ifdef ERROR_CLUSTER_SYSTEM_CONFIG_CHANGED
	{	ERROR_CLUSTER_SYSTEM_CONFIG_CHANGED,                      "ERROR_CLUSTER_SYSTEM_CONFIG_CHANGED",
		"The system configuration changed during the cluster join or form operation. The join or form operation was aborted.",
		"The system configuration changed during the cluster join or form operation. The join or form operation was aborted."
	},
#endif // #ifdef ERROR_CLUSTER_SYSTEM_CONFIG_CHANGED
#ifdef ERROR_CLUSTER_RESOURCE_TYPE_NOT_FOUND
	{	ERROR_CLUSTER_RESOURCE_TYPE_NOT_FOUND,                    "ERROR_CLUSTER_RESOURCE_TYPE_NOT_FOUND",
		"The specified resource type was not found.",
		"The specified resource type was not found."
	},
#endif // #ifdef ERROR_CLUSTER_RESOURCE_TYPE_NOT_FOUND
#ifdef ERROR_CLUSTER_RESTYPE_NOT_SUPPORTED
	{	ERROR_CLUSTER_RESTYPE_NOT_SUPPORTED,                      "ERROR_CLUSTER_RESTYPE_NOT_SUPPORTED",
		"The specified node does not support a resource of this type. This may be due to version inconsistencies or due to the absence of the resource DLL on this node.",
		"The specified node does not support a resource of this type. This may be due to version inconsistencies or due to the absence of the resource DLL on this node."
	},
#endif // #ifdef ERROR_CLUSTER_RESTYPE_NOT_SUPPORTED
#ifdef ERROR_CLUSTER_RESNAME_NOT_FOUND
	{	ERROR_CLUSTER_RESNAME_NOT_FOUND,                          "ERROR_CLUSTER_RESNAME_NOT_FOUND",
		"The specified resource name is supported by this resource DLL. This may be due to a bad (or changed) name supplied to the resource DLL.",
		"The specified resource name is supported by this resource DLL. This may be due to a bad (or changed) name supplied to the resource DLL."
	},
#endif // #ifdef ERROR_CLUSTER_RESNAME_NOT_FOUND
#ifdef ERROR_CLUSTER_NO_RPC_PACKAGES_REGISTERED
	{	ERROR_CLUSTER_NO_RPC_PACKAGES_REGISTERED,                 "ERROR_CLUSTER_NO_RPC_PACKAGES_REGISTERED",
		"No authentication package could be registered with the RPC server.",
		"No authentication package could be registered with the RPC server."
	},
#endif // #ifdef ERROR_CLUSTER_NO_RPC_PACKAGES_REGISTERED
#ifdef ERROR_CLUSTER_OWNER_NOT_IN_PREFLIST
	{	ERROR_CLUSTER_OWNER_NOT_IN_PREFLIST,                      "ERROR_CLUSTER_OWNER_NOT_IN_PREFLIST",
		"You cannot bring the group online because the owner of the group is not in the preferred list for the group. To change the owner node for the group, move the group.",
		"You cannot bring the group online because the owner of the group is not in the preferred list for the group. To change the owner node for the group, move the group."
	},
#endif // #ifdef ERROR_CLUSTER_OWNER_NOT_IN_PREFLIST
#ifdef ERROR_CLUSTER_DATABASE_SEQMISMATCH
	{	ERROR_CLUSTER_DATABASE_SEQMISMATCH,                       "ERROR_CLUSTER_DATABASE_SEQMISMATCH",
		"The join operation failed because the cluster database sequence number has changed or is incompatible with the locker node. This may happen during a join operation if the cluster database was changing during the join.",
		"The join operation failed because the cluster database sequence number has changed or is incompatible with the locker node. This may happen during a join operation if the cluster database was changing during the join."
	},
#endif // #ifdef ERROR_CLUSTER_DATABASE_SEQMISMATCH
#ifdef ERROR_RESMON_INVALID_STATE
	{	ERROR_RESMON_INVALID_STATE,                               "ERROR_RESMON_INVALID_STATE",
		"The resource monitor will not allow the fail operation to be performed while the resource is in its current state. This may happen if the resource is in a pending state.",
		"The resource monitor will not allow the fail operation to be performed while the resource is in its current state. This may happen if the resource is in a pending state."
	},
#endif // #ifdef ERROR_RESMON_INVALID_STATE
#ifdef ERROR_CLUSTER_GUM_NOT_LOCKER
	{	ERROR_CLUSTER_GUM_NOT_LOCKER,                             "ERROR_CLUSTER_GUM_NOT_LOCKER",
		"A non locker code got a request to reserve the lock for making global updates.",
		"A non locker code got a request to reserve the lock for making global updates."
	},
#endif // #ifdef ERROR_CLUSTER_GUM_NOT_LOCKER
#ifdef ERROR_QUORUM_DISK_NOT_FOUND
	{	ERROR_QUORUM_DISK_NOT_FOUND,                              "ERROR_QUORUM_DISK_NOT_FOUND",
		"The quorum disk could not be located by the cluster service.",
		"The quorum disk could not be located by the cluster service."
	},
#endif // #ifdef ERROR_QUORUM_DISK_NOT_FOUND
#ifdef ERROR_DATABASE_BACKUP_CORRUPT
	{	ERROR_DATABASE_BACKUP_CORRUPT,                            "ERROR_DATABASE_BACKUP_CORRUPT",
		"The backup up cluster database is possibly corrupt.",
		"The backup up cluster database is possibly corrupt."
	},
#endif // #ifdef ERROR_DATABASE_BACKUP_CORRUPT
#ifdef ERROR_CLUSTER_NODE_ALREADY_HAS_DFS_ROOT
	{	ERROR_CLUSTER_NODE_ALREADY_HAS_DFS_ROOT,                  "ERROR_CLUSTER_NODE_ALREADY_HAS_DFS_ROOT",
		"A DFS root already exists in this cluster node.",
		"A DFS root already exists in this cluster node."
	},
#endif // #ifdef ERROR_CLUSTER_NODE_ALREADY_HAS_DFS_ROOT
#ifdef ERROR_RESOURCE_PROPERTY_UNCHANGEABLE
	{	ERROR_RESOURCE_PROPERTY_UNCHANGEABLE,                     "ERROR_RESOURCE_PROPERTY_UNCHANGEABLE",
		"An attempt to modify a resource property failed because it conflicts with another existing property.",
		"An attempt to modify a resource property failed because it conflicts with another existing property."
	},
#endif // #ifdef ERROR_RESOURCE_PROPERTY_UNCHANGEABLE
#ifdef ERROR_CLUSTER_MEMBERSHIP_INVALID_STATE
	{	ERROR_CLUSTER_MEMBERSHIP_INVALID_STATE,                   "ERROR_CLUSTER_MEMBERSHIP_INVALID_STATE",
		"An operation was attempted that is incompatible with the current membership state of the node.",
		"An operation was attempted that is incompatible with the current membership state of the node."
	},
#endif // #ifdef ERROR_CLUSTER_MEMBERSHIP_INVALID_STATE
#ifdef ERROR_CLUSTER_QUORUMLOG_NOT_FOUND
	{	ERROR_CLUSTER_QUORUMLOG_NOT_FOUND,                        "ERROR_CLUSTER_QUORUMLOG_NOT_FOUND",
		"The quorum resource does not contain the quorum log.",
		"The quorum resource does not contain the quorum log."
	},
#endif // #ifdef ERROR_CLUSTER_QUORUMLOG_NOT_FOUND
#ifdef ERROR_CLUSTER_MEMBERSHIP_HALT
	{	ERROR_CLUSTER_MEMBERSHIP_HALT,                            "ERROR_CLUSTER_MEMBERSHIP_HALT",
		"The membership engine requested shutdown of the cluster service on this node.",
		"The membership engine requested shutdown of the cluster service on this node."
	},
#endif // #ifdef ERROR_CLUSTER_MEMBERSHIP_HALT
#ifdef ERROR_CLUSTER_INSTANCE_ID_MISMATCH
	{	ERROR_CLUSTER_INSTANCE_ID_MISMATCH,                       "ERROR_CLUSTER_INSTANCE_ID_MISMATCH",
		"The join operation failed because the cluster instance ID of the joining node does not match the cluster instance ID of the sponsor node.",
		"The join operation failed because the cluster instance ID of the joining node does not match the cluster instance ID of the sponsor node."
	},
#endif // #ifdef ERROR_CLUSTER_INSTANCE_ID_MISMATCH
#ifdef ERROR_CLUSTER_NETWORK_NOT_FOUND_FOR_IP
	{	ERROR_CLUSTER_NETWORK_NOT_FOUND_FOR_IP,                   "ERROR_CLUSTER_NETWORK_NOT_FOUND_FOR_IP",
		"A matching network for the specified IP address could not be found. Please also specify a subnet mask and a cluster network.",
		"A matching network for the specified IP address could not be found. Please also specify a subnet mask and a cluster network."
	},
#endif // #ifdef ERROR_CLUSTER_NETWORK_NOT_FOUND_FOR_IP
#ifdef ERROR_CLUSTER_PROPERTY_DATA_TYPE_MISMATCH
	{	ERROR_CLUSTER_PROPERTY_DATA_TYPE_MISMATCH,                "ERROR_CLUSTER_PROPERTY_DATA_TYPE_MISMATCH",
		"The actual data type of the property did not match the expected data type of the property.",
		"The actual data type of the property did not match the expected data type of the property."
	},
#endif // #ifdef ERROR_CLUSTER_PROPERTY_DATA_TYPE_MISMATCH
#ifdef ERROR_CLUSTER_EVICT_WITHOUT_CLEANUP
	{	ERROR_CLUSTER_EVICT_WITHOUT_CLEANUP,                      "ERROR_CLUSTER_EVICT_WITHOUT_CLEANUP",
		"The cluster node was evicted from the cluster successfully, but the node was not cleaned up. Extended status information explaining why the node was not cleaned up is available.",
		"The cluster node was evicted from the cluster successfully, but the node was not cleaned up. Extended status information explaining why the node was not cleaned up is available."
	},
#endif // #ifdef ERROR_CLUSTER_EVICT_WITHOUT_CLEANUP
#ifdef ERROR_CLUSTER_PARAMETER_MISMATCH
	{	ERROR_CLUSTER_PARAMETER_MISMATCH,                         "ERROR_CLUSTER_PARAMETER_MISMATCH",
		"Two or more parameter values specified for a resource's properties are in conflict.",
		"Two or more parameter values specified for a resource's properties are in conflict."
	},
#endif // #ifdef ERROR_CLUSTER_PARAMETER_MISMATCH
#ifdef ERROR_NODE_CANNOT_BE_CLUSTERED
	{	ERROR_NODE_CANNOT_BE_CLUSTERED,                           "ERROR_NODE_CANNOT_BE_CLUSTERED",
		"This computer cannot be made a member of a cluster.",
		"This computer cannot be made a member of a cluster."
	},
#endif // #ifdef ERROR_NODE_CANNOT_BE_CLUSTERED
#ifdef ERROR_CLUSTER_WRONG_OS_VERSION
	{	ERROR_CLUSTER_WRONG_OS_VERSION,                           "ERROR_CLUSTER_WRONG_OS_VERSION",
		"This computer cannot be made a member of a cluster because it does not have the correct version of Windows installed.",
		"This computer cannot be made a member of a cluster because it does not have the correct version of Windows installed."
	},
#endif // #ifdef ERROR_CLUSTER_WRONG_OS_VERSION
#ifdef ERROR_CLUSTER_CANT_CREATE_DUP_CLUSTER_NAME
	{	ERROR_CLUSTER_CANT_CREATE_DUP_CLUSTER_NAME,               "ERROR_CLUSTER_CANT_CREATE_DUP_CLUSTER_NAME",
		"A cluster cannot be created with the specified cluster name because that cluster name is already in use. Specify a different name for the cluster.",
		"A cluster cannot be created with the specified cluster name because that cluster name is already in use. Specify a different name for the cluster."
	},
#endif // #ifdef ERROR_CLUSTER_CANT_CREATE_DUP_CLUSTER_NAME
#ifdef ERROR_CLUSCFG_ALREADY_COMMITTED
	{	ERROR_CLUSCFG_ALREADY_COMMITTED,                          "ERROR_CLUSCFG_ALREADY_COMMITTED",
		"The cluster configuration action has already been committed.",
		"The cluster configuration action has already been committed."
	},
#endif // #ifdef ERROR_CLUSCFG_ALREADY_COMMITTED
#ifdef ERROR_CLUSCFG_ROLLBACK_FAILED
	{	ERROR_CLUSCFG_ROLLBACK_FAILED,                            "ERROR_CLUSCFG_ROLLBACK_FAILED",
		"The cluster configuration action could not be rolled back.",
		"The cluster configuration action could not be rolled back."
	},
#endif // #ifdef ERROR_CLUSCFG_ROLLBACK_FAILED
#ifdef ERROR_CLUSCFG_SYSTEM_DISK_DRIVE_LETTER_CONFLICT
	{	ERROR_CLUSCFG_SYSTEM_DISK_DRIVE_LETTER_CONFLICT,          "ERROR_CLUSCFG_SYSTEM_DISK_DRIVE_LETTER_CONFLICT",
		"The drive letter assigned to a system disk on one node conflicted with the driver letter assigned to a disk on another node.",
		"The drive letter assigned to a system disk on one node conflicted with the driver letter assigned to a disk on another node."
	},
#endif // #ifdef ERROR_CLUSCFG_SYSTEM_DISK_DRIVE_LETTER_CONFLICT
#ifdef ERROR_CLUSTER_OLD_VERSION
	{	ERROR_CLUSTER_OLD_VERSION,                                "ERROR_CLUSTER_OLD_VERSION",
		"One or more nodes in the cluster are running a version of Windows that does not support this operation.",
		"One or more nodes in the cluster are running a version of Windows that does not support this operation."
	},
#endif // #ifdef ERROR_CLUSTER_OLD_VERSION
#ifdef ERROR_CLUSTER_MISMATCHED_COMPUTER_ACCT_NAME
	{	ERROR_CLUSTER_MISMATCHED_COMPUTER_ACCT_NAME,              "ERROR_CLUSTER_MISMATCHED_COMPUTER_ACCT_NAME",
		"The name of the corresponding computer account doesn't match the Network Name for this resource.",
		"The name of the corresponding computer account doesn't match the Network Name for this resource."
	},
#endif // #ifdef ERROR_CLUSTER_MISMATCHED_COMPUTER_ACCT_NAME
#ifdef ERROR_ENCRYPTION_FAILED
	{	ERROR_ENCRYPTION_FAILED,                                  "ERROR_ENCRYPTION_FAILED",
		"The specified file could not be encrypted.",
		"The specified file could not be encrypted."
	},
#endif // #ifdef ERROR_ENCRYPTION_FAILED
#ifdef ERROR_DECRYPTION_FAILED
	{	ERROR_DECRYPTION_FAILED,                                  "ERROR_DECRYPTION_FAILED",
		"The specified file could not be decrypted.",
		"The specified file could not be decrypted."
	},
#endif // #ifdef ERROR_DECRYPTION_FAILED
#ifdef ERROR_FILE_ENCRYPTED
	{	ERROR_FILE_ENCRYPTED,                                     "ERROR_FILE_ENCRYPTED",
		"The specified file is encrypted and the user does not have the ability to decrypt it.",
		"The specified file is encrypted and the user does not have the ability to decrypt it."
	},
#endif // #ifdef ERROR_FILE_ENCRYPTED
#ifdef ERROR_NO_RECOVERY_POLICY
	{	ERROR_NO_RECOVERY_POLICY,                                 "ERROR_NO_RECOVERY_POLICY",
		"There is no valid encryption recovery policy configured for this system.",
		"There is no valid encryption recovery policy configured for this system."
	},
#endif // #ifdef ERROR_NO_RECOVERY_POLICY
#ifdef ERROR_NO_EFS
	{	ERROR_NO_EFS,                                             "ERROR_NO_EFS",
		"The required encryption driver is not loaded for this system.",
		"The required encryption driver is not loaded for this system."
	},
#endif // #ifdef ERROR_NO_EFS
#ifdef ERROR_WRONG_EFS
	{	ERROR_WRONG_EFS,                                          "ERROR_WRONG_EFS",
		"The file was encrypted with a different encryption driver than is currently loaded.",
		"The file was encrypted with a different encryption driver than is currently loaded."
	},
#endif // #ifdef ERROR_WRONG_EFS
#ifdef ERROR_NO_USER_KEYS
	{	ERROR_NO_USER_KEYS,                                       "ERROR_NO_USER_KEYS",
		"There are no EFS keys defined for the user.",
		"There are no EFS keys defined for the user."
	},
#endif // #ifdef ERROR_NO_USER_KEYS
#ifdef ERROR_FILE_NOT_ENCRYPTED
	{	ERROR_FILE_NOT_ENCRYPTED,                                 "ERROR_FILE_NOT_ENCRYPTED",
		"The specified file is not encrypted.",
		"The specified file is not encrypted."
	},
#endif // #ifdef ERROR_FILE_NOT_ENCRYPTED
#ifdef ERROR_NOT_EXPORT_FORMAT
	{	ERROR_NOT_EXPORT_FORMAT,                                  "ERROR_NOT_EXPORT_FORMAT",
		"The specified file is not in the defined EFS export format.",
		"The specified file is not in the defined EFS export format."
	},
#endif // #ifdef ERROR_NOT_EXPORT_FORMAT
#ifdef ERROR_FILE_READ_ONLY
	{	ERROR_FILE_READ_ONLY,                                     "ERROR_FILE_READ_ONLY",
		"The specified file is read only.",
		"The specified file is read only."
	},
#endif // #ifdef ERROR_FILE_READ_ONLY
#ifdef ERROR_DIR_EFS_DISALLOWED
	{	ERROR_DIR_EFS_DISALLOWED,                                 "ERROR_DIR_EFS_DISALLOWED",
		"The directory has been disabled for encryption.",
		"The directory has been disabled for encryption."
	},
#endif // #ifdef ERROR_DIR_EFS_DISALLOWED
#ifdef ERROR_EFS_SERVER_NOT_TRUSTED
	{	ERROR_EFS_SERVER_NOT_TRUSTED,                             "ERROR_EFS_SERVER_NOT_TRUSTED",
		"The server is not trusted for remote encryption operation.",
		"The server is not trusted for remote encryption operation."
	},
#endif // #ifdef ERROR_EFS_SERVER_NOT_TRUSTED
#ifdef ERROR_BAD_RECOVERY_POLICY
	{	ERROR_BAD_RECOVERY_POLICY,                                "ERROR_BAD_RECOVERY_POLICY",
		"Recovery policy configured for this system contains invalid recovery certificate.",
		"Recovery policy configured for this system contains invalid recovery certificate."
	},
#endif // #ifdef ERROR_BAD_RECOVERY_POLICY
#ifdef ERROR_EFS_ALG_BLOB_TOO_BIG
	{	ERROR_EFS_ALG_BLOB_TOO_BIG,                               "ERROR_EFS_ALG_BLOB_TOO_BIG",
		"The encryption algorithm used on the source file needs a bigger key buffer than the one on the destination file.",
		"The encryption algorithm used on the source file needs a bigger key buffer than the one on the destination file."
	},
#endif // #ifdef ERROR_EFS_ALG_BLOB_TOO_BIG
#ifdef ERROR_VOLUME_NOT_SUPPORT_EFS
	{	ERROR_VOLUME_NOT_SUPPORT_EFS,                             "ERROR_VOLUME_NOT_SUPPORT_EFS",
		"The disk partition does not support file encryption.",
		"The disk partition does not support file encryption."
	},
#endif // #ifdef ERROR_VOLUME_NOT_SUPPORT_EFS
#ifdef ERROR_EFS_DISABLED
	{	ERROR_EFS_DISABLED,                                       "ERROR_EFS_DISABLED",
		"This machine is disabled for file encryption.",
		"This machine is disabled for file encryption."
	},
#endif // #ifdef ERROR_EFS_DISABLED
#ifdef ERROR_EFS_VERSION_NOT_SUPPORT
	{	ERROR_EFS_VERSION_NOT_SUPPORT,                            "ERROR_EFS_VERSION_NOT_SUPPORT",
		"A newer system is required to decrypt this encrypted file.",
		"A newer system is required to decrypt this encrypted file."
	},
#endif // #ifdef ERROR_EFS_VERSION_NOT_SUPPORT
#ifdef ERROR_NO_BROWSER_SERVERS_FOUND
	{	ERROR_NO_BROWSER_SERVERS_FOUND,                           "ERROR_NO_BROWSER_SERVERS_FOUND",
		"The list of servers for this workgroup is not currently available.",
		"The list of servers for this workgroup is not currently available."
	},
#endif // #ifdef ERROR_NO_BROWSER_SERVERS_FOUND
#ifdef SCHED_E_SERVICE_NOT_LOCALSYSTEM
	{	SCHED_E_SERVICE_NOT_LOCALSYSTEM,                          "SCHED_E_SERVICE_NOT_LOCALSYSTEM",
		"The Task Scheduler service must be configured to run in the System account to function properly. Individual tasks may be configured to run in other accounts.",
		"The Task Scheduler service must be configured to run in the System account to function properly. Individual tasks may be configured to run in other accounts."
	},
#endif // #ifdef SCHED_E_SERVICE_NOT_LOCALSYSTEM
#ifdef ERROR_CTX_WINSTATION_NAME_INVALID
	{	ERROR_CTX_WINSTATION_NAME_INVALID,                        "ERROR_CTX_WINSTATION_NAME_INVALID",
		"The specified session name is invalid.",
		"The specified session name is invalid."
	},
#endif // #ifdef ERROR_CTX_WINSTATION_NAME_INVALID
#ifdef ERROR_CTX_INVALID_PD
	{	ERROR_CTX_INVALID_PD,                                     "ERROR_CTX_INVALID_PD",
		"The specified protocol driver is invalid.",
		"The specified protocol driver is invalid."
	},
#endif // #ifdef ERROR_CTX_INVALID_PD
#ifdef ERROR_CTX_PD_NOT_FOUND
	{	ERROR_CTX_PD_NOT_FOUND,                                   "ERROR_CTX_PD_NOT_FOUND",
		"The specified protocol driver was not found in the system path.",
		"The specified protocol driver was not found in the system path."
	},
#endif // #ifdef ERROR_CTX_PD_NOT_FOUND
#ifdef ERROR_CTX_WD_NOT_FOUND
	{	ERROR_CTX_WD_NOT_FOUND,                                   "ERROR_CTX_WD_NOT_FOUND",
		"The specified terminal connection driver was not found in the system path.",
		"The specified terminal connection driver was not found in the system path."
	},
#endif // #ifdef ERROR_CTX_WD_NOT_FOUND
#ifdef ERROR_CTX_CANNOT_MAKE_EVENTLOG_ENTRY
	{	ERROR_CTX_CANNOT_MAKE_EVENTLOG_ENTRY,                     "ERROR_CTX_CANNOT_MAKE_EVENTLOG_ENTRY",
		"A registry key for event logging could not be created for this session.",
		"A registry key for event logging could not be created for this session."
	},
#endif // #ifdef ERROR_CTX_CANNOT_MAKE_EVENTLOG_ENTRY
#ifdef ERROR_CTX_SERVICE_NAME_COLLISION
	{	ERROR_CTX_SERVICE_NAME_COLLISION,                         "ERROR_CTX_SERVICE_NAME_COLLISION",
		"A service with the same name already exists on the system.",
		"A service with the same name already exists on the system."
	},
#endif // #ifdef ERROR_CTX_SERVICE_NAME_COLLISION
#ifdef ERROR_CTX_CLOSE_PENDING
	{	ERROR_CTX_CLOSE_PENDING,                                  "ERROR_CTX_CLOSE_PENDING",
		"A close operation is pending on the session.",
		"A close operation is pending on the session."
	},
#endif // #ifdef ERROR_CTX_CLOSE_PENDING
#ifdef ERROR_CTX_NO_OUTBUF
	{	ERROR_CTX_NO_OUTBUF,                                      "ERROR_CTX_NO_OUTBUF",
		"There are no free output buffers available.",
		"There are no free output buffers available."
	},
#endif // #ifdef ERROR_CTX_NO_OUTBUF
#ifdef ERROR_CTX_MODEM_INF_NOT_FOUND
	{	ERROR_CTX_MODEM_INF_NOT_FOUND,                            "ERROR_CTX_MODEM_INF_NOT_FOUND",
		"The MODEM.INF file was not found.",
		"The MODEM.INF file was not found."
	},
#endif // #ifdef ERROR_CTX_MODEM_INF_NOT_FOUND
#ifdef ERROR_CTX_INVALID_MODEMNAME
	{	ERROR_CTX_INVALID_MODEMNAME,                              "ERROR_CTX_INVALID_MODEMNAME",
		"The modem name was not found in MODEM.INF.",
		"The modem name was not found in MODEM.INF."
	},
#endif // #ifdef ERROR_CTX_INVALID_MODEMNAME
#ifdef ERROR_CTX_MODEM_RESPONSE_ERROR
	{	ERROR_CTX_MODEM_RESPONSE_ERROR,                           "ERROR_CTX_MODEM_RESPONSE_ERROR",
		"The modem did not accept the command sent to it. Verify that the configured modem name matches the attached modem.",
		"The modem did not accept the command sent to it. Verify that the configured modem name matches the attached modem."
	},
#endif // #ifdef ERROR_CTX_MODEM_RESPONSE_ERROR
#ifdef ERROR_CTX_MODEM_RESPONSE_TIMEOUT
	{	ERROR_CTX_MODEM_RESPONSE_TIMEOUT,                         "ERROR_CTX_MODEM_RESPONSE_TIMEOUT",
		"The modem did not respond to the command sent to it. Verify that the modem is properly cabled and powered on.",
		"The modem did not respond to the command sent to it. Verify that the modem is properly cabled and powered on."
	},
#endif // #ifdef ERROR_CTX_MODEM_RESPONSE_TIMEOUT
#ifdef ERROR_CTX_MODEM_RESPONSE_NO_CARRIER
	{	ERROR_CTX_MODEM_RESPONSE_NO_CARRIER,                      "ERROR_CTX_MODEM_RESPONSE_NO_CARRIER",
		"Carrier detect has failed or carrier has been dropped due to disconnect.",
		"Carrier detect has failed or carrier has been dropped due to disconnect."
	},
#endif // #ifdef ERROR_CTX_MODEM_RESPONSE_NO_CARRIER
#ifdef ERROR_CTX_MODEM_RESPONSE_NO_DIALTONE
	{	ERROR_CTX_MODEM_RESPONSE_NO_DIALTONE,                     "ERROR_CTX_MODEM_RESPONSE_NO_DIALTONE",
		"Dial tone not detected within the required time. Verify that the phone cable is properly attached and functional.",
		"Dial tone not detected within the required time. Verify that the phone cable is properly attached and functional."
	},
#endif // #ifdef ERROR_CTX_MODEM_RESPONSE_NO_DIALTONE
#ifdef ERROR_CTX_MODEM_RESPONSE_BUSY
	{	ERROR_CTX_MODEM_RESPONSE_BUSY,                            "ERROR_CTX_MODEM_RESPONSE_BUSY",
		"Busy signal detected at remote site on callback.",
		"Busy signal detected at remote site on callback."
	},
#endif // #ifdef ERROR_CTX_MODEM_RESPONSE_BUSY
#ifdef ERROR_CTX_MODEM_RESPONSE_VOICE
	{	ERROR_CTX_MODEM_RESPONSE_VOICE,                           "ERROR_CTX_MODEM_RESPONSE_VOICE",
		"Voice detected at remote site on callback.",
		"Voice detected at remote site on callback."
	},
#endif // #ifdef ERROR_CTX_MODEM_RESPONSE_VOICE
#ifdef ERROR_CTX_TD_ERROR
	{	ERROR_CTX_TD_ERROR,                                       "ERROR_CTX_TD_ERROR",
		"Transport driver error",
		"Transport driver error"
	},
#endif // #ifdef ERROR_CTX_TD_ERROR
#ifdef ERROR_CTX_WINSTATION_NOT_FOUND
	{	ERROR_CTX_WINSTATION_NOT_FOUND,                           "ERROR_CTX_WINSTATION_NOT_FOUND",
		"The specified session cannot be found.",
		"The specified session cannot be found."
	},
#endif // #ifdef ERROR_CTX_WINSTATION_NOT_FOUND
#ifdef ERROR_CTX_WINSTATION_ALREADY_EXISTS
	{	ERROR_CTX_WINSTATION_ALREADY_EXISTS,                      "ERROR_CTX_WINSTATION_ALREADY_EXISTS",
		"The specified session name is already in use.",
		"The specified session name is already in use."
	},
#endif // #ifdef ERROR_CTX_WINSTATION_ALREADY_EXISTS
#ifdef ERROR_CTX_WINSTATION_BUSY
	{	ERROR_CTX_WINSTATION_BUSY,                                "ERROR_CTX_WINSTATION_BUSY",
		"The requested operation cannot be completed because the terminal connection is currently busy processing a connect, disconnect, reset, or delete operation.",
		"The requested operation cannot be completed because the terminal connection is currently busy processing a connect, disconnect, reset, or delete operation."
	},
#endif // #ifdef ERROR_CTX_WINSTATION_BUSY
#ifdef ERROR_CTX_BAD_VIDEO_MODE
	{	ERROR_CTX_BAD_VIDEO_MODE,                                 "ERROR_CTX_BAD_VIDEO_MODE",
		"An attempt has been made to connect to a session whose video mode is not supported by the current client.",
		"An attempt has been made to connect to a session whose video mode is not supported by the current client."
	},
#endif // #ifdef ERROR_CTX_BAD_VIDEO_MODE
#ifdef ERROR_CTX_GRAPHICS_INVALID
	{	ERROR_CTX_GRAPHICS_INVALID,                               "ERROR_CTX_GRAPHICS_INVALID",
		"The application attempted to enable DOS graphics mode. DOS graphics mode is not supported.",
		"The application attempted to enable DOS graphics mode. DOS graphics mode is not supported."
	},
#endif // #ifdef ERROR_CTX_GRAPHICS_INVALID
#ifdef ERROR_CTX_LOGON_DISABLED
	{	ERROR_CTX_LOGON_DISABLED,                                 "ERROR_CTX_LOGON_DISABLED",
		"Your interactive logon privilege has been disabled. Please contact your administrator.",
		"Your interactive logon privilege has been disabled. Please contact your administrator."
	},
#endif // #ifdef ERROR_CTX_LOGON_DISABLED
#ifdef ERROR_CTX_NOT_CONSOLE
	{	ERROR_CTX_NOT_CONSOLE,                                    "ERROR_CTX_NOT_CONSOLE",
		"The requested operation can be performed only on the system console. This is most often the result of a driver or system DLL requiring direct console access.",
		"The requested operation can be performed only on the system console. This is most often the result of a driver or system DLL requiring direct console access."
	},
#endif // #ifdef ERROR_CTX_NOT_CONSOLE
#ifdef ERROR_CTX_CLIENT_QUERY_TIMEOUT
	{	ERROR_CTX_CLIENT_QUERY_TIMEOUT,                           "ERROR_CTX_CLIENT_QUERY_TIMEOUT",
		"The client failed to respond to the server connect message.",
		"The client failed to respond to the server connect message."
	},
#endif // #ifdef ERROR_CTX_CLIENT_QUERY_TIMEOUT
#ifdef ERROR_CTX_CONSOLE_DISCONNECT
	{	ERROR_CTX_CONSOLE_DISCONNECT,                             "ERROR_CTX_CONSOLE_DISCONNECT",
		"Disconnecting the console session is not supported.",
		"Disconnecting the console session is not supported."
	},
#endif // #ifdef ERROR_CTX_CONSOLE_DISCONNECT
#ifdef ERROR_CTX_CONSOLE_CONNECT
	{	ERROR_CTX_CONSOLE_CONNECT,                                "ERROR_CTX_CONSOLE_CONNECT",
		"Reconnecting a disconnected session to the console is not supported.",
		"Reconnecting a disconnected session to the console is not supported."
	},
#endif // #ifdef ERROR_CTX_CONSOLE_CONNECT
#ifdef ERROR_CTX_SHADOW_DENIED
	{	ERROR_CTX_SHADOW_DENIED,                                  "ERROR_CTX_SHADOW_DENIED",
		"The request to control another session remotely was denied.",
		"The request to control another session remotely was denied."
	},
#endif // #ifdef ERROR_CTX_SHADOW_DENIED
#ifdef ERROR_CTX_WINSTATION_ACCESS_DENIED
	{	ERROR_CTX_WINSTATION_ACCESS_DENIED,                       "ERROR_CTX_WINSTATION_ACCESS_DENIED",
		"The requested session access is denied.",
		"The requested session access is denied."
	},
#endif // #ifdef ERROR_CTX_WINSTATION_ACCESS_DENIED
#ifdef ERROR_CTX_INVALID_WD
	{	ERROR_CTX_INVALID_WD,                                     "ERROR_CTX_INVALID_WD",
		"The specified terminal connection driver is invalid.",
		"The specified terminal connection driver is invalid."
	},
#endif // #ifdef ERROR_CTX_INVALID_WD
#ifdef ERROR_CTX_SHADOW_INVALID
	{	ERROR_CTX_SHADOW_INVALID,                                 "ERROR_CTX_SHADOW_INVALID",
		"The requested session cannot be controlled remotely. This may be because the session is disconnected or does not currently have a user logged on.",
		"The requested session cannot be controlled remotely. This may be because the session is disconnected or does not currently have a user logged on."
	},
#endif // #ifdef ERROR_CTX_SHADOW_INVALID
#ifdef ERROR_CTX_SHADOW_DISABLED
	{	ERROR_CTX_SHADOW_DISABLED,                                "ERROR_CTX_SHADOW_DISABLED",
		"The requested session is not configured to allow remote control.",
		"The requested session is not configured to allow remote control."
	},
#endif // #ifdef ERROR_CTX_SHADOW_DISABLED
#ifdef ERROR_CTX_CLIENT_LICENSE_IN_USE
	{	ERROR_CTX_CLIENT_LICENSE_IN_USE,                          "ERROR_CTX_CLIENT_LICENSE_IN_USE",
		"Your request to connect to this Terminal Server has been rejected. Your Terminal Server client license number is currently being used by another user. Please call your system administrator to obtain a unique license number.",
		"Your request to connect to this Terminal Server has been rejected. Your Terminal Server client license number is currently being used by another user. Please call your system administrator to obtain a unique license number."
	},
#endif // #ifdef ERROR_CTX_CLIENT_LICENSE_IN_USE
#ifdef ERROR_CTX_CLIENT_LICENSE_NOT_SET
	{	ERROR_CTX_CLIENT_LICENSE_NOT_SET,                         "ERROR_CTX_CLIENT_LICENSE_NOT_SET",
		"Your request to connect to this Terminal Server has been rejected. Your Terminal Server client license number has not been entered for this copy of the Terminal Server client. Please contact your system administrator.",
		"Your request to connect to this Terminal Server has been rejected. Your Terminal Server client license number has not been entered for this copy of the Terminal Server client. Please contact your system administrator."
	},
#endif // #ifdef ERROR_CTX_CLIENT_LICENSE_NOT_SET
#ifdef ERROR_CTX_LICENSE_NOT_AVAILABLE
	{	ERROR_CTX_LICENSE_NOT_AVAILABLE,                          "ERROR_CTX_LICENSE_NOT_AVAILABLE",
		"The system has reached its licensed logon limit. Please try again later.",
		"The system has reached its licensed logon limit. Please try again later."
	},
#endif // #ifdef ERROR_CTX_LICENSE_NOT_AVAILABLE
#ifdef ERROR_CTX_LICENSE_CLIENT_INVALID
	{	ERROR_CTX_LICENSE_CLIENT_INVALID,                         "ERROR_CTX_LICENSE_CLIENT_INVALID",
		"The client you are using is not licensed to use this system. Your logon request is denied.",
		"The client you are using is not licensed to use this system. Your logon request is denied."
	},
#endif // #ifdef ERROR_CTX_LICENSE_CLIENT_INVALID
#ifdef ERROR_CTX_LICENSE_EXPIRED
	{	ERROR_CTX_LICENSE_EXPIRED,                                "ERROR_CTX_LICENSE_EXPIRED",
		"The system license has expired. Your logon request is denied.",
		"The system license has expired. Your logon request is denied."
	},
#endif // #ifdef ERROR_CTX_LICENSE_EXPIRED
#ifdef ERROR_CTX_SHADOW_NOT_RUNNING
	{	ERROR_CTX_SHADOW_NOT_RUNNING,                             "ERROR_CTX_SHADOW_NOT_RUNNING",
		"Remote control could not be terminated because the specified session is not currently being remotely controlled.",
		"Remote control could not be terminated because the specified session is not currently being remotely controlled."
	},
#endif // #ifdef ERROR_CTX_SHADOW_NOT_RUNNING
#ifdef ERROR_CTX_SHADOW_ENDED_BY_MODE_CHANGE
	{	ERROR_CTX_SHADOW_ENDED_BY_MODE_CHANGE,                    "ERROR_CTX_SHADOW_ENDED_BY_MODE_CHANGE",
		"The remote control of the console was terminated because the display mode was changed. Changing the display mode in a remote control session is not supported.",
		"The remote control of the console was terminated because the display mode was changed. Changing the display mode in a remote control session is not supported."
	},
#endif // #ifdef ERROR_CTX_SHADOW_ENDED_BY_MODE_CHANGE
#ifdef ERROR_ACTIVATION_COUNT_EXCEEDED
	{	ERROR_ACTIVATION_COUNT_EXCEEDED,                          "ERROR_ACTIVATION_COUNT_EXCEEDED",
		"Activation has already been reset the maximum number of times for this installation. Your activation timer will not be cleared.",
		"Activation has already been reset the maximum number of times for this installation. Your activation timer will not be cleared."
	},
#endif // #ifdef ERROR_ACTIVATION_COUNT_EXCEEDED
#ifdef FRS_ERR_INVALID_API_SEQUENCE
	{	FRS_ERR_INVALID_API_SEQUENCE,                             "FRS_ERR_INVALID_API_SEQUENCE",
		"The file replication service API was called incorrectly.",
		"The file replication service API was called incorrectly."
	},
#endif // #ifdef FRS_ERR_INVALID_API_SEQUENCE
#ifdef FRS_ERR_STARTING_SERVICE
	{	FRS_ERR_STARTING_SERVICE,                                 "FRS_ERR_STARTING_SERVICE",
		"The file replication service cannot be started.",
		"The file replication service cannot be started."
	},
#endif // #ifdef FRS_ERR_STARTING_SERVICE
#ifdef FRS_ERR_STOPPING_SERVICE
	{	FRS_ERR_STOPPING_SERVICE,                                 "FRS_ERR_STOPPING_SERVICE",
		"The file replication service cannot be stopped.",
		"The file replication service cannot be stopped."
	},
#endif // #ifdef FRS_ERR_STOPPING_SERVICE
#ifdef FRS_ERR_INTERNAL_API
	{	FRS_ERR_INTERNAL_API,                                     "FRS_ERR_INTERNAL_API",
		"The file replication service API terminated the request. The event log may have more information.",
		"The file replication service API terminated the request. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_INTERNAL_API
#ifdef FRS_ERR_INTERNAL
	{	FRS_ERR_INTERNAL,                                         "FRS_ERR_INTERNAL",
		"The file replication service terminated the request. The event log may have more information.",
		"The file replication service terminated the request. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_INTERNAL
#ifdef FRS_ERR_SERVICE_COMM
	{	FRS_ERR_SERVICE_COMM,                                     "FRS_ERR_SERVICE_COMM",
		"The file replication service cannot be contacted. The event log may have more information.",
		"The file replication service cannot be contacted. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_SERVICE_COMM
#ifdef FRS_ERR_INSUFFICIENT_PRIV
	{	FRS_ERR_INSUFFICIENT_PRIV,                                "FRS_ERR_INSUFFICIENT_PRIV",
		"The file replication service cannot satisfy the request because the user has insufficient privileges. The event log may have more information.",
		"The file replication service cannot satisfy the request because the user has insufficient privileges. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_INSUFFICIENT_PRIV
#ifdef FRS_ERR_AUTHENTICATION
	{	FRS_ERR_AUTHENTICATION,                                   "FRS_ERR_AUTHENTICATION",
		"The file replication service cannot satisfy the request because authenticated RPC is not available. The event log may have more information.",
		"The file replication service cannot satisfy the request because authenticated RPC is not available. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_AUTHENTICATION
#ifdef FRS_ERR_PARENT_INSUFFICIENT_PRIV
	{	FRS_ERR_PARENT_INSUFFICIENT_PRIV,                         "FRS_ERR_PARENT_INSUFFICIENT_PRIV",
		"The file replication service cannot satisfy the request because the user has insufficient privileges on the domain controller. The event log may have more information.",
		"The file replication service cannot satisfy the request because the user has insufficient privileges on the domain controller. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_PARENT_INSUFFICIENT_PRIV
#ifdef FRS_ERR_PARENT_AUTHENTICATION
	{	FRS_ERR_PARENT_AUTHENTICATION,                            "FRS_ERR_PARENT_AUTHENTICATION",
		"The file replication service cannot satisfy the request because authenticated RPC is not available on the domain controller. The event log may have more information.",
		"The file replication service cannot satisfy the request because authenticated RPC is not available on the domain controller. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_PARENT_AUTHENTICATION
#ifdef FRS_ERR_CHILD_TO_PARENT_COMM
	{	FRS_ERR_CHILD_TO_PARENT_COMM,                             "FRS_ERR_CHILD_TO_PARENT_COMM",
		"The file replication service cannot communicate with the file replication service on the domain controller. The event log may have more information.",
		"The file replication service cannot communicate with the file replication service on the domain controller. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_CHILD_TO_PARENT_COMM
#ifdef FRS_ERR_PARENT_TO_CHILD_COMM
	{	FRS_ERR_PARENT_TO_CHILD_COMM,                             "FRS_ERR_PARENT_TO_CHILD_COMM",
		"The file replication service on the domain controller cannot communicate with the file replication service on this computer. The event log may have more information.",
		"The file replication service on the domain controller cannot communicate with the file replication service on this computer. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_PARENT_TO_CHILD_COMM
#ifdef FRS_ERR_SYSVOL_POPULATE
	{	FRS_ERR_SYSVOL_POPULATE,                                  "FRS_ERR_SYSVOL_POPULATE",
		"The file replication service cannot populate the system volume because of an internal error. The event log may have more information.",
		"The file replication service cannot populate the system volume because of an internal error. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_SYSVOL_POPULATE
#ifdef FRS_ERR_SYSVOL_POPULATE_TIMEOUT
	{	FRS_ERR_SYSVOL_POPULATE_TIMEOUT,                          "FRS_ERR_SYSVOL_POPULATE_TIMEOUT",
		"The file replication service cannot populate the system volume because of an internal timeout. The event log may have more information.",
		"The file replication service cannot populate the system volume because of an internal timeout. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_SYSVOL_POPULATE_TIMEOUT
#ifdef FRS_ERR_SYSVOL_IS_BUSY
	{	FRS_ERR_SYSVOL_IS_BUSY,                                   "FRS_ERR_SYSVOL_IS_BUSY",
		"The file replication service cannot process the request. The system volume is busy with a previous request.",
		"The file replication service cannot process the request. The system volume is busy with a previous request."
	},
#endif // #ifdef FRS_ERR_SYSVOL_IS_BUSY
#ifdef FRS_ERR_SYSVOL_DEMOTE
	{	FRS_ERR_SYSVOL_DEMOTE,                                    "FRS_ERR_SYSVOL_DEMOTE",
		"The file replication service cannot stop replicating the system volume because of an internal error. The event log may have more information.",
		"The file replication service cannot stop replicating the system volume because of an internal error. The event log may have more information."
	},
#endif // #ifdef FRS_ERR_SYSVOL_DEMOTE
#ifdef FRS_ERR_INVALID_SERVICE_PARAMETER
	{	FRS_ERR_INVALID_SERVICE_PARAMETER,                        "FRS_ERR_INVALID_SERVICE_PARAMETER",
		"The file replication service detected an invalid parameter.",
		"The file replication service detected an invalid parameter."
	},
#endif // #ifdef FRS_ERR_INVALID_SERVICE_PARAMETER
#ifdef ERROR_DS_NOT_INSTALLED
	{	ERROR_DS_NOT_INSTALLED,                                   "ERROR_DS_NOT_INSTALLED",
		"An error occurred while installing the directory service. For more information, see the event log.",
		"An error occurred while installing the directory service. For more information, see the event log."
	},
#endif // #ifdef ERROR_DS_NOT_INSTALLED
#ifdef ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY
	{	ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY,                    "ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY",
		"The directory service evaluated group memberships locally.",
		"The directory service evaluated group memberships locally."
	},
#endif // #ifdef ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY
#ifdef ERROR_DS_NO_ATTRIBUTE_OR_VALUE
	{	ERROR_DS_NO_ATTRIBUTE_OR_VALUE,                           "ERROR_DS_NO_ATTRIBUTE_OR_VALUE",
		"The specified directory service attribute or value does not exist.",
		"The specified directory service attribute or value does not exist."
	},
#endif // #ifdef ERROR_DS_NO_ATTRIBUTE_OR_VALUE
#ifdef ERROR_DS_INVALID_ATTRIBUTE_SYNTAX
	{	ERROR_DS_INVALID_ATTRIBUTE_SYNTAX,                        "ERROR_DS_INVALID_ATTRIBUTE_SYNTAX",
		"The attribute syntax specified to the directory service is invalid.",
		"The attribute syntax specified to the directory service is invalid."
	},
#endif // #ifdef ERROR_DS_INVALID_ATTRIBUTE_SYNTAX
#ifdef ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED
	{	ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED,                        "ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED",
		"The attribute type specified to the directory service is not defined.",
		"The attribute type specified to the directory service is not defined."
	},
#endif // #ifdef ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED
#ifdef ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS
	{	ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS,                       "ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS",
		"The specified directory service attribute or value already exists.",
		"The specified directory service attribute or value already exists."
	},
#endif // #ifdef ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS
#ifdef ERROR_DS_BUSY
	{	ERROR_DS_BUSY,                                            "ERROR_DS_BUSY",
		"The directory service is busy.",
		"The directory service is busy."
	},
#endif // #ifdef ERROR_DS_BUSY
#ifdef ERROR_DS_UNAVAILABLE
	{	ERROR_DS_UNAVAILABLE,                                     "ERROR_DS_UNAVAILABLE",
		"The directory service is unavailable.",
		"The directory service is unavailable."
	},
#endif // #ifdef ERROR_DS_UNAVAILABLE
#ifdef ERROR_DS_NO_RIDS_ALLOCATED
	{	ERROR_DS_NO_RIDS_ALLOCATED,                               "ERROR_DS_NO_RIDS_ALLOCATED",
		"The directory service was unable to allocate a relative identifier.",
		"The directory service was unable to allocate a relative identifier."
	},
#endif // #ifdef ERROR_DS_NO_RIDS_ALLOCATED
#ifdef ERROR_DS_NO_MORE_RIDS
	{	ERROR_DS_NO_MORE_RIDS,                                    "ERROR_DS_NO_MORE_RIDS",
		"The directory service has exhausted the pool of relative identifiers.",
		"The directory service has exhausted the pool of relative identifiers."
	},
#endif // #ifdef ERROR_DS_NO_MORE_RIDS
#ifdef ERROR_DS_INCORRECT_ROLE_OWNER
	{	ERROR_DS_INCORRECT_ROLE_OWNER,                            "ERROR_DS_INCORRECT_ROLE_OWNER",
		"The requested operation could not be performed because the directory service is not the master for that type of operation.",
		"The requested operation could not be performed because the directory service is not the master for that type of operation."
	},
#endif // #ifdef ERROR_DS_INCORRECT_ROLE_OWNER
#ifdef ERROR_DS_RIDMGR_INIT_ERROR
	{	ERROR_DS_RIDMGR_INIT_ERROR,                               "ERROR_DS_RIDMGR_INIT_ERROR",
		"The directory service was unable to initialize the subsystem that allocates relative identifiers.",
		"The directory service was unable to initialize the subsystem that allocates relative identifiers."
	},
#endif // #ifdef ERROR_DS_RIDMGR_INIT_ERROR
#ifdef ERROR_DS_OBJ_CLASS_VIOLATION
	{	ERROR_DS_OBJ_CLASS_VIOLATION,                             "ERROR_DS_OBJ_CLASS_VIOLATION",
		"The requested operation did not satisfy one or more constraints associated with the class of the object.",
		"The requested operation did not satisfy one or more constraints associated with the class of the object."
	},
#endif // #ifdef ERROR_DS_OBJ_CLASS_VIOLATION
#ifdef ERROR_DS_CANT_ON_NON_LEAF
	{	ERROR_DS_CANT_ON_NON_LEAF,                                "ERROR_DS_CANT_ON_NON_LEAF",
		"The directory service can perform the requested operation only on a leaf object.",
		"The directory service can perform the requested operation only on a leaf object."
	},
#endif // #ifdef ERROR_DS_CANT_ON_NON_LEAF
#ifdef ERROR_DS_CANT_ON_RDN
	{	ERROR_DS_CANT_ON_RDN,                                     "ERROR_DS_CANT_ON_RDN",
		"The directory service cannot perform the requested operation on the RDN attribute of an object.",
		"The directory service cannot perform the requested operation on the RDN attribute of an object."
	},
#endif // #ifdef ERROR_DS_CANT_ON_RDN
#ifdef ERROR_DS_CANT_MOD_OBJ_CLASS
	{	ERROR_DS_CANT_MOD_OBJ_CLASS,                              "ERROR_DS_CANT_MOD_OBJ_CLASS",
		"The directory service detected an attempt to modify the object class of an object.",
		"The directory service detected an attempt to modify the object class of an object."
	},
#endif // #ifdef ERROR_DS_CANT_MOD_OBJ_CLASS
#ifdef ERROR_DS_CROSS_DOM_MOVE_ERROR
	{	ERROR_DS_CROSS_DOM_MOVE_ERROR,                            "ERROR_DS_CROSS_DOM_MOVE_ERROR",
		"The requested cross-domain move operation could not be performed.",
		"The requested cross-domain move operation could not be performed."
	},
#endif // #ifdef ERROR_DS_CROSS_DOM_MOVE_ERROR
#ifdef ERROR_DS_GC_NOT_AVAILABLE
	{	ERROR_DS_GC_NOT_AVAILABLE,                                "ERROR_DS_GC_NOT_AVAILABLE",
		"Unable to contact the global catalog server.",
		"Unable to contact the global catalog server."
	},
#endif // #ifdef ERROR_DS_GC_NOT_AVAILABLE
#ifdef ERROR_SHARED_POLICY
	{	ERROR_SHARED_POLICY,                                      "ERROR_SHARED_POLICY",
		"The policy object is shared and can only be modified at the root.",
		"The policy object is shared and can only be modified at the root."
	},
#endif // #ifdef ERROR_SHARED_POLICY
#ifdef ERROR_POLICY_OBJECT_NOT_FOUND
	{	ERROR_POLICY_OBJECT_NOT_FOUND,                            "ERROR_POLICY_OBJECT_NOT_FOUND",
		"The policy object does not exist.",
		"The policy object does not exist."
	},
#endif // #ifdef ERROR_POLICY_OBJECT_NOT_FOUND
#ifdef ERROR_POLICY_ONLY_IN_DS
	{	ERROR_POLICY_ONLY_IN_DS,                                  "ERROR_POLICY_ONLY_IN_DS",
		"The requested policy information is only in the directory service.",
		"The requested policy information is only in the directory service."
	},
#endif // #ifdef ERROR_POLICY_ONLY_IN_DS
#ifdef ERROR_PROMOTION_ACTIVE
	{	ERROR_PROMOTION_ACTIVE,                                   "ERROR_PROMOTION_ACTIVE",
		"A domain controller promotion is currently active.",
		"A domain controller promotion is currently active."
	},
#endif // #ifdef ERROR_PROMOTION_ACTIVE
#ifdef ERROR_NO_PROMOTION_ACTIVE
	{	ERROR_NO_PROMOTION_ACTIVE,                                "ERROR_NO_PROMOTION_ACTIVE",
		"A domain controller promotion is not currently active",
		"A domain controller promotion is not currently active"
	},
#endif // #ifdef ERROR_NO_PROMOTION_ACTIVE
#ifdef ERROR_DS_OPERATIONS_ERROR
	{	ERROR_DS_OPERATIONS_ERROR,                                "ERROR_DS_OPERATIONS_ERROR",
		"An operations error occurred.",
		"An operations error occurred."
	},
#endif // #ifdef ERROR_DS_OPERATIONS_ERROR
#ifdef ERROR_DS_PROTOCOL_ERROR
	{	ERROR_DS_PROTOCOL_ERROR,                                  "ERROR_DS_PROTOCOL_ERROR",
		"A protocol error occurred.",
		"A protocol error occurred."
	},
#endif // #ifdef ERROR_DS_PROTOCOL_ERROR
#ifdef ERROR_DS_TIMELIMIT_EXCEEDED
	{	ERROR_DS_TIMELIMIT_EXCEEDED,                              "ERROR_DS_TIMELIMIT_EXCEEDED",
		"The time limit for this request was exceeded.",
		"The time limit for this request was exceeded."
	},
#endif // #ifdef ERROR_DS_TIMELIMIT_EXCEEDED
#ifdef ERROR_DS_SIZELIMIT_EXCEEDED
	{	ERROR_DS_SIZELIMIT_EXCEEDED,                              "ERROR_DS_SIZELIMIT_EXCEEDED",
		"The size limit for this request was exceeded.",
		"The size limit for this request was exceeded."
	},
#endif // #ifdef ERROR_DS_SIZELIMIT_EXCEEDED
#ifdef ERROR_DS_ADMIN_LIMIT_EXCEEDED
	{	ERROR_DS_ADMIN_LIMIT_EXCEEDED,                            "ERROR_DS_ADMIN_LIMIT_EXCEEDED",
		"The administrative limit for this request was exceeded.",
		"The administrative limit for this request was exceeded."
	},
#endif // #ifdef ERROR_DS_ADMIN_LIMIT_EXCEEDED
#ifdef ERROR_DS_COMPARE_FALSE
	{	ERROR_DS_COMPARE_FALSE,                                   "ERROR_DS_COMPARE_FALSE",
		"The compare response was false.",
		"The compare response was false."
	},
#endif // #ifdef ERROR_DS_COMPARE_FALSE
#ifdef ERROR_DS_COMPARE_TRUE
	{	ERROR_DS_COMPARE_TRUE,                                    "ERROR_DS_COMPARE_TRUE",
		"The compare response was true.",
		"The compare response was true."
	},
#endif // #ifdef ERROR_DS_COMPARE_TRUE
#ifdef ERROR_DS_AUTH_METHOD_NOT_SUPPORTED
	{	ERROR_DS_AUTH_METHOD_NOT_SUPPORTED,                       "ERROR_DS_AUTH_METHOD_NOT_SUPPORTED",
		"The requested authentication method is not supported by the server.",
		"The requested authentication method is not supported by the server."
	},
#endif // #ifdef ERROR_DS_AUTH_METHOD_NOT_SUPPORTED
#ifdef ERROR_DS_STRONG_AUTH_REQUIRED
	{	ERROR_DS_STRONG_AUTH_REQUIRED,                            "ERROR_DS_STRONG_AUTH_REQUIRED",
		"A more secure authentication method is required for this server.",
		"A more secure authentication method is required for this server."
	},
#endif // #ifdef ERROR_DS_STRONG_AUTH_REQUIRED
#ifdef ERROR_DS_INAPPROPRIATE_AUTH
	{	ERROR_DS_INAPPROPRIATE_AUTH,                              "ERROR_DS_INAPPROPRIATE_AUTH",
		"Inappropriate authentication.",
		"Inappropriate authentication."
	},
#endif // #ifdef ERROR_DS_INAPPROPRIATE_AUTH
#ifdef ERROR_DS_AUTH_UNKNOWN
	{	ERROR_DS_AUTH_UNKNOWN,                                    "ERROR_DS_AUTH_UNKNOWN",
		"The authentication mechanism is unknown.",
		"The authentication mechanism is unknown."
	},
#endif // #ifdef ERROR_DS_AUTH_UNKNOWN
#ifdef ERROR_DS_REFERRAL
	{	ERROR_DS_REFERRAL,                                        "ERROR_DS_REFERRAL",
		"A referral was returned from the server.",
		"A referral was returned from the server."
	},
#endif // #ifdef ERROR_DS_REFERRAL
#ifdef ERROR_DS_UNAVAILABLE_CRIT_EXTENSION
	{	ERROR_DS_UNAVAILABLE_CRIT_EXTENSION,                      "ERROR_DS_UNAVAILABLE_CRIT_EXTENSION",
		"The server does not support the requested critical extension.",
		"The server does not support the requested critical extension."
	},
#endif // #ifdef ERROR_DS_UNAVAILABLE_CRIT_EXTENSION
#ifdef ERROR_DS_CONFIDENTIALITY_REQUIRED
	{	ERROR_DS_CONFIDENTIALITY_REQUIRED,                        "ERROR_DS_CONFIDENTIALITY_REQUIRED",
		"This request requires a secure connection.",
		"This request requires a secure connection."
	},
#endif // #ifdef ERROR_DS_CONFIDENTIALITY_REQUIRED
#ifdef ERROR_DS_INAPPROPRIATE_MATCHING
	{	ERROR_DS_INAPPROPRIATE_MATCHING,                          "ERROR_DS_INAPPROPRIATE_MATCHING",
		"Inappropriate matching.",
		"Inappropriate matching."
	},
#endif // #ifdef ERROR_DS_INAPPROPRIATE_MATCHING
#ifdef ERROR_DS_CONSTRAINT_VIOLATION
	{	ERROR_DS_CONSTRAINT_VIOLATION,                            "ERROR_DS_CONSTRAINT_VIOLATION",
		"A constraint violation occurred.",
		"A constraint violation occurred."
	},
#endif // #ifdef ERROR_DS_CONSTRAINT_VIOLATION
#ifdef ERROR_DS_NO_SUCH_OBJECT
	{	ERROR_DS_NO_SUCH_OBJECT,                                  "ERROR_DS_NO_SUCH_OBJECT",
		"There is no such object on the server.",
		"There is no such object on the server."
	},
#endif // #ifdef ERROR_DS_NO_SUCH_OBJECT
#ifdef ERROR_DS_ALIAS_PROBLEM
	{	ERROR_DS_ALIAS_PROBLEM,                                   "ERROR_DS_ALIAS_PROBLEM",
		"There is an alias problem.",
		"There is an alias problem."
	},
#endif // #ifdef ERROR_DS_ALIAS_PROBLEM
#ifdef ERROR_DS_INVALID_DN_SYNTAX
	{	ERROR_DS_INVALID_DN_SYNTAX,                               "ERROR_DS_INVALID_DN_SYNTAX",
		"An invalid dn syntax has been specified.",
		"An invalid dn syntax has been specified."
	},
#endif // #ifdef ERROR_DS_INVALID_DN_SYNTAX
#ifdef ERROR_DS_IS_LEAF
	{	ERROR_DS_IS_LEAF,                                         "ERROR_DS_IS_LEAF",
		"The object is a leaf object.",
		"The object is a leaf object."
	},
#endif // #ifdef ERROR_DS_IS_LEAF
#ifdef ERROR_DS_ALIAS_DEREF_PROBLEM
	{	ERROR_DS_ALIAS_DEREF_PROBLEM,                             "ERROR_DS_ALIAS_DEREF_PROBLEM",
		"There is an alias dereferencing problem.",
		"There is an alias dereferencing problem."
	},
#endif // #ifdef ERROR_DS_ALIAS_DEREF_PROBLEM
#ifdef ERROR_DS_UNWILLING_TO_PERFORM
	{	ERROR_DS_UNWILLING_TO_PERFORM,                            "ERROR_DS_UNWILLING_TO_PERFORM",
		"The server is unwilling to process the request.",
		"The server is unwilling to process the request."
	},
#endif // #ifdef ERROR_DS_UNWILLING_TO_PERFORM
#ifdef ERROR_DS_LOOP_DETECT
	{	ERROR_DS_LOOP_DETECT,                                     "ERROR_DS_LOOP_DETECT",
		"A loop has been detected.",
		"A loop has been detected."
	},
#endif // #ifdef ERROR_DS_LOOP_DETECT
#ifdef ERROR_DS_NAMING_VIOLATION
	{	ERROR_DS_NAMING_VIOLATION,                                "ERROR_DS_NAMING_VIOLATION",
		"There is a naming violation.",
		"There is a naming violation."
	},
#endif // #ifdef ERROR_DS_NAMING_VIOLATION
#ifdef ERROR_DS_OBJECT_RESULTS_TOO_LARGE
	{	ERROR_DS_OBJECT_RESULTS_TOO_LARGE,                        "ERROR_DS_OBJECT_RESULTS_TOO_LARGE",
		"The result set is too large.",
		"The result set is too large."
	},
#endif // #ifdef ERROR_DS_OBJECT_RESULTS_TOO_LARGE
#ifdef ERROR_DS_AFFECTS_MULTIPLE_DSAS
	{	ERROR_DS_AFFECTS_MULTIPLE_DSAS,                           "ERROR_DS_AFFECTS_MULTIPLE_DSAS",
		"The operation affects multiple DSAs",
		"The operation affects multiple DSAs"
	},
#endif // #ifdef ERROR_DS_AFFECTS_MULTIPLE_DSAS
#ifdef ERROR_DS_SERVER_DOWN
	{	ERROR_DS_SERVER_DOWN,                                     "ERROR_DS_SERVER_DOWN",
		"The server is not operational.",
		"The server is not operational."
	},
#endif // #ifdef ERROR_DS_SERVER_DOWN
#ifdef ERROR_DS_LOCAL_ERROR
	{	ERROR_DS_LOCAL_ERROR,                                     "ERROR_DS_LOCAL_ERROR",
		"A local error has occurred.",
		"A local error has occurred."
	},
#endif // #ifdef ERROR_DS_LOCAL_ERROR
#ifdef ERROR_DS_ENCODING_ERROR
	{	ERROR_DS_ENCODING_ERROR,                                  "ERROR_DS_ENCODING_ERROR",
		"An encoding error has occurred.",
		"An encoding error has occurred."
	},
#endif // #ifdef ERROR_DS_ENCODING_ERROR
#ifdef ERROR_DS_DECODING_ERROR
	{	ERROR_DS_DECODING_ERROR,                                  "ERROR_DS_DECODING_ERROR",
		"A decoding error has occurred.",
		"A decoding error has occurred."
	},
#endif // #ifdef ERROR_DS_DECODING_ERROR
#ifdef ERROR_DS_FILTER_UNKNOWN
	{	ERROR_DS_FILTER_UNKNOWN,                                  "ERROR_DS_FILTER_UNKNOWN",
		"The search filter cannot be recognized.",
		"The search filter cannot be recognized."
	},
#endif // #ifdef ERROR_DS_FILTER_UNKNOWN
#ifdef ERROR_DS_PARAM_ERROR
	{	ERROR_DS_PARAM_ERROR,                                     "ERROR_DS_PARAM_ERROR",
		"One or more parameters are illegal.",
		"One or more parameters are illegal."
	},
#endif // #ifdef ERROR_DS_PARAM_ERROR
#ifdef ERROR_DS_NOT_SUPPORTED
	{	ERROR_DS_NOT_SUPPORTED,                                   "ERROR_DS_NOT_SUPPORTED",
		"The specified method is not supported.",
		"The specified method is not supported."
	},
#endif // #ifdef ERROR_DS_NOT_SUPPORTED
#ifdef ERROR_DS_NO_RESULTS_RETURNED
	{	ERROR_DS_NO_RESULTS_RETURNED,                             "ERROR_DS_NO_RESULTS_RETURNED",
		"No results were returned.",
		"No results were returned."
	},
#endif // #ifdef ERROR_DS_NO_RESULTS_RETURNED
#ifdef ERROR_DS_CONTROL_NOT_FOUND
	{	ERROR_DS_CONTROL_NOT_FOUND,                               "ERROR_DS_CONTROL_NOT_FOUND",
		"The specified control is not supported by the server.",
		"The specified control is not supported by the server."
	},
#endif // #ifdef ERROR_DS_CONTROL_NOT_FOUND
#ifdef ERROR_DS_CLIENT_LOOP
	{	ERROR_DS_CLIENT_LOOP,                                     "ERROR_DS_CLIENT_LOOP",
		"A referral loop was detected by the client.",
		"A referral loop was detected by the client."
	},
#endif // #ifdef ERROR_DS_CLIENT_LOOP
#ifdef ERROR_DS_REFERRAL_LIMIT_EXCEEDED
	{	ERROR_DS_REFERRAL_LIMIT_EXCEEDED,                         "ERROR_DS_REFERRAL_LIMIT_EXCEEDED",
		"The preset referral limit was exceeded.",
		"The preset referral limit was exceeded."
	},
#endif // #ifdef ERROR_DS_REFERRAL_LIMIT_EXCEEDED
#ifdef ERROR_DS_SORT_CONTROL_MISSING
	{	ERROR_DS_SORT_CONTROL_MISSING,                            "ERROR_DS_SORT_CONTROL_MISSING",
		"The search requires a SORT control.",
		"The search requires a SORT control."
	},
#endif // #ifdef ERROR_DS_SORT_CONTROL_MISSING
#ifdef ERROR_DS_OFFSET_RANGE_ERROR
	{	ERROR_DS_OFFSET_RANGE_ERROR,                              "ERROR_DS_OFFSET_RANGE_ERROR",
		"The search results exceed the offset range specified.",
		"The search results exceed the offset range specified."
	},
#endif // #ifdef ERROR_DS_OFFSET_RANGE_ERROR
#ifdef ERROR_DS_ROOT_MUST_BE_NC
	{	ERROR_DS_ROOT_MUST_BE_NC,                                 "ERROR_DS_ROOT_MUST_BE_NC",
		"The root object must be the head of a naming context. The root object cannot have an instantiated parent.",
		"The root object must be the head of a naming context. The root object cannot have an instantiated parent."
	},
#endif // #ifdef ERROR_DS_ROOT_MUST_BE_NC
#ifdef ERROR_DS_ADD_REPLICA_INHIBITED
	{	ERROR_DS_ADD_REPLICA_INHIBITED,                           "ERROR_DS_ADD_REPLICA_INHIBITED",
		"The add replica operation cannot be performed. The naming context must be writeable in order to create the replica.",
		"The add replica operation cannot be performed. The naming context must be writeable in order to create the replica."
	},
#endif // #ifdef ERROR_DS_ADD_REPLICA_INHIBITED
#ifdef ERROR_DS_ATT_NOT_DEF_IN_SCHEMA
	{	ERROR_DS_ATT_NOT_DEF_IN_SCHEMA,                           "ERROR_DS_ATT_NOT_DEF_IN_SCHEMA",
		"A reference to an attribute that is not defined in the schema occurred.",
		"A reference to an attribute that is not defined in the schema occurred."
	},
#endif // #ifdef ERROR_DS_ATT_NOT_DEF_IN_SCHEMA
#ifdef ERROR_DS_MAX_OBJ_SIZE_EXCEEDED
	{	ERROR_DS_MAX_OBJ_SIZE_EXCEEDED,                           "ERROR_DS_MAX_OBJ_SIZE_EXCEEDED",
		"The maximum size of an object has been exceeded.",
		"The maximum size of an object has been exceeded."
	},
#endif // #ifdef ERROR_DS_MAX_OBJ_SIZE_EXCEEDED
#ifdef ERROR_DS_OBJ_STRING_NAME_EXISTS
	{	ERROR_DS_OBJ_STRING_NAME_EXISTS,                          "ERROR_DS_OBJ_STRING_NAME_EXISTS",
		"An attempt was made to add an object to the directory with a name that is already in use.",
		"An attempt was made to add an object to the directory with a name that is already in use."
	},
#endif // #ifdef ERROR_DS_OBJ_STRING_NAME_EXISTS
#ifdef ERROR_DS_NO_RDN_DEFINED_IN_SCHEMA
	{	ERROR_DS_NO_RDN_DEFINED_IN_SCHEMA,                        "ERROR_DS_NO_RDN_DEFINED_IN_SCHEMA",
		"An attempt was made to add an object of a class that does not have an RDN defined in the schema.",
		"An attempt was made to add an object of a class that does not have an RDN defined in the schema."
	},
#endif // #ifdef ERROR_DS_NO_RDN_DEFINED_IN_SCHEMA
#ifdef ERROR_DS_RDN_DOESNT_MATCH_SCHEMA
	{	ERROR_DS_RDN_DOESNT_MATCH_SCHEMA,                         "ERROR_DS_RDN_DOESNT_MATCH_SCHEMA",
		"An attempt was made to add an object using an RDN that is not the RDN defined in the schema.",
		"An attempt was made to add an object using an RDN that is not the RDN defined in the schema."
	},
#endif // #ifdef ERROR_DS_RDN_DOESNT_MATCH_SCHEMA
#ifdef ERROR_DS_NO_REQUESTED_ATTS_FOUND
	{	ERROR_DS_NO_REQUESTED_ATTS_FOUND,                         "ERROR_DS_NO_REQUESTED_ATTS_FOUND",
		"None of the requested attributes were found on the objects.",
		"None of the requested attributes were found on the objects."
	},
#endif // #ifdef ERROR_DS_NO_REQUESTED_ATTS_FOUND
#ifdef ERROR_DS_USER_BUFFER_TO_SMALL
	{	ERROR_DS_USER_BUFFER_TO_SMALL,                            "ERROR_DS_USER_BUFFER_TO_SMALL",
		"The user buffer is too small.",
		"The user buffer is too small."
	},
#endif // #ifdef ERROR_DS_USER_BUFFER_TO_SMALL
#ifdef ERROR_DS_ATT_IS_NOT_ON_OBJ
	{	ERROR_DS_ATT_IS_NOT_ON_OBJ,                               "ERROR_DS_ATT_IS_NOT_ON_OBJ",
		"The attribute specified in the operation is not present on the object.",
		"The attribute specified in the operation is not present on the object."
	},
#endif // #ifdef ERROR_DS_ATT_IS_NOT_ON_OBJ
#ifdef ERROR_DS_ILLEGAL_MOD_OPERATION
	{	ERROR_DS_ILLEGAL_MOD_OPERATION,                           "ERROR_DS_ILLEGAL_MOD_OPERATION",
		"Illegal modify operation. Some aspect of the modification is not permitted.",
		"Illegal modify operation. Some aspect of the modification is not permitted."
	},
#endif // #ifdef ERROR_DS_ILLEGAL_MOD_OPERATION
#ifdef ERROR_DS_OBJ_TOO_LARGE
	{	ERROR_DS_OBJ_TOO_LARGE,                                   "ERROR_DS_OBJ_TOO_LARGE",
		"The specified object is too large.",
		"The specified object is too large."
	},
#endif // #ifdef ERROR_DS_OBJ_TOO_LARGE
#ifdef ERROR_DS_BAD_INSTANCE_TYPE
	{	ERROR_DS_BAD_INSTANCE_TYPE,                               "ERROR_DS_BAD_INSTANCE_TYPE",
		"The specified instance type is not valid.",
		"The specified instance type is not valid."
	},
#endif // #ifdef ERROR_DS_BAD_INSTANCE_TYPE
#ifdef ERROR_DS_MASTERDSA_REQUIRED
	{	ERROR_DS_MASTERDSA_REQUIRED,                              "ERROR_DS_MASTERDSA_REQUIRED",
		"The operation must be performed at a master DSA.",
		"The operation must be performed at a master DSA."
	},
#endif // #ifdef ERROR_DS_MASTERDSA_REQUIRED
#ifdef ERROR_DS_OBJECT_CLASS_REQUIRED
	{	ERROR_DS_OBJECT_CLASS_REQUIRED,                           "ERROR_DS_OBJECT_CLASS_REQUIRED",
		"The object class attribute must be specified.",
		"The object class attribute must be specified."
	},
#endif // #ifdef ERROR_DS_OBJECT_CLASS_REQUIRED
#ifdef ERROR_DS_MISSING_REQUIRED_ATT
	{	ERROR_DS_MISSING_REQUIRED_ATT,                            "ERROR_DS_MISSING_REQUIRED_ATT",
		"A required attribute is missing.",
		"A required attribute is missing."
	},
#endif // #ifdef ERROR_DS_MISSING_REQUIRED_ATT
#ifdef ERROR_DS_ATT_NOT_DEF_FOR_CLASS
	{	ERROR_DS_ATT_NOT_DEF_FOR_CLASS,                           "ERROR_DS_ATT_NOT_DEF_FOR_CLASS",
		"An attempt was made to modify an object to include an attribute that is not legal for its class",
		"An attempt was made to modify an object to include an attribute that is not legal for its class"
	},
#endif // #ifdef ERROR_DS_ATT_NOT_DEF_FOR_CLASS
#ifdef ERROR_DS_ATT_ALREADY_EXISTS
	{	ERROR_DS_ATT_ALREADY_EXISTS,                              "ERROR_DS_ATT_ALREADY_EXISTS",
		"The specified attribute is already present on the object.",
		"The specified attribute is already present on the object."
	},
#endif // #ifdef ERROR_DS_ATT_ALREADY_EXISTS
#ifdef ERROR_DS_CANT_ADD_ATT_VALUES
	{	ERROR_DS_CANT_ADD_ATT_VALUES,                             "ERROR_DS_CANT_ADD_ATT_VALUES",
		"The specified attribute is not present, or has no values.",
		"The specified attribute is not present, or has no values."
	},
#endif // #ifdef ERROR_DS_CANT_ADD_ATT_VALUES
#ifdef ERROR_DS_SINGLE_VALUE_CONSTRAINT
	{	ERROR_DS_SINGLE_VALUE_CONSTRAINT,                         "ERROR_DS_SINGLE_VALUE_CONSTRAINT",
		"Multiple values were specified for an attribute that can have only one value.",
		"Multiple values were specified for an attribute that can have only one value."
	},
#endif // #ifdef ERROR_DS_SINGLE_VALUE_CONSTRAINT
#ifdef ERROR_DS_RANGE_CONSTRAINT
	{	ERROR_DS_RANGE_CONSTRAINT,                                "ERROR_DS_RANGE_CONSTRAINT",
		"A value for the attribute was not in the acceptable range of values.",
		"A value for the attribute was not in the acceptable range of values."
	},
#endif // #ifdef ERROR_DS_RANGE_CONSTRAINT
#ifdef ERROR_DS_ATT_VAL_ALREADY_EXISTS
	{	ERROR_DS_ATT_VAL_ALREADY_EXISTS,                          "ERROR_DS_ATT_VAL_ALREADY_EXISTS",
		"The specified value already exists.",
		"The specified value already exists."
	},
#endif // #ifdef ERROR_DS_ATT_VAL_ALREADY_EXISTS
#ifdef ERROR_DS_CANT_REM_MISSING_ATT
	{	ERROR_DS_CANT_REM_MISSING_ATT,                            "ERROR_DS_CANT_REM_MISSING_ATT",
		"The attribute cannot be removed because it is not present on the object.",
		"The attribute cannot be removed because it is not present on the object."
	},
#endif // #ifdef ERROR_DS_CANT_REM_MISSING_ATT
#ifdef ERROR_DS_CANT_REM_MISSING_ATT_VAL
	{	ERROR_DS_CANT_REM_MISSING_ATT_VAL,                        "ERROR_DS_CANT_REM_MISSING_ATT_VAL",
		"The attribute value cannot be removed because it is not present on the object.",
		"The attribute value cannot be removed because it is not present on the object."
	},
#endif // #ifdef ERROR_DS_CANT_REM_MISSING_ATT_VAL
#ifdef ERROR_DS_ROOT_CANT_BE_SUBREF
	{	ERROR_DS_ROOT_CANT_BE_SUBREF,                             "ERROR_DS_ROOT_CANT_BE_SUBREF",
		"The specified root object cannot be a subref.",
		"The specified root object cannot be a subref."
	},
#endif // #ifdef ERROR_DS_ROOT_CANT_BE_SUBREF
#ifdef ERROR_DS_NO_CHAINING
	{	ERROR_DS_NO_CHAINING,                                     "ERROR_DS_NO_CHAINING",
		"Chaining is not permitted.",
		"Chaining is not permitted."
	},
#endif // #ifdef ERROR_DS_NO_CHAINING
#ifdef ERROR_DS_NO_CHAINED_EVAL
	{	ERROR_DS_NO_CHAINED_EVAL,                                 "ERROR_DS_NO_CHAINED_EVAL",
		"Chained evaluation is not permitted.",
		"Chained evaluation is not permitted."
	},
#endif // #ifdef ERROR_DS_NO_CHAINED_EVAL
#ifdef ERROR_DS_NO_PARENT_OBJECT
	{	ERROR_DS_NO_PARENT_OBJECT,                                "ERROR_DS_NO_PARENT_OBJECT",
		"The operation could not be performed because the object's parent is either uninstantiated or deleted.",
		"The operation could not be performed because the object's parent is either uninstantiated or deleted."
	},
#endif // #ifdef ERROR_DS_NO_PARENT_OBJECT
#ifdef ERROR_DS_PARENT_IS_AN_ALIAS
	{	ERROR_DS_PARENT_IS_AN_ALIAS,                              "ERROR_DS_PARENT_IS_AN_ALIAS",
		"Having a parent that is an alias is not permitted. Aliases are leaf objects.",
		"Having a parent that is an alias is not permitted. Aliases are leaf objects."
	},
#endif // #ifdef ERROR_DS_PARENT_IS_AN_ALIAS
#ifdef ERROR_DS_CANT_MIX_MASTER_AND_REPS
	{	ERROR_DS_CANT_MIX_MASTER_AND_REPS,                        "ERROR_DS_CANT_MIX_MASTER_AND_REPS",
		"The object and parent must be of the same type, either both masters or both replicas.",
		"The object and parent must be of the same type, either both masters or both replicas."
	},
#endif // #ifdef ERROR_DS_CANT_MIX_MASTER_AND_REPS
#ifdef ERROR_DS_CHILDREN_EXIST
	{	ERROR_DS_CHILDREN_EXIST,                                  "ERROR_DS_CHILDREN_EXIST",
		"The operation cannot be performed because child objects exist. This operation can only be performed on a leaf object.",
		"The operation cannot be performed because child objects exist. This operation can only be performed on a leaf object."
	},
#endif // #ifdef ERROR_DS_CHILDREN_EXIST
#ifdef ERROR_DS_OBJ_NOT_FOUND
	{	ERROR_DS_OBJ_NOT_FOUND,                                   "ERROR_DS_OBJ_NOT_FOUND",
		"Directory object not found.",
		"Directory object not found."
	},
#endif // #ifdef ERROR_DS_OBJ_NOT_FOUND
#ifdef ERROR_DS_ALIASED_OBJ_MISSING
	{	ERROR_DS_ALIASED_OBJ_MISSING,                             "ERROR_DS_ALIASED_OBJ_MISSING",
		"The aliased object is missing.",
		"The aliased object is missing."
	},
#endif // #ifdef ERROR_DS_ALIASED_OBJ_MISSING
#ifdef ERROR_DS_BAD_NAME_SYNTAX
	{	ERROR_DS_BAD_NAME_SYNTAX,                                 "ERROR_DS_BAD_NAME_SYNTAX",
		"The object name has bad syntax.",
		"The object name has bad syntax."
	},
#endif // #ifdef ERROR_DS_BAD_NAME_SYNTAX
#ifdef ERROR_DS_ALIAS_POINTS_TO_ALIAS
	{	ERROR_DS_ALIAS_POINTS_TO_ALIAS,                           "ERROR_DS_ALIAS_POINTS_TO_ALIAS",
		"It is not permitted for an alias to refer to another alias.",
		"It is not permitted for an alias to refer to another alias."
	},
#endif // #ifdef ERROR_DS_ALIAS_POINTS_TO_ALIAS
#ifdef ERROR_DS_CANT_DEREF_ALIAS
	{	ERROR_DS_CANT_DEREF_ALIAS,                                "ERROR_DS_CANT_DEREF_ALIAS",
		"The alias cannot be dereferenced.",
		"The alias cannot be dereferenced."
	},
#endif // #ifdef ERROR_DS_CANT_DEREF_ALIAS
#ifdef ERROR_DS_OUT_OF_SCOPE
	{	ERROR_DS_OUT_OF_SCOPE,                                    "ERROR_DS_OUT_OF_SCOPE",
		"The operation is out of scope.",
		"The operation is out of scope."
	},
#endif // #ifdef ERROR_DS_OUT_OF_SCOPE
#ifdef ERROR_DS_OBJECT_BEING_REMOVED
	{	ERROR_DS_OBJECT_BEING_REMOVED,                            "ERROR_DS_OBJECT_BEING_REMOVED",
		"The operation cannot continue because the object is in the process of being removed.",
		"The operation cannot continue because the object is in the process of being removed."
	},
#endif // #ifdef ERROR_DS_OBJECT_BEING_REMOVED
#ifdef ERROR_DS_CANT_DELETE_DSA_OBJ
	{	ERROR_DS_CANT_DELETE_DSA_OBJ,                             "ERROR_DS_CANT_DELETE_DSA_OBJ",
		"The DSA object cannot be deleted.",
		"The DSA object cannot be deleted."
	},
#endif // #ifdef ERROR_DS_CANT_DELETE_DSA_OBJ
#ifdef ERROR_DS_GENERIC_ERROR
	{	ERROR_DS_GENERIC_ERROR,                                   "ERROR_DS_GENERIC_ERROR",
		"A directory service error has occurred.",
		"A directory service error has occurred."
	},
#endif // #ifdef ERROR_DS_GENERIC_ERROR
#ifdef ERROR_DS_DSA_MUST_BE_INT_MASTER
	{	ERROR_DS_DSA_MUST_BE_INT_MASTER,                          "ERROR_DS_DSA_MUST_BE_INT_MASTER",
		"The operation can only be performed on an internal master DSA object.",
		"The operation can only be performed on an internal master DSA object."
	},
#endif // #ifdef ERROR_DS_DSA_MUST_BE_INT_MASTER
#ifdef ERROR_DS_CLASS_NOT_DSA
	{	ERROR_DS_CLASS_NOT_DSA,                                   "ERROR_DS_CLASS_NOT_DSA",
		"The object must be of class DSA.",
		"The object must be of class DSA."
	},
#endif // #ifdef ERROR_DS_CLASS_NOT_DSA
#ifdef ERROR_DS_INSUFF_ACCESS_RIGHTS
	{	ERROR_DS_INSUFF_ACCESS_RIGHTS,                            "ERROR_DS_INSUFF_ACCESS_RIGHTS",
		"Insufficient access rights to perform the operation.",
		"Insufficient access rights to perform the operation."
	},
#endif // #ifdef ERROR_DS_INSUFF_ACCESS_RIGHTS
#ifdef ERROR_DS_ILLEGAL_SUPERIOR
	{	ERROR_DS_ILLEGAL_SUPERIOR,                                "ERROR_DS_ILLEGAL_SUPERIOR",
		"The object cannot be added because the parent is not on the list of possible superiors.",
		"The object cannot be added because the parent is not on the list of possible superiors."
	},
#endif // #ifdef ERROR_DS_ILLEGAL_SUPERIOR
#ifdef ERROR_DS_ATTRIBUTE_OWNED_BY_SAM
	{	ERROR_DS_ATTRIBUTE_OWNED_BY_SAM,                          "ERROR_DS_ATTRIBUTE_OWNED_BY_SAM",
		"Access to the attribute is not permitted because the attribute is owned by the Security Accounts Manager (SAM).",
		"Access to the attribute is not permitted because the attribute is owned by the Security Accounts Manager (SAM)."
	},
#endif // #ifdef ERROR_DS_ATTRIBUTE_OWNED_BY_SAM
#ifdef ERROR_DS_NAME_TOO_MANY_PARTS
	{	ERROR_DS_NAME_TOO_MANY_PARTS,                             "ERROR_DS_NAME_TOO_MANY_PARTS",
		"The name has too many parts.",
		"The name has too many parts."
	},
#endif // #ifdef ERROR_DS_NAME_TOO_MANY_PARTS
#ifdef ERROR_DS_NAME_TOO_LONG
	{	ERROR_DS_NAME_TOO_LONG,                                   "ERROR_DS_NAME_TOO_LONG",
		"The name is too long.",
		"The name is too long."
	},
#endif // #ifdef ERROR_DS_NAME_TOO_LONG
#ifdef ERROR_DS_NAME_VALUE_TOO_LONG
	{	ERROR_DS_NAME_VALUE_TOO_LONG,                             "ERROR_DS_NAME_VALUE_TOO_LONG",
		"The name value is too long.",
		"The name value is too long."
	},
#endif // #ifdef ERROR_DS_NAME_VALUE_TOO_LONG
#ifdef ERROR_DS_NAME_UNPARSEABLE
	{	ERROR_DS_NAME_UNPARSEABLE,                                "ERROR_DS_NAME_UNPARSEABLE",
		"The directory service encountered an error parsing a name.",
		"The directory service encountered an error parsing a name."
	},
#endif // #ifdef ERROR_DS_NAME_UNPARSEABLE
#ifdef ERROR_DS_NAME_TYPE_UNKNOWN
	{	ERROR_DS_NAME_TYPE_UNKNOWN,                               "ERROR_DS_NAME_TYPE_UNKNOWN",
		"The directory service cannot get the attribute type for a name.",
		"The directory service cannot get the attribute type for a name."
	},
#endif // #ifdef ERROR_DS_NAME_TYPE_UNKNOWN
#ifdef ERROR_DS_NOT_AN_OBJECT
	{	ERROR_DS_NOT_AN_OBJECT,                                   "ERROR_DS_NOT_AN_OBJECT",
		"The name does not identify an object; the name identifies a phantom.",
		"The name does not identify an object; the name identifies a phantom."
	},
#endif // #ifdef ERROR_DS_NOT_AN_OBJECT
#ifdef ERROR_DS_SEC_DESC_TOO_SHORT
	{	ERROR_DS_SEC_DESC_TOO_SHORT,                              "ERROR_DS_SEC_DESC_TOO_SHORT",
		"The security descriptor is too short.",
		"The security descriptor is too short."
	},
#endif // #ifdef ERROR_DS_SEC_DESC_TOO_SHORT
#ifdef ERROR_DS_SEC_DESC_INVALID
	{	ERROR_DS_SEC_DESC_INVALID,                                "ERROR_DS_SEC_DESC_INVALID",
		"The security descriptor is invalid.",
		"The security descriptor is invalid."
	},
#endif // #ifdef ERROR_DS_SEC_DESC_INVALID
#ifdef ERROR_DS_NO_DELETED_NAME
	{	ERROR_DS_NO_DELETED_NAME,                                 "ERROR_DS_NO_DELETED_NAME",
		"Failed to create name for deleted object.",
		"Failed to create name for deleted object."
	},
#endif // #ifdef ERROR_DS_NO_DELETED_NAME
#ifdef ERROR_DS_SUBREF_MUST_HAVE_PARENT
	{	ERROR_DS_SUBREF_MUST_HAVE_PARENT,                         "ERROR_DS_SUBREF_MUST_HAVE_PARENT",
		"The parent of a new subref must exist.",
		"The parent of a new subref must exist."
	},
#endif // #ifdef ERROR_DS_SUBREF_MUST_HAVE_PARENT
#ifdef ERROR_DS_NCNAME_MUST_BE_NC
	{	ERROR_DS_NCNAME_MUST_BE_NC,                               "ERROR_DS_NCNAME_MUST_BE_NC",
		"The object must be a naming context.",
		"The object must be a naming context."
	},
#endif // #ifdef ERROR_DS_NCNAME_MUST_BE_NC
#ifdef ERROR_DS_CANT_ADD_SYSTEM_ONLY
	{	ERROR_DS_CANT_ADD_SYSTEM_ONLY,                            "ERROR_DS_CANT_ADD_SYSTEM_ONLY",
		"It is not permitted to add an attribute which is owned by the system.",
		"It is not permitted to add an attribute which is owned by the system."
	},
#endif // #ifdef ERROR_DS_CANT_ADD_SYSTEM_ONLY
#ifdef ERROR_DS_CLASS_MUST_BE_CONCRETE
	{	ERROR_DS_CLASS_MUST_BE_CONCRETE,                          "ERROR_DS_CLASS_MUST_BE_CONCRETE",
		"The class of the object must be structural; you cannot instantiate an abstract class.",
		"The class of the object must be structural; you cannot instantiate an abstract class."
	},
#endif // #ifdef ERROR_DS_CLASS_MUST_BE_CONCRETE
#ifdef ERROR_DS_INVALID_DMD
	{	ERROR_DS_INVALID_DMD,                                     "ERROR_DS_INVALID_DMD",
		"The schema object could not be found.",
		"The schema object could not be found."
	},
#endif // #ifdef ERROR_DS_INVALID_DMD
#ifdef ERROR_DS_OBJ_GUID_EXISTS
	{	ERROR_DS_OBJ_GUID_EXISTS,                                 "ERROR_DS_OBJ_GUID_EXISTS",
		"A local object with this GUID (dead or alive) already exists.",
		"A local object with this GUID (dead or alive) already exists."
	},
#endif // #ifdef ERROR_DS_OBJ_GUID_EXISTS
#ifdef ERROR_DS_NOT_ON_BACKLINK
	{	ERROR_DS_NOT_ON_BACKLINK,                                 "ERROR_DS_NOT_ON_BACKLINK",
		"The operation cannot be performed on a back link.",
		"The operation cannot be performed on a back link."
	},
#endif // #ifdef ERROR_DS_NOT_ON_BACKLINK
#ifdef ERROR_DS_NO_CROSSREF_FOR_NC
	{	ERROR_DS_NO_CROSSREF_FOR_NC,                              "ERROR_DS_NO_CROSSREF_FOR_NC",
		"The cross reference for the specified naming context could not be found.",
		"The cross reference for the specified naming context could not be found."
	},
#endif // #ifdef ERROR_DS_NO_CROSSREF_FOR_NC
#ifdef ERROR_DS_SHUTTING_DOWN
	{	ERROR_DS_SHUTTING_DOWN,                                   "ERROR_DS_SHUTTING_DOWN",
		"The operation could not be performed because the directory service is shutting down.",
		"The operation could not be performed because the directory service is shutting down."
	},
#endif // #ifdef ERROR_DS_SHUTTING_DOWN
#ifdef ERROR_DS_UNKNOWN_OPERATION
	{	ERROR_DS_UNKNOWN_OPERATION,                               "ERROR_DS_UNKNOWN_OPERATION",
		"The directory service request is invalid.",
		"The directory service request is invalid."
	},
#endif // #ifdef ERROR_DS_UNKNOWN_OPERATION
#ifdef ERROR_DS_INVALID_ROLE_OWNER
	{	ERROR_DS_INVALID_ROLE_OWNER,                              "ERROR_DS_INVALID_ROLE_OWNER",
		"The role owner attribute could not be read.",
		"The role owner attribute could not be read."
	},
#endif // #ifdef ERROR_DS_INVALID_ROLE_OWNER
#ifdef ERROR_DS_COULDNT_CONTACT_FSMO
	{	ERROR_DS_COULDNT_CONTACT_FSMO,                            "ERROR_DS_COULDNT_CONTACT_FSMO",
		"The requested FSMO operation failed. The current FSMO holder could not be reached.",
		"The requested FSMO operation failed. The current FSMO holder could not be reached."
	},
#endif // #ifdef ERROR_DS_COULDNT_CONTACT_FSMO
#ifdef ERROR_DS_CROSS_NC_DN_RENAME
	{	ERROR_DS_CROSS_NC_DN_RENAME,                              "ERROR_DS_CROSS_NC_DN_RENAME",
		"Modification of a DN across a naming context is not permitted.",
		"Modification of a DN across a naming context is not permitted."
	},
#endif // #ifdef ERROR_DS_CROSS_NC_DN_RENAME
#ifdef ERROR_DS_CANT_MOD_SYSTEM_ONLY
	{	ERROR_DS_CANT_MOD_SYSTEM_ONLY,                            "ERROR_DS_CANT_MOD_SYSTEM_ONLY",
		"The attribute cannot be modified because it is owned by the system.",
		"The attribute cannot be modified because it is owned by the system."
	},
#endif // #ifdef ERROR_DS_CANT_MOD_SYSTEM_ONLY
#ifdef ERROR_DS_REPLICATOR_ONLY
	{	ERROR_DS_REPLICATOR_ONLY,                                 "ERROR_DS_REPLICATOR_ONLY",
		"Only the replicator can perform this function.",
		"Only the replicator can perform this function."
	},
#endif // #ifdef ERROR_DS_REPLICATOR_ONLY
#ifdef ERROR_DS_OBJ_CLASS_NOT_DEFINED
	{	ERROR_DS_OBJ_CLASS_NOT_DEFINED,                           "ERROR_DS_OBJ_CLASS_NOT_DEFINED",
		"The specified class is not defined.",
		"The specified class is not defined."
	},
#endif // #ifdef ERROR_DS_OBJ_CLASS_NOT_DEFINED
#ifdef ERROR_DS_OBJ_CLASS_NOT_SUBCLASS
	{	ERROR_DS_OBJ_CLASS_NOT_SUBCLASS,                          "ERROR_DS_OBJ_CLASS_NOT_SUBCLASS",
		"The specified class is not a subclass.",
		"The specified class is not a subclass."
	},
#endif // #ifdef ERROR_DS_OBJ_CLASS_NOT_SUBCLASS
#ifdef ERROR_DS_NAME_REFERENCE_INVALID
	{	ERROR_DS_NAME_REFERENCE_INVALID,                          "ERROR_DS_NAME_REFERENCE_INVALID",
		"The name reference is invalid.",
		"The name reference is invalid."
	},
#endif // #ifdef ERROR_DS_NAME_REFERENCE_INVALID
#ifdef ERROR_DS_CROSS_REF_EXISTS
	{	ERROR_DS_CROSS_REF_EXISTS,                                "ERROR_DS_CROSS_REF_EXISTS",
		"A cross reference already exists.",
		"A cross reference already exists."
	},
#endif // #ifdef ERROR_DS_CROSS_REF_EXISTS
#ifdef ERROR_DS_CANT_DEL_MASTER_CROSSREF
	{	ERROR_DS_CANT_DEL_MASTER_CROSSREF,                        "ERROR_DS_CANT_DEL_MASTER_CROSSREF",
		"It is not permitted to delete a master cross reference.",
		"It is not permitted to delete a master cross reference."
	},
#endif // #ifdef ERROR_DS_CANT_DEL_MASTER_CROSSREF
#ifdef ERROR_DS_SUBTREE_NOTIFY_NOT_NC_HEAD
	{	ERROR_DS_SUBTREE_NOTIFY_NOT_NC_HEAD,                      "ERROR_DS_SUBTREE_NOTIFY_NOT_NC_HEAD",
		"Subtree notifications are only supported on NC heads.",
		"Subtree notifications are only supported on NC heads."
	},
#endif // #ifdef ERROR_DS_SUBTREE_NOTIFY_NOT_NC_HEAD
#ifdef ERROR_DS_NOTIFY_FILTER_TOO_COMPLEX
	{	ERROR_DS_NOTIFY_FILTER_TOO_COMPLEX,                       "ERROR_DS_NOTIFY_FILTER_TOO_COMPLEX",
		"Notification filter is too complex.",
		"Notification filter is too complex."
	},
#endif // #ifdef ERROR_DS_NOTIFY_FILTER_TOO_COMPLEX
#ifdef ERROR_DS_DUP_RDN
	{	ERROR_DS_DUP_RDN,                                         "ERROR_DS_DUP_RDN",
		"Schema update failed: duplicate RDN.",
		"Schema update failed: duplicate RDN."
	},
#endif // #ifdef ERROR_DS_DUP_RDN
#ifdef ERROR_DS_DUP_OID
	{	ERROR_DS_DUP_OID,                                         "ERROR_DS_DUP_OID",
		"Schema update failed: duplicate OID",
		"Schema update failed: duplicate OID"
	},
#endif // #ifdef ERROR_DS_DUP_OID
#ifdef ERROR_DS_DUP_MAPI_ID
	{	ERROR_DS_DUP_MAPI_ID,                                     "ERROR_DS_DUP_MAPI_ID",
		"Schema update failed: duplicate MAPI identifier.",
		"Schema update failed: duplicate MAPI identifier."
	},
#endif // #ifdef ERROR_DS_DUP_MAPI_ID
#ifdef ERROR_DS_DUP_SCHEMA_ID_GUID
	{	ERROR_DS_DUP_SCHEMA_ID_GUID,                              "ERROR_DS_DUP_SCHEMA_ID_GUID",
		"Schema update failed: duplicate schema-id GUID.",
		"Schema update failed: duplicate schema-id GUID."
	},
#endif // #ifdef ERROR_DS_DUP_SCHEMA_ID_GUID
#ifdef ERROR_DS_DUP_LDAP_DISPLAY_NAME
	{	ERROR_DS_DUP_LDAP_DISPLAY_NAME,                           "ERROR_DS_DUP_LDAP_DISPLAY_NAME",
		"Schema update failed: duplicate LDAP display name.",
		"Schema update failed: duplicate LDAP display name."
	},
#endif // #ifdef ERROR_DS_DUP_LDAP_DISPLAY_NAME
#ifdef ERROR_DS_SEMANTIC_ATT_TEST
	{	ERROR_DS_SEMANTIC_ATT_TEST,                               "ERROR_DS_SEMANTIC_ATT_TEST",
		"Schema update failed: range-lower less than range upper",
		"Schema update failed: range-lower less than range upper"
	},
#endif // #ifdef ERROR_DS_SEMANTIC_ATT_TEST
#ifdef ERROR_DS_SYNTAX_MISMATCH
	{	ERROR_DS_SYNTAX_MISMATCH,                                 "ERROR_DS_SYNTAX_MISMATCH",
		"Schema update failed: syntax mismatch",
		"Schema update failed: syntax mismatch"
	},
#endif // #ifdef ERROR_DS_SYNTAX_MISMATCH
#ifdef ERROR_DS_EXISTS_IN_MUST_HAVE
	{	ERROR_DS_EXISTS_IN_MUST_HAVE,                             "ERROR_DS_EXISTS_IN_MUST_HAVE",
		"Schema deletion failed: attribute is used in must-contain",
		"Schema deletion failed: attribute is used in must-contain"
	},
#endif // #ifdef ERROR_DS_EXISTS_IN_MUST_HAVE
#ifdef ERROR_DS_EXISTS_IN_MAY_HAVE
	{	ERROR_DS_EXISTS_IN_MAY_HAVE,                              "ERROR_DS_EXISTS_IN_MAY_HAVE",
		"Schema deletion failed: attribute is used in may-contain",
		"Schema deletion failed: attribute is used in may-contain"
	},
#endif // #ifdef ERROR_DS_EXISTS_IN_MAY_HAVE
#ifdef ERROR_DS_NONEXISTENT_MAY_HAVE
	{	ERROR_DS_NONEXISTENT_MAY_HAVE,                            "ERROR_DS_NONEXISTENT_MAY_HAVE",
		"Schema update failed: attribute in may-contain does not exist",
		"Schema update failed: attribute in may-contain does not exist"
	},
#endif // #ifdef ERROR_DS_NONEXISTENT_MAY_HAVE
#ifdef ERROR_DS_NONEXISTENT_MUST_HAVE
	{	ERROR_DS_NONEXISTENT_MUST_HAVE,                           "ERROR_DS_NONEXISTENT_MUST_HAVE",
		"Schema update failed: attribute in must-contain does not exist",
		"Schema update failed: attribute in must-contain does not exist"
	},
#endif // #ifdef ERROR_DS_NONEXISTENT_MUST_HAVE
#ifdef ERROR_DS_AUX_CLS_TEST_FAIL
	{	ERROR_DS_AUX_CLS_TEST_FAIL,                               "ERROR_DS_AUX_CLS_TEST_FAIL",
		"Schema update failed: class in aux-class list does not exist or is not an auxiliary class",
		"Schema update failed: class in aux-class list does not exist or is not an auxiliary class"
	},
#endif // #ifdef ERROR_DS_AUX_CLS_TEST_FAIL
#ifdef ERROR_DS_NONEXISTENT_POSS_SUP
	{	ERROR_DS_NONEXISTENT_POSS_SUP,                            "ERROR_DS_NONEXISTENT_POSS_SUP",
		"Schema update failed: class in poss-superiors does not exist",
		"Schema update failed: class in poss-superiors does not exist"
	},
#endif // #ifdef ERROR_DS_NONEXISTENT_POSS_SUP
#ifdef ERROR_DS_SUB_CLS_TEST_FAIL
	{	ERROR_DS_SUB_CLS_TEST_FAIL,                               "ERROR_DS_SUB_CLS_TEST_FAIL",
		"Schema update failed: class in subclassof list does not exist or does not satisfy hierarchy rules",
		"Schema update failed: class in subclassof list does not exist or does not satisfy hierarchy rules"
	},
#endif // #ifdef ERROR_DS_SUB_CLS_TEST_FAIL
#ifdef ERROR_DS_BAD_RDN_ATT_ID_SYNTAX
	{	ERROR_DS_BAD_RDN_ATT_ID_SYNTAX,                           "ERROR_DS_BAD_RDN_ATT_ID_SYNTAX",
		"Schema update failed: Rdn-Att-Id has wrong syntax",
		"Schema update failed: Rdn-Att-Id has wrong syntax"
	},
#endif // #ifdef ERROR_DS_BAD_RDN_ATT_ID_SYNTAX
#ifdef ERROR_DS_EXISTS_IN_AUX_CLS
	{	ERROR_DS_EXISTS_IN_AUX_CLS,                               "ERROR_DS_EXISTS_IN_AUX_CLS",
		"Schema deletion failed: class is used as auxiliary class",
		"Schema deletion failed: class is used as auxiliary class"
	},
#endif // #ifdef ERROR_DS_EXISTS_IN_AUX_CLS
#ifdef ERROR_DS_EXISTS_IN_SUB_CLS
	{	ERROR_DS_EXISTS_IN_SUB_CLS,                               "ERROR_DS_EXISTS_IN_SUB_CLS",
		"Schema deletion failed: class is used as sub class",
		"Schema deletion failed: class is used as sub class"
	},
#endif // #ifdef ERROR_DS_EXISTS_IN_SUB_CLS
#ifdef ERROR_DS_EXISTS_IN_POSS_SUP
	{	ERROR_DS_EXISTS_IN_POSS_SUP,                              "ERROR_DS_EXISTS_IN_POSS_SUP",
		"Schema deletion failed: class is used as poss superior",
		"Schema deletion failed: class is used as poss superior"
	},
#endif // #ifdef ERROR_DS_EXISTS_IN_POSS_SUP
#ifdef ERROR_DS_RECALCSCHEMA_FAILED
	{	ERROR_DS_RECALCSCHEMA_FAILED,                             "ERROR_DS_RECALCSCHEMA_FAILED",
		"Schema update failed in recalculating validation cache.",
		"Schema update failed in recalculating validation cache."
	},
#endif // #ifdef ERROR_DS_RECALCSCHEMA_FAILED
#ifdef ERROR_DS_TREE_DELETE_NOT_FINISHED
	{	ERROR_DS_TREE_DELETE_NOT_FINISHED,                        "ERROR_DS_TREE_DELETE_NOT_FINISHED",
		"The tree deletion is not finished.",
		"The tree deletion is not finished."
	},
#endif // #ifdef ERROR_DS_TREE_DELETE_NOT_FINISHED
#ifdef ERROR_DS_CANT_DELETE
	{	ERROR_DS_CANT_DELETE,                                     "ERROR_DS_CANT_DELETE",
		"The requested delete operation could not be performed.",
		"The requested delete operation could not be performed."
	},
#endif // #ifdef ERROR_DS_CANT_DELETE
#ifdef ERROR_DS_ATT_SCHEMA_REQ_ID
	{	ERROR_DS_ATT_SCHEMA_REQ_ID,                               "ERROR_DS_ATT_SCHEMA_REQ_ID",
		"Cannot read the governs class identifier for the schema record.",
		"Cannot read the governs class identifier for the schema record."
	},
#endif // #ifdef ERROR_DS_ATT_SCHEMA_REQ_ID
#ifdef ERROR_DS_BAD_ATT_SCHEMA_SYNTAX
	{	ERROR_DS_BAD_ATT_SCHEMA_SYNTAX,                           "ERROR_DS_BAD_ATT_SCHEMA_SYNTAX",
		"The attribute schema has bad syntax.",
		"The attribute schema has bad syntax."
	},
#endif // #ifdef ERROR_DS_BAD_ATT_SCHEMA_SYNTAX
#ifdef ERROR_DS_CANT_CACHE_ATT
	{	ERROR_DS_CANT_CACHE_ATT,                                  "ERROR_DS_CANT_CACHE_ATT",
		"The attribute could not be cached.",
		"The attribute could not be cached."
	},
#endif // #ifdef ERROR_DS_CANT_CACHE_ATT
#ifdef ERROR_DS_CANT_CACHE_CLASS
	{	ERROR_DS_CANT_CACHE_CLASS,                                "ERROR_DS_CANT_CACHE_CLASS",
		"The class could not be cached.",
		"The class could not be cached."
	},
#endif // #ifdef ERROR_DS_CANT_CACHE_CLASS
#ifdef ERROR_DS_CANT_REMOVE_ATT_CACHE
	{	ERROR_DS_CANT_REMOVE_ATT_CACHE,                           "ERROR_DS_CANT_REMOVE_ATT_CACHE",
		"The attribute could not be removed from the cache.",
		"The attribute could not be removed from the cache."
	},
#endif // #ifdef ERROR_DS_CANT_REMOVE_ATT_CACHE
#ifdef ERROR_DS_CANT_REMOVE_CLASS_CACHE
	{	ERROR_DS_CANT_REMOVE_CLASS_CACHE,                         "ERROR_DS_CANT_REMOVE_CLASS_CACHE",
		"The class could not be removed from the cache.",
		"The class could not be removed from the cache."
	},
#endif // #ifdef ERROR_DS_CANT_REMOVE_CLASS_CACHE
#ifdef ERROR_DS_CANT_RETRIEVE_DN
	{	ERROR_DS_CANT_RETRIEVE_DN,                                "ERROR_DS_CANT_RETRIEVE_DN",
		"The distinguished name attribute could not be read.",
		"The distinguished name attribute could not be read."
	},
#endif // #ifdef ERROR_DS_CANT_RETRIEVE_DN
#ifdef ERROR_DS_MISSING_SUPREF
	{	ERROR_DS_MISSING_SUPREF,                                  "ERROR_DS_MISSING_SUPREF",
		"No superior reference has been configured for the directory service. The directory service is therefore unable to issue referrals to objects outside this forest.",
		"No superior reference has been configured for the directory service. The directory service is therefore unable to issue referrals to objects outside this forest."
	},
#endif // #ifdef ERROR_DS_MISSING_SUPREF
#ifdef ERROR_DS_CANT_RETRIEVE_INSTANCE
	{	ERROR_DS_CANT_RETRIEVE_INSTANCE,                          "ERROR_DS_CANT_RETRIEVE_INSTANCE",
		"The instance type attribute could not be retrieved.",
		"The instance type attribute could not be retrieved."
	},
#endif // #ifdef ERROR_DS_CANT_RETRIEVE_INSTANCE
#ifdef ERROR_DS_CODE_INCONSISTENCY
	{	ERROR_DS_CODE_INCONSISTENCY,                              "ERROR_DS_CODE_INCONSISTENCY",
		"An internal error has occurred.",
		"An internal error has occurred."
	},
#endif // #ifdef ERROR_DS_CODE_INCONSISTENCY
#ifdef ERROR_DS_DATABASE_ERROR
	{	ERROR_DS_DATABASE_ERROR,                                  "ERROR_DS_DATABASE_ERROR",
		"A database error has occurred.",
		"A database error has occurred."
	},
#endif // #ifdef ERROR_DS_DATABASE_ERROR
#ifdef ERROR_DS_GOVERNSID_MISSING
	{	ERROR_DS_GOVERNSID_MISSING,                               "ERROR_DS_GOVERNSID_MISSING",
		"The attribute GOVERNSID is missing.",
		"The attribute GOVERNSID is missing."
	},
#endif // #ifdef ERROR_DS_GOVERNSID_MISSING
#ifdef ERROR_DS_MISSING_EXPECTED_ATT
	{	ERROR_DS_MISSING_EXPECTED_ATT,                            "ERROR_DS_MISSING_EXPECTED_ATT",
		"An expected attribute is missing.",
		"An expected attribute is missing."
	},
#endif // #ifdef ERROR_DS_MISSING_EXPECTED_ATT
#ifdef ERROR_DS_NCNAME_MISSING_CR_REF
	{	ERROR_DS_NCNAME_MISSING_CR_REF,                           "ERROR_DS_NCNAME_MISSING_CR_REF",
		"The specified naming context is missing a cross reference.",
		"The specified naming context is missing a cross reference."
	},
#endif // #ifdef ERROR_DS_NCNAME_MISSING_CR_REF
#ifdef ERROR_DS_SECURITY_CHECKING_ERROR
	{	ERROR_DS_SECURITY_CHECKING_ERROR,                         "ERROR_DS_SECURITY_CHECKING_ERROR",
		"A security checking error has occurred.",
		"A security checking error has occurred."
	},
#endif // #ifdef ERROR_DS_SECURITY_CHECKING_ERROR
#ifdef ERROR_DS_SCHEMA_NOT_LOADED
	{	ERROR_DS_SCHEMA_NOT_LOADED,                               "ERROR_DS_SCHEMA_NOT_LOADED",
		"The schema is not loaded.",
		"The schema is not loaded."
	},
#endif // #ifdef ERROR_DS_SCHEMA_NOT_LOADED
#ifdef ERROR_DS_SCHEMA_ALLOC_FAILED
	{	ERROR_DS_SCHEMA_ALLOC_FAILED,                             "ERROR_DS_SCHEMA_ALLOC_FAILED",
		"Schema allocation failed. Please check if the machine is running low on memory.",
		"Schema allocation failed. Please check if the machine is running low on memory."
	},
#endif // #ifdef ERROR_DS_SCHEMA_ALLOC_FAILED
#ifdef ERROR_DS_ATT_SCHEMA_REQ_SYNTAX
	{	ERROR_DS_ATT_SCHEMA_REQ_SYNTAX,                           "ERROR_DS_ATT_SCHEMA_REQ_SYNTAX",
		"Failed to obtain the required syntax for the attribute schema.",
		"Failed to obtain the required syntax for the attribute schema."
	},
#endif // #ifdef ERROR_DS_ATT_SCHEMA_REQ_SYNTAX
#ifdef ERROR_DS_GCVERIFY_ERROR
	{	ERROR_DS_GCVERIFY_ERROR,                                  "ERROR_DS_GCVERIFY_ERROR",
		"The global catalog verification failed. The global catalog is not available or does not support the operation. Some part of the directory is currently not available.",
		"The global catalog verification failed. The global catalog is not available or does not support the operation. Some part of the directory is currently not available."
	},
#endif // #ifdef ERROR_DS_GCVERIFY_ERROR
#ifdef ERROR_DS_DRA_SCHEMA_MISMATCH
	{	ERROR_DS_DRA_SCHEMA_MISMATCH,                             "ERROR_DS_DRA_SCHEMA_MISMATCH",
		"The replication operation failed because of a schema mismatch between the servers involved.",
		"The replication operation failed because of a schema mismatch between the servers involved."
	},
#endif // #ifdef ERROR_DS_DRA_SCHEMA_MISMATCH
#ifdef ERROR_DS_CANT_FIND_DSA_OBJ
	{	ERROR_DS_CANT_FIND_DSA_OBJ,                               "ERROR_DS_CANT_FIND_DSA_OBJ",
		"The DSA object could not be found.",
		"The DSA object could not be found."
	},
#endif // #ifdef ERROR_DS_CANT_FIND_DSA_OBJ
#ifdef ERROR_DS_CANT_FIND_EXPECTED_NC
	{	ERROR_DS_CANT_FIND_EXPECTED_NC,                           "ERROR_DS_CANT_FIND_EXPECTED_NC",
		"The naming context could not be found.",
		"The naming context could not be found."
	},
#endif // #ifdef ERROR_DS_CANT_FIND_EXPECTED_NC
#ifdef ERROR_DS_CANT_FIND_NC_IN_CACHE
	{	ERROR_DS_CANT_FIND_NC_IN_CACHE,                           "ERROR_DS_CANT_FIND_NC_IN_CACHE",
		"The naming context could not be found in the cache.",
		"The naming context could not be found in the cache."
	},
#endif // #ifdef ERROR_DS_CANT_FIND_NC_IN_CACHE
#ifdef ERROR_DS_CANT_RETRIEVE_CHILD
	{	ERROR_DS_CANT_RETRIEVE_CHILD,                             "ERROR_DS_CANT_RETRIEVE_CHILD",
		"The child object could not be retrieved.",
		"The child object could not be retrieved."
	},
#endif // #ifdef ERROR_DS_CANT_RETRIEVE_CHILD
#ifdef ERROR_DS_SECURITY_ILLEGAL_MODIFY
	{	ERROR_DS_SECURITY_ILLEGAL_MODIFY,                         "ERROR_DS_SECURITY_ILLEGAL_MODIFY",
		"The modification was not permitted for security reasons.",
		"The modification was not permitted for security reasons."
	},
#endif // #ifdef ERROR_DS_SECURITY_ILLEGAL_MODIFY
#ifdef ERROR_DS_CANT_REPLACE_HIDDEN_REC
	{	ERROR_DS_CANT_REPLACE_HIDDEN_REC,                         "ERROR_DS_CANT_REPLACE_HIDDEN_REC",
		"The operation cannot replace the hidden record.",
		"The operation cannot replace the hidden record."
	},
#endif // #ifdef ERROR_DS_CANT_REPLACE_HIDDEN_REC
#ifdef ERROR_DS_BAD_HIERARCHY_FILE
	{	ERROR_DS_BAD_HIERARCHY_FILE,                              "ERROR_DS_BAD_HIERARCHY_FILE",
		"The hierarchy file is invalid.",
		"The hierarchy file is invalid."
	},
#endif // #ifdef ERROR_DS_BAD_HIERARCHY_FILE
#ifdef ERROR_DS_BUILD_HIERARCHY_TABLE_FAILED
	{	ERROR_DS_BUILD_HIERARCHY_TABLE_FAILED,                    "ERROR_DS_BUILD_HIERARCHY_TABLE_FAILED",
		"The attempt to build the hierarchy table failed.",
		"The attempt to build the hierarchy table failed."
	},
#endif // #ifdef ERROR_DS_BUILD_HIERARCHY_TABLE_FAILED
#ifdef ERROR_DS_CONFIG_PARAM_MISSING
	{	ERROR_DS_CONFIG_PARAM_MISSING,                            "ERROR_DS_CONFIG_PARAM_MISSING",
		"The directory configuration parameter is missing from the registry.",
		"The directory configuration parameter is missing from the registry."
	},
#endif // #ifdef ERROR_DS_CONFIG_PARAM_MISSING
#ifdef ERROR_DS_COUNTING_AB_INDICES_FAILED
	{	ERROR_DS_COUNTING_AB_INDICES_FAILED,                      "ERROR_DS_COUNTING_AB_INDICES_FAILED",
		"The attempt to count the address book indices failed.",
		"The attempt to count the address book indices failed."
	},
#endif // #ifdef ERROR_DS_COUNTING_AB_INDICES_FAILED
#ifdef ERROR_DS_HIERARCHY_TABLE_MALLOC_FAILED
	{	ERROR_DS_HIERARCHY_TABLE_MALLOC_FAILED,                   "ERROR_DS_HIERARCHY_TABLE_MALLOC_FAILED",
		"The allocation of the hierarchy table failed.",
		"The allocation of the hierarchy table failed."
	},
#endif // #ifdef ERROR_DS_HIERARCHY_TABLE_MALLOC_FAILED
#ifdef ERROR_DS_INTERNAL_FAILURE
	{	ERROR_DS_INTERNAL_FAILURE,                                "ERROR_DS_INTERNAL_FAILURE",
		"The directory service encountered an internal failure.",
		"The directory service encountered an internal failure."
	},
#endif // #ifdef ERROR_DS_INTERNAL_FAILURE
#ifdef ERROR_DS_UNKNOWN_ERROR
	{	ERROR_DS_UNKNOWN_ERROR,                                   "ERROR_DS_UNKNOWN_ERROR",
		"The directory service encountered an unknown failure.",
		"The directory service encountered an unknown failure."
	},
#endif // #ifdef ERROR_DS_UNKNOWN_ERROR
#ifdef ERROR_DS_ROOT_REQUIRES_CLASS_TOP
	{	ERROR_DS_ROOT_REQUIRES_CLASS_TOP,                         "ERROR_DS_ROOT_REQUIRES_CLASS_TOP",
		"A root object requires a class of 'top'.",
		"A root object requires a class of 'top'."
	},
#endif // #ifdef ERROR_DS_ROOT_REQUIRES_CLASS_TOP
#ifdef ERROR_DS_REFUSING_FSMO_ROLES
	{	ERROR_DS_REFUSING_FSMO_ROLES,                             "ERROR_DS_REFUSING_FSMO_ROLES",
		"This directory server is shutting down, and cannot take ownership of new floating single-master operation roles.",
		"This directory server is shutting down, and cannot take ownership of new floating single-master operation roles."
	},
#endif // #ifdef ERROR_DS_REFUSING_FSMO_ROLES
#ifdef ERROR_DS_MISSING_FSMO_SETTINGS
	{	ERROR_DS_MISSING_FSMO_SETTINGS,                           "ERROR_DS_MISSING_FSMO_SETTINGS",
		"The directory service is missing mandatory configuration information, and is unable to determine the ownership of floating single-master operation roles.",
		"The directory service is missing mandatory configuration information, and is unable to determine the ownership of floating single-master operation roles."
	},
#endif // #ifdef ERROR_DS_MISSING_FSMO_SETTINGS
#ifdef ERROR_DS_UNABLE_TO_SURRENDER_ROLES
	{	ERROR_DS_UNABLE_TO_SURRENDER_ROLES,                       "ERROR_DS_UNABLE_TO_SURRENDER_ROLES",
		"The directory service was unable to transfer ownership of one or more floating single-master operation roles to other servers.",
		"The directory service was unable to transfer ownership of one or more floating single-master operation roles to other servers."
	},
#endif // #ifdef ERROR_DS_UNABLE_TO_SURRENDER_ROLES
#ifdef ERROR_DS_DRA_GENERIC
	{	ERROR_DS_DRA_GENERIC,                                     "ERROR_DS_DRA_GENERIC",
		"The replication operation failed.",
		"The replication operation failed."
	},
#endif // #ifdef ERROR_DS_DRA_GENERIC
#ifdef ERROR_DS_DRA_INVALID_PARAMETER
	{	ERROR_DS_DRA_INVALID_PARAMETER,                           "ERROR_DS_DRA_INVALID_PARAMETER",
		"An invalid parameter was specified for this replication operation.",
		"An invalid parameter was specified for this replication operation."
	},
#endif // #ifdef ERROR_DS_DRA_INVALID_PARAMETER
#ifdef ERROR_DS_DRA_BUSY
	{	ERROR_DS_DRA_BUSY,                                        "ERROR_DS_DRA_BUSY",
		"The directory service is too busy to complete the replication operation at this time.",
		"The directory service is too busy to complete the replication operation at this time."
	},
#endif // #ifdef ERROR_DS_DRA_BUSY
#ifdef ERROR_DS_DRA_BAD_DN
	{	ERROR_DS_DRA_BAD_DN,                                      "ERROR_DS_DRA_BAD_DN",
		"The distinguished name specified for this replication operation is invalid.",
		"The distinguished name specified for this replication operation is invalid."
	},
#endif // #ifdef ERROR_DS_DRA_BAD_DN
#ifdef ERROR_DS_DRA_BAD_NC
	{	ERROR_DS_DRA_BAD_NC,                                      "ERROR_DS_DRA_BAD_NC",
		"The naming context specified for this replication operation is invalid.",
		"The naming context specified for this replication operation is invalid."
	},
#endif // #ifdef ERROR_DS_DRA_BAD_NC
#ifdef ERROR_DS_DRA_DN_EXISTS
	{	ERROR_DS_DRA_DN_EXISTS,                                   "ERROR_DS_DRA_DN_EXISTS",
		"The distinguished name specified for this replication operation already exists.",
		"The distinguished name specified for this replication operation already exists."
	},
#endif // #ifdef ERROR_DS_DRA_DN_EXISTS
#ifdef ERROR_DS_DRA_INTERNAL_ERROR
	{	ERROR_DS_DRA_INTERNAL_ERROR,                              "ERROR_DS_DRA_INTERNAL_ERROR",
		"The replication system encountered an internal error.",
		"The replication system encountered an internal error."
	},
#endif // #ifdef ERROR_DS_DRA_INTERNAL_ERROR
#ifdef ERROR_DS_DRA_INCONSISTENT_DIT
	{	ERROR_DS_DRA_INCONSISTENT_DIT,                            "ERROR_DS_DRA_INCONSISTENT_DIT",
		"The replication operation encountered a database inconsistency.",
		"The replication operation encountered a database inconsistency."
	},
#endif // #ifdef ERROR_DS_DRA_INCONSISTENT_DIT
#ifdef ERROR_DS_DRA_CONNECTION_FAILED
	{	ERROR_DS_DRA_CONNECTION_FAILED,                           "ERROR_DS_DRA_CONNECTION_FAILED",
		"The server specified for this replication operation could not be contacted.",
		"The server specified for this replication operation could not be contacted."
	},
#endif // #ifdef ERROR_DS_DRA_CONNECTION_FAILED
#ifdef ERROR_DS_DRA_BAD_INSTANCE_TYPE
	{	ERROR_DS_DRA_BAD_INSTANCE_TYPE,                           "ERROR_DS_DRA_BAD_INSTANCE_TYPE",
		"The replication operation encountered an object with an invalid instance type.",
		"The replication operation encountered an object with an invalid instance type."
	},
#endif // #ifdef ERROR_DS_DRA_BAD_INSTANCE_TYPE
#ifdef ERROR_DS_DRA_OUT_OF_MEM
	{	ERROR_DS_DRA_OUT_OF_MEM,                                  "ERROR_DS_DRA_OUT_OF_MEM",
		"The replication operation failed to allocate memory.",
		"The replication operation failed to allocate memory."
	},
#endif // #ifdef ERROR_DS_DRA_OUT_OF_MEM
#ifdef ERROR_DS_DRA_MAIL_PROBLEM
	{	ERROR_DS_DRA_MAIL_PROBLEM,                                "ERROR_DS_DRA_MAIL_PROBLEM",
		"The replication operation encountered an error with the mail system.",
		"The replication operation encountered an error with the mail system."
	},
#endif // #ifdef ERROR_DS_DRA_MAIL_PROBLEM
#ifdef ERROR_DS_DRA_REF_ALREADY_EXISTS
	{	ERROR_DS_DRA_REF_ALREADY_EXISTS,                          "ERROR_DS_DRA_REF_ALREADY_EXISTS",
		"The replication reference information for the target server already exists.",
		"The replication reference information for the target server already exists."
	},
#endif // #ifdef ERROR_DS_DRA_REF_ALREADY_EXISTS
#ifdef ERROR_DS_DRA_REF_NOT_FOUND
	{	ERROR_DS_DRA_REF_NOT_FOUND,                               "ERROR_DS_DRA_REF_NOT_FOUND",
		"The replication reference information for the target server does not exist.",
		"The replication reference information for the target server does not exist."
	},
#endif // #ifdef ERROR_DS_DRA_REF_NOT_FOUND
#ifdef ERROR_DS_DRA_OBJ_IS_REP_SOURCE
	{	ERROR_DS_DRA_OBJ_IS_REP_SOURCE,                           "ERROR_DS_DRA_OBJ_IS_REP_SOURCE",
		"The naming context cannot be removed because it is replicated to another server.",
		"The naming context cannot be removed because it is replicated to another server."
	},
#endif // #ifdef ERROR_DS_DRA_OBJ_IS_REP_SOURCE
#ifdef ERROR_DS_DRA_DB_ERROR
	{	ERROR_DS_DRA_DB_ERROR,                                    "ERROR_DS_DRA_DB_ERROR",
		"The replication operation encountered a database error.",
		"The replication operation encountered a database error."
	},
#endif // #ifdef ERROR_DS_DRA_DB_ERROR
#ifdef ERROR_DS_DRA_NO_REPLICA
	{	ERROR_DS_DRA_NO_REPLICA,                                  "ERROR_DS_DRA_NO_REPLICA",
		"The naming context is in the process of being removed or is not replicated from the specified server.",
		"The naming context is in the process of being removed or is not replicated from the specified server."
	},
#endif // #ifdef ERROR_DS_DRA_NO_REPLICA
#ifdef ERROR_DS_DRA_ACCESS_DENIED
	{	ERROR_DS_DRA_ACCESS_DENIED,                               "ERROR_DS_DRA_ACCESS_DENIED",
		"Replication access was denied.",
		"Replication access was denied."
	},
#endif // #ifdef ERROR_DS_DRA_ACCESS_DENIED
#ifdef ERROR_DS_DRA_NOT_SUPPORTED
	{	ERROR_DS_DRA_NOT_SUPPORTED,                               "ERROR_DS_DRA_NOT_SUPPORTED",
		"The requested operation is not supported by this version of the directory service.",
		"The requested operation is not supported by this version of the directory service."
	},
#endif // #ifdef ERROR_DS_DRA_NOT_SUPPORTED
#ifdef ERROR_DS_DRA_RPC_CANCELLED
	{	ERROR_DS_DRA_RPC_CANCELLED,                               "ERROR_DS_DRA_RPC_CANCELLED",
		"The replication remote procedure call was cancelled.",
		"The replication remote procedure call was cancelled."
	},
#endif // #ifdef ERROR_DS_DRA_RPC_CANCELLED
#ifdef ERROR_DS_DRA_SOURCE_DISABLED
	{	ERROR_DS_DRA_SOURCE_DISABLED,                             "ERROR_DS_DRA_SOURCE_DISABLED",
		"The source server is currently rejecting replication requests.",
		"The source server is currently rejecting replication requests."
	},
#endif // #ifdef ERROR_DS_DRA_SOURCE_DISABLED
#ifdef ERROR_DS_DRA_SINK_DISABLED
	{	ERROR_DS_DRA_SINK_DISABLED,                               "ERROR_DS_DRA_SINK_DISABLED",
		"The destination server is currently rejecting replication requests.",
		"The destination server is currently rejecting replication requests."
	},
#endif // #ifdef ERROR_DS_DRA_SINK_DISABLED
#ifdef ERROR_DS_DRA_NAME_COLLISION
	{	ERROR_DS_DRA_NAME_COLLISION,                              "ERROR_DS_DRA_NAME_COLLISION",
		"The replication operation failed due to a collision of object names.",
		"The replication operation failed due to a collision of object names."
	},
#endif // #ifdef ERROR_DS_DRA_NAME_COLLISION
#ifdef ERROR_DS_DRA_SOURCE_REINSTALLED
	{	ERROR_DS_DRA_SOURCE_REINSTALLED,                          "ERROR_DS_DRA_SOURCE_REINSTALLED",
		"The replication source has been reinstalled.",
		"The replication source has been reinstalled."
	},
#endif // #ifdef ERROR_DS_DRA_SOURCE_REINSTALLED
#ifdef ERROR_DS_DRA_MISSING_PARENT
	{	ERROR_DS_DRA_MISSING_PARENT,                              "ERROR_DS_DRA_MISSING_PARENT",
		"The replication operation failed because a required parent object is missing.",
		"The replication operation failed because a required parent object is missing."
	},
#endif // #ifdef ERROR_DS_DRA_MISSING_PARENT
#ifdef ERROR_DS_DRA_PREEMPTED
	{	ERROR_DS_DRA_PREEMPTED,                                   "ERROR_DS_DRA_PREEMPTED",
		"The replication operation was preempted.",
		"The replication operation was preempted."
	},
#endif // #ifdef ERROR_DS_DRA_PREEMPTED
#ifdef ERROR_DS_DRA_ABANDON_SYNC
	{	ERROR_DS_DRA_ABANDON_SYNC,                                "ERROR_DS_DRA_ABANDON_SYNC",
		"The replication synchronization attempt was abandoned because of a lack of updates.",
		"The replication synchronization attempt was abandoned because of a lack of updates."
	},
#endif // #ifdef ERROR_DS_DRA_ABANDON_SYNC
#ifdef ERROR_DS_DRA_SHUTDOWN
	{	ERROR_DS_DRA_SHUTDOWN,                                    "ERROR_DS_DRA_SHUTDOWN",
		"The replication operation was terminated because the system is shutting down.",
		"The replication operation was terminated because the system is shutting down."
	},
#endif // #ifdef ERROR_DS_DRA_SHUTDOWN
#ifdef ERROR_DS_DRA_INCOMPATIBLE_PARTIAL_SET
	{	ERROR_DS_DRA_INCOMPATIBLE_PARTIAL_SET,                    "ERROR_DS_DRA_INCOMPATIBLE_PARTIAL_SET",
		"The replication synchronization attempt failed as the destination partial attribute set is not a subset of source partial attribute set.",
		"The replication synchronization attempt failed as the destination partial attribute set is not a subset of source partial attribute set."
	},
#endif // #ifdef ERROR_DS_DRA_INCOMPATIBLE_PARTIAL_SET
#ifdef ERROR_DS_DRA_SOURCE_IS_PARTIAL_REPLICA
	{	ERROR_DS_DRA_SOURCE_IS_PARTIAL_REPLICA,                   "ERROR_DS_DRA_SOURCE_IS_PARTIAL_REPLICA",
		"The replication synchronization attempt failed because a master replica attempted to sync from a partial replica.",
		"The replication synchronization attempt failed because a master replica attempted to sync from a partial replica."
	},
#endif // #ifdef ERROR_DS_DRA_SOURCE_IS_PARTIAL_REPLICA
#ifdef ERROR_DS_DRA_EXTN_CONNECTION_FAILED
	{	ERROR_DS_DRA_EXTN_CONNECTION_FAILED,                      "ERROR_DS_DRA_EXTN_CONNECTION_FAILED",
		"The server specified for this replication operation was contacted, but that server was unable to contact an additional server needed to complete the operation.",
		"The server specified for this replication operation was contacted, but that server was unable to contact an additional server needed to complete the operation."
	},
#endif // #ifdef ERROR_DS_DRA_EXTN_CONNECTION_FAILED
#ifdef ERROR_DS_INSTALL_SCHEMA_MISMATCH
	{	ERROR_DS_INSTALL_SCHEMA_MISMATCH,                         "ERROR_DS_INSTALL_SCHEMA_MISMATCH",
		"The version of the Active Directory schema of the source forest is not compatible with the version of Active Directory on this computer.",
		"The version of the Active Directory schema of the source forest is not compatible with the version of Active Directory on this computer."
	},
#endif // #ifdef ERROR_DS_INSTALL_SCHEMA_MISMATCH
#ifdef ERROR_DS_DUP_LINK_ID
	{	ERROR_DS_DUP_LINK_ID,                                     "ERROR_DS_DUP_LINK_ID",
		"Schema update failed: An attribute with the same link identifier already exists.",
		"Schema update failed: An attribute with the same link identifier already exists."
	},
#endif // #ifdef ERROR_DS_DUP_LINK_ID
#ifdef ERROR_DS_NAME_ERROR_RESOLVING
	{	ERROR_DS_NAME_ERROR_RESOLVING,                            "ERROR_DS_NAME_ERROR_RESOLVING",
		"Name translation: Generic processing error.",
		"Name translation: Generic processing error."
	},
#endif // #ifdef ERROR_DS_NAME_ERROR_RESOLVING
#ifdef ERROR_DS_NAME_ERROR_NOT_FOUND
	{	ERROR_DS_NAME_ERROR_NOT_FOUND,                            "ERROR_DS_NAME_ERROR_NOT_FOUND",
		"Name translation: Could not find the name or insufficient right to see name.",
		"Name translation: Could not find the name or insufficient right to see name."
	},
#endif // #ifdef ERROR_DS_NAME_ERROR_NOT_FOUND
#ifdef ERROR_DS_NAME_ERROR_NOT_UNIQUE
	{	ERROR_DS_NAME_ERROR_NOT_UNIQUE,                           "ERROR_DS_NAME_ERROR_NOT_UNIQUE",
		"Name translation: Input name mapped to more than one output name.",
		"Name translation: Input name mapped to more than one output name."
	},
#endif // #ifdef ERROR_DS_NAME_ERROR_NOT_UNIQUE
#ifdef ERROR_DS_NAME_ERROR_NO_MAPPING
	{	ERROR_DS_NAME_ERROR_NO_MAPPING,                           "ERROR_DS_NAME_ERROR_NO_MAPPING",
		"Name translation: Input name found, but not the associated output format.",
		"Name translation: Input name found, but not the associated output format."
	},
#endif // #ifdef ERROR_DS_NAME_ERROR_NO_MAPPING
#ifdef ERROR_DS_NAME_ERROR_DOMAIN_ONLY
	{	ERROR_DS_NAME_ERROR_DOMAIN_ONLY,                          "ERROR_DS_NAME_ERROR_DOMAIN_ONLY",
		"Name translation: Unable to resolve completely, only the domain was found.",
		"Name translation: Unable to resolve completely, only the domain was found."
	},
#endif // #ifdef ERROR_DS_NAME_ERROR_DOMAIN_ONLY
#ifdef ERROR_DS_NAME_ERROR_NO_SYNTACTICAL_MAPPING
	{	ERROR_DS_NAME_ERROR_NO_SYNTACTICAL_MAPPING,               "ERROR_DS_NAME_ERROR_NO_SYNTACTICAL_MAPPING",
		"Name translation: Unable to perform purely syntactical mapping at the client without going out to the wire.",
		"Name translation: Unable to perform purely syntactical mapping at the client without going out to the wire."
	},
#endif // #ifdef ERROR_DS_NAME_ERROR_NO_SYNTACTICAL_MAPPING
#ifdef ERROR_DS_CONSTRUCTED_ATT_MOD
	{	ERROR_DS_CONSTRUCTED_ATT_MOD,                             "ERROR_DS_CONSTRUCTED_ATT_MOD",
		"Modification of a constructed attribute is not allowed.",
		"Modification of a constructed attribute is not allowed."
	},
#endif // #ifdef ERROR_DS_CONSTRUCTED_ATT_MOD
#ifdef ERROR_DS_WRONG_OM_OBJ_CLASS
	{	ERROR_DS_WRONG_OM_OBJ_CLASS,                              "ERROR_DS_WRONG_OM_OBJ_CLASS",
		"The OM-Object-Class specified is incorrect for an attribute with the specified syntax.",
		"The OM-Object-Class specified is incorrect for an attribute with the specified syntax."
	},
#endif // #ifdef ERROR_DS_WRONG_OM_OBJ_CLASS
#ifdef ERROR_DS_DRA_REPL_PENDING
	{	ERROR_DS_DRA_REPL_PENDING,                                "ERROR_DS_DRA_REPL_PENDING",
		"The replication request has been posted; waiting for reply.",
		"The replication request has been posted; waiting for reply."
	},
#endif // #ifdef ERROR_DS_DRA_REPL_PENDING
#ifdef ERROR_DS_DS_REQUIRED
	{	ERROR_DS_DS_REQUIRED,                                     "ERROR_DS_DS_REQUIRED",
		"The requested operation requires a directory service, and none was available.",
		"The requested operation requires a directory service, and none was available."
	},
#endif // #ifdef ERROR_DS_DS_REQUIRED
#ifdef ERROR_DS_INVALID_LDAP_DISPLAY_NAME
	{	ERROR_DS_INVALID_LDAP_DISPLAY_NAME,                       "ERROR_DS_INVALID_LDAP_DISPLAY_NAME",
		"The LDAP display name of the class or attribute contains non-ASCII characters.",
		"The LDAP display name of the class or attribute contains non-ASCII characters."
	},
#endif // #ifdef ERROR_DS_INVALID_LDAP_DISPLAY_NAME
#ifdef ERROR_DS_NON_BASE_SEARCH
	{	ERROR_DS_NON_BASE_SEARCH,                                 "ERROR_DS_NON_BASE_SEARCH",
		"The requested search operation is only supported for base searches.",
		"The requested search operation is only supported for base searches."
	},
#endif // #ifdef ERROR_DS_NON_BASE_SEARCH
#ifdef ERROR_DS_CANT_RETRIEVE_ATTS
	{	ERROR_DS_CANT_RETRIEVE_ATTS,                              "ERROR_DS_CANT_RETRIEVE_ATTS",
		"The search failed to retrieve attributes from the database.",
		"The search failed to retrieve attributes from the database."
	},
#endif // #ifdef ERROR_DS_CANT_RETRIEVE_ATTS
#ifdef ERROR_DS_BACKLINK_WITHOUT_LINK
	{	ERROR_DS_BACKLINK_WITHOUT_LINK,                           "ERROR_DS_BACKLINK_WITHOUT_LINK",
		"The schema update operation tried to add a backward link attribute that has no corresponding forward link.",
		"The schema update operation tried to add a backward link attribute that has no corresponding forward link."
	},
#endif // #ifdef ERROR_DS_BACKLINK_WITHOUT_LINK
#ifdef ERROR_DS_EPOCH_MISMATCH
	{	ERROR_DS_EPOCH_MISMATCH,                                  "ERROR_DS_EPOCH_MISMATCH",
		"Source and destination of a cross domain move do not agree on the object's epoch number. Either source or destination does not have the latest version of the object.",
		"Source and destination of a cross domain move do not agree on the object's epoch number. Either source or destination does not have the latest version of the object."
	},
#endif // #ifdef ERROR_DS_EPOCH_MISMATCH
#ifdef ERROR_DS_SRC_NAME_MISMATCH
	{	ERROR_DS_SRC_NAME_MISMATCH,                               "ERROR_DS_SRC_NAME_MISMATCH",
		"Source and destination of a cross domain move do not agree on the object's current name. Either source or destination does not have the latest version of the object.",
		"Source and destination of a cross domain move do not agree on the object's current name. Either source or destination does not have the latest version of the object."
	},
#endif // #ifdef ERROR_DS_SRC_NAME_MISMATCH
#ifdef ERROR_DS_SRC_AND_DST_NC_IDENTICAL
	{	ERROR_DS_SRC_AND_DST_NC_IDENTICAL,                        "ERROR_DS_SRC_AND_DST_NC_IDENTICAL",
		"Source and destination of a cross domain move operation are identical. Caller should use local move operation instead of cross domain move operation.",
		"Source and destination of a cross domain move operation are identical. Caller should use local move operation instead of cross domain move operation."
	},
#endif // #ifdef ERROR_DS_SRC_AND_DST_NC_IDENTICAL
#ifdef ERROR_DS_DST_NC_MISMATCH
	{	ERROR_DS_DST_NC_MISMATCH,                                 "ERROR_DS_DST_NC_MISMATCH",
		"Source and destination for a cross domain move are not in agreement on the naming contexts in the forest. Either source or destination does not have the latest version of the Partitions container.",
		"Source and destination for a cross domain move are not in agreement on the naming contexts in the forest. Either source or destination does not have the latest version of the Partitions container."
	},
#endif // #ifdef ERROR_DS_DST_NC_MISMATCH
#ifdef ERROR_DS_NOT_AUTHORITIVE_FOR_DST_NC
	{	ERROR_DS_NOT_AUTHORITIVE_FOR_DST_NC,                      "ERROR_DS_NOT_AUTHORITIVE_FOR_DST_NC",
		"Destination of a cross domain move is not authoritative for the destination naming context.",
		"Destination of a cross domain move is not authoritative for the destination naming context."
	},
#endif // #ifdef ERROR_DS_NOT_AUTHORITIVE_FOR_DST_NC
#ifdef ERROR_DS_SRC_GUID_MISMATCH
	{	ERROR_DS_SRC_GUID_MISMATCH,                               "ERROR_DS_SRC_GUID_MISMATCH",
		"Source and destination of a cross domain move do not agree on the identity of the source object. Either source or destination does not have the latest version of the source object.",
		"Source and destination of a cross domain move do not agree on the identity of the source object. Either source or destination does not have the latest version of the source object."
	},
#endif // #ifdef ERROR_DS_SRC_GUID_MISMATCH
#ifdef ERROR_DS_CANT_MOVE_DELETED_OBJECT
	{	ERROR_DS_CANT_MOVE_DELETED_OBJECT,                        "ERROR_DS_CANT_MOVE_DELETED_OBJECT",
		"Object being moved across domains is already known to be deleted by the destination server. The source server does not have the latest version of the source object.",
		"Object being moved across domains is already known to be deleted by the destination server. The source server does not have the latest version of the source object."
	},
#endif // #ifdef ERROR_DS_CANT_MOVE_DELETED_OBJECT
#ifdef ERROR_DS_PDC_OPERATION_IN_PROGRESS
	{	ERROR_DS_PDC_OPERATION_IN_PROGRESS,                       "ERROR_DS_PDC_OPERATION_IN_PROGRESS",
		"Another operation which requires exclusive access to the PDC PSMO is already in progress.",
		"Another operation which requires exclusive access to the PDC PSMO is already in progress."
	},
#endif // #ifdef ERROR_DS_PDC_OPERATION_IN_PROGRESS
#ifdef ERROR_DS_CROSS_DOMAIN_CLEANUP_REQD
	{	ERROR_DS_CROSS_DOMAIN_CLEANUP_REQD,                       "ERROR_DS_CROSS_DOMAIN_CLEANUP_REQD",
		"A cross domain move operation failed such that the two versions of the moved object exist - one each in the source and destination domains. The destination object needs to be removed to restore the system to a consistent state.",
		"A cross domain move operation failed such that the two versions of the moved object exist - one each in the source and destination domains. The destination object needs to be removed to restore the system to a consistent state."
	},
#endif // #ifdef ERROR_DS_CROSS_DOMAIN_CLEANUP_REQD
#ifdef ERROR_DS_ILLEGAL_XDOM_MOVE_OPERATION
	{	ERROR_DS_ILLEGAL_XDOM_MOVE_OPERATION,                     "ERROR_DS_ILLEGAL_XDOM_MOVE_OPERATION",
		"This object may not be moved across domain boundaries either because cross domain moves for this class are disallowed, or the object has some special characteristics, e.g.: trust account or restricted RID, which prevent its move.",
		"This object may not be moved across domain boundaries either because cross domain moves for this class are disallowed, or the object has some special characteristics, e.g.: trust account or restricted RID, which prevent its move."
	},
#endif // #ifdef ERROR_DS_ILLEGAL_XDOM_MOVE_OPERATION
#ifdef ERROR_DS_CANT_WITH_ACCT_GROUP_MEMBERSHPS
	{	ERROR_DS_CANT_WITH_ACCT_GROUP_MEMBERSHPS,                 "ERROR_DS_CANT_WITH_ACCT_GROUP_MEMBERSHPS",
		"Can't move objects with memberships across domain boundaries as once moved, this would violate the membership conditions of the account group. Remove the object from any account group memberships and retry.",
		"Can't move objects with memberships across domain boundaries as once moved, this would violate the membership conditions of the account group. Remove the object from any account group memberships and retry."
	},
#endif // #ifdef ERROR_DS_CANT_WITH_ACCT_GROUP_MEMBERSHPS
#ifdef ERROR_DS_NC_MUST_HAVE_NC_PARENT
	{	ERROR_DS_NC_MUST_HAVE_NC_PARENT,                          "ERROR_DS_NC_MUST_HAVE_NC_PARENT",
		"A naming context head must be the immediate child of another naming context head, not of an interior node.",
		"A naming context head must be the immediate child of another naming context head, not of an interior node."
	},
#endif // #ifdef ERROR_DS_NC_MUST_HAVE_NC_PARENT
#ifdef ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE
	{	ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE,                       "ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE",
		"The directory cannot validate the proposed naming context name because it does not hold a replica of the naming context above the proposed naming context. Please ensure that the domain naming master role is held by a server that is configured as a global catalog server, and that the server is up to date with its replication partners. (Applies only to Windows 2000 Domain Naming masters)",
		"The directory cannot validate the proposed naming context name because it does not hold a replica of the naming context above the proposed naming context. Please ensure that the domain naming master role is held by a server that is configured as a global catalog server, and that the server is up to date with its replication partners. (Applies only to Windows 2000 Domain Naming masters)"
	},
#endif // #ifdef ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE
#ifdef ERROR_DS_DST_DOMAIN_NOT_NATIVE
	{	ERROR_DS_DST_DOMAIN_NOT_NATIVE,                           "ERROR_DS_DST_DOMAIN_NOT_NATIVE",
		"Destination domain must be in native mode.",
		"Destination domain must be in native mode."
	},
#endif // #ifdef ERROR_DS_DST_DOMAIN_NOT_NATIVE
#ifdef ERROR_DS_MISSING_INFRASTRUCTURE_CONTAINER
	{	ERROR_DS_MISSING_INFRASTRUCTURE_CONTAINER,                "ERROR_DS_MISSING_INFRASTRUCTURE_CONTAINER",
		"The operation cannot be performed because the server does not have an infrastructure container in the domain of interest.",
		"The operation cannot be performed because the server does not have an infrastructure container in the domain of interest."
	},
#endif // #ifdef ERROR_DS_MISSING_INFRASTRUCTURE_CONTAINER
#ifdef ERROR_DS_CANT_MOVE_ACCOUNT_GROUP
	{	ERROR_DS_CANT_MOVE_ACCOUNT_GROUP,                         "ERROR_DS_CANT_MOVE_ACCOUNT_GROUP",
		"Cross-domain move of non-empty account groups is not allowed.",
		"Cross-domain move of non-empty account groups is not allowed."
	},
#endif // #ifdef ERROR_DS_CANT_MOVE_ACCOUNT_GROUP
#ifdef ERROR_DS_CANT_MOVE_RESOURCE_GROUP
	{	ERROR_DS_CANT_MOVE_RESOURCE_GROUP,                        "ERROR_DS_CANT_MOVE_RESOURCE_GROUP",
		"Cross-domain move of non-empty resource groups is not allowed.",
		"Cross-domain move of non-empty resource groups is not allowed."
	},
#endif // #ifdef ERROR_DS_CANT_MOVE_RESOURCE_GROUP
#ifdef ERROR_DS_INVALID_SEARCH_FLAG
	{	ERROR_DS_INVALID_SEARCH_FLAG,                             "ERROR_DS_INVALID_SEARCH_FLAG",
		"The search flags for the attribute are invalid. The ANR bit is valid only on attributes of Unicode or Teletex strings.",
		"The search flags for the attribute are invalid. The ANR bit is valid only on attributes of Unicode or Teletex strings."
	},
#endif // #ifdef ERROR_DS_INVALID_SEARCH_FLAG
#ifdef ERROR_DS_NO_TREE_DELETE_ABOVE_NC
	{	ERROR_DS_NO_TREE_DELETE_ABOVE_NC,                         "ERROR_DS_NO_TREE_DELETE_ABOVE_NC",
		"Tree deletions starting at an object which has an NC head as a descendant are not allowed.",
		"Tree deletions starting at an object which has an NC head as a descendant are not allowed."
	},
#endif // #ifdef ERROR_DS_NO_TREE_DELETE_ABOVE_NC
#ifdef ERROR_DS_COULDNT_LOCK_TREE_FOR_DELETE
	{	ERROR_DS_COULDNT_LOCK_TREE_FOR_DELETE,                    "ERROR_DS_COULDNT_LOCK_TREE_FOR_DELETE",
		"The directory service failed to lock a tree in preparation for a tree deletion because the tree was in use.",
		"The directory service failed to lock a tree in preparation for a tree deletion because the tree was in use."
	},
#endif // #ifdef ERROR_DS_COULDNT_LOCK_TREE_FOR_DELETE
#ifdef ERROR_DS_COULDNT_IDENTIFY_OBJECTS_FOR_TREE_DELETE
	{	ERROR_DS_COULDNT_IDENTIFY_OBJECTS_FOR_TREE_DELETE,        "ERROR_DS_COULDNT_IDENTIFY_OBJECTS_FOR_TREE_DELETE",
		"The directory service failed to identify the list of objects to delete while attempting a tree deletion.",
		"The directory service failed to identify the list of objects to delete while attempting a tree deletion."
	},
#endif // #ifdef ERROR_DS_COULDNT_IDENTIFY_OBJECTS_FOR_TREE_DELETE
#ifdef ERROR_DS_SAM_INIT_FAILURE
	{	ERROR_DS_SAM_INIT_FAILURE,                                "ERROR_DS_SAM_INIT_FAILURE",
		"Security Accounts Manager initialization failed because of the following error: %1. Error Status: 0x%2. Click OK to shut down the system and reboot into Directory Services Restore Mode. Check the event log for detailed information.",
		"Security Accounts Manager initialization failed because of the following error: %1. Error Status: 0x%2. Click OK to shut down the system and reboot into Directory Services Restore Mode. Check the event log for detailed information."
	},
#endif // #ifdef ERROR_DS_SAM_INIT_FAILURE
#ifdef ERROR_DS_SENSITIVE_GROUP_VIOLATION
	{	ERROR_DS_SENSITIVE_GROUP_VIOLATION,                       "ERROR_DS_SENSITIVE_GROUP_VIOLATION",
		"Only an administrator can modify the membership list of an administrative group.",
		"Only an administrator can modify the membership list of an administrative group."
	},
#endif // #ifdef ERROR_DS_SENSITIVE_GROUP_VIOLATION
#ifdef ERROR_DS_CANT_MOD_PRIMARYGROUPID
	{	ERROR_DS_CANT_MOD_PRIMARYGROUPID,                         "ERROR_DS_CANT_MOD_PRIMARYGROUPID",
		"Cannot change the primary group ID of a domain controller account.",
		"Cannot change the primary group ID of a domain controller account."
	},
#endif // #ifdef ERROR_DS_CANT_MOD_PRIMARYGROUPID
#ifdef ERROR_DS_ILLEGAL_BASE_SCHEMA_MOD
	{	ERROR_DS_ILLEGAL_BASE_SCHEMA_MOD,                         "ERROR_DS_ILLEGAL_BASE_SCHEMA_MOD",
		"An attempt is made to modify the base schema.",
		"An attempt is made to modify the base schema."
	},
#endif // #ifdef ERROR_DS_ILLEGAL_BASE_SCHEMA_MOD
#ifdef ERROR_DS_NONSAFE_SCHEMA_CHANGE
	{	ERROR_DS_NONSAFE_SCHEMA_CHANGE,                           "ERROR_DS_NONSAFE_SCHEMA_CHANGE",
		"Adding a new mandatory attribute to an existing class, deleting a mandatory attribute from an existing class, or adding an optional attribute to the special class Top that is not a backlink attribute (directly or through inheritance, for example, by adding or deleting an auxiliary class) is not allowed.",
		"Adding a new mandatory attribute to an existing class, deleting a mandatory attribute from an existing class, or adding an optional attribute to the special class Top that is not a backlink attribute (directly or through inheritance, for example, by adding or deleting an auxiliary class) is not allowed."
	},
#endif // #ifdef ERROR_DS_NONSAFE_SCHEMA_CHANGE
#ifdef ERROR_DS_SCHEMA_UPDATE_DISALLOWED
	{	ERROR_DS_SCHEMA_UPDATE_DISALLOWED,                        "ERROR_DS_SCHEMA_UPDATE_DISALLOWED",
		"Schema update is not allowed on this DC because the DC is not the schema FSMO Role Owner.",
		"Schema update is not allowed on this DC because the DC is not the schema FSMO Role Owner."
	},
#endif // #ifdef ERROR_DS_SCHEMA_UPDATE_DISALLOWED
#ifdef ERROR_DS_CANT_CREATE_UNDER_SCHEMA
	{	ERROR_DS_CANT_CREATE_UNDER_SCHEMA,                        "ERROR_DS_CANT_CREATE_UNDER_SCHEMA",
		"An object of this class cannot be created under the schema container. You can only create attribute-schema and class-schema objects under the schema container.",
		"An object of this class cannot be created under the schema container. You can only create attribute-schema and class-schema objects under the schema container."
	},
#endif // #ifdef ERROR_DS_CANT_CREATE_UNDER_SCHEMA
#ifdef ERROR_DS_INSTALL_NO_SRC_SCH_VERSION
	{	ERROR_DS_INSTALL_NO_SRC_SCH_VERSION,                      "ERROR_DS_INSTALL_NO_SRC_SCH_VERSION",
		"The replica/child install failed to get the objectVersion attribute on the schema container on the source DC. Either the attribute is missing on the schema container or the credentials supplied do not have permission to read it.",
		"The replica/child install failed to get the objectVersion attribute on the schema container on the source DC. Either the attribute is missing on the schema container or the credentials supplied do not have permission to read it."
	},
#endif // #ifdef ERROR_DS_INSTALL_NO_SRC_SCH_VERSION
#ifdef ERROR_DS_INSTALL_NO_SCH_VERSION_IN_INIFILE
	{	ERROR_DS_INSTALL_NO_SCH_VERSION_IN_INIFILE,               "ERROR_DS_INSTALL_NO_SCH_VERSION_IN_INIFILE",
		"The replica/child install failed to read the objectVersion attribute in the SCHEMA section of the file schema.ini in the system32 directory.",
		"The replica/child install failed to read the objectVersion attribute in the SCHEMA section of the file schema.ini in the system32 directory."
	},
#endif // #ifdef ERROR_DS_INSTALL_NO_SCH_VERSION_IN_INIFILE
#ifdef ERROR_DS_INVALID_GROUP_TYPE
	{	ERROR_DS_INVALID_GROUP_TYPE,                              "ERROR_DS_INVALID_GROUP_TYPE",
		"The specified group type is invalid.",
		"The specified group type is invalid."
	},
#endif // #ifdef ERROR_DS_INVALID_GROUP_TYPE
#ifdef ERROR_DS_NO_NEST_GLOBALGROUP_IN_MIXEDDOMAIN
	{	ERROR_DS_NO_NEST_GLOBALGROUP_IN_MIXEDDOMAIN,              "ERROR_DS_NO_NEST_GLOBALGROUP_IN_MIXEDDOMAIN",
		"Cannot nest global groups in a mixed domain if the group is security-enabled.",
		"Cannot nest global groups in a mixed domain if the group is security-enabled."
	},
#endif // #ifdef ERROR_DS_NO_NEST_GLOBALGROUP_IN_MIXEDDOMAIN
#ifdef ERROR_DS_NO_NEST_LOCALGROUP_IN_MIXEDDOMAIN
	{	ERROR_DS_NO_NEST_LOCALGROUP_IN_MIXEDDOMAIN,               "ERROR_DS_NO_NEST_LOCALGROUP_IN_MIXEDDOMAIN",
		"Cannot nest local groups in a mixed domain if the group is security-enabled.",
		"Cannot nest local groups in a mixed domain if the group is security-enabled."
	},
#endif // #ifdef ERROR_DS_NO_NEST_LOCALGROUP_IN_MIXEDDOMAIN
#ifdef ERROR_DS_GLOBAL_CANT_HAVE_LOCAL_MEMBER
	{	ERROR_DS_GLOBAL_CANT_HAVE_LOCAL_MEMBER,                   "ERROR_DS_GLOBAL_CANT_HAVE_LOCAL_MEMBER",
		"A global group cannot have a local group as a member.",
		"A global group cannot have a local group as a member."
	},
#endif // #ifdef ERROR_DS_GLOBAL_CANT_HAVE_LOCAL_MEMBER
#ifdef ERROR_DS_GLOBAL_CANT_HAVE_UNIVERSAL_MEMBER
	{	ERROR_DS_GLOBAL_CANT_HAVE_UNIVERSAL_MEMBER,               "ERROR_DS_GLOBAL_CANT_HAVE_UNIVERSAL_MEMBER",
		"A global group cannot have a universal group as a member.",
		"A global group cannot have a universal group as a member."
	},
#endif // #ifdef ERROR_DS_GLOBAL_CANT_HAVE_UNIVERSAL_MEMBER
#ifdef ERROR_DS_UNIVERSAL_CANT_HAVE_LOCAL_MEMBER
	{	ERROR_DS_UNIVERSAL_CANT_HAVE_LOCAL_MEMBER,                "ERROR_DS_UNIVERSAL_CANT_HAVE_LOCAL_MEMBER",
		"A universal group cannot have a local group as a member.",
		"A universal group cannot have a local group as a member."
	},
#endif // #ifdef ERROR_DS_UNIVERSAL_CANT_HAVE_LOCAL_MEMBER
#ifdef ERROR_DS_GLOBAL_CANT_HAVE_CROSSDOMAIN_MEMBER
	{	ERROR_DS_GLOBAL_CANT_HAVE_CROSSDOMAIN_MEMBER,             "ERROR_DS_GLOBAL_CANT_HAVE_CROSSDOMAIN_MEMBER",
		"A global group cannot have a cross-domain member.",
		"A global group cannot have a cross-domain member."
	},
#endif // #ifdef ERROR_DS_GLOBAL_CANT_HAVE_CROSSDOMAIN_MEMBER
#ifdef ERROR_DS_LOCAL_CANT_HAVE_CROSSDOMAIN_LOCAL_MEMBER
	{	ERROR_DS_LOCAL_CANT_HAVE_CROSSDOMAIN_LOCAL_MEMBER,        "ERROR_DS_LOCAL_CANT_HAVE_CROSSDOMAIN_LOCAL_MEMBER",
		"A local group cannot have another cross-domain local group as a member.",
		"A local group cannot have another cross-domain local group as a member."
	},
#endif // #ifdef ERROR_DS_LOCAL_CANT_HAVE_CROSSDOMAIN_LOCAL_MEMBER
#ifdef ERROR_DS_HAVE_PRIMARY_MEMBERS
	{	ERROR_DS_HAVE_PRIMARY_MEMBERS,                            "ERROR_DS_HAVE_PRIMARY_MEMBERS",
		"A group with primary members cannot change to a security-disabled group.",
		"A group with primary members cannot change to a security-disabled group."
	},
#endif // #ifdef ERROR_DS_HAVE_PRIMARY_MEMBERS
#ifdef ERROR_DS_STRING_SD_CONVERSION_FAILED
	{	ERROR_DS_STRING_SD_CONVERSION_FAILED,                     "ERROR_DS_STRING_SD_CONVERSION_FAILED",
		"The schema cache load failed to convert the string default SD on a class-schema object.",
		"The schema cache load failed to convert the string default SD on a class-schema object."
	},
#endif // #ifdef ERROR_DS_STRING_SD_CONVERSION_FAILED
#ifdef ERROR_DS_NAMING_MASTER_GC
	{	ERROR_DS_NAMING_MASTER_GC,                                "ERROR_DS_NAMING_MASTER_GC",
		"Only DSAs configured to be Global Catalog servers should be allowed to hold the Domain Naming Master FSMO role. (Applies only to Windows 2000 servers)",
		"Only DSAs configured to be Global Catalog servers should be allowed to hold the Domain Naming Master FSMO role. (Applies only to Windows 2000 servers)"
	},
#endif // #ifdef ERROR_DS_NAMING_MASTER_GC
#ifdef ERROR_DS_LOOKUP_FAILURE
	{	ERROR_DS_LOOKUP_FAILURE,                                  "ERROR_DS_LOOKUP_FAILURE",
		"The DSA operation is unable to proceed because of a DNS lookup failure.",
		"The DSA operation is unable to proceed because of a DNS lookup failure."
	},
#endif // #ifdef ERROR_DS_LOOKUP_FAILURE
#ifdef ERROR_DS_COULDNT_UPDATE_SPNS
	{	ERROR_DS_COULDNT_UPDATE_SPNS,                             "ERROR_DS_COULDNT_UPDATE_SPNS",
		"While processing a change to the DNS Host Name for an object, the Service Principal Name values could not be kept in sync.",
		"While processing a change to the DNS Host Name for an object, the Service Principal Name values could not be kept in sync."
	},
#endif // #ifdef ERROR_DS_COULDNT_UPDATE_SPNS
#ifdef ERROR_DS_CANT_RETRIEVE_SD
	{	ERROR_DS_CANT_RETRIEVE_SD,                                "ERROR_DS_CANT_RETRIEVE_SD",
		"The Security Descriptor attribute could not be read.",
		"The Security Descriptor attribute could not be read."
	},
#endif // #ifdef ERROR_DS_CANT_RETRIEVE_SD
#ifdef ERROR_DS_KEY_NOT_UNIQUE
	{	ERROR_DS_KEY_NOT_UNIQUE,                                  "ERROR_DS_KEY_NOT_UNIQUE",
		"The object requested was not found, but an object with that key was found.",
		"The object requested was not found, but an object with that key was found."
	},
#endif // #ifdef ERROR_DS_KEY_NOT_UNIQUE
#ifdef ERROR_DS_WRONG_LINKED_ATT_SYNTAX
	{	ERROR_DS_WRONG_LINKED_ATT_SYNTAX,                         "ERROR_DS_WRONG_LINKED_ATT_SYNTAX",
		"The syntax of the linked attributed being added is incorrect. Forward links can only have syntax 2.5.5.1, 2.5.5.7, and 2.5.5.14, and backlinks can only have syntax 2.5.5.1.",
		"The syntax of the linked attributed being added is incorrect. Forward links can only have syntax 2.5.5.1, 2.5.5.7, and 2.5.5.14, and backlinks can only have syntax 2.5.5.1."
	},
#endif // #ifdef ERROR_DS_WRONG_LINKED_ATT_SYNTAX
#ifdef ERROR_DS_SAM_NEED_BOOTKEY_PASSWORD
	{	ERROR_DS_SAM_NEED_BOOTKEY_PASSWORD,                       "ERROR_DS_SAM_NEED_BOOTKEY_PASSWORD",
		"Security Account Manager needs to get the boot password.",
		"Security Account Manager needs to get the boot password."
	},
#endif // #ifdef ERROR_DS_SAM_NEED_BOOTKEY_PASSWORD
#ifdef ERROR_DS_SAM_NEED_BOOTKEY_FLOPPY
	{	ERROR_DS_SAM_NEED_BOOTKEY_FLOPPY,                         "ERROR_DS_SAM_NEED_BOOTKEY_FLOPPY",
		"Security Account Manager needs to get the boot key from floppy disk.",
		"Security Account Manager needs to get the boot key from floppy disk."
	},
#endif // #ifdef ERROR_DS_SAM_NEED_BOOTKEY_FLOPPY
#ifdef ERROR_DS_CANT_START
	{	ERROR_DS_CANT_START,                                      "ERROR_DS_CANT_START",
		"Directory Service cannot start.",
		"Directory Service cannot start."
	},
#endif // #ifdef ERROR_DS_CANT_START
#ifdef ERROR_DS_INIT_FAILURE
	{	ERROR_DS_INIT_FAILURE,                                    "ERROR_DS_INIT_FAILURE",
		"Directory Services could not start.",
		"Directory Services could not start."
	},
#endif // #ifdef ERROR_DS_INIT_FAILURE
#ifdef ERROR_DS_NO_PKT_PRIVACY_ON_CONNECTION
	{	ERROR_DS_NO_PKT_PRIVACY_ON_CONNECTION,                    "ERROR_DS_NO_PKT_PRIVACY_ON_CONNECTION",
		"The connection between client and server requires packet privacy or better.",
		"The connection between client and server requires packet privacy or better."
	},
#endif // #ifdef ERROR_DS_NO_PKT_PRIVACY_ON_CONNECTION
#ifdef ERROR_DS_SOURCE_DOMAIN_IN_FOREST
	{	ERROR_DS_SOURCE_DOMAIN_IN_FOREST,                         "ERROR_DS_SOURCE_DOMAIN_IN_FOREST",
		"The source domain may not be in the same forest as destination.",
		"The source domain may not be in the same forest as destination."
	},
#endif // #ifdef ERROR_DS_SOURCE_DOMAIN_IN_FOREST
#ifdef ERROR_DS_DESTINATION_DOMAIN_NOT_IN_FOREST
	{	ERROR_DS_DESTINATION_DOMAIN_NOT_IN_FOREST,                "ERROR_DS_DESTINATION_DOMAIN_NOT_IN_FOREST",
		"The destination domain must be in the forest.",
		"The destination domain must be in the forest."
	},
#endif // #ifdef ERROR_DS_DESTINATION_DOMAIN_NOT_IN_FOREST
#ifdef ERROR_DS_DESTINATION_AUDITING_NOT_ENABLED
	{	ERROR_DS_DESTINATION_AUDITING_NOT_ENABLED,                "ERROR_DS_DESTINATION_AUDITING_NOT_ENABLED",
		"The operation requires that destination domain auditing be enabled.",
		"The operation requires that destination domain auditing be enabled."
	},
#endif // #ifdef ERROR_DS_DESTINATION_AUDITING_NOT_ENABLED
#ifdef ERROR_DS_CANT_FIND_DC_FOR_SRC_DOMAIN
	{	ERROR_DS_CANT_FIND_DC_FOR_SRC_DOMAIN,                     "ERROR_DS_CANT_FIND_DC_FOR_SRC_DOMAIN",
		"The operation couldn't locate a DC for the source domain.",
		"The operation couldn't locate a DC for the source domain."
	},
#endif // #ifdef ERROR_DS_CANT_FIND_DC_FOR_SRC_DOMAIN
#ifdef ERROR_DS_SRC_OBJ_NOT_GROUP_OR_USER
	{	ERROR_DS_SRC_OBJ_NOT_GROUP_OR_USER,                       "ERROR_DS_SRC_OBJ_NOT_GROUP_OR_USER",
		"The source object must be a group or user.",
		"The source object must be a group or user."
	},
#endif // #ifdef ERROR_DS_SRC_OBJ_NOT_GROUP_OR_USER
#ifdef ERROR_DS_SRC_SID_EXISTS_IN_FOREST
	{	ERROR_DS_SRC_SID_EXISTS_IN_FOREST,                        "ERROR_DS_SRC_SID_EXISTS_IN_FOREST",
		"The source object's SID already exists in destination forest.",
		"The source object's SID already exists in destination forest."
	},
#endif // #ifdef ERROR_DS_SRC_SID_EXISTS_IN_FOREST
#ifdef ERROR_DS_SRC_AND_DST_OBJECT_CLASS_MISMATCH
	{	ERROR_DS_SRC_AND_DST_OBJECT_CLASS_MISMATCH,               "ERROR_DS_SRC_AND_DST_OBJECT_CLASS_MISMATCH",
		"The source and destination object must be of the same type.",
		"The source and destination object must be of the same type."
	},
#endif // #ifdef ERROR_DS_SRC_AND_DST_OBJECT_CLASS_MISMATCH
#ifdef ERROR_SAM_INIT_FAILURE
	{	ERROR_SAM_INIT_FAILURE,                                   "ERROR_SAM_INIT_FAILURE",
		"Security Accounts Manager initialization failed because of the following error: %1. Error Status: 0x%2. Click OK to shut down the system and reboot into Safe Mode. Check the event log for detailed information.",
		"Security Accounts Manager initialization failed because of the following error: %1. Error Status: 0x%2. Click OK to shut down the system and reboot into Safe Mode. Check the event log for detailed information."
	},
#endif // #ifdef ERROR_SAM_INIT_FAILURE
#ifdef ERROR_DS_DRA_SCHEMA_INFO_SHIP
	{	ERROR_DS_DRA_SCHEMA_INFO_SHIP,                            "ERROR_DS_DRA_SCHEMA_INFO_SHIP",
		"Schema information could not be included in the replication request.",
		"Schema information could not be included in the replication request."
	},
#endif // #ifdef ERROR_DS_DRA_SCHEMA_INFO_SHIP
#ifdef ERROR_DS_DRA_SCHEMA_CONFLICT
	{	ERROR_DS_DRA_SCHEMA_CONFLICT,                             "ERROR_DS_DRA_SCHEMA_CONFLICT",
		"The replication operation could not be completed due to a schema incompatibility.",
		"The replication operation could not be completed due to a schema incompatibility."
	},
#endif // #ifdef ERROR_DS_DRA_SCHEMA_CONFLICT
#ifdef ERROR_DS_DRA_EARLIER_SCHEMA_CONLICT
	{	ERROR_DS_DRA_EARLIER_SCHEMA_CONLICT,                      "ERROR_DS_DRA_EARLIER_SCHEMA_CONLICT",
		"The replication operation could not be completed due to a previous schema incompatibility.",
		"The replication operation could not be completed due to a previous schema incompatibility."
	},
#endif // #ifdef ERROR_DS_DRA_EARLIER_SCHEMA_CONLICT
#ifdef ERROR_DS_DRA_OBJ_NC_MISMATCH
	{	ERROR_DS_DRA_OBJ_NC_MISMATCH,                             "ERROR_DS_DRA_OBJ_NC_MISMATCH",
		"The replication update could not be applied because either the source or the destination has not yet received information regarding a recent cross-domain move operation.",
		"The replication update could not be applied because either the source or the destination has not yet received information regarding a recent cross-domain move operation."
	},
#endif // #ifdef ERROR_DS_DRA_OBJ_NC_MISMATCH
#ifdef ERROR_DS_NC_STILL_HAS_DSAS
	{	ERROR_DS_NC_STILL_HAS_DSAS,                               "ERROR_DS_NC_STILL_HAS_DSAS",
		"The requested domain could not be deleted because there exist domain controllers that still host this domain.",
		"The requested domain could not be deleted because there exist domain controllers that still host this domain."
	},
#endif // #ifdef ERROR_DS_NC_STILL_HAS_DSAS
#ifdef ERROR_DS_GC_REQUIRED
	{	ERROR_DS_GC_REQUIRED,                                     "ERROR_DS_GC_REQUIRED",
		"The requested operation can be performed only on a global catalog server.",
		"The requested operation can be performed only on a global catalog server."
	},
#endif // #ifdef ERROR_DS_GC_REQUIRED
#ifdef ERROR_DS_LOCAL_MEMBER_OF_LOCAL_ONLY
	{	ERROR_DS_LOCAL_MEMBER_OF_LOCAL_ONLY,                      "ERROR_DS_LOCAL_MEMBER_OF_LOCAL_ONLY",
		"A local group can only be a member of other local groups in the same domain.",
		"A local group can only be a member of other local groups in the same domain."
	},
#endif // #ifdef ERROR_DS_LOCAL_MEMBER_OF_LOCAL_ONLY
#ifdef ERROR_DS_NO_FPO_IN_UNIVERSAL_GROUPS
	{	ERROR_DS_NO_FPO_IN_UNIVERSAL_GROUPS,                      "ERROR_DS_NO_FPO_IN_UNIVERSAL_GROUPS",
		"Foreign security principals cannot be members of universal groups.",
		"Foreign security principals cannot be members of universal groups."
	},
#endif // #ifdef ERROR_DS_NO_FPO_IN_UNIVERSAL_GROUPS
#ifdef ERROR_DS_CANT_ADD_TO_GC
	{	ERROR_DS_CANT_ADD_TO_GC,                                  "ERROR_DS_CANT_ADD_TO_GC",
		"The attribute is not allowed to be replicated to the GC because of security reasons.",
		"The attribute is not allowed to be replicated to the GC because of security reasons."
	},
#endif // #ifdef ERROR_DS_CANT_ADD_TO_GC
#ifdef ERROR_DS_NO_CHECKPOINT_WITH_PDC
	{	ERROR_DS_NO_CHECKPOINT_WITH_PDC,                          "ERROR_DS_NO_CHECKPOINT_WITH_PDC",
		"The checkpoint with the PDC could not be taken because there are too many modifications being processed currently.",
		"The checkpoint with the PDC could not be taken because there are too many modifications being processed currently."
	},
#endif // #ifdef ERROR_DS_NO_CHECKPOINT_WITH_PDC
#ifdef ERROR_DS_SOURCE_AUDITING_NOT_ENABLED
	{	ERROR_DS_SOURCE_AUDITING_NOT_ENABLED,                     "ERROR_DS_SOURCE_AUDITING_NOT_ENABLED",
		"The operation requires that source domain auditing be enabled.",
		"The operation requires that source domain auditing be enabled."
	},
#endif // #ifdef ERROR_DS_SOURCE_AUDITING_NOT_ENABLED
#ifdef ERROR_DS_CANT_CREATE_IN_NONDOMAIN_NC
	{	ERROR_DS_CANT_CREATE_IN_NONDOMAIN_NC,                     "ERROR_DS_CANT_CREATE_IN_NONDOMAIN_NC",
		"Security principal objects can only be created inside domain naming contexts.",
		"Security principal objects can only be created inside domain naming contexts."
	},
#endif // #ifdef ERROR_DS_CANT_CREATE_IN_NONDOMAIN_NC
#ifdef ERROR_DS_INVALID_NAME_FOR_SPN
	{	ERROR_DS_INVALID_NAME_FOR_SPN,                            "ERROR_DS_INVALID_NAME_FOR_SPN",
		"A Service Principal Name (SPN) could not be constructed because the provided hostname is not in the necessary format.",
		"A Service Principal Name (SPN) could not be constructed because the provided hostname is not in the necessary format."
	},
#endif // #ifdef ERROR_DS_INVALID_NAME_FOR_SPN
#ifdef ERROR_DS_FILTER_USES_CONTRUCTED_ATTRS
	{	ERROR_DS_FILTER_USES_CONTRUCTED_ATTRS,                    "ERROR_DS_FILTER_USES_CONTRUCTED_ATTRS",
		"A Filter was passed that uses constructed attributes.",
		"A Filter was passed that uses constructed attributes."
	},
#endif // #ifdef ERROR_DS_FILTER_USES_CONTRUCTED_ATTRS
#ifdef ERROR_DS_UNICODEPWD_NOT_IN_QUOTES
	{	ERROR_DS_UNICODEPWD_NOT_IN_QUOTES,                        "ERROR_DS_UNICODEPWD_NOT_IN_QUOTES",
		"The unicodePwd attribute value must be enclosed in double quotes.",
		"The unicodePwd attribute value must be enclosed in double quotes."
	},
#endif // #ifdef ERROR_DS_UNICODEPWD_NOT_IN_QUOTES
#ifdef ERROR_DS_MACHINE_ACCOUNT_QUOTA_EXCEEDED
	{	ERROR_DS_MACHINE_ACCOUNT_QUOTA_EXCEEDED,                  "ERROR_DS_MACHINE_ACCOUNT_QUOTA_EXCEEDED",
		"Your computer could not be joined to the domain. You have exceeded the maximum number of computer accounts you are allowed to create in this domain. Contact your system administrator to have this limit reset or increased.",
		"Your computer could not be joined to the domain. You have exceeded the maximum number of computer accounts you are allowed to create in this domain. Contact your system administrator to have this limit reset or increased."
	},
#endif // #ifdef ERROR_DS_MACHINE_ACCOUNT_QUOTA_EXCEEDED
#ifdef ERROR_DS_MUST_BE_RUN_ON_DST_DC
	{	ERROR_DS_MUST_BE_RUN_ON_DST_DC,                           "ERROR_DS_MUST_BE_RUN_ON_DST_DC",
		"For security reasons, the operation must be run on the destination DC.",
		"For security reasons, the operation must be run on the destination DC."
	},
#endif // #ifdef ERROR_DS_MUST_BE_RUN_ON_DST_DC
#ifdef ERROR_DS_SRC_DC_MUST_BE_SP4_OR_GREATER
	{	ERROR_DS_SRC_DC_MUST_BE_SP4_OR_GREATER,                   "ERROR_DS_SRC_DC_MUST_BE_SP4_OR_GREATER",
		"For security reasons, the source DC must be NT4SP4 or greater.",
		"For security reasons, the source DC must be NT4SP4 or greater."
	},
#endif // #ifdef ERROR_DS_SRC_DC_MUST_BE_SP4_OR_GREATER
#ifdef ERROR_DS_CANT_TREE_DELETE_CRITICAL_OBJ
	{	ERROR_DS_CANT_TREE_DELETE_CRITICAL_OBJ,                   "ERROR_DS_CANT_TREE_DELETE_CRITICAL_OBJ",
		"Critical Directory Service System objects cannot be deleted during tree delete operations. The tree delete may have been partially performed.",
		"Critical Directory Service System objects cannot be deleted during tree delete operations. The tree delete may have been partially performed."
	},
#endif // #ifdef ERROR_DS_CANT_TREE_DELETE_CRITICAL_OBJ
#ifdef ERROR_DS_INIT_FAILURE_CONSOLE
	{	ERROR_DS_INIT_FAILURE_CONSOLE,                            "ERROR_DS_INIT_FAILURE_CONSOLE",
		"Directory Services could not start because of the following error: %1. Error Status: 0x%2. Please click OK to shutdown the system. You can use the recovery console to diagnose the system further.",
		"Directory Services could not start because of the following error: %1. Error Status: 0x%2. Please click OK to shutdown the system. You can use the recovery console to diagnose the system further."
	},
#endif // #ifdef ERROR_DS_INIT_FAILURE_CONSOLE
#ifdef ERROR_DS_SAM_INIT_FAILURE_CONSOLE
	{	ERROR_DS_SAM_INIT_FAILURE_CONSOLE,                        "ERROR_DS_SAM_INIT_FAILURE_CONSOLE",
		"Security Accounts Manager initialization failed because of the following error: %1. Error Status: 0x%2. Please click OK to shutdown the system. You can use the recovery console to diagnose the system further.",
		"Security Accounts Manager initialization failed because of the following error: %1. Error Status: 0x%2. Please click OK to shutdown the system. You can use the recovery console to diagnose the system further."
	},
#endif // #ifdef ERROR_DS_SAM_INIT_FAILURE_CONSOLE
#ifdef ERROR_DS_FOREST_VERSION_TOO_HIGH
	{	ERROR_DS_FOREST_VERSION_TOO_HIGH,                         "ERROR_DS_FOREST_VERSION_TOO_HIGH",
		"The version of the operating system installed is incompatible with the current forest functional level. You must upgrade to a new version of the operating system before this server can become a domain controller in this forest.",
		"The version of the operating system installed is incompatible with the current forest functional level. You must upgrade to a new version of the operating system before this server can become a domain controller in this forest."
	},
#endif // #ifdef ERROR_DS_FOREST_VERSION_TOO_HIGH
#ifdef ERROR_DS_DOMAIN_VERSION_TOO_HIGH
	{	ERROR_DS_DOMAIN_VERSION_TOO_HIGH,                         "ERROR_DS_DOMAIN_VERSION_TOO_HIGH",
		"The version of the operating system installed is incompatible with the current domain functional level. You must upgrade to a new version of the operating system before this server can become a domain controller in this domain.",
		"The version of the operating system installed is incompatible with the current domain functional level. You must upgrade to a new version of the operating system before this server can become a domain controller in this domain."
	},
#endif // #ifdef ERROR_DS_DOMAIN_VERSION_TOO_HIGH
#ifdef ERROR_DS_FOREST_VERSION_TOO_LOW
	{	ERROR_DS_FOREST_VERSION_TOO_LOW,                          "ERROR_DS_FOREST_VERSION_TOO_LOW",
		"This version of the operating system installed on this server no longer supports the current forest functional level. You must raise the forest functional level before this server can become a domain controller in this forest.",
		"This version of the operating system installed on this server no longer supports the current forest functional level. You must raise the forest functional level before this server can become a domain controller in this forest."
	},
#endif // #ifdef ERROR_DS_FOREST_VERSION_TOO_LOW
#ifdef ERROR_DS_DOMAIN_VERSION_TOO_LOW
	{	ERROR_DS_DOMAIN_VERSION_TOO_LOW,                          "ERROR_DS_DOMAIN_VERSION_TOO_LOW",
		"This version of the operating system installed on this server no longer supports the current domain functional level. You must raise the domain functional level before this server can become a domain controller in this domain.",
		"This version of the operating system installed on this server no longer supports the current domain functional level. You must raise the domain functional level before this server can become a domain controller in this domain."
	},
#endif // #ifdef ERROR_DS_DOMAIN_VERSION_TOO_LOW
#ifdef ERROR_DS_INCOMPATIBLE_VERSION
	{	ERROR_DS_INCOMPATIBLE_VERSION,                            "ERROR_DS_INCOMPATIBLE_VERSION",
		"The version of the operating system installed on this server is incompatible with the functional level of the domain or forest.",
		"The version of the operating system installed on this server is incompatible with the functional level of the domain or forest."
	},
#endif // #ifdef ERROR_DS_INCOMPATIBLE_VERSION
#ifdef ERROR_DS_LOW_DSA_VERSION
	{	ERROR_DS_LOW_DSA_VERSION,                                 "ERROR_DS_LOW_DSA_VERSION",
		"The functional level of the domain (or forest) cannot be raised to the requested value, because there exist one or more domain controllers in the domain (or forest) that are at a lower incompatible functional level.",
		"The functional level of the domain (or forest) cannot be raised to the requested value, because there exist one or more domain controllers in the domain (or forest) that are at a lower incompatible functional level."
	},
#endif // #ifdef ERROR_DS_LOW_DSA_VERSION
#ifdef ERROR_DS_NO_BEHAVIOR_VERSION_IN_MIXEDDOMAIN
	{	ERROR_DS_NO_BEHAVIOR_VERSION_IN_MIXEDDOMAIN,              "ERROR_DS_NO_BEHAVIOR_VERSION_IN_MIXEDDOMAIN",
		"The forest functional level cannot be raised to the requested level since one or more domains are still in mixed domain mode. All domains in the forest must be in native mode before you can raise the forest functional level.",
		"The forest functional level cannot be raised to the requested level since one or more domains are still in mixed domain mode. All domains in the forest must be in native mode before you can raise the forest functional level."
	},
#endif // #ifdef ERROR_DS_NO_BEHAVIOR_VERSION_IN_MIXEDDOMAIN
#ifdef ERROR_DS_NOT_SUPPORTED_SORT_ORDER
	{	ERROR_DS_NOT_SUPPORTED_SORT_ORDER,                        "ERROR_DS_NOT_SUPPORTED_SORT_ORDER",
		"The sort order requested is not supported.",
		"The sort order requested is not supported."
	},
#endif // #ifdef ERROR_DS_NOT_SUPPORTED_SORT_ORDER
#ifdef ERROR_DS_NAME_NOT_UNIQUE
	{	ERROR_DS_NAME_NOT_UNIQUE,                                 "ERROR_DS_NAME_NOT_UNIQUE",
		"The requested name already exists as a unique identifier.",
		"The requested name already exists as a unique identifier."
	},
#endif // #ifdef ERROR_DS_NAME_NOT_UNIQUE
#ifdef ERROR_DS_MACHINE_ACCOUNT_CREATED_PRENT4
	{	ERROR_DS_MACHINE_ACCOUNT_CREATED_PRENT4,                  "ERROR_DS_MACHINE_ACCOUNT_CREATED_PRENT4",
		"The machine account was created pre-NT4. The account needs to be recreated.",
		"The machine account was created pre-NT4. The account needs to be recreated."
	},
#endif // #ifdef ERROR_DS_MACHINE_ACCOUNT_CREATED_PRENT4
#ifdef ERROR_DS_OUT_OF_VERSION_STORE
	{	ERROR_DS_OUT_OF_VERSION_STORE,                            "ERROR_DS_OUT_OF_VERSION_STORE",
		"The database is out of version store.",
		"The database is out of version store."
	},
#endif // #ifdef ERROR_DS_OUT_OF_VERSION_STORE
#ifdef ERROR_DS_INCOMPATIBLE_CONTROLS_USED
	{	ERROR_DS_INCOMPATIBLE_CONTROLS_USED,                      "ERROR_DS_INCOMPATIBLE_CONTROLS_USED",
		"Unable to continue operation because multiple conflicting controls were used.",
		"Unable to continue operation because multiple conflicting controls were used."
	},
#endif // #ifdef ERROR_DS_INCOMPATIBLE_CONTROLS_USED
#ifdef ERROR_DS_NO_REF_DOMAIN
	{	ERROR_DS_NO_REF_DOMAIN,                                   "ERROR_DS_NO_REF_DOMAIN",
		"Unable to find a valid security descriptor reference domain for this partition.",
		"Unable to find a valid security descriptor reference domain for this partition."
	},
#endif // #ifdef ERROR_DS_NO_REF_DOMAIN
#ifdef ERROR_DS_RESERVED_LINK_ID
	{	ERROR_DS_RESERVED_LINK_ID,                                "ERROR_DS_RESERVED_LINK_ID",
		"Schema update failed: The link identifier is reserved.",
		"Schema update failed: The link identifier is reserved."
	},
#endif // #ifdef ERROR_DS_RESERVED_LINK_ID
#ifdef ERROR_DS_LINK_ID_NOT_AVAILABLE
	{	ERROR_DS_LINK_ID_NOT_AVAILABLE,                           "ERROR_DS_LINK_ID_NOT_AVAILABLE",
		"Schema update failed: There are no link identifiers available.",
		"Schema update failed: There are no link identifiers available."
	},
#endif // #ifdef ERROR_DS_LINK_ID_NOT_AVAILABLE
#ifdef ERROR_DS_AG_CANT_HAVE_UNIVERSAL_MEMBER
	{	ERROR_DS_AG_CANT_HAVE_UNIVERSAL_MEMBER,                   "ERROR_DS_AG_CANT_HAVE_UNIVERSAL_MEMBER",
		"An account group cannot have a universal group as a member.",
		"An account group cannot have a universal group as a member."
	},
#endif // #ifdef ERROR_DS_AG_CANT_HAVE_UNIVERSAL_MEMBER
#ifdef ERROR_DS_MODIFYDN_DISALLOWED_BY_INSTANCE_TYPE
	{	ERROR_DS_MODIFYDN_DISALLOWED_BY_INSTANCE_TYPE,            "ERROR_DS_MODIFYDN_DISALLOWED_BY_INSTANCE_TYPE",
		"Rename or move operations on naming context heads or read-only objects are not allowed.",
		"Rename or move operations on naming context heads or read-only objects are not allowed."
	},
#endif // #ifdef ERROR_DS_MODIFYDN_DISALLOWED_BY_INSTANCE_TYPE
#ifdef ERROR_DS_NO_OBJECT_MOVE_IN_SCHEMA_NC
	{	ERROR_DS_NO_OBJECT_MOVE_IN_SCHEMA_NC,                     "ERROR_DS_NO_OBJECT_MOVE_IN_SCHEMA_NC",
		"Move operations on objects in the schema naming context are not allowed.",
		"Move operations on objects in the schema naming context are not allowed."
	},
#endif // #ifdef ERROR_DS_NO_OBJECT_MOVE_IN_SCHEMA_NC
#ifdef ERROR_DS_MODIFYDN_DISALLOWED_BY_FLAG
	{	ERROR_DS_MODIFYDN_DISALLOWED_BY_FLAG,                     "ERROR_DS_MODIFYDN_DISALLOWED_BY_FLAG",
		"A system flag has been set on the object and does not allow the object to be moved or renamed.",
		"A system flag has been set on the object and does not allow the object to be moved or renamed."
	},
#endif // #ifdef ERROR_DS_MODIFYDN_DISALLOWED_BY_FLAG
#ifdef ERROR_DS_MODIFYDN_WRONG_GRANDPARENT
	{	ERROR_DS_MODIFYDN_WRONG_GRANDPARENT,                      "ERROR_DS_MODIFYDN_WRONG_GRANDPARENT",
		"This object is not allowed to change its grandparent container. Moves are not forbidden on this object, but are restricted to sibling containers.",
		"This object is not allowed to change its grandparent container. Moves are not forbidden on this object, but are restricted to sibling containers."
	},
#endif // #ifdef ERROR_DS_MODIFYDN_WRONG_GRANDPARENT
#ifdef ERROR_DS_NAME_ERROR_TRUST_REFERRAL
	{	ERROR_DS_NAME_ERROR_TRUST_REFERRAL,                       "ERROR_DS_NAME_ERROR_TRUST_REFERRAL",
		"Unable to resolve completely, a referral to another forest is generated.",
		"Unable to resolve completely, a referral to another forest is generated."
	},
#endif // #ifdef ERROR_DS_NAME_ERROR_TRUST_REFERRAL
#ifdef ERROR_NOT_SUPPORTED_ON_STANDARD_SERVER
	{	ERROR_NOT_SUPPORTED_ON_STANDARD_SERVER,                   "ERROR_NOT_SUPPORTED_ON_STANDARD_SERVER",
		"The requested action is not supported on standard server.",
		"The requested action is not supported on standard server."
	},
#endif // #ifdef ERROR_NOT_SUPPORTED_ON_STANDARD_SERVER
#ifdef ERROR_DS_CANT_ACCESS_REMOTE_PART_OF_AD
	{	ERROR_DS_CANT_ACCESS_REMOTE_PART_OF_AD,                   "ERROR_DS_CANT_ACCESS_REMOTE_PART_OF_AD",
		"Could not access a partition of the Active Directory located on a remote server. Make sure at least one server is running for the partition in question.",
		"Could not access a partition of the Active Directory located on a remote server. Make sure at least one server is running for the partition in question."
	},
#endif // #ifdef ERROR_DS_CANT_ACCESS_REMOTE_PART_OF_AD
#ifdef ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE_V2
	{	ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE_V2,                    "ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE_V2",
		"The directory cannot validate the proposed naming context (or partition) name because it does not hold a replica nor can it contact a replica of the naming context above the proposed naming context. Please ensure that the parent naming context is properly registered in DNS, and at least one replica of this naming context is reachable by the Domain Naming master.",
		"The directory cannot validate the proposed naming context (or partition) name because it does not hold a replica nor can it contact a replica of the naming context above the proposed naming context. Please ensure that the parent naming context is properly registered in DNS, and at least one replica of this naming context is reachable by the Domain Naming master."
	},
#endif // #ifdef ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE_V2
#ifdef ERROR_DS_THREAD_LIMIT_EXCEEDED
	{	ERROR_DS_THREAD_LIMIT_EXCEEDED,                           "ERROR_DS_THREAD_LIMIT_EXCEEDED",
		"The thread limit for this request was exceeded.",
		"The thread limit for this request was exceeded."
	},
#endif // #ifdef ERROR_DS_THREAD_LIMIT_EXCEEDED
#ifdef ERROR_DS_NOT_CLOSEST
	{	ERROR_DS_NOT_CLOSEST,                                     "ERROR_DS_NOT_CLOSEST",
		"The Global catalog server is not in the closet site.",
		"The Global catalog server is not in the closet site."
	},
#endif // #ifdef ERROR_DS_NOT_CLOSEST
#ifdef ERROR_DS_CANT_DERIVE_SPN_WITHOUT_SERVER_REF
	{	ERROR_DS_CANT_DERIVE_SPN_WITHOUT_SERVER_REF,              "ERROR_DS_CANT_DERIVE_SPN_WITHOUT_SERVER_REF",
		"The DS cannot derive a service principal name (SPN) with which to mutually authenticate the target server because the corresponding server object in the local DS database has no serverReference attribute.",
		"The DS cannot derive a service principal name (SPN) with which to mutually authenticate the target server because the corresponding server object in the local DS database has no serverReference attribute."
	},
#endif // #ifdef ERROR_DS_CANT_DERIVE_SPN_WITHOUT_SERVER_REF
#ifdef ERROR_DS_SINGLE_USER_MODE_FAILED
	{	ERROR_DS_SINGLE_USER_MODE_FAILED,                         "ERROR_DS_SINGLE_USER_MODE_FAILED",
		"The Directory Service failed to enter single user mode.",
		"The Directory Service failed to enter single user mode."
	},
#endif // #ifdef ERROR_DS_SINGLE_USER_MODE_FAILED
#ifdef ERROR_DS_NTDSCRIPT_SYNTAX_ERROR
	{	ERROR_DS_NTDSCRIPT_SYNTAX_ERROR,                          "ERROR_DS_NTDSCRIPT_SYNTAX_ERROR",
		"The Directory Service cannot parse the script because of a syntax error.",
		"The Directory Service cannot parse the script because of a syntax error."
	},
#endif // #ifdef ERROR_DS_NTDSCRIPT_SYNTAX_ERROR
#ifdef ERROR_DS_NTDSCRIPT_PROCESS_ERROR
	{	ERROR_DS_NTDSCRIPT_PROCESS_ERROR,                         "ERROR_DS_NTDSCRIPT_PROCESS_ERROR",
		"The Directory Service cannot process the script because of an error.",
		"The Directory Service cannot process the script because of an error."
	},
#endif // #ifdef ERROR_DS_NTDSCRIPT_PROCESS_ERROR
#ifdef ERROR_DS_DIFFERENT_REPL_EPOCHS
	{	ERROR_DS_DIFFERENT_REPL_EPOCHS,                           "ERROR_DS_DIFFERENT_REPL_EPOCHS",
		"The directory service cannot perform the requested operation because the servers involved are of different replication epochs (which is usually related to a domain rename that is in progress).",
		"The directory service cannot perform the requested operation because the servers involved are of different replication epochs (which is usually related to a domain rename that is in progress)."
	},
#endif // #ifdef ERROR_DS_DIFFERENT_REPL_EPOCHS
#ifdef ERROR_DS_DRS_EXTENSIONS_CHANGED
	{	ERROR_DS_DRS_EXTENSIONS_CHANGED,                          "ERROR_DS_DRS_EXTENSIONS_CHANGED",
		"The directory service binding must be renegotiated due to a change in the server extensions information.",
		"The directory service binding must be renegotiated due to a change in the server extensions information."
	},
#endif // #ifdef ERROR_DS_DRS_EXTENSIONS_CHANGED
#ifdef ERROR_DS_REPLICA_SET_CHANGE_NOT_ALLOWED_ON_DISABLED_CR
	{	ERROR_DS_REPLICA_SET_CHANGE_NOT_ALLOWED_ON_DISABLED_CR,   "ERROR_DS_REPLICA_SET_CHANGE_NOT_ALLOWED_ON_DISABLED_CR",
		"Operation not allowed on a disabled cross ref.",
		"Operation not allowed on a disabled cross ref."
	},
#endif // #ifdef ERROR_DS_REPLICA_SET_CHANGE_NOT_ALLOWED_ON_DISABLED_CR
#ifdef ERROR_DS_NO_MSDS_INTID
	{	ERROR_DS_NO_MSDS_INTID,                                   "ERROR_DS_NO_MSDS_INTID",
		"Schema update failed: No values for msDS-IntId are available.",
		"Schema update failed: No values for msDS-IntId are available."
	},
#endif // #ifdef ERROR_DS_NO_MSDS_INTID
#ifdef ERROR_DS_DUP_MSDS_INTID
	{	ERROR_DS_DUP_MSDS_INTID,                                  "ERROR_DS_DUP_MSDS_INTID",
		"Schema update failed: Duplicate msDS-INtId. Retry the operation.",
		"Schema update failed: Duplicate msDS-INtId. Retry the operation."
	},
#endif // #ifdef ERROR_DS_DUP_MSDS_INTID
#ifdef ERROR_DS_EXISTS_IN_RDNATTID
	{	ERROR_DS_EXISTS_IN_RDNATTID,                              "ERROR_DS_EXISTS_IN_RDNATTID",
		"Schema deletion failed: attribute is used in rDNAttID.",
		"Schema deletion failed: attribute is used in rDNAttID."
	},
#endif // #ifdef ERROR_DS_EXISTS_IN_RDNATTID
#ifdef ERROR_DS_AUTHORIZATION_FAILED
	{	ERROR_DS_AUTHORIZATION_FAILED,                            "ERROR_DS_AUTHORIZATION_FAILED",
		"The directory service failed to authorize the request.",
		"The directory service failed to authorize the request."
	},
#endif // #ifdef ERROR_DS_AUTHORIZATION_FAILED
#ifdef ERROR_DS_INVALID_SCRIPT
	{	ERROR_DS_INVALID_SCRIPT,                                  "ERROR_DS_INVALID_SCRIPT",
		"The Directory Service cannot process the script because it is invalid.",
		"The Directory Service cannot process the script because it is invalid."
	},
#endif // #ifdef ERROR_DS_INVALID_SCRIPT
#ifdef ERROR_DS_REMOTE_CROSSREF_OP_FAILED
	{	ERROR_DS_REMOTE_CROSSREF_OP_FAILED,                       "ERROR_DS_REMOTE_CROSSREF_OP_FAILED",
		"The remote create cross reference operation failed on the Domain Naming Master FSMO. The operation's error is in the extended data.",
		"The remote create cross reference operation failed on the Domain Naming Master FSMO. The operation's error is in the extended data."
	},
#endif // #ifdef ERROR_DS_REMOTE_CROSSREF_OP_FAILED
#ifdef ERROR_DS_CROSS_REF_BUSY
	{	ERROR_DS_CROSS_REF_BUSY,                                  "ERROR_DS_CROSS_REF_BUSY",
		"A cross reference is in use locally with the same name.",
		"A cross reference is in use locally with the same name."
	},
#endif // #ifdef ERROR_DS_CROSS_REF_BUSY
#ifdef ERROR_DS_CANT_DERIVE_SPN_FOR_DELETED_DOMAIN
	{	ERROR_DS_CANT_DERIVE_SPN_FOR_DELETED_DOMAIN,              "ERROR_DS_CANT_DERIVE_SPN_FOR_DELETED_DOMAIN",
		"The DS cannot derive a service principal name (SPN) with which to mutually authenticate the target server because the server's domain has been deleted from the forest.",
		"The DS cannot derive a service principal name (SPN) with which to mutually authenticate the target server because the server's domain has been deleted from the forest."
	},
#endif // #ifdef ERROR_DS_CANT_DERIVE_SPN_FOR_DELETED_DOMAIN
#ifdef ERROR_DS_CANT_DEMOTE_WITH_WRITEABLE_NC
	{	ERROR_DS_CANT_DEMOTE_WITH_WRITEABLE_NC,                   "ERROR_DS_CANT_DEMOTE_WITH_WRITEABLE_NC",
		"Writeable NCs prevent this DC from demoting.",
		"Writeable NCs prevent this DC from demoting."
	},
#endif // #ifdef ERROR_DS_CANT_DEMOTE_WITH_WRITEABLE_NC
#ifdef ERROR_DS_DUPLICATE_ID_FOUND
	{	ERROR_DS_DUPLICATE_ID_FOUND,                              "ERROR_DS_DUPLICATE_ID_FOUND",
		"The requested object has a non-unique identifier and cannot be retrieved.",
		"The requested object has a non-unique identifier and cannot be retrieved."
	},
#endif // #ifdef ERROR_DS_DUPLICATE_ID_FOUND
#ifdef ERROR_DS_INSUFFICIENT_ATTR_TO_CREATE_OBJECT
	{	ERROR_DS_INSUFFICIENT_ATTR_TO_CREATE_OBJECT,              "ERROR_DS_INSUFFICIENT_ATTR_TO_CREATE_OBJECT",
		"Insufficient attributes were given to create an object. This object may not exist because it may have been deleted and already garbage collected.",
		"Insufficient attributes were given to create an object. This object may not exist because it may have been deleted and already garbage collected."
	},
#endif // #ifdef ERROR_DS_INSUFFICIENT_ATTR_TO_CREATE_OBJECT
#ifdef ERROR_DS_GROUP_CONVERSION_ERROR
	{	ERROR_DS_GROUP_CONVERSION_ERROR,                          "ERROR_DS_GROUP_CONVERSION_ERROR",
		"The group cannot be converted due to attribute restrictions on the requested group type.",
		"The group cannot be converted due to attribute restrictions on the requested group type."
	},
#endif // #ifdef ERROR_DS_GROUP_CONVERSION_ERROR
#ifdef ERROR_DS_CANT_MOVE_APP_BASIC_GROUP
	{	ERROR_DS_CANT_MOVE_APP_BASIC_GROUP,                       "ERROR_DS_CANT_MOVE_APP_BASIC_GROUP",
		"Cross-domain move of non-empty basic application groups is not allowed.",
		"Cross-domain move of non-empty basic application groups is not allowed."
	},
#endif // #ifdef ERROR_DS_CANT_MOVE_APP_BASIC_GROUP
#ifdef ERROR_DS_CANT_MOVE_APP_QUERY_GROUP
	{	ERROR_DS_CANT_MOVE_APP_QUERY_GROUP,                       "ERROR_DS_CANT_MOVE_APP_QUERY_GROUP",
		"Cross-domain move on non-empty query based application groups is not allowed.",
		"Cross-domain move on non-empty query based application groups is not allowed."
	},
#endif // #ifdef ERROR_DS_CANT_MOVE_APP_QUERY_GROUP
#ifdef ERROR_DS_ROLE_NOT_VERIFIED
	{	ERROR_DS_ROLE_NOT_VERIFIED,                               "ERROR_DS_ROLE_NOT_VERIFIED",
		"The role owner could not be verified because replication of its partition has not occurred recently.",
		"The role owner could not be verified because replication of its partition has not occurred recently."
	},
#endif // #ifdef ERROR_DS_ROLE_NOT_VERIFIED
#ifdef ERROR_DS_WKO_CONTAINER_CANNOT_BE_SPECIAL
	{	ERROR_DS_WKO_CONTAINER_CANNOT_BE_SPECIAL,                 "ERROR_DS_WKO_CONTAINER_CANNOT_BE_SPECIAL",
		"The target container for a redirection of a well-known object container cannot already be a special container.",
		"The target container for a redirection of a well-known object container cannot already be a special container."
	},
#endif // #ifdef ERROR_DS_WKO_CONTAINER_CANNOT_BE_SPECIAL
#ifdef ERROR_DS_DOMAIN_RENAME_IN_PROGRESS
	{	ERROR_DS_DOMAIN_RENAME_IN_PROGRESS,                       "ERROR_DS_DOMAIN_RENAME_IN_PROGRESS",
		"The Directory Service cannot perform the requested operation because a domain rename operation is in progress.",
		"The Directory Service cannot perform the requested operation because a domain rename operation is in progress."
	},
#endif // #ifdef ERROR_DS_DOMAIN_RENAME_IN_PROGRESS
#ifdef ERROR_DS_EXISTING_AD_CHILD_NC
	{	ERROR_DS_EXISTING_AD_CHILD_NC,                            "ERROR_DS_EXISTING_AD_CHILD_NC",
		"The Active Directory detected an Active Directory child partition below the requested new partition name. The Active Directory's partition hierarchy must be created in a top-down method.",
		"The Active Directory detected an Active Directory child partition below the requested new partition name. The Active Directory's partition hierarchy must be created in a top-down method."
	},
#endif // #ifdef ERROR_DS_EXISTING_AD_CHILD_NC
#ifdef DNS_ERROR_RCODE_FORMAT_ERROR
	{	DNS_ERROR_RCODE_FORMAT_ERROR,                             "DNS_ERROR_RCODE_FORMAT_ERROR",
		"DNS server unable to interpret format.",
		"DNS server unable to interpret format."
	},
#endif // #ifdef DNS_ERROR_RCODE_FORMAT_ERROR
#ifdef DNS_ERROR_RCODE_SERVER_FAILURE
	{	DNS_ERROR_RCODE_SERVER_FAILURE,                           "DNS_ERROR_RCODE_SERVER_FAILURE",
		"DNS server failure.",
		"DNS server failure."
	},
#endif // #ifdef DNS_ERROR_RCODE_SERVER_FAILURE
#ifdef DNS_ERROR_RCODE_NAME_ERROR
	{	DNS_ERROR_RCODE_NAME_ERROR,                               "DNS_ERROR_RCODE_NAME_ERROR",
		"DNS name does not exist.",
		"DNS name does not exist."
	},
#endif // #ifdef DNS_ERROR_RCODE_NAME_ERROR
#ifdef DNS_ERROR_RCODE_NOT_IMPLEMENTED
	{	DNS_ERROR_RCODE_NOT_IMPLEMENTED,                          "DNS_ERROR_RCODE_NOT_IMPLEMENTED",
		"DNS request not supported by name server.",
		"DNS request not supported by name server."
	},
#endif // #ifdef DNS_ERROR_RCODE_NOT_IMPLEMENTED
#ifdef DNS_ERROR_RCODE_REFUSED
	{	DNS_ERROR_RCODE_REFUSED,                                  "DNS_ERROR_RCODE_REFUSED",
		"DNS operation refused.",
		"DNS operation refused."
	},
#endif // #ifdef DNS_ERROR_RCODE_REFUSED
#ifdef DNS_ERROR_RCODE_YXDOMAIN
	{	DNS_ERROR_RCODE_YXDOMAIN,                                 "DNS_ERROR_RCODE_YXDOMAIN",
		"DNS name that ought not exist, does exist.",
		"DNS name that ought not exist, does exist."
	},
#endif // #ifdef DNS_ERROR_RCODE_YXDOMAIN
#ifdef DNS_ERROR_RCODE_YXRRSET
	{	DNS_ERROR_RCODE_YXRRSET,                                  "DNS_ERROR_RCODE_YXRRSET",
		"DNS RR set that ought not exist, does exist.",
		"DNS RR set that ought not exist, does exist."
	},
#endif // #ifdef DNS_ERROR_RCODE_YXRRSET
#ifdef DNS_ERROR_RCODE_NXRRSET
	{	DNS_ERROR_RCODE_NXRRSET,                                  "DNS_ERROR_RCODE_NXRRSET",
		"DNS RR set that ought to exist, does not exist.",
		"DNS RR set that ought to exist, does not exist."
	},
#endif // #ifdef DNS_ERROR_RCODE_NXRRSET
#ifdef DNS_ERROR_RCODE_NOTAUTH
	{	DNS_ERROR_RCODE_NOTAUTH,                                  "DNS_ERROR_RCODE_NOTAUTH",
		"DNS server not authoritative for zone.",
		"DNS server not authoritative for zone."
	},
#endif // #ifdef DNS_ERROR_RCODE_NOTAUTH
#ifdef DNS_ERROR_RCODE_NOTZONE
	{	DNS_ERROR_RCODE_NOTZONE,                                  "DNS_ERROR_RCODE_NOTZONE",
		"DNS name in update or prereq is not in zone.",
		"DNS name in update or prereq is not in zone."
	},
#endif // #ifdef DNS_ERROR_RCODE_NOTZONE
#ifdef DNS_ERROR_RCODE_BADSIG
	{	DNS_ERROR_RCODE_BADSIG,                                   "DNS_ERROR_RCODE_BADSIG",
		"DNS signature failed to verify.",
		"DNS signature failed to verify."
	},
#endif // #ifdef DNS_ERROR_RCODE_BADSIG
#ifdef DNS_ERROR_RCODE_BADKEY
	{	DNS_ERROR_RCODE_BADKEY,                                   "DNS_ERROR_RCODE_BADKEY",
		"DNS bad key.",
		"DNS bad key."
	},
#endif // #ifdef DNS_ERROR_RCODE_BADKEY
#ifdef DNS_ERROR_RCODE_BADTIME
	{	DNS_ERROR_RCODE_BADTIME,                                  "DNS_ERROR_RCODE_BADTIME",
		"DNS signature validity expired.",
		"DNS signature validity expired."
	},
#endif // #ifdef DNS_ERROR_RCODE_BADTIME
#ifdef DNS_INFO_NO_RECORDS
	{	DNS_INFO_NO_RECORDS,                                      "DNS_INFO_NO_RECORDS",
		"No records found for given DNS query.",
		"No records found for given DNS query."
	},
#endif // #ifdef DNS_INFO_NO_RECORDS
#ifdef DNS_ERROR_BAD_PACKET
	{	DNS_ERROR_BAD_PACKET,                                     "DNS_ERROR_BAD_PACKET",
		"Bad DNS packet.",
		"Bad DNS packet."
	},
#endif // #ifdef DNS_ERROR_BAD_PACKET
#ifdef DNS_ERROR_NO_PACKET
	{	DNS_ERROR_NO_PACKET,                                      "DNS_ERROR_NO_PACKET",
		"No DNS packet.",
		"No DNS packet."
	},
#endif // #ifdef DNS_ERROR_NO_PACKET
#ifdef DNS_ERROR_RCODE
	{	DNS_ERROR_RCODE,                                          "DNS_ERROR_RCODE",
		"DNS error, check rcode.",
		"DNS error, check rcode."
	},
#endif // #ifdef DNS_ERROR_RCODE
#ifdef DNS_ERROR_UNSECURE_PACKET
	{	DNS_ERROR_UNSECURE_PACKET,                                "DNS_ERROR_UNSECURE_PACKET",
		"Unsecured DNS packet.",
		"Unsecured DNS packet."
	},
#endif // #ifdef DNS_ERROR_UNSECURE_PACKET
#ifdef DNS_ERROR_INVALID_TYPE
	{	DNS_ERROR_INVALID_TYPE,                                   "DNS_ERROR_INVALID_TYPE",
		"Invalid DNS type.",
		"Invalid DNS type."
	},
#endif // #ifdef DNS_ERROR_INVALID_TYPE
#ifdef DNS_ERROR_INVALID_IP_ADDRESS
	{	DNS_ERROR_INVALID_IP_ADDRESS,                             "DNS_ERROR_INVALID_IP_ADDRESS",
		"Invalid IP address.",
		"Invalid IP address."
	},
#endif // #ifdef DNS_ERROR_INVALID_IP_ADDRESS
#ifdef DNS_ERROR_INVALID_PROPERTY
	{	DNS_ERROR_INVALID_PROPERTY,                               "DNS_ERROR_INVALID_PROPERTY",
		"Invalid property.",
		"Invalid property."
	},
#endif // #ifdef DNS_ERROR_INVALID_PROPERTY
#ifdef DNS_ERROR_TRY_AGAIN_LATER
	{	DNS_ERROR_TRY_AGAIN_LATER,                                "DNS_ERROR_TRY_AGAIN_LATER",
		"Try DNS operation again later.",
		"Try DNS operation again later."
	},
#endif // #ifdef DNS_ERROR_TRY_AGAIN_LATER
#ifdef DNS_ERROR_NOT_UNIQUE
	{	DNS_ERROR_NOT_UNIQUE,                                     "DNS_ERROR_NOT_UNIQUE",
		"Record for given name and type is not unique.",
		"Record for given name and type is not unique."
	},
#endif // #ifdef DNS_ERROR_NOT_UNIQUE
#ifdef DNS_ERROR_NON_RFC_NAME
	{	DNS_ERROR_NON_RFC_NAME,                                   "DNS_ERROR_NON_RFC_NAME",
		"DNS name does not comply with RFC specifications.",
		"DNS name does not comply with RFC specifications."
	},
#endif // #ifdef DNS_ERROR_NON_RFC_NAME
#ifdef DNS_STATUS_FQDN
	{	DNS_STATUS_FQDN,                                          "DNS_STATUS_FQDN",
		"DNS name is a fully-qualified DNS name.",
		"DNS name is a fully-qualified DNS name."
	},
#endif // #ifdef DNS_STATUS_FQDN
#ifdef DNS_STATUS_DOTTED_NAME
	{	DNS_STATUS_DOTTED_NAME,                                   "DNS_STATUS_DOTTED_NAME",
		"DNS name is dotted (multi-label).",
		"DNS name is dotted (multi-label)."
	},
#endif // #ifdef DNS_STATUS_DOTTED_NAME
#ifdef DNS_STATUS_SINGLE_PART_NAME
	{	DNS_STATUS_SINGLE_PART_NAME,                              "DNS_STATUS_SINGLE_PART_NAME",
		"DNS name is a single-part name.",
		"DNS name is a single-part name."
	},
#endif // #ifdef DNS_STATUS_SINGLE_PART_NAME
#ifdef DNS_ERROR_INVALID_NAME_CHAR
	{	DNS_ERROR_INVALID_NAME_CHAR,                              "DNS_ERROR_INVALID_NAME_CHAR",
		"DSN name contains an invalid character.",
		"DSN name contains an invalid character."
	},
#endif // #ifdef DNS_ERROR_INVALID_NAME_CHAR
#ifdef DNS_ERROR_NUMERIC_NAME
	{	DNS_ERROR_NUMERIC_NAME,                                   "DNS_ERROR_NUMERIC_NAME",
		"DNS name is entirely numeric.",
		"DNS name is entirely numeric."
	},
#endif // #ifdef DNS_ERROR_NUMERIC_NAME
#ifdef DNS_ERROR_NOT_ALLOWED_ON_ROOT_SERVER
	{	DNS_ERROR_NOT_ALLOWED_ON_ROOT_SERVER,                     "DNS_ERROR_NOT_ALLOWED_ON_ROOT_SERVER",
		"The operation requested is not permitted on a DNS root server.",
		"The operation requested is not permitted on a DNS root server."
	},
#endif // #ifdef DNS_ERROR_NOT_ALLOWED_ON_ROOT_SERVER
#ifdef DNS_ERROR_NOT_ALLOWED_UNDER_DELEGATION
	{	DNS_ERROR_NOT_ALLOWED_UNDER_DELEGATION,                   "DNS_ERROR_NOT_ALLOWED_UNDER_DELEGATION",
		"The record could not be created because this part of the DNS namespace has been delegated to another server.",
		"The record could not be created because this part of the DNS namespace has been delegated to another server."
	},
#endif // #ifdef DNS_ERROR_NOT_ALLOWED_UNDER_DELEGATION
#ifdef DNS_ERROR_CANNOT_FIND_ROOT_HINTS
	{	DNS_ERROR_CANNOT_FIND_ROOT_HINTS,                         "DNS_ERROR_CANNOT_FIND_ROOT_HINTS",
		"The DNS server could not find a set of root hints.",
		"The DNS server could not find a set of root hints."
	},
#endif // #ifdef DNS_ERROR_CANNOT_FIND_ROOT_HINTS
#ifdef DNS_ERROR_INCONSISTENT_ROOT_HINTS
	{	DNS_ERROR_INCONSISTENT_ROOT_HINTS,                        "DNS_ERROR_INCONSISTENT_ROOT_HINTS",
		"The DNS server found root hints but they were not consistent across all adapters.",
		"The DNS server found root hints but they were not consistent across all adapters."
	},
#endif // #ifdef DNS_ERROR_INCONSISTENT_ROOT_HINTS
#ifdef DNS_ERROR_ZONE_DOES_NOT_EXIST
	{	DNS_ERROR_ZONE_DOES_NOT_EXIST,                            "DNS_ERROR_ZONE_DOES_NOT_EXIST",
		"DNS zone does not exist.",
		"DNS zone does not exist."
	},
#endif // #ifdef DNS_ERROR_ZONE_DOES_NOT_EXIST
#ifdef DNS_ERROR_NO_ZONE_INFO
	{	DNS_ERROR_NO_ZONE_INFO,                                   "DNS_ERROR_NO_ZONE_INFO",
		"DNS zone information not available.",
		"DNS zone information not available."
	},
#endif // #ifdef DNS_ERROR_NO_ZONE_INFO
#ifdef DNS_ERROR_INVALID_ZONE_OPERATION
	{	DNS_ERROR_INVALID_ZONE_OPERATION,                         "DNS_ERROR_INVALID_ZONE_OPERATION",
		"Invalid operation for DNS zone.",
		"Invalid operation for DNS zone."
	},
#endif // #ifdef DNS_ERROR_INVALID_ZONE_OPERATION
#ifdef DNS_ERROR_ZONE_CONFIGURATION_ERROR
	{	DNS_ERROR_ZONE_CONFIGURATION_ERROR,                       "DNS_ERROR_ZONE_CONFIGURATION_ERROR",
		"Invalid DNS zone configuration.",
		"Invalid DNS zone configuration."
	},
#endif // #ifdef DNS_ERROR_ZONE_CONFIGURATION_ERROR
#ifdef DNS_ERROR_ZONE_HAS_NO_SOA_RECORD
	{	DNS_ERROR_ZONE_HAS_NO_SOA_RECORD,                         "DNS_ERROR_ZONE_HAS_NO_SOA_RECORD",
		"DNS zone has no start of authority (SOA) record.",
		"DNS zone has no start of authority (SOA) record."
	},
#endif // #ifdef DNS_ERROR_ZONE_HAS_NO_SOA_RECORD
#ifdef DNS_ERROR_ZONE_HAS_NO_NS_RECORDS
	{	DNS_ERROR_ZONE_HAS_NO_NS_RECORDS,                         "DNS_ERROR_ZONE_HAS_NO_NS_RECORDS",
		"DNS zone has no name server (NS) record.",
		"DNS zone has no name server (NS) record."
	},
#endif // #ifdef DNS_ERROR_ZONE_HAS_NO_NS_RECORDS
#ifdef DNS_ERROR_ZONE_LOCKED
	{	DNS_ERROR_ZONE_LOCKED,                                    "DNS_ERROR_ZONE_LOCKED",
		"DNS zone is locked.",
		"DNS zone is locked."
	},
#endif // #ifdef DNS_ERROR_ZONE_LOCKED
#ifdef DNS_ERROR_ZONE_CREATION_FAILED
	{	DNS_ERROR_ZONE_CREATION_FAILED,                           "DNS_ERROR_ZONE_CREATION_FAILED",
		"DNS zone creation failed.",
		"DNS zone creation failed."
	},
#endif // #ifdef DNS_ERROR_ZONE_CREATION_FAILED
#ifdef DNS_ERROR_ZONE_ALREADY_EXISTS
	{	DNS_ERROR_ZONE_ALREADY_EXISTS,                            "DNS_ERROR_ZONE_ALREADY_EXISTS",
		"DNS zone already exists.",
		"DNS zone already exists."
	},
#endif // #ifdef DNS_ERROR_ZONE_ALREADY_EXISTS
#ifdef DNS_ERROR_AUTOZONE_ALREADY_EXISTS
	{	DNS_ERROR_AUTOZONE_ALREADY_EXISTS,                        "DNS_ERROR_AUTOZONE_ALREADY_EXISTS",
		"DNS automatic zone already exists.",
		"DNS automatic zone already exists."
	},
#endif // #ifdef DNS_ERROR_AUTOZONE_ALREADY_EXISTS
#ifdef DNS_ERROR_INVALID_ZONE_TYPE
	{	DNS_ERROR_INVALID_ZONE_TYPE,                              "DNS_ERROR_INVALID_ZONE_TYPE",
		"Invalid DNS zone type.",
		"Invalid DNS zone type."
	},
#endif // #ifdef DNS_ERROR_INVALID_ZONE_TYPE
#ifdef DNS_ERROR_SECONDARY_REQUIRES_MASTER_IP
	{	DNS_ERROR_SECONDARY_REQUIRES_MASTER_IP,                   "DNS_ERROR_SECONDARY_REQUIRES_MASTER_IP",
		"Secondary DNS zone requires master IP address.",
		"Secondary DNS zone requires master IP address."
	},
#endif // #ifdef DNS_ERROR_SECONDARY_REQUIRES_MASTER_IP
#ifdef DNS_ERROR_ZONE_NOT_SECONDARY
	{	DNS_ERROR_ZONE_NOT_SECONDARY,                             "DNS_ERROR_ZONE_NOT_SECONDARY",
		"DNS zone not secondary.",
		"DNS zone not secondary."
	},
#endif // #ifdef DNS_ERROR_ZONE_NOT_SECONDARY
#ifdef DNS_ERROR_NEED_SECONDARY_ADDRESSES
	{	DNS_ERROR_NEED_SECONDARY_ADDRESSES,                       "DNS_ERROR_NEED_SECONDARY_ADDRESSES",
		"Need secondary IP address.",
		"Need secondary IP address."
	},
#endif // #ifdef DNS_ERROR_NEED_SECONDARY_ADDRESSES
#ifdef DNS_ERROR_WINS_INIT_FAILED
	{	DNS_ERROR_WINS_INIT_FAILED,                               "DNS_ERROR_WINS_INIT_FAILED",
		"WINS initialization failed.",
		"WINS initialization failed."
	},
#endif // #ifdef DNS_ERROR_WINS_INIT_FAILED
#ifdef DNS_ERROR_NEED_WINS_SERVERS
	{	DNS_ERROR_NEED_WINS_SERVERS,                              "DNS_ERROR_NEED_WINS_SERVERS",
		"Need WINS servers.",
		"Need WINS servers."
	},
#endif // #ifdef DNS_ERROR_NEED_WINS_SERVERS
#ifdef DNS_ERROR_NBSTAT_INIT_FAILED
	{	DNS_ERROR_NBSTAT_INIT_FAILED,                             "DNS_ERROR_NBSTAT_INIT_FAILED",
		"NBTSTAT initialization call failed.",
		"NBTSTAT initialization call failed."
	},
#endif // #ifdef DNS_ERROR_NBSTAT_INIT_FAILED
#ifdef DNS_ERROR_SOA_DELETE_INVALID
	{	DNS_ERROR_SOA_DELETE_INVALID,                             "DNS_ERROR_SOA_DELETE_INVALID",
		"Invalid delete of start of authority (SOA)",
		"Invalid delete of start of authority (SOA)"
	},
#endif // #ifdef DNS_ERROR_SOA_DELETE_INVALID
#ifdef DNS_ERROR_FORWARDER_ALREADY_EXISTS
	{	DNS_ERROR_FORWARDER_ALREADY_EXISTS,                       "DNS_ERROR_FORWARDER_ALREADY_EXISTS",
		"A conditional forwarding zone already exists for that name.",
		"A conditional forwarding zone already exists for that name."
	},
#endif // #ifdef DNS_ERROR_FORWARDER_ALREADY_EXISTS
#ifdef DNS_ERROR_ZONE_REQUIRES_MASTER_IP
	{	DNS_ERROR_ZONE_REQUIRES_MASTER_IP,                        "DNS_ERROR_ZONE_REQUIRES_MASTER_IP",
		"This zone must be configured with one or more master DNS server IP addresses.",
		"This zone must be configured with one or more master DNS server IP addresses."
	},
#endif // #ifdef DNS_ERROR_ZONE_REQUIRES_MASTER_IP
#ifdef DNS_ERROR_ZONE_IS_SHUTDOWN
	{	DNS_ERROR_ZONE_IS_SHUTDOWN,                               "DNS_ERROR_ZONE_IS_SHUTDOWN",
		"The operation cannot be performed because this zone is shutdown.",
		"The operation cannot be performed because this zone is shutdown."
	},
#endif // #ifdef DNS_ERROR_ZONE_IS_SHUTDOWN
#ifdef DNS_ERROR_PRIMARY_REQUIRES_DATAFILE
	{	DNS_ERROR_PRIMARY_REQUIRES_DATAFILE,                      "DNS_ERROR_PRIMARY_REQUIRES_DATAFILE",
		"Primary DNS zone requires datafile.",
		"Primary DNS zone requires datafile."
	},
#endif // #ifdef DNS_ERROR_PRIMARY_REQUIRES_DATAFILE
#ifdef DNS_ERROR_INVALID_DATAFILE_NAME
	{	DNS_ERROR_INVALID_DATAFILE_NAME,                          "DNS_ERROR_INVALID_DATAFILE_NAME",
		"Invalid datafile name for DNS zone.",
		"Invalid datafile name for DNS zone."
	},
#endif // #ifdef DNS_ERROR_INVALID_DATAFILE_NAME
#ifdef DNS_ERROR_DATAFILE_OPEN_FAILURE
	{	DNS_ERROR_DATAFILE_OPEN_FAILURE,                          "DNS_ERROR_DATAFILE_OPEN_FAILURE",
		"Failed to open datafile for DNS zone.",
		"Failed to open datafile for DNS zone."
	},
#endif // #ifdef DNS_ERROR_DATAFILE_OPEN_FAILURE
#ifdef DNS_ERROR_FILE_WRITEBACK_FAILED
	{	DNS_ERROR_FILE_WRITEBACK_FAILED,                          "DNS_ERROR_FILE_WRITEBACK_FAILED",
		"Failed to write datafile for DNS zone.",
		"Failed to write datafile for DNS zone."
	},
#endif // #ifdef DNS_ERROR_FILE_WRITEBACK_FAILED
#ifdef DNS_ERROR_DATAFILE_PARSING
	{	DNS_ERROR_DATAFILE_PARSING,                               "DNS_ERROR_DATAFILE_PARSING",
		"Failure while reading datafile for DNS zone.",
		"Failure while reading datafile for DNS zone."
	},
#endif // #ifdef DNS_ERROR_DATAFILE_PARSING
#ifdef DNS_ERROR_RECORD_DOES_NOT_EXIST
	{	DNS_ERROR_RECORD_DOES_NOT_EXIST,                          "DNS_ERROR_RECORD_DOES_NOT_EXIST",
		"DNS record does not exist.",
		"DNS record does not exist."
	},
#endif // #ifdef DNS_ERROR_RECORD_DOES_NOT_EXIST
#ifdef DNS_ERROR_RECORD_FORMAT
	{	DNS_ERROR_RECORD_FORMAT,                                  "DNS_ERROR_RECORD_FORMAT",
		"DNS record format error.",
		"DNS record format error."
	},
#endif // #ifdef DNS_ERROR_RECORD_FORMAT
#ifdef DNS_ERROR_NODE_CREATION_FAILED
	{	DNS_ERROR_NODE_CREATION_FAILED,                           "DNS_ERROR_NODE_CREATION_FAILED",
		"Node creation failure in DNS.",
		"Node creation failure in DNS."
	},
#endif // #ifdef DNS_ERROR_NODE_CREATION_FAILED
#ifdef DNS_ERROR_UNKNOWN_RECORD_TYPE
	{	DNS_ERROR_UNKNOWN_RECORD_TYPE,                            "DNS_ERROR_UNKNOWN_RECORD_TYPE",
		"Unknown DNS record type.",
		"Unknown DNS record type."
	},
#endif // #ifdef DNS_ERROR_UNKNOWN_RECORD_TYPE
#ifdef DNS_ERROR_RECORD_TIMED_OUT
	{	DNS_ERROR_RECORD_TIMED_OUT,                               "DNS_ERROR_RECORD_TIMED_OUT",
		"DNS record timed out.",
		"DNS record timed out."
	},
#endif // #ifdef DNS_ERROR_RECORD_TIMED_OUT
#ifdef DNS_ERROR_NAME_NOT_IN_ZONE
	{	DNS_ERROR_NAME_NOT_IN_ZONE,                               "DNS_ERROR_NAME_NOT_IN_ZONE",
		"Name not in DNS zone.",
		"Name not in DNS zone."
	},
#endif // #ifdef DNS_ERROR_NAME_NOT_IN_ZONE
#ifdef DNS_ERROR_CNAME_LOOP
	{	DNS_ERROR_CNAME_LOOP,                                     "DNS_ERROR_CNAME_LOOP",
		"CNAME loop detected.",
		"CNAME loop detected."
	},
#endif // #ifdef DNS_ERROR_CNAME_LOOP
#ifdef DNS_ERROR_NODE_IS_CNAME
	{	DNS_ERROR_NODE_IS_CNAME,                                  "DNS_ERROR_NODE_IS_CNAME",
		"Node is a CNAME DNS record.",
		"Node is a CNAME DNS record."
	},
#endif // #ifdef DNS_ERROR_NODE_IS_CNAME
#ifdef DNS_ERROR_CNAME_COLLISION
	{	DNS_ERROR_CNAME_COLLISION,                                "DNS_ERROR_CNAME_COLLISION",
		"A CNAME record already exists for given name.",
		"A CNAME record already exists for given name."
	},
#endif // #ifdef DNS_ERROR_CNAME_COLLISION
#ifdef DNS_ERROR_RECORD_ONLY_AT_ZONE_ROOT
	{	DNS_ERROR_RECORD_ONLY_AT_ZONE_ROOT,                       "DNS_ERROR_RECORD_ONLY_AT_ZONE_ROOT",
		"Record only at DNS zone root.",
		"Record only at DNS zone root."
	},
#endif // #ifdef DNS_ERROR_RECORD_ONLY_AT_ZONE_ROOT
#ifdef DNS_ERROR_RECORD_ALREADY_EXISTS
	{	DNS_ERROR_RECORD_ALREADY_EXISTS,                          "DNS_ERROR_RECORD_ALREADY_EXISTS",
		"DNS record already exists.",
		"DNS record already exists."
	},
#endif // #ifdef DNS_ERROR_RECORD_ALREADY_EXISTS
#ifdef DNS_ERROR_SECONDARY_DATA
	{	DNS_ERROR_SECONDARY_DATA,                                 "DNS_ERROR_SECONDARY_DATA",
		"Secondary DNS zone data error.",
		"Secondary DNS zone data error."
	},
#endif // #ifdef DNS_ERROR_SECONDARY_DATA
#ifdef DNS_ERROR_NO_CREATE_CACHE_DATA
	{	DNS_ERROR_NO_CREATE_CACHE_DATA,                           "DNS_ERROR_NO_CREATE_CACHE_DATA",
		"Could not create DNS cache data.",
		"Could not create DNS cache data."
	},
#endif // #ifdef DNS_ERROR_NO_CREATE_CACHE_DATA
#ifdef DNS_ERROR_NAME_DOES_NOT_EXIST
	{	DNS_ERROR_NAME_DOES_NOT_EXIST,                            "DNS_ERROR_NAME_DOES_NOT_EXIST",
		"DNS name does not exist.",
		"DNS name does not exist."
	},
#endif // #ifdef DNS_ERROR_NAME_DOES_NOT_EXIST
#ifdef DNS_WARNING_PTR_CREATE_FAILED
	{	DNS_WARNING_PTR_CREATE_FAILED,                            "DNS_WARNING_PTR_CREATE_FAILED",
		"Could not create pointer (PTR) record.",
		"Could not create pointer (PTR) record."
	},
#endif // #ifdef DNS_WARNING_PTR_CREATE_FAILED
#ifdef DNS_WARNING_DOMAIN_UNDELETED
	{	DNS_WARNING_DOMAIN_UNDELETED,                             "DNS_WARNING_DOMAIN_UNDELETED",
		"DNS domain was undeleted.",
		"DNS domain was undeleted."
	},
#endif // #ifdef DNS_WARNING_DOMAIN_UNDELETED
#ifdef DNS_ERROR_DS_UNAVAILABLE
	{	DNS_ERROR_DS_UNAVAILABLE,                                 "DNS_ERROR_DS_UNAVAILABLE",
		"The directory service is unavailable.",
		"The directory service is unavailable."
	},
#endif // #ifdef DNS_ERROR_DS_UNAVAILABLE
#ifdef DNS_ERROR_DS_ZONE_ALREADY_EXISTS
	{	DNS_ERROR_DS_ZONE_ALREADY_EXISTS,                         "DNS_ERROR_DS_ZONE_ALREADY_EXISTS",
		"DNS zone already exists in the directory service.",
		"DNS zone already exists in the directory service."
	},
#endif // #ifdef DNS_ERROR_DS_ZONE_ALREADY_EXISTS
#ifdef DNS_ERROR_NO_BOOTFILE_IF_DS_ZONE
	{	DNS_ERROR_NO_BOOTFILE_IF_DS_ZONE,                         "DNS_ERROR_NO_BOOTFILE_IF_DS_ZONE",
		"DNS server not creating or reading the boot file for the directory service integrated DNS zone.",
		"DNS server not creating or reading the boot file for the directory service integrated DNS zone."
	},
#endif // #ifdef DNS_ERROR_NO_BOOTFILE_IF_DS_ZONE
#ifdef DNS_INFO_AXFR_COMPLETE
	{	DNS_INFO_AXFR_COMPLETE,                                   "DNS_INFO_AXFR_COMPLETE",
		"DNS AXFR (zone transfer) complete.",
		"DNS AXFR (zone transfer) complete."
	},
#endif // #ifdef DNS_INFO_AXFR_COMPLETE
#ifdef DNS_ERROR_AXFR
	{	DNS_ERROR_AXFR,                                           "DNS_ERROR_AXFR",
		"DNS zone transfer failed.",
		"DNS zone transfer failed."
	},
#endif // #ifdef DNS_ERROR_AXFR
#ifdef DNS_INFO_ADDED_LOCAL_WINS
	{	DNS_INFO_ADDED_LOCAL_WINS,                                "DNS_INFO_ADDED_LOCAL_WINS",
		"Added local WINS server.",
		"Added local WINS server."
	},
#endif // #ifdef DNS_INFO_ADDED_LOCAL_WINS
#ifdef DNS_STATUS_CONTINUE_NEEDED
	{	DNS_STATUS_CONTINUE_NEEDED,                               "DNS_STATUS_CONTINUE_NEEDED",
		"Secure update call needs to continue update request.",
		"Secure update call needs to continue update request."
	},
#endif // #ifdef DNS_STATUS_CONTINUE_NEEDED
#ifdef DNS_ERROR_NO_TCPIP
	{	DNS_ERROR_NO_TCPIP,                                       "DNS_ERROR_NO_TCPIP",
		"TCP/IP network protocol not installed.",
		"TCP/IP network protocol not installed."
	},
#endif // #ifdef DNS_ERROR_NO_TCPIP
#ifdef DNS_ERROR_NO_DNS_SERVERS
	{	DNS_ERROR_NO_DNS_SERVERS,                                 "DNS_ERROR_NO_DNS_SERVERS",
		"No DNS servers configured for local system.",
		"No DNS servers configured for local system."
	},
#endif // #ifdef DNS_ERROR_NO_DNS_SERVERS
#ifdef DNS_ERROR_DP_DOES_NOT_EXIST
	{	DNS_ERROR_DP_DOES_NOT_EXIST,                              "DNS_ERROR_DP_DOES_NOT_EXIST",
		"The specified directory partition does not exist.",
		"The specified directory partition does not exist."
	},
#endif // #ifdef DNS_ERROR_DP_DOES_NOT_EXIST
#ifdef DNS_ERROR_DP_ALREADY_EXISTS
	{	DNS_ERROR_DP_ALREADY_EXISTS,                              "DNS_ERROR_DP_ALREADY_EXISTS",
		"The specified directory partition already exists.",
		"The specified directory partition already exists."
	},
#endif // #ifdef DNS_ERROR_DP_ALREADY_EXISTS
#ifdef DNS_ERROR_DP_NOT_ENLISTED
	{	DNS_ERROR_DP_NOT_ENLISTED,                                "DNS_ERROR_DP_NOT_ENLISTED",
		"The DNS server is not enlisted in the specified directory partition.",
		"The DNS server is not enlisted in the specified directory partition."
	},
#endif // #ifdef DNS_ERROR_DP_NOT_ENLISTED
#ifdef DNS_ERROR_DP_ALREADY_ENLISTED
	{	DNS_ERROR_DP_ALREADY_ENLISTED,                            "DNS_ERROR_DP_ALREADY_ENLISTED",
		"The DNS server is already enlisted in the specified directory partition.",
		"The DNS server is already enlisted in the specified directory partition."
	},
#endif // #ifdef DNS_ERROR_DP_ALREADY_ENLISTED
#ifdef DNS_ERROR_DP_NOT_AVAILABLE
	{	DNS_ERROR_DP_NOT_AVAILABLE,                               "DNS_ERROR_DP_NOT_AVAILABLE",
		"The directory partition is not available at this time. Please wait a few minutes and try again.",
		"The directory partition is not available at this time. Please wait a few minutes and try again."
	},
#endif // #ifdef DNS_ERROR_DP_NOT_AVAILABLE
#ifdef WSAEINTR
	{	WSAEINTR,                                                 "WSAEINTR",
		"A blocking operation was interrupted by a call to WSACancelBlockingCall.",
		"A blocking operation was interrupted by a call to WSACancelBlockingCall."
	},
#endif // #ifdef WSAEINTR
#ifdef WSAEBADF
	{	WSAEBADF,                                                 "WSAEBADF",
		"The file handle supplied is not valid.",
		"The file handle supplied is not valid."
	},
#endif // #ifdef WSAEBADF
#ifdef WSAEACCES
	{	WSAEACCES,                                                "WSAEACCES",
		"An attempt was made to access a socket in a way forbidden by its access permissions.",
		"An attempt was made to access a socket in a way forbidden by its access permissions."
	},
#endif // #ifdef WSAEACCES
#ifdef WSAEFAULT
	{	WSAEFAULT,                                                "WSAEFAULT",
		"The system detected an invalid pointer address in attempting to use a pointer argument in a call.",
		"The system detected an invalid pointer address in attempting to use a pointer argument in a call."
	},
#endif // #ifdef WSAEFAULT
#ifdef WSAEINVAL
	{	WSAEINVAL,                                                "WSAEINVAL",
		"An invalid argument was supplied.",
		"An invalid argument was supplied."
	},
#endif // #ifdef WSAEINVAL
#ifdef WSAEMFILE
	{	WSAEMFILE,                                                "WSAEMFILE",
		"Too many open sockets.",
		"Too many open sockets."
	},
#endif // #ifdef WSAEMFILE
#ifdef WSAEWOULDBLOCK
	{	WSAEWOULDBLOCK,                                           "WSAEWOULDBLOCK",
		"A non-blocking socket operation could not be completed immediately.",
		"A non-blocking socket operation could not be completed immediately."
	},
#endif // #ifdef WSAEWOULDBLOCK
#ifdef WSAEINPROGRESS
	{	WSAEINPROGRESS,                                           "WSAEINPROGRESS",
		"A blocking operation is currently executing.",
		"A blocking operation is currently executing."
	},
#endif // #ifdef WSAEINPROGRESS
#ifdef WSAEALREADY
	{	WSAEALREADY,                                              "WSAEALREADY",
		"An operation was attempted on a non-blocking socket that already had an operation in progress.",
		"An operation was attempted on a non-blocking socket that already had an operation in progress."
	},
#endif // #ifdef WSAEALREADY
#ifdef WSAENOTSOCK
	{	WSAENOTSOCK,                                              "WSAENOTSOCK",
		"An operation was attempted on something that is not a socket.",
		"An operation was attempted on something that is not a socket."
	},
#endif // #ifdef WSAENOTSOCK
#ifdef WSAEDESTADDRREQ
	{	WSAEDESTADDRREQ,                                          "WSAEDESTADDRREQ",
		"A required address was omitted from an operation on a socket.",
		"A required address was omitted from an operation on a socket."
	},
#endif // #ifdef WSAEDESTADDRREQ
#ifdef WSAEMSGSIZE
	{	WSAEMSGSIZE,                                              "WSAEMSGSIZE",
		"A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself.",
		"A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself."
	},
#endif // #ifdef WSAEMSGSIZE
#ifdef WSAEPROTOTYPE
	{	WSAEPROTOTYPE,                                            "WSAEPROTOTYPE",
		"A protocol was specified in the socket function call that does not support the semantics of the socket type requested.",
		"A protocol was specified in the socket function call that does not support the semantics of the socket type requested."
	},
#endif // #ifdef WSAEPROTOTYPE
#ifdef WSAENOPROTOOPT
	{	WSAENOPROTOOPT,                                           "WSAENOPROTOOPT",
		"An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call.",
		"An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call."
	},
#endif // #ifdef WSAENOPROTOOPT
#ifdef WSAEPROTONOSUPPORT
	{	WSAEPROTONOSUPPORT,                                       "WSAEPROTONOSUPPORT",
		"The requested protocol has not been configured into the system, or no implementation for it exists.",
		"The requested protocol has not been configured into the system, or no implementation for it exists."
	},
#endif // #ifdef WSAEPROTONOSUPPORT
#ifdef WSAESOCKTNOSUPPORT
	{	WSAESOCKTNOSUPPORT,                                       "WSAESOCKTNOSUPPORT",
		"The support for the specified socket type does not exist in this address family.",
		"The support for the specified socket type does not exist in this address family."
	},
#endif // #ifdef WSAESOCKTNOSUPPORT
#ifdef WSAEOPNOTSUPP
	{	WSAEOPNOTSUPP,                                            "WSAEOPNOTSUPP",
		"The attempted operation is not supported for the type of object referenced.",
		"The attempted operation is not supported for the type of object referenced."
	},
#endif // #ifdef WSAEOPNOTSUPP
#ifdef WSAEPFNOSUPPORT
	{	WSAEPFNOSUPPORT,                                          "WSAEPFNOSUPPORT",
		"The protocol family has not been configured into the system or no implementation for it exists.",
		"The protocol family has not been configured into the system or no implementation for it exists."
	},
#endif // #ifdef WSAEPFNOSUPPORT
#ifdef WSAEAFNOSUPPORT
	{	WSAEAFNOSUPPORT,                                          "WSAEAFNOSUPPORT",
		"An address incompatible with the requested protocol was used.",
		"An address incompatible with the requested protocol was used."
	},
#endif // #ifdef WSAEAFNOSUPPORT
#ifdef WSAEADDRINUSE
	{	WSAEADDRINUSE,                                            "WSAEADDRINUSE",
		"Only one usage of each socket address (protocol/network address/port) is normally permitted.",
		"Only one usage of each socket address (protocol/network address/port) is normally permitted."
	},
#endif // #ifdef WSAEADDRINUSE
#ifdef WSAEADDRNOTAVAIL
	{	WSAEADDRNOTAVAIL,                                         "WSAEADDRNOTAVAIL",
		"The requested address is not valid in its context.",
		"The requested address is not valid in its context."
	},
#endif // #ifdef WSAEADDRNOTAVAIL
#ifdef WSAENETDOWN
	{	WSAENETDOWN,                                              "WSAENETDOWN",
		"A socket operation encountered a dead network.",
		"A socket operation encountered a dead network."
	},
#endif // #ifdef WSAENETDOWN
#ifdef WSAENETUNREACH
	{	WSAENETUNREACH,                                           "WSAENETUNREACH",
		"A socket operation was attempted to an unreachable network.",
		"A socket operation was attempted to an unreachable network."
	},
#endif // #ifdef WSAENETUNREACH
#ifdef WSAENETRESET
	{	WSAENETRESET,                                             "WSAENETRESET",
		"The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.",
		"The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress."
	},
#endif // #ifdef WSAENETRESET
#ifdef WSAECONNABORTED
	{	WSAECONNABORTED,                                          "WSAECONNABORTED",
		"An established connection was aborted by the software in your host machine.",
		"An established connection was aborted by the software in your host machine."
	},
#endif // #ifdef WSAECONNABORTED
#ifdef WSAECONNRESET
	{	WSAECONNRESET,                                            "WSAECONNRESET",
		"An existing connection was forcibly closed by the remote host.",
		"An existing connection was forcibly closed by the remote host."
	},
#endif // #ifdef WSAECONNRESET
#ifdef WSAENOBUFS
	{	WSAENOBUFS,                                               "WSAENOBUFS",
		"An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.",
		"An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full."
	},
#endif // #ifdef WSAENOBUFS
#ifdef WSAEISCONN
	{	WSAEISCONN,                                               "WSAEISCONN",
		"A connect request was made on an already connected socket.",
		"A connect request was made on an already connected socket."
	},
#endif // #ifdef WSAEISCONN
#ifdef WSAENOTCONN
	{	WSAENOTCONN,                                              "WSAENOTCONN",
		"A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.",
		"A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied."
	},
#endif // #ifdef WSAENOTCONN
#ifdef WSAESHUTDOWN
	{	WSAESHUTDOWN,                                             "WSAESHUTDOWN",
		"A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.",
		"A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call."
	},
#endif // #ifdef WSAESHUTDOWN
#ifdef WSAETOOMANYREFS
	{	WSAETOOMANYREFS,                                          "WSAETOOMANYREFS",
		"Too many references to some kernel object.",
		"Too many references to some kernel object."
	},
#endif // #ifdef WSAETOOMANYREFS
#ifdef WSAETIMEDOUT
	{	WSAETIMEDOUT,                                             "WSAETIMEDOUT",
		"A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.",
		"A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond."
	},
#endif // #ifdef WSAETIMEDOUT
#ifdef WSAECONNREFUSED
	{	WSAECONNREFUSED,                                          "WSAECONNREFUSED",
		"No connection could be made because the target machine actively refused it.",
		"No connection could be made because the target machine actively refused it."
	},
#endif // #ifdef WSAECONNREFUSED
#ifdef WSAELOOP
	{	WSAELOOP,                                                 "WSAELOOP",
		"Cannot translate name.",
		"Cannot translate name."
	},
#endif // #ifdef WSAELOOP
#ifdef WSAENAMETOOLONG
	{	WSAENAMETOOLONG,                                          "WSAENAMETOOLONG",
		"Name component or name was too long.",
		"Name component or name was too long."
	},
#endif // #ifdef WSAENAMETOOLONG
#ifdef WSAEHOSTDOWN
	{	WSAEHOSTDOWN,                                             "WSAEHOSTDOWN",
		"A socket operation failed because the destination host was down.",
		"A socket operation failed because the destination host was down."
	},
#endif // #ifdef WSAEHOSTDOWN
#ifdef WSAEHOSTUNREACH
	{	WSAEHOSTUNREACH,                                          "WSAEHOSTUNREACH",
		"A socket operation was attempted to an unreachable host.",
		"A socket operation was attempted to an unreachable host."
	},
#endif // #ifdef WSAEHOSTUNREACH
#ifdef WSAENOTEMPTY
	{	WSAENOTEMPTY,                                             "WSAENOTEMPTY",
		"Cannot remove a directory that is not empty.",
		"Cannot remove a directory that is not empty."
	},
#endif // #ifdef WSAENOTEMPTY
#ifdef WSAEPROCLIM
	{	WSAEPROCLIM,                                              "WSAEPROCLIM",
		"A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.",
		"A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously."
	},
#endif // #ifdef WSAEPROCLIM
#ifdef WSAEUSERS
	{	WSAEUSERS,                                                "WSAEUSERS",
		"Ran out of quota.",
		"Ran out of quota."
	},
#endif // #ifdef WSAEUSERS
#ifdef WSAEDQUOT
	{	WSAEDQUOT,                                                "WSAEDQUOT",
		"Ran out of disk quota.",
		"Ran out of disk quota."
	},
#endif // #ifdef WSAEDQUOT
#ifdef WSAESTALE
	{	WSAESTALE,                                                "WSAESTALE",
		"File handle reference is no longer available.",
		"File handle reference is no longer available."
	},
#endif // #ifdef WSAESTALE
#ifdef WSAEREMOTE
	{	WSAEREMOTE,                                               "WSAEREMOTE",
		"Item is not available locally.",
		"Item is not available locally."
	},
#endif // #ifdef WSAEREMOTE
#ifdef WSASYSNOTREADY
	{	WSASYSNOTREADY,                                           "WSASYSNOTREADY",
		"WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.",
		"WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable."
	},
#endif // #ifdef WSASYSNOTREADY
#ifdef WSAVERNOTSUPPORTED
	{	WSAVERNOTSUPPORTED,                                       "WSAVERNOTSUPPORTED",
		"The Windows Sockets version requested is not supported.",
		"The Windows Sockets version requested is not supported."
	},
#endif // #ifdef WSAVERNOTSUPPORTED
#ifdef WSANOTINITIALISED
	{	WSANOTINITIALISED,                                        "WSANOTINITIALISED",
		"Either the application has not called WSAStartup, or WSAStartup failed.",
		"Either the application has not called WSAStartup, or WSAStartup failed."
	},
#endif // #ifdef WSANOTINITIALISED
#ifdef WSAEDISCON
	{	WSAEDISCON,                                               "WSAEDISCON",
		"Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.",
		"Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence."
	},
#endif // #ifdef WSAEDISCON
#ifdef WSAENOMORE
	{	WSAENOMORE,                                               "WSAENOMORE",
		"No more results can be returned by WSALookupServiceNext.",
		"No more results can be returned by WSALookupServiceNext."
	},
#endif // #ifdef WSAENOMORE
#ifdef WSAECANCELLED
	{	WSAECANCELLED,                                            "WSAECANCELLED",
		"A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.",
		"A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled."
	},
#endif // #ifdef WSAECANCELLED
#ifdef WSAEINVALIDPROCTABLE
	{	WSAEINVALIDPROCTABLE,                                     "WSAEINVALIDPROCTABLE",
		"The procedure call table is invalid.",
		"The procedure call table is invalid."
	},
#endif // #ifdef WSAEINVALIDPROCTABLE
#ifdef WSAEINVALIDPROVIDER
	{	WSAEINVALIDPROVIDER,                                      "WSAEINVALIDPROVIDER",
		"The requested service provider is invalid.",
		"The requested service provider is invalid."
	},
#endif // #ifdef WSAEINVALIDPROVIDER
#ifdef WSAEPROVIDERFAILEDINIT
	{	WSAEPROVIDERFAILEDINIT,                                   "WSAEPROVIDERFAILEDINIT",
		"The requested service provider could not be loaded or initialized.",
		"The requested service provider could not be loaded or initialized."
	},
#endif // #ifdef WSAEPROVIDERFAILEDINIT
#ifdef WSASYSCALLFAILURE
	{	WSASYSCALLFAILURE,                                        "WSASYSCALLFAILURE",
		"A system call that should never fail has failed.",
		"A system call that should never fail has failed."
	},
#endif // #ifdef WSASYSCALLFAILURE
#ifdef WSASERVICE_NOT_FOUND
	{	WSASERVICE_NOT_FOUND,                                     "WSASERVICE_NOT_FOUND",
		"No such service is known. The service cannot be found in the specified name space.",
		"No such service is known. The service cannot be found in the specified name space."
	},
#endif // #ifdef WSASERVICE_NOT_FOUND
#ifdef WSATYPE_NOT_FOUND
	{	WSATYPE_NOT_FOUND,                                        "WSATYPE_NOT_FOUND",
		"The specified class was not found.",
		"The specified class was not found."
	},
#endif // #ifdef WSATYPE_NOT_FOUND
#ifdef WSA_E_NO_MORE
	{	WSA_E_NO_MORE,                                            "WSA_E_NO_MORE",
		"No more results can be returned by WSALookupServiceNext.",
		"No more results can be returned by WSALookupServiceNext."
	},
#endif // #ifdef WSA_E_NO_MORE
#ifdef WSA_E_CANCELLED
	{	WSA_E_CANCELLED,                                          "WSA_E_CANCELLED",
		"A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.",
		"A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled."
	},
#endif // #ifdef WSA_E_CANCELLED
#ifdef WSAEREFUSED
	{	WSAEREFUSED,                                              "WSAEREFUSED",
		"A database query failed because it was actively refused.",
		"A database query failed because it was actively refused."
	},
#endif // #ifdef WSAEREFUSED
#ifdef WSAHOST_NOT_FOUND
	{	WSAHOST_NOT_FOUND,                                        "WSAHOST_NOT_FOUND",
		"No such host is known.",
		"No such host is known."
	},
#endif // #ifdef WSAHOST_NOT_FOUND
#ifdef WSATRY_AGAIN
	{	WSATRY_AGAIN,                                             "WSATRY_AGAIN",
		"This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server.",
		"This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server."
	},
#endif // #ifdef WSATRY_AGAIN
#ifdef WSANO_RECOVERY
	{	WSANO_RECOVERY,                                           "WSANO_RECOVERY",
		"A non-recoverable error occurred during a database lookup.",
		"A non-recoverable error occurred during a database lookup."
	},
#endif // #ifdef WSANO_RECOVERY
#ifdef WSANO_DATA
	{	WSANO_DATA,                                               "WSANO_DATA",
		"The requested name is valid, but no data of the requested type was found.",
		"The requested name is valid, but no data of the requested type was found."
	},
#endif // #ifdef WSANO_DATA
#ifdef WSA_QOS_RECEIVERS
	{	WSA_QOS_RECEIVERS,                                        "WSA_QOS_RECEIVERS",
		"At least one reserve has arrived.",
		"At least one reserve has arrived."
	},
#endif // #ifdef WSA_QOS_RECEIVERS
#ifdef WSA_QOS_SENDERS
	{	WSA_QOS_SENDERS,                                          "WSA_QOS_SENDERS",
		"At least one path has arrived.",
		"At least one path has arrived."
	},
#endif // #ifdef WSA_QOS_SENDERS
#ifdef WSA_QOS_NO_SENDERS
	{	WSA_QOS_NO_SENDERS,                                       "WSA_QOS_NO_SENDERS",
		"There are no senders.",
		"There are no senders."
	},
#endif // #ifdef WSA_QOS_NO_SENDERS
#ifdef WSA_QOS_NO_RECEIVERS
	{	WSA_QOS_NO_RECEIVERS,                                     "WSA_QOS_NO_RECEIVERS",
		"There are no receivers.",
		"There are no receivers."
	},
#endif // #ifdef WSA_QOS_NO_RECEIVERS
#ifdef WSA_QOS_REQUEST_CONFIRMED
	{	WSA_QOS_REQUEST_CONFIRMED,                                "WSA_QOS_REQUEST_CONFIRMED",
		"Reserve has been confirmed.",
		"Reserve has been confirmed."
	},
#endif // #ifdef WSA_QOS_REQUEST_CONFIRMED
#ifdef WSA_QOS_ADMISSION_FAILURE
	{	WSA_QOS_ADMISSION_FAILURE,                                "WSA_QOS_ADMISSION_FAILURE",
		"Error due to lack of resources.",
		"Error due to lack of resources."
	},
#endif // #ifdef WSA_QOS_ADMISSION_FAILURE
#ifdef WSA_QOS_POLICY_FAILURE
	{	WSA_QOS_POLICY_FAILURE,                                   "WSA_QOS_POLICY_FAILURE",
		"Rejected for administrative reasons - bad credentials.",
		"Rejected for administrative reasons - bad credentials."
	},
#endif // #ifdef WSA_QOS_POLICY_FAILURE
#ifdef WSA_QOS_BAD_STYLE
	{	WSA_QOS_BAD_STYLE,                                        "WSA_QOS_BAD_STYLE",
		"Unknown or conflicting style.",
		"Unknown or conflicting style."
	},
#endif // #ifdef WSA_QOS_BAD_STYLE
#ifdef WSA_QOS_BAD_OBJECT
	{	WSA_QOS_BAD_OBJECT,                                       "WSA_QOS_BAD_OBJECT",
		"Problem with some part of the filterspec or providerspecific buffer in general.",
		"Problem with some part of the filterspec or providerspecific buffer in general."
	},
#endif // #ifdef WSA_QOS_BAD_OBJECT
#ifdef WSA_QOS_TRAFFIC_CTRL_ERROR
	{	WSA_QOS_TRAFFIC_CTRL_ERROR,                               "WSA_QOS_TRAFFIC_CTRL_ERROR",
		"Problem with some part of the flowspec.",
		"Problem with some part of the flowspec."
	},
#endif // #ifdef WSA_QOS_TRAFFIC_CTRL_ERROR
#ifdef WSA_QOS_GENERIC_ERROR
	{	WSA_QOS_GENERIC_ERROR,                                    "WSA_QOS_GENERIC_ERROR",
		"General QOS error.",
		"General QOS error."
	},
#endif // #ifdef WSA_QOS_GENERIC_ERROR
#ifdef WSA_QOS_ESERVICETYPE
	{	WSA_QOS_ESERVICETYPE,                                     "WSA_QOS_ESERVICETYPE",
		"An invalid or unrecognized service type was found in the flowspec.",
		"An invalid or unrecognized service type was found in the flowspec."
	},
#endif // #ifdef WSA_QOS_ESERVICETYPE
#ifdef WSA_QOS_EFLOWSPEC
	{	WSA_QOS_EFLOWSPEC,                                        "WSA_QOS_EFLOWSPEC",
		"An invalid or inconsistent flowspec was found in the QOS structure.",
		"An invalid or inconsistent flowspec was found in the QOS structure."
	},
#endif // #ifdef WSA_QOS_EFLOWSPEC
#ifdef WSA_QOS_EPROVSPECBUF
	{	WSA_QOS_EPROVSPECBUF,                                     "WSA_QOS_EPROVSPECBUF",
		"Invalid QOS provider-specific buffer.",
		"Invalid QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_EPROVSPECBUF
#ifdef WSA_QOS_EFILTERSTYLE
	{	WSA_QOS_EFILTERSTYLE,                                     "WSA_QOS_EFILTERSTYLE",
		"An invalid QOS filter style was used.",
		"An invalid QOS filter style was used."
	},
#endif // #ifdef WSA_QOS_EFILTERSTYLE
#ifdef WSA_QOS_EFILTERTYPE
	{	WSA_QOS_EFILTERTYPE,                                      "WSA_QOS_EFILTERTYPE",
		"An invalid QOS filter type was used.",
		"An invalid QOS filter type was used."
	},
#endif // #ifdef WSA_QOS_EFILTERTYPE
#ifdef WSA_QOS_EFILTERCOUNT
	{	WSA_QOS_EFILTERCOUNT,                                     "WSA_QOS_EFILTERCOUNT",
		"An incorrect number of QOS FILTERSPECs were specified in the FLOWDESCRIPTOR.",
		"An incorrect number of QOS FILTERSPECs were specified in the FLOWDESCRIPTOR."
	},
#endif // #ifdef WSA_QOS_EFILTERCOUNT
#ifdef WSA_QOS_EOBJLENGTH
	{	WSA_QOS_EOBJLENGTH,                                       "WSA_QOS_EOBJLENGTH",
		"An object with an invalid ObjectLength field was specified in the QOS provider-specific buffer.",
		"An object with an invalid ObjectLength field was specified in the QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_EOBJLENGTH
#ifdef WSA_QOS_EFLOWCOUNT
	{	WSA_QOS_EFLOWCOUNT,                                       "WSA_QOS_EFLOWCOUNT",
		"An incorrect number of flow descriptors was specified in the QOS structure.",
		"An incorrect number of flow descriptors was specified in the QOS structure."
	},
#endif // #ifdef WSA_QOS_EFLOWCOUNT
#ifdef WSA_QOS_EUNKNOWNPSOBJ
	{	WSA_QOS_EUNKNOWNPSOBJ,                                    "WSA_QOS_EUNKNOWNPSOBJ",
		"An unrecognized object was found in the QOS provider-specific buffer.",
		"An unrecognized object was found in the QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_EUNKNOWNPSOBJ
#ifdef WSA_QOS_EPOLICYOBJ
	{	WSA_QOS_EPOLICYOBJ,                                       "WSA_QOS_EPOLICYOBJ",
		"An invalid policy object was found in the QOS provider-specific buffer.",
		"An invalid policy object was found in the QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_EPOLICYOBJ
#ifdef WSA_QOS_EFLOWDESC
	{	WSA_QOS_EFLOWDESC,                                        "WSA_QOS_EFLOWDESC",
		"An invalid QOS flow descriptor was found in the flow descriptor list.",
		"An invalid QOS flow descriptor was found in the flow descriptor list."
	},
#endif // #ifdef WSA_QOS_EFLOWDESC
#ifdef WSA_QOS_EPSFLOWSPEC
	{	WSA_QOS_EPSFLOWSPEC,                                      "WSA_QOS_EPSFLOWSPEC",
		"An invalid or inconsistent flowspec was found in the QOS provider-specific buffer.",
		"An invalid or inconsistent flowspec was found in the QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_EPSFLOWSPEC
#ifdef WSA_QOS_EPSFILTERSPEC
	{	WSA_QOS_EPSFILTERSPEC,                                    "WSA_QOS_EPSFILTERSPEC",
		"An invalid FILTERSPEC was found in the QOS provider-specific buffer.",
		"An invalid FILTERSPEC was found in the QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_EPSFILTERSPEC
#ifdef WSA_QOS_ESDMODEOBJ
	{	WSA_QOS_ESDMODEOBJ,                                       "WSA_QOS_ESDMODEOBJ",
		"An invalid shape discard mode object was found in the QOS provider-specific buffer.",
		"An invalid shape discard mode object was found in the QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_ESDMODEOBJ
#ifdef WSA_QOS_ESHAPERATEOBJ
	{	WSA_QOS_ESHAPERATEOBJ,                                    "WSA_QOS_ESHAPERATEOBJ",
		"An invalid shaping rate object was found in the QOS provider-specific buffer.",
		"An invalid shaping rate object was found in the QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_ESHAPERATEOBJ
#ifdef WSA_QOS_RESERVED_PETYPE
	{	WSA_QOS_RESERVED_PETYPE,                                  "WSA_QOS_RESERVED_PETYPE",
		"A reserved policy element was found in the QOS provider-specific buffer.",
		"A reserved policy element was found in the QOS provider-specific buffer."
	},
#endif // #ifdef WSA_QOS_RESERVED_PETYPE
#ifdef ERROR_IPSEC_QM_POLICY_EXISTS
	{	ERROR_IPSEC_QM_POLICY_EXISTS,                             "ERROR_IPSEC_QM_POLICY_EXISTS",
		"The specified quick mode policy already exists.",
		"The specified quick mode policy already exists."
	},
#endif // #ifdef ERROR_IPSEC_QM_POLICY_EXISTS
#ifdef ERROR_IPSEC_QM_POLICY_NOT_FOUND
	{	ERROR_IPSEC_QM_POLICY_NOT_FOUND,                          "ERROR_IPSEC_QM_POLICY_NOT_FOUND",
		"The specified quick mode policy was not found.",
		"The specified quick mode policy was not found."
	},
#endif // #ifdef ERROR_IPSEC_QM_POLICY_NOT_FOUND
#ifdef ERROR_IPSEC_QM_POLICY_IN_USE
	{	ERROR_IPSEC_QM_POLICY_IN_USE,                             "ERROR_IPSEC_QM_POLICY_IN_USE",
		"The specified quick mode policy is being used.",
		"The specified quick mode policy is being used."
	},
#endif // #ifdef ERROR_IPSEC_QM_POLICY_IN_USE
#ifdef ERROR_IPSEC_MM_POLICY_EXISTS
	{	ERROR_IPSEC_MM_POLICY_EXISTS,                             "ERROR_IPSEC_MM_POLICY_EXISTS",
		"The specified main mode policy already exists.",
		"The specified main mode policy already exists."
	},
#endif // #ifdef ERROR_IPSEC_MM_POLICY_EXISTS
#ifdef ERROR_IPSEC_MM_POLICY_NOT_FOUND
	{	ERROR_IPSEC_MM_POLICY_NOT_FOUND,                          "ERROR_IPSEC_MM_POLICY_NOT_FOUND",
		"The specified main mode policy was not found.",
		"The specified main mode policy was not found."
	},
#endif // #ifdef ERROR_IPSEC_MM_POLICY_NOT_FOUND
#ifdef ERROR_IPSEC_MM_POLICY_IN_USE
	{	ERROR_IPSEC_MM_POLICY_IN_USE,                             "ERROR_IPSEC_MM_POLICY_IN_USE",
		"The specified main mode policy is being used.",
		"The specified main mode policy is being used."
	},
#endif // #ifdef ERROR_IPSEC_MM_POLICY_IN_USE
#ifdef ERROR_IPSEC_MM_FILTER_EXISTS
	{	ERROR_IPSEC_MM_FILTER_EXISTS,                             "ERROR_IPSEC_MM_FILTER_EXISTS",
		"The specified main mode filter already exists.",
		"The specified main mode filter already exists."
	},
#endif // #ifdef ERROR_IPSEC_MM_FILTER_EXISTS
#ifdef ERROR_IPSEC_MM_FILTER_NOT_FOUND
	{	ERROR_IPSEC_MM_FILTER_NOT_FOUND,                          "ERROR_IPSEC_MM_FILTER_NOT_FOUND",
		"The specified main mode filter was not found.",
		"The specified main mode filter was not found."
	},
#endif // #ifdef ERROR_IPSEC_MM_FILTER_NOT_FOUND
#ifdef ERROR_IPSEC_TRANSPORT_FILTER_EXISTS
	{	ERROR_IPSEC_TRANSPORT_FILTER_EXISTS,                      "ERROR_IPSEC_TRANSPORT_FILTER_EXISTS",
		"The specified transport mode filter already exists.",
		"The specified transport mode filter already exists."
	},
#endif // #ifdef ERROR_IPSEC_TRANSPORT_FILTER_EXISTS
#ifdef ERROR_IPSEC_TRANSPORT_FILTER_NOT_FOUND
	{	ERROR_IPSEC_TRANSPORT_FILTER_NOT_FOUND,                   "ERROR_IPSEC_TRANSPORT_FILTER_NOT_FOUND",
		"The specified transport mode filter does not exist.",
		"The specified transport mode filter does not exist."
	},
#endif // #ifdef ERROR_IPSEC_TRANSPORT_FILTER_NOT_FOUND
#ifdef ERROR_IPSEC_MM_AUTH_EXISTS
	{	ERROR_IPSEC_MM_AUTH_EXISTS,                               "ERROR_IPSEC_MM_AUTH_EXISTS",
		"The specified main mode authentication list exists.",
		"The specified main mode authentication list exists."
	},
#endif // #ifdef ERROR_IPSEC_MM_AUTH_EXISTS
#ifdef ERROR_IPSEC_MM_AUTH_NOT_FOUND
	{	ERROR_IPSEC_MM_AUTH_NOT_FOUND,                            "ERROR_IPSEC_MM_AUTH_NOT_FOUND",
		"The specified main mode authentication list was not found.",
		"The specified main mode authentication list was not found."
	},
#endif // #ifdef ERROR_IPSEC_MM_AUTH_NOT_FOUND
#ifdef ERROR_IPSEC_MM_AUTH_IN_USE
	{	ERROR_IPSEC_MM_AUTH_IN_USE,                               "ERROR_IPSEC_MM_AUTH_IN_USE",
		"The specified quick mode policy is being used.",
		"The specified quick mode policy is being used."
	},
#endif // #ifdef ERROR_IPSEC_MM_AUTH_IN_USE
#ifdef ERROR_IPSEC_DEFAULT_MM_POLICY_NOT_FOUND
	{	ERROR_IPSEC_DEFAULT_MM_POLICY_NOT_FOUND,                  "ERROR_IPSEC_DEFAULT_MM_POLICY_NOT_FOUND",
		"The specified main mode policy was not found.",
		"The specified main mode policy was not found."
	},
#endif // #ifdef ERROR_IPSEC_DEFAULT_MM_POLICY_NOT_FOUND
#ifdef ERROR_IPSEC_DEFAULT_MM_AUTH_NOT_FOUND
	{	ERROR_IPSEC_DEFAULT_MM_AUTH_NOT_FOUND,                    "ERROR_IPSEC_DEFAULT_MM_AUTH_NOT_FOUND",
		"The specified quick mode policy was not found.",
		"The specified quick mode policy was not found."
	},
#endif // #ifdef ERROR_IPSEC_DEFAULT_MM_AUTH_NOT_FOUND
#ifdef ERROR_IPSEC_DEFAULT_QM_POLICY_NOT_FOUND
	{	ERROR_IPSEC_DEFAULT_QM_POLICY_NOT_FOUND,                  "ERROR_IPSEC_DEFAULT_QM_POLICY_NOT_FOUND",
		"The manifest file contains one or more syntax errors.",
		"The manifest file contains one or more syntax errors."
	},
#endif // #ifdef ERROR_IPSEC_DEFAULT_QM_POLICY_NOT_FOUND
#ifdef ERROR_IPSEC_TUNNEL_FILTER_EXISTS
	{	ERROR_IPSEC_TUNNEL_FILTER_EXISTS,                         "ERROR_IPSEC_TUNNEL_FILTER_EXISTS",
		"The application attempted to activate a disabled activation context.",
		"The application attempted to activate a disabled activation context."
	},
#endif // #ifdef ERROR_IPSEC_TUNNEL_FILTER_EXISTS
#ifdef ERROR_IPSEC_TUNNEL_FILTER_NOT_FOUND
	{	ERROR_IPSEC_TUNNEL_FILTER_NOT_FOUND,                      "ERROR_IPSEC_TUNNEL_FILTER_NOT_FOUND",
		"The requested lookup key was not found in any active activation context.",
		"The requested lookup key was not found in any active activation context."
	},
#endif // #ifdef ERROR_IPSEC_TUNNEL_FILTER_NOT_FOUND
#ifdef ERROR_IPSEC_MM_FILTER_PENDING_DELETION
	{	ERROR_IPSEC_MM_FILTER_PENDING_DELETION,                   "ERROR_IPSEC_MM_FILTER_PENDING_DELETION",
		"The Main Mode filter is pending deletion.",
		"The Main Mode filter is pending deletion."
	},
#endif // #ifdef ERROR_IPSEC_MM_FILTER_PENDING_DELETION
#ifdef ERROR_IPSEC_TRANSPORT_FILTER_PENDING_DELETION
	{	ERROR_IPSEC_TRANSPORT_FILTER_PENDING_DELETION,            "ERROR_IPSEC_TRANSPORT_FILTER_PENDING_DELETION",
		"The transport filter is pending deletion.",
		"The transport filter is pending deletion."
	},
#endif // #ifdef ERROR_IPSEC_TRANSPORT_FILTER_PENDING_DELETION
#ifdef ERROR_IPSEC_TUNNEL_FILTER_PENDING_DELETION
	{	ERROR_IPSEC_TUNNEL_FILTER_PENDING_DELETION,               "ERROR_IPSEC_TUNNEL_FILTER_PENDING_DELETION",
		"The tunnel filter is pending deletion.",
		"The tunnel filter is pending deletion."
	},
#endif // #ifdef ERROR_IPSEC_TUNNEL_FILTER_PENDING_DELETION
#ifdef ERROR_IPSEC_MM_POLICY_PENDING_DELETION
	{	ERROR_IPSEC_MM_POLICY_PENDING_DELETION,                   "ERROR_IPSEC_MM_POLICY_PENDING_DELETION",
		"The Main Mode policy is pending deletion.",
		"The Main Mode policy is pending deletion."
	},
#endif // #ifdef ERROR_IPSEC_MM_POLICY_PENDING_DELETION
#ifdef ERROR_IPSEC_MM_AUTH_PENDING_DELETION
	{	ERROR_IPSEC_MM_AUTH_PENDING_DELETION,                     "ERROR_IPSEC_MM_AUTH_PENDING_DELETION",
		"The Main Mode authentication bundle is pending deletion.",
		"The Main Mode authentication bundle is pending deletion."
	},
#endif // #ifdef ERROR_IPSEC_MM_AUTH_PENDING_DELETION
#ifdef ERROR_IPSEC_QM_POLICY_PENDING_DELETION
	{	ERROR_IPSEC_QM_POLICY_PENDING_DELETION,                   "ERROR_IPSEC_QM_POLICY_PENDING_DELETION",
		"The Quick Mode policy is pending deletion.",
		"The Quick Mode policy is pending deletion."
	},
#endif // #ifdef ERROR_IPSEC_QM_POLICY_PENDING_DELETION
#ifdef WARNING_IPSEC_MM_POLICY_PRUNED
	{	WARNING_IPSEC_MM_POLICY_PRUNED,                           "WARNING_IPSEC_MM_POLICY_PRUNED",
		"The Main Mode policy was successfully added, but some of the requested offers are not supported.",
		"The Main Mode policy was successfully added, but some of the requested offers are not supported."
	},
#endif // #ifdef WARNING_IPSEC_MM_POLICY_PRUNED
#ifdef WARNING_IPSEC_QM_POLICY_PRUNED
	{	WARNING_IPSEC_QM_POLICY_PRUNED,                           "WARNING_IPSEC_QM_POLICY_PRUNED",
		"The Quick Mode policy was successfully added, but some of the requested offers are not supported.",
		"The Quick Mode policy was successfully added, but some of the requested offers are not supported."
	},
#endif // #ifdef WARNING_IPSEC_QM_POLICY_PRUNED
#ifdef ERROR_IPSEC_IKE_AUTH_FAIL
	{	ERROR_IPSEC_IKE_AUTH_FAIL,                                "ERROR_IPSEC_IKE_AUTH_FAIL",
		"IKE authentication credentials are unacceptable.",
		"IKE authentication credentials are unacceptable."
	},
#endif // #ifdef ERROR_IPSEC_IKE_AUTH_FAIL
#ifdef ERROR_IPSEC_IKE_ATTRIB_FAIL
	{	ERROR_IPSEC_IKE_ATTRIB_FAIL,                              "ERROR_IPSEC_IKE_ATTRIB_FAIL",
		"IKE security attributes are unacceptable.",
		"IKE security attributes are unacceptable."
	},
#endif // #ifdef ERROR_IPSEC_IKE_ATTRIB_FAIL
#ifdef ERROR_IPSEC_IKE_NEGOTIATION_PENDING
	{	ERROR_IPSEC_IKE_NEGOTIATION_PENDING,                      "ERROR_IPSEC_IKE_NEGOTIATION_PENDING",
		"IKE Negotiation in progress.",
		"IKE Negotiation in progress."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NEGOTIATION_PENDING
#ifdef ERROR_IPSEC_IKE_GENERAL_PROCESSING_ERROR
	{	ERROR_IPSEC_IKE_GENERAL_PROCESSING_ERROR,                 "ERROR_IPSEC_IKE_GENERAL_PROCESSING_ERROR",
		"General processing error.",
		"General processing error."
	},
#endif // #ifdef ERROR_IPSEC_IKE_GENERAL_PROCESSING_ERROR
#ifdef ERROR_IPSEC_IKE_TIMED_OUT
	{	ERROR_IPSEC_IKE_TIMED_OUT,                                "ERROR_IPSEC_IKE_TIMED_OUT",
		"Negotiation timed out.",
		"Negotiation timed out."
	},
#endif // #ifdef ERROR_IPSEC_IKE_TIMED_OUT
#ifdef ERROR_IPSEC_IKE_NO_CERT
	{	ERROR_IPSEC_IKE_NO_CERT,                                  "ERROR_IPSEC_IKE_NO_CERT",
		"IKE failed to find valid machine certificate.",
		"IKE failed to find valid machine certificate."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NO_CERT
#ifdef ERROR_IPSEC_IKE_SA_DELETED
	{	ERROR_IPSEC_IKE_SA_DELETED,                               "ERROR_IPSEC_IKE_SA_DELETED",
		"IKE SA deleted by peer before establishment completed.",
		"IKE SA deleted by peer before establishment completed."
	},
#endif // #ifdef ERROR_IPSEC_IKE_SA_DELETED
#ifdef ERROR_IPSEC_IKE_SA_REAPED
	{	ERROR_IPSEC_IKE_SA_REAPED,                                "ERROR_IPSEC_IKE_SA_REAPED",
		"IKE SA deleted before establishment completed.",
		"IKE SA deleted before establishment completed."
	},
#endif // #ifdef ERROR_IPSEC_IKE_SA_REAPED
#ifdef ERROR_IPSEC_IKE_MM_ACQUIRE_DROP
	{	ERROR_IPSEC_IKE_MM_ACQUIRE_DROP,                          "ERROR_IPSEC_IKE_MM_ACQUIRE_DROP",
		"Negotiation request sat in Queue too long.",
		"Negotiation request sat in Queue too long."
	},
#endif // #ifdef ERROR_IPSEC_IKE_MM_ACQUIRE_DROP
#ifdef ERROR_IPSEC_IKE_QM_ACQUIRE_DROP
	{	ERROR_IPSEC_IKE_QM_ACQUIRE_DROP,                          "ERROR_IPSEC_IKE_QM_ACQUIRE_DROP",
		"Negotiation request sat in Queue too long.",
		"Negotiation request sat in Queue too long."
	},
#endif // #ifdef ERROR_IPSEC_IKE_QM_ACQUIRE_DROP
#ifdef ERROR_IPSEC_IKE_QUEUE_DROP_MM
	{	ERROR_IPSEC_IKE_QUEUE_DROP_MM,                            "ERROR_IPSEC_IKE_QUEUE_DROP_MM",
		"Negotiation request sat in Queue too long.",
		"Negotiation request sat in Queue too long."
	},
#endif // #ifdef ERROR_IPSEC_IKE_QUEUE_DROP_MM
#ifdef ERROR_IPSEC_IKE_QUEUE_DROP_NO_MM
	{	ERROR_IPSEC_IKE_QUEUE_DROP_NO_MM,                         "ERROR_IPSEC_IKE_QUEUE_DROP_NO_MM",
		"Negotiation request sat in Queue too long.",
		"Negotiation request sat in Queue too long."
	},
#endif // #ifdef ERROR_IPSEC_IKE_QUEUE_DROP_NO_MM
#ifdef ERROR_IPSEC_IKE_DROP_NO_RESPONSE
	{	ERROR_IPSEC_IKE_DROP_NO_RESPONSE,                         "ERROR_IPSEC_IKE_DROP_NO_RESPONSE",
		"No response from peer.",
		"No response from peer."
	},
#endif // #ifdef ERROR_IPSEC_IKE_DROP_NO_RESPONSE
#ifdef ERROR_IPSEC_IKE_MM_DELAY_DROP
	{	ERROR_IPSEC_IKE_MM_DELAY_DROP,                            "ERROR_IPSEC_IKE_MM_DELAY_DROP",
		"Negotiation took too long.",
		"Negotiation took too long."
	},
#endif // #ifdef ERROR_IPSEC_IKE_MM_DELAY_DROP
#ifdef ERROR_IPSEC_IKE_QM_DELAY_DROP
	{	ERROR_IPSEC_IKE_QM_DELAY_DROP,                            "ERROR_IPSEC_IKE_QM_DELAY_DROP",
		"Negotiation took too long.",
		"Negotiation took too long."
	},
#endif // #ifdef ERROR_IPSEC_IKE_QM_DELAY_DROP
#ifdef ERROR_IPSEC_IKE_ERROR
	{	ERROR_IPSEC_IKE_ERROR,                                    "ERROR_IPSEC_IKE_ERROR",
		"Unknown error occurred.",
		"Unknown error occurred."
	},
#endif // #ifdef ERROR_IPSEC_IKE_ERROR
#ifdef ERROR_IPSEC_IKE_CRL_FAILED
	{	ERROR_IPSEC_IKE_CRL_FAILED,                               "ERROR_IPSEC_IKE_CRL_FAILED",
		"Certificate Revocation Check failed.",
		"Certificate Revocation Check failed."
	},
#endif // #ifdef ERROR_IPSEC_IKE_CRL_FAILED
#ifdef ERROR_IPSEC_IKE_INVALID_KEY_USAGE
	{	ERROR_IPSEC_IKE_INVALID_KEY_USAGE,                        "ERROR_IPSEC_IKE_INVALID_KEY_USAGE",
		"Invalid certificate key usage.",
		"Invalid certificate key usage."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_KEY_USAGE
#ifdef ERROR_IPSEC_IKE_INVALID_CERT_TYPE
	{	ERROR_IPSEC_IKE_INVALID_CERT_TYPE,                        "ERROR_IPSEC_IKE_INVALID_CERT_TYPE",
		"Invalid certificate type.",
		"Invalid certificate type."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_CERT_TYPE
#ifdef ERROR_IPSEC_IKE_NO_PRIVATE_KEY
	{	ERROR_IPSEC_IKE_NO_PRIVATE_KEY,                           "ERROR_IPSEC_IKE_NO_PRIVATE_KEY",
		"No private key associated with machine certificate.",
		"No private key associated with machine certificate."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NO_PRIVATE_KEY
#ifdef ERROR_IPSEC_IKE_DH_FAIL
	{	ERROR_IPSEC_IKE_DH_FAIL,                                  "ERROR_IPSEC_IKE_DH_FAIL",
		"Failure in Diffie-Helman computation.",
		"Failure in Diffie-Helman computation."
	},
#endif // #ifdef ERROR_IPSEC_IKE_DH_FAIL
#ifdef ERROR_IPSEC_IKE_INVALID_HEADER
	{	ERROR_IPSEC_IKE_INVALID_HEADER,                           "ERROR_IPSEC_IKE_INVALID_HEADER",
		"Invalid header.",
		"Invalid header."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_HEADER
#ifdef ERROR_IPSEC_IKE_NO_POLICY
	{	ERROR_IPSEC_IKE_NO_POLICY,                                "ERROR_IPSEC_IKE_NO_POLICY",
		"No policy configured.",
		"No policy configured."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NO_POLICY
#ifdef ERROR_IPSEC_IKE_INVALID_SIGNATURE
	{	ERROR_IPSEC_IKE_INVALID_SIGNATURE,                        "ERROR_IPSEC_IKE_INVALID_SIGNATURE",
		"Failed to verify signature.",
		"Failed to verify signature."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_SIGNATURE
#ifdef ERROR_IPSEC_IKE_KERBEROS_ERROR
	{	ERROR_IPSEC_IKE_KERBEROS_ERROR,                           "ERROR_IPSEC_IKE_KERBEROS_ERROR",
		"Failed to authenticate using Kerberos.",
		"Failed to authenticate using Kerberos."
	},
#endif // #ifdef ERROR_IPSEC_IKE_KERBEROS_ERROR
#ifdef ERROR_IPSEC_IKE_NO_PUBLIC_KEY
	{	ERROR_IPSEC_IKE_NO_PUBLIC_KEY,                            "ERROR_IPSEC_IKE_NO_PUBLIC_KEY",
		"Peer's certificate did not have a public key.",
		"Peer's certificate did not have a public key."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NO_PUBLIC_KEY
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR
	{	ERROR_IPSEC_IKE_PROCESS_ERR,                              "ERROR_IPSEC_IKE_PROCESS_ERR",
		"Error processing error payload.",
		"Error processing error payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_SA
	{	ERROR_IPSEC_IKE_PROCESS_ERR_SA,                           "ERROR_IPSEC_IKE_PROCESS_ERR_SA",
		"Error processing SA payload.",
		"Error processing SA payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_SA
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_PROP
	{	ERROR_IPSEC_IKE_PROCESS_ERR_PROP,                         "ERROR_IPSEC_IKE_PROCESS_ERR_PROP",
		"Error processing Proposal payload.",
		"Error processing Proposal payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_PROP
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_TRANS
	{	ERROR_IPSEC_IKE_PROCESS_ERR_TRANS,                        "ERROR_IPSEC_IKE_PROCESS_ERR_TRANS",
		"Error processing Transform payload.",
		"Error processing Transform payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_TRANS
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_KE
	{	ERROR_IPSEC_IKE_PROCESS_ERR_KE,                           "ERROR_IPSEC_IKE_PROCESS_ERR_KE",
		"Error processing KE payload.",
		"Error processing KE payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_KE
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_ID
	{	ERROR_IPSEC_IKE_PROCESS_ERR_ID,                           "ERROR_IPSEC_IKE_PROCESS_ERR_ID",
		"Error processing ID payload.",
		"Error processing ID payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_ID
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_CERT
	{	ERROR_IPSEC_IKE_PROCESS_ERR_CERT,                         "ERROR_IPSEC_IKE_PROCESS_ERR_CERT",
		"Error processing Cert payload.",
		"Error processing Cert payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_CERT
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_CERT_REQ
	{	ERROR_IPSEC_IKE_PROCESS_ERR_CERT_REQ,                     "ERROR_IPSEC_IKE_PROCESS_ERR_CERT_REQ",
		"Error processing Certificate Request payload.",
		"Error processing Certificate Request payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_CERT_REQ
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_HASH
	{	ERROR_IPSEC_IKE_PROCESS_ERR_HASH,                         "ERROR_IPSEC_IKE_PROCESS_ERR_HASH",
		"Error processing Hash payload.",
		"Error processing Hash payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_HASH
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_SIG
	{	ERROR_IPSEC_IKE_PROCESS_ERR_SIG,                          "ERROR_IPSEC_IKE_PROCESS_ERR_SIG",
		"Error processing Signature payload.",
		"Error processing Signature payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_SIG
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_NONCE
	{	ERROR_IPSEC_IKE_PROCESS_ERR_NONCE,                        "ERROR_IPSEC_IKE_PROCESS_ERR_NONCE",
		"Error processing Nonce payload.",
		"Error processing Nonce payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_NONCE
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_NOTIFY
	{	ERROR_IPSEC_IKE_PROCESS_ERR_NOTIFY,                       "ERROR_IPSEC_IKE_PROCESS_ERR_NOTIFY",
		"Error processing Notify payload.",
		"Error processing Notify payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_NOTIFY
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_DELETE
	{	ERROR_IPSEC_IKE_PROCESS_ERR_DELETE,                       "ERROR_IPSEC_IKE_PROCESS_ERR_DELETE",
		"Error processing Delete Payload.",
		"Error processing Delete Payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_DELETE
#ifdef ERROR_IPSEC_IKE_PROCESS_ERR_VENDOR
	{	ERROR_IPSEC_IKE_PROCESS_ERR_VENDOR,                       "ERROR_IPSEC_IKE_PROCESS_ERR_VENDOR",
		"Error processing VendorId payload.",
		"Error processing VendorId payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PROCESS_ERR_VENDOR
#ifdef ERROR_IPSEC_IKE_INVALID_PAYLOAD
	{	ERROR_IPSEC_IKE_INVALID_PAYLOAD,                          "ERROR_IPSEC_IKE_INVALID_PAYLOAD",
		"Invalid payload received.",
		"Invalid payload received."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_PAYLOAD
#ifdef ERROR_IPSEC_IKE_LOAD_SOFT_SA
	{	ERROR_IPSEC_IKE_LOAD_SOFT_SA,                             "ERROR_IPSEC_IKE_LOAD_SOFT_SA",
		"Soft SA loaded.",
		"Soft SA loaded."
	},
#endif // #ifdef ERROR_IPSEC_IKE_LOAD_SOFT_SA
#ifdef ERROR_IPSEC_IKE_SOFT_SA_TORN_DOWN
	{	ERROR_IPSEC_IKE_SOFT_SA_TORN_DOWN,                        "ERROR_IPSEC_IKE_SOFT_SA_TORN_DOWN",
		"Soft SA torn down.",
		"Soft SA torn down."
	},
#endif // #ifdef ERROR_IPSEC_IKE_SOFT_SA_TORN_DOWN
#ifdef ERROR_IPSEC_IKE_INVALID_COOKIE
	{	ERROR_IPSEC_IKE_INVALID_COOKIE,                           "ERROR_IPSEC_IKE_INVALID_COOKIE",
		"Invalid cookie received..",
		"Invalid cookie received.."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_COOKIE
#ifdef ERROR_IPSEC_IKE_NO_PEER_CERT
	{	ERROR_IPSEC_IKE_NO_PEER_CERT,                             "ERROR_IPSEC_IKE_NO_PEER_CERT",
		"Peer failed to send valid machine certificate.",
		"Peer failed to send valid machine certificate."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NO_PEER_CERT
#ifdef ERROR_IPSEC_IKE_PEER_CRL_FAILED
	{	ERROR_IPSEC_IKE_PEER_CRL_FAILED,                          "ERROR_IPSEC_IKE_PEER_CRL_FAILED",
		"Certification Revocation check of peer's certificate failed.",
		"Certification Revocation check of peer's certificate failed."
	},
#endif // #ifdef ERROR_IPSEC_IKE_PEER_CRL_FAILED
#ifdef ERROR_IPSEC_IKE_POLICY_CHANGE
	{	ERROR_IPSEC_IKE_POLICY_CHANGE,                            "ERROR_IPSEC_IKE_POLICY_CHANGE",
		"New policy invalidated SAs formed with old policy.",
		"New policy invalidated SAs formed with old policy."
	},
#endif // #ifdef ERROR_IPSEC_IKE_POLICY_CHANGE
#ifdef ERROR_IPSEC_IKE_NO_MM_POLICY
	{	ERROR_IPSEC_IKE_NO_MM_POLICY,                             "ERROR_IPSEC_IKE_NO_MM_POLICY",
		"There is no available Main Mode IKE policy.",
		"There is no available Main Mode IKE policy."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NO_MM_POLICY
#ifdef ERROR_IPSEC_IKE_NOTCBPRIV
	{	ERROR_IPSEC_IKE_NOTCBPRIV,                                "ERROR_IPSEC_IKE_NOTCBPRIV",
		"Failed to enabled TCB privilege.",
		"Failed to enabled TCB privilege."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NOTCBPRIV
#ifdef ERROR_IPSEC_IKE_SECLOADFAIL
	{	ERROR_IPSEC_IKE_SECLOADFAIL,                              "ERROR_IPSEC_IKE_SECLOADFAIL",
		"Failed to load SECURITY.DLL.",
		"Failed to load SECURITY.DLL."
	},
#endif // #ifdef ERROR_IPSEC_IKE_SECLOADFAIL
#ifdef ERROR_IPSEC_IKE_FAILSSPINIT
	{	ERROR_IPSEC_IKE_FAILSSPINIT,                              "ERROR_IPSEC_IKE_FAILSSPINIT",
		"Failed to obtain security function table dispatch address from SSPI.",
		"Failed to obtain security function table dispatch address from SSPI."
	},
#endif // #ifdef ERROR_IPSEC_IKE_FAILSSPINIT
#ifdef ERROR_IPSEC_IKE_FAILQUERYSSP
	{	ERROR_IPSEC_IKE_FAILQUERYSSP,                             "ERROR_IPSEC_IKE_FAILQUERYSSP",
		"Failed to query Kerberos package to obtain max token size.",
		"Failed to query Kerberos package to obtain max token size."
	},
#endif // #ifdef ERROR_IPSEC_IKE_FAILQUERYSSP
#ifdef ERROR_IPSEC_IKE_SRVACQFAIL
	{	ERROR_IPSEC_IKE_SRVACQFAIL,                               "ERROR_IPSEC_IKE_SRVACQFAIL",
		"Failed to obtain Kerberos server credentials for ISAKMP/ERROR_IPSEC_IKE service. Kerberos authentication will not function. The most likely reason for this is lack of domain membership. This is normal if your computer is a member of a workgroup.",
		"Failed to obtain Kerberos server credentials for ISAKMP/ERROR_IPSEC_IKE service. Kerberos authentication will not function. The most likely reason for this is lack of domain membership. This is normal if your computer is a member of a workgroup."
	},
#endif // #ifdef ERROR_IPSEC_IKE_SRVACQFAIL
#ifdef ERROR_IPSEC_IKE_SRVQUERYCRED
	{	ERROR_IPSEC_IKE_SRVQUERYCRED,                             "ERROR_IPSEC_IKE_SRVQUERYCRED",
		"Failed to determine SSPI principal name for ISAKMP/ERROR_IPSEC_IKE service (QueryCredentialsAttributes).",
		"Failed to determine SSPI principal name for ISAKMP/ERROR_IPSEC_IKE service (QueryCredentialsAttributes)."
	},
#endif // #ifdef ERROR_IPSEC_IKE_SRVQUERYCRED
#ifdef ERROR_IPSEC_IKE_GETSPIFAIL
	{	ERROR_IPSEC_IKE_GETSPIFAIL,                               "ERROR_IPSEC_IKE_GETSPIFAIL",
		"Failed to obtain new SPI for the inbound SA from Ipsec driver. The most common cause for this is that the driver does not have the correct filter. Check your policy to verify the filters.",
		"Failed to obtain new SPI for the inbound SA from Ipsec driver. The most common cause for this is that the driver does not have the correct filter. Check your policy to verify the filters."
	},
#endif // #ifdef ERROR_IPSEC_IKE_GETSPIFAIL
#ifdef ERROR_IPSEC_IKE_INVALID_FILTER
	{	ERROR_IPSEC_IKE_INVALID_FILTER,                           "ERROR_IPSEC_IKE_INVALID_FILTER",
		"Given filter is invalid.",
		"Given filter is invalid."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_FILTER
#ifdef ERROR_IPSEC_IKE_OUT_OF_MEMORY
	{	ERROR_IPSEC_IKE_OUT_OF_MEMORY,                            "ERROR_IPSEC_IKE_OUT_OF_MEMORY",
		"Memory allocation failed.",
		"Memory allocation failed."
	},
#endif // #ifdef ERROR_IPSEC_IKE_OUT_OF_MEMORY
#ifdef ERROR_IPSEC_IKE_ADD_UPDATE_KEY_FAILED
	{	ERROR_IPSEC_IKE_ADD_UPDATE_KEY_FAILED,                    "ERROR_IPSEC_IKE_ADD_UPDATE_KEY_FAILED",
		"Failed to add Security Association to IPSec Driver. The most common cause for this is if the IKE negotiation took too long to complete. If the problem persists, reduce the load on the faulting machine.",
		"Failed to add Security Association to IPSec Driver. The most common cause for this is if the IKE negotiation took too long to complete. If the problem persists, reduce the load on the faulting machine."
	},
#endif // #ifdef ERROR_IPSEC_IKE_ADD_UPDATE_KEY_FAILED
#ifdef ERROR_IPSEC_IKE_INVALID_POLICY
	{	ERROR_IPSEC_IKE_INVALID_POLICY,                           "ERROR_IPSEC_IKE_INVALID_POLICY",
		"Invalid policy.",
		"Invalid policy."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_POLICY
#ifdef ERROR_IPSEC_IKE_UNKNOWN_DOI
	{	ERROR_IPSEC_IKE_UNKNOWN_DOI,                              "ERROR_IPSEC_IKE_UNKNOWN_DOI",
		"Invalid DOI.",
		"Invalid DOI."
	},
#endif // #ifdef ERROR_IPSEC_IKE_UNKNOWN_DOI
#ifdef ERROR_IPSEC_IKE_INVALID_SITUATION
	{	ERROR_IPSEC_IKE_INVALID_SITUATION,                        "ERROR_IPSEC_IKE_INVALID_SITUATION",
		"Invalid situation.",
		"Invalid situation."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_SITUATION
#ifdef ERROR_IPSEC_IKE_DH_FAILURE
	{	ERROR_IPSEC_IKE_DH_FAILURE,                               "ERROR_IPSEC_IKE_DH_FAILURE",
		"Diffie-Hellman failure.",
		"Diffie-Hellman failure."
	},
#endif // #ifdef ERROR_IPSEC_IKE_DH_FAILURE
#ifdef ERROR_IPSEC_IKE_INVALID_GROUP
	{	ERROR_IPSEC_IKE_INVALID_GROUP,                            "ERROR_IPSEC_IKE_INVALID_GROUP",
		"Invalid Diffie-Hellman group.",
		"Invalid Diffie-Hellman group."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_GROUP
#ifdef ERROR_IPSEC_IKE_ENCRYPT
	{	ERROR_IPSEC_IKE_ENCRYPT,                                  "ERROR_IPSEC_IKE_ENCRYPT",
		"Error encrypting payload.",
		"Error encrypting payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_ENCRYPT
#ifdef ERROR_IPSEC_IKE_DECRYPT
	{	ERROR_IPSEC_IKE_DECRYPT,                                  "ERROR_IPSEC_IKE_DECRYPT",
		"Error decrypting payload.",
		"Error decrypting payload."
	},
#endif // #ifdef ERROR_IPSEC_IKE_DECRYPT
#ifdef ERROR_IPSEC_IKE_POLICY_MATCH
	{	ERROR_IPSEC_IKE_POLICY_MATCH,                             "ERROR_IPSEC_IKE_POLICY_MATCH",
		"Policy match error.",
		"Policy match error."
	},
#endif // #ifdef ERROR_IPSEC_IKE_POLICY_MATCH
#ifdef ERROR_IPSEC_IKE_UNSUPPORTED_ID
	{	ERROR_IPSEC_IKE_UNSUPPORTED_ID,                           "ERROR_IPSEC_IKE_UNSUPPORTED_ID",
		"Unsupported ID.",
		"Unsupported ID."
	},
#endif // #ifdef ERROR_IPSEC_IKE_UNSUPPORTED_ID
#ifdef ERROR_IPSEC_IKE_INVALID_HASH
	{	ERROR_IPSEC_IKE_INVALID_HASH,                             "ERROR_IPSEC_IKE_INVALID_HASH",
		"Hash verification failed.",
		"Hash verification failed."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_HASH
#ifdef ERROR_IPSEC_IKE_INVALID_HASH_ALG
	{	ERROR_IPSEC_IKE_INVALID_HASH_ALG,                         "ERROR_IPSEC_IKE_INVALID_HASH_ALG",
		"Invalid hash algorithm.",
		"Invalid hash algorithm."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_HASH_ALG
#ifdef ERROR_IPSEC_IKE_INVALID_HASH_SIZE
	{	ERROR_IPSEC_IKE_INVALID_HASH_SIZE,                        "ERROR_IPSEC_IKE_INVALID_HASH_SIZE",
		"Invalid hash size.",
		"Invalid hash size."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_HASH_SIZE
#ifdef ERROR_IPSEC_IKE_INVALID_ENCRYPT_ALG
	{	ERROR_IPSEC_IKE_INVALID_ENCRYPT_ALG,                      "ERROR_IPSEC_IKE_INVALID_ENCRYPT_ALG",
		"Invalid encryption algorithm.",
		"Invalid encryption algorithm."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_ENCRYPT_ALG
#ifdef ERROR_IPSEC_IKE_INVALID_AUTH_ALG
	{	ERROR_IPSEC_IKE_INVALID_AUTH_ALG,                         "ERROR_IPSEC_IKE_INVALID_AUTH_ALG",
		"Invalid authentication algorithm.",
		"Invalid authentication algorithm."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_AUTH_ALG
#ifdef ERROR_IPSEC_IKE_INVALID_SIG
	{	ERROR_IPSEC_IKE_INVALID_SIG,                              "ERROR_IPSEC_IKE_INVALID_SIG",
		"Invalid certificate signature.",
		"Invalid certificate signature."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_SIG
#ifdef ERROR_IPSEC_IKE_LOAD_FAILED
	{	ERROR_IPSEC_IKE_LOAD_FAILED,                              "ERROR_IPSEC_IKE_LOAD_FAILED",
		"Load failed.",
		"Load failed."
	},
#endif // #ifdef ERROR_IPSEC_IKE_LOAD_FAILED
#ifdef ERROR_IPSEC_IKE_RPC_DELETE
	{	ERROR_IPSEC_IKE_RPC_DELETE,                               "ERROR_IPSEC_IKE_RPC_DELETE",
		"Deleted via RPC call.",
		"Deleted via RPC call."
	},
#endif // #ifdef ERROR_IPSEC_IKE_RPC_DELETE
#ifdef ERROR_IPSEC_IKE_BENIGN_REINIT
	{	ERROR_IPSEC_IKE_BENIGN_REINIT,                            "ERROR_IPSEC_IKE_BENIGN_REINIT",
		"Temporary state created to perform reinit. This is not a real failure.",
		"Temporary state created to perform reinit. This is not a real failure."
	},
#endif // #ifdef ERROR_IPSEC_IKE_BENIGN_REINIT
#ifdef ERROR_IPSEC_IKE_INVALID_RESPONDER_LIFETIME_NOTIFY
	{	ERROR_IPSEC_IKE_INVALID_RESPONDER_LIFETIME_NOTIFY,        "ERROR_IPSEC_IKE_INVALID_RESPONDER_LIFETIME_NOTIFY",
		"The lifetime value received in the Responder Lifetime Notify is below the Windows 2000 configured minimum value. Please fix the policy on the peer machine.",
		"The lifetime value received in the Responder Lifetime Notify is below the Windows 2000 configured minimum value. Please fix the policy on the peer machine."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_RESPONDER_LIFETIME_NOTIFY
#ifdef ERROR_IPSEC_IKE_INVALID_CERT_KEYLEN
	{	ERROR_IPSEC_IKE_INVALID_CERT_KEYLEN,                      "ERROR_IPSEC_IKE_INVALID_CERT_KEYLEN",
		"Key length in certificate is too small for configured security requirements.",
		"Key length in certificate is too small for configured security requirements."
	},
#endif // #ifdef ERROR_IPSEC_IKE_INVALID_CERT_KEYLEN
#ifdef ERROR_IPSEC_IKE_MM_LIMIT
	{	ERROR_IPSEC_IKE_MM_LIMIT,                                 "ERROR_IPSEC_IKE_MM_LIMIT",
		"Max number of established MM SAs to peer exceeded.",
		"Max number of established MM SAs to peer exceeded."
	},
#endif // #ifdef ERROR_IPSEC_IKE_MM_LIMIT
#ifdef ERROR_IPSEC_IKE_NEGOTIATION_DISABLED
	{	ERROR_IPSEC_IKE_NEGOTIATION_DISABLED,                     "ERROR_IPSEC_IKE_NEGOTIATION_DISABLED",
		"IKE received a policy that disables negotiation.",
		"IKE received a policy that disables negotiation."
	},
#endif // #ifdef ERROR_IPSEC_IKE_NEGOTIATION_DISABLED
#ifdef ERROR_IPSEC_IKE_NEG_STATUS_END
	{	ERROR_IPSEC_IKE_NEG_STATUS_END,                           "ERROR_IPSEC_IKE_NEG_STATUS_END",
		"ERROR_IPSEC_IKE_NEG_STATUS_END",
		"ERROR_IPSEC_IKE_NEG_STATUS_END"
	},
#endif // #ifdef ERROR_IPSEC_IKE_NEG_STATUS_END
#ifdef ERROR_SXS_SECTION_NOT_FOUND
	{	ERROR_SXS_SECTION_NOT_FOUND,                              "ERROR_SXS_SECTION_NOT_FOUND",
		"The requested section was not present in the activation context.",
		"The requested section was not present in the activation context."
	},
#endif // #ifdef ERROR_SXS_SECTION_NOT_FOUND
#ifdef ERROR_SXS_CANT_GEN_ACTCTX
	{	ERROR_SXS_CANT_GEN_ACTCTX,                                "ERROR_SXS_CANT_GEN_ACTCTX",
		"This application has failed to start because the application configuration is incorrect. Reinstalling the application may fix this problem.",
		"This application has failed to start because the application configuration is incorrect. Reinstalling the application may fix this problem."
	},
#endif // #ifdef ERROR_SXS_CANT_GEN_ACTCTX
#ifdef ERROR_SXS_INVALID_ACTCTXDATA_FORMAT
	{	ERROR_SXS_INVALID_ACTCTXDATA_FORMAT,                      "ERROR_SXS_INVALID_ACTCTXDATA_FORMAT",
		"The application binding data format is invalid.",
		"The application binding data format is invalid."
	},
#endif // #ifdef ERROR_SXS_INVALID_ACTCTXDATA_FORMAT
#ifdef ERROR_SXS_ASSEMBLY_NOT_FOUND
	{	ERROR_SXS_ASSEMBLY_NOT_FOUND,                             "ERROR_SXS_ASSEMBLY_NOT_FOUND",
		"The referenced assembly is not installed on your system.",
		"The referenced assembly is not installed on your system."
	},
#endif // #ifdef ERROR_SXS_ASSEMBLY_NOT_FOUND
#ifdef ERROR_SXS_MANIFEST_FORMAT_ERROR
	{	ERROR_SXS_MANIFEST_FORMAT_ERROR,                          "ERROR_SXS_MANIFEST_FORMAT_ERROR",
		"The manifest file does not begin with the required tag and format information.",
		"The manifest file does not begin with the required tag and format information."
	},
#endif // #ifdef ERROR_SXS_MANIFEST_FORMAT_ERROR
#ifdef ERROR_SXS_MANIFEST_PARSE_ERROR
	{	ERROR_SXS_MANIFEST_PARSE_ERROR,                           "ERROR_SXS_MANIFEST_PARSE_ERROR",
		"The manifest file contains one or more syntax errors.",
		"The manifest file contains one or more syntax errors."
	},
#endif // #ifdef ERROR_SXS_MANIFEST_PARSE_ERROR
#ifdef ERROR_SXS_ACTIVATION_CONTEXT_DISABLED
	{	ERROR_SXS_ACTIVATION_CONTEXT_DISABLED,                    "ERROR_SXS_ACTIVATION_CONTEXT_DISABLED",
		"The application attempted to activate a disabled activation context.",
		"The application attempted to activate a disabled activation context."
	},
#endif // #ifdef ERROR_SXS_ACTIVATION_CONTEXT_DISABLED
#ifdef ERROR_SXS_KEY_NOT_FOUND
	{	ERROR_SXS_KEY_NOT_FOUND,                                  "ERROR_SXS_KEY_NOT_FOUND",
		"The requested lookup key was not found in any active activation context.",
		"The requested lookup key was not found in any active activation context."
	},
#endif // #ifdef ERROR_SXS_KEY_NOT_FOUND
#ifdef ERROR_SXS_VERSION_CONFLICT
	{	ERROR_SXS_VERSION_CONFLICT,                               "ERROR_SXS_VERSION_CONFLICT",
		"A component version required by the application conflicts with another component version already active.",
		"A component version required by the application conflicts with another component version already active."
	},
#endif // #ifdef ERROR_SXS_VERSION_CONFLICT
#ifdef ERROR_SXS_WRONG_SECTION_TYPE
	{	ERROR_SXS_WRONG_SECTION_TYPE,                             "ERROR_SXS_WRONG_SECTION_TYPE",
		"The type requested activation context section does not match the query API used.",
		"The type requested activation context section does not match the query API used."
	},
#endif // #ifdef ERROR_SXS_WRONG_SECTION_TYPE
#ifdef ERROR_SXS_THREAD_QUERIES_DISABLED
	{	ERROR_SXS_THREAD_QUERIES_DISABLED,                        "ERROR_SXS_THREAD_QUERIES_DISABLED",
		"Lack of system resources has required isolated activation to be disabled for the current thread of execution.",
		"Lack of system resources has required isolated activation to be disabled for the current thread of execution."
	},
#endif // #ifdef ERROR_SXS_THREAD_QUERIES_DISABLED
#ifdef ERROR_SXS_PROCESS_DEFAULT_ALREADY_SET
	{	ERROR_SXS_PROCESS_DEFAULT_ALREADY_SET,                    "ERROR_SXS_PROCESS_DEFAULT_ALREADY_SET",
		"An attempt to set the process default activation context failed because the process default activation context was already set.",
		"An attempt to set the process default activation context failed because the process default activation context was already set."
	},
#endif // #ifdef ERROR_SXS_PROCESS_DEFAULT_ALREADY_SET
#ifdef ERROR_SXS_UNKNOWN_ENCODING_GROUP
	{	ERROR_SXS_UNKNOWN_ENCODING_GROUP,                         "ERROR_SXS_UNKNOWN_ENCODING_GROUP",
		"The encoding group identifier specified is not recognized.",
		"The encoding group identifier specified is not recognized."
	},
#endif // #ifdef ERROR_SXS_UNKNOWN_ENCODING_GROUP
#ifdef ERROR_SXS_UNKNOWN_ENCODING
	{	ERROR_SXS_UNKNOWN_ENCODING,                               "ERROR_SXS_UNKNOWN_ENCODING",
		"The encoding requested is not recognized.",
		"The encoding requested is not recognized."
	},
#endif // #ifdef ERROR_SXS_UNKNOWN_ENCODING
#ifdef ERROR_SXS_INVALID_XML_NAMESPACE_URI
	{	ERROR_SXS_INVALID_XML_NAMESPACE_URI,                      "ERROR_SXS_INVALID_XML_NAMESPACE_URI",
		"The manifest contains a reference to an invalid URI.",
		"The manifest contains a reference to an invalid URI."
	},
#endif // #ifdef ERROR_SXS_INVALID_XML_NAMESPACE_URI
#ifdef ERROR_SXS_ROOT_MANIFEST_DEPENDENCY_NOT_INSTALLED
	{	ERROR_SXS_ROOT_MANIFEST_DEPENDENCY_NOT_INSTALLED,         "ERROR_SXS_ROOT_MANIFEST_DEPENDENCY_NOT_INSTALLED",
		"The application manifest contains a reference to a dependent assembly which is not installed.",
		"The application manifest contains a reference to a dependent assembly which is not installed."
	},
#endif // #ifdef ERROR_SXS_ROOT_MANIFEST_DEPENDENCY_NOT_INSTALLED
#ifdef ERROR_SXS_LEAF_MANIFEST_DEPENDENCY_NOT_INSTALLED
	{	ERROR_SXS_LEAF_MANIFEST_DEPENDENCY_NOT_INSTALLED,         "ERROR_SXS_LEAF_MANIFEST_DEPENDENCY_NOT_INSTALLED",
		"The manifest for an assembly used by the application has a reference to a dependent assembly which is not installed.",
		"The manifest for an assembly used by the application has a reference to a dependent assembly which is not installed."
	},
#endif // #ifdef ERROR_SXS_LEAF_MANIFEST_DEPENDENCY_NOT_INSTALLED
#ifdef ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE
	{	ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE,            "ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE",
		"The manifest contains an attribute for the assembly identity which is not valid.",
		"The manifest contains an attribute for the assembly identity which is not valid."
	},
#endif // #ifdef ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE
#ifdef ERROR_SXS_MANIFEST_MISSING_REQUIRED_DEFAULT_NAMESPACE
	{	ERROR_SXS_MANIFEST_MISSING_REQUIRED_DEFAULT_NAMESPACE,    "ERROR_SXS_MANIFEST_MISSING_REQUIRED_DEFAULT_NAMESPACE",
		"The manifest is missing the required default namespace specification on the assembly element.",
		"The manifest is missing the required default namespace specification on the assembly element."
	},
#endif // #ifdef ERROR_SXS_MANIFEST_MISSING_REQUIRED_DEFAULT_NAMESPACE
#ifdef ERROR_SXS_MANIFEST_INVALID_REQUIRED_DEFAULT_NAMESPACE
	{	ERROR_SXS_MANIFEST_INVALID_REQUIRED_DEFAULT_NAMESPACE,    "ERROR_SXS_MANIFEST_INVALID_REQUIRED_DEFAULT_NAMESPACE",
		"The manifest has a default namespace specified on the assembly element but its value is not \"urn:schemas-microsoft-com:asm.v1\".",
		"The manifest has a default namespace specified on the assembly element but its value is not \"urn:schemas-microsoft-com:asm.v1\"."
	},
#endif // #ifdef ERROR_SXS_MANIFEST_INVALID_REQUIRED_DEFAULT_NAMESPACE
#ifdef ERROR_SXS_PRIVATE_MANIFEST_CROSS_PATH_WITH_REPARSE_POINT
	{	ERROR_SXS_PRIVATE_MANIFEST_CROSS_PATH_WITH_REPARSE_POINT, "ERROR_SXS_PRIVATE_MANIFEST_CROSS_PATH_WITH_REPARSE_POINT",
		"The private manifest probe has crossed the reparse-point-associated path.",
		"The private manifest probe has crossed the reparse-point-associated path."
	},
#endif // #ifdef ERROR_SXS_PRIVATE_MANIFEST_CROSS_PATH_WITH_REPARSE_POINT
#ifdef ERROR_SXS_DUPLICATE_DLL_NAME
	{	ERROR_SXS_DUPLICATE_DLL_NAME,                             "ERROR_SXS_DUPLICATE_DLL_NAME",
		"Two or more components referenced directly or indirectly by the application manifest have files by the same name.",
		"Two or more components referenced directly or indirectly by the application manifest have files by the same name."
	},
#endif // #ifdef ERROR_SXS_DUPLICATE_DLL_NAME
#ifdef ERROR_SXS_DUPLICATE_WINDOWCLASS_NAME
	{	ERROR_SXS_DUPLICATE_WINDOWCLASS_NAME,                     "ERROR_SXS_DUPLICATE_WINDOWCLASS_NAME",
		"Two or more components referenced directly or indirectly by the application manifest have window classes with the same name.",
		"Two or more components referenced directly or indirectly by the application manifest have window classes with the same name."
	},
#endif // #ifdef ERROR_SXS_DUPLICATE_WINDOWCLASS_NAME
#ifdef ERROR_SXS_DUPLICATE_CLSID
	{	ERROR_SXS_DUPLICATE_CLSID,                                "ERROR_SXS_DUPLICATE_CLSID",
		"Two or more components referenced directly or indirectly by the application manifest have the same COM server CLSIDs.",
		"Two or more components referenced directly or indirectly by the application manifest have the same COM server CLSIDs."
	},
#endif // #ifdef ERROR_SXS_DUPLICATE_CLSID
#ifdef ERROR_SXS_DUPLICATE_IID
	{	ERROR_SXS_DUPLICATE_IID,                                  "ERROR_SXS_DUPLICATE_IID",
		"Two or more components referenced directly or indirectly by the application manifest have proxies for the same COM interface IIDs.",
		"Two or more components referenced directly or indirectly by the application manifest have proxies for the same COM interface IIDs."
	},
#endif // #ifdef ERROR_SXS_DUPLICATE_IID
#ifdef ERROR_SXS_DUPLICATE_TLBID
	{	ERROR_SXS_DUPLICATE_TLBID,                                "ERROR_SXS_DUPLICATE_TLBID",
		"Two or more components referenced directly or indirectly by the application manifest have the same COM type library TLBIDs.",
		"Two or more components referenced directly or indirectly by the application manifest have the same COM type library TLBIDs."
	},
#endif // #ifdef ERROR_SXS_DUPLICATE_TLBID
#ifdef ERROR_SXS_DUPLICATE_PROGID
	{	ERROR_SXS_DUPLICATE_PROGID,                               "ERROR_SXS_DUPLICATE_PROGID",
		"Two or more components referenced directly or indirectly by the application manifest have the same COM ProgIDs.",
		"Two or more components referenced directly or indirectly by the application manifest have the same COM ProgIDs."
	},
#endif // #ifdef ERROR_SXS_DUPLICATE_PROGID
#ifdef ERROR_SXS_DUPLICATE_ASSEMBLY_NAME
	{	ERROR_SXS_DUPLICATE_ASSEMBLY_NAME,                        "ERROR_SXS_DUPLICATE_ASSEMBLY_NAME",
		"Two or more components referenced directly or indirectly by the application manifest are different versions of the same component which is not permitted.",
		"Two or more components referenced directly or indirectly by the application manifest are different versions of the same component which is not permitted."
	},
#endif // #ifdef ERROR_SXS_DUPLICATE_ASSEMBLY_NAME
#ifdef ERROR_SXS_FILE_HASH_MISMATCH
	{	ERROR_SXS_FILE_HASH_MISMATCH,                             "ERROR_SXS_FILE_HASH_MISMATCH",
		"A component's file does not match the verification information present in the component manifest.",
		"A component's file does not match the verification information present in the component manifest."
	},
#endif // #ifdef ERROR_SXS_FILE_HASH_MISMATCH
#ifdef ERROR_SXS_POLICY_PARSE_ERROR
	{	ERROR_SXS_POLICY_PARSE_ERROR,                             "ERROR_SXS_POLICY_PARSE_ERROR",
		"The policy manifest contains one or more syntax errors.",
		"The policy manifest contains one or more syntax errors."
	},
#endif // #ifdef ERROR_SXS_POLICY_PARSE_ERROR
#ifdef ERROR_SXS_XML_E_MISSINGQUOTE
	{	ERROR_SXS_XML_E_MISSINGQUOTE,                             "ERROR_SXS_XML_E_MISSINGQUOTE",
		"Manifest Parse Error : A string literal was expected, but no opening quote character was found.",
		"Manifest Parse Error : A string literal was expected, but no opening quote character was found."
	},
#endif // #ifdef ERROR_SXS_XML_E_MISSINGQUOTE
#ifdef ERROR_SXS_XML_E_COMMENTSYNTAX
	{	ERROR_SXS_XML_E_COMMENTSYNTAX,                            "ERROR_SXS_XML_E_COMMENTSYNTAX",
		"Manifest Parse Error : Incorrect syntax was used in a comment.",
		"Manifest Parse Error : Incorrect syntax was used in a comment."
	},
#endif // #ifdef ERROR_SXS_XML_E_COMMENTSYNTAX
#ifdef ERROR_SXS_XML_E_BADSTARTNAMECHAR
	{	ERROR_SXS_XML_E_BADSTARTNAMECHAR,                         "ERROR_SXS_XML_E_BADSTARTNAMECHAR",
		"Manifest Parse Error : A name was started with an invalid character.",
		"Manifest Parse Error : A name was started with an invalid character."
	},
#endif // #ifdef ERROR_SXS_XML_E_BADSTARTNAMECHAR
#ifdef ERROR_SXS_XML_E_BADNAMECHAR
	{	ERROR_SXS_XML_E_BADNAMECHAR,                              "ERROR_SXS_XML_E_BADNAMECHAR",
		"Manifest Parse Error : A name contained an invalid character.",
		"Manifest Parse Error : A name contained an invalid character."
	},
#endif // #ifdef ERROR_SXS_XML_E_BADNAMECHAR
#ifdef ERROR_SXS_XML_E_BADCHARINSTRING
	{	ERROR_SXS_XML_E_BADCHARINSTRING,                          "ERROR_SXS_XML_E_BADCHARINSTRING",
		"Manifest Parse Error : A string literal contained an invalid character.",
		"Manifest Parse Error : A string literal contained an invalid character."
	},
#endif // #ifdef ERROR_SXS_XML_E_BADCHARINSTRING
#ifdef ERROR_SXS_XML_E_XMLDECLSYNTAX
	{	ERROR_SXS_XML_E_XMLDECLSYNTAX,                            "ERROR_SXS_XML_E_XMLDECLSYNTAX",
		"Manifest Parse Error : Invalid syntax for an XML declaration.",
		"Manifest Parse Error : Invalid syntax for an XML declaration."
	},
#endif // #ifdef ERROR_SXS_XML_E_XMLDECLSYNTAX
#ifdef ERROR_SXS_XML_E_BADCHARDATA
	{	ERROR_SXS_XML_E_BADCHARDATA,                              "ERROR_SXS_XML_E_BADCHARDATA",
		"Manifest Parse Error : An invalid character was found in text content.",
		"Manifest Parse Error : An invalid character was found in text content."
	},
#endif // #ifdef ERROR_SXS_XML_E_BADCHARDATA
#ifdef ERROR_SXS_XML_E_MISSINGWHITESPACE
	{	ERROR_SXS_XML_E_MISSINGWHITESPACE,                        "ERROR_SXS_XML_E_MISSINGWHITESPACE",
		"Manifest Parse Error : Required white space was missing.",
		"Manifest Parse Error : Required white space was missing."
	},
#endif // #ifdef ERROR_SXS_XML_E_MISSINGWHITESPACE
#ifdef ERROR_SXS_XML_E_EXPECTINGTAGEND
	{	ERROR_SXS_XML_E_EXPECTINGTAGEND,                          "ERROR_SXS_XML_E_EXPECTINGTAGEND",
		"Manifest Parse Error : The character '>' was expected.",
		"Manifest Parse Error : The character '>' was expected."
	},
#endif // #ifdef ERROR_SXS_XML_E_EXPECTINGTAGEND
#ifdef ERROR_SXS_XML_E_MISSINGSEMICOLON
	{	ERROR_SXS_XML_E_MISSINGSEMICOLON,                         "ERROR_SXS_XML_E_MISSINGSEMICOLON",
		"Manifest Parse Error : A semi colon character was expected.",
		"Manifest Parse Error : A semi colon character was expected."
	},
#endif // #ifdef ERROR_SXS_XML_E_MISSINGSEMICOLON
#ifdef ERROR_SXS_XML_E_UNBALANCEDPAREN
	{	ERROR_SXS_XML_E_UNBALANCEDPAREN,                          "ERROR_SXS_XML_E_UNBALANCEDPAREN",
		"Manifest Parse Error : Unbalanced parentheses.",
		"Manifest Parse Error : Unbalanced parentheses."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNBALANCEDPAREN
#ifdef ERROR_SXS_XML_E_INTERNALERROR
	{	ERROR_SXS_XML_E_INTERNALERROR,                            "ERROR_SXS_XML_E_INTERNALERROR",
		"Manifest Parse Error : Internal error.",
		"Manifest Parse Error : Internal error."
	},
#endif // #ifdef ERROR_SXS_XML_E_INTERNALERROR
#ifdef ERROR_SXS_XML_E_UNEXPECTED_WHITESPACE
	{	ERROR_SXS_XML_E_UNEXPECTED_WHITESPACE,                    "ERROR_SXS_XML_E_UNEXPECTED_WHITESPACE",
		"Manifest Parse Error : White space is not allowed at this location.",
		"Manifest Parse Error : White space is not allowed at this location."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNEXPECTED_WHITESPACE
#ifdef ERROR_SXS_XML_E_INCOMPLETE_ENCODING
	{	ERROR_SXS_XML_E_INCOMPLETE_ENCODING,                      "ERROR_SXS_XML_E_INCOMPLETE_ENCODING",
		"Manifest Parse Error : End of file reached in invalid state for current encoding.",
		"Manifest Parse Error : End of file reached in invalid state for current encoding."
	},
#endif // #ifdef ERROR_SXS_XML_E_INCOMPLETE_ENCODING
#ifdef ERROR_SXS_XML_E_MISSING_PAREN
	{	ERROR_SXS_XML_E_MISSING_PAREN,                            "ERROR_SXS_XML_E_MISSING_PAREN",
		"Manifest Parse Error : Missing parenthesis.",
		"Manifest Parse Error : Missing parenthesis."
	},
#endif // #ifdef ERROR_SXS_XML_E_MISSING_PAREN
#ifdef ERROR_SXS_XML_E_EXPECTINGCLOSEQUOTE
	{	ERROR_SXS_XML_E_EXPECTINGCLOSEQUOTE,                      "ERROR_SXS_XML_E_EXPECTINGCLOSEQUOTE",
		"Manifest Parse Error : A single or double closing quote character (\' or \") is missing.",
		"Manifest Parse Error : A single or double closing quote character (\' or \") is missing."
	},
#endif // #ifdef ERROR_SXS_XML_E_EXPECTINGCLOSEQUOTE
#ifdef ERROR_SXS_XML_E_MULTIPLE_COLONS
	{	ERROR_SXS_XML_E_MULTIPLE_COLONS,                          "ERROR_SXS_XML_E_MULTIPLE_COLONS",
		"Manifest Parse Error : Multiple colons are not allowed in a name.",
		"Manifest Parse Error : Multiple colons are not allowed in a name."
	},
#endif // #ifdef ERROR_SXS_XML_E_MULTIPLE_COLONS
#ifdef ERROR_SXS_XML_E_INVALID_DECIMAL
	{	ERROR_SXS_XML_E_INVALID_DECIMAL,                          "ERROR_SXS_XML_E_INVALID_DECIMAL",
		"Manifest Parse Error : Invalid character for decimal digit.",
		"Manifest Parse Error : Invalid character for decimal digit."
	},
#endif // #ifdef ERROR_SXS_XML_E_INVALID_DECIMAL
#ifdef ERROR_SXS_XML_E_INVALID_HEXIDECIMAL
	{	ERROR_SXS_XML_E_INVALID_HEXIDECIMAL,                      "ERROR_SXS_XML_E_INVALID_HEXIDECIMAL",
		"Manifest Parse Error : Invalid character for hexadecimal digit.",
		"Manifest Parse Error : Invalid character for hexadecimal digit."
	},
#endif // #ifdef ERROR_SXS_XML_E_INVALID_HEXIDECIMAL
#ifdef ERROR_SXS_XML_E_INVALID_UNICODE
	{	ERROR_SXS_XML_E_INVALID_UNICODE,                          "ERROR_SXS_XML_E_INVALID_UNICODE",
		"Manifest Parse Error : Invalid Unicode character value for this platform.",
		"Manifest Parse Error : Invalid Unicode character value for this platform."
	},
#endif // #ifdef ERROR_SXS_XML_E_INVALID_UNICODE
#ifdef ERROR_SXS_XML_E_WHITESPACEORQUESTIONMARK
	{	ERROR_SXS_XML_E_WHITESPACEORQUESTIONMARK,                 "ERROR_SXS_XML_E_WHITESPACEORQUESTIONMARK",
		"Manifest Parse Error : Expecting white space or '?'.",
		"Manifest Parse Error : Expecting white space or '?'."
	},
#endif // #ifdef ERROR_SXS_XML_E_WHITESPACEORQUESTIONMARK
#ifdef ERROR_SXS_XML_E_UNEXPECTEDENDTAG
	{	ERROR_SXS_XML_E_UNEXPECTEDENDTAG,                         "ERROR_SXS_XML_E_UNEXPECTEDENDTAG",
		"Manifest Parse Error : End tag was not expected at this location.",
		"Manifest Parse Error : End tag was not expected at this location."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNEXPECTEDENDTAG
#ifdef ERROR_SXS_XML_E_UNCLOSEDTAG
	{	ERROR_SXS_XML_E_UNCLOSEDTAG,                              "ERROR_SXS_XML_E_UNCLOSEDTAG",
		"Manifest Parse Error : The following tags were not closed: %1.",
		"Manifest Parse Error : The following tags were not closed: %1."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNCLOSEDTAG
#ifdef ERROR_SXS_XML_E_DUPLICATEATTRIBUTE
	{	ERROR_SXS_XML_E_DUPLICATEATTRIBUTE,                       "ERROR_SXS_XML_E_DUPLICATEATTRIBUTE",
		"Manifest Parse Error : Duplicate attribute.",
		"Manifest Parse Error : Duplicate attribute."
	},
#endif // #ifdef ERROR_SXS_XML_E_DUPLICATEATTRIBUTE
#ifdef ERROR_SXS_XML_E_MULTIPLEROOTS
	{	ERROR_SXS_XML_E_MULTIPLEROOTS,                            "ERROR_SXS_XML_E_MULTIPLEROOTS",
		"Manifest Parse Error : Only one top level element is allowed in an XML document.",
		"Manifest Parse Error : Only one top level element is allowed in an XML document."
	},
#endif // #ifdef ERROR_SXS_XML_E_MULTIPLEROOTS
#ifdef ERROR_SXS_XML_E_INVALIDATROOTLEVEL
	{	ERROR_SXS_XML_E_INVALIDATROOTLEVEL,                       "ERROR_SXS_XML_E_INVALIDATROOTLEVEL",
		"Manifest Parse Error : Invalid at the top level of the document.",
		"Manifest Parse Error : Invalid at the top level of the document."
	},
#endif // #ifdef ERROR_SXS_XML_E_INVALIDATROOTLEVEL
#ifdef ERROR_SXS_XML_E_BADXMLDECL
	{	ERROR_SXS_XML_E_BADXMLDECL,                               "ERROR_SXS_XML_E_BADXMLDECL",
		"Manifest Parse Error : Invalid XML declaration.",
		"Manifest Parse Error : Invalid XML declaration."
	},
#endif // #ifdef ERROR_SXS_XML_E_BADXMLDECL
#ifdef ERROR_SXS_XML_E_MISSINGROOT
	{	ERROR_SXS_XML_E_MISSINGROOT,                              "ERROR_SXS_XML_E_MISSINGROOT",
		"Manifest Parse Error : XML document must have a top level element.",
		"Manifest Parse Error : XML document must have a top level element."
	},
#endif // #ifdef ERROR_SXS_XML_E_MISSINGROOT
#ifdef ERROR_SXS_XML_E_UNEXPECTEDEOF
	{	ERROR_SXS_XML_E_UNEXPECTEDEOF,                            "ERROR_SXS_XML_E_UNEXPECTEDEOF",
		"Manifest Parse Error : Unexpected end of file.",
		"Manifest Parse Error : Unexpected end of file."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNEXPECTEDEOF
#ifdef ERROR_SXS_XML_E_BADPEREFINSUBSET
	{	ERROR_SXS_XML_E_BADPEREFINSUBSET,                         "ERROR_SXS_XML_E_BADPEREFINSUBSET",
		"Manifest Parse Error : Parameter entities cannot be used inside markup declarations in an internal subset.",
		"Manifest Parse Error : Parameter entities cannot be used inside markup declarations in an internal subset."
	},
#endif // #ifdef ERROR_SXS_XML_E_BADPEREFINSUBSET
#ifdef ERROR_SXS_XML_E_UNCLOSEDSTARTTAG
	{	ERROR_SXS_XML_E_UNCLOSEDSTARTTAG,                         "ERROR_SXS_XML_E_UNCLOSEDSTARTTAG",
		"Manifest Parse Error : Element was not closed.",
		"Manifest Parse Error : Element was not closed."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNCLOSEDSTARTTAG
#ifdef ERROR_SXS_XML_E_UNCLOSEDENDTAG
	{	ERROR_SXS_XML_E_UNCLOSEDENDTAG,                           "ERROR_SXS_XML_E_UNCLOSEDENDTAG",
		"Manifest Parse Error : End element was missing the character '>'.",
		"Manifest Parse Error : End element was missing the character '>'."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNCLOSEDENDTAG
#ifdef ERROR_SXS_XML_E_UNCLOSEDSTRING
	{	ERROR_SXS_XML_E_UNCLOSEDSTRING,                           "ERROR_SXS_XML_E_UNCLOSEDSTRING",
		"Manifest Parse Error : A string literal was not closed.",
		"Manifest Parse Error : A string literal was not closed."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNCLOSEDSTRING
#ifdef ERROR_SXS_XML_E_UNCLOSEDCOMMENT
	{	ERROR_SXS_XML_E_UNCLOSEDCOMMENT,                          "ERROR_SXS_XML_E_UNCLOSEDCOMMENT",
		"Manifest Parse Error : A comment was not closed.",
		"Manifest Parse Error : A comment was not closed."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNCLOSEDCOMMENT
#ifdef ERROR_SXS_XML_E_UNCLOSEDDECL
	{	ERROR_SXS_XML_E_UNCLOSEDDECL,                             "ERROR_SXS_XML_E_UNCLOSEDDECL",
		"Manifest Parse Error : A declaration was not closed.",
		"Manifest Parse Error : A declaration was not closed."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNCLOSEDDECL
#ifdef ERROR_SXS_XML_E_UNCLOSEDCDATA
	{	ERROR_SXS_XML_E_UNCLOSEDCDATA,                            "ERROR_SXS_XML_E_UNCLOSEDCDATA",
		"Manifest Parse Error : A CDATA section was not closed.",
		"Manifest Parse Error : A CDATA section was not closed."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNCLOSEDCDATA
#ifdef ERROR_SXS_XML_E_RESERVEDNAMESPACE
	{	ERROR_SXS_XML_E_RESERVEDNAMESPACE,                        "ERROR_SXS_XML_E_RESERVEDNAMESPACE",
		"Manifest Parse Error : The namespace prefix is not allowed to start with the reserved string \"xml\".",
		"Manifest Parse Error : The namespace prefix is not allowed to start with the reserved string \"xml\"."
	},
#endif // #ifdef ERROR_SXS_XML_E_RESERVEDNAMESPACE
#ifdef ERROR_SXS_XML_E_INVALIDENCODING
	{	ERROR_SXS_XML_E_INVALIDENCODING,                          "ERROR_SXS_XML_E_INVALIDENCODING",
		"Manifest Parse Error : System does not support the specified encoding.",
		"Manifest Parse Error : System does not support the specified encoding."
	},
#endif // #ifdef ERROR_SXS_XML_E_INVALIDENCODING
#ifdef ERROR_SXS_XML_E_INVALIDSWITCH
	{	ERROR_SXS_XML_E_INVALIDSWITCH,                            "ERROR_SXS_XML_E_INVALIDSWITCH",
		"Manifest Parse Error : Switch from current encoding to specified encoding not supported.",
		"Manifest Parse Error : Switch from current encoding to specified encoding not supported."
	},
#endif // #ifdef ERROR_SXS_XML_E_INVALIDSWITCH
#ifdef ERROR_SXS_XML_E_BADXMLCASE
	{	ERROR_SXS_XML_E_BADXMLCASE,                               "ERROR_SXS_XML_E_BADXMLCASE",
		"Manifest Parse Error : The name 'xml' is reserved and must be lower case.",
		"Manifest Parse Error : The name 'xml' is reserved and must be lower case."
	},
#endif // #ifdef ERROR_SXS_XML_E_BADXMLCASE
#ifdef ERROR_SXS_XML_E_INVALID_STANDALONE
	{	ERROR_SXS_XML_E_INVALID_STANDALONE,                       "ERROR_SXS_XML_E_INVALID_STANDALONE",
		"Manifest Parse Error : The standalone attribute must have the value 'yes' or 'no'.",
		"Manifest Parse Error : The standalone attribute must have the value 'yes' or 'no'."
	},
#endif // #ifdef ERROR_SXS_XML_E_INVALID_STANDALONE
#ifdef ERROR_SXS_XML_E_UNEXPECTED_STANDALONE
	{	ERROR_SXS_XML_E_UNEXPECTED_STANDALONE,                    "ERROR_SXS_XML_E_UNEXPECTED_STANDALONE",
		"Manifest Parse Error : The standalone attribute cannot be used in external entities.",
		"Manifest Parse Error : The standalone attribute cannot be used in external entities."
	},
#endif // #ifdef ERROR_SXS_XML_E_UNEXPECTED_STANDALONE
#ifdef ERROR_SXS_XML_E_INVALID_VERSION
	{	ERROR_SXS_XML_E_INVALID_VERSION,                          "ERROR_SXS_XML_E_INVALID_VERSION",
		"Manifest Parse Error : Invalid version number.",
		"Manifest Parse Error : Invalid version number."
	},
#endif // #ifdef ERROR_SXS_XML_E_INVALID_VERSION
#ifdef ERROR_SXS_XML_E_MISSINGEQUALS
	{	ERROR_SXS_XML_E_MISSINGEQUALS,                            "ERROR_SXS_XML_E_MISSINGEQUALS",
		"Manifest Parse Error : Missing equals sign between attribute and attribute value.",
		"Manifest Parse Error : Missing equals sign between attribute and attribute value."
	},
#endif // #ifdef ERROR_SXS_XML_E_MISSINGEQUALS
#ifdef ERROR_SXS_PROTECTION_RECOVERY_FAILED
	{	ERROR_SXS_PROTECTION_RECOVERY_FAILED,                     "ERROR_SXS_PROTECTION_RECOVERY_FAILED",
		"Assembly Protection Error: Unable to recover the specified assembly.",
		"Assembly Protection Error: Unable to recover the specified assembly."
	},
#endif // #ifdef ERROR_SXS_PROTECTION_RECOVERY_FAILED
#ifdef ERROR_SXS_PROTECTION_PUBLIC_KEY_TOO_SHORT
	{	ERROR_SXS_PROTECTION_PUBLIC_KEY_TOO_SHORT,                "ERROR_SXS_PROTECTION_PUBLIC_KEY_TOO_SHORT",
		"Assembly Protection Error: The public key for an assembly was too short to be allowed.",
		"Assembly Protection Error: The public key for an assembly was too short to be allowed."
	},
#endif // #ifdef ERROR_SXS_PROTECTION_PUBLIC_KEY_TOO_SHORT
#ifdef ERROR_SXS_PROTECTION_CATALOG_NOT_VALID
	{	ERROR_SXS_PROTECTION_CATALOG_NOT_VALID,                   "ERROR_SXS_PROTECTION_CATALOG_NOT_VALID",
		"Assembly Protection Error: The catalog for an assembly is not valid, or does not match the assembly's manifest.",
		"Assembly Protection Error: The catalog for an assembly is not valid, or does not match the assembly's manifest."
	},
#endif // #ifdef ERROR_SXS_PROTECTION_CATALOG_NOT_VALID
#ifdef ERROR_SXS_UNTRANSLATABLE_HRESULT
	{	ERROR_SXS_UNTRANSLATABLE_HRESULT,                         "ERROR_SXS_UNTRANSLATABLE_HRESULT",
		"An HRESULT could not be translated to a corresponding Win32 error code.",
		"An HRESULT could not be translated to a corresponding Win32 error code."
	},
#endif // #ifdef ERROR_SXS_UNTRANSLATABLE_HRESULT
#ifdef ERROR_SXS_PROTECTION_CATALOG_FILE_MISSING
	{	ERROR_SXS_PROTECTION_CATALOG_FILE_MISSING,                "ERROR_SXS_PROTECTION_CATALOG_FILE_MISSING",
		"Assembly Protection Error: The catalog for an assembly is missing.",
		"Assembly Protection Error: The catalog for an assembly is missing."
	},
#endif // #ifdef ERROR_SXS_PROTECTION_CATALOG_FILE_MISSING
#ifdef ERROR_SXS_MISSING_ASSEMBLY_IDENTITY_ATTRIBUTE
	{	ERROR_SXS_MISSING_ASSEMBLY_IDENTITY_ATTRIBUTE,            "ERROR_SXS_MISSING_ASSEMBLY_IDENTITY_ATTRIBUTE",
		"The supplied assembly identity is missing one or more attributes which must be present in this context.",
		"The supplied assembly identity is missing one or more attributes which must be present in this context."
	},
#endif // #ifdef ERROR_SXS_MISSING_ASSEMBLY_IDENTITY_ATTRIBUTE
#ifdef ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE_NAME
	{	ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE_NAME,       "ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE_NAME",
		"The supplied assembly identity has one or more attribute names that contain characters not permitted in XML names.",
		"The supplied assembly identity has one or more attribute names that contain characters not permitted in XML names."
	},
#endif // #ifdef ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE_NAME
};
const unsigned int SystemErrorCount =
	(sizeof(SystemErrorList) / sizeof(SystemErrorList[0]));
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ErrnoSpec_Internal *GetErrnoPtr(int errno_code)
{
	if ((errno_code >= 0) && (errno_code < static_cast<int>(ErrnoCount)) &&
		(errno_code == ErrnoList[errno_code].error_code))
		return(ErrnoList + errno_code);
	else {
		unsigned int count_1;
		for (count_1 = 0; count_1 < ErrnoCount; count_1++) {
			if (errno_code == ErrnoList[count_1].error_code)
				return(ErrnoList + count_1);
		}
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
const SystemErrorSpec_Internal *GetSystemErrorPtr
	(SystemErrorCode error_code)
{
	if ((error_code < static_cast<int>(SystemErrorCount)) &&
		(error_code == SystemErrorList[error_code].error_code))
		return(SystemErrorList + error_code);
	else {
		unsigned int count_1;
		for (count_1 = 0; count_1 < SystemErrorCount; count_1++) {
			if (error_code == SystemErrorList[count_1].error_code)
				return(SystemErrorList + count_1);
		}
	}

	return(NULL);
}
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace 

// ////////////////////////////////////////////////////////////////////////////
std::string GetErrnoString(int errno_code, bool long_flag)
{
	
	const ErrnoSpec_Internal *spec_ptr = GetErrnoPtr(errno_code);
	char                      buffer[4196];
	char                      number_buffer[IntToString_MaxLen + 2];

	if (spec_ptr != NULL) {
		strcat(strcat(strcat(strcat(strcat(strcat(strcat(strcat(strcpy(buffer,
			"Error code "), IntToString(number_buffer, errno_code)),
			" ("), spec_ptr->error_name), " --- "),
			spec_ptr->short_description), ") "),
			(long_flag && *spec_ptr->long_description) ? " " : ""),
			(long_flag && *spec_ptr->long_description) ?
			spec_ptr->long_description : "");
	}
	else
		strcat(strcat(strcpy(buffer, "Invalid error code ("),
			IntToString(number_buffer, errno_code)), ")");

	return(buffer);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetSystemErrorString(SystemErrorCode error_code, bool long_flag)
{
	
#ifdef _Windows
	const SystemErrorSpec_Internal *spec_ptr = GetSystemErrorPtr(error_code);
	char                            buffer[4196];
	char                            number_buffer[IntToString_MaxLen + 2];

	if (spec_ptr != NULL) {
		strcat(strcat(strcat(strcat(strcat(strcat(strcat(strcat(strcpy(buffer,
			"Windows error code "), IntToString(number_buffer, error_code)),
			" ("), spec_ptr->error_name), " --- "),
			spec_ptr->short_description), ") "),
			(long_flag && *spec_ptr->long_description) ? " " : ""),
			(long_flag && *spec_ptr->long_description) ?
			spec_ptr->long_description : "");
	}
	else
		strcat(strcat(strcpy(buffer, "Invalid Windows error code ("),
			IntToString(number_buffer, error_code)), ")");

	return(buffer);
#else
	return(GetErrnoString(error_code, long_flag));
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int GetLastErrnoCode()
{
	return(errno);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SetLastErrnoCode(int error_code)
{
	errno = error_code;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowErrno(int error_code, const char *error_text)
{
	throw ExceptionErrno(error_code, error_text);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowErrno(int error_code, const std::string &error_text)
{
	ThrowErrno(error_code, error_text.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowErrno(int error_code, const std::ostringstream &error_text)
{
	ThrowErrno(error_code, error_text.str().c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowErrno(const char *error_text)
{
	ThrowErrno(GetLastErrnoCode(), error_text);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowErrno(const std::string &error_text)
{
	ThrowErrno(error_text.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowErrno(const std::ostringstream &error_text)
{
	ThrowErrno(error_text.str().c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SystemErrorCode GetLastSystemErrorCode()
{
#ifdef _Windows
	return(GetLastError());
#else
	return(errno);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SetLastSystemErrorCode(SystemErrorCode error_code)
{
#ifdef _Windows
	SetLastError(error_code);
#else
	errno = error_code;
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowSystemError(SystemErrorCode error_code, const char *error_text)
{
#ifdef _Windows
	throw ExceptionSystemError(error_code, error_text);
#else
	ThrowErrno(error_code, error_text);
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowSystemError(SystemErrorCode error_code, const std::string &error_text)
{
	ThrowSystemError(error_code, error_text.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowSystemError(SystemErrorCode error_code,
	const std::ostringstream &error_text)
{
	ThrowSystemError(error_code, error_text.str().c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowSystemError(const char *error_text)
{
	ThrowSystemError(GetLastSystemErrorCode(), error_text);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowSystemError(const std::string &error_text)
{
	ThrowSystemError(error_text.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ThrowSystemError(const std::ostringstream &error_text)
{
	ThrowSystemError(error_text.str().c_str());
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <cstdlib>
#include <iomanip>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
template <typename ErrorInfo> void ShowErrorList(const char *error_type,
	const unsigned int error_count, const ErrorInfo *error_list)
{
	std::cout << std::setfill('=') << std::setw(79) << "" << std::setfill(' ') <<
		std::endl;
	std::cout << error_type << std::endl;
	std::cout << std::setfill('-') << std::setw(79) << "" << std::setfill(' ') <<
		std::endl;

	unsigned int count_1;
	for (count_1 = 0; count_1 < error_count; ++count_1) {
		if (count_1)
			std::cout << std::setfill('-') << std::setw(79) << "" <<
				std::setfill(' ') << std::endl;
		std::cout << std::setw(11) << error_list[count_1].error_code << ": " <<
			error_list[count_1].error_name << std::endl;
		std::cout << error_list[count_1].short_description << std::endl;
		std::cout << ((error_list[count_1].long_description != NULL) ?
			error_list[count_1].long_description : "*NULL*") << std::endl;
	}

	std::cout << std::setfill('=') << std::setw(79) << "" << std::setfill(' ') <<
		std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	ShowErrorList("Errno Error List", ErrnoCount, ErrnoList);

#ifdef _Windows
	ShowErrorList("System Error List", SystemErrorCount, SystemErrorList);
#endif // #ifdef _Windows

	return(EXIT_SUCCESS);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

