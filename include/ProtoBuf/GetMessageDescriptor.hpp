//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GetMessageDescriptor.hpp

	File Description	:	Include file for functions used to determine, given a
								string, the Google ProtoBuf message descriptor.

	Revision History	:	2016-06-05 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__GetMessageDescriptor_hpp___HH

#define HH___MLB__ProtoBuf__GetMessageDescriptor_hpp___HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <string>

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4100 4512)
#endif // #ifdef _MSC_VER

#include <google/protobuf/descriptor.pb.h>

#ifdef _MSC_VER
# pragma warning(pop)
#endif // #ifdef _MSC_VER

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
const ::google::protobuf::Descriptor *GetMessageDescriptor(
	const std::string &in_name,
	const ::google::protobuf::DescriptorPool *descriptor_pool,
	bool throw_if_not_found = true);
const ::google::protobuf::Descriptor *GetMessageDescriptor(
	const std::string &in_name, bool throw_if_not_found = true);
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__GetMessageDescriptor_hpp___HH

