// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Field decode tests.

	Revision History	:	2008-11-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/ExcContext.hpp>
#include <VFast/VFastException.hpp>

#include <Utility/StringSupport.hpp>

#include <iostream>
#include <sstream>
#include <cctype>

#include <boost/shared_array.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
struct UT_FieldDecode_Test {
	//	Ctor for integers...
	template <typename ThisType>
	UT_FieldDecode_Test(const std::string &xml_string, unsigned int pmap_bits,
		DataType data_type, ThisType data_value, bool is_optional, bool is_null,
		std::size_t byte_length, const char *byte_data);
	//	Ctor for char arrays...
	UT_FieldDecode_Test(const std::string &xml_string, unsigned int pmap_bits,
		DataType data_type, std::size_t data_length, const char *data_value,
		bool is_optional, bool is_null, std::size_t byte_length,
		const char *byte_data);


	const StreamDatum *FieldDecode(DictValue &datum) const;

	std::string        xml_string_;
	unsigned int       pmap_bits_;
	DataType           data_type_;
	DictValue          value_datum_;
	bool               is_optional_;
	bool               is_null_;
	unsigned int       byte_length_;
	const StreamDatum *begin_ptr_;
	const StreamDatum *end_ptr_;

	static int RunTests(unsigned int test_count,
		const UT_FieldDecode_Test *test_list);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ThisType>
	UT_FieldDecode_Test::UT_FieldDecode_Test(const std::string &xml_string,
	unsigned int pmap_bits, DataType data_type, ThisType data_value,
	bool is_optional, bool is_null, std::size_t byte_length,
	const char *byte_data)
	:xml_string_(xml_string)
	,pmap_bits_(pmap_bits)
	,data_type_(data_type)
	,value_datum_(data_type, is_null)
	,is_optional_(is_optional)
	,is_null_(is_null)
	,byte_length_(static_cast<unsigned int>(byte_length))
	,begin_ptr_(reinterpret_cast<const StreamDatum *>(byte_data))
	,end_ptr_(reinterpret_cast<const StreamDatum *>(byte_data + byte_length))
{
	if (!value_datum_.IsTypeNumeric())
		MLB::Utility::ThrowInvalidArgument("Encountered an invalid data "
			"type (" + DataTypeToString(data_type) + ") --- expected one of "
			"the numeric types.");

	value_datum_.data_type_ = data_type;

	value_datum_.SetValue(data_value);

	if (is_null)
		value_datum_.SetNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
UT_FieldDecode_Test::UT_FieldDecode_Test(const std::string &xml_string,
	 unsigned int pmap_bits, DataType data_type, std::size_t data_length,
	const char *data_value, bool is_optional, bool is_null,
	std::size_t byte_length, const char *byte_data)
	:xml_string_(xml_string)
	,pmap_bits_(pmap_bits)
	,data_type_(data_type)
	,value_datum_(static_cast<unsigned int>(data_length), data_value, data_type)
	,is_optional_(is_optional)
	,is_null_(is_null)
	,byte_length_(static_cast<unsigned int>(byte_length))
	,begin_ptr_(reinterpret_cast<const StreamDatum *>(byte_data))
	,end_ptr_(reinterpret_cast<const StreamDatum *>(byte_data + byte_length))
{
	if (!value_datum_.IsTypeCharArray())
		MLB::Utility::ThrowInvalidArgument("Encountered an invalid data "
			"type (" + DataTypeToString(data_type) + ") --- expected one of "
			"character array types.");

	value_datum_.data_type_ = data_type;

	value_datum_.SetValue(static_cast<unsigned int>(data_length), data_value);

	if (is_null)
		value_datum_.SetNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const StreamDatum *UT_FieldDecode_Test::FieldDecode(
	DictValue &datum) const
{
	std::string                      xml_string(
		"<template name=\"TestFieldDecode\" id=\"666\">" + xml_string_ +
		"</template>");
	InsContext                       ins_context(true, xml_string);
	ExcContext                       exc_context(ins_context);
	unsigned int                     byte_length = 1 + 2 + byte_length_;
	boost::shared_array<StreamDatum> stream_data(new StreamDatum[byte_length]);

/*
	666 = 1010011010
			00000101		= 0x05
			0011010		= 0x1A
*/
	stream_data[0] = static_cast<StreamDatum>(0xc0 | pmap_bits_);
	stream_data[1] = static_cast<StreamDatum>('\x05');
	stream_data[2] = static_cast<StreamDatum>('\x9a');
	::memcpy(stream_data.get() + 1 + 2, begin_ptr_, byte_length_);

	const StreamDatum *begin_ptr = stream_data.get();
	const StreamDatum *end_ptr   = stream_data.get() + byte_length;

	datum.data_type_ = value_datum_.data_type_;
	datum.ClearNull();

	exc_context.FieldDecode(begin_ptr, end_ptr);

	const ExcEntry *entry_ptr = exc_context.GetEntryList() + 1;

	switch (datum.data_type_) {
		case DataType_Ascii					:
		case DataType_Unicode				:
		case DataType_ByteVector			:
			datum.string_length_ = entry_ptr->datum_length_;
			memcpy(datum.string_, entry_ptr->datum_ptr_,
				entry_ptr->datum_length_ + 1);
			break;
		case DataType_SInt32					:
			datum.sint_32_ = *entry_ptr->GetDatumPtr<Type_SInt32>();
			break;
		case DataType_UInt32					:
			datum.uint_32_ = *entry_ptr->GetDatumPtr<Type_UInt32>();
			break;
		case DataType_SInt64					:
			datum.sint_64_ = *entry_ptr->GetDatumPtr<Type_SInt64>();
			break;
		case DataType_UInt64					:
			datum.uint_64_ = *entry_ptr->GetDatumPtr<Type_UInt64>();
			break;
		case DataType_Decimal				:
			datum.decimal_ = *entry_ptr->GetDatumPtr<Type_Decimal>();
			break;
		case DataType_Invalid				:
		case DataType_Sequence				:
		case DataType_Group					:
		case DataType_Template				:
		case DataType_TemplateRefStatic	:
		case DataType_TemplateRefDynamic	:
		case DataType_Count					:
		default									:
			MLB::Utility::ThrowLogicError("Invalid primitive type encountered "
				"in the test attempt (" +
				DataTypeToString(datum.data_type_) + ").");
			break;
	}
	
	if (entry_ptr->is_null_)
		datum.SetNull();

	return(begin_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int UT_FieldDecode_Test::RunTests(unsigned int test_count,
	const UT_FieldDecode_Test *test_list)
{
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	unsigned int count_1;
	unsigned int failure_count = 0;

	for (count_1 = 0; count_1 < test_count; ++count_1) {
		const UT_FieldDecode_Test &test_ref = test_list[count_1];
		const StreamDatum            *next_ptr;
		DictValue                    out_datum;
		next_ptr = test_ref.FieldDecode(out_datum);
		std::cout << std::setw(5) << count_1 << ": " <<
			std::left << std::setw(10) << test_ref.value_datum_.data_type_ <<
			" " << ((test_ref.is_optional_) ? "OPT" : "   ") << " " <<
			((test_ref.is_null_) ? "NULL" : "    ") << " " << std::right <<
			test_ref.value_datum_.ValueToStringWidth(23) << " -> " <<
			std::flush;
		try {
			test_ref.value_datum_.CheckIsEquivalent(out_datum);
			std::cout << "***** SUCCEEDED" << std::endl;
		}
		catch (const std::exception &except) {
			std::cout << "***** FAILED: " << except.what() << std::endl;
			++failure_count;
		}
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	std::cout << "Total Tests Executed  : " << std::setw(10) <<
		test_count << std::endl;
	std::cout << "Number of Failed Tests: " << std::setw(10) <<
		failure_count << std::endl;

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;

	std::cout << "UNIT TEST " << ((failure_count) ? "FAILURE" : "SUCCESS") <<
		std::endl;

	return((failure_count) ? EXIT_FAILURE : EXIT_SUCCESS);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#define SET_INTERNAL_TEST_VALUE_NUM(xml_string, p_bits, d_type, d_value,	\
	is_opt, is_null, p_data)																\
	UT_FieldDecode_Test(xml_string, p_bits, d_type, d_value, is_opt,			\
		is_null, sizeof(p_data) - 1, p_data)
#define SET_INTERNAL_TEST_VALUE_STR(xml_string, p_bits, d_type, d_value,	\
	is_opt, is_null, p_data)																\
	UT_FieldDecode_Test(xml_string, p_bits, d_type, sizeof(d_value) - 1,		\
		d_value, is_opt, is_null, sizeof(p_data) - 1, p_data)
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const UT_FieldDecode_Test TEST_InternalTest_List[] = {
	/*
		Appendix 3.1.1 FAST v1.1 Signed Integer Examples
		-------- ----- ---- ---- ------ ------- --------
	*/
	//	-------------------------------------------------------------------------
	//	1. Int32 Example – Optional Positive Number
	SET_INTERNAL_TEST_VALUE_NUM(
		"<int32 id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_SInt32, static_cast<Type_SInt32>(     942755),  true, false, "\x39\x45\xa4"),
	//	-------------------------------------------------------------------------
	//	2. Int32 Example – Mandatory Positive Number
	SET_INTERNAL_TEST_VALUE_NUM(
		"<int32 id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_SInt32, static_cast<Type_SInt32>(     942755), false, false, "\x39\x45\xa3"),
	//	-------------------------------------------------------------------------
	//	3. Int32 Example – Optional Negative Number
	SET_INTERNAL_TEST_VALUE_NUM(
		"<int32 id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_SInt32, static_cast<Type_SInt32>(    -942755),  true, false, "\x46\x3a\xdd"),
	//	-------------------------------------------------------------------------
	//	4. Int32 Example – Mandatory Negative Number
	SET_INTERNAL_TEST_VALUE_NUM(
		"<int32 id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_SInt32, static_cast<Type_SInt32>(   -7942755), false, false, "\x7c\x1b\x1b\x9d"),
	//	-------------------------------------------------------------------------
	//	5. Int32 Example – Mandatory Positive Number with sign-bit extension
	SET_INTERNAL_TEST_VALUE_NUM(
		"<int32 id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_SInt32, static_cast<Type_SInt32>(       8193), false, false, "\x00\x40\x81"),
	//	-------------------------------------------------------------------------
	//	6. Int32 Example – Mandatory Negative Number with sign-bit extension
	//    - The spec has "\x73\x3f\xff", but it should be "\x7f\x3f\xff"
	//      See the FAST Protocol topic "Errors in the FAST 1.1 examples",
	//      dated 14 Nov 2007 9:23AM ET.
	SET_INTERNAL_TEST_VALUE_NUM(
		"<int32 id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_SInt32, static_cast<Type_SInt32>(      -8193), false, false, "\x7f\x3f\xff"),
	//	-------------------------------------------------------------------------
	/*
		Other SInt32 tests...
	*/
	//	Mentioned on FAST v1.1 in section 10.6.1.1 on page 23
	SET_INTERNAL_TEST_VALUE_NUM(
		"<int32 id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_SInt32, static_cast<Type_SInt32>(         64), false, false, "\x00\xc0"),
	//	-------------------------------------------------------------------------

	/*
		Appendix 3.1.2 FAST v1.1 Unsigned Integer Examples
		-------- ----- ---- ---- -------- ------- --------
	*/
	//	-------------------------------------------------------------------------
	//	1. uInt32 Example – Optional Number
	SET_INTERNAL_TEST_VALUE_NUM(
		"<uInt32 id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_UInt32, static_cast<Type_UInt32>(          0),  true,  true, "\x80"),
	SET_INTERNAL_TEST_VALUE_NUM(
		"<uInt32 id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_UInt32, static_cast<Type_UInt32>(          0),  true, false, "\x81"),
	SET_INTERNAL_TEST_VALUE_NUM(
		"<uInt32 id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_UInt32, static_cast<Type_UInt32>(          1),  true, false, "\x82"),
	SET_INTERNAL_TEST_VALUE_NUM(
		"<uInt32 id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_UInt32, static_cast<Type_UInt32>(     942755),  true, false, "\x39\x45\xa4"),
	//	-------------------------------------------------------------------------
	//	2. uInt32 Example – Mandatory Number
	SET_INTERNAL_TEST_VALUE_NUM(
		"<uInt32 id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_UInt32, static_cast<Type_UInt32>(          0), false, false, "\x80"),
	SET_INTERNAL_TEST_VALUE_NUM(
		"<uInt32 id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_UInt32, static_cast<Type_UInt32>(          1), false, false, "\x81"),
	SET_INTERNAL_TEST_VALUE_NUM(
		"<uInt32 id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_UInt32, static_cast<Type_UInt32>(     942755), false, false, "\x39\x45\xa3"),
	//	-------------------------------------------------------------------------

	/*
		Other UInt32 tests...
	*/
	//	-------------------------------------------------------------------------
	//	From FAST v1.1 in section 10.6.1 on page 23
	SET_INTERNAL_TEST_VALUE_NUM(
		"<uInt32 id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_UInt32, static_cast<Type_UInt32>( 4294967295),  true, false, "\x10\x00\x00\x00\x80"),
	//	-------------------------------------------------------------------------

	/*
		Appendix 3.1.3 FAST v1.1 String Examples
		-------- ----- ---- ---- ------ --------
	*/
	//	-------------------------------------------------------------------------
	//	1. US ASCII string Example – Optional String
	SET_INTERNAL_TEST_VALUE_STR(
		"<string id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_Ascii, "",                        				   true,  true, "\x80"),
	SET_INTERNAL_TEST_VALUE_STR(
		"<string id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_Ascii, "ABC",                     				   true, false, "\x41\x42\xc3"),
	SET_INTERNAL_TEST_VALUE_STR(
		"<string id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_Ascii, "",	                    				   	true, false, "\x00\x80"),
	//	-------------------------------------------------------------------------
	//	2. US ASCII string Example – Mandatory String
	SET_INTERNAL_TEST_VALUE_STR(
		"<string id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_Ascii, "ABC",                    				   false, false, "\x41\x42\xc3"),
	SET_INTERNAL_TEST_VALUE_STR(
		"<string id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_Ascii, "",                       				   false, false, "\x80"),
	//	-------------------------------------------------------------------------

	/*
		Other ASCII string tests...
	*/
	//	-------------------------------------------------------------------------
	//	From FAST v1.1 the table in section 10.6.3 on page 24
	SET_INTERNAL_TEST_VALUE_STR(
		"<string id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_Ascii, "\x00",                    				   true, false, "\x00\x00\x80"),
	//	-------------------------------------------------------------------------

	/*
		Appendix 3.1.4 FAST v1.1 Byte Vector Examples
		-------- ----- ---- ---- ---- ------ --------
	*/
	//	-------------------------------------------------------------------------
	//	1. byteVector Example – Optional byteVector
	SET_INTERNAL_TEST_VALUE_STR(
		"<byteVector id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_ByteVector, "",                        				   true,  true, "\x80"),
	SET_INTERNAL_TEST_VALUE_STR(
		"<byteVector id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_ByteVector, "ABC",                     				   true, false, "\x84\x41\x42\x43"),
	SET_INTERNAL_TEST_VALUE_STR(
		"<byteVector id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_ByteVector, "",                        				   true, false, "\x81"),
	//	-------------------------------------------------------------------------
	//	1. byteVector Example – Mandatory byteVector
	SET_INTERNAL_TEST_VALUE_STR(
		"<byteVector id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_ByteVector, "ABC",                    				   false, false, "\x83\x41\x42\x43"),
	SET_INTERNAL_TEST_VALUE_STR(
		"<byteVector id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_ByteVector, "",                       				   false, false, "\x80"),
	//	-------------------------------------------------------------------------

	/*
		Appendix 3.1.5 FAST v1.1 Decimal Examples
		-------- ----- ---- ---- ------- --------
	*/
	//	-------------------------------------------------------------------------
	//	1. Decimal Example – Mandatory Positive Decimal
	SET_INTERNAL_TEST_VALUE_NUM(
		"<decimal id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_Decimal, static_cast<Type_Decimal>(94275500.00), false, false, "\x82\x39\x45\xa3"),
	//	-------------------------------------------------------------------------
	//	2. Decimal Example – Mandatory Positive Decimal with Scaled Mantissa
	SET_INTERNAL_TEST_VALUE_NUM(
		"<decimal id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_Decimal, static_cast<Type_Decimal>(94275500.00), false, false, "\x81\x04\x3f\x34\xde"),
	//	-------------------------------------------------------------------------
	//	3. Decimal Example – Optional Positive Decimal
	SET_INTERNAL_TEST_VALUE_NUM(
		"<decimal id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_Decimal, static_cast<Type_Decimal>(94275500.00),  true, false, "\x83\x39\x45\xa3"),
	//	-------------------------------------------------------------------------
	//	4. Decimal Example – Mandatory Positive Decimal
	SET_INTERNAL_TEST_VALUE_NUM(
		"<decimal id=\"1\" presence=\"mandatory\" name=\"Value\"/>",
		0x00,
		DataType_Decimal, static_cast<Type_Decimal>(    9427.55), false, false, "\xfe\x39\x45\xa3"),
	//	-------------------------------------------------------------------------
	//	5. Decimal Example – Optional Negative Decimal
	SET_INTERNAL_TEST_VALUE_NUM(
		"<decimal id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_Decimal, static_cast<Type_Decimal>(   -9427.55),  true, false, "\xfe\x46\x3a\xdd"),
	//	-------------------------------------------------------------------------
	//	6. Decimal Example – Optional Positive Decimal with single field operator
	SET_INTERNAL_TEST_VALUE_NUM(
		"<decimal id=\"1\" presence=\"optional\" name=\"Value\"><copy/></decimal>",
		0x20,
		DataType_Decimal, static_cast<Type_Decimal>(    9427.55),  true, false, "\xfe\x39\x45\xa3"),
	//	-------------------------------------------------------------------------
	//	7. Decimal Example – Optional Positive Decimal with individual field operators
	SET_INTERNAL_TEST_VALUE_NUM(
		"<decimal id=\"1\" presence=\"optional\" name=\"Value\">"
			"<exponent> <copy/> </exponent>"
			"<mantissa> <delta/> </mantissa>"
		"</decimal>",
		0x20,
		DataType_Decimal, static_cast<Type_Decimal>(    9427.55),  true, false, "\xfe\x39\x45\xa3"),
	//	-------------------------------------------------------------------------
	//	8. Decimal Example – Optional Negative Decimal with sign bit extension
	//    - The spec has "\xfd\x73\x3f\xff", but it should be "\xfd\x7f\x3f\xff"
	//      See the FAST Protocol topic "Errors in the FAST 1.1 examples",
	//      dated 14 Nov 2007 9:23AM ET.
	SET_INTERNAL_TEST_VALUE_NUM(
		"<decimal id=\"1\" presence=\"optional\" name=\"Value\"/>",
		0x00,
		DataType_Decimal, static_cast<Type_Decimal>(     -8.193),  true, false, "\xfd\x7f\x3f\xff"),
	//	-------------------------------------------------------------------------
};
const unsigned int                    TEST_InternalTest_Count  =
	sizeof(TEST_InternalTest_List) / sizeof(TEST_InternalTest_List[0]);
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

} // namespace VFast

} // namespace MLB

using namespace MLB::VFast;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		return_code = UT_FieldDecode_Test::RunTests(TEST_InternalTest_Count,
			TEST_InternalTest_List);
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "UNEXPECTED UNIT TEST ERROR: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

