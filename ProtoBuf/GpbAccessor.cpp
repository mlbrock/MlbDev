//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GpbAccessor.cpp

	File Description	:	Implementation of the GpbAccessor functions.

	Revision History	:	2016-07-26 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/IsGpbType.hpp>

#include <mbtypes.h>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
struct GpbCppToTypeBase {
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <enum ::google::protobuf::FieldDescriptor::CppType>
	struct GpbCppToType;
//	----------------------------------------------------------------------------
template <> 
	struct GpbCppToType< ::google::protobuf::FieldDescriptor::CPPTYPE_INT32> :
		public GpbCppToTypeBase {
	typedef Native_SInt32                           type;
	typedef ::google::protobuf::RepeatedField<type> repeated_type;

	static const ::google::protobuf::FieldDescriptor::CppType cpp_type =
		::google::protobuf::FieldDescriptor::CPPTYPE_INT32;

	static type GetSimple(const ::google::protobuf::Message &src_msg,
		const ::google::protobuf::Reflection &src_reflection,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor)
	{
		return(src_reflection.GetInt32(src_msg, &src_field_descriptor));
	}

	static type GetSimple(const ::google::protobuf::Message &src_msg,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor)
	{
		return(GetSimple(src_msg, *src_msg.GetReflection(), src_field_descriptor));
	}

	static void SetSimple(::google::protobuf::Message &dst_msg,
		const ::google::protobuf::Reflection &dst_reflection,
		const ::google::protobuf::FieldDescriptor &dst_field_descriptor,
		const type &value)
	{
		dst_reflection.SetInt32(&dst_msg, &dst_field_descriptor, value);
	}

	static void SetSimple(::google::protobuf::Message &dst_msg,
		const ::google::protobuf::FieldDescriptor &dst_field_descriptor,
		const type &value)
	{
		SetSimple(dst_msg, *dst_msg.GetReflection(), dst_field_descriptor, value);
	}

	static void CopySimple(const ::google::protobuf::Message &src_msg,
		const ::google::protobuf::Reflection &src_reflection,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor,
		::google::protobuf::Message &dst_msg,
		const ::google::protobuf::Reflection &dst_reflection,
		const ::google::protobuf::FieldDescriptor &dst_field_descriptor)
	{
		SetSimple(dst_msg, dst_reflection, dst_field_descriptor,
			GetSimple(src_msg, src_reflection, src_field_descriptor));
	}

	static void CopySimple(const ::google::protobuf::Message &src_msg,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor,
		::google::protobuf::Message &dst_msg,
		const ::google::protobuf::FieldDescriptor &dst_field_descriptor)
	{
		CopySimple(src_msg, *src_msg.GetReflection(), src_field_descriptor,
			dst_msg, *dst_msg.GetReflection(), dst_field_descriptor);
	}

	static const repeated_type &GetRepeated(
		const ::google::protobuf::Message &src_msg,
		const ::google::protobuf::Reflection &src_reflection,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor)
	{
		return(src_reflection.GetRepeatedField<type>(src_msg,
			&src_field_descriptor));
	}

	static const repeated_type &GetRepeated(
		const ::google::protobuf::Message &src_msg,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor)
	{
		return(GetRepeated(src_msg, *src_msg.GetReflection(),
			src_field_descriptor));
	}

	static repeated_type &GetRepeatedMutable(
		::google::protobuf::Message &src_msg,
		const ::google::protobuf::Reflection &src_reflection,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor)
	{
		return(*src_reflection.MutableRepeatedField<type>(&src_msg,
			&src_field_descriptor));
	}

	static repeated_type &GetRepeatedMutable(
		::google::protobuf::Message &src_msg,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor)
	{
		return(GetRepeatedMutable(src_msg, *src_msg.GetReflection(),
			src_field_descriptor));
	}

	static void SetRepeated(::google::protobuf::Message &dst_msg,
		const ::google::protobuf::Reflection &dst_reflection,
		const ::google::protobuf::FieldDescriptor &dst_field_descriptor,
		const repeated_type &value)
	{
		GetRepeatedMutable(dst_msg, dst_reflection, dst_field_descriptor).
			CopyFrom(value);
	}

	static void SetRepeated(::google::protobuf::Message &dst_msg,
		const ::google::protobuf::FieldDescriptor &dst_field_descriptor,
		const repeated_type &value)
	{
		SetRepeated(dst_msg, *dst_msg.GetReflection(), dst_field_descriptor, value);
	}

	static void CopyRepeated(const ::google::protobuf::Message &src_msg,
		const ::google::protobuf::Reflection &src_reflection,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor,
		::google::protobuf::Message &dst_msg,
		const ::google::protobuf::Reflection &dst_reflection,
		const ::google::protobuf::FieldDescriptor &dst_field_descriptor)
	{
		SetRepeated(dst_msg, dst_reflection, dst_field_descriptor,
			GetRepeated(src_msg, src_reflection, src_field_descriptor));
	}

	static void CopyRepeated(const ::google::protobuf::Message &src_msg,
		const ::google::protobuf::FieldDescriptor &src_field_descriptor,
		::google::protobuf::Message &dst_msg,
		const ::google::protobuf::FieldDescriptor &dst_field_descriptor)
	{
		CopyRepeated(src_msg, *src_msg.GetReflection(), src_field_descriptor,
			dst_msg, *dst_msg.GetReflection(), dst_field_descriptor);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <enum ::google::protobuf::FieldDescriptor::CppType>
	struct GpbAccessorSimple;
//	----------------------------------------------------------------------------
/*
template <>
	GpbAccessorSimple {
};
*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool AreTypesCompatibleInfo(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info, bool &are_compatible)
{
	bool are_identical = true;

	are_compatible = false;

	if (lhs_info.GetCppType() != rhs_info.GetCppType())
		return(false);

	if ((lhs_info.GetLabel() != rhs_info.GetLabel()) &&
		((lhs_info.GetLabel() == ::google::protobuf::FieldDescriptor::LABEL_REPEATED) ||
		 (rhs_info.GetLabel() == ::google::protobuf::FieldDescriptor::LABEL_REPEATED)))
		return(false);

	if (lhs_info.GetCppType() ==
		::google::protobuf::FieldDescriptor::CPPTYPE_ENUM) {
		if (lhs_info.GetEnumDescriptor() == rhs_info.GetEnumDescriptor())
			are_compatible = true;
		else {
			are_identical = false;
			;	//	Check number and permissible values for compatibility.
		}
	}
	else if (lhs_info.GetCppType() ==
		::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {
		if ((lhs_info.GetDescriptor() == rhs_info.GetDescriptor()) ||
			 (lhs_info.GetTypeNameFull() == rhs_info.GetTypeNameFull()))
			are_compatible = true;
		else {
			are_identical = false;
			;	//	Check each member --- recursively for those different.
		}
	}

	return(are_identical);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool AreTypesCompatible(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info)
{
	bool are_compatible;

	return((AreTypesCompatibleInfo(lhs_info, rhs_info, are_compatible)) ?
		true : are_compatible);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool AreTypesIdentical(const GpbElementInfo &lhs_info,
	const GpbElementInfo &rhs_info)
{
	bool are_compatible;

	return(AreTypesCompatibleInfo(lhs_info, rhs_info, are_compatible));
}
//	////////////////////////////////////////////////////////////////////////////


} // namespace ProtoBuf

} // namespace MLB

#ifdef TEST_MAIN

#include <ProtoBuf/GpbElementInfo.hpp>

#include <ProtoBuf/GetMessageNew.hpp>			// CODE NOTE: Needed by copy test only.
#include <Utility/Utility_Exception.hpp>		// CODE NOTE: Needed by copy test only.

#include <cstdlib>
#include <iostream>

#include <boost/io/ios_state.hpp>
#include <boost/algorithm/string.hpp>

namespace {

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitSep(char sep_char, std::streamsize sep_width = 79,
	std::ostream &o_str = std::cout)
{
	boost::io::ios_all_saver io_state(o_str);

	o_str << std::setfill(sep_char) << std::setw(sep_width) << "" << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *TEST_MessageNameList[] = {
	"GpbElementInfoTestOne::AddressBook",
	"GpbElementInfoTestTwo::AddressBook",
	"GpbElementInfoTestTwo::Person",
	"GpbElementInfoTestTwo::PhoneNumber",
	"GpbElementInfoTestThree::AddressBookOne",
	"GpbElementInfoTestThree::AddressBookTwo",
	"GpbElementInfoTestThree::PersonOne",
	"GpbElementInfoTestThree::PersonTwo",
	NULL
};
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

namespace TEST_MessageCopy {

//	////////////////////////////////////////////////////////////////////////////
void TEST_CreateMessageHelper(::google::protobuf::Message &msg,
	const MLB::ProtoBuf::GpbElementInfo &msg_info,
	const ::google::protobuf::Reflection &msg_reflection,
	const std::string &in_parental_path)
{
	using namespace MLB::ProtoBuf;

	std::string                                parental_path(in_parental_path);
	const ::google::protobuf::FieldDescriptor *field_descriptor =
		msg_info.GetFieldDescriptor();

	if (*msg_info.GetMemberName() && field_descriptor) {
		double value_num = ::pow(10.0, 1.0 + msg_info.GetDepth()) + msg_info.GetMemberIndex();
		parental_path += std::string(".") + msg_info.GetMemberName();
		switch (msg_info.GetDatumType()) {
			case GpbDatumType_SInt32		:
/*
				msg_reflection.SetInt32(&msg, field_descriptor, value_num);
				break;
*/
				GpbCppToType<
					::google::protobuf::FieldDescriptor::CPPTYPE_INT32
				>::SetSimple(msg, msg_reflection, *field_descriptor,
					static_cast<GpbCppToType<
						::google::protobuf::FieldDescriptor::CPPTYPE_INT32
					>::type>(value_num));
				break;
			case GpbDatumType_SInt64		:
				msg_reflection.SetInt64(&msg, field_descriptor, value_num);
				break;
			case GpbDatumType_UInt32		:
				msg_reflection.SetUInt32(&msg, field_descriptor, value_num);
				break;
			case GpbDatumType_UInt64		:
				msg_reflection.SetUInt64(&msg, field_descriptor, value_num);
				break;
			case GpbDatumType_Double		:
				msg_reflection.SetDouble(&msg, field_descriptor, value_num);
				break;
			case GpbDatumType_Float			:
				msg_reflection.SetFloat(&msg, field_descriptor, value_num);
				break;
			case GpbDatumType_Bool			:
				msg_reflection.SetBool(&msg, field_descriptor,
					(msg_info.GetMemberIndex() % 2) ? true : false);
				break;
/*
			case GpbDatumType_Enum			:
				msg_reflection.SetEnum(&msg, field_descriptor,
					src_reflection->GetEnum(src_msg, descriptors_list_[src_index]));
				break;
*/
			case GpbDatumType_Enum			:
				break;
			case GpbDatumType_String		:
				{
					std::ostringstream o_str;
					o_str << parental_path << "=" << msg_info.GetMemberIndex();
					msg_reflection.SetString(&msg, field_descriptor, o_str.str());
				}
				break;
#if 0
			case GpbDatumType_Message		:
				{
					GpbElementInfoVectorIterC iter_b(msg_info.GetMemberList().begin());
					GpbElementInfoVectorIterC iter_e(msg_info.GetMemberList().end());
					for ( ; iter_b != iter_e; ++iter_b) {
						if (iter_b->GetDescriptor()) {
							::google::protobuf::Message *field_msg =
								msg_reflection.MutableMessage(&msg,
								iter_b->GetFieldDescriptor());
							TEST_CreateMessageHelper(*field_msg, *iter_b,
								*field_msg->GetReflection(), parental_path);
						}
						else
							TEST_CreateMessageHelper(msg, *iter_b,
								msg_reflection, parental_path);
					}
				}
				break;
			case GpbDatumType_Message		:
				msg_reflection.MutableMessage(msg, field_descriptor)->
					CopyFrom(src_reflection->GetMessage(src_msg,
					descriptors_list_[src_index]));
				break;
			case GpbDatumType_Repeated		:
				//	Munged.
				msg_reflection.MutableRepeatedField<int>(msg, field_descriptor)->
					CopyFrom(src_reflection->GetRepeatedField<int>(src_msg,
					descriptors_list_[src_index]));
				break;
			case GpbDatumType_RepeatedPtr	:
				/*
					Also munged, but won't work because method invokes
					protected method MutableRawRepeated[String|Message]() which
					needs a pointer to the Descriptor for the type.

					Will get around it --- preferably by inheritance using my
					own derived class to get access. Or by the #define trick.
				*/
				msg_reflection.MutableRepeatedPtrField<std::string>(&msg,
					field_descriptor)->CopyFrom(src_reflection->GetRepeatedPtrField<std::string>(src_msg,
					descriptors_list_[src_index]));
				break;
#endif // #if 0
			case GpbDatumType_Message		:
			case GpbDatumType_Repeated		:
			case GpbDatumType_RepeatedPtr	:
				break;
			default								:
				{
					std::ostringstream o_str;
					o_str << "Unknown/invalid GpbElementInfo datum type "
						"encountered (" << msg_info.GetDatumType() << ") --- this is an "
						"internal error.";
					MLB::Utility::ThrowRuntimeError(o_str.str());
				}
				break;
		}
	}

