// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for RvUtilX TibrvMsg wrapper functions.

	Revision History	:	2007-02-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_RvMsgAdd_hpp__HH

#define HH__MLB__RvUtilX_RvMsgAdd_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RvMsg.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
API_RVUTILX TibrvMsg &MsgAddBool(TibrvMsg &msg, const char *field_name,
	bool field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddBool(TibrvMsg &msg, const std::string &field_name,
	bool field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddF32(TibrvMsg &msg, const char *field_name,
	float field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddF32(TibrvMsg &msg, const std::string &field_name,
	float field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddF64(TibrvMsg &msg, const char *field_name,
	double field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddF64(TibrvMsg &msg, const std::string &field_name,
	double field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddI8(TibrvMsg &msg, const char *field_name,
	signed char field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddI8(TibrvMsg &msg, const std::string &field_name,
	signed char field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddI16(TibrvMsg &msg, const char *field_name,
	signed short field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddI16(TibrvMsg &msg, const std::string &field_name,
	signed short field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddI32(TibrvMsg &msg, const char *field_name,
	signed int field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddI32(TibrvMsg &msg, const std::string &field_name,
	signed int field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddI64(TibrvMsg &msg, const char *field_name,
	signed long long field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddI64(TibrvMsg &msg, const std::string &field_name,
	signed long long field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddU8(TibrvMsg &msg, const char *field_name,
	unsigned char field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddU8(TibrvMsg &msg, const std::string &field_name,
	unsigned char field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddU16(TibrvMsg &msg, const char *field_name,
	unsigned short field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddU16(TibrvMsg &msg, const std::string &field_name,
	unsigned short field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddU32(TibrvMsg &msg, const char *field_name,
	unsigned int field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddU32(TibrvMsg &msg, const std::string &field_name,
	unsigned int field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddU64(TibrvMsg &msg, const char *field_name,
	unsigned long long field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddU64(TibrvMsg &msg, const std::string &field_name,
	unsigned long long field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddIPAddr32(TibrvMsg &msg, const char *field_name,
	unsigned int field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddIPAddr32(TibrvMsg &msg,
	const std::string &field_name, unsigned int field_value,
	tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddIPPort16(TibrvMsg &msg, const char *field_name,
	unsigned short field_value, tibrv_u16 field_id = 0);
API_RVUTILX TibrvMsg &MsgAddIPPort16(TibrvMsg &msg,
	const std::string &field_name, unsigned short field_value,
	tibrv_u16 field_id = 0);
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvMsgAdd_hpp__HH

