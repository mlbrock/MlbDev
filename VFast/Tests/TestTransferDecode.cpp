// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Transfer decode tests.

	Revision History	:	2008-11-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/DictValue.hpp>
#include <VFast/VFastException.hpp>
#include <VFast/TransferDecode.hpp>

#include <Utility/StringSupport.hpp>

#include <iostream>
#include <sstream>
#include <cctype>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
struct UT_TransferDecode_Test {
	//	Ctor for integers...
	template <typename ThisType>
	UT_TransferDecode_Test(DataType data_type, ThisType data_value,
		bool is_optional, bool is_null, std::size_t byte_length,
		const char *byte_data);
	//	Ctor for char arrays...
	UT_TransferDecode_Test(DataType data_type, std::size_t data_length,
		const char *data_value, bool is_optional, bool is_null,
		std::size_t byte_length, const char *byte_data);


	const StreamDatum *TransferDecode(DictValue &datum) const;

	DataType           data_type_;
	DictValue         value_datum_;
	bool               is_optional_;
	bool               is_null_;
	unsigned int       byte_length_;
	const StreamDatum *begin_ptr_;
	const StreamDatum *end_ptr_;

	static int RunTests(unsigned int test_count,
		const UT_TransferDecode_Test *test_list);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ThisType>
	UT_TransferDecode_Test::UT_TransferDecode_Test(DataType data_type,
	ThisType data_value, bool is_optional, bool is_null, std::size_t byte_length,
	const char *byte_data)
	:data_type_(data_type)
	,value_datum_(data_type, is_null)
	,is_optional_(is_optional)
	,is_null_(is_null)
	,byte_length_(static_cast<unsigned int>(byte_length))
	,begin_ptr_(reinterpret_cast<const StreamDatum *>(byte_data))
	,end_ptr_(reinterpret_cast<const StreamDatum *>(byte_data + byte_length))
{
	if (!value_datum_.IsTypeInt())
		MLB::Utility::ThrowInvalidArgument("Encountered an invalid data "
			"type (" + DataTypeToString(data_type) + ") --- expected one of "
			"the integer types.");

	value_datum_.data_type_ = data_type;

	value_datum_.SetValue(data_value);

	if (is_null)
		value_datum_.SetNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
UT_TransferDecode_Test::UT_TransferDecode_Test(DataType data_type,
	std::size_t data_length, const char *data_value, bool is_optional,
	bool is_null, std::size_t byte_length, const char *byte_data)
	:data_type_(data_type)
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
const StreamDatum *UT_TransferDecode_Test::TransferDecode(
	DictValue &datum) const
{
	const StreamDatum *begin_ptr = begin_ptr_;

	bool is_null = false;

	datum.data_type_ = value_datum_.data_type_;
	datum.ClearNull();

	if (value_datum_.data_type_ == DataType_SInt32) {
		if (!is_optional_)
			TransferDecode_SInt32(begin_ptr, end_ptr_, datum.sint_32_);
		else
			TransferDecode_SInt32_Null(begin_ptr, end_ptr_, datum.sint_32_,
				is_null);
	}
	else if (value_datum_.data_type_ == DataType_UInt32) {
		if (!is_optional_)
			TransferDecode_UInt32(begin_ptr, end_ptr_, datum.uint_32_);
		else
			TransferDecode_UInt32_Null(begin_ptr, end_ptr_, datum.uint_32_,
				is_null);
	}
	else if (value_datum_.data_type_ == DataType_SInt64) {
		if (!is_optional_)
			TransferDecode_SInt64(begin_ptr, end_ptr_, datum.sint_64_);
		else
			TransferDecode_SInt64_Null(begin_ptr, end_ptr_, datum.sint_64_,
				is_null);
	}
	else if (value_datum_.data_type_ == DataType_UInt64) {
		if (!is_optional_)
			TransferDecode_UInt64(begin_ptr, end_ptr_, datum.uint_64_);
		else
			TransferDecode_UInt64_Null(begin_ptr, end_ptr_, datum.uint_64_,
				is_null);
	}
	else if (value_datum_.data_type_ == DataType_Ascii) {
		if (!is_optional_)
			TransferDecode_Ascii(begin_ptr, end_ptr_,
				sizeof(value_datum_.string_) - 1, datum.string_length_,
				datum.string_);
		else
			TransferDecode_Ascii_Null(begin_ptr, end_ptr_,
				sizeof(value_datum_.string_) - 1, datum.string_length_,
				datum.string_, is_null);
	}
	else if ((value_datum_.data_type_ == DataType_Unicode) ||
				(value_datum_.data_type_ == DataType_ByteVector)) {
		if (!is_optional_)
			TransferDecode_ByteVector(begin_ptr, end_ptr_,
				sizeof(value_datum_.string_) - 1, datum.string_length_,
				datum.string_);
		else
			TransferDecode_ByteVector_Null(begin_ptr, end_ptr_,
				sizeof(value_datum_.string_) - 1, datum.string_length_,
				datum.string_, is_null);
	}

	if (is_null)
		datum.SetNull();

	return(begin_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int UT_TransferDecode_Test::RunTests(unsigned int test_count,
	const UT_TransferDecode_Test *test_list)
{
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	unsigned int count_1;
	unsigned int failure_count = 0;

	for (count_1 = 0; count_1 < test_count; ++count_1) {
		const UT_TransferDecode_Test &test_ref = test_list[count_1];
		const StreamDatum            *next_ptr;
		DictValue                    out_datum;
		next_ptr = test_ref.TransferDecode(out_datum);
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
#define SET_INTERNAL_TEST_VALUE_NUM(d_type, d_value, is_opt, is_null, p_data)	\
	UT_TransferDecode_Test(d_type, d_value, is_opt, is_null,							\
		sizeof(p_data) - 1, p_data)
#define SET_INTERNAL_TEST_VALUE_STR(d_type, d_value, is_opt, is_null, p_data)	\
	UT_TransferDecode_Test(d_type, sizeof(d_value) - 1, d_value, is_opt,			\
		is_null, sizeof(p_data) - 1, p_data)
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const UT_TransferDecode_Test TEST_InternalTest_List[] = {
	/*
		Appendix 3.1.1 FAST v1.1 Signed Integer Examples
		-------- ----- ---- ---- ------ ------- --------
	*/
	//	-------------------------------------------------------------------------
	//	1. Int32 Example – Optional Positive Number
	SET_INTERNAL_TEST_VALUE_NUM(DataType_SInt32, static_cast<Type_SInt32>(     942755),  true, false, "\x39\x45\xa4"),
	//	-------------------------------------------------------------------------
	//	2. Int32 Example – Mandatory Positive Number
	SET_INTERNAL_TEST_VALUE_NUM(DataType_SInt32, static_cast<Type_SInt32>(     942755), false, false, "\x39\x45\xa3"),
	//	-------------------------------------------------------------------------
	//	3. Int32 Example – Optional Negative Number
	SET_INTERNAL_TEST_VALUE_NUM(DataType_SInt32, static_cast<Type_SInt32>(    -942755),  true, false, "\x46\x3a\xdd"),
	//	-------------------------------------------------------------------------
	//	4. Int32 Example – Mandatory Negative Number
	SET_INTERNAL_TEST_VALUE_NUM(DataType_SInt32, static_cast<Type_SInt32>(   -7942755), false, false, "\x7c\x1b\x1b\x9d"),
	//	-------------------------------------------------------------------------
	//	5. Int32 Example – Mandatory Positive Number with sign-bit extension
	SET_INTERNAL_TEST_VALUE_NUM(DataType_SInt32, static_cast<Type_SInt32>(       8193), false, false, "\x00\x40\x81"),
	//	-------------------------------------------------------------------------
	//	6. Int32 Example – Mandatory Negative Number with sign-bit extension
	//    - The spec has "\x73\x3f\xff", but it should be "\x7f\x3f\xff"
	//      See the FAST Protocol topic "Errors in the FAST 1.1 examples",
	//      dated 14 Nov 2007 9:23AM ET.
	SET_INTERNAL_TEST_VALUE_NUM(DataType_SInt32, static_cast<Type_SInt32>(      -8193), false, false, "\x7f\x3f\xff"),
	//	-------------------------------------------------------------------------
	/*
		Other SInt32 tests...
	*/
	//	Mentioned on FAST v1.1 in section 10.6.1.1 on page 23
	SET_INTERNAL_TEST_VALUE_NUM(DataType_SInt32, static_cast<Type_SInt32>(         64), false, false, "\x00\xc0"),
	//	-------------------------------------------------------------------------

	/*
		Appendix 3.1.2 FAST v1.1 Unsigned Integer Examples
		-------- ----- ---- ---- -------- ------- --------
	*/
	//	-------------------------------------------------------------------------
	//	1. uInt32 Example – Optional Number
	SET_INTERNAL_TEST_VALUE_NUM(DataType_UInt32, static_cast<Type_UInt32>(          0),  true,  true, "\x80"),
	SET_INTERNAL_TEST_VALUE_NUM(DataType_UInt32, static_cast<Type_UInt32>(          0),  true, false, "\x81"),
	SET_INTERNAL_TEST_VALUE_NUM(DataType_UInt32, static_cast<Type_UInt32>(          1),  true, false, "\x82"),
	SET_INTERNAL_TEST_VALUE_NUM(DataType_UInt32, static_cast<Type_UInt32>(     942755),  true, false, "\x39\x45\xa4"),
	//	-------------------------------------------------------------------------
	//	2. uInt32 Example – Mandatory Number
	SET_INTERNAL_TEST_VALUE_NUM(DataType_UInt32, static_cast<Type_UInt32>(          0), false, false, "\x80"),
	SET_INTERNAL_TEST_VALUE_NUM(DataType_UInt32, static_cast<Type_UInt32>(          1), false, false, "\x81"),
	SET_INTERNAL_TEST_VALUE_NUM(DataType_UInt32, static_cast<Type_UInt32>(     942755), false, false, "\x39\x45\xa3"),
	//	-------------------------------------------------------------------------

	/*
		Other UInt32 tests...
	*/
	//	-------------------------------------------------------------------------
	//	From FAST v1.1 in section 10.6.1.1 on page 23
	SET_INTERNAL_TEST_VALUE_NUM(DataType_UInt32, static_cast<Type_UInt32>( 4294967295),  true, false, "\x10\x00\x00\x00\x80"),
	//	-------------------------------------------------------------------------

	/*
		Appendix 3.1.3 FAST v1.1 String Examples
		-------- ----- ---- ---- ------ --------
	*/
	//	-------------------------------------------------------------------------
	//	1. US ASCII string Example – Optional String
	SET_INTERNAL_TEST_VALUE_STR(DataType_Ascii, "",                        				   true,  true, "\x80"),
	SET_INTERNAL_TEST_VALUE_STR(DataType_Ascii, "ABC",                     				   true, false, "\x41\x42\xc3"),
	SET_INTERNAL_TEST_VALUE_STR(DataType_Ascii, "",	                    				   true, false, "\x00\x80"),
	//	-------------------------------------------------------------------------
	//	2. US ASCII string Example – Mandatory String
	SET_INTERNAL_TEST_VALUE_STR(DataType_Ascii, "ABC",                    				   false, false, "\x41\x42\xc3"),
	SET_INTERNAL_TEST_VALUE_STR(DataType_Ascii, "",                       				   false, false, "\x80"),
	//	-------------------------------------------------------------------------

	/*
		Other ASCII string tests...
	*/
	//	-------------------------------------------------------------------------
	//	From FAST v1.1 the table in section 10.6.3 on page 24
	SET_INTERNAL_TEST_VALUE_STR(DataType_Ascii, "\x00",                    				   true, false, "\x00\x00\x80"),
	//	-------------------------------------------------------------------------

	/*
		Appendix 3.1.4 FAST v1.1 Byte Vector Examples
		-------- ----- ---- ---- ---- ------ --------
	*/
	//	-------------------------------------------------------------------------
	//	1. byteVector Example – Optional byteVector
	SET_INTERNAL_TEST_VALUE_STR(DataType_ByteVector, "",                        				   true,  true, "\x80"),
	SET_INTERNAL_TEST_VALUE_STR(DataType_ByteVector, "ABC",                     				   true, false, "\x84\x41\x42\x43"),
	SET_INTERNAL_TEST_VALUE_STR(DataType_ByteVector, "",                        				   true, false, "\x81"),
	//	-------------------------------------------------------------------------
	//	1. byteVector Example – Mandatory byteVector
	SET_INTERNAL_TEST_VALUE_STR(DataType_ByteVector, "ABC",                    				   false, false, "\x83\x41\x42\x43"),
	SET_INTERNAL_TEST_VALUE_STR(DataType_ByteVector, "",                       				   false, false, "\x80"),
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
		return_code = UT_TransferDecode_Test::RunTests(TEST_InternalTest_Count,
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

