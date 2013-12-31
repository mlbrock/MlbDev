// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the ExcContext class.

	Revision History	:	2008-11-24 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__ExcContext_hpp__HH

#define HH__MLB__VFast__ExcContext_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/InsContext.hpp>
#include <VFast/ExcResults.hpp>
#include <VFast/ExcContextInfo.hpp>
#include <VFast/SpecException.hpp>

#include <boost/shared_array.hpp>

//	CODE NOTE: Temporary until TransferDecode logic is refactored.
#include <VFast/TransferDecode.hpp>

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef VFAST_DEBUG_EXC_ENTRY
# define VFAST_DEBUG_EXC_ENTRY_PMAP_SET()	\
	last_entry_ptr_->pmap_flag_ = true
# define VFAST_DEBUG_EXC_ENTRY_PMAP_CLR()	\
	last_entry_ptr_->pmap_flag_ = false

# define VFAST_DEBUG_EXC_ENTRY_END_PTR_SET()					\
	last_entry_ptr_->end_ptr_ = src_begin_ptr_
# define VFAST_DEBUG_EXC_ENTRY_END_PTR_CLR()					\
	last_entry_ptr_->end_ptr_ = last_entry_ptr_->begin_ptr_;
# define VFAST_DEBUG_EXC_ENTRY_END_PTR_RESET(begin_ptr)	\
	last_entry_ptr_->end_ptr_ = (begin_ptr)
#else
# define VFAST_DEBUG_EXC_ENTRY_PMAP_SET()
# define VFAST_DEBUG_EXC_ENTRY_PMAP_CLR()

# define VFAST_DEBUG_EXC_ENTRY_END_PTR_SET()
# define VFAST_DEBUG_EXC_ENTRY_END_PTR_CLR()
# define VFAST_DEBUG_EXC_ENTRY_END_PTR_RESET(begin_ptr)
#endif // #ifdef VFAST_DEBUG_EXC_ENTRY
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	Specifies the default maximum number of different data fields which may
	be decoded from a single FAST message.
*/
const unsigned int MaxExcEntryCount             = 2048;

/**
	Specifies the default size of the area into which fields decoded from a
	single FAST message will be placed.
*/
const unsigned int DefaultExcDataBufferLength   = 65536 * 4;

/**
	\note This \e must be (substantially) less than MaxExcDataBufferLength.
*/
const unsigned int MaxTransferDecodeFieldLength = 1023;

const unsigned int MaxExcDataBufferLength       =
	DefaultExcDataBufferLength - MaxTransferDecodeFieldLength;
// ////////////////////////////////////////////////////////////////////////////

