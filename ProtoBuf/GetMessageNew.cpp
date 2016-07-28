//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GetMessageNew.cpp

	File Description	:	Implementation of logic to create a copy of the
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

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/GetMessageNew.hpp>
#include <ProtoBuf/GetMessagePrototype.hpp>
#include <ProtoBuf/GetMessageDescriptor.hpp>

#include <Utility/C_StringSupport.hpp>

#include <sstream>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
::google::protobuf::Message *GetMessageNewPtr(
	const ::google::protobuf::Descriptor *descriptor_ptr,
	::google::protobuf::MessageFactory *msg_factory_ptr)
{
	MLB::Utility::ThrowIfNull(descriptor_ptr, "The pointer to the '::google::"
		"protobuf::Descriptor' passed to GetMessageNewPtr() is NULL.");

	::google::protobuf::Message *msg_ptr = NULL;

	try {
		msg_ptr = GetMessagePrototype(descriptor_ptr, msg_factory_ptr)->New();
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Unable to create a new Google ProtoBuf message of type '" <<
			descriptor_ptr->name() << "' from its prototypical form: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(msg_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
::google::protobuf::Message *GetMessageNewPtr(const std::string &msg_name,
	::google::protobuf::MessageFactory *msg_factory_ptr)
{
	return(GetMessageNewPtr(GetMessageDescriptor(msg_name, true),
		msg_factory_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbMessageSPtr GetMessageNewSPtr(
	const ::google::protobuf::Descriptor *descriptor_ptr,
	::google::protobuf::MessageFactory *msg_factory_ptr)
{
	return(GpbMessageSPtr(GetMessageNewPtr(descriptor_ptr, msg_factory_ptr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbMessageSPtr GetMessageNewSPtr(const std::string &msg_name,
	::google::protobuf::MessageFactory *msg_factory_ptr)
{
	return(GpbMessageSPtr(GetMessageNewPtr(msg_name, msg_factory_ptr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbMessageUPtr GetMessageNewUPtr(
	const ::google::protobuf::Descriptor *descriptor_ptr,
	::google::protobuf::MessageFactory *msg_factory_ptr)
{
	return(GpbMessageUPtr(GetMessageNewPtr(descriptor_ptr, msg_factory_ptr)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbMessageUPtr GetMessageNewUPtr(const std::string &msg_name,
	::google::protobuf::MessageFactory *msg_factory_ptr)
{
	return(GpbMessageUPtr(GetMessageNewPtr(msg_name, msg_factory_ptr)));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB
