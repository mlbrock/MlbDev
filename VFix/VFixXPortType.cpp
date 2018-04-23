// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	VFixXPortType.cpp

	File Description	:	Implementation of the supporting logic for the
								VFixXPortType enumeration.

	Revision History	:	2015-06-28 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/VFixXPortType.hpp>

#include <Utility/C_StringSupport.hpp>
//#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

namespace {

// ////////////////////////////////////////////////////////////////////////////
const char *DataTypeStringFixToVFixList[VFixXPortType_Count][2] = {
	{ "int"                   , "Int"               },
	{ "xs:nonNegativeInteger" , "IntNonNegative"    },
	{ "xs:positiveInteger"    , "IntPositive"       },
	{ "Length"                , "IntLength"         },
	{ "TagNum"                , "IntTagNum"         },
	{ "SeqNum"                , "IntSeqNum"         },
	{ "NumInGroup"            , "IntNumInGroup"     },
	{ "DayOfMonth"            , "IntDayOfMonth"     },
	{ "float"                 , "Float"             },
	{ "xs:decimal"            , "FloatDecimal"      },
	{ "Qty"                   , "FloatQty"          },
	{ "Price"                 , "FloatPrice"        },
	{ "PriceOffset"           , "FloatPriceOffset"  },
	{ "Amt"                   , "FloatAmt"          },
	{ "Percentage"            , "FloatPercentage"   },
	{ "Char"                  , "Char"              },
	{ "Boolean"               , "CharBoolean"       },
	{ "MultipleCharValue"     , "MultiValueChar"    },
	{ "MultipleStringValue"   , "MultiValueString"  },
	{ "String"                , "String"            },
	{ "Language"              , "StringLanguage"    },
	{ "data"                  , "StringRawData"     },
	{ "Tenor"                 , "StringTenor"       },
	{ "XMLData"               , "StringXmlData"     },
	{ "Country"               , "StringIsoCountry"  },
	{ "Currency"              , "StringIsoCurrency" },
	{ "Exchange"              , "StringIsoExchange" },
	{ "MonthYear"             , "MonthYear"         },
	{ "UTCTimestamp"          , "UtcTimestamp"      },
	{ "UTCTimeOnly"           , "UtcTimeOnly"       },
	{ "UTCDateOnly"           , "UtcDateOnly"       },
	{ "LocalMktDate"          , "LocalMktDate"      },
	{ "TZTimeOnly"            , "TzTimeOnly"        },
	{ "TZTimestamp"           , "TzTimestamp"       },
	{ "Date"                  , "Date"              },
	{ "DateTime"              , "DateTime"          },
	{ "Time"                  , "Time"              },
	{ "Reserved100Plus"       , "ReservedPlus100"   },
	{ "Reserved1000Plus"      , "ReservedPlus1000"  },
	{ "Reserved4000Plus"      , "ReservedPlus4000"  },
	{ "None"                  , "None"              }
};
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
VFixXPortType StringToVFixXPortType(const char *src, bool throw_on_error)
{
	if (src && *src) {
		for (int count_1 = 0; count_1 < VFixXPortType_Count; ++count_1) {
			if (!MLB::Utility::Utility_stricmp(src,
				DataTypeStringFixToVFixList[count_1][0]))
				return(static_cast<VFixXPortType>(count_1));
		}
		//	Look-aside...
		if (!MLB::Utility::Utility_stricmp(src, "xs:integer"))
			return(VFixXPortType_Int);
		else if (!MLB::Utility::Utility_stricmp(src, "xs:string"))
			return(VFixXPortType_String);
/*
	Can't resolve these uniquely as they're actually semantic types.
		else if (!MLB::Utility::Utility_stricmp(src, "xs:date"))
		else if (!MLB::Utility::Utility_stricmp(src, "xs:time"))
		else if (!MLB::Utility::Utility_stricmp(src, "xs:dateTime"))
*/
		else if (!MLB::Utility::Utility_stricmp(src, "xs:language"))
			return(VFixXPortType_StringLanguage);
		else if (throw_on_error) {
			std::ostringstream o_str;
			o_str << "Unable to map the string '" << src << "' to a VFix "
				"data type enumeration.";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
	}
	else if (throw_on_error)
		MLB::Utility::ThrowInvalidArgument("No data type name specified.");

	return(VFixXPortType_None);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFixXPortType StringToVFixXPortType(const std::string &src,
	bool throw_on_error)
{
	return(StringToVFixXPortType(src.c_str(), throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFixXPortType StringToVFixXPortType(const char *src_1, const char *src_2,
	bool throw_on_error)
{
	if (((!src_1) || (!(*src_1))) && ((!src_2) || (!(*src_2))))
		return(StringToVFixXPortType(NULL, throw_on_error));
	else if ((src_1 && *src_1) && ((!src_2) || (!(*src_2))))
		return(StringToVFixXPortType(src_1, throw_on_error));
	else if (((!src_1) || (!(*src_1))) && (src_2 || (*src_2)))
		return(StringToVFixXPortType(src_2, throw_on_error));

	VFixXPortType return_code = StringToVFixXPortType(src_1, false);

	return((return_code != VFixXPortType_None) ? return_code :
		StringToVFixXPortType(src_2, throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFixXPortType StringToVFixXPortType(const std::string &src_1,
	const std::string &src_2, bool throw_on_error)
{
	return(StringToVFixXPortType(src_1.c_str(), src_2.c_str(),
		throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *VFixXPortTypeToString(VFixXPortType src,
	std::size_t selector_index, bool throw_on_error)
{
	if (selector_index > 1) {
		std::ostringstream o_str;
		o_str << "Invalid selector index (" << selector_index << ") passed to "
			"VFixXPortTypeToString().";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
	else if ((src < VFixXPortType_Minimum) || (src > VFixXPortType_Maximum)) {
		if (throw_on_error) {
			std::ostringstream o_str;
			o_str << "Invalid VFixXPortType enumeration value (" << src << ").";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		return("*INVALID*");
	}

	return(DataTypeStringFixToVFixList[src][selector_index]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *VFixXPortTypeToString(VFixXPortType src, bool throw_on_error)
{
	return(VFixXPortTypeToString(src, 1, throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *VFixXPortTypeToStringPFix(VFixXPortType src, bool throw_on_error)
{
	return(VFixXPortTypeToString(src, 0, throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

