// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the PacketFormat enumeration.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__PacketFormat_hpp__HH

#define HH__MLB__SocketIo__PacketFormat_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo.hpp>

#include <ostream>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
enum PacketFormat {
	PacketFormat_Invalid      = -1,
	PacketFormat_Raw          = 0,
	PacketFormat_Length       = 1,
	PacketFormat_Time         = 2,
	PacketFormat_IpSource     = 3,
	PacketFormat_TimeIpSource = 4,
	PacketFormat_IpSourceTLen = 5,
	PacketFormat_Min          = PacketFormat_Raw,
	PacketFormat_Max          = PacketFormat_IpSourceTLen,
	PacketFormat_Count        = (PacketFormat_Max - PacketFormat_Min) + 1
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool         PacketFormatIsValid(PacketFormat packet_format);
PacketFormat CheckPacketFormat(PacketFormat packet_format);
bool         IsPacketFormatString(const char *in_packet_format,
	PacketFormat &out_packet_format);
bool         IsPacketFormatString(const std::string &in_packet_format,
	PacketFormat &out_packet_format);
bool         IsPacketFormatString(const char *in_packet_format);
bool         IsPacketFormatString(const std::string &in_packet_format);
std::string  PacketFormatToString(PacketFormat packet_format);
PacketFormat StringToPacketFormat(const std::string &packet_format);
PacketFormat StringToPacketFormat(const char *packet_format);

std::ostream & operator << (std::ostream &o_str, const PacketFormat &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__PacketFormat_hpp__HH

