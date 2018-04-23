//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class SocketX.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SockLibX__SocketX_hpp__HH

#define HH__MLB__SockLibX__SocketX_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SockLibX.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
class API_SOCKLIBX SocketX {
public:
	SocketX(NativeSocketHandle in_socket_handle,
		const NativeSockAddr &in_end_point,
		AddressFamily in_address_family = AddressFamily_Internet,
		SocketType in_socket_type = SocketType_Stream,
		ProtocolType in_protocol_type = ProtocolType_DEFAULT);
	SocketX(NativeSocketHandle in_socket_handle,
		const NativeSockAddr_In &in_end_point,
		AddressFamily in_address_family = AddressFamily_Internet,
		SocketType in_socket_type = SocketType_Stream,
		ProtocolType in_protocol_type = ProtocolType_DEFAULT);
	SocketX(NativeSocketHandle in_socket_handle,
		const EndPointIP &in_end_point,
		AddressFamily in_address_family = AddressFamily_Internet,
		SocketType in_socket_type = SocketType_Stream,
		ProtocolType in_protocol_type = ProtocolType_DEFAULT);
	SocketX(const EndPointIP &in_end_point,
		AddressFamily in_address_family = AddressFamily_Internet,
		SocketType in_socket_type = SocketType_Stream,
		ProtocolType in_protocol_type = ProtocolType_DEFAULT);
	SocketX(AddressFamily in_address_family = AddressFamily_Internet,
		SocketType in_socket_type = SocketType_Stream,
		ProtocolType in_protocol_type = ProtocolType_DEFAULT);
	~SocketX();

	SocketX(const SocketX &other);
	SocketX & operator = (const SocketX &other);

	void         Create();
	void         Connect();
	void         Connect(const EndPointIP &in_end_point);
	void         Close();
	void         Disconnect();

	SocketX             Accept();
	SocketX            &Accept(SocketX &client_socket);
	NativeSocketHandle  Accept(EndPointIP &end_point);
	NativeSocketHandle  Accept(NativeSockAddr_In &addr);
	NativeSocketHandle  Accept(NativeSockAddr &addr,
		NativeSockLen_T &addr_length);
	NativeSocketHandle  Accept(NativeSockAddr *addr = NULL,
		NativeSockLen_T *addrlen = NULL) const;

	void         Bind(const EndPointIP &end_point) const;
	void         Bind(const NativeSockAddr_In &addr) const;
	void         Bind(const NativeSockAddr &addr) const;
	void         Bind(const NativeSockAddr *addr, NativeSockLen_T addrlen) const;

	void         CloseSocket();

	EndPointIP   GetPeerName();

	EndPointIP   GetSockName();

	void         GetSockOpt(int opt_level, int opt_name, void *opt_value,
		int *opt_length);
	void         GetSockOpt(int opt_level, int opt_name, bool &opt_value);
	void         GetSockOpt(int opt_level, int opt_name, int &opt_value);

	void         Listen(unsigned int backlog_count) const;

	unsigned int Recv(std::string &buffer, unsigned int buffer_length = 8192,
		int flags = 0) const;
	unsigned int Recv(char *buffer_ptr, unsigned int buffer_length,
		int flags = 0) const;
	unsigned int Recv(void *buffer_ptr, unsigned int buffer_length,
		int flags = 0) const;

	unsigned int RecvAppend(std::string &buffer, int flags = 0) const;

	unsigned int RecvFrom(char *buffer_ptr, unsigned int buffer_length,
		int flags = 0, NativeSockAddr *addr = NULL,
		NativeSockLen_T *addrlen = NULL) const;
	unsigned int RecvFrom(void *buffer_ptr, unsigned int buffer_length,
		int flags = 0, NativeSockAddr *addr = NULL,
		NativeSockLen_T *addrlen = NULL) const;
	unsigned int RecvFrom(void *buffer_ptr, unsigned int buffer_length,
		int flags, EndPointIP &end_point) const;
	unsigned int RecvFrom(std::string &buffer, unsigned int buffer_length,
		int flags, NativeSockAddr &addr, NativeSockLen_T &addr_length);
	unsigned int RecvFrom(std::string &buffer, unsigned int buffer_length,
		int flags, NativeSockAddr_In &addr);
	unsigned int RecvFrom(std::string &buffer, unsigned int buffer_length,
		int flags, EndPointIP &end_point);

	unsigned int Select(NativeFDSet_T *read_fd_set,
		NativeFDSet_T *write_fd_set, NativeFDSet_T *error_fd_set,
		const struct timeval *time_out_ptr = NULL) const;

	unsigned int SelectRead(const struct timeval *time_out_ptr = NULL) const;
	unsigned int SelectRead(const struct timeval &time_out_ref) const;
	unsigned int SelectRead(unsigned int wait_usecs) const;

	unsigned int SelectWrite(const struct timeval *time_out_ptr = NULL) const;
	unsigned int SelectWrite(const struct timeval &time_out_ref) const;
	unsigned int SelectWrite(unsigned int wait_usecs) const;

	unsigned int SelectError(const struct timeval *time_out_ptr = NULL) const;
	unsigned int SelectError(const struct timeval &time_out_ref) const;
	unsigned int SelectError(unsigned int wait_usecs) const;

