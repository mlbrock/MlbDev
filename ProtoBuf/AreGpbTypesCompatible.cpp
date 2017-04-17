//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	AreGpbTypesCompatible.cpp

	File Description	:	Implementation of the AreGpbTypesCompatible functions.

	Revision History	:  2016-07-26 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/AreGpbTypesCompatible.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

namespace {

//	////////////////////////////////////////////////////////////////////////////
bool AreTypesCompatibleInfoHelper(bool &are_compatible,
	bool type_name_same_identical, bool type_name_same_compatible)
{
	if (type_name_same_identical) {
		are_compatible = true;
		return(true);
	}

	if (type_name_same_compatible)
		are_compatible = true;

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
bool AreTypesCompatibleInfo(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info, bool &are_compatible,
	bool type_name_same_identical, bool type_name_same_compatible,
	bool ignore_enum_name_compatibility)
{
	bool are_identical = true;

	are_compatible = false;

	if (lhs_info.GetCppType() != rhs_info.GetCppType())
		return(false);

	if ((lhs_info.GetLabel() != rhs_info.GetLabel()) &&
		(lhs_info.IsGpbTypeRepeated() != rhs_info.IsGpbTypeRepeated()))
		return(false);

	if (lhs_info.GetCppType() ==
		::google::protobuf::FieldDescriptor::CPPTYPE_ENUM) {
		if (lhs_info.GetEnumDescriptor() == rhs_info.GetEnumDescriptor()) {
			are_compatible = true;
			return(true);
		}
		if ((type_name_same_identical || type_name_same_compatible) &&
			 (lhs_info.GetTypeNameFull() == rhs_info.GetTypeNameFull())) {
			are_identical = AreTypesCompatibleInfoHelper(are_compatible,
				type_name_same_identical, type_name_same_compatible);
			if (are_identical || are_compatible)
				return(are_identical);
		}
		are_identical = false;
		int lhs_count = lhs_info.GetEnumDescriptor()->value_count();
		if (lhs_count != rhs_info.GetEnumDescriptor()->value_count())
			return(false);
		for (int count_1 = 0; count_1 < lhs_count; ++count_1) {
			if (lhs_info.GetEnumDescriptor()->value(count_1)->number() !=
				 rhs_info.GetEnumDescriptor()->value(count_1)->number())
				return(false);
			if ((!ignore_enum_name_compatibility) &&
				 (lhs_info.GetEnumDescriptor()->value(count_1)->name() !=
				  rhs_info.GetEnumDescriptor()->value(count_1)->name()))
				return(false);
		}
	}
	else if (lhs_info.GetCppType() ==
		::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {
		if (lhs_info.GetDescriptor() == rhs_info.GetDescriptor()) {
			are_compatible = true;
			return(true);
		}
		if ((type_name_same_identical || type_name_same_compatible) &&
			 (lhs_info.GetTypeNameFull() == rhs_info.GetTypeNameFull())) {
			are_identical = AreTypesCompatibleInfoHelper(are_compatible,
				type_name_same_identical, type_name_same_compatible);
			if (are_identical || are_compatible)
				return(are_identical);
		}
		are_identical = false;
		if (lhs_info.GetMemberList().size() != rhs_info.GetMemberList().size())
			return(false);
		GpbElementInfoVectorIterC lhs_iter_b(lhs_info.GetMemberList().begin());
		GpbElementInfoVectorIterC lhs_iter_e(lhs_info.GetMemberList().end());
		GpbElementInfoVectorIterC rhs_iter_b(lhs_info.GetMemberList().begin());
		for ( ; lhs_iter_b != lhs_iter_e; ++lhs_iter_b, ++rhs_iter_b) {
			if ((!AreTypesCompatibleInfo(*lhs_iter_b, *rhs_iter_b,
				are_compatible, type_name_same_identical,
				type_name_same_compatible, ignore_enum_name_compatibility)) &&
				(!are_compatible))
				return(false);
		}
	}

	are_compatible = true;

	return(are_identical);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool AreTypesCompatible(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info, bool type_name_same_identical,
	bool type_name_same_compatible, bool ignore_enum_name_compatibility)
{
	bool are_compatible;

	return((AreTypesCompatibleInfoHelper(lhs_info, rhs_info, are_compatible,
		type_name_same_identical, type_name_same_compatible,
		ignore_enum_name_compatibility)) ? true : are_compatible);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool AreTypesIdentical(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info, bool type_name_same_identical,
	bool type_name_same_compatible, bool ignore_enum_name_compatibility)
{
	bool are_compatible;

	return(AreTypesCompatibleInfoHelper(lhs_info, rhs_info, are_compatible,
		type_name_same_identical, type_name_same_compatible,
		ignore_enum_name_compatibility));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

