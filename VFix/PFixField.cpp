// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixField.cpp

	File Description	:	Implementation of the PFixField class.

	Revision History	:	2015-07-03 --- Creation.
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

#include <VFix/PFixField.hpp>

#include <Utility/InlineContainer.hpp>
#include <Utility/ParseNumericString.hpp>
#include <Utility/TabularReportSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <boost/io/ios_state.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
PFixField::PFixField(VFixTagNum tag)
	:tag_(tag)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixField::PFixField(VFixTagNum tag, const std::string &name,
	const std::string &type_name)
	:tag_(tag)
	,name_(name)
	,type_name_(type_name)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixField::PFixField(const MLB::RapidXmlUtils::XmlDomElement &xml_element)
try
	:tag_()
	,ref_to_tag_(0)
	,ref_from_tag_(0)
	,name_()
	,type_name_()
	,abbreviation_()
	,fix_version_()
	,description_()
	,vfix_xport_type_(VFixXPortType_None)
{
	using namespace MLB::RapidXmlUtils;

	if ((xml_element.node_type_ != XmlDomElement::NodeType_Element) ||
		(xml_element.element_name_ != "Field")) {
		std::ostringstream o_str;
		o_str << "Unexpected XML combination of element name ('" <<
			xml_element.element_name_ << "') and node type (" <<
			xml_element.node_type_ << "), expected element name 'Field' and "
			"node type " << XmlDomElement::NodeType_Element << ".";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}		
		
	const std::string   &added(xml_element.GetAttributeValue("added", true));
	const std::string   &tag(xml_element.GetChildRef("Tag").
		GetNodeTextFromChildRef());
	const std::string   &name(xml_element.GetChildRef("Name").
		GetNodeTextFromChildRef());
	const std::string   &type_name(xml_element.GetChildRef("Type").
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
	const XmlDomElement *ref_ptr(xml_element.GetChildPtr("AssociatedDataTag"));
	std::string          assoc_tag((ref_ptr) ?
		ref_ptr->GetNodeTextFromChildRef() : "");

	VFixTagNum tag_num       =
		MLB::Utility::CheckIsNumericString<VFixTagNum>(tag, 1);
	VFixTagNum assoc_tag_num = (!ref_ptr) ? 0 :
		MLB::Utility::CheckIsNumericString<VFixTagNum>(
		ref_ptr->GetNodeTextFromChildRef(), 1);

	PFixField(tag_num, assoc_tag_num, 0, name, type_name, abbreviation, added,
		description, vfix_xport_type_).swap(*this);
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Unable to construct an PFixField "
		"instance: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixField::PFixField(VFixTagNum tag, VFixTagNum ref_to_tag,
	VFixTagNum ref_from_tag, const std::string &name,
	const std::string &type_name, const std::string &abbreviation,
	const std::string &fix_version, const std::string &description,
	VFixXPortType vfix_xport_type)
	:tag_(tag)
	,ref_to_tag_(ref_to_tag)
	,ref_from_tag_(ref_from_tag)
	,name_(name)
	,type_name_(type_name)
	,abbreviation_(abbreviation)
	,fix_version_(fix_version)
	,description_(description)
	,vfix_xport_type_(vfix_xport_type)
{
	if (tag < 1)
		MLB::Utility::ThrowInvalidArgument("Field tag number may not be less "
			"than 1.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixField::swap(PFixField &other)
{
	std::swap(tag_, other.tag_);
	std::swap(name_, other.name_);
	std::swap(type_name_, other.type_name_);
	std::swap(abbreviation_, other.abbreviation_);
	std::swap(fix_version_, other.fix_version_);
	std::swap(description_, other.description_);
	std::swap(vfix_xport_type_, other.vfix_xport_type_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PFixField::ShouldApplyXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
{
	using namespace MLB::RapidXmlUtils;

	return((xml_element.node_type_ == XmlDomElement::NodeType_Element) &&
		(xml_element.element_name_ == "Field"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldSet &PFixField::ParseXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element,
	PFixFieldSet &out_set)
{
	using namespace MLB::RapidXmlUtils;

	PFixFieldSet tmp_set;

	try {
		if ((xml_element.node_type_ != XmlDomElement::NodeType_Unknown) ||
			(xml_element.element_name_ != "Fields")) {
			std::ostringstream o_str;
			o_str << "Expected the major node in the XML for a FIX datatype to "
				"have the name 'Fields' and the node type " <<
				XmlDomElement::NodeType_Unknown << " , but its actual name is '" << 
				xml_element.element_name_ << "' and the node type is " <<
				xml_element.node_type_ << ".";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		for (std::size_t count_1 = 0; count_1 < xml_element.child_list_.size();
			++count_1) {
			const XmlDomElement &this_element(xml_element.child_list_[count_1]);
			if (ShouldApplyXmlElement(this_element))
				AddElement(PFixField(this_element), tmp_set);
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX field XML data failed: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	out_set.swap(tmp_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldSet &PFixField::ParseXmlFile(const std::string &file_name,
	PFixFieldSet &out_set)
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
		o_str << "Attempt to parse the FIX fields XML file '" <<
			file_name << "' failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldSet PFixField::ParseXmlFile(const std::string &file_name)
{
	PFixFieldSet out_set;

	return(ParseXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixField::AddElement(const PFixField &datum,
	PFixFieldSet_I &out_set)
{
	PFixFieldSetInsertPair insert_pair(out_set.insert(datum));

	if (!insert_pair.second) {
		std::ostringstream o_str;
		o_str << "Attempt to insert field element for field tag " <<
			datum.tag_ << ", name '" << datum.name_ << "', abbreviation '" <<
			datum.abbreviation_ << "' failed because it is already in the set of "
			"fields.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::TabularReportSupport MyTabularReportSupport(
	MLB::Utility::MakeInlineVector<std::size_t>(10)(10)(10)(39)(19)(28)(10)(2)(19),
	MLB::Utility::MakeInlineVector<std::string>
		("Field Tag")("Ref To")("Ref From")("Field")("Type")("Abbreviation")("Fix Version")("XPort")("XPort"),
	MLB::Utility::MakeInlineVector<std::string>
		("Number")("Tag")("Tag")("Name")("Name")("Name")("Added")("Value")("Name"));
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixField::EmitTabular(std::ostream &o_str) const
{
	MyTabularReportSupport.AssertColCountMinimum(9);

	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< std::right
		<< std::setw(MyTabularReportSupport[0]) << tag_             << " "
		<< std::setw(MyTabularReportSupport[1]) << ref_to_tag_      << " "
		<< std::setw(MyTabularReportSupport[2]) << ref_from_tag_    << " "
		<< std::left
		<< std::setw(MyTabularReportSupport[3]) << name_            << " "
		<< std::setw(MyTabularReportSupport[4]) << type_name_       << " "
		<< std::setw(MyTabularReportSupport[5]) << abbreviation_    << " "
		<< std::setw(MyTabularReportSupport[6]) << fix_version_     << " "
		<< std::right
		<< std::setw(MyTabularReportSupport[7]) << vfix_xport_type_ << " "
		<< std::left
		<< VFixXPortTypeToString(vfix_xport_type_)
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixField::EmitTabular(const PFixFieldSet &in_set,
	std::ostream &o_str)
{
	MyTabularReportSupport.EmitHeader(o_str, '=', '-', ' ', true);

	PFixFieldSetIterC iter_b(in_set.begin());
	PFixFieldSetIterC iter_e(in_set.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		iter_b->EmitTabular(o_str);
		o_str << std::endl;
	}

	MyTabularReportSupport.EmitTrailer(o_str, '=', ' ', true);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixField &datum)
{
	o_str
		<< "{"
		<< "\"tag_\": "               << datum.tag_          << ", "
		<< "\"ref_to_tag_\": "        << datum.ref_to_tag_   << ", "
		<< "\"ref_from_tag_\": "      << datum.ref_from_tag_ << ", "
		<< "\"name_\": \""            << datum.name_         << "\", "
		<< "\"type_name\": \""        << datum.type_name_    << "\", "
		<< "\"abbreviation_\": \""    << datum.abbreviation_ << "\", "
		<< "\"fix_version_\": \""     << datum.fix_version_  << "\", "
		<< "\"vfix_xport_type_\": \"" <<
			VFixXPortTypeToString(datum.vfix_xport_type_)     << "\""
		<< "}"
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PFixFieldSet &datum)
{
	PFixFieldSetIterC iter_b(datum.begin());
	PFixFieldSetIterC iter_e(datum.end());

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
void TEST_RunTest(const char *field_file_name, const char *data_type_file_name)
{
	PFixFieldSet element_set(PFixField::ParseXmlFile(field_file_name));

//	std::cout << element_set << std::endl << std::endl;

	PFixField::EmitTabular(element_set, std::cout);
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
				"   " << argv[0] << " <fix-xml-field-file> "
				"[ <fix-xml-datatype-file> ]" <<
				std::endl << std::endl;
			return(EXIT_SUCCESS);
		}
		else if (argc < 2)
			ThrowInvalidArgument("No XML files were specified. "
				"Use '-h' for help.");
		else if (argc > 3)
			ThrowInvalidArgument("Unexpected arguments encountered. "
				"Use '-h' for help.");
		TEST_RunTest(argv[1], (argc == 3) ? argv[2] : NULL);
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
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\Fields.xml ..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\Datatypes.xml > TestResults.MLB.VFix.PFixField.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	XML/FIX.5.0SP2/Base/Datatypes.xml > TestResults.MLB.VFix.PFixField.txt
*/

#endif // #ifdef TEST_MAIN

