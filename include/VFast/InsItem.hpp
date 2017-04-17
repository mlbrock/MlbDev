// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the InsItem class.

	Revision History	:	2008-11-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__InsItem_hpp__HH

#define HH__MLB__VFast__InsItem_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/FieldOperator.hpp>
#include <VFast/DictValue.hpp>

#include <XercesUtils/XmlDomElement.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	\enum InsItemFlag Bit flags which describe the state of a single instruction
	context item.
*/
enum InsItemFlag {
	/** Indicates no flag is set. */
	InsItemFlag_None            = 0x00000000,
	/** Indicates that the instruction context item field is optional. */
	InsItemFlag_IsOptional      = 0x00000001,
	/** Indicates that the instruction context item field will have a presence map bit in messages. */ 
	InsItemFlag_HasPMapBit      = 0x00000002,
	/** Indicates that the instruction context item field is nullable. */
	InsItemFlag_IsNullable      = 0x00000004,
	/** Indicates that the instruction context item field has a numeric field identifier. This is usually the FIX identifier, but VFast does not enforce this. */
	InsItemFlag_HasFieldId      = 0x00000008,
	/** Indicates that the instruction context item field has an initial value in a \c DictValue dictionary value. */
	InsItemFlag_HasInitialValue = 0x00000010,
	/** Indicates that the instruction context item field is an aggregate which requires a presence map bit. */
	InsItemFlag_AggNeedsPMapBit = 0x00000020,
	/** Indicates that the instruction context item field is within a sequence. */
	InsItemFlag_IsInSequence    = 0x00000040,
	/** Indicates that the instruction context item field is within a group. */
	InsItemFlag_IsInGroup       = 0x00000080,
	InsItemFlag_IsIn            = InsItemFlag_IsInSequence | InsItemFlag_IsInGroup,
	/** Indicates that the instruction context item field is the exponent portion of a decimal field. */
	InsItemFlag_IsDecimalExp    = 0x00000100,
	/** Indicates that the instruction context item field is the mantissa portion of a decimal field. */
	InsItemFlag_IsDecimalMan    = 0x00000200,
	InsItemFlag_IsDecimalChild  = InsItemFlag_IsDecimalExp | InsItemFlag_IsDecimalMan,
	/** Indicates that the instruction context item field is the field in which is stored the length of a sequence. */
	InsItemFlag_IsSeqLength     = 0x00000400,
	/** Indicates that the instruction context item field is the field in which is stored the length of a byte vector. */
	InsItemFlag_IsByteLength    = 0x00000800,
	InsItemFlag_IsLength        = InsItemFlag_IsSeqLength | InsItemFlag_IsByteLength,
	/** Indicates that the instruction context item field was generated internally by the VFast software (for example, to add explicitly a sequence length not specified in the XML template file). */
	InsItemFlag_IsGenerated     = 0x00001000,
	InsItemFlag_ModifiesOp      = InsItemFlag_IsOptional | InsItemFlag_HasPMapBit | InsItemFlag_IsNullable | InsItemFlag_HasInitialValue,
	/** The mask of all valid \c InsItemFlag flag values. */
	InsItemFlag_Mask            = InsItemFlag_IsOptional | InsItemFlag_HasPMapBit | InsItemFlag_IsNullable | InsItemFlag_HasFieldId | InsItemFlag_HasInitialValue | InsItemFlag_AggNeedsPMapBit | InsItemFlag_IsInSequence | InsItemFlag_IsInGroup | InsItemFlag_IsDecimalExp | InsItemFlag_IsDecimalMan | InsItemFlag_IsSeqLength | InsItemFlag_IsByteLength | InsItemFlag_IsGenerated,
	InsItemFlag_Min             = InsItemFlag_None,
	InsItemFlag_MinValid        = InsItemFlag_IsOptional,
	InsItemFlag_Max             = InsItemFlag_IsGenerated,
	InsItemFlag_MaxValid        = InsItemFlag_IsGenerated,
};
// ////////////////////////////////////////////////////////////////////////////

