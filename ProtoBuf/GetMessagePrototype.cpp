//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GetMessagePrototype.cpp

	File Description	:	Implementation of logic to retrieve the prototype
								instance of a Google ProtoBuf message by name.

	Revision History	:	2016-06-06 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/GetMessagePrototype.hpp>
#include <ProtoBuf/GetMessageDescriptor.hpp>

#include <Utility/C_StringSupport.hpp>

#include <sstream>

#include <boost/algorithm/string.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
const ::google::protobuf::Message *GetMessagePrototype(
	const ::google::protobuf::Descriptor *descriptor_ptr,
	::google::protobuf::MessageFactory *msg_factory_ptr)
{
	MLB::Utility::ThrowIfNull(descriptor_ptr, "The pointer to the '::google::"
		"protobuf::Descriptor' passed to GetMessagePrototype() is NULL.");

	return(MLB::Utility::ThrowIfNull(((msg_factory_ptr) ? msg_factory_ptr :
		::google::protobuf::MessageFactory::generated_factory())->
		GetPrototype(descriptor_ptr),
		"An instance of the MessageFactory class or one of its descendent "
		"classes GetPrototype() method returned NULL."));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const ::google::protobuf::Message *GetMessagePrototype(
	const std::string &msg_name,
	::google::protobuf::MessageFactory *msg_factory_ptr)
{
	return(GetMessagePrototype(GetMessageDescriptor(msg_name, true),
		msg_factory_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

