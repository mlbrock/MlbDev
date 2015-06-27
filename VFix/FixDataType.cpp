// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	InitDatatypesXml.cpp

	File Description	:	Implementation of the InsContext class.

	Revision History	:	2015-06-27 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RapidXmlUtils/RapidXmlContext.hpp>
#include <RapidXmlUtils/XmlDomElement.hpp>

//	CODE NOTE: Needed by the .cpp only.
#include <Utility/C_StringSupport.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to some .hpp file...
const std::size_t MaxTextLength_FixDataType     = 15;
const std::size_t MaxTextLength_FixDataTypeBase = 24;

const std::size_t MaxTextLength_FixVersion      = 15;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
enum VFixDataType {
	VFixDataType_Int               =  0,
	VFixDataType_IntNonNegative    =  1,//	>= 0
	VFixDataType_IntPositive       =  2,//	>  0
	VFixDataType_IntLength         =  3,
	VFixDataType_IntTagNum         =  4,//	>  0, no leading zeroes in string
	VFixDataType_IntSeqNum         =  5,//	>  0
	VFixDataType_IntNumInGroup     =  6,//	>= 0
	VFixDataType_IntDayOfMonth     =  7,//	>= 1 <= 12
	VFIXDataType_Float             =  8,
	VFixDataType_FloatDecimal      =  9,
	VFIXDataType_FloatQty          = 10,
	VFIXDataType_FloatPrice        = 11,
	VFIXDataType_FloatPriceOffset  = 12,
	VFIXDataType_FloatAmt          = 13,
	VFIXDataType_FloatPercentage   = 14,	
	VFIXDataType_Char              = 15,
	VFIXDataType_CharBoolean       = 16,//	'Y' or 'N'
	VFIXDataType_CharMultiValue    = 17,//	Single-char fields seperated by space.
	VFixDataType_String            = 18,
	VFixDataType_StringLanguage    = 19,
	VFixDataType_StringRawData     = 20,
	VFixDataType_StringTenor       = 21,//	{D|M|W|Y}x (x > 0)
	VFixDataType_StringXmlData     = 22,
	VFIXDataType_StringMultiValue  = 23,//	Multi-char fields seperated by space.	
	VFixDataType_StringIsoCountry  = 24,//	ISO 3166 country code
	VFixDataType_StringIsoCurrency = 25,//	ISO 4217 currecny code
	VFixDataType_StringIsoExchange = 26,//	ISO 10383 market identifier code
	VFixDataType_MonthYear         = 27,//	yyyymm[dd|ww]
	VFIXDataType_UtcTimestamp      = 28,//	yyyy-mm-ddThh:mm:ss[.xxx]
	VFIXDataType_UtcTimeOnly       = 29,//	hh:mm:ss[.xxx]
	VFIXDataType_UtcDateOnly       = 30,//	yyyy-mm-dd
	VFIXDataType_LocalMktDate      = 31,//	ISO 8601 (yyyy-mm-dd local-time)
	VFIXDataType_TzTimeOnly        = 32,//	hh:mm[:ss][Z|OFF[+|-hh[:mm]]]
	VFIXDataType_TzTimestamp       = 33,//	yyyymmdd-hh:mm:ss[Z|OFF[+|-hh[:mm]]]
	VFixDataType_Date              = 34,
	VFixDataType_DateTime          = 35,
	VFixDataType_Time              = 36,
	VFixDataType_ReservedPlus100   = 37,//	Values > 100 are reserved.
	VFixDataType_ReservedPlus1000  = 38,//	Values > 1000 are reserved.
	VFixDataType_ReservedPlus4000  = 39,//	Values > 4000 are reserved.
	VFixDataType_None              = 40,
	VFixDataType_Minimum           =  VFixDataType_Int,
	VFixDataType_Maximum           =  VFixDataType_None,
	VFixDataType_Count             = 41
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to some .hpp file...
const char *CopyToString(std::size_t src_len, const char *src_str,
	std::size_t dst_len, char *dst_str, bool can_truncate_flag = false);
const char *CopyToString(const char *src_str, std::size_t dst_len,
	char *dst_str, bool can_truncate_flag = false);
const char *CopyToString(const std::string &src_str, std::size_t dst_len,
	char *dst_ptr, bool can_truncate_flag = false);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFixDataType MapStringToVFixDataType(const char *src);
VFixDataType MapStringToVFixDataType(const std::string &src);
VFixDataType MapStringToVFixDataType(const char *src_1, const char *src_2);
VFixDataType MapStringToVFixDataType(const std::string &src_1,
	const std::string &src_2);

const char *VFixDataTypeToString(VFixDataType src);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to FixDataType.hpp.
class FixDataType {
public:
	typedef std::set<FixDataType> FixDataTypeSet_I;