namespace {
// ////////////////////////////////////////////////////////////////////////////
/*
	CODE NOTE: Temporary until TransferDecode logic is refactored.

	Wrapper functions to be replaced when transfer decode functions are
	changed.
*/
inline void XTransferDecode_Int(DataType data_type,
	const StreamDatum *&begin_ptr, const StreamDatum *end_ptr, void *datum_ptr)
{
	if (data_type == DataType_SInt32)
		TransferDecode_SInt32(begin_ptr, end_ptr,
			*static_cast<Type_SInt32 *>(datum_ptr));
	else if (data_type == DataType_UInt32)
		TransferDecode_UInt32(begin_ptr, end_ptr,
			*static_cast<Type_UInt32 *>(datum_ptr));
	else if (data_type == DataType_SInt64)
		TransferDecode_SInt64(begin_ptr, end_ptr,
			*static_cast<Type_SInt64 *>(datum_ptr));
	else if (data_type == DataType_UInt64)
		TransferDecode_UInt64(begin_ptr, end_ptr,
			*static_cast<Type_UInt64 *>(datum_ptr));
	else
		MLB::Utility::ThrowLogicError("Encountered an unexpected type (" +
			DataTypeToString(data_type) + " where an integral type was expected.");
}
inline void XTransferDecode_Int_Null(DataType data_type,
	const StreamDatum *&begin_ptr, const StreamDatum *end_ptr, void *datum_ptr,
	bool &is_null)
{
	if (data_type == DataType_SInt32)
		TransferDecode_SInt32_Null(begin_ptr, end_ptr,
			*static_cast<Type_SInt32 *>(datum_ptr), is_null);
	else if (data_type == DataType_UInt32)
		TransferDecode_UInt32_Null(begin_ptr, end_ptr,
			*static_cast<Type_UInt32 *>(datum_ptr), is_null);
	else if (data_type == DataType_SInt64)
		TransferDecode_SInt64_Null(begin_ptr, end_ptr,
			*static_cast<Type_SInt64 *>(datum_ptr), is_null);
	else if (data_type == DataType_UInt64)
		TransferDecode_UInt64_Null(begin_ptr, end_ptr,
			*static_cast<Type_UInt64 *>(datum_ptr), is_null);
	else
		MLB::Utility::ThrowLogicError("Encountered an unexpected type (" +
			DataTypeToString(data_type) + " where an integral type was expected.");
}
inline void XTransferDecode_Ascii(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, unsigned int max_bytes,
	unsigned int &datum_length, char *datum_ptr)
{
	TransferDecode_Ascii(begin_ptr, end_ptr, max_bytes, datum_length,
		datum_ptr);
}
inline void XTransferDecode_Ascii_Null(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, unsigned int max_bytes,
	unsigned int &datum_length, char *datum_ptr, bool &is_null)
{
	TransferDecode_Ascii_Null(begin_ptr, end_ptr, max_bytes, datum_length,
		datum_ptr, is_null);
}
inline void XTransferDecode_ByteVector(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, unsigned int max_bytes,
	unsigned int &datum_length, char *datum_ptr)
{
	TransferDecode_ByteVector(begin_ptr, end_ptr, max_bytes, datum_length,
		datum_ptr);
}
inline void XTransferDecode_ByteVector_Null(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, unsigned int max_bytes,
	unsigned int &datum_length, char *datum_ptr, bool &is_null)
{
	TransferDecode_ByteVector_Null(begin_ptr, end_ptr, max_bytes, datum_length,
		datum_ptr, is_null);
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief This execution context class controls the parsing of FAST messages.

	Some members of this class depend upon the elements of a \c std::vector
	being contiguous in memory --- something which is almost certainly true
	in C++98 and guaranteed by subsequent standards. But if it should happen
	to \e not be the case for your compiler, you will have to refactor the
	logic in the members of this class.
*/
class ExcContext {
public:
	friend struct ExcContextDispatchLink;
	typedef void (ExcContext::*FieldDecodeDispatch)(unsigned int item_index,
		unsigned int sequence_number);
	typedef boost::shared_array<FieldDecodeDispatch> FieldDecodeDispatchList;

	/**
		Constructs a FAST execution context \c ExcContext instance.

		\param ins_context is a reference to the instruction context which is
		to be used in constructing the new execution context.

		\param exc_context_info is a reference to an instance of
		\c ExcContextInfo (or class derived from it) which will be used to
		provide fine-grained guidance in the parsing of FAST messages.
	*/
	explicit ExcContext(const InsContext &ins_context,
		const ExcContextInfo &exc_context_info = ExcContextInfo());

	/**
		Parses a stream of bytes as a FAST message.

		\param src_begin_ptr points to the first byte of the stream to be
		parsed.

		\param src_end_ptr points to the byte after the last byte of the
		stream to be parsed.

		\return A pointer to the byte after the last byte which was parsed
		by this method.

		- For byte streams which contain a single FAST message, this return
		value will be equal to the \e src_end_ptr parameter.

		- For byte streams which contain multiple FAST messages, this return
		value will point to the first byte of the next FAST message in the
		stream.
	*/
	const StreamDatum *FieldDecode(const StreamDatum *src_begin_ptr,
		const StreamDatum *src_end_ptr);

	/**
		Parses a stream of bytes as a FAST message.

		\param src_begin_ptr points to the first byte of the stream to be
		parsed.

		\param src_end_ptr points to the byte after the last byte of the
		stream to be parsed.

		\param exc_results is a reference to an \c ExcResults into which
		this method will place data from the parsed FAST message.
 
		\return A pointer to the byte after the last byte which was parsed
		by this method.

		- For byte streams which contain a single FAST message, this return
		value will be equal to the \e src_end_ptr parameter.

		- For byte streams which contain multiple FAST messages, this return
		value will point to the first byte of the next FAST message in the
		stream.
	*/
	const StreamDatum *FieldDecode(const StreamDatum *src_begin_ptr,
		const StreamDatum *src_end_ptr, ExcResults &exc_results);

	/**
		Parses a stream of bytes as a FAST message.

		\param src_begin_ptr points to the first byte of the stream to be
		parsed.

		\param src_end_ptr points to the byte after the last byte of the
		stream to be parsed.

		\param template_id is the template identifier of the message to be
		parsed. This value must be equal to the template identifier of the
		immediately preceding message.

		\return A pointer to the byte after the last byte which was parsed
		by this method.

		- For byte streams which contain a single FAST message, this return
		value will be equal to the \e src_end_ptr parameter.

		- For byte streams which contain multiple FAST messages, this return
		value will point to the first byte of the next FAST message in the
		stream.
	*/
	const StreamDatum *FieldDecode(const StreamDatum *src_begin_ptr,
		const StreamDatum *src_end_ptr, unsigned int template_id);

	/**
		Gets results data from the last parsed FAST message.

		\param exc_results is a reference to an \c ExcResults into which
		this method will place results data from the last parsed FAST message.
	*/
	void GetDecodeResults(ExcResults &exc_results) const;

	/**
		Resets the FAST field dictionary for the specified template.

		\param template_id specifies the template for which the field
		dictionary is to be reset.
	*/
	void ResetTemplateDictionary(unsigned int template_id);

	/**
		Resets the FAST field dictionary for the specified template.

		\param template_name specifies the template for which the field
		dictionary is to be reset.
	*/
	void ResetTemplateDictionary(const std::string &template_name);

	/**
		Resets the FAST field dictionary for the specified template.

		\param template_name specifies the template for which the field
		dictionary is to be reset.
	*/
	void ResetTemplateDictionary(const char *template_name);

	/**
		Resets the FAST field dictionary for the template of the last FAST
		message processed by the \c ExcContext instance.

		\note At least one FAST message must have been parsed by the
		\c ExcContext instance or this method will throw.
	*/
	void ResetLastTemplateDictionary();

	/**
		Resets the FAST field dictionaries for all templates.
	*/
	void ResetDictionaries();

	/**
		Resets the \c ExcContext destination data members to their default
		or to their initial values. These members are:

		- curr_entry_begin_ptr_
		- last_entry_ptr_
		- curr_dst_begin_ptr_
	*/
	void ResetDst();

	/**
		Resets the \c ExcContext destination data members to their default
		or to their initial values. These members are:

		- src_begin_ptr_
		- src_end_ptr_
		- curr_src_begin_ptr_
		- pmap_begin_ptr_
		- pmap_end_ptr_
		- pmap_mask_bit_

		\param src_begin_ptr is the value to which the \e src_begin_ptr_
		member datum is to be set. It may be \e NULL .

		\param src_end_ptr is the value to which the \e src_end_ptr_
		member datum is to be set. If \e src_end_ptr is \e NULL , then
		this parameter must also be \e NULL . Otherwise, it must not be
		\e NULL .
	*/
	void ResetSrc(const StreamDatum *src_begin_ptr = NULL,
		const StreamDatum *src_end_ptr = NULL);

	/**
		Resets the \c ExcContext destination data members to their default
		or to their initial values. These members are:

		- src_begin_ptr_
		- src_end_ptr_
		- curr_src_begin_ptr_
		- pmap_begin_ptr_
		- pmap_end_ptr_
		- pmap_mask_bit_

		\param src_begin_ptr is the value to which the \e src_begin_ptr_
		member datum is to be set. It must not be \e NULL .

		\param src_end_ptr is the value to which the \e src_end_ptr_
		member datum is to be set. It must not be \e NULL .
	*/
	void ResetSrcChecked(const StreamDatum *src_begin_ptr,
		const StreamDatum *src_end_ptr);

	/**
		Returns the number of entries resulting from parsing the last FAST
		message.
	*/
	std::size_t GetEntryCount() const
	{
		return(static_cast<std::size_t>(
			curr_entry_begin_ptr_ - entry_begin_ptr_));
	}

	/**
		Returns a \c ExcEntry pointer to the first of the entries which
		resulted from parsing the last FAST message.
	*/
	const ExcEntry *GetEntryList() const
	{
		return(entry_begin_ptr_);
	}

	/**
		Returns the template identifier of the last FAST message parsed.
	*/
	unsigned int GetLastTemplateId() const
	{
		return(last_template_id_);
	}

	/**
		Returns the length of the presence map in bytes in the last FAST
		message parsed.
	*/
	unsigned int GetLastPMapLength() const
	{
		return(last_pmap_length_);
	}

	/**
		Returns the length in bytes of the last FAST message parsed.
	*/
	unsigned int GetLastSrcDataLength() const
	{
		return(static_cast<unsigned int>(src_begin_ptr_ - curr_src_begin_ptr_));
	}

	/**
		Gets a reference to the \c InsItemList used by this \c ExcContext
		instance for parsing FAST messages.
	*/
	const InsItemList &GetInsItemList() const
	{
		return(ins_context_.ins_item_list_);
	}

	/**
		Constructs a \c std::vector of strings which constitutes a human-readable
		report of the field dispatch functions for the current instruction
		context.

		\return A \c std::vector which contains a \c std::string for each line
		in the instruction context field dispatch report.
	*/
	std::string GetFieldDispatchReport() const;

	/**
		Checks the specified instruction context to determine whether all
		contained \c InsItem elements can be resolved for the purposes of
		execution context field dispatch.

		\param ins_context is the instruction context to be checked.

		\param error_list will be populated with a description of any
		errors in field dispatch resolution found.

		\return True if no no were found, \e false otherwise.

		\note This function was used in the development of the execution
		context logic. There's probably no reason for application programmers
		to call this method.
	*/
	static bool CheckFieldDispatch(const InsContext &ins_context,
		std::vector<std::string> &error_list);

	/**
		Checks the specified list of instruction context item elements to
		determine whether all \c InsItem elements contained in the list can
		be resolved for the purposes of execution context field dispatch.

		\param item_list is the list of instruction items to be checked.

		\param error_list will be populated with a description of any
		errors in field dispatch resolution found.

		\return True if no errors were found, \e false otherwise.

		\note This function was used in the development of the execution
		context logic. There's probably no reason for application programmers
		to call this method.
	*/
	static bool CheckFieldDispatch(const InsItemList &item_list,
		std::vector<std::string> &error_list);

private:
	ExcContextInfo                       exc_context_info_;
	InsContext                           ins_context_;
	DictValueList                        active_dict_;
	FieldDecodeDispatchList              dispatch_list_;
	FieldDecodeDispatch                 *dispatch_list_ptr_;
	unsigned int                         last_template_id_;
	unsigned int                         last_template_index_;
	unsigned int                         last_pmap_length_;
	boost::shared_array<ExcEntry>        entry_list_;
	boost::shared_array<char>            dst_buffer_;
	boost::shared_array<ExcEntryLookup>  lookup_list_sptr_;
	ExcEntry                            *entry_begin_ptr_;
	ExcEntry                            *entry_end_ptr_;
	ExcEntry                            *curr_entry_begin_ptr_;
	ExcEntry                            *last_entry_ptr_;
	char                                *dst_begin_ptr_;
	char                                *dst_end_ptr_;
	char                                *curr_dst_begin_ptr_;
	const StreamDatum                   *src_begin_ptr_;
	const StreamDatum                   *src_end_ptr_;
	const StreamDatum                   *curr_src_begin_ptr_;
	const StreamDatum                   *pmap_begin_ptr_;
	const StreamDatum                   *pmap_end_ptr_;
	unsigned int                         pmap_mask_bit_;
	InsItem                             *current_item_ptr_;

	void ResetTemplateByIndex(unsigned int template_index);

	const StreamDatum *FieldDecodeList(unsigned int start_index,
		unsigned int item_count, const InsItem *item_list,
		unsigned int sequence_number);

	void EnsureDstMem(std::size_t datum_length);
	void MoveCurrDstPtr(std::size_t move_length);
	void AlignDstMem(unsigned int datum_alignment);

	ExcEntry *AddEntry(unsigned int item_index, unsigned int sequence_number,
		unsigned int datum_alignment, unsigned int datum_length,
		bool alloc_plus, bool is_null);

	class PMapStateSaver {
	public:
		inline PMapStateSaver(ExcContext * const exc_context_ptr)
			:exc_context_ref_(*exc_context_ptr)
			,pmap_begin_ptr_(exc_context_ptr->pmap_begin_ptr_)
			,pmap_end_ptr_(exc_context_ptr->pmap_end_ptr_)
			,pmap_mask_bit_(exc_context_ptr->pmap_mask_bit_)
		{
		}

		inline ~PMapStateSaver()
		{
			exc_context_ref_.pmap_begin_ptr_ = pmap_begin_ptr_;
			exc_context_ref_.pmap_end_ptr_   = pmap_end_ptr_;
			exc_context_ref_.pmap_mask_bit_  = pmap_mask_bit_;
		}

	private:
		ExcContext        &exc_context_ref_;
		const StreamDatum *pmap_begin_ptr_;
		const StreamDatum *pmap_end_ptr_;
		unsigned int       pmap_mask_bit_;

		//	Not implemented.
		PMapStateSaver(const PMapStateSaver &other);
		PMapStateSaver & operator = (const PMapStateSaver &other);
	};

	inline void DispatchToItem(unsigned int item_index,
		unsigned int sequence_number)
	{
		(this->*(dispatch_list_ptr_[item_index]))(item_index, sequence_number);
	}

	void FixUpDictValuePtrs();

	static FieldDecodeDispatchList FixUpDispatch(const InsItemList &item_list);
	static FieldDecodeDispatchList FixUpDispatch(const InsItemList &item_list,
		std::vector<std::string> &error_list, bool throw_on_error);

	inline void GetPMap()
	{
		pmap_begin_ptr_ = src_begin_ptr_;

		src_begin_ptr_ += TransferDecode_Datum_Length(DataType_Ascii,
			src_begin_ptr_, src_end_ptr_, MaxPMapByteLength, false);

		pmap_end_ptr_ = src_begin_ptr_;
	}

	inline bool GetPMapBit()
	{
		if (pmap_begin_ptr_ >= pmap_end_ptr_)
			return(false);

		bool bit_is_set = ((*pmap_begin_ptr_ & pmap_mask_bit_) != 0);

		if (pmap_mask_bit_ != 1)
			pmap_mask_bit_ >>= 1;
		else {
			++pmap_begin_ptr_;
			pmap_mask_bit_ = FirstPMapBit;
		}

		return(bit_is_set);
	}

	inline bool IsPMapBitSet()
	{
		return((pmap_begin_ptr_ >= pmap_end_ptr_) ? false :
			((*pmap_begin_ptr_ & pmap_mask_bit_) != 0));
	}

	inline DictValue *GetDictValuePtr(unsigned int item_index)
	{
		return(ins_context_.ins_item_list_[item_index].dict_value_ptr_);
	}

	// //////////////////////////////////////////////////////////////////////
	//	Integral types default here...
	template <DataType DType> void AddEntryForType(unsigned int item_index,
		unsigned int sequence_number)
	{
		AddEntry(item_index, sequence_number,
			sizeof(typename TypeEnumToType<DType>::value_type),
			sizeof(typename TypeEnumToType<DType>::value_type), false, false);
	}
	//	----------------------------------------------------------------------
	template <DataType DType> void AddEntryForType_Null(unsigned int item_index,
		unsigned int sequence_number)
	{
		AddEntry(item_index, sequence_number,
			sizeof(typename TypeEnumToType<DType>::value_type),
			sizeof(typename TypeEnumToType<DType>::value_type), false, true);
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Integral types default here...
	template <DataType DType> void AddEntryDictValueForType(
		unsigned int item_index, unsigned int sequence_number)
	{
		const DictValue *dict_value_ptr = GetDictValuePtr(item_index);

		AddEntry(item_index, sequence_number,
			sizeof(typename TypeEnumToType<DType>::value_type),
			sizeof(typename TypeEnumToType<DType>::value_type), false,
			dict_value_ptr->IsNull());

		*reinterpret_cast<typename TypeEnumToType<DType>::value_type *>(
			last_entry_ptr_->datum_ptr_) =
			*dict_value_ptr->GetDatumPtr<typename TypeEnumToType<DType>::value_type>();
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Integral types default here...
	template <DataType DType> void SetEntryFromDictValueForType(
		const DictValue *dict_value_ptr)
	{
		*reinterpret_cast<typename TypeEnumToType<DType>::value_type *>(
			last_entry_ptr_->datum_ptr_) =
			*dict_value_ptr->GetDatumPtr<typename TypeEnumToType<DType>::value_type>();
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Integral types default here...
	template <DataType DType> void SetDictValueForType(
		DictValue::DictValueState data_state)
	{
		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		*dict_value_ptr = *reinterpret_cast<typename TypeEnumToType<DType>::value_type *>(
			last_entry_ptr_->datum_ptr_);

		dict_value_ptr->data_flag_  = (last_entry_ptr_->is_null_) ?
			DictValue::DictValueFlag_IsNull : DictValue::DictValueFlag_None;
		dict_value_ptr->data_state_ = data_state;
	}
	inline void SetDictValueForType_String(DictValue::DictValueState data_state)
	{
		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		dict_value_ptr->string_length_ = last_entry_ptr_->datum_length_;

		::memcpy(dict_value_ptr->string_, last_entry_ptr_->datum_ptr_,
			last_entry_ptr_->datum_length_ + 1);

		dict_value_ptr->data_flag_  = (last_entry_ptr_->is_null_) ?
			DictValue::DictValueFlag_IsNull : DictValue::DictValueFlag_None;
		dict_value_ptr->data_state_ = data_state;
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Integral types default here...
	template <DataType DType> void DecodeForType()
	{
		XTransferDecode_Int(DType, src_begin_ptr_, src_end_ptr_,
			last_entry_ptr_->datum_ptr_);
		VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
	}
	//	----------------------------------------------------------------------
	//	Integral types default here...
	template <DataType DType> void DecodeForType_Null()
	{
		XTransferDecode_Int_Null(DType, src_begin_ptr_, src_end_ptr_,
			last_entry_ptr_->datum_ptr_, last_entry_ptr_->is_null_);
		VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
	}
	// //////////////////////////////////////////////////////////////////////

	//	CODE NOTE: The dispatch functions are to be made private again!!!
public:
	void FieldDecode_Template(unsigned int item_index,
		unsigned int sequence_number);
	void FieldDecode_TemplateRefStatic(unsigned int item_index,
		unsigned int sequence_number);
	void FieldDecode_Sequence_A(unsigned int item_index,
		unsigned int sequence_number);
	void FieldDecode_Sequence_X(unsigned int item_index,
		unsigned int sequence_number);
	void FieldDecode_Group_P(unsigned int item_index,
		unsigned int sequence_number);
	void FieldDecode_Group_X(unsigned int item_index,
		unsigned int sequence_number);

	inline void FieldDecode_ByteLength(unsigned int item_index,
		unsigned int sequence_number)
	{
		AddEntryForType<DataType_UInt32>(item_index, sequence_number);
		*reinterpret_cast<Type_UInt32 *>(last_entry_ptr_->datum_ptr_) =
			last_entry_ptr_[-1].datum_length_;
		last_entry_ptr_->is_null_ = last_entry_ptr_[-1].is_null_;
	}
	// //////////////////////////////////////////////////////////////////////
	//	Dispatch Decimal...
	inline void FieldDecode_Decimal_PO(unsigned int item_index,
		unsigned int sequence_number)
	{
		if (!GetPMapBit()) {
			AddEntryForType_Null<DataType_Decimal>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
			AddEntryForType_Null<DataType_SInt32>(++item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
			AddEntryForType_Null<DataType_SInt64>(++item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
		}
		else
			FieldDecode_Decimal_XO(item_index, sequence_number);
	}
	inline void FieldDecode_Decimal_XO(unsigned int item_index,
		unsigned int sequence_number)
	{
		AddEntryForType<DataType_Decimal>(item_index, sequence_number);

		DispatchToItem(item_index + 1, sequence_number);

		if (last_entry_ptr_->is_null_) {
			last_entry_ptr_[-1].is_null_ = true;
			AddEntryForType_Null<DataType_SInt64>(item_index + 2, sequence_number);
			return;
		}

		DispatchToItem(item_index + 2, sequence_number);

		FieldDecode_Decimal_Finalize();
	}
	inline void FieldDecode_Decimal_PX(unsigned int item_index,
		unsigned int sequence_number)
	{
		if (!GetPMapBit()) {
			AddEntryForType_Null<DataType_Decimal>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
			AddEntryForType_Null<DataType_SInt32>(++item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
			AddEntryForType_Null<DataType_SInt64>(++item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
		}
		else
			FieldDecode_Decimal_XX(item_index, sequence_number);
	}
	inline void FieldDecode_Decimal_XX(unsigned int item_index,
		unsigned int sequence_number)
	{
		AddEntryForType<DataType_Decimal>(item_index, sequence_number);

		DispatchToItem(item_index + 1, sequence_number);
		DispatchToItem(item_index + 2, sequence_number);

		FieldDecode_Decimal_Finalize();
	}
	inline void FieldDecode_Decimal_Finalize()
	{
		Type_SInt32 exp =
			*reinterpret_cast<Type_SInt32 *>(last_entry_ptr_[-1].datum_ptr_);

		if ((exp < -63) || (exp >  63))
			MLB::Utility::ThrowLogicError("The decimal field exponent (" +
				 MLB::Utility::AnyToString(exp) + ") is outside the "
				"permissible range (-63 to 63, inclusive).");

		Type_SInt64 man =
			*reinterpret_cast<Type_SInt64 *>(last_entry_ptr_->datum_ptr_);

		*reinterpret_cast<Type_Decimal *>(last_entry_ptr_[-2].datum_ptr_) =
			static_cast<double>(man) * ::pow(10.0, static_cast<double>(exp));
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Dispatch field operator = None...
	template <DataType DType> inline void FieldDecode_Non_PO(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			FieldDecode_Non_XO<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_SET();
		}
		else {
			AddEntryForType_Null<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
		}
	}
	template <DataType DType> inline void FieldDecode_Non_XO(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);
		DecodeForType_Null<DType>();
	}
	template <DataType DType> inline void FieldDecode_Non_PX(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			FieldDecode_Non_XX<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_SET();
		}
		else {
			AddEntryForType<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
		}
	}
	template <DataType DType> inline void FieldDecode_Non_XX(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);
		DecodeForType<DType>();
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Dispatch field operator = Constant...
	template <DataType DType> inline void FieldDecode_Con_O(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			FieldDecode_Con_X<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_SET();
		}
		else {
			AddEntryForType_Null<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
		}
	}
	template <DataType DType> inline void FieldDecode_Con_X(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryDictValueForType<DType>(item_index, sequence_number);
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Dispatch field operator = Default...
	template <DataType DType> inline void FieldDecode_Def_PO(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			AddEntryForType<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_SET();
			DecodeForType_Null<DType>();
		}
		else {
			AddEntryDictValueForType<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
		}
	}
	template <DataType DType> inline void FieldDecode_Def_PX(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			AddEntryForType<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_SET();
			DecodeForType<DType>();
		}
		else {
			AddEntryDictValueForType<DType>(item_index, sequence_number);
			VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();
		}
	}
	template <DataType DType> inline void FieldDecode_Def_XO(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);
		VFAST_DEBUG_EXC_ENTRY_PMAP_SET();
		DecodeForType_Null<DType>();
	}
	template <DataType DType> inline void FieldDecode_Def_XX(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);
		VFAST_DEBUG_EXC_ENTRY_PMAP_SET();
		DecodeForType<DType>();
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Dispatch field operator = Copy...
	template <DataType DType> inline void FieldDecode_Cop_OI(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);

		if (GetPMapBit()) {
			DecodeForType_Null<DType>();
			if (!last_entry_ptr_->is_null_)
				SetDictValueForType<DType>(DictValue::DictValueState_Assigned);
			else
				SetDictValueForType<DType>(DictValue::DictValueState_Empty);
			return;
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned())
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		else if (dict_value_ptr->IsUndefined()) {
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			dict_value_ptr->SetIsAssigned();
		}
		else
			last_entry_ptr_->is_null_ = true;
	}
	template <DataType DType> inline void FieldDecode_Cop_XI(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);

		if (GetPMapBit()) {
			DecodeForType<DType>();
			SetDictValueForType<DType>(DictValue::DictValueState_Assigned);
			return;
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned())
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		else if (dict_value_ptr->IsUndefined()) {
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			dict_value_ptr->SetIsAssigned();
		}
		else
			SpecExceptionDynamic::ThrowException(6, "Operator Copy encountered "
				"with a mandatory field which is empty.");
	}
	template <DataType DType> inline void FieldDecode_Cop_OX(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType_Null<DType>(item_index, sequence_number);

		if (GetPMapBit()) {
			DecodeForType_Null<DType>();
			if (!last_entry_ptr_->is_null_)
				SetDictValueForType<DType>(DictValue::DictValueState_Assigned);
			else
				SetDictValueForType<DType>(DictValue::DictValueState_Empty);
			return;
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned()) {
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			last_entry_ptr_->is_null_ = false;
		}
		else if (dict_value_ptr->IsUndefined())
			dict_value_ptr->SetIsEmpty();
		else
			last_entry_ptr_->is_null_ = true;
	}
	template <DataType DType> inline void FieldDecode_Cop_XX(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);

		if (GetPMapBit()) {
			DecodeForType<DType>();
			SetDictValueForType<DType>(DictValue::DictValueState_Assigned);
			return;
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned())
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		else if (dict_value_ptr->IsUndefined())
			SpecExceptionDynamic::ThrowException(5, "Operator Copy encountered "
				"with a mandatory field which is Undefined.");
		else
			SpecExceptionDynamic::ThrowException(6, "Operator Copy encountered "
				"with a mandatory field which is Empty.");
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Dispatch field operator = Increment...
	template <DataType DType> inline void FieldDecode_Inc_OI(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType_Null<DType>(item_index, sequence_number);

		if (GetPMapBit()) {
			DecodeForType_Null<DType>();
			if (!last_entry_ptr_->is_null_)
				SetDictValueForType<DType>(DictValue::DictValueState_Assigned);
			return;
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned()) {
			++(*dict_value_ptr->GetDatumPtr<typename TypeEnumToType<DType>::value_type>());
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			last_entry_ptr_->is_null_ = false;
		}
		else if (dict_value_ptr->IsUndefined()) {
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			dict_value_ptr->SetIsAssigned();
		}
	}
	template <DataType DType> inline void FieldDecode_Inc_XI(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);

		if (GetPMapBit()) {
			DecodeForType<DType>();
			SetDictValueForType<DType>(DictValue::DictValueState_Assigned);
			return;
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned()) {
			++(*dict_value_ptr->GetDatumPtr<typename TypeEnumToType<DType>::value_type>());
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		}
		else if (dict_value_ptr->IsUndefined()) {
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			dict_value_ptr->SetIsAssigned();
		}
		else
			SpecExceptionDynamic::ThrowException(6, "Operator Copy encountered "
				"with a mandatory field which is empty.");
	}
	template <DataType DType> inline void FieldDecode_Inc_OX(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType_Null<DType>(item_index, sequence_number);

		if (GetPMapBit()) {
			DecodeForType_Null<DType>();
			if (!last_entry_ptr_->is_null_)
				SetDictValueForType<DType>(DictValue::DictValueState_Assigned);
			return;
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned()) {
			++(*dict_value_ptr->GetDatumPtr<typename TypeEnumToType<DType>::value_type>());
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			last_entry_ptr_->is_null_ = false;
		}
		else if (dict_value_ptr->IsUndefined())
			dict_value_ptr->SetIsEmpty();
	}
	template <DataType DType> inline void FieldDecode_Inc_XX(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);

		if (GetPMapBit()) {
			DecodeForType<DType>();
			SetDictValueForType<DType>(DictValue::DictValueState_Assigned);
			return;
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned()) {
			++(*dict_value_ptr->GetDatumPtr<typename TypeEnumToType<DType>::value_type>());
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		}
		else if (dict_value_ptr->IsUndefined())
			SpecExceptionDynamic::ThrowException(5, "Operator Copy encountered "
				"with a mandatory field which is Undefined.");
		else
			SpecExceptionDynamic::ThrowException(6, "Operator Copy encountered "
				"with a mandatory field which is Empty.");
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Dispatch field operator = Delta...
	//	----------------------------------------------------------------------
	inline void GetDeltaValue(Type_SInt64 &delta_value)
	{
		TransferDecode_SInt64(src_begin_ptr_, src_end_ptr_, delta_value);
	}
	inline bool GetDeltaValue_Null(Type_SInt64 &delta_value)
	{
		bool is_null;

		TransferDecode_SInt64_Null(src_begin_ptr_, src_end_ptr_, delta_value,
			is_null);

		return(is_null);
	}
	//	----------------------------------------------------------------------
	template <DataType DType> inline void FieldDecode_Del_O_Int(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType_Null<DType>(item_index, sequence_number);

		Type_SInt64 delta_value;

		//	Returns true if NULL.
		if (!GetDeltaValue_Null(delta_value)) {
			last_entry_ptr_->is_null_ = false;
			FieldDecode_Del_Int_Finalize<DType>(delta_value);
		}

		VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
	}
	template <DataType DType> inline void FieldDecode_Del_X_Int(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);

		Type_SInt64 delta_value;

		GetDeltaValue(delta_value);

		FieldDecode_Del_Int_Finalize<DType>(delta_value);

		VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
	}
	template <DataType DType> inline void FieldDecode_Del_Int_Finalize(
		Type_SInt64 delta_value)
	{
		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsEmpty())
			SpecExceptionDynamic::ThrowException(6, "Operator Delta encountered "
				"with a dictionary value which is Empty.");

		typename TypeEnumToType<DType>::value_type base_value =
			*dict_value_ptr->GetDatumPtr<typename TypeEnumToType<DType>::value_type>();

		typename TypeEnumToType<DType>::value_type new_value =
			static_cast<typename TypeEnumToType<DType>::value_type>(base_value +
			delta_value);

		//	Detect wrap-around caused by excessive delta values...
		if (((delta_value > 0) && (new_value < base_value)) ||
			 ((delta_value < 0) && (new_value > base_value)))
			SpecExceptionReportable::ThrowException(4,
				"Operator Delta encountered with a base value of " +
				MLB::Utility::AnyToString(base_value) + " and a delta value of " +
				MLB::Utility::AnyToString(delta_value) + " which is too large to "
				"fit into the destination field type of " +
				DataTypeToString(DType) + ".");

		*reinterpret_cast<typename TypeEnumToType<DType>::value_type *>(
			last_entry_ptr_->datum_ptr_) = new_value;

		*dict_value_ptr->GetDatumPtr<typename TypeEnumToType<DType>::value_type>() =
			new_value;

		dict_value_ptr->SetIsAssigned();
	}
	//	----------------------------------------------------------------------
	//	Default to ByteVector and Unicode...
	template <DataType DType> inline void GetDeltaString(
		Type_UInt32 &datum_length, char *datum_ptr)
	{
		TransferDecode_ByteVector(src_begin_ptr_, src_end_ptr_,
			VFastMaxStringLength, datum_length, datum_ptr);
	}
	//	----------------------------------------------------------------------
	template <DataType DType> inline void FieldDecode_Del_O_Str(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType_Null<DType>(item_index, sequence_number);

		Type_SInt64 delta_value;

		//	Returns true if NULL.
		if (!GetDeltaValue_Null(delta_value)) {
			last_entry_ptr_->is_null_ = false;
			FieldDecode_Del_Str_Finalize<DType>(delta_value);
		}

		VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
	}
	template <DataType DType> inline void FieldDecode_Del_X_Str(
		unsigned int item_index, unsigned int sequence_number)
	{
		AddEntryForType<DType>(item_index, sequence_number);

		Type_SInt64 delta_value;

		GetDeltaValue(delta_value);

		FieldDecode_Del_Str_Finalize<DType>(delta_value);

		VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
	}
	//	----------------------------------------------------------------------
	template <DataType DType> inline void FieldDecode_Del_Str_Finalize(
		Type_SInt64 delta_value)
	{
		if ((delta_value < std::numeric_limits<Type_SInt32>::min()) ||
			 (delta_value > std::numeric_limits<Type_SInt32>::max())) {
			std::ostringstream o_str;
			o_str << "Attempt to perform a Delta operation on a " <<
				DType << " field instruction where the subtraction value (" <<
				delta_value << ") which is outside of the range of type " <<
				DataType_SInt32 << " (" <<
				std::numeric_limits<Type_SInt32>::min() << " to " <<
				std::numeric_limits<Type_SInt32>::max() << ", inclusive).";
			SpecExceptionDynamic::ThrowException(7, o_str);
		}

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsEmpty())
			SpecExceptionDynamic::ThrowException(6, "Operator Delta encountered "
				"with a dictionary value which is Empty.");

