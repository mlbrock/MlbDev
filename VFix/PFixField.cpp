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

namespace {

// ////////////////////////////////////////////////////////////////////////////
typedef boost::multi_index::index<PFixFieldMISet, 
	PFixFieldByTag>::type                          PFixFieldMISetIdxByTag;
typedef boost::multi_index::index<PFixFieldMISet,
	PFixFieldByName>::type                         PFixFieldMISetIdxByName;
typedef boost::multi_index::index<PFixFieldMISet,
	PFixFieldByAbbr>::type                         PFixFieldMISetIdxByAbbr;
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
PFixField::PFixField(PFixTagNum tag)
	:tag_(tag)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixField::PFixField(PFixTagNum tag, const std::string &name,
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

	PFixTagNum tag_num       =
		MLB::Utility::CheckIsNumericString<PFixTagNum>(tag, 1);
	PFixTagNum assoc_tag_num = (!ref_ptr) ? 0 :
		MLB::Utility::CheckIsNumericString<PFixTagNum>(
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
PFixField::PFixField(PFixTagNum tag, PFixTagNum ref_to_tag,
	PFixTagNum ref_from_tag, const std::string &name,
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
std::string PFixField::GetIdString() const
{
	std::ostringstream o_str;

	o_str << "Key: {field tag number '" << tag_ << "', name '" << name_ <<
		"', abbreviation '" << abbreviation_ << "'}";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <typename MISetIndexType>
	const PFixField *FindElementHelper(const MISetIndexType &set_index,
		const char *by_name, const typename MISetIndexType::key_type &key,
		bool throw_if_not_found)
{
	typename MISetIndexType::const_iterator iter_f(set_index.find(key));

	if (iter_f == set_index.end()) {
		if (!throw_if_not_found)
			return(NULL);
		std::ostringstream o_str;
		o_str << "Unable to locate field " << by_name << " '" << key <<
			"' in the set of " << set_index.size() << " PFixField elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(&(*iter_f));
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const PFixField *PFixField::FindElementByTag(const PFixFieldSet &in_set,
	PFixTagNum key, bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixFieldByTag>(),
		"tag number", key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixField *PFixField::FindElementByName(const PFixFieldSet &in_set,
	const std::string &key, bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixFieldByName>(),
		"name", key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixField *PFixField::FindElementByAbbr(const PFixFieldSet &in_set,
	const std::string &key, bool throw_if_not_found)
{
	return(FindElementHelper(in_set.Get().get<PFixFieldByAbbr>(),
		"abbreviation", key, throw_if_not_found));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PFixField *PFixField::FindElement(const PFixFieldSet &in_set,
	const std::string &key, bool throw_if_not_found)
{
	const PFixField *datum_ptr;

	if ((datum_ptr = FindElementByName(in_set, key, false)) != NULL)
		return(datum_ptr);

	const PFixFieldMISetIdxByAbbr           &abbr_idx(
		in_set.Get().get<PFixFieldByAbbr>());
	PFixFieldMISetIdxByAbbr::const_iterator  iter_f(abbr_idx.lower_bound(key));

	if ((iter_f != abbr_idx.end()) && (iter_f->abbreviation_ == key) &&
		((++iter_f == abbr_idx.end()) || (iter_f->abbreviation_ != key)))
		return(&(*--iter_f));

	PFixTagNum tag;

	if (MLB::Utility::ParseNumericString<PFixTagNum>(key, tag, false, 1) &&
		((datum_ptr = FindElementByTag(in_set, tag, false)) != NULL))
		return(datum_ptr);

	if (throw_if_not_found) {
		std::ostringstream o_str;
		o_str << "Unable to locate '" << key << "' as a field name, field "
			"abbreviation or field tag number in the set of " << in_set.size() <<
			" PFixField elements.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(NULL);
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
void PFixField::AddElement(const PFixField &datum, PFixFieldSet &out_set)
{
	try {
		MLB::Utility::ThrowIfEmpty(datum.name_, "Name");
		const PFixField *msg_ptr;
/*
		CODE NOTE: Not implemented as the field abbreviations in the FIX
		repository XML file Fields.xml are not unique to a field (as the
		message abbreviations in Messages.xml are).

		if ((msg_ptr = FindElementByName(out_set, datum.abbreviation_,
			false)) != NULL) {
			//	Error if new abbreviation is in the set of message type name...
			std::ostringstream o_str;
			o_str << "The new element abbreviation '" << datum.abbreviation_ <<
				"' is already in use as a field name in element " <<
				msg_ptr->GetIdString() << ".";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
*/
		if ((msg_ptr = FindElementByAbbr(out_set, datum.name_, false)) != NULL) {
			//	Error if new message type name is in the set of abbreviations...
			std::ostringstream o_str;
			o_str << "The new element field name '" << datum.name_ <<
				"' is already in use as an abbreviation in element " <<
				msg_ptr->GetIdString() << ".";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		std::pair<PFixFieldMISetIdxByTag::const_iterator, bool>
			insert_pair(out_set.Get().get<PFixFieldByTag>().insert(datum));
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

// ////////////////////////////////////////////////////////////////////////////
bool PFixField::FixUpSet(PFixFieldSet &in_set)
{
	return(FixUpSet(in_set, NULL));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PFixField::FixUpSet(PFixFieldSet &in_set,
	std::vector<std::string> &out_error_list)
{
	return(FixUpSet(in_set, &out_error_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PFixField::FixUpSet(PFixFieldSet &in_set,
	std::vector<std::string> *out_error_list)
{
	std::size_t out_error_count = (out_error_list) ? out_error_list->size() : 0;

	const PFixFieldMISetIdxByTag           &tag_idx(
		in_set.Get().get<PFixFieldByTag>());
	PFixFieldMISetIdxByTag::const_iterator  iter_b(tag_idx.begin());
	PFixFieldMISetIdxByTag::const_iterator  iter_e(tag_idx.end());

	for ( ; iter_b != iter_e; ++iter_b)
		const_cast<PFixField *>(&(*iter_b))->ref_from_tag_ = 0;

	iter_b = tag_idx.begin();

	for (std::size_t count_1 = 0 ; iter_b != iter_e; ++iter_b, ++count_1) {
		try {
			if (iter_b->tag_ < 1) {
				std::ostringstream o_str;
				o_str << "Invalid tag number (" << iter_b->tag_ << "): valid "
					"range is from 1 to " <<
					std::numeric_limits<PFixTagNum>::max() << ", inclusive.";
				MLB::Utility::ThrowInvalidArgument(o_str.str());
			}
			if (iter_b->ref_to_tag_) {
				const PFixField *ref_ptr_1 =
					PFixField::FindElementByTag(in_set, iter_b->ref_to_tag_);
				if (!ref_ptr_1) {
					std::ostringstream o_str;
					o_str << "Unable to locate reference target tag number " <<
						iter_b->ref_to_tag_ << ".";
					MLB::Utility::ThrowInvalidArgument(o_str.str());
				}
				if (ref_ptr_1->ref_from_tag_) {
					const PFixField *ref_ptr_2 =
						PFixField::FindElementByTag(in_set,
						ref_ptr_1->ref_from_tag_);
					std::ostringstream o_str;
					o_str << "The reference by that element to tag number " <<
						iter_b->ref_to_tag_ << " (" << ref_ptr_1->GetIdString() <<
						") has been usurped by tag number " << ref_ptr_2->tag_ <<
						" (" << ref_ptr_2->GetIdString() << ").";
					MLB::Utility::ThrowInvalidArgument(o_str.str());
				}
				std::set<int>::difference_type difference = ref_ptr_1 - &(*iter_b);
				const_cast<PFixField *>(ref_ptr_1)->ref_from_tag_ = iter_b->tag_;
			}
		}
		catch (const std::exception &except) {
			std::ostringstream o_str;
			o_str << "Element " << iter_b->GetIdString() << " is invalid: " <<
				except.what();
			if (!out_error_list)
				MLB::Utility::Rethrow(except, o_str.str());
			out_error_list->push_back(o_str.str());
		}
	}

	return((!out_error_list) || (out_error_list->size() == out_error_count));
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

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <typename MISetIndexType>
	std::ostream &EmitTabularHelper(const MISetIndexType &set_index,
		const char *by_type, std::ostream &o_str)
{
	{
		std::ostringstream tmp_o_str;
		tmp_o_str << " PFixField Report by " << by_type << " ";
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
std::ostream &PFixField::EmitTabularByTag(const PFixFieldSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixFieldByTag>(),
		"Tag Number", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixField::EmitTabularByName(const PFixFieldSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixFieldByName>(),
		"Name", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixField::EmitTabularByAbbr(const PFixFieldSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularHelper(in_set.Get().get<PFixFieldByAbbr>(),
		"Abbreviation", o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixField::EmitTabular(const PFixFieldSet &in_set,
	std::ostream &o_str)
{
	return(EmitTabularByTag(in_set, o_str));
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
	PFixFieldMISet::nth_index<0>::type::const_iterator iter_b(
		datum.Get().get<0>().begin());
	PFixFieldMISet::nth_index<0>::type::const_iterator iter_e(
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
void TEST_RunTest(const char *field_file_name, const char *data_type_file_name)
{
	PFixFieldSet element_set(PFixField::ParseXmlFile(field_file_name));

//	std::cout << element_set << std::endl << std::endl;

	PFixField::EmitTabularByTag(element_set, std::cout);
	std::cout << std::endl;

	PFixField::EmitTabularByName(element_set, std::cout);
	std::cout << std::endl;

	PFixField::EmitTabularByAbbr(element_set, std::cout);
	std::cout << std::endl;

	std::vector<std::string> error_list;

	if (!PFixField::FixUpSet(element_set, error_list))
		std::copy(error_list.begin(), error_list.end(),
			std::ostream_iterator<std::string>(std::cout, " "));
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

