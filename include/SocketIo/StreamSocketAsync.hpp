// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the StreamSocketAsync class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__StreamSocketAsync_hpp__HH

#define HH__MLB__SocketIo__StreamSocketAsync_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/StreamSocket.hpp>
#include <SocketIo/SocketBuffer.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class StreamSocketAsync : public StreamSocket {
public:
	StreamSocketAsync(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		IpPortType ip_port, const std::string &host_interface = "");
	StreamSocketAsync(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &ip_port, const std::string &host_interface = "");
	StreamSocketAsync(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec);
	StreamSocketAsync(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service);

	StreamSocketAsync(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, IpPortType ip_port,
		const std::string &host_interface = "");
	StreamSocketAsync(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, const std::string &ip_port,
		const std::string &host_interface = "");
	StreamSocketAsync(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec);
	StreamSocketAsync(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service);

	StreamSocketAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, IpPortType ip_port,
		const std::string &host_interface = "");
	StreamSocketAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &ip_port,
		const std::string &host_interface = "");
	StreamSocketAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec);
	StreamSocketAsync(std::size_t buffer_size,
		boost::asio::io_service &io_service);

	virtual ~StreamSocketAsync();

	bool IsRunning() const;
	bool Run();
	bool RecvAsync(const boost::system::error_code &error,
		std::size_t bytes_received);

protected:
	bool         is_running_;
	SocketBuffer buffer_;

	virtual bool RunImpl();
	virtual bool RecvAsyncImpl(const boost::system::error_code &error,
		std::size_t bytes_received);

	void Schedule();

private:
	StreamSocketAsync(const StreamSocketAsync &other);
	StreamSocketAsync & operator = (const StreamSocketAsync &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<StreamSocketAsync> StreamSocketAsyncSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__StreamSocketAsync_hpp__HH

