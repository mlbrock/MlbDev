// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Xerces-C Utilities Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Xerces-C include file.

	Revision History	:	2005-01-07 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__XercesUtils__XercesUtils_hpp__HH

#define HH__MLB__XercesUtils__XercesUtils_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
# pragma warning(push)
# pragma warning(disable:4365)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1500)

#include <xercesc/util/XMLChTranscoder.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
# pragma warning(pop)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1500)

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/*
	Setup for static or dynamic linking.

	Define MLB__LIB__SO_XercesUtils if the library is to be built (or linked)
	dynamically.
*/
// ////////////////////////////////////////////////////////////////////////////

#define MLB__LIB__SO_XercesUtils

#ifdef MLB__LIB__SO_XercesUtils
# ifdef XERCESCUTILS_DLL_EXPORTS
#  define API_XERCESUTILS		MB_LIB_EXPORT
# else
#  define API_XERCESUTILS		MB_LIB_IMPORT
# endif // #ifdef XERCESCUTILS_DLL_EXPORTS
#else
#  define API_XERCESUTILS
#endif // #ifdef MLB__LIB__XercesUtils

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_XERCESUTILS

// ////////////////////////////////////////////////////////////////////////////

/**
	\namespace MLB

	\brief The corporate namespace.
*/
namespace MLB {

/**
	\namespace XercesUtils

	\brief The Xerces library utility project namespace.
*/
namespace XercesUtils {

// ////////////////////////////////////////////////////////////////////////////
std::string API_XERCESUTILS &XmlStringToString(const XMLCh *in_string,
	std::string &out_string);
std::string API_XERCESUTILS  XmlStringToString(const XMLCh *in_string);
// ////////////////////////////////////////////////////////////////////////////

} // namespace XercesUtils

} // namespace MLB

#endif // #ifndef HH__MLB__XercesUtils__XercesUtils_hpp__HH

