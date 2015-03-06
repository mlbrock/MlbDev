// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ExcContext class.

	Revision History	:	2008-11-28 --- Creation.
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

#include <VFast/ExcContext.hpp>
#include <VFast/VFastException.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
struct ExcContextDispatchLink {
	ExcContextDispatchLink(DataType data_type, FieldOperator field_operator,
		bool has_pmap_bit, bool is_optional, bool has_initial_value,
		ExcContext::FieldDecodeDispatch field_dispatch, const char *link_name)
		:data_type_(data_type)
		,field_operator_(field_operator)
		,has_pmap_bit_(has_pmap_bit)
		,is_optional_(is_optional)
		,has_initial_value_(has_initial_value)
		,agg_needs_pmap_bit_(false)
		,field_dispatch_(field_dispatch)
		,link_name_(link_name)
	{
	}
	ExcContextDispatchLink(DataType data_type, FieldOperator field_operator,
		bool has_pmap_bit, bool is_optional, bool has_initial_value,
		bool agg_needs_pmap_bit,
		ExcContext::FieldDecodeDispatch field_dispatch, const char *link_name)
		:data_type_(data_type)
		,field_operator_(field_operator)
		,has_pmap_bit_(has_pmap_bit)
		,is_optional_(is_optional)
		,has_initial_value_(has_initial_value)
		,agg_needs_pmap_bit_(agg_needs_pmap_bit)
		,field_dispatch_(field_dispatch)
		,link_name_(link_name)
	{
	}
	//	Ctor for find()...	
	ExcContextDispatchLink(const InsItem &ins_item)
		:data_type_(ins_item.data_type_)
		,field_operator_(ins_item.field_operator_)
		,has_pmap_bit_(ins_item.HasPMapBit())
		,is_optional_(ins_item.IsOptional())
		,has_initial_value_(ins_item.HasInitialValue())
		,agg_needs_pmap_bit_(ins_item.AggNeedsPMapBit())
		,field_dispatch_(NULL)
		,link_name_()
	{
		//	Decimal fields aren't matched against certain qualifiers...
		if (data_type_ == DataType_Decimal) {
			field_operator_     = FieldOperator_None;
			has_initial_value_  = false;
			agg_needs_pmap_bit_ = false;
		}
		//	... Nor are Group fields...
		else if (data_type_ == DataType_Group) {
			is_optional_        = false;
			has_initial_value_  = false;
			agg_needs_pmap_bit_ = false;
		}
		/*
			The only time a field with a Constant operator has a PMap bit
			is when it's an optional field...
		*/
		else if (field_operator_ == FieldOperator_Constant)
			has_pmap_bit_ = false;
		/*
			All fields with Delta operators have dictionary values
			imposed by the instruction context logic, even if they
			officially don't have an initial value.
		*/
		else if (field_operator_ == FieldOperator_Delta)
			has_initial_value_ = true;
	}

	int Compare(const ExcContextDispatchLink &other) const
	{
		int cmp;

		if ((cmp = (static_cast<int>(data_type_) -
			static_cast<int>(other.data_type_))) == 0) {
			if ((cmp = (static_cast<int>(field_operator_) -
				static_cast<int>(other.field_operator_))) == 0) {
				if ((cmp = (static_cast<int>(has_pmap_bit_) -
					static_cast<int>(other.has_pmap_bit_))) == 0) {
					if ((cmp = (static_cast<int>(is_optional_) -
						static_cast<int>(other.is_optional_))) == 0) {
						if ((cmp = (static_cast<int>(has_initial_value_) -
							static_cast<int>(other.has_initial_value_))) == 0)
							cmp = static_cast<int>(agg_needs_pmap_bit_) -
								static_cast<int>(other.agg_needs_pmap_bit_);
					}
				}
			}
		}
	
		return(cmp);
	}

	bool operator <  (const ExcContextDispatchLink &other) const
	{
		return(Compare(other) < 0);
	}
	bool operator == (const ExcContextDispatchLink &other) const
	{
		return(!Compare(other));
	}

