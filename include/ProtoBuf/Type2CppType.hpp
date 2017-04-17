//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	Type2CppType.hpp

	File Description	:	Definition of the Type2CppType class template.

	Revision History	:	2016-07-26 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__Type2CppType_hpp___HH

#define HH___MLB__ProtoBuf__Type2CppType_hpp___HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/ProtoBuf.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
template <typename GpbType>
	struct Type2CppType;
//	----------------------------------------------------------------------------
template <> struct Type2CppType<Native_SInt32> {
	typedef Native_SInt32                                     type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_INT32;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType<Native_SInt64> {
	typedef Native_SInt64                                     type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_INT64;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType<Native_UInt32> {
	typedef Native_UInt32                                     type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_UINT32;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType<Native_UInt64> {
	typedef Native_UInt64                                     type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_UINT64;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType<Native_Float64> {
	typedef Native_Float64                                    type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType<Native_Float32> {
	typedef Native_Float32                                    type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType<bool> {
	typedef bool                                              type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_BOOL;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType< ::google::protobuf::EnumValueDescriptor> {
	typedef ::google::protobuf::EnumValueDescriptor           type; 
	typedef ::google::protobuf::RepeatedField<type>           repeated_type; 
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_ENUM;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType<std::string> {
	typedef std::string                                       type;
	typedef ::google::protobuf::RepeatedPtrField<type>        repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_STRING;
};
//	----------------------------------------------------------------------------
template <> struct Type2CppType< ::google::protobuf::Message> {
	typedef ::google::protobuf::Message                       type;
	typedef ::google::protobuf::RepeatedPtrField<type>        repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE;
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__Type2CppType_hpp___HH

