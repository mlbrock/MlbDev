// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for VFast CME Fix/FAST 2.0 packet headers.

	Revision History	:	2010-02-07 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__CmePacketHeader_hpp__HH

#define HH__MLB__VFast__CmePacketHeader_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file CmePacketHeader.hpp

/	\brief Support for CME packet headers introduced with CME Fix/FAST 2.0.
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
struct CmePacketHeader2_0 {
	/**
		The preamble sequence number is a 4 byte, unsigned integer representing
		the value distributed in tag \e 34-SeqNum of the related FAST message.
		The preamble sequence number can be used during the arbitration process
		to determine whether any messages have been missed without decoding the
		FAST message, reducing processing time.
	*/
	Type_UInt32 sequence_number_;

	/**
		The sub-channel identifier is 1 unsigned byte found between the preamble
		sequence number and the FAST message. It will be used to identify the
		sub-channel for all FAST messages. This will make it possible to process
		only the messages which carry market data for certain sub-channels and
		ignore all other sub-channels.
	*/
	Type_UInt8  sub_channel_id_;

	void NetToHostOrder()
	{
		sequence_number_ = ntohl(sequence_number_);
	}

	static const unsigned int PacketHeaderLength = 5;
};
// ////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

// ////////////////////////////////////////////////////////////////////////////
inline std::ostream & operator << (std::ostream &o_str,
	const CmePacketHeader2_0 &datum)
{
	o_str
		<< "MsgSeqNum="    << std::setw(10) <<
			datum.sequence_number_ << ":"
		<< "SubChannelId=" << std::setw( 3) <<
			static_cast<unsigned int>(datum.sub_channel_id_)
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__CmePacketHeader_hpp__HH
 
