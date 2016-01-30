// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixComponent.hpp

	File Description	:	Definition of the PFixComponent class.

	Revision History	:	2015-07-19 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFix__PFixComponent_hpp__HH

#define HH__MLB__VFix__PFixComponent_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PFixComponent.hpp

	\brief	The VFix header file containing the definition of the
	PFixComponent class.
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
class PFixComponentSet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixComponent {
	typedef boost::multi_index_container<PFixComponent> PFixComponentMISet;
public:

	//	Constructors used for searches...
	explicit PFixComponent(PFixComponentId component_id = 0);
	explicit PFixComponent(const std::string &name);

	explicit PFixComponent(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixComponent &other) const
	{
		return(component_id_ < other.component_id_);
	}

	void swap(PFixComponent &other);

	std::string GetIdString() const;

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	static std::ostream &EmitTabularById(const PFixComponentSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabularByName(const PFixComponentSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabularByAbbr(const PFixComponentSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabular(const PFixComponentSet &in_set,
		std::ostream &o_str = std::cout);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixComponent &datum);

	//	Search for the provided component id as a string.
	static const PFixComponent *FindElementById(const PFixComponentSet &in_set,
		const std::string &key, bool throw_if_not_found = false);
	static const PFixComponent *FindElementById(const PFixComponentSet &in_set,
		PFixComponentId key, bool throw_if_not_found = false);
	static const PFixComponent *FindElementByName(const PFixComponentSet &in_set,
		const std::string &key, bool throw_if_not_found = false);
	static const PFixComponent *FindElementByAbbr(const PFixComponentSet &in_set,
		const std::string &key, bool throw_if_not_found = false);

	/*
		This method searches first by name, then by abbreviation, and finally
		component id.
	*/
	static const PFixComponent *FindElement(const PFixComponentSet &in_set,
		const std::string &key, bool thow_if_not_found = false);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixComponentSet &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixComponentSet &out_set);
	static PFixComponentSet &ParseXmlFile(const std::string &file_name,
		PFixComponentSet &out_set);
	static PFixComponentSet  ParseXmlFile(const std::string &file_name);

	static void              AddElement(const PFixComponent &element,
		PFixComponentSet &out_set);

	PFixComponentId component_id_;
	std::string     component_type_;
	std::string     name_;
	std::string     category_id_;
	std::string     abbreviation_;
	std::string     fix_version_;
	std::string     description_;

private:
	PFixComponent(PFixComponentId component_id, const std::string &component_type,
		const std::string &name, const std::string &category_id,
		const std::string &abbreviation, const std::string &fix_version,
		const std::string &description);
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct PFixComponentByCompId  { };
struct PFixComponentByName    { };
struct PFixComponentByAbbr    { };
//	----------------------------------------------------------------------------
typedef boost::multi_index_container<
	PFixComponent,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<PFixComponentByCompId>,
			BOOST_MULTI_INDEX_MEMBER(PFixComponent, PFixComponentId , component_id_)
		>
		,
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<PFixComponentByName>,
			BOOST_MULTI_INDEX_MEMBER(PFixComponent, std::string, name_)
		>
		,
		boost::multi_index::ordered_non_unique<
			boost::multi_index::tag<PFixComponentByAbbr>,
			BOOST_MULTI_INDEX_MEMBER(PFixComponent, std::string, abbreviation_)
		>
	>
> PFixComponentMISet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixComponentSet : public VFixMISetHolder<PFixComponentMISet> { };
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixComponentSet &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixComponent_hpp__HH