	//	Constructor used for searches...
	explicit FixDataType(const char *name = "");

	explicit FixDataType(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const FixDataType &other) const
	{
		return(name_ < other.name_);
	}

	void swap(FixDataType &other);

	friend std::ostream & operator << (std::ostream &o_str,
		const FixDataType &datum);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static FixDataTypeSet_I &GetDataFromXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		FixDataTypeSet_I &out_set);

	static FixDataTypeSet_I &GetDataFromXmlFile(const std::string &file_name,
		FixDataTypeSet_I &out_set);
	static FixDataTypeSet_I  GetDataFromXmlFile(const std::string &file_name);

	std::string  name_;
	std::string  base_name_1_;
	std::string  base_name_2_;
	std::string  fix_version_added_;
	std::string  description_;
	VFixDataType vfix_data_type_;

private:
	FixDataType(const std::string &name, const std::string &base_name_1,
		const std::string &base_name_2, const std::string &fix_version_added,
		const std::string &description, VFixDataType vfix_data_type);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<FixDataType>          FixDataTypeVector;
typedef FixDataTypeVector::iterator       FixDataTypeVectorIter;
typedef FixDataTypeVector::const_iterator FixDataTypeVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<FixDataType>               FixDataTypeSet;
typedef FixDataTypeSet::iterator            FixDataTypeSetIter;
typedef FixDataTypeSet::const_iterator      FixDataTypeSetIterC;
typedef std::pair<FixDataTypeSetIter, bool> FixDataTypeSetInsertPair;
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to some .cpp file...
const char *CopyToString(std::size_t src_len, const char *src_str,
	std::size_t dst_len, char *dst_str, bool can_truncate_flag)
{
	if (src_len <= dst_len)
		::strcpy(dst_str, src_str);
	else if (!can_truncate_flag){
		std::ostringstream o_str;
		o_str << "Unable to copy a source string of length " << src_len <<
			" because it exceeds the maximum destination string length of " <<
			dst_len << ".";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
	else
		MLB::Utility::nstrcpy(dst_str, src_str, dst_len);

	return(dst_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *CopyToString(const char *src_str, std::size_t dst_len,
	char *dst_str, bool can_truncate_flag)
{
	return(CopyToString(::strlen(src_str), src_str, dst_len, dst_str,
		can_truncate_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *CopyToString(const std::string &src_str, std::size_t dst_len,
	char *dst_str, bool can_truncate_flag)
{
	return(CopyToString(src_str.size(), src_str.c_str(), dst_len, dst_str,
		can_truncate_flag));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

namespace {

// ////////////////////////////////////////////////////////////////////////////
const char *DataTypeStringFixToVFixList[VFixDataType_Count][2] = {
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
	{ "MultipleCharValue"     , "CharMultiValue"    },
	{ "String"                , "String"            },
	{ "Language"              , "StringLanguage"    },
	{ "data"                  , "StringRawData"     },
	{ "Tenor"                 , "StringTenor"       },
	{ "XMLData"               , "StringXmlData"     },
	{ "MultipleStringValue"   , "StringMultiValue"  },
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
VFixDataType MapStringToVFixDataType(const char *src, bool throw_on_error)
{
	if (src && *src) {
		for (int count_1 = 0; count_1 < VFixDataType_Count; ++count_1) {
			if (!MLB::Utility::Utility_stricmp(src,
				DataTypeStringFixToVFixList[count_1][0]))
				return(static_cast<VFixDataType>(count_1));
		}
		//	Look-aside...
		if (!MLB::Utility::Utility_stricmp(src, "xs:integer"))
			return(VFixDataType_Int);
		else if (!MLB::Utility::Utility_stricmp(src, "xs:string"))
			return(VFixDataType_String);
/*
	Can't resolve these uniquely.
		else if (!MLB::Utility::Utility_stricmp(src, "xs:date"))
		else if (!MLB::Utility::Utility_stricmp(src, "xs:time"))
		else if (!MLB::Utility::Utility_stricmp(src, "xs:dateTime"))
*/
		else if (!MLB::Utility::Utility_stricmp(src, "xs:language"))
			return(VFixDataType_StringLanguage);
		else if (throw_on_error) {
			std::ostringstream o_str;
			o_str << "Unable to map the string '" << src << "' to a VFix "
				"data type enumeration.";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
	}
	else if (throw_on_error)
		MLB::Utility::ThrowInvalidArgument("No data type name specified.");


	return(VFixDataType_None);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFixDataType MapStringToVFixDataType(const std::string &src,
	bool throw_on_error)
{
	return(MapStringToVFixDataType(src.c_str(), throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFixDataType MapStringToVFixDataType(const char *src_1, const char *src_2,
	bool throw_on_error)
{
	if (((!src_1) || (!(*src_1))) && ((!src_2) || (!(*src_2))))
		return(MapStringToVFixDataType(NULL, throw_on_error));
	else if ((src_1 && *src_1) && ((!src_2) || (!(*src_2))))
		return(MapStringToVFixDataType(src_1, throw_on_error));
	else if (((!src_1) || (!(*src_1))) && (src_2 || (*src_2)))
		return(MapStringToVFixDataType(src_2, throw_on_error));

	VFixDataType return_code = MapStringToVFixDataType(src_2, false);

	return((return_code != VFixDataType_None) ? return_code :
		MapStringToVFixDataType(src_1, throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VFixDataType MapStringToVFixDataType(const std::string &src_1,
	const std::string &src_2, bool throw_on_error)
{
	return(MapStringToVFixDataType(src_1.c_str(), src_2.c_str(),
		throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *VFixDataTypeToString(VFixDataType src)
{
	return(((src >= VFixDataType_Minimum) && (src <= VFixDataType_Maximum)) ?
		DataTypeStringFixToVFixList[src][1] : "*INVALID*");
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
FixDataType::FixDataType(const char *name)
	:name_(name)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FixDataType::FixDataType(const MLB::RapidXmlUtils::XmlDomElement &xml_element)
try {
	using namespace MLB::RapidXmlUtils;

	if ((xml_element.node_type_ != XmlDomElement::NodeType_Element) ||
		(xml_element.element_name_ != "Datatype")) {
		std::ostringstream o_str;
		o_str << "Unexpected XML combination of element name ('" <<
			xml_element.element_name_ << "') and node type (" <<
			xml_element.node_type_ << "), expected element name 'Datatype' and "
			"node type " << XmlDomElement::NodeType_Element << ".";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}		
		
	const std::string   &added(xml_element.GetAttributeValue("added", true));
	const std::string   &name(xml_element.GetChildRef("Name").
		GetNodeTextFromChildRef());
	const std::string   &description(xml_element.GetChildRef("Description").
		GetNodeTextFromChildRef());
	const XmlDomElement *base_info_ptr_1(xml_element.GetChildPtr("BaseType"));
	const std::string   *base_name_ptr_1 = NULL;
	const std::string   *base_name_ptr_2 = NULL;


	if (base_info_ptr_1) {
		base_name_ptr_1 = &base_info_ptr_1->GetNodeTextFromChildRef();
		if (xml_element.GetChildPtr("XML") &&
			xml_element.GetChildRef("XML").GetChildPtr("Base"))
			base_name_ptr_2 = &xml_element.GetChildRef("XML").GetChildRef("Base").
				GetNodeTextFromChildRef();
	}
	else {
		try {
			base_name_ptr_2 = &xml_element.GetChildRef("XML").GetChildRef("Base").
				GetNodeTextFromChildRef();
		}
		catch (const std::exception &except) {
			MLB::Utility::ThrowInvalidArgument("Able to locate the FIX "
				"datatype base name in neither <Datatype><Base> nor "
				"<Datatype><XML><Base>: " + std::string(except.what()));
		}
	}

	FixDataType(name, (base_name_ptr_1) ? *base_name_ptr_1 : "",
		(base_name_ptr_2) ? *base_name_ptr_2 : "", added, description,
		MapStringToVFixDataType(
			(base_name_ptr_1) ? base_name_ptr_1->c_str() : NULL,
			(base_name_ptr_2) ? base_name_ptr_2->c_str() : NULL, true)).
		swap(*this);
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Unable to construct an FixDataType "
		"instance: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FixDataType::FixDataType(const std::string &name, const std::string &base_name_1,
	const std::string &base_name_2, const std::string &fix_version_added,
	const std::string &description, VFixDataType vfix_data_type)
	:name_(name)
	,base_name_1_(base_name_1)
	,base_name_2_(base_name_2)
	,fix_version_added_(fix_version_added)
	,description_(description)
	,vfix_data_type_(vfix_data_type)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FixDataType::swap(FixDataType &other)
{
	std::swap(name_, other.name_);
	std::swap(base_name_1_, other.base_name_1_);
	std::swap(base_name_2_, other.base_name_2_);
	std::swap(fix_version_added_, other.fix_version_added_);
	std::swap(description_, other.description_);
	std::swap(vfix_data_type_, other.vfix_data_type_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool FixDataType::ShouldApplyXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
{
	using namespace MLB::RapidXmlUtils;

	return((xml_element.node_type_ == XmlDomElement::NodeType_Element) &&
		(xml_element.element_name_ == "Datatype") &&
		(xml_element.GetChildRef("Name").GetNodeTextFromChildRef() !=
		"Pattern"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FixDataTypeSet &FixDataType::GetDataFromXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element,
	FixDataTypeSet &out_set)
{
	using namespace MLB::RapidXmlUtils;

	FixDataTypeSet tmp_set;

	try {
		if ((xml_element.node_type_ != XmlDomElement::NodeType_Unknown) ||
			(xml_element.element_name_ != "Datatypes")) {
			std::ostringstream o_str;
			o_str << "Expected the major node in the XML for a FIX datatype to "
				"have the name 'Datatypes' and the node type " <<
				XmlDomElement::NodeType_Unknown << " , but its actual name is '" << 
				xml_element.element_name_ << "' and the node type is " <<
				xml_element.node_type_ << ".";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		for (std::size_t count_1 = 0; count_1 < xml_element.child_list_.size();
			++count_1) {
			const XmlDomElement &this_element(xml_element.child_list_[count_1]);
			if (ShouldApplyXmlElement(this_element)) {
				FixDataType tmp_datum(this_element);
				FixDataTypeSetInsertPair insert_pair(tmp_set.insert(tmp_datum));
				if (!insert_pair.second) {
					std::ostringstream o_str;
					o_str << "Attempt to insert datatype element for datatype "
						"name '" << tmp_datum.name_ << "' failed because it is "
						"already in the set of datatypes.";
					MLB::Utility::ThrowInvalidArgument(o_str.str());
				}
			}
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX datatypes XML data failed: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	out_set.swap(tmp_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FixDataTypeSet &FixDataType::GetDataFromXmlFile(const std::string &file_name,
	FixDataTypeSet &out_set)
{
	MLB::RapidXmlUtils::RapidXmlContext xml_context;
	MLB::RapidXmlUtils::XmlDomElement   xml_element;

	try {
		MLB::RapidXmlUtils::XmlDomElement::ParseXmlFile(file_name,
			xml_element);
		GetDataFromXmlElement(xml_element, out_set);
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX datatypes XML file '" <<
			file_name << "' failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FixDataTypeSet FixDataType::GetDataFromXmlFile(const std::string &file_name)
{
	FixDataTypeSet out_set;

	return(GetDataFromXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const FixDataType &datum)
{
	o_str
		<< std::left
		<< std::setw(23) << datum.name_              << " "
		<< std::setw(15) << datum.base_name_1_       << " "
		<< std::setw(10) << datum.fix_version_added_ << " "
		<< VFixDataTypeToString(datum.vfix_data_type_)
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/StringSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::VFix;

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest(const char *file_name)
{
	FixDataTypeSet      element_set(FixDataType::GetDataFromXmlFile(file_name));
	FixDataTypeSetIterC iter_b(element_set.begin());
	FixDataTypeSetIterC iter_e(element_set.end());

	for ( ; iter_b != iter_e; ++iter_b)
		std::cout << *iter_b << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	using namespace MLB::Utility;

	int return_code = EXIT_SUCCESS;

	try {
		if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
			std::cout << "USAGE: " << std::endl <<
				"   " << argv[0] << " <xml-file-name> [ <xml-file-name> ... ]" <<
				std::endl << std::endl;
			return(EXIT_SUCCESS);
		}
		else if (argc < 2)
			ThrowInvalidArgument("No XML files were specified.");
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			TEST_RunTest(argv[count_1]);
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

/*
	Testing command line for Linux:
	------- ------- ---- --- ------
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\Datatypes.xml > TestResults.InitDatatypesXml.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	XML/FIX.5.0SP2/Base/Datatypes.xml > TestResults.InitDatatypesXml.txt
*/

#endif // #ifdef TEST_MAIN

