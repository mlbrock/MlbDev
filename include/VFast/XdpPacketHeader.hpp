// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for VFast Liffe XDP packet headers.

	Revision History	:	2010-05-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__XdpPacketHeader_hpp__HH

#define HH__MLB__VFast__XdpPacketHeader_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file XdpPacketHeader.hpp

/	\brief Support for the Liffe XDP packet headers.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

#pragma pack(push, 1)

// ////////////////////////////////////////////////////////////////////////////
struct XdpPacketHeader {
	/**
		Length of the packet including the 16-byte packet header.
	*/
	Type_UInt16   PacketLength;

	/**
		Identifier for the type of data contained in the packet.
	*/
	Type_UInt16   PacketType;

	/**
		Packet sequence number, unique for each broadcast stream (except if
		reset during the day)
	*/
	Type_UInt32   PacketSeqNum;

	/**
		SourceTime in milliseconds indicating the packet broadcast time. The
		number represents the number of milliseconds since the previous Sunday
		\e 00:00 \e UTC .

		Indicates the time the packet was sent by the market data application.
	*/
	Type_UInt32   SendTime;

	/**
		Numeric value identifying the multicast group.
	*/
	Type_UInt16   ServiceID;

	/**
		Indicates delivery method.

		-  0 = Real Time message (uncompressed)
		-  1 = Refresh message (uncompressed)
		-  2 = Retransmission message (uncompressed)
		-  8 = Real Time message (FAST optimised)
		-  9 = Refresh message (FAST optimised)
		- 10 = Retransmission message (FAST optimised)
	*/
	unsigned char DeliveryFlag;

	/**
		The number of messages that are contained within the packet.
	*/
	unsigned char NumberMsgEntries;

	inline void NetToHostOrder_PacketSeqNum()
	{
		PacketSeqNum = ntohl(PacketSeqNum);
	}

	inline void NetToHostOrder_Other()
	{
		PacketLength = ntohs(PacketLength);
		PacketType   = ntohs(PacketType);
		SendTime     = ntohl(SendTime);
		ServiceID    = ntohs(ServiceID);
	}

	inline void NetToHostOrder()
	{
		NetToHostOrder_PacketSeqNum();
		NetToHostOrder_Other();
	}

	static const unsigned int PacketHeaderLength = 16;
};
// ////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

// ////////////////////////////////////////////////////////////////////////////
inline std::ostream & operator << (std::ostream &o_str,
	const XdpPacketHeader &datum)
{
	o_str
		<< "PacketLength="     << std::setw( 5) << datum.PacketLength     << ":"
		<< "PacketType="       << std::setw( 5) << datum.PacketType       << ":"
		<< "PacketSeqNum="     << std::setw(10) << datum.PacketSeqNum     << ":"
		<< "SendTime="         << std::setw(10) <<
			MLB::Utility::TimeVal(datum.SendTime / 1000,
			static_cast<long>(datum.SendTime % 1000) * 1000L).
			ToStringInterval(MLB::Utility::LengthInterval_TimeVal - 3)     << ":"
		<< "ServiceID="        << std::setw( 5) << datum.ServiceID        << ":"
		<< "DeliveryFlag="     << std::setw( 3) <<
			static_cast<unsigned int>(datum.DeliveryFlag)                  << ":"
		<< "NumberMsgEntries=" << std::setw( 3) <<
			static_cast<unsigned int>(datum.NumberMsgEntries)
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__XdpPacketHeader_hpp__HH
 
