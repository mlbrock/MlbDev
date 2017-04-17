//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of logging management.

	Revision History	:	1993-10-02 --- Creation of predecessor 'mlog' facility.
									Michael L. Brock
								2005-01-02 --- New ostream-based log model.
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogManager.hpp>
#include <Utility/EnumFlagOps.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/PathName.hpp>
#include <Utility/FilesystemSupport.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4217 4242 4668)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/shared_array.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4217 4668)
# if _MSC_VER >= 1500
#  pragma warning(disable: 4347 4360 4571)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/exception.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <fstream>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {



namespace {
//	////////////////////////////////////////////////////////////////////////////
const char         *LogTextList[] = {
	"LITERAL  ",
	"SPAM     ",
	"MINUTIAE ",
	"DEBUG    ",
	"DETAIL   ",
	"INFO     ",
	"NOTICE   ",
	"WARNING  ",
	"ERROR    ",
	"CRITICAL ",
	"ALERT    ",
	"EMERGENCY",
	"FATAL    "
};
const unsigned int  LogTextCount  =
	sizeof(LogTextList) / sizeof(LogTextList[0]);
const char         *LogTextListSimple[] = {
	"LITERAL",
	"SPAM",
	"MINUTIAE",
	"DEBUG",
	"DETAIL",
	"INFO",
	"NOTICE",
	"WARNING",
	"ERROR",
	"CRITICAL",
	"ALERT",
	"EMERGENCY",
	"FATAL"
};
//	////////////////////////////////////////////////////////////////////////////
} //	Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
const char *ConvertLogLevelToTextRaw(LogLevel log_level)
{
	return(((static_cast<int>(log_level) < 0) ||
		(static_cast<unsigned int>(log_level) >= LogTextCount)) ? "*********" :
		LogTextList[static_cast<int>(log_level)]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &ConvertLogLevelToText(LogLevel log_level,
	std::string &out_string)
{
	return(out_string.assign(ConvertLogLevelToTextRaw(log_level)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string ConvertLogLevelToText(LogLevel log_level)
{
	return(ConvertLogLevelToTextRaw(log_level));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *ConvertLogLevelToTextSimpleRaw(LogLevel log_level)
{
	return(((static_cast<int>(log_level) < 0) ||
		(static_cast<unsigned int>(log_level) >= LogTextCount)) ? "*********" :
		LogTextListSimple[static_cast<int>(log_level)]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &ConvertLogLevelToTextSimple(LogLevel log_level,
	std::string &out_string)
{
	return(out_string.assign(ConvertLogLevelToTextSimpleRaw(log_level)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string ConvertLogLevelToTextSimple(LogLevel log_level)
{
	return(ConvertLogLevelToTextSimpleRaw(log_level));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogLevel CheckLogLevel(LogLevel log_level)
{
	if ((log_level < LogLevel_Minimum) || (log_level > LogLevel_Maximum))
		ThrowInvalidArgument("Invalid log level encountered (" +
			AnyToString(log_level) + ") --- permissible values are from ('" +
			ConvertLogLevelToTextSimple(LogLevel_Minimum) + "' = " +
			AnyToString(LogLevel_Minimum) + ") to ('" +
			ConvertLogLevelToTextSimple(LogLevel_Maximum) + "' = " +
			AnyToString(LogLevel_Maximum) + "), inclusive.");

	return(log_level);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogLevelPair LogLevelFlagsToLevels(LogLevelFlag log_level_flags)
{
	unsigned int tmp_flags =
		static_cast<unsigned int>(log_level_flags) & LogFlag_Mask;

	if (!tmp_flags)
		ThrowInvalidArgument("Invalid log level flags parameter (" +
			ValueToStringHex(log_level_flags) + " = " +
			AnyToString(log_level_flags) + ").");

	bool         low_done_flag = false;
	LogLevelPair tmp_pair(LogLevel_Minimum, LogLevel_Minimum);

	while (tmp_flags) {
		if (!low_done_flag) {
			if (tmp_flags & 1)
				low_done_flag = true;
			else
				tmp_pair.first = EnumValueIncrement(tmp_pair.first);
		}
		if (tmp_flags == 1)
			break;
		tmp_pair.second = EnumValueIncrement(tmp_pair.second);
		tmp_flags >>= 1;
	}

	return(tmp_pair);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
StringVector &GetLogLevelNameList(StringVector &name_list, bool simple_flag)
{
	StringVector tmp_name_list;
	unsigned int count_1;

	for (count_1 = 0; count_1 < LogTextCount; ++count_1)
		tmp_name_list.push_back((simple_flag) ? LogTextListSimple[count_1] :
			LogTextList[count_1]);

	name_list.swap(tmp_name_list);

	return(name_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
StringVector GetLogLevelNameList(bool simple_flag)
{
	StringVector name_list;

	return(GetLogLevelNameList(name_list, simple_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
StringVector &GetLogLevelNameListSimple(StringVector &name_list)
{
	return(GetLogLevelNameList(name_list, true));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
StringVector GetLogLevelNameListSimple()
{
	StringVector name_list;

	return(GetLogLevelNameListSimple(name_list));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogLevelPair LogManager::GetLogLevelConsole() const
{
	return(LogLevelFlagsToLevels(log_level_screen_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogLevelPair LogManager::GetLogLevelFile() const
{
	return(LogLevelFlagsToLevels(log_level_persistent_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogLevelPair LogManager::SetLogLevelConsole(LogLevel min_log_level,
	LogLevel max_log_level)
{
	boost::mutex::scoped_lock my_lock(the_lock_);
	LogLevelPair              old_levels(GetLogLevelConsole());

	log_level_screen_ = GetLogLevelMask(min_log_level, max_log_level);

	return(old_levels);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogLevelPair LogManager::SetLogLevelFile(LogLevel min_log_level,
	LogLevel max_log_level)
{
	boost::mutex::scoped_lock my_lock(the_lock_);
	LogLevelPair              old_levels(GetLogLevelFile());

	log_level_persistent_ = GetLogLevelMask(min_log_level, max_log_level);

	return(old_levels);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerConsole::LogHandlerConsole()
	:LogHandler()
	,the_lock_()
	,iostreams_init_()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerConsole::~LogHandlerConsole()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerConsole::InstallHandler()
{
	boost::mutex::scoped_lock my_lock(the_lock_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerConsole::RemoveHandler()
{
	boost::mutex::scoped_lock my_lock(the_lock_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerConsole::EmitLine(const LogEmitControl &emit_control)
{
	if (emit_control.ShouldLogScreen()) {
		boost::mutex::scoped_lock my_lock(the_lock_);
		emit_control.UpdateTime();
		std::cout.write(emit_control.GetLeaderPtr(),
			static_cast<std::streamsize>(emit_control.GetLeaderLength()));
		std::cout.write(emit_control.line_buffer_.c_str(),
			static_cast<std::streamsize>(emit_control.line_buffer_.size()));
		std::cout << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerConsole::EmitLiteral(unsigned int literal_length,
	const char *literal_string)
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	std::cout.write(literal_string, static_cast<std::streamsize>(literal_length));
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerConsole::EmitLiteral(const LogEmitControl &emit_control,
	unsigned int literal_length, const char *literal_string)
{
	if (emit_control.ShouldLogScreen())
		EmitLiteral(literal_length, literal_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile()
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(Default)
	,the_lock_()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const char *file_name, LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(file_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const std::string &file_name,
	LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(file_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const char *base_name, const char *dir_name,
	LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(base_name, dir_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const std::string &base_name,
	const std::string &dir_name, LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(base_name, dir_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const char *base_name, const char *dir_name,
	const MLB::Utility::TimeT &start_time, LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(base_name, dir_name, start_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time,
	LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(base_name, dir_name, start_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::~LogHandlerFile()
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	if (out_file_ptr_->is_open()) {
		out_file_ptr_->flush();
		out_file_ptr_->close();
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::InstallHandler()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::RemoveHandler()
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	if (out_file_ptr_->is_open())
		out_file_ptr_->flush();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::EmitLine(const LogEmitControl &emit_control)
{
	if (emit_control.ShouldLogPersistent() || emit_control.ShouldLogScreen()) {
		boost::mutex::scoped_lock my_lock(the_lock_);
		emit_control.UpdateTime();
		if (emit_control.ShouldLogPersistent() && (out_file_ptr_ != NULL)) {
			out_file_ptr_->write(emit_control.GetLeaderPtr(),
				static_cast<std::streamsize>(emit_control.GetLeaderLength()));
			out_file_ptr_->write(emit_control.line_buffer_.c_str(),
				static_cast<std::streamsize>(emit_control.line_buffer_.size()));
			*out_file_ptr_ << std::endl;
// CODE NOTE: Shouldn't flush unless actually desired by user.
//			out_file_ptr_->flush();
		}
		if ((!(my_flags_ & NoConsoleOutput)) && emit_control.ShouldLogScreen()) {
			std::cout.write(emit_control.GetLeaderPtr(),
				static_cast<std::streamsize>(emit_control.GetLeaderLength()));
			std::cout.write(emit_control.line_buffer_.c_str(),
				static_cast<std::streamsize>(emit_control.line_buffer_.size()));
			std::cout << std::endl;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::EmitLiteral(unsigned int literal_length,
	const char *literal_string)
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	if (out_file_ptr_ != NULL) {
		out_file_ptr_->write(literal_string,
			static_cast<std::streamsize>(literal_length));
		*out_file_ptr_ << std::endl;
	}

	if (!(my_flags_ & NoConsoleOutput)) {
		std::cout.write(literal_string,
			static_cast<std::streamsize>(literal_length));
		std::cout << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::EmitLiteral(const LogEmitControl &emit_control,
	unsigned int literal_length, const char *literal_string)
{
	if (emit_control.ShouldLogPersistent() || emit_control.ShouldLogScreen()) {
		boost::mutex::scoped_lock my_lock(the_lock_);
		if (emit_control.ShouldLogPersistent() && (out_file_ptr_ != NULL)) {
			out_file_ptr_->write(literal_string,
				static_cast<std::streamsize>(literal_length));
			*out_file_ptr_ << std::endl;
		}
		if ((!(my_flags_ & NoConsoleOutput)) && emit_control.ShouldLogScreen()) {
			std::cout.write(literal_string,
				static_cast<std::streamsize>(literal_length));
			std::cout << std::endl;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const char *file_name)
{
	if ((file_name == NULL) || (!(*file_name)))
		ThrowException("Specified log file name is NULL or an empty string.");

	try {
		boost::shared_ptr<std::ofstream> tmp_file_ptr(
			new std::ofstream(file_name,
				(!(my_flags_ & DoNotAppend)) ?
				(std::ios_base::app | std::ios_base::ate) :
				(std::ios_base::app | std::ios_base::trunc)));
		if (tmp_file_ptr->fail())
			ThrowErrno("Open attempt failed.");
		{
			boost::mutex::scoped_lock my_lock(the_lock_);
			if ((out_file_ptr_ != NULL) && out_file_ptr_->is_open()) {
				out_file_ptr_->flush();
				out_file_ptr_->close();
				out_file_ptr_.reset();
			}
			out_file_ptr_  = tmp_file_ptr;
			out_file_name_ = file_name;
		}
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to open log file '" + std::string(file_name) +
			std::string("': ") + except.what());
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const std::string &file_name)
{
	OpenFile(file_name.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const char *base_name, const char *dir_name)
{
	OpenFile(base_name, dir_name, MLB::Utility::TimeT());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const std::string &base_name,
	const std::string &dir_name)
{
	OpenFile(base_name.c_str(), dir_name.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const char *base_name, const char *dir_name,
	const MLB::Utility::TimeT &start_time)
{
	std::string file_name;

	if ((base_name == NULL) || (!(*base_name)))
		file_name = "LogFile.";
	else {
		file_name = base_name;
		if (base_name[strlen(base_name) - 1] != '.')
			file_name += '.';
	}

	std::string tmp_date_time(start_time.ToString());

	tmp_date_time[10]  = '.';
	tmp_date_time[13]  = '.';
	tmp_date_time[16]  = '.';
	file_name         += tmp_date_time + "." + GetHostNameCanonical() + "." +
		AnyToString(CurrentProcessId()) + ".log";

	boost::filesystem::path tmp_file(BoostFs_ConstructNativePath(file_name));

	if ((dir_name != NULL) && *dir_name) {
		std::string tmp_dir_name;
		ResolveFilePathGeneral(dir_name, tmp_dir_name, "", true, true, false);
		boost::filesystem::path tmp_path(
			BoostFs_ConstructNativePath(tmp_dir_name));
		boost::filesystem::path this_file;
		this_file        = tmp_path / tmp_file;
		file_name        = BoostFs_GetNativeFileString(this_file);
	}
	else {
		if (!tmp_file.has_root_path())
			tmp_file = boost::filesystem::system_complete(tmp_file);
		file_name = BoostFs_GetNativeFileString(tmp_file);
	}

	OpenFile(file_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time)
{
	OpenFile(base_name.c_str(), dir_name.c_str(), start_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFileFlag LogHandlerFile::GetFlags() const
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	return(my_flags_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFileFlag LogHandlerFile::SetFlags(
	LogHandlerFileFlag new_flags)
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	LogHandlerFileFlag old_flags = my_flags_;

	my_flags_ = new_flags;

	return(old_flags);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string LogHandlerFile::GetFileName() const
{
	return(out_file_name_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogStream::~LogStream()
{
	try {
		boost::mutex::scoped_lock my_lock(the_lock_);
		thread_stream_map_.clear();
	}
	catch (const std::exception &) {
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogStream::LogSeparator(char sep_char, unsigned int text_length)
{
	GetThreadStream()->GetBufferPtrRef()->LogSeparator(sep_char, text_length);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogStream::ThreadStreamPtr LogStream::GetThreadStream()
{
	boost::mutex::scoped_lock my_lock(the_lock_);
	MLB::Utility::ThreadId    thread_id(MLB::Utility::CurrentThreadId());
	ThreadStreamMapIter       iter_f(thread_stream_map_.find(thread_id));

	if (iter_f != thread_stream_map_.end())
		return(iter_f->second);

	ThreadStreamBufferPtr buffer_ptr(
									new ThreadStreamBuffer(manager_ref_, log_level_));
	ThreadStreamPtr       ostream_ptr(
									new ThreadStream(manager_ref_, log_level_,
									buffer_ptr));

	thread_stream_map_[thread_id] = ostream_ptr;

	return(ostream_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool LogLevelParseCmdLineArg::ParseCmdLineDatum(const std::string &param_value,
	LogLevel &out_datum)
{
	std::string tmp_string(UpperCase(Trim(param_value)));

	if (tmp_string.empty())
		ThrowInvalidArgument("Log level is empty.");

	if (std::isdigit(tmp_string[0])) {
		int tmp_log_level;
		try {
			ParseCmdLineNumeric<int>(tmp_string, tmp_log_level,
				static_cast<int>(LogLevel_Minimum),
				static_cast<int>(LogLevel_Maximum));
		}
		catch (const std::exception &except) {
			Rethrow(except, "Unable to parse a log level numeric value: " +
				std::string(except.what()));
		}
		out_datum = static_cast<LogLevel>(tmp_log_level);
	}
	else {
		unsigned int count_1;
		for (count_1 = 0; count_1 < LogTextCount; ++count_1) {
			if (tmp_string == LogTextListSimple[count_1]) {
				out_datum = static_cast<LogLevel>(count_1);
				return(true);
			}
		}
		ThrowInvalidArgument("Log level '" + param_value + "' is invalid.");
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool LogLevelParseCmdLineArg::ParseLogLevelConsoleMin(
	unsigned int &current_index, int argc, char **argv, LogLevel &out_datum)
{
	return(ParseLogLevel(MLB::Utility::MakeInlineVector<std::string>
		("-LOG_LEVEL_CONSOLE_MINIMUM")
		("-LOG_LEVEL_CONSOLE_MIN")
		("-LOG_CONSOLE_MINIMUM")
		("-LOG_CONSOLE_MIN")
		("-LOG_LEVEL_SCREEN_MINIMUM")
		("-LOG_LEVEL_SCREEN_MIN")
		("-LOG_SCREEN_MINIMUM")
		("-LOG_SCREEN_MIN"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool LogLevelParseCmdLineArg::ParseLogLevelConsoleMax(
	unsigned int &current_index, int argc, char **argv, LogLevel &out_datum)
{
	return(ParseLogLevel(MLB::Utility::MakeInlineVector<std::string>
		("-LOG_LEVEL_CONSOLE_MAXIMUM")
		("-LOG_LEVEL_CONSOLE_MAX")
		("-LOG_CONSOLE_MAXIMUM")
		("-LOG_CONSOLE_MAX")
		("-LOG_LEVEL_CONS_MAXIMUM")
		("-LOG_LEVEL_CONS_MAX")
		("-LOG_CONS_MAXIMUM")
		("-LOG_CONS_MAX")
		("-LOG_LEVEL_SCREEN_MAXIMUM")
		("-LOG_LEVEL_SCREEN_MAX")
		("-LOG_SCREEN_MAXIMUM")
		("-LOG_SCREEN_MAX"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool LogLevelParseCmdLineArg::ParseLogLevelPersistentMin(
	unsigned int &current_index, int argc, char **argv, LogLevel &out_datum)
{
	return(ParseLogLevel(MLB::Utility::MakeInlineVector<std::string>
		("-LOG_LEVEL_PERSISTENT_MINIMUM")
		("-LOG_LEVEL_PERSISTENT_MIN")
		("-LOG_PERSISTENT_MINIMUM")
		("-LOG_PERSISTENT_MIN")
		("-LOG_LEVEL_FILE_MINIMUM")
		("-LOG_LEVEL_FILE_MIN")
		("-LOG_FILE_MINIMUM")
		("-LOG_FILE_MIN")
		//	Support for the spelling-challenged...
		("-LOG_LEVEL_PERSISTANT_MINIMUM")
		("-LOG_LEVEL_PERSISTANT_MIN")
		("-LOG_PERSISTANT_MINIMUM")
		("-LOG_PERSISTANT_MIN"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool LogLevelParseCmdLineArg::ParseLogLevelPersistentMax(
	unsigned int &current_index, int argc, char **argv, LogLevel &out_datum)
{
	return(ParseLogLevel(MLB::Utility::MakeInlineVector<std::string>
		("-LOG_LEVEL_PERSISTENT_MAXIMUM")
		("-LOG_LEVEL_PERSISTENT_MAX")
		("-LOG_PERSISTENT_MAXIMUM")
		("-LOG_PERSISTENT_MAX")
		("-LOG_LEVEL_FILE_MAXIMUM")
		("-LOG_LEVEL_FILE_MAX")
		("-LOG_FILE_MAXIMUM")
		("-LOG_FILE_MAX")
		//	Support for the spelling-challenged...
		("-LOG_LEVEL_PERSISTANT_MAXIMUM")
		("-LOG_LEVEL_PERSISTANT_MAX")
		("-LOG_PERSISTANT_MAXIMUM")
		("-LOG_PERSISTANT_MAX"),
		current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool LogLevelParseCmdLineArg::ParseLogLevel(const std::string &param_name,
	unsigned int &current_index, int argc, char **argv, LogLevel &out_datum)
{
	std::string tmp_string;

	if (!ParseCmdLineFollowing(param_name, current_index, argc, argv,
		tmp_string))
		return(false);

	return(ParseLogLevelInternal(argv[current_index - 1], tmp_string,
		out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool LogLevelParseCmdLineArg::ParseLogLevelInternal(
	const std::string &param_name, const std::string &param_value,
	LogLevel &out_datum)
{
	try {
		return(ParseCmdLineDatum(param_value, out_datum));
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to parse the log level which follows the '" +
			param_name + "' parameter: " + std::string(except.what()));
	}

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/Sleep.hpp>

using namespace MLB::Utility;

LogManagerMacroDefinition(MB_LIB_LOCAL)

#ifdef _Windows
# pragma warning(disable:4217 4625 4626 4668)
#endif // #ifdef _Windows

#include <boost/thread/thread.hpp> 

#ifdef _Windows
# pragma warning(default:4217 4625 4626 4668)
#endif // #ifdef _Windows

#include <iomanip>

//	////////////////////////////////////////////////////////////////////////////
const char         *MultiLineTestList[] = {
	"Line 1\nLine 2\n\nLine 4",
	"\nLine 2\n\n\nLine 5\n\n",
	"\n\nLine 3\n\n\nLine 6\n\n"
};
const unsigned int  MultiLineTestCount  =
	sizeof(MultiLineTestList) / sizeof(MultiLineTestList[0]);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static int          TEST_IsolationCharacter = 0;

static boost::mutex TEST_IsolationLock;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_MultiLineOperation()
{
	unsigned int count_1;

	std::cout << std::setfill('*') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
	std::cout << std::setfill('*') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	for (count_1 = 0; count_1 < MultiLineTestCount; ++count_1) {
		std::string test_string(MultiLineTestList[count_1]);
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		std::string tmp_string(test_string);
		std::string::size_type eol_pos = 0;
		while ((eol_pos = tmp_string.find('\n', eol_pos)) != std::string::npos)
			tmp_string[eol_pos] = '|';
		std::cout << tmp_string << std::endl;
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		LogEmitControl emit_control(Default, LogFlag_Mask, LogFlag_Mask,
			TimeSpec(), LogLevel_Info, LogFlag_Info, test_string);
		unsigned int  line_length;
		const char   *line_ptr;
		while ((line_ptr = emit_control.GetNextLine(line_length)) != NULL) {
			std::cout << emit_control.GetLeaderPtr();
			std::cout.write(line_ptr, static_cast<std::streamsize>(line_length));
			std::cout << std::endl;
		}
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_IsolationThreadProc()
{
	int this_value;

	{
		boost::mutex::scoped_lock my_lock(TEST_IsolationLock);
		this_value = TEST_IsolationCharacter++;
	}

	char this_char  = static_cast<char>(this_value) + 'A';

	LogInfo << "[" << CurrentThreadId() << "]";
	SleepMilliSecs(rand() % 100);
	LogInfo << "[" << this_char << "]";
	SleepMilliSecs(rand() % 100);
	LogInfo << TimeVal::Now();
	SleepMilliSecs(rand() % 100);
	LogInfo << "]";
	SleepMilliSecs(rand() % 100);
	LogInfo << "]: Character = '";
	SleepMilliSecs(rand() % 100);
	LogInfo << this_char;
	SleepMilliSecs(rand() % 100);
	LogInfo << "' (" << std::setw(2) << this_value << "): ";
	SleepMilliSecs(rand() % 100);
	LogInfo << std::setfill(this_char);
	SleepMilliSecs(rand() % 100);

	//	Delay threads managing the letter 'A' for an extended period...
	if (this_char == 'A')
		SleepMilliSecs(5000);

	LogInfo << std::setw(this_value);
	SleepMilliSecs(rand() % 100);
	LogInfo << "";
	SleepMilliSecs(rand() % 100);
	LogInfo << "--->DONE";
	SleepMilliSecs(rand() % 100);
	LogInfo << std::endl;

	SleepMilliSecs(rand() % 1000);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class TEST_IsolationThread : public boost::thread {
public:
	TEST_IsolationThread(int character) :
		 boost::thread(TEST_IsolationThreadProc)
		,character_(character) {
	}

	int character_;

private:
	MLB_Utility_NonCopyable_Macro(TEST_IsolationThread);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_Isolation()
{
	int                 count_1;
	boost::thread_group thr_group;

	for (count_1 = 0; count_1 < 26; ++count_1) {
//		thr_group.create_thread(&TEST_IsolationThreadProc);
		TEST_IsolationThread *this_thread = new TEST_IsolationThread(count_1);
		thr_group.add_thread(this_thread);
	}

	thr_group.join_all();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_NoEOL()
{
	LogInfo << "Line without explicit 'std::endl'";
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	TEST_MultiLineOperation();

	try {
		MyLogManager.SetLogLevelConsoleAll();
		MyLogManager.SetLogLevelFileAll();
		//	Create a LogHandlerFile...
		boost::shared_ptr<LogHandlerFile>
			my_log_handler(new LogHandlerFile("TestLogFile.VERSION_OLD_001.log"));
		//	... And install the handler...
		MyLogManager.HandlerInstall(my_log_handler);
		//	Some simple tests...
LogLiteral << std::string("LITERAL #1: std::string(hello, world)") << std::endl;
LogLiteral << std::string("LITERAL #2: std::string(hello, world)") << std::endl;
LogInfo    << std::string("std::string(hello, world)") << std::endl;
		LogInfo << "hello, world" << std::endl;
		LogInfo << 666 << std::endl;
		LogInfo << "Last line" << std::endl;
		LogInfo << std::endl;
		//	Write 100 formatted INFO lines...
		unsigned int count_1 = 0;
		while (count_1 < 100)
			LogInfo << std::setw(8) << ++count_1 << " " << std::setfill('*') <<
				std::setw(10) << "" << std::setfill(' ') << 666 << "=0x" <<
				std::hex << count_1 << std::dec << std::endl;
		//	Write 100 formatted ERROR lines...
		count_1 = 0;
		while (count_1 < 100)
			LogError << std::setw(8) << ++count_1 << " " << std::setfill('*') <<
				std::setw(10) << "" << std::setfill(' ') << std::endl;
		//	Perform the threading test...
		TEST_Isolation();
		//	This last line doesn't provide line termination... But should be
		//	emitted at the time the thread stream is destroyed...
		TEST_NoEOL();
		//	Write some separator strings...
		LogInfo << ">>>>> Following is a call to LogSeparator():" <<
			std::endl;
		LogInfo.LogSeparator();
		LogInfo << ">>>>> Following is a call to LogSeparator('='):" <<
			std::endl;
		LogInfo.LogSeparator('=');
		LogInfo << ">>>>> Following is a call to LogSeparator('=', 10):" <<
			std::endl;
		LogInfo.LogSeparator('=', 10);
		//	Emit a line for each of the log severity levels...
		LogInfo << ">>>>> Following we use LogToLevel() for each of the log "
			"levels: " << std::endl;
		LogLevel min_level = LogLevel_Minimum;
		LogLevel max_level = LogLevel_Maximum;
		unsigned int level_flags = 0;
		while (min_level <= max_level) {
			level_flags = 1 << static_cast<unsigned int>(min_level);
			LogInfo.LogToLevel(min_level, "Logging to level " +
				PadLeft(AnyToString(min_level), 2) + " (flag equivalent = " +
				ValueToStringHex(level_flags) + ").");
			min_level    =
				static_cast<LogLevel>(static_cast<unsigned int>(min_level) + 1);
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

