// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the StreamAcceptorAsync class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/StreamAcceptorAsync.hpp>

#include <boost/bind.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptorAsync::StreamAcceptorAsync(std::size_t buffer_size,
	boost::asio::io_service &io_service)
	:StreamAcceptor(io_service)
	,buffer_size_(buffer_size)
	,is_running_(false)
	,accepted_socket_sptr_(new StreamSocketAsync(buffer_size_, io_service))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptorAsync::StreamAcceptorAsync(std::size_t buffer_size,
	boost::asio::io_service &io_service, IpPortType ip_port,
	const std::string &host_interface, bool reuse_addr)
	:StreamAcceptor(io_service, ip_port, host_interface, reuse_addr)
	,buffer_size_(buffer_size)
	,is_running_(false)
	,accepted_socket_sptr_(new StreamSocketAsync(buffer_size_, io_service))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptorAsync::StreamAcceptorAsync(std::size_t buffer_size,
	boost::asio::io_service &io_service, const std::string &ip_port,
	const std::string &host_interface, bool reuse_addr)
	:StreamAcceptor(io_service, ip_port, host_interface, reuse_addr)
	,buffer_size_(buffer_size)
	,is_running_(false)
	,accepted_socket_sptr_(new StreamSocketAsync(buffer_size_, io_service))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptorAsync::StreamAcceptorAsync(std::size_t buffer_size,
	boost::asio::io_service &io_service, const SocketSpec &socket_spec,
	bool reuse_addr)
	:StreamAcceptor(io_service, socket_spec, reuse_addr)
	,buffer_size_(buffer_size)
	,is_running_(false)
	,accepted_socket_sptr_(new StreamSocketAsync(buffer_size_, io_service))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StreamAcceptorAsync::~StreamAcceptorAsync()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamAcceptorAsync::IsRunning() const
{
	return(is_running_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamAcceptorAsync::Run()
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
bool StreamAcceptorAsync::AcceptAsync(const boost::system::error_code &error)
{
	accepted_socket_sptr_->SetIsConnected();

	//	Create a new socket for subsequent accepts.
	StreamSocketAsyncSPtr new_socket_sptr(
		new StreamSocketAsync(buffer_size_, GetIoService()));

	//	Swap with the existing member...
	accepted_socket_sptr_.swap(new_socket_sptr);

	if (AcceptAsyncImpl(error, new_socket_sptr)) {
		Schedule();
		return(true);
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamAcceptorAsync::RunImpl()
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool StreamAcceptorAsync::AcceptAsyncImpl(
	const boost::system::error_code & /* error */,
	StreamSocketAsyncSPtr & /* new_socket_sptr */)
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StreamAcceptorAsync::Schedule()
{
	socket_.async_accept(
		accepted_socket_sptr_->GetSocket(),
		accepted_socket_sptr_->GetEndPoint(),
		boost::bind(&StreamAcceptorAsync::AcceptAsync,
		this, boost::asio::placeholders::error));

	is_running_ = true;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

#include <SocketIo/StreamAcceptorAsyncFwd.hpp>
#include <SocketIo/TimerHandlerCriticalEvent.hpp>

#include <Utility/CriticalEventHandler.hpp>
#include <Utility/Utility_Exception.hpp>

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
struct TEST_AcceptFwd {
	typedef MLB::SocketIo::StreamAcceptorAsyncFwd<TEST_AcceptFwd> MyRecvType;

	TEST_AcceptFwd()
	{
	}

	bool operator () (MyRecvType & /* receiver */,
		const boost::system::error_code &error_code,
		StreamSocketAsyncSPtr &new_socket_sptr)
	{
		if (error_code)
			return(false);

		SocketSpec cln_addr(new_socket_sptr->GetIpAddress(),
			new_socket_sptr->GetPort());

		std::cout << "Accepted a connection from " << cln_addr << "." <<
			std::endl;

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
		std::string ip_port("8612");
		std::string host_interface;
		if (argc > 1) {
			ip_port = argv[1];
			if (argc > 2)
				host_interface = argv[2];
		}
		SocketSpec srv_addr("127.0.0.1", ip_port, host_interface);
		std::cout << "Server will listen on " << srv_addr << std::endl;
		boost::asio::io_service                io_service;
		TimerHandlerCriticalEvent              critical_timer(io_service, 10);
		TEST_AcceptFwd                         accept_func;
		StreamAcceptorAsyncFwd<TEST_AcceptFwd> test_acceptor(accept_func,
			1000000, io_service, ip_port, host_interface);
		critical_timer.Run();
		test_acceptor.Listen();
		test_acceptor.Run();
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

