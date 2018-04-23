// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the StreamAcceptorAsyncFwd class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__StreamAcceptorAsyncFwd_hpp__HH

#define HH__MLB__SocketIo__StreamAcceptorAsyncFwd_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/StreamAcceptorAsync.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
template <typename FwdTarget>
	class StreamAcceptorAsyncFwd : public StreamAcceptorAsync {
public:

	StreamAcceptorAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service)
		:StreamAcceptorAsync(buffer_size, io_service)
		,fwd_func_(fwd_func)
	{
	}
	StreamAcceptorAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, IpPortType ip_port,
		const std::string &host_interface = "", bool reuse_addr = true)
		:StreamAcceptorAsync(buffer_size, io_service, ip_port, host_interface,
			reuse_addr)
		,fwd_func_(fwd_func)
	{
	}
	StreamAcceptorAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &ip_port,
		const std::string &host_interface = "", bool reuse_addr = true)
		:StreamAcceptorAsync(buffer_size, io_service, ip_port, host_interface,
			reuse_addr)
		,fwd_func_(fwd_func)
	{
	}
	StreamAcceptorAsyncFwd(const FwdTarget &fwd_func, std::size_t buffer_size,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec,
		bool reuse_addr = true)
		:StreamAcceptorAsync(buffer_size, io_service, socket_spec, reuse_addr)
		,fwd_func_(fwd_func)
	{
	}

protected:
	FwdTarget fwd_func_;

	virtual bool AcceptAsyncImpl(const boost::system::error_code &error,
		StreamSocketAsyncSPtr &new_socket_sptr)
	{
		return(fwd_func_(*this, error, new_socket_sptr));
	}

private:
	StreamAcceptorAsyncFwd(const StreamAcceptorAsyncFwd &other);
	StreamAcceptorAsyncFwd & operator = (const StreamAcceptorAsyncFwd &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__StreamAcceptorAsyncFwd_hpp__HH

