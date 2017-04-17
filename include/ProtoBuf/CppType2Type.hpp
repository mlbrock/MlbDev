//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	CppType2Type.hpp

	File Description	:	Definition of the CppType2Type class template.

	Revision History	:	2016-07-26 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__CppType2Type_hpp___HH

#define HH___MLB__ProtoBuf__CppType2Type_hpp___HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/ProtoBuf.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
template < ::google::protobuf::FieldDescriptor::CppType>
	struct CppType2Type;
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_INT32> {
	typedef Native_SInt32                                     type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_INT32;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_INT64> {
	typedef Native_SInt64                                     type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_INT64;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_UINT32> {
	typedef Native_UInt32                                     type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_UINT32;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_UINT64> {
	typedef Native_UInt64                                     type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_UINT64;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE> {
	typedef Native_Float64                                    type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT> {
	typedef Native_Float32                                    type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL> {
	typedef bool                                              type;
	typedef ::google::protobuf::RepeatedField<type>           repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_BOOL;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_ENUM> {
	typedef ::google::protobuf::EnumValueDescriptor           type; 
	typedef ::google::protobuf::RepeatedField<type>           repeated_type; 
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_ENUM;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_STRING> {
	typedef std::string                                       type;
	typedef ::google::protobuf::RepeatedPtrField<type>        repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_STRING;
};
//	----------------------------------------------------------------------------
template <> struct
	CppType2Type< ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE> {
	typedef ::google::protobuf::Message                       type;
	typedef ::google::protobuf::RepeatedPtrField<type>        repeated_type;
	static const ::google::protobuf::FieldDescriptor::CppType value =
		::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE;
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__CppType2Type_hpp___HH

