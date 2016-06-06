//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GetMessagePrototype.hpp

	File Description	:	Include file for logic to retrieve the prototype
								instance of a Google ProtoBuf message by name.

	Revision History	:	2016-06-06 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__GetMessagePrototype_hpp___HH

#define HH___MLB__ProtoBuf__GetMessagePrototype_hpp___HH	1

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
const ::google::protobuf::Message *GetMessagePrototype(
	const std::string &msg_name,
	::google::protobuf::MessageFactory *msg_factory_ptr);
const ::google::protobuf::Message *GetMessagePrototype(
	const std::string &msg_name);
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__GetMessagePrototype_hpp___HH
