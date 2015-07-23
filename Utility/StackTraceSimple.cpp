// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	StackTrace.cpp

	File Description	:	Lightweight stack trace facility.

	Revision History	:	2015-07-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>

#ifdef _MSC_VER
# include <Windows.h>
# include <DbgHelp.h>
#else
# include <execinfo.h>
# include <cxxabi.h>
#endif // #ifdef _MSC_VER

#ifndef _MSC_VER
# include <boost/regex.hpp>
#endif // #ifndef _MSC_VER

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

/*
// ////////////////////////////////////////////////////////////////////////////
class BackTraceHashBase {
	virtual ~BackTraceHashBase()
	{
	}
};
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
const std::size_t MaxFrameTotalCount    = 0x7fff;
const std::size_t DefaultMaxFrameCount  = 1024;
const std::size_t DefaultSkipFrameCount = 0;
// private:
const std::size_t StackTraceBufferSize  = 256;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct StackTrace {
	enum Flags {
		Flags_None       = 0x0000,
		Flags_NoDemangle = 0x0001,
		Flags_FileName   = 0x0002,
		Flags_FuncName   = 0x0004,
		Flags_Extra_1    = 0x0008,
		Flags_Extra_2    = 0x0010,
		Flags_Default    = Flags_FileName | Flags_FuncName,
		Flags_Minimum    = Flags_None,
		Flags_Maximum    = Flags_Extra_2,
		Flags_Count      = 6
	};

	typedef std::vector<StackTrace> StackTraceVector_I;

	explicit StackTrace(const char *file_name = NULL,
		const char *func_name = NULL, std::size_t func_offset = 0,
		const void *func_address = NULL, const void  *current_address = NULL)
		:file_name_((file_name) ? file_name : "")
		,func_name_((func_name) ? func_name : "")
		,func_offset_(func_offset)
		,func_address_(func_address)
		,current_address_(current_address)
	{
	}

	void swap(StackTrace &other);

	std::string GetFileName() const
	{
		return(file_name_);
	}

	std::string GetFunctionName() const
	{
		return(func_name_);
	}

	std::size_t GetFunctionOffset() const
	{
		return(func_offset_);
	}

	const void *GetFunctionAddress() const
	{
		return(func_address_);
	}

	static StackTraceVector_I &GetBackTrace(StackTraceVector_I &dst_list,
		StackTrace::Flags flags = StackTrace::Flags_Default,
		std::size_t max_frame_count = DefaultMaxFrameCount,
		std::size_t skip_frame_count = DefaultSkipFrameCount);
	static StackTraceVector_I  GetBackTrace(
		StackTrace::Flags flags = StackTrace::Flags_Default,
		std::size_t max_frame_count = DefaultMaxFrameCount,
		std::size_t skip_frame_count = DefaultSkipFrameCount);

	static std::pair<std::size_t, std::size_t> GetMaxStringLengths(
		const StackTraceVector_I &trace_list, bool demangle_flag = false);

	static void Emit(const StackTraceVector_I &trace_list,
		bool demangle_flag = false, std::ostream &o_str = std::cout);

	std::string  file_name_;
	std::string  func_name_;
	std::size_t  func_offset_;
	const void  *func_address_;
	const void  *current_address_;

#ifdef WIN32
	SYMBOL_INFO       symbol_info_;
	IMAGEHLP_MODULE64 module_info_;
#endif // #ifdef WIN32
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef StackTrace::StackTraceVector_I   StackTraceVector;
typedef StackTraceVector::iterator 		  StackTraceVectorIter;
typedef StackTraceVector::const_iterator StackTraceVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
class BackTraceHashCounter : public BackTraceHashBase {
	struct BackTraceHash {
		explicit BackTraceHash(Native_UInt64 hash_key)
			:hash_key_(hash_key)
			,instance_count_(0)
			,trace_list_()
		{
		}

		bool operator < (const BackTraceHash &other) const
		{
			return(hash_key_ < other.hash_key_);
		}

		Native_UInt64    hash_key_;
		Native_UInt64    instance_count_;
		StackTraceVector trace_list_;
	};

	typedef std::set<BackTraceHash>                 BackTraceHashSet_I;
	typedef BackTraceHashSet_I::const_iterator      BackTraceHashSetIterC_I;
	typedef std::pair<BackTraceHashSetIter_I, bool> BackTraceHashSetPair_I;

	BackTraceHash *ApplyHash(Native_UInt64 hashed_key)
	{
		return(const_cast<BackTraceHash *>(&(
			*observed_set_.insert(BackTraceHash(hash_key)).first)));
	}

	BackTraceHashSet_I observed_set_;
};
// ////////////////////////////////////////////////////////////////////////////
*/

