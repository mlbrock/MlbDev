// ////////////////////////////////////////////////////////////////////////////

#include <mbtypes.h>

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
 - messageSchema
	 -	byteOrder
	 -	description
	 -	id
	 -	package  															
	 -	semanticVersion
	 -	version
	 - message
		 - blockLength
		 - description
		 - id
		 - name
		 - semanticType
		 - data
			 - description
			 - id
			 - name
			 - semanticType
			 - sinceVersion
			 - type
		 - field
			 - description
			 - id
			 - name
			 - offset
			 -	presence
			 - semanticType
			 - sinceVersion
			 - type
		 - group
			 - blockLength
			 - description
			 - dimensionType
			 - id
			 - name
	 - types
		 - type
			 -	name
			 -	presence
			 -	semanticType
			 -	description
			 -	primitiveType
			 -	length
			 -	characterEncoding
		 - composite
			 - description
			 - name
			 - semanticType
			 - sinceVersion
		 - enum
			 - description
			 - encodingType
			 - name
			 - presence
			 - semanticType
		 - set
			 - description
			 - encodingType
			 - name
			 - presence
			 - semanticType
*/
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef const std::pair<const char *, std::size_t> BuiltInType_I;

const BuiltInType_I BuiltInTypeList[] = {
	 BuiltInType_I("NONE",	0)
	,BuiltInType_I("char",	sizeof(char))
	,BuiltInType_I("int8",	sizeof(Native_SInt8))
	,BuiltInType_I("int16",	sizeof(Native_SInt16))
	,BuiltInType_I("int32",	sizeof(Native_SInt32))
	,BuiltInType_I("int64",	sizeof(Native_SInt64))
	,BuiltInType_I("uint8",	sizeof(Native_UInt8))
	,BuiltInType_I("uint16",sizeof(Native_UInt16))
	,BuiltInType_I("uint32",sizeof(Native_UInt32))
	,BuiltInType_I("uint64",sizeof(Native_UInt64))
	,BuiltInType_I("float",	sizeof(Native_Float32))
	,BuiltInType_I("double",sizeof(Native_Float64))
};

const std::size_t   BuiltInTypeCount  =
	sizeof(BuiltInTypeList) / sizeof(BuiltInTypeList[0]);
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
class DatumType {
public:
	typedef std::set<DatumType> DatumTypeSet_I;

	enum PrimitiveType {
		Type_NONE,
		Type_char,
		Type_int8,
		Type_int16,
		Type_int32,
		Type_int64,
		Type_uint8,
		Type_uint16,
		Type_uint32,
		Type_uint64,
		Type_float,
		Type_double
	};

	enum Presence {
		Presence_Constant,
		Presence_Optional,
		Presence_Required
	};

	bool operator < (const DatumType &lhs, const DatumType &rhs);

	const std::string &GetName() const;

	const std::string &GetDescription() const;

	inline std::size_t GetLength() const
	{
		return(length_);
	}

	inline std::size_t GetOffsetNominal() const
	{
		return(offset_nominal_);
	}

	inline std::size_t GetOffsetActual() const
	{
		return(offset_actual_);
	}

	inline Native_UInt32 GetSinceVersion() const
	{
		return(since_version_);
	}

	inline Presence GetPresence() const
	{
		return(presence_);
	}

