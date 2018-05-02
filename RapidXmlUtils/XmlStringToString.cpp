// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB RapidXmlUtils Utilities Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	XmlStringToString.cpp

	File Description	:	Implementation of logic to convert from RapidXml
								strings to C++ strings.

	Revision History	:	2015-03-21 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RapidXmlUtils/RapidXmlUtils.hpp>

#include <Utility/MapUtf8ToAscii.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RapidXmlUtils {

// ////////////////////////////////////////////////////////////////////////////
std::string &XmlStringToString(const char *in_string, std::string &out_string)
{
	//	To be compatible with the Xerces-C transcode() function.
	return(MLB::Utility::MapUtf8ToAscii8InPlace(out_string.assign(in_string)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string XmlStringToString(const char *in_string)
{
	std::string out_string;

	return(XmlStringToString(in_string, out_string));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RapidXmlUtils

} // namespace MLB

