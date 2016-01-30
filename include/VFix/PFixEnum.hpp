// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixEnum.hpp

	File Description	:	Definition of the PFixEnum class.

	Revision History	:	2015-08-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFix__PFixEnum_hpp__HH

#define HH__MLB__VFix__PFixEnum_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PFixEnum.hpp

	\brief	The VFix header file containing the definition of the PFixEnum
	class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/VFix.hpp>
#include <VFix/VFixMISetHolder.hpp>

#include <RapidXmlUtils/RapidXmlContext.hpp>
#include <RapidXmlUtils/XmlDomElement.hpp>

#include <set>

#include <set>

#ifdef _Windows
# pragma warning(push)
//# pragma warning(disable:4217 4510 4610 4623 4625 4626 4668)
# pragma warning(disable:4625 4626)
#endif // #ifdef _Windows

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
class PFixEnumSet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixEnum {
public:

	PFixEnum();

	explicit PFixEnum(PFixTagNum tag, const std::string &value = "");

	explicit PFixEnum(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixEnum &other) const
	{
		return((tag_ < other.tag_) ? true : ((tag_ != other.tag_) ? false :
			(value_.compare(other.value_) < 0)));
	}

	void swap(PFixEnum &other);

	std::string GetIdString() const;

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	static std::ostream &EmitTabularByTagValue(const PFixEnumSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabularByTagOrder(const PFixEnumSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabular(const PFixEnumSet &in_set,
		std::ostream &o_str = std::cout);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixEnum &datum);

	static const PFixEnum *FindElementByTagValue(const PFixEnumSet &in_set,
		PFixTagNum tag_key, const std::string &value_key,
		bool throw_if_not_found = false);
	static const PFixEnum *FindElementByTagOrder(const PFixEnumSet &in_set,
		PFixTagNum tag_key, PFixEnumSortOrder order_key,
		bool throw_if_not_found = false);

	static const PFixEnum *FindElement(const PFixEnumSet &in_set,
		PFixTagNum tag_key, const std::string &other_key,
		bool thow_if_not_found = false);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixEnumSet &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixEnumSet &out_set);
	static PFixEnumSet &ParseXmlFile(const std::string &file_name,
		PFixEnumSet &out_set);
	static PFixEnumSet  ParseXmlFile(const std::string &file_name);

	static void AddElement(const PFixEnum &datum, PFixEnumSet &out_set);

	PFixTagNum        tag_;
	std::string       value_;
	std::string       symbolic_name_;
	PFixEnumSortOrder sort_order_;
	std::string       fix_version_;
	std::string       description_;

private:
	PFixEnum(PFixTagNum tag, const std::string &value,
		const std::string &symbolic_name, PFixEnumSortOrder sort_order,
		const std::string &fix_version, const std::string &description);
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct PFixEnumByTagValue { };
struct PFixEnumByTagOrder { };
//	----------------------------------------------------------------------------
typedef boost::multi_index_container<
	PFixEnum,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<PFixEnumByTagValue>,
			boost::multi_index::composite_key<
				PFixEnum,
				BOOST_MULTI_INDEX_MEMBER(PFixEnum, PFixTagNum,  tag_),
				BOOST_MULTI_INDEX_MEMBER(PFixEnum, std::string, value_)
			>
		>
		,
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<PFixEnumByTagOrder>,
			boost::multi_index::composite_key<
				PFixEnum,
				BOOST_MULTI_INDEX_MEMBER(PFixEnum, PFixTagNum,        tag_),
				BOOST_MULTI_INDEX_MEMBER(PFixEnum, PFixEnumSortOrder, sort_order_)
			>
		>
	>
> PFixEnumMISet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixEnumSet : public VFixMISetHolder<PFixEnumMISet> { };
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixEnumSet &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixEnum_hpp__HH

