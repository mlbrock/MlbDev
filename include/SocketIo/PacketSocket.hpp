// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketSocket class.

	Revision History	:	2010-11-19 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketSocket_hpp__HH

#define HH__MLB__SocketIo__PacketSocket_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketBase.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class PacketSocket : public SocketBase<boost::asio::ip::udp::socket> {
public:
	PacketSocket(boost::asio::io_service &io_service,
		const std::string &socket_address, IpPortType ip_port,
		const std::string &host_interface = "", bool is_broadcast = true,
		bool bind_local = true);
	PacketSocket(boost::asio::io_service &io_service,
		const std::string &socket_address, const std::string &ip_port,
		const std::string &host_interface = "", bool is_broadcast = true,
		bool bind_local = true);
	PacketSocket(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec, bool is_broadcast = true,
		bool bind_local = true);

	virtual ~PacketSocket();

	//	CODE NOTE: To be moved into SocketBase.hpp
	bool IsMulticast() const
	{
		return(is_multicast_);
	}
	//	CODE NOTE: To be moved into SocketBase.hpp
	bool IsBroadcast() const
	{
		return(is_broadcast_);
	}

protected:
	bool                           is_multicast_;	//	CODE NOTE: To be moved into SocketBase.hpp
	bool                           is_broadcast_;	//	CODE NOTE: To be moved into SocketBase.hpp
	boost::asio::ip::address       my_address_;
	EndpointType                   packet_endpoint_;
	boost::asio::ip::udp::endpoint recv_endpoint_;

private:
	PacketSocket(const PacketSocket &other);
	PacketSocket & operator = (const PacketSocket &other);

	void FixUpConstruction();
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<PacketSocket> PacketSocketSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketSocket_hpp__HH

