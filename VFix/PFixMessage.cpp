// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixMessage.cpp

	File Description	:	Implementation of the PFixMessage class.

	Revision History	:	2015-07-09 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2016.
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

namespace {

//	////////////////////////////////////////////////////////////////////////////
typedef boost::multi_index::index<PFixMessageMISet, 
	PFixMessageByMsgType>::type PFixMessageMISetIdxByMsgType;
typedef boost::multi_index::index<PFixMessageMISet,
	PFixMessageByName>::type    PFixMessageMISetIdxByName;
typedef boost::multi_index::index<PFixMessageMISet, 
	PFixMessageByCompId>::type  PFixMessageMISetIdxByCompId;
typedef boost::multi_index::index<PFixMessageMISet,
	PFixMessageByAbbr>::type    PFixMessageMISetIdxByAbbr;
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
PFixMessage::PFixMessage(const std::string &message_type)
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
		text of the element is the same as the message name). But we won't
		impose the requirement that it be present...
	*/
	const XmlDomElement *abbr_ptr(xml_element.GetChildPtr("AbbrName"));
	const std::string   &abbreviation((abbr_ptr) ?
		abbr_ptr->GetNodeTextFromChildRef() : "");
	const XmlDomElement &desc_ref(xml_element.GetChildRef("Description"));
	const std::string   &description((desc_ref.HasNodeTextFromChild()) ?
		desc_ref.GetNodeTextFromChildRef() : "");

	PFixComponentId component_id =
		MLB::Utility::CheckIsNumericString<PFixComponentId>(comp_id, 1);

	PFixMessage(component_id, message_type, name, category_id,
		section_id, abbreviation, added, description).swap(*this);
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Unable to construct an PFixMessage "
		"instance: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMessage::PFixMessage(PFixComponentId component_id,
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