	GpbElementInfoVectorIterC iter_b(msg_info.GetMemberList().begin());
	GpbElementInfoVectorIterC iter_e(msg_info.GetMemberList().end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->IsGpbTypeRepeated())
			continue;
		if (iter_b->GetDescriptor()) {
			::google::protobuf::Message *field_msg =
				msg_reflection.MutableMessage(&msg,
				iter_b->GetFieldDescriptor());
			TEST_CreateMessageHelper(*field_msg, *iter_b,
				*field_msg->GetReflection(), parental_path);
		}
		else
			TEST_CreateMessageHelper(msg, *iter_b,
				msg_reflection, parental_path);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::ProtoBuf::GpbMessageSPtr TEST_CreateMessage(
	const MLB::ProtoBuf::GpbElementInfo &msg_info)
{
	using namespace MLB::ProtoBuf;

	GpbMessageSPtr msg_sptr(GetMessageNewSPtr(msg_info.GetDescriptor()));

	TEST_CreateMessageHelper(*msg_sptr, msg_info,
		*msg_sptr->GetReflection(), msg_info.GetTypeNameFull());

	std::cout << msg_sptr->DebugString() << std::flush;

	return(msg_sptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TEST_RunTestForMessage(int &return_code, const char *message_name)
{
	TEST_EmitSep('=');
	TEST_EmitSep('=');

	std::cout << "TEST_MessageCopy: " << message_name << ":" << std::endl;

	bool test_passed = true;

	try {
		MLB::ProtoBuf::GpbElementInfo element(message_name);
		TEST_EmitSep('-');
		TEST_CreateMessage(element);
	}
	catch (const std::exception &except) {
		std::cerr << "TEST FAILED: " << except.what() << std::endl;
		test_passed = false;
	}

	TEST_EmitSep('=');
	std::cout << std::endl;

	if (!test_passed)
		return_code = EXIT_FAILURE;

	return(test_passed);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RunTest(int &return_code)
{
	for (const char **name_ptr = TEST_MessageNameList; *name_ptr != NULL;
		++name_ptr)
		TEST_RunTestForMessage(return_code, *name_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace TEST_MessageCopy

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_MessageCopy::RunTest(return_code);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

