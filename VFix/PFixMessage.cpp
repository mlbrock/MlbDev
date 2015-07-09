// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixMessage.cpp

	File Description	:	Implementation of the PFixMessage class.

	Revision History	:	2015-07-09 --- Creation.
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

#include <VFix/PFixMessage.hpp>

#include <Utility/C_StringSupport.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/ParseNumericString.hpp>
#include <Utility/TabularReportSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <boost/io/ios_state.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
PFixMessage::PFixMessage(const char *message_type)
	:message_type_(message_type)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMessage::PFixMessage(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
try
	:component_id_(0)
	,message_type_()
	,name_()
	,category_id_()
	,section_id_()
	,abbreviation_()
	,fix_version_()
	,description_()
{
	using namespace MLB::RapidXmlUtils;

	if ((xml_element.node_type_ != XmlDomElement::NodeType_Element) ||
		(xml_element.element_name_ != "Message")) {
		std::ostringstream o_str;
		o_str << "Unexpected XML combination of element name ('" <<
			xml_element.element_name_ << "') and node type (" <<
			xml_element.node_type_ << "), expected element name 'Message' and "
			"node type " << XmlDomElement::NodeType_Element << ".";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}		
		
	const std::string   &added(xml_element.GetAttributeValue("added", true));
	const std::string   &comp_id(xml_element.GetChildRef("ComponentID").
		GetNodeTextFromChildRef());
	const std::string   &message_type(xml_element.GetChildRef("MsgType").
		GetNodeTextFromChildRef());
	const std::string   &name(xml_element.GetChildRef("Name").
		GetNodeTextFromChildRef());
	const std::string   &category_id(xml_element.GetChildRef("CategoryID").
		GetNodeTextFromChildRef());
	const std::string   &section_id(xml_element.GetChildRef("SectionID").
		GetNodeTextFromChildRef());
	/*
		The FIX repository seems to always have this element (even if the
		text of the element is the same as the field name). But we won't
		impose the requirement that it be present...
	*/
	const XmlDomElement *abbr_ptr(xml_element.GetChildPtr("AbbrName"));
	const std::string   &abbreviation((abbr_ptr) ?
		abbr_ptr->GetNodeTextFromChildRef() : "");
	const XmlDomElement &desc_ref(xml_element.GetChildRef("Description"));
	const std::string   &description((desc_ref.HasNodeTextFromChild()) ?
		desc_ref.GetNodeTextFromChildRef() : "");

	VFixComponentId component_id =
		MLB::Utility::CheckIsNumericString<VFixComponentId>(comp_id, 1);

	PFixMessage(component_id, message_type, name, category_id,
		section_id, abbreviation, added, description).swap(*this);
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Unable to construct an PFixMessage "
		"instance: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMessage::PFixMessage(VFixComponentId component_id,
	const std::string &message_type, const std::string &name,
	const std::string &category_id, const std::string &section_id,
	const std::string &abbreviation, const std::string &fix_version,
	const std::string &description)
	:component_id_(component_id)
	,message_type_(message_type)
	,name_(name)
	,category_id_(category_id)
	,section_id_(section_id)
	,abbreviation_(abbreviation)
	,fix_version_(fix_version)
	,description_(description)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixMessage::swap(PFixMessage &other)
{
	std::swap(component_id_, other.component_id_);
	std::swap(message_type_, other.message_type_);
	std::swap(name_,         other.name_);
	std::swap(category_id_,  other.category_id_);
	std::swap(section_id_,   other.section_id_);
	std::swap(abbreviation_, other.abbreviation_);
	std::swap(fix_version_,  other.fix_version_);
	std::swap(description_,  other.description_);
}
// ////////////////////////////////////////////////////////////////////////////

/*
	CODE NOTE: To be removed.
// ////////////////////////////////////////////////////////////////////////////
const PFixMessage *PFixMessage::FindElement(const PFixMessageSet &in_set,
	const char *name, bool thow_if_not_found)
{
	PFixMessageSetIterC iter_f(in_set.find(PFixMessage(name)));

	if (iter_f == in_set.end()) {
		if (!thow_if_not_found)
			return(NULL);
		std::ostringstream o_str;
		o_str << "Unable to locate type name '" << name << "' in the set of " <<
			in_set.size() << " PFixMessage elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
		
	return(&(*iter_f));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMessage *PFixMessage::FindElement(const PFixMessageSet &in_set,
	const std::string &name, bool thow_if_not_found)
{
	return(FindElement(in_set, name.c_str(), thow_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
bool PFixMessage::ShouldApplyXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
{
	using namespace MLB::RapidXmlUtils;

	return((xml_element.node_type_ == XmlDomElement::NodeType_Element) &&
		(xml_element.element_name_ == "Message"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMessageSet &PFixMessage::ParseXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element,
	PFixMessageSet &out_set)
{
	using namespace MLB::RapidXmlUtils;

	PFixMessageSet tmp_set;

	try {
		if ((xml_element.node_type_ != XmlDomElement::NodeType_Unknown) ||
			(xml_element.element_name_ != "Messages")) {
			std::ostringstream o_str;
			o_str << "Expected the major node in the XML for a FIX datatype to "
				"have the name 'Message' and the node type " <<
				XmlDomElement::NodeType_Unknown << " , but its actual name is '" << 
				xml_element.element_name_ << "' and the node type is " <<
				xml_element.node_type_ << ".";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		for (std::size_t count_1 = 0; count_1 < xml_element.child_list_.size();
			++count_1) {
			const XmlDomElement &this_element(xml_element.child_list_[count_1]);
			if (ShouldApplyXmlElement(this_element))
				AddElement(PFixMessage(this_element), tmp_set);
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX message XML data failed: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	out_set.swap(tmp_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMessageSet &PFixMessage::ParseXmlFile(const std::string &file_name,
	PFixMessageSet &out_set)
{
	MLB::RapidXmlUtils::RapidXmlContext xml_context;
	MLB::RapidXmlUtils::XmlDomElement   xml_element;

	try {
		MLB::RapidXmlUtils::XmlDomElement::ParseXmlFile(file_name,
			xml_element);
		ParseXmlElement(xml_element, out_set);
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX messages XML file '" <<
			file_name << "' failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMessageSet PFixMessage::ParseXmlFile(const std::string &file_name)
{
	PFixMessageSet out_set;

	return(ParseXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixMessage::AddElement(const PFixMessage &datum,
	PFixMessageSet_I &out_set)
{
	PFixMessageSetInsertPair insert_pair(out_set.insert(datum));

	if (!insert_pair.second) {
		std::ostringstream o_str;
		o_str << "Attempt to insert datatype element for datatype name '" <<
			datum.name_ << "' failed because it is already in the set of "
			"datatypes.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::TabularReportSupport MyTabularReportSupport(
	MLB::Utility::MakeInlineVector<std::size_t>(5)(39)(5)(29)(23)(23)(10),
	MLB::Utility::MakeInlineVector<std::string>
		("Message")("Message")("Component")("Category")("Section")("Abbreviation")("Fix Version"),
	MLB::Utility::MakeInlineVector<std::string>
		("Type")("Name")("Id")("Id")("Id")("Name")("Added"));
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMessage::EmitTabular(std::ostream &o_str) const
{
	MyTabularReportSupport.AssertColCountMinimum(7);

	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< std::left
		<< std::setw(MyTabularReportSupport[0]) << message_type_ << " "
		<< std::setw(MyTabularReportSupport[1]) << name_         << " "
		<< std::right
		<< std::setw(MyTabularReportSupport[2]) << component_id_ << " "
		<< std::left
		<< std::setw(MyTabularReportSupport[3]) << category_id_  << " "
		<< std::setw(MyTabularReportSupport[4]) << section_id_   << " "
		<< std::setw(MyTabularReportSupport[5]) << abbreviation_ << " "
		<< std::setw(MyTabularReportSupport[6]) << fix_version_
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMessage::EmitTabular(const PFixMessageSet &in_set,
	std::ostream &o_str)
{
	MyTabularReportSupport.EmitHeader(o_str, '=', '-', ' ', true);

	PFixMessageSetIterC iter_b(in_set.begin());
	PFixMessageSetIterC iter_e(in_set.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		iter_b->EmitTabular(o_str);
		o_str << std::endl;
	}

	MyTabularReportSupport.EmitTrailer(o_str, '=', ' ', true);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixMessage &datum)
{
	MyTabularReportSupport.AssertColCountMinimum(6);

	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< "{"
		<< "\"component_id_\": \"" << datum.component_id_ << "\", "
		<< "\"message_type_\": \"" << datum.message_type_ << "\", "
		<< "\"name_\": \""         << datum.name_         << "\", "
		<< "\"category_id_\": \""  << datum.category_id_  << "\", "
		<< "\"section_id_\": \""   << datum.section_id_   << "\", "
		<< "\"abbreviation_\": \"" << datum.abbreviation_ << "\", "
		<< "\"fix_version_\": \""  << datum.fix_version_  << "\""
		<< "}"
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PFixMessageSet &datum)
{
	PFixMessageSetIterC iter_b(datum.begin());
	PFixMessageSetIterC iter_e(datum.end());

	for ( ; iter_b != iter_e; ++iter_b)
		o_str << *iter_b << std::endl;

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
	PFixMessageSet element_set(PFixMessage::ParseXmlFile(file_name));

	std::cout << element_set << std::endl << std::endl;

	PFixMessage::EmitTabular(element_set, std::cout);
	std::cout << std::endl;
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
				"   " << argv[0] << " <fix-xml-datatype-file> "
				"[ <fix-xml-datatype-file> ... ]" <<
				std::endl << std::endl;
			return(EXIT_SUCCESS);
		}
		else if (argc < 2)
			ThrowInvalidArgument("No XML files were specified. "
				"Use '-h' for help.");
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
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\Messages.xml > TestResults.MLB.VFix.PFixMessage.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	XML/FIX.5.0SP2/Base/Messages.xml > TestResults.MLB.VFix.PFixMessage.txt
*/

#endif // #ifdef TEST_MAIN

