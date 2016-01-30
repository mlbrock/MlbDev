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
								class RvParseCmdLineArg.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
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

#include <RvUtilX/RvParseCmdLineArg.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineService(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowing("-SERVICE", current_index, argc, argv,
		out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineNetwork(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowing("-NETWORK", current_index, argc, argv,
		out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineDaemon(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowing("-DAEMON", current_index, argc, argv,
		out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineServiceFT(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowing("-FTSERVICE", current_index, argc, argv,
		out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineNetworkFT(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowing("-FTNETWORK", current_index, argc, argv,
		out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineDaemonFT(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowing("-FTDAEMON", current_index, argc, argv,
		out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineServiceNetworkDaemon(
	unsigned int &current_index, int argc, char **argv, std::string &out_service,
	std::string &out_network, std::string &out_daemon)
{
	if (ParseCmdLineService(current_index, argc, argv, out_service))
		return(true);
	else if (ParseCmdLineNetwork(current_index, argc, argv, out_network))
		return(true);
	else if (ParseCmdLineDaemon(current_index, argc, argv, out_daemon))
		return(true);

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineServiceNetworkDaemonFT(
	unsigned int &current_index, int argc, char **argv, std::string &out_service,
	std::string &out_network, std::string &out_daemon)
{
	if (ParseCmdLineServiceFT(current_index, argc, argv, out_service))
		return(true);
	else if (ParseCmdLineNetworkFT(current_index, argc, argv, out_network))
		return(true);
	else if (ParseCmdLineDaemonFT(current_index, argc, argv, out_daemon))
		return(true);

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineFTGroupName(unsigned int &current_index,
	int argc, char **argv, std::string &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
			("-FT_GROUP_NAME")
			("-FT_GROUPNAME")
			("-FTGROUP_NAME")
			("-FTGROUPNAME")
			("-FT_GROUP")
			("-FTGROUP"),
		current_index, argc, argv, tmp_string))
		return(false);

	return(ParseCmdLineSubjectFull(tmp_string, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineFTWeight(unsigned int &current_index,
	int argc, char **argv, unsigned int &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
			("-FT_MEMBER_WEIGHT")
			("-FT_MEMBERWEIGHT")
			("-FTMEMBER_WEIGHT")
			("-FTMEMBERWEIGHT")
			("-FT_WEIGHT")
			("-FTWEIGHT"),
		current_index, argc, argv, tmp_string))
		return(false);

	//	Constant valid range specified by TibCo.
	return(ParseCmdLineNumeric<unsigned int>(tmp_string, out_datum, 1, 65535));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineFTActiveGoal(unsigned int &current_index,
	int argc, char **argv, unsigned int &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
			("-FT_ACTIVE_GOAL")
			("-FT_ACTIVEGOAL")
			("-FT_ACTIVE_GOAL")
			("-FTACTIVE_GOAL")
			("-FTACTIVEGOAL"),
		current_index, argc, argv, tmp_string))
		return(false);

	//	Constant valid range specified by TibCo.
	return(ParseCmdLineNumeric<unsigned int>(tmp_string, out_datum, 1, 65535));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineFTHeartbeatInterval(
	unsigned int &current_index, int argc, char **argv, double &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
			("-FT_HEARTBEAT_INTERVAL")
			("-FT_HEARTBEATINTERVAL")
			("-FTHEARTBEAT_INTERVAL")
			("-FTHEARTBEATINTERVAL")
			("-FT_HEARTBEAT")
			("-FTHEARTBEAT")
			("-FT_HEART_INTERVAL")
			("-FT_HEARTINTERVAL")
			("-FTHEART_INTERVAL")
			("-FTHEARTINTERVAL")
			("-FT_HEART")
			("-FTHEART"),
		current_index, argc, argv, tmp_string))
		return(false);

	//	Minimum valid value specified by TibCo.
	return(ParseCmdLineNumeric(tmp_string, out_datum, 0.001));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineFTPreparationInterval(
	unsigned int &current_index, int argc, char **argv, double &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
			("-FT_PREPARATION_INTERVAL")
			("-FT_PREPARATIONINTERVAL")
			("-FTPREPARATION_INTERVAL")
			("-FTPREPARATIONINTERVAL")
			("-FT_PREPARATION")
			("-FTPREPARATION")
			("-FT_PREP_INTERVAL")
			("-FT_PREPINTERVAL")
			("-FTPREP_INTERVAL")
			("-FTPREPINTERVAL")
			("-FT_PREP")
			("-FTPREP"),
		current_index, argc, argv, tmp_string))
		return(false);

	//	Minimum valid value specified by TibCo.
	return(ParseCmdLineNumeric(tmp_string, out_datum, 0.0));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineFTActivationInterval(
	unsigned int &current_index, int argc, char **argv, double &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
			("-FT_ACTIVATION_INTERVAL")
			("-FT_ACTIVATIONINTERVAL")
			("-FTACTIVATION_INTERVAL")
			("-FTACTIVATIONINTERVAL")
			("-FT_ACTIVATION")
			("-FTACTIVATION")
			("-FT_ACTIVE_INTERVAL")
			("-FT_ACTIVEINTERVAL")
			("-FTACTIVE_INTERVAL")
			("-FTACTIVEINTERVAL")
			("-FT_ACTIVE")
			("-FTACTIVE"),
		current_index, argc, argv, tmp_string))
		return(false);

	//	Minimum valid value specified by TibCo.
	return(ParseCmdLineNumeric(tmp_string, out_datum, 0.001));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineFTParams(unsigned int &current_index,
	int argc, char **argv, std::string &group_name, unsigned int &weight,
	unsigned int &active_goal, double &heartbeat_interval,
	double &preparation_interval, double &activation_interval)
{
	if (ParseCmdLineFTGroupName(current_index, argc, argv, group_name))
		return(true);
	else if (ParseCmdLineFTWeight(current_index, argc, argv, weight))
		return(true);
	else if (ParseCmdLineFTActiveGoal(current_index, argc, argv, active_goal))
		return(true);
	else if (ParseCmdLineFTHeartbeatInterval(current_index, argc, argv,
		heartbeat_interval))
		return(true);
	else if (ParseCmdLineFTPreparationInterval(current_index, argc, argv,
		preparation_interval))
		return(true);
	else if (ParseCmdLineFTActivationInterval(current_index, argc, argv,
		activation_interval))
		return(true);

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineCombinedServiceNetwork(
	unsigned int &current_index, int argc, char **argv, SNDSpec &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-SERVICE_NETWORK")
		("-SERVICENETWORK")
		("-SERVICE_NET")
		("-SERVICENET")
		("-SERV_NETWORK")
		("-SERVNETWORK")
		("-SERV_NET")
		("-SERVNET")
		("-SRV_NETWORK")
		("-SRVNETWORK")
		("-SRV_NET")
		("-SRVNET"),
		current_index, argc, argv, tmp_string))
		return(false);

	std::string::size_type found_pos = tmp_string.find(":");

	if (found_pos == std::string::npos)
		out_datum = SNDSpec(tmp_string, "", out_datum.daemon_);
	else if (!found_pos)
		out_datum = SNDSpec("", tmp_string.c_str() + 1, out_datum.daemon_);
	else
		out_datum = SNDSpec(tmp_string.substr(0, found_pos),
			tmp_string.substr(found_pos + 1), out_datum.daemon_);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubject(const std::string &param_name,
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, tmp_string))
		return(false);

	try {
		ParseCmdLineSubject(argv[current_index], out_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "The specified value for parameter '" +
			std::string(argv[current_index - 1]) + "' is in error: " +
			std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubject(const char *param_value,
	std::string &out_datum)
{
	try {
		SubjectNameFlat tmp_name(param_value);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Invalid subject name: " +
			std::string(except.what()));
	}

	out_datum = param_value;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubject(const std::string &param_name,
	std::string &out_datum)
{
	return(ParseCmdLineSubject(param_name.c_str(), out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubjectFull(const std::string &param_name,
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, tmp_string))
		return(false);

	try {
		ParseCmdLineSubjectFull(argv[current_index], out_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "The specified value for parameter '" +
			std::string(argv[current_index - 1]) + "' is in error: " +
			std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubjectFull(const char *param_value,
	std::string &out_datum)
{
	try {
		SubjectNameFlat tmp_name(param_value);
		tmp_name.CheckFullName();
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Invalid subject name: " +
			std::string(except.what()));
	}

	out_datum = param_value;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubjectFull(const std::string &param_name,
	std::string &out_datum)
{
	return(ParseCmdLineSubjectFull(param_name.c_str(), out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubjectElement(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, std::string &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, tmp_string))
		return(false);

	try {
		ParseCmdLineSubjectElement(argv[current_index], out_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "The specified value for parameter '" +
			std::string(argv[current_index - 1]) + "' is in error: " +
			std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubjectElement(const char *param_value,
	std::string &out_datum)
{
	try {
		SubjectNameFlat tmp_name(param_value);
		if (tmp_name.element_count_ > 1) {
			std::ostringstream error_text;
			error_text << "The specified subject name element ('" <<
				param_value << "') parses into more than one (" <<
				tmp_name.element_count_ << "Rendezvous subject elements.";
			MLB::Utility::ThrowInvalidArgument(error_text);
		}
		tmp_name.CheckFullName();
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Invalid subject name element: " +
			std::string(except.what()));
	}

	out_datum = param_value;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSubjectElement(
	const std::string &param_name, std::string &out_datum)
{
	return(ParseCmdLineSubjectElement(param_name.c_str(), out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSNDSpec(const std::string &in_param,
	SNDSpec &out_spec, const std::string &delimiter)
{
	out_spec = SNDSpec::FromDelimitedString(in_param, delimiter);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSNDSpec(
	const MLB::Utility::StringVector &param_name_list,
	unsigned int &current_index, int argc, char **argv, SNDSpec &out_spec,
	const std::string &delimiter)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(param_name_list, current_index, argc, argv,
		tmp_string))
		return(false);

	return(ParseCmdLineSNDSpec(tmp_string, out_spec, delimiter));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool RvParseCmdLineArg::ParseCmdLineSNDSpec(unsigned int &current_index,
	int argc, char **argv, SNDSpec &out_spec, const std::string &delimiter)
{
	return(ParseCmdLineSNDSpec(MLB::Utility::MakeInlineVector<std::string>
		("-SND_SPECIFICATION")
		("-SNDSPECIFICATION")
		("-SND_SPEC")
		("-SNDSPEC"),
		current_index, argc, argv, out_spec, delimiter));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

