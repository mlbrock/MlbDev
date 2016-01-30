// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the ExcEntry class.

	Revision History	:	2008-11-22 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__ExcEntry_hpp__HH

#define HH__MLB__VFast__ExcEntry_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/InsItem.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	This class encapsulates the data resulting from the parse of a a single
	field datum from a FAST stream.
*/
struct ExcEntry {
	/**
		Gets the size of the parsed datum.

		\param item_list is a reference to the \c InsItemList used (as a member)
		to construct the \c ExcContext which performed the parsing.

		\return The size of the parsed datum.

		\note \e item_list must be the same as that in the \c InsContext instance
		which was used to construct the \c ExcContext which performed the FAST
		parsing (or an identical copy).
	*/
	std::size_t GetDatumContentSize(const InsItemList &item_list) const
	{
		return(GetDatumContentSize(&(item_list[0])));
	}

	/**
		Gets the size of the parsed datum.

		\param item_list is a pointer to the first \c InsItem in the list of
		instruction items used (as a member) to construct the \c ExcContext
		which performed the parsing.

		\return The size of the parsed datum.

		\note \e item_list must be the same as that in the \c InsContext instance
		which was used to construct the \c ExcContext which performed the FAST
		parsing (or an identical copy).
	*/
	std::size_t GetDatumContentSize(const InsItem *item_list) const
	{
		return((item_list[item_index_].IsTypeNumeric()) ? datum_length_ :
			((item_list[item_index_].IsTypeCharArray()) ? (datum_length_ + 1): 0));
	}

	/**
		Gets the \e datum_ptr_ member cast to a \c const pointer to the
		specified type.

		\param TypeName is the type to which the \e datum_ptr_ member is to
		be cast. This template parameter may be any type visible to the code.

		\return The value of the \e datum_ptr_ member cast to a \c const
		pointer to the template instantiation type \c TypeName .

		\note The implementation ensures that, for the target platform, a parsed
		datum is placed at an address with the appropriate alignment for that
		datum.
	*/
	template <typename TypeName> const TypeName *GetDatumPtr() const
	{
		return(reinterpret_cast<const TypeName *>(datum_ptr_));
	}

	/**
		Gets the \e datum_ptr_ member cast to a pointer the specified type.

		\param TypeName is the type to which the \e datum_ptr_ member is to
		be cast. This template parameter may be any type visible to the code.

		\return The value of the \e datum_ptr_ member cast to a pointer to the
		template instantiation type \c TypeName .

		\note The implementation ensures that, for the target platform, a parsed
		datum is placed at an address with the appropriate alignment for that
		datum.

		\note This non-const version of the method just forwards to the \c const
		version.
	*/
	template <typename TypeName> TypeName *GetDatumPtr()
	{
		return(const_cast<TypeName *>(const_cast<const ExcEntry * const>(this)->
			GetDatumPtr<TypeName>()));
	}

	/**
		Gets the \e datum_ptr_ member cast to a \c const pointer of the
		actual underlying type of the logical VFast type.

		Mappings are as follow:

\code
		VFast Type     Windows Type       Linux Type  Other Type
		-------------- ------------------ ----------- -------------------
		Type_SInt16    signed __int16     int16_t     signed short       
		Type_UInt16    unsigned __int16   u_int16_t   unsigned short     
		Type_SInt32    signed __int32     int32_t     signed int         
		Type_UInt32    unsigned __int32   u_int32_t   unsigned int       
		Type_SInt64    signed __int64     int64_t     signed long long   
		Type_UInt64    unsigned __int64   u_int64_t   unsigned long long 
		Type_Decimal   double             double      double             
		Type_String    char               char        char               
		-------------- ------------------ ----------- -------------------
\endcode

		\param DType is the \c DataType VFast type for which the mapping will
		be performed. For a field described in an instruction context list of
		\c InsItem elements, this value corresponds to the value of the
		\e data_type_ member of \c InsItem .

		\return The value of the \e datum_ptr_ member cast to a \c const
		pointer to the appopriate type from the above table.

		\note The implementation ensures that, for the target platform, a parsed
		datum is placed at an address with the appropriate alignment for that
		datum.
	*/
	template <DataType DType>
		const typename TypeEnumToType<DType>::value_type *GetDTypePtr() const
	{
		return(GetDatumPtr<typename TypeEnumToType<DType>::value_type>());
	}

