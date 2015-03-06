// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the InsItem class.

	Revision History	:	2008-11-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/ParseCmdLineArg.hpp>

#include <VFast/InsItem.hpp>
#include <VFast/SpecException.hpp>

#include <iostream>
#include <sstream>
#include <cctype>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

namespace {

// ////////////////////////////////////////////////////////////////////////////
void SetTemplateIndex(InsItemList &item_list, std::size_t in_item_index)
{
	std::size_t  item_index    = in_item_index;
	InsItem     *this_item_ptr = &(item_list[item_index]);

	for (const InsItem *item_ptr = this_item_ptr; ; --item_index, --item_ptr) {
		if (item_ptr->data_type_ == DataType_Template) {
			this_item_ptr->template_index_ = item_index;
			return;
		}
		if (!item_index)
			break;
	}

	std::ostringstream o_str;

	o_str << "Attempt to locate the enclosing template for the field at " <<
		"index " << in_item_index << " (" << this_item_ptr->auxiliary_id_ <<
		"='" << this_item_ptr->field_name_ << "') failed.";

	MLB::Utility::ThrowLogicError(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void AddValueDict(InsItemList &item_list, const DictValue &dict_value,
	DictValueList &dict_value_list, std::size_t item_index)
{
	InsItem &item_ref = item_list[item_index];

	if (item_ref.field_operator_ != FieldOperator_Constant) {
		InsItemList::size_type  count_1;
		InsItemList::size_type  item_count = item_list.size();
		const InsItem          *item_ptr   = &(item_list[0]);
		//	Linear search, but should be sufficient to the purpose...
		for (count_1 = 0; count_1 < item_count; ++count_1, ++item_ptr) {
			if ((count_1 == item_index) ||
				(item_ptr->field_operator_ == FieldOperator_Constant))
				;
			else if ((item_ptr->field_operator_ != FieldOperator_None)   &&
				(item_ptr->dictionary_name_ == item_ref.dictionary_name_) &&
				(item_ptr->key_name_ == item_ref.key_name_)) {
				if (dict_value.IsEquivalent(
					dict_value_list[item_ptr->dict_value_index_])) {
					item_ref.dict_value_index_ = item_ptr->dict_value_index_;
					item_ref.dict_value_count_ = item_ptr->dict_value_count_;
					return;
				}
				else if (!item_ref.IsSeqLength()) {	//	Needed for Liffe XDP...
					try {
						dict_value.CheckIsEquivalent(
							dict_value_list[item_ptr->dict_value_index_]);
					}
					catch (const std::exception &except) {
						std::ostringstream o_str;
						o_str << "Incompatible field operators found in "
							"dictionary '" << item_ref.dictionary_name_ <<
							"', key name '" << item_ref.key_name_ <<
							"': Field at index " << count_1 << " in template " <<
							item_list[item_ptr->template_index_].auxiliary_id_ <<
							"='" << item_list[item_ptr->template_index_].field_name_ <<
							"' (" << item_ptr->auxiliary_id_ << "='" <<
							item_ptr->field_name_ << "') has operator '" <<
							item_ptr->field_operator_ << "', whereas a corresponding "
							"field at index " << item_index << " in template " <<
							item_list[item_ref.template_index_].auxiliary_id_ <<
							"='" << item_list[item_ref.template_index_].field_name_ <<
							"' (" << item_ref.auxiliary_id_ << "='" <<
							item_ref.field_name_ << "') has operator '" <<
							item_ref.field_operator_ << ": " << except.what();
						MLB::Utility::Rethrow(except, o_str.str());
					}
				}
			}
		}
	}

	//	Didn't find a dictionary entry for the key, so add one...
	dict_value_list.push_back(dict_value);

	item_ref.dict_value_index_ =
		static_cast<unsigned int>(dict_value_list.size()) - 1;
	item_ref.dict_value_count_ = 1;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void AddValueDict(InsItemList &item_list, const DictValue &dict_value,
	DictValueList &dict_value_list)
{
	AddValueDict(item_list, dict_value, dict_value_list, item_list.size() - 1);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ApplyFieldOperator(const MLB::XercesUtils::XmlDomElement &xml_element,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	InsItem       &item(item_list.back());
	FieldOperator  field_operator;

	if (!IsFieldOperatorString(xml_element.element_name_, field_operator))
		return(false);

	if (item.field_operator_ != FieldOperator_None)
		MLB::Utility::ThrowInvalidArgument("Multiple field operators were "
			"encountered for a field (" +
			FieldOperatorToString(item.field_operator_) + " and " +
			FieldOperatorToString(field_operator) + ").");

	if (field_operator == FieldOperator_None)
		MLB::Utility::ThrowInvalidArgument("A field operator of " +
			FieldOperatorToString(field_operator) + " was encountered.");

	if ((field_operator == FieldOperator_Tail) && (!item.IsTypeCharArray()))
		MLB::Utility::ThrowInvalidArgument("Encountered a Tail field operator "
			"for a field which is not a string or byte vector (" +
			xml_element.element_name_ + ").");

	if ((field_operator == FieldOperator_Increment) && (!item.IsTypeInt()))
		MLB::Utility::ThrowInvalidArgument("Encountered an Increment field "
			"operator for a field which is not an integral type).");

	DictValue dict_value(item.data_type_);

	if (!xml_element.HasAttribute("value")) {
		if (field_operator == FieldOperator_Constant)
			SpecExceptionStatic::ThrowException(4, "Field operator is Constant, "
				"but there is no initial value specified.");
		else if ((field_operator == FieldOperator_Default) &&
			(!item.IsOptional())) {
			if ((!item.IsDecimalChild()) ||
				(item_list[item.parent_index_].field_operator_ != FieldOperator_None) ||
				(!item_list[item.parent_index_].IsOptional()))
				SpecExceptionStatic::ThrowException(5, "Field operator is "
					"Default, but there is no initial value specified and the "
					"field does not have optional presence.");
		}
		item.ClearHasInitialValue();
		item.field_operator_ = field_operator;
	}
	else {
		item.SetHasInitialValue();
		item.field_operator_ = field_operator;
		dict_value.SetValueFromString(xml_element.GetAttributeValue("value"));
	}

	AddValueDict(item_list, dict_value, dict_value_list);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ChildIsFieldOperator(const MLB::XercesUtils::XmlDomElement &xml_element)
{
	return(IsFieldOperatorString(xml_element.element_name_));
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
InsItem::InsItem()
	:template_index_(0)
	,parent_index_(0)
	,data_type_(DataType_Invalid)
	,flags_(InsItemFlag_None)
	,auxiliary_id_(0)
	,field_operator_(FieldOperator_None)
	,item_index_(0)
	,element_count_(0)
	,element_count_compat_(0)
	,dict_value_index_(0)
	,dict_value_count_(0)
	,static_ref_index_(0)
	,field_name_()
	,dictionary_name_()
	,key_name_()
	,dict_value_ptr_(NULL)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
InsItem::InsItem(DataType data_type, unsigned int fix_tag_id,
	const std::string &field_name)
	:template_index_(0)
	,parent_index_(0)
	,data_type_(data_type)
	,flags_(InsItemFlag_None)
	,auxiliary_id_(fix_tag_id)
	,field_operator_(FieldOperator_None)
	,item_index_(0)
	,element_count_(0)
	,element_count_compat_(0)
	,dict_value_index_(0)
	,dict_value_count_(0)
	,static_ref_index_(0)
	,field_name_(field_name)
	,dictionary_name_()
	,key_name_()
	,dict_value_ptr_(NULL)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseTemplateElement(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemList &item_list,
	DictValueList &dict_value_list)
{
	if (!xml_element.IsElement("template"))
		MLB::Utility::ThrowInvalidArgument("Invalid element encountered at the "
			"template parsing level ('" + xml_element.element_name_ + "') --- "
			"'template' was expected.");

	ParseTemplateElementInternal(ins_ctx_info, xml_element, InsItemFlag_None,
		item_list, dict_value_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::operator < (const InsItem &other) const
{
	return(auxiliary_id_ < other.auxiliary_id_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypeInt() const
{
	return(MLB::VFast::IsTypeInt(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypeSInt() const
{
	return(MLB::VFast::IsTypeSInt(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypeUInt() const
{
	return(MLB::VFast::IsTypeUInt(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypeInt32() const
{
	return(MLB::VFast::IsTypeInt32(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypeInt64() const
{
	return(MLB::VFast::IsTypeInt64(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypeNumeric() const
{
	return(MLB::VFast::IsTypeNumeric(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypeCharArray() const
{
	return(MLB::VFast::IsTypeCharArray(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypePrimitive() const
{
	return(MLB::VFast::IsTypePrimitive(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::IsTypeComposite() const
{
	return(MLB::VFast::IsTypeComposite(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::GetDescriptiveText() const
{
	return("field name '" + field_name_ + "', auxiliary identifier " +
		MLB::Utility::AnyToString(auxiliary_id_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::SetDictionaryName(const std::string &src_name)
{
	dictionary_name_ = src_name;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::SetKeyName(const std::string &src_name)
{
   key_name_ = src_name;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::EmitDatum() const
{
	std::ostringstream o_str;

	o_str
		<< *this;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::EmitDatumFull(const DictValueList &dict_value_list)
	const
{
	std::ostringstream o_str;

	o_str
		<< EmitDatum() << " " << std::left
		<< std::setw(15) << 
			((dictionary_name_.size() <= 15) ?
			dictionary_name_ : (dictionary_name_.substr(0, 14) + ">")) << " "
		<< std::setw(40) <<
			((key_name_.size() <= 40) ? key_name_ :
			(key_name_.substr(0, 39) + ">"))
		<< std::right
		;

	if (field_operator_ != FieldOperator_None)
		o_str << " " <<
			((dict_value_list[dict_value_index_].IsNull()) ? "<*NULL*>" :
			dict_value_list[dict_value_index_].ValueToString());

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::InitFromParseContext(const InsCtxInfo &ins_ctx_info)
{
	if (!ins_ctx_info.dictionary_name_.empty())
		SetDictionaryName(ins_ctx_info.GetDictionaryName());

	parent_index_   = ins_ctx_info.GetParentIndex();
	template_index_ = (template_index_) ? template_index_ : parent_index_;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ProcessAttribute_Presence(const std::string &attr_value)
{
	if (!MLB::Utility::Utility_stricmp(attr_value.c_str(), "optional"))
		SetIsOptional();
	else if (!MLB::Utility::Utility_stricmp(attr_value.c_str(), "mandatory"))
		ClearIsOptional();
	else
		MLB::Utility::ThrowLogicError("The 'presence' attribute ('" +
			attr_value + "') is not one of valid values --- expected either "
			"'optional' or 'mandatory'.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool InsItem::ProcessAttribute_CharSet(const std::string &attr_value)
{
	bool is_ascii_flag = false;

	if (!MLB::Utility::Utility_stricmp(attr_value.c_str(), "ascii"))
		is_ascii_flag = true;
	else if (MLB::Utility::Utility_stricmp(attr_value.c_str(), "unicode"))
		MLB::Utility::ThrowLogicError("The 'charset' attribute  ('" +
			attr_value + "')is not one of valid values --- expected either "
			"'ascii' or 'unicode'.");

	return(is_ascii_flag);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ProcessAttribute_Dictionary(const std::string &attr_value)
{
	try {
		if (attr_value.empty())
			MLB::Utility::ThrowLogicError("The dictionary name attribute is "
				"empty.");
		SetDictionaryName(attr_value);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to set the dictionary attribute "
			"failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ProcessAttribute_Key(const std::string &attr_value)
{
	try {
		if (attr_value.empty())
			MLB::Utility::ThrowLogicError("The key name attribute is empty.");
		SetKeyName(attr_value);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to set the 'key' attribute "
			"failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::FixUpFieldFlagsDecimalMultiOp(InsItem *item_ptr)
{
	//	Process decimal fields with individual operators only...
	if ((item_ptr->data_type_ == DataType_Decimal) &&
		 ((item_ptr[1].field_operator_ != FieldOperator_None) ||
		  (item_ptr[2].field_operator_ != FieldOperator_None))) {
		if (!item_ptr->IsOptional()) {
			item_ptr[1].ClearIsOptional();
			item_ptr[1].FixUpFieldFlags();
			item_ptr[2].ClearIsOptional();
			item_ptr[2].FixUpFieldFlags();
		}
		else {
			item_ptr[1].SetIsOptional();
			item_ptr[1].FixUpFieldFlags();
			item_ptr[2].ClearIsOptional();
			item_ptr[2].FixUpFieldFlags();
		}
	}
	else
		MLB::Utility::ThrowLogicError("The member function 'InsItem::"
			"FixUpFieldFlagsDecimalMultiOp()' was invoked for a non-Decimal "
			"field (" + DataTypeToString(item_ptr->data_type_) + ").");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::FixUpFieldFlags(InsItem *item_ptr)
{
	bool field_has_pmap_bit = false;
	bool field_is_nullable  = false;

	if ((item_ptr->data_type_ == DataType_Decimal) &&
		 ((item_ptr[1].field_operator_ != FieldOperator_None) ||
		  (item_ptr[2].field_operator_ != FieldOperator_None)))
			FixUpFieldFlagsDecimalMultiOp(item_ptr);
	else {
		switch (item_ptr->field_operator_) {
			case FieldOperator_None				:
				if (item_ptr->IsOptional())
					field_is_nullable = true;
				break;
			case FieldOperator_Constant		:
				if (item_ptr->IsOptional())
					field_has_pmap_bit = true;
				break;
			case FieldOperator_Default			:
			case FieldOperator_Copy				:
			case FieldOperator_Increment		:
				if (!item_ptr->IsOptional()) {
					if ((item_ptr->data_type_ == DataType_SInt32)  ||
						 (item_ptr->data_type_ == DataType_UInt32)  ||
						 (item_ptr->data_type_ == DataType_SInt64)  ||
						 (item_ptr->data_type_ == DataType_UInt64)  ||
						 (item_ptr->data_type_ == DataType_Decimal) ||
						 (item_ptr->data_type_ == DataType_Ascii)   ||
						 (item_ptr->data_type_ == DataType_Unicode) ||
						 (item_ptr->data_type_ == DataType_ByteVector))
						field_has_pmap_bit = true;
				}
				else {
					if ((item_ptr->data_type_ == DataType_SInt32)  ||
						 (item_ptr->data_type_ == DataType_UInt32)  ||
						 (item_ptr->data_type_ == DataType_SInt64)  ||
						 (item_ptr->data_type_ == DataType_UInt64)  ||
						 (item_ptr->data_type_ == DataType_Decimal) ||
						 (item_ptr->data_type_ == DataType_Ascii)   ||
						 (item_ptr->data_type_ == DataType_Unicode) ||
						 (item_ptr->data_type_ == DataType_ByteVector)) {
						field_has_pmap_bit = true;
						field_is_nullable  = true;
					}
				}
				break;
			case FieldOperator_Delta			:
				if (item_ptr->IsOptional()) {
					if ((item_ptr->data_type_ == DataType_SInt32)  ||
						 (item_ptr->data_type_ == DataType_UInt32)  ||
						 (item_ptr->data_type_ == DataType_SInt64)  ||
						 (item_ptr->data_type_ == DataType_UInt64)  ||
						 (item_ptr->data_type_ == DataType_Decimal) ||
						 (item_ptr->data_type_ == DataType_Ascii)   ||
						 (item_ptr->data_type_ == DataType_Unicode) ||
						 (item_ptr->data_type_ == DataType_ByteVector))
						field_is_nullable  = true;
				}
				break;
			case FieldOperator_Tail				:
				if (!item_ptr->IsOptional()) {
					if ((item_ptr->data_type_ == DataType_Ascii)   ||
						 (item_ptr->data_type_ == DataType_Unicode) ||
						 (item_ptr->data_type_ == DataType_ByteVector))
						field_has_pmap_bit = true;
				}
				else {
					if ((item_ptr->data_type_ == DataType_Ascii)   ||
						 (item_ptr->data_type_ == DataType_Unicode) ||
						 (item_ptr->data_type_ == DataType_ByteVector)) {
						field_has_pmap_bit = true;
						field_is_nullable  = true;
					}
				}
				break;
			case FieldOperator_Count			:
			default									:
				MLB::Utility::ThrowLogicError("Attempt to fix-up field flags for "
					"an inappropriate type (" +
					DataTypeToString(item_ptr->data_type_) + ").");
		}
	}

	if (field_has_pmap_bit)
		item_ptr->SetHasPMapBit();
	else
		item_ptr->ClearHasPMapBit();

	if (field_is_nullable)
		item_ptr->SetIsNullable();
	else
		item_ptr->ClearIsNullable();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::FixUpFieldFlags()
{
	if (data_type_ == DataType_Decimal)
		MLB::Utility::ThrowLogicError("The non-static instance version of the "
			"member function 'InsItem::FixUpFieldFlags()' was invoked for "
			"a Decimal field.");

	FixUpFieldFlags(this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int InsItem::GetAuxiliaryId(
	const MLB::XercesUtils::XmlDomElement &xml_element, bool throw_on_error)
{
	unsigned int tmp_id = 0;

	try {
		std::string str_id(xml_element.GetAttributeValue("id", throw_on_error));
		if (!str_id.empty())
			MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric(str_id, tmp_id);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to parse an instruction item "
			"id failed: " + std::string(except.what()));
	}

	return(tmp_id);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::DumpItemList(const InsItemList &item_list)
{
	DumpItemList(item_list.size(), &(item_list[0]));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::DumpItemList(std::size_t item_count, const InsItem *item_list)
{
	std::cout << EmitFillLine('=') << std::endl;
	std::cout << EmitHeaderText() << std::endl;
	std::cout << EmitFillLine('-') << std::endl;

	while (item_count--)
		std::cout << (item_list++)->EmitDatum() << std::endl;

	std::cout << EmitFillLine('=') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::DumpItemListFull(const InsItemList &item_list,
	const DictValueList &dict_value_list)
{
	DumpItemListFull(item_list.size(), &(item_list[0]), dict_value_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::DumpItemListFull(std::size_t item_count, const InsItem *item_list,
	const DictValueList &dict_value_list)
{
	std::cout << EmitFillLineFull('=') << std::endl;
	std::cout << EmitHeaderTextFull() << std::endl;
	std::cout << EmitFillLineFull('-') << std::endl;

	while (item_count--) {
		std::cout << item_list->EmitDatumFull(dict_value_list) << std::endl;
		++item_list;
	}

	std::cout << EmitFillLineFull('=') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::EmitHeaderText()
{
	std::ostringstream o_str;

	o_str
		<< std::left
		<< std::setw( 2) << "In"        << " "
		<< std::setw( 6) << "Aux Id"    << " "
		<< std::setw(40) << "Name"      << " "
		<< std::setw( 5) << "Index"     << " "
		<< std::setw(14) << "Data Type" << " "
		<< std::setw( 5) << "Count"     << " "
		<< std::setw(13) << "Flags"     << " "
		<< std::setw(10) << "Operator"  << " "
		<< std::setw( 6) << "DIndex"    << " "
		<< std::setw( 6) << "DCount"
		<< std::right
		;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::EmitFillLineHeader()
{
	return(EmitFillLine('-'));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::EmitFillLine(int fill_char)
{
	std::ostringstream o_str;

	o_str
		<< std::setfill(static_cast<char>(fill_char))
		<< std::setw( 2) << "" << " "
		<< std::setw( 6) << "" << " "
		<< std::setw(40) << "" << " "
		<< std::setw( 5) << "" << " "
		<< std::setw(14) << "" << " "
		<< std::setw( 5) << "" << " "
		<< std::setw(13) << "" << " "
		<< std::setw(10) << "" << " "
		<< std::setw( 6) << "" << " "
		<< std::setw( 6) << ""
		<< std::setfill(' ')
		;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::EmitHeaderTextFull()
{
	std::ostringstream o_str;

	o_str
		<< EmitHeaderText() << " "
		<< std::left
		<< std::setw(15) << "Dictionary Name"  << " "
		<< std::setw(40) << "Key Name"         << " "
		<< std::setw(16) << "Dictionary Value" << " "
		<< std::right
		;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::EmitFillLineHeaderFull()
{
	return(EmitFillLineFull('-'));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string InsItem::EmitFillLineFull(int fill_char)
{
	std::ostringstream o_str;

	o_str
		<< EmitFillLine(fill_char) << " "
		<< std::setfill(static_cast<char>(fill_char))
		<< std::setw(15) << "" << " "
		<< std::setw(40) << "" << " "
		<< std::setw(16) << ""
		<< std::setfill(' ')
		;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseTemplateElementInternal(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	try {
		std::size_t item_index       = item_list.size();
		std::size_t dict_value_count = dict_value_list.size();
		if (xml_element.IsElement("string"))
			ParseElement_String(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("byteVector"))
			ParseElement_ByteVector(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("int32"))
			ParseElement_SInt32(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("uint32"))
			ParseElement_UInt32(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("int64"))
			ParseElement_SInt64(ins_ctx_info, xml_element, parent_flags,
				item_list,  dict_value_list);
		else if (xml_element.IsElement("uint64"))
			ParseElement_UInt64(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("decimal"))
			ParseElement_Decimal(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("mantissa"))
			ParseElement_DecimalMantissa(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("exponent"))
			ParseElement_DecimalExponent(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("length"))
			ParseElement_Length(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("sequence"))
			ParseElement_Sequence(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("group"))
			ParseElement_Group(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("template"))
			ParseElement_Template(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("templateref"))
			ParseElement_TemplateRef(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if (xml_element.IsElement("typeref"))
			ParseElement_TypeRef(ins_ctx_info, xml_element, parent_flags,
				item_list, dict_value_list);
		else if ((!xml_element.IsElement("#text")) &&
			(!xml_element.IsElement("#comment")))
			MLB::Utility::ThrowLogicError("The specified instruction context "
				"element name is invalid.");
		InsItem *item_ptr = &(item_list[item_index]);
		item_ptr->element_count_ =
			static_cast<unsigned int>(item_list.size()) - item_index;
		if (item_ptr->data_type_ == DataType_Template) {
			item_ptr->dict_value_index_ =
				static_cast<unsigned int>(dict_value_count);
			item_ptr->dict_value_count_ =
				static_cast<unsigned int>(dict_value_list.size()) -
				dict_value_count;
		}
		item_ptr->element_count_compat_ = item_ptr->element_count_;
		for ( ; item_index < item_list.size(); ++item_index) {
			if (item_list[item_index].element_count_ > 1)
				--item_ptr->element_count_compat_;
		}
		if ((item_ptr->data_type_ == DataType_Template) &&
			(item_ptr->element_count_compat_ == 1) &&
			(item_ptr->element_count_ == 1))
			item_ptr->element_count_compat_ = 0;
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to parse element '" +
			xml_element.element_name_ + "' failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_SInt32(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	ParseElement_Integral<Type_SInt32>(ins_ctx_info, xml_element, parent_flags,
		item_list, dict_value_list, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_UInt32(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	ParseElement_Integral<Type_UInt32>(ins_ctx_info, xml_element, parent_flags,
		item_list, dict_value_list, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_SInt64(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	ParseElement_Integral<Type_SInt64>(ins_ctx_info, xml_element, parent_flags,
		item_list, dict_value_list, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_UInt64(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	ParseElement_Integral<Type_UInt64>(ins_ctx_info, xml_element, parent_flags,
		item_list, dict_value_list, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_Decimal(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem(DataType_Decimal,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", true)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else if (iter_b->first == "dictionary") {
				item_ptr->ProcessAttribute_Dictionary(iter_b->second);
				my_context.dictionary_name_ = item_ptr->dictionary_name_;
			}
			else if (iter_b->first == "key") {
				item_ptr->ProcessAttribute_Key(iter_b->second);
			}
			else if (iter_b->first == "xmlns") {
				;	//	CODE NOTE: No-op at present...
			}
			else if (iter_b->first == "presence") {
				item_ptr->ProcessAttribute_Presence(iter_b->second);
			}
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	if (item_ptr->key_name_.empty())
		item_ptr->SetKeyName(item_ptr->field_name_);

	// //////////////////////////////////////////////////////////////////////
	//	Determine field operator...
	if (!child_list.empty()) {
		DictValue             value_datum;
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment"))
				continue; // Ignore...
			else if (ApplyFieldOperator(*iter_b, item_list, dict_value_list))
				continue;	// Done...
		}
	}
	// //////////////////////////////////////////////////////////////////////

	unsigned int child_count;
	std::size_t  exponent_index = 0;
	std::size_t  mantissa_index = 0;

	// //////////////////////////////////////////////////////////////////////
	//	Find or construct the exponent...
	child_count = xml_element.CountChildren("exponent");
	if (child_count) {
		if (child_count != 1)
			MLB::Utility::ThrowInvalidArgument("A maximium of one 'exponent' "
				"element is permitted within a Decimal instruction, but " +
				MLB::Utility::AnyToString(child_count) + " were found.");
		ParseTemplateElementInternal(my_context,
			xml_element.GetChildRef("exponent"), item_ptr->flags_, item_list,
			dict_value_list);
		exponent_index = item_list.size() - 1;
		item_ptr       = &(item_list[item_index]);
		if (item_list.back().field_operator_ != FieldOperator_None) {
			const DictValue &dict_ref =
				dict_value_list[item_list.back().dict_value_index_];
			if ((dict_ref.sint_32_ < -63) || (dict_ref.sint_32_ >  63)) {
				std::ostringstream o_str;
				o_str << "The specified Decimal exponent (" << dict_ref.sint_32_ <<
					") is outside the valid range for exponent fields (-63 to 63, "
					"inclusive).";
				MLB::Utility::ThrowLogicError(o_str.str());
			}
		}
	}
	else {
		item_list.push_back(InsItem(DataType_SInt32, 0, ""));
		exponent_index              = item_list.size() - 1;
		item_ptr                    = &(item_list[item_index]);
		item_list.back().flags_ = static_cast<InsItemFlag>(
			item_list.back().flags_ | (item_ptr->flags_ & InsItemFlag_IsIn));
		item_list.back().SetIsDecimalExp();
		item_list.back().SetIsGenerated();
		item_list.back().SetDictionaryName(item_ptr->dictionary_name_);
		item_list.back().SetKeyName(std::string("<EXP>::") +
			item_ptr->key_name_);
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Find or construct the mantissa...
	child_count = xml_element.CountChildren("mantissa");
	if (child_count) {
		if (child_count != 1)
			MLB::Utility::ThrowInvalidArgument("A maximium of one 'mantissa' "
				"element is permitted within a Decimal instruction, but " +
				MLB::Utility::AnyToString(child_count) + " were found.");
		ParseTemplateElementInternal(my_context,
			xml_element.GetChildRef("mantissa"), item_ptr->flags_, item_list,
			dict_value_list);
		mantissa_index = item_list.size() - 1;
		item_ptr       = &(item_list[item_index]);
	}
	else {
		item_list.push_back(InsItem(DataType_SInt64, 0, ""));
		mantissa_index          = item_list.size() - 1;
		item_ptr                = &(item_list[item_index]);
		item_list.back().flags_ = static_cast<InsItemFlag>(
			item_list.back().flags_ | (item_ptr->flags_ & InsItemFlag_IsIn));
		item_list.back().SetIsDecimalMan();
		item_list.back().SetIsGenerated();
		item_list.back().SetDictionaryName(item_ptr->dictionary_name_);
		item_list.back().SetKeyName(std::string("<MAN>::") +
			item_ptr->key_name_);
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Examine the other children...
	if (!child_list.empty()) {
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment") ||
				ChildIsFieldOperator(*iter_b) ||
				iter_b->IsElement("exponent") || iter_b->IsElement("mantissa"))
				continue; // Ignore...
			else
				MLB::Utility::ThrowLogicError("An unknown child element element "
					"was encountered ('" + iter_b->element_name_ + "').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	//	CODE NOTE: Also have to normalize.

	/*
		IMPLEMENTATION NOTE: Must call the static overload of the member
		function FixUpFieldFlags() because it references the two fields
		beyond it in the item list (the exponent and mantissa).
	*/
	FixUpFieldFlags(item_ptr);

	if ((item_ptr->field_operator_   != FieldOperator_None) &&
		 (item_ptr[1].field_operator_ == FieldOperator_None) &&
		 (item_ptr[2].field_operator_ == FieldOperator_None)) {
		item_ptr[1].field_operator_ = item_ptr->field_operator_;
		item_ptr[2].field_operator_ = item_ptr->field_operator_;
		/*
			CODE NOTE: Will need modification to support initial values extracted
			from the decimal initial value.
		*/
		AddValueDict(item_list, DictValue(DataType_SInt32), dict_value_list,
			item_index + 1);
		AddValueDict(item_list, DictValue(DataType_SInt64), dict_value_list,
			item_index + 2);
	}
	else if ((item_ptr->data_type_ == DataType_Decimal) &&
		item_ptr->IsOptional()) {
		item_ptr[1].SetIsOptional();
		item_ptr[1].SetIsNullable();
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_DecimalExponent(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	parent_flags = static_cast<InsItemFlag>(
		parent_flags | InsItemFlag_IsDecimalExp);

	ParseElement_Integral<Type_SInt32>(ins_ctx_info, xml_element, parent_flags,
		item_list, dict_value_list, false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_DecimalMantissa(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	parent_flags = static_cast<InsItemFlag>(
		parent_flags | InsItemFlag_IsDecimalMan);

	ParseElement_Integral<Type_SInt64>(ins_ctx_info, xml_element, parent_flags,
		item_list, dict_value_list, false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_String(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem(DataType_Ascii,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", true)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else if (iter_b->first == "dictionary") {
				item_ptr->ProcessAttribute_Dictionary(iter_b->second);
				my_context.dictionary_name_ = item_ptr->dictionary_name_;
			}
			else if (iter_b->first == "key")
				item_ptr->ProcessAttribute_Key(iter_b->second);
			else if (iter_b->first == "xmlns")
				;	//	CODE NOTE: No-op at present...
			else if (iter_b->first == "presence")
				item_ptr->ProcessAttribute_Presence(iter_b->second);
			else if (iter_b->first == "charset") {
				if (!item_ptr->ProcessAttribute_CharSet(iter_b->second))
					item_ptr->data_type_ = DataType_Unicode;
			}
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	if (item_ptr->key_name_.empty())
		item_ptr->SetKeyName(item_ptr->field_name_);

	// //////////////////////////////////////////////////////////////////////
	//	Determine field operator...
	if (!child_list.empty()) {
		DictValue             value_datum;
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment"))
				continue; // Ignore...
			else if (ApplyFieldOperator(*iter_b, item_list, dict_value_list))
				continue;	// Done...
		}
	}
	// //////////////////////////////////////////////////////////////////////

	unsigned int child_count;
	std::size_t  length_index = 0;

	// //////////////////////////////////////////////////////////////////////
	//	Find or construct the length for Unicode strings...
	child_count = xml_element.CountChildren("length");
	if (child_count) {
		if (item_ptr->data_type_ != DataType_Unicode)
			MLB::Utility::ThrowLogicError("A non-Unicode string field with "
				"a 'length' element was encountered.");
		else if (child_count != 1)
			MLB::Utility::ThrowInvalidArgument("A maxmium of one 'length' "
				"element is permitted within a Unicode string instruction, but " +
				MLB::Utility::AnyToString(child_count) + " were found.");
		ParseTemplateElementInternal(my_context,
			xml_element.GetChildRef("length"), item_ptr->flags_, item_list,
			dict_value_list);
		length_index = item_list.size() - 1;
		item_ptr     = &(item_list[item_index]);
	}
	else if (item_ptr->data_type_ == DataType_Unicode) {
		item_list.push_back(InsItem(DataType_UInt32, 0, ""));
		length_index            = item_list.size() - 1;
		item_ptr                = &(item_list[item_index]);
		item_list.back().flags_ = static_cast<InsItemFlag>(
			item_list.back().flags_ | (item_ptr->flags_ & InsItemFlag_IsIn));
		item_list.back().SetIsByteLength();
		item_list.back().SetIsGenerated();
		item_list.back().SetDictionaryName(item_ptr->dictionary_name_);
		item_list.back().SetKeyName(std::string("<ULN>::") +
			item_ptr->key_name_);
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Examine the other children...
	if (!child_list.empty()) {
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment") ||
				ChildIsFieldOperator(*iter_b) || iter_b->IsElement("length"))
				continue; // Ignore...
			else
				MLB::Utility::ThrowLogicError("An unknown/invalid field operator "
					"was encountered for a field (" + iter_b->element_name_ + ").");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	item_ptr->FixUpFieldFlags();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_ByteVector(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem(DataType_ByteVector,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", true)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else if (iter_b->first == "dictionary") {
				item_ptr->ProcessAttribute_Dictionary(iter_b->second);
				my_context.dictionary_name_ = item_ptr->dictionary_name_;
			}
			else if (iter_b->first == "key")
				item_ptr->ProcessAttribute_Key(iter_b->second);
			else if (iter_b->first == "xmlns")
				;	//	CODE NOTE: No-op at present...
			else if (iter_b->first == "presence")
				item_ptr->ProcessAttribute_Presence(iter_b->second);
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	if (item_ptr->key_name_.empty())
		item_ptr->SetKeyName(item_ptr->field_name_);

	// //////////////////////////////////////////////////////////////////////
	//	Determine field operator...
	if (!child_list.empty()) {
		DictValue             value_datum;
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment"))
				continue; // Ignore...
			else if (ApplyFieldOperator(*iter_b, item_list, dict_value_list))
				continue;	// Done...
		}
	}
	// //////////////////////////////////////////////////////////////////////

	unsigned int child_count;
	std::size_t  length_index = 0;

	// //////////////////////////////////////////////////////////////////////
	//	Find or construct the length for byte vectors...
	child_count = xml_element.CountChildren("length");
	if (child_count) {
		if (child_count != 1)
			MLB::Utility::ThrowInvalidArgument("A maxmium of one 'length' "
				"element is permitted within a byte vector instruction, but " +
				MLB::Utility::AnyToString(child_count) + " were found.");
		ParseTemplateElementInternal(my_context,
			xml_element.GetChildRef("length"), item_ptr->flags_, item_list,
			dict_value_list);
		length_index = item_list.size() - 1;
		item_ptr     = &(item_list[item_index]);
	}
	else {
		item_list.push_back(InsItem(DataType_UInt32, 0, ""));
		length_index            = item_list.size() - 1;
		item_ptr                = &(item_list[item_index]);
		item_list.back().flags_ = static_cast<InsItemFlag>(
			item_list.back().flags_ | (item_ptr->flags_ & InsItemFlag_IsIn));
		item_list.back().SetIsByteLength();
		item_list.back().SetIsGenerated();
		item_list.back().SetDictionaryName(item_ptr->dictionary_name_);
		item_list.back().SetKeyName(std::string("<VLN>::") +
			item_ptr->key_name_);
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Examine the other children...
	if (!child_list.empty()) {
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment") ||
				ChildIsFieldOperator(*iter_b) || iter_b->IsElement("length"))
				continue; // Ignore...
			else
				MLB::Utility::ThrowLogicError("An unknown/invalid field operator "
					"was encountered for a field (" + iter_b->element_name_ + ").");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	item_ptr->FixUpFieldFlags();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_Length(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	using namespace MLB::XercesUtils;

	/*
		\brief Sequence lengths are of type \e UInt32 .

		\note Reference FAST Specification 1.1 dated 2006-12-20, section
		6.2.5, page 12.
	*/
	item_list.push_back(InsItem(DataType_UInt32,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", true)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	if (item_list[ins_ctx_info.parent_index_].data_type_ == DataType_Sequence)
		item_ptr->SetIsSeqLength();
	else if (item_list[ins_ctx_info.parent_index_].data_type_ ==
		DataType_Unicode)
		item_ptr->SetIsByteLength();
	else if (item_list[ins_ctx_info.parent_index_].data_type_ ==
		DataType_ByteVector)
		item_ptr->SetIsByteLength();
	else
		MLB::Utility::ThrowLogicError("Attempt to parse a 'length' instruction "
			"for a parent instruction which is not 'Sequence', 'Unicode' or "
			"'ByteVector'.");

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else if (iter_b->first == "presence") {
				item_ptr->ProcessAttribute_Presence(iter_b->second);
			}
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	if (item_ptr->key_name_.empty())
		item_ptr->SetKeyName(item_ptr->field_name_);

	// //////////////////////////////////////////////////////////////////////
	//	Determine field operator...
	if (!child_list.empty()) {
		DictValue             value_datum;
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment"))
				continue; // Ignore...
			else if (ApplyFieldOperator(*iter_b, item_list, dict_value_list))
				continue;	// Done...
			else
				MLB::Utility::ThrowLogicError("An unknown/invalid field operator "
					"was encountered for a field (" + iter_b->element_name_ + ").");
		}
	}
	// //////////////////////////////////////////////////////////////////////
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_Group(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem(DataType_Group,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", true)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else if (iter_b->first == "dictionary") {
				item_ptr->ProcessAttribute_Dictionary(iter_b->second);
				my_context.dictionary_name_ = item_ptr->dictionary_name_;
			}
			else if (iter_b->first == "xmlns") {
				;	//	CODE NOTE: No-op at present...
			}
			else if (iter_b->first == "presence")
				item_ptr->ProcessAttribute_Presence(iter_b->second);
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	if (!child_list.empty()) {
		InsItemFlag tmp_flags = static_cast<InsItemFlag>(item_ptr->flags_ |
			InsItemFlag_IsInGroup);
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment"))
				continue; // Ignore...
			/*
				CODE NOTE: We here hack typeRef to use as a dictionary name
				which is our kludgery to get ISE working...
			*/
			else if (iter_b->IsElement("typeref")) {
				InsItem typeref_item(DataType_Invalid,
					GetAuxiliaryId(xml_element, false),
					iter_b->GetAttributeValue("name", true));
/*
	CODE NOTE: Application name is not (yet?) implemented.
				item_ptr->ProcessAttribute_ApplicationName(typeref_item.field_name_);
				application_name_ = item_ptr->application_name_;
*/
				my_context.SetApplicationName(typeref_item.field_name_);
			}
			else if (iter_b->IsElement("template"))
				MLB::Utility::ThrowLogicError("A template instruction was "
					"encountered within a group instruction.");
			else {
				ParseTemplateElementInternal(my_context, *iter_b, tmp_flags,
					item_list, dict_value_list);
				item_ptr = &(item_list[item_index]);
			}
		}
	}

	//	A group has to have fields inside...
	if (item_index == (item_list.size() - 1)) {
		std::ostringstream o_str;
		o_str << "No elements were found within a '" <<
			xml_element.element_name_ << "' element.";
		MLB::Utility::ThrowLogicError(o_str);
	}

	std::size_t count_1;
	std::size_t item_list_count = item_list.size();

	for (count_1 = (item_index + 1); count_1 < item_list_count; ++count_1) {
		//	Mark the child field as being in a group...
		item_list[count_1].SetIsInGroup();
	}

	if (item_ptr->IsOptional()) {
		item_ptr->SetHasPMapBit();
		item_ptr->SetAggNeedsPMapBit();
	}
	else {
		item_ptr->ClearHasPMapBit();
		item_ptr->ClearAggNeedsPMapBit();
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_TemplateRef(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList & /* dict_value_list */)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem((xml_element.HasAttribute("name")) ?
		DataType_TemplateRefStatic : DataType_TemplateRefDynamic,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", false)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	if (!child_list.empty()) {
		std::ostringstream o_str;
		o_str << "A " << xml_element.element_name_ + " element was found to "
			"have " << static_cast<unsigned int>(child_list.size()) <<
			" child element" << ((child_list.size() > 1) ? "s" : "") <<
			" within it --- child elements are not permitted in this context.";
		MLB::Utility::ThrowLogicError(o_str.str());
	}

	item_ptr->element_count_ = 1;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_TypeRef(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList & /* dict_value_list */)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem(DataType_Invalid,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", true)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	//	CODE NOTE: Obsolete. To be removed.
	if (!child_list.empty()) {
		std::ostringstream o_str;
		o_str << "A " << xml_element.element_name_ + " element was found to "
			"have " << static_cast<unsigned int>(child_list.size()) <<
			" child element" << ((child_list.size() > 1) ? "s" : "") <<
			" within it --- child elements are not permitted in this context.";
		MLB::Utility::ThrowLogicError(o_str.str());
	}

	item_ptr->element_count_ = 1;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_Template(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem(DataType_Template,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", true)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

/*
	CODE NOTE: FIX ME!!!
	my_context.template_id_ = item_ptr->auxiliary_id_;
*/

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else if (iter_b->first == "dictionary") {
				item_ptr->ProcessAttribute_Dictionary(iter_b->second);
				my_context.dictionary_name_ = item_ptr->dictionary_name_;
			}
			else if (iter_b->first == "key") {
				item_ptr->ProcessAttribute_Key(iter_b->second);
			}
			else if (iter_b->first == "xmlns") {
				;	//	CODE NOTE: No-op at present...
			}
			else if (iter_b->first == "presence")
				MLB::Utility::ThrowLogicError("The 'presence' attribute is not "
					"appropriate for a '" + xml_element.element_name_ + "').");
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	if (!child_list.empty()) {
		InsItemFlag tmp_flags = item_ptr->flags_;
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment"))
				continue; // Ignore...
			else if (iter_b->IsElement("length"))
				MLB::Utility::ThrowLogicError("A '" + iter_b->element_name_ +
					"' element was encountered inside of a '" +
					xml_element.element_name_ + "' element --- expected to find '" +
					iter_b->element_name_ + "' only inside of 'sequence' elements.");
			/*
				CODE NOTE: We here hack typeRef to use as a dictionary name
				which is our kludgery to get ISE working...
			*/
			else if (iter_b->IsElement("typeref")) {
				InsItem typeref_item(DataType_Invalid,
					GetAuxiliaryId(xml_element, false),
					iter_b->GetAttributeValue("name", true));
/*
	CODE NOTE: Application name is not (yet?) implemented.
				item_ptr->ProcessAttribute_ApplicationName(typeref_item.field_name_);
				application_name_ = item_ptr->application_name_;
*/
				my_context.SetApplicationName(typeref_item.field_name_);
			}
			else {
				ParseTemplateElementInternal(my_context, *iter_b, tmp_flags,
					item_list, dict_value_list);
				item_ptr = &(item_list[item_index]);
			}
		}
	}

	std::size_t count_1;
	std::size_t item_list_count = item_list.size();

	for (count_1 = item_index; count_1 < item_list_count; ++count_1)
		item_list[count_1].template_index_ =
			static_cast<unsigned int>(item_index);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void InsItem::ParseElement_Sequence(const InsCtxInfo &ins_ctx_info,
	const MLB::XercesUtils::XmlDomElement &xml_element, InsItemFlag parent_flags,
	InsItemList &item_list, DictValueList &dict_value_list)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem(DataType_Sequence,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", true)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags & InsItemFlag_IsIn);

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else if (iter_b->first == "dictionary") {
				item_ptr->ProcessAttribute_Dictionary(iter_b->second);
				my_context.dictionary_name_ = item_ptr->dictionary_name_;
			}
			else if (iter_b->first == "key") {
				item_ptr->ProcessAttribute_Key(iter_b->second);
			}
			else if (iter_b->first == "xmlns") {
				;	//	CODE NOTE: No-op at present...
			}
			else if (iter_b->first == "presence")
				item_ptr->ProcessAttribute_Presence(iter_b->second);
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	std::size_t sequence_length_index = 0;

	if (!child_list.empty()) {
		InsItemFlag tmp_flags = static_cast<InsItemFlag>(item_ptr->flags_ |
			InsItemFlag_IsInSequence);
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment"))
				continue; // Ignore...
			else if (iter_b->IsElement("length")) {
				if (sequence_length_index)
					MLB::Utility::ThrowLogicError("Multiple '" +
						iter_b->element_name_ + "' elements were encountered within "
						"a '" + xml_element.element_name_ + "' element.");
				if (item_index != (item_list.size() - 1))
					MLB::Utility::ThrowLogicError("A '" + iter_b->element_name_ +
						"' element which is not the first field was found in a " +
						xml_element.element_name_ + "'.");
				ParseTemplateElementInternal(my_context, *iter_b, item_ptr->flags_,
					item_list, dict_value_list);
				item_ptr              = &(item_list[item_index]);
				sequence_length_index = item_list.size() - 1;
			}
			/*
				CODE NOTE: We here hack typeRef to use as a dictionary name
				which is our kludgery to get ISE working...
			*/
			else if (iter_b->IsElement("typeref")) {
				InsItem typeref_item(DataType_Invalid,
					GetAuxiliaryId(xml_element, false),
					iter_b->GetAttributeValue("name", true));
/*
	CODE NOTE: Application name is not (yet?) implemented.
				item_ptr->ProcessAttribute_ApplicationName(typeref_item.field_name_);
				application_name_ = item_ptr->application_name_;
*/
				my_context.SetApplicationName(typeref_item.field_name_);
			}
			else {
				/*
					If no length field has yet been encountered, insert one...
				*/
				if (!sequence_length_index) {
					item_list.push_back(InsItem(DataType_UInt32, 0, ""));
					sequence_length_index   = item_list.size() - 1;
					item_ptr                = &(item_list[item_index]);
					item_list.back().flags_ = static_cast<InsItemFlag>(
						item_list.back().flags_ |
						(item_ptr->flags_ & InsItemFlag_IsIn));
					item_list.back().SetIsSeqLength();
					item_list.back().SetIsGenerated();
				}
				ParseTemplateElementInternal(my_context, *iter_b, tmp_flags,
					item_list, dict_value_list);
				item_ptr = &(item_list[item_index]);
			}
		}
	}

	//	A sequence has to have fields inside...
	//	Can happen if no fields were specified within a sequence...
	if (sequence_length_index == (item_list.size() - 1)) {
		std::ostringstream o_str;
		o_str << "No elements were found within a '" <<
			xml_element.element_name_ << "' element.";
		MLB::Utility::ThrowLogicError(o_str);
	}

	if (item_ptr->IsOptional())
		item_list[sequence_length_index].SetIsOptional();
	else
		item_list[sequence_length_index].ClearIsOptional();

	item_list[sequence_length_index].FixUpFieldFlags();

	std::size_t count_1;
	std::size_t item_list_count = item_list.size();

	for (count_1 = sequence_length_index; count_1 < item_list_count; ++count_1){
		//	Mark the child field as being in a sequence...
		item_list[count_1].SetIsInSequence();
		//	If any field needs a PMap bit, then the sequence as a whole does...
/*
	CODE NOTE: The old code included the sequence length field. This doesn't
	work for Liffe XDP as their length fields often have operators and default
	values. To be removed.
		if (item_list[count_1].HasPMapBit())
			item_ptr->SetAggNeedsPMapBit();
*/
		if ((count_1 > sequence_length_index) && item_list[count_1].HasPMapBit())
			item_ptr->SetAggNeedsPMapBit();
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename IntegralType>
	void InsItem::ParseElement_Integral(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemList &item_list,
		DictValueList &dict_value_list, bool name_required_flag)
{
	using namespace MLB::XercesUtils;

	item_list.push_back(InsItem(TypeToTypeEnum<IntegralType>::value,
		GetAuxiliaryId(xml_element, false),
		xml_element.GetAttributeValue("name", name_required_flag)));

	std::size_t  item_index =   item_list.size() - 1;
	InsItem     *item_ptr   = &(item_list[item_index]);

	item_ptr->flags_ = static_cast<InsItemFlag>(parent_flags &
		(InsItemFlag_IsIn | InsItemFlag_IsDecimalChild));

	item_ptr->InitFromParseContext(ins_ctx_info);
	SetTemplateIndex(item_list, item_index);

	InsCtxInfo my_context(ins_ctx_info);
	my_context.SetParentIndex(item_index);

	const XmlDomElementList    &child_list(xml_element.child_list_);
	const XmlDomElementAttrMap &attr_map(xml_element.attribute_map_);

	// //////////////////////////////////////////////////////////////////////
	//	Examine the attributes...
	if (!attr_map.empty()) {
		XmlDomElementAttrMapIterC iter_b(attr_map.begin());
		XmlDomElementAttrMapIterC iter_e(attr_map.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((iter_b->first == "name") ||
				(iter_b->first == "#text") ||
				(iter_b->first == "#comment"))
				; // Ignore...
			else if (iter_b->first == "id")
				item_ptr->SetHasFieldId();
			else if (iter_b->first == "dictionary") {
				item_ptr->ProcessAttribute_Dictionary(iter_b->second);
				my_context.dictionary_name_ = item_ptr->dictionary_name_;
			}
			else if (iter_b->first == "key") {
				item_ptr->ProcessAttribute_Key(iter_b->second);
			}
			else if (iter_b->first == "xmlns") {
				;	//	CODE NOTE: No-op at present...
			}
			else if (iter_b->first == "presence") {
				item_ptr->ProcessAttribute_Presence(iter_b->second);
			}
			else
				MLB::Utility::ThrowLogicError("An unknown attribute was "
					"encountered ('" + iter_b->first + "' = '" + iter_b->second +
					"').");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Fix-up key names, if required...
	if (item_ptr->key_name_.empty()) {
		if (!item_ptr->field_name_.empty())
			item_ptr->SetKeyName(item_ptr->field_name_);
		else {
#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4127)
#endif // #ifdef _Windows
			if (item_list[item_ptr->parent_index_].data_type_ == DataType_Decimal){
				if (TypeToTypeEnum<IntegralType>::value == DataType_SInt32)
					item_ptr->SetKeyName("<EXP>::" +
						item_list[item_ptr->parent_index_].key_name_);
				else if (TypeToTypeEnum<IntegralType>::value == DataType_SInt64)
					item_ptr->SetKeyName("<MAN>::" +
						item_list[item_ptr->parent_index_].key_name_);
				else
					MLB::Utility::ThrowLogicError("Invalid instruction type "
						"encountered within a Decimal instruction ('" +
						DataTypeToString(item_ptr->data_type_) + "').");
			}
			else if (item_list[item_ptr->parent_index_].data_type_ ==
				DataType_Sequence) {
				if (TypeToTypeEnum<IntegralType>::value == DataType_UInt32)
					item_ptr->SetKeyName("<SLN>::" +
						item_list[item_ptr->parent_index_].key_name_);
				else
					MLB::Utility::ThrowLogicError("Invalid instruction type "
						"encountered within the length instruction of a Sequence ('" +
						DataTypeToString(item_ptr->data_type_) + "').");
			}
			else
				MLB::Utility::ThrowLogicError("Unexpected instruction type "
					"encountered within a " +
					DataTypeToString(item_list[item_ptr->parent_index_].
					data_type_) + " (" +
					DataTypeToString(item_ptr->data_type_) + "').");
#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows
		}
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Determine field operator...
	if (!child_list.empty()) {
		DictValue             value_datum;
		XmlDomElementListIterC iter_b(child_list.begin());
		XmlDomElementListIterC iter_e(child_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if (iter_b->IsElement("#text") || iter_b->IsElement("#comment"))
				continue; // Ignore...
			else if (ApplyFieldOperator(*iter_b, item_list, dict_value_list))
				continue;	// Done...
			else
				MLB::Utility::ThrowLogicError("An unknown/invalid field operator "
					"was encountered for a field (" + iter_b->element_name_ + ").");
		}
	}
	// //////////////////////////////////////////////////////////////////////

	item_ptr->FixUpFieldFlags();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const InsItem &datum)
{
	o_str
		<< ((datum.IsInSequence())    ? "S" : " ")
		<< ((datum.IsInGroup())       ? "G" : " ")
		<< " "
		<< std::setw(6) << datum.auxiliary_id_ << " "
		<< std::setw(40) << std::left << datum.field_name_ << std::right << " "
		<< std::setw(5) << datum.item_index_ << " "
		<< std::left
		<< std::setw(14) << DataTypeToString(datum.data_type_) <<
			std::right << " "
		<< std::setw(5) << datum.element_count_ << " "
		<< std::setw(13) << InsItemFlagBitsToString(datum.flags_) << " "
		<< std::left << std::setw(10) << datum.field_operator_ << " " <<
			std::right
		<< std::setw( 6) << datum.dict_value_index_ << " "
		<< std::setw( 6) << datum.dict_value_count_
		;


	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#ifdef TEST_MAIN

#include <XercesUtils/XercesContext.hpp>

using namespace MLB::VFast;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int         return_code = EXIT_SUCCESS;
	std::string xml_file((argc > 1) ? argv[1] : "./templates.xml");

	try {
		MLB::XercesUtils::XercesContext xerces_context;
		MLB::XercesUtils::XmlDomElement root_element(
			MLB::XercesUtils::XmlDomElement::ParseXmlFile(xml_file));
		InsItemList   item_list;
		DictValueList dict_value_list;
		InsItem::ParseTemplateElement(InsCtxInfo(), root_element,
			item_list, dict_value_list);
		InsItem::DumpItemListFull(item_list, dict_value_list);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

