// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the InsContext class.

	Revision History	:	2008-11-22 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__InsContext_hpp__HH

#define HH__MLB__VFast__InsContext_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file InsContext.hpp

	\brief	Definition of the InsContext instruction context class.
*/
// ////////////////////////////////////////////////////////////////////////////

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
	Repesents an invalid or unspecified template index within an instruction
	context.
*/
const unsigned int InvalidTemplateIndex =
	std::numeric_limits<unsigned int>::max();
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief This instruction context class contains the result of parsing a FAST
	template for subsequent use in the contruction of an instance of the
	execution context class \c ExcContext .
*/
class InsContext {
	friend class InsContextCompat;
public:
	/**
		Describes the location and extent of \c InsItem elements which
		collectively comprise a FAST template.

		The \e first member is the index of the first \c InsItem element of
		a template within an array of \c InsItem elements.

		The \e second member is the number of \c InsItem elements for a
		template within an array of \c InsItem elements.
	*/
	typedef std::pair<unsigned int, unsigned int>    InsItemMapValueI;

	/**
		Provides a mapping from a template identifier to a corresponding
		\c InsItemMapValueI instance.
	*/
	typedef std::map<unsigned int, InsItemMapValueI> InsItemMapI;

	/**
		Provides a mapping from a template name to a corresponding
		\c InsItemMapValueI instance.
	*/
	typedef std::map<std::string, InsItemMapValueI>  InsItemNameMapI;

	/**
		Constructs a FAST instruction context \c InsContext instance from an
		XML template file.

		\param xml_file_name is the name of the FAST XML template file from
		which the instance is to be constructed.

		\param ins_ctx_info is a reference to an instance of \c InsCtxInfo (or
		class derived from it) which will be used to provide fine-grained
		guidance in the parsing and interpretation of the FAST XML template.
	*/
	explicit InsContext(const std::string &xml_file_name,
		const InsCtxInfo &ins_ctx_info = InsCtxInfo());

	/**
		Constructs a FAST instruction context \c InsContext instance from an
		XML std::string in memory.

		\param disambiguator is a boolean provided to distinguish this
		constructor from the file-oriented constructor. Its value, whether
		\e true or \e false , is not used by the construction logic.

		\param xml_string is the \c std::string which contains the FAST XML
		template from which the instance is to be constructed.

		\param ins_ctx_info is a reference to an instance of \c InsCtxInfo (or
		class derived from it) which will be used to provide fine-grained
		guidance in the parsing and interpretation of the FAST XML template.
	*/
	         InsContext(bool disambiguator, const std::string &xml_string,
		const InsCtxInfo &ins_ctx_info = InsCtxInfo());

	/**
		Constructs a FAST instruction context \c InsContext instance from an
		XML const char * string
		in memory...

		\param disambiguator is a boolean provided to distinguish this
		constructor from the file-oriented constructor. Its value, whether
		\e true or \e false , is not used by the construction logic.

		\param xml_string is a pointer to the ASCII NUL-terminated string which
		contains the FAST XML template from which the instance is to be
		constructed.

		\param ins_ctx_info is a reference to an instance of \c InsCtxInfo (or
		class derived from it) which will be used to provide fine-grained
		guidance in the parsing and interpretation of the FAST XML template.
	*/
	         InsContext(bool disambiguator, const char *xml_string,
		const InsCtxInfo &ins_ctx_info = InsCtxInfo());

	/**
		Swaps the contents of this \c InsContext with those of the specified
		\c InsContext .
	*/
	void swap(InsContext &other);

	/**
		Determines the index of the specified template within the
		\e ins_item_list_ member.

		\param template_id is the template identifier of the desired template.

		\return If a template with a template identifier equal to \e template_id
		is found, returns the index of \c InsItem element which represents that
		template in the list of \c InsItem elements in the \e ins_item_list_
		member. Otherwise, returns \e InvalidTemplateIndex .
	*/
	unsigned int DetermineTemplateIndex(unsigned int template_id) const;

