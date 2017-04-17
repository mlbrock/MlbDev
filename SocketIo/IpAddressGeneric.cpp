// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the IpAddressGeneric class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/IpAddressGeneric.hpp>
#include <SocketIo/MulticastIP.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

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
		tmp_datum = IpAddressGeneric::from_string(in_string);
		if ((!ipv6_ok) && tmp_datum.is_v6()) {
			if (!tmp_datum.to_v6().is_v4_compatible())
				MLB::Utility::ThrowInvalidArgument("The address is an IPv6 "
					"address which is not convertible to an IPv4 equivalent.");
			tmp_datum = tmp_datum.to_v6().to_v4();
		}
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