	/**
		Gets the \e datum_ptr_ member cast to a pointer of the actual underlying
		type of the logical VFast type.

		Mappings are as follow:

\code
		VFast Type     Windows Type       Linux Type  Other Type
		-------------- ------------------ ----------- -------------------
		Type_SInt16    signed __int16     int16_t     signed short       
		Type_UInt16    unsigned __int16   u_int16_t   unsigned short     
		Type_SInt32    signed __int32     int32_t     signed int         
		Type_UInt32    unsigned __int32   u_int32_t   unsigned int       
		Type_SInt64    signed __int64     int64_t     signed long long   
		Type_UInt64    unsigned __int64   u_int64_t   unsigned long long 
		Type_Decimal   double             double      double             
		Type_String    char               char        char               
		-------------- ------------------ ----------- -------------------
\endcode

		\param DType is the \c DataType VFast type for which the mapping will
		be performed. For a field described in an instruction context list of
		\c InsItem elements, this value corresponds to the value of the
		\e data_type_ member of \c InsItem .

		\return The value of the \e datum_ptr_ member cast to a pointer to the
		appopriate type from the above table.

		\note The implementation ensures that, for the target platform, a parsed
		datum is placed at an address with the appropriate alignment for that
		datum.

		\note This non-const version of the method just forwards to the \c const
		version.
	*/
	template <DataType DType>
		typename TypeEnumToType<DType>::value_type *GetDTypePtr()
	{
		return(const_cast<typename TypeEnumToType<DType>::value_type *>(
			const_cast<const ExcEntry * const>(this)->GetDTypePtr<DType>()));
	}

	/**
		Gets the entry and its data as parsed from a FAST message as a string.

		\param item_list is an \c InsItemList which contains the \c InsItem
		elements which were used to parse the FAST message.

		\return A \c std::string representing the entry and its data.
	*/
	std::string ToString(const InsItemList &item_list) const;

	/**
		Gets the entry and its data as parsed from a FAST message as a string.

		\param item_list is a pointer to the first element of the array of
		\c InsItem element which were used to parse the FAST message.

		\return A \c std::string representing the entry and its data.
	*/
	std::string ToString(const InsItem *item_list) const;

	/**
		Gets the entry and its data as parsed from a FAST message as a string.

		A hexadecimal representation of the source FAST stream data for each
		element will be emitted along with descriptive information on the
		instruction context detail for the element if this library was compiled
		with the appropriate options. Otherwise, the stream data will be
		represented by the a string "??" for each missing byte.

		\param item_list is an \c InsItemList which contains the \c InsItem
		elements which were used to parse the FAST message.

		\return A \c std::string representing the entry and its data.
	*/
	std::string ToStringDebug(const InsItemList &item_list) const;

	/**
		Gets the entry and its data as parsed from a FAST message as a string.

		A hexadecimal representation of the source FAST stream data for each
		element will be emitted along with descriptive information on the
		instruction context detail for the element if this library was compiled
		with the appropriate options. Otherwise, the stream data will be
		represented by the a string "??" for each missing byte.

		\param item_list is a pointer to the first element of the array of
		\c InsItem element which were used to parse the FAST message.

		\return A \c std::string representing the entry and its data.
	*/
	std::string ToStringDebug(const InsItem *item_list) const;

