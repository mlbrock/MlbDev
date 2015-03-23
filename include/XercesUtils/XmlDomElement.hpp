// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Xerces-C Utilities Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the XmlDomElement class.

	Revision History	:	2005-01-07 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__XercesUtils__XmlDomElement_hpp__HH

#define HH__MLB__XercesUtils__XmlDomElement_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>
#include <Utility/StringSupport.hpp>

#include <XercesUtils/XercesUtils.hpp>

#include <map>
#include <string>
#include <vector>

#include <xercesc/dom/DOMElement.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace XercesUtils {

// ////////////////////////////////////////////////////////////////////////////
class API_XERCESUTILS XmlDomElement {
public:
	typedef std::vector<XmlDomElement>          XmlDomElementListI;
	typedef std::pair<std::string, std::string> XmlDomAttributeI;
	typedef std::map<std::string, std::string,
		MLB::Utility::StringIsLessThanIgnoreCase>   XmlDomElementAttrMapI;

	enum NodeType {
		NodeType_Unknown,
		NodeType_Element,
		NodeType_Attribute,
		NodeType_Text,
		NodeType_CData,
		NodeType_EntityRef,
		NodeType_Entity,
		NodeType_ProcIns,
		NodeType_Comment,
		NodeType_Doc,
		NodeType_DocType,
		NodeType_DocFrag,
		NodeType_Notation,
		NodeType_Minimum   = NodeType_Unknown,
		NodeType_Maximum   = NodeType_Notation,
		NodeType_Count     = (NodeType_Maximum - NodeType_Minimum) + 1
	};

	         XmlDomElement();
	explicit XmlDomElement(xercesc::DOMElement *element_ptr);

	void                 swap(XmlDomElement &other);

	bool                 IsElement(const std::string &element_name) const;

	const std::string   &GetNodeTextRef() const;
	std::string          GetNodeTextNormal() const;
	std::string         &GetNodeTextNormal(std::string &out_string) const;

	bool                 HasNodeTextFromChild() const;
	const std::string   &GetNodeTextFromChildRef() const;
	std::string          GetNodeTextFromChildNormal() const;
	std::string         &GetNodeTextFromChildNormal(std::string &out_string)
		const;

	bool                 HasChild(const std::string &child_name) const;
	const XmlDomElement &GetChildRef(const std::string &child_name) const;
	const XmlDomElement *GetChildPtr(const std::string &child_name) const;
	unsigned int         CountChildren(const std::string &child_name) const;

	bool             HasAttribute(const std::string &attribute_name) const;
	XmlDomAttributeI GetAttribute(const std::string &attribute_name,
		bool throw_on_error = true) const;
	std::string      GetAttributeValue(const std::string &attribute_name,
		bool throw_on_error = true) const;

	void ConcatElementTree(std::ostream &o_str, unsigned int depth = 0) const;
	void EmitElementTree(unsigned int depth = 0) const;

	/*
		In the ParseXmlString() methods, the destructive_xml_parse parameter
		specifies whether it is permissible that the XML parsing logic modify
		the contents of the xml_string parameter during parsing.

		Not useful for XML parsing which uses Xerces, but can speed up XML
		parsing which uses RapidXml.
	*/
	static XmlDomElement  ParseXmlString(const std::string &xml_string,
		bool destructive_xml_parse = false);
	static XmlDomElement &ParseXmlString(const std::string &xml_string,
		XmlDomElement &xml_element, bool destructive_xml_parse = false);
	static XmlDomElement  ParseXmlString(const char *xml_string,
		bool destructive_xml_parse = false);
	static XmlDomElement &ParseXmlString(const char *xml_string,
		XmlDomElement &xml_element, bool destructive_xml_parse = false);

	static XmlDomElement  ParseXmlFile(const std::string &file_name);
	static XmlDomElement &ParseXmlFile(const std::string &file_name,
		XmlDomElement &xml_element);

	NodeType              node_type_;
	std::string           element_name_;
	std::string           node_text_;
	XmlDomElementListI    child_list_;
	XmlDomElementAttrMapI attribute_map_;

private:
	void ThrowChildElementNotFound(const std::string &child_name) const;
	void ThrowAttributeNotFound(const std::string &attribute_name) const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef XmlDomElement::XmlDomAttributeI       XmlDomAttribute;

typedef XmlDomElement::XmlDomElementListI     XmlDomElementList;
typedef XmlDomElementList::iterator           XmlDomElementListIter;
typedef XmlDomElementList::const_iterator     XmlDomElementListIterC;

typedef XmlDomElement::XmlDomElementAttrMapI  XmlDomElementAttrMap;
typedef XmlDomElementAttrMap::iterator        XmlDomElementAttrMapIter;
typedef XmlDomElementAttrMap::const_iterator  XmlDomElementAttrMapIterC;
// ////////////////////////////////////////////////////////////////////////////

} // namespace XercesUtils

} // namespace MLB

#endif // #ifndef HH__MLB__XercesUtils__XmlDomElement_hpp__HH

