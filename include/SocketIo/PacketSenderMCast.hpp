// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketSenderMCast class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketSenderMCast_hpp__HH

#define HH__MLB__SocketIo__PacketSenderMCast_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/PacketSender.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class PacketSenderMCast : public PacketSender {
public:
	PacketSenderMCast(boost::asio::io_service &io_service,
		const std::string &socket_address, IpPortType ip_port,
		const std::string &host_interface = "");
	PacketSenderMCast(boost::asio::io_service &io_service,
		const std::string &socket_address, const std::string &ip_port,
		const std::string &host_interface = "");
	PacketSenderMCast(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec);

	virtual ~PacketSenderMCast();

protected:
	virtual void FixUpConstruction();

private:
	PacketSenderMCast(const PacketSenderMCast &other);
	PacketSenderMCast & operator = (const PacketSenderMCast &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<PacketSenderMCast> PacketSenderMCastSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketSenderMCast_hpp__HH