	/**
		Emits a list of data resulting from the parse of a FAST message in FIX
		format.

		\param datum_count is the number of elements in the array \e datum_list .

		\param datum_list is the array of elements to be emitted.

		\param item_list is an \c InsItemList which contains the \c InsItem
		elements which were used to parse the FAST message.

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
	static void EmitFix(std::size_t datum_count, const ExcEntry *datum_list,
		const InsItemList &item_list, bool null_flag = false,
		bool fid_flag = false, char pad_char = '\x01',
		std::ostream &o_str = std::cout);

	/**
		Emits a list of data resulting from the parse of a FAST message in FIX
		format.

		\param datum_count is the number of elements in the array \e datum_list .

		\param datum_list is the array of elements to be emitted.

		\param item_list is a pointer to the first element of the array of
		\c InsItem element which were used to parse the FAST message.

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
	static void EmitFix(std::size_t datum_count, const ExcEntry *datum_list,
		const InsItem *item_list, bool null_flag = false, bool fid_flag = false,
		char pad_char = '\x01', std::ostream &o_str = std::cout);

	/**
		Emits a list of data resulting from the parse of a FAST message in XML
		format.

		\param datum_count is the number of elements in the array \e datum_list .

		\param datum_list is the array of elements to be emitted.

		\param item_list is an \c InsItemList which contains the \c InsItem
		elements which were used to parse the FAST message.

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
	static void EmitXml(std::size_t datum_count, const ExcEntry *datum_list,
		const InsItemList &item_list, bool null_flag = false,
		bool fid_flag = false, unsigned int pad_count = 2,
		std::ostream &o_str = std::cout);

	/**
		Emits a list of data resulting from the parse of a FAST message in XML
		format.

		\param datum_count is the number of elements in the array \e datum_list .

		\param datum_list is the array of elements to be emitted.

		\param item_list is a pointer to the first element of the array of
		\c InsItem element which were used to parse the FAST message.

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
	static void EmitXml(std::size_t datum_count, const ExcEntry *datum_list,
		const InsItem *item_list, bool null_flag = false, bool fid_flag = false,
		unsigned int pad_count = 2, std::ostream &o_str = std::cout);

	/**
		Emits a list of data resulting from the parse of a FAST message in
		tabular format.

		\param datum_count is the number of elements in the array \e datum_list .

		\param datum_list is the array of elements to be emitted.

		\param item_list is an \c InsItemList which contains the \c InsItem
		elements which were used to parse the FAST message.

		\param o_str is a reference to a \c std::ostream (or descendant of that
		class) to which this method will emit the resulting output.

		\note This method uses the instance method \e ToString for each
		element emitted.
	*/
	static void EmitList(std::size_t datum_count, const ExcEntry *datum_list,
		const InsItemList &item_list, std::ostream &o_str = std::cout);

	/**
		Emits a list of data resulting from the parse of a FAST message in
		tabular format.

		\param datum_count is the number of elements in the array \e datum_list .

		\param datum_list is the array of elements to be emitted.

		\param item_list is a pointer to the first element of the array of
		\c InsItem element which were used to parse the FAST message.

		\param o_str is a reference to a \c std::ostream (or descendant of that
		class) to which this method will emit the resulting output.

		\note This method uses the instance method \e ToString for each
		element emitted.
	*/
	static void EmitList(std::size_t datum_count, const ExcEntry *datum_list,
		const InsItem *item_list, std::ostream &o_str = std::cout);

	/**
		Emits a list of data resulting from the parse of a FAST message in
		tabular format.

		A hexadecimal representation of the source FAST stream data for each
		element will be emitted along with descriptive information on the
		instruction context detail for the element if this library was compiled
		with the appropriate options. Otherwise, the stream data will be
		represented by the a string "??" for each missing byte.

		\param datum_count is the number of elements in the array \e datum_list .

		\param datum_list is the array of elements to be emitted.

		\param item_list is an \c InsItemList which contains the \c InsItem
		elements which were used to parse the FAST message.

		\param o_str is a reference to a \c std::ostream (or descendant of that
		class) to which this method will emit the resulting output.

		\note This method uses the instance method \e ToStringDebug for each
		element emitted.
	*/
	static void EmitListDebug(std::size_t datum_count,
		const ExcEntry *datum_list, const InsItemList &item_list,
		std::ostream &o_str = std::cout);

	/**
		Emits a list of data resulting from the parse of a FAST message in
		tabular format.

		A hexadecimal representation of the source FAST stream data for each
		element will be emitted along with descriptive information on the
		instruction context detail for the element if this library was compiled
		with the appropriate options. Otherwise, the stream data will be
		represented by the a string "??" for each missing byte.

		\param datum_count is the number of elements in the array \e datum_list .

		\param datum_list is the array of elements to be emitted.

		\param item_list is a pointer to the first element of the array of
		\c InsItem element which were used to parse the FAST message.

		\param o_str is a reference to a \c std::ostream (or descendant of that
		class) to which this method will emit the resulting output.

		\note This method uses the instance method \e ToStringDebug for each
		element emitted.
	*/
	static void EmitListDebug(std::size_t datum_count,
		const ExcEntry *datum_list, const InsItem *item_list,
		std::ostream &o_str = std::cout);

	unsigned int       item_index_;
	unsigned int       datum_offset_;
	unsigned int       datum_length_;
	unsigned int       sequence_number_;
	bool               is_null_;
	char              *datum_ptr_;
	const StreamDatum *begin_ptr_;
	const StreamDatum *end_ptr_;
	unsigned int       pmap_flag_;

private:
	static void EmitXmlInternal(std::size_t datum_count,
		const ExcEntry *datum_list, const InsItem *item_list, bool null_flag,
		bool fid_flag, unsigned int pad_count, std::ostream &o_str,
		unsigned int indent_count);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifndef HH__MLB__VFast__ExcEntry_hpp__HH

