//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class ResolveHost.

	Revision History	:	2008-04-22 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__ResolveHost_hpp__HH

#define HH__MLB__SocketIo__ResolveHost_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/BoostAsioInclude.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

//	////////////////////////////////////////////////////////////////////////////
struct HostEntry {
	explicit HostEntry(const hostent *host_entry_ptr = NULL)
		:h_name()
		,h_aliases()
		,h_addrtype(0)
		,h_length(0)
		,h_addr_list()
		,h_addr_list_raw_()
	{
		if (host_entry_ptr != NULL) {
			h_name     = host_entry_ptr->h_name;
			h_addrtype = host_entry_ptr->h_addrtype;
			h_length   = host_entry_ptr->h_length;
			char **tmp_list;
			if ((tmp_list = host_entry_ptr->h_aliases)[0] != NULL) {
				while (*tmp_list != NULL)
					h_aliases.push_back(*tmp_list++);
			}
			if ((h_addrtype == AF_INET) &&
				((tmp_list = host_entry_ptr->h_addr_list)[0] != NULL)) {
				while (*tmp_list != NULL)
					h_addr_list.push_back(
						*reinterpret_cast<const IpAddressType *>(*tmp_list++));
			}
			if (h_length) {
				h_addr_list_raw_.
					reset(new char[static_cast<unsigned int>(h_length)]);
				::memcpy(h_addr_list_raw_.get(), *host_entry_ptr->h_addr_list,
					static_cast<std::size_t>(h_length));
			}
		}
	}

	void swap(HostEntry &other)
	{
		h_name.swap(other.h_name);
		h_aliases.swap(other.h_aliases);
		std::swap(h_addrtype, other.h_addrtype);
		std::swap(h_length, other.h_length);
		h_addr_list.swap(other.h_addr_list);
		h_addr_list_raw_.swap(other.h_addr_list_raw_);
	}

	void CheckIpAddress() const
	{
		if (h_addrtype != AF_INET)
			MLB::Utility::ThrowInvalidArgument("The address type (" +
				MLB::Utility::AnyToString(h_addrtype) + ") is not equal to "
				"'AF_INET' (" + MLB::Utility::AnyToString(AF_INET) + ").");
		else if (h_addr_list.empty())
			MLB::Utility::ThrowInvalidArgument("The addresses were found in the "
				"list of IP addresses.");
	}

	std::string                h_name;
	std::vector<std::string>   h_aliases;
	int                        h_addrtype;
	int                        h_length;
	std::vector<IpAddressType> h_addr_list;
	boost::shared_ptr<char>    h_addr_list_raw_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry                HostNameToEntry(const char *host_name);
HostEntry                HostNameToEntry(const std::string &host_name);

HostEntry                HostAddressToEntry(const void *host_address,
	int host_address_length, int host_address_type);
HostEntry                HostAddressToEntry(
	const boost::asio::ip::address_v4 &host_address);
HostEntry                HostAddressToEntry(
	const boost::asio::ip::address_v6 &host_address);
HostEntry                HostAddressToEntry(
	const boost::asio::ip::address &host_address);
HostEntry                HostAddressToEntry(const char *host_address);
HostEntry                HostAddressToEntry(const std::string &host_address);

HostEntry                HostToEntry(const char *host);
HostEntry                HostToEntry(const std::string &host);

std::string              HostToHostName(const char *host);
std::string              HostToHostName(const std::string &host);

boost::asio::ip::address HostToIpAddress(const HostEntry &h_entry);
boost::asio::ip::address HostToIpAddress(const char *host);
boost::asio::ip::address HostToIpAddress(const std::string &host);

std::string              HostToIpAddressString(const char *host);
std::string              HostToIpAddressString(const std::string &host);

boost::asio::ip::address ResolveHost(const char *host);
boost::asio::ip::address ResolveHost(const std::string &host);
//	////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__ResolveHost_hpp__HH