#ifdef __linux__

// ////////////////////////////////////////////////////////////////////////////
struct StackTraceWork {
	typedef std::vector<StackTraceWork> StackTraceWorkVector_I;

	StackTraceWork(
		std::size_t file_name_size = 0, const char *file_name_ptr = NULL,
		std::size_t func_name_size = 0, const char *func_name_ptr = NULL,
		std::size_t func_offset_size = 0, const char *func_offset_ptr = NULL,
		std::size_t func_address_size = 0, const char *func_address_ptr = NULL)
		:file_name_size_(file_name_size)
		,file_name_ptr_(file_name_ptr)
		,func_name_size_(func_name_size)
		,func_name_ptr_(func_name_ptr)
		,func_offset_size_(func_offset_size)
		,func_offset_ptr_(func_offset_ptr)
		,func_address_size_(func_address_size)
		,func_address_ptr_(func_address_ptr)
		,func_offset_value_(0)
		,func_address_value_(0)
	{
	}

	void swap(StackTraceWork &other);

	std::string GetFileName() const
	{
		return((!file_name_ptr_) ? "" :
			std::string(file_name_ptr_, file_name_size_));
	}

	std::string GetFunctionName(bool demangle_flag = false) const
	{
		return((!func_name_ptr_) ? "" : ((demangle_flag) ?
			GetFunctionNameDemangled() :
			std::string(func_name_ptr_, func_name_size_)));
	}

	std::string GetFunctionNameDemangled() const
	{
		if (!func_name_ptr_)
			return("");

		std::string tmp_name(func_name_ptr_, func_name_size_);

		int                     status_code;
		boost::scoped_ptr<char> final_name(abi::__cxa_demangle(
			tmp_name.c_str(), NULL, NULL, &status_code));

		return((!status_code) ? std::string(final_name.get()) : tmp_name);
	}

	std::size_t GetFunctionOffset() const
	{
		return(func_offset_value_);
	}

	const void *GetFunctionAddress() const
	{
		return(func_address_value_);
	}

	std::size_t ParseFunctionOffset()
	{
		if (!func_offset_ptr_)
			return(0);
		else if (func_offset_value_)
			return(func_offset_value_);

		char **end_ptr = NULL;

#ifdef _MSC_VER
		return(func_offset_value_ = ::_strtoui64(func_offset_ptr_, end_ptr, 16));
#else
		return(func_offset_value_ = ::strtoull(func_offset_ptr_, end_ptr, 16));
#endif // #ifdef _MSC_VER
	}

	const void *ParseFunctionAddress()
	{
		if (!func_address_ptr_)
			return(0);
		else if (func_address_value_)
			return(func_address_value_);

		char **end_ptr = NULL;

#ifdef _MSC_VER
		return(func_address_value_ = static_cast<const void *>(
			::_strtoui64(func_address_ptr_, end_ptr, 16)));
#else
		return(func_address_value_ = reinterpret_cast<const void *>(
			::strtoull(func_address_ptr_, end_ptr, 16)));
#endif // #ifdef _MSC_VER
	}

	void ParseNumericValues()
	{
		ParseFunctionOffset();
		ParseFunctionAddress();
	}

	std::size_t  file_name_size_;
	const char  *file_name_ptr_;
	std::size_t  func_name_size_;
	const char  *func_name_ptr_;
	std::size_t  func_offset_size_;
	const char  *func_offset_ptr_;
	std::size_t  func_address_size_;
	const char  *func_address_ptr_;
	std::size_t  func_offset_value_;
	const void  *func_address_value_;

	static StackTraceWork &Extract(const char *src, StackTraceWork &dst);
	static StackTraceWork  Extract(const char *src);

	static StackTraceWorkVector_I &Extract(int src_count,
		const char **src_list, StackTraceWorkVector_I &dst_list);
	static StackTraceWorkVector_I  Extract(int src_count, 
		const char **src_list);

