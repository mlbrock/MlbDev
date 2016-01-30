//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class ServerX.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SockLibX__ServerX_hpp__HH

#define HH__MLB__SockLibX__ServerX_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SockLibX/SocketX.hpp>

#include <Utility/AtomicTypes.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
class ServerX {
public:
	/**
		Construct a server using the listen socket provided in the \e socket
		parameter.

		\param socket is the socket on which listening for client connections
		is to be performed.

		\param listen_backlog describes depth of the queue on which pending
		client requests will be held pending an acept by the server. Note that
		the operating system may quietly ignore your request in this regard.
	*/
	ServerX(SocketX &socket, unsigned int listen_backlog = 0);

	/**
		Construct a server with a listen socket using the specified AddressIP
		and port. By creating and using an AddressIP as a parameter to this
		constructor you can:

		-	Specify that a particular interface be used when creating the
			socket, which may be required on a multi-homed host.

		-	Specify that the socket use \e 127.0.0.1 (or \e localhost ). This
			will prevent connections from processes other than those located on
			the same machine:

		\code
AddressIP("127.0.0.1"); // Bind to the loopback interface using its IP address.
AddressIP("localhost");	// Bind to the loopback interface using its domain name.
		\endcode

		\param server_endpoint is an AddressIP which describes the address of
		the interface to which the server socket is to be bound.

		\param port is an \c int which describes the port number to which the
		server socket is to be bound.

		\param listen_backlog describes depth of the queue on which pending
		client requests will be held pending an acept by the server. Note that
		the operating system may quietly ignore your request in this regard.

		\param address_family is an AddressFamily value which specifies the
		address family of the server socket. Defaults to
		\e AddressFamily_Internet .

		\param socket_type is a SocketType value which specifies the type of the
		server socket. Defaults to \e SocketType_Stream .

		\param protocol_type is a ProtocolType value which specifies the
		protocol for \e address_family of the server socket. Defaults to
		\e ProtocolType_DEFAULT .
	*/
	ServerX(const AddressIP &server_endpoint, int port,
		unsigned int listen_backlog = 0,
		AddressFamily address_family = AddressFamily_Internet,
		SocketType socket_type = SocketType_Stream,
		ProtocolType protocol_type = ProtocolType_DEFAULT);

	/**
		Construct a server with a listen socket using the specified AddressIP
		and port. By creating and using an AddressIP as a parameter to this
		constructor you can:

		-	Specify that a particular interface be used when creating the
			socket, which may be required on a multi-homed host.

		-	Specify that the socket use \e 127.0.0.1 (or \e localhost ). This
			will prevent connections from processes other than those located on
			the same machine:

		\code
AddressIP("127.0.0.1"); // Bind to the loopback interface using its IP address.
AddressIP("localhost");	// Bind to the loopback interface using its domain name.
		\endcode

		\param server_endpoint is an AddressIP which describes the address of
		the interface to which the server socket is to be bound.

		\param port is an pointer to \c const \c char which contains the port
		number or service name to which the server socket is to be bound. 

		\param listen_backlog describes depth of the queue on which pending
		client requests will be held pending an acept by the server. Note that
		the operating system may quietly ignore your request in this regard.

		\param address_family is an AddressFamily value which specifies the
		address family of the server socket. Defaults to
		\e AddressFamily_Internet .

		\param socket_type is a SocketType value which specifies the type of the
		server socket. Defaults to \e SocketType_Stream .

		\param protocol_type is a ProtocolType value which specifies the
		protocol for \e address_family of the server socket. Defaults to
		\e ProtocolType_DEFAULT .
	*/
	ServerX(const AddressIP &server_endpoint, const char *port,
		unsigned int listen_backlog = 0,
		AddressFamily address_family = AddressFamily_Internet,
		SocketType socket_type = SocketType_Stream,
		ProtocolType protocol_type = ProtocolType_DEFAULT);

	/**
		Construct a server with a listen socket using the specified AddressIP
		and port. By creating and using an AddressIP as a parameter to this
		constructor you can:

		-	Specify that a particular interface be used when creating the
			socket, which may be required on a multi-homed host.

		-	Specify that the socket use \e 127.0.0.1 (or \e localhost ). This
			will prevent connections from processes other than those located on
			the same machine:

		\code
AddressIP("127.0.0.1"); // Bind to the loopback interface using its IP address.
AddressIP("localhost");	// Bind to the loopback interface using its domain name.
		\endcode

		\param server_endpoint is an AddressIP which describes the address of
		the interface to which the server socket is to be bound.

		\param port is an \c const \c std::string which contains the port
		number or service name to which the server socket is to be bound. 

		\param listen_backlog describes depth of the queue on which pending
		client requests will be held pending an acept by the server. Note that
		the operating system may quietly ignore your request in this regard.

		\param address_family is an AddressFamily value which specifies the
		address family of the server socket. Defaults to
		\e AddressFamily_Internet .

		\param socket_type is a SocketType value which specifies the type of the
		server socket. Defaults to \e SocketType_Stream .

		\param protocol_type is a ProtocolType value which specifies the
		protocol for \e address_family of the server socket. Defaults to
		\e ProtocolType_DEFAULT .
	*/
	ServerX(const AddressIP &server_endpoint, const std::string &port,
		unsigned int listen_backlog = 0,
		AddressFamily address_family = AddressFamily_Internet,
		SocketType socket_type = SocketType_Stream,
		ProtocolType protocol_type = ProtocolType_DEFAULT);

