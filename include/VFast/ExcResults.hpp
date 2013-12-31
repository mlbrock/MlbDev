// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the ExcResults class.

	Revision History	:	2008-11-22 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__ExcResults_hpp__HH

#define HH__MLB__VFast__ExcResults_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/ExcEntryLookup.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	Encapuslates the results from the parse of a single FAST message.

	\note For reasons of efficiency, this class does not have a private copy
	of any heap memory members (#item_list_ , #entry_list_ , #lookup_list_ ).
	Instead \e shallow copy semantics are employed. Responsibility for the
	management of deletion of the heap objects remains with the source of the
	data (the instance of the \c ExcContext class which populated a particular
	\c ExcResults instance). Therefore it is important that:
	-# No attempt be made to \e delete or \e free these members.
	-# The lifetime of instances of this class, \c ExcResults , do not exceed
	the lifetime of the \c ExcContext class which populated them.
*/
struct ExcResults {
	/**
		Constructs an empty \c ExcResults instance.
	*/
	ExcResults()
		:template_id_(0)
		,template_index_(0)
		,item_count_(0)
		,item_list_(NULL)
		,entry_count_(0)
		,entry_list_(NULL)
		,lookup_list_(NULL)
	{
	}

	/**
		The numeric template identifier of the template which governed parsing
		of the message resulting in the results.
	*/
	unsigned int    template_id_;

	/**
		The index of the template \c InsItem within the \e item_list_ member
		of the template which governed parsing of the message resulting in the
		results.
	*/
	unsigned int    template_index_;

	/**
		The number of \c InsItem elements in the #item_list_ member array .
	*/
	unsigned int    item_count_;

	/**
		A pointer of the list of \c InsItem elements used to parse the message
		and produce the results.

		\see The caveats concerning this member and the members #entry_list_
		and #lookup_list_ .
	*/
	const InsItem  *item_list_;

	/**
		The number of \c ExcEntry elements in the #entry_list_ member array .
	*/
   unsigned int    entry_count_;

	/**
		A pointer of the list of \c ExcEntry elements in which are stored the
		parsed message results.

		\see The caveats concerning this member and the members #item_list_
		and #lookup_list_ .
	*/
	const ExcEntry *entry_list_;

	/**
		A pointer of the list of \c ExcEntryLookup elements reserved within
		\c ExcContext for use in resolving template fields to the fields parsed
		from the message.

		\note This data is not populated by default by the \c ExcContext class
		as all users of that class will not it. Therefore, if you intend to use
		this member, call #PopulateExcEntryLookupList before first use.

		\see The caveats concerning this member and the members #item_list_
		and #entry_list_ .
	*/
	ExcEntryLookup *lookup_list_;

	/**
		Populates the \e lookup_list_ member by calling
		#ExcEntryLookup::PopulateExcEntryLookupList .

		\note You must call this method before using the #lookup_list_ member.
	*/
	void PopulateExcEntryLookupList();

	/**
		Attempts to locate a specified entry. If found, the datum parsed from
		the FAST stream for the entry is copied to a specified datum.

		\param auxiliary_id, if not zero, specifies the desired \e auxiliary_id_
		member.

		\param field_name, if not \e NULL , specifies the desired \e field_name_
		of the entry as indicated in its instruction context element.

		\param data_type specifies the data type of the entry as indicated in its
		instruction context element. If the element is found by auxiliary id
		and/or field name, but the data type does not match, the method will
		throw an exception.

		\param out_datum is a reference to a datum into which this method will
		copy the specified datum if it is found. If said datum is not found,
		and the \e is_required parameter is \e false , this method will zero
		the contents of the \e out_datum parameter.

		\param is_required, if \e true , specifies that an exception must be
		thrown if a field matching the criteria is not found.

		\return A \c const pointer to the \c ExcEntry element of \e entry_list_
		which matches. If no such element is found, and the \e is_required
		parameter is \e false , this method returns \e NULL .

		\note This method is used for scalar data types. ASCII strings, Unicode
		strings and byte vectors should use the overload which has the added
		\e max_char_array_length parameter.
	*/
	template <typename DatumType>
		const ExcEntry *FindValue(unsigned int auxiliary_id,
			const char *field_name, DataType data_type, DatumType &out_datum,
			bool is_required) const;

	/**
		Attempts to locate a specified entry. If found, the datum parsed from
		the FAST stream for the entry is copied to a specified datum.

		\param auxiliary_id, if not zero, specifies the desired \e auxiliary_id_
		member.

		\param field_name, if not \e NULL , specifies the desired \e field_name_
		of the entry as indicated in its instruction context element.

		\param data_type specifies the data type of the entry as indicated in its
		instruction context element. If the element is found by auxiliary id
		and/or field name, but the data type does not match, the method will
		throw an exception.

		\param out_datum is a reference to a datum into which this method will
		copy the specified datum if it is found. If said datum is not found,
		and the \e is_required parameter is \e false , this method will zero
		the contents of the \e out_datum parameter.

		\param is_required, if \e true , specifies that an exception must be
		thrown if a field matching the criteria is not found.

		\param max_char_array_length indicates that \e datum_length_ member of
		entry must not exceed the indicated value or this method will throw an
		exception. This parameter is used to ensure that ASCII strings, Unicode
		strings and byte vectors do not exceed the lengths allocated for them
		in program storage.

		\return A \c const pointer to the \c ExcEntry element of \e entry_list_
		which matches. If no such element is found, and the \e is_required
		parameter is \e false , this method returns \e NULL .

		\note This method is used for ASCII strings, Unicode strings and byte
		vectors. Scalar data types should use the overload which omits the
		\e max_char_array_length parameter.
	*/
	template <typename DatumType>
		const ExcEntry *FindValue(unsigned int auxiliary_id,
			const char *field_name, DataType data_type, DatumType &out_datum,
			bool is_required, std::size_t max_char_array_length) const;

	/**
		Attempts to find an entry in the list \e entry_list_ .

		\param auxiliary_id specifies the entry for which this method
		is to search.

		\return A \c const pointer to the first \c ExcEntry element of
		\e entry_list_ which has an \e auxiliary_id_ member equal that of
		the parameter \e auxiliary_id . Otherwise, returns \e NULL .

		\note More than one entry may have the specified auxiliary identifier.
		This method will return a pointer only to the first found.
	*/
	const ExcEntry *FindEntry(unsigned int auxiliary_id) const;

	/**
		Attempts to find an entry in the list \e entry_list_ .

		\param field_name specifies the entry for which this method
		is to search.

		\return A \c const pointer to the first \c ExcEntry element of
		\e entry_list_ for which instruction context InsItem for the entry
		has an \e field_name_ member equal that of the parameter \e field_name .
		Otherwise, returns \e NULL .

		\note More than one entry may have the specified field name. This
		method will return a pointer only to the first found.
	*/
	const ExcEntry *FindEntry(const char *field_name) const;

	/**
		Attempts to find an entry in the list \e entry_list_ .

		\param auxiliary_id, if not zero, specifies the desired \e auxiliary_id_
		member.

		\param field_name, if not \e NULL , specifies the desired \e field_name_
		of the entry as indicated in its instruction context element.

		\param data_type specifies the data type of the entry as indicated in its
		instruction context element. If the element is found by auxiliary id
		and/or field name, but the data type does not match, the method will
		throw an exception.

		\param is_required, if \e true , specifies that an exception must be
		thrown if a field matching the criteria is not found.

		\param max_char_array_length indicates that \e datum_length_ member of
		entry must not exceed the indicated value or this method will throw an
		exception. This parameter is used to ensure that ASCII strings, Unicode
		strings and byte vectors do not exceed the lengths allocated for them
		in program storage.

		\return A \c const pointer to the \c ExcEntry element of \e entry_list_
		which matches. If no such element is found, and the \e is_required
		parameter is \e false , this method returns \e NULL .
	*/
	const ExcEntry *FindEntry(unsigned int auxiliary_id,
		const char *field_name, DataType data_type, bool is_required,
		std::size_t max_char_array_length) const;

	template <typename DatumType>
		const ExcEntry *LookupValueFirst(unsigned int item_index,
			DatumType &out_datum, bool is_required) const;
	template <typename DatumType>
		const ExcEntry *LookupValueFirst(unsigned int item_index,
			DatumType &out_datum, bool is_required,
			std::size_t max_char_array_length) const;
	template <typename DatumType>
		const ExcEntry *LookupValueLast(unsigned int item_index,
			DatumType &out_datum, bool is_required) const;
	template <typename DatumType>
		const ExcEntry *LookupValueLast(unsigned int item_index,
			DatumType &out_datum, bool is_required,
			std::size_t max_char_array_length) const;
	template <typename DatumType>
		const ExcEntry *LookupValueAt(unsigned int item_index,
			unsigned int entry_index, DatumType &out_datum,
			bool is_required) const;
	template <typename DatumType>
		const ExcEntry *LookupValueAt(unsigned int item_index,
			unsigned int entry_index, DatumType &out_datum, bool is_required,
			std::size_t max_char_array_length) const;

	const ExcEntry *LookupEntryFirst(unsigned int item_index,
		bool is_required, std::size_t max_char_array_length) const;
	const ExcEntry *LookupEntryLast(unsigned int item_index,
		bool is_required, std::size_t max_char_array_length) const;
	const ExcEntry *LookupEntryAt(unsigned int item_index,
		unsigned int entry_index, bool is_required,
		std::size_t max_char_array_length) const;

	bool MoveFirstToNext(ExcEntryLookup &lookup_item) const;

	bool IsPresent(unsigned int item_index) const;
	bool CheckIsPresent(unsigned int item_index, bool is_required) const;
	void CheckItemIndex(unsigned int item_index) const;
	void CheckEntryIndex(unsigned int entry_index) const;

	/**
		Emits the results data fields resulting from the parse of a FAST message
		in FIX format.

		\param null_flag if \e true , specifies that data fields which were
		null or not present in the FAST message should be emitted.

		\param fid_flag if \e true , specifies that field identifiers (from the
		\e auxiliary_id_ of \c InsItem ) should be used instead of the field
		name (from the \e field_name_ member of \c InsItem ).

		\param pad_char specifies the character to be used to separate individual
		fields.

		\param o_str is a reference to a \c std::ostream (or descendant of that
		class) to which this method will emit the FIX stream.
	*/
	void EmitFix(bool null_flag = false, bool fid_flag = false,
		char pad_char = '\x01', std::ostream &o_str = std::cout) const;

	/**
		Emits the results data fields resulting from the parse of a FAST message
		in XML format.

		\param null_flag if \e true , specifies that data fields which were
		null or not present in the FAST message should be emitted.

		\param fid_flag if \e true , specifies that field identifiers (from the
		\e auxiliary_id_ of \c InsItem ) should be used instead of the field
		name (from the \e field_name_ member of \c InsItem ).

		\param pad_char specifies the character to be used to separate individual
		fields.

		\param o_str is a reference to a \c std::ostream (or descendant of that
		class) to which this method will emit the XML.
	*/
	void EmitXml(bool null_flag = false, bool fid_flag = false,
		unsigned int pad_count = 2, std::ostream &o_str = std::cout) const;

	/**
		Emits the results data fields resulting from the parse of a FAST message
		in tabular format.

		\param o_str is a reference to a \c std::ostream (or descendant of that
		class) to which this method will emit the resulting output.
	*/
	void EmitList(std::ostream &o_str = std::cout) const;

	/**
		Emits the results data fields resulting from the parse of a FAST message
		in tabular format with debugging data.

		A hexadecimal representation of the source FAST stream data for each
		element will be emitted along with descriptive information on the
		instruction context detail for the element if this library was compiled
		with the appropriate options. Otherwise, the stream data will be
		represented by the a string "??" for each missing byte.

		\param o_str is a reference to a \c std::ostream (or descendant of that
		class) to which this method will emit the resulting output.
	*/
	void EmitListDebug(std::ostream &o_str = std::cout) const;

private:
	template <typename DatumType>
		const ExcEntry *CopyToOutDatum(const ExcEntry *entry_ptr,
			DatumType &out_datum) const;
	template <typename DatumType>
		const ExcEntry *CopyToOutDatum(const ExcEntry *entry_ptr,
			DatumType &out_datum, std::size_t max_char_array_length) const;

	const ExcEntry *LookupEntryInternal(unsigned int item_index,
		unsigned int entry_index, bool is_required, int boundary_spec,
		std::size_t max_char_array_length) const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::FindValue(unsigned int auxiliary_id,
		const char *field_name, DataType data_type, DatumType &out_datum,
		bool is_required) const
{
	const ExcEntry *entry_ptr = FindEntry(auxiliary_id, field_name, data_type,
		is_required, 0);

	return(CopyToOutDatum(entry_ptr, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::FindValue(unsigned int auxiliary_id,
		const char *field_name, DataType data_type, DatumType &out_datum,
		bool is_required, std::size_t max_char_array_length) const
{
	const ExcEntry *entry_ptr = FindEntry(auxiliary_id, field_name, data_type,
		is_required, max_char_array_length);

	return(CopyToOutDatum(entry_ptr, out_datum, max_char_array_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::LookupValueFirst(unsigned int item_index,
		DatumType &out_datum, bool is_required) const
{
	const ExcEntry *entry_ptr = LookupEntryFirst(item_index, is_required, 0);

	return(CopyToOutDatum(entry_ptr, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::LookupValueFirst(unsigned int item_index,
		DatumType &out_datum, bool is_required,
		std::size_t max_char_array_length) const
{
	const ExcEntry *entry_ptr = LookupEntryFirst(item_index, is_required,
		max_char_array_length);

	return(CopyToOutDatum(entry_ptr, out_datum, max_char_array_length));
}
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::LookupValueLast(unsigned int item_index,
		DatumType &out_datum, bool is_required) const
{
	const ExcEntry *entry_ptr = LookupEntryLast(item_index, is_required, 0);

	return(CopyToOutDatum(entry_ptr, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::LookupValueLast(unsigned int item_index,
		DatumType &out_datum, bool is_required,
		std::size_t max_char_array_length) const
{
	const ExcEntry *entry_ptr = LookupEntryLast(item_index, is_required,
		max_char_array_length);

	return(CopyToOutDatum(entry_ptr, out_datum, max_char_array_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::LookupValueAt(unsigned int item_index,
		unsigned int entry_index, DatumType &out_datum, bool is_required) const
{
	const ExcEntry *entry_ptr = LookupEntryAt(item_index, entry_index,
		is_required, 0);

	return(CopyToOutDatum(entry_ptr, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::LookupValueAt(unsigned int item_index,
		unsigned int entry_index, DatumType &out_datum, bool is_required,
		std::size_t max_char_array_length) const
{
	const ExcEntry *entry_ptr = LookupEntryAt(item_index, entry_index,
		is_required, max_char_array_length);

	return(CopyToOutDatum(entry_ptr, out_datum, max_char_array_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::CopyToOutDatum(const ExcEntry *entry_ptr,
		DatumType &out_datum) const
{
	if ((entry_ptr != NULL) && (!entry_ptr->is_null_))
		out_datum = *entry_ptr->GetDatumPtr<DatumType>();
	else
		out_datum = static_cast<DatumType>(0);

	return(entry_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	const ExcEntry *ExcResults::CopyToOutDatum(const ExcEntry *entry_ptr,
		DatumType &out_datum, std::size_t max_char_array_length) const
{
	if ((entry_ptr != NULL) && (!entry_ptr->is_null_)) {
		::memcpy(out_datum, entry_ptr->datum_ptr_, entry_ptr->datum_length_);
		out_datum[entry_ptr->datum_length_] = '\0';
	}
	else
		::memset(out_datum, '\0', max_char_array_length + 1);

	return(entry_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifndef HH__MLB__VFast__ExcResults_hpp__HH

