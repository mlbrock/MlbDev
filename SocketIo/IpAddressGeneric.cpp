// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  IpAddressGeneric.cpp

   File Description  :  Implementation of the IpAddressGeneric class.

   Revision History  :  2008-12-20 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2008 - 2026.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/IpAddressGeneric.hpp>
#include <SocketIo/MulticastIP.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
IpAddressGeneric AddressFromString(const char *src)
{
	if ((src == NULL) || (!(*src)))
		MLB::Utility::ThrowInvalidArgument("The IP address string is empty.");

	/*
		ASIO version 1.11.0 deprecated a variety of address manipulation
		functions.

		The replacement support was added in Boost version 1.66.0 /
		ASIO version 1.12.0.
	*/
#if (BOOST_VERSION < 106600)
	return(IpAddressGeneric::from_string(src));
#else
	return(boost::asio::ip::make_address(src));
#endif // #if (BOOST_VERSION < 106600)
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
IpAddressGeneric AddressFromString(const std::string &src)
{
	return(AddressFromString(src.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
IpAddressGeneric ParseIpAddressFromString(const char *in_string, bool ipv6_ok,
	bool mc_ok, bool require_mc)
{
	IpAddressGeneric tmp_datum;

	try {
		if ((in_string == NULL) || (!(*in_string))) {
			in_string = "";
			MLB::Utility::ThrowInvalidArgument("The IP address string is empty.");
		}
//		tmp_datum = IpAddressGeneric::from_string(in_string);
		tmp_datum = AddressFromString(in_string);
		/*
			ASIO version 1.11.0 deprecated the way ASIO converts ipv6 to ipv4,
			along with some other address-related logic.

			The updated support was added in Boost version 1.66.0 /
			ASIO version 1.12.0.
		*/
#if (BOOST_VERSION < 106600)
		if ((!ipv6_ok) && tmp_datum.is_v6()) {
			if (!tmp_datum.to_v6().is_v4_compatible())
				MLB::Utility::ThrowInvalidArgument("The address is an IPv6 "
					"address which is not convertible to an IPv4 equivalent.");
			tmp_datum = tmp_datum.to_v6().to_v4();
		}
#else
		if ((!ipv6_ok) && tmp_datum.is_v6()) {
			// Convertibility inside of make_address_v4() function.
			tmp_datum = boost::asio::ip::make_address_v4(
				boost::asio::ip::v4_mapped, tmp_datum.to_v6());
		}
#endif // #if (BOOST_VERSION < 106600)
		if ((!mc_ok) && IsMulticastIP(tmp_datum))
			MLB::Utility::ThrowInvalidArgument("The address is a multicast group "
				"which was not specified as desireable in this context.");
		if (require_mc)
			CheckIsMulticastIP(tmp_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "The " +
			std::string((require_mc) ? " multicast group " : " ip address ") +
			"specification string '" + std::string(in_string) +
			"' is invalid: " + std::string(except.what()));
	}

	return(tmp_datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
IpAddressGeneric ParseIpAddressFromString(const std::string &in_string,
	bool ipv6_ok, bool mc_ok, bool require_mc)
{
	return(ParseIpAddressFromString(in_string.c_str(), ipv6_ok, mc_ok,
		require_mc));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

