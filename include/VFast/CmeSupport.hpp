// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for VFast CME-specific support.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__CmeSupport_hpp__HH

#define HH__MLB__VFast__CmeSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file CmeSupport.hpp

	\brief VFast CME-specific support.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/InsContext.hpp>

#include <Utility/Utility_Exception.hpp>

#include <map>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
const unsigned int CmeFid_MessageType = 35;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char Cme_MsgType_HeartbeatChar             = '0';
const char Cme_MsgType_LogoutChar                = '5';
const char Cme_MsgType_LogonChar                 = 'A';
const char Cme_MsgType_NewsChar                  = 'B';
const char Cme_MsgType_SecurityDefinitionChar    = 'd';
const char Cme_MsgType_SecurityStatusChar        = 'f';
const char Cme_MsgType_QuoteRequestChar          = 'R';
const char Cme_MsgType_MDRequestChar             = 'V';
const char Cme_MsgType_MDSnapshotFullRefreshChar = 'W';
const char Cme_MsgType_MDIncrementalRefreshChar  = 'X';
const char Cme_MsgType_MDRequestRejectChar       = 'Y';
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::map<unsigned int, char>           CmeTidToMessageTypeMap;
typedef CmeTidToMessageTypeMap::iterator       CmeTidToMessageTypeMapIter;
typedef CmeTidToMessageTypeMap::const_iterator CmeTidToMessageTypeMapIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char             *DetermineCmeMessageType(const InsContext &ins_context,
	const InsItem &template_ref);
const char             *DetermineCmeMessageType(const InsContext &ins_context,
	unsigned int template_id);
CmeTidToMessageTypeMap &ConstructCmeTidToMessageTypeMap(
	const InsContext &ins_context, CmeTidToMessageTypeMap &out_map);
CmeTidToMessageTypeMap  ConstructCmeTidToMessageTypeMap(
	const InsContext &ins_context);
char                    CmeTidToMessageType(
	const CmeTidToMessageTypeMap &msg_type_map, unsigned int template_id);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__CmeSupport_hpp__HH

