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

	Revision History	:	1993-10-02 --- Creation of predecessor 'mlog' facility.
									Michael L. Brock
								2005-01-02 --- New ostream-based log model.
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogHandlerFile.hpp>

#include <fstream>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile()
	:LogHandlerFileBase()
	,out_file_ptr_()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const char *file_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(file_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const std::string &file_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(file_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const char *base_name, const char *dir_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(base_name, dir_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const std::string &base_name,
	const std::string &dir_name, LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(base_name, dir_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const char *base_name, const char *dir_name,
	const MLB::Utility::TimeT &start_time, LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(base_name, dir_name, start_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(base_name, dir_name, start_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::~LogHandlerXFile()
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	if ((out_file_ptr_ != NULL) && out_file_ptr_->is_open()) {
		out_file_ptr_->flush();
		out_file_ptr_->close();
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::InstallHandlerImpl()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::RemoveHandlerImpl()
{
	if (out_file_ptr_->is_open())
		out_file_ptr_->flush();
}
//	////////////////////////////////////////////////////////////////////////////

//	CODE NOTE: LogFileHandler buffering test code. To be removed.
static char TestFileBuffer[2000000];

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::OpenFileImpl(const char *file_name)
{
	boost::shared_ptr<std::ofstream> tmp_file_ptr(
		new std::ofstream(file_name,
			(!(my_flags_ & DoNotAppend)) ?
			(std::ios_base::app | std::ios_base::ate) :
			(std::ios_base::app | std::ios_base::trunc)));

	if (tmp_file_ptr->fail())
		ThrowErrno("Open attempt failed.");

//	CODE NOTE: LogFileHandler buffering test code. To be removed.
std::streambuf *new_buffer_ptr =
	tmp_file_ptr->rdbuf()->pubsetbuf(TestFileBuffer, sizeof(TestFileBuffer));
if (new_buffer_ptr == NULL)
	ThrowErrno("Attempt to set the log file buffer size to " +
		AnyToString(sizeof(TestFileBuffer)) + " bytes failed.");

	{
		std::string               tmp_file_name(file_name);
		boost::mutex::scoped_lock my_lock(the_lock_);
		if ((out_file_ptr_ != NULL) && out_file_ptr_->is_open()) {
			out_file_ptr_->flush();
			out_file_ptr_->close();
			out_file_ptr_.reset();
		}
		out_file_ptr_.swap(tmp_file_ptr);
		out_file_name_.swap(tmp_file_name);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::FlushImpl()
{
	if ((out_file_ptr_ != NULL) && out_file_ptr_->is_open())
		out_file_ptr_->flush();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::EmitLineImpl(const LogEmitControl &emit_control)
{
	if (out_file_ptr_ != NULL) {
		out_file_ptr_->write(emit_control.GetLeaderPtr(),
			static_cast<std::streamsize>(emit_control.GetLeaderLength()));
		out_file_ptr_->write(emit_control.line_buffer_.c_str(),
			static_cast<std::streamsize>(emit_control.line_buffer_.size()));
		*out_file_ptr_ << std::endl;
// CODE NOTE: Shouldn't flush unless actually desired by user.
//		out_file_ptr_->flush();
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::EmitLiteralImpl(unsigned int literal_length,
	const char *literal_string)
{
	if (out_file_ptr_ != NULL) {
		out_file_ptr_->write(literal_string,
			static_cast<std::streamsize>(literal_length));
		*out_file_ptr_ << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Sleep.hpp>
#include <Utility/HRTimer.hpp>
#include <Utility/StringSupport.hpp>

using namespace MLB::Utility;

LogManagerMacroDefinition(MB_LIB_LOCAL)

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4217 4350 4625 4626 4668)
#endif // #ifdef _Windows

#include <boost/thread/thread.hpp> 

#ifdef _Windows
# pragma warning(pop)
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
void TEST_StressLines()
{
	int         line_count  = 10000;
	int         line_length = 200;
	std::string test_string(line_length, 'X');

	MLB::Utility::HRTimer hr_timer;

	hr_timer.Start();

	for (int count_1 = 0; count_1 < line_count; ++count_1)
		LogDetail << std::setw(10) << count_1 << ": " << test_string << std::endl;

	hr_timer.End();

	LogInfo << "Time required to write " << line_count << " log lines of "
		"length " << line_length << ": " << std::setw(32) <<
		std::setprecision(12) << hr_timer.GetTickDiffInSeconds() <<
		" seconds." << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_StressSize()
{
	int         line_count  = 1;
	int         line_length = 2000000;
	std::string test_string(line_length, 'Y');

	MLB::Utility::HRTimer hr_timer;

	hr_timer.Start();

	for (int count_1 = 0; count_1 < line_count; ++count_1)
		LogDetail << "REALLY BIG STRING: [" << test_string << "]" << std::endl;

	hr_timer.End();

	LogInfo << "Time required to write " << line_count << " log lines of "
		"length " << line_length << ": " << std::setw(32) <<
		std::setprecision(12) << hr_timer.GetTickDiffInSeconds() <<
		" seconds." << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	TEST_MultiLineOperation();

	try {
		LogLevelPair old_level_console = MyLogManager.GetLogLevelConsole();
		LogLevelPair old_levels_file   = MyLogManager.GetLogLevelFile();
		MyLogManager.SetLogLevelConsoleAll();
		MyLogManager.SetLogLevelFileAll();
		//	Create a LogHandlerFile...
		boost::shared_ptr<LogHandlerXFile>
			my_log_handler(new LogHandlerXFile("TestLogXFile.VERSION_OLD_001.log"));
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
		MyLogManager.SetLogLevelConsole(old_level_console.first,
			old_level_console.second);
		MyLogManager.SetLogLevelFile(old_levels_file.first,
			old_levels_file.second);
		TEST_StressLines();
		TEST_StressSize();
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

