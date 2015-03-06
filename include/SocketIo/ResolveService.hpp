//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class ResolveService.

	Revision History	:	2008-04-22 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__ResolveService_hpp__HH

#define HH__MLB__SocketIo__ResolveService_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/BoostAsioInclude.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

//	////////////////////////////////////////////////////////////////////////////
struct ServiceEntry {
	explicit ServiceEntry(const servent *serv_entry_ptr = NULL)
		:s_name()
		,s_aliases()
		,s_port(0)
		,s_proto()
	{
		if (serv_entry_ptr != NULL) {
			s_name = serv_entry_ptr->s_name;
			s_port = serv_entry_ptr->s_port;
			char **tmp_list;
			if ((tmp_list = serv_entry_ptr->s_aliases)[0] != NULL) {
				while (*tmp_list != NULL)
					s_aliases.push_back(*tmp_list++);
			}
			if ((serv_entry_ptr->s_proto != NULL) && *serv_entry_ptr->s_proto)
				s_proto = serv_entry_ptr->s_proto;
		}
	}

	void swap(ServiceEntry &other)
	{
		s_name.swap(other.s_name);
		s_aliases.swap(other.s_aliases);
		std::swap(s_port, other.s_port);
		s_proto.swap(other.s_proto);
	}

	std::string              s_name;
	std::vector<std::string> s_aliases;
	int                      s_port;
	std::string              s_proto;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServiceNameToEntry(const char *service,
	const char *protocol = NULL);
ServiceEntry ServiceNameToEntry(const std::string &service,
	const std::string &protocol = "");

ServiceEntry ServicePortToEntry(int service, const char *protocol = NULL);
ServiceEntry ServicePortToEntry(int service, std::string &protocol);
ServiceEntry ServicePortToEntry(const char *service,
	const char *protocol = NULL);
ServiceEntry ServicePortToEntry(const std::string &service,
	const std::string &protocol = "");

ServiceEntry ServiceToEntry(const char *service,
	const char *protocol = NULL);
ServiceEntry ServiceToEntry(const std::string &service,
	const std::string &protocol = "");

IpPortType ResolveService(const char *service, const char *protocol = NULL);
IpPortType ResolveService(const std::string &service,
	const std::string &protocol = "");
IpPortType ResolveService(unsigned int service);
IpPortType ResolveService(int service);
//	////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__ResolveService_hpp__HH

