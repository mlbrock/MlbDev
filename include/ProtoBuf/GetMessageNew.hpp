//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GetMessageNew.hpp

	File Description	:	Include file for logic to create a copy of the
								prototype instance of a Google ProtoBuf message by
								name.

	Revision History	:	2016-06-06 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__GetMessageNew_hpp___HH

#define HH___MLB__ProtoBuf__GetMessageNew_hpp___HH	1

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
::google::protobuf::Message *GetMessageNewPtr(
	const ::google::protobuf::Descriptor *descriptor_ptr,
	::google::protobuf::MessageFactory *msg_factory_ptr = NULL);
::google::protobuf::Message *GetMessageNewPtr(const std::string &msg_name,
	::google::protobuf::MessageFactory *msg_factory_ptr = NULL);

GpbMessageSPtr GetMessageNewSPtr(
	const ::google::protobuf::Descriptor *descriptor_ptr,
	::google::protobuf::MessageFactory *msg_factory_ptr = NULL);
GpbMessageSPtr GetMessageNewSPtr(const std::string &msg_name,
	::google::protobuf::MessageFactory *msg_factory_ptr = NULL);

GpbMessageUPtr GetMessageNewUPtr(
	const ::google::protobuf::Descriptor *descriptor_ptr,
	::google::protobuf::MessageFactory *msg_factory_ptr = NULL);
GpbMessageUPtr GetMessageNewUPtr(const std::string &msg_name,
	::google::protobuf::MessageFactory *msg_factory_ptr = NULL);
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__GetMessageNew_hpp___HH

