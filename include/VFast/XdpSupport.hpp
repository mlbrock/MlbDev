// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for VFast Liffe XDP-specific support.

	Revision History	:	2010-05-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__XdpSupport_hpp__HH

#define HH__MLB__VFast__XdpSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file XdpSupport.hpp

	\brief VFast XDP -specific support.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

#include <cstddef>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

#pragma pack(push, 1)

// ////////////////////////////////////////////////////////////////////////////
const Type_UInt32 XdpTemplate_MarketUpdateId         = 1;
const Type_UInt32 XdpTemplate_SettlementPricesId     = 11;
const Type_UInt32 XdpTemplate_OutrightStandingDataId = 12;
const Type_UInt32 XdpTemplate_StrategyStandingDataId = 13;
const Type_UInt32 XdpTemplate_ProductAvailabilityId  = 14;
const Type_UInt32 XdpTemplate_MarketStatusId         = 15;
const Type_UInt32 XdpTemplate_ExchangeMsgId          = 16;
const Type_UInt32 XdpTemplate_ValueAddParamsId       = 17;
const Type_UInt32 XdpTemplate_OpenInterestId         = 20;
const Type_UInt32 XdpTemplate_OptionValuationId      = 21;

const char XdpTemplate_MarketUpdateName[]         = "MarketUpdate";
const char XdpTemplate_SettlementPricesName[]     = "SettlementPrices";
const char XdpTemplate_OutrightStandingDataName[] = "OutrightStandingData";
const char XdpTemplate_StrategyStandingDataName[] = "StrategyStandingData";
const char XdpTemplate_ProductAvailabilityName[]  = "ProductAvailability";
const char XdpTemplate_MarketStatusName[]         = "MarketStatus";
const char XdpTemplate_ExchangeMsgName[]          = "ExchangeMsg";
const char XdpTemplate_ValueAddParamsName[]       = "ValueAddParams";
const char XdpTemplate_OpenInterestName[]         = "OpenInterest";
const char XdpTemplate_OptionValuationName[]      = "OptionValuation";

const Type_UInt32 XdpMsgType_MarketUpdate         = 702;
const Type_UInt32 XdpMsgType_SettlementPrices     = 712;
const Type_UInt32 XdpMsgType_OutrightStandingData = 722;
const Type_UInt32 XdpMsgType_StrategyStandingData = 732;
const Type_UInt32 XdpMsgType_ProductAvailability  = 741;
const Type_UInt32 XdpMsgType_MarketStatus         = 752;
const Type_UInt32 XdpMsgType_ExchangeMsg          = 761;
const Type_UInt32 XdpMsgType_ValueAddParams       = 772;
const Type_UInt32 XdpMsgType_OpenInterest         = 782;
const Type_UInt32 XdpMsgType_OptionValuation      = 792;

const char XdpMsgType_MarketUpdateString[]         = "702";
const char XdpMsgType_SettlementPricesString[]     = "712";
const char XdpMsgType_OutrightStandingDataString[] = "722";
const char XdpMsgType_StrategyStandingDataString[] = "732";
const char XdpMsgType_ProductAvailabilityString[]  = "741";
const char XdpMsgType_MarketStatusString[]         = "752";
const char XdpMsgType_ExchangeMsgString[]          = "761";
const char XdpMsgType_ValueAddParamsString[]       = "772";
const char XdpMsgType_OpenInterestString[]         = "782";
const char XdpMsgType_OptionValuationString[]      = "792";
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	PacketType = 1
*/
struct XdpAPH_SequenceNumberReset {
	/**
		Contains the packet sequence number value that the client should expect
		in the immediately succeeding data packet. Note that this packet will
		contain its own valid packet sequence number in the header portion of
		the message.
	*/
	Type_UInt32 NextSeqNumber;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	PacketType = 10
*/
struct XdpAPH_RetransmissionResponseMessage {
	/**
		This field contains the request message sequence number assigned by the
		client. It is used by the client to couple the request with the response
		message.
	*/
	Type_UInt32 SourceSeqNum;

