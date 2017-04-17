// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for FieldOperator enumeration support.

	Revision History	:	2008-11-15 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__FieldOperator_hpp__HH

#define HH__MLB__VFast__FieldOperator_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file FieldOperator.hpp

	\brief	The VFast field operator header file.

	Contains definitions and logic required for FieldOperator enumeration
		support.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	\enum FieldOperator The permissible Fix Fast operators.
*/
enum FieldOperator {
	/** Field has no field operator. */
	FieldOperator_None      = 0,
	/** Field has a \e constant field operator. */
	FieldOperator_Constant  = 1,
	/** Field has a \e copy field operator. */
	FieldOperator_Copy      = 2,
	/** Field has a \e default field operator. */
	FieldOperator_Default   = 3,
	/** Field has a \e delta field operator. */
	FieldOperator_Delta     = 4,
	/** Field has a \e increment field operator. */
	FieldOperator_Increment = 5,
	/** Field has a \e tail field operator. */
	FieldOperator_Tail      = 6,
	/** The minimum field operator value. */
	FieldOperator_Min       = FieldOperator_None,
	/** The maximum field operator value. */
	FieldOperator_Max       = FieldOperator_Tail,
	/** The total number of field operators. */
	FieldOperator_Count     = (FieldOperator_Max - FieldOperator_Min) + 1
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool          FieldOperatorIsValid(FieldOperator field_operator);
FieldOperator CheckFieldOperator(FieldOperator field_operator);
bool          IsFieldOperatorString(const std::string &in_field_operator,
	FieldOperator &out_field_operator);
bool          IsFieldOperatorString(const std::string &in_field_operator);
std::string   FieldOperatorToString(FieldOperator field_operator);
FieldOperator StringToFieldOperator(const std::string &field_operator);
std::ostream & operator << (std::ostream &o_str, const FieldOperator &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__FieldOperator_hpp__HH

