// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the multicast IP address checking
								support.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__MulticastIP_hpp__HH

#define HH__MLB__SocketIo__MulticastIP_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/BoostAsioInclude.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
bool IsMulticastIP(const boost::asio::ip::address_v4 &ip_address);
bool IsMulticastIP(const boost::asio::ip::address_v6 &ip_address);
bool IsMulticastIP(const boost::asio::ip::address &ip_address);
bool IsMulticastIP(const char *ip_address);
bool IsMulticastIP(const std::string &ip_address);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const boost::asio::ip::address_v4 &CheckIsMulticastIP(
	const boost::asio::ip::address_v4 &ip_address);
const boost::asio::ip::address_v6 &CheckIsMulticastIP(
	const boost::asio::ip::address_v6 &ip_address);
const boost::asio::ip::address    &CheckIsMulticastIP(
	const boost::asio::ip::address &ip_address);
const char                        *CheckIsMulticastIP(const char *ip_address);
const std::string                 &CheckIsMulticastIP(
	const std::string &ip_address);
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__MulticastIP_hpp__HH

