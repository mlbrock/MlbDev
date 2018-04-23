//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GpbMessageConverter.cpp

	File Description	:	Implementation of the GpbMessageConverter class.

	Revision History	:	2015-07-04 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include "GpbElementInfo.hpp"

#include <iomanip>
#include <sstream>

#include <boost/algorithm/string.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

/*
//	////////////////////////////////////////////////////////////////////////////
typedef std::pair<GpbElementInfo, GpbElementInfo> GpbElementInfoPair;
typedef std::vector<GpbElementInfoPair>           GpbElementInfoPairVector;
typedef GpbElementInfoPairVector::iterator        GpbElementInfoPairVectorIter;
typedef GpbElementInfoPairVector::const_iterator  GpbElementInfoPairVectorIterC;
//	////////////////////////////////////////////////////////////////////////////
*/

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct GbpConvertDatum {
/*
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
		GpbDatumType_Count       =
			(GpbDatumType_Maximum - GpbDatumType_Minimum) + 1
	};
*/
	GbpConvertDatum();

	explicit GbpConvertDatum(const GpbElementInfoPair &intersect_pair);

	inline void operator () (std::size_t src_index, std::size_t dst_index,
		const ::google::protobuf::Reflection *src_reflection,
		const ::google::protobuf::Reflection *dst_reflection,
		const ::google::protobuf::Message &src_msg,
		::google::protobuf::Message &dst_msg) const
	{
		switch (datum_type_) {
			case GpbDatumType_SInt32		:
				dst_reflection->SetInt32(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetInt32(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_SInt64		:
				dst_reflection->SetInt64(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetInt64(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_UInt32		:
				dst_reflection->SetUInt32(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetUInt32(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_UInt64		:
				dst_reflection->SetUInt64(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetUInt64(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_Double		:
				dst_reflection->SetDouble(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetDouble(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_Float			:
				dst_reflection->SetFloat(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetFloat(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_Bool			:
				dst_reflection->SetBool(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetBool(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_Enum			:
				dst_reflection->SetEnum(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetEnum(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_String		:
				dst_reflection->SetString(&dst_msg, descriptors_list_[dst_index],
					src_reflection->GetString(src_msg, descriptors_list_[src_index]));
				break;
			case GpbDatumType_Message		:
				dst_reflection->MutableMessage(&dst_msg,
					descriptors_list_[dst_index])->
					CopyFrom(src_reflection->GetMessage(src_msg,
					descriptors_list_[src_index]));
				break;
			case GpbDatumType_Repeated		:
				//	Munged.
				dst_reflection->MutableRepeatedField<int>(&dst_msg,
					descriptors_list_[dst_index])->
					CopyFrom(src_reflection->GetRepeatedField<int>(src_msg,
					descriptors_list_[src_index]));
				break;
			case GpbDatumType_RepeatedPtr	:
				/*
					Also munged, but won't work because method invokes
					protected method MutableRawRepeated[String|Message]() which
					needs a pointer to the Descriptor for the type.

					Will get around it --- preferably by inheritance using my
					own derived class to get access. Or by the #define trick.
				*/
				dst_reflection->MutableRepeatedPtrField<std::string>(&dst_msg,
					descriptors_list_[dst_index])->
					CopyFrom(src_reflection->GetRepeatedPtrField<std::string>(src_msg,
					descriptors_list_[src_index]));
				break;
			default								:
				{
					std::ostringstream o_str;
					o_str << "Unknown/invalid GpbElementInfo datum type "
						"encountered (" << datum_type_ << ") --- this is an "
						"internal error.";
/*
					CODE NOTE: Should use MLB::Utility
					MLB::Utility::ThrowRunTimeError(o_str.str());
*/
					throw std::runtime_error(o_str.str());
				}
				break;
		}
	}

	GpbElementInfoPair                         element_pair_;
	GpbDatumType                               datum_type_;
	const ::google::protobuf::FieldDescriptor *descriptors_list_[2];
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::vector<GbpConvertDatum>          GbpConvertDatumVector;
typedef GbpConvertDatumVector::iterator       GbpConvertDatumVectorIter;
typedef GbpConvertDatumVector::const_iterator GbpConvertDatumVectorIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class GpbMessageConverter {
public:
	typedef ::google::protobuf::Message         GPB_Message;
	typedef ::google::protobuf::FieldDescriptor GPB_FieldDescriptor;

	GpbMessageConverter();

	GpbMessageConverter(const ::google::protobuf::Message &lhs,
		const ::google::protobuf::Message &rhs);

	inline void ConvertMessage(const GPB_Message &src_msg,
		GPB_Message &dst_msg) const
	{
		std::size_t src_index = ResolveReflection(src_msg, dst_msg);
		std::size_t dst_index = (src_index + 1) % 2;

		GbpConvertDatumVectorIterC iter_b(data_vector_.begin());
		GbpConvertDatumVectorIterC iter_e(data_vector_.end());

		for ( ; iter_b != iter_e; ++iter_b)
			(*iter_b)(src_index, dst_index, reflection_list_[src_index],
				reflection_list_[dst_index], src_msg, dst_msg);
	}

	void swap(GpbMessageConverter &other);

private:
	const ::google::protobuf::Reflection *reflection_list_[2];
	GbpConvertDatumVector                 data_vector_;

	inline std::size_t ResolveReflection(const GPB_Message &src_msg,
		const GPB_Message &dst_msg) const
	{
		if ((src_msg.GetReflection() == reflection_list_[0]) &&
			 (dst_msg.GetReflection() == reflection_list_[1]))
			return(0);
		else if ((src_msg.GetReflection() != reflection_list_[1]) ||
					(dst_msg.GetReflection() != reflection_list_[0])) {
			std::ostringstream o_str;
			o_str << "Unable to map both he source message '" <<
				src_msg.GetTypeName() << "' and the destination message '" <<
				dst_msg.GetTypeName() << "'to the configured conversion types.";
/*
			CODE NOTE: Should use MLB::Utility
			MLB::Utility::ThrowInvalidArgument(o_str.str());
*/
			throw std::invalid_argument(o_str.str());
		}

		return(1);
	}

	GpbMessageConverter(const ::google::protobuf::Reflection *lhs,
		const ::google::protobuf::Reflection *rhs,
		const GpbElementInfoPairVector &intersect_list);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
GbpConvertDatum::GbpConvertDatum()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GbpConvertDatum::GbpConvertDatum(const GpbElementInfoPair &intersect_pair)
	:element_pair_(intersect_pair)
	,datum_type_(intersect_pair.first.GetDatumType())
{
	descriptors_list_[0] = intersect_pair.first.GetFieldDescriptor();
	descriptors_list_[1] = intersect_pair.second.GetFieldDescriptor();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbMessageConverter::GpbMessageConverter()
	:data_vector_()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbMessageConverter::GpbMessageConverter(const ::google::protobuf::Message &lhs,
	const ::google::protobuf::Message &rhs)
	:data_vector_()
{
/*
	CODE NOTE: Should use MLB::Utility
	MLB::Utility::ThrowIfNull(lhs, "The 'lhs' parameter")
	MLB::Utility::ThrowIfNull(rhs, "The 'rhs' parameter")
*/

/*
	CODE NOTE: Should use MLB::Utility
	if (lhs == rhs)
		MLB::Utility::ThrowInvalidArgument("The 'lhs' parameter is equal to "
			"the 'rhs' parameter.");
*/
	if ((&lhs == &rhs) || (lhs.GetTypeName() == rhs.GetTypeName()))
		throw std::invalid_argument("The 'lhs' parameter is equal to "
			"the 'rhs' parameter.");

	GpbElementInfo lhs_info(lhs.GetTypeName());
	GpbElementInfo rhs_info(rhs.GetTypeName());

	GpbMessageConverter(lhs.GetReflection(), rhs.GetReflection(),
		lhs_info.SetIntersectionPair(rhs_info)).swap(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbMessageConverter::GpbMessageConverter(
	const ::google::protobuf::Reflection *lhs,
	const ::google::protobuf::Reflection *rhs,
	const GpbElementInfoPairVector &intersect_list)
	:data_vector_(intersect_list.size())
{
	reflection_list_[0] = lhs;
	reflection_list_[1] = rhs;

	for (std::size_t count_1 = 0; count_1 < intersect_list.size(); ++count_1)
		data_vector_[count_1] = GbpConvertDatum(intersect_list[count_1]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GpbMessageConverter::swap(GpbMessageConverter &other)
{
	std::swap(reflection_list_[0], other.reflection_list_[0]);
	std::swap(reflection_list_[1], other.reflection_list_[1]);
	std::swap(data_vector_,        other.data_vector_);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB



#ifdef TEST_MAIN

#include <cstdlib>
#include <iostream>

#include <boost/io/ios_state.hpp>

#include "Tests/CppOut/GpbElementInfoTestThree.pb.h"

namespace {

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitSep(char sep_char, std::streamsize sep_width = 79,
	std::ostream &o_str = std::cout)
{
	boost::io::ios_all_saver io_state(o_str);

	o_str << std::setfill(sep_char) << std::setw(sep_width) << "" << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitDatum(const MLB::ProtoBuf::GpbElementInfo &datum)
{
#if 1
	std::cout << datum << std::endl;
#else
	datum.TestFileName();
	std::cout << std::endl;
#endif // #if 1
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_Compare_HelperInner(
	const MLB::ProtoBuf::GpbElementInfo &element_1,
	const MLB::ProtoBuf::GpbElementInfo &element_2,
	MLB::ProtoBuf::GpbElementInfoVector &inte_list,
	MLB::ProtoBuf::GpbElementInfoVector &diff_list)
{
	inte_list = element_1.SetIntersection(element_2);
	diff_list = element_1.SetDifference(element_2);

	std::cout << "Source     : " << element_1.GetTypeNameFull() << std::endl;
	std::cout << "Destination: " << element_2.GetTypeNameFull() << std::endl;
	TEST_EmitSep('-');
	std::cout << "Same in source and in destination: " << inte_list.size() <<
		std::endl;
	std::copy(inte_list.begin(), inte_list.end(), std::ostream_iterator<
		MLB::ProtoBuf::GpbElementInfoVector::value_type>(
		std::cout, "\n"));
	TEST_EmitSep('-');
	std::cout << "In source, not in destination    : " << diff_list.size() <<
		std::endl;
	std::copy(diff_list.begin(), diff_list.end(), std::ostream_iterator<
		MLB::ProtoBuf::GpbElementInfoVector::value_type>(
		std::cout, "\n"));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_Compare_Helper(
	const MLB::ProtoBuf::GpbElementInfo &element_1,
	const MLB::ProtoBuf::GpbElementInfo &element_2,
	MLB::ProtoBuf::GpbElementInfoVector inte_lists[2],
	MLB::ProtoBuf::GpbElementInfoVector diff_lists[2])
{
	TEST_Compare_HelperInner(element_1, element_2, inte_lists[0],
		diff_lists[0]);
	TEST_EmitSep('=');
	TEST_Compare_HelperInner(element_2, element_1, inte_lists[1],
		diff_lists[1]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TEST_Compare(int &return_code, const std::string &message_name_1,
	const std::string &message_name_2)
{
	TEST_EmitSep('=');
	TEST_EmitSep('=');
	std::cout << "TEST of conversion logic generation:" << std::endl;
	std::cout << "   From: " << message_name_1 << std::endl;
	std::cout << "   To  : " << message_name_2 << std::endl;
	TEST_EmitSep('=');

	bool test_passed = true;

	try {
		MLB::ProtoBuf::GpbElementInfo       element_1(message_name_1);
		MLB::ProtoBuf::GpbElementInfo       element_2(message_name_2);
		MLB::ProtoBuf::GpbElementInfoVector inte_lists[2];
		MLB::ProtoBuf::GpbElementInfoVector diff_lists[2];
		TEST_Compare_Helper(element_1, element_2, inte_lists, diff_lists);
	}
	catch (const std::exception &except) {
		std::cerr << "TEST FAILED: " << except.what() << std::endl;
		test_passed = false;
	}

	TEST_EmitSep('=');
	std::cout << std::endl;

	if (!test_passed)
		return_code = EXIT_FAILURE;

	return(test_passed);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TEST_RunTest_1(int &return_code, const std::string &message_name_1,
	const std::string &message_name_2)
{
	return(TEST_Compare(return_code, message_name_1, message_name_2));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TEST_RunTest_2(int &return_code)
{
	GpbElementInfoTestThree::PersonOne msg_1;
	GpbElementInfoTestThree::PersonTwo msg_2;

	bool test_passed =
		TEST_Compare(return_code, msg_1.GetTypeName(), msg_2.GetTypeName());

	if (!test_passed)
		return(false);

	try {
		MLB::ProtoBuf::GpbMessageConverter msg_converter(msg_1, msg_2);
		GpbElementInfoTestThree::PhoneNumber phone;
		//	Message 1
		msg_1.set_name("John D. One");
		msg_1.set_id(42);
		msg_1.set_email("John.D.One@yahoo.com");
		phone.set_number("+1 201-111-0001");
		phone.set_type(GpbElementInfoTestThree::PhoneType::HOME);
		*msg_1.add_phone() = phone;
		msg_1.set_different_datum(
			std::numeric_limits<::google::protobuf::int32>::max());
		msg_1.set_name_different_1(1);
		//	Message 2
		msg_2.set_name("Jane R. Two");
		msg_2.set_id(666);
		msg_2.set_email("Jane.R.Two@gmail.com");
		phone.set_number("+1 201-222-0001");
		phone.set_type(GpbElementInfoTestThree::PhoneType::HOME);
		*msg_2.add_phone() = phone;
		phone.set_number("+1 201-222-0002");
		phone.set_type(GpbElementInfoTestThree::PhoneType::WORK);
		*msg_2.add_phone() = phone;
		msg_2.set_different_datum(
			std::numeric_limits<::google::protobuf::uint32>::max());
		msg_2.set_name_different_2(2);
		// We'll work with copies...
		{
			GpbElementInfoTestThree::PersonOne test_msg_1 = msg_1;
			GpbElementInfoTestThree::PersonTwo test_msg_2; // Empty
			msg_converter.ConvertMessage(test_msg_1, test_msg_2);
		}
		{
			GpbElementInfoTestThree::PersonOne test_msg_1; // Empty
			GpbElementInfoTestThree::PersonTwo test_msg_2 = msg_2;
			msg_converter.ConvertMessage(test_msg_2, test_msg_1);
		}
	}
	catch (const std::exception &except) {
		std::cerr << "TEST FAILED: Attempt to test the Google Protocol Buffer "
			"message converted failed: " << except.what() << std::endl;
		test_passed = false;
	}

	return(test_passed);
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_RunTest_1(return_code,
			"GpbElementInfoTestThree::PersonOne",
			"GpbElementInfoTestThree::PersonTwo");
		TEST_RunTest_2(return_code);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

