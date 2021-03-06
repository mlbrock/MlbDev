// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixFieldType.hpp

	File Description	:	Definition of the PFixFieldType class.

	Revision History	:	2015-06-27 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFix__PFixFieldType_hpp__HH

#define HH__MLB__VFix__PFixFieldType_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PFixFieldType.hpp

	\brief	The VFix header file containing the definition of the
	PFixFieldType class.
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

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
class PFixFieldType {
public:
	typedef std::set<PFixFieldType> PFixFieldTypeSet_I;

	//	Constructor used for searches...
	explicit PFixFieldType(const std::string &name = "");

	explicit PFixFieldType(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixFieldType &other) const
	{
		return(name_ < other.name_);
	}

	void swap(PFixFieldType &other);

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	static std::ostream &EmitTabular(const PFixFieldTypeSet_I &in_set,
		std::ostream &o_str = std::cout);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixFieldType &datum);

	static const PFixFieldType *FindElement(const PFixFieldTypeSet_I &in_set,
		const std::string &name, bool throw_if_not_found = false);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixFieldTypeSet_I &ParseXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixFieldTypeSet_I &out_set);
	static PFixFieldTypeSet_I &ParseXmlFile(const std::string &file_name,
		PFixFieldTypeSet_I &out_set);
	static PFixFieldTypeSet_I  ParseXmlFile(const std::string &file_name);

	static void                AddElement(const PFixFieldType &element,
		PFixFieldTypeSet_I &out_set);

	//	CODE NOTE: For testing. May be removed at some later date.
	static std::set<VFixXPortType> &CheckForUnusedTypes(
		const PFixFieldTypeSet_I &in_set, std::set<VFixXPortType> &out_set);
	//	CODE NOTE: For testing. May be removed at some later date.
	static std::size_t    EmitUnusedTypes(const PFixFieldTypeSet_I &in_set,
		std::ostream &o_str = std::cout);

	std::string   name_;
	std::string   base_name_1_;
	std::string   base_name_2_;
	std::string   fix_version_;
	std::string   description_;
	VFixXPortType vfix_xport_type_;

private:
	PFixFieldType(const std::string &name, const std::string &base_name_1,
		const std::string &base_name_2, const std::string &fix_version,
		const std::string &description, VFixXPortType vfix_xport_type);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<PFixFieldType>          PFixFieldTypeVector;
typedef PFixFieldTypeVector::iterator       PFixFieldTypeVectorIter;
typedef PFixFieldTypeVector::const_iterator PFixFieldTypeVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef PFixFieldType::PFixFieldTypeSet_I     PFixFieldTypeSet;
typedef PFixFieldTypeSet::iterator            PFixFieldTypeSetIter;
typedef PFixFieldTypeSet::const_iterator      PFixFieldTypeSetIterC;
typedef std::pair<PFixFieldTypeSetIter, bool> PFixFieldTypeSetInsertPair;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PFixFieldTypeSet &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixFieldType_hpp__HH