namespace {
// ////////////////////////////////////////////////////////////////////////////
const char InsItemFlagBitChars[] = "gBLMEGSAIFNPO";
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
inline std::string InsItemFlagBitsToString(InsItemFlag item_flags)
{
	char        buffer[(sizeof(InsItemFlag) * CHAR_BIT) + 1];
	char       *dst_ptr = buffer;
	const char *src_ptr = InsItemFlagBitChars;

	int tmp_bit = InsItemFlag_MaxValid;

	while (tmp_bit) {
		*dst_ptr++ = (tmp_bit & item_flags) ? *src_ptr : '-';
		++src_ptr;
		tmp_bit >>= 1;
	}

	*dst_ptr = '\0';

	return(buffer);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief Provides some general information of use in the construction of
	instances of the \c InsContext class.
*/
struct InsCtxInfo {
	explicit InsCtxInfo(const std::string &application_name = "any",
		const std::string &dictionary_name = "global")
		:application_name_(application_name)
		,dictionary_name_(dictionary_name)
		,template_id_()
		,parent_index_(0)
	{
	}

	std::string GetApplicationName() const
	{
		return(application_name_);
	}
	std::string GetDictionaryName() const
	{
		return(dictionary_name_);
	}
	unsigned int GetParentIndex() const
	{
		return(parent_index_);
	}

	void SetApplicationName(const std::string &application_name)
	{
		application_name_ = application_name;
	}
	void SetDictionaryName(const std::string &dictionary_name)
	{
		dictionary_name_ = dictionary_name;
	}
	void SetParentIndex(std::size_t parent_index)
	{
		parent_index_ = static_cast<unsigned int>(parent_index);
	}

	std::string  application_name_;
	std::string  dictionary_name_;
	std::string  template_id_;
	unsigned int parent_index_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief Describes a single instruction context item.
*/
class InsItem {
public:
	typedef std::vector<InsItem> InsItemListI;

	//	Default ctor for STL containers...
	InsItem();

	bool operator < (const InsItem &other) const;

	//	InsItemFlag check accessors..
	inline bool IsOptional() const
	{
		return((flags_ & InsItemFlag_IsOptional) != 0);
	}
	inline bool HasPMapBit() const
	{
		return((flags_ & InsItemFlag_HasPMapBit) != 0);
	}
	inline bool IsNullable() const
	{
		return((flags_ & InsItemFlag_IsNullable) != 0);
	}
	inline bool HasFieldId() const
	{
		return((flags_ & InsItemFlag_HasFieldId) != 0);
	}
	inline bool HasInitialValue() const
	{
		return((flags_ & InsItemFlag_HasInitialValue) != 0);
	}
	inline bool AggNeedsPMapBit() const
	{
		return((flags_ & InsItemFlag_AggNeedsPMapBit) != 0);
	}
	inline bool IsInSequence() const
	{
		return((flags_ & InsItemFlag_IsInSequence) != 0);
	}
	inline bool IsInGroup() const
	{
		return((flags_ & InsItemFlag_IsInGroup) != 0);
	}
	inline bool IsIn() const
	{
		return((flags_ & InsItemFlag_IsIn) != 0);
	}
	inline bool IsDecimalExp() const
	{
		return((flags_ & InsItemFlag_IsDecimalExp) != 0);
	}
	inline bool IsDecimalMan() const
	{
		return((flags_ & InsItemFlag_IsDecimalMan) != 0);
	}
	inline bool IsDecimalChild() const
	{
		return((flags_ & InsItemFlag_IsDecimalChild) != 0);
	}
	inline bool IsSeqLength() const
	{
		return((flags_ & InsItemFlag_IsSeqLength) != 0);
	}
	inline bool IsByteLength() const
	{
		return((flags_ & InsItemFlag_IsByteLength) != 0);
	}
	inline bool IsLength() const
	{
		return((flags_ & InsItemFlag_IsLength) != 0);
	}
	inline bool IsGenerated() const
	{
		return((flags_ & InsItemFlag_IsGenerated) != 0);
	}

	//	InsItemFlag set accessors..
	inline void SetIsOptional()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsOptional);
	}
	inline void SetHasPMapBit()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_HasPMapBit);
	}
	inline void SetIsNullable()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsNullable);
	}
	inline void SetHasFieldId()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_HasFieldId);
	}
	inline void SetHasInitialValue()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_HasInitialValue);
	}
	inline void SetAggNeedsPMapBit()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_AggNeedsPMapBit);
	}
	inline void SetIsInSequence()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsInSequence);
	}
	inline void SetIsInGroup()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsInGroup);
	}
	inline void SetIsIn()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsIn);
	}
	inline void SetIsDecimalExp()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsDecimalExp);
	}
	inline void SetIsDecimalMan()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsDecimalMan);
	}
	inline void SetIsDecimalChild()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsDecimalChild);
	}
	inline void SetIsSeqLength()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsSeqLength);
	}
	inline void SetIsByteLength()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsByteLength);
	}
	inline void SetIsLength()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsLength);
	}
	inline void SetIsGenerated()
	{
			flags_ = static_cast<InsItemFlag>(flags_ | InsItemFlag_IsGenerated);
	}

	//	InsItemFlag set accessors..
	inline void ClearIsOptional()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsOptional);
	}
	inline void ClearHasPMapBit()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_HasPMapBit);
	}
	inline void ClearIsNullable()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsNullable);
	}
	inline void ClearHasFieldId()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_HasFieldId);
	}
	inline void ClearHasInitialValue()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_HasInitialValue);
	}
	inline void ClearAggNeedsPMapBit()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_AggNeedsPMapBit);
	}
	inline void ClearIsInSequence()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsInSequence);
	}
	inline void ClearIsInGroup()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsInGroup);
	}
	inline void ClearIsIn()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsIn);
	}
	inline void ClearIsDecimalExp()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsDecimalExp);
	}
	inline void ClearIsDecimalMan()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsDecimalMan);
	}
	inline void ClearIsDecimalChild()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsDecimalChild);
	}
	inline void ClearIsSeqLength()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsSeqLength);
	}
	inline void ClearIsByteLength()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsByteLength);
	}
	inline void ClearIsLength()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsLength);
	}
	inline void ClearIsGenerated()
	{
		flags_ = static_cast<InsItemFlag>(flags_ & ~InsItemFlag_IsGenerated);
	}

	bool IsTypeInt() const;
	bool IsTypeSInt() const;
	bool IsTypeUInt() const;
	bool IsTypeInt32() const;
	bool IsTypeInt64() const;
	bool IsTypeNumeric() const;
	bool IsTypeCharArray() const;
	bool IsTypePrimitive() const;

	bool IsTypeComposite() const;

	std::string GetDescriptiveText() const;

	void SetDictionaryName(const std::string &src_name);
	void SetKeyName(const std::string &src_name);

	std::string EmitDatum() const;
	std::string EmitDatumFull(const DictValueList &dict_value_list) const;

	unsigned int  template_index_;
	unsigned int  parent_index_;
	DataType      data_type_;
	InsItemFlag   flags_;
	unsigned int  auxiliary_id_;
	FieldOperator field_operator_;
	unsigned int  item_index_;
	unsigned int  element_count_;
	unsigned int  element_count_compat_;
	unsigned int  dict_value_index_;
	unsigned int  dict_value_count_;
	unsigned int  static_ref_index_;
	std::string   field_name_;
	std::string   dictionary_name_;
	std::string   key_name_;
	DictValue    *dict_value_ptr_;
	

	static void ParseTemplateElement(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemListI &item_list, DictValueList &dict_value_list);

	static void DumpItemList(const InsItemListI &item_list);
	static void DumpItemList(std::size_t item_count,
		const InsItem *item_list);
	static void DumpItemListFull(const InsItemListI &item_list,
		const DictValueList &dict_value_list);
	static void DumpItemListFull(std::size_t item_count,
		const InsItem *item_list, const DictValueList &dict_value_list);

	static std::string EmitHeaderText();
	static std::string EmitFillLineHeader();
	static std::string EmitFillLine(int fill_char);
	static std::string EmitHeaderTextFull();
	static std::string EmitFillLineHeaderFull();
	static std::string EmitFillLineFull(int fill_char);

