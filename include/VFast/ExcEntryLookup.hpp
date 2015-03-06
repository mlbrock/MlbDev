// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the ExcEntryLookup class.

	Revision History	:	2008-11-22 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__ExcEntryLookup_hpp__HH

#define HH__MLB__VFast__ExcEntryLookup_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/ExcEntry.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	Stores the resolution of a single template field vis-a-vis the results of
	parsing a message as represented by an array of \c ExcEntry elements.
*/
struct ExcEntryLookup {
	/// The number of times the field appears in the \c ExcEntry list.
	unsigned int count_;
	/// Index in the \c ExcEntry list of the first occurrence of the field.
	unsigned int first_;
	/// Index in the \c ExcEntry list of the last occurrence of the field.
	unsigned int last_;

	/// Returns the value of the #count_ member.
	unsigned int GetCount() const
	{
		return(count_);
	}

	/// Sets the value of the #count_ member to \e 0 .
	void ClearCount()
	{
		count_ = 0;
	}

	/**
		Populates an array of \c ExcEntryLookup elements.

		\param entry_count is the number of \c ExcEntry elements in the
		#entry_list parameter array.

		\param entry_list is a pointer of the list of \c ExcEntry elements in
		which are stored the parsed message results.

		\param item_count is the number of \c InsItem elements in the
		\c InsContext used to construct the \c ExcContext which parsed the
		the message. There is one \c ExcEntryLookup element for each
		\c InsItem (though the element may have a #count_ member if \e 0 ).

		\param lookup_list is the array of \c ExcEntryLookup elements to be
		populated by this method.
	*/
	static void PopulateExcEntryLookupList(unsigned int entry_count,
		const ExcEntry *entry_list, unsigned int item_count,
		ExcEntryLookup *lookup_list);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifndef HH__MLB__VFast__ExcEntryLookup_hpp__HH

