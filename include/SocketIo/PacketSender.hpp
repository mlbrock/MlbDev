// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketSender class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketSender_hpp__HH

#define HH__MLB__SocketIo__PacketSender_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketBase.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class PacketSender : public SocketBase<boost::asio::ip::udp::socket> {
public:
	PacketSender(boost::asio::io_service &io_service,
		const std::string &socket_address, IpPortType ip_port,
		const std::string &host_interface, bool bind_local = false);
	PacketSender(boost::asio::io_service &io_service,
		const std::string &socket_address, const std::string &ip_port,
		const std::string &host_interface, bool bind_local = false);
	PacketSender(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec, bool bind_local = false);

	virtual ~PacketSender();

protected:
	virtual void FixUpConstruction();

	EndpointType packet_endpoint_;
	bool         is_multicast_;

private:
	PacketSender(const PacketSender &other);
	PacketSender & operator = (const PacketSender &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<PacketSender> PacketSenderSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketSender_hpp__HH

