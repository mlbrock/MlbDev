//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for resolving a host name or
								host IP address.

								Based on the original MLB NPSL code as modified for
								use in the MLB SockLibX library.

	Revision History	:	2008-07-31 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/ResolveHost.hpp>
#include <SocketIo/IpAddressGeneric.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/ToHexString.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

//	////////////////////////////////////////////////////////////////////////////
#if defined(__SVR4)
# define MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS__gethostbyname_r
# define MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS__gethostbyaddr_r
# define MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS_REENTRANT_FUNCTION
#elif __linux__
# define MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS__gethostbyname_r
# define MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS__gethostbyaddr_r
# define MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS_REENTRANT_FUNCTION
#endif // #if defined(__SVR4)
//	////////////////////////////////////////////////////////////////////////////

#ifdef MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS_REENTRANT_FUNCTION
namespace {
//	////////////////////////////////////////////////////////////////////////////
struct ResolveHostNullDeleter {
	void operator()(void const *) const {}
};
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace
#endif // #ifdef MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS_REENTRANT_FUNCTION

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostNameToEntry(const char *host_name)
{
	if ((host_name == NULL) || (!(*host_name)))
		MLB::Utility::ThrowInvalidArgument("The host name specified for "
			"resolution is " + std::string((host_name == NULL) ? "NULL." :
			"an empty string."));

	HostEntry out_host_entry;

	try {
		hostent                 *host_entry_ptr = NULL;
#ifdef MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS__gethostbyname_r
		hostent                  host_entry;
		char                     tmp_buffer[0xFFFF + 1];
		std::size_t              buffer_length = sizeof(tmp_buffer);
		boost::shared_ptr<char>  buffer_sptr(tmp_buffer, ResolveHostNullDeleter());
		for ( ; ; ) {
			int local_errno;
# if defined(__SVR4)
			if ((host_entry_ptr = ::gethostbyname_r(host_name, &host_entry,
				buffer_sptr.get(), static_cast<int>(buffer_length),
				&local_errno)) == NULL)
				MLB::Utility::ThrowSystemError("Unable to locate host with "
					"'gethostbyname_r()'.");
			else if (!local_errno)
				break;
			else if (local_errno != ERANGE)
				MLB::Utility::ThrowSystemError(local_errno,
					"Call to 'gethostbyname_r()' failed");
			else if (buffer_length >= (1 << 20))
				MLB::Utility::ThrowSystemError(local_errno,
					"Call to 'gethostbyname_r()' failed with 'ERANGE' and the "
					"maximum provisional buffer size was reached");
# elif __linux__
			if (::gethostbyname_r(host_name, &host_entry, buffer_sptr.get(),
				static_cast<int>(buffer_length), &host_entry_ptr,
				&local_errno) == 0) {
				if (host_entry_ptr == NULL)
					MLB::Utility::ThrowSystemError("Unable to locate host with "
						"'gethostbyname_r()'.");
				break;
			}
			else if (local_errno != ERANGE)
				MLB::Utility::ThrowSystemError(local_errno,
					"Call to 'gethostbyname_r()' failed");
			else if (buffer_length >= (1 << 20))
				MLB::Utility::ThrowSystemError(local_errno,
					"Call to 'gethostbyname_r()' failed with 'ERANGE' and the "
					"maximum provisional buffer size was reached");
# endif // #if defined(__SVR4)
			buffer_length += 0xFFFF + 1;
			buffer_sptr.reset(new char[buffer_length]);
		}
#else
		if ((host_entry_ptr = ::gethostbyname(host_name)) == NULL)
			MLB::Utility::ThrowSystemError("Unable to locate host with "
				"'gethostbyname()'.");
#endif // #ifdef MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS__gethostbyname_r
		HostEntry(host_entry_ptr).swap(out_host_entry);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to resolve host name '" +
			std::string(host_name) + "': " + std::string(except.what()));
	}