	DataType                        data_type_;
	FieldOperator                   field_operator_;
	bool                            has_pmap_bit_;
	bool                            is_optional_;
	bool                            has_initial_value_;
	bool                            agg_needs_pmap_bit_;
	ExcContext::FieldDecodeDispatch field_dispatch_;
	std::string                     link_name_;
};
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC(d_type, f_op, f_op_name,		\
	has_pmap_bit, is_optional, has_inital_value, op_tag)								\
	ExcContextDispatchLink(d_type, f_op, has_pmap_bit, is_optional,				\
		has_inital_value,																			\
		(&ExcContext::FieldDecode_##f_op_name##_##op_tag<d_type>),					\
		"ExcContext::FieldDecode_" #f_op_name "_" #op_tag "<" #d_type ">")
	
#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT(f_op, f_op_name, has_pmap_bit,	\
	is_optional, has_inital_value, op_tag)													\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC(DataType_SInt32,     f_op,			\
		f_op_name, has_pmap_bit, is_optional, has_inital_value, op_tag),			\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC(DataType_UInt32,     f_op,			\
		f_op_name, has_pmap_bit, is_optional, has_inital_value, op_tag),			\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC(DataType_SInt64,     f_op,			\
		f_op_name, has_pmap_bit, is_optional, has_inital_value, op_tag),			\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC(DataType_UInt64,     f_op,			\
		f_op_name, has_pmap_bit, is_optional, has_inital_value, op_tag)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR(f_op, f_op_name, has_pmap_bit,	\
	is_optional, has_inital_value, op_tag)													\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC(DataType_Ascii,     f_op,			\
		f_op_name, has_pmap_bit, is_optional, has_inital_value, op_tag),			\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC(DataType_Unicode,    f_op,			\
		f_op_name, has_pmap_bit, is_optional, has_inital_value, op_tag),			\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC(DataType_ByteVector, f_op,			\
		f_op_name, has_pmap_bit, is_optional, has_inital_value, op_tag)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(f_op, f_op_name, has_pmap_bit,	\
	is_optional, has_inital_value, op_tag)													\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT(f_op, f_op_name, has_pmap_bit,		\
		is_optional, has_inital_value, op_tag),											\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR(f_op, f_op_name, has_pmap_bit,		\
		is_optional, has_inital_value, op_tag)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Non()									\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_None,      Non, true,  true,  false, PO),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_None,      Non, false, true,  false, XO),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_None,      Non, true,  false, false, PX),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_None,      Non, false, false, false, XX)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Con()									\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Constant,  Con, false, true,  true,  O),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Constant,  Con, false, false, true,  X)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Def()									\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Default,   Def, true,  true,  true,  PO),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Default,   Def, true,  true,  false, PO),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Default,   Def, true,  false, true,  PX),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Default,   Def, false, true,  true,  XO),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Default,   Def, false, true,  false, XO),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Default,   Def, false, false, false, XX)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Cop()									\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Copy,      Cop, true,   true, true,  OI),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Copy,      Cop, true,  false, true,  XI),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Copy,      Cop, true,   true, false, OX),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL(FieldOperator_Copy,      Cop, true,  false, false, XX)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Inc()									\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT(FieldOperator_Increment, Inc, true,   true, true,  OI),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT(FieldOperator_Increment, Inc, true,  false, true,  XI),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT(FieldOperator_Increment, Inc, true,   true, false, OX),\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT(FieldOperator_Increment, Inc, true,  false, false, XX)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Del()									\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT(FieldOperator_Delta,     Del, false, true,  true,  O_Int),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT(FieldOperator_Delta,     Del, false, false, true,  X_Int),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR(FieldOperator_Delta,     Del, false, true,  true,  O_Str),	\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR(FieldOperator_Delta,     Del, false, false, true,  X_Str)

