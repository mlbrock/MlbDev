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

#include <ProtoBuf/ProtoBuf.hpp>

#include <string>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
const ::google::protobuf::Descriptor *GetMessageDescriptor(
	const std::string &msg_name,
	const ::google::protobuf::DescriptorPool *descriptor_pool_ptr,
	bool throw_if_not_found = true);
const ::google::protobuf::Descriptor *GetMessageDescriptor(
	const std::string &msg_name, bool throw_if_not_found = true);
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__GetMessageDescriptor_hpp___HH

