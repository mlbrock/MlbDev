//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class SocketSpec.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__SocketSpec_hpp__HH

#define HH__MLB__SocketIo__SocketSpec_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/BoostAsioInclude.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
struct SocketSpec {
	typedef std::vector<SocketSpec> SocketSpecListI;

	explicit SocketSpec(const std::string &ip_address = "",
		const std::string &ip_port = "", const std::string &local_interface = "");
	         SocketSpec(const std::string &ip_address, IpPortType ip_port,
		const std::string &local_interface = "");
	         SocketSpec(IpAddressType ip_address, IpPortType ip_port,
		const std::string &local_interface = "");
	~SocketSpec();

	bool operator <  (const SocketSpec &other) const;
	bool operator == (const SocketSpec &other) const;
	bool operator != (const SocketSpec &other) const;

	void swap(SocketSpec &other);

	bool IPAddressIsEmpty() const;
	bool IPPortIsEmpty() const;
	bool IPAddressOrIPPortIsEmpty() const;
	bool LocalInterfaceIsEmpty() const;

	void CheckIPAddressAndIPPort() const;

	std::string ToString(char sep_char = '/') const;
	std::string ToStringFormatted(char sep_char = '/') const;

	SocketSpec &FromString(const std::string &src_string,
		char sep_char = '/');

	boost::asio::ip::address ip_address_;
	IpPortType               ip_port_;
	boost::asio::ip::address local_interface_;

	bool IsMulticastIPAddress() const;
	bool IsLocalhostIPAddress() const;

	const SocketSpec &CheckMulticastIPAddress() const;

	static void ParseSocketSpecList(const std::string &in_string,
		SocketSpecListI &out_datum, bool require_mc = false);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef SocketSpec::SocketSpecListI    SocketSpecList;
typedef SocketSpecList::iterator       SocketSpecListIter;
typedef SocketSpecList::const_iterator SocketSpecListIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const SocketSpec &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__SocketSpec_hpp__HH

