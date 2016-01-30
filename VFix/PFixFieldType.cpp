// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixFieldType.cpp

	File Description	:	Implementation of the PFixFieldType class.

	Revision History	:	2015-06-27 --- Creation.
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

#include <VFix/PFixFieldType.hpp>

#include <Utility/C_StringSupport.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/TabularReportSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <boost/io/ios_state.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
PFixFieldType::PFixFieldType(const std::string &name)
	:name_(name)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldType::PFixFieldType(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
try
	:name_()
	,base_name_1_()
	,base_name_2_()
	,fix_version_()
	,description_()
	,vfix_xport_type_(VFixXPortType_None)
{
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
	const XmlDomElement &desc_ref(xml_element.GetChildRef("Description"));
	const std::string   &description((desc_ref.HasNodeTextFromChild()) ?
		desc_ref.GetNodeTextFromChildRef() : "");
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
/*
		StringToVFixXPortType(
			(base_name_ptr_1) ? base_name_ptr_1->c_str() : NULL,
			(base_name_ptr_2) ? base_name_ptr_2->c_str() : NULL, true)).
*/
		StringToVFixXPortType(name.c_str(), true)).
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
	const std::string &fix_version, const std::string &description,
	VFixXPortType vfix_xport_type)
	:name_(name)
	,base_name_1_(base_name_1)
	,base_name_2_(base_name_2)
	,fix_version_(fix_version)
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
	std::swap(fix_version_, other.fix_version_);
	std::swap(description_, other.description_);
	std::swap(vfix_xport_type_, other.vfix_xport_type_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixFieldType *PFixFieldType::FindElement(const PFixFieldTypeSet &in_set,
	const std::string &name, bool throw_if_not_found)
{
	PFixFieldTypeSetIterC iter_f(in_set.find(PFixFieldType(name)));

	if (iter_f == in_set.end()) {
		if (!throw_if_not_found)
			return(NULL);
		std::ostringstream o_str;
		o_str << "Unable to locate type name '" << name << "' in the set of " <<
			in_set.size() << " PFixFieldType elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
		
	return(&(*iter_f));
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
PFixFieldTypeSet &PFixFieldType::ParseXmlElement(
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
			if (ShouldApplyXmlElement(this_element))
				AddElement(PFixFieldType(this_element), tmp_set);
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX datatype XML data failed: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	out_set.swap(tmp_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldTypeSet &PFixFieldType::ParseXmlFile(const std::string &file_name,
	PFixFieldTypeSet &out_set)
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
		o_str << "Attempt to parse the FIX datatypes XML file '" <<
			file_name << "' failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldTypeSet PFixFieldType::ParseXmlFile(const std::string &file_name)
{
	PFixFieldTypeSet out_set;

	return(ParseXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::set<VFixXPortType> &PFixFieldType::CheckForUnusedTypes(
	const PFixFieldTypeSet &in_set, std::set<VFixXPortType> &out_set)
{
	std::set<VFixXPortType> tmp_set;

	for (int count_1 = VFixXPortType_Minimum; count_1 <= VFixXPortType_Maximum;
		++count_1) {
		if (count_1 != VFixXPortType_None)
			tmp_set.insert(static_cast<VFixXPortType>(count_1));
	}

	PFixFieldTypeSetIterC iter_b(in_set.begin());
	PFixFieldTypeSetIterC iter_e(in_set.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		std::set<VFixXPortType>::iterator iter_f(
			tmp_set.find(iter_b->vfix_xport_type_));
		if (iter_f != tmp_set.end())
			tmp_set.erase(iter_f);
	}

	out_set.swap(tmp_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixFieldType::AddElement(const PFixFieldType &datum,
	PFixFieldTypeSet_I &out_set)
{
	PFixFieldTypeSetInsertPair insert_pair(out_set.insert(datum));

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
	MLB::Utility::MakeInlineVector<std::size_t>(23)(7)(23)(10)(2)(19),
	MLB::Utility::MakeInlineVector<std::string>
		("Type")("Base")("Base")("Fix Version")("XPort")("XPort"),
	MLB::Utility::MakeInlineVector<std::string>
		("Name")("Name 1")("Name 2")("Added")("Value")("Name"));
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::TabularReportSupport MyTabularReportSupportUnused(
	MLB::Utility::MakeInlineVector<std::size_t>(23)(2)(19),
	MLB::Utility::MakeInlineVector<std::string>
		("Type")("XPort")("XPort"),
	MLB::Utility::MakeInlineVector<std::string>
		("Name")("Value")("Name"));
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixFieldType::EmitTabular(std::ostream &o_str) const
{
	MyTabularReportSupport.AssertColCountMinimum(6);

	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< std::left
		<< std::setw(MyTabularReportSupport[0]) << name_            << " "
		<< std::setw(MyTabularReportSupport[1]) << base_name_1_     << " "
		<< std::setw(MyTabularReportSupport[2]) << base_name_2_     << " "
		<< std::setw(MyTabularReportSupport[3]) << fix_version_     << " "
		<< std::right
		<< std::setw(MyTabularReportSupport[4]) << vfix_xport_type_ << " "
		<< std::left
		<< VFixXPortTypeToString(vfix_xport_type_)
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixFieldType::EmitTabular(const PFixFieldTypeSet &in_set,
	std::ostream &o_str)
{
	MyTabularReportSupport.EmitHeader(o_str, '=', '-', ' ', true);

	PFixFieldTypeSetIterC iter_b(in_set.begin());
	PFixFieldTypeSetIterC iter_e(in_set.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		iter_b->EmitTabular(o_str);
		o_str << std::endl;
	}

	MyTabularReportSupport.EmitTrailer(o_str, '=', ' ', true);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PFixFieldType::EmitUnusedTypes(const PFixFieldTypeSet &in_set,
	std::ostream &o_str)
{
	MyTabularReportSupportUnused.EmitFillLineUnbroken(o_str, '*', true);
	MyTabularReportSupportUnused.EmitCentered(" UNUSED PFIX TYPES REPORT ",
		o_str, '*', true);
	MyTabularReportSupportUnused.EmitFillLineUnbroken(o_str, '*', true);

	std::set<VFixXPortType> out_set;

	CheckForUnusedTypes(in_set, out_set);

	MyTabularReportSupportUnused.EmitHeader(o_str, '=', '-', ' ', true);

	std::set<VFixXPortType>::const_iterator iter_b(out_set.begin());
	std::set<VFixXPortType>::const_iterator iter_e(out_set.end());

	for ( ; iter_b != iter_e; ++iter_b)
		o_str
			<< std::left
			<< std::setw(MyTabularReportSupportUnused[0])            <<
				VFixXPortTypeToString(*iter_b, 0, true)               << " "
			<< std::right
			<< std::setw(MyTabularReportSupportUnused[1]) << *iter_b << " "
			<< std::left
			<< VFixXPortTypeToString(*iter_b, 1, true)
			<< "\n"
				;

	MyTabularReportSupportUnused.EmitTrailer(o_str, '=', ' ', true);

	MyTabularReportSupportUnused.EmitFillLineUnbroken(o_str, '*', true);

	return(out_set.size());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixFieldType &datum)
{
	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< "{"
		<< "\"name_\": \""            << datum.name_        << "\", "
		<< "\"base_name_1_\": \""     << datum.base_name_1_ << "\", "
		<< "\"base_name_2_\": \""     << datum.base_name_2_ << "\", "
		<< "\"fix_version_\": \""     << datum.fix_version_ << "\", "
		<< "\"vfix_xport_type_\": \"" <<
			VFixXPortTypeToString(datum.vfix_xport_type_)    << "\""
		<< "}"
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PFixFieldTypeSet &datum)
{
	PFixFieldTypeSetIterC iter_b(datum.begin());
	PFixFieldTypeSetIterC iter_e(datum.end());

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
	PFixFieldTypeSet element_set(PFixFieldType::ParseXmlFile(file_name));

	std::cout << element_set << std::endl << std::endl;

	PFixFieldType::EmitTabular(element_set, std::cout);
	std::cout << std::endl;

	PFixFieldType::EmitUnusedTypes(element_set);
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
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\Datatypes.xml > TestResults.MLB.VFix.PFixFieldType.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	XML/FIX.5.0SP2/Base/Datatypes.xml > TestResults.MLB.VFix.PFixFieldType.txt
*/

#endif // #ifdef TEST_MAIN