	/**
		Determines the index of the specified template within the
		\e ins_item_list_ member.

		\param template_name is the name of the desired template.

		\return If a template with a template identifier equal to \e template_id
		is found, returns the index of \c InsItem element which represents that
		template in the list of \c InsItem elements in the \e ins_item_list_
		member. Otherwise, returns \e InvalidTemplateIndex .
	*/
	unsigned int DetermineTemplateIndex(const std::string &template_name) const;

	/**
		Determines the index of the specified template within the
		\e ins_item_list_ member.

		\param template_name is the name of the desired template.

		\return If a template with a template identifier equal to \e template_id
		is found, returns the index of \c InsItem element which represents that
		template in the list of \c InsItem elements in the \e ins_item_list_
		member. Otherwise, returns \e InvalidTemplateIndex .
	*/
	unsigned int DetermineTemplateIndex(const char *template_name) const;

	/**
		Determines the index of the specified template within the
		\e ins_item_list_ member.

		\param template_id is the template identifier of the desired template.

		\return If a template with a template identifier equal to \e template_id
		is found, returns the index of \c InsItem element which represents that
		template in the list of \c InsItem elements in the \e ins_item_list_
		member. Otherwise, throws an exception.
	*/
	unsigned int GetTemplateIndex(unsigned int template_id) const;

	/**
		Determines the index of the specified template within the
		\e ins_item_list_ member.

		\param template_name is the name of the desired template.

		\return If a template with a template identifier equal to \e template_id
		is found, returns the index of \c InsItem element which represents that
		template in the list of \c InsItem elements in the \e ins_item_list_
		member. Otherwise, throws an exception.
	*/
	unsigned int GetTemplateIndex(const std::string &template_name) const;

	/**
		Determines the index of the specified template within the
		\e ins_item_list_ member.

		\param template_name is the name of the desired template.

		\return If a template with a template identifier equal to \e template_id
		is found, returns the index of \c InsItem element which represents that
		template in the list of \c InsItem elements in the \e ins_item_list_
		member. Otherwise, throws an exception.
	*/
	unsigned int GetTemplateIndex(const char *template_name) const;

	const InsItem *GetItemPtr(const InsItem &parent_ref,
		unsigned int auxiliary_id) const;
	const InsItem *GetItemPtr(unsigned int template_id,
		unsigned int auxiliary_id) const;
	const InsItem *GetItemPtr(const std::string &template_name,
		unsigned int auxiliary_id) const;
	const InsItem *GetItemPtr(const InsItem &parent_ref,
		const std::string &field_name) const;
	const InsItem *GetItemPtr(unsigned int template_id,
		const std::string &field_name) const;
	const InsItem *GetItemPtr(const std::string &template_name,
		const std::string &field_name) const;

	const InsItem *GetItemPtr(const InsItem &parent_ref,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type, bool is_required = true) const;
	const InsItem *GetItemPtr(unsigned int template_id,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type, bool is_required = true) const;
	const InsItem *GetItemPtr(const std::string &template_name,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type, bool is_required = true) const;

	const InsItem &GetItemRef(const InsItem &parent_ref,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type) const;
	const InsItem &GetItemRef(unsigned int template_id,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type) const;
	const InsItem &GetItemRef(const std::string &template_name,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type) const;

	unsigned int GetItemIndex(const InsItem &parent_ref,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type, bool is_required = true) const;
	unsigned int GetItemIndex(unsigned int template_id,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type, bool is_required = true) const;
	unsigned int GetItemIndex(const std::string &template_name,
		unsigned int auxiliary_id, const std::string &field_name,
		DataType data_type, bool is_required = true) const;

	/**
		Returns the lowest value of any numeric template identifier within the
		instruction context.
	*/
	unsigned int GetMinTemplateId() const;

