//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class SocketSpecLocal.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__SocketSpecLocal_hpp__HH

#define HH__MLB__SocketIo__SocketSpecLocal_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketSpec.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
struct SocketSpecLocal : public SocketSpec {
	typedef std::vector<SocketSpecLocal> SocketSpecLocalListI;

	explicit SocketSpecLocal(const std::string &ip_port = "",
		const std::string &local_interface = "");
	explicit SocketSpecLocal(IpPortType ip_port,
		const std::string &local_interface = "");
	~SocketSpecLocal();

	bool operator < (const SocketSpecLocal &other) const;

	std::string ToString(char sep_char = '/') const;
	std::string ToStringFormatted(char sep_char = '/') const;

	SocketSpecLocal &FromString(const std::string &src_string,
		char sep_char = '/');

	static void ParseSocketSpecList(const std::string &in_string,
		SocketSpecLocalListI &out_datum);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef SocketSpecLocal::SocketSpecLocalListI SocketSpecLocalList;
typedef SocketSpecLocalList::iterator         SocketSpecLocalListIter;
typedef SocketSpecLocalList::const_iterator   SocketSpecLocalListIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const SocketSpecLocal &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__SocketSpecLocal_hpp__HH

