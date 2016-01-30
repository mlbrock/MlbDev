// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ExcEntry class.

	Revision History	:	2008-11-23 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/ExcEntry.hpp>
#include <VFast/VFastUtility.hpp>
#include <VFast/VFastException.hpp>

#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
std::string ExcEntry::ToString(const InsItemList &item_list) const
{
	return(ToString(&(item_list[0])));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ExcEntry::ToString(const InsItem *item_list) const
{
	std::ostringstream o_str;

	o_str
		<< std::setw(5) << sequence_number_ << " "
		<< ((item_list[item_index_].IsInSequence())    ? "S" : " ")
		<< ((item_list[item_index_].IsInGroup())       ? "G" : " ")
		<< " "
		<< std::setw(5) << item_list[item_index_].auxiliary_id_ <<
			std::left << " "
		<< std::setw(31) << std::left << item_list[item_index_].field_name_ <<
			" " << std::setw(10) <<
			DataTypeToString(item_list[item_index_].data_type_) <<
			std::right << " = [";

	if (item_list[item_index_].IsTypePrimitive()) {
		if (is_null_)
			o_str << "] <*NULL*>";
		else {
			if (item_list[item_index_].IsTypeCharArray())
				o_str << datum_ptr_ << "]";
			else
				o_str << DatumToString(item_list[item_index_].data_type_,
					datum_length_, datum_ptr_) << "]";
		}
	}
	else
		o_str << "] <*N/A*>";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ExcEntry::ToStringDebug(const InsItemList &item_list) const
{
	return(ToStringDebug(&(item_list[0])));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ExcEntry::ToStringDebug(const InsItem *item_list) const
{
	std::ostringstream o_str;

	o_str
		<< std::setw(5) << sequence_number_ << " "
		<< ((item_list[item_index_].IsInSequence())    ? "S" : " ")
		<< ((item_list[item_index_].IsInGroup())       ? "G" : " ")
		<< " "
		<< std::setw(5) << item_list[item_index_].auxiliary_id_ <<
			std::left << " "
		<< std::setw(31) << std::left << item_list[item_index_].field_name_ <<
			" " << std::setw(10) <<
			DataTypeToString(item_list[item_index_].data_type_) << " "
		<< std::setw(13) <<
			InsItemFlagBitsToString(item_list[item_index_].flags_) << " "
		<< std::setw(10) << item_list[item_index_].field_operator_
#ifdef VFAST_DEBUG_EXC_ENTRY
		<< " "
		<< "[PMap=" << pmap_flag_ << "] "
		<< "[Data=" << ToHexStringDebug(begin_ptr_, end_ptr_) << "]"
#else
		<< " "
		<< "[PMap=?] "
		<< "[Data=?? ?? ?? ?? ?? ?? ?? ?? ?? ??]"
#endif // #ifdef VFAST_DEBUG_EXC_ENTRY
		<< std::right << " = [";

	if (item_list[item_index_].IsTypePrimitive()) {
		if (is_null_)
			o_str << "] <*NULL*>";
		else {
			if (item_list[item_index_].IsTypeCharArray())
				o_str << datum_ptr_ << "]";
			else
				o_str << DatumToString(item_list[item_index_].data_type_,
					datum_length_, datum_ptr_) << "]";
		}
	}
	else
		o_str << "] <*N/A*>";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitFix(std::size_t datum_count, const ExcEntry *datum_list,
	const InsItemList &item_list, bool null_flag, bool fid_flag, char pad_char,
	std::ostream &o_str)
{
	EmitFix(datum_count, datum_list, &(item_list[0]), null_flag, fid_flag,
		pad_char, o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitFix(std::size_t datum_count, const ExcEntry *datum_list,
	const InsItem *item_list, bool null_flag, bool fid_flag, char pad_char,
	std::ostream &o_str)
{
	std::vector<std::string> tmp_list;

	tmp_list.reserve(datum_count);

	for (; datum_count; --datum_count, ++datum_list) {
		const InsItem *item_ptr = item_list + datum_list->item_index_;
		if (item_ptr->IsTypePrimitive() && (!item_ptr->IsLength()) &&
			(!item_ptr->IsDecimalChild())) {
			if ((!datum_list->is_null_) || null_flag) {
				std::ostringstream tmp_o_str;
				if (fid_flag)
					tmp_o_str << item_ptr->auxiliary_id_ << "=";
				else
					tmp_o_str << item_ptr->field_name_ << "=";
				tmp_o_str << ((datum_list->is_null_) ? "" :
					DatumToString(item_ptr->data_type_, datum_list->datum_length_,
						datum_list->datum_ptr_));
				tmp_list.push_back(tmp_o_str.str());
			}
		}
	}

	o_str << MLB::Utility::JoinString(tmp_list, std::string(1, pad_char));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitXml(std::size_t datum_count, const ExcEntry *datum_list,
	const InsItemList &item_list, bool null_flag, bool fid_flag,
	unsigned int pad_count, std::ostream &o_str)
{
	EmitXml(datum_count, datum_list, &(item_list[0]), null_flag, fid_flag,
		pad_count, o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitXml(std::size_t datum_count, const ExcEntry *datum_list,
	const InsItem *item_list, bool null_flag, bool fid_flag,
	unsigned int pad_count, std::ostream &o_str)
{
	if (!datum_count)
		return;

	o_str
		<< "<messages>" << std::endl
		<< std::setw(static_cast<std::streamsize>(pad_count)) << ""
		<< "<" << item_list[datum_list->item_index_].field_name_ << ">" <<
			std::endl;

	EmitXmlInternal(datum_count - 1, datum_list + 1, item_list, null_flag,
		fid_flag, pad_count, o_str, 1);

	o_str
		<< std::setw(static_cast<std::streamsize>(pad_count)) << ""
		<< "</" << item_list[datum_list->item_index_].field_name_ << ">" <<
			std::endl
		<< "</messages>";
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitXmlInternal(std::size_t datum_count,
	const ExcEntry *datum_list, const InsItem *item_list, bool null_flag,
	bool fid_flag, unsigned int pad_count, std::ostream &o_str,
	unsigned int indent_count)
{
	std::size_t count_1;

	for (count_1 = 0; count_1 < datum_count; ++count_1, ++datum_list) {
		const InsItem *item_ptr = item_list + datum_list->item_index_;
		if (item_ptr->data_type_ == DataType_Sequence) {
			const InsItem *seq_item_ptr   = item_ptr;
			unsigned int   sequence_count =
				*datum_list[1].GetDatumPtr<Type_UInt32>();
			count_1    += 2;
			datum_list += 2;
			if (sequence_count) {
				o_str
					<< std::setw(static_cast<std::streamsize>(
						indent_count * pad_count)) << "" << "<"
					<< seq_item_ptr->field_name_ << ">" << std::endl;
				unsigned int count_2;
				for (count_2 = 0; count_2 < sequence_count;
					++count_2, datum_list += (seq_item_ptr->element_count_ - 2)) {
					o_str
						<< std::setw(static_cast<std::streamsize>(
							(indent_count + 1) * pad_count)) << "" << "<"
						<< seq_item_ptr->field_name_ << ">" << std::endl;
					EmitXmlInternal(seq_item_ptr->element_count_ - 2,
						datum_list, item_list, null_flag, fid_flag, pad_count,
						o_str, indent_count + 2);
					o_str
						<< std::setw(static_cast<std::streamsize>(
							(indent_count + 1) * pad_count)) << "" << "</"
						<< seq_item_ptr->field_name_ << ">" << std::endl;
				}
				o_str
					<< std::setw(static_cast<std::streamsize>(
						indent_count * pad_count)) << "" << "</"
					<< seq_item_ptr->field_name_ << ">" << std::endl;
				count_1 += 2 + ((seq_item_ptr->element_count_ - 2) * sequence_count);
			}
		}
		else if (item_ptr->data_type_ == DataType_Group) {
			;
		}
		else if (item_ptr->data_type_ == DataType_TemplateRefStatic) {
			;
		}
		else if (item_ptr->data_type_ == DataType_TemplateRefDynamic) {
			;
		}
		else {
			o_str
				<< std::setw(static_cast<std::streamsize>(
					indent_count * pad_count)) << "" << "<"
				<< item_ptr->field_name_ << ">";
			if (!datum_list->is_null_)
				o_str << DatumToString(item_ptr->data_type_,
					datum_list->datum_length_, datum_list->datum_ptr_);
			o_str
				<< "</" << item_ptr->field_name_ << ">" << std::endl;
			if (item_ptr->data_type_ == DataType_Decimal) {
				count_1    += 2;
				datum_list += 2;
			}
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitList(std::size_t datum_count, const ExcEntry *datum_list,
	const InsItemList &item_list, std::ostream &o_str)
{
	return(EmitList(datum_count, datum_list, &(item_list[0]), o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitList(std::size_t datum_count, const ExcEntry *datum_list,
	const InsItem *item_list, std::ostream &o_str)
{
	std::size_t count_1;

	for (count_1 = 0; count_1 < datum_count; ++count_1, ++datum_list)
		o_str << std::setw(5) << static_cast<unsigned int>(count_1) << " " <<
			datum_list->ToString(item_list) << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitListDebug(std::size_t datum_count,
	const ExcEntry *datum_list, const InsItemList &item_list,
	std::ostream &o_str)
{
	return(EmitListDebug(datum_count, datum_list, &(item_list[0]), o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcEntry::EmitListDebug(std::size_t datum_count,
	const ExcEntry *datum_list, const InsItem *item_list, std::ostream &o_str)
{
	std::size_t count_1;

	for (count_1 = 0; count_1 < datum_count; ++count_1, ++datum_list)
		o_str
			<< datum_list->ToStringDebug(item_list) << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

