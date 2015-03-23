// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB RapidXmlUtils Utilities Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	XmlDomElement.cpp

	File Description	:	Implementation of the XmlDomElement class.

	Revision History	:	2015-03-21 --- Creation.
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

#include <RapidXmlUtils/XmlDomElement.hpp>

#include <Utility/StringSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>

#include <boost/shared_ptr.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RapidXmlUtils {

// ////////////////////////////////////////////////////////////////////////////
XmlDomElement::XmlDomElement()
	:node_type_(NodeType_Unknown)
	,element_name_()
	,node_text_()
	,child_list_()
	,attribute_map_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XmlDomElement::XmlDomElement(
	boost::property_tree::detail::rapidxml::xml_node<> *element_ptr)
	:element_name_()
	,child_list_()
	,attribute_map_()
{
	using namespace boost::property_tree::detail;

	if (element_ptr == NULL)
		MLB::Utility::ThrowLogicError("Invalid element pointer passed to "
			"'XmlDomElement' constructor. Does the input XML file exist?");

	rapidxml::node_type xerces_node_type = element_ptr->type();

	const char *xerces_element_name = NULL;

	switch (xerces_node_type) {
		case rapidxml::node_element		:
			//	Xerces = xercesc::DOMNode::ELEMENT_NODE
			node_type_ = NodeType_Element;
			break;
/*
		case xercesc::DOMNode::ATTRIBUTE_NODE					:
			node_type_ = NodeType_Attribute;
			break;
*/
/*
		case xercesc::DOMNode::TEXT_NODE							:
			node_type_ = NodeType_Text;
			break;
*/
		case rapidxml::node_data			:
			//	EXPERIMENTAL: Xerces = xercesc::DOMNode::TEXT_NODE
			xerces_element_name = "#text";
			node_type_          = NodeType_Text;
			break;
		case rapidxml::node_cdata			:
			//	Xerces = xercesc::DOMNode::CDATA_SECTION_NODE
			node_type_ = NodeType_CData;
			break;
/*
		case xercesc::DOMNode::ENTITY_REFERENCE_NODE			:
			node_type_ = NodeType_EntityRef;
			break;
		case xercesc::DOMNode::ENTITY_NODE						:
			node_type_ = NodeType_Entity;
			break;
*/
		case rapidxml::node_pi				:
			//	Xerces = xercesc::DOMNode::PROCESSING_INSTRUCTION_NODE
			node_type_ = NodeType_ProcIns;
			break;
		case rapidxml::node_comment		:
			//	Xerces = xercesc::DOMNode::COMMENT_NODE
			xerces_element_name = "#comment";
			node_type_          = NodeType_Comment;
			break;
		case rapidxml::node_document		:
			//	Xerces = xercesc::DOMNode::DOCUMENT_NODE
			node_type_ = NodeType_Doc;
			break;
		case rapidxml::node_doctype		:
			//	Xerces = xercesc::DOMNode::DOCUMENT_TYPE_NODE
			node_type_ = NodeType_DocType;
			break;
/*
		case xercesc::DOMNode::DOCUMENT_FRAGMENT_NODE		:
			node_type_ = NodeType_DocFrag;
			break;
		case xercesc::DOMNode::NOTATION_NODE					:
			node_type_ = NodeType_Notation;
			break;
*/
//		case rapidxml::node_data			:
		case rapidxml::node_declaration	:
			{
				std::ostringstream o_str;
				o_str << "Unhandled rapidxml::node_type enumeration value (" <<
					xerces_node_type << ").";
				throw std::logic_error(o_str.str());
			}
			break;
		default																:
			node_type_ = NodeType_Unknown;
			break;
	}

	element_name_ = MLB::RapidXmlUtils::XmlStringToString(
		element_ptr->name());

	if (xerces_element_name != NULL) {
		if (!element_name_.empty()) {
			std::ostringstream o_str;
			o_str << "Node type is " << node_type_ << " uses an imputed Xerces "
				"element name ('" << xerces_element_name << "'), but the node "
				"name is not empty ('" << element_name_ << "').";
			throw std::logic_error(o_str.str());
		}
		element_name_ = xerces_element_name;	//	Duplicate Xerces element name.
	}


	//	Get the list of children...
	rapidxml::xml_node<> *child_ptr = element_ptr->first_node();
	if (child_ptr) {
		while (child_ptr) {
			child_list_.push_back(XmlDomElement(child_ptr));
			child_ptr = child_ptr->next_sibling();
		}
	}
	else {
		const char *tmp_node_text = element_ptr->value();
		if (tmp_node_text != NULL)
			node_text_ = MLB::RapidXmlUtils::XmlStringToString(tmp_node_text);
	}

	//	Get the list of attributes...
	rapidxml::xml_attribute<> *attr_ptr = element_ptr->first_attribute();
	if (attr_ptr) {
		while (attr_ptr) {
			attribute_map_[attr_ptr->name()] = attr_ptr->value();
			attr_ptr = attr_ptr->next_attribute();
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
void XmlDomElement::swap(XmlDomElement &other)
{
	element_name_.swap(other.element_name_);
	child_list_.swap(other.child_list_);
	attribute_map_.swap(other.attribute_map_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool XmlDomElement::IsElement(const std::string &element_name) const
{
	return(MLB::Utility::Utility_stricmp(element_name.c_str(),
		element_name_.c_str()) == 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::string &XmlDomElement::GetNodeTextRef() const
{
	return(node_text_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string XmlDomElement::GetNodeTextNormal() const
{
	std::string out_string;

	return(GetNodeTextNormal(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &XmlDomElement::GetNodeTextNormal(std::string &out_string) const
{
	return(MLB::Utility::TrimString(GetNodeTextRef(), out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool XmlDomElement::HasNodeTextFromChild() const
{
	return((!child_list_.empty()) &&
		(child_list_.front().node_type_ == XmlDomElement::NodeType_Text));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::string &XmlDomElement::GetNodeTextFromChildRef() const
{
	if (!HasNodeTextFromChild())
		MLB::Utility::ThrowLogicError("Invocation of 'XmlDomElement::"
			"GetNodeTextFromChildRef()' failed as the child node list does not "
			"meet the required criteria.");

	return(child_list_.front().node_text_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string XmlDomElement::GetNodeTextFromChildNormal() const
{
	std::string out_string;

	return(GetNodeTextFromChildNormal(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &XmlDomElement::GetNodeTextFromChildNormal(
	std::string &out_string) const
{
	return(MLB::Utility::TrimString(GetNodeTextFromChildRef(), out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool XmlDomElement::HasChild(const std::string &child_name) const
{
	return(GetChildPtr(child_name) != NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const XmlDomElement &XmlDomElement::GetChildRef(const std::string &child_name)
	const
{
	const XmlDomElement *child_ptr = GetChildPtr(child_name);

	if (child_ptr == NULL)
		ThrowChildElementNotFound(child_name);

	return(*child_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const XmlDomElement *XmlDomElement::GetChildPtr(const std::string &child_name)
	const
{
	XmlDomElementListI::const_iterator iter_b(child_list_.begin());
	XmlDomElementListI::const_iterator iter_e(child_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->IsElement(child_name))
			return(&(*iter_b));
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int XmlDomElement::CountChildren(const std::string &child_name) const
{
	unsigned int child_count = 0;

	XmlDomElementListI::const_iterator iter_b(child_list_.begin());
	XmlDomElementListI::const_iterator iter_e(child_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->IsElement(child_name))
			++child_count;
	}

	return(child_count);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool XmlDomElement::HasAttribute(const std::string &attribute_name) const
{
	return(attribute_map_.find(attribute_name) != attribute_map_.end());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XmlDomAttribute XmlDomElement::GetAttribute(const std::string &attribute_name,
	bool throw_on_error) const
{
	XmlDomElementAttrMapIterC iter_f(attribute_map_.find(attribute_name));

	if (iter_f == attribute_map_.end()) {
		if (throw_on_error)
			ThrowAttributeNotFound(attribute_name);
		return(XmlDomAttribute());
	}

	return(*iter_f);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string XmlDomElement::GetAttributeValue(const std::string &attribute_name,
	bool throw_on_error) const
{
	return(GetAttribute(attribute_name, throw_on_error).second);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XmlDomElement XmlDomElement::ParseXmlString(const std::string &xml_string,
	bool destructive_xml_parse)
{
	XmlDomElement xml_element;

	return(ParseXmlString(xml_string, xml_element, destructive_xml_parse));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XmlDomElement &XmlDomElement::ParseXmlString(const std::string &xml_string,
	XmlDomElement &xml_element, bool destructive_xml_parse)
{
	return(ParseXmlString(xml_string.c_str(), xml_element,
		destructive_xml_parse));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XmlDomElement XmlDomElement::ParseXmlString(const char *xml_string,
	bool destructive_xml_parse)
{
	XmlDomElement xml_element;

	return(ParseXmlString(xml_string, xml_element, destructive_xml_parse));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XmlDomElement &XmlDomElement::ParseXmlString(const char *xml_string,
	XmlDomElement &xml_element, bool destructive_xml_parse)
{
	using namespace boost::property_tree::detail;

	try {
		if (xml_string == NULL)
			MLB::Utility::ThrowInvalidArgument("The specified XML string pointer "
				"is 'NULL'.");
		if (!(*xml_string))
			MLB::Utility::ThrowInvalidArgument("The specified XML string is "
				"empty.");
# pragma warning(disable:4625 4626)
#if defined(_MSC_VER) && (_MSC_VER >= 1000)
# pragma warning(push)
# pragma warning(disable:4625 4626)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1300)
		rapidxml::xml_document<> xml_document;
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
# pragma warning(pop)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1300)
		char                    *tmp_xml_string;
		boost::shared_ptr<char>  string_sptr;
		if (destructive_xml_parse)
			tmp_xml_string = const_cast<char *>(xml_string);
		else {
			tmp_xml_string = ::strdup(xml_string);
			if (!tmp_xml_string)
				throw std::bad_alloc();
			string_sptr.reset(tmp_xml_string, ::free);
		}
		xml_document.parse<rapidxml::parse_comment_nodes>(tmp_xml_string);
		//	We ignore leading comment blocks (as Xerces does).
		rapidxml::xml_node<> *node_ptr = xml_document.first_node();
		while ((node_ptr->type() == rapidxml::node_comment) &&
			node_ptr->next_sibling())
			node_ptr = node_ptr->next_sibling();
		XmlDomElement(node_ptr).swap(xml_element);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to parse an XML string failed: " +
			std::string(except.what()));
	}

	return(xml_element);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XmlDomElement XmlDomElement::ParseXmlFile(const std::string &file_name)
{
	XmlDomElement xml_element;

	return(ParseXmlFile(file_name, xml_element));

}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XmlDomElement &XmlDomElement::ParseXmlFile(const std::string &file_name,
	XmlDomElement &xml_element)
{
	try {
		if (file_name.empty())
			MLB::Utility::ThrowInvalidArgument("The specified XML file name is "
				"empty.");
		XmlDomElement tmp_xml_element;
		ParseXmlString(MLB::Utility::ReadFileData(file_name),
			tmp_xml_element, true);
		tmp_xml_element.swap(xml_element);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to parse XML file '" +
			file_name + "' failed: " + std::string(except.what()));
	}

	return(xml_element);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void XmlDomElement::ConcatElementTree(std::ostream &o_str,
	unsigned int depth) const
{
	o_str << std::setw(static_cast<std::streamsize>(depth)) << "" <<
		element_name_;

	if (!node_text_.empty())
		o_str << " = {" << GetNodeTextNormal() << "}";

	o_str << std::endl;

	XmlDomElementAttrMapIterC iter_attr_b(attribute_map_.begin());
	XmlDomElementAttrMapIterC iter_attr_e(attribute_map_.end());

	for ( ; iter_attr_b != iter_attr_e; ++iter_attr_b) {
		o_str << std::setw(static_cast<std::streamsize>(depth + 3)) << "" <<
			"[" << iter_attr_b->first << "] = [" << iter_attr_b->second << "]" <<
			std::endl;
	}

	XmlDomElementListIterC iter_b(child_list_.begin());
	XmlDomElementListIterC iter_e(child_list_.end());

	for ( ; iter_b != iter_e; ++iter_b)
		iter_b->ConcatElementTree(o_str, depth + 3);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void XmlDomElement::EmitElementTree(unsigned int depth) const
{
	ConcatElementTree(std::cout, depth);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void XmlDomElement::ThrowChildElementNotFound(
	const std::string &child_name) const
{
	MLB::Utility::ThrowLogicError("Unable to locate child element name '" +
		child_name + "' within element '" + element_name_ + "'.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void XmlDomElement::ThrowAttributeNotFound(const std::string &attribute_name)
	const
{
	MLB::Utility::ThrowLogicError("Unable to locate attribute name '" +
		attribute_name + "' within element '" + element_name_ + ".'");
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RapidXmlUtils

} // namespace MLB

#ifdef TEST_MAIN

#include <RapidXmlUtils/RapidXmlContext.hpp>

#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/InlineContainer.hpp>

using namespace MLB::RapidXmlUtils;

// ////////////////////////////////////////////////////////////////////////////
void TEST_XmlDomElement(int argc, char **argv)
{
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " [ -stress <stress-test-cycle-count> ] " <<
			"[ -dump <boolean> ] <xml-file-name> [ <xml-file-name> ... ]" <<
			std::endl << std::endl;
		return;
	}

	unsigned int             count_1;
	unsigned int             stress_test_count = 1;
	bool                     dump_flag         = false;
	std::vector<std::string> file_list;

	file_list.reserve(static_cast<std::size_t>(argc));

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric
			<std::vector<std::string>, unsigned int>(
			MLB::Utility::MakeInlineVector<std::string>
				("-STRESS")
				("-STRESS_TEST")
				("-STRESSTEST"),
				count_1, argc, argv, stress_test_count, 1))
			;
		else if (MLB::Utility::ParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-DUMP")
				("-DUMP_DATA")
				("-DUMPDATA")
				("-DUMP_TREE")
				("-DUMPTREE"),
				count_1, argc, argv, dump_flag))
			;
		else
			file_list.push_back(argv[count_1]);
	}

	RapidXmlContext rapidxml_context;

/*
	Some internal initial tests...
	{
		const char *xml_string = "<top><middle a=\"1\" b=\"2\" c=\"3\"></middle>"
			"</top>";
		XmlDomElement root_element(XmlDomElement::ParseXmlString(xml_string));
		root_element.EmitElementTree();
	}

	{
		const char *xml_string = "<top><!-- Comment 1 -->"
			"<middle a=\"1\" b=\"2\" c=\"3\"></middle></top>";
		XmlDomElement root_element(XmlDomElement::ParseXmlString(xml_string));
		root_element.EmitElementTree();
	}
*/

	if (file_list.empty())
		MLB::Utility::ThrowInvalidArgument("No XML files were specified.");

	std::cout << "Will parse " <<
		static_cast<unsigned int>(file_list.size()) << " files for " <<
		stress_test_count << " iterations." << std::endl;

	for (count_1 = 0; count_1 < stress_test_count; ++count_1) {
		std::vector<std::string>::const_iterator iter_b(file_list.begin());
		std::vector<std::string>::const_iterator iter_e(file_list.end());
		for ( ; iter_b != iter_e; ++iter_b)
			XmlDomElement root_element(XmlDomElement::ParseXmlFile(*iter_b));
	}

	if (dump_flag) {
		std::vector<std::string>::const_iterator iter_b(file_list.begin());
		std::vector<std::string>::const_iterator iter_e(file_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			XmlDomElement root_element(XmlDomElement::ParseXmlFile(*iter_b));
			std::cout << std::setfill('=') << std::setw(79) << "" <<
				std::setfill(' ') << std::endl;
			std::cout << "Filename: " << *iter_b << std::endl;
			std::cout << std::setfill('-') << std::setw(79) << "" <<
				std::setfill(' ') << std::endl;
			root_element.EmitElementTree();
			std::cout << std::setfill('=') << std::setw(79) << "" <<
				std::setfill(' ') << std::endl;
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_XmlDomElement(argc, argv);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

