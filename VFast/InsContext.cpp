// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the InsContext class.

	Revision History	:	2008-11-22 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/InsContext.hpp>
#include <VFast/ExcContext.hpp>
#include <VFast/VFastException.hpp>

#include <XercesUtils/XercesContext.hpp>
#include <XercesUtils/XmlDomElement.hpp>

#include <iterator>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

namespace {
// ////////////////////////////////////////////////////////////////////////////
void InsContextInternalWork(const MLB::XercesUtils::XmlDomElement &root_element,
	const InsCtxInfo &ins_ctx_info, InsItemMap &template_map_,
	InsItemNameMap &template_name_map_, InsItemList &ins_item_list_,
	DictValueList &dict_value_list_, unsigned int &min_template_id_,
	unsigned int &max_template_id_)
{
	using namespace MLB::XercesUtils;

	try {
		if (root_element.IsElement("template")) {
			InsItem::ParseTemplateElement(ins_ctx_info, root_element,
				ins_item_list_, dict_value_list_);
			template_map_[ins_item_list_[0].auxiliary_id_]    =
				InsItemMapValue(0,
				static_cast<unsigned int>(ins_item_list_.size()));
			template_name_map_[ins_item_list_[0].field_name_] =
				InsItemMapValue(0,
				static_cast<unsigned int>(ins_item_list_.size()));
		}
		else if (root_element.IsElement("templates")) {
			XmlDomElementListIterC iter_b(root_element.child_list_.begin());
			XmlDomElementListIterC iter_e(root_element.child_list_.end());
			for ( ; iter_b != iter_e; ++iter_b) {
				if ((!iter_b->IsElement("#text")) &&
					(!iter_b->IsElement("#comment"))) {
					unsigned int start_index =
						static_cast<unsigned int>(ins_item_list_.size());
					InsItem::ParseTemplateElement(ins_ctx_info, *iter_b,
						ins_item_list_, dict_value_list_);
					unsigned int item_count =
						static_cast<unsigned int>(ins_item_list_.size()) -
						start_index;
					//	Ensure there is no template id conflict...
					InsItemMapIterC iter_f(
						template_map_.find(ins_item_list_[start_index].auxiliary_id_));
					if (iter_f != template_map_.end())
						MLB::Utility::ThrowInvalidArgument("Template id " +
							MLB::Utility::AnyToString(
							ins_item_list_[start_index].auxiliary_id_) + " ('" +
							ins_item_list_[start_index].field_name_ + "') conflicts "
							"with a previously-parsed template with the same id ('" +
							ins_item_list_[iter_f->second.first].field_name_ + "').");
					InsItemNameMapIterC iter_f_name(
						template_name_map_.find(ins_item_list_[start_index].field_name_));
					//	Ensure there is no template name conflict...
					if (iter_f_name != template_name_map_.end())
						MLB::Utility::ThrowInvalidArgument("Template name '" +
							ins_item_list_[start_index].field_name_ + "' (template "
							"id " + MLB::Utility::AnyToString(
							ins_item_list_[start_index].auxiliary_id_) + " conflicts "
							"with a previously-parsed template with the same name ("
							"template id " + MLB::Utility::AnyToString(
							ins_item_list_[iter_f->second.first].auxiliary_id_) + ").");
					template_map_[ins_item_list_[start_index].auxiliary_id_]    =
						InsItemMapValue(start_index, item_count);
					template_name_map_[ins_item_list_[start_index].field_name_] =
						InsItemMapValue(start_index, item_count);
				}
			}
		}
		else
			MLB::Utility::ThrowInvalidArgument("Invalid top-level element '" +
				root_element.element_name_ + "' was encountered --- either of "
				"'template' or 'templates' was expected.");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create instruction context: " +
			std::string(except.what()));
	}

	InsItemList::size_type  item_index = 0;
	InsItemList::size_type  item_count = ins_item_list_.size();
	InsItem                *item_ptr   = &(ins_item_list_[0]);

	min_template_id_ = std::numeric_limits<unsigned int>::max();
	max_template_id_ = std::numeric_limits<unsigned int>::min();

