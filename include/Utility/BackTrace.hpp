// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	BackTrace.hpp

	File Description	:	Definition of the BackTrace class.

	Revision History	:	2015-07-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__BackTrace_hpp__HH

#define HH__MLB__Utility__BackTrace_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file BackTrace.hpp

	\brief	The BackTrace class header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#ifdef _MSC_VER
# include <DbgHelp.h>
#endif // #ifdef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
const std::size_t MaxFrameTotalCount    = 0x7fff;
const std::size_t DefaultMaxFrameCount  = 1024;
const std::size_t DefaultSkipFrameCount = 0;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_UTILITY BackTrace {
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

	typedef std::vector<BackTrace> BackTraceVector_I;

	explicit BackTrace(const char *file_name = NULL,
		const char *func_name = NULL, std::size_t func_offset = 0,
		const void *func_address = NULL, const void  *current_address = NULL)
		:file_name_((file_name) ? file_name : "")
		,func_name_((func_name) ? func_name : "")
		,func_offset_(func_offset)
		,func_address_(func_address)
		,current_address_(current_address)
	{
	}

	void swap(BackTrace &other);

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

	static BackTraceVector_I &GetBackTrace(BackTraceVector_I &dst_list,
		BackTrace::Flags flags = BackTrace::Flags_Default,
		std::size_t max_frame_count = DefaultMaxFrameCount,
		std::size_t skip_frame_count = DefaultSkipFrameCount);
	static BackTraceVector_I  GetBackTrace(
		BackTrace::Flags flags = BackTrace::Flags_Default,
		std::size_t max_frame_count = DefaultMaxFrameCount,
		std::size_t skip_frame_count = DefaultSkipFrameCount);

	static std::pair<std::size_t, std::size_t> GetMaxStringLengths(
		const BackTraceVector_I &trace_list, bool demangle_flag = false);

	static void Emit(const BackTraceVector_I &trace_list,
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
typedef BackTrace::BackTraceVector_I   BackTraceVector;
typedef BackTraceVector::iterator 		  BackTraceVectorIter;
typedef BackTraceVector::const_iterator BackTraceVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

#ifdef __linux__

// ////////////////////////////////////////////////////////////////////////////
struct API_UTILITY BackTraceWork {
	typedef std::vector<BackTraceWork> BackTraceWorkVector_I;

	BackTraceWork(
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

	void swap(BackTraceWork &other);

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

	std::string GetFunctionNameDemangled() const;

	std::size_t GetFunctionOffset() const
	{
		return(func_offset_value_);
	}

	const void *GetFunctionAddress() const
	{
		return(func_address_value_);
	}

	std::size_t ParseFunctionOffset();

	const void *ParseFunctionAddress();

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

	static BackTraceWork &Extract(const char *src, BackTraceWork &dst);
	static BackTraceWork  Extract(const char *src);

	static BackTraceWorkVector_I &Extract(int src_count,
		const char **src_list, BackTraceWorkVector_I &dst_list);
	static BackTraceWorkVector_I  Extract(int src_count, 
		const char **src_list);

	static BackTraceWorkVector_I &FixUpNumericValues(
		BackTraceWorkVector_I &trace_list);

	static std::pair<std::size_t, std::size_t> GetMaxStringLengths(
		const BackTraceWorkVector_I &trace_list, bool demangle_flag = false);

	static void Emit(const BackTraceWorkVector_I &trace_list,
		bool demangle_flag = false, std::ostream &o_str = std::cout);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef BackTraceWork::BackTraceWorkVector_I BackTraceWorkVector;
typedef BackTraceWorkVector::iterator 			  BackTraceWorkVectorIter;
typedef BackTraceWorkVector::const_iterator 	  BackTraceWorkVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef __linux__

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__BackTrace_hpp__HH

