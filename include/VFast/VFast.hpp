// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The VFast include file.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__VFast_hpp__HH

#define HH__MLB__VFast__VFast_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file VFast.hpp

	\brief	The basic Fast Fix header file.

	Contains basic classes and definitions required for the implementation of
	the VFast library.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <mbcompat.h>

#if defined(linux)
# include <sys/types.h>
#endif // #if defined(linux)

//	CODE NOTE: Fix me!!!
#define API_VFAST

// ////////////////////////////////////////////////////////////////////////////

/**
	\namespace MLB

	\brief The corporate namespace.
*/
namespace MLB {

/**
	\namespace VFast

	\brief The VFast parser library project namespace.
*/
namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
typedef signed char        Type_SInt8;		///< 8-bit signed integer value
typedef unsigned char      Type_UInt8;		///< 8-bit unsigned integer value
#ifdef _MSC_VER
typedef signed __int16     Type_SInt16;	///< 16-bit signed integer value
typedef unsigned __int16   Type_UInt16;	///< 16-bit unsigned integer value
typedef signed __int32     Type_SInt32;	///< 32-bit signed integer value
typedef unsigned __int32   Type_UInt32;	///< 32-bit unsigned integer value
typedef signed __int64     Type_SInt64;	///< 64-bit signed integer value
typedef unsigned __int64   Type_UInt64;	///< 64-bit unsigned integer value
typedef double             Type_Decimal;	///< 64-bit IEEE floating-point value
typedef char               Type_String;	///< Strings (ASCII and Unicode)
#elif defined(linux)
typedef int16_t            Type_SInt16;	///< 16-bit signed integer value
typedef u_int16_t          Type_UInt16;	///< 16-bit unsigned integer value
typedef int32_t            Type_SInt32;	///< 32-bit signed integer value
typedef u_int32_t          Type_UInt32;	///< 32-bit unsigned integer value
typedef int64_t            Type_SInt64;	///< 64-bit signed integer value
typedef u_int64_t          Type_UInt64;	///< 64-bit unsigned integer value
typedef double             Type_Decimal;	///< 64-bit IEEE floating-point value
typedef char               Type_String;	///< Strings (ASCII and Unicode)
#else
typedef signed short       Type_SInt16;	///< 16-bit signed integer value
typedef unsigned short     Type_UInt16;	///< 16-bit unsigned integer value
typedef signed int         Type_SInt32;	///< 32-bit signed integer value
typedef unsigned int       Type_UInt32;	///< 32-bit unsigned integer value
typedef signed long long   Type_SInt64;	///< 64-bit signed integer value
typedef unsigned long long Type_UInt64;	///< 64-bit unsigned integer value
typedef double             Type_Decimal;	///< 64-bit IEEE floating-point value
typedef char               Type_String;	///< Strings (ASCII and Unicode)
#endif // #ifdef _MSC_VER
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	The data type of the raw bytes within a Fast Fix message.
*/
typedef unsigned char StreamDatum;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	The length in bytes of the character array types (\e DataType_Ascii ,
	\e DataType_UTF8 and \e DataType_ByteVector ).
*/
const unsigned int VFastMaxStringLength = 255;
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__VFast_hpp__HH