// ////////////////////////////////////////////////////////////////////////////
std::string PFixMessage::GetIdString() const
{
	std::ostringstream o_str;

	o_str << "Key: {message type '" << message_type_ << "', name '" << name_ <<
		"', component id '" << component_id_ << "', abbreviation '" <<
		abbreviation_ << "'}";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <typename MISetIndexType>
	const PFixMessage *FindElementHelper(const MISetIndexType &set_index,
		const char *by_name, const std::string &key, bool throw_if_not_found)
{
	typename MISetIndexType::const_iterator iter_f(set_index.find(key));

	if (iter_f == set_index.end()) {
		if (!throw_if_not_found)
			return(NULL);
		std::ostringstream o_str;
		o_str << "Unable to locate message " << by_name << " '" << key <<
			"' in the set of " << set_index.size() << " PFixMessage elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(&(*iter_f));
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const PFixMessage *PFixMessage::FindElementByMessageType(
	const PFixMessageSet &in_set, const std::string &key,
	bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixMessageByMsgType>(),
		"type", key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMessage *PFixMessage::FindElementByMessageType(
	const PFixMessageSet &in_set, std::size_t key, bool throw_if_not_found)
{
	std::ostringstream o_str;

	o_str << key;

	return(FindElementByMessageType(in_set, o_str.str(), throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMessage *PFixMessage::FindElementByName(const PFixMessageSet &in_set,
	const std::string &key, bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixMessageByName>(),
		"name", key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMessage *PFixMessage::FindElementByAbbr(const PFixMessageSet &in_set,
	const std::string &key, bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixMessageByAbbr>(),
		"abbreviation", key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMessage *PFixMessage::FindElement(const PFixMessageSet &in_set,
	const std::string &key, bool throw_if_not_found)
{
	const PFixMessage *datum_ptr;

	if (((datum_ptr = FindElementByName(in_set, key, false)) != NULL) ||
		 ((datum_ptr = FindElementByAbbr(in_set, key, false)) != NULL) ||
		 ((datum_ptr = FindElementByMessageType(in_set, key, false)) != NULL))
		return(datum_ptr);

	if (throw_if_not_found) {
		std::ostringstream o_str;
		o_str << "Unable to locate '" << key << "' as a message name, "
			"message abbreviation or message type string in the set of " <<
			in_set.size() << " PFixMessage elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

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
				"have the name 'Messages' and the node type " <<
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
	PFixMessageSet &out_set)
{
	try {
		MLB::Utility::ThrowIfEmpty(datum.message_type_, "MessageType");
		MLB::Utility::ThrowIfEmpty(datum.name_,         "Name");
		MLB::Utility::ThrowIfEmpty(datum.abbreviation_, "Abbreviation");
		const PFixMessage *msg_ptr;
		if ((msg_ptr = FindElementByName(out_set, datum.abbreviation_,
			false)) != NULL) {
			//	Error if new abbreviation is in the set of message type name...
			std::ostringstream o_str;
			o_str << "The new element abbreviation '" << datum.abbreviation_ <<
				"' is already in use as a message type name in element " <<
				msg_ptr->GetIdString() << ".";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		else if ((msg_ptr = FindElementByAbbr(out_set, datum.name_,
			false)) != NULL) {
			//	Error if new message type name is in the set of abbreviations...
			std::ostringstream o_str;
			o_str << "The new element message type name '" << datum.name_ <<
				"' is already in use as an abbreviation in element " <<
				msg_ptr->GetIdString() << ".";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		std::pair<PFixMessageMISetIdxByMsgType::const_iterator, bool>
			insert_pair(out_set.Get().get<PFixMessageByMsgType>().insert(datum));
		if (!insert_pair.second)
			MLB::Utility::ThrowInvalidArgument("The return value of the set "
				"insert operation indicates that one or more unique keys are "
				"already in the set and conflict with element " +
				insert_pair.first->GetIdString() + ".");
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to insert element " << datum.GetIdString() <<
			" failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::TabularReportSupport MyTabularReportSupport(
	MLB::Utility::MakeInlineVector<std::size_t>(5)(39)(5)(29)(15)(19)(10),
	MLB::Utility::MakeInlineVector<std::string>
		("Message")("Message")("Component")("Category")("Section")
			("Abbreviation")("Fix Version"),
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

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <typename MISetIndexType>
	std::ostream &EmitTabularHelper(const MISetIndexType &set_index,
		const char *by_type, std::ostream &o_str)
{
	{
		std::ostringstream tmp_o_str;
		tmp_o_str << " PFixMessage Report by " << by_type << " ";
		MyTabularReportSupport.EmitFillLineUnbroken(o_str, '=', true);
		MyTabularReportSupport.EmitCentered(tmp_o_str.str(), o_str, ' ');
	}

	MyTabularReportSupport.EmitHeader(o_str, '=', '-', ' ', true);

	typename MISetIndexType::const_iterator iter_b(set_index.begin());
	typename MISetIndexType::const_iterator iter_e(set_index.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		iter_b->EmitTabular(o_str);
		o_str << std::endl;
	}

	MyTabularReportSupport.EmitTrailer(o_str, '=', ' ', true);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMessage::EmitTabularByMessageType(
	const PFixMessageSet &in_set, std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixMessageByMsgType>(),
		"Message Type", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMessage::EmitTabularByName(const PFixMessageSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixMessageByName>(),
		"Name", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMessage::EmitTabularByAbbr(const PFixMessageSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixMessageByAbbr>(),
		"Abbreviation", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMessage::EmitTabular(const PFixMessageSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularByMessageType(in_set, o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixMessage &datum)
{
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
	PFixMessageMISet::nth_index<0>::type::const_iterator iter_b(
		datum.Get().get<0>().begin());
	PFixMessageMISet::nth_index<0>::type::const_iterator iter_e(
		datum.Get().get<0>().end());

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

	PFixMessage::EmitTabularByMessageType(element_set, std::cout);
	std::cout << std::endl;

	PFixMessage::EmitTabularByName(element_set, std::cout);
	std::cout << std::endl;

	PFixMessage::EmitTabularByAbbr(element_set, std::cout);
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
				"   " << argv[0] << " <fix-xml-message-file> "
				"[ <fix-xml-message-file> ... ]" <<
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

