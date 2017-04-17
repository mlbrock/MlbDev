//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
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
 
#ifndef HH__MLB__Utility__LogManager_hpp__HH

#define HH__MLB__Utility__LogManager_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file		LogManager.hpp

	\brief	The header file for a core LogManager logic.
*/
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogLevel.hpp>
#include <Utility/TimeSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>

#include <mbtypes.h>	//	CODE NOTE: Needed by LogEmitControl::LogEmitControl() only.

#include <iomanip>
#include <set>

#include <map>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4302)
# if _MSC_VER < 1400
#  pragma warning(disable:4061 4242 4244 4347 4640)
# endif // # if _MSC_VER < 1400
# pragma warning(disable:4217 4275 4512 4625 4626)
# if _MSC_VER >= 1500
#  pragma warning(disable:4061 4244 4365 4640)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/thread/mutex.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4217 4668)
# if _MSC_VER >= 1500
#  pragma warning(disable:4244 4571)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/shared_ptr.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <fstream>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
enum LogFlag {
	None         = 0x0000,
	LogLocalTime = 0x0001,
	LogBothTimes = 0x0002,
	LogZeroTime  = 0x0004,
	LogZeroTid   = 0x0008,
	Default      = 0x0000
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::pair<LogLevel, LogLevel> LogLevelPair;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const unsigned int LogLevelTextMaxLength = 9;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const unsigned int LogLineLeaderLength = Length_TimeSpec + 1 +
	LogLevelTextMaxLength + 1 + 10 + 2;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY const char   *ConvertLogLevelToTextRaw(LogLevel log_level);
API_UTILITY std::string  &ConvertLogLevelToText(LogLevel log_level,
	std::string &out_string);
API_UTILITY std::string   ConvertLogLevelToText(LogLevel log_level);

API_UTILITY const char   *ConvertLogLevelToTextSimpleRaw(LogLevel log_level);
API_UTILITY std::string  &ConvertLogLevelToTextSimple(LogLevel log_level,
	std::string &out_string);
API_UTILITY std::string   ConvertLogLevelToTextSimple(LogLevel log_level);

API_UTILITY LogLevel      CheckLogLevel(LogLevel log_level);

API_UTILITY LogLevelPair  LogLevelFlagsToLevels(LogLevelFlag log_level_flags);

API_UTILITY StringVector &GetLogLevelNameList(StringVector &name_list,
	bool simple_flag = false);
API_UTILITY StringVector  GetLogLevelNameList(bool simple_flag = false);
API_UTILITY StringVector &GetLogLevelNameListSimple(StringVector &name_list);
API_UTILITY StringVector  GetLogLevelNameListSimple();
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY LogEmitControl {
	//	Constructor for formatted log lines...
	LogEmitControl(LogFlag log_flags, LogLevelFlag log_level_screen,
		LogLevelFlag log_level_persistent, const TimeSpec &line_start_time,
		LogLevel log_level, LogLevelFlag log_level_flag,
		const std::string &line_buffer)
		:log_flags_(log_flags)
		,log_level_screen_(log_level_screen)
		,log_level_persistent_(log_level_persistent)
		,line_start_time_(line_start_time)
		,log_level_(log_level)
		,log_level_flag_(log_level_flag)
		,line_buffer_empty_()
		,line_buffer_(line_buffer)
		,this_line_offset_(0)
	{
		memcpy(line_leader_ + Length_TimeSpec + 1,
			 ConvertLogLevelToTextRaw(log_level), LogLevelTextMaxLength);
		line_leader_[Length_TimeSpec + 1 + LogLevelTextMaxLength] = ' ';
		//	Maximum length of a thread id is coerced to 10 characters...
		char tid_string[IntToString_MaxLen_Unsigned_10 + 1];
		IntToString(tid_string,
			static_cast<Native_UInt32>(CurrentThreadId() % 0xFFFFFFFF), 10, " ");
		memcpy(line_leader_ + Length_TimeSpec + 1 + LogLevelTextMaxLength + 1,
			tid_string, 10);
		line_leader_[Length_TimeSpec + 1 + LogLevelTextMaxLength + 1 + 10] = ':';
		line_leader_[Length_TimeSpec + 1 + LogLevelTextMaxLength + 1 + 11] = ' ';
		line_leader_[Length_TimeSpec + 1 + LogLevelTextMaxLength + 1 + 12] = '\0';
	}
	//	Constructor for literal log lines...
	LogEmitControl(LogFlag log_flags, LogLevelFlag log_level_screen,
		LogLevelFlag log_level_persistent, LogLevel log_level,
		LogLevelFlag log_level_flag)
		:log_flags_(log_flags)
		,log_level_screen_(log_level_screen)
		,log_level_persistent_(log_level_persistent)
		,line_start_time_(0, 0)
		,log_level_(log_level)
		,log_level_flag_(log_level_flag)
		,line_buffer_empty_()
		,line_buffer_(line_buffer_empty_)
		,this_line_offset_(0)
	{
		line_leader_[0] = '\0';
	}

	unsigned int       GetLeaderLength() const {
		return(LogLineLeaderLength);
	}
	const char        *GetLeaderPtr() const {
		return(line_leader_);
	}
	const char        *GetNextLine(unsigned int &line_length) const {
		if (this_line_offset_ > line_buffer_.size()) {
			line_length = 0;
			return(NULL);
		}
		else if ((!this_line_offset_) && line_buffer_.empty()) {
			this_line_offset_ = 1;
			line_length       = 0;
			return("");
		}
		const char             *ptr = line_buffer_.c_str() + this_line_offset_;
		std::string::size_type  next_end =
			line_buffer_.find('\n', this_line_offset_);
		if (next_end == std::string::npos) {
			line_length       = static_cast<unsigned int>(line_buffer_.size()) -
				this_line_offset_;
			this_line_offset_ = static_cast<unsigned int>(line_buffer_.size()) + 1;
		}
		else {
			line_length       = static_cast<unsigned int>(next_end) -
				this_line_offset_;
			this_line_offset_ = static_cast<unsigned int>(next_end) + 1;
		}
		return(ptr);
	}
	void               ResetLines() const {
		this_line_offset_ = 0;
	}
	void               UpdateTime() const {
		if (log_flags_ & LogZeroTime)
			::memcpy(line_leader_, "0000-00-00 00:00:00.000000000",
				Length_TimeSpec);
		else {
			if (log_flags_ & LogLocalTime)
				TimeSpec().ToStringLocal(line_leader_);
			else
				TimeSpec().ToString(line_leader_);
			line_leader_[Length_TimeSpec] = ' ';
		}
	}

	bool               ShouldLogScreen() const {
		return((log_level_flag_ & log_level_screen_) != 0);
	}
	bool               ShouldLogPersistent() const {
		return((log_level_flag_ & log_level_persistent_) != 0);
	}
	LogFlag            GetLogFlags() const {
		return(log_flags_);
	}
	const TimeSpec    &GetLogStartTime() const {
		return(line_start_time_);
	}
	LogLevel           GetLogLevel() const {
		return(log_level_);
	}
	const std::string &GetLogMessage() const {
		return(line_buffer_);
	}

	LogFlag               log_flags_;
	LogLevelFlag          log_level_screen_;
	LogLevelFlag          log_level_persistent_;
	TimeSpec              line_start_time_;
	LogLevel              log_level_;
	LogLevelFlag          log_level_flag_;
	std::string           line_buffer_empty_;
	const std::string    &line_buffer_;
	mutable char          line_leader_[LogLineLeaderLength + 1];
	mutable unsigned int  this_line_offset_;

private:
	MLB_Utility_NonCopyable_Macro(LogEmitControl);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandler {
public:
	LogHandler() { }
	virtual ~LogHandler() { }

	virtual void InstallHandler() { }
	virtual void RemoveHandler() { }
	virtual void EmitLine(const LogEmitControl &emit_control) = 0;
	virtual void EmitLiteral(unsigned int literal_length,
		const char *literal_string) = 0;
	virtual void EmitLiteral(const LogEmitControl &emit_control,
		unsigned int literal_length, const char *literal_ptr) = 0;
	virtual void EmitLineSpecific(const std::string &line_buffer,
		LogLevel log_level = LogLevel_Info) {
/*
		MinGW gcc 3.4.5 doesn't like this, it thinks it needs the copy ctor...
		EmitLine(LogEmitControl(Default, LogFlag_Mask, LogFlag_Mask, TimeSpec(),
			log_level, LogFlag_Info, line_buffer));
*/
		LogEmitControl tmp_ctrl(Default, LogFlag_Mask, LogFlag_Mask, TimeSpec(),
			log_level, LogFlag_Info, line_buffer);
		EmitLine(tmp_ctrl);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<LogHandler> LogHandlerPtr;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogManager {
public:
	LogManager(LogFlag log_flags = Default,
		LogLevel min_log_level_screen = LogLevel_Info,
		LogLevel max_log_level_screen = LogLevel_Fatal,
		LogLevel min_log_level_persistent = LogLevel_Spam,
		LogLevel max_log_level_persistent = LogLevel_Fatal) :
		 log_handler_ptr_()
		,log_flags_(log_flags)
		,log_level_screen_(GetLogLevelMask(min_log_level_screen,
			max_log_level_screen))
		,log_level_persistent_(GetLogLevelMask(min_log_level_persistent,
			max_log_level_persistent))
		,the_lock_() {
	}
	explicit LogManager(LogHandlerPtr log_handler_ptr,
		LogFlag log_flags = Default,
		LogLevel min_log_level_screen = LogLevel_Info,
		LogLevel max_log_level_screen = LogLevel_Fatal,
		LogLevel min_log_level_persistent = LogLevel_Spam,
		LogLevel max_log_level_persistent = LogLevel_Fatal) :
		 log_handler_ptr_()
		,log_flags_(log_flags)
		,log_level_screen_(GetLogLevelMask(min_log_level_screen,
			max_log_level_screen))
		,log_level_persistent_(GetLogLevelMask(min_log_level_persistent,
			max_log_level_persistent))
		,the_lock_() {
		HandlerInstall(log_handler_ptr);
	}
	~LogManager() {
		HandlerRemove();
	}

	LogHandlerPtr HandlerInstall(LogHandlerPtr log_handler_ptr) {
		boost::mutex::scoped_lock my_lock(the_lock_);
		LogHandlerPtr             old_log_handler_ptr = log_handler_ptr_;
		if (log_handler_ptr_ != NULL)
			log_handler_ptr_->RemoveHandler();
		log_handler_ptr_ = log_handler_ptr;
		if (log_handler_ptr_ != NULL)
			log_handler_ptr_->InstallHandler();
		return(old_log_handler_ptr);
	}
	LogHandlerPtr HandlerRemove() {
		return(HandlerInstall(LogHandlerPtr()));
	}
	LogHandlerPtr GetHandlerPtr() {
		boost::mutex::scoped_lock my_lock(the_lock_);
		return(log_handler_ptr_);
	}

	LogLevelPair GetLogLevelConsole() const;
	LogLevelPair GetLogLevelFile() const;

	LogLevelPair SetLogLevelConsole(LogLevel min_log_level,
		LogLevel max_log_level = LogLevel_Maximum);
	LogLevelPair SetLogLevelFile(LogLevel min_log_level,
		LogLevel max_log_level = LogLevel_Maximum);

	void SetLogLevelConsoleAll() {
		SetLogLevelConsole(LogLevel_Minimum, LogLevel_Maximum);
	}
	void SetLogLevelFileAll() {
		SetLogLevelFile(LogLevel_Minimum, LogLevel_Maximum);
	}

	void EmitLine(const TimeSpec &line_start_time, LogLevel log_level,
		const std::string &line_buffer) {
		LogLevelFlag log_level_flag = static_cast<LogLevelFlag>
			((1 << log_level) & LogFlag_Mask);
		boost::mutex::scoped_lock my_lock(the_lock_);
		if ((log_handler_ptr_ != NULL) && ((log_level_flag & log_level_screen_) ||
			(log_level_flag & log_level_persistent_))) {
			LogEmitControl emit_ctl(log_flags_, log_level_screen_,
				log_level_persistent_, line_start_time, log_level, log_level_flag,
				line_buffer);
			my_lock.unlock();
			log_handler_ptr_->EmitLine(emit_ctl);
		}
	}
	void EmitLine(const std::string &line_buffer,
		LogLevel log_level = LogLevel_Info) {
		EmitLine(TimeSpec(), log_level, line_buffer);
	}
	void EmitLiteral(const std::string &literal_string) {
		EmitLiteral(static_cast<unsigned int>(literal_string.size()),
			literal_string.c_str());
	}
	void EmitLiteral(unsigned int literal_length, const char *literal_ptr) {
		literal_ptr = (literal_ptr == NULL) ? "" : literal_ptr;
		boost::mutex::scoped_lock my_lock(the_lock_);
		if (log_handler_ptr_ != NULL)
			log_handler_ptr_->EmitLiteral(literal_length, literal_ptr);
	}
	void EmitLiteral(LogLevel log_level, const std::string &literal_string) {
		EmitLiteral(log_level, static_cast<unsigned int>(literal_string.size()),
			literal_string.c_str());
	}
	void EmitLiteral(LogLevel log_level, unsigned int literal_length,
		const char *literal_ptr) {
		literal_ptr = (literal_ptr == NULL) ? "" : literal_ptr;
		LogLevelFlag log_level_flag = static_cast<LogLevelFlag>
			((1 << log_level) & LogFlag_Mask);
		boost::mutex::scoped_lock my_lock(the_lock_);
		if ((log_handler_ptr_ != NULL) && ((log_level_flag & log_level_screen_) ||
			(log_level_flag & log_level_persistent_))) {
			LogEmitControl emit_ctl(log_flags_, log_level_screen_,
				log_level_persistent_, log_level, log_level_flag);
			log_handler_ptr_->EmitLiteral(emit_ctl, literal_length, literal_ptr);
		}
	}

	//	Public to provide speed by permitting access to a pointer to will be
	//	const memory on most systems. Don't ever try to write through the
	//	returned pointer!
 	static const char *LogLevelToTextRaw(LogLevel log_level) {
		return(ConvertLogLevelToTextRaw(log_level));
	}

	static std::string LogLevelToText(LogLevel log_level) {
		return(LogLevelToTextRaw(log_level));
	}

private:
	MLB_Utility_NonCopyable_Macro(LogManager);

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	LogHandlerPtr log_handler_ptr_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	LogFlag       log_flags_;
	LogLevelFlag  log_level_screen_;
	LogLevelFlag  log_level_persistent_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	boost::mutex  the_lock_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

	static LogLevelFlag GetLogLevelMask(LogLevel min_level, LogLevel max_level) {
		min_level = std::max(min_level, LogLevel_Minimum);
		max_level = std::min(max_level, LogLevel_Maximum);
		if (min_level > max_level)
			std::swap(min_level, max_level);
		unsigned int level_flags = 0;
		while (min_level <= max_level) {
			level_flags |= 1 << static_cast<unsigned int>(min_level);
			min_level    =
				static_cast<LogLevel>(static_cast<unsigned int>(min_level) + 1);
		}
		return(static_cast<LogLevelFlag>(level_flags));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerConsole : public LogHandler {
public:
	LogHandlerConsole();

	virtual ~LogHandlerConsole();

	virtual void InstallHandler();
	virtual void RemoveHandler();

	virtual void EmitLine(const LogEmitControl &emit_control);
	virtual void EmitLiteral(unsigned int literal_length,
		const char *literal_string);
	virtual void EmitLiteral(const LogEmitControl &emit_control,
		unsigned int literal_length, const char *literal_string);

protected:
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	mutable boost::mutex the_lock_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

private:
	std::ios_base::Init iostreams_init_;

	MLB_Utility_NonCopyable_Macro(LogHandlerConsole);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerFile : public LogHandler {
public:
	enum LogHandlerFileFlag {
		None            = 0x0000,
		DoNotAppend     = 0x0001,
		NoConsoleOutput = 0x0002,
		Default         = None
	};
	LogHandlerFile();
	explicit LogHandlerFile(const char *file_name,
		LogHandlerFileFlag flags = Default);
	explicit LogHandlerFile(const std::string &file_name,
		LogHandlerFileFlag flags = Default);
	LogHandlerFile(const char *base_name, const char *dir_name,
		LogHandlerFileFlag flags = Default);
	LogHandlerFile(const std::string &base_name, const std::string &dir_name,
		LogHandlerFileFlag flags = Default);
	LogHandlerFile(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileFlag flags = Default);
	LogHandlerFile(const std::string &base_name, const std::string &dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileFlag flags = Default);

	virtual ~LogHandlerFile();

	void InstallHandler();
	void RemoveHandler();

	virtual void EmitLine(const LogEmitControl &emit_control);
	virtual void EmitLiteral(unsigned int literal_length,
		const char *literal_string);
	virtual void EmitLiteral(const LogEmitControl &emit_control,
		unsigned int literal_length, const char *literal_string);

	virtual void OpenFile(const char *file_name);
	virtual void OpenFile(const std::string &file_name);
	virtual void OpenFile(const char *base_name, const char *dir_name);
	virtual void OpenFile(const std::string &base_name,
		const std::string &dir_name);
	virtual void OpenFile(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time);
	virtual void OpenFile(const std::string &base_name,
		const std::string &dir_name, const MLB::Utility::TimeT &start_time);

	LogHandlerFileFlag GetFlags() const;
	LogHandlerFileFlag SetFlags(LogHandlerFileFlag new_flags);
	std::string        GetFileName() const;

protected:
	std::string                      out_file_name_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	boost::shared_ptr<std::ofstream> out_file_ptr_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	LogHandlerFileFlag               my_flags_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	mutable boost::mutex             the_lock_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

private:
	MLB_Utility_NonCopyable_Macro(LogHandlerFile);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY ThreadStreamBuffer : public std::streambuf {
public:
	ThreadStreamBuffer(LogManager &manager_ref, LogLevel log_level)
		:std::streambuf()
		,last_used_time_()
		,manager_ref_(manager_ref)
		,log_level_(log_level)
		,line_start_time_()
		,line_buffer_()
		,sep_buffer_()
	 {
		setp(0, 0);
      setg(0, 0, 0);
	}

	~ThreadStreamBuffer() {
		Synchronize();
	}

	void Synchronize(bool force_line_flag = false) {
		sync(force_line_flag);
	}
	void PutChar(int datum) {
		put_char(datum);
	}
	void PutString(const std::string &datum) {
		std::string::size_type string_length = datum.size();
		std::string::size_type string_index  = 0;
		while (string_index < string_length)
         put_char(datum[string_index++]);
	}
	void PutLiteral(unsigned int literal_length, const char *literal_string) {
		Synchronize();
		manager_ref_.EmitLiteral(log_level_, literal_length, literal_string);
	}

	void LogSeparator(char sep_char = '*', unsigned int sep_length = 80)
	{
		unsigned int total_sep_length = LogLineLeaderLength + sep_length;

		if (sep_buffer_.empty() || (total_sep_length != sep_buffer_.size()) ||
			(sep_buffer_[0] != sep_char)) {
			sep_buffer_.assign(total_sep_length, sep_char);
			sep_buffer_[Length_TimeSpec]                                          = ' ';
			sep_buffer_[Length_TimeSpec + 1 + LogLevelTextMaxLength]              = ' ';
			sep_buffer_[Length_TimeSpec + 1 + LogLevelTextMaxLength + 1 + 10 + 1] = ' ';
		}

		PutLiteral(total_sep_length, sep_buffer_.c_str());
	}

protected:
	std::streambuf::int_type overflow(int c) {
      if (c != EOF)
  			put_char(c);
		return(0);
	}

	int sync() {
		return(sync(false));
	}
	int sync(bool force_line_flag) {
		put_buffer(force_line_flag);
		return(0);
	}

private:

	TimeVal      last_used_time_;
	LogManager  &manager_ref_;
	LogLevel     log_level_;
	TimeSpec     line_start_time_;
	std::string  line_buffer_;
	std::string  sep_buffer_;

	void put_char(int chr) {
		if (line_buffer_.empty())
			line_start_time_ = TimeSpec();
		if (chr == '\n')
			put_buffer(true);
		else {
			line_buffer_    += static_cast<char>(chr);
			last_used_time_  = TimeVal();
		}
	}

	void put_buffer(bool force_flag) {
		if (force_flag || (!line_buffer_.empty())) {
			if (log_level_ == LogLevel_Literal)
				manager_ref_.EmitLiteral(log_level_, line_buffer_);
			else
				manager_ref_.EmitLine(line_start_time_, log_level_, line_buffer_);
			line_buffer_.clear();
		}
		last_used_time_ = TimeVal();
	}

	MLB_Utility_NonCopyable_Macro(ThreadStreamBuffer);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<ThreadStreamBuffer> ThreadStreamBufferPtr;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY ThreadStream : public std::ostream {
public:
	ThreadStream(LogManager &manager_ref, LogLevel log_level,
		ThreadStreamBufferPtr buffer_ptr) :
		 std::ostream(buffer_ptr.get())
		,manager_ref_(manager_ref)
		,log_level_(log_level)
		,buffer_ptr_(buffer_ptr) {
	}
	~ThreadStream() {
		buffer_ptr_->Synchronize();
	}

	ThreadStreamBufferPtr  GetBufferPtrRef() {
		return(buffer_ptr_);
	}

	//	Not truly necessary to keep a local copy of of the manager reference and
	//	the log level, but they were useful during debugging...
	LogManager            &manager_ref_;
	LogLevel               log_level_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	ThreadStreamBufferPtr  buffer_ptr_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

private:
	MLB_Utility_NonCopyable_Macro(ThreadStream);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogStream : public std::ostream {
public:
	LogStream(LogManager &manager_ref, LogLevel log_level):
		 std::ostream(new ThreadStreamBuffer(manager_ref, log_level))
		,manager_ref_(manager_ref)
		,log_level_(log_level)
		,thread_stream_map_() {
	}
	~LogStream();

	LogStream & operator << (std::ostream & (*pfn)(std::ostream &)) {
		*GetThreadStream() << pfn;
		return(*this);
	}
	LogStream & operator << (std::ios_base & (*pfn)(std::ios_base &)) {
		*GetThreadStream() << pfn;
		return(*this);
	}
	LogStream & operator << (std::ios & (*pfn)(std::ios &)) {
		*GetThreadStream() << pfn;
		return(*this);
	}
	template <typename DataType> LogStream & operator << (
		const DataType &datum) {
		*GetThreadStream() << datum;
		return(*this);
	}
	LogStream & operator << (const std::string &datum) {
		*GetThreadStream() << datum.c_str();
		return(*this);
	}
/*
	LogStream & operator << (const char *datum) {
		*GetThreadStream() << datum;
		return(*this);
	}
*/
/*
	LogStream & operator << (unsigned int datum) {
		*GetThreadStream() << datum;
		return(*this);
	}
*/

	void LogLiteral(const std::string &literal_string) {
		LogLiteral(static_cast<unsigned int>(literal_string.size()),
			literal_string.c_str());
	}
	void LogLiteral(const char *literal_string) {
		LogLiteral((literal_string == NULL) ? 0 :
			static_cast<unsigned int>(strlen(literal_string)), literal_string);
	}
	void LogLiteral(unsigned int literal_length, const char *literal_string) {
		GetThreadStream()->GetBufferPtrRef()->PutLiteral(literal_length,
			literal_string);
	}
	void LogSeparator(char sep_char = '*', unsigned int text_length = 80);

	void LogToLevel(LogLevel log_level, const std::string &log_text) {
		ThreadStreamBufferPtr buffer_ptr(
									new ThreadStreamBuffer(manager_ref_, log_level));
		ThreadStream tmp_stream(manager_ref_, log_level, buffer_ptr);
		tmp_stream << log_text;
	}

private:
	typedef boost::shared_ptr<ThreadStream>                   ThreadStreamPtr;
	typedef std::map<MLB::Utility::ThreadId, ThreadStreamPtr> ThreadStreamMap;
	typedef ThreadStreamMap::iterator                         ThreadStreamMapIter;

	MLB_Utility_NonCopyable_Macro(LogStream);

	LogManager      &manager_ref_;
	LogLevel         log_level_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	ThreadStreamMap  thread_stream_map_;
	boost::mutex     the_lock_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(default:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

	ThreadStreamPtr GetThreadStream();
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY LogLevelParseCmdLineArg : public ParseCmdLineArg {
	static bool ParseCmdLineDatum(const std::string &param_value,
		LogLevel &out_datum);

	static bool ParseLogLevelScreenMin(unsigned int &current_index, int argc,
		char **argv, LogLevel &out_datum) {
		return(ParseLogLevelConsoleMin(current_index, argc, argv, out_datum));
	}
	static bool ParseLogLevelScreenMax(unsigned int &current_index, int argc,
		char **argv, LogLevel &out_datum) {
		return(ParseLogLevelConsoleMax(current_index, argc, argv, out_datum));
	}
	static bool ParseLogLevelFileMin(unsigned int &current_index, int argc,
		char **argv, LogLevel &out_datum) {
		return(ParseLogLevelPersistentMin(current_index, argc, argv, out_datum));
	}
	static bool ParseLogLevelFileMax(unsigned int &current_index, int argc,
		char **argv, LogLevel &out_datum) {
		return(ParseLogLevelPersistentMax(current_index, argc, argv, out_datum));
	}
	static bool ParseLogLevelConsoleMin(unsigned int &current_index, int argc,
		char **argv, LogLevel &out_datum);
	static bool ParseLogLevelConsoleMax(unsigned int &current_index, int argc,
		char **argv, LogLevel &out_datum);
	static bool ParseLogLevelPersistentMin(unsigned int &current_index, int argc,
		char **argv, LogLevel &out_datum);
	static bool ParseLogLevelPersistentMax(unsigned int &current_index, int argc,
		char **argv, LogLevel &out_datum);

	static bool ParseLogLevel(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv, LogLevel &out_datum);
	template <class ContainerType>
		static bool ParseLogLevel(const ContainerType &spec_cont,
		unsigned int &current_index, int argc, char **argv,
		LogLevel &out_datum) {
		std::string tmp_string;
		if (!ParseCmdLineFollowingSpec(spec_cont, current_index, argc, argv,
			tmp_string))
			return(false);
		return(ParseLogLevelInternal(argv[current_index - 1], tmp_string,
			out_datum));
	}

private:
	static bool ParseLogLevelInternal(const std::string &param_name,
		const std::string &param_value, LogLevel &out_datum);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

//	////////////////////////////////////////////////////////////////////////////
	/**
		Use once in any collective executable unit... That is, only one \e .cpp
		should use this macro in any program. A good place to do so is in the
		translation unit which contains the \c main function.

		If these names need to be visibile in other translation units besides the
		one which contains \c main, the #LogManagerMacroDeclaration macro should
		be used instead.
	*/
#define LogManagerMacroDefinition(export_spec)																			\
	export_spec MLB::Utility::LogManager MyLogManager;																	\
	export_spec MLB::Utility::LogStream  LogLiteral(MyLogManager,   MLB::Utility::LogLevel_Literal);	\
	export_spec MLB::Utility::LogStream  LogSpam(MyLogManager,      MLB::Utility::LogLevel_Spam);		\
	export_spec MLB::Utility::LogStream  LogMinutiae(MyLogManager,  MLB::Utility::LogLevel_Minutiae);	\
	export_spec MLB::Utility::LogStream  LogDebug(MyLogManager,     MLB::Utility::LogLevel_Debug);		\
	export_spec MLB::Utility::LogStream  LogDetail(MyLogManager,    MLB::Utility::LogLevel_Detail);		\
	export_spec MLB::Utility::LogStream  LogInfo(MyLogManager,      MLB::Utility::LogLevel_Info);		\
	export_spec MLB::Utility::LogStream  LogNotice(MyLogManager,    MLB::Utility::LogLevel_Notice);		\
	export_spec MLB::Utility::LogStream  LogWarning(MyLogManager,   MLB::Utility::LogLevel_Warning);	\
	export_spec MLB::Utility::LogStream  LogError(MyLogManager,     MLB::Utility::LogLevel_Error);		\
	export_spec MLB::Utility::LogStream  LogCritical(MyLogManager,  MLB::Utility::LogLevel_Critical);	\
	export_spec MLB::Utility::LogStream  LogAlert(MyLogManager,     MLB::Utility::LogLevel_Alert);		\
	export_spec MLB::Utility::LogStream  LogEmergency(MyLogManager, MLB::Utility::LogLevel_Emergency);	\
	export_spec MLB::Utility::LogStream  LogFatal(MyLogManager,     MLB::Utility::LogLevel_Fatal);

	/**
		Use wherever logging is needed...
	*/
#define LogManagerMacroDeclaration(import_spec)						\
	extern import_spec MLB::Utility::LogManager MyLogManager;	\
	extern import_spec MLB::Utility::LogStream  LogLiteral;		\
	extern import_spec MLB::Utility::LogStream  LogSpam;			\
	extern import_spec MLB::Utility::LogStream  LogMinutiae;		\
	extern import_spec MLB::Utility::LogStream  LogDebug;			\
	extern import_spec MLB::Utility::LogStream  LogDetail;		\
	extern import_spec MLB::Utility::LogStream  LogInfo;			\
	extern import_spec MLB::Utility::LogStream  LogNotice;		\
	extern import_spec MLB::Utility::LogStream  LogWarning;		\
	extern import_spec MLB::Utility::LogStream  LogError;			\
	extern import_spec MLB::Utility::LogStream  LogCritical;		\
	extern import_spec MLB::Utility::LogStream  LogAlert;			\
	extern import_spec MLB::Utility::LogStream  LogEmergency;	\
	extern import_spec MLB::Utility::LogStream  LogFatal;
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifndef HH__MLB__Utility__LogManager_hpp__HH