	return(out_host_entry);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostNameToEntry(const std::string &host_name)
{
	return(HostNameToEntry(host_name.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostAddressToEntry(const void *host_address,
	int host_address_length, int host_address_type)
{
	if (host_address == NULL)
		MLB::Utility::ThrowInvalidArgument("The host address specified for "
			"resolution is a NULL pointer.");
	else if (host_address_length < 1)
		MLB::Utility::ThrowInvalidArgument("The length of the host address "
			"specified for resolution (" +
			MLB::Utility::AnyToString(host_address_length) + ") is less than 1.");

	HostEntry out_host_entry;

	try {
		hostent                 *host_entry_ptr = NULL;
#ifdef MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS__gethostbyaddr_r
		hostent                  host_entry;
		char                     tmp_buffer[0xFFFF + 1];
		std::size_t              buffer_length = sizeof(tmp_buffer);
		boost::shared_ptr<char>  buffer_sptr(tmp_buffer, ResolveHostNullDeleter());
		for ( ; ; ) {
			int local_errno;
# if defined(__SVR4)
			if ((host_entry_ptr = ::gethostbyaddr_r(host_address,
				host_address_length, host_address_type, &host_entry,
				buffer_sptr.get(), static_cast<int>(buffer_length),
				&local_errno)) == NULL)
				MLB::Utility::ThrowSystemError("Unable to locate host with "
					"'gethostbyaddr_r()'.");
			else if (!local_errno)
				break;
			else if (local_errno != ERANGE)
				MLB::Utility::ThrowSystemError(local_errno,
					"Call to 'gethostbyaddr_r()' failed");
			else if (buffer_length >= (1 << 20))
				MLB::Utility::ThrowSystemError(local_errno,
					"Call to 'gethostbyaddr_r()' failed with 'ERANGE' and the "
					"maximum provisional buffer size was reached");
# elif __linux__
			if (::gethostbyaddr_r(host_address, host_address_length,
				host_address_type, &host_entry, buffer_sptr.get(),
				static_cast<int>(buffer_length), &host_entry_ptr,
				&local_errno) == 0) {
				if (host_entry_ptr == NULL)
					MLB::Utility::ThrowSystemError("Unable to locate host with "
						"'gethostbyaddr_r()'.");
				break;
			}
			else if (local_errno != ERANGE)
				MLB::Utility::ThrowSystemError(local_errno,
					"Call to 'gethostbyaddr_r()' failed");
			else if (buffer_length >= (1 << 20))
				MLB::Utility::ThrowSystemError(local_errno,
					"Call to 'gethostbyaddr_r()' failed with 'ERANGE' and the "
					"maximum provisional buffer size was reached");
# endif // #if defined(__SVR4)
			buffer_length += 0xFFFF + 1;
			buffer_sptr.reset(new char[buffer_length]);
		}
#else
# if _MSC_VER
		if ((host_entry_ptr = ::gethostbyaddr(
			static_cast<const char *>(host_address), host_address_length,
			host_address_type)) == NULL)
# else
		if ((host_entry_ptr = ::gethostbyaddr(host_address,
			host_address_length, host_address_type)) == NULL)
# endif // # if _MSC_VER
			MLB::Utility::ThrowSystemError("Unable to locate host with "
				"'gethostbyaddr()'.");
#endif // #ifdef MLB_SOCKET_IO_RESOLVE_HOST_CPP_HAS__gethostbyaddr_r
		HostEntry(host_entry_ptr).swap(out_host_entry);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to resolve host address at '" +
			MLB::Utility::ValueToStringHex(host_address) + " (" +
			MLB::Utility::ToHexString(
			static_cast<std::size_t>(host_address_length),
			static_cast<const char *>(host_address)) + ") with an address length "
			"of " + MLB::Utility::AnyToString(host_address_length) + " and an "
			"address type of " + MLB::Utility::AnyToString(host_address_type) +
			": " + std::string(except.what()));
	}

	return(out_host_entry);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostAddressToEntry(const boost::asio::ip::address_v4 &host_address)
{

	return(HostAddressToEntry(host_address.to_bytes().data(),
		sizeof(boost::asio::ip::address_v4::bytes_type), AF_INET));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostAddressToEntry(const boost::asio::ip::address_v6 &host_address)
{

	return(HostAddressToEntry(host_address.to_bytes().data(),
		sizeof(boost::asio::ip::address_v6::bytes_type), AF_INET6));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostAddressToEntry(const boost::asio::ip::address &host_address)
{
	return((host_address.is_v6()) ?
		HostAddressToEntry(host_address.to_v6()) :
		HostAddressToEntry(host_address.to_v4()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostAddressToEntry(const char *host_address)
{
	return(HostAddressToEntry(ParseIpAddressFromString(host_address)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostAddressToEntry(const std::string &host_address)
{
	return(HostAddressToEntry(host_address.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostToEntry(const char *host)
{
	return(((host != NULL) && *host && std::isdigit(*host)) ?
		HostAddressToEntry(host) : HostNameToEntry(host));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostEntry HostToEntry(const std::string &host)
{
	return(HostToEntry(host.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string HostToHostName(const char *host)
{
	return(HostToEntry(host).h_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string HostToHostName(const std::string &host)
{
	return(HostToHostName(host.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address HostToIpAddress(const HostEntry &h_entry)
{
	try {
		if (h_entry.h_addr_list.empty())
			MLB::Utility::ThrowInvalidArgument("No elements were located in the "
				"host entry address list.");
		if ((h_entry.h_addrtype != AF_INET) && (h_entry.h_addrtype != AF_INET6))
			MLB::Utility::ThrowInvalidArgument("The elements in the host entry "
				"address list are of address type " +
				MLB::Utility::AnyToString(h_entry.h_addrtype) + " --- only "
				"'AF_INET' (" + MLB::Utility::AnyToString(AF_INET) + ") and "
				"'AF_INET6' (" + MLB::Utility::AnyToString(AF_INET6) + ") are "
				"supported.");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to convert a host name to an IP "
			"address: " + std::string(except.what()));
	}

	if (h_entry.h_addrtype == AF_INET) {
		boost::asio::ip::address_v4::bytes_type tmp_bytes;
		::memcpy(tmp_bytes.elems, h_entry.h_addr_list_raw_.get(), 4);
		return(boost::asio::ip::address_v4(tmp_bytes));
	}

	boost::asio::ip::address_v6::bytes_type tmp_bytes;

	::memcpy(tmp_bytes.elems, h_entry.h_addr_list_raw_.get(), 16);

	return(boost::asio::ip::address_v6(tmp_bytes));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address HostToIpAddress(const char *host)
{
	return(HostToIpAddress(HostToEntry(host)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address HostToIpAddress(const std::string &host)
{
	return(HostToIpAddress(host.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string HostToIpAddressString(const char *host)
{
	return(HostToIpAddress(host).to_string());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string HostToIpAddressString(const std::string &host)
{
	return(HostToIpAddressString(host.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address ResolveHost(const char *host)
{
	return(HostToIpAddress(host));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
boost::asio::ip::address ResolveHost(const std::string &host)
{
	return(ResolveHost(host.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::SocketIo;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			std::cout << "Host name [" << argv[count_1] << "]" << std::flush;
			try {
				std::string host_name(HostToHostName(argv[count_1]));
				std::string ip_address(HostToIpAddressString(argv[count_1]));
				std::cout << " = [" << host_name << "] = [" << ip_address <<
					"]" << std::endl;
			}
			catch (const std::exception &except) {
				std::cout << " ERROR: " << except.what() << std::endl;
				return_code = EXIT_FAILURE;
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

