// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixMessage.hpp

	File Description	:	Definition of the PFixMessage class.

	Revision History	:	2015-07-09 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2017.
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
#include <VFix/VFixMISetHolder.hpp>

#include <RapidXmlUtils/RapidXmlContext.hpp>
#include <RapidXmlUtils/XmlDomElement.hpp>

#include <set>

#ifdef _Windows
# pragma warning(push)
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
class PFixMessageSet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixMessage {
	typedef boost::multi_index_container<PFixMessage> PFixMessageMISet;
public:

	//	Constructor used for searches...
	explicit PFixMessage(const std::string &name = "");

	explicit PFixMessage(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixMessage &other) const
	{
		return(message_type_ < other.message_type_);
	}

	void swap(PFixMessage &other);

	std::string GetIdString() const;

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	static std::ostream &EmitTabularByMessageType(const PFixMessageSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabularByName(const PFixMessageSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabularByAbbr(const PFixMessageSet &in_set,
		std::ostream &o_str = std::cout);
	static std::ostream &EmitTabular(const PFixMessageSet &in_set,
		std::ostream &o_str = std::cout);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixMessage &datum);

	static const PFixMessage *FindElementByMessageType(
		const PFixMessageSet &in_set, const std::string &key,
		bool throw_if_not_found = false);

	//	Only works for message types which are numeric, obviously.
	static const PFixMessage *FindElementByMessageType(
		const PFixMessageSet &in_set, std::size_t key,
		bool throw_if_not_found = false);
	static const PFixMessage *FindElementByName(const PFixMessageSet &in_set,
		const std::string &key, bool throw_if_not_found = false);
	static const PFixMessage *FindElementByAbbr(const PFixMessageSet &in_set,
		const std::string &key, bool throw_if_not_found = false);

	/*
		This method searches first by name, then by abbreviation, and finally
		message type.
	*/
	static const PFixMessage *FindElement(const PFixMessageSet &in_set,
		const std::string &key, bool thow_if_not_found = false);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixMessageSet &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixMessageSet &out_set);
	static PFixMessageSet &ParseXmlFile(const std::string &file_name,
		PFixMessageSet &out_set);
	static PFixMessageSet  ParseXmlFile(const std::string &file_name);

	static void              AddElement(const PFixMessage &element,
		PFixMessageSet &out_set);

	PFixComponentId component_id_;
	std::string     message_type_;
	std::string     name_;
	std::string     category_id_;
	std::string     section_id_;
	std::string     abbreviation_;
	std::string     fix_version_;
	std::string     description_;

private:
	PFixMessage(PFixComponentId component_id, const std::string &message_type,
		const std::string &name, const std::string &category_id,
		const std::string &section_id, const std::string &abbreviation,
		const std::string &fix_version, const std::string &description);
};
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
			boost::multi_index::tag<PFixMessageByCompId>,
			BOOST_MULTI_INDEX_MEMBER(PFixMessage, PFixComponentId , component_id_)
		>
		,
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<PFixMessageByAbbr>,
			BOOST_MULTI_INDEX_MEMBER(PFixMessage, std::string, abbreviation_)
		>
	>
> PFixMessageMISet;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixMessageSet : public VFixMISetHolder<PFixMessageMISet> { };
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixMessageSet &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixMessage_hpp__HH

