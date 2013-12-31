// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the StreamSocketAsyncFwd class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__StreamSocketAsyncFwd_hpp__HH

#define HH__MLB__SocketIo__StreamSocketAsyncFwd_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/StreamSocketAsync.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
template <typename FwdTarget>
	class StreamSocketAsyncFwd : public StreamSocketAsync {
public:

	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
	 boost::asio::io_service &io_service)
		:StreamSocketAsync(buffer_size, io_service)
		,fwd_func_(fwd_func)
	{
	}
	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
	 boost::asio::io_service &io_service, const std::string &ip_port,
		const std::string &host_interface)
		:StreamSocketAsync(buffer_size, io_service, ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}

/*
	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &ip_address, IpPortType ip_port,
		const std::string &host_interface = "")
		:StreamSocketAsync(buffer_size, buffer_sptr, io_service, ip_address,
			ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}
	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &ip_address, const std::string &ip_port,
		const std::string &host_interface = "")
		:StreamSocketAsync(buffer_size, buffer_sptr, io_service, ip_address,
			ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}
	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec)
		:StreamSocketAsync(buffer_size, buffer_sptr, io_service, socket_spec)
		,fwd_func_(fwd_func)
	{
	}

	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const std::string &ip_address, IpPortType ip_port,
		const std::string &host_interface = "")
		:StreamSocketAsync(buffer_size, buffer_ptr, io_service, ip_address,
			ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}
	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const std::string &ip_address, const std::string &ip_port,
		const std::string &host_interface = "")
		:StreamSocketAsync(buffer_size, buffer_ptr, io_service, ip_address,
			ip_port, host_interface)
		,fwd_func_(fwd_func)
	{
	}
	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		char *buffer_ptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec)
		:StreamSocketAsync(buffer_size, buffer_ptr, io_service, socket_spec)
		,fwd_func_(fwd_func)
	{
	}

	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &ip_address,
		IpPortType ip_port, const std::string &host_interface = "")
		:StreamSocketAsync(buffer_size, io_service, ip_address, ip_port,
			host_interface)
		,fwd_func_(fwd_func)
	{
	}
	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &ip_address,
		const std::string &ip_port, const std::string &host_interface = "")
		:StreamSocketAsync(buffer_size, io_service, ip_address, ip_port,
			host_interface)
		,fwd_func_(fwd_func)
	{
	}
	StreamSocketAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec)
		:StreamSocketAsync(buffer_size, io_service, socket_spec)
		,fwd_func_(fwd_func)
	{
	}
*/

protected:
	FwdTarget fwd_func_;

	virtual bool RecvAsyncImpl(const boost::system::error_code &error,
		std::size_t bytes_received)
	{
		return(fwd_func_(*this, error, bytes_received, buffer_.GetPtr()));
	}

private:
	StreamSocketAsyncFwd(const StreamSocketAsyncFwd &other);
	StreamSocketAsyncFwd & operator = (const StreamSocketAsyncFwd &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__StreamSocketAsyncFwd_hpp__HH

