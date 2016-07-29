//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	IsGpbType.cpp

	File Description	:	Implementation of the IsGpbType functions.

	Revision History	:	2016-07-05 --- Creation.
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

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(GpbDatumType datum_type)
{
	return(
		(datum_type == GpbDatumType_SInt32) ||
		(datum_type == GpbDatumType_SInt64) ||
		(datum_type == GpbDatumType_UInt32) ||
		(datum_type == GpbDatumType_UInt64) ||
		(datum_type == GpbDatumType_Double) ||
		(datum_type == GpbDatumType_Float ) ||
		(datum_type == GpbDatumType_Bool  ) ||
		(datum_type == GpbDatumType_Enum  ));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(
	::google::protobuf::FieldDescriptor::CppType cpp_type)
{
	return(
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_INT32 ) ||
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_INT64 ) ||
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_UINT32) ||
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_UINT64) ||
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE) ||
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT ) ||
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL  ) ||
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_ENUM  ));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label)
{
	return(IsGpbTypeScalar(cpp_type) &&
		(label != ::google::protobuf::FieldDescriptor::LABEL_REPEATED));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(
	const ::google::protobuf::FieldDescriptor &field_descriptor)
{
	return(IsGpbTypeScalar(field_descriptor.cpp_type(),
		field_descriptor.label()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(
	const ::google::protobuf::FieldDescriptor *field_descriptor)
{
	return((field_descriptor) ? IsGpbTypeScalar(*field_descriptor) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(const GpbElementInfo &msg_info)
{
	return(IsGpbTypeScalar(msg_info.GetFieldDescriptor()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(const GpbElementInfo *msg_info_ptr)
{
	return((msg_info_ptr) ? IsGpbTypeScalar(*msg_info_ptr) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(const GpbElementInfoSPtr &msg_info_ptr)
{
	return(IsGpbTypeScalar(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(const GpbElementInfoUPtr &msg_info_ptr)
{
	return(IsGpbTypeScalar(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(GpbDatumType datum_type)
{
	return(IsGpbTypeScalar(datum_type) || (datum_type == GpbDatumType_String));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(
	::google::protobuf::FieldDescriptor::CppType cpp_type)
{
	return(IsGpbTypeScalar(cpp_type) ||
		(cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_STRING));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label)
{
	return(IsGpbTypeSimple(cpp_type) &&
		(label != ::google::protobuf::FieldDescriptor::LABEL_REPEATED));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(
	const ::google::protobuf::FieldDescriptor &field_descriptor)
{
	return(IsGpbTypeSimple(field_descriptor.cpp_type(),
		field_descriptor.label()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(
	const ::google::protobuf::FieldDescriptor *field_descriptor)
{
	return((field_descriptor) ? IsGpbTypeSimple(*field_descriptor) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(const GpbElementInfo &msg_info)
{
	return(IsGpbTypeSimple(msg_info.GetFieldDescriptor()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(const GpbElementInfo *msg_info_ptr)
{
	return((msg_info_ptr) ? IsGpbTypeSimple(*msg_info_ptr) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(const GpbElementInfoSPtr &msg_info_ptr)
{
	return(IsGpbTypeSimple(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(const GpbElementInfoUPtr &msg_info_ptr)
{
	return(IsGpbTypeSimple(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(GpbDatumType datum_type)
{
	return((datum_type == GpbDatumType_String) ||
			 (datum_type == GpbDatumType_Message));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(::google::protobuf::FieldDescriptor::CppType cpp_type)
{
	return((cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_STRING) ||
			 (cpp_type == ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label)
{
	return(IsGpbTypePtr(cpp_type) &&
		(label != ::google::protobuf::FieldDescriptor::LABEL_REPEATED));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(const ::google::protobuf::FieldDescriptor &field_descriptor)
{
	return(IsGpbTypePtr(field_descriptor.cpp_type(), field_descriptor.label()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(const ::google::protobuf::FieldDescriptor *field_descriptor)
{
	return((field_descriptor) ? IsGpbTypePtr(*field_descriptor) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(const GpbElementInfo &msg_info)
{
	return(IsGpbTypePtr(msg_info.GetFieldDescriptor()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(const GpbElementInfo *msg_info_ptr)
{
	return((msg_info_ptr) ? IsGpbTypePtr(*msg_info_ptr) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(const GpbElementInfoSPtr &msg_info_ptr)
{
	return(IsGpbTypePtr(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypePtr(const GpbElementInfoUPtr &msg_info_ptr)
{
	return(IsGpbTypePtr(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedScalar(GpbDatumType datum_type)
{
	return(datum_type == GpbDatumType_Repeated);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedScalar(
	::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label)
{
	return(IsGpbTypeScalar(cpp_type) &&
			(label == ::google::protobuf::FieldDescriptor::LABEL_REPEATED));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedScalar(
	const ::google::protobuf::FieldDescriptor &field_descriptor)
{
	return(IsGpbTypeRepeatedScalar(field_descriptor.cpp_type(),
		field_descriptor.label()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedScalar(
	const ::google::protobuf::FieldDescriptor *field_descriptor)
{
	return((field_descriptor) ? IsGpbTypeRepeatedScalar(*field_descriptor) :
		false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedScalar(const GpbElementInfo &msg_info)
{
	return(IsGpbTypeRepeatedScalar(msg_info.GetFieldDescriptor()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedScalar(const GpbElementInfo *msg_info_ptr)
{
	return((msg_info_ptr) ? IsGpbTypeRepeatedScalar(*msg_info_ptr) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedScalar(const GpbElementInfoSPtr &msg_info_ptr)
{
	return(IsGpbTypeRepeatedScalar(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedScalar(const GpbElementInfoUPtr &msg_info_ptr)
{
	return(IsGpbTypeRepeatedScalar(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(GpbDatumType datum_type)
{
	return(datum_type == GpbDatumType_Repeated);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(
	::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label)
{
	return(IsGpbTypeSimple(cpp_type) &&
			(label == ::google::protobuf::FieldDescriptor::LABEL_REPEATED));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(
	const ::google::protobuf::FieldDescriptor &field_descriptor)
{
	return(IsGpbTypeRepeatedSimple(field_descriptor.cpp_type(),
		field_descriptor.label()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(
	const ::google::protobuf::FieldDescriptor *field_descriptor)
{
	return((field_descriptor) ? IsGpbTypeRepeatedSimple(*field_descriptor) :
		false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(const GpbElementInfo &msg_info)
{
	return(IsGpbTypeRepeatedSimple(msg_info.GetFieldDescriptor()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(const GpbElementInfo *msg_info_ptr)
{
	return((msg_info_ptr) ? IsGpbTypeRepeatedSimple(*msg_info_ptr) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(const GpbElementInfoSPtr &msg_info_ptr)
{
	return(IsGpbTypeRepeatedSimple(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(const GpbElementInfoUPtr &msg_info_ptr)
{
	return(IsGpbTypeRepeatedSimple(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(GpbDatumType datum_type)
{
	return(datum_type == GpbDatumType_RepeatedPtr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label)
{
	return(IsGpbTypePtr(cpp_type) &&
			(label == ::google::protobuf::FieldDescriptor::LABEL_REPEATED));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(
	const ::google::protobuf::FieldDescriptor &field_descriptor)
{
	return(IsGpbTypeRepeatedPtr(field_descriptor.cpp_type(),
		field_descriptor.label()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(
	const ::google::protobuf::FieldDescriptor *field_descriptor)
{
	return((field_descriptor) ? IsGpbTypeRepeatedPtr(*field_descriptor) :
		false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(const GpbElementInfo &msg_info)
{
	return(IsGpbTypeRepeatedPtr(msg_info.GetFieldDescriptor()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(const GpbElementInfo *msg_info_ptr)
{
	return((msg_info_ptr) ? IsGpbTypeRepeatedPtr(*msg_info_ptr) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(const GpbElementInfoSPtr &msg_info_ptr)
{
	return(IsGpbTypeRepeatedPtr(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(const GpbElementInfoUPtr &msg_info_ptr)
{
	return(IsGpbTypeRepeatedPtr(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(GpbDatumType datum_type)
{
	return(IsGpbTypeRepeatedPtr(datum_type) ||
		IsGpbTypeRepeatedScalar(datum_type));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(
	::google::protobuf::FieldDescriptor::Label label)
{
	return(label == ::google::protobuf::FieldDescriptor::LABEL_REPEATED);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label)
{
	return(IsGpbTypeRepeatedScalar(cpp_type, label) ||
		IsGpbTypeRepeatedPtr(cpp_type, label));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(
	const ::google::protobuf::FieldDescriptor &field_descriptor)
{
	return(IsGpbTypeRepeated(field_descriptor.cpp_type(),
		field_descriptor.label()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(
	const ::google::protobuf::FieldDescriptor *field_descriptor)
{
	return((field_descriptor) ? IsGpbTypeRepeated(*field_descriptor) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(const GpbElementInfo &msg_info)
{
	return(IsGpbTypeRepeated(msg_info.GetFieldDescriptor()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(const GpbElementInfo *msg_info_ptr)
{
	return((msg_info_ptr) ? IsGpbTypeRepeated(*msg_info_ptr) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(const GpbElementInfoSPtr &msg_info_ptr)
{
	return(IsGpbTypeRepeated(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(const GpbElementInfoUPtr &msg_info_ptr)
{
	return(IsGpbTypeRepeated(msg_info_ptr.get()));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#ifdef TEST_MAIN

#include <ProtoBuf/GpbElementInfo.hpp>

#include <ProtoBuf/CppType2Type.hpp>
#include <ProtoBuf/Type2CppType.hpp>

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

namespace TEST_IsGpbType {

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitElement(const MLB::ProtoBuf::GpbElementInfo &datum)
{
	std::cout
		<< std::setw(3) << datum.GetDepth() << " "
		<< ((IsGpbTypeScalar(datum))         ? "s -" : "  -")
		<< ((IsGpbTypeSimple(datum))         ? "S -" : "  -")
		<< ((IsGpbTypePtr(datum))            ? "P -" : "  -")
		<< ((IsGpbTypeRepeatedScalar(datum)) ? "Rs-" : "  -")
		<< ((IsGpbTypeRepeatedSimple(datum)) ? "RS-" : "  -")
		<< ((IsGpbTypeRepeatedPtr(datum))    ? "RP-" : "  -")
		<< ((IsGpbTypeRepeated(datum))       ? "R -" : "  -")
		<< std::left << " "
		<< std::setw(20) << datum.GetTypeName() << " "
		<< std::setw(20) << datum.GetName()     << std::right << "\n";

		for (std::size_t count_1 = 0; count_1 < datum.GetMemberList().size();
			++count_1)
			TEST_EmitElement(datum.GetMemberList()[count_1]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TEST_RunTestForMessage(int &return_code, const char *message_name)
{
	TEST_EmitSep('=');
	TEST_EmitSep('=');

	std::cout << "TEST_IsGpbType: " << message_name << ":" << std::endl;

	bool test_passed = true;

	try {
		MLB::ProtoBuf::GpbElementInfo element(message_name);
		TEST_EmitSep('-');
		TEST_EmitElement(element);
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

} // namespace TEST_IsGpbType

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_IsGpbType::RunTest(return_code);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN
