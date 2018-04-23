//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	AreGpbTypesCompatible.hpp

	File Description	:	Prototypes of the AreGpbTypesCompatible functions.

	Revision History	:  2016-07-26 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__AreGpbTypesCompatible_hpp___HH

#define HH___MLB__ProtoBuf__AreGpbTypesCompatible_hpp___HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/GpbElementInfo.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
bool AreTypesCompatibleInfoHelper(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info, bool &are_compatible,
	bool type_name_same_identical = false,
	bool type_name_same_compatible = false,
	bool ignore_enum_name_compatibility = false);
bool AreTypesCompatible(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info, bool type_name_same_identical = false,
	bool type_name_same_compatible = false,
	bool ignore_enum_name_compatibility = false);
bool AreTypesIdentical(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info, bool type_name_same_identical = false,
	bool type_name_same_compatible = false,
	bool ignore_enum_name_compatibility = false);
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__AreGpbTypesCompatible_hpp___HH

