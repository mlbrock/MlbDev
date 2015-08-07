// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixMsgContent.hpp

	File Description	:	Definition of the PFixMsgContent class.

	Revision History	:	2015-07-19 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFix__PFixMsgContent_hpp__HH

#define HH__MLB__VFix__PFixMsgContent_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PFixMsgContent.hpp

	\brief	The VFix header file containing the definition of the
	PFixMsgContent class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/VFixXPortType.hpp>
#include <VFix/VFixMISetHolder.hpp>

#include <RapidXmlUtils/RapidXmlContext.hpp>
#include <RapidXmlUtils/XmlDomElement.hpp>

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
/*
	Moved here preparatory to a type change in order to accomodate postion
	values shown in the FIX repository of the form:

		\d+(\.\d+)?
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixMsgContentSet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixMsgContent {
	typedef boost::multi_index_container<PFixMsgContent> PFixMsgContentMISet;
public:

	PFixMsgContent();

	explicit PFixMsgContent(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixMsgContent &other) const
	{
		return(component_id_ < other.component_id_);
	}

	void swap(PFixMsgContent &other);

	std::string GetIdString() const;

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	static std::ostream &EmitTabularByTag(const PFixMsgContentSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabularByPosition(const PFixMsgContentSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabular(const PFixMsgContentSet &in_set,
		std::ostream &o_str = std::cout);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixMsgContent &datum);

	//	Search for the provided component id as a string.
	static const PFixMsgContent *FindElementByTag(const PFixMsgContentSet &in_set,
		PFixComponentId component_id_key, const std::string &tag_key,
		bool throw_if_not_found = false);
	static const PFixMsgContent *FindElementByTag(const PFixMsgContentSet &in_set,
		PFixComponentId component_id_key, PFixTagNum tag_key,
		bool throw_if_not_found = false);
	static const PFixMsgContent *FindElementByPosition(
		const PFixMsgContentSet &in_set, PFixComponentId component_id_key,
		const std::string &position_key, bool throw_if_not_found = false);

	/*
		This method searches first for a unique combination of component id and
		tag number, then for a unique combination of component id and position
		string.
	*/
	static const PFixMsgContent *FindElement(const PFixMsgContentSet &in_set,
		PFixComponentId component_id_key, const std::string &key,
		bool thow_if_not_found = false);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixMsgContentSet &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixMsgContentSet &out_set);
	static PFixMsgContentSet &ParseXmlFile(const std::string &file_name,
		PFixMsgContentSet &out_set);
	static PFixMsgContentSet  ParseXmlFile(const std::string &file_name);

	static void              AddElement(const PFixMsgContent &element,
		PFixMsgContentSet &out_set);

	PFixComponentId component_id_;
	std::string     tag_text_;
	PFixTagNum      tag_;
	PFixIndent      indent_;
	std::string     position_;
	bool            reqd_;
	std::string     fix_version_;
	std::string     description_;

private:
	PFixMsgContent(PFixComponentId component_id, const std::string &tag_text,
		PFixTagNum tag, PFixIndent indent, const std::string &position,
		bool reqd, const std::string &fix_version,
		const std::string &description);
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct PFixMsgContentByCompIdTag { };
struct PFixMsgContentByCompIdPos { };
//	----------------------------------------------------------------------------
typedef boost::multi_index_container<
	PFixMsgContent,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<PFixMsgContentByCompIdTag>,
			boost::multi_index::composite_key<
				PFixMsgContent,
				BOOST_MULTI_INDEX_MEMBER(PFixMsgContent, PFixComponentId, component_id_),
				BOOST_MULTI_INDEX_MEMBER(PFixMsgContent, std::string,     tag_text_)
			>
		>
		,
		boost::multi_index::ordered_non_unique<
			boost::multi_index::tag<PFixMsgContentByCompIdPos>,
			boost::multi_index::composite_key<
				PFixMsgContent,
				BOOST_MULTI_INDEX_MEMBER(PFixMsgContent, PFixComponentId, component_id_),
				BOOST_MULTI_INDEX_MEMBER(PFixMsgContent, std::string,     position_)
			>
		>
	>
> PFixMsgContentMISet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixMsgContentSet : public VFixMISetHolder<PFixMsgContentMISet> { };
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixMsgContentSet &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixMsgContent_hpp__HH

