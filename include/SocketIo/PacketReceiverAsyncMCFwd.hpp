// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketReceiverAsyncMCFwd class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketReceiverAsyncMCFwd_hpp__HH

#define HH__MLB__SocketIo__PacketReceiverAsyncMCFwd_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketReceiverAsyncMC.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
template <typename FwdTarget>
	class PacketReceiverAsyncMCFwd : public PacketReceiverAsyncMC {
public:
	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &mc_group, IpPortType ip_port,
		const std::string &host_interface = "")
		:PacketReceiverAsyncMC(buffer_size, buffer_sptr, io_service, mc_group,
			ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}
	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &mc_group, const std::string &ip_port,
		const std::string &host_interface = "")
		:PacketReceiverAsyncMC(buffer_size, buffer_sptr, io_service, mc_group,
			ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}
	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec)
		:PacketReceiverAsyncMC(buffer_size, buffer_sptr, io_service, socket_spec)
		,fwd_func_(fwd_func)
	{
	}

	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const std::string &mc_group, IpPortType ip_port,
		const std::string &host_interface = "")
		:PacketReceiverAsyncMC(buffer_size, buffer_ptr, io_service, mc_group,
			ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}
	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const std::string &mc_group, const std::string &ip_port,
		const std::string &host_interface = "")
		:PacketReceiverAsyncMC(buffer_size, buffer_ptr, io_service, mc_group,
			ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}
	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec)
		:PacketReceiverAsyncMC(buffer_size, buffer_ptr, io_service, socket_spec)
		,fwd_func_(fwd_func)
	{
	}

	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &mc_group,
		IpPortType ip_port, const std::string &host_interface = "")
		:PacketReceiverAsyncMC(buffer_size, io_service, mc_group, ip_port,
			host_interface)
		,fwd_func_(fwd_func)
	{
	}
	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &mc_group,
		const std::string &ip_port, const std::string &host_interface = "")
		:PacketReceiverAsyncMC(buffer_size, io_service, mc_group, ip_port,
			host_interface)
		,fwd_func_(fwd_func)
	{
	}
	PacketReceiverAsyncMCFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec)
		:PacketReceiverAsyncMC(buffer_size, io_service, socket_spec)
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
	PacketReceiverAsyncMCFwd(const PacketReceiverAsyncMCFwd &other);
	PacketReceiverAsyncMCFwd & operator=(const PacketReceiverAsyncMCFwd &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketReceiverAsyncMCFwd_hpp__HH

