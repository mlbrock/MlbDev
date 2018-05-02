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

#include <boost/algorithm/string.hpp>

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into MlbDev/include/Utility/MapUtf8ToAscii.hpp .
typedef std::pair<const char *, const char *> Utf8MapEntry;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &MapUtf8ToAsciiInPlace(const std::size_t map_count,
	const Utf8MapEntry *map_list,  std::string &src);
std::string  MapUtf8ToAscii(const std::size_t map_count,
	const Utf8MapEntry *map_list,  const std::string &src);

std::string &MapUtf8ToAscii7InPlace(std::string &src);
std::string  MapUtf8ToAscii7(const std::string &src);

std::string &MapUtf8ToAscii8InPlace(std::string &src);
std::string  MapUtf8ToAscii8(const std::string &src);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
const Utf8MapEntry UTF8To7BitAsciiList[] = {
	Utf8MapEntry("\xE2\x80\x98", "'"),	// U+2018 (LEFT SINGLE QUOTATION MARK)
	Utf8MapEntry("\xE2\x80\x99", "'"),	// U+2019 (RIGHT SINGLE QUOTATION MARK)
	Utf8MapEntry("\xE2\x80\x9C", "\""),	// U+201C (LEFT DOUBLE QUOTATION MARK)
	Utf8MapEntry("\xE2\x80\x9D", "\""),	// U+201D (RIGHTT DOUBLE QUOTATION MARK)
	Utf8MapEntry("\xE2\x80\xA2", "."),	// U+2022 (BULLET)
	Utf8MapEntry("\xE2\x80\x93", "-") 	// U+2013 (EN DASH)
};
const std::size_t  UTF8To7BitAsciiCount  =
	sizeof(UTF8To7BitAsciiList) / sizeof(UTF8To7BitAsciiList[0]);
//	----------------------------------------------------------------------------
const Utf8MapEntry UTF8To8BitAsciiList[] = {
	Utf8MapEntry("\xE2\x80\x98", "‘"),	// U+2018 (LEFT SINGLE QUOTATION MARK)
	Utf8MapEntry("\xE2\x80\x99", "’"),	// U+2019 (RIGHT SINGLE QUOTATION MARK)
	Utf8MapEntry("\xE2\x80\x9C", "“"),	// U+201C (LEFT DOUBLE QUOTATION MARK)
	Utf8MapEntry("\xE2\x80\x9D", "”"),	// U+201D (RIGHTT DOUBLE QUOTATION MARK)
	Utf8MapEntry("\xE2\x80\xA2", "•"),	// U+2022 (BULLET)
	Utf8MapEntry("\xE2\x80\x93", "–")	// U+2013 (EN DASH)
};
const std::size_t  UTF8To8BitAsciiCount  =
	sizeof(UTF8To8BitAsciiList) / sizeof(UTF8To8BitAsciiList[0]);
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into MlbDev/Utility/MapUtf8ToAscii.cpp .
std::string &MapUtf8ToAsciiInPlace(const std::size_t map_count,
	const Utf8MapEntry *map_list,  std::string &src)
{
	if (src.empty())
		return(src);

	for (std::size_t count_1 = 0; count_1 < map_count; ++count_1)
		boost::replace_all(src, map_list[count_1].first,
			map_list[count_1].second);

	return(src);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into MlbDev/Utility/MapUtf8ToAscii.cpp .
std::string MapUtf8ToAscii(const std::size_t map_count,
	const Utf8MapEntry *map_list,  const std::string &src)
{
	std::string dst(src);

	return(MapUtf8ToAsciiInPlace(map_count, map_list, dst));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into MlbDev/Utility/MapUtf8ToAscii.cpp .
std::string &MapUtf8ToAscii7InPlace(std::string &src)
{
	return(MapUtf8ToAsciiInPlace(UTF8To7BitAsciiCount,
		UTF8To7BitAsciiList, src));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into MlbDev/Utility/MapUtf8ToAscii.cpp .
std::string MapUtf8ToAscii7(const std::string &src)
{
	return(MapUtf8ToAscii(UTF8To7BitAsciiCount, UTF8To7BitAsciiList, src));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into MlbDev/Utility/MapUtf8ToAscii.cpp .
std::string &MapUtf8ToAscii8InPlace(std::string &src)
{
	return(MapUtf8ToAsciiInPlace(UTF8To8BitAsciiCount,
		UTF8To8BitAsciiList, src));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved into MlbDev/Utility/MapUtf8ToAscii.cpp .
std::string MapUtf8ToAscii8(const std::string &src)
{
	return(MapUtf8ToAscii(UTF8To8BitAsciiCount, UTF8To8BitAsciiList, src));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB


// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
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

