//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	IsGpbType.hpp

	File Description	:	Prototypes for the IsGpbType functions.

	Revision History	:	2016-07-05 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__IsGpbType_hpp___HH

#define HH___MLB__ProtoBuf__IsGpbType_hpp___HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/GpbElementInfo.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedPtr(GpbDatumType datum_type);
bool IsGpbTypeRepeatedPtr(::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label);
bool IsGpbTypeRepeatedPtr(
	const ::google::protobuf::FieldDescriptor &field_descriptor);
bool IsGpbTypeRepeatedPtr(
	const ::google::protobuf::FieldDescriptor *field_descriptor);
bool IsGpbTypeRepeatedPtr(const GpbElementInfo &msg_info);
bool IsGpbTypeRepeatedPtr(const GpbElementInfo *msg_info_ptr);
bool IsGpbTypeRepeatedPtr(const GpbElementInfoSPtr &msg_info_ptr);
bool IsGpbTypeRepeatedPtr(const GpbElementInfoUPtr &msg_info_ptr);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeatedSimple(GpbDatumType datum_type);
bool IsGpbTypeRepeatedSimple(
	::google::protobuf::FieldDescriptor::CppType cpp_type,
	::google::protobuf::FieldDescriptor::Label label);
bool IsGpbTypeRepeatedSimple(
	const ::google::protobuf::FieldDescriptor &field_descriptor);
bool IsGpbTypeRepeatedSimple(
	const ::google::protobuf::FieldDescriptor *field_descriptor);
bool IsGpbTypeRepeatedSimple(const GpbElementInfo &msg_info);
bool IsGpbTypeRepeatedSimple(const GpbElementInfo *msg_info_ptr);
bool IsGpbTypeRepeatedSimple(const GpbElementInfoSPtr &msg_info_ptr);
bool IsGpbTypeRepeatedSimple(const GpbElementInfoUPtr &msg_info_ptr);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeRepeated(GpbDatumType datum_type);
bool IsGpbTypeRepeated(::google::protobuf::FieldDescriptor::Label label);
//	IMPLEMENTATION NOTE: For parallelism.
bool IsGpbTypeRepeated(::google::protobuf::FieldDescriptor::Label label,
	::google::protobuf::FieldDescriptor::CppType /* cpp_type */);
bool IsGpbTypeRepeated(
	const ::google::protobuf::FieldDescriptor &field_descriptor);
bool IsGpbTypeRepeated(
	const ::google::protobuf::FieldDescriptor *field_descriptor);
bool IsGpbTypeRepeated(const GpbElementInfo &msg_info);
bool IsGpbTypeRepeated(const GpbElementInfo *msg_info_ptr);
bool IsGpbTypeRepeated(const GpbElementInfoSPtr &msg_info_ptr);
bool IsGpbTypeRepeated(const GpbElementInfoUPtr &msg_info_ptr);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeSimple(GpbDatumType datum_type);
bool IsGpbTypeSimple(::google::protobuf::FieldDescriptor::CppType cpp_type);

bool IsGpbTypeSimple(
	const ::google::protobuf::FieldDescriptor &field_descriptor);
bool IsGpbTypeSimple(
	const ::google::protobuf::FieldDescriptor *field_descriptor);
bool IsGpbTypeSimple(const GpbElementInfo &msg_info);
bool IsGpbTypeSimple(const GpbElementInfo *msg_info_ptr);
bool IsGpbTypeSimple(const GpbElementInfoSPtr &msg_info_ptr);
bool IsGpbTypeSimple(const GpbElementInfoUPtr &msg_info_ptr);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsGpbTypeScalar(GpbDatumType datum_type);
bool IsGpbTypeScalar(::google::protobuf::FieldDescriptor::CppType cpp_type);
bool IsGpbTypeScalar(
	const ::google::protobuf::FieldDescriptor &field_descriptor);
bool IsGpbTypeScalar(
	const ::google::protobuf::FieldDescriptor *field_descriptor);
bool IsGpbTypeScalar(const GpbElementInfo &msg_info);
bool IsGpbTypeScalar(const GpbElementInfo *msg_info_ptr);
bool IsGpbTypeScalar(const GpbElementInfoSPtr &msg_info_ptr);
bool IsGpbTypeScalar(const GpbElementInfoUPtr &msg_info_ptr);
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__IsGpbType_hpp___HH

