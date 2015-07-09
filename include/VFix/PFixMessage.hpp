// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixMessage.hpp

	File Description	:	Definition of the PFixMessage class.

	Revision History	:	2015-07-09 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFix__PFixMessage_hpp__HH

#define HH__MLB__VFix__PFixMessage_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PFixMessage.hpp

	\brief	The VFix header file containing the definition of the
	PFixMessage class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/VFixXPortType.hpp>

#include <RapidXmlUtils/RapidXmlContext.hpp>
#include <RapidXmlUtils/XmlDomElement.hpp>

#include <set>

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
typedef unsigned int VFixComponentId;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixMessage {
	typedef boost::multi_index_container<PFixMessage> PFixMessageMISet;
public:
	typedef std::set<PFixMessage> PFixMessageSet_I;

	//	Constructor used for searches...
	explicit PFixMessage(const char *name = "");

	explicit PFixMessage(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixMessage &other) const
	{
		return(message_type_ < other.message_type_);
	}

	void swap(PFixMessage &other);

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	static std::ostream &EmitTabular(const PFixMessageSet_I &in_set,
		std::ostream &o_str = std::cout);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixMessage &datum);

	static const PFixMessage *FindElement(const PFixMessageSet_I &in_set,
		const char *name, bool thow_if_not_found = false);
	static const PFixMessage *FindElement(const PFixMessageSet_I &in_set,
		const std::string &name, bool thow_if_not_found = false);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixMessageSet_I &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixMessageSet_I &out_set);
	static PFixMessageSet_I &ParseXmlFile(const std::string &file_name,
		PFixMessageSet_I &out_set);
	static PFixMessageSet_I  ParseXmlFile(const std::string &file_name);

	static void              AddElement(const PFixMessage &element,
		PFixMessageSet_I &out_set);

	VFixComponentId component_id_;
	std::string     message_type_;
	std::string     name_;
	std::string     category_id_;
	std::string     section_id_;
	std::string     abbreviation_;
	std::string     fix_version_;
	std::string     description_;

private:
	PFixMessage(VFixComponentId component_id, const std::string &message_type,
		const std::string &name, const std::string &category_id,
		const std::string &section_id, const std::string &abbreviation,
		const std::string &fix_version, const std::string &description);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<PFixMessage>          PFixMessageVector;
typedef PFixMessageVector::iterator       PFixMessageVectorIter;
typedef PFixMessageVector::const_iterator PFixMessageVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef PFixMessage::PFixMessageSet_I       PFixMessageSet;
typedef PFixMessageSet::iterator            PFixMessageSetIter;
typedef PFixMessageSet::const_iterator      PFixMessageSetIterC;
typedef std::pair<PFixMessageSetIter, bool> PFixMessageSetInsertPair;
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct PFixMessageByCompId  { };
struct PFixMessageByMsgType { };
struct PFixMessageByName    { };
struct PFixMessageByAbbr    { };
//	----------------------------------------------------------------------------
typedef boost::multi_index_container<
	PFixMessage,
		boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<PFixMessageByCompId>,
				BOOST_MULTI_INDEX_MEMBER(PFixMessage, VFixComponentId , component_id_)
			>
			,
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<PFixMessageByMsgType>,
				BOOST_MULTI_INDEX_MEMBER(PFixMessage, std::string, message_type_)
			>
			,
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<PFixMessageByName>,
				BOOST_MULTI_INDEX_MEMBER(PFixMessage, std::string, name_)
			>
			,
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<PFixMessageByAbbr>,
				BOOST_MULTI_INDEX_MEMBER(PFixMessage, std::string, abbreviation_)
			>
		>
	> PFixMessageMISet;
//	----------------------------------------------------------------------------
typedef boost::multi_index::index<PFixMessageMISet, 
	PFixMessageByCompId>::type                       PFixMessageMISetIdxByCompId;
typedef PFixMessageMISetIdxByCompId::iterator       PFixMessageMISetIdxByCompIdIter;
typedef PFixMessageMISetIdxByCompId::const_iterator PFixMessageMISetIdxByCompIdIterC;

//	----------------------------------------------------------------------------
typedef boost::multi_index::index<PFixMessageMISet, 
	PFixMessageByMsgType>::type                       PFixMessageMISetIdxByMsgType;
typedef PFixMessageMISetIdxByMsgType::iterator       PFixMessageMISetIdxByMsgTypeIter;
typedef PFixMessageMISetIdxByMsgType::const_iterator PFixMessageMISetIdxByMsgTypeIterC;
//	----------------------------------------------------------------------------
typedef boost::multi_index::index<PFixMessageMISet,
	PFixMessageByName>::type                       PFixMessageMISetIdxByName;
typedef PFixMessageMISetIdxByName::iterator       PFixMessageMISetIdxByNameIter;
typedef PFixMessageMISetIdxByName::const_iterator PFixMessageMISetIdxByNameIterC;
//	----------------------------------------------------------------------------
typedef boost::multi_index::index<PFixMessageMISet,
	PFixMessageByAbbr>::type                       PFixMessageMISetIdxByAbbr;
typedef PFixMessageMISetIdxByAbbr::iterator       PFixMessageMISetIdxByAbbrIter;
typedef PFixMessageMISetIdxByAbbr::const_iterator PFixMessageMISetIdxByAbbrIterC;
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixMessageSet &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixMessage_hpp__HH

