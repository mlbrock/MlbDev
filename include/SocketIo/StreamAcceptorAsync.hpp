// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the StreamAcceptor class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__StreamAcceptorAsync_hpp__HH

#define HH__MLB__SocketIo__StreamAcceptorAsync_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/StreamAcceptor.hpp>
#include <SocketIo/StreamSocketAsync.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class StreamAcceptorAsync : public StreamAcceptor {
public:
	StreamAcceptorAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service);
	StreamAcceptorAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, IpPortType ip_port,
		const std::string &host_interface = "", bool reuse_addr = true);
	StreamAcceptorAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &ip_port,
		const std::string &host_interface = "", bool reuse_addr = true);
	StreamAcceptorAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec,
		bool reuse_addr = true);

	virtual ~StreamAcceptorAsync();

	bool IsRunning() const;
	bool Run();
	bool AcceptAsync(const boost::system::error_code &error);

protected:
	std::size_t           buffer_size_;
	bool                  is_running_;
	StreamSocketAsyncSPtr accepted_socket_sptr_;

	virtual bool RunImpl();
	virtual bool AcceptAsyncImpl(const boost::system::error_code &error,
		StreamSocketAsyncSPtr &new_socket_sptr);

	void Schedule();

private:
	StreamAcceptorAsync(const StreamAcceptorAsync &other);
	StreamAcceptorAsync & operator = (const StreamAcceptorAsync &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<StreamAcceptorAsync> StreamAcceptorAsyncSPtr;
// ////////////////////////////////////////////////////////////////////////////


} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__StreamAcceptorAsync_hpp__HH

