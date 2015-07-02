// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	VFixXPortType.hpp

	File Description	:	Include file for VFixXPortType enumeration support.

	Revision History	:	2015-06-28 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFix__VFixXPortType_hpp__HH

#define HH__MLB__VFix__VFixXPortType_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file VFixXPortType.hpp

	\brief	The VFix header file containing VFixXPortType enumeration support.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/VFix.hpp>

#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
/**
	\enum VFixXPortType The supported VFix transport types.
*/
enum VFixXPortType {
	//	Integer types...
	VFixXPortType_Int,
	VFixXPortType_IntNonNegative,		//	>= 0
	VFixXPortType_IntPositive,			//	>  0
	VFixXPortType_IntLength,
	VFixXPortType_IntTagNum,			//	>  0, no leading zeroes in string
	VFixXPortType_IntSeqNum,			//	>  0
	VFixXPortType_IntNumInGroup,		//	>= 0
	VFixXPortType_IntDayOfMonth,		//	>= 1 <= 12
	//	Floating-point types....
	VFixXPortType_Float,
	VFixXPortType_FloatDecimal,
	VFixXPortType_FloatQty,
	VFixXPortType_FloatPrice,
	VFixXPortType_FloatPriceOffset,
	VFixXPortType_FloatAmt,
	VFixXPortType_FloatPercentage,	
	//	Single-character types...
	VFixXPortType_Char,
	VFixXPortType_CharBoolean,			//	'Y' or 'N'
	//	String types...
	VFixXPortType_MultiValueChar,		//	Single-char fields seperated by space.
	VFixXPortType_MultiValueString,	//	Multi-char fields seperated by space.	
	VFixXPortType_String,
	VFixXPortType_StringLanguage,
	VFixXPortType_StringRawData,
	VFixXPortType_StringTenor,			//	{D|M|W|Y}x (x > 0)
	VFixXPortType_StringXmlData,
	VFixXPortType_StringIsoCountry,	//	ISO 3166 country code
	VFixXPortType_StringIsoCurrency,	//	ISO 4217 currecny code
	VFixXPortType_StringIsoExchange,	//	ISO 10383 market identifier code
	VFixXPortType_MonthYear,				//	yyyymm[dd|ww]
	VFixXPortType_UtcTimestamp,			//	yyyy-mm-ddThh:mm:ss[.xxx]
	VFixXPortType_UtcTimeOnly,			//	hh:mm:ss[.xxx]
	VFixXPortType_UtcDateOnly,			//	yyyy-mm-dd
	VFixXPortType_LocalMktDate,			//	ISO 8601 (yyyy-mm-dd local-time)
	VFixXPortType_TzTimeOnly,			//	hh:mm[:ss][Z|OFF[+|-hh[:mm]]]
	VFixXPortType_TzTimestamp,			//	yyyymmdd-hh:mm:ss[Z|OFF[+|-hh[:mm]]]
	VFixXPortType_Date,
	VFixXPortType_DateTime,
	VFixXPortType_Time,
	//	???
	VFixXPortType_ReservedPlus100,	//	Values > 100 are reserved.
	VFixXPortType_ReservedPlus1000,	//	Values > 1000 are reserved.
	VFixXPortType_ReservedPlus4000,	//	Values > 4000 are reserved.
	VFixXPortType_None,
	VFixXPortType_Minimum =  VFixXPortType_Int,
	VFixXPortType_Maximum =  VFixXPortType_None,
	VFixXPortType_Count   = (VFixXPortType_Maximum - VFixXPortType_Minimum) + 1
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.hpp.
VFixXPortType StringToVFixXPortType(const char *src,
	bool throw_on_error = true);
VFixXPortType StringToVFixXPortType(const std::string &src,
	bool throw_on_error = true);
VFixXPortType StringToVFixXPortType(const char *src_1, const char *src_2,
	bool throw_on_error = true);
VFixXPortType StringToVFixXPortType(const std::string &src_1,
	const std::string &src_2, bool throw_on_error = true);

const char *VFixXPortTypeToString(VFixXPortType src,
	std::size_t selector_index,  bool throw_on_error = true);
const char *VFixXPortTypeToString(VFixXPortType src,
	bool throw_on_error = true);
const char *VFixXPortTypeToStringPFix(VFixXPortType src,
	bool throw_on_error = true);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__VFixXPortType_hpp__HH

