// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the argument parsing and validation
								class RvParseCmdLineArgFtSpec.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/InlineContainer.hpp>

#include <RvUtilX/RvParseCmdLineArgFtSpec.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArgFtSpec::ParseCmdLineFtSpec(const std::string &in_param,
	FtSpec &out_spec, const std::string &delimiter)
{
	out_spec = FtSpec::FromDelimitedString(in_param, delimiter);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArgFtSpec::ParseCmdLineFtSpec(
	const MLB::Utility::StringVector &param_name_list,
	unsigned int &current_index, int argc, char **argv, FtSpec &out_spec,
	const std::string &delimiter)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(param_name_list, current_index, argc, argv,
		tmp_string))
		return(false);

	return(ParseCmdLineFtSpec(tmp_string, out_spec, delimiter));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArgFtSpec::ParseCmdLineFtSpec(unsigned int &current_index,
	int argc, char **argv, FtSpec &out_spec, const std::string &delimiter)
{
	return(ParseCmdLineFtSpec(MLB::Utility::MakeInlineVector<std::string>
		("-FT_SPECIFICATION")
		("-FTSPECIFICATION")
		("-FT_SPEC")
		("-FTSPEC"),
		current_index, argc, argv, out_spec, delimiter));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

