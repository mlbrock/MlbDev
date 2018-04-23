// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The VFast transfer decoding include file.

	Revision History	:	2008-11-17 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__TransferDecode_hpp__HH

#define HH__MLB__VFast__TransferDecode_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file TransferDecode.hpp
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
const unsigned int FirstPMapBit              = 0x40;
const unsigned int MaxPMapByteLength         = 1024;

const StreamDatum StreamDatumStopBit         = 0x80;
const StreamDatum StreamDatumTemplateIdBit   = 0x40;
const StreamDatum StreamDatumIntegralSignBit = 0x40;
const StreamDatum StreamDatumValueMask       =
	static_cast<StreamDatum>(~StreamDatumStopBit);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline void ThrowUnexpectedDatumEnd(const StreamDatum *begin_ptr,
	const StreamDatum *end_ptr, DataType data_type, bool is_nullable)
{
	MLB::Utility::ThrowLogicError("Decoded past the end of the transfer stream "
		"without encountering the end of a field of type " +
		DataTypeToString(data_type) + " which was " +
		((is_nullable) ? "" : " not") + " flagged as nullable, byte range " +
		MLB::Utility::ValueToStringHex(begin_ptr) + " to " +
		MLB::Utility::ValueToStringHex(end_ptr) + ".");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline void ThrowMaxDatumBytes(const StreamDatum *begin_ptr,
	const StreamDatum *end_ptr, DataType data_type, unsigned int max_bytes,
	bool is_nullable)
{
	MLB::Utility::ThrowLogicError("Decoded the maximum number of bytes of the "
		"transfer stream without encountering the end of a field of type " +
		DataTypeToString(data_type) + " (" +
		MLB::Utility::AnyToString(max_bytes) + " bytes) which was " +
		((is_nullable) ? "" : " not") + " flagged as nullable, byte range " +
		MLB::Utility::ValueToStringHex(begin_ptr) + " to " +
		MLB::Utility::ValueToStringHex(end_ptr) + ".");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline void ThrowOffEnd(const StreamDatum *original_begin_ptr,
	const StreamDatum *begin_ptr, const StreamDatum *end_ptr, DataType data_type,
	unsigned int max_bytes, bool is_nullable)
{
	if (begin_ptr >= end_ptr)
		ThrowUnexpectedDatumEnd(original_begin_ptr, end_ptr, data_type,
			is_nullable);
	else
		ThrowMaxDatumBytes(original_begin_ptr, end_ptr, data_type, max_bytes,
			is_nullable);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
	\param max_bytes \e must be > 0
*/
inline unsigned int TransferDecode_Datum_Length(DataType data_type,
	const StreamDatum *begin_ptr, const StreamDatum *end_ptr,
	unsigned int max_bytes, bool is_nullable)
{
	unsigned int byte_count = 0;

	while (begin_ptr < end_ptr) {
		++byte_count;
		if (*begin_ptr & StreamDatumStopBit)
			return(byte_count);
		++begin_ptr;
		if (byte_count >= max_bytes)
			ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, data_type,
				max_bytes, is_nullable);
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, data_type,
		max_bytes, is_nullable);

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
*/
inline void TransferDecode_SInt32(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, Type_SInt32 &datum)
{
	datum = (*begin_ptr & StreamDatumIntegralSignBit) ? -1 : 0;

	unsigned int byte_count = 0;

	for ( ; (begin_ptr < end_ptr) && (byte_count < 5);
		++begin_ptr, ++byte_count) {
		datum = (datum << 7) |
			static_cast<Type_SInt32>(*begin_ptr & StreamDatumValueMask);
		if (*begin_ptr & StreamDatumStopBit) {
			++begin_ptr;
			return;
		}
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, DataType_SInt32,
		5, false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
*/
inline void TransferDecode_SInt32_Null(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, Type_SInt32 &datum, bool &is_null)
{
	if (*begin_ptr == StreamDatumStopBit) {
		datum   = 0;
		is_null = true;
		++begin_ptr;
		return;
	}
		
	datum   = (*begin_ptr & StreamDatumIntegralSignBit) ? -1 : 0;
	is_null = false;

	unsigned int byte_count = 0;

	for ( ; (begin_ptr < end_ptr) && (byte_count < 5);
		++begin_ptr, ++byte_count) {
		datum = (datum << 7) |
			static_cast<Type_SInt32>(*begin_ptr & StreamDatumValueMask);
		if (*begin_ptr & StreamDatumStopBit) {
			if (datum > 0)
				--datum;
			++begin_ptr;
			return;
		}
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, DataType_SInt32,
		5, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
*/
inline void TransferDecode_UInt32(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, Type_UInt32 &datum)
{
	datum = 0;

	unsigned int byte_count = 0;

	for ( ; (begin_ptr < end_ptr) && (byte_count < 5);
		++begin_ptr, ++byte_count) {
		datum = (datum << 7) |
			static_cast<Type_UInt32>(*begin_ptr & StreamDatumValueMask);
		if (*begin_ptr & StreamDatumStopBit) {
			++begin_ptr;
			return;
		}
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, DataType_UInt32,
		5, false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
*/
inline void TransferDecode_UInt32_Null(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, Type_UInt32 &datum, bool &is_null)
{
	if (*begin_ptr == StreamDatumStopBit) {
		datum    = 0;
		is_null  = true;
		++begin_ptr;
		return;
	}
		
	datum   = 0;
	is_null = false;

	unsigned int byte_count = 0;

	for ( ; (begin_ptr < end_ptr) && (byte_count < 5);
		++begin_ptr, ++byte_count) {
		datum = (datum << 7) |
			static_cast<Type_UInt32>(*begin_ptr & StreamDatumValueMask);
		if (*begin_ptr & StreamDatumStopBit) {
			if (datum > 0)
				--datum;
			else if ((!datum) && (byte_count == 4))
				--datum;
			++begin_ptr;
			return;
		}
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, DataType_UInt32,
		5, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
*/
inline void TransferDecode_SInt64(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, Type_SInt64 &datum)
{
	datum = (*begin_ptr & StreamDatumIntegralSignBit) ? -1 : 0;

	unsigned int byte_count = 0;

	for ( ; (begin_ptr < end_ptr) && (byte_count < 10);
		++begin_ptr, ++byte_count) {
		datum = (datum << 7) |
			static_cast<Type_SInt64>(*begin_ptr & StreamDatumValueMask);
		if (*begin_ptr & StreamDatumStopBit) {
			++begin_ptr;
			return;
		}
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, DataType_SInt64,
		10, false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
*/
inline void TransferDecode_SInt64_Null(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, Type_SInt64 &datum, bool &is_null)
{
	if (*begin_ptr == StreamDatumStopBit) {
		datum   = 0;
		is_null = true;
		++begin_ptr;
		return;
	}
		
	datum   = (*begin_ptr & StreamDatumIntegralSignBit) ? -1 : 0;
	is_null = false;

	unsigned int byte_count = 0;

	for ( ; (begin_ptr < end_ptr) && (byte_count < 10);
		++begin_ptr, ++byte_count) {
		datum = (datum << 7) |
			static_cast<Type_SInt64>(*begin_ptr & StreamDatumValueMask);
		if (*begin_ptr & StreamDatumStopBit) {
			if (datum > 0)
				--datum;
			++begin_ptr;
			return;
		}
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, DataType_SInt64,
		10, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
*/
inline void TransferDecode_UInt64(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, Type_UInt64 &datum)
{
	datum = 0;

	unsigned int byte_count = 0;

	for ( ; (begin_ptr < end_ptr) && (byte_count < 10);
		++begin_ptr, ++byte_count) {
		datum = (datum << 7) |
			static_cast<Type_UInt64>(*begin_ptr & StreamDatumValueMask);
		if (*begin_ptr & StreamDatumStopBit) {
			++begin_ptr;
			return;
		}
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, DataType_UInt64,
		10, false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\param begin_ptr \e must be < end_ptr
*/
inline void TransferDecode_UInt64_Null(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, Type_UInt64 &datum, bool &is_null)
{
	if (*begin_ptr == StreamDatumStopBit) {
		datum    = 0;
		is_null  = true;
		++begin_ptr;
		return;
	}
		
	datum   = 0;
	is_null = false;

	unsigned int byte_count = 0;

	for ( ; (begin_ptr < end_ptr) && (byte_count < 10);
		++begin_ptr, ++byte_count) {
		datum = (datum << 7) |
			static_cast<Type_UInt64>(*begin_ptr & StreamDatumValueMask);
		if (*begin_ptr & StreamDatumStopBit) {
			if (datum > 0)
				--datum;
			else if ((!datum) && (byte_count == 9))
				--datum;
			++begin_ptr;
			return;
		}
	}

	ThrowOffEnd(begin_ptr - byte_count, begin_ptr, end_ptr, DataType_UInt64,
		10, true);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void TransferDecode_Ascii(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, unsigned int max_bytes,
	unsigned int &datum_length, char *datum)
{
	unsigned int byte_count = TransferDecode_Datum_Length(DataType_Ascii,
		begin_ptr, end_ptr, max_bytes, false);

	if (*begin_ptr == StreamDatumStopBit) {
		datum_length = 0;
		datum[0]     = '\0';
		++begin_ptr;
		return;
	}
	else if ((byte_count == 2) && (!(*begin_ptr)) &&
		(begin_ptr[1] == StreamDatumStopBit)) {
		datum_length = 1;
		datum[0]     = '\0';
		datum[1]     = '\0';
		begin_ptr += 2;
		return;
	}

	datum_length = byte_count;

	while (byte_count--)
		*datum++ = *begin_ptr++ & StreamDatumValueMask;

	*datum = '\0';
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void TransferDecode_Ascii_Null(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, unsigned int max_bytes,
	unsigned int &datum_length, char *datum, bool &is_null)
{
	unsigned int byte_count = TransferDecode_Datum_Length(DataType_Ascii,
		begin_ptr, end_ptr, max_bytes, true);

	if (byte_count == 1) {
		if (*begin_ptr == StreamDatumStopBit) {
			datum_length = 0;
			datum[0]     = '\0';
			is_null      = true;
		}
		else {
			datum_length = 1;
			datum[0]     = *begin_ptr & StreamDatumValueMask;
			datum[1]     = '\0';
			is_null      = false;
		}
		++begin_ptr;
		return;
	}

	is_null = false;

	if ((byte_count == 2) && (!(*begin_ptr)) &&
		(begin_ptr[1] == StreamDatumStopBit)) {
		datum_length = 0;
		datum[0]     = '\0';
		begin_ptr += 2;
		return;
	}
	else if ((byte_count == 3) && (!(*begin_ptr)) && (!begin_ptr[1]) &&
		(begin_ptr[2] == StreamDatumStopBit)) {
		datum_length = 1;
		datum[0]     = '\0';
		datum[1]     = '\0';
		begin_ptr += 3;
		return;
	}

	datum_length = byte_count;

	while (byte_count--)
		*datum++ = *begin_ptr++ & StreamDatumValueMask;

	*datum = '\0';
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void TransferDecode_ByteVector(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, unsigned int max_bytes,
	unsigned int &datum_length, char *datum)
{
	Type_UInt32 byte_count;

	TransferDecode_UInt32(begin_ptr, end_ptr, byte_count);

	if ((begin_ptr + byte_count) > end_ptr)
		ThrowUnexpectedDatumEnd(begin_ptr, end_ptr, DataType_ByteVector, false);
	else if (byte_count > max_bytes)
		ThrowMaxDatumBytes(begin_ptr, end_ptr, DataType_ByteVector, max_bytes,
			false);

	datum_length = byte_count;

	while (byte_count--)
		*datum++ = static_cast<char>(*begin_ptr++);

	*datum = '\0';
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline void TransferDecode_ByteVector_Null(const StreamDatum *&begin_ptr,
	const StreamDatum *end_ptr, unsigned int max_bytes,
	unsigned int &datum_length, char *datum, bool &is_null)
{
	Type_UInt32 byte_count;

	TransferDecode_UInt32_Null(begin_ptr, end_ptr, byte_count, is_null);

	if (is_null)
		datum_length = 0;
	else {
		if ((begin_ptr + byte_count) > end_ptr)
			ThrowUnexpectedDatumEnd(begin_ptr, end_ptr, DataType_ByteVector,
				true);
		else if (byte_count > max_bytes)
			ThrowMaxDatumBytes(begin_ptr, end_ptr, DataType_ByteVector,
				max_bytes, true);
		datum_length = byte_count;
		while (byte_count--)
			*datum++ = static_cast<char>(*begin_ptr++);
	}

	*datum = '\0';
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__TransferDecode_hpp__HH