		//	Get the base string...
		unsigned int base_length = dict_value_ptr->string_length_;
		Type_String  base_string[VFastMaxStringLength + 1];
		::memcpy(base_string, dict_value_ptr->string_, base_length + 1);

		//	Get the delta string...
		Type_UInt32 delta_length;
		Type_String delta_string[VFastMaxStringLength + 1];
		GetDeltaString<DType>(delta_length, delta_string);

		Type_SInt64 delta_value_norm = delta_value +
			((delta_value < 0) ? 1 : 0);
		Type_SInt64 delta_value_abs  = (delta_value_norm < 0) ?
			(0 - delta_value_norm) : delta_value_norm;

		if (delta_value_abs > base_length) {
			std::ostringstream o_str;
			o_str << "Attempt to perform a Delta operation on a " <<
				DType << " instruction field where the subtraction value (" <<
				delta_value << ") for which the normalized absolute "
				"value (" << delta_value_abs << ") exceeds the length of the base "
				"string (" << base_length << ").";
			SpecExceptionDynamic::ThrowException(7, o_str);
		}

		unsigned int new_length = static_cast<unsigned int>(delta_length +
			(static_cast<Type_SInt64>(base_length) - delta_value_abs));
		if (new_length > static_cast<Type_SInt64>(VFastMaxStringLength)) {
			std::ostringstream o_str;
			o_str << "The size of the string to be copied into a "
				"ValueDatum string as a result of a delta operation (" <<
				static_cast<unsigned int>(new_length) << ") exceeds the "
				"maximum permissible (" << VFastMaxStringLength << ").";
			MLB::Utility::ThrowLogicError(o_str.str());
		}

