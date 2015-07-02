// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixFieldType.hpp

	File Description	:	Definition of the PFixFieldType class.

	Revision History	:	2015-06-27 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
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
	explicit PFixFieldType(const char *name = "");

	explicit PFixFieldType(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixFieldType &other) const
	{
		return(name_ < other.name_);
	}

	void swap(PFixFieldType &other);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixFieldType &datum);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixFieldTypeSet_I &LoadFromXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixFieldTypeSet_I &out_set);

	static PFixFieldTypeSet_I &LoadFromXmlFile(const std::string &file_name,
		PFixFieldTypeSet_I &out_set);
	static PFixFieldTypeSet_I  LoadFromXmlFile(const std::string &file_name);

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

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixFieldType_hpp__HH

