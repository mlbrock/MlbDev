// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketReceiver class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketReceiver_hpp__HH

#define HH__MLB__SocketIo__PacketReceiver_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketBase.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class PacketReceiver : public SocketBase<boost::asio::ip::udp::socket> {
public:
	PacketReceiver(boost::asio::io_service &io_service,
		const std::string &socket_address, IpPortType ip_port,
		const std::string &host_interface = "", bool bind_local = true);
	PacketReceiver(boost::asio::io_service &io_service,
		const std::string &socket_address, const std::string &ip_port,
		const std::string &host_interface = "", bool bind_local = true);
	PacketReceiver(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec, bool bind_local = true);

	virtual ~PacketReceiver();

protected:
	boost::asio::ip::address my_address_;

private:
	PacketReceiver(const PacketReceiver &other);
	PacketReceiver & operator = (const PacketReceiver &other);

	void FixUpConstruction();
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<PacketReceiver> PacketReceiverSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketReceiver_hpp__HH

