// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The RvParseCmdLineArg class include file.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_RvParseCmdLineArg_hpp__HH

#define HH__MLB__RvUtilX_RvParseCmdLineArg_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////


#include <Utility/ParseCmdLineArg.hpp>

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
struct API_RVUTILX RvParseCmdLineArg : public MLB::Utility::ParseCmdLineArg {
	static bool ParseCmdLineService(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineNetwork(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineDaemon(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineServiceFT(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineNetworkFT(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineDaemonFT(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineServiceNetworkDaemon(unsigned int &current_index,
		int argc, char **argv, std::string &out_service, std::string &out_network,
		std::string &out_daemon);
	static bool ParseCmdLineServiceNetworkDaemonFT(unsigned int &current_index,
		int argc, char **argv, std::string &out_service, std::string &out_network,
		std::string &out_daemon);
	static bool ParseCmdLineFTGroupName(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum);
	static bool ParseCmdLineFTWeight(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool ParseCmdLineFTActiveGoal(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool ParseCmdLineFTHeartbeatInterval(unsigned int &current_index,
		int argc, char **argv, double &out_datum);
	static bool ParseCmdLineFTPreparationInterval(unsigned int &current_index,
		int argc, char **argv, double &out_datum);
	static bool ParseCmdLineFTActivationInterval(unsigned int &current_index,
		int argc, char **argv, double &out_datum);
	static bool ParseCmdLineFTParams(unsigned int &current_index, int argc,
		char **argv, std::string &group_name, unsigned int &weight,
		unsigned int &active_goal, double &heartbeat_interval,
		double &preparation_interval, double &activation_interval);

	static bool ParseCmdLineCombinedServiceNetwork(unsigned int &current_index,
		int argc, char **argv, SNDSpec &out_datum);

	static bool ParseCmdLineSubject(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool ParseCmdLineSubject(const char *param_value,
		std::string &out_datum);
	static bool ParseCmdLineSubject(const std::string &param_name,
		std::string &out_datum);
	static bool ParseCmdLineSubjectFull(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool ParseCmdLineSubjectFull(const char *param_value,
		std::string &out_datum);
	static bool ParseCmdLineSubjectFull(const std::string &param_name,
		std::string &out_datum);
	static bool ParseCmdLineSubjectElement(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool ParseCmdLineSubjectElement(const char *param_value,
		std::string &out_datum);
	static bool ParseCmdLineSubjectElement(const std::string &param_name,
		std::string &out_datum);

	static bool ParseCmdLineSNDSpec(const std::string &in_param,
		SNDSpec &out_spec, const std::string &delimiter = "/");
	static bool ParseCmdLineSNDSpec(
		const MLB::Utility::StringVector &param_name_list,
		unsigned int &current_index, int argc, char **argv, SNDSpec &out_spec,
		const std::string &delimiter = "/");
	static bool ParseCmdLineSNDSpec(unsigned int &current_index, int argc,
		char **argv, SNDSpec &out_spec, const std::string &delimiter = "/");
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvParseCmdLineArg_hpp__HH

