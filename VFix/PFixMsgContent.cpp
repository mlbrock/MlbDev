// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixMsgContent.cpp

	File Description	:	Implementation of the PFixMsgContent class.

	Revision History	:	2015-07-19 --- Creation.
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

#include <VFix/PFixMsgContent.hpp>
#include <VFix/VFixMISetHolder.hpp>

#include <Utility/C_StringSupport.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/ParseNumericString.hpp>
#include <Utility/TabularReportSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <boost/io/ios_state.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
class PFixMsgContentSet : public VFixMISetHolder<PFixMsgContentMISet> { };
// ////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
typedef boost::multi_index::index<PFixMsgContentMISet, 
	PFixMsgContentByCompIdTag>::type  PFixMsgContentMISetIdxByCompIdTag;
typedef boost::multi_index::index<PFixMsgContentMISet,
	PFixMsgContentByCompIdPos>::type  PFixMsgContentMISetIdxByCompIdPos;
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
PFixMsgContent::PFixMsgContent(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
try
	:component_id_()
	,tag_text_()
	,tag_(0)
	,indent_(0)
	,position_(0)
	,reqd_(false)
	,fix_version_()
	,description_()
{
	using namespace MLB::RapidXmlUtils;

	if ((xml_element.node_type_ != XmlDomElement::NodeType_Element) ||
		(xml_element.element_name_ != "MsgContent")) {
		std::ostringstream o_str;
		o_str << "Unexpected XML combination of element name ('" <<
			xml_element.element_name_ << "') and node type (" <<
			xml_element.node_type_ << "), expected element name 'MsgContent' and "
			"node type " << XmlDomElement::NodeType_Element << ".";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}		
		
	const std::string   &added(xml_element.GetAttributeValue("added", true));
	const std::string   &comp_id(xml_element.GetChildRef("ComponentID").
		GetNodeTextFromChildRef());
	const std::string   &tag_text(xml_element.GetChildRef("TagText").
		GetNodeTextFromChildRef());
	const std::string   &indent_string(xml_element.GetChildRef("Indent").
		GetNodeTextFromChildRef());
	const std::string   &position_string(xml_element.GetChildRef("Position").
		GetNodeTextFromChildRef());
	const std::string   &reqd_string(xml_element.GetChildRef("Reqd").
		GetNodeTextFromChildRef());
	const XmlDomElement *desc_ptr(xml_element.GetChildPtr("Description"));
	const std::string    description((desc_ptr &&
		desc_ptr->HasNodeTextFromChild()) ? desc_ptr->GetNodeTextFromChildRef() :
		"");

	PFixTagNum tag_num = 0;
	MLB::Utility::ParseNumericString(tag_text, tag_num, false);
	PFixComponentId component_id =
		MLB::Utility::CheckIsNumericString<PFixComponentId>(comp_id, 1);
	PFixIndent      indent       =
		MLB::Utility::CheckIsNumericString<PFixIndent>(indent_string);
	PFixPosition    position     =
		MLB::Utility::CheckIsNumericString<PFixPosition>(position_string, 1);
	bool            reqd;
	MLB::Utility::ParseCmdLineArg::ParseCmdLineDatum(reqd_string, reqd);

	PFixMsgContent(component_id, tag_text, tag_num, indent, position, reqd,
		added, description).swap(*this);
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Unable to construct an PFixMsgContent "
		"instance: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMsgContent::PFixMsgContent(PFixComponentId component_id,
	const std::string &tag_text, PFixTagNum tag, PFixIndent indent,
	PFixPosition position, bool reqd, const std::string &fix_version,
	const std::string &description)
	:component_id_(component_id)
	,tag_text_(tag_text)
	,tag_(tag)
	,indent_(indent)
	,position_(position)
	,reqd_(reqd)
	,fix_version_(fix_version)
	,description_(description)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixMsgContent::swap(PFixMsgContent &other)
{
	std::swap(component_id_, other.component_id_);
	std::swap(tag_text_,     other.tag_text_);
	std::swap(tag_,          other.tag_);
	std::swap(indent_,       other.indent_);
	std::swap(position_,     other.position_);
	std::swap(reqd_,         other.reqd_);
	std::swap(fix_version_,  other.fix_version_);
	std::swap(description_,  other.description_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string PFixMsgContent::GetIdString() const
{
	std::ostringstream o_str;

	o_str << "Key: {component id '" << component_id_ << "', tag text '" <<
		tag_text_ << "', position '" << position_ << "'}";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <typename MISetIndexType, typename MISetIndexKeyType>
	const PFixMsgContent *FindElementHelper(const MISetIndexType &set_index,
		const char *by_name, PFixComponentId component_id_key,
		const MISetIndexKeyType &other_key, bool throw_if_not_found)
{
	typename MISetIndexType::const_iterator iter_f(
		set_index.find(boost::make_tuple(component_id_key, other_key)));

	if (iter_f == set_index.end()) {
		if (!throw_if_not_found)
			return(NULL);
		std::ostringstream o_str;
		o_str << "Unable to locate component id '" << component_id_key << "' "
			"and message content " << by_name << " '" << other_key << "' in the "
			"set of " << set_index.size() << " PFixMsgContent elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(&(*iter_f));
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const PFixMsgContent *PFixMsgContent::FindElementByTag(
	const PFixMsgContentSet &in_set, PFixComponentId component_id_key,
	const std::string &tag_key, bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixMsgContentByCompIdTag>(),
		"tag text", component_id_key, tag_key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMsgContent *PFixMsgContent::FindElementByTag(
	const PFixMsgContentSet &in_set, PFixComponentId component_id_key,
	PFixTagNum tag_key, bool throw_if_not_found)
{
	std::ostringstream o_str;

	o_str << tag_key;

	return(FindElementByTag(in_set, component_id_key, o_str.str(),
		throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMsgContent *PFixMsgContent::FindElementByPosition(
	const PFixMsgContentSet &in_set, PFixComponentId component_id_key,
	PFixPosition position_key, bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixMsgContentByCompIdPos>(),
		"position", component_id_key, position_key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMsgContent *PFixMsgContent::FindElementByPosition(
	const PFixMsgContentSet &in_set, PFixComponentId component_id_key,
	const std::string &position_key, bool throw_if_not_found)
{
	PFixComponentId tmp_position_key;

	try {
		tmp_position_key =
			MLB::Utility::CheckIsNumericString<PFixComponentId>(position_key);
	}
	catch (const std::exception &) {
		if (!throw_if_not_found)
			return(NULL);
		throw;
	}

	return(FindElementByPosition(in_set, component_id_key, tmp_position_key,
		throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixMsgContent *PFixMsgContent::FindElement(
	const PFixMsgContentSet &in_set, PFixComponentId component_id_key,
	const std::string &other_key, bool throw_if_not_found)
{
	const PFixMsgContent *datum_ptr;

	if (((datum_ptr = FindElementByTag(in_set, component_id_key, other_key,
		false)) != NULL) ||
		 ((datum_ptr = FindElementByPosition(in_set, component_id_key,
			other_key, false)) != NULL))
		return(datum_ptr);

	if (throw_if_not_found) {
		std::ostringstream o_str;
		o_str << "Unable to locate the combination of component id '" <<
			component_id_key << "' and other key '" << other_key << "' as a "
			"message content tag text string or a message content position "
			"string in the set of " << in_set.size() << " PFixMsgContent "
			"elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PFixMsgContent::ShouldApplyXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
{
	using namespace MLB::RapidXmlUtils;

	return((xml_element.node_type_ == XmlDomElement::NodeType_Element) &&
		(xml_element.element_name_ == "MsgContent"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMsgContentSet &PFixMsgContent::ParseXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element,
	PFixMsgContentSet &out_set)
{
	using namespace MLB::RapidXmlUtils;

	PFixMsgContentSet tmp_set;

	try {
		if ((xml_element.node_type_ != XmlDomElement::NodeType_Unknown) ||
			(xml_element.element_name_ != "MsgContents")) {
			std::ostringstream o_str;
			o_str << "Expected the major node in the XML for a FIX datatype to "
				"have the name 'MsgContents' and the node type " <<
				XmlDomElement::NodeType_Unknown << " , but its actual name is '" << 
				xml_element.element_name_ << "' and the node type is " <<
				xml_element.node_type_ << ".";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		for (std::size_t count_1 = 0; count_1 < xml_element.child_list_.size();
			++count_1) {
			const XmlDomElement &this_element(xml_element.child_list_[count_1]);
			if (ShouldApplyXmlElement(this_element))
				AddElement(PFixMsgContent(this_element), tmp_set);
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX component XML data failed: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	out_set.swap(tmp_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMsgContentSet &PFixMsgContent::ParseXmlFile(const std::string &file_name,
	PFixMsgContentSet &out_set)
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
		o_str << "Attempt to parse the FIX components XML file '" <<
			file_name << "' failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixMsgContentSet PFixMsgContent::ParseXmlFile(const std::string &file_name)
{
	PFixMsgContentSet out_set;

	return(ParseXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixMsgContent::AddElement(const PFixMsgContent &datum,
	PFixMsgContentSet &out_set)
{
	try {
		MLB::Utility::ThrowIfEmpty(datum.tag_text_, "Tag Text");
		std::pair<PFixMsgContentMISetIdxByCompIdTag::const_iterator, bool>
			insert_pair(out_set.Get().get<PFixMsgContentByCompIdTag>().
			insert(datum));
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
	MLB::Utility::MakeInlineVector<std::size_t>(5)(34)(31)(26)(22)(10),
	MLB::Utility::MakeInlineVector<std::string>
		("Component")("Tag")("Indent")("Position")("Field Is")("Fix Version"),
	MLB::Utility::MakeInlineVector<std::string>
		("Id")("Text")("Value")("Value")("Required")("Added"));
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMsgContent::EmitTabular(std::ostream &o_str) const
{
	MyTabularReportSupport.AssertColCountMinimum(6);

	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< std::right
		<< std::setw(MyTabularReportSupport[0]) << component_id_ << " "
		<< std::left
		<< std::setw(MyTabularReportSupport[1]) << tag_text_     << " "
		<< std::right
		<< std::setw(MyTabularReportSupport[2]) << indent_ << " "
		<< std::setw(MyTabularReportSupport[3]) << position_     << " "
		<< std::left
		<< std::setw(MyTabularReportSupport[4]) <<
			MLB::Utility::AnyToString(reqd_)                      << " "
		<<                                         fix_version_
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
		tmp_o_str << " PFixMsgContent Report by " << by_type << " ";
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
std::ostream &PFixMsgContent::EmitTabularByTag(const PFixMsgContentSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixMsgContentByCompIdTag>(),
		"Component Id / Tag Text", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMsgContent::EmitTabularByPosition(
	const PFixMsgContentSet &in_set, std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixMsgContentByCompIdPos>(),
		"Component Id / Position", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixMsgContent::EmitTabular(const PFixMsgContentSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularByTag(in_set, o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixMsgContent &datum)
{
	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< "{"
		<< "\"component_id_\": \"" << datum.component_id_ << "\", "
		<< "\"tag_text_\": \""     << datum.tag_text_     << "\", "
		<< "\"indent_\": \""       << datum.indent_       << "\", "
		<< "\"position_\": \""     << datum.position_     << "\", "
		<< "\"reqd_\": \""         << datum.reqd_         << "\", "
		<< "\"fix_version_\": \""  << datum.fix_version_  << "\""
		<< "}"
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PFixMsgContentSet &datum)
{
	PFixMsgContentMISet::nth_index<0>::type::const_iterator iter_b(
		datum.Get().get<0>().begin());
	PFixMsgContentMISet::nth_index<0>::type::const_iterator iter_e(
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
	PFixMsgContentSet element_set(PFixMsgContent::ParseXmlFile(file_name));

	std::cout << element_set << std::endl << std::endl;

	PFixMsgContent::EmitTabularByTag(element_set, std::cout);
	std::cout << std::endl;

	PFixMsgContent::EmitTabularByPosition(element_set, std::cout);
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
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\MsgContents.xml > TestResults.MLB.VFix.PFixMsgContent.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	XML/FIX.5.0SP2/Base/MsgContents.xml > TestResults.MLB.VFix.PFixMsgContent.txt
*/

#endif // #ifdef TEST_MAIN

