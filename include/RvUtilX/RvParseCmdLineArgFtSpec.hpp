// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The RvParseCmdLineArgFtSpec class include file.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_RvParseCmdLineArgFtSpec_hpp__HH

#define HH__MLB__RvUtilX_RvParseCmdLineArgFtSpec_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RvParseCmdLineArg.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
struct API_RVUTILX RvParseCmdLineArgFtSpec : public RvParseCmdLineArg {
	static bool ParseCmdLineFtSpec(const std::string &in_param, FtSpec &out_spec,
		const std::string &delimiter);
	static bool ParseCmdLineFtSpec(
		const MLB::Utility::StringVector &param_name_list,
		unsigned int &current_index, int argc, char **argv, FtSpec &out_spec,
		const std::string &delimiter);
	static bool ParseCmdLineFtSpec(unsigned int &current_index, int argc,
		char **argv, FtSpec &out_spec, const std::string &delimiter);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvParseCmdLineArgFtSpec_hpp__HH

