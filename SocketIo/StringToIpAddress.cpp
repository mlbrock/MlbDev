// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of logic for the conversion of strings
								to IP addresses.

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

#include <SocketIo/StringToIpAddress.hpp>
#include <SocketIo/ResolveHost.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address StringToIpAddress(const std::string &ip_address)
{
	boost::asio::ip::address out_ip_address;

	try {
		if ((!ip_address.empty()) && (!std::isdigit(ip_address[0])))
			return(HostToIpAddress(ip_address));
		return(boost::asio::ip::address::from_string(ip_address));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to convert '" + ip_address +
			"' to an IP address: " + std::string(except.what()));
	}

	//	Not reached.
	return(boost::asio::ip::address());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address_v4 StringToIpAddressV4(const std::string &ip_address)
{
	try {
		if ((!ip_address.empty()) && (!std::isdigit(ip_address[0])))
			return(HostToIpAddress(ip_address).to_v4());
		return(boost::asio::ip::address_v4::from_string(ip_address));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to convert '" + ip_address +
			"' to a v4 IP address: " + std::string(except.what()));
	}

	//	Not reached.
	return(boost::asio::ip::address_v4());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address_v6 StringToIpAddressV6(const std::string &ip_address)
{
	try {
		if ((!ip_address.empty()) && (!std::isdigit(ip_address[0])))
			return(HostToIpAddress(ip_address).to_v6());
		return(boost::asio::ip::address_v6::from_string(ip_address));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to convert '" + ip_address +
			"' to a v6 IP address: " + std::string(except.what()));
	}

	//	Not reached.
	return(boost::asio::ip::address_v6());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address HostInterfaceStringToIpAddress(
	const std::string &host_interface)
{
	return(StringToIpAddress((host_interface.empty()) ? "0.0.0.0" :
		host_interface));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB


