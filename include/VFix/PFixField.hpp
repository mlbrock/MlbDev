// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixField.hpp

	File Description	:	Definition of the PFixField class.

	Revision History	:	2015-07-03 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFix__PFixField_hpp__HH

#define HH__MLB__VFix__PFixField_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PFixField.hpp

	\brief	The VFix header file containing the definition of the PFixField
	class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/PFixFieldType.hpp>

#ifdef _Windows
# pragma warning(push)
//# pragma warning(disable:4217 4510 4610 4623 4625 4626 4668)
# pragma warning(disable:4625 4626)
#endif // #ifdef _Windows

#include <boost/multi_index_container_fwd.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/key_extractors.hpp>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
class PFixFieldSet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef unsigned int VFixTagNum;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixField {
	typedef boost::multi_index_container<PFixField> PFixFieldMISet;
public:

	//	Constructor used for searches in std::set<>s...
	explicit PFixField(VFixTagNum tag = 0);

	//	Constructor used for searches in boost::multi_index<>s...
	PFixField(VFixTagNum tag, const std::string &name,
		const std::string &type_name);

	explicit PFixField(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixField &other) const
	{
		return(tag_ < other.tag_);
	}

	void swap(PFixField &other);

	std::string GetIdString() const;

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	static std::ostream &EmitTabularByTag(const PFixFieldSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabularByName(const PFixFieldSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabularByAbbr(const PFixFieldSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabular(const PFixFieldSet &in_set,
		std::ostream &o_str = std::cout);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixField &datum);

	static const PFixField *FindElementByTag(const PFixFieldSet &in_set,
		VFixTagNum key, bool throw_if_not_found = false);
	static const PFixField *FindElementByName(const PFixFieldSet &in_set,
		const std::string &key, bool throw_if_not_found = false);
	static const PFixField *FindElementByAbbr(const PFixFieldSet &in_set,
		const std::string &key, bool throw_if_not_found = false);

	/*
		I've observed that field abbreviations in the FIX repository XML
		file Fields.xml are not unique to a field (as the message abbreviations
		in Messages.xml are).

		This method therefore searches first by name. If the desired key is
		not found by name, it then searches by abbreviation, but only returns a
		found abbreviation if it is unique. If no abbreviation is found, or
		if a found abbreviation was not unique, the method will, if the
		provided key is wholly numeric, search for the tag number.
	*/
	static const PFixField *FindElement(const PFixFieldSet &in_set,
		const std::string &key, bool thow_if_not_found = false);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixFieldSet &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixFieldSet &out_set);
	static PFixFieldSet &ParseXmlFile(const std::string &file_name,
		PFixFieldSet &out_set);
	static PFixFieldSet  ParseXmlFile(const std::string &file_name);

	static void AddElement(const PFixField &datum, PFixFieldSet &out_set);

	VFixTagNum    tag_;
	VFixTagNum    ref_to_tag_;
	VFixTagNum    ref_from_tag_;
	std::string   name_;
	std::string   type_name_;
	std::string   abbreviation_;
	std::string   fix_version_;
	std::string   description_;
	VFixXPortType vfix_xport_type_;

private:
	PFixField(VFixTagNum tag, VFixTagNum ref_to_tag,
		VFixTagNum ref_from_tag, const std::string &name,
		const std::string &type_name_, const std::string &abbreviation,
		const std::string &fix_version, const std::string &description,
		VFixXPortType vfix_xport_type);
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct PFixFieldByTag  { };
struct PFixFieldByName { };
struct PFixFieldByAbbr { };
//	----------------------------------------------------------------------------
typedef boost::multi_index_container<
	PFixField,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<PFixFieldByTag>,
			BOOST_MULTI_INDEX_MEMBER(PFixField, VFixTagNum, tag_)
		>
		,
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<PFixFieldByName>,
			BOOST_MULTI_INDEX_MEMBER(PFixField, std::string, name_)
		>
		,
		boost::multi_index::ordered_non_unique<
			boost::multi_index::tag<PFixFieldByAbbr>,
			BOOST_MULTI_INDEX_MEMBER(PFixField, std::string, abbreviation_)
		>
	>
> PFixFieldMISet;
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixFieldSet {
public:
	PFixFieldSet()
	{
	}

	PFixFieldMISet &Get()
	{
		return(the_set_);
	}

	const PFixFieldMISet &Get() const
	{
		return(the_set_);
	}

	void swap(PFixFieldSet &other)
	{
		the_set_.swap(other.the_set_);
	}

	std::size_t size() const
	{
		return(the_set_.size());
	}

private:
	PFixFieldMISet the_set_;
};
// //////////////////////////////////////////////////////////////////////////// 

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixFieldSet &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixField_hpp__HH

