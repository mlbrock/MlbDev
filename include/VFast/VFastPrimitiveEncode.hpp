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

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__VFastPrimitiveEncode_hpp__HH

#define HH__MLB__VFast__VFastPrimitiveEncode_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file VFastPrimitiveEncode.hpp

	\brief	The VFast encode header file.

	Contains logic for encoding FAST types.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

#if 0
// ////////////////////////////////////////////////////////////////////////////
/**
	\param stream_ptr Should point to at least five free bytes.

	\note To encode NULL, place 0x80 in the stream.
*/
inline StreamDatum *Encode(Type_SInt32 datum, StreamDatum *stream_ptr,
	bool is_nullable)
{
	if (is_nullable && (datum > 0))
		++datum;

	unsigned int bytes_needed;

	if (datum < 0) {
      if (     datum >= static_cast<Type_SInt32>(0xffffffc0))
			bytes_needed = 1; // -64
      else if (datum >= static_cast<Type_SInt32>(0xffffe000))
			bytes_needed = 2; // -8192
      else if (datum >= static_cast<Type_SInt32>(0xfff00000))
			bytes_needed = 3; // -1048576
      else if (datum >= static_cast<Type_SInt32>(0xf8000000))
			bytes_needed = 4; // -124317728
		else
			bytes_needed = 5;
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
			*stream_ptr++ = (datum >> 28) & 0x7f;
		case 4	:
			*stream_ptr++ = (datum >> 21) & 0x7f;
		case 3	:
			*stream_ptr++ = (datum >> 14) & 0x7f;
		case 2	:
			*stream_ptr++ = (datum >>  7) & 0x7f;
		case 1	:
			*stream_ptr++ = (datum & 0x7f) | 0x80;
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
			*stream_ptr++ = (datum >> 28) & 0x7f;
		case 4	:
			*stream_ptr++ = (datum >> 21) & 0x7f;
		case 3	:
			*stream_ptr++ = (datum >> 14) & 0x7f;
		case 2	:
			*stream_ptr++ = (datum >>  7) & 0x7f;
		case 1	:
			*stream_ptr++ = (datum & 0x7f) | 0x80;
			break;
		default	:
			break;
	}

	return(stream_ptr);
}
// ////////////////////////////////////////////////////////////////////////////
#endif // #if 0

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__VFastPrimitiveEncode_hpp__HH

