// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for PacketDecodeRegime enumeration support.

	Revision History	:	2008-11-26 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__PacketDecodeRegime_hpp__HH

#define HH__MLB__VFast__PacketDecodeRegime_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PacketDecodeRegime.hpp

	\brief	The VFast PacketDecodeRegime header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

//	////////////////////////////////////////////////////////////////////////////
/**
	\enum PacketDecodeRegime Packet decode regimes are shorthand for the varying
	particularities involved in decoding FAST packets from different exchanges.
*/
enum PacketDecodeRegime {
	/** None or unspecified packet decode regime. */
	PacketDecodeRegime_None    = 0,
	/** Enhanced Broadcast Solution (EBS) for Eurex and Xetra. */
	PacketDecodeRegime_EBS     = 1,
	/** Chicago Mercantile Exchange (and other Globex-based exchanges) original FAST protocol released in autumn 2008. */
	PacketDecodeRegime_CME     = 2,
	/** Synomyn for \e PacketDecodeRegime_CME . */
	PacketDecodeRegime_CME_1   = PacketDecodeRegime_CME,
	/** Synomyn for \e PacketDecodeRegime_CME . */
	PacketDecodeRegime_CME_1_0 = PacketDecodeRegime_CME,
	/** Synomyn for \e PacketDecodeRegime_CME . */
	PacketDecodeRegime_CME_1_X = PacketDecodeRegime_CME,
	/** International Stock Exchange (ISE) FAST messages. */
	PacketDecodeRegime_ISE     = 3,
	/** Chicago Mercantile Exchange (and other Globex-based exchanges) new 2.0 FAST protocol released in 2010. */
	PacketDecodeRegime_CME_2   = 4,
	/** Synomyn for \e PacketDecodeRegime_CME_2 . */
	PacketDecodeRegime_CME_2_0 = PacketDecodeRegime_CME_2,
	/** Synomyn for \e PacketDecodeRegime_CME_2 . */
	PacketDecodeRegime_CME_2_X = PacketDecodeRegime_CME_2,
	/** New York Stock Exchange Liffe (and other NYSE XDP-based exchanges). */
	PacketDecodeRegime_XDP    = 5,
	/** The minimum packet decode regime enumeration value. */
	PacketDecodeRegime_Min     = PacketDecodeRegime_None,
	/** The maximum packet decode regime enumeration value. */
	PacketDecodeRegime_Max     = PacketDecodeRegime_XDP,
	/** The total number of packet decode regime enumeration values. */
	PacketDecodeRegime_Count   = (PacketDecodeRegime_Max - PacketDecodeRegime_Min) + 1
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool               PacketDecodeRegimeIsValid(PacketDecodeRegime in_datum);
PacketDecodeRegime CheckPacketDecodeRegime(PacketDecodeRegime in_datum);
bool               IsPacketDecodeRegimeString(const std::string &in_datum,
	PacketDecodeRegime &out_datum);
bool               IsPacketDecodeRegimeString(const std::string &in_datum);
bool               IsPacketDecodeRegimeStringCme(const std::string &in_datum,
	PacketDecodeRegime &out_datum);
bool               IsPacketDecodeRegimeStringCme(const std::string &in_datum);
std::string        PacketDecodeRegimeToString(PacketDecodeRegime in_datum);
PacketDecodeRegime StringToPacketDecodeRegime(const std::string &in_datum);
PacketDecodeRegime StringToPacketDecodeRegimeCme(const std::string &in_datum);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PacketDecodeRegime &datum);
//	////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__PacketDecodeRegime_hpp__HH

