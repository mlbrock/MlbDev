//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class SocketSpec.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SockLibX__SocketSpec_hpp__HH

#define HH__MLB__SockLibX__SocketSpec_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SockLibX.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

// ////////////////////////////////////////////////////////////////////////////
struct SocketSpec {
	explicit SocketSpec(const std::string &ip_address = "",
		const std::string &ip_port = "", const std::string &local_interface = "");
	~SocketSpec();

	bool operator < (const SocketSpec &other) const;

	void swap(SocketSpec &other);

	bool IPAddressIsEmpty() const;
	bool LocalInterfaceIsEmpty() const;

	std::string ToString(char sep_char = '/') const;
	std::string ToStringFormatted(char sep_char = '/') const;

	SocketSpec &FromString(const std::string &src_string,
		char sep_char = '/');

	MLB::SockLibX::EndPointIP ip_address_;
	MLB::SockLibX::AddressIP  local_interface_;
	std::string               ip_address_string_;
	std::string               local_interface_string_;

	bool IsMulticastIPAddress() const;
	const SocketSpec &CheckMulticastIPAddress() const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const SocketSpec &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#endif // #ifndef HH__MLB__SockLibX__SocketSpec_hpp__HH