	static StackTraceWorkVector_I &FixUpNumericValues(
		StackTraceWorkVector_I &trace_list);

	static std::pair<std::size_t, std::size_t> GetMaxStringLengths(
		const StackTraceWorkVector_I &trace_list, bool demangle_flag = false);

	static void Emit(const StackTraceWorkVector_I &trace_list,
		bool demangle_flag = false, std::ostream &o_str = std::cout);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef StackTraceWork::StackTraceWorkVector_I StackTraceWorkVector;
typedef StackTraceWorkVector::iterator 			  StackTraceWorkVectorIter;
typedef StackTraceWorkVector::const_iterator 	  StackTraceWorkVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef __linux__

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

#ifdef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////
StackTraceVector &StackTrace::GetBackTrace(StackTraceVector &dst_list,
	StackTrace::Flags flags, std::size_t max_frame_count,
	std::size_t skip_frame_count)
{
	StackTraceVector tmp_dst_list;

	max_frame_count  = (max_frame_count) ? max_frame_count :
		DefaultMaxFrameCount;
	max_frame_count  = (max_frame_count <= MaxFrameTotalCount) ?
		max_frame_count : MaxFrameTotalCount;
	skip_frame_count = (skip_frame_count <= MaxFrameTotalCount) ?
		skip_frame_count : MaxFrameTotalCount;

	std::size_t total_frames = max_frame_count + skip_frame_count;

	total_frames = (total_frames <= MaxFrameTotalCount) ?
		total_frames : MaxFrameTotalCount;

	try {
		HANDLE process_handle = ::GetCurrentProcess();
		::SymInitialize(process_handle, NULL, TRUE);
		USHORT                        trace_count;
		void                         *trace_buffer[StackTraceBufferSize];
		void                        **trace_list;
		boost::scoped_array<void *>   trace_sptr;
		if (total_frames <= StackTraceBufferSize)
			trace_list = trace_buffer;
		else {
			trace_sptr.reset(new void *[total_frames]);
			trace_list = trace_sptr.get();
		}
		SYMBOL_INFO       symbol_info[4096 / sizeof(SYMBOL_INFO)];
		IMAGEHLP_MODULE64 module_info;
		module_info.SizeOfStruct = sizeof(module_info);
		symbol_info[0].MaxNameLen   = (sizeof(symbol_info) -
			sizeof(symbol_info[0])) - 1;
		symbol_info[0].SizeOfStruct = sizeof(SYMBOL_INFO);
		trace_count = ::CaptureStackBackTrace(skip_frame_count, total_frames,
			trace_list, NULL);
		for (USHORT count_1 = 0; count_1 < trace_count; ++count_1) {
			try {
				tmp_dst_list.push_back(
					StackTrace(NULL, NULL, 0, NULL, trace_list[count_1]));
				::SymFromAddr(process_handle,
					reinterpret_cast<DWORD64>(trace_list[count_1]), 0, symbol_info);
				if (flags & Flags_FuncName)
					tmp_dst_list.back().func_name_   = symbol_info[0].Name;
				tmp_dst_list.back().func_offset_    = static_cast<unsigned int>(
					reinterpret_cast<ULONG64>(trace_list[count_1]) -
					symbol_info[0].Address);
				tmp_dst_list.back().func_address_   =
					reinterpret_cast<const void *>(symbol_info[0].Address);
				if (flags & Flags_Extra_1)
					tmp_dst_list.back().symbol_info_ = symbol_info[0];
/*
				std::cout
					<< "0x" << std::right << std::setfill('0') << std::hex
					<< std::setw(sizeof(void *) * 2) <<
						symbol_info[0].Address
					<< " " << std::dec << std::setfill(' ')
					<< symbol_info[0].Name
						;
*/
				if ((flags & Flags_FileName) && symbol_info[0].ModBase) {
					if (::SymGetModuleInfo64(process_handle, symbol_info[0].ModBase,
						&module_info) != TRUE) {
						std::ostringstream o_str;
						o_str << "Attempt to retrieve the module information for "
							"the module located beginning at address 0x" <<
							std::hex << symbol_info[0].ModBase << std::dec <<
							" with 'SymGetModuleInfo64()' failed";
						ThrowSystemError(o_str.str());
					}
					if (flags & Flags_FileName)
						tmp_dst_list.back().file_name_ = module_info.ImageName;
					if (flags & Flags_Extra_2)
						tmp_dst_list.back().module_info_ = module_info;
				}
//				std::cout << std::endl;
			}
			catch (const std::exception &except) {
				std::ostringstream o_str;
				o_str << "Unable to resolve back trace symbol information for "
					"frame index " << count_1 << " (with " << skip_frame_count <<
					" frames skipped) and a total of " << trace_count <<
					"frames captured: " << except.what();
				Rethrow(except, o_str.str());
			}
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Unable to assemble stack trace data with flags 0x" <<
			std::hex << std::setfill('0') << std::setw(4) << 
			flags << std::dec << std::setfill(' ') << ": " << except.what();
		Rethrow(except, o_str.str());
	}

	dst_list.swap(tmp_dst_list);

	return(dst_list);
}
// ////////////////////////////////////////////////////////////////////////////

#else

// ////////////////////////////////////////////////////////////////////////////
StackTraceVector &StackTrace::GetBackTrace(StackTraceVector &dst_list,
	StackTrace::Flags flags, std::size_t max_frame_count,
	std::size_t skip_frame_count)
{
	StackTraceVector tmp_dst_list;

	max_frame_count  = (max_frame_count) ? max_frame_count :
		DefaultMaxFrameCount;
	max_frame_count  = (max_frame_count <= MaxFrameTotalCount) ?
		max_frame_count : MaxFrameTotalCount;
	skip_frame_count = (skip_frame_count <= MaxFrameTotalCount) ?
		skip_frame_count : MaxFrameTotalCount;

	std::size_t total_frames = max_frame_count + skip_frame_count;

	total_frames = (total_frames <= MaxFrameTotalCount) ?
		total_frames : MaxFrameTotalCount;

	try {
		int                           trace_count;
		void                         *trace_buffer[StackTraceBufferSize];
		void                        **trace_list;
		boost::scoped_array<void *>   trace_sptr;
		if (total_frames <= StackTraceBufferSize)
			trace_list = trace_buffer;
		else {
			trace_sptr.reset(new void *[total_frames]);
			trace_list = trace_sptr.get();
		}
		trace_count = static_cast<int>(total_frames);
		int func_count;
		if ((func_count = ::backtrace(trace_list, trace_count)) < 1) {
			std::ostringstream o_str;
			o_str << "Invocation of ::backtrace() returned " << func_count << ".";
			throw std::runtime_error(o_str.str());
		}
		boost::scoped_array<const char *> func_list(const_cast<const char **>(
			::backtrace_symbols(trace_list, func_count)));
		for (int count_1 = static_cast<int>(skip_frame_count);
			count_1 < func_count; ++count_1) {
			StackTraceWork work_item(StackTraceWork::Extract(func_list[count_1]));
			work_item.ParseNumericValues();
			tmp_dst_list.push_back(
				StackTrace(
					(flags & Flags_FileName) ? work_item.file_name_ptr_ : NULL,
					(flags & Flags_FuncName) ? ((flags & Flags_NoDemangle) ?
						work_item.func_name_ptr_ :
						work_item.GetFunctionNameDemangled().c_str()) : NULL,
					work_item.GetFunctionOffset(),
					work_item.ParseFunctionAddress(),
					trace_list[count_1]
				)
			);
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Unable to assemble stack trace data with flags 0x" <<
			std::hex << std::setfill('0') << std::setw(4) << 
			flags << std::dec << std::setfill(' ') << ": " << except.what();
		Rethrow(except, o_str.str());
	}

	dst_list.swap(tmp_dst_list);


	return(dst_list);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef _MSC_VER


// ////////////////////////////////////////////////////////////////////////////
StackTraceVector StackTrace::GetBackTrace(StackTrace::Flags flags,
	std::size_t max_frame_count, std::size_t skip_frame_count)
{
	StackTraceVector dst_list;

	return(GetBackTrace(dst_list, flags, max_frame_count,
		(skip_frame_count) ? (skip_frame_count + 1) : 0));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef _MSC_VER

namespace MLB {

namespace Utility {

/*
// ////////////////////////////////////////////////////////////////////////////
const std::size_t MaxFrameTotalCount    = 0x7fff;
const std::size_t DefaultMaxFrameCount  = 1024;
const std::size_t DefaultSkipFrameCount = 0;
// private:
const std::size_t StackTraceBufferSize  = 256;
// ////////////////////////////////////////////////////////////////////////////
*/

} // namespace Utility

} // namespace MLB

#else

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
/*
	backtrace_symbols() output formats:

		<invocation-file-name>(<symbol-name>+0x<hex-offset>) [0x<hex-address>]

		PCRE: /^(.+)\(((.+)\+0x([\dA-Fa-f]+))*\)\s+\[0x([\dA-Fa-f]+)\]$

		Print the following captures: $1 $3 $4 $5

	Parentheses are empty for functions not compiled with -rdynamic
*/
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
void StackTraceWork::swap(StackTraceWork &other)
{
	std::swap(file_name_size_,     other.file_name_size_);
	std::swap(file_name_ptr_,      other.file_name_ptr_);
	std::swap(func_name_size_,     other.func_name_size_);
	std::swap(func_name_ptr_,      other.func_name_ptr_);
	std::swap(func_offset_size_,   other.func_offset_size_);
	std::swap(func_offset_ptr_,    other.func_offset_ptr_);
	std::swap(func_address_size_,  other.func_address_size_);
	std::swap(func_address_ptr_,   other.func_address_ptr_);
	std::swap(func_offset_value_,  other.func_offset_value_);
	std::swap(func_address_value_, other.func_address_value_);
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const boost::regex MyBackTraceMatcher(
	"^(.+)\\(((.+)\\+0x([\\dA-Fa-f]+))*\\)\\s+\\[0x([\\dA-Fa-f]+)\\]$",
	boost::regex::perl | boost::regex::icase);
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
StackTraceWork &StackTraceWork::Extract(const char *src,
	StackTraceWork &dst)
{
//		PCRE: /^(.+)\(((.+)\+0x([\dA-Fa-f]+))*\)\s+\[0x([\dA-Fa-f]+)\]$

	if ((!src) || (!(*src)))
		throw std::invalid_argument("Null src parameter.");

	StackTraceWork tmp_dst;
	boost::cmatch     results;

	if (boost::regex_match(src, results, MyBackTraceMatcher,
		boost::match_default) && (results.size() == 6) && results[1].matched &&
		results[5].matched) {
		if (results[3].matched && results[4].matched)
			StackTraceWork(
				results[1].second - results[1].first,
				results[1].first,
				results[3].second - results[3].first,
				results[3].first,
				results[4].second - results[4].first,
				results[4].first,
				results[5].second - results[5].first,
				results[5].first).swap(dst);
		else
			StackTraceWork(
				results[1].second - results[1].first,
				results[1].first,
				0,
				NULL,
				0,
				NULL,
				results[5].second - results[5].first,
				results[5].first).swap(dst);
	}
	else
		StackTraceWork().swap(dst);

	return(dst);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StackTraceWork StackTraceWork::Extract(const char *src)
{
	StackTraceWork dst;

	return(Extract(src, dst));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StackTraceWorkVector &StackTraceWork::Extract(int src_count,
	const char **src_list, StackTraceWorkVector &dst_list)
{
	StackTraceWorkVector tmp_dst_list;

	if (src_count > 0) {
		for (int count_1 = 0; count_1 < src_count; ++count_1)
			tmp_dst_list.push_back(Extract(src_list[count_1]));
	}

	dst_list.swap(tmp_dst_list);

	return(dst_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StackTraceWorkVector StackTraceWork::Extract(int src_count, 
	const char **src_list)
{
	StackTraceWorkVector dst_list;

	return(Extract(src_count, src_list, dst_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
StackTraceWorkVector &StackTraceWork::FixUpNumericValues(
	StackTraceWorkVector &trace_list)
{
	StackTraceWorkVectorIter iter_b(trace_list.begin());
	StackTraceWorkVectorIter iter_e(trace_list.end());

	for ( ; iter_b != iter_e; ++iter_b)
		iter_b->ParseNumericValues();

	return(trace_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::pair<std::size_t, std::size_t> StackTraceWork::GetMaxStringLengths(
	const StackTraceWorkVector &trace_list, bool demangle_flag)
{
	std::pair<std::size_t, std::size_t> results(0, 0);
	StackTraceWorkVectorIterC  iter_b(trace_list.begin());
	StackTraceWorkVectorIterC  iter_e(trace_list.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		results.first  = std::max(results.first,  iter_b->file_name_size_);
		results.second = std::max(results.second, (!demangle_flag) ?
			iter_b->func_name_size_ : iter_b->GetFunctionNameDemangled().size());
	}

	return(results);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void StackTraceWork::Emit(const StackTraceWorkVector &trace_list,
	bool demangle_flag, std::ostream &o_str)
{
	std::pair<std::size_t, std::size_t> max_lengths(
		GetMaxStringLengths(trace_list, demangle_flag));

	StackTraceWorkVectorIterC iter_b(trace_list.begin());
	StackTraceWorkVectorIterC iter_e(trace_list.end());

	o_str << std::setfill(' ');

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->file_name_ptr_) {
			o_str
				<< std::left
				<< std::setw(max_lengths.first) << iter_b->GetFileName() << " "
				<< "0x" << std::right << std::setfill('0') << std::hex
				<< std::setw(sizeof(void *) * 2) <<
					reinterpret_cast<unsigned long long>(
					iter_b->GetFunctionAddress())
				<< " " << std::dec << std::setfill(' ')
					;
			if (iter_b->func_name_ptr_)
				o_str
					<< std::left
					<< std::setw(max_lengths.second) <<
						iter_b->GetFunctionName(demangle_flag)
					<< " "
					<< "0x" << std::right << std::setfill('0') << std::hex
					<< std::setw(4) << iter_b->GetFunctionOffset()
					<< " " << std::dec << std::setfill(' ')
						;
			o_str << std::endl;
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_GetBackTrace()
{
	const int  list_count = 1024;
	void      *list_data[list_count];
	int        trace_count;

	if ((trace_count = ::backtrace(list_data, list_count)) < 1) {
		std::ostringstream o_str;
		o_str << "Invocation of ::backtrace() returned " << trace_count << ".";
		throw std::runtime_error(o_str.str());
	}

	boost::shared_ptr<const char *> trace_list(const_cast<const char **>(
		::backtrace_symbols(list_data, trace_count)));

	StackTraceWorkVector trace_vector(
		StackTraceWork::Extract(trace_count, trace_list.get()));
	StackTraceWork::FixUpNumericValues(trace_vector);

	StackTraceWork::Emit(trace_vector, false);
	std::cout << std::endl;

	StackTraceWork::Emit(trace_vector, true);
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifdef _MSC_VER

#ifdef TEST_MAIN

#include <cstdlib>

using namespace MLB::Utility;

namespace TEST_NamespaceName {

// ////////////////////////////////////////////////////////////////////////////
void TEST_GetBackTrace(StackTrace::Flags flags = StackTrace::Flags_Default,
	std::size_t max_frame_count = DefaultMaxFrameCount,
	std::size_t skip_frame_count = DefaultSkipFrameCount)
{
	StackTraceVector dst_list;

	StackTrace::GetBackTrace(dst_list, flags, max_frame_count,
		skip_frame_count + 1);

	StackTraceVectorIterC iter_b(dst_list.begin());
	StackTraceVectorIterC iter_e(dst_list.end());

	for ( ; iter_b != iter_e; ++iter_b)
		std::cout
			<< "0x" << std::right << std::setfill('0') << std::hex
			<< std::setw(sizeof(void *) * 2) <<
				iter_b->func_address_
			<< " " << std::dec << std::setfill(' ')
			<< iter_b->func_name_
			<< std::endl;
				;

/*
	max_frame_count  = (max_frame_count) ? max_frame_count :
		DefaultMaxFrameCount;
	max_frame_count  = (max_frame_count <= MaxFrameTotalCount) ?
		max_frame_count : MaxFrameTotalCount;
	skip_frame_count = (skip_frame_count <= MaxFrameTotalCount) ?
		skip_frame_count : MaxFrameTotalCount;

	std::size_t total_frames = max_frame_count + skip_frame_count;

	total_frames = (total_frames <= MaxFrameTotalCount) ?
		total_frames : MaxFrameTotalCount;

	try {
		HANDLE process_handle = ::GetCurrentProcess();
		::SymInitialize(process_handle, NULL, TRUE);
		USHORT                        trace_count;
		void                         *trace_buffer[StackTraceBufferSize];
		void                        **trace_list;
		boost::scoped_array<void *>   trace_sptr;
		if (total_frames <= StackTraceBufferSize)
			trace_list = trace_buffer;
		else {
			trace_sptr.reset(new void *[total_frames]);
			trace_list = trace_sptr.get();
		}
		SYMBOL_INFO  symbol_info[4096 / sizeof(SYMBOL_INFO)];
		IMAGEHLP_MODULE64 module_info;
		module_info.SizeOfStruct = sizeof(module_info);
		symbol_info[0].MaxNameLen   = (sizeof(symbol_info) -
			sizeof(symbol_info[0])) - 1;
		symbol_info[0].SizeOfStruct = sizeof(SYMBOL_INFO);
		trace_count = ::CaptureStackBackTrace(skip_frame_count, total_frames,
			trace_list, NULL);
		for (USHORT count_1 = 0; count_1 < trace_count; ++count_1) {
			try {
				::SymFromAddr(process_handle,
					reinterpret_cast<DWORD64>(trace_list[count_1]), 0, symbol_info);
				std::cout
					<< "0x" << std::right << std::setfill('0') << std::hex
					<< std::setw(sizeof(void *) * 2) <<
						symbol_info[0].Address
					<< " " << std::dec << std::setfill(' ')
					<< symbol_info[0].Name
						;
				if (symbol_info[0].ModBase) {
					if (::SymGetModuleInfo64(process_handle, symbol_info[0].ModBase,
						&module_info) != TRUE) {
						std::ostringstream o_str;
						o_str << "Attempt to retrieve the module information for "
							"the module located beginning at address 0x" <<
							std::hex << symbol_info[0].ModBase << std::dec <<
							" with 'SymGetModuleInfo64()' failed";
						ThrowSystemError(o_str.str());
					}
				}
				std::cout << std::endl;
			}
			catch (const std::exception &except) {
				std::ostringstream o_str;
				o_str << "Unable to resolve back trace symbol information for "
					"frame index " << count_1 << " (with " << skip_frame_count <<
					" frames skipped) and a total of " << trace_count <<
					"frames captured: " << except.what();
				Rethrow(except, o_str.str());
			}
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Unable to assemble stack trace data with flags 0x" <<
			std::hex << std::setfill('0') << std::setw(4) << 
			flags << std::dec << std::setfill(' ') << ": " << except.what();
		Rethrow(except, o_str.str());
	}

	dst_list.swap(tmp_dst_list);

	return(dst_list);
*/
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct TEST_ClassBottom {
	void NonStaticNonConstFunc()
	{
		NonStaticConstFunc();
	}

	void NonStaticConstFunc() const
	{
		StaticFunc();
	}

	static void StaticFunc()
	{
		TEST_GetBackTrace();
	}
};
//	----------------------------------------------------------------------------
void TEST_InterveningFreeFunc_3()
{
	TEST_ClassBottom bottom_instance;

	bottom_instance.NonStaticConstFunc();
}
//	----------------------------------------------------------------------------
void TEST_InterveningFreeFunc_2()
{
	TEST_InterveningFreeFunc_3();
}
//	----------------------------------------------------------------------------
void TEST_InterveningFreeFunc_1()
{
	TEST_InterveningFreeFunc_2();
}
//	----------------------------------------------------------------------------
struct TEST_ClassTop {
	void NonStaticNonConstFunc()
	{
		NonStaticConstFunc();
	}

	void NonStaticConstFunc() const
	{
		StaticFunc();
	}

	static void StaticFunc()
	{
		TEST_InterveningFreeFunc_1();
	}
};
//	----------------------------------------------------------------------------
void TEST_RunTest(int &return_code)
{
	try {
		TEST_ClassTop top_instance;
		top_instance.NonStaticNonConstFunc();
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR in TEST_RunTest(): " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace TEST_NamespaceName

int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_NamespaceName::TEST_RunTest(return_code);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}

#endif // #ifdef TEST_MAIN