#define VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Tai()									\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR(FieldOperator_Tail,      Tai,  true, true,   true,  OI),		\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR(FieldOperator_Tail,      Tai,  true, false,  true,  XI),		\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR(FieldOperator_Tail,      Tai,  true, true,  false,  OX),		\
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR(FieldOperator_Tail,      Tai,  true, false, false,  XX)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ExcContextDispatchLink ExcContextDispatchList[] = {
	ExcContextDispatchLink(DataType_Template,          FieldOperator_None, false, false, false, false, (&ExcContext::FieldDecode_Template),          "ExcContext::FieldDecode_Template"),
	ExcContextDispatchLink(DataType_TemplateRefStatic, FieldOperator_None, false, false, false, false, (&ExcContext::FieldDecode_TemplateRefStatic), "ExcContext::FieldDecode_TemplateRefStatic"),
	ExcContextDispatchLink(DataType_Sequence,          FieldOperator_None, false, false, false,  true, (&ExcContext::FieldDecode_Sequence_A),        "ExcContext::FieldDecode_Sequence_A"),
	ExcContextDispatchLink(DataType_Sequence,          FieldOperator_None, false,  true, false,  true, (&ExcContext::FieldDecode_Sequence_A),        "ExcContext::FieldDecode_Sequence_A"),
	ExcContextDispatchLink(DataType_Sequence,          FieldOperator_None, false, false, false, false, (&ExcContext::FieldDecode_Sequence_X),        "ExcContext::FieldDecode_Sequence_X"),
	ExcContextDispatchLink(DataType_Sequence,          FieldOperator_None, false,  true, false, false, (&ExcContext::FieldDecode_Sequence_X),        "ExcContext::FieldDecode_Sequence_X"),
	ExcContextDispatchLink(DataType_Decimal,           FieldOperator_None,  true,  true, false, false, (&ExcContext::FieldDecode_Decimal_PO),        "ExcContext::FieldDecode_Decimal_PO"),
	ExcContextDispatchLink(DataType_Decimal,           FieldOperator_None, false,  true, false, false, (&ExcContext::FieldDecode_Decimal_XO),        "ExcContext::FieldDecode_Decimal_XO"),
	ExcContextDispatchLink(DataType_Decimal,           FieldOperator_None,  true, false, false, false, (&ExcContext::FieldDecode_Decimal_PX),        "ExcContext::FieldDecode_Decimal_PX"),
	ExcContextDispatchLink(DataType_Decimal,           FieldOperator_None, false, false, false, false, (&ExcContext::FieldDecode_Decimal_XX),        "ExcContext::FieldDecode_Decimal_XX"),
	ExcContextDispatchLink(DataType_Group,             FieldOperator_None,  true, false, false, false, (&ExcContext::FieldDecode_Group_P),           "ExcContext::FieldDecode_Group_P"),
	ExcContextDispatchLink(DataType_Group,             FieldOperator_None, false, false, false, false, (&ExcContext::FieldDecode_Group_X),           "ExcContext::FieldDecode_Group_X"),
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Non(),
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Con(),
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Def(),
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Cop(),
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Inc(),
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Del(),
	VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Tai()
};
const unsigned int           ExcContextDispatchCount  =
	sizeof(ExcContextDispatchList) / sizeof(ExcContextDispatchList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_BASIC
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_INT
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_STR
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Non
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Con
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Def
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Cop
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Inc
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Del
#undef VFAST_EXC_CONTEXT_DISPATCH_PERMUTE_ALL_Tai
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
ExcContext::ExcContext(const InsContext &ins_context,
	const ExcContextInfo &exc_context_info)
	:exc_context_info_(exc_context_info)
	,ins_context_(ins_context)
	,active_dict_(ins_context.dict_value_list_)
	,dispatch_list_(FixUpDispatch(ins_context_.ins_item_list_))
	,dispatch_list_ptr_(dispatch_list_.get())
	,last_template_id_(0)
	,last_template_index_(0)
	,last_pmap_length_(0)
	,entry_list_(new ExcEntry[MaxExcEntryCount])
	,dst_buffer_(new char[DefaultExcDataBufferLength])
	,lookup_list_sptr_(new ExcEntryLookup[ins_context_.ins_item_list_.size()])
	,entry_begin_ptr_(entry_list_.get())
	,entry_end_ptr_(entry_list_.get() + MaxExcEntryCount)
	,curr_entry_begin_ptr_(entry_begin_ptr_)
	,last_entry_ptr_(curr_entry_begin_ptr_)
	,dst_begin_ptr_(dst_buffer_.get())
	,dst_end_ptr_(dst_begin_ptr_+ MaxExcDataBufferLength)
	,curr_dst_begin_ptr_(dst_begin_ptr_)
	,src_begin_ptr_(NULL)
	,src_end_ptr_(NULL)
	,curr_src_begin_ptr_(src_begin_ptr_)
	,pmap_begin_ptr_(NULL)
	,pmap_end_ptr_(NULL)
	,pmap_mask_bit_(0)
	,current_item_ptr_(NULL)
{
	FixUpDictValuePtrs();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetTemplateDictionary(unsigned int template_id)
{
	ResetTemplateByIndex(ins_context_.GetTemplateIndex(template_id));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetTemplateDictionary(const std::string &template_name)
{
	ResetTemplateByIndex(ins_context_.GetTemplateIndex(template_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetTemplateDictionary(const char *template_name)
{
	ResetTemplateByIndex(ins_context_.GetTemplateIndex(template_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetLastTemplateDictionary()
{
	if (!last_template_id_)
		MLB::Utility::ThrowLogicError("There is no last template for which "
			"dictionary entries can be reset.");

	if (last_template_id_ != InvalidTemplateIndex)
		ResetTemplateByIndex(last_template_index_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetDictionaries()
{
	if (!active_dict_.empty())
		::memcpy(&(active_dict_[0]), &(ins_context_.dict_value_list_[0]),
			active_dict_.size() * sizeof(active_dict_[0]));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetTemplateByIndex(unsigned int template_index)
{
	InsItem *template_ptr = &(ins_context_.ins_item_list_[template_index]);

	if (template_ptr->dict_value_count_)
		::memcpy(&(active_dict_[template_ptr->dict_value_index_]),
			&(ins_context_.dict_value_list_[template_ptr->dict_value_index_]),
			template_ptr->dict_value_count_ * sizeof(active_dict_[0]));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetDst()
{
	curr_entry_begin_ptr_ = entry_begin_ptr_;
	last_entry_ptr_       = curr_entry_begin_ptr_;
	curr_dst_begin_ptr_   = dst_begin_ptr_;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetSrc(const StreamDatum *src_begin_ptr,
	const StreamDatum *src_end_ptr)
{
	if ((src_begin_ptr != NULL) || (src_end_ptr != NULL)) {
		if ((src_begin_ptr != NULL) && (src_end_ptr == NULL))
			MLB::Utility::ThrowInvalidArgument("The source stream begin data "
				"pointer is NULL, but the source stream end pointer is not.");
		if ((src_begin_ptr == NULL) && (src_end_ptr != NULL))
			MLB::Utility::ThrowInvalidArgument("The source stream end data "
				"pointer is NULL, but the source stream begin pointer is not.");
		if (src_begin_ptr >= src_end_ptr )
			MLB::Utility::ThrowInvalidArgument("The source stream begin data "
				"pointer (" + MLB::Utility::ValueToStringHex(src_begin_ptr) +
				") is not less than the the source stream end pointer (" +
				MLB::Utility::ValueToStringHex(src_end_ptr) + ").");
	}

	src_begin_ptr_        = src_begin_ptr;
	src_end_ptr_          = src_end_ptr;
	curr_src_begin_ptr_   = src_begin_ptr_;
	pmap_begin_ptr_       = NULL;
	pmap_end_ptr_         = NULL;
	pmap_mask_bit_        = 0;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::ResetSrcChecked(const StreamDatum *src_begin_ptr,
	const StreamDatum *src_end_ptr)
{
	//	Only need to check one ptr, other checks in ResetSrcChecked()...
	if (src_begin_ptr == NULL)
		MLB::Utility::ThrowInvalidArgument("The source stream begin data "
			"pointer is NULL.");

	ResetSrc(src_begin_ptr, src_end_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const StreamDatum *ExcContext::FieldDecode(const StreamDatum *src_begin_ptr,
	const StreamDatum *src_end_ptr)
{
	ResetSrcChecked(src_begin_ptr, src_end_ptr);
	ResetDst();

	GetPMap();

	last_pmap_length_ =
		static_cast<unsigned int>(src_begin_ptr_ - pmap_begin_ptr_);

	if (*pmap_begin_ptr_ & StreamDatumTemplateIdBit) {
		unsigned int template_id;
		TransferDecode_UInt32(src_begin_ptr_, src_end_ptr_, template_id);
		/*
			CODE NOTE: Implicitly processed SCP FastReset messages should use
			a cached template index value (which will be 0 for EBS/CEF Ultra+).
		*/
		if ((template_id == 120) && exc_context_info_.SupportsImplicitSCP120()) {
			last_template_index_ = ins_context_.DetermineTemplateIndex(template_id);
			last_template_id_    = template_id;
			return(src_begin_ptr_);
		}
		last_template_index_ = ins_context_.GetTemplateIndex(template_id);
		last_template_id_    = template_id;
	}
//	CODE NOTE: Add logic to support last_begin_ptr_
//	else if ((last_begin_ptr_ != begin_pmap_ptr) || (!last_template_id_))
	else if (!last_template_id_)
		MLB::Utility::ThrowLogicError("The pmap does not have the template id "
			"bit set (" + MLB::Utility::ValueToStringHex(*pmap_begin_ptr_) +
			" & " + MLB::Utility::ValueToStringHex(
			static_cast<unsigned char>(StreamDatumTemplateIdBit)) + " is false) "
			"but the saved last template id is not valid.");
	else if (last_template_index_ == InvalidTemplateIndex)
		MLB::Utility::ThrowLogicError("The pmap does not have the template id "
			"bit set (" + MLB::Utility::ValueToStringHex(*pmap_begin_ptr_) +
			" & " + MLB::Utility::ValueToStringHex(
			static_cast<unsigned char>(StreamDatumTemplateIdBit)) + " is false) "
			"and the saved last template id (" +
			MLB::Utility::AnyToString(last_template_id_) + ") is valid, but its "
			"internal index is not valid.");

	pmap_mask_bit_ = 0x20;

	FieldDecode_Template(last_template_index_, 0);

	return(src_begin_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const StreamDatum *ExcContext::FieldDecode(const StreamDatum *src_begin_ptr,
	const StreamDatum *src_end_ptr, ExcResults &exc_results)
{
	FieldDecode(src_begin_ptr, src_end_ptr);

	GetDecodeResults(exc_results);

	return(src_begin_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const StreamDatum *ExcContext::FieldDecode(const StreamDatum *src_begin_ptr,
	const StreamDatum *src_end_ptr, unsigned int template_id)
{
	if (template_id != last_template_id_)
		MLB::Utility::ThrowInvalidArgument("The specified template id (" +
			MLB::Utility::AnyToString(template_id) + ") is not equal to the "
			"last template id processed by this execution context (" +
			MLB::Utility::AnyToString(last_template_id_) + ").");

	ResetSrcChecked(src_begin_ptr, src_end_ptr);
	ResetDst();

	GetPMap();

	last_pmap_length_ = static_cast<unsigned int>(src_begin_ptr_ - src_begin_ptr);

	pmap_mask_bit_    = 0x20;

	FieldDecode_Template(last_template_index_, 0);

	return(src_begin_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::GetDecodeResults(ExcResults &exc_results) const
{
	exc_results.template_id_    = last_template_id_;
	exc_results.template_index_ = last_template_index_;
	exc_results.item_count_     = (ins_context_.ins_item_list_.empty()) ? 0 :
		static_cast<unsigned int>(ins_context_.ins_item_list_.size());
	exc_results.item_list_      = (ins_context_.ins_item_list_.empty()) ? NULL :
		&(ins_context_.ins_item_list_[0]);
	exc_results.entry_count_    = static_cast<unsigned int>(GetEntryCount());
	exc_results.entry_list_     = GetEntryList();
	exc_results.lookup_list_    = lookup_list_sptr_.get();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ExcContext::GetFieldDispatchReport() const
{

	std::ostringstream      o_str;
	const InsItem          *item_ptr(&(ins_context_.ins_item_list_[0]));
	InsItemList::size_type  item_count(ins_context_.ins_item_list_.size());
	InsItemList::size_type  count_1;
	unsigned int            template_index = item_count;

	for (count_1 = 0; count_1 < item_count; ++count_1, ++item_ptr) {
		if (template_index != item_ptr->template_index_) {
			if (count_1)
				o_str << InsItem::EmitFillLine('=') << std::endl << std::endl;
			o_str << InsItem::EmitFillLine('=') << std::endl;
			o_str << InsItem::EmitHeaderText() << std::endl;
			o_str << InsItem::EmitFillLine('-') << std::endl;
			template_index = item_ptr->template_index_;
		}
		o_str << item_ptr->EmitDatum() << " ";
		for (std::size_t count_2 = 0; count_2 < ExcContextDispatchCount;
			++count_2) {
			if (dispatch_list_ptr_[count_1] ==
				ExcContextDispatchList[count_2].field_dispatch_) {
				o_str << " " << ExcContextDispatchList[count_2].link_name_;
				break;
			}
		}
		o_str << std::endl;
	}

	o_str << InsItem::EmitFillLine('=');

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ExcContext::CheckFieldDispatch(const InsContext &ins_context,
	std::vector<std::string> &error_list)
{
	return(CheckFieldDispatch(ins_context.ins_item_list_, error_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ExcContext::CheckFieldDispatch(const InsItemList &item_list,
	std::vector<std::string> &error_list)
{
	std::vector<std::string> tmp_error_list;

	FixUpDispatch(item_list, tmp_error_list, false);

	error_list.swap(tmp_error_list);

	return(error_list.empty());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::FieldDecode_Template(unsigned int item_index,
	unsigned int sequence_number)
{
	const InsItem *item_list  = &(ins_context_.ins_item_list_[item_index]);
	unsigned int   item_count = item_list->element_count_;

	AddEntry(item_index, sequence_number, 1, 0, false, false);

	FieldDecodeList(item_index + 1, item_count - 1, item_list + 1,
		sequence_number);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::FieldDecode_TemplateRefStatic(unsigned int item_index,
	unsigned int sequence_number)
{
	FieldDecode_Template(
		ins_context_.ins_item_list_[item_index].static_ref_index_,
		sequence_number);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::FieldDecode_Sequence_A(unsigned int item_index,
	unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, sizeof(Type_UInt32),
		sizeof(Type_UInt32), false, false);

	DispatchToItem(item_index + 1, sequence_number);

	Type_UInt32 sequence_count = *last_entry_ptr_->GetDatumPtr<Type_UInt32>();

	if (!sequence_count) {
		last_entry_ptr_[-1].is_null_               = true;
		return;
	}

	const InsItem *item_ptr   = &(ins_context_.ins_item_list_[item_index]);
	const InsItem *item_list  = &(ins_context_.ins_item_list_[item_index + 2]);
	unsigned int   item_count = item_ptr->element_count_ - 2;
	unsigned int   count_1;

	PMapStateSaver pmap_saved_state(this);

	for (count_1 = 1; count_1 <= sequence_count; ++count_1) {
		GetPMap();
		pmap_mask_bit_ = StreamDatumTemplateIdBit;
		FieldDecodeList(item_index + 2, item_count, item_list, count_1);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::FieldDecode_Sequence_X(unsigned int item_index,
	unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, sizeof(Type_UInt32),
		sizeof(Type_UInt32), false, false);

	DispatchToItem(item_index + 1, sequence_number);

	Type_UInt32 sequence_count = *last_entry_ptr_->GetDatumPtr<Type_UInt32>();

	if (!sequence_count) {
		last_entry_ptr_[-1].is_null_               = true;
		return;
	}

	const InsItem *item_ptr   = &(ins_context_.ins_item_list_[item_index]);
	const InsItem *item_list  = &(ins_context_.ins_item_list_[item_index + 2]);
	unsigned int   item_count = item_ptr->element_count_ - 2;
	unsigned int   count_1;

	for (count_1 = 1; count_1 <= sequence_count; ++count_1)
		FieldDecodeList(item_index + 2, item_count, item_list, count_1);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::FieldDecode_Group_P(unsigned int item_index,
	unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, 1, 0, false, false);

	if (!GetPMapBit()) {
		last_entry_ptr_->is_null_ = true;
		return;
	}

	const InsItem *item_list  = &(ins_context_.ins_item_list_[item_index]);
	unsigned int   item_count = item_list->element_count_;

	FieldDecodeList(item_index + 1, item_count - 1, item_list + 1,
		sequence_number);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::FieldDecode_Group_X(unsigned int item_index,
	unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, 1, 0, false, false);

	const InsItem *item_list  = &(ins_context_.ins_item_list_[item_index]);
	unsigned int   item_count = item_list->element_count_;

	FieldDecodeList(item_index + 1, item_count - 1, item_list + 1,
		sequence_number);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const StreamDatum *ExcContext::FieldDecodeList(unsigned int start_index,
	unsigned int item_count, const InsItem * /* item_list */,
	unsigned int sequence_number)
{
	unsigned int end_index  = start_index + item_count;

	while (start_index < end_index) {
		DispatchToItem(start_index, sequence_number);
		start_index += ins_context_.ins_item_list_[start_index].element_count_;
	}

	return(src_begin_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::EnsureDstMem(std::size_t datum_length)
{
	if ((curr_dst_begin_ptr_ + datum_length) >= dst_end_ptr_)
		MLB::Utility::ThrowLogicError("The requested destination memory size (" +
			MLB::Utility::AnyToString(datum_length) + ") when added to the "
			"current destination location pointer (" +
			MLB::Utility::ValueToStringHex(curr_dst_begin_ptr_) + ") exceeds the "
			"pre-allocated destination memory buffer of " +
			MLB::Utility::AnyToString(DefaultExcDataBufferLength) + " bytes by " +
			MLB::Utility::AnyToString(static_cast<unsigned int>(
			((curr_dst_begin_ptr_ + datum_length) - dst_end_ptr_) + 1)) +
			" bytes.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::MoveCurrDstPtr(std::size_t move_length)
{
	EnsureDstMem(move_length);

	curr_dst_begin_ptr_ += move_length;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::AlignDstMem(unsigned int datum_alignment)
{
	if ((datum_alignment > 1) &&
		((reinterpret_cast<std::size_t>(curr_dst_begin_ptr_) %
		datum_alignment) != 0))
		MoveCurrDstPtr(reinterpret_cast<std::size_t>(curr_dst_begin_ptr_) %
			datum_alignment);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ExcEntry *ExcContext::AddEntry(unsigned int item_index,
	unsigned int sequence_number, unsigned int datum_alignment,
	unsigned int datum_length, bool alloc_plus, bool is_null)
{
	AlignDstMem(datum_alignment);

	EnsureDstMem(datum_length + ((alloc_plus) ? 1 : 0));

	if (curr_entry_begin_ptr_ >= entry_end_ptr_)
		MLB::Utility::ThrowLogicError("The maximum number of fields has been "
			"added (" + MLB::Utility::AnyToString(MaxExcEntryCount) + ").");

	curr_entry_begin_ptr_->item_index_       = item_index;
	curr_entry_begin_ptr_->datum_offset_     =
		static_cast<unsigned int>(curr_dst_begin_ptr_ - dst_begin_ptr_);
	curr_entry_begin_ptr_->datum_length_     = datum_length;
	curr_entry_begin_ptr_->sequence_number_  = sequence_number;
	curr_entry_begin_ptr_->is_null_          = is_null;
	curr_entry_begin_ptr_->datum_ptr_        = curr_dst_begin_ptr_ +
		curr_entry_begin_ptr_->datum_offset_;
	curr_dst_begin_ptr_                     += datum_length + ((alloc_plus) ? 1 : 0);
	last_entry_ptr_                          = curr_entry_begin_ptr_;

#ifdef VFAST_DEBUG_EXC_ENTRY
	curr_entry_begin_ptr_->begin_ptr_        = src_begin_ptr_;
	curr_entry_begin_ptr_->end_ptr_          = src_begin_ptr_;
	curr_entry_begin_ptr_->pmap_flag_        = IsPMapBitSet();
	current_item_ptr_                        =
		&(ins_context_.ins_item_list_[item_index]);
#endif // #ifdef VFAST_DEBUG_EXC_ENTRY

	return(curr_entry_begin_ptr_++);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcContext::FixUpDictValuePtrs()
{
	InsItemListIter iter_b(ins_context_.ins_item_list_.begin());
	InsItemListIter iter_e(ins_context_.ins_item_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->dict_value_count_ == 1)
			iter_b->dict_value_ptr_ = &(active_dict_[iter_b->dict_value_index_]);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ExcContext::FieldDecodeDispatchList ExcContext::FixUpDispatch(
	const InsItemList &item_list)
{
	std::vector<std::string> error_list;

	return(FixUpDispatch(item_list, error_list, true));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ExcContext::FieldDecodeDispatchList ExcContext::FixUpDispatch(
	const InsItemList &item_list, std::vector<std::string> &error_list,
	bool throw_on_error)
{
	std::vector<std::string> tmp_error_list;
	FieldDecodeDispatchList  dispatch_list(
		new FieldDecodeDispatch[item_list.size()]);

	const InsItem          *item_ptr(&(item_list[0]));
	InsItemList::size_type  item_count(item_list.size());
	InsItemList::size_type  count_1;

	for (count_1 = 0; count_1 < item_count; ++count_1, ++item_ptr) {
		if (item_ptr->IsByteLength()) {
			dispatch_list.get()[count_1] = (&ExcContext::FieldDecode_ByteLength);
			continue;
		}
		ExcContextDispatchLink tmp_link(*item_ptr);
		//	CODE NOTE: Linear lookup to be changed when set is implemented...
		unsigned int count_2;
		for (count_2 = 0; count_2 < ExcContextDispatchCount; ++count_2) {
			if (ExcContextDispatchList[count_2] == tmp_link)
				break;
		}
		if (count_2 < ExcContextDispatchCount)
			dispatch_list.get()[count_1] =
				ExcContextDispatchList[count_2].field_dispatch_;
		else {
			std::string error_text("Unable to locate an execution "
				"context dispatch method for field '" +
				((!item_ptr->field_name_.empty()) ? item_ptr->field_name_  :
				item_ptr->key_name_) + "' in template " +
				MLB::Utility::AnyToString(
					item_list[item_ptr->template_index_].auxiliary_id_) +
				" ('" + item_list[item_ptr->template_index_].field_name_ + "') "
				"with field operator " +
				FieldOperatorToString(item_ptr->field_operator_) + ", data type " +
				DataTypeToString(item_ptr->data_type_) + " where HasPMapBit = " +
				MLB::Utility::AnyToString(item_ptr->HasPMapBit()) +
				", IsOptional = " +
				MLB::Utility::AnyToString(item_ptr->IsOptional()) +
				", HasInitialValue = " +
				MLB::Utility::AnyToString(item_ptr->HasInitialValue()) +
				", and AggNeedsPMapBit = " +
				MLB::Utility::AnyToString(item_ptr->AggNeedsPMapBit()) + ".");
			if (throw_on_error)
				MLB::Utility::ThrowLogicError(error_text);
			else
				tmp_error_list.push_back(error_text);
		}
	}

	error_list.swap(tmp_error_list);

	return(dispatch_list);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/HRTimer.hpp>

using namespace MLB::VFast;

namespace {
// ////////////////////////////////////////////////////////////////////////////
struct FastFixTestPacket {
   unsigned short       packet_length_;
   const unsigned char *packet_data_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const FastFixTestPacket TestMe = {    50, reinterpret_cast<const unsigned char *>("\xC0\xB9\x08\x32\x69\xB0\x23\x55\x6F\x6B\x4B\x06\x3E\xF1\x09\x49\x52\x9D\x82\x6D\xC2\x80\x80\x80\x18\x69\xD0\x13\x72\xBF\x05\x0C\x8E\x3F\x7A\x79\xD8\x80\x80\x80\xC3\x30\xC0\x80\xCE\x80\x80\x80\x80\x80") };
// ////////////////////////////////////////////////////////////////////////////
} // namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
			std::cout << "USAGE: " << std::endl <<
				"   " << argv[0] << " <XML-template-file>" <<
				std::endl << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (argc != 2)
			MLB::Utility::ThrowInvalidArgument("Expected a single argument of "
				"the name of an Fix/FAST XML template file. Use '-h' for help.");
		InsContext ins_context(argv[1]);
		ExcContext exc_context(ins_context);
		//	----------------------------------------------------------------
		try {
			exc_context.FieldDecode(TestMe.packet_data_,
				TestMe.packet_data_ + TestMe.packet_length_);
		}
		catch (const std::exception &except) {
			std::cout << "DECODE ERROR: " << except.what() << std::endl;
			return_code = EXIT_FAILURE;
		}
		std::size_t     entry_count = exc_context.GetEntryCount();
		const ExcEntry *entry_list  = exc_context.GetEntryList();
		ExcEntry::EmitListDebug(entry_count, entry_list,
			exc_context.GetInsItemList());
		exc_context.ResetLastTemplateDictionary();
		//	----------------
		unsigned int count_1;
		unsigned int iteration_count = 1000000;
		MLB::Utility::HRTimer hr_timer;
		hr_timer.Start();
		for (count_1 = 0; count_1 < iteration_count; ++count_1) {
			exc_context.FieldDecode(TestMe.packet_data_,
				TestMe.packet_data_ + TestMe.packet_length_);
			exc_context.ResetLastTemplateDictionary();
		}
		hr_timer.End();
		std::cout << "Elapsed time    = " << std::setprecision(16) <<
			hr_timer.GetTickDiffInSeconds() << std::endl;
		std::cout << "Time per packet = " <<
			(hr_timer.GetTickDiffInSeconds() /
			static_cast<double>(iteration_count)) << std::endl;
		//	----------------------------------------------------------------
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

