// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixRepository.hpp

	File Description	:	Definition of the PFixRepository class.

	Revision History	:	2015-08-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFix__PFixRepository_hpp__HH

#define HH__MLB__VFix__PFixRepository_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PFixRepository.hpp

	\brief	The VFix header file containing the definition of the
	PFixRepository class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/PFixComponent.hpp>
#include <VFix/PFixEnum.hpp>
#include <VFix/PFixField.hpp>
#include <VFix/PFixFieldType.hpp>
#include <VFix/PFixMessage.hpp>
#include <VFix/PFixMsgContent.hpp>

#include <boost/shared_ptr.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
class VFixInsItem {
	std::string name_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<PFixComponentSet>  PFixComponentSetSPtr;
typedef boost::shared_ptr<PFixEnumSet>       PFixEnumSetSPtr;
typedef boost::shared_ptr<PFixFieldSet>      PFixFieldSetSPtr;
typedef boost::shared_ptr<PFixFieldTypeSet>  PFixFieldTypeSetSPtr;
typedef boost::shared_ptr<PFixMessageSet>    PFixMessageSetSPtr;
typedef boost::shared_ptr<PFixMsgContentSet> PFixMsgContentSetSPtr;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PFixRepository {
public:

	PFixRepository();

	explicit PFixRepository(const std::string &directory_name);

	void swap(PFixRepository &other);

	void LoadFromDirectory(const std::string &directory_name = "./");

	std::ostream &EmitTabular(std::ostream &o_str = std::cout) const;

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixRepository &datum);

private:
	PFixComponentSetSPtr  component_set_ptr_;
	PFixEnumSetSPtr       enum_set_ptr_;
	PFixFieldSetSPtr      field_set_ptr_;
	PFixFieldTypeSetSPtr  fieldtype_set_ptr_;
	PFixMessageSetSPtr    message_set_ptr_;
	PFixMsgContentSetSPtr msgcontent_set_ptr_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixRepository &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#endif // #ifdef HH__MLB__VFix__PFixRepository_hpp__HH

