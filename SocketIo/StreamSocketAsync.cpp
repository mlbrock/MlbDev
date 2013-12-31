// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the StreamSocket class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/StreamSocketAsync.hpp>

#include <boost/bind.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
	IpPortType ip_port, const std::string &host_interface)
	:StreamSocket(io_service, ip_port, host_interface)
	,is_running_(false)
	,buffer_(buffer_size, buffer_sptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
	const std::string &ip_port, const std::string &host_interface)
	:StreamSocket(io_service, ip_port, host_interface)
	,is_running_(false)
	,buffer_(buffer_size, buffer_sptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
	const SocketSpec &socket_spec)
	:StreamSocket(io_service, socket_spec)
	,is_running_(false)
	,buffer_(buffer_size, buffer_sptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service)
	:StreamSocket(io_service)
	,is_running_(false)
	,buffer_(buffer_size, buffer_sptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	char *buffer_ptr, boost::asio::io_service &io_service, IpPortType ip_port,
	const std::string &host_interface)
	:StreamSocket(io_service, ip_port, host_interface)
	,is_running_(false)
	,buffer_(buffer_size, buffer_ptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	char *buffer_ptr, boost::asio::io_service &io_service,
	const std::string &ip_port, const std::string &host_interface)
	:StreamSocket(io_service, ip_port, host_interface)
	,is_running_(false)
	,buffer_(buffer_size, buffer_ptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	char *buffer_ptr, boost::asio::io_service &io_service,
	const SocketSpec &socket_spec)
	:StreamSocket(io_service, socket_spec)
	,is_running_(false)
	,buffer_(buffer_size, buffer_ptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	char *buffer_ptr, boost::asio::io_service &io_service)
	:StreamSocket(io_service)
	,is_running_(false)
	,buffer_(buffer_size, buffer_ptr)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	boost::asio::io_service &io_service, IpPortType ip_port,
	const std::string &host_interface)
	:StreamSocket(io_service, ip_port, host_interface)
	,is_running_(false)
	,buffer_(buffer_size)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	boost::asio::io_service &io_service, const std::string &ip_port,
	const std::string &host_interface)
	:StreamSocket(io_service, ip_port, host_interface)
	,is_running_(false)
	,buffer_(buffer_size)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	boost::asio::io_service &io_service, const SocketSpec &socket_spec)
	:StreamSocket(io_service, socket_spec)
	,is_running_(false)
	,buffer_(buffer_size)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::StreamSocketAsync(std::size_t buffer_size,
	boost::asio::io_service &io_service)
	:StreamSocket(io_service)
	,is_running_(false)
	,buffer_(buffer_size)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamSocketAsync::~StreamSocketAsync()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamSocketAsync::IsRunning() const
{
	return(is_running_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamSocketAsync::Run()
{
	if (!RunImpl())
		return(false);

	if (!is_running_) {
		Schedule();
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamSocketAsync::RecvAsync(const boost::system::error_code &error,
	std::size_t bytes_received)
{
	if (RecvAsyncImpl(error, bytes_received)) {
		Schedule();
		return(true);
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamSocketAsync::RunImpl()
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamSocketAsync::RecvAsyncImpl(
	const boost::system::error_code & /* error */,
	std::size_t /* bytes_received */)
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StreamSocketAsync::Schedule()
{
	socket_.async_receive(
		boost::asio::buffer(buffer_.GetPtr(), buffer_.GetSize()),
		0,
		boost::bind(&StreamSocketAsync::RecvAsync,
		this, boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	is_running_ = true;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

#include <SocketIo/StreamSocketAsyncFwd.hpp>
#include <SocketIo/TimerHandlerCriticalEvent.hpp>

#include <Utility/CriticalEventHandler.hpp>
#include <Utility/Utility_Exception.hpp>

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
struct TEST_RecvFwd {
	typedef MLB::SocketIo::StreamSocketAsyncFwd<TEST_RecvFwd> MyRecvType;

	TEST_RecvFwd()
	{
	}

	bool operator () (MyRecvType & /* receiver */,
		const boost::system::error_code &error_code,
		std::size_t bytes_received, const void *data_buffer)
	{
		if (error_code)
			return(false);

		std::cout << "[" << std::string(static_cast<const char *>(data_buffer),
			bytes_received) << "]" << std::endl;

		return(true);
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		std::string ip_address("localhost");
ip_address = "127.0.0.1";
		std::string ip_port("8612");
		std::string host_interface;
		if (argc > 1) {
			ip_address = argv[1];
			if (argc > 2) {
				ip_port = argv[2];
				if (argc > 3)
					host_interface = argv[3];
			}
		}
/*
		SocketSpec srv_addr(ip_address, ip_port, host_interface);
		std::cout << "Client will connect to " << srv_addr << std::endl;
		boost::asio::io_service io_service;
		StreamSocketAsync       test_receiver(1000000, io_service, ip_port,
			host_interface);
		test_receiver.Connect(ip_address, ip_port);
		std::string rcv_text;
		test_receiver.Recv(rcv_text);
		std::cout << "Message 1 of 3: [" << rcv_text << "]" << std::endl;
		test_receiver.Recv(rcv_text);
		std::cout << "Message 2 of 3: [" << rcv_text << "]" << std::endl;
		test_receiver.Recv(rcv_text);
		std::cout << "Message 3 of 3: [" << rcv_text << "]" << std::endl;
*/
		SocketSpec srv_addr(ip_address, ip_port, host_interface);
		std::cout << "Client will connect to " << srv_addr << std::endl;
		boost::asio::io_service            io_service;
		TimerHandlerCriticalEvent          critical_timer(io_service, 10);
		TEST_RecvFwd                       recv_func;
		StreamSocketAsyncFwd<TEST_RecvFwd> test_receiver(recv_func, 1000000,
			io_service);
		critical_timer.Run();
		test_receiver.Connect(ip_address, ip_port);
		test_receiver.Run();
		io_service.run();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