	unsigned int Send(const std::ostringstream &buffer, int flags = 0) const;
	unsigned int Send(const std::string &buffer, int flags = 0) const;
	unsigned int Send(const char *buffer_ptr, unsigned int buffer_length,
		int flags = 0) const;
	unsigned int Send(const void *buffer_ptr, unsigned int buffer_length,
		int flags = 0) const;

	unsigned int SendTo(const std::ostringstream &buffer, int flags = 0,
		const NativeSockAddr *addr = NULL, NativeSockLen_T addrlen = 0) const;
	unsigned int SendTo(const std::string &buffer, int flags,
		const NativeSockAddr &addr) const;
	unsigned int SendTo(const std::string &buffer, int flags,
		const NativeSockAddr_In &addr) const;
	unsigned int SendTo(const std::string &buffer, int flags,
		const EndPointIP &end_point) const;
	unsigned int SendTo(const std::string &buffer, int flags = 0,
		const NativeSockAddr *addr = NULL, NativeSockLen_T addrlen = 0) const;
	unsigned int SendTo(const char *buffer_ptr, unsigned int buffer_length,
		int flags = 0, const NativeSockAddr *addr = NULL,
		NativeSockLen_T addrlen = 0) const;

	void         SetSockOpt(int opt_level, int opt_name, const void *opt_value,
		int opt_length);
	void         SetSockOpt(int opt_level, int opt_name, bool opt_value);
	void         SetSockOpt(int opt_level, int opt_name, int opt_value);

	void         ShutDown(ShutDownHow shut_down_how);

	//	/////////////////////////////////////////////////////////////////
	//	/////////////////////////////////////////////////////////////////
	//	Some specialized select methods...
	//	/////////////////////////////////////////////////////////////////
	unsigned int SelectWait(NativeFDSet_T *read_fd_set,
		NativeFDSet_T *write_fd_set, NativeFDSet_T *error_fd_set,
		const struct timeval *time_out_ptr = NULL) const;
	unsigned int SelectWaitOnRead(const struct timeval *time_out_ptr = NULL)
		const;
	unsigned int SelectWaitOnWrite(const struct timeval *time_out_ptr = NULL)
		const;
	unsigned int SelectWaitOnError(const struct timeval *time_out_ptr = NULL)
		const;
	unsigned int SelectWaitOnAny(const struct timeval *time_out_ptr = NULL)
		const;
	//	/////////////////////////////////////////////////////////////////

	//	/////////////////////////////////////////////////////////////////
	//	/////////////////////////////////////////////////////////////////
	// Some specialized {get|set} socket option methods...
	//	/////////////////////////////////////////////////////////////////
	unsigned int GetRecvBufferSize();
	unsigned int GetSendBufferSize();
	void         SetRecvBufferSize(unsigned int buffer_size);
	void         SetSendBufferSize(unsigned int buffer_size);
	//	/////////////////////////////////////////////////////////////////

	//	/////////////////////////////////////////////////////////////////
	//	/////////////////////////////////////////////////////////////////
	// Some supporting bit players...
	//	/////////////////////////////////////////////////////////////////
	NativeFDSet_T  GetFDSet() const;
	NativeFDSet_T &AppendToFDSet(NativeFDSet_T &in_fd_set) const;
	void           SetBlockingMode(bool blocking_mode_flag) const;
	void           SetBlockingModeOn() const;
	void           SetBlockingModeOff() const;
	//	/////////////////////////////////////////////////////////////////

	//	/////////////////////////////////////////////////////////////////
	//	/////////////////////////////////////////////////////////////////
	//	Get socket status...
	//	/////////////////////////////////////////////////////////////////
	bool IsCreated() const;
	bool IsConnected() const;
	bool IsReadyReadOrWrite() const;
	bool IsReadyRead() const;
	bool IsReadyWrite() const;
	//	/////////////////////////////////////////////////////////////////

	//	/////////////////////////////////////////////////////////////////
	//	/////////////////////////////////////////////////////////////////
	// Run-time assertion of socket status...
	//	/////////////////////////////////////////////////////////////////
	void CheckCreated() const;
	void CheckConnected() const;
	void CheckNotCreated() const;
	void CheckNotConnected() const;
	//	/////////////////////////////////////////////////////////////////

	//	/////////////////////////////////////////////////////////////////
	//	/////////////////////////////////////////////////////////////////
	// Various get accessors...
	//	/////////////////////////////////////////////////////////////////
	NativeSocketHandle GetSocket() const;
	AddressFamily      GetAddressFamily() const;
	SocketType         GetSocketType() const;
	ProtocolType       GetProtocolType() const;
	std::string        GetHostIPAddress() const;
	std::string        GetHostIPAddressZeroFilled() const;
	std::string        GetHostName() const;
	int                GetPort() const;
	EndPointIP         GetEndPointIP() const;
	NativeSockAddr_In  GetSockaddrIn() const;
	NativeSockAddr     GetSockaddr() const;
	//	/////////////////////////////////////////////////////////////////

	//	/////////////////////////////////////////////////////////////////
	//	/////////////////////////////////////////////////////////////////
	//	Some special methods...
	//	/////////////////////////////////////////////////////////////////
	void Invalidate();
	//	/////////////////////////////////////////////////////////////////

private:
	AddressFamily      address_family;
	SocketType         socket_type;
	ProtocolType       protocol_type;
	NativeSocketHandle socket_handle;
	EndPointIP         end_point;
	NativeSockAddr     end_point_addr;
	bool               is_connected;
};
//	//////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#endif // #ifndef HH__MLB__SockLibX__SocketX_hpp__HH

