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

#include <Utility/LogHandlerFileMMap.hpp>

#include <Utility/PathName.hpp>
#include <Utility/AlignmentSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {
//	////////////////////////////////////////////////////////////////////////////
const unsigned int LogFileMMapDefaultAllocSize = 1 << 20;
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap()
	:LogHandlerFileBase()
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const char *file_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(file_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const std::string &file_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(file_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const char *base_name,
	const char *dir_name, LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(base_name, dir_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const std::string &base_name,
	const std::string &dir_name, LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(base_name, dir_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const char *base_name,
	const char *dir_name, const MLB::Utility::TimeT &start_time,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(base_name, dir_name, start_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(base_name, dir_name, start_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::~LogHandlerFileMMap()
{
	bool                      truncate_file_flag = false;
	MyMappingValue            truncate_file_size = 0;
	boost::mutex::scoped_lock my_lock(the_lock_);

	if (region_sptr_.get() != NULL) {
		/*
			Truncate the file just beyond the last byte written in the current
			chunk. This drops the zero-filled portion of the file...
		*/
		truncate_file_flag = true;
		truncate_file_size = mapping_offset_ + write_offset_;
		try {
			region_sptr_->flush();
		}
		catch (const std::exception &) {
		}
	}

	region_sptr_.reset();
	mapping_sptr_.reset();

	if (truncate_file_flag) {
		try {
			TruncateFileSize(out_file_name_, truncate_file_size);
		}
		catch (const std::exception &) {
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::InstallHandlerImpl()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::RemoveHandlerImpl()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::OpenFileImpl(const char *file_name)
{
	bool               file_created = false;
	unsigned long long file_size    = 0;
	std::string        tmp_file_name;
	bool               file_existed;

	file_existed = ResolveFilePathGeneral(file_name, tmp_file_name, "",
		false, false, true);

	using namespace boost::interprocess;

	try {
		unsigned int   mapping_size   = chunk_alloc_size_;
		MyMappingValue mapping_offset = 0;
		MyMappingValue write_offset   = 0;
		if (file_existed) {
			/*
				If the file already exists, we attempt to determine whether the
				file ends in a string of ASCII NUL characters. If any such string
				is found, we'll attempt to 'back over' it.

				The reason such a string can come into existence is a result of
				the fact that memory-mapping, in both Windows and various Unices,
				operates on pages, and not characters. Pages are padded-out by
				being zero-filled upon assignment to the process by the operating
				system.

				The destructor logic is written to truncate the file such that
				file contents will include only characters written. But if a
				run time mishap prevents the destructor from completing correctly,
				the removal of the padding bytes may not be performed.
			*/
			if (my_flags_ & DoNotAppend) {
				TruncateFileSize(tmp_file_name, 0);
				file_size = 0;
			}
			else
				file_size = MLB::Utility::GetFileSize(tmp_file_name);
			if (!file_size) {
				mapping_offset = 0;
				write_offset   = 0;
			}
			if (!(file_size % page_alloc_size_)) {
				unsigned int   tmp_map_size;
				MyMappingValue tmp_map_offset;
				if (file_size < chunk_alloc_size_) {
					tmp_map_size   = static_cast<unsigned int>(file_size);
					tmp_map_offset = 0;
				}
				else {
					tmp_map_size   = chunk_alloc_size_;
					tmp_map_offset = file_size - chunk_alloc_size_;
				}
				file_mapping  tmp_mapping(tmp_file_name.c_str(), read_write);
				mapped_region tmp_region(tmp_mapping, read_write,
					static_cast<boost::interprocess::offset_t>(tmp_map_offset),
					tmp_map_size);
				const char *start_ptr = 
					static_cast<const char *>(tmp_region.get_address());
				const char *end_ptr   = start_ptr + tmp_map_size;
				if (end_ptr[-1]) {
					TruncateFileSize(tmp_file_name, file_size + chunk_alloc_size_);
					mapping_offset = file_size;
					write_offset   = 0;
				}
				else {
					while ((end_ptr > start_ptr) && (!end_ptr[-1]))
						--end_ptr;
					if (end_ptr > start_ptr) {
						mapping_size   = tmp_map_size;
						mapping_offset = tmp_map_offset;
						write_offset   =
							static_cast<MyMappingValue>(end_ptr - start_ptr);
					}
					else {
						mapping_size   = tmp_map_size;
						mapping_offset = tmp_map_offset;
						write_offset   = 0;
					}
				}
			}
			else {
				file_size = GranularRoundUp<unsigned long long>(file_size,
					chunk_alloc_size_);
				TruncateFileSize(tmp_file_name, file_size);
				unsigned int   tmp_map_size   = chunk_alloc_size_;
				MyMappingValue tmp_map_offset = file_size - chunk_alloc_size_;
				file_mapping   tmp_mapping(tmp_file_name.c_str(), read_write);
				mapped_region  tmp_region(tmp_mapping, read_write,
					static_cast<boost::interprocess::offset_t>(tmp_map_offset),
					tmp_map_size);
				const char *start_ptr = 
					static_cast<const char *>(tmp_region.get_address());
				const char *end_ptr   = start_ptr + tmp_map_size;
				while ((end_ptr > start_ptr) && (!end_ptr[-1]))
					--end_ptr;
				if (end_ptr > start_ptr) {
					mapping_size   = tmp_map_size;
					mapping_offset = tmp_map_offset;
					write_offset   =
						static_cast<MyMappingValue>(end_ptr - start_ptr);
				}
				else {
					mapping_size   = tmp_map_size;
					mapping_offset = tmp_map_offset;
					write_offset   = 0;
				}
			}
		}
		else {
			/*
				Otherwise, the file doesn't exist. So create it...
			*/
			{
				std::ofstream this_file(tmp_file_name.c_str(),
					(!(my_flags_ & DoNotAppend)) ?
					(std::ios_base::app | std::ios_base::ate) :
					(std::ios_base::app | std::ios_base::trunc));
				if (this_file.fail())
					ThrowErrno("Open attempt for the new file failed.");
			}
			TruncateFileSize(tmp_file_name, mapping_size);
			file_created   = true;
			mapping_offset = 0;
			write_offset   = 0;
		}
		MyFileMappingSPtr mapping_sptr(
			new file_mapping(tmp_file_name.c_str(), read_write));
		MyMappedRegionSPtr region_sptr(
			new mapped_region(*mapping_sptr, read_write,
			static_cast<boost::interprocess::offset_t>(mapping_offset),
			mapping_size));
		{
			boost::mutex::scoped_lock my_lock(the_lock_);
			mapping_sptr_.swap(mapping_sptr);
			region_sptr_.swap(region_sptr);
			out_file_name_.swap(tmp_file_name);
			mapping_size_   = mapping_size;
			mapping_offset_ = mapping_offset;
			write_offset_   = write_offset;
		}
	}
	catch (const std::exception &except) {
		if (file_created) {
			try {
				RemoveFile(tmp_file_name, true);
			}
			catch (const std::exception &) {
			}
		}
		MLB::Utility::Rethrow(except, "Attempt to open log file '" +
			tmp_file_name + "' for memory-mapped write access failed: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::FlushImpl()
{
	if (region_sptr_.get() != NULL)
		region_sptr_->flush();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::EmitLineImpl(const LogEmitControl &emit_control)
{
	if (region_sptr_.get() != NULL) {
		EnsureNeededSpace(emit_control.GetLeaderLength() +
			emit_control.line_buffer_.size() + eol_string_length_);
		::memcpy(GetCurrentPtr(), emit_control.GetLeaderPtr(),
			emit_control.GetLeaderLength());
		AddToOffset(emit_control.GetLeaderLength());
		::memcpy(GetCurrentPtr(), emit_control.line_buffer_.c_str(),
			emit_control.line_buffer_.size());
		AddToOffset(emit_control.line_buffer_.size());
		::memcpy(GetCurrentPtr(), eol_string_.c_str(), eol_string_length_);
		AddToOffset(eol_string_length_);
// CODE NOTE: Shouldn't flush unless actually desired by user.
//		out_file_ptr_->flush();
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::EmitLiteralImpl(unsigned int literal_length,
	const char *literal_string)
{
	if (region_sptr_.get() != NULL) {
		EnsureNeededSpace(literal_length + eol_string_length_);
		::memcpy(GetCurrentPtr(), literal_string, literal_length);
		AddToOffset(literal_length);
		::memcpy(GetCurrentPtr(), eol_string_.c_str(), eol_string_length_);
		AddToOffset(eol_string_length_);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	Assumes the lock has been acquired.
*/
void LogHandlerFileMMap::EnsureNeededSpace(std::size_t needed_length)
{
	MyMappingValue remaining_length = (write_offset_ <= mapping_size_) ?
		(mapping_size_ - write_offset_) : 0;

	//	Note that if equal we'll go ahead and allocate more space...
	if (needed_length < remaining_length)
		return;

	MyMappingValue mapping_offset = GranularRoundDown<MyMappingValue>(
		mapping_offset_ + write_offset_, page_alloc_size_);
	MyMappingValue write_offset   = (mapping_offset_ + write_offset_) -
		mapping_offset;
	MyMappingValue mapping_size   = std::max(chunk_alloc_size_,
		GranularRoundUp<unsigned int>(needed_length, page_alloc_size_) +
		page_alloc_size_);

	TruncateFileSize(out_file_name_, mapping_offset + mapping_size);

	using namespace boost::interprocess;

	MyFileMappingSPtr        mapping_sptr(
		new file_mapping(out_file_name_.c_str(), read_write));
	MyMappedRegionSPtr        region_sptr(
		new mapped_region(*mapping_sptr, read_write,
		static_cast<boost::interprocess::offset_t>(mapping_offset),
		static_cast<std::size_t>(mapping_size)));

	mapping_sptr_.swap(mapping_sptr);
	region_sptr_.swap(region_sptr);

	mapping_size_   = static_cast<unsigned int>(mapping_size);
	mapping_offset_ = mapping_offset;
	write_offset_   = write_offset;
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
std::cout << GranularRoundDown(1048576, 65536) << std::endl;
std::cout << GranularRoundDown(1048575, 65536) << std::endl;

	int return_code = EXIT_SUCCESS;

	TEST_MultiLineOperation();

	try {
		LogLevelPair old_level_console = MyLogManager.GetLogLevelConsole();
		LogLevelPair old_levels_file   = MyLogManager.GetLogLevelFile();
		MyLogManager.SetLogLevelConsoleAll();
		MyLogManager.SetLogLevelFileAll();
		//	Create a LogHandlerFile...
		boost::shared_ptr<LogHandlerFileMMap>
			my_log_handler(new LogHandlerFileMMap("TestLogFileMMap.VERSION_OLD_001.log"));
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