	/**
		Returns the higeest value of any numeric template identifier within the
		instruction context.
	*/
	unsigned int GetMaxTemplateId() const;

	/**
		Checks the instruction context to determine whether it is internally
		consistent.

		\param error_list will be populated with a description of any
		inconsistencies found.

		\return True if no inconsistencies were found, \e false otherwise.

		\note This function was used in the development of the \c InsContext
		class. Unless you make changes to that implementation of the class,
		there's probably no reason for application programmers to call this
		method.
	*/
	bool CheckContext(std::vector<std::string> &error_list) const;

	/**
		Checks the instruction context to determine whether it is internally
		consistent.

		If any inconsistencies found this function will emit a description of
		each one to \c std::cout .

		\return True if no inconsistencies were found, \e false otherwise.

		\note This function was used in the development of the \c InsContext
		class. Unless you make changes to that implementation of the class,
		there's probably no reason for application programmers to call this
		method.
	*/
	bool CheckContext() const;

	/**
		Dumps basic information on the contained FAST templates to \c std::cout .
	*/
	void DumpTemplateMap();

	/**
		Dumps complete information on the contained FAST templates to
		\c std::cout .
	*/
	void DumpTemplateMapFull();

	/**
		Dumps information on the contained FAST templates to \c std::cout . This
		information includes the names of the \c ExcContext methods to which
		the logic will dispatch at the time of message parse execution.
	*/
	void DumpTemplateMapDispatch() const;

	/**
		Determines the index of the specified template within the
		\e ins_item_list parameter.

		\param template_name_map is a mapping of template names to array indices
		which will be used to determine the index within \e ins_item_list at
		which is located the first \c InsItem element of the desired template.

		\param ins_item_list is the list of \c InsItem structures to be
		searched for the desired template.

		\param template_name is the name of the desired template.

		\return If a template with a template identifier equal to \e template_id
		is found, returns the index of \c InsItem element which represents that
		template in the list of \c InsItem elements in the \e ins_item_list_
		member. Otherwise, returns \e InvalidTemplateIndex .
	*/
	static unsigned int DetermineTemplateIndex(
		const InsItemNameMapI &template_name_map,
		const InsItemList &ins_item_list, const char *template_name);

	/**
		The mapping of numeric template identifiers to \c InsItemMapValue
		instances which describe the location of \c InsItem elements within
		the \e ins_item_list_ member.
	*/
	InsItemMapI     template_map_;
	/**
		The mapping of string template names to \c InsItemMapValue
		instances which describe the location of \c InsItem elements within
		the \e ins_item_list_ member.
	*/
	InsItemNameMapI template_name_map_;
	/**
		The list of \c InsItem elements which collectively describe the
		instruction context.
	*/
	InsItemList     ins_item_list_;
	/**
		Contains the list of initial dictionary values for field elements
		within the \e ins_item_list_ member. Resetting a dictionary for this
		instruction context consists of a simple \c memcpy() with the data in
		this member as a source.
	*/
	DictValueList   dict_value_list_;
	/**
		The lowest value of any numeric template identifier within the
		instruction context.
	*/
	unsigned int    min_template_id_;
	/**
		The highest value of any numeric template identifier within the
		instruction context.
	*/
	unsigned int    max_template_id_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef InsContext::InsItemMapValueI             InsItemMapValue;

typedef InsContext::InsItemMapI                  InsItemMap;
typedef InsItemMap::iterator                     InsItemMapIter;
typedef InsItemMap::const_iterator               InsItemMapIterC;
typedef std::pair<unsigned int, InsItemMapValue> InsItemMapPair;

typedef InsContext::InsItemNameMapI              InsItemNameMap;
typedef InsItemNameMap::iterator                 InsItemNameMapIter;
typedef InsItemNameMap::const_iterator           InsItemNameMapIterC;
typedef std::pair<std::string, InsItemMapValue>  InsItemNameMapPair;
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifndef HH__MLB__VFast__InsContext_hpp__HH

