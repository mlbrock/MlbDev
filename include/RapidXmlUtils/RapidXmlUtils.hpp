// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB RapidXmlUtils Utilities Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	RapidXmlUtils.hpp

	File Description	:	RapidXml include file.

	Revision History	:	2015-03-21 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RapidXmlUtils__RapidXmlUtils_hpp__HH

#define HH__MLB__RapidXmlUtils__RapidXmlUtils_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
# pragma warning(push)
# pragma warning(disable: 4365 4625 4626)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1500)

#include <boost/property_tree/detail/rapidxml.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
# pragma warning(pop)
#endif // #if defined(_MSC_VER) && (_MSC_VER >= 1500)

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/*
	Setup for static or dynamic linking.

	Define MLB__LIB__SO_RapidXmlUtils if the library is to be built (or linked)
	dynamically.
*/
// ////////////////////////////////////////////////////////////////////////////

#define MLB__LIB__SO_RapidXmlUtils

#ifdef MLB__LIB__SO_RapidXmlUtils
# ifdef RAPIDXMLUTILS_DLL_EXPORTS
#  define API_RAPIDXMLUTILS		MB_LIB_EXPORT
# else
#  define API_RAPIDXMLUTILS		MB_LIB_IMPORT
# endif // #ifdef RAPIDXMLUTILS_DLL_EXPORTS
#else
#  define API_RAPIDXMLUTILS
#endif // #ifdef MLB__LIB__RapidXmlUtils

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE			API_RAPIDXMLUTILS

// ////////////////////////////////////////////////////////////////////////////

/**
	\namespace MLB

	\brief The corporate namespace.
*/
namespace MLB {

/**
	\namespace RapidXmlUtils

	\brief The RapidXml library utility project namespace.
*/
namespace RapidXmlUtils {

// ////////////////////////////////////////////////////////////////////////////
std::string API_RAPIDXMLUTILS &XmlStringToString(const char *in_string,
	std::string &out_string);
std::string API_RAPIDXMLUTILS  XmlStringToString(const char *in_string);
// ////////////////////////////////////////////////////////////////////////////

} // namespace RapidXmlUtils

} // namespace MLB

#endif // #ifndef HH__MLB__RapidXmlUtils__RapidXmlUtils_hpp__HH

