//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for class SocketSpec.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketSpec.hpp>
#include <SocketIo/ResolveService.hpp>
#include <SocketIo/MulticastIP.hpp>
#include <SocketIo/StringToIpAddress.hpp>

#include <Utility/StringSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
SocketSpec::SocketSpec(const std::string &ip_address,
	const std::string &ip_port, const std::string &local_interface)
	:ip_address_((ip_address.empty()) ? boost::asio::ip::address() :
		StringToIpAddress(ip_address))
	,ip_port_((ip_port.empty()) ? static_cast<IpPortType>(0) :
		ResolveService(ip_port))
	,local_interface_((local_interface.empty()) ? boost::asio::ip::address() :
		StringToIpAddress(local_interface))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SocketSpec::SocketSpec(const std::string &ip_address, IpPortType ip_port,
	const std::string &local_interface)
	:ip_address_((ip_address.empty()) ? boost::asio::ip::address() :
		StringToIpAddress(ip_address))
	,ip_port_(ResolveService(ip_port))
	,local_interface_((local_interface.empty()) ? boost::asio::ip::address() :
		StringToIpAddress(local_interface))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SocketSpec::SocketSpec(IpAddressType ip_address, IpPortType ip_port,
	const std::string &local_interface)
	:ip_address_(boost::asio::ip::address_v4(ip_address))
	,ip_port_(ip_port)
	,local_interface_((local_interface.empty()) ? boost::asio::ip::address() :
		StringToIpAddress(local_interface))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SocketSpec::~SocketSpec()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::operator < (const SocketSpec &other) const
{
	if (ip_address_ < other.ip_address_)
		return(true);
	else if ((ip_address_ == other.ip_address_) && (ip_port_ < other.ip_port_))
		return(true);

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::operator == (const SocketSpec &other) const
{
	return((!(*this < other)) && (!(other < *this)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::operator != (const SocketSpec &other) const
{
	return((*this < other) || (other < *this));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SocketSpec::swap(SocketSpec &other)
{
	std::swap(ip_address_, other.ip_address_);
	std::swap(ip_port_, other.ip_port_);
	std::swap(local_interface_, other.local_interface_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::IPAddressIsEmpty() const
{
	return(ip_address_ == boost::asio::ip::address());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::IPPortIsEmpty() const
{
	return(ip_port_ == static_cast<IpPortType>(0));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::IPAddressOrIPPortIsEmpty() const
{
	return(IPAddressIsEmpty() || IPPortIsEmpty());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::LocalInterfaceIsEmpty() const
{
	return(local_interface_ == boost::asio::ip::address());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SocketSpec::CheckIPAddressAndIPPort() const
{
	if (IPAddressIsEmpty())
		MLB::Utility::ThrowInvalidArgument("The IP address or host has not been "
			"specified.");
	else if (ip_port_ == static_cast<IpPortType>(0))
		MLB::Utility::ThrowInvalidArgument("The IP port has not been specified.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string SocketSpec::ToString(char sep_char) const
{
	std::ostringstream o_str;

	o_str << ip_address_ << sep_char << ip_port_;

	if (local_interface_ != boost::asio::ip::address())
		o_str << sep_char << local_interface_;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string SocketSpec::ToStringFormatted(char sep_char) const
{
	std::ostringstream o_str;

	o_str << std::setw(3 + 1 + 3 + 1 + 3 + 1 + 3 + 1 + 3 + 1 + 3) <<
		ip_address_ << sep_char << std::setw(5) << ip_port_;

	if (local_interface_ != boost::asio::ip::address())
		o_str << sep_char <<
			std::setw(3 + 1 + 3 + 1 + 3 + 1 + 3 + 1 + 3 + 1 + 3) <<
			local_interface_;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SocketSpec &SocketSpec::FromString(const std::string &src_string,
	char sep_char)
{
	std::vector<std::string> tmp_datum;

	MLB::Utility::SplitString(src_string, std::string(1, sep_char),
		tmp_datum, 0, false);

	if (tmp_datum.empty() || (tmp_datum.size() < 2) || (tmp_datum.size() > 3))
		MLB::Utility::ThrowInvalidArgument("Expected a IP socket "
			"specification string in the format <ip-address>" +
			std::string(1, sep_char) + "<ip-port>[" + std::string(1, sep_char) +
			"<host-interface>]");

	tmp_datum[0] = MLB::Utility::Trim(tmp_datum[0]);
	tmp_datum[1] = MLB::Utility::Trim(tmp_datum[1]);

	if (tmp_datum.size() > 2)
		tmp_datum[2] = MLB::Utility::Trim(tmp_datum[2]);

	SocketSpec(tmp_datum[0], tmp_datum[1],
		(tmp_datum.size() > 2) ? tmp_datum[2] : "").swap(*this);

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::IsMulticastIPAddress() const
{
	return(IsMulticastIP(ip_address_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpec::IsLocalhostIPAddress() const
{
	return(ip_address_ == StringToIpAddress("127.0.0.1"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const SocketSpec &SocketSpec::CheckMulticastIPAddress() const
{
	CheckIsMulticastIP(ip_address_);

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SocketSpec::ParseSocketSpecList(const std::string &spec_string,
	SocketSpecList &out_datum, bool require_mc)
{
	std::vector<std::string> tmp_list;

	MLB::Utility::SplitString(spec_string, ",", tmp_list, 0, false);

	SocketSpecList                           tmp_datum;
	std::set<SocketSpec>                     mc_spec_set;
	std::vector<std::string>::const_iterator iter_b(tmp_list.begin());
	std::vector<std::string>::const_iterator iter_e(tmp_list.end());

	try {
		for ( ; iter_b != iter_e; ++iter_b) {
			SocketSpec tmp_spec;
			tmp_spec.FromString(*iter_b);
			if (require_mc)
				tmp_spec.CheckMulticastIPAddress();
			if (tmp_spec.IPAddressIsEmpty())
				MLB::Utility::ThrowInvalidArgument("The socket " +
					std::string((require_mc) ? "multicast group " : "") +
					"specification '" + (*iter_b) + "' is invalid.");
			else if (mc_spec_set.find(tmp_spec) != mc_spec_set.end())
				MLB::Utility::ThrowInvalidArgument("The socket " +
					std::string((require_mc) ? "multicast group " : "") +
					"specification '" + (*iter_b) + "' occurs multiple times.");
			tmp_datum.push_back(tmp_spec);
			mc_spec_set.insert(tmp_spec);
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse the list of socket "
			"specificatons: " + std::string(except.what()));
	}
	
	out_datum.swap(tmp_datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const SocketSpec &datum)
{
	o_str << datum.ToString();

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

