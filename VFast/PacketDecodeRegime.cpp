// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for the PacketDecodeRegime enumeration.

	Revision History	:	2008-11-26 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/PacketDecodeRegime.hpp>
#include <VFast/VFastException.hpp>

#include <Utility/C_StringSupport.hpp>
#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

namespace {
//	////////////////////////////////////////////////////////////////////////////
const char *PacketDecodeRegimeNameList[PacketDecodeRegime_Count] = {
	"NONE",
	"EBS",
	"CME",
	"ISE",
	"CME2",
	"XDP"
};
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
bool PacketDecodeRegimeIsValid(PacketDecodeRegime in_datum)
{
	return((in_datum >= PacketDecodeRegime_Min) &&
		(in_datum <= PacketDecodeRegime_Max));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
PacketDecodeRegime CheckPacketDecodeRegime(PacketDecodeRegime in_datum)
{
	if (!PacketDecodeRegimeIsValid(in_datum))
		MLB::Utility::ThrowInvalidArgument("Invalid packet decode regime (" +
			PacketDecodeRegimeToString(in_datum) + ").");
		
	return(in_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsPacketDecodeRegimeString(const std::string &in_datum,
	PacketDecodeRegime &out_datum)
{
	if (!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "NONE"))
		out_datum = PacketDecodeRegime_None;
	else if ((!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "EUREX")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "XETRA")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CEF"))   ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "EBS")))
		out_datum = PacketDecodeRegime_EBS;
	else if ((!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME"))     ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME1"))    ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME 1"))   ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME-1"))   ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME_1"))   ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME1.0"))  ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME 1.0")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME-1.0")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME_1.0")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME1.X"))  ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME 1.X")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME-1.X")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME_1.X")))
		out_datum = PacketDecodeRegime_CME;
	else if (!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "ISE"))
		out_datum = PacketDecodeRegime_ISE;
	else if ((!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME2"))    ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME 2"))   ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME-2"))   ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME_2"))   ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME2.0"))  ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME 2.0")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME-2.0")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME_2.0")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME2.X"))  ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME 2.X")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME-2.X")) ||
				(!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "CME_2.X")))
		out_datum = PacketDecodeRegime_CME_2;
	else if (!::MLB::Utility::Utility_stricmp(in_datum.c_str(), "XDP"))
		out_datum = PacketDecodeRegime_XDP;
	else
		return(false);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsPacketDecodeRegimeString(const std::string &in_datum)
{
	PacketDecodeRegime out_datum;

	return(IsPacketDecodeRegimeString(in_datum, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsPacketDecodeRegimeStringCme(const std::string &in_datum,
	PacketDecodeRegime &out_datum)
{
	if (!IsPacketDecodeRegimeString(in_datum, out_datum))
		return(false);

	return((out_datum == PacketDecodeRegime_CME_1) ||
		(out_datum == PacketDecodeRegime_CME_2));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsPacketDecodeRegimeStringCme(const std::string &in_datum)
{
	PacketDecodeRegime out_datum;

	return(IsPacketDecodeRegimeStringCme(in_datum, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string PacketDecodeRegimeToString(PacketDecodeRegime in_datum)
{
	return((PacketDecodeRegimeIsValid(in_datum)) ?
		PacketDecodeRegimeNameList[in_datum] :
		MLB::Utility::ValueToStringHex(in_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
PacketDecodeRegime StringToPacketDecodeRegime(const std::string &in_datum)
{
	PacketDecodeRegime out_datum = PacketDecodeRegime_None;

	if (!IsPacketDecodeRegimeString(in_datum, out_datum))
		MLB::Utility::ThrowInvalidArgument("Invalid packet decode regime "
			"string '" + in_datum + "': expected one of 'NONE', 'CME-1', "
			"'CME-2', 'ISE', 'EBS', or 'XDP'.");

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
PacketDecodeRegime StringToPacketDecodeRegimeCme(const std::string &in_datum)
{
	PacketDecodeRegime out_datum = PacketDecodeRegime_None;

	if (!IsPacketDecodeRegimeStringCme(in_datum, out_datum))
		MLB::Utility::ThrowInvalidArgument("Invalid packet decode regime "
			"string '" + in_datum + "': expected either 'CME-1' or 'CME-2'.");

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PacketDecodeRegime &datum)
{
	o_str
		<< PacketDecodeRegimeToString(datum);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

