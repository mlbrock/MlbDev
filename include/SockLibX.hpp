//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for SockLibX support.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock
								1996-04-10 --- Based upon the portable Network
													Programming Suppoprt Library (NPSL).
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SockLibX__SockLibX_hpp__HH

#define HH__MLB__SockLibX__SockLibX_hpp__HH		1

#define MLB__LIB__SockLibX

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/TimeSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef MLB__LIB__SockLibX
# ifdef SOCKLIBX_DLL_EXPORTS
#  define API_SOCKLIBX		MB_LIB_EXPORT
# else
#  define API_SOCKLIBX		MB_LIB_IMPORT
# endif // #ifdef SOCKLIBX_DLL_EXPORTS
#else
#  define API_SOCKLIBX
#endif // #ifdef MLB__LIB__SockLibX

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_SOCKLIBX

#ifdef _Windows
#else
# include <errno.h>
#endif // #ifdef _Windows

#include "npsl.h"

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
typedef unsigned char  NetInt_1;
typedef unsigned short NetInt_2;
typedef UINT32         NetInt_4;
#else
typedef unsigned char  NetInt_1;
typedef unsigned short NetInt_2;
typedef unsigned int   NetInt_4;
#endif // #ifdef _Windows
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef NPSL_SOCKET_HANDLE NativeSocketHandle;
typedef NPSL_SOCKLEN_T     NativeSockLen_T;
typedef NPSL_SOCKADDR_IN   NativeSockAddr_In;
typedef NPSL_SOCKADDR      NativeSockAddr;
typedef NPSL_FD_SET_T      NativeFDSet_T;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef MLB::Utility::SystemErrorCode SocketErrorCode;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
const SocketErrorCode SocketError_None        = 0;
const SocketErrorCode SocketError_WouldBlock  = WSAEWOULDBLOCK;
const SocketErrorCode SocketError_TimedOut    = WSAETIMEDOUT;
const SocketErrorCode SocketError_ConnRefused = WSAECONNREFUSED;
const SocketErrorCode SocketError_ConnReset   = WSAECONNRESET;
const SocketErrorCode SocketError_ConnAborted = WSAECONNABORTED;
#else
const SocketErrorCode SocketError_None        = 0;
const SocketErrorCode SocketError_WouldBlock  = EWOULDBLOCK;
const SocketErrorCode SocketError_TimedOut    = ETIMEDOUT;
const SocketErrorCode SocketError_ConnRefused = ECONNREFUSED;
const SocketErrorCode SocketError_ConnReset   = ECONNRESET;
const SocketErrorCode SocketError_ConnAborted = ECONNABORTED;
#endif // #ifdef _Windows
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
enum AddressFamily {
	AddressFamily_NONE      = AF_UNSPEC,
	AddressFamily_Unix      = AF_UNIX,
	AddressFamily_Internet  = AF_INET,
#ifdef AF_IMPLINK
	AddressFamily_Implink   = AF_IMPLINK,
#endif // #ifdef AF_IMPLINK
#ifdef AF_PUP
	AddressFamily_Pup       = AF_PUP,
#endif // #ifdef AF_PUP
#ifdef AF_CHAOS
	AddressFamily_Chaos     = AF_CHAOS,
#endif // #ifdef AF_CHAOS
#ifdef AF_NS
	AddressFamily_Ns        = AF_NS,
#endif // #ifdef AF_NS
#ifdef AF_IPX
	AddressFamily_Ipx       = AF_IPX,
#endif // #ifdef AF_IPX
#ifdef AF_ISO
	AddressFamily_Iso       = AF_ISO,
#endif // #ifdef AF_ISO
#ifdef AF_OSI
	AddressFamily_Osi       = AF_OSI,
#endif // #ifdef AF_OSI
#ifdef AF_ECMA
	AddressFamily_Ecma      = AF_ECMA,
#endif // #ifdef AF_ECMA
#ifdef AF_DATAKIT
	AddressFamily_Datakit   = AF_DATAKIT,
#endif // #ifdef AF_DATAKIT
#ifdef AF_CCITT
	AddressFamily_Ccitt     = AF_CCITT,
#endif // #ifdef AF_CCITT
#ifdef AF_SNA
	AddressFamily_Sna       = AF_SNA,
#endif // #ifdef AF_SNA
#ifdef AF_DECnet
	AddressFamily_Decnet    = AF_DECnet,
#endif // #ifdef AF_DECnet
#ifdef AF_DLI
	AddressFamily_Dli       = AF_DLI,
#endif // #ifdef AF_DLI
#ifdef AF_LAT
	AddressFamily_Lat       = AF_LAT,
#endif // #ifdef AF_LAT
#ifdef AF_HYLINK
	AddressFamily_Hylink    = AF_HYLINK,
#endif // #ifdef AF_HYLINK
#ifdef AF_APPLETALK
	AddressFamily_Appletalk = AF_APPLETALK,
#endif // #ifdef AF_APPLETALK
#ifdef AF_NETBIOS
	AddressFamily_Netbios   = AF_NETBIOS,
#endif // #ifdef AF_NETBIOS
#ifdef AF_VOICEVIEW
	AddressFamily_Voiceview = AF_VOICEVIEW,
#endif // #ifdef AF_VOICEVIEW
#ifdef AF_FIREFOX
	AddressFamily_Firefox   = AF_FIREFOX,
#endif // #ifdef AF_FIREFOX
#ifdef AF_UNKNOWN1
	AddressFamily_Unknown1  = AF_UNKNOWN1,
#endif // #ifdef AF_UNKNOWN1
#ifdef AF_BAN
	AddressFamily_Ban       = AF_BAN,
#endif // #ifdef AF_BAN
#ifdef AF_ATM
	AddressFamily_Atm       = AF_ATM,
#endif // #ifdef AF_ATM
#ifdef AF_INET6
	AddressFamily_Inet6     = AF_INET6,
#endif // #ifdef AF_INET6
#ifdef AF_CLUSTER
	AddressFamily_Cluster   = AF_CLUSTER,
#endif // #ifdef AF_CLUSTER
#ifdef AF_12844
	AddressFamily_12844     = AF_12844,
#endif // #ifdef AF_12844
	AddressFamily_DEFAULT   = AF_INET
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
enum SocketType {
	SocketType_Stream     = SOCK_STREAM,
	SocketType_DataGram   = SOCK_DGRAM,
	SocketType_Raw        = SOCK_RAW,
	SocketType_RDM        = SOCK_RDM,
	SocketType_SseqPacket = SOCK_SEQPACKET,
	SocketType_DEFAULT    = SOCK_STREAM
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
enum ProtocolType {
	ProtocolType_NONE      = PF_UNSPEC,
	ProtocolType_Unix      = PF_UNIX,
	ProtocolType_Internet  = PF_INET,
#ifdef PF_IMPLINK
	ProtocolType_Implink   = PF_IMPLINK,
#endif // #ifdef PF_IMPLINK
#ifdef PF_PUP
	ProtocolType_Pup       = PF_PUP,
#endif // #ifdef PF_PUP
#ifdef PF_CHAOS
	ProtocolType_Chaos     = PF_CHAOS,
#endif // #ifdef PF_CHAOS
#ifdef PF_NS
	ProtocolType_Ns        = PF_NS,
#endif // #ifdef PF_NS
#ifdef PF_IPX
	ProtocolType_Ipx       = PF_IPX,
#endif // #ifdef PF_IPX
#ifdef PF_ISO
	ProtocolType_Iso       = PF_ISO,
#endif // #ifdef PF_ISO
#ifdef PF_OSI
	ProtocolType_Osi       = PF_OSI,
#endif // #ifdef PF_OSI
#ifdef PF_ECMA
	ProtocolType_Ecma      = PF_ECMA,
#endif // #ifdef PF_ECMA
#ifdef PF_DATAKIT
	ProtocolType_Datakit   = PF_DATAKIT,
#endif // #ifdef PF_DATAKIT
#ifdef PF_CCITT
	ProtocolType_Ccitt     = PF_CCITT,
#endif // #ifdef PF_CCITT
#ifdef PF_SNA
	ProtocolType_Sna       = PF_SNA,
#endif // #ifdef PF_SNA
#ifdef PF_DECnet
	ProtocolType_Decnet    = PF_DECnet,
#endif // #ifdef PF_DECnet
#ifdef PF_DLI
	ProtocolType_Dli       = PF_DLI,
#endif // #ifdef PF_DLI
#ifdef PF_LAT
	ProtocolType_Lat       = PF_LAT,
#endif // #ifdef PF_LAT
#ifdef PF_HYLINK
	ProtocolType_Hylink    = PF_HYLINK,
#endif // #ifdef PF_HYLINK
#ifdef PF_APPLETALK
	ProtocolType_Appletalk = PF_APPLETALK,
#endif // #ifdef PF_APPLETALK
#ifdef PF_NETBIOS
	ProtocolType_Netbios   = PF_NETBIOS,
#endif // #ifdef PF_NETBIOS
#ifdef PF_VOICEVIEW
	ProtocolType_Voiceview = PF_VOICEVIEW,
#endif // #ifdef PF_VOICEVIEW
#ifdef PF_FIREFOX
	ProtocolType_Firefox   = PF_FIREFOX,
#endif // #ifdef PF_FIREFOX
#ifdef PF_UNKNOWN1
	ProtocolType_Unknown1  = PF_UNKNOWN1,
#endif // #ifdef PF_UNKNOWN1
#ifdef PF_BAN
	ProtocolType_Ban       = PF_BAN,
#endif // #ifdef PF_BAN
#ifdef PF_ATM
	ProtocolType_Atm       = PF_ATM,
#endif // #ifdef PF_ATM
#ifdef PF_INET6
	ProtocolType_Inet6     = PF_INET6,
#endif // #ifdef PF_INET6
#ifdef PF_CLUSTER
	ProtocolType_Cluster   = PF_CLUSTER,
#endif // #ifdef PF_CLUSTER
#ifdef PF_12844
	ProtocolType_12844     = PF_12844,
#endif // #ifdef PF_12844
	ProtocolType_DEFAULT   = 0
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
enum ShutDownHow {
	ShutDownHow_Recv = NPSL_SHUTDOWN_FLAG_RECV,
	ShutDownHow_Send = NPSL_SHUTDOWN_FLAG_SEND,
	ShutDownHow_Both = NPSL_SHUTDOWN_FLAG_BOTH
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_SOCKLIBX AddressIP {
public:
	AddressIP();
	explicit AddressIP(long in_address);
	explicit AddressIP(unsigned long in_address);
	explicit AddressIP(const unsigned char *in_address);
	explicit AddressIP(const char *in_address);
	explicit AddressIP(const std::string &in_address);
	AddressIP(const char *in_address, bool is_raw_ip_flag);
	AddressIP(const std::string &in_address, bool is_raw_ip_flag);
	AddressIP(const char *in_host_name, const char *in_address);
	AddressIP(const std::string &in_host_name, const std::string &in_address);

	AddressIP(const AddressIP &other);
	AddressIP & operator = (const AddressIP &other);

	void swap(AddressIP &other);

	void SetAddress(long in_address);
	void SetAddress(unsigned long in_address);
	void SetAddress(const unsigned char *in_address);
	void SetAddress(const char *in_address, bool is_raw_ip_flag = false);
	void SetAddress(const std::string &in_address, bool is_raw_ip_flag = false);
	void SetHostName(const std::string &in_host_name);

	std::string       GetHostIPAddress() const;
	std::string       GetHostIPAddressZeroFilled() const;
	std::string       GetHostId() const;
	std::string       GetHostName() const;
	struct in_addr    GetInAddr() const;
	NativeSockAddr_In GetSockaddrIn() const;
	NativeSockAddr_In GetSockaddrIn(AddressFamily address_family) const;
	NativeSockAddr    GetSockaddr() const;
	NativeSockAddr    GetSockaddr(AddressFamily address_family) const;

	std::string ToString() const;

	bool operator <  (const AddressIP &other) const;
	bool operator >  (const AddressIP &other) const;
	bool operator <= (const AddressIP &other) const;
	bool operator >= (const AddressIP &other) const;
	bool operator == (const AddressIP &other) const;
	bool operator != (const AddressIP &other) const;

	       int Compare(const AddressIP &other) const;
	static int Compare(const AddressIP *ptr_1, const AddressIP *ptr_2);

	std::string   host_name_;
	unsigned char ip_address[4];
};
API_SOCKLIBX std::ostream & operator << (std::ostream &o_str,
	const AddressIP &datum);
API_SOCKLIBX void swap(AddressIP &arg_1, AddressIP &arg_2);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_SOCKLIBX EndPointIP {
public:
	EndPointIP();
	explicit EndPointIP(const NativeSockAddr &in_sockaddr);
	explicit EndPointIP(const NativeSockAddr_In &in_sockaddr_in);
	EndPointIP(const AddressIP &in_address, int in_port);
	EndPointIP(const AddressIP &in_address, const char *in_port);
	EndPointIP(const AddressIP &in_address, const std::string &in_port);

	/**
		Constructs a local endpoint on an operating system specified interface
		(\e INADDR_ANY ).
	*/
	explicit EndPointIP(int port,
		AddressFamily address_family = AddressFamily_Internet);
	/**
		Constructs a local endpoint on an operating system specified interface
		(\e INADDR_ANY ).
	*/
	explicit EndPointIP(const char *port,
		AddressFamily address_family = AddressFamily_Internet);
	/**
		Constructs a local endpoint on an operating system specified interface
		(\e INADDR_ANY ).
	*/
	explicit EndPointIP(const std::string &port,
		AddressFamily address_family = AddressFamily_Internet);

	EndPointIP(const EndPointIP &other);
	EndPointIP & operator = (const EndPointIP &other);

	void swap(EndPointIP &other);

	EndPointIP &SetAddressAndPort(const std::string &in_address,
		const std::string &in_port);
	EndPointIP &SetAddressAndPort(const char *in_address, int in_port);
	EndPointIP &SetAddressAndPort(const AddressIP &in_address, int in_port);
	EndPointIP &SetAddressAndPort(const AddressIP &in_address,
		const char *in_port);
	EndPointIP &SetAddressAndPort(const AddressIP &in_address,
		const std::string &in_port);
	EndPointIP &SetAddressAndPort(const NativeSockAddr &in_sockaddr);
	EndPointIP &SetAddressAndPort(const NativeSockAddr_In &in_sockaddr_in);

	/**
		Sets the endpoint to a local operating system specified interface
		(\e INADDR_ANY ).
	*/
	EndPointIP &SetAddressAndPortLocalAny(int in_port,
		AddressFamily address_family = AddressFamily_Internet);
	/**
		Sets the endpoint to a local operating system specified interface
		(\e INADDR_ANY ).
	*/
	EndPointIP &SetAddressAndPortLocalAny(const char *in_port,
		AddressFamily address_family = AddressFamily_Internet);
	/**
		Sets the endpoint to a local operating system specified interface
		(\e INADDR_ANY ).
	*/
	EndPointIP &SetAddressAndPortLocalAny(const std::string &in_port,
		AddressFamily address_family = AddressFamily_Internet);

	EndPointIP &SetAddress(long in_address);
	EndPointIP &SetAddress(unsigned long in_address);
	EndPointIP &SetAddress(const unsigned char *in_address);
	EndPointIP &SetAddress(const char *in_address);
	EndPointIP &SetAddress(const std::string &in_address);
	EndPointIP &SetAddress(const AddressIP &in_address);
	EndPointIP &SetAddress(const struct in_addr &in_addr_in);

	EndPointIP &SetPort(int in_port);
	EndPointIP &SetPort(const char *in_port);
	EndPointIP &SetPort(const std::string &in_port);

	AddressIP         GetAddress() const;
	std::string       GetHostIPAddress() const;
	std::string       GetHostIPAddressZeroFilled() const;
	std::string       GetHostId() const;
	std::string       GetHostName() const;
	int               GetPort() const;
	unsigned short    GetPortInNetworkOrder() const;
	struct in_addr    GetInAddr() const;
	NativeSockAddr_In GetSockaddrIn() const;
	NativeSockAddr_In GetSockaddrIn(AddressFamily address_family) const;
	NativeSockAddr    GetSockaddr() const;
	NativeSockAddr    GetSockaddr(AddressFamily address_family) const;

	std::string ToString() const;

	bool operator <  (const EndPointIP &other) const;
	bool operator >  (const EndPointIP &other) const;
	bool operator <= (const EndPointIP &other) const;
	bool operator >= (const EndPointIP &other) const;
	bool operator == (const EndPointIP &other) const;
	bool operator != (const EndPointIP &other) const;

	       int Compare(const EndPointIP &other) const;
	static int Compare(const EndPointIP *ptr_1, const EndPointIP *ptr_2);

	AddressIP address_;
	int       port_;
};
API_SOCKLIBX std::ostream & operator << (std::ostream &o_str,
	const EndPointIP &datum);
API_SOCKLIBX void swap(EndPointIP &arg_1, EndPointIP &arg_2);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
API_SOCKLIBX NativeSocketHandle Accept(NativeSocketHandle in_socket_handle,
	NativeSockAddr *addr = NULL, NativeSockLen_T *addr_length = NULL);
API_SOCKLIBX NativeSocketHandle Accept(NativeSocketHandle in_socket_handle,
	NativeSockAddr &addr, NativeSockLen_T &addr_length);
API_SOCKLIBX NativeSocketHandle Accept(NativeSocketHandle in_socket_handle,
	NativeSockAddr_In &addr);
API_SOCKLIBX NativeSocketHandle Accept(NativeSocketHandle in_socket_handle,
	EndPointIP &end_point);
API_SOCKLIBX NativeSocketHandle Accept(NativeSocketHandle in_socket_handle);
	//
API_SOCKLIBX int Bind(NativeSocketHandle socket_handle,
	const NativeSockAddr *addr, NativeSockLen_T addrlen);
API_SOCKLIBX int Bind(NativeSocketHandle socket_handle,
	const NativeSockAddr &addr);
API_SOCKLIBX int Bind(NativeSocketHandle socket_handle,
	const NativeSockAddr_In &addr);
API_SOCKLIBX int Bind(NativeSocketHandle socket_handle,
	const EndPointIP &end_point);
	//
API_SOCKLIBX int CloseSocket(NativeSocketHandle socket_handle);
	//
API_SOCKLIBX int Connect(NativeSocketHandle socket_handle,
	const NativeSockAddr *addr, unsigned int addr_length);
API_SOCKLIBX int Connect(NativeSocketHandle socket_handle,
	const NativeSockAddr &addr);
API_SOCKLIBX int Connect(NativeSocketHandle socket_handle,
	const NativeSockAddr_In &addr);
API_SOCKLIBX int Connect(NativeSocketHandle socket_handle,
	const EndPointIP &in_end_point);
	//
API_SOCKLIBX int        GetPeerName(NativeSocketHandle socket_handle,
	NativeSockAddr *addr, NativeSockLen_T *addr_length);
API_SOCKLIBX int        GetPeerName(NativeSocketHandle socket_handle,
	NativeSockAddr &addr, NativeSockLen_T &addr_length);
API_SOCKLIBX int        GetPeerName(NativeSocketHandle socket_handle,
	NativeSockAddr_In &addr);
API_SOCKLIBX EndPointIP GetPeerName(NativeSocketHandle socket_handle);
	//
API_SOCKLIBX int        GetSockName(NativeSocketHandle socket_handle,
	NativeSockAddr *addr, NativeSockLen_T *addr_length);
API_SOCKLIBX int        GetSockName(NativeSocketHandle socket_handle,
	NativeSockAddr &addr, NativeSockLen_T &addr_length);
API_SOCKLIBX int        GetSockName(NativeSocketHandle socket_handle,
	NativeSockAddr_In &addr);
API_SOCKLIBX EndPointIP GetSockName(NativeSocketHandle socket_handle);
	//
API_SOCKLIBX int GetSockOpt(NativeSocketHandle socket_handle, int opt_level,
	int opt_name, void *opt_value, int *opt_length);
API_SOCKLIBX int GetSockOpt(NativeSocketHandle socket_handle, int opt_level,
	int opt_name, bool &opt_value);
API_SOCKLIBX int GetSockOpt(NativeSocketHandle socket_handle, int opt_level,
	int opt_name, int &opt_value);
//
API_SOCKLIBX int Listen(NativeSocketHandle socket_handle,
	unsigned int backlog_count);
	//
API_SOCKLIBX unsigned int Recv(NativeSocketHandle socket_handle, char *buffer,
	unsigned int buffer_length, int flags);
API_SOCKLIBX unsigned int Recv(NativeSocketHandle socket_handle, void *buffer,
	unsigned int buffer_length, int flags);
API_SOCKLIBX unsigned int Recv(NativeSocketHandle socket_handle,
	std::string &buffer, unsigned int buffer_length, int flags);
	//
	//	*****
API_SOCKLIBX unsigned int RecvAppend(NativeSocketHandle socket_handle,
	std::string &buffer, int flags);
	//
API_SOCKLIBX unsigned int RecvFrom(NativeSocketHandle socket_handle,
	char *buffer_ptr, unsigned int buffer_length, int flags = 0,
	NativeSockAddr *addr = NULL, NativeSockLen_T *addrlen = NULL);
API_SOCKLIBX unsigned int RecvFrom(NativeSocketHandle socket_handle,
	void *buffer_ptr, unsigned int buffer_length, int flags = 0,
	NativeSockAddr *addr = NULL, NativeSockLen_T *addrlen = NULL);
API_SOCKLIBX unsigned int RecvFrom(NativeSocketHandle socket_handle,
	void *buffer_ptr, unsigned int buffer_length, int flags,
	EndPointIP &end_point);
API_SOCKLIBX unsigned int RecvFrom(NativeSocketHandle socket_handle,
	std::string &buffer, unsigned int buffer_length, int flags = 0,
	NativeSockAddr *addr = NULL, NativeSockLen_T *addrlen = NULL);
API_SOCKLIBX unsigned int RecvFrom(NativeSocketHandle socket_handle,
	std::string &buffer, unsigned int buffer_length, int flags,
	NativeSockAddr &addr, NativeSockLen_T &addr_length);
API_SOCKLIBX unsigned int RecvFrom(NativeSocketHandle socket_handle,
	std::string &buffer, unsigned int buffer_length, int flags,
	NativeSockAddr_In &addr);
API_SOCKLIBX unsigned int RecvFrom(NativeSocketHandle socket_handle,
	std::string &buffer, unsigned int buffer_length, int flags,
	EndPointIP &end_point);
	//
API_SOCKLIBX unsigned int Select(NativeSocketHandle socket_handle,
	NativeFDSet_T *read_fd_set, NativeFDSet_T *write_fd_set,
	NativeFDSet_T *error_fd_set, const struct timeval *time_out_ptr = NULL);
API_SOCKLIBX unsigned int Select(NativeSocketHandle socket_handle,
	NativeFDSet_T *read_fd_set, NativeFDSet_T *write_fd_set,
	NativeFDSet_T *error_fd_set, const struct timeval &time_out_ref);
API_SOCKLIBX unsigned int Select(NativeSocketHandle socket_handle,
	NativeFDSet_T *read_fd_set, NativeFDSet_T *write_fd_set,
	NativeFDSet_T *error_fd_set, unsigned int wait_usecs);
	//
API_SOCKLIBX unsigned int Send(NativeSocketHandle socket_handle,
	const char *buffer_ptr, unsigned int buffer_length, int flags);
API_SOCKLIBX unsigned int Send(NativeSocketHandle socket_handle,
	const void *buffer_ptr, unsigned int buffer_length, int flags);
API_SOCKLIBX unsigned int Send(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags);
API_SOCKLIBX unsigned int Send(NativeSocketHandle socket_handle,
	const std::ostringstream &buffer, int flags);
	//
API_SOCKLIBX unsigned int SendTo(NativeSocketHandle socket_handle,
	const char *buffer_ptr, unsigned int buffer_length, int flags,
	const NativeSockAddr *addr, NativeSockLen_T addrlen);
API_SOCKLIBX unsigned int SendTo(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags, const NativeSockAddr *addr,
	NativeSockLen_T addrlen);
API_SOCKLIBX unsigned int SendTo(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags, const NativeSockAddr &addr);
API_SOCKLIBX unsigned int SendTo(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags, const NativeSockAddr_In &addr);
API_SOCKLIBX unsigned int SendTo(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags, const EndPointIP &end_point);
	//
API_SOCKLIBX int SetSockOpt(NativeSocketHandle socket_handle, int opt_level,
	int opt_name, const void *opt_value, int opt_length);
API_SOCKLIBX int SetSockOpt(NativeSocketHandle socket_handle, int opt_level,
	int opt_name, bool opt_value);
API_SOCKLIBX int SetSockOpt(NativeSocketHandle socket_handle, int opt_level,
	int opt_name, int opt_value);
	//
API_SOCKLIBX int ShutDown(NativeSocketHandle socket_handle,
	ShutDownHow shut_down_how);
	//
API_SOCKLIBX NativeSocketHandle Socket(
	AddressFamily address_family = AddressFamily_Internet,
	SocketType socket_type = SocketType_Stream,
	ProtocolType protocol_type = ProtocolType_DEFAULT);