private:
	//	Ctor for individual template constituent elements...
	InsItem(DataType data_type, unsigned int fix_tag_id,
		const std::string &field_name);

	static void ParseTemplateElementInternal(
		const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_SInt32(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_UInt32(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_SInt64(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_UInt64(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_Decimal(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_DecimalExponent(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_DecimalMantissa(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_String(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_ByteVector(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_Length(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_Group(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_TemplateRef(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_TypeRef(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_Template(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);
	static void ParseElement_Sequence(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list);

	template <typename IntegralType>
	static void ParseElement_Integral(const InsCtxInfo &ins_ctx_info,
		const MLB::XercesUtils::XmlDomElement &xml_element,
		InsItemFlag parent_flags, InsItemListI &item_list,
		DictValueList &dict_value_list, bool name_required_flag);

	void InitFromParseContext(const InsCtxInfo &ins_ctx_info);
	void ProcessAttribute_Presence(const std::string &attr_value);
	//	\return Boolean \e true if element 'charset' = 'ascii'
	bool ProcessAttribute_CharSet(const std::string &attr_value);
	void ProcessAttribute_Dictionary(const std::string &attr_value);
	void ProcessAttribute_Key(const std::string &attr_value);

	static void FixUpFieldFlagsDecimalMultiOp(InsItem *item_ptr);
	static void FixUpFieldFlags(InsItem *item_ptr);
	       void FixUpFieldFlags();

	static unsigned int GetAuxiliaryId(
		const MLB::XercesUtils::XmlDomElement &xml_element,
		bool throw_on_error = true);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef InsItem::InsItemListI       InsItemList;
typedef InsItemList::iterator       InsItemListIter;
typedef InsItemList::const_iterator InsItemListIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const InsItem &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifndef HH__MLB__VFast__InsItem_hpp__HH

