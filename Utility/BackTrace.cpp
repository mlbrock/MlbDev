// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	BackTrace.cpp

	File Description	:	Lightweight stack trace facility.

	Revision History	:	2015-07-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/BackTrace.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __linux__
# include <execinfo.h>
# include <cxxabi.h>
# include <boost/regex.hpp>
#endif // #ifdef __linux__

#include <boost/scoped_array.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
const std::size_t BackTraceBufferSize  = 256;
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
BackTraceVector BackTrace::GetBackTrace(BackTrace::Flags flags,
	std::size_t max_frame_count, std::size_t skip_frame_count)
{
	BackTraceVector dst_list;

	return(GetBackTrace(dst_list, flags, max_frame_count,
		(skip_frame_count) ? (skip_frame_count + 1) : 0));
}
// ////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////
BackTraceVector &BackTrace::GetBackTrace(BackTraceVector &dst_list,
	BackTrace::Flags flags, std::size_t max_frame_count,
	std::size_t skip_frame_count)
{
	BackTraceVector tmp_dst_list;

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
		void                         *trace_buffer[BackTraceBufferSize];
		void                        **trace_list;
		boost::scoped_array<void *>   trace_sptr;
		if (total_frames <= BackTraceBufferSize)
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
					BackTrace(NULL, NULL, 0, NULL, trace_list[count_1]));
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
BackTraceVector &BackTrace::GetBackTrace(BackTraceVector &dst_list,
	BackTrace::Flags flags, std::size_t max_frame_count,
	std::size_t skip_frame_count)
{
	BackTraceVector tmp_dst_list;

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
		void                         *trace_buffer[BackTraceBufferSize];
		void                        **trace_list;
		boost::scoped_array<void *>   trace_sptr;
		if (total_frames <= BackTraceBufferSize)
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
			BackTraceWork work_item(BackTraceWork::Extract(func_list[count_1]));
			work_item.ParseNumericValues();
			tmp_dst_list.push_back(
				BackTrace(
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

} // namespace Utility

} // namespace MLB

#ifdef __linux__

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

namespace {

// ////////////////////////////////////////////////////////////////////////////
const boost::regex MyBackTraceMatcher(
	"^(.+)\\(((.+)\\+0x([\\dA-Fa-f]+))*\\)\\s+\\[0x([\\dA-Fa-f]+)\\]$",
	boost::regex::perl | boost::regex::icase);
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void BackTraceWork::swap(BackTraceWork &other)
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

// ////////////////////////////////////////////////////////////////////////////
std::size_t BackTraceWork::ParseFunctionOffset()
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
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const void *BackTraceWork::ParseFunctionAddress()
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
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string BackTraceWork::GetFunctionNameDemangled() const
{
	if (!func_name_ptr_)
		return("");

	std::string             tmp_name(func_name_ptr_, func_name_size_);
	int                     status_code;
	boost::scoped_ptr<char> final_name(abi::__cxa_demangle(
		tmp_name.c_str(), NULL, NULL, &status_code));

	return((!status_code) ? std::string(final_name.get()) : tmp_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BackTraceWork &BackTraceWork::Extract(const char *src,
	BackTraceWork &dst)
{
//		PCRE: /^(.+)\(((.+)\+0x([\dA-Fa-f]+))*\)\s+\[0x([\dA-Fa-f]+)\]$

	if ((!src) || (!(*src)))
		throw std::invalid_argument("Null src parameter.");

	BackTraceWork tmp_dst;
	boost::cmatch     results;

	if (boost::regex_match(src, results, MyBackTraceMatcher,
		boost::match_default) && (results.size() == 6) && results[1].matched &&
		results[5].matched) {
		if (results[3].matched && results[4].matched)
			BackTraceWork(
				results[1].second - results[1].first,
				results[1].first,
				results[3].second - results[3].first,
				results[3].first,
				results[4].second - results[4].first,
				results[4].first,
				results[5].second - results[5].first,
				results[5].first).swap(dst);
		else
			BackTraceWork(
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
		BackTraceWork().swap(dst);

	return(dst);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BackTraceWork BackTraceWork::Extract(const char *src)
{
	BackTraceWork dst;

	return(Extract(src, dst));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BackTraceWorkVector &BackTraceWork::Extract(int src_count,
	const char **src_list, BackTraceWorkVector &dst_list)
{
	BackTraceWorkVector tmp_dst_list;

	if (src_count > 0) {
		for (int count_1 = 0; count_1 < src_count; ++count_1)
			tmp_dst_list.push_back(Extract(src_list[count_1]));
	}

	dst_list.swap(tmp_dst_list);

	return(dst_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BackTraceWorkVector BackTraceWork::Extract(int src_count, 
	const char **src_list)
{
	BackTraceWorkVector dst_list;

	return(Extract(src_count, src_list, dst_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BackTraceWorkVector &BackTraceWork::FixUpNumericValues(
	BackTraceWorkVector &trace_list)
{
	BackTraceWorkVectorIter iter_b(trace_list.begin());
	BackTraceWorkVectorIter iter_e(trace_list.end());

	for ( ; iter_b != iter_e; ++iter_b)
		iter_b->ParseNumericValues();

	return(trace_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::pair<std::size_t, std::size_t> BackTraceWork::GetMaxStringLengths(
	const BackTraceWorkVector &trace_list, bool demangle_flag)
{
	std::pair<std::size_t, std::size_t> results(0, 0);
	BackTraceWorkVectorIterC  iter_b(trace_list.begin());
	BackTraceWorkVectorIterC  iter_e(trace_list.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		results.first  = std::max(results.first,  iter_b->file_name_size_);
		results.second = std::max(results.second, (!demangle_flag) ?
			iter_b->func_name_size_ : iter_b->GetFunctionNameDemangled().size());
	}

	return(results);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void BackTraceWork::Emit(const BackTraceWorkVector &trace_list,
	bool demangle_flag, std::ostream &o_str)
{
	std::pair<std::size_t, std::size_t> max_lengths(
		GetMaxStringLengths(trace_list, demangle_flag));

	BackTraceWorkVectorIterC iter_b(trace_list.begin());
	BackTraceWorkVectorIterC iter_e(trace_list.end());

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

} // namespace Utility

} // namespace MLB

#endif // #ifdef __linux__

#ifdef TEST_MAIN

#include <cstdlib>

using namespace MLB::Utility;

namespace TEST_Namespace {

namespace InnerNamespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_GetBackTrace(BackTrace::Flags flags = BackTrace::Flags_Default,
	std::size_t max_frame_count = DefaultMaxFrameCount,
	std::size_t skip_frame_count = DefaultSkipFrameCount)
{
	BackTraceVector dst_list;

	BackTrace::GetBackTrace(dst_list, flags, max_frame_count,
		skip_frame_count + 1);

	BackTraceVectorIterC iter_b(dst_list.begin());
	BackTraceVectorIterC iter_e(dst_list.end());

	for ( ; iter_b != iter_e; ++iter_b)
		std::cout
			<< "0x" << std::right << std::setfill('0') << std::hex
			<< std::setw(sizeof(unsigned long long) * 2) <<
				reinterpret_cast<unsigned long long>(iter_b->func_address_)
			<< " " << std::dec << std::setfill(' ')
			<< iter_b->func_name_
			<< std::endl;
				;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace InnerNamespace

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
		InnerNamespace::TEST_GetBackTrace();
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

} // namespace TEST_Namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_Namespace::TEST_RunTest(return_code);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

