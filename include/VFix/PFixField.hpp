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
typedef unsigned int VFixTagNum;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixField {
	typedef boost::multi_index_container<PFixField> PFixFieldMISet;
public:
	typedef std::set<PFixField> PFixFieldSet_I;

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

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	static std::ostream &EmitTabular(const PFixFieldSet_I &in_set,
		std::ostream &o_str = std::cout);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixField &datum);

/*
	CODE NOTE: Pending implementation.
	static const PFixField *FindElement(const PFixFieldSet_I &in_set,
		VFixTagNum tag, bool thow_if_not_found = false);
	static const PFixField *FindElement(const PFixFieldSet_I &in_set,
		const char *name, bool thow_if_not_found = false);
	static const PFixField *FindElement(const PFixFieldSet_I &in_set,
		const std::string &name, bool thow_if_not_found = false);
*/

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixFieldSet_I &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixFieldSet_I &out_set);
	static PFixFieldSet_I &ParseXmlFile(const std::string &file_name,
		PFixFieldSet_I &out_set);
	static PFixFieldSet_I  ParseXmlFile(const std::string &file_name);

	static void AddElement(const PFixField &datum, PFixFieldSet_I &out_set);

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

// ////////////////////////////////////////////////////////////////////////////
typedef PFixField::PFixFieldSet_I         PFixFieldSet;
typedef PFixFieldSet::iterator            PFixFieldSetIter;
typedef PFixFieldSet::const_iterator      PFixFieldSetIterC;
typedef std::pair<PFixFieldSetIter, bool> PFixFieldSetInsertPair;
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
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<PFixFieldByAbbr>,
				BOOST_MULTI_INDEX_MEMBER(PFixField, std::string, abbreviation_)
			>
		>
	> PFixFieldMISet;
//	----------------------------------------------------------------------------
typedef boost::multi_index::index<PFixFieldMISet, 
	PFixFieldByTag>::type                       PFixFieldMISetIdxByTag;
typedef PFixFieldMISetIdxByTag::iterator       PFixFieldMISetIdxByTagIter;
typedef PFixFieldMISetIdxByTag::const_iterator PFixFieldMISetIdxByTagIterC;
//	----------------------------------------------------------------------------
typedef boost::multi_index::index<PFixFieldMISet,
	PFixFieldByName>::type                       PFixFieldMISetIdxByName;
typedef PFixFieldMISetIdxByName::iterator       PFixFieldMISetIdxByNameIter;
typedef PFixFieldMISetIdxByName::const_iterator PFixFieldMISetIdxByNameIterC;
//	----------------------------------------------------------------------------
typedef boost::multi_index::index<PFixFieldMISet,
	PFixFieldByAbbr>::type                       PFixFieldMISetIdxByAbbr;
typedef PFixFieldMISetIdxByAbbr::iterator       PFixFieldMISetIdxByAbbrIter;
typedef PFixFieldMISetIdxByAbbr::const_iterator PFixFieldMISetIdxByAbbrIterC;
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixFieldSet &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixField_hpp__HH

