//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for class SocketMC.

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

#include <SockLibX/SocketMC.hpp>

#ifdef __linux__
# include <arpa/inet.h>
#endif // #ifdef __linux__

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
SocketMC::SocketMC()
	:socket_spec_()
	,mc_socket_sptr_()
	,group_join_done_flag_(false)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SocketMC::SocketMC(const SocketSpec &socket_spec)
	:socket_spec_(socket_spec.CheckMulticastIPAddress())
	,mc_socket_sptr_(new SocketX(AddressFamily_Internet, SocketType_DataGram,
		ProtocolType_DEFAULT))
	,group_join_done_flag_(false)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SocketMC::~SocketMC()
{
	if (HasMCJoined()) {
		try {
			LeaveMulticastGroup();
		}
		catch (const std::exception &) {
		}
	}

	ResetSocket();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketMC::HasMCSpec() const
{
	return(socket_spec_.IsMulticastIPAddress());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SocketMC::HasMCJoined() const
{
	return(group_join_done_flag_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SocketMC::CheckHasMCSpec() const
{
	try {
		socket_spec_.CheckMulticastIPAddress();
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "No multicast socket specification has "
			"been created.");
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SocketMC::CheckHasMCJoined() const
{
	CheckHasMCSpec();

	if (!HasMCJoined())
		MLB::Utility::ThrowLogicError("Multicast group '" +
			socket_spec_.ToString() + "' seems valid but has not yet been "
			"joined.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SocketSpec SocketMC::GetSocketSpec() const
{
	return(socket_spec_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SocketSpec SocketMC::SetSocketSpec(const SocketSpec &socket_spec)
{
	mc_socket_sptr_->CheckNotConnected();

	SocketSpec old_socket_spec(socket_spec_);

	socket_spec_ = socket_spec.CheckMulticastIPAddress();

	return(old_socket_spec);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SocketMC::JoinMulticastGroup()
{
	CheckHasMCSpec();

	if (HasMCJoined())
		MLB::Utility::ThrowLogicError("Multicast group '" +
			socket_spec_.ToString() + "' has already been joined.");

	//	Set the socket address reuse to true...
	int reuse_addr_flag = 1;

	mc_socket_sptr_->SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &reuse_addr_flag,
		sizeof(reuse_addr_flag));

	//	Bind to the desired port...
	NativeSockAddr_In bind_info;
	bind_info.sin_family      = AF_INET;
//	bind_info.sin_addr.s_addr = ::inet_addr(socket_spec_.ip_address_string_.c_str()); 
	bind_info.sin_addr.s_addr = INADDR_ANY;
	bind_info.sin_port        = socket_spec_.ip_address_.GetPortInNetworkOrder(); 
	mc_socket_sptr_->Bind(bind_info);

	AddDropSupport(IP_ADD_MEMBERSHIP);

	group_join_done_flag_ = true;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SocketMC::LeaveMulticastGroup()
{
	CheckHasMCJoined();

	try {
		AddDropSupport(IP_DROP_MEMBERSHIP);
	}
	catch (const std::exception &) {
		ResetSocket();
		group_join_done_flag_ = false;
		throw;
	}

	ResetSocket();

	group_join_done_flag_ = false;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeSocketHandle SocketMC::GetSocketHandle() const
{
	return(mc_socket_sptr_->GetSocket());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SocketMC::RecvFrom(char *buffer_ptr, unsigned int buffer_length,
	int flags, NativeSockAddr *addr, NativeSockLen_T *addrlen) const
{
	return(mc_socket_sptr_->RecvFrom(buffer_ptr, buffer_length, flags, addr,
		addrlen));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SocketMC::SendTo(const char *buffer_ptr,
	unsigned int buffer_length, int flags, const NativeSockAddr *addr,
	NativeSockLen_T addrlen) const
{
	return(mc_socket_sptr_->SendTo(buffer_ptr, buffer_length, flags, addr,
		addrlen));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SocketMC::ResetSocket()
{
	try {
		mc_socket_sptr_.reset(new SocketX(AddressFamily_Internet,
			SocketType_DataGram, ProtocolType_DEFAULT));
	}
	catch (const std::exception &) {
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SocketMC::AddDropSupport(int opt_level)
{
	try {
		struct ip_mreq mc_info;
		mc_info.imr_multiaddr.s_addr =
			::inet_addr(socket_spec_.ip_address_string_.c_str());
//		mc_info.imr_interface.s_addr = (socket_spec_.LocalInterfaceIsEmpty()) ?
//			htonl(INADDR_ANY) :
//			::inet_addr(socket_spec_.local_interface_string_.c_str()); 
if (socket_spec_.local_interface_string_.empty())
	mc_info.imr_interface.s_addr = htonl(INADDR_ANY);
else
	mc_info.imr_interface.s_addr = ::inet_addr(socket_spec_.local_interface_string_.c_str());

		mc_socket_sptr_->SetSockOpt(IPPROTO_IP, opt_level, &mc_info,
			sizeof(mc_info));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to " +
			std::string((opt_level == IP_ADD_MEMBERSHIP) ? "add" : "drop") +
			"membership to the IP multicast group '" +
			socket_spec_.ToString() + "' failed: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