	//	Miscellaneous wrapper functions for the file descriptor list...
API_SOCKLIBX NativeFDSet_T &GetFDSet(NativeSocketHandle socket_handle,
	NativeFDSet_T &in_fd_set);
API_SOCKLIBX NativeFDSet_T  GetFDSet(NativeSocketHandle socket_handle);
API_SOCKLIBX NativeFDSet_T &AppendToFDSet(NativeSocketHandle socket_handle,
	NativeFDSet_T &in_fd_set);

	//	Miscellaneous wrapper functions for getsockopt()...
API_SOCKLIBX unsigned int GetRecvBufferSize(NativeSocketHandle socket_handle);
API_SOCKLIBX unsigned int GetSendBufferSize(NativeSocketHandle socket_handle);

	//	Miscellaneous wrapper functions for select()...
API_SOCKLIBX unsigned int SelectRead(NativeSocketHandle socket_handle,
	const struct timeval *time_out_ptr = NULL);
API_SOCKLIBX unsigned int SelectRead(NativeSocketHandle socket_handle,
	const struct timeval &time_out_ref);
API_SOCKLIBX unsigned int SelectRead(NativeSocketHandle socket_handle,
	unsigned int wait_usecs);

API_SOCKLIBX unsigned int SelectWrite(NativeSocketHandle socket_handle,
	const struct timeval *time_out_ptr = NULL);
API_SOCKLIBX unsigned int SelectWrite(NativeSocketHandle socket_handle,
	const struct timeval &time_out_ref);
API_SOCKLIBX unsigned int SelectWrite(NativeSocketHandle socket_handle,
	unsigned int wait_usecs);

API_SOCKLIBX unsigned int SelectError(NativeSocketHandle socket_handle,
	const struct timeval *time_out_ptr = NULL);
API_SOCKLIBX unsigned int SelectError(NativeSocketHandle socket_handle,
	const struct timeval &time_out_ref);
API_SOCKLIBX unsigned int SelectError(NativeSocketHandle socket_handle,
	unsigned int wait_usecs);