		if (delta_value < -1) {
			::memcpy(last_entry_ptr_->datum_ptr_, delta_string, delta_length);
			::memcpy(last_entry_ptr_->datum_ptr_ + delta_length,
				base_string + delta_value_abs,
				(base_length - static_cast<unsigned int>(delta_value_abs)) + 1);
		}
		else if (delta_value > 0) {	//	Positive delta length...
			::memcpy(last_entry_ptr_->datum_ptr_, base_string, base_length);
			::memcpy(last_entry_ptr_->datum_ptr_ +
				(base_length - delta_value_abs), delta_string, delta_length + 1);
		}
		else {
			::memcpy(last_entry_ptr_->datum_ptr_, base_string, base_length);
			::memcpy(last_entry_ptr_->datum_ptr_ + base_length, delta_string,
				delta_length + 1);
		}

		last_entry_ptr_->is_null_      = false;
		last_entry_ptr_->datum_length_ = new_length;
		dict_value_ptr->string_length_ = new_length;

		::memcpy(dict_value_ptr->string_, last_entry_ptr_->datum_ptr_,
			last_entry_ptr_->datum_length_ + 1);
		
		dict_value_ptr->SetIsAssigned();
		dict_value_ptr->ClearNull();
		dict_value_ptr->ClearAbsent();
	}
	// //////////////////////////////////////////////////////////////////////

	// //////////////////////////////////////////////////////////////////////
	//	Dispatch field operator = Tail...
	template <DataType DType> inline void FieldDecode_Tai_OI(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			AddEntryForType<DType>(item_index, sequence_number);
			DecodeForType_Null<DType>();
			if (!last_entry_ptr_->is_null_)
				FieldDecode_Tai_Finalize_Present(item_index, sequence_number);
			return;
		}

		AddEntryForType<DType>(item_index, sequence_number);

		VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned())
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		else if (dict_value_ptr->IsUndefined()) {
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			dict_value_ptr->SetIsAssigned();
		}
		else
			last_entry_ptr_->is_null_ = true;
	}
	template <DataType DType> inline void FieldDecode_Tai_XI(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			AddEntryForType<DType>(item_index, sequence_number);
			DecodeForType<DType>();
			FieldDecode_Tai_Finalize_Present(item_index, sequence_number);
			return;
		}

		AddEntryForType<DType>(item_index, sequence_number);

		VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned())
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		else if (dict_value_ptr->IsUndefined()) {
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
			dict_value_ptr->SetIsAssigned();
		}
		else {
			std::ostringstream o_str;
			o_str << "Attempt to perform a Tail operation on a " <<
				DType << " instruction field where the dictionary value is Empty "
				"and the field is not optional.";
			SpecExceptionDynamic::ThrowException(7, o_str);
		}
	}
	template <DataType DType> inline void FieldDecode_Tai_OX(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			AddEntryForType<DType>(item_index, sequence_number);
			DecodeForType_Null<DType>();
			if (!last_entry_ptr_->is_null_)
				FieldDecode_Tai_Finalize_Present(item_index, sequence_number);
			return;
		}

		AddEntryForType<DType>(item_index, sequence_number);

		VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned())
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		else if (dict_value_ptr->IsUndefined()) {
			last_entry_ptr_->is_null_ = true;
			dict_value_ptr->SetIsEmpty();
		}
		else
			last_entry_ptr_->is_null_ = true;
	}
	template <DataType DType> inline void FieldDecode_Tai_XX(
		unsigned int item_index, unsigned int sequence_number)
	{
		if (GetPMapBit()) {
			AddEntryForType<DType>(item_index, sequence_number);
			DecodeForType<DType>();
			FieldDecode_Tai_Finalize_Present(item_index, sequence_number);
			return;
		}

		AddEntryForType<DType>(item_index, sequence_number);

		VFAST_DEBUG_EXC_ENTRY_PMAP_CLR();

		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		if (dict_value_ptr->IsAssigned())
			SetEntryFromDictValueForType<DType>(dict_value_ptr);
		else if (dict_value_ptr->IsUndefined())
			SpecExceptionDynamic::ThrowException(6, "Operator Tail encountered "
				"with a mandatory field and an Unassigned dictionary entry which "
				"has no initial value.");
		else {
			std::ostringstream o_str;
			o_str << "Attempt to perform a Tail operation on a " <<
				DType << " instruction field where the dictionary value is Empty "
				"and the field is not optional.";
			SpecExceptionDynamic::ThrowException(7, o_str);
		}
	}
	//	----------------------------------------------------------------------
	inline void FieldDecode_Tai_Finalize_Present(unsigned int /* item_index */,
		unsigned int /* sequence_number */)
	{
		DictValue *dict_value_ptr = GetDictValuePtr(last_entry_ptr_->item_index_);

		//	Get the base string...
		unsigned int base_length = dict_value_ptr->string_length_;
		Type_String  base_string[VFastMaxStringLength + 1];
		::memcpy(base_string, dict_value_ptr->string_, base_length + 1);

		if (last_entry_ptr_->datum_length_ < base_length) {
			unsigned int diff_count = base_length -
				last_entry_ptr_->datum_length_;
			::memcpy(base_string + diff_count,
				last_entry_ptr_->datum_ptr_,
				last_entry_ptr_->datum_length_);
			::memcpy(last_entry_ptr_->datum_ptr_, base_string,
				base_length + 1);
			last_entry_ptr_->datum_length_ = base_length;
			SetDictValueForType_String(DictValue::DictValueState_Assigned);
		}
		else if (last_entry_ptr_->datum_length_ > base_length) {
			unsigned int new_length = last_entry_ptr_->datum_length_ +
				base_length;
			if (new_length >= sizeof(base_string)) {
				std::ostringstream o_str;
				o_str << "Tail operation specified a base value length (" <<
					base_length << " bytes) plus a stream value "
					"length (" << last_entry_ptr_->datum_length_ <<
					" bytes) which exceeds the maximum permissible length (" <<
					(sizeof(base_string) - 1) << " bytes).";
				MLB::Utility::ThrowLogicError(o_str.str());
			}
			::memcpy(base_string + base_length,
				last_entry_ptr_->datum_ptr_,
				last_entry_ptr_->datum_length_);
			base_string[new_length] = '\0';
			::memcpy(last_entry_ptr_->datum_ptr_, base_string,
				new_length + 1);
			last_entry_ptr_->datum_length_ = new_length;
			SetDictValueForType_String(DictValue::DictValueState_Assigned);
		}
		else
			SetDictValueForType_String(DictValue::DictValueState_Assigned);
	}
	// //////////////////////////////////////////////////////////////////////

