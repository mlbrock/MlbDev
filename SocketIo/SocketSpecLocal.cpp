//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for class SocketSpecLocal.

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

#include <SocketIo/SocketSpecLocal.hpp>
#include <SocketIo/ResolveService.hpp>
#include <SocketIo/MulticastIP.hpp>
#include <SocketIo/StringToIpAddress.hpp>

#include <Utility/StringSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
SocketSpecLocal::SocketSpecLocal(const std::string &ip_port,
	const std::string &local_interface)
	:SocketSpec("0.0.0.0", ip_port, local_interface)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SocketSpecLocal::SocketSpecLocal(IpPortType ip_port,
	const std::string &local_interface)
	:SocketSpec("0.0.0.0", ip_port, local_interface)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SocketSpecLocal::~SocketSpecLocal()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SocketSpecLocal::operator < (const SocketSpecLocal &other) const
{
	return(ip_port_ < other.ip_port_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string SocketSpecLocal::ToString(char sep_char) const
{
	std::ostringstream o_str;

	o_str << ip_port_;

	if (local_interface_ != boost::asio::ip::address())
		o_str << sep_char << local_interface_;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string SocketSpecLocal::ToStringFormatted(char sep_char) const
{
	std::ostringstream o_str;

	o_str << std::setw(5) << ip_port_;

	if (local_interface_ != boost::asio::ip::address())
		o_str << sep_char <<
			std::setw(3 + 1 + 3 + 1 + 3 + 1 + 3 + 1 + 3 + 1 + 3) <<
			local_interface_;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SocketSpecLocal &SocketSpecLocal::FromString(const std::string &src_string,
	char sep_char)
{
	std::vector<std::string> tmp_datum;

	MLB::Utility::SplitString(src_string, std::string(1, sep_char),
		tmp_datum, 0, false);

	if (tmp_datum.empty() || (tmp_datum.size() > 2))
		MLB::Utility::ThrowInvalidArgument("Expected a local IP socket "
			"specification string in the format <ip-port>[" +
			std::string(1, sep_char) + "<host-interface>]");

	tmp_datum[0] = MLB::Utility::Trim(tmp_datum[0]);

	if (tmp_datum.size() > 1)
		tmp_datum[1] = MLB::Utility::Trim(tmp_datum[1]);

	SocketSpecLocal(tmp_datum[0],
		(tmp_datum.size() > 1) ? tmp_datum[1] : "").swap(*this);

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SocketSpecLocal::ParseSocketSpecList(const std::string &spec_string,
	SocketSpecLocalList &out_datum)
{
	std::vector<std::string> tmp_list;

	MLB::Utility::SplitString(spec_string, ",", tmp_list, 0, false);

	SocketSpecLocalList                      tmp_datum;
	std::set<SocketSpecLocal>                mc_spec_set;
	std::vector<std::string>::const_iterator iter_b(tmp_list.begin());
	std::vector<std::string>::const_iterator iter_e(tmp_list.end());

	try {
		for ( ; iter_b != iter_e; ++iter_b) {
			SocketSpecLocal tmp_spec;
			tmp_spec.FromString(*iter_b);
			if (mc_spec_set.find(tmp_spec) != mc_spec_set.end())
				MLB::Utility::ThrowInvalidArgument("The local socket "
					"specification '" + (*iter_b) + "' occurs multiple times.");
			tmp_datum.push_back(tmp_spec);
			mc_spec_set.insert(tmp_spec);
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse the list of local socket "
			"specificatons: " + std::string(except.what()));
	}
	
	out_datum.swap(tmp_datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const SocketSpecLocal &datum)
{
	o_str << datum.ToString();

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

