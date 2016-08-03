//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GpbMessageTypes.cpp

	File Description	:	Logic to help ensure that library messages are linked.

	Revision History	:	2016-07-26 --- Creation.
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

#include <ProtoBuf/GpbMessageTypes.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

namespace {

//	////////////////////////////////////////////////////////////////////////////
struct Ensure_Mlb_ProtoBuf_Message_Types_Linked {
	Ensure_Mlb_ProtoBuf_Message_Types_Linked()
		:msg_list_()
	{
		std::vector<GpbMessageSPtr> msgs;

		msgs.push_back(GpbMessageSPtr(new ArraySInt32));
		msgs.push_back(GpbMessageSPtr(new ArrayInt32));
		msgs.push_back(GpbMessageSPtr(new ArrayUInt32));
		msgs.push_back(GpbMessageSPtr(new ArraySInt64));
		msgs.push_back(GpbMessageSPtr(new ArrayInt64));
		msgs.push_back(GpbMessageSPtr(new ArrayUInt64));
		msgs.push_back(GpbMessageSPtr(new ArrayFloat32));
		msgs.push_back(GpbMessageSPtr(new ArrayFloat64));
		msgs.push_back(GpbMessageSPtr(new ArrayFloat));
		msgs.push_back(GpbMessageSPtr(new ArrayDouble));
		msgs.push_back(GpbMessageSPtr(new ArrayBool));
		msgs.push_back(GpbMessageSPtr(new ArrayString));

		msgs.push_back(GpbMessageSPtr(new ArrayOfArraySInt32));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayInt32));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayUInt32));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArraySInt64));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayInt64));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayUInt64));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayFloat32));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayFloat64));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayFloat));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayDouble));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayBool));
		msgs.push_back(GpbMessageSPtr(new ArrayOfArrayString));

		msgs.push_back(GpbMessageSPtr(new Array1SInt32));
		msgs.push_back(GpbMessageSPtr(new Array1Int32));
		msgs.push_back(GpbMessageSPtr(new Array1UInt32));
		msgs.push_back(GpbMessageSPtr(new Array1SInt64));
		msgs.push_back(GpbMessageSPtr(new Array1Int64));
		msgs.push_back(GpbMessageSPtr(new Array1UInt64));
		msgs.push_back(GpbMessageSPtr(new Array1Float32));
		msgs.push_back(GpbMessageSPtr(new Array1Float64));
		msgs.push_back(GpbMessageSPtr(new Array1Float));
		msgs.push_back(GpbMessageSPtr(new Array1Double));
		msgs.push_back(GpbMessageSPtr(new Array1Bool));
		msgs.push_back(GpbMessageSPtr(new Array1String));

		msgs.push_back(GpbMessageSPtr(new Array2SInt32));
		msgs.push_back(GpbMessageSPtr(new Array2Int32));
		msgs.push_back(GpbMessageSPtr(new Array2UInt32));
		msgs.push_back(GpbMessageSPtr(new Array2SInt64));
		msgs.push_back(GpbMessageSPtr(new Array2Int64));
		msgs.push_back(GpbMessageSPtr(new Array2UInt64));
		msgs.push_back(GpbMessageSPtr(new Array2Float32));
		msgs.push_back(GpbMessageSPtr(new Array2Float64));
		msgs.push_back(GpbMessageSPtr(new Array2Float));
		msgs.push_back(GpbMessageSPtr(new Array2Double));
		msgs.push_back(GpbMessageSPtr(new Array2Bool));
		msgs.push_back(GpbMessageSPtr(new Array2String));

		msgs.push_back(GpbMessageSPtr(new FieldTypeTest));
		msgs.push_back(GpbMessageSPtr(new FieldTypeTest::BigCurrencyPosition));

		msg_list_.swap(msgs);
	}

	std::vector<GpbMessageSPtr> GetTypesList() const
	{
		return(msg_list_);
	}

	std::vector<std::string> GetNamesList() const
	{
		std::vector<std::string> name_list;

		name_list.reserve(msg_list_.size());

		for (std::size_t count_1 =  0; count_1 < msg_list_.size(); ++count_1)
			name_list.push_back(msg_list_[count_1]->GetTypeName());

		return(name_list);
	}

private:
	std::vector<GpbMessageSPtr> msg_list_;
} Ensure_Mlb_ProtoBuf_Message_Types_Linked_INSTANCE;
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
std::vector<GpbMessageSPtr> GetGpbMessageTypesList()
{
	return(Ensure_Mlb_ProtoBuf_Message_Types_Linked_INSTANCE.GetTypesList());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> GetGpbMessageNamesList()
{
	return(Ensure_Mlb_ProtoBuf_Message_Types_Linked_INSTANCE.GetNamesList());
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

