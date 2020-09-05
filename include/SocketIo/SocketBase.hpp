// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the SocketBase class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__SocketBase_hpp__HH

#define HH__MLB__SocketIo__SocketBase_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketSpec.hpp>
#include <SocketIo/StringToIpAddress.hpp>
#include <SocketIo/ResolveHost.hpp>
#include <SocketIo/ResolveService.hpp>
#include <SocketIo/SocketSettings.hpp>

#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
template <typename InSocketType>
	class SocketBase {
public:
	typedef          InSocketType                     SocketType;
	typedef          boost::asio::detail::socket_type SocketTypeNative;
	typedef typename SocketType::endpoint_type        EndpointType;
protected:
	SocketBase(boost::asio::io_service &io_service)
		:socket_(io_service)
		,endpoint_()
		,host_interface_()
		,is_connected_(false)
	{
	}
	SocketBase(boost::asio::io_service &io_service, IpPortType ip_port,
		const std::string &host_interface = "", bool reuse_addr = true,
		bool bind_local = true)
		:socket_(io_service)
		,endpoint_()
		,host_interface_()
		,is_connected_(false)
	{
		Open();
		if (reuse_addr)
			SetSockOpt_ReuseAddr(true);
		if (bind_local)
			Bind(ResolveService(ip_port), host_interface);
	}
	SocketBase(boost::asio::io_service &io_service, const std::string &ip_port,
		const std::string &host_interface = "", bool reuse_addr = true,
		bool bind_local = true)
		:socket_(io_service)
		,endpoint_()
		,host_interface_()
		,is_connected_(false)
	{
		Open();
		if (reuse_addr)
			SetSockOpt_ReuseAddr(true);
		if (bind_local)
			Bind(ResolveService(ip_port), host_interface);
	}
	SocketBase(boost::asio::io_service &io_service,
		const SocketSpec &socket_spec, bool reuse_addr = true,
		bool bind_local = true)
		:socket_(io_service)
		,endpoint_()
		,host_interface_()
		,is_connected_(false)
	{
		Open();
		if (reuse_addr)
			SetSockOpt_ReuseAddr(true);
		if (bind_local)
			Bind(EndpointType(socket_spec.local_interface_, socket_spec.ip_port_));
	}

	virtual ~SocketBase()
	{
	}

	SocketType               socket_;
	EndpointType             endpoint_;
	boost::asio::ip::address host_interface_;
	bool                     is_connected_;

public:
	virtual bool IsOpen() const
	{
		return(socket_.is_open());
	}
	virtual bool IsConnected() const
	{
		return(IsOpen() && is_connected_);
	}
	virtual void CheckIsOpen() const
	{
		if (!IsOpen())
			MLB::Utility::ThrowLogicError("Socket has not been opened.");
	}
	virtual void CheckIsNotOpen() const
	{
		if (IsOpen())
			MLB::Utility::ThrowLogicError("Socket has already been opened.");
	}
	virtual void CheckIsConnected() const
	{
		if (!IsConnected())
			MLB::Utility::ThrowLogicError("Socket has not been connected.");
	}
	virtual void CheckIsNotConnected() const
	{
		if (IsConnected())
			MLB::Utility::ThrowLogicError("Socket has already been connected.");
	}

	virtual void SetIsConnected()
	{
		is_connected_ = true;
	}
	virtual void ClearIsConnected()
	{
		is_connected_ = false;
	}

	virtual SocketType &GetSocket()
	{
		return(socket_);
	}
	virtual SocketTypeNative GetSocketNative()
	{
/*
		MLB 2020-09-05:
		The native() member function was deprecated beginning in Boost 1.47.0.
		It was subsequently been removed in Boost 1.66.0.
*/
#if (BOOST_VERSION < 104700)
		return(static_cast<SocketTypeNative>(GetSocket().native()));
#else
		return(static_cast<SocketTypeNative>(GetSocket().native_handle()));
#endif // #if (BOOST_VERSION < 104700)
	}
	virtual EndpointType &GetEndPoint()
	{
		return(endpoint_);
	}
	virtual boost::asio::io_service &GetIoService()
	{
		return(socket_.get_io_service());
	}

	/**
		Determine the IP address of the multicast group in host order.

		\return the IP address of the multicast group.
	*/
	virtual IpAddressType GetIpAddress() const
	{
		return(endpoint_.address().to_v4().to_ulong());
	}
	/**
		Determine the port of the multicast group in host order.

		\return The port of the multicast group.
	*/
	virtual IpPortType GetPort() const
	{
		return(endpoint_.port());
	}
	std::size_t GetRecvBufferSize() const
	{
		return(GetSockOpt_RecvBufferSize());
	}
	std::size_t GetSendBufferSize() const
	{
		return(GetSockOpt_SendBufferSize());
	}
	void SetRecvBufferSize(std::size_t buffer_size)
	{
		SetSockOpt_RecvBufferSize(buffer_size);
	}
	void SetSendBufferSize(std::size_t buffer_size)
	{
		SetSockOpt_SendBufferSize(buffer_size);
	}

	//	Virtual to permit the MC classes to override for Linux cruftiness...
	virtual void Bind(const EndpointType &bind_endpoint)
	{
//		CheckIsNotOpen();
		try {
			socket_.bind(bind_endpoint);
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Attempt to bind the socket to its "
				"local endpoint failed: " + std::string(except.what()));
		}
		endpoint_       = bind_endpoint;
		host_interface_ = endpoint_.address();
	}
	void Bind(IpPortType ip_port, const std::string &host_interface = "")
	{
		Bind(EndpointType(HostInterfaceStringToIpAddress(host_interface),
			ip_port));
	}
	void Bind(const std::string &host_interface = "")
	{
		if (host_interface.empty())
			Bind(EndpointType());
		else
			Bind(0, host_interface);
	}
	void Close()
	{
		if (IsOpen())
			socket_.close();
	}
	int GetSockOpt_TimeToLive() const
	{
		return(MLB::SocketIo::GetSockOpt_TimeToLive(socket_));
	}
	bool GetSockOpt_TcpNoDelay() const
	{
		return(MLB::SocketIo::GetSockOpt_TcpNoDelay(socket_));
	}
	std::size_t GetSockOpt_RecvBufferSize() const
	{
		return(MLB::SocketIo::GetSockOpt_RecvBufferSize(socket_));
	}
	bool GetSockOpt_ReuseAddr() const
	{
		return(MLB::SocketIo::GetSockOpt_ReuseAddr(socket_));
	}
	std::size_t GetSockOpt_SendBufferSize() const
	{
		return(MLB::SocketIo::GetSockOpt_SendBufferSize(socket_));
	}
	bool GetSockOpt_IpMulticastLoop() const
	{
		return(MLB::SocketIo::GetSockOpt_IpMulticastLoop(socket_));
	}
	bool GetSockOpt_DoNotRoute() const
	{
		return(MLB::SocketIo::GetSockOpt_DoNotRoute(socket_));
	}
	bool GetSockOpt_KeepAlive() const
	{
		return(MLB::SocketIo::GetSockOpt_KeepAlive(socket_));
	}

	void Open()
	{
		try {
			CheckIsNotOpen();
			socket_.open(endpoint_.protocol());
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Attempt to open the socket failed: " +
				std::string(except.what()));
		}
	}

	std::size_t RecvFrom(const boost::asio::mutable_buffers_1 &packet_buffer,
		boost::asio::ip::udp::endpoint &src_endpoint)
	{
		return(socket_.receive_from(packet_buffer, src_endpoint));
	}
	std::size_t RecvFrom(std::size_t packet_length, void *packet_ptr,
		boost::asio::ip::udp::endpoint &src_endpoint)
	{
		return(RecvFrom(boost::asio::buffer(packet_ptr, packet_length),
			src_endpoint));
	}
	std::size_t RecvFrom(void *begin_ptr, void *end_ptr,
		boost::asio::ip::udp::endpoint &src_endpoint)
	{
		if (begin_ptr >= end_ptr)
			MLB::Utility::ThrowInvalidArgument("The length of the data in the "
				"packet to be sent is specified by a begin pointer (" +
				MLB::Utility::ValueToStringHex(begin_ptr) + ") which is not less "
				"than the end pointer (" + MLB::Utility::ValueToStringHex(end_ptr) + 
				").");

		return(RecvFrom(static_cast<std::size_t>(static_cast<char *>(end_ptr) -
			static_cast<char *>(begin_ptr)), begin_ptr, src_endpoint));
	}
	std::size_t RecvFrom(std::string &packet_data,
		boost::asio::ip::udp::endpoint &src_endpoint)
	{
		char packet_buffer[0xFFFF + 1];

		std::size_t packet_size = RecvFrom(sizeof(packet_buffer), packet_buffer,
			src_endpoint);

		if (packet_size <= packet_data.size()) {
			::memcpy(const_cast<char *>(packet_data.data()), packet_buffer,
				packet_size);
			if (packet_size < packet_data.size())
				packet_data.resize(packet_size);
		}
		else
			std::string(packet_buffer, packet_size).swap(packet_data);

		return(packet_size);
	}
	std::size_t RecvFrom(const boost::asio::mutable_buffers_1 &packet_buffer)
	{
		boost::asio::ip::udp::endpoint src_endpoint;

		return(RecvFrom(packet_buffer, src_endpoint));
	}
	std::size_t RecvFrom(std::size_t packet_length, void *packet_ptr)
	{
		boost::asio::ip::udp::endpoint src_endpoint;

		return(RecvFrom(packet_length, packet_ptr, src_endpoint));
	}
	std::size_t RecvFrom(void *begin_ptr, void *end_ptr)
	{
		boost::asio::ip::udp::endpoint src_endpoint;

		return(RecvFrom(begin_ptr, end_ptr, src_endpoint));
	}
	std::size_t RecvFrom(std::string &packet_data)
	{
		boost::asio::ip::udp::endpoint src_endpoint;

		return(RecvFrom(packet_data, src_endpoint));
	}

	std::size_t SendTo(const boost::asio::const_buffers_1 &packet_buffer)
	{
		return(socket_.send_to(packet_buffer, endpoint_));
	}
	std::size_t SendTo(std::size_t packet_length, const void *packet_ptr)
	{
		if (packet_length < 1)
			MLB::Utility::ThrowInvalidArgument("The length of the data in the "
				"packet to be sent is specified by a length of zero.");

		return(SendTo(boost::asio::buffer(packet_ptr, packet_length)));
	}
	std::size_t SendTo(const void *begin_ptr, const void *end_ptr)
	{
		if (begin_ptr >= end_ptr)
			MLB::Utility::ThrowInvalidArgument("The length of the data in the "
				"packet to be sent is specified by a begin pointer (" +
				MLB::Utility::ValueToStringHex(begin_ptr) + ") which is not less "
				"than the end pointer (" +
				MLB::Utility::ValueToStringHex(end_ptr) + ").");

		return(SendTo(static_cast<std::size_t>(
			static_cast<const char *>(end_ptr) -
			static_cast<const char *>(begin_ptr)), begin_ptr));
	}
	std::size_t SendTo(const std::string &packet_data)
	{
		return(SendTo(packet_data.size(), packet_data.c_str()));
	}

	void SetNonBlockingMode(bool blocking_mode)
	{
		MLB::SocketIo::SetNonBlockingMode(socket_, blocking_mode);
	}

	void SetSockOpt_TimeToLive(int opt_value)
	{
		MLB::SocketIo::SetSockOpt_TimeToLive(socket_, opt_value);
	}
	void SetSockOpt_TcpNoDelay(bool opt_value)
	{
		MLB::SocketIo::SetSockOpt_TcpNoDelay(socket_, opt_value);
	}
	void SetSockOpt_RecvBufferSize(std::size_t opt_value)
	{
		MLB::SocketIo::SetSockOpt_RecvBufferSize(socket_, opt_value);
	}
	void SetSockOpt_ReuseAddr(bool opt_value)
	{
		MLB::SocketIo::SetSockOpt_ReuseAddr(socket_, opt_value);
	}
	void SetSockOpt_SendBufferSize(std::size_t opt_value)
	{
		MLB::SocketIo::SetSockOpt_SendBufferSize(socket_, opt_value);
	}
	void SetSockOpt_IpMulticastLoop(bool opt_value)
	{
		MLB::SocketIo::SetSockOpt_IpMulticastLoop(socket_, opt_value);
	}

	void SetSockOpt_KeepAlive(bool opt_value)
	{
		MLB::SocketIo::SetSockOpt_KeepAlive(socket_, opt_value);
	}

	void SetSockOpt_DoNotRoute(bool opt_value)
	{
		MLB::SocketIo::SetSockOpt_DoNotRoute(socket_, opt_value);
	}

private:
	SocketBase(const SocketBase &other);
	SocketBase & operator = (const SocketBase &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__SocketBase_hpp__HH