	for ( ; item_index < item_count; ++item_index, ++item_ptr) {
		//	Fix-up the element self-index members...
		item_ptr->item_index_ = static_cast<unsigned int>(item_index);
		//	Verify static template references here...
		if (item_ptr->data_type_ == DataType_TemplateRefStatic) {
			try {
				unsigned int template_index = InsContext::DetermineTemplateIndex(
					template_name_map_, ins_item_list_,
					item_ptr->field_name_.c_str());
				if (template_index == InvalidTemplateIndex)
					MLB::Utility::ThrowInvalidArgument("Template name '" +
						std::string(item_ptr->field_name_) + "' not found.");
				item_ptr->static_ref_index_ = template_index;
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except, "Unable to locate the target "
					"template name of a template static reference: " +
					std::string(except.what()));
			}
		}
		else if (item_ptr->data_type_ == DataType_Template) {
			min_template_id_ = std::min(min_template_id_, item_ptr->auxiliary_id_);
			max_template_id_ = std::max(max_template_id_, item_ptr->auxiliary_id_);
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
InsContext::InsContext(const std::string &xml_file_name,
	const InsCtxInfo &ins_ctx_info)
	:template_map_()
	,template_name_map_()
	,ins_item_list_()
	,dict_value_list_()
	,min_template_id_(0)
	,max_template_id_(0)
{
	try {
		MLB::XercesUtils::XercesContext xerces_context;
		InsContextInternalWork(
			MLB::XercesUtils::XmlDomElement::ParseXmlFile(xml_file_name),
			ins_ctx_info, template_map_, template_name_map_, ins_item_list_,
			dict_value_list_, min_template_id_, max_template_id_);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse XML template file '" +
			xml_file_name + "': " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
InsContext::InsContext(bool, const std::string &xml_string,
	const InsCtxInfo &ins_ctx_info)
	:template_map_()
	,template_name_map_()
	,ins_item_list_()
	,dict_value_list_()
	,min_template_id_(0)
	,max_template_id_(0)
{
	//	Gimme forwarding ctors plz!
	InsContext tmp_ins_context(true, xml_string.c_str(), ins_ctx_info);

	swap(tmp_ins_context);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
InsContext::InsContext(bool, const char *xml_string,
	const InsCtxInfo &ins_ctx_info)
	:template_map_()
	,template_name_map_()
	,ins_item_list_()
	,dict_value_list_()
	,min_template_id_(0)
	,max_template_id_(0)
{
	try {
		if (xml_string == NULL)
			MLB::Utility::ThrowInvalidArgument("The XML string is NULL.");
		MLB::XercesUtils::XercesContext xerces_context;
		InsContextInternalWork(
			MLB::XercesUtils::XmlDomElement::ParseXmlString(xml_string),
			ins_ctx_info, template_map_, template_name_map_, ins_item_list_,
			dict_value_list_, min_template_id_, max_template_id_);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse XML template string: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsContext::swap(InsContext &other)
{
	template_map_.swap(other.template_map_);
	template_name_map_.swap(other.template_name_map_);
	ins_item_list_.swap(other.ins_item_list_);
	dict_value_list_.swap(other.dict_value_list_);
	std::swap(min_template_id_, other.min_template_id_);
	std::swap(max_template_id_, other.max_template_id_);

}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::DetermineTemplateIndex(unsigned int template_id) const
{
	InsItemMapIterC iter_f(template_map_.find(template_id));

	return((iter_f != template_map_.end()) ? iter_f->second.first :
		InvalidTemplateIndex);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::DetermineTemplateIndex(
	const std::string &template_name) const
{
	return(DetermineTemplateIndex(template_name.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::DetermineTemplateIndex(
	const char *template_name) const
{
	return(DetermineTemplateIndex(template_name_map_, ins_item_list_,
		template_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::GetTemplateIndex(unsigned int template_id) const
{
	unsigned int template_index = DetermineTemplateIndex(template_id);

	if (template_index == InvalidTemplateIndex)
		MLB::Utility::ThrowInvalidArgument("Template identifier " +
			MLB::Utility::AnyToString(template_id) + " not found.");

	return(template_index);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::GetTemplateIndex(const std::string &template_name)
	const
{
	return(GetTemplateIndex(template_name.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::GetTemplateIndex(const char *template_name) const
{
	unsigned int template_index = DetermineTemplateIndex(template_name);

	if (template_index == InvalidTemplateIndex)
		MLB::Utility::ThrowInvalidArgument("Template name '" +
			std::string(template_name) + "' not found.");

	return(template_index);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const InsItem *InsContext::GetItemPtr(const InsItem &parent_ref,
	unsigned int auxiliary_id) const
{
	const InsItem *item_ptr = &(ins_item_list_[parent_ref.item_index_]);

	for (unsigned int count_1 = 0; count_1 < parent_ref.element_count_;
		++count_1, ++item_ptr) {
		if ((auxiliary_id == item_ptr->auxiliary_id_) &&
			(parent_ref.item_index_ != item_ptr->item_index_))
			return(item_ptr);
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const InsItem *InsContext::GetItemPtr(unsigned int template_id,
	unsigned int auxiliary_id) const
{
	return(GetItemPtr(ins_item_list_[GetTemplateIndex(template_id)],
		auxiliary_id));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const InsItem *InsContext::GetItemPtr(const std::string &template_name,
	unsigned int auxiliary_id) const
{
	return(GetItemPtr(ins_item_list_[GetTemplateIndex(template_name)],
		auxiliary_id));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const InsItem *InsContext::GetItemPtr(const InsItem &parent_ref,
	const std::string &field_name) const
{
	const InsItem *item_ptr = &(ins_item_list_[parent_ref.item_index_]);

	for (unsigned int count_1 = 0; count_1 < parent_ref.element_count_;
		++count_1, ++item_ptr) {
		if (!::strcmp(field_name.c_str(), item_ptr->field_name_.c_str()))
			return(item_ptr);
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const InsItem *InsContext::GetItemPtr(unsigned int template_id,
	const std::string &field_name) const
{
	return(GetItemPtr(ins_item_list_[GetTemplateIndex(template_id)],
		field_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const InsItem *InsContext::GetItemPtr(const std::string &template_name,
	const std::string &field_name) const
{
	return(GetItemPtr(ins_item_list_[GetTemplateIndex(template_name)],
		field_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
const InsItem *InsContext::GetItemPtr(const InsItem &parent_ref,
	unsigned int auxiliary_id, const std::string &field_name, DataType data_type,
	bool is_required) const
{
	try {
		if ((!auxiliary_id) && field_name.empty())
			MLB::Utility::ThrowInvalidArgument("The specified auxiliary "
				"identifier is zero and the specified field name is empty --- "
				"at least one of them must be specified in order to locate an "
				"instruction item field.");
		const InsItem *item_ptr = &(ins_item_list_[parent_ref.item_index_]);
		for (unsigned int count_1 = 0; count_1 < parent_ref.element_count_;
			++count_1, ++item_ptr) {
			if ((count_1 == 0) && (data_type != DataType_Template))
				;
			else {
				bool match_flag;
				if (auxiliary_id)
					 match_flag = (auxiliary_id == item_ptr->auxiliary_id_) &&
						(field_name == item_ptr->field_name_);
				else
					 match_flag = (field_name == item_ptr->field_name_);
				if (match_flag) {
					if ((data_type != DataType_Invalid) &&
						(item_ptr->data_type_ != data_type))
						MLB::Utility::ThrowLogicError("Expected data type " +
							DataTypeToString(data_type) + ", but the actual data "
							"type is " + DataTypeToString(item_ptr->data_type_) + ".");
					return(item_ptr);
				}
			}
		}
		if (is_required)
			MLB::Utility::ThrowLogicError("Unable to locate this required field.");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to locate instruction context "
			"field name '" + field_name + "', auxiliary identifier " +
			MLB::Utility::AnyToString(auxiliary_id) + " as a child field within "
			"parent " + DataTypeToString(parent_ref.data_type_) + " '" +
			parent_ref.field_name_ + "' failed: " + std::string(except.what()));
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
const InsItem *InsContext::GetItemPtr(unsigned int template_id,
	unsigned int auxiliary_id, const std::string &field_name, DataType data_type,
	bool is_required) const
{
	return(GetItemPtr(ins_item_list_[GetTemplateIndex(template_id)],
		auxiliary_id, field_name, data_type, is_required));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
const InsItem *InsContext::GetItemPtr(const std::string &template_name,
	unsigned int auxiliary_id, const std::string &field_name, DataType data_type,
	bool is_required) const
{
	return(GetItemPtr(ins_item_list_[GetTemplateIndex(template_name)],
		auxiliary_id, field_name, data_type, is_required));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
const InsItem &InsContext::GetItemRef(const InsItem &parent_ref,
	unsigned int auxiliary_id, const std::string &field_name,
	DataType data_type) const
{
	return(*GetItemPtr(parent_ref, auxiliary_id, field_name, data_type, true));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
const InsItem &InsContext::GetItemRef(unsigned int template_id,
	unsigned int auxiliary_id, const std::string &field_name,
	DataType data_type) const
{
	return(GetItemRef(ins_item_list_[GetTemplateIndex(template_id)],
		auxiliary_id, field_name, data_type));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
const InsItem &InsContext::GetItemRef(const std::string &template_name,
	unsigned int auxiliary_id, const std::string &field_name,
	DataType data_type) const
{
	return(GetItemRef(ins_item_list_[GetTemplateIndex(template_name)],
		auxiliary_id, field_name, data_type));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
unsigned int InsContext::GetItemIndex(const InsItem &parent_ref,
	unsigned int auxiliary_id, const std::string &field_name,
	DataType data_type, bool is_required) const
{
	const InsItem *item_ptr =
		GetItemPtr(parent_ref, auxiliary_id, field_name, data_type, is_required);

	return((item_ptr != NULL) ? item_ptr->item_index_ : 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
unsigned int InsContext::GetItemIndex(unsigned int template_id,
	unsigned int auxiliary_id, const std::string &field_name,
	DataType data_type, bool is_required) const
{
	const InsItem *item_ptr =
		GetItemPtr(template_id, auxiliary_id, field_name, data_type, is_required);

	return((item_ptr != NULL) ? item_ptr->item_index_ : 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param data_type If this parameter is not equal to \e DataType_Invalid ,
	this function will check to ensure that any instruction context item field
	located has the same data type.
*/
unsigned int InsContext::GetItemIndex(const std::string &template_name,
	unsigned int auxiliary_id, const std::string &field_name,
	DataType data_type, bool is_required) const
{
	const InsItem *item_ptr =
		GetItemPtr(template_name, auxiliary_id, field_name, data_type,
		is_required);

	return((item_ptr != NULL) ? item_ptr->item_index_ : 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::GetMinTemplateId() const
{
	return(min_template_id_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::GetMaxTemplateId() const
{
	return(max_template_id_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsContext::CheckContext(std::vector<std::string> &error_list) const
{
	typedef std::map<unsigned int, unsigned int>  DVMap;
	typedef DVMap::iterator                       DVMapIter;
	typedef DVMap::const_iterator                 DVMapIterC;
	typedef std::pair<unsigned int, unsigned int> DVMapPair;

	unsigned int ins_item_count   =
		static_cast<unsigned int>(ins_item_list_.size());
	unsigned int dict_value_count =
		static_cast<unsigned int>(dict_value_list_.size());

	unsigned int             count_1;
	DVMap                    dv_map;
	std::vector<std::string> tmp_error_list;

	for (count_1 = 0; count_1 < ins_item_count; ++count_1) {
		const InsItem     &this_item(ins_item_list_[count_1]);
		std::stringstream  item_text;
		item_text << "Error in instruction item index " << count_1 << ", fid " <<
			this_item.auxiliary_id_ << " ('" <<
			this_item.field_name_ << "') with parent index " <<
			this_item.parent_index_ << ": ";
		try {
			if (this_item.parent_index_ >= ins_item_count) {
				std::ostringstream o_str;
				o_str << "The parent index (" << this_item.parent_index_ <<
					") is not less than the number of instruction items (" <<
					ins_item_count << ").";
				tmp_error_list.push_back(item_text.str() + o_str.str());
			}
			if ((count_1 + this_item.element_count_) > ins_item_count) {
				std::ostringstream o_str;
				o_str << "The item index plus the element count (" << count_1 <<
					" + " << this_item.element_count_ << " = " <<
					(count_1 + this_item.element_count_) << ") is greater "
					"than number of instruction items (" << ins_item_count << ").";
				tmp_error_list.push_back(item_text.str() + o_str.str());
			}
			if (this_item.field_operator_ == FieldOperator_None) {
				if ((this_item.dict_value_index_) &&
					(this_item.data_type_ != DataType_Template)) {
					std::ostringstream o_str;
					o_str << "The field has an operator of 'None', but the "
						"dictionary value index is " <<
						this_item.dict_value_index_ << " (should be 0).";
					tmp_error_list.push_back(item_text.str() + o_str.str());
				}
				if ((this_item.dict_value_count_) &&
					(this_item.data_type_ != DataType_Template)) {
					std::ostringstream o_str;
					o_str << "The field has an operator of 'None', but the "
						"dictionary value count is " <<
						this_item.dict_value_count_ << " (should be 0).";
					tmp_error_list.push_back(item_text.str() + o_str.str());
				}
			}
			else {
				if (!this_item.dict_value_count_) {
					std::ostringstream o_str;
					o_str << "The field has an operator of '" <<
						this_item.field_operator_ << "', but the dictionary value "
						"count is 0.";
					tmp_error_list.push_back(item_text.str() + o_str.str());
				}
				else if ((this_item.dict_value_index_ +
					this_item.dict_value_count_) > dict_value_count) {
					std::ostringstream o_str;
					o_str << "The dictionary value index plus the dictionary value "
						"count (" << this_item.dict_value_index_ << " + " <<
						this_item.dict_value_count_ << " = " <<
						(this_item.dict_value_index_ + this_item.dict_value_count_) <<
						") is greater than number of dictionary values (" <<
						dict_value_count << ").";
					tmp_error_list.push_back(item_text.str() + o_str.str());
				}
			}
		}
		catch (const std::exception &except) {
			tmp_error_list.push_back(item_text.str() + except.what());
		}
	}

	error_list.swap(tmp_error_list);

	return(error_list.empty());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsContext::CheckContext() const
{
	std::vector<std::string> error_list;

	if (CheckContext(error_list))
		return(true);

	std::copy(error_list.begin(), error_list.end(),
		std::ostream_iterator<std::string>(std::cout, "\n"));

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsContext::DumpTemplateMap()
{
	InsItemMapIterC iter_b(template_map_.begin());
	InsItemMapIterC iter_e(template_map_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		InsItem::DumpItemList(iter_b->second.second,
			&(ins_item_list_[iter_b->second.first]));
		std::cout << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsContext::DumpTemplateMapFull()
{
	InsItemMapIterC iter_b(template_map_.begin());
	InsItemMapIterC iter_e(template_map_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		InsItem::DumpItemListFull(iter_b->second.second,
			&(ins_item_list_[iter_b->second.first]), dict_value_list_);
		std::cout << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsContext::DumpTemplateMapDispatch() const
{
	std::cout << ExcContext(*this).GetFieldDispatchReport() <<
		std::endl << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsContext::DetermineTemplateIndex(
	const InsItemNameMapI &template_name_map, const InsItemList &ins_item_list,
	const char *template_name)
{
	if ((template_name == NULL) || (!(*template_name)))
		MLB::Utility::ThrowInvalidArgument("The template name to be located "
			"is " + std::string((template_name == NULL) ? "NULL." : "empty."));

	InsItemNameMapIterC iter_b(template_name_map.begin());
	InsItemNameMapIterC iter_e(template_name_map.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (!MLB::Utility::Utility_stricmp(template_name,
			ins_item_list[iter_b->second.first].field_name_.c_str()))
			return(iter_b->second.first);
	}

	return(InvalidTemplateIndex);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/StringSupport.hpp>

using namespace MLB::VFast;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		int count_1;
		for (count_1 = 1; count_1 < argc; ++count_1) {
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << "Template File: " << argv[count_1] << std::endl;
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			InsContext ins_context(argv[count_1]);
			ins_context.CheckContext();
			ins_context.DumpTemplateMapFull();
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << std::endl;
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
	Testing command line for Windows:
	------- ------- ---- --- --------
	C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20080925_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20081007_A.xml C:\DevEnv\Tests\FastCommon\Templates\ISE\ISE_fast_templates_v2.0.ORIGINAL.xml > C:\DevEnv\Tests\VFast\UnitTests\InsContext.txt

	C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20090610_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20080925_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20081007_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20090623_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20091104_A.xml C:\DevEnv\Tests\FastCommon\Templates\ISE\ISE_fast_templates_v2.0.ORIGINAL.xml > C:\DevEnv\Tests\VFast\UnitTests\InsContext.txt
*/

#endif // #ifdef TEST_MAIN

