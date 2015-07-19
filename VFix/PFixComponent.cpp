// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixComponent.cpp

	File Description	:	Implementation of the PFixComponent class.

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

#include <VFix/PFixComponent.hpp>
#include <VFix/VFixMISetHolder.hpp>

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
class PFixComponentSet : public VFixMISetHolder<PFixComponentMISet> { };
// ////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
typedef boost::multi_index::index<PFixComponentMISet, 
	PFixComponentByCompId>::type  PFixComponentMISetIdxByCompId;
typedef boost::multi_index::index<PFixComponentMISet,
	PFixComponentByName>::type    PFixComponentMISetIdxByName;
typedef boost::multi_index::index<PFixComponentMISet,
	PFixComponentByAbbr>::type    PFixComponentMISetIdxByAbbr;
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
PFixComponent::PFixComponent(const PFixComponentId component_id)
	:component_id_(component_id)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixComponent::PFixComponent(const std::string &name)
	:name_(name)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixComponent::PFixComponent(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
try
	:component_id_(0)
	,component_type_()
	,name_()
	,category_id_()
	,abbreviation_()
	,fix_version_()
	,description_()
{
	using namespace MLB::RapidXmlUtils;

	if ((xml_element.node_type_ != XmlDomElement::NodeType_Element) ||
		(xml_element.element_name_ != "Component")) {
		std::ostringstream o_str;
		o_str << "Unexpected XML combination of element name ('" <<
			xml_element.element_name_ << "') and node type (" <<
			xml_element.node_type_ << "), expected element name 'Component' and "
			"node type " << XmlDomElement::NodeType_Element << ".";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}		
		
	const std::string   &added(xml_element.GetAttributeValue("added", true));
	const std::string   &comp_id(xml_element.GetChildRef("ComponentID").
		GetNodeTextFromChildRef());
	const std::string   &component_type(xml_element.GetChildRef("ComponentType").
		GetNodeTextFromChildRef());
	const std::string   &name(xml_element.GetChildRef("Name").
		GetNodeTextFromChildRef());
	const std::string   &category_id(xml_element.GetChildRef("CategoryID").
		GetNodeTextFromChildRef());
	//	We don't require the existence of an abbreviation.
	const XmlDomElement *abbr_ptr(xml_element.GetChildPtr("AbbrName"));
	const std::string   &abbreviation((abbr_ptr) ?
		abbr_ptr->GetNodeTextFromChildRef() : "");
	const XmlDomElement *desc_ptr(xml_element.GetChildPtr("Description"));
	const std::string    description((desc_ptr &&
		desc_ptr->HasNodeTextFromChild()) ? desc_ptr->GetNodeTextFromChildRef() :
		"");

	PFixComponentId component_id =
		MLB::Utility::CheckIsNumericString<PFixComponentId>(comp_id, 1);

	PFixComponent(component_id, component_type, name, category_id,
		abbreviation, added, description).swap(*this);
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Unable to construct an PFixComponent "
		"instance: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixComponent::PFixComponent(PFixComponentId component_id,
	const std::string &component_type, const std::string &name,
	const std::string &category_id, const std::string &abbreviation,
	const std::string &fix_version, const std::string &description)
	:component_id_(component_id)
	,component_type_(component_type)
	,name_(name)
	,category_id_(category_id)
	,abbreviation_(abbreviation)
	,fix_version_(fix_version)
	,description_(description)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixComponent::swap(PFixComponent &other)
{
	std::swap(component_id_,   other.component_id_);
	std::swap(component_type_, other.component_type_);
	std::swap(name_,           other.name_);
	std::swap(category_id_,    other.category_id_);
	std::swap(abbreviation_,   other.abbreviation_);
	std::swap(fix_version_,    other.fix_version_);
	std::swap(description_,    other.description_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string PFixComponent::GetIdString() const
{
	std::ostringstream o_str;

	o_str << "Key: {component id '" << component_id_ << "', name '" << name_ <<
		"', abbreviation '" << abbreviation_ << "'}";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <typename MISetIndexType, typename MISetIndexKeyType>
	const PFixComponent *FindElementHelper(const MISetIndexType &set_index,
		const char *by_name, const MISetIndexKeyType &key,
		bool throw_if_not_found)
{
	typename MISetIndexType::const_iterator iter_f(set_index.find(key));

	if (iter_f == set_index.end()) {
		if (!throw_if_not_found)
			return(NULL);
		std::ostringstream o_str;
		o_str << "Unable to locate component " << by_name << " '" << key <<
			"' in the set of " << set_index.size() << " PFixComponent elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(&(*iter_f));
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const PFixComponent *PFixComponent::FindElementById(
	const PFixComponentSet &in_set, const std::string &key,
	bool throw_if_not_found)
{
	PFixComponentId component_id =
		MLB::Utility::CheckIsNumericString<PFixComponentId>(key);

	return(FindElementHelper(in_set.Get().get<PFixComponentByCompId>(),
		"identifier", component_id, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixComponent *PFixComponent::FindElementById(
	const PFixComponentSet &in_set, PFixComponentId key, bool throw_if_not_found)
{
	std::ostringstream o_str;

	o_str << key;

	return(FindElementById(in_set, o_str.str(), throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixComponent *PFixComponent::FindElementByName(
	const PFixComponentSet &in_set, const std::string &key,
	bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixComponentByName>(),
		"name", key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixComponent *PFixComponent::FindElementByAbbr(
	const PFixComponentSet &in_set, const std::string &key,
	bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixComponentByAbbr>(),
		"abbreviation", key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixComponent *PFixComponent::FindElement(const PFixComponentSet &in_set,
	const std::string &key, bool throw_if_not_found)
{
/*
	const PFixComponent *datum_ptr;

	if (((datum_ptr = FindElementByName(in_set, key, false)) != NULL) ||
		 ((datum_ptr = FindElementByAbbr(in_set, key, false)) != NULL) ||
		 ((datum_ptr = FindElementById(in_set, key, false)) != NULL))
		return(datum_ptr);
*/
	const PFixComponent *datum_ptr;

	if ((datum_ptr = FindElementByName(in_set, key, false)) != NULL)
		return(datum_ptr);

	const PFixComponentMISetIdxByAbbr           &abbr_idx(
		in_set.Get().get<PFixComponentByAbbr>());
	PFixComponentMISetIdxByAbbr::const_iterator  iter_f(
		abbr_idx.lower_bound(key));

	if ((iter_f != abbr_idx.end()) && (iter_f->abbreviation_ == key) &&
		((++iter_f == abbr_idx.end()) || (iter_f->abbreviation_ != key)))
		return(&(*--iter_f));

	if ((datum_ptr = FindElementById(in_set, key, false)) != NULL)
		return(datum_ptr);

	if (throw_if_not_found) {
		std::ostringstream o_str;
		o_str << "Unable to locate '" << key << "' as a component name, "
			"component abbreviation or component identifier string in the set "
			"of " << in_set.size() << " PFixComponent elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PFixComponent::ShouldApplyXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
{
	using namespace MLB::RapidXmlUtils;

	return((xml_element.node_type_ == XmlDomElement::NodeType_Element) &&
		(xml_element.element_name_ == "Component"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixComponentSet &PFixComponent::ParseXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element,
	PFixComponentSet &out_set)
{
	using namespace MLB::RapidXmlUtils;

	PFixComponentSet tmp_set;

	try {
		if ((xml_element.node_type_ != XmlDomElement::NodeType_Unknown) ||
			(xml_element.element_name_ != "Components")) {
			std::ostringstream o_str;
			o_str << "Expected the major node in the XML for a FIX datatype to "
				"have the name 'Components' and the node type " <<
				XmlDomElement::NodeType_Unknown << " , but its actual name is '" << 
				xml_element.element_name_ << "' and the node type is " <<
				xml_element.node_type_ << ".";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		for (std::size_t count_1 = 0; count_1 < xml_element.child_list_.size();
			++count_1) {
			const XmlDomElement &this_element(xml_element.child_list_[count_1]);
			if (ShouldApplyXmlElement(this_element))
				AddElement(PFixComponent(this_element), tmp_set);
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
PFixComponentSet &PFixComponent::ParseXmlFile(const std::string &file_name,
	PFixComponentSet &out_set)
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
PFixComponentSet PFixComponent::ParseXmlFile(const std::string &file_name)
{
	PFixComponentSet out_set;

	return(ParseXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixComponent::AddElement(const PFixComponent &datum,
	PFixComponentSet &out_set)
{
	try {
		MLB::Utility::ThrowIfEmpty(datum.component_type_, "ComponentType");
		MLB::Utility::ThrowIfEmpty(datum.name_,           "Name");
		MLB::Utility::ThrowIfEmpty(datum.category_id_,    "CategoryId");
		MLB::Utility::ThrowIfEmpty(datum.abbreviation_,   "Abbreviation");
		const PFixComponent *msg_ptr;
		if ((msg_ptr = FindElementByName(out_set, datum.abbreviation_,
			false)) != NULL) {
			//	Error if new abbreviation is in the set of component names...
			std::ostringstream o_str;
			o_str << "The new element abbreviation '" << datum.abbreviation_ <<
				"' is already in use as a component name in element " <<
				msg_ptr->GetIdString() << ".";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		else if ((msg_ptr = FindElementByAbbr(out_set, datum.name_,
			false)) != NULL) {
			//	Error if new message type name is in the set of abbreviations...
			std::ostringstream o_str;
			o_str << "The new element component name '" << datum.name_ <<
				"' is already in use as an abbreviation in element " <<
				msg_ptr->GetIdString() << ".";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		std::pair<PFixComponentMISetIdxByCompId::const_iterator, bool>
			insert_pair(out_set.Get().get<PFixComponentByCompId>().insert(datum));
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
		("Component")("Component")("Component")("Category")("Abbreviation")
			("Fix Version"),
	MLB::Utility::MakeInlineVector<std::string>
		("Id")("Name")("Type")("Id")("Name")("Added"));
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixComponent::EmitTabular(std::ostream &o_str) const
{
	MyTabularReportSupport.AssertColCountMinimum(6);

	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< std::right
		<< std::setw(MyTabularReportSupport[0]) << component_id_   << " "
		<< std::left
		<< std::setw(MyTabularReportSupport[1]) << name_           << " "
		<< std::setw(MyTabularReportSupport[2]) << component_type_ << " "
		<< std::setw(MyTabularReportSupport[3]) << category_id_    << " "
		<< std::setw(MyTabularReportSupport[4]) << abbreviation_   << " "
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
		tmp_o_str << " PFixComponent Report by " << by_type << " ";
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
std::ostream &PFixComponent::EmitTabularById(const PFixComponentSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixComponentByCompId>(),
		"Component Id", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixComponent::EmitTabularByName(const PFixComponentSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixComponentByName>(),
		"Name", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixComponent::EmitTabularByAbbr(const PFixComponentSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixComponentByAbbr>(),
		"Abbreviation", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixComponent::EmitTabular(const PFixComponentSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularById(in_set, o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixComponent &datum)
{
	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< "{"
		<< "\"component_id_\": \""   << datum.component_id_   << "\", "
		<< "\"name_\": \""           << datum.name_           << "\", "
		<< "\"component_type_\": \"" << datum.component_type_ << "\", "
		<< "\"category_id_\": \""    << datum.category_id_    << "\", "
		<< "\"abbreviation_\": \""   << datum.abbreviation_   << "\", "
		<< "\"fix_version_\": \""    << datum.fix_version_    << "\""
		<< "}"
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PFixComponentSet &datum)
{
	PFixComponentMISet::nth_index<0>::type::const_iterator iter_b(
		datum.Get().get<0>().begin());
	PFixComponentMISet::nth_index<0>::type::const_iterator iter_e(
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
	PFixComponentSet element_set(PFixComponent::ParseXmlFile(file_name));

	std::cout << element_set << std::endl << std::endl;

	PFixComponent::EmitTabularById(element_set, std::cout);
	std::cout << std::endl;

	PFixComponent::EmitTabularByName(element_set, std::cout);
	std::cout << std::endl;

	PFixComponent::EmitTabularByAbbr(element_set, std::cout);
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
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\Components.xml > TestResults.MLB.VFix.PFixComponent.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	XML/FIX.5.0SP2/Base/Components.xml > TestResults.MLB.VFix.PFixComponent.txt
*/

#endif // #ifdef TEST_MAIN

