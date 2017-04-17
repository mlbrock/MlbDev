// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ExcResults class.

	Revision History	:	2008-11-23 --- Creation.
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

#include <VFast/ExcResults.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
void ExcResults::PopulateExcEntryLookupList()
{
	ExcEntryLookup::PopulateExcEntryLookupList(entry_count_, entry_list_,
		item_count_, lookup_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ExcEntry *ExcResults::FindEntry(unsigned int auxiliary_id) const
{
	for (unsigned int count_1 = 0; count_1 < entry_count_; ++count_1) {
		if (auxiliary_id ==
			item_list_[entry_list_[count_1].item_index_].auxiliary_id_)
			return(entry_list_ + count_1);
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ExcEntry *ExcResults::FindEntry(const char *field_name) const
{
	for (unsigned int count_1 = 0; count_1 < entry_count_; ++count_1) {
		if (!::strcmp(field_name,
			item_list_[entry_list_[count_1].item_index_].field_name_.c_str()))
			return(entry_list_ + count_1);
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ExcEntry *ExcResults::FindEntry(unsigned int auxiliary_id,
	const char *field_name, DataType data_type, bool is_required,
	std::size_t max_char_array_length) const
{
	try {
		for (unsigned int count_1 = 0; count_1 < entry_count_; ++count_1) {
			if ((auxiliary_id ==
				item_list_[entry_list_[count_1].item_index_].auxiliary_id_) &&
				((auxiliary_id != 0) || ((field_name != NULL) &&
				(!::strcmp(field_name, item_list_[entry_list_[count_1].
				item_index_].field_name_.c_str()))))) {
				if (item_list_[entry_list_[count_1].item_index_].data_type_ !=
					data_type)
					MLB::Utility::ThrowLogicError("Expected data type " +
						DataTypeToString(data_type) + ", but the actual data type "
						"is " +
						DataTypeToString(item_list_[entry_list_[count_1].
						item_index_].data_type_) + ".");
				else if (max_char_array_length &&
					(entry_list_[count_1].datum_length_ > max_char_array_length))
					MLB::Utility::ThrowLogicError("The length of the received char "
						"array data (" + MLB::Utility::AnyToString(
						entry_list_[count_1].datum_length_) + ") exceeds the "
						"maximum permissible (" +
						MLB::Utility::AnyToString(max_char_array_length) + ").");
				return(entry_list_ + count_1);
			}
		}
		if (is_required)
			MLB::Utility::ThrowLogicError("Unable to locate this required field.");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to locate execution context "
			"result field name '" +
			std::string((field_name == NULL) ? "" : field_name) + "', auxiliary "
			"identifier " + MLB::Utility::AnyToString(auxiliary_id) + " within "
			"the list of results from parsing a message with template "
			"identifier " + MLB::Utility::AnyToString(template_id_) +
			" failed: " + std::string(except.what()));
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ExcEntry *ExcResults::LookupEntryFirst(unsigned int item_index,
	bool is_required, std::size_t max_char_array_length) const
{
	return(LookupEntryInternal(item_index, 0, is_required, -1,
		max_char_array_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ExcEntry *ExcResults::LookupEntryLast(unsigned int item_index,
	bool is_required, std::size_t max_char_array_length) const
{
	return(LookupEntryInternal(item_index, 0, is_required, 1,
		max_char_array_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ExcEntry *ExcResults::LookupEntryAt(unsigned int item_index,
	unsigned int entry_index, bool is_required,
	std::size_t max_char_array_length) const
{
	return(LookupEntryInternal(item_index, entry_index, is_required, 0,
		max_char_array_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ExcResults::MoveFirstToNext(ExcEntryLookup &lookup_item) const
{
	if ((!lookup_item.count_) || (lookup_item.first_ >= lookup_item.last_))
		return(false);

	unsigned int item_index = entry_list_[lookup_item.first_++].item_index_;

	while ((lookup_item.first_ < lookup_item.last_) &&
		(item_index != entry_list_[lookup_item.first_].item_index_))
		++lookup_item.first_;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ExcResults::IsPresent(unsigned int item_index) const
{
	return(lookup_list_[item_index].count_ > 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ExcResults::CheckIsPresent(unsigned int item_index, bool is_required) const
{
	if (lookup_list_[item_index].count_)
		return(true);
	else if (is_required)
		MLB::Utility::ThrowLogicError("Unable to locate a decoded entry for "
			"this required field.");

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcResults::CheckItemIndex(unsigned int item_index) const
{
	if (item_index >= item_count_)
		MLB::Utility::ThrowLogicError("Specified item index is invalid (" +
			MLB::Utility::AnyToString(item_index) + " --- item count is " +
			MLB::Utility::AnyToString(item_count_) + ".");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcResults::CheckEntryIndex(unsigned int entry_index) const
{
	if (entry_index >= entry_count_)
		MLB::Utility::ThrowLogicError("Specified entry index is invalid (" +
			MLB::Utility::AnyToString(entry_index) + " --- entry count is " +
			MLB::Utility::AnyToString(entry_count_) + ".");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcResults::EmitFix(bool null_flag, bool fid_flag, char pad_char,
	std::ostream &o_str) const
{
	ExcEntry::EmitFix(entry_count_, entry_list_, item_list_, null_flag,
		fid_flag, pad_char, o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcResults::EmitXml(bool null_flag, bool fid_flag, unsigned int pad_count,
	std::ostream &o_str) const
{
	ExcEntry::EmitXml(entry_count_, entry_list_, item_list_, null_flag,
		fid_flag, pad_count, o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcResults::EmitList(std::ostream &o_str) const
{
	ExcEntry::EmitList(entry_count_, entry_list_, item_list_, o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ExcResults::EmitListDebug(std::ostream &o_str) const
{
	ExcEntry::EmitListDebug(entry_count_, entry_list_, item_list_, o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const ExcEntry *ExcResults::LookupEntryInternal(unsigned int item_index,
	unsigned int entry_index, bool is_required, int boundary_spec,
	std::size_t max_char_array_length) const
{
	CheckItemIndex(item_index);

	try {
		if (CheckIsPresent(item_index, is_required)) {
			if (!boundary_spec) {
				CheckEntryIndex(entry_index);
				if ((entry_index < lookup_list_[item_index].first_) ||
					(entry_index > lookup_list_[item_index].last_))
					MLB::Utility::ThrowLogicError("The specified entry index (" +
						MLB::Utility::AnyToString(entry_index) + ") does not fall "
						"within the valid bounds of " +
						MLB::Utility::AnyToString(lookup_list_[item_index].first_) +
						" to " +
						MLB::Utility::AnyToString(lookup_list_[item_index].last_) +
						", inclusive.");
			}
			else if (boundary_spec < 0)
				entry_index = lookup_list_[item_index].first_;
			else
				entry_index = lookup_list_[item_index].last_;
			const ExcEntry *entry_ptr = entry_list_ + entry_index;
			if (entry_ptr->item_index_ != item_index)
				MLB::Utility::ThrowLogicError("The specified entry index (" +
					MLB::Utility::AnyToString(entry_index) + ") is for item "
					" index " + MLB::Utility::AnyToString(entry_ptr->item_index_) +
					" (" + item_list_[entry_ptr->item_index_].GetDescriptiveText() +
					") and not for the item indicated by item index " +
					MLB::Utility::AnyToString(item_index) + " (" +
					item_list_[item_index].GetDescriptiveText() + ").");
			if (max_char_array_length &&
				(entry_ptr->datum_length_ > max_char_array_length))
				MLB::Utility::ThrowLogicError("The length of the received "
					"character array data (" + MLB::Utility::AnyToString(
					entry_ptr->datum_length_) +
					") exceeds the maximum permissible (" +
				MLB::Utility::AnyToString(max_char_array_length) + ").");
			return(entry_ptr);
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to locate execution context "
			"result " + item_list_[item_index].GetDescriptiveText() +
			" within the list of results from parsing a message with template "
			"identifier " + MLB::Utility::AnyToString(template_id_) +
			" failed: " + std::string(except.what()));
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

