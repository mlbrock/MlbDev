// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixFieldType.cpp

	File Description	:	Implementation of the PFixFieldType class.

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
const std::size_t MaxTextLength_PFixFieldType     = 15;
const std::size_t MaxTextLength_PFixFieldTypeBase = 24;

const std::size_t MaxTextLength_FixVersion      = 15;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.hpp.
enum VFixXPortType {
	VFixXPortType_Int,
	VFixXPortType_IntNonNegative,		//	>= 0
	VFixXPortType_IntPositive,			//	>  0
	VFixXPortType_IntLength,
	VFixXPortType_IntTagNum,			//	>  0, no leading zeroes in string
	VFixXPortType_IntSeqNum,			//	>  0
	VFixXPortType_IntNumInGroup,		//	>= 0
	VFixXPortType_IntDayOfMonth,		//	>= 1 <= 12
	VFixXPortType_Float,
	VFixXPortType_FloatDecimal,
	VFixXPortType_FloatQty,
	VFixXPortType_FloatPrice,
	VFixXPortType_FloatPriceOffset,
	VFixXPortType_FloatAmt,
	VFixXPortType_FloatPercentage,	
	VFixXPortType_Char,
	VFixXPortType_CharBoolean,			//	'Y' or 'N'
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
	bool throw_on_error = true);
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
//	CODE NOTE: Move to PFixFieldType.hpp.
class PFixFieldType {
public:
	typedef std::set<PFixFieldType> PFixFieldTypeSet_I;

	//	Constructor used for searches...
	explicit PFixFieldType(const char *name = "");

	explicit PFixFieldType(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixFieldType &other) const
	{
		return(name_ < other.name_);
	}

	void swap(PFixFieldType &other);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixFieldType &datum);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixFieldTypeSet_I &LoadFromXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixFieldTypeSet_I &out_set);

	static PFixFieldTypeSet_I &LoadFromXmlFile(const std::string &file_name,
		PFixFieldTypeSet_I &out_set);
	static PFixFieldTypeSet_I  LoadFromXmlFile(const std::string &file_name);

	std::string   name_;
	std::string   base_name_1_;
	std::string   base_name_2_;
	std::string   fix_version_added_;
	std::string   description_;
	VFixXPortType vfix_xport_type_;

private:
	PFixFieldType(const std::string &name, const std::string &base_name_1,
		const std::string &base_name_2, const std::string &fix_version_added,
		const std::string &description, VFixXPortType vfix_xport_type);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<PFixFieldType>          PFixFieldTypeVector;
typedef PFixFieldTypeVector::iterator       PFixFieldTypeVectorIter;
typedef PFixFieldTypeVector::const_iterator PFixFieldTypeVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef PFixFieldType::PFixFieldTypeSet_I     PFixFieldTypeSet;
typedef PFixFieldTypeSet::iterator            PFixFieldTypeSetIter;
typedef PFixFieldTypeSet::const_iterator      PFixFieldTypeSetIterC;
typedef std::pair<PFixFieldTypeSetIter, bool> PFixFieldTypeSetInsertPair;
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
//	CODE NOTE: Move to VFixXPortType.cpp.
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
//	CODE NOTE: Move to VFixXPortType.cpp.
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
//	CODE NOTE: Move to VFixXPortType.cpp.
VFixXPortType StringToVFixXPortType(const std::string &src,
	bool throw_on_error)
{
	return(StringToVFixXPortType(src.c_str(), throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.cpp.
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
//	CODE NOTE: Move to VFixXPortType.cpp.
VFixXPortType StringToVFixXPortType(const std::string &src_1,
	const std::string &src_2, bool throw_on_error)
{
	return(StringToVFixXPortType(src_1.c_str(), src_2.c_str(),
		throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.cpp.
const char *VFixXPortTypeToString(VFixXPortType src, bool throw_on_error)
{
	if ((src < VFixXPortType_Minimum) || (src > VFixXPortType_Maximum)) {
		if (throw_on_error) {
			std::ostringstream o_str;
			o_str << "Invalid VFixXPortType enumeration value (" << src << ").";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		return("*INVALID*");
	}

	return(DataTypeStringFixToVFixList[src][1]);
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
PFixFieldType::PFixFieldType(const char *name)
	:name_(name)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldType::PFixFieldType(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
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

	PFixFieldType(name, (base_name_ptr_1) ? *base_name_ptr_1 : "",
		(base_name_ptr_2) ? *base_name_ptr_2 : "", added, description,
		StringToVFixXPortType(
			(base_name_ptr_1) ? base_name_ptr_1->c_str() : NULL,
			(base_name_ptr_2) ? base_name_ptr_2->c_str() : NULL, true)).
		swap(*this);
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Unable to construct an PFixFieldType "
		"instance: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldType::PFixFieldType(const std::string &name,
	const std::string &base_name_1, const std::string &base_name_2,
	const std::string &fix_version_added, const std::string &description,
	VFixXPortType vfix_xport_type)
	:name_(name)
	,base_name_1_(base_name_1)
	,base_name_2_(base_name_2)
	,fix_version_added_(fix_version_added)
	,description_(description)
	,vfix_xport_type_(vfix_xport_type)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixFieldType::swap(PFixFieldType &other)
{
	std::swap(name_, other.name_);
	std::swap(base_name_1_, other.base_name_1_);
	std::swap(base_name_2_, other.base_name_2_);
	std::swap(fix_version_added_, other.fix_version_added_);
	std::swap(description_, other.description_);
	std::swap(vfix_xport_type_, other.vfix_xport_type_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PFixFieldType::ShouldApplyXmlElement(
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
PFixFieldTypeSet &PFixFieldType::LoadFromXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element,
	PFixFieldTypeSet &out_set)
{
	using namespace MLB::RapidXmlUtils;

	PFixFieldTypeSet tmp_set;

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
				PFixFieldType tmp_datum(this_element);
				PFixFieldTypeSetInsertPair insert_pair(tmp_set.insert(tmp_datum));
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
PFixFieldTypeSet &PFixFieldType::LoadFromXmlFile(const std::string &file_name,
	PFixFieldTypeSet &out_set)
{
	MLB::RapidXmlUtils::RapidXmlContext xml_context;
	MLB::RapidXmlUtils::XmlDomElement   xml_element;

	try {
		MLB::RapidXmlUtils::XmlDomElement::ParseXmlFile(file_name,
			xml_element);
		LoadFromXmlElement(xml_element, out_set);
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
PFixFieldTypeSet PFixFieldType::LoadFromXmlFile(const std::string &file_name)
{
	PFixFieldTypeSet out_set;

	return(LoadFromXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixFieldType &datum)
{
	o_str
		<< std::left
		<< std::setw(23) << datum.name_              << " "
		<< std::setw( 7) << datum.base_name_1_       << " "
		<< std::setw(23) << datum.base_name_2_       << " "
		<< std::setw(10) << datum.fix_version_added_ << " "
		<< std::right
		<< std::setw( 2) << datum.vfix_xport_type_   << " "
		<< VFixXPortTypeToString(datum.vfix_xport_type_)
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
	PFixFieldTypeSet      element_set(PFixFieldType::LoadFromXmlFile(file_name));
	PFixFieldTypeSetIterC iter_b(element_set.begin());
	PFixFieldTypeSetIterC iter_e(element_set.end());

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

