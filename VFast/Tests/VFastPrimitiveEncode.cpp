// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	The VFast Primitive Encode Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The VFast include file for the encoding of primitives.

	Revision History	:	2008-11-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFastPrimitiveEncode.hpp>

#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	\param stream_ptr Should point to at least five free bytes.

	\note To encode NULL, place 0x80 in the stream.

	11111111111111111111111111000000 = 0xffffffc0 =        -64 = 26 +  6 bits
	11111111111111111110000000000000 = 0xffffe000 =      -8192 = 19 + 13 bits
	11111111111100000000000000000000 = 0xfff00000 =   -1048576 = 12 + 20 bits
	11111000000000000000000000000000 = 0xf8000000 = -124317728 =  5 + 27 bits

	00000000000000000000000001000000 = 0x00000040 =         64 = 25 +  7 bits
	00000000000000000010000000000000 = 0x00002000 =       8192 = 18 + 14 bits
	00000000000100000000000000000000 = 0x00100000 =    1048576 = 11 + 21 bits
	00001000000000000000000000000000 = 0x08000000 =  134217728 =  4 + 28 bits
*/
// ////////////////////////////////////////////////////////////////////////////
inline StreamDatum *Encode(Type_SInt32 datum, StreamDatum *stream_ptr,
	bool is_nullable)
{
	if (is_nullable && (datum > 0))
		++datum;

	unsigned int bytes_needed;

	if (datum < 0) {
      if (     datum >= static_cast<Type_SInt32>(0xffffffc0)) // <= 6 bits
			bytes_needed = 1;
      else if (datum >= static_cast<Type_SInt32>(0xffffe000)) // <= 7 +  6 bits
			bytes_needed = 2;
      else if (datum >= static_cast<Type_SInt32>(0xfff00000)) // <= 7 + 13 bits
			bytes_needed = 3;
      else if (datum >= static_cast<Type_SInt32>(0xf8000000)) // <= 7 + 20 bits
			bytes_needed = 4;
		else
			bytes_needed = 5;					// <= 4 + 27 bits
	}
	else {
      if (     datum < static_cast<Type_SInt32>(0x00000040))
			bytes_needed = 1; // 64
      else if (datum < static_cast<Type_SInt32>(0x00002000))
			bytes_needed = 2; // 8192
      else if (datum < static_cast<Type_SInt32>(0x00100000))
			bytes_needed = 3; // 1048576
      else if (datum < static_cast<Type_SInt32>(0x08000000))
			bytes_needed = 4; // 134217728
		else
			bytes_needed = 5;
	}

	switch (bytes_needed) {
		case 5	:
			*stream_ptr++ = static_cast<StreamDatum>((datum >> 28) & 0x7f);
		case 4	:
			*stream_ptr++ = static_cast<StreamDatum>((datum >> 21) & 0x7f);
		case 3	:
			*stream_ptr++ = static_cast<StreamDatum>((datum >> 14) & 0x7f);
		case 2	:
			*stream_ptr++ = static_cast<StreamDatum>((datum >>  7) & 0x7f);
		case 1	:
			*stream_ptr++ = static_cast<StreamDatum>((datum & 0x7f) | 0x80);
			break;
		default	:
			break;
	}

	return(stream_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param stream_ptr Should point to at least five free bytes.

	\note To encode NULL, place 0x80 in the stream.
*/
inline StreamDatum *Encode(Type_UInt32 datum, StreamDatum *stream_ptr,
	bool is_nullable)
{
	if (is_nullable && (datum > 0)) {
		if (datum < 0xffffffff)
			++datum;
		else {
			stream_ptr[0] = 0x10;
			stream_ptr[1] = 0x00;
			stream_ptr[2] = 0x00;
			stream_ptr[3] = 0x00;
			stream_ptr[4] = 0x80;
			return(stream_ptr + 5);
		}
	}

	unsigned int bytes_needed;

   if      (datum < static_cast<Type_UInt32>(0x00000080))
		bytes_needed = 1; // 128
   else if (datum < static_cast<Type_UInt32>(0x00004000))
		bytes_needed = 2; // 16384
   else if (datum < static_cast<Type_UInt32>(0x00200000))
		bytes_needed = 3; // 2097152
   else if (datum < static_cast<Type_UInt32>(0x10000000))
		bytes_needed = 4; // 268435456
	else
		bytes_needed = 5;

	switch (bytes_needed) {
		case 5	:
			*stream_ptr++ = static_cast<StreamDatum>((datum >> 28) & 0x7f);
		case 4	:
			*stream_ptr++ = static_cast<StreamDatum>((datum >> 21) & 0x7f);
		case 3	:
			*stream_ptr++ = static_cast<StreamDatum>((datum >> 14) & 0x7f);
		case 2	:
			*stream_ptr++ = static_cast<StreamDatum>((datum >>  7) & 0x7f);
		case 1	:
			*stream_ptr++ = static_cast<StreamDatum>((datum & 0x7f) | 0x80);
			break;
		default	:
			break;
	}

	return(stream_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#include <Utility.hpp>

#include <cstdlib>
#include <cstring>

using namespace MLB::VFast;

// ////////////////////////////////////////////////////////////////////////////
std::string ToHexString(const StreamDatum *begin_ptr,
	const StreamDatum *end_ptr, bool prefix_flag = true, bool pad_to_8 = true)
{
	std::string out_string;

	for ( ; begin_ptr < end_ptr; ++begin_ptr)
		out_string += std::string((out_string.empty()) ? "" : " ") +
			((pad_to_8) ? ((prefix_flag) ? "    " : "      ") : "") +
			MLB::Utility::ValueToStringHex(*begin_ptr, prefix_flag);

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_ShowResults(const std::string &datum, const char *type_name,
	bool is_nullable, const StreamDatum *begin_ptr, const StreamDatum *end_ptr)
{
	const char *nullable = (is_nullable) ? "NULLABLE" : "NOT-NULLABLE";

	std::cout << type_name << " " << nullable << std::endl;
	std::cout << std::setfill('-') <<
		std::setw(static_cast<std::streamsize>(::strlen(type_name))) << "" <<
		" " <<
		std::setw(static_cast<std::streamsize>(::strlen(nullable))) << "" <<
		std::setfill(' ') << std::endl;
	std::cout << "Value: [" << datum << "]" << std::endl;
	std::cout << "Bytes: [" << ToHexString(begin_ptr, end_ptr, false, false) <<
		"]" << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename PrimitiveType>
	void TEST_PrimitiveTypeEncode(PrimitiveType datum, const char *type_name)
{
	StreamDatum  buffer[4096];
	StreamDatum *begin_ptr = buffer;
	StreamDatum *end_ptr;

	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	{
		begin_ptr = buffer;
		end_ptr   = Encode(datum, begin_ptr, false);
		std::ostringstream o_str;
		o_str << datum;
		TEST_ShowResults(o_str.str(), type_name, false, begin_ptr, end_ptr);
	}

	std::cout << std::setfill('-') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	{
		begin_ptr = buffer;
		end_ptr   = Encode(datum, begin_ptr, true);
		std::ostringstream o_str;
		o_str << datum;
		TEST_ShowResults(o_str.str(), type_name, true, begin_ptr, end_ptr);
	}

	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
template <typename PrimitiveType>
	void TEST_PrimitiveTypeEncodeCycleOne(PrimitiveType datum,
	const char *type_name, bool is_nullable)
{
	StreamDatum     buffer[4096];
	StreamDatum    *begin_ptr;
	StreamDatum    *end_encode_ptr;
	StreamDatum    *end_decode_ptr;
	PrimitiveType  result;

	end_encode_ptr = Encode(datum, buffer, is_nullable);
	end_decode_ptr = Decode(result, buffer, is_nullable);

	if (datum != result) {
		std::cout << "ERROR (" << type_name << ")" << std::endl;
		std::cout << "ENCODED: [" << std::setw(23) << datum << "]" << std::endl;
		std::cout << "DECODED: [" << std::setw(23) << result << "]" << std::endl;
		std::cout << "ENBYTES: [" <<
			ToHexString(begin_encode_ptr, end_ptr, false, false) << "]" <<
			std::endl;
		std::cout << "DEBYTES: [" <<
			ToHexString(begin_decode_ptr, end_ptr, false, false) << "]" <<
			std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename PrimitiveType>
	void TEST_PrimitiveTypeEncodeCycle(PrimitiveType datum,
	const char *type_name)
{
	PrimitiveType low  = std::numeric_limits<PrimitiveType>::min();
	PrimitiveType high = std::numeric_limits<PrimitiveType>::max();
	PrimitiveType last = high - 1;

	for ( ; ; ) {
		TEST_PrimitiveTypeEncodeCycleOne(low, type_name, false)
		TEST_PrimitiveTypeEncodeCycleOne(low, type_name, true)
		if (low == high)
			break;
		++low;
	} 
}
// ////////////////////////////////////////////////////////////////////////////
*/
// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_PrimitiveTypeEncode<Type_SInt32>(  0, "Type_SInt32");
		TEST_PrimitiveTypeEncode<Type_SInt32>(-63, "Type_SInt32");
		TEST_PrimitiveTypeEncode<Type_SInt32>(-64, "Type_SInt32");
		TEST_PrimitiveTypeEncode<Type_SInt32>(-65, "Type_SInt32");
		TEST_PrimitiveTypeEncode<Type_SInt32>( 63, "Type_SInt32");
		TEST_PrimitiveTypeEncode<Type_SInt32>( 64, "Type_SInt32");
		TEST_PrimitiveTypeEncode<Type_SInt32>( 65, "Type_SInt32");
		TEST_PrimitiveTypeEncode<Type_UInt32>(  0, "Type_UInt32");
		TEST_PrimitiveTypeEncode<Type_UInt32>( 63, "Type_UInt32");
		TEST_PrimitiveTypeEncode<Type_UInt32>( 64, "Type_UInt32");
		TEST_PrimitiveTypeEncode<Type_UInt32>( 66, "Type_UInt32");
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

