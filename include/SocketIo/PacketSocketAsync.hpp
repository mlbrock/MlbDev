// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketSocketAsync class.

	Revision History	:	2010-11-19 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketSocketAsync_hpp__HH

#define HH__MLB__SocketIo__PacketSocketAsync_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketSocket.hpp>
#include <SocketIo/SocketBuffer.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class PacketSocketAsync : public PacketSocket {
public:
	PacketSocketAsync(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &mc_group, IpPortType ip_port,
		const std::string &host_interface = "", bool is_broadcast = true,
		bool bind_local = true);
	PacketSocketAsync(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &mc_group, const std::string &ip_port,
		const std::string &host_interface = "", bool is_broadcast = true,
		bool bind_local = true);
	PacketSocketAsync(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec, bool is_broadcast = true,
		bool bind_local = true);

	PacketSocketAsync(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, const std::string &mc_group,
		IpPortType ip_port, const std::string &host_interface = "",
		bool is_broadcast = true, bool bind_local = true);
	PacketSocketAsync(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, const std::string &mc_group,
		const std::string &ip_port, const std::string &host_interface = "",
		bool is_broadcast = true, bool bind_local = true);
	PacketSocketAsync(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec,
		bool is_broadcast = true, bool bind_local = true);

	PacketSocketAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &mc_group,
		IpPortType ip_port, const std::string &host_interface = "",
		bool is_broadcast = true, bool bind_local = true);
	PacketSocketAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &mc_group,
		const std::string &ip_port, const std::string &host_interface = "",
		bool is_broadcast = true, bool bind_local = true);
	PacketSocketAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec,
		bool is_broadcast = true, bool bind_local = true);

	virtual ~PacketSocketAsync();

	bool IsRunning() const;
	bool Run();
	bool RecvFromAsync(const boost::system::error_code& error,
		std::size_t bytes_received);

protected:
	bool                            is_running_;
	boost::asio::ip::udp::endpoint  sender_endpoint_;
	SocketBuffer                    buffer_;

	virtual bool RunImpl();
	virtual bool RecvFromAsyncImpl(const boost::system::error_code &error,
		std::size_t bytes_received);

	void Schedule();

private:
	PacketSocketAsync(const PacketSocketAsync &other);
	PacketSocketAsync & operator = (const PacketSocketAsync &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketSocketAsync_hpp__HH