	//	Miscellaneous wrapper functions for setsockopt()...
API_SOCKLIBX void SetRecvBufferSize(NativeSocketHandle socket_handle,
	unsigned int buffer_size);
API_SOCKLIBX void SetSendBufferSize(NativeSocketHandle socket_handle,
	unsigned int buffer_size);
API_SOCKLIBX void SetBlockingMode(NativeSocketHandle socket_handle,
	bool blocking_mode_flag);
API_SOCKLIBX void SetBlockingModeOn(NativeSocketHandle socket_handle);
API_SOCKLIBX void SetBlockingModeOff(NativeSocketHandle socket_handle);

	//	Miscellaneous supporting functions...
API_SOCKLIBX SocketErrorCode  GetLastSocketErrorCode();

API_SOCKLIBX std::string      GetSocketErrorString(SocketErrorCode error_code);
API_SOCKLIBX std::string      GetSocketErrorString();

API_SOCKLIBX char            *ResolveHostToName(const char *in_host_name,
	char *out_host_name);
API_SOCKLIBX std::string     &ResolveHostToName(const std::string &in_host_name,
	std::string &out_host_name);
API_SOCKLIBX std::string      ResolveHostToName(const std::string &in_host_name);

API_SOCKLIBX int              ResolveServToPort(const char *in_service,
	const char *protocol = NULL);
API_SOCKLIBX int              ResolveServToPort(const std::string &in_service,
	const std::string &protocol = "");

API_SOCKLIBX unsigned long    INetToAddr(const char *ip_address);
API_SOCKLIBX unsigned long    INetToAddr(const std::string &ip_address);

API_SOCKLIBX bool             IsMulticastIP(const unsigned char *ip_address);
API_SOCKLIBX bool             IsMulticastIP(unsigned long ip_address);
API_SOCKLIBX bool             IsMulticastIP(const char *ip_address);
API_SOCKLIBX bool             IsMulticastIP(const std::string &ip_address);

API_SOCKLIBX const unsigned char *CheckIsMulticastIP(
	const unsigned char *ip_address);
API_SOCKLIBX unsigned long        CheckIsMulticastIP(unsigned long ip_address);
API_SOCKLIBX const char          *CheckIsMulticastIP(const char *ip_address);
API_SOCKLIBX const std::string   &CheckIsMulticastIP(
	const std::string &ip_address);

API_SOCKLIBX NetInt_1         HostToNetworkByteOrder(NetInt_1 datum);
API_SOCKLIBX NetInt_2         HostToNetworkByteOrder(NetInt_2 datum);
API_SOCKLIBX NetInt_4         HostToNetworkByteOrder(NetInt_4 datum);

API_SOCKLIBX NetInt_1         NetworkToHostByteOrder(NetInt_1 datum);
API_SOCKLIBX NetInt_2         NetworkToHostByteOrder(NetInt_2 datum);
API_SOCKLIBX NetInt_4         NetworkToHostByteOrder(NetInt_4 datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_SOCKLIBX) SockLibXException :
	public MLB::Utility::ExceptionGeneral {
public:
	SockLibXException();
	SockLibXException(const char *except_string);
	SockLibXException(const std::string &except_string);
	SockLibXException(const std::ostringstream &except_string);

	~SockLibXException() MBCOMPAT_EXCEPT_NOTHROW;

	virtual void Rethrow(const char *except_string = NULL) const;

	virtual bool IsInitialConnectFailure() const;
	virtual bool IsPeerConnectShutdown() const;
	virtual bool IsWouldBlock() const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_SOCKLIBX) SockLibXExceptionError :
	public MLB::Utility::ExceptionSystemError {
public:
	//	Default constructor.
	SockLibXExceptionError();
	//	String-only constructors...
	SockLibXExceptionError(const char *except_string);
	SockLibXExceptionError(const std::string &except_string);
	SockLibXExceptionError(const std::ostringstream &except_string);
	//	SystemErrorCode - based constructors...
	SockLibXExceptionError(SocketErrorCode error_code);
	SockLibXExceptionError(SocketErrorCode error_code,
		const char *except_string);
	SockLibXExceptionError(SocketErrorCode error_code,
		const std::string &except_string);
	SockLibXExceptionError(SocketErrorCode error_code,
		const std::ostringstream &except_string);

	//	Forwards to MLB::Utility::ExceptionSystemError without additional text.
	//	The 'bool' parameter is just for constructor disambiguation.
	SockLibXExceptionError(bool, SocketErrorCode error_code,
		const char *except_string);
	SockLibXExceptionError(bool, SocketErrorCode error_code,
		const std::string &except_string);

	//	Forwards to MLB::Utility::ExceptionSystemError without additional text.
	//	The 'bool' parameter is just for constructor disambiguation.
	SockLibXExceptionError(bool, const char *except_string);
	SockLibXExceptionError(bool, const std::string &except_string);

	~SockLibXExceptionError() MBCOMPAT_EXCEPT_NOTHROW;

	virtual void Rethrow(const char *except_string = NULL) const;
	virtual bool IsInitialConnectFailure() const;
	virtual bool IsPeerConnectShutdown() const;
	virtual bool IsWouldBlock() const;

private:
	static std::string GetStatusString(const char *other_text = NULL);
	static std::string GetStatusString(SocketErrorCode error_code,
		const char *other_text = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_SOCKLIBX) SockLibXTimeOut :
	public SockLibXException {
public:
	//	Default constructor.
	SockLibXTimeOut();
	//	String-only constructors...
	SockLibXTimeOut(const char *except_string);
	SockLibXTimeOut(const std::string &except_string);
	SockLibXTimeOut(const std::ostringstream &except_string);
	// Other constructors...
	SockLibXTimeOut(const MLB::Utility::TimeVal &time_out);
	SockLibXTimeOut(const MLB::Utility::TimeVal &time_out,
		const char *except_string);
	SockLibXTimeOut(const MLB::Utility::TimeVal &time_out,
		const std::string &except_string);
	SockLibXTimeOut(const MLB::Utility::TimeVal &time_out,
		const std::ostringstream &except_string);
	~SockLibXTimeOut() MBCOMPAT_EXCEPT_NOTHROW;

	MLB::Utility::TimeVal GetTimeOut() const;

	virtual void Rethrow(const char *except_string = NULL) const;

	static std::string GetTimeOutString(const MLB::Utility::TimeVal &time_out,
		const char *other_text = NULL);

private:
	MLB::Utility::TimeVal time_out_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_SOCKLIBX SockLibXContext {
public:
	SockLibXContext();
	SockLibXContext(const SockLibXContext &);
	//	In case one wants to inherit to ensure instance lifetime scope...
	virtual ~SockLibXContext();

	bool IsReady() const;
	void EnsureReady() const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_SOCKLIBX) SockLibXConnectFailure :
	public SockLibXExceptionError {
public:
	//	Default constructor.
	SockLibXConnectFailure();
	SockLibXConnectFailure(const EndPointIP &endpoint,
		const char *except_string);
	SockLibXConnectFailure(const EndPointIP &endpoint,
		const std::string &except_string, SocketErrorCode error_code);
	~SockLibXConnectFailure() MBCOMPAT_EXCEPT_NOTHROW;

	EndPointIP GetEndPoint() const;

	virtual void Rethrow(const char *except_string = NULL) const;

	EndPointIP endpoint_;

protected:
	static std::string GetStatusString(const EndPointIP *endpoint_ptr = NULL,
		const char *other_text = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_SOCKLIBX) SockLibXConnectTimeOut :
	public SockLibXConnectFailure {
public:
	SockLibXConnectTimeOut(const EndPointIP &endpoint, const char *except_string,
		SocketErrorCode error_code);
	~SockLibXConnectTimeOut() MBCOMPAT_EXCEPT_NOTHROW;

	virtual void Rethrow(const char *except_string = NULL) const;

private:
	static std::string GetStatusString(const EndPointIP *endpoint_ptr = NULL,
		const char *other_text = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_SOCKLIBX) SockLibXConnectRefused :
	public SockLibXConnectFailure {
public:
	SockLibXConnectRefused(const EndPointIP &endpoint, const char *except_string,
		SocketErrorCode error_code);
	~SockLibXConnectRefused() MBCOMPAT_EXCEPT_NOTHROW;
	virtual void Rethrow(const char *except_string = NULL) const;

private:
	static std::string GetStatusString(const EndPointIP *endpoint_ptr = NULL,
		const char *other_text = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/**
	This exception is for use when a 'recv()' operation returns 0 bytes and
	you want to indicate the situation with an exception...
*/
class MB_LIB_EXCEPTION_CLASS(API_SOCKLIBX) SockLibXPeerClosed :
	public SockLibXExceptionError {
public:
	SockLibXPeerClosed(const char *except_string = NULL);
	SockLibXPeerClosed(const std::string &except_string);

	~SockLibXPeerClosed() MBCOMPAT_EXCEPT_NOTHROW;

	virtual void Rethrow(const char *except_string = NULL) const;
	virtual bool IsPeerConnectShutdown() const;

private:
	static std::string GetPeerClosedString(const char *other_text = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#endif // #ifndef HH__MLB__SockLibX__SockLibX_hpp__HH

