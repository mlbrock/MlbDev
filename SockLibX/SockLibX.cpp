//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation for the MLB::SockLibX free functions.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SockLibX.hpp>

#include <Utility/ValueToStringRadix.hpp>

#include <memory.h>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_array.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

#define SOCKLIBZ_DEFAULT_RECV_BUFFER_SIZE	8192

namespace {
//	////////////////////////////////////////////////////////////////////////////
template <typename OptDataType>
	int GetSockOptTyped(NativeSocketHandle socket_handle, int opt_level,
		int opt_name, OptDataType &opt_value)
{
	int opt_length  = sizeof(opt_value);
	int return_code =
		GetSockOpt(socket_handle, opt_level, opt_name, &opt_value, &opt_length);

	if (opt_length != sizeof(opt_value))
		throw SockLibXException("Invocation of 'GetSockOpt()' for option "
			"level " + MLB::Utility::AnyToString(opt_level) + ", option name " +
			MLB::Utility::AnyToString(opt_name) + " for the option value located "
			"at address " + MLB::Utility::ValueToStringHex(&opt_value) + " with "
			"a size of " + MLB::Utility::AnyToString(sizeof(opt_value)) +
			" bytes resulted in the socket library indicating a size of " +
			MLB::Utility::AnyToString(opt_length) + " bytes.");

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
NativeSocketHandle Accept(NativeSocketHandle in_socket_handle,
	NativeSockAddr *addr, NativeSockLen_T *addr_length)
{
	NativeSocketHandle out_socket_handle = NPSL_INVALID_SOCKET;
	char               npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_Accept(in_socket_handle, &out_socket_handle, addr,
		addr_length, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(out_socket_handle);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeSocketHandle Accept(NativeSocketHandle in_socket_handle,
	NativeSockAddr &addr, NativeSockLen_T &addr_length)
{
	return(Accept(in_socket_handle, &addr, &addr_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeSocketHandle Accept(NativeSocketHandle in_socket_handle,
	NativeSockAddr_In &addr)
{
	NativeSockLen_T addr_length = sizeof(addr);

	return(Accept(in_socket_handle, reinterpret_cast<NativeSockAddr *>(&addr),
		&addr_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeSocketHandle Accept(NativeSocketHandle in_socket_handle,
	EndPointIP &end_point)
{
	NativeSockAddr  addr;
	NativeSockLen_T addr_length = sizeof(addr);

	NativeSocketHandle out_socket_handle = Accept(in_socket_handle,
		addr, addr_length);

	end_point = EndPointIP(addr);

	return(out_socket_handle);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeSocketHandle Accept(NativeSocketHandle in_socket_handle)
{
	return(Accept(in_socket_handle, NULL, NULL));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Bind(NativeSocketHandle socket_handle, const NativeSockAddr *addr,
	NativeSockLen_T addrlen)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_Bind(socket_handle, addr, addrlen, npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Bind(NativeSocketHandle socket_handle, const NativeSockAddr &addr)
{
	return(Bind(socket_handle, &addr, sizeof(addr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Bind(NativeSocketHandle socket_handle, const NativeSockAddr_In &addr)
{
	return(Bind(socket_handle, reinterpret_cast<const NativeSockAddr *>(&addr),
		sizeof(addr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Bind(NativeSocketHandle socket_handle, const EndPointIP &end_point)
{
	NativeSockAddr_In addr = end_point.GetSockaddrIn();

	return(Bind(socket_handle, addr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int CloseSocket(NativeSocketHandle socket_handle)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_CloseSocket(socket_handle, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Connect(NativeSocketHandle socket_handle, const NativeSockAddr *addr,
	unsigned int addr_length)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_Connect(socket_handle, addr,
		static_cast<NPSL_SOCKLEN_T>(addr_length), npsl_error_text) !=
		NPSL_SUCCESS) {
		MLB::Utility::SystemErrorCode error_code = GetLastSocketErrorCode();
		EndPointIP end_point(*addr);
		if (error_code == SocketError_TimedOut)
			throw SockLibXConnectTimeOut(end_point, npsl_error_text, error_code);
		else if (error_code == SocketError_ConnRefused)
			throw SockLibXConnectRefused(end_point, npsl_error_text, error_code);
		else
			throw SockLibXConnectFailure(end_point, npsl_error_text);
	}

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Connect(NativeSocketHandle socket_handle, const NativeSockAddr &addr)
{
	return(Connect(socket_handle, &addr, sizeof(addr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Connect(NativeSocketHandle socket_handle, const NativeSockAddr_In &addr)
{
	return(Connect(socket_handle,
		reinterpret_cast<const NativeSockAddr *>(&addr), sizeof(addr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Connect(NativeSocketHandle socket_handle, const EndPointIP &end_point)
{
	NativeSockAddr_In end_point_addr = end_point.GetSockaddrIn();

	return(Connect(socket_handle, end_point_addr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetPeerName(NativeSocketHandle socket_handle, NativeSockAddr *addr,
	NativeSockLen_T *addr_length)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_GetPeerName(socket_handle, addr, addr_length, npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetPeerName(NativeSocketHandle socket_handle, NativeSockAddr &addr,
	NativeSockLen_T &addr_length)
{
	return(GetPeerName(socket_handle, &addr, &addr_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetPeerName(NativeSocketHandle socket_handle, NativeSockAddr_In &addr)
{
	NativeSockLen_T addr_length = sizeof(addr);

	return(GetPeerName(socket_handle, reinterpret_cast<NativeSockAddr *>(&addr),
		&addr_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
EndPointIP GetPeerName(NativeSocketHandle socket_handle)
{
	NativeSockAddr_In addr;

	GetPeerName(socket_handle, addr);

	return(EndPointIP(addr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetSockName(NativeSocketHandle socket_handle, NativeSockAddr *addr,
	NativeSockLen_T *addr_length)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_GetSockName(socket_handle, addr, addr_length, npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetSockName(NativeSocketHandle socket_handle, NativeSockAddr &addr,
	NativeSockLen_T &addr_length)
{
	return(GetSockName(socket_handle, &addr, &addr_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetSockName(NativeSocketHandle socket_handle, NativeSockAddr_In &addr)
{
	NativeSockLen_T addr_length = sizeof(addr);

	return(GetSockName(socket_handle, reinterpret_cast<NativeSockAddr *>(&addr),
		&addr_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
EndPointIP GetSockName(NativeSocketHandle socket_handle)
{
	NativeSockAddr_In addr;

	GetSockName(socket_handle, addr);

	return(EndPointIP(addr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetSockOpt(NativeSocketHandle socket_handle, int opt_level, int opt_name,
	void *opt_value, int *opt_length)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_GetSockOpt(socket_handle, opt_level, opt_name, opt_value,
		opt_length, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetSockOpt(NativeSocketHandle socket_handle, int opt_level, int opt_name,
	bool &opt_value)
{
	return(GetSockOptTyped(socket_handle, opt_level, opt_name, opt_value));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int GetSockOpt(NativeSocketHandle socket_handle, int opt_level, int opt_name,
	int &opt_value)
{
	return(GetSockOptTyped(socket_handle, opt_level, opt_name, opt_value));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int Listen(NativeSocketHandle socket_handle, unsigned int backlog_count)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_Listen(socket_handle, static_cast<int>(backlog_count),
		npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int Recv(NativeSocketHandle socket_handle, char *buffer,
	unsigned int buffer_length, int flags)
{
	unsigned int recv_count;
	char         npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_Recv(socket_handle, buffer, buffer_length, flags,
		&recv_count, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(recv_count);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	The version which takes a 'char *' argument matches most closely the BSD
//	library function, so this 'void *' version invokes it instead of the other
//	way around...
unsigned int Recv(NativeSocketHandle socket_handle, void *buffer,
	unsigned int buffer_length, int flags)
{
	return(Recv(socket_handle, static_cast<char *>(buffer), buffer_length,
		flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int Recv(NativeSocketHandle socket_handle, std::string &buffer,
	unsigned int buffer_length, int flags)
{
	unsigned int recv_length;

	if (buffer_length <= SOCKLIBZ_DEFAULT_RECV_BUFFER_SIZE) {
		char          tmp_buffer[SOCKLIBZ_DEFAULT_RECV_BUFFER_SIZE];
		recv_length = Recv(socket_handle, tmp_buffer, buffer_length, flags);
		buffer.assign(tmp_buffer, recv_length);
	}
	else {
		boost::shared_array<char> tmp_buffer(new char[buffer_length]);
		recv_length = Recv(socket_handle, tmp_buffer.get(), buffer_length, flags);
		buffer.assign(tmp_buffer.get(), recv_length);
	}

	return(recv_length);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int RecvAppend(NativeSocketHandle socket_handle, std::string &buffer,
	int flags)
{
	unsigned int recv_count;
	char         tmp_buffer[SOCKLIBZ_DEFAULT_RECV_BUFFER_SIZE];

	if ((recv_count = Recv(socket_handle, tmp_buffer, sizeof(tmp_buffer),
		flags)) > 0)
		buffer.append(tmp_buffer, 0, recv_count);

	return(recv_count);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int RecvFrom(NativeSocketHandle socket_handle, char *buffer_ptr,
	unsigned int buffer_length, int flags, NativeSockAddr *addr,
	NativeSockLen_T *addrlen)
{
	unsigned int recv_count;
	char         npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_RecvFrom(socket_handle, buffer_ptr, buffer_length, flags,
		addr, addrlen, &recv_count, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(recv_count);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	The version which takes a 'char *' argument matches most closely the BSD
//	library function, so this 'void *' version invokes it instead of the other
//	way around...
unsigned int RecvFrom(NativeSocketHandle socket_handle, void *buffer_ptr,
	unsigned int buffer_length, int flags, NativeSockAddr *addr,
	NativeSockLen_T *addrlen)
{
	return(RecvFrom(socket_handle, static_cast<char *>(buffer_ptr),
		buffer_length, flags, addr, addrlen));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int RecvFrom(NativeSocketHandle socket_handle, void *buffer_ptr,
	unsigned int buffer_length, int flags, EndPointIP &end_point)
{
	NativeSockAddr  addr;
	NativeSockLen_T addr_length = sizeof(addr);

	unsigned int recv_length = RecvFrom(socket_handle,
		static_cast<char *>(buffer_ptr), buffer_length, flags, &addr,
		&addr_length);

	end_point = EndPointIP(addr);

	return(recv_length);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int RecvFrom(NativeSocketHandle socket_handle, std::string &buffer,
	unsigned int buffer_length, int flags, NativeSockAddr *addr,
	NativeSockLen_T *addrlen)
{
	unsigned int recv_length;

	if (buffer_length <= SOCKLIBZ_DEFAULT_RECV_BUFFER_SIZE) {
		char          tmp_buffer[SOCKLIBZ_DEFAULT_RECV_BUFFER_SIZE];
		recv_length = RecvFrom(socket_handle, tmp_buffer, buffer_length, flags,
			addr, addrlen);
		buffer.assign(tmp_buffer, recv_length);
	}
	else {
		boost::shared_array<char> tmp_buffer(new char[buffer_length]);
		recv_length = RecvFrom(socket_handle, tmp_buffer.get(), buffer_length,
			flags, addr, addrlen);
		buffer.assign(tmp_buffer.get(), recv_length);
	}

	return(recv_length);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int RecvFrom(NativeSocketHandle socket_handle, std::string &buffer,
	unsigned int buffer_length, int flags, NativeSockAddr &addr,
	NativeSockLen_T &addr_length)
{
	return(RecvFrom(socket_handle, buffer, buffer_length, flags,
		&addr, &addr_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int RecvFrom(NativeSocketHandle socket_handle, std::string &buffer,
	unsigned int buffer_length, int flags, NativeSockAddr_In &addr)
{
	NativeSockLen_T addr_length = sizeof(addr);

	return(RecvFrom(socket_handle, buffer, buffer_length, flags,
		reinterpret_cast<NativeSockAddr *>(&addr), &addr_length));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int RecvFrom(NativeSocketHandle socket_handle, std::string &buffer,
	unsigned int buffer_length, int flags, EndPointIP &end_point)
{
	NativeSockAddr  addr;
	NativeSockLen_T addr_length = sizeof(addr);

	unsigned int recv_length = RecvFrom(socket_handle, buffer,
		buffer_length, flags, &addr, &addr_length);

	end_point = EndPointIP(addr);

	return(recv_length);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int Select(NativeSocketHandle socket_handle,
	NativeFDSet_T *read_fd_set, NativeFDSet_T *write_fd_set,
	NativeFDSet_T *error_fd_set, const struct timeval *time_out_ptr)
{
	unsigned int ready_fd_count;
	char         npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_Select(static_cast<int>(socket_handle) + 1, read_fd_set,
		write_fd_set, error_fd_set, time_out_ptr, &ready_fd_count,
		npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(ready_fd_count);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int Select(NativeSocketHandle socket_handle,
	NativeFDSet_T *read_fd_set, NativeFDSet_T *write_fd_set,
	NativeFDSet_T *error_fd_set, const struct timeval &time_out_ref)
{
	return(Select(socket_handle, read_fd_set, write_fd_set, error_fd_set,
		&time_out_ref));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int Select(NativeSocketHandle socket_handle,
	NativeFDSet_T *read_fd_set, NativeFDSet_T *write_fd_set,
	NativeFDSet_T *error_fd_set, unsigned int wait_usecs)
{
	return(Select(socket_handle, read_fd_set, write_fd_set, error_fd_set,
		MLB::Utility::TimeVal(wait_usecs / 1000000, wait_usecs % 1000000)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int Send(NativeSocketHandle socket_handle, const char *buffer_ptr,
	unsigned int buffer_length, int flags)
{
	unsigned int send_count;
	char         npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_Send(socket_handle, buffer_ptr, buffer_length, flags,
		&send_count, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(send_count);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	The version which takes a 'const char *' argument matches most closely the
//	BSD library function, so this 'const void *' version invokes it instead of
//	the other way around...
unsigned int Send(NativeSocketHandle socket_handle, const void *buffer_ptr,
	unsigned int buffer_length, int flags)
{
	return(Send(socket_handle, static_cast<const char *>(buffer_ptr),
		buffer_length, flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int Send(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags)
{
	return(Send(socket_handle, buffer.c_str(),
		static_cast<unsigned int>(buffer.size()), flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int Send(NativeSocketHandle socket_handle,
	const std::ostringstream &buffer, int flags)
{
	return(Send(socket_handle, buffer.str().c_str(),
		static_cast<unsigned int>(buffer.str().size()), flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SendTo(NativeSocketHandle socket_handle,
	const char *buffer_ptr, unsigned int buffer_length,
	int flags, const NativeSockAddr *addr, NativeSockLen_T addrlen)
{
	unsigned int send_count;
	char         npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_SendTo(socket_handle, buffer_ptr, buffer_length, flags,
		addr, addrlen, &send_count, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(send_count);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SendTo(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags, const NativeSockAddr *addr,
	NativeSockLen_T addrlen)
{
	return(SendTo(socket_handle, buffer.c_str(),
		static_cast<unsigned int>(buffer.size()), flags, addr, addrlen));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SendTo(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags, const NativeSockAddr &addr)
{
	return(SendTo(socket_handle, buffer.c_str(),
		static_cast<unsigned int>(buffer.size()), flags, &addr, sizeof(addr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SendTo(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags, const NativeSockAddr_In &addr)
{
	return(SendTo(socket_handle, buffer.c_str(),
		static_cast<unsigned int>(buffer.size()), flags,
		reinterpret_cast<const NativeSockAddr *>(&addr), sizeof(addr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SendTo(NativeSocketHandle socket_handle,
	const std::string &buffer, int flags, const EndPointIP &end_point)
{
	NativeSockAddr_In addr = end_point.GetSockaddrIn();

	return(SendTo(socket_handle, buffer, flags, addr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int SetSockOpt(NativeSocketHandle socket_handle, int opt_level,
	int opt_name, const void *opt_value, int opt_length)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_SetSockOpt(socket_handle, opt_level, opt_name, opt_value,
		opt_length, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int SetSockOpt(NativeSocketHandle socket_handle, int opt_level, int opt_name,
	bool opt_value)
{
	return(SetSockOpt(socket_handle, opt_level, opt_name, &opt_value,
		sizeof(opt_value)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int SetSockOpt(NativeSocketHandle socket_handle, int opt_level, int opt_name,
	int opt_value)
{
	return(SetSockOpt(socket_handle, opt_level, opt_name, &opt_value,
		sizeof(opt_value)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ShutDown(NativeSocketHandle socket_handle, ShutDownHow shut_down_how)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_ShutDown(socket_handle, shut_down_how, npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeSocketHandle Socket(AddressFamily address_family,
	SocketType socket_type, ProtocolType protocol_type)
{
	NativeSocketHandle socket_handle = NPSL_INVALID_SOCKET;
	char               npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_Socket(address_family, socket_type, protocol_type,
		&socket_handle, npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(socket_handle);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeFDSet_T &GetFDSet(NativeSocketHandle socket_handle,
	NativeFDSet_T &in_fd_set)
{
	FD_ZERO(&in_fd_set);

	return(AppendToFDSet(socket_handle, in_fd_set));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeFDSet_T GetFDSet(NativeSocketHandle socket_handle)
{
	NativeFDSet_T tmp_fd_set;

	return(GetFDSet(socket_handle, tmp_fd_set));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeFDSet_T &AppendToFDSet(NativeSocketHandle socket_handle,
	NativeFDSet_T &in_fd_set)
{
#ifdef _MSC_VER
# pragma warning(disable:4127)
#endif // # ifdef _MSC_VER

	FD_SET(socket_handle, &in_fd_set);

#ifdef _MSC_VER
# pragma warning(default:4127)
#endif // # ifdef _MSC_VER

	return(in_fd_set);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int GetRecvBufferSize(NativeSocketHandle socket_handle)
{
	unsigned int buffer_size;
	char         npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_GetSockOpt_RcvBuf(socket_handle, &buffer_size,npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(buffer_size);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int GetSendBufferSize(NativeSocketHandle socket_handle)
{
	unsigned int buffer_size;
	char         npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_GetSockOpt_SndBuf(socket_handle, &buffer_size, npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(buffer_size);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectRead(NativeSocketHandle socket_handle,
	const struct timeval *time_out_ptr)
{
	NativeFDSet_T tmp_fd_set = GetFDSet(socket_handle);

	return(Select(socket_handle, &tmp_fd_set, NULL, NULL, time_out_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectRead(NativeSocketHandle socket_handle,
	const struct timeval &time_out_ref)
{
	return(SelectRead(socket_handle, &time_out_ref));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectRead(NativeSocketHandle socket_handle,
	unsigned int wait_usecs)
{
	return(SelectRead(socket_handle,
		MLB::Utility::TimeVal(wait_usecs / 1000000, wait_usecs % 1000000)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectWrite(NativeSocketHandle socket_handle,
	const struct timeval *time_out_ptr)
{
	NativeFDSet_T tmp_fd_set = GetFDSet(socket_handle);

	return(Select(socket_handle, NULL, &tmp_fd_set, NULL, time_out_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectWrite(NativeSocketHandle socket_handle,
	const struct timeval &time_out_ref)
{
	return(SelectWrite(socket_handle, &time_out_ref));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectWrite(NativeSocketHandle socket_handle,
	unsigned int wait_usecs)
{
	return(SelectWrite(socket_handle,
		MLB::Utility::TimeVal(wait_usecs / 1000000, wait_usecs % 1000000)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectError(NativeSocketHandle socket_handle,
	const struct timeval *time_out_ptr)
{
	NativeFDSet_T tmp_fd_set = GetFDSet(socket_handle);

	return(Select(socket_handle, NULL, NULL, &tmp_fd_set, time_out_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectError(NativeSocketHandle socket_handle,
	const struct timeval &time_out_ref)
{
	return(SelectError(socket_handle, &time_out_ref));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SelectError(NativeSocketHandle socket_handle,
	unsigned int wait_usecs)
{
	return(SelectError(socket_handle,
		MLB::Utility::TimeVal(wait_usecs / 1000000, wait_usecs % 1000000)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SetRecvBufferSize(NativeSocketHandle socket_handle,
	unsigned int buffer_size)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_SetSockOpt_RcvBuf(socket_handle, buffer_size, npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SetSendBufferSize(NativeSocketHandle socket_handle,
	unsigned int buffer_size)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_SetSockOpt_SndBuf(socket_handle, buffer_size, npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SetBlockingMode(NativeSocketHandle socket_handle,
	bool blocking_mode_flag)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_SetBlockingMode(socket_handle, (blocking_mode_flag) ? 1 : 0,
		npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SetBlockingModeOn(NativeSocketHandle socket_handle)
{
	SetBlockingMode(socket_handle, true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SetBlockingModeOff(NativeSocketHandle socket_handle)
{
	SetBlockingMode(socket_handle, false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SocketErrorCode GetLastSocketErrorCode()
{
#ifdef _Windows
	return(WSAGetLastError());
#else
	return(errno);	
#endif // #ifdef _Windows
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GetSocketErrorString(SocketErrorCode error_code)
{
	char error_text[(NPSL_MAX_ERROR_TEXT * 2) + 1];

	return(NPSL_GetErrorString(error_code, false, sizeof(error_text) - 1,
		error_text));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GetSocketErrorString()
{
	return(GetSocketErrorString(GetLastSocketErrorCode()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *ResolveHostToName(const char *in_host_name, char *out_host_name)
{
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_ResolveHostToName(in_host_name, out_host_name,
		npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXException(npsl_error_text);

	return(out_host_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &ResolveHostToName(const std::string &in_host_name,
	std::string &out_host_name)
{
	char tmp_host_name[MLB::Utility::MaxHostNameLength + 1];

	return(out_host_name.assign(ResolveHostToName(in_host_name.c_str(),
		tmp_host_name)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string ResolveHostToName(const std::string &in_host_name)
{
	std::string out_host_name;

	return(ResolveHostToName(in_host_name, out_host_name));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ResolveServToPort(const char *in_service, const char *protocol)
{
	int  tmp_port;
	char npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_ResolveServToPort(in_service, protocol, &tmp_port,
		npsl_error_text) != NPSL_SUCCESS)
		throw SockLibXExceptionError(false, npsl_error_text);

	return(tmp_port);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ResolveServToPort(const std::string &in_service,
	const std::string &protocol)
{
	return(ResolveServToPort(in_service.c_str(),
		(protocol.empty()) ? NULL : protocol.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned long INetToAddr(const char *ip_address)
{
	unsigned long tmp_addr;
	char          npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (NPSL_INetToAddrBasic(ip_address, &tmp_addr, npsl_error_text) !=
		NPSL_SUCCESS)
		throw SockLibXException(npsl_error_text);

	return(tmp_addr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned long INetToAddr(const std::string &ip_address)
{
	return(INetToAddr(ip_address.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsMulticastIP(const unsigned char *ip_address)
{
	return((ip_address[0] >= 224) && (ip_address[0] <= 239));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsMulticastIP(unsigned long ip_address)
{
	unsigned char tmp_ip_address[4];

	::memcpy(tmp_ip_address, &ip_address, sizeof(tmp_ip_address));

	return(IsMulticastIP(tmp_ip_address));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsMulticastIP(const char *ip_address)
{
	return(IsMulticastIP(INetToAddr(ip_address)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsMulticastIP(const std::string &ip_address)
{
	return(IsMulticastIP(ip_address.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const unsigned char *CheckIsMulticastIP(const unsigned char *ip_address)
{
	if (!IsMulticastIP(ip_address))
		MLB::Utility::ThrowInvalidArgument("The IP address '" +
			AddressIP(ip_address).GetHostIPAddress() + "' is not a multicast address.");

	return(ip_address);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long CheckIsMulticastIP(unsigned long ip_address)
{
	unsigned char tmp_ip_address[4];

	::memcpy(tmp_ip_address, &ip_address, sizeof(tmp_ip_address));

	return(ip_address);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *CheckIsMulticastIP(const char *ip_address)
{
	CheckIsMulticastIP(INetToAddr(ip_address));

	return(ip_address);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const std::string &CheckIsMulticastIP(const std::string &ip_address)
{
	CheckIsMulticastIP(ip_address.c_str());

	return(ip_address);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NetInt_1 HostToNetworkByteOrder(NetInt_1 datum)
{
	return(datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NetInt_2 HostToNetworkByteOrder(NetInt_2 datum)
{
	return(htons(datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NetInt_4 HostToNetworkByteOrder(NetInt_4 datum)
{
	return(htonl(datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NetInt_1 NetworkToHostByteOrder(NetInt_1 datum)
{
	return(datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NetInt_2 NetworkToHostByteOrder(NetInt_2 datum)
{
	return(ntohs(datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NetInt_4 NetworkToHostByteOrder(NetInt_4 datum)
{
	return(ntohl(datum));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#ifdef TEST_MAIN

# include <iostream>

using namespace MLB::SockLibX;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	unsigned int  return_code = EXIT_SUCCESS;
	const char   *host;
	const char   *port;

	host = (argc > 1) ? argv[1] : "localhost";
//	port = (argc > 2) ? argv[2] : "8500";
	port = (argc > 2) ? argv[2] : "7500";
//	port = (argc > 2) ? argv[2] : "smtp";

	try {
		SockLibXContext socket_lib;
		unsigned int    count_1;
		for (count_1 = 0; count_1 < 1000; ++count_1) {
			NativeSocketHandle socket_handle = Socket();
			Connect(socket_handle, EndPointIP(AddressIP(host), port));
			CloseSocket(socket_handle);
		}
	}
	catch (std::exception &except) {
		std::cout << std::endl << "MLB::SockLibX error: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << "Test completed." << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

