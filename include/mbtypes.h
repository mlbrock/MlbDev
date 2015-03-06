/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Primitive Types Include File															*/
/*	***********************************************************************	*/
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Primitive data type include file.

	Revision History	:	1990-03-13 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1990 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/*	***********************************************************************	*/

#ifndef HH__mbtypes_h__HH

#define HH__mbtypes_h__HH					1

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Required include files...																*/
/*	***********************************************************************	*/

#if defined(__linux__)
# include <sys/types.h>
#endif // #if defined(__linux__)

/*	***********************************************************************	*/

/*	***********************************************************************	*/
/**
	\file 	mbtypes.h

	\brief	Native primitive types.

	Contains typedefs for the platform basic native primitive types.
*/
/*	***********************************************************************	*/
typedef signed char        Native_SInt8;	/**< 8-bit signed integer			*/
typedef unsigned char      Native_UInt8;	/**< 8-bit unsigned integer		*/

#ifdef __MSDOS__
typedef signed short       Native_SInt16;	/**< 16-bit signed integer			*/
typedef unsigned short     Native_UInt16;	/**< 16-bit unsigned integer		*/
typedef signed long        Native_SInt32;	/**< 32-bit signed integer			*/
typedef unsigned long      Native_UInt32;	/**< 32-bit unsigned integer		*/
typedef float              Native_Float32;/**< 32-bit IEEE floating point	*/
typedef double             Native_Float64;/**< 64-bit IEEE floating point	*/
#elif _MSC_VER
typedef signed __int16     Native_SInt16;	/**< 16-bit signed integer			*/
typedef unsigned __int16   Native_UInt16;	/**< 16-bit unsigned integer		*/
typedef signed __int32     Native_SInt32;	/**< 32-bit signed integer			*/
typedef unsigned __int32   Native_UInt32;	/**< 32-bit unsigned integer		*/
typedef signed __int64     Native_SInt64;	/**< 64-bit signed integer			*/
typedef unsigned __int64   Native_UInt64;	/**< 64-bit unsigned integer		*/
typedef float              Native_Float32;/**< 32-bit IEEE floating point	*/
typedef double             Native_Float64;/**< 64-bit IEEE floating point	*/
#elif defined(__linux__)
typedef int16_t            Native_SInt16;	/**< 16-bit signed integer			*/
typedef u_int16_t          Native_UInt16;	/**< 16-bit unsigned integer		*/
typedef int32_t            Native_SInt32;	/**< 32-bit signed integer			*/
typedef u_int32_t          Native_UInt32;	/**< 32-bit unsigned integer		*/
typedef int64_t            Native_SInt64;	/**< 64-bit signed integer			*/
typedef u_int64_t          Native_UInt64;	/**< 64-bit unsigned integer		*/
typedef float              Native_Float32;/**< 32-bit IEEE floating point	*/
typedef double             Native_Float64;/**< 64-bit IEEE floating point	*/
#else
typedef signed short       Native_SInt16;	/**< 16-bit signed integer			*/
typedef unsigned short     Native_UInt16;	/**< 16-bit unsigned integer		*/
typedef signed int         Native_SInt32;	/**< 32-bit signed integer			*/
typedef unsigned int       Native_UInt32;	/**< 32-bit unsigned integer		*/
typedef signed long long   Native_SInt64;	/**< 64-bit signed integer			*/
typedef unsigned long long Native_UInt64;	/**< 64-bit unsigned integer		*/
typedef float              Native_Float32;/**< 32-bit IEEE floating point	*/
typedef double             Native_Float64;/**< 64-bit IEEE floating point	*/
#endif /* #ifdef __MSDOS__ */
/*	***********************************************************************	*/

#endif /* #ifndef HH__mbtypes_h__HH */

