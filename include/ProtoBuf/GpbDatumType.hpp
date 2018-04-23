//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GpbDatumType.hpp

	File Description	:	Definition of the GpbDatumType class.

	Revision History	:	2015-06-20 --- Creation (in GpbElementInfo.hpp).
									Michael L. Brock
								2016-07-26 --- Moved into its own header file.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__GpbDatumType_hpp___HH

#define HH___MLB__ProtoBuf__GpbDatumType_hpp___HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/ProtoBuf.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
enum GpbDatumType {
	GpbDatumType_SInt32      = ::google::protobuf::FieldDescriptor::CPPTYPE_INT32,
	GpbDatumType_SInt64      = ::google::protobuf::FieldDescriptor::CPPTYPE_INT64,
	GpbDatumType_UInt32      = ::google::protobuf::FieldDescriptor::CPPTYPE_UINT32,
	GpbDatumType_UInt64      = ::google::protobuf::FieldDescriptor::CPPTYPE_UINT64,
	GpbDatumType_Double      = ::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE,
	GpbDatumType_Float       = ::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT,
	GpbDatumType_Bool        = ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL,
	GpbDatumType_Enum        = ::google::protobuf::FieldDescriptor::CPPTYPE_ENUM,
	GpbDatumType_String      = ::google::protobuf::FieldDescriptor::CPPTYPE_STRING,
	GpbDatumType_Message     = ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE,
	GpbDatumType_Repeated    = ::google::protobuf::FieldDescriptor::MAX_CPPTYPE + 1,
	GpbDatumType_RepeatedPtr,
	GpbDatumType_Minimum     = GpbDatumType_SInt32,
	GpbDatumType_Maximum     = GpbDatumType_RepeatedPtr,
	GpbDatumType_Count       = (GpbDatumType_Maximum - GpbDatumType_Minimum) + 1
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__GpbDatumType_hpp___HH

