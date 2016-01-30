// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketReceiverAsyncMC class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketReceiverAsyncMC_hpp__HH

#define HH__MLB__SocketIo__PacketReceiverAsyncMC_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketReceiverMC.hpp>
#include <SocketIo/SocketBuffer.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class PacketReceiverAsyncMC : public PacketReceiverMC {
public:
	PacketReceiverAsyncMC(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &mc_group, IpPortType ip_port,
		const std::string &host_interface = "");
	PacketReceiverAsyncMC(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const std::string &mc_group, const std::string &ip_port,
		const std::string &host_interface = "");
	PacketReceiverAsyncMC(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr, boost::asio::io_service &io_service,
		const SocketSpec &socket_spec);

	PacketReceiverAsyncMC(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, const std::string &mc_group,
		IpPortType ip_port, const std::string &host_interface = "");
	PacketReceiverAsyncMC(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, const std::string &mc_group,
		const std::string &ip_port, const std::string &host_interface = "");
	PacketReceiverAsyncMC(std::size_t buffer_size, char *buffer_ptr,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec);

	PacketReceiverAsyncMC(std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &mc_group,
		IpPortType ip_port, const std::string &host_interface = "");
	PacketReceiverAsyncMC(std::size_t buffer_size,
		boost::asio::io_service &io_service, const std::string &mc_group,
		const std::string &ip_port, const std::string &host_interface = "");
	PacketReceiverAsyncMC(std::size_t buffer_size,
		boost::asio::io_service &io_service, const SocketSpec &socket_spec);

	virtual ~PacketReceiverAsyncMC();

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
	PacketReceiverAsyncMC(const PacketReceiverAsyncMC &other);
	PacketReceiverAsyncMC & operator = (const PacketReceiverAsyncMC &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketReceiverAsyncMC_hpp__HH

