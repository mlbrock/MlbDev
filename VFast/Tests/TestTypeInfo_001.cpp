// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Fast FIX Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Basic type dispatch tests.

	Revision History	:	2008-11-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
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

#include <iostream>
#include <sstream>
#include <cctype>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/*
typedef boost::mpl::vector<
	DataType,
	DataType_Ascii,
	DataType_Unicode,
	DataType_ByteVector,
	DataType_SInt32,
	DataType_UInt32,
	DataType_SInt64,
	DataType_UInt64,
	DataType_Decimal>	DataType_IntegerList;

typedef boost::mpl::vector<
	DataType,
	DataType_Ascii>	DataType_AsciiSimpleList;

typedef boost::mpl::vector<
	DataType,
	DataType_Unicode,
	DataType_ByteVector>	DataType_AsciiCountedList;
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_Emit(DataType data_type, const char *this_name, const char *args)
{
	std::cout << "In " << DataTypeToString(data_type) << ":" << this_name <<
		"(" << args << ")" << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <DataType ThisDType> struct DTypeDispatch { };

template <> struct DTypeDispatch<DataType_SInt32>
{
	typedef Type_SInt32 type;

	static void TransferDecodeForType()
	{
		TEST_Emit(DataType_SInt32, "TransferDecodeForType", "");
	}
	static void AddEntryForType()
	{
		TEST_Emit(DataType_SInt32, "AddEntryForType", "");
	}
};

template <> struct DTypeDispatch<DataType_Ascii>
{
	typedef Type_String type;

	static void TransferDecodeForType()
	{
		TEST_Emit(DataType_Ascii, "TransferDecodeForType", "");
	}
	static void AddEntryForType()
	{
		TEST_Emit(DataType_Ascii, "AddEntryForType", "");
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class SomeClass {
public:
	SomeClass()
		:datum_(666)
	{
	}

	template <DataType DType> void TransferDecode()
	{
		TEST_Emit(DType, "TransferDecode_Null", "");
	}
	template <DataType DType> void TransferDecode_Null()
	{
		TEST_Emit(DType, "TransferDecode_Null", "");
	}

	template <> void TransferDecode<DataType_Ascii>()
	{
		TEST_Emit(DataType_Ascii, "TransferDecode", "");
	}
	template <> void TransferDecode_Null<DataType_Ascii>()
	{
		TEST_Emit(DataType_Ascii, "TransferDecode_Null", "");
	}

	template <DataType DType> void AddEntryForType()
	{
		TEST_Emit(DType, "AddEntryForType", "");
	}
	template <> void AddEntryForType<DataType_Ascii>()
	{
		TEST_Emit(DataType_Ascii, "AddEntryForType", "");
	}

private:
	int datum_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

using namespace MLB::VFast;

// ////////////////////////////////////////////////////////////////////////////
void TEST_Run()
{
	DTypeDispatch<DataType_SInt32>::TransferDecodeForType();
	DTypeDispatch<DataType_Ascii>::AddEntryForType();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_Run();
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "UNEXPECTED UNIT TEST ERROR: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