	/**
		This field represents the Identifier of the source (client) requesting
		retransmission. Field is null padded, left aligned.
	*/
	char        SourceID[20];

	/**
		Indicates whether the retransmission request was accepted or rejected.

		Valid values:

		- A = Accepted.
		- R = Rejected.
	*/
	char        Status;

	/**
		Indicates the reason for the rejection. Valid values:

		- 0 = Message was accepted.
		- 1 = Rejected - invalid username (SourceID).
		- 2 = Rejected - invalid sequence number range.
		- 3 = Rejected - number of packets requested exceeds the predefined system limit.
		- 4 = Rejected - number of retransmission requests in the current day exceeds the predefined system limit.
		- 5 = Rejected - requested packets are not available
		- 6 = Rejected - incorrectly formatted request packet
	*/
	Type_UInt8  RejectReason;

	/**
		For future use.
	*/
	char        Filler[2];
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	PacketType = 20
*/
struct XdpAPH_RetransmissionRequestMessage {
	/**
		Begin Sequence Number of the requested range of messages to be
		retransmitted. Note the Sequence Number refers to the PacketSeqNum in
		the header.

		\c Remark:

		The broadcast stream from which a retransmission is requested has to be
		stated in the field ServiceID in the Packet header of the
		RetransmissionRequest message.
	*/
	Type_UInt32 BeginSeqNum;

	/**
		End Sequence Number of the requested range of messages to be
		retransmitted. Note the Sequence Number refers to the PacketSeqNum in
		the header.

		\c Remark:

		The broadcast stream from which a retransmission is requested has to be
		stated in the field ServiceID in the Packet header of the
		RetransmissionRequest message.
	*/
	Type_UInt32 EndSeqNum;

	/**
		This field represents the Identifier of the source (client) requesting
		retransmission. SourceID is pre-set by the Exchange and subject to
		validation.

		Field is null padded, left aligned.
	*/
	char        SourceID[20];
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	PacketType = 22
*/
struct XdpAPH_RefreshRequestMessage {
	/**
		Identifier of the Source ID requesting the refresh.
	*/
	char SourceID[20];
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	PacketType = 23
*/
struct XdpAPH_RefreshResponseMessage {
	/**
		This field contains the request packet sequence number assigned by the
		client. It is used by the client to couple the request with the response
		message.
	*/
	Type_UInt32 SourceSeqNum;

	/**
		Identifier of the source requesting the refresh.
	*/
	char        SourceID[20];

	/**
		Indicates whether the refresh request has been accepted.

		Valid values:

		- A = Accepted.
		- R = Rejected.
	*/
	char        Status;

	/**
		Indicates the reason for the rejection. Valid values:

		- 0 = Message was accepted.
		- 1 = Rejected - username (SourceID) invalid or already in use.
		- 2 = Rejected - incorrect ServiceID.
		- 3 = Rejected - request incorrectly formatted.
		- 4 = Rejected - incorrect packet type sent.
		- 5 = Rejected - exceeded maximum number of requests per day
		- 6 = Rejected - requested refresh data unavailable
		- 7 = Rejected - request sent to incorrect server (secondary instead of primary).
	*/
	Type_UInt8  RejectReason;

	/**
		For future use.
	*/
	char        Filler[2];
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	PacketType = 24
*/
struct XdpAPH_HeartbeatResponseMessage {
	/**
		This field represents the Identifier of the source (client).

		Field is null padded, left aligned.
	*/
	char SourceID[20];
};
// ////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

// ////////////////////////////////////////////////////////////////////////////
struct XdpAddedPacketHeader {
	Type_UInt16   packet_type_;
	unsigned int  min_added_length_;
	unsigned int  max_added_length_;
	const char   *packet_struct_name_;
	const char   *packet_type_name_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const XdpAddedPacketHeader *GetXdpAddedPacketHeaderInfoPtr(
	unsigned int packet_type, bool throw_on_error = true);
void CheckAddedPacketHeaderForXdp(const XdpAddedPacketHeader &added_header_type,
	std::size_t added_packet_length, std::size_t packet_length);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__XdpSupport_hpp__HH
 
