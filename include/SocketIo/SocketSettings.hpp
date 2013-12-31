// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Miscellaneous socket settings accessors.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__SocketSettings_hpp__HH

#define HH__MLB__SocketIo__SocketSettings_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/BoostAsioInclude.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved to some implementation file.
template <typename SocketType>
	void SetNonBlockingMode(SocketType &socket_ref, bool blocking_mode)
{
	try {
		boost::asio::socket_base::non_blocking_io this_command(blocking_mode);
		socket_ref.io_control(this_command);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to set the socket non-blocking "
			"mode to '" + MLB::Utility::AnyToString(blocking_mode) +
			"' failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved to some implementation file.
inline
std::string GetSockOpt_RethrowMsg(const std::exception &except,
	const std::string &opt_name)
{
	return("Attempt to get the socket " + opt_name + " failed: " +
		std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved to some implementation file.
inline
std::string SetSockOpt_RethrowMsg(const std::exception &except,
	const std::string &opt_name, const std::string &opt_value)
{
	return("Attempt to set the socket " + opt_name + " to '" + opt_value +
		"' failed: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType, typename OptionType>
	OptionType &GetSockOpt(const SocketType &socket_ref, OptionType &get_option,
		const char *opt_name)
{
	try {
		socket_ref.get_option(get_option);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, GetSockOpt_RethrowMsg(except, opt_name));
	}

	return(get_option);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType, typename OptionType>
	const OptionType &SetSockOpt(SocketType &socket, OptionType &set_option,
		const char *opt_name)
{
	try {
		socket.set_option(set_option);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, SetSockOpt_RethrowMsg(except,
			opt_name, MLB::Utility::AnyToString(set_option.value())));
	}

	return(set_option);
}
// ////////////////////////////////////////////////////////////////////////////

//	****************************************************************************

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	int GetSockOpt_TimeToLive(const SocketType &socket_ref)
{
	boost::asio::ip::unicast::hops option;

	return(GetSockOpt(socket_ref, option, "time-to-live hops").value());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	bool GetSockOpt_TcpNoDelay(const SocketType &socket_ref)
{
	typename boost::asio::ip::tcp::no_delay option;

	return(GetSockOpt(socket_ref, option, "TCP no delay flag").value());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	std::size_t GetSockOpt_RecvBufferSize(const SocketType &socket_ref)
{
	boost::asio::socket_base::receive_buffer_size option;

	return(GetSockOpt(socket_ref, option, "receive buffer size").value());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	bool GetSockOpt_ReuseAddr(const SocketType &socket_ref)
{
	typename SocketType::reuse_address option;

	return(GetSockOpt(socket_ref, option, "address reuse flag").value());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	std::size_t GetSockOpt_SendBufferSize(const SocketType &socket_ref)
{
	boost::asio::socket_base::send_buffer_size option;

	return(GetSockOpt(socket_ref, option, "send buffer size").value());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	bool GetSockOpt_DoNotRoute(const SocketType &socket_ref)
{
	boost::asio::socket_base::do_not_route get_option;

	return(GetSockOpt(socket_ref, get_option, "do not route flag").value());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	bool GetSockOpt_IpMulticastLoop(const SocketType &socket_ref)
{
	boost::asio::ip::multicast::enable_loopback get_option;

	return(GetSockOpt(socket_ref, get_option, "ip multicast loop flag").value());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	bool GetSockOpt_KeepAlive(const SocketType &socket_ref)
{
	boost::asio::socket_base::keep_alive get_option;

	return(GetSockOpt(socket_ref, get_option, "keep-alive flag").value());
}
// ////////////////////////////////////////////////////////////////////////////

//	****************************************************************************

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	void SetSockOpt_TimeToLive(SocketType &socket_ref, int opt_value)
{
	boost::asio::ip::unicast::hops set_option(opt_value);

	SetSockOpt(socket_ref, set_option, "time-to-live hops");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	void SetSockOpt_TcpNoDelay(SocketType &socket_ref, bool opt_value)
{
	boost::asio::ip::tcp::no_delay set_option(opt_value);

	SetSockOpt(socket_ref, set_option, "TCP no delay flag");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	void SetSockOpt_RecvBufferSize(SocketType &socket_ref, std::size_t opt_value)
{
	boost::asio::socket_base::receive_buffer_size set_option(opt_value);

	SetSockOpt(socket_ref, set_option, "receive buffer size");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	void SetSockOpt_ReuseAddr(SocketType &socket_ref, bool opt_value)
{
	typename SocketType::reuse_address set_option(opt_value);

	SetSockOpt(socket_ref, set_option, "address reuse flag");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	void SetSockOpt_SendBufferSize(SocketType &socket_ref, std::size_t opt_value)
{
	boost::asio::socket_base::send_buffer_size set_option(opt_value);

	SetSockOpt(socket_ref, set_option, "send buffer size");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	void SetSockOpt_DoNotRoute(SocketType &socket_ref, bool opt_value)
{
	boost::asio::socket_base::do_not_route set_option(opt_value);

	SetSockOpt(socket_ref, set_option, "do not route flag");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	void SetSockOpt_IpMulticastLoop(SocketType &socket_ref, bool opt_value)
{
	boost::asio::ip::multicast::enable_loopback set_option(opt_value);

	SetSockOpt(socket_ref, set_option, "ip multicast loop flag");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SocketType>
	void SetSockOpt_KeepAlive(SocketType &socket_ref, bool opt_value)
{
	boost::asio::socket_base::keep_alive set_option(opt_value);

	SetSockOpt(socket_ref, set_option, "keep-alive flag");
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__SocketSettings_hpp__HH