	/**
		Construct a server with a listen socket on the specified port, and
		an unspecified interface on the local host using \e INADDR_ANY .

		\param port is an \c int which describes the port number to which the
		server socket is to be bound.

		\param listen_backlog describes depth of the queue on which pending
		client requests will be held pending an acept by the server. Note that
		the operating system may quietly ignore your request in this regard.

		\param address_family is an AddressFamily value which specifies the
		address family of the server socket. Defaults to
		\e AddressFamily_Internet .

		\param socket_type is a SocketType value which specifies the type of the
		server socket. Defaults to \e SocketType_Stream .

		\param protocol_type is a ProtocolType value which specifies the
		protocol for \e address_family of the server socket. Defaults to
		\e ProtocolType_DEFAULT .
	*/
	explicit ServerX(int port, unsigned int listen_backlog = 0,
		AddressFamily address_family = AddressFamily_Internet,
		SocketType socket_type = SocketType_Stream,
		ProtocolType protocol_type = ProtocolType_DEFAULT);

	/**
		Construct a server with a listen socket on the specified port, and
		an unspecified interface on the local host using \e INADDR_ANY .

		\param port is an pointer to \c const \c char which contains the port
		number or service name to which the server socket is to be bound. 

		\param listen_backlog describes depth of the queue on which pending
		client requests will be held pending an acept by the server. Note that
		the operating system may quietly ignore your request in this regard.

		\param address_family is an AddressFamily value which specifies the
		address family of the server socket. Defaults to
		\e AddressFamily_Internet .

		\param socket_type is a SocketType value which specifies the type of the
		server socket. Defaults to \e SocketType_Stream .

		\param protocol_type is a ProtocolType value which specifies the
		protocol for \e address_family of the server socket. Defaults to
		\e ProtocolType_DEFAULT .
	*/
	explicit ServerX(const char *port, unsigned int listen_backlog = 0,
		AddressFamily address_family = AddressFamily_Internet,
		SocketType socket_type = SocketType_Stream,
		ProtocolType protocol_type = ProtocolType_DEFAULT);

	/**
		Construct a server with a listen socket on the specified port, and
		an unspecified interface on the local host using \e INADDR_ANY .

		\param port is an \c const \c std::string which contains the port
		number or service name to which the server socket is to be bound. 

		\param listen_backlog describes depth of the queue on which pending
		client requests will be held pending an acept by the server. Note that
		the operating system may quietly ignore your request in this regard.

		\param address_family is an AddressFamily value which specifies the
		address family of the server socket. Defaults to
		\e AddressFamily_Internet .

		\param socket_type is a SocketType value which specifies the type of the
		server socket. Defaults to \e SocketType_Stream .

		\param protocol_type is a ProtocolType value which specifies the
		protocol for \e address_family of the server socket. Defaults to
		\e ProtocolType_DEFAULT .
	*/
	explicit ServerX(const std::string &port, unsigned int listen_backlog = 0,
		AddressFamily address_family = AddressFamily_Internet,
		SocketType socket_type = SocketType_Stream,
		ProtocolType protocol_type = ProtocolType_DEFAULT);

	SocketX &GetSocketRef();

	/**
		Waits until a client connects to the server or until \e max_wait_usecs
		microseconds have elapsed.

		\param client_socket is the socket into which information about
		the new client connection will be placed if an accept is performed.

		\param max_wait_usecs is the maximum number of microseconds to wait
		for a client to connect before returning.

		\return \e true if a client connection has been accepted, \e false
		otherwise.

	*/
	bool Accept(SocketX &client_socket, unsigned int max_wait_usecs = 1);

	/**
		Waits until a client connects to the server or until \e wait_until_time
		is reached.

		\param client_socket is the socket into which information about
		the new client connection will be placed if an accept is performed.

		\param wait_until_time is the GMT/UTC time until which this method
		will wait for a client connection before returning. Note that if the
		\e wait_until_time has already passed when this method is called,
		it will check once to determine whether any client connections are
		pending before returning.

		\param cycle_usecs is the maximum number of microseconds to wait in
		\e select() for a client connection. The default is 100,000 microseconds
		( one-tenth of a second).

		\param cycle_interrupt_flag_ptr is a pointer to a flag which this
		method will interrogate each cycle. If its value is found to be	
		non-zero, this method will return \e false.

		The default for \e cycle_interrupt_flag_ptr is \e NULL , which indicates
		that no checking of the flag is to be performed.

		\return \e true if a client connection has been accepted, \e false
		otherwise.

		\note This method checks to see if a critical event has been registered
		by calling \e CriticalEventTest() each pass through the loop. If one
		has, it returns \e false.
	*/
	bool WaitForAccept(SocketX &client_socket,
		const MLB::Utility::TimeVal &wait_until_time,
		unsigned int cycle_usecs = 100000,
		volatile MLB::Utility::AtomicFlag *cycle_interrupt_flag_ptr = NULL);

	bool ShouldTryAccept(
		volatile MLB::Utility::AtomicFlag *cycle_interrupt_flag_ptr) const;
	bool ShouldTryAccept() const;

	EndPointIP   endpoint_;
	unsigned int listen_backlog_;
	SocketX      socket_;

private:
	void Initialize();
	bool AcceptInternal(SocketX &client_socket);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#endif // #ifndef HH__MLB__SockLibX__ServerX_hpp__HH