private:
	ExcContext(const ExcContext &other);
	ExcContext & operator = (const ExcContext &other);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void ExcContext::AddEntryForType<DataType_Ascii>(
	unsigned int item_index, unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, 1, VFastMaxStringLength,
		true, false);

	last_entry_ptr_->datum_length_ = 0;
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::AddEntryForType<DataType_Unicode>(
	unsigned int item_index, unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, 1, VFastMaxStringLength,
		true, false);

	last_entry_ptr_->datum_length_ = 0;
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::AddEntryForType<DataType_ByteVector>(
	unsigned int item_index, unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, 1, VFastMaxStringLength,
		true, false);

	last_entry_ptr_->datum_length_ = 0;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void ExcContext::AddEntryForType_Null<DataType_Ascii>(
	unsigned int item_index, unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, 1, VFastMaxStringLength,
		true, true);

	last_entry_ptr_->datum_length_ = 0;
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::AddEntryForType_Null<DataType_Unicode>(
	unsigned int item_index, unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, 1, VFastMaxStringLength,
		true, true);

	last_entry_ptr_->datum_length_ = 0;
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::AddEntryForType_Null<DataType_ByteVector>(
	unsigned int item_index, unsigned int sequence_number)
{
	AddEntry(item_index, sequence_number, 1, VFastMaxStringLength,
		true, true);

	last_entry_ptr_->datum_length_ = 0;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void ExcContext::AddEntryDictValueForType<DataType_Ascii>(
	unsigned int item_index, unsigned int sequence_number)
{
	const DictValue *dict_value_ptr = GetDictValuePtr(item_index);

	AddEntry(item_index, sequence_number,
		1, dict_value_ptr->string_length_, true, dict_value_ptr->IsNull());

	::memcpy(last_entry_ptr_->datum_ptr_, dict_value_ptr->string_,
		dict_value_ptr->string_length_ + 1);
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::AddEntryDictValueForType<DataType_Unicode>(
	unsigned int item_index, unsigned int sequence_number)
{
	const DictValue *dict_value_ptr = GetDictValuePtr(item_index);

	AddEntry(item_index, sequence_number,
		1, dict_value_ptr->string_length_, true, dict_value_ptr->IsNull());

	::memcpy(last_entry_ptr_->datum_ptr_, dict_value_ptr->string_,
		dict_value_ptr->string_length_ + 1);
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::AddEntryDictValueForType<DataType_ByteVector>(
	unsigned int item_index, unsigned int sequence_number)
{
	const DictValue *dict_value_ptr = GetDictValuePtr(item_index);

	AddEntry(item_index, sequence_number,
		1, dict_value_ptr->string_length_, true, dict_value_ptr->IsNull());

	::memcpy(last_entry_ptr_->datum_ptr_, dict_value_ptr->string_,
		dict_value_ptr->string_length_ + 1);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void ExcContext::SetEntryFromDictValueForType<DataType_Ascii>(
	const DictValue *dict_value_ptr)
{
	last_entry_ptr_->datum_length_ = dict_value_ptr->string_length_;

	::memcpy(last_entry_ptr_->datum_ptr_, dict_value_ptr->string_,
		dict_value_ptr->string_length_ + 1);
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::SetEntryFromDictValueForType<DataType_Unicode>(
	const DictValue *dict_value_ptr)
{
	SetEntryFromDictValueForType<DataType_Ascii>(dict_value_ptr);
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::SetEntryFromDictValueForType
	<DataType_ByteVector>(const DictValue *dict_value_ptr)
{
	SetEntryFromDictValueForType<DataType_Ascii>(dict_value_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void ExcContext::SetDictValueForType<DataType_Ascii>(
	DictValue::DictValueState data_state)
{
	SetDictValueForType_String(data_state);
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::SetDictValueForType<DataType_Unicode>(
	DictValue::DictValueState data_state)
{
	SetDictValueForType_String(data_state);
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::SetDictValueForType<DataType_ByteVector>(
	DictValue::DictValueState data_state)
{
	SetDictValueForType_String(data_state);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void ExcContext::DecodeForType<DataType_Ascii>()
{
	XTransferDecode_Ascii(src_begin_ptr_, src_end_ptr_,
		VFastMaxStringLength, last_entry_ptr_->datum_length_,
		last_entry_ptr_->datum_ptr_);

	VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::DecodeForType<DataType_ByteVector>()
{
	XTransferDecode_ByteVector(src_begin_ptr_, src_end_ptr_,
		VFastMaxStringLength, last_entry_ptr_->datum_length_,
		last_entry_ptr_->datum_ptr_);

	VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::DecodeForType<DataType_Unicode>()
{
	XTransferDecode_ByteVector(src_begin_ptr_, src_end_ptr_,
		VFastMaxStringLength, last_entry_ptr_->datum_length_,
		last_entry_ptr_->datum_ptr_);

	VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void ExcContext::DecodeForType_Null<DataType_Ascii>()
{
	XTransferDecode_Ascii_Null(src_begin_ptr_, src_end_ptr_,
		VFastMaxStringLength, last_entry_ptr_->datum_length_,
		last_entry_ptr_->datum_ptr_, last_entry_ptr_->is_null_);

	VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::DecodeForType_Null<DataType_ByteVector>()
{
	XTransferDecode_ByteVector_Null(src_begin_ptr_, src_end_ptr_,
		VFastMaxStringLength, last_entry_ptr_->datum_length_,
		last_entry_ptr_->datum_ptr_, last_entry_ptr_->is_null_);

	VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
}
//	----------------------------------------------------------------------------
template <> inline void ExcContext::DecodeForType_Null<DataType_Unicode>()
{
	XTransferDecode_ByteVector_Null(src_begin_ptr_, src_end_ptr_,
		VFastMaxStringLength, last_entry_ptr_->datum_length_,
		last_entry_ptr_->datum_ptr_, last_entry_ptr_->is_null_);

	VFAST_DEBUG_EXC_ENTRY_END_PTR_SET();
}
// ////////////////////////////////////////////////////////////////////////////
//	----------------------------------------------------------------------------

// ////////////////////////////////////////////////////////////////////////////
template <> inline void ExcContext::GetDeltaString<DataType_Ascii>(
	Type_UInt32 &datum_length, char *datum_ptr)
{
	TransferDecode_Ascii(src_begin_ptr_, src_end_ptr_,
		VFastMaxStringLength, datum_length, datum_ptr);
}
// ////////////////////////////////////////////////////////////////////////////
//
} // namespace VFast

} // namespace MLB

#endif // #ifndef HH__MLB__VFast__ExcContext_hpp__HH

