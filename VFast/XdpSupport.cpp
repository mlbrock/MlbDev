// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Miscellaneous supporting logic for use with Liffe XDP.

	Revision History	:	2010-05-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/XdpSupport.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

namespace {
// ////////////////////////////////////////////////////////////////////////////
const XdpAddedPacketHeader XdpNonFastPacketDataList[] = {
	{     1,     4,     4,	"SequenceNumberReset",				"Sequence Number Reset"					},
	{     2,     0,     0,	"HeartbeatMessage",					"Heartbeat Message"						},
	{    10,    28,    28,	"RetransmissionResponseMessage",	"Retransmission Response Message"	},
	{    20,    28,    28,	"RetransmissionRequestMessage",	"Retransmission Request Message"		},
	{    22,    20,    20,	"RefreshRequestMessage",			"Refresh Request Message"				},
	{    23,    28,    28,	"RefreshResponseMessage",			"Refresh Response Message"				},
	{    24,    20,    20,	"HeartbeatResponseMessage",		"Heartbeat Response Message"			},
	{   799,     0, 65535,	"GenericDerivativesMessage",		"Generic Derivatives Message"			}
};
const unsigned int          XdpNonFastPacketDataCount  =
	sizeof(XdpNonFastPacketDataList) / sizeof(XdpNonFastPacketDataList[0]);
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const XdpAddedPacketHeader *GetXdpAddedPacketHeaderInfoPtr(
	unsigned int packet_type, bool throw_on_error)
{
	//	Uses switch instead of loop for speed...
	switch (packet_type) {
		case     1:
			return(XdpNonFastPacketDataList + 0);
			break;
		case     2:
			return(XdpNonFastPacketDataList + 1);
			break;
		case    10:
			return(XdpNonFastPacketDataList + 2);
			break;
		case    20:
			return(XdpNonFastPacketDataList + 3);
			break;
		case    22:
			return(XdpNonFastPacketDataList + 4);
			break;
		case    23:
			return(XdpNonFastPacketDataList + 5);
			break;
		case    24:
			return(XdpNonFastPacketDataList + 6);
			break;
		case   799:
			return(XdpNonFastPacketDataList + 7);
			break;
		default   :
			if (throw_on_error)
				MLB::Utility::ThrowInvalidArgument("The specified packet type (" +
					MLB::Utility::AnyToString(packet_type) + ") is not one of the "
					"recognized XDP packet types.");
			break;
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CheckAddedPacketHeaderForXdp(const XdpAddedPacketHeader &added_header_type,
	std::size_t added_packet_length, std::size_t packet_length)
{
	std::ostringstream o_str;

	if ((packet_length < added_header_type.min_added_length_) ||
		 (packet_length > added_header_type.max_added_length_))
		o_str << "The " << added_header_type.packet_type_ << " ('" <<
		added_header_type.packet_type_name_ << "') packet type has a remaining "
		"packet length (" << packet_length << ") which is not within the "
		"permissible bounds of that packet type (" <<
		added_header_type.min_added_length_ << " to " <<
		added_header_type.min_added_length_ << ", inclusive.";
	else if (added_packet_length <= packet_length)
		return;
	else
		o_str << "The " << added_header_type.packet_type_ << " ('" <<
			added_header_type.packet_type_name_ << "') packet type has an "
			"additional packet length (" << added_packet_length << ") which is "
			"greater than the total packet length (" << packet_length << ").";

	MLB::Utility::ThrowLogicError(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

