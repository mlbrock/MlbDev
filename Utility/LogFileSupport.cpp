//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the log handler file class.

	Revision History	:	2010-10-23 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogFileSupport.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>
#include <Utility/PathName.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
void CheckFileNameBaseSegment(const std::string &string_value,
	const char *string_name, bool is_required)
{
	if (is_required)
		ThrowIfEmpty(string_value, string_name);

	if (!string_value.empty()) {
		if ((string_value[0] == '.') || (string_value.end()[-1] == '.')) {
			std::ostringstream o_str;
			o_str << string_name <<
				((string_value[0] == '.') ? " begins" : " ends") << " with the "
				"segment separator character ('.').";
				MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		std::string::const_iterator iter_b(string_value.begin());
		std::string::const_iterator iter_e(string_value.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			if ((!isalnum(*iter_b)) && (::strchr("_-.", *iter_b) == NULL)) {
				std::ostringstream o_str;
				o_str << string_name << " contains at least one invalid "
					"character (ASCII " << static_cast<unsigned int>(*iter_b) <<
					").";
				MLB::Utility::ThrowInvalidArgument(o_str.str());
			}
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ConstructFileNameBase(const std::string &dir_name,
	const std::string &env_name, const std::string &domain_name,
	const std::string &base_name, const std::string &ext_name,
	const MLB::Utility::TimeSpec &start_time, const std::string &host_name,
	const std::string &user_name, ProcessId process_id)
{
	std::string file_name_base;

	try {
		std::string tmp_dir_name;
		if (!dir_name.empty()) {
			tmp_dir_name = PathNameToGeneric(dir_name, "", true, true, false);
			if (!IsPathNameSlash(tmp_dir_name.end()[-1]))
				tmp_dir_name = PathNameToGeneric(
					CanonicalizePathNameSlashes(tmp_dir_name) + "/");
		}
		CheckFileNameBaseSegment(env_name, "The environment name", true);
		CheckFileNameBaseSegment(domain_name, "The domain name", false);
		CheckFileNameBaseSegment(base_name, "The base name", true);
		CheckFileNameBaseSegment(ext_name, "The extended name", false);
		CheckFileNameBaseSegment(host_name, "The host name", true);
		CheckFileNameBaseSegment(user_name, "The user name", true);
		std::ostringstream o_str;
		o_str << tmp_dir_name << env_name << ".";
		if (!domain_name.empty())
			o_str << domain_name << ".";
		o_str << base_name << ".";
		if (!ext_name.empty())
			o_str << ext_name << ".";
		char tmp_buffer[MLB::Utility::Length_TimeSpec + 1];
		start_time.ToString(tmp_buffer);
		tmp_buffer[ 4] = '_';
		tmp_buffer[ 7] = '_';
		tmp_buffer[10] = '_';
		tmp_buffer[13] = '_';
		tmp_buffer[16] = '_';
		tmp_buffer[19] = '_';
		o_str << tmp_buffer << "." << host_name << "." << user_name << "." <<
			ZeroFill(process_id, 10) << ".";
		file_name_base = o_str.str();
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Unable to construct a file name base string for directory "
			"name '" << dir_name << "', environment name '" << env_name <<
			"', domain name '" << domain_name << "', base name '" << base_name <<
			"', extended name '" << ext_name << "', time '" << start_time <<
			"', host name '" << host_name << "', user name '" << user_name <<
			"' process identifier '" << process_id << "': " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(file_name_base);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string AppendToFileNameBase(const std::string &in_file_name_base,
	const std::string &new_segment)
{
	CheckFileNameBaseSegment(new_segment, "The new file name segment", true);

	if (in_file_name_base.empty())
		MLB::Utility::ThrowInvalidArgument("The file name base string is empty.");

	std::string out_file_name_base;

	if (in_file_name_base.end()[-1] == '.')
		out_file_name_base = in_file_name_base + new_segment;
	else
		out_file_name_base = in_file_name_base + "." + new_segment;

	return(out_file_name_base);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogFilePreConfigureParse::LogFilePreConfigureParse(int argc, char **argv,
	const char *log_dir_regex, const char *env_regex, const char *domain_regex,
	const char *ext_regex)
	:log_dir_("./")
	,env_name_()
	,domain_name_()
	,ext_name_()
{
	try {
		for (unsigned int count_1 = 1; count_1 < static_cast<unsigned int>(argc);
			++count_1) {
			if ((log_dir_regex != NULL) &&
				(ParseCmdLineArg::ParseDirectory(RegexParamNameAdaptor(
				log_dir_regex, argv[count_1]), count_1, argc, argv, log_dir_,
				"", false)))
				;
			else if ((env_regex != NULL) &&
				(ParseCmdLineArg::ParseCmdLineDatum(RegexParamNameAdaptor(
				env_regex, argv[count_1]), count_1, argc, argv, env_name_)))
				CheckFileNameBaseSegment(env_name_, "The environment name", true);
			else if ((domain_regex != NULL) &&
				(ParseCmdLineArg::ParseCmdLineDatum(RegexParamNameAdaptor(
				domain_regex, argv[count_1]), count_1, argc, argv, domain_name_)))
				CheckFileNameBaseSegment(domain_name_, "The domain name", true);
			else if ((ext_regex != NULL) &&
				(ParseCmdLineArg::ParseCmdLineDatum(RegexParamNameAdaptor(
				ext_regex, argv[count_1]), count_1, argc, argv, ext_name_)))
				CheckFileNameBaseSegment(ext_name_, "The extended name", true);
		}
		if (domain_regex != NULL)
			CheckFileNameBaseSegment(domain_name_, "The domain name", true);
		if (ext_regex != NULL)
			CheckFileNameBaseSegment(ext_name_, "The extended name", true);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to parse the command line log parameters: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogFilePreConfigureParse::~LogFilePreConfigureParse()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogFilePreConfigure::LogFilePreConfigure(LogManager &log_manager, int argc,
	char **argv, const std::string &base_name, const std::string &ext_name,
	const char *log_dir_regex, const char *env_regex, const char *domain_regex,
	const char *ext_regex)
	:start_time_()
	,file_name_base_()
	,argv_(argv, argv + argc)
	,start_dir_(GetCurrentPath())
	,log_dir_("./")
	,env_name_()
	,domain_name_()
	,base_name_(base_name)
	,ext_name_(ext_name)
	,log_file_name_()
	,log_file_is_open_(false)
{
	try {
		{
			LogFilePreConfigureParse parse_cmd_line(argc, argv, log_dir_regex,
				env_regex, domain_regex, ext_regex);
			if (!parse_cmd_line.log_dir_.empty())
				log_dir_.swap(parse_cmd_line.log_dir_);
			if (!parse_cmd_line.env_name_.empty())
				env_name_.swap(parse_cmd_line.env_name_);
			if (!parse_cmd_line.domain_name_.empty())
				domain_name_.swap(parse_cmd_line.domain_name_);
			if (!parse_cmd_line.ext_name_.empty())
				ext_name_.swap(parse_cmd_line.ext_name_);
		}
		{
			//	CODE NOTE: Test for directory existence...
			if (!ResolveDirectoryPath(log_dir_, "", false)) {
				try {
					CreatePathDirExtended(log_dir_, true);
				}
				catch (const std::exception &except) {
					Rethrow(except, "Attempt to resolve and/or create the "
						"specified logging directory failed: " +
						std::string(except.what()));
				}
			}
			std::string tmp_log_dir(PathNameToGeneric(log_dir_, "",
				false, true, false));
			if (!IsPathNameSlash(tmp_log_dir.end()[-1]))
				log_dir_ = GetCanonicalNativePath(tmp_log_dir +
					PathNameSeparatorCanonical_String);
			else
				log_dir_.swap(tmp_log_dir);
		}
		file_name_base_ = ConstructFileNameBase("", env_name_, domain_name_,
			base_name_, ext_name_, start_time_);
		log_file_name_ = AppendToFileNameBase(log_dir_ + file_name_base_,
			"TXTLOG") + ".log";
		try {
			boost::shared_ptr<LogHandlerFile> my_log_handler(
				new LogHandlerFile(log_file_name_));
			log_manager.HandlerInstall(my_log_handler);
		}
		catch (const std::exception &except) {
			Rethrow(except, "Unable to install a log file and/or create log "
				"file '" + log_file_name_ + "': " + std::string(except.what()));
		}
		log_file_is_open_ = true;
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to complete logging pre-configuration: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogFilePreConfigure::~LogFilePreConfigure()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogFilePreConfigure::swap(LogFilePreConfigure &other)
{
	start_time_.swap(other.start_time_);
	file_name_base_.swap(other.file_name_base_);
	argv_.swap(other.argv_);
	start_dir_.swap(other.start_dir_);
	log_dir_.swap(other.log_dir_);
	env_name_.swap(other.env_name_);
	domain_name_.swap(other.domain_name_);
	base_name_.swap(other.base_name_);
	ext_name_.swap(other.ext_name_);
	log_file_name_.swap(other.log_file_name_);
	std::swap(log_file_is_open_, other.log_file_is_open_);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB


#ifdef TEST_MAIN

// ////////////////////////////////////////////////////////////////////////////
/*
	Instantiate the log manager...
*/
LogManagerMacroDefinition(MB_LIB_LOCAL)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	using namespace MLB::Utility;

	int return_code = EXIT_SUCCESS;

	try {
		try {
			LogFilePreConfigure log_pre(MyLogManager, argc, argv,
				"BaseName1", "ExtName1");
			std::cout << "TEST 1 RESULTS: " << log_pre.log_file_name_ << std::endl;
		}
		catch (const std::exception &except) {
			std::cerr << "TEST 1 FAILURE: " << except.what() << std::endl;
		}
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