	static DatumTypeSet_I GetPrimitiveTypeSet();

private:
	std::string   name_;
	std::string   description_;
	std::size_t   length_;
	std::size_t   offset_nominal_;
	std::size_t   offset_actual_;
	Native_SInt32 since_version_;
	Presence      presence_;
	int           flags_;
	Native_UInt16 id_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef DatumType::DatumTypeSet_I DatumTypeSet;
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
class InsItem {
public:
	typedef std::set<InsItem>    InsItemSet_I;
	typedef std::vector<InsItem> InsItemVec_I;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef InsItem::InsItemSet_I InsItemSet;
typedef InsItem::InsItemVec_I InsItemVec;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class MessageSchema {
public:
	static MessageSchema &ParseXmlFile(const std::string &file_name,
		MessageSchema &out_schema);
	static MessageSchema  ParseXmlFile(const std::string &file_name);

private:
	std::string   package;
	std::string   semanticVersion;
	std::string   description;
	std::string   headerType;
	Native_UInt32 id;
	Native_SInt32 version;
	bool          byteOrder;

	static MessageSchema &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		MessageSchema &out_schema);
};
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
std::string ParseXmlAttributeValue(
	const MLB::RapidXmlUtils::XmlDomAttribute &attr_info)
{
	std::string dst;

	try {
		if ((attr_info.second.size() < 2) ||
			(attr_info.second[0] != '\"') ||
			(attr_info.second.rbegin()[0] != '\"'))
			MLB::Utility::ThrowStdException("Expected attribute value to begin "
				"and end with a double-quote ('\"').");
		if (attr_info.second.size() > 2)
			dst = attr_info.second.substr(1, attr_info.second.size() - 2);
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Unable to extract attribute '" << attr_info.first <<
			"' value: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(dst);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> DataType &ParseXmlAttribute(
	const MLB::RapidXmlUtils::XmlDomElement &src_xml_element,
	const std::string &attribute_name, DataType &dst,
	const DataType *default_value)
{
	using namespace MLB::RapidXmlUtils;

	try {
		XmlDomAttribute attr_info(GetAttributeValue(src_xml_element,
			attribute_name, (default_value) ? false : true));
		if (!attr_info.first.empty())
			MLB::Utility::ParseCmdLineArg::ParseCmdLineDatum(
				ParseXmlAttributeValue(attr_info), dst);
		else
			dst = *default_value;
	}
	catch (const std::string &except) {
		std::ostringstream o_str;
		o_str << "Unable to get attribute '" << attribute_name << "' value from "
			"element '" << src_xml_element.element_name_ << "': " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(dst);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> DataType &ParseXmlAttribute(
	const MLB::RapidXmlUtils::XmlDomElement &src_xml_element,
	const std::string &attribute_name, DataType &dst,
	const DataType &default_value)
{
	return(ParseXmlAttribute(src_xml_element, dst, attribute_name, dst,
		&default_value));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> DataType &ParseXmlAttribute(
	const MLB::RapidXmlUtils::XmlDomElement &src_xml_element,
	const std::string &attribute_name, DataType &dst)
{
	return(ParseXmlAttribute(src_xml_element, dst, attribute_name, dst, NULL));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> DataType ParseXmlAttribute(
	const MLB::RapidXmlUtils::XmlDomElement &src_xml_element,
	const std::string &attribute_name)
{
	DataType dst;

	return(ParseXmlAttribute(src_xml_element, attribute_name, dst));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MessageSchema &MessageSchema::ParseXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element,
	MessageSchema &out_schema)
{
	using namespace MLB::RapidXmlUtils;

	MessageSchema tmp_schema;

	try {
		if ((xml_element.node_type_ != XmlDomElement::NodeType_Unknown) ||
			(xml_element.element_name_ != "messageSchema")) {
			std::ostringstream o_str;
			o_str << "Expected the major node in XML input for SBE to have the "
				"name 'messageSchema' and the node type " <<
				XmlDomElement::NodeType_Unknown << " , but its actual name is '" << 
				xml_element.element_name_ << "' and the node type is " <<
				xml_element.node_type_ << ".";
			MLB::Utility::ThrowLogicError(o_str.str());
		}

		ParseXmlAttribute(src_xml_element, "package",         package,         "");
		ParseXmlAttribute(src_xml_element, "id",              id);
		ParseXmlAttribute(src_xml_element, "version",         version);
		ParseXmlAttribute(src_xml_element, "semanticVersion", semanticVersion, "");
		ParseXmlAttribute(src_xml_element, "byteOrder",       byteOrder,       "littleEndian");
		ParseXmlAttribute(src_xml_element, "description",     description,     "");
		ParseXmlAttribute(src_xml_element, "headerType",      headerType,      "messageHeader");

		for (std::size_t count_1 = 0; count_1 < xml_element.child_list_.size();
			++count_1) {
			const XmlDomElement &this_element(xml_element.child_list_[count_1]);
			try {
				if (this_element.node_type_ != XmlDomElement::NodeType_Element) {
					std::ostringstream o_str;
					o_str << "Expected element node type 'XmlDomElement::"
						"NodeType_Element' (" << XmlDomElement::NodeType_Element <<
						").";
					MLB::Utility::ThrowLogicError(o_str.str());
				}
				else if (xml_element.element_name_ == "types") {
std::cout << "ELEMENT: types\n";
				}
				else if (xml_element.element_name_ == "message") {
std::cout << "ELEMENT: message\n";
				}
				else
					MLB::Utility::ThrowLogicError("Expected element name 'types' "
						"or 'message'.");
			}
			catch (const std::exception &except) {
				std::ostringstream o_str;
				o_str << "Unable to parse element index " << count_1 <<
					", name '" << this_element.element_name_ << "', node type " <<
					this_element.node_type_ << ": " << except.what();
				MLB::Utility::Rethrow(except, o_str.str());
			}
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the XML messageSchema element failed: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	out_schema.swap(tmp_schema);

	return(out_schema);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MessageSchema &MessageSchema::ParseXmlFile(const std::string &file_name,
	MessageSchema &out_schema)
{
	MLB::RapidXmlUtils::RapidXmlContext xml_context;
	MLB::RapidXmlUtils::XmlDomElement   xml_element;

	try {
		MLB::RapidXmlUtils::XmlDomElement::ParseXmlFile(file_name, xml_element);
		ParseXmlElement(xml_element, out_schema);
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the SBE XML file '" << file_name <<
			"' failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(out_schema);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MessageSchema MessageSchema::ParseXmlFile(const std::string &file_name)
{
	MessageSchema out_schema;

	return(ParseXmlFile(file_name, out_schema));
}
// ////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest(const char *file_name, int &return_code)
{
	try {
		std::cout << "File '" << file_name << "': " << std::flush;
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cout << "ERROR: " << except.what() << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		for (int count_1 = 1; count_1 < argc; ++count_1)
			TEST_RunTest(argv[count_1], return_code);
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cerr << "ERROR: " << except.what() << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////


#endif // #ifdef TEST_MAIN

