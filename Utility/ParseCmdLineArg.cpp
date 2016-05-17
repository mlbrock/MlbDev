//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the argument parsing and validation
								class ParseCmdLineArg.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/Sleep.hpp>
#include <Utility/PathName.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::HasCmdLineHelp(int argc, char **argv, int first_index)
{
	return(HasCmdLineArg("-HELP", argc, argv, first_index) ||
		HasCmdLineArg("-H", argc, argv, first_index) ||
		HasCmdLineArg("-?", argc, argv, first_index));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::HasCmdLineArg(const char *arg_name, int argc, char **argv,
	int first_index)
{
	return(FindCmdLineArg(arg_name, argc, argv, first_index) > -1);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ParseCmdLineArg::FindCmdLineArg(const char *arg_name, int argc, char **argv,
	int first_index)
{
	CheckCmdLineArgList(argc, argv);

	if ((first_index >= 0) && (first_index < argc)) {
		while (first_index < argc) {
			if (!Utility_stricmp(argv[first_index], arg_name))
				return(first_index);
			++first_index;
		}
	}

	return(-1);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::IsCmdLineArg(const char *param_name,
	unsigned int current_index, int argc, char **argv)
{
	CheckCmdLineSource(current_index, argc, argv);

	return(Utility_stricmp(argv[current_index], param_name) == 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseDirectory(const std::string &param_name,
	unsigned int &current_index, int argc, char **argv, std::string &out_datum,
	const std::string &base_path, bool require_existence)
{
	if (!ParseCmdLineFollowing(param_name, current_index, argc, argv,
		out_datum))
		return(false);

	ResolveDirectoryPath(out_datum, base_path, require_existence);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseWorkingDir(unsigned int &current_index, int argc,
	char **argv, std::string &out_datum, const std::string &base_path,
	bool require_existence)
{
	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-WORKING_DIRECTORY")
		("-WORKINGDIRECTORY")
		("-WORKING_DIR")
		("-WORKINGDIR")
		("-WORK_DIRECTORY")
		("-WORKDIRECTORY")
		("-WORK_DIR")
		("-WORKDIR"),
		current_index, argc, argv, out_datum))
		return(false);

	ResolveDirectoryPath(out_datum, base_path, require_existence);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseLoggingDir(unsigned int &current_index, int argc,
	char **argv, std::string &out_datum, const std::string &base_path,
	bool require_existence)
{
	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-LOGGING_DIRECTORY")
		("-LOGGINGDIRECTORY")
		("-LOGGING_DIR")
		("-LOGGINGDIR")
		("-LOG_DIRECTORY")
		("-LOGDIRECTORY")
		("-LOG_DIR")
		("-LOGDIR"),
		current_index, argc, argv, out_datum))
		return(false);

	ResolveDirectoryPath(out_datum, base_path, require_existence);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseLoggingFlag(unsigned int &current_index, int argc,
	char **argv, bool &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowingSpec(MLB::Utility::MakeInlineVector<std::string>
		("-LOGGING_FLAG")
		("-LOGGINGFLAG")
		("-LOGGING")
		("-LOG_FLAG")
		("-LOGFLAG")
		("-LOG"),
		current_index, argc, argv, tmp_string))
		return(false);

	ParseCmdLineDatum(tmp_string, out_datum);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	bool &out_datum)
{
	if ((!Utility_stricmp(param_value.c_str(), "true")) || (param_value == "1") ||
		(!Utility_stricmp(param_value.c_str(), "on")) ||
		(!Utility_stricmp(param_value.c_str(), "yes")))
		out_datum = true;
	else if ((!Utility_stricmp(param_value.c_str(), "false")) ||
		(param_value == "0") || (!Utility_stricmp(param_value.c_str(), "off")) ||
		(!Utility_stricmp(param_value.c_str(), "no")))
		out_datum = false;
	else
		ThrowInvalidArgument(std::string("Unable to parse expected boolean "
			"value ('") + param_value + "').");
		
	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	signed char &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	unsigned char &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	signed short &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	unsigned short &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	signed int &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	unsigned int &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	signed long &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	unsigned long &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	signed long long &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	unsigned long long &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	float &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	double &out_datum)
{
	return(ParseCmdLineNumeric(param_value, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	std::string &out_datum)
{
	out_datum = param_value;

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	UniqueId &out_datum)
{
	out_datum = UniqueId::FromString(param_value);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	TimeT &out_datum)
{
	out_datum = TimeT::FromString(param_value);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	TimeVal &out_datum)
{
	out_datum = TimeVal::FromString(param_value);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	TimeSpec &out_datum)
{
	out_datum = TimeSpec::FromString(param_value);

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineFollowing(const std::string &param_name,
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowing(param_name.c_str(), current_index, argc,
		argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineFollowing(const char *param_name,
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	CheckCmdLineSource(current_index, argc, argv);

	if (Utility_stricmp(argv[current_index], param_name))
		return(false);

	RequireFollowingParameter(current_index, argc, argv);

	out_datum = argv[++current_index];

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineFollowingNotEmpty(
	const std::string &param_name, unsigned int &current_index, int argc,
	char **argv, std::string &out_datum)
{
	return(ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
		argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseCmdLineFollowingNotEmpty(const char *param_name,
	unsigned int &current_index, int argc, char **argv, std::string &out_datum)
{
	CheckCmdLineSource(current_index, argc, argv);

	if (Utility_stricmp(argv[current_index], param_name))
		return(false);

	RequireFollowingParameterNotEmpty(current_index, argc, argv);

	out_datum = argv[++current_index];

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseCmdLineArg::CheckCmdLineSource(unsigned int current_index,
	int argc, char **argv)
{
	CheckCmdLineArgList(argc, argv);

	if (current_index >= static_cast<unsigned int>(argc)) {
		std::ostringstream error_text;
		error_text << "Invalid argument list index (" << current_index <<
			"), argument count is " << argc << ".";
		ThrowInvalidArgument(error_text.str().c_str());
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseCmdLineArg::CheckCmdLineArgList(int argc, char **argv)
{
	if (argc < 0) {
		std::ostringstream error_text;
		error_text << "Invalid argument count (" << argc << ").";
		ThrowInvalidArgument(error_text);
	}

	if (argv == NULL)
		ThrowInvalidArgument("Invalid argument list (NULL).");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseCmdLineArg::RequireFollowingParameter(unsigned int current_index,
	int argc, char **argv)
{
	CheckCmdLineSource(current_index, argc, argv);

	if ((current_index + 1) == static_cast<unsigned int>(argc)) {
		std::ostringstream error_text;
		error_text << "Expected a command-line qualifier to follow the '" <<
			argv[current_index] << "' parameter.";
		ThrowInvalidArgument(error_text.str().c_str());
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseCmdLineArg::RequireFollowingParameterNotEmpty(
	unsigned int current_index, int argc, char **argv)
{
	RequireFollowingParameter(current_index, argc, argv);

	if (!(*argv[current_index])) {
		std::ostringstream error_text;
		error_text << "The command-line qualifier following the '" <<
			argv[current_index] << "' parameter is an empty string.";
		ThrowInvalidArgument(error_text.str().c_str());
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseCmdLineArg::InvalidArgument(const char *parameter)
{
	std::ostringstream error_text;

	error_text << "Invalid parameter '" << parameter <<
		"' was encountered on the command line.";

	ThrowInvalidArgument(error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseCmdLineArg::CheckDirectoryPath(const std::string &param_name,
	std::string &in_path, const std::string &base_path, bool require_existence)
{
	CheckFilePathGeneral(param_name, in_path, base_path, require_existence,
		true, false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseCmdLineArg::CheckFilePath(const std::string &param_name,
	std::string &in_path, const std::string &base_path, bool require_existence)
{
	CheckFilePathGeneral(param_name, in_path, base_path, require_existence,
		false, true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseCmdLineArg::CheckFilePathGeneral(const std::string &param_name,
	std::string &in_path, const std::string &base_path, bool require_existence,
	bool is_directory, bool is_file)
{
	try {
		ResolveFilePathGeneral(in_path, in_path, base_path, require_existence,
			is_directory, is_file);
	}
	catch (const std::exception &except) {
		MLB::Utility::ThrowException("The specified " + param_name +
			" parameter could not be resolved: " + except.what());
	}
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ParseCmdLineArg::GetMainWaitMSecsHelpShort()
{
	return("[ -main_function_sleep_milliseconds <milliseconds-to-sleep> ]");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ParseCmdLineArg::GetMainWaitMSecsHelpLong()
{
	return("\
   -main_function_sleep_milliseconds <milliseconds-to-sleep>\n\
      Specifies the number of milliseconds the program is to sleep in main()\n\
      before beginning operation.\n\n\
      The intent of this parameter is to impose a delay to assist in program\n\
      debugging and problem resolution.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseMainWaitMSecs(unsigned int &current_index, int argc,
	char **argv, unsigned int &out_datum)
{
	return(ParseCmdLineDatum(RegexParamNameAdaptor(
//		"\\-MAIN(_*FUNC(TION)*)*_*SLEEP(_*((M(ILLI)*SEC(OND)*)S*))*(_*TIMES*)*$",
		"^--?MAIN((-|_)?FUNC(TION)?)?(-|_)?SLEEP((-|_)?((M(ILLI)?SEC(OND)?)S?))?"
			"((-|_)?TIMES?)?$",
		current_index, argc, argv), current_index, argc, argv, out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ParseCmdLineArg::ParseMainWaitMSecs(unsigned int &current_index, int argc,
	char **argv)
{
	unsigned int out_datum;

	return(ParseMainWaitMSecs(current_index, argc, argv,
		out_datum));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int ParseCmdLineArg::GetMainWaitMSecs(int argc, char **argv)
{
	unsigned int sleep_msecs;
	unsigned int count_1;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		try {
			if (ParseMainWaitMSecs(count_1, argc, argv, sleep_msecs))
				return(sleep_msecs);
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Unable to parse the main() sleep "
				"milliseconds value from the command line: " +
				std::string(except.what()));
		}
	}

	return(0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int ParseCmdLineArg::PerformMainWaitMSecs(int argc, char **argv,
	std::ostream &o_str)
{
	unsigned int sleep_msecs = GetMainWaitMSecs(argc, argv);

	if (sleep_msecs) {
		o_str << "Sleeping within main() for " << sleep_msecs <<
			" milliseconds..." << std::endl;
		SleepMilliSecs(sleep_msecs);
		o_str << "Sleep within main() is complete." << std::endl;
	}

	return(sleep_msecs);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

