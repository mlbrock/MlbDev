//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	VFast Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The VFast ParseCmdLineArg class include file.

	Revision History	:	2008-12-29 --- Creation.

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__VFastParseCmdLineArg_hpp__HH

#define HH__MLB__VFast__VFastParseCmdLineArg_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <VFast/PacketDecodeRegime.hpp>

#include <Utility/ParseCmdLineArg.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

//	////////////////////////////////////////////////////////////////////////////
struct VFastParseCmdLineArg :
	public MLB::Utility::ParseCmdLineArg {
	static bool ParseCmdLineXmlTemplateFile(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLinePacketDecodeRegime(unsigned int &current_index,
		int argc, char **argv, PacketDecodeRegime &out_datum);
	static bool ParseCmdLinePacketDecodeRegimeCme(unsigned int &current_index,
		int argc, char **argv, PacketDecodeRegime &out_datum);
	static bool ParseCmdLineFirstPacketIndex(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool ParseCmdLineMaxPacketCount(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool ParseCmdLineMaxMessageCount(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool ParseCmdLineIterationCount(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool ParseCmdLineDebugFlag(unsigned int &current_index, int argc,
		char **argv, bool &out_datum);
	static bool ParseCmdLineElapsedTimer(unsigned int &current_index, int argc,
		char **argv, bool &out_datum);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifndef HH__MLB__VFast__VFastParseCmdLineArg_hpp__HH

