// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketSocketAsyncFwd class.

	Revision History	:	2010-11-19 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketSocketAsyncFwd_hpp__HH

#define HH__MLB__SocketIo__PacketSocketAsyncFwd_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketSocketAsync.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
template <typename FwdTarget>
	class PacketSocketAsyncFwd : public PacketSocketAsync {
public:
	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &mc_group, IpPortType ip_port,
		const std::string &host_interface = "", bool is_broadcast = true,
		bool bind_local = true)
		:PacketSocketAsync(buffer_size, buffer_sptr, io_service, mc_group,
			ip_port, host_interface, is_broadcast, bind_local)
		,fwd_func_(fwd_func)
	{
	}
	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &mc_group, const std::string &ip_port,
		const std::string &host_interface = "", bool is_broadcast = true,
		bool bind_local = true)
		:PacketSocketAsync(buffer_size, buffer_sptr, io_service, mc_group,
			ip_port, host_interface, is_broadcast, bind_local)
		,fwd_func_(fwd_func)
	{
	}
	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec, bool is_broadcast = true,
		bool bind_local = true)
		:PacketSocketAsync(buffer_size, buffer_sptr, io_service, socket_spec,
			is_broadcast, bind_local)
		,fwd_func_(fwd_func)
	{
	}

	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const std::string &mc_group, IpPortType ip_port,
		const std::string &host_interface = "", bool is_broadcast = true,
		bool bind_local = true)
		:PacketSocketAsync(buffer_size, buffer_ptr, io_service, mc_group,
			ip_port, host_interface, is_broadcast, bind_local)
		,fwd_func_(fwd_func)
	{
	}
	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const std::string &mc_group, const std::string &ip_port,
		const std::string &host_interface = "", bool is_broadcast = true,
		bool bind_local = true)
		:PacketSocketAsync(buffer_size, buffer_ptr, io_service, mc_group,
			ip_port, host_interface, is_broadcast, bind_local)
		,fwd_func_(fwd_func)
	{
	}
	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec, bool is_broadcast = true,
		bool bind_local = true)
		:PacketSocketAsync(buffer_size, buffer_ptr, io_service, socket_spec,
			is_broadcast, bind_local)
		,fwd_func_(fwd_func)
	{
	}

	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &mc_group,
		IpPortType ip_port, const std::string &host_interface = "",
		bool is_broadcast = true, bool bind_local = true)
		:PacketSocketAsync(buffer_size, io_service, mc_group, ip_port,
			host_interface, is_broadcast, bind_local)
		,fwd_func_(fwd_func)
	{
	}
	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &mc_group,
		const std::string &ip_port, const std::string &host_interface = "",
		bool is_broadcast = true, bool bind_local = true)
		:PacketSocketAsync(buffer_size, io_service, mc_group, ip_port,
			host_interface, is_broadcast, bind_local)
		,fwd_func_(fwd_func)
	{
	}
	PacketSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec,
		bool is_broadcast = true, bool bind_local = true)
		:PacketSocketAsync(buffer_size, io_service, socket_spec, is_broadcast,
			bind_local)
		,fwd_func_(fwd_func)
	{
	}

protected:
	FwdTarget fwd_func_;

	virtual bool RecvFromAsyncImpl(const boost::system::error_code &error,
		std::size_t bytes_received)
	{
		return(fwd_func_(*this, error, bytes_received, buffer_.GetPtr(),
			sender_endpoint_));
	}

private:
	PacketSocketAsyncFwd(const PacketSocketAsyncFwd &other);
	PacketSocketAsyncFwd & operator=(const PacketSocketAsyncFwd &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketSocketAsyncFwd_hpp__HH

