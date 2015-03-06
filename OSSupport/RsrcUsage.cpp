// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the RsrcUsage class.

	Revision History	:	1992-10-11 --- Initial genfuncs C library logic.
									Michael L. Brock
								1998-04-08 --- Modified for C++.
									Michael L. Brock

		Copyright Michael L. Brock 1992 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

//#include <OSSupport_RsrcUsage.h>

//	CODE NOTE: To be removed.
#include <OSSupport.h>

#include <Utility/StringSupport.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>
#include <Utility/InlineContainer.hpp>

#include <memory.h>

#ifdef __SVR4
# define __EXTENSIONS__
# include <sys/types.h>
# include <sys/signal.h>
# include <sys/fault.h>
# include <sys/syscall.h>
# include <sys/procfs.h>
#endif // #ifdef __SVR4


//	CPP only.
#include <map>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

//	////////////////////////////////////////////////////////////////////////////
typedef MLB::Utility::TimeSpec RsrcUsageTime;
typedef unsigned long long     RsrcUsageValue;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RsrcUsageValue RsrcUsageValueMax =
	std::numeric_limits<RsrcUsageValue>::max();
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
/* BOH

	NAME			:	RsrcUsage

	DESCRIPTION	:	The ''RsrcUsage'' structure contains members which describe,
						in a fashion independent of the operating system, the
						resource usage of an executable.

						The ''RsrcUsage'' structure is declared as follows:

{|@|}typedef struct {
	RsrcUsageTime   user_cpu_time;
	RsrcUsageTime   system_cpu_time;
	RsrcUsageTime   trap_cpu_time;
	RsrcUsageTime   text_pagef_time;
	RsrcUsageTime   data_pagef_time;
	RsrcUsageTime   kernel_pagef_time;
	RsrcUsageTime   user_lock_time;
	RsrcUsageTime   other_sleep_time;
	RsrcUsageTime   wait_cpu_time;
	RsrcUsageTime   stopped_time;
	RsrcUsageValue  minor_pagef;
	RsrcUsageValue  major_pagef;
	RsrcUsageValue  process_swaps;
	RsrcUsageValue  input_blocks;
	RsrcUsageValue  output_blocks;
	RsrcUsageValue  messages_sent;
	RsrcUsageValue  messages_received;
	RsrcUsageValue  messages_other;
	RsrcUsageValue  signals_received;
	RsrcUsageValue  vol_context_switch;
	RsrcUsageValue  invol_context_switch;
	RsrcUsageValue  system_calls;
	RsrcUsageValue  chars_read_written;
	RsrcUsageValue  chars_read;
	RsrcUsageValue  chars_written;
	RsrcUsageValue  chars_other;
	RsrcUsageValue  working_set_size;
	RsrcUsageValue  working_set_size_peak;
	RsrcUsageValue  pagefile_usage;
	RsrcUsageValue  pagefile_usage_peak;
} RsrcUsage;

	MEMBERS		:	The members of this structure are populated by the function
						``GetRsrcUsage`` depending upon the run-time operating
						system.

						The table below shows the mappings between the members of
						the ''RsrcUsage'' structure, the BSD ''struct rusage''
						structure, the System V ''struct prusage'' structure and
						The Windows API calls.

						You will note that all mappings are not available from all
						operating systems.

{|@|}	Utility                    SunOS/BSD               Solaris/SysV             Microsoft
	RsrcUsage                 struct rusage           struct prusage           Windows
	--------------------       ------------------      -------------------      ---------------------------------------------------------------------------
	user_cpu_time         ---> rusage.ru_utime    ---> prusage.pr_utime    ---> GetProcessTimes().lpUserTime.SDTIF_WIN32_FILETIME_64
	system_cpu_time       ---> rusage.ru_stime    ---> prusage.pr_stime    ---> GetProcessTimes().lpKernelTime.SDTIF_WIN32_FILETIME_64
	trap_cpu_time         --->                    ---> prusage.pr_ttime    ---> 
	text_pagef_time       --->                    ---> prusage.pr_tftime   ---> 
	data_pagef_time       --->                    ---> prusage.pr_dftime   ---> 
	kernel_pagef_time     --->                    ---> prusage.pr_kftime   ---> 
	user_lock_time        --->                    ---> prusage.pr_ltime    ---> 
	other_sleep_time      --->                    ---> prusage.pr_slptime  ---> 
	wait_cpu_time         --->                    ---> prusage.pr_wtime    ---> 
	stopped_time          --->                    ---> prusage.pr_stoptime ---> 
	minor_pagef           ---> rusage.ru_minflt   ---> prusage.pr_minf     ---> 
	major_pagef           ---> rusage.ru_majflt   ---> prusage.pr_majf     ---> GetProcessMemoryInfo().PROCESS_MEMORY_COUNTERS.PageFaultCount
	process_swaps         ---> rusage.ru_nswap    ---> prusage.pr_nswap    ---> 
	input_blocks          ---> rusage.ru_inblock  ---> prusage.pr_inblk    ---> 
	output_blocks         ---> rusage.ru_oublock  ---> prusage.pr_oublk    ---> 
	messages_sent         ---> rusage.ru_msgsnd   ---> prusage.pr_msnd     ---> GetProcessIoCounters().IO_COUNTERS.WriteOperationCount
	messages_received     ---> rusage.ru_msgrcv   ---> prusage.pr_mrcv     ---> GetProcessIoCounters().IO_COUNTERS.ReadOperationCount
	messages_other        --->                    --->                     ---> GetProcessIoCounters().IO_COUNTERS.OtherOperationCount
	signals_received      ---> rusage.ru_nsignals ---> prusage.pr_sigs     ---> 
	vol_context_switch    ---> rusage.ru_nvcsw    ---> prusage.pr_vctx     ---> 
	invol_context_switch  ---> rusage.ru_nivcsw   ---> prusage.pr_ictx     ---> 
	system_calls          --->                    ---> prusage.sysc        ---> 
	chars_read_written    --->                    ---> prusage.ioch        ---> GetProcessIoCounters().IO_COUNTERS.ReadOperationCount + ReadOperationCount
	chars_read            --->                    --->                     ---> GetProcessIoCounters().IO_COUNTERS.ReadTransferCount
	chars_written         --->                    --->                     ---> GetProcessIoCounters().IO_COUNTERS.WriteTransferCount
	chars_other           --->                    --->                     ---> GetProcessIoCounters().IO_COUNTERS.OtherTransferCount
	working_set_size      --->                    --->                     ---> GetProcessMemoryInfo().PROCESS_MEMORY_COUNTERS.WorkingSetSize
	working_set_size_peak --->                    --->                     ---> GetProcessMemoryInfo().PROCESS_MEMORY_COUNTERS.PeakWorkingSetSize
	pagefile_usage        --->                    --->                     ---> GetProcessMemoryInfo().PROCESS_MEMORY_COUNTERS.PagefileUsage
	pagefile_usage_peak   --->                    --->                     ---> GetProcessMemoryInfo().PROCESS_MEMORY_COUNTERS.PeakPagefileUsage
	--------------------       ------------------      -------------------      ---------------------------------------------------------------------------

	AUTHOR		:	Michael L. Brock

	COPYRIGHT	:	Copyright 1992 - 2015 Michael L. Brock

	OUTPUT INDEX:	RsrcUsage
						Usage Description Structures:RsrcUsage
						Utility:Structures:RsrcUsage
						Utility:Usage Description Structures:RsrcUsage
						Utility Functions:See Utility

	PUBLISH XREF:	RsrcUsage

	PUBLISH NAME:	RsrcUsage

	ENTRY CLASS	:	Structures and Data Types

EOH */
//	////////////////////////////////////////////////////////////////////////////
struct API_OSSUPPORT RsrcUsage {
	enum RsrcUsageEmptyFormat {
		RsrcUsageEmptyFormatNone = 0,
		RsrcUsageEmptyFormatZero = 1,
		RsrcUsageEmptyFormatNull = 2,
		RsrcUsageEmptyFormatSkip = 3
	};
	RsrcUsage(bool get_now_flag = false);

	RsrcUsage(const RsrcUsage &other);
	RsrcUsage & operator = (const RsrcUsage &other);
	void swap(RsrcUsage &other);

	bool operator < (const RsrcUsage &other) const;

	void GetRsrcUsage(MLB::Utility::ProcessId selector);
	void GetRsrcUsage();

	std::string   ToString() const;
	std::string   ToStringLines(
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28, const std::string &separator = ": ") const;
	std::string &ToStringLines(std::string &out_string,
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28, const std::string &separator = ": ") const;

	MLB::Utility::StringVector  ToStringList(
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28, const std::string &separator = ": ") const;
	MLB::Utility::StringVector &ToStringList(MLB::Utility::StringVector &out_list,
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28, const std::string &separator = ": ") const;

	RsrcUsage GetRsrcUsageDelta() const;
	RsrcUsage GetRsrcUsageDelta(const RsrcUsage &end_rusage) const;

	static RsrcUsage   GetRsrcUsageInstance(MLB::Utility::ProcessId selector);
	static RsrcUsage   GetRsrcUsageInstance();
	static MLB::Utility::StringVector ToStringList(const RsrcUsage &rusage_datum,
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28);
	static void ToStringList(MLB::Utility::StringVector out_list,
		const RsrcUsage &rusage_datum,
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28);

	static RsrcUsage &GetRsrcUsageDelta(const RsrcUsage &start_rusage,
		const RsrcUsage &end_rusage, RsrcUsage &delta_rusage);
	static RsrcUsage  GetRsrcUsageDelta(const RsrcUsage &start_rusage,
		const RsrcUsage &end_rusage);

#ifdef _Windows
	static void GetRsrcUsageByWindowsHandle(HANDLE selector, RsrcUsage &datum);
#endif // #ifdef _Windows

	RsrcUsageTime  user_cpu_time;
	RsrcUsageTime  system_cpu_time;
	RsrcUsageTime  trap_cpu_time;
	RsrcUsageTime  text_pagef_time;
	RsrcUsageTime  data_pagef_time;
	RsrcUsageTime  kernel_pagef_time;
	RsrcUsageTime  user_lock_time;
	RsrcUsageTime  other_sleep_time;
	RsrcUsageTime  wait_cpu_time;
	RsrcUsageTime  stopped_time;
	RsrcUsageValue minor_pagef;
	RsrcUsageValue major_pagef;
	RsrcUsageValue process_swaps;
	RsrcUsageValue input_blocks;
	RsrcUsageValue output_blocks;
	RsrcUsageValue messages_sent;
	RsrcUsageValue messages_received;
	RsrcUsageValue messages_other;
	RsrcUsageValue signals_received;
	RsrcUsageValue vol_context_switch;
	RsrcUsageValue invol_context_switch;
	RsrcUsageValue system_calls;
	RsrcUsageValue chars_read_written;
	RsrcUsageValue chars_read;
	RsrcUsageValue chars_written;
	RsrcUsageValue chars_other;
	RsrcUsageValue working_set_size;
	RsrcUsageValue working_set_size_peak;
	RsrcUsageValue pagefile_usage;
	RsrcUsageValue pagefile_usage_peak;

private:
	typedef std::vector<RsrcUsageTime>  RsrcUsageTimeVector;
	typedef std::vector<RsrcUsageValue> RsrcUsageValueVector;

	void ToStringListTimes(RsrcUsageEmptyFormat null_format_type,
		unsigned int text_width, MLB::Utility::StringVector &out_list,
		const std::string &separator) const;
	void ToStringListValues(RsrcUsageEmptyFormat null_format_type,
		unsigned int text_width, MLB::Utility::StringVector &out_list,
		const std::string &separator) const;

	void GetNameListTimes(MLB::Utility::StringVector &out_list) const;
	void GetNameListValues(MLB::Utility::StringVector &out_list) const;
	void GetListTimes(RsrcUsageTimeVector &out_list) const;
	void GetListValues(RsrcUsageValueVector &out_list) const;
};
API_OSSUPPORT std::ostream & operator << (std::ostream &o_str,
	const RsrcUsage &datum);
//	////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
struct RsrcUsageElement {
	RsrcUsageElement(size_t member_offset,
		const std::string &title_name = "", const std::string &member_name = "",
		bool is_time_flag = false)
		:member_offset_(member_offset)
		,title_name_(title_name)
		,member_name_(member_name)
		,is_time_flag_(is_time_flag)
		,is_supported_flag_(true)
	{
	}

	bool operator < (const RsrcUsageElement &other) const {
		return(member_offset_ < other.member_offset_);
	}

	size_t      member_offset_;
	std::string title_name_;
	std::string member_name_;
	bool        is_time_flag_;
	bool        is_supported_flag_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RsrcUsageSpec {
	typedef std::set<RsrcUsageElement>          RsrcUsageElementSet;
	typedef RsrcUsageElementSet::const_iterator RsrcUsageElementSetIterC;

	RsrcUsageSpec();

	MLB::Utility::StringVector &ToStringList(const RsrcUsage &rusage_datum,
		RsrcUsage::RsrcUsageEmptyFormat null_format_type, unsigned int text_width,
		const std::string &separator, MLB::Utility::StringVector &out_list) const;

	RsrcUsageElementSet member_set_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#define RsrcUsageElement_DEF(member_title, member_name, time_flag)	\
	RsrcUsageElement(offsetof(RsrcUsage, member_name), member_title,	\
		#member_name, time_flag)
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//#define RsrcUsageSpec_NULL_DEF(member_name)	\
//	member_set_.find(RsrcUsageElement(			\
//		offsetof(RsrcUsage, member_name)))->is_supported_flag_ = false
#define RsrcUsageSpec_NULL_DEF(member_name)												\
	{																									\
		RsrcUsageSpec::RsrcUsageElementSet::iterator iter_f(member_set_.find(RsrcUsageElement(	\
			offsetof(RsrcUsage, member_name))));											\
		const_cast<RsrcUsageElement *>(&(*iter_f))->is_supported_flag_ = false;		\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const RsrcUsageElement PRIVATE_RsrcUsageMemberList[] = {
	//	Time members...
	RsrcUsageElement_DEF("User Level CPU Time",          user_cpu_time,         true),
	RsrcUsageElement_DEF("System Call CPU Time",         system_cpu_time,       true),
	RsrcUsageElement_DEF("Other System Trap CPU Time",   trap_cpu_time,         true),
	RsrcUsageElement_DEF("Text Page Fault Sleep Time",   text_pagef_time,       true),
	RsrcUsageElement_DEF("Data Page Fault Sleep Time",   data_pagef_time,       true),
	RsrcUsageElement_DEF("Kernel Page Fault Sleep Time", kernel_pagef_time,     true),
	RsrcUsageElement_DEF("User Lock Wait Sleep Time",    user_lock_time,        true),
	RsrcUsageElement_DEF("Other Sleep Time",             other_sleep_time,      true),
	RsrcUsageElement_DEF("Wait-CPU Latency Time",        wait_cpu_time,         true),
	RsrcUsageElement_DEF("Stopped Time",                 stopped_time,          true),
	//	Value members...
	RsrcUsageElement_DEF("Minor Page Faults",            minor_pagef,           false),
	RsrcUsageElement_DEF("Major Page Faults",            major_pagef,           false),
	RsrcUsageElement_DEF("Process Swaps",                process_swaps,         false),
	RsrcUsageElement_DEF("Input Blocks",                 input_blocks,          false),
	RsrcUsageElement_DEF("Output Blocks",                output_blocks,         false),
	RsrcUsageElement_DEF("Messages Sent",                messages_sent,         false),
	RsrcUsageElement_DEF("Messages Received",            messages_received,     false),
	RsrcUsageElement_DEF("Messages Other",               messages_other,        false),
	RsrcUsageElement_DEF("Signals Received",             signals_received,      false),
	RsrcUsageElement_DEF("Voluntary Context Switches",   vol_context_switch,    false),
	RsrcUsageElement_DEF("Involuntary Context Switches", invol_context_switch,  false),
	RsrcUsageElement_DEF("System Calls",                 system_calls,          false),
	RsrcUsageElement_DEF("Characters Read and Written",  chars_read_written,    false),
	RsrcUsageElement_DEF("Characters Read",              chars_read,            false),
	RsrcUsageElement_DEF("Characters Written",           chars_written,         false),
	RsrcUsageElement_DEF("Characters Other",             chars_other,           false),
	RsrcUsageElement_DEF("Working Set Size",             working_set_size,      false),
	RsrcUsageElement_DEF("Working Set Size Peak",        working_set_size_peak, false),
	RsrcUsageElement_DEF("Pagefile Usage",               pagefile_usage,        false),
	RsrcUsageElement_DEF("Pagefile Usage Peak",          pagefile_usage_peak,   false)
};
const unsigned int         PRIVATE_RsrcUsageMemberCount  =
	sizeof(PRIVATE_RsrcUsageMemberList) / sizeof(PRIVATE_RsrcUsageMemberList[0]);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsageSpec::RsrcUsageSpec()
	:member_set_(PRIVATE_RsrcUsageMemberList,
		PRIVATE_RsrcUsageMemberList + PRIVATE_RsrcUsageMemberCount)
{
#ifdef __MSDOS__
#elif _Windows
	RsrcUsageSpec_NULL_DEF(trap_cpu_time);
	RsrcUsageSpec_NULL_DEF(text_pagef_time);
	RsrcUsageSpec_NULL_DEF(data_pagef_time);
	RsrcUsageSpec_NULL_DEF(kernel_pagef_time);
	RsrcUsageSpec_NULL_DEF(user_lock_time);
	RsrcUsageSpec_NULL_DEF(other_sleep_time);
	RsrcUsageSpec_NULL_DEF(wait_cpu_time);
	RsrcUsageSpec_NULL_DEF(stopped_time);
	RsrcUsageSpec_NULL_DEF(minor_pagef);
	RsrcUsageSpec_NULL_DEF(process_swaps);
	RsrcUsageSpec_NULL_DEF(input_blocks);
	RsrcUsageSpec_NULL_DEF(output_blocks);
	RsrcUsageSpec_NULL_DEF(signals_received);
	RsrcUsageSpec_NULL_DEF(vol_context_switch);
	RsrcUsageSpec_NULL_DEF(invol_context_switch);
	RsrcUsageSpec_NULL_DEF(system_calls);
#elif __SVR4
	RsrcUsageSpec_NULL_DEF(messages_other);
	RsrcUsageSpec_NULL_DEF(chars_read);
	RsrcUsageSpec_NULL_DEF(chars_written);
	RsrcUsageSpec_NULL_DEF(chars_other);
	RsrcUsageSpec_NULL_DEF(working_set_size);
	RsrcUsageSpec_NULL_DEF(working_set_size_peak=);
	RsrcUsageSpec_NULL_DEF(pagefile_usage);
	RsrcUsageSpec_NULL_DEF(pagefile_usage_peak);
#else
	RsrcUsageSpec_NULL_DEF(trap_cpu_time);
	RsrcUsageSpec_NULL_DEF(text_pagef_time);
	RsrcUsageSpec_NULL_DEF(data_pagef_time);
	RsrcUsageSpec_NULL_DEF(kernel_pagef_time);
	RsrcUsageSpec_NULL_DEF(user_lock_time);
	RsrcUsageSpec_NULL_DEF(other_sleep_time);
	RsrcUsageSpec_NULL_DEF(wait_cpu_time);
	RsrcUsageSpec_NULL_DEF(stopped_time);
	RsrcUsageSpec_NULL_DEF(messages_other);
	RsrcUsageSpec_NULL_DEF(system_calls);
	RsrcUsageSpec_NULL_DEF(chars_read_written);
	RsrcUsageSpec_NULL_DEF(chars_read);
	RsrcUsageSpec_NULL_DEF(chars_written);
	RsrcUsageSpec_NULL_DEF(chars_other);
	RsrcUsageSpec_NULL_DEF(working_set_size);
	RsrcUsageSpec_NULL_DEF(working_set_size_peak=);
	RsrcUsageSpec_NULL_DEF(pagefile_usage);
	RsrcUsageSpec_NULL_DEF(pagefile_usage_peak);
#endif // #ifdef __MSDOS__
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector &RsrcUsageSpec::ToStringList(
	const RsrcUsage &rusage_datum,
	RsrcUsage::RsrcUsageEmptyFormat null_format_type, unsigned int text_width,
	const std::string &separator, MLB::Utility::StringVector &out_list) const
{
	MLB::Utility::StringVector tmp_list;

	tmp_list.reserve(26);

	RsrcUsageElementSetIterC iter_b(member_set_.begin());
	RsrcUsageElementSetIterC iter_e(member_set_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		std::ostringstream o_str;
		if (!iter_b->is_supported_flag_) {
			if (null_format_type == RsrcUsage::RsrcUsageEmptyFormatSkip)
				continue;
			o_str << std::left << std::setw(text_width) << iter_b->title_name_ <<
				std::right << separator;
			if (null_format_type == RsrcUsage::RsrcUsageEmptyFormatNull) {
				if (iter_b->is_time_flag_)
					o_str << "?????? ??:??:??.?????????";
				else
					o_str << std::setw(
						MLB::Utility::IntegralValueMaxLengthDecUnsigned<
						RsrcUsageValue>()) << "?";
			}
			else {								//	Defaults to RsrcUsageEmptyFormatZero
				if (iter_b->is_time_flag_)
					o_str << "000000 00:00:00.000000000";
				else
					o_str << std::setw(
						MLB::Utility::IntegralValueMaxLengthDecUnsigned<
						RsrcUsageValue>()) << "0";
			}
		}
		else {
			o_str << std::left << std::setw(text_width) << iter_b->title_name_ <<
				std::right << separator;
			if (iter_b->is_time_flag_)
				o_str << reinterpret_cast<const RsrcUsageTime *>(
					reinterpret_cast<const char *>(&rusage_datum) +
					iter_b->member_offset_)->ToStringInterval();
			else
				o_str << std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned<
					RsrcUsageValue>()) << *reinterpret_cast<const RsrcUsageValue *>(
					reinterpret_cast<const char *>(&rusage_datum) +
					iter_b->member_offset_);
		}
		tmp_list.push_back(o_str.str());
	}

	out_list.swap(tmp_list);

	return(out_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsageSpec PRIVATE_RsrcUsageSpec_INSTANCE;
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
RsrcUsage::RsrcUsage(bool get_now_flag) :
	 user_cpu_time(0, 0)
	,system_cpu_time(0, 0)
	,trap_cpu_time(0, 0)
	,text_pagef_time(0, 0)
	,data_pagef_time(0, 0)
	,kernel_pagef_time(0, 0)
	,user_lock_time(0, 0)
	,other_sleep_time(0, 0)
	,wait_cpu_time(0, 0)
	,stopped_time(0, 0)
	,minor_pagef(0)
	,major_pagef(0)
	,process_swaps(0)
	,input_blocks(0)
	,output_blocks(0)
	,messages_sent(0)
	,messages_received(0)
	,messages_other(0)
	,signals_received(0)
	,vol_context_switch(0)
	,invol_context_switch(0)
	,system_calls(0)
	,chars_read_written(0)
	,chars_read(0)
	,chars_written(0)
	,chars_other(0)
	,working_set_size(0)
	,working_set_size_peak(0)
	,pagefile_usage(0)
	,pagefile_usage_peak(0)
{
	if (get_now_flag)
		GetRsrcUsage();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsage::RsrcUsage(const RsrcUsage &other) :
	 user_cpu_time(other.user_cpu_time)
	,system_cpu_time(other.system_cpu_time)
	,trap_cpu_time(other.trap_cpu_time)
	,text_pagef_time(other.text_pagef_time)
	,data_pagef_time(other.data_pagef_time)
	,kernel_pagef_time(other.kernel_pagef_time)
	,user_lock_time(other.user_lock_time)
	,other_sleep_time(other.other_sleep_time)
	,wait_cpu_time(other.wait_cpu_time)
	,stopped_time(other.stopped_time)
	,minor_pagef(other.minor_pagef)
	,major_pagef(other.major_pagef)
	,process_swaps(other.process_swaps)
	,input_blocks(other.input_blocks)
	,output_blocks(other.output_blocks)
	,messages_sent(other.messages_sent)
	,messages_received(other.messages_received)
	,messages_other(other.messages_other)
	,signals_received(other.signals_received)
	,vol_context_switch(other.vol_context_switch)
	,invol_context_switch(other.invol_context_switch)
	,system_calls(other.system_calls)
	,chars_read_written(other.chars_read_written)
	,chars_read(other.chars_read)
	,chars_written(other.chars_written)
	,chars_other(other.chars_other)
	,working_set_size(other.working_set_size)
	,working_set_size_peak(other.working_set_size_peak)
	,pagefile_usage(other.pagefile_usage)
	,pagefile_usage_peak(other.pagefile_usage_peak)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsage & RsrcUsage::operator = (const RsrcUsage &other)
{
	RsrcUsage tmp_datum(other);

	swap(tmp_datum);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::swap(RsrcUsage &other)
{
	std::swap(user_cpu_time, other.user_cpu_time);
	std::swap(system_cpu_time, other.system_cpu_time);
	std::swap(trap_cpu_time, other.trap_cpu_time);
	std::swap(text_pagef_time, other.text_pagef_time);
	std::swap(data_pagef_time, other.data_pagef_time);
	std::swap(kernel_pagef_time, other.kernel_pagef_time);
	std::swap(user_lock_time, other.user_lock_time);
	std::swap(other_sleep_time, other.other_sleep_time);
	std::swap(wait_cpu_time, other.wait_cpu_time);
	std::swap(stopped_time, other.stopped_time);
	std::swap(minor_pagef, other.minor_pagef);
	std::swap(major_pagef, other.major_pagef);
	std::swap(process_swaps, other.process_swaps);
	std::swap(input_blocks, other.input_blocks);
	std::swap(output_blocks, other.output_blocks);
	std::swap(messages_sent, other.messages_sent);
	std::swap(messages_received, other.messages_received);
	std::swap(messages_other, other.messages_other);
	std::swap(signals_received, other.signals_received);
	std::swap(vol_context_switch, other.vol_context_switch);
	std::swap(invol_context_switch, other.invol_context_switch);
	std::swap(system_calls, other.system_calls);
	std::swap(chars_read_written, other.chars_read_written);
	std::swap(chars_read, other.chars_read);
	std::swap(chars_written, other.chars_written);
	std::swap(chars_other, other.chars_other);
	std::swap(working_set_size, other.working_set_size);
	std::swap(working_set_size_peak, other.working_set_size_peak);
	std::swap(pagefile_usage, other.pagefile_usage);
	std::swap(pagefile_usage_peak, other.pagefile_usage_peak);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RsrcUsage::operator < (const RsrcUsage &other) const
{
	//	CODE NOTE: Must implement a better operator < than this!!!
	return(user_cpu_time < other.user_cpu_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RsrcUsage::ToString() const
{
	std::ostringstream o_str;

	o_str
		<< "[" << user_cpu_time.ToStringInterval()     << "]"
		<< "[" << system_cpu_time.ToStringInterval()   << "]"
		<< "[" << trap_cpu_time.ToStringInterval()     << "]"
		<< "[" << text_pagef_time.ToStringInterval()   << "]"
		<< "[" << data_pagef_time.ToStringInterval()   << "]"
		<< "[" << kernel_pagef_time.ToStringInterval() << "]"
		<< "[" << user_lock_time.ToStringInterval()    << "]"
		<< "[" << other_sleep_time.ToStringInterval()  << "]"
		<< "[" << wait_cpu_time.ToStringInterval()     << "]"
		<< "[" << stopped_time.ToStringInterval()      << "]"
		<< "[" << minor_pagef                          << "]"
		<< "[" << major_pagef                          << "]"
		<< "[" << process_swaps                        << "]"
		<< "[" << input_blocks                         << "]"
		<< "[" << output_blocks                        << "]"
		<< "[" << messages_sent                        << "]"
		<< "[" << messages_received                    << "]"
		<< "[" << messages_other                       << "]"
		<< "[" << signals_received                     << "]"
		<< "[" << vol_context_switch                   << "]"
		<< "[" << invol_context_switch                 << "]"
		<< "[" << system_calls                         << "]"
		<< "[" << chars_read_written                   << "]"
		<< "[" << chars_read                           << "]"
		<< "[" << chars_written                        << "]"
		<< "[" << chars_other                          << "]"
		<< "[" << working_set_size                     << "]"
		<< "[" << working_set_size_peak                << "]"
		<< "[" << pagefile_usage                       << "]"
		<< "[" << pagefile_usage_peak                  << "]";

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RsrcUsage::ToStringLines(RsrcUsageEmptyFormat null_format_type,
	unsigned int text_width, const std::string &separator) const
{
	std::string out_string;

	return(ToStringLines(out_string, null_format_type, text_width, separator));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RsrcUsage::ToStringLines(std::string &out_string,
	RsrcUsageEmptyFormat null_format_type, unsigned int text_width,
	const std::string &separator) const
{
	MLB::Utility::StringVector tmp_list;

	return(MLB::Utility::JoinString(out_string, ToStringList(tmp_list, null_format_type,
		text_width, separator), "\n"));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector RsrcUsage::ToStringList(RsrcUsageEmptyFormat null_format_type,
	unsigned int text_width, const std::string &separator) const
{
	MLB::Utility::StringVector out_list;

	return(ToStringList(out_list, null_format_type, text_width, separator));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector &RsrcUsage::ToStringList(MLB::Utility::StringVector &out_list,
	RsrcUsageEmptyFormat null_format_type, unsigned int text_width,
	const std::string &separator) const
{
/*
	CODE NOTE: Old NULL management logic. To be removed.
	MLB::Utility::StringVector tmp_list;

	tmp_list.reserve(26);

	text_width = (!text_width) ? 28 : text_width;

	ToStringListTimes(null_format_type, text_width, tmp_list, separator);
	ToStringListValues(null_format_type, text_width, tmp_list, separator);

	out_list.swap(tmp_list);

	return(out_list);
*/
	return(PRIVATE_RsrcUsageSpec_INSTANCE.ToStringList(*this, null_format_type,
		text_width, separator, out_list));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsage RsrcUsage::GetRsrcUsageDelta() const
{
	return(GetRsrcUsageDelta(*this, RsrcUsage::GetRsrcUsageInstance()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsage RsrcUsage::GetRsrcUsageDelta(const RsrcUsage &end_rusage) const
{
	return(GetRsrcUsageDelta(*this, end_rusage));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsage RsrcUsage::GetRsrcUsageInstance(MLB::Utility::ProcessId selector)
{
	RsrcUsage tmp_datum;

	tmp_datum.GetRsrcUsage(selector);

	return(tmp_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsage RsrcUsage::GetRsrcUsageInstance()
{
	RsrcUsage tmp_datum;

	tmp_datum.GetRsrcUsage();

	return(tmp_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector RsrcUsage::ToStringList(const RsrcUsage &rusage_datum,
	RsrcUsageEmptyFormat null_format_type, unsigned int text_width)
{
	MLB::Utility::StringVector out_list;

	rusage_datum.ToStringList(out_list, null_format_type, text_width);

	return(out_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::ToStringList(MLB::Utility::StringVector out_list,
	const RsrcUsage &rusage_datum, RsrcUsageEmptyFormat null_format_type,
	unsigned int text_width)
{
	rusage_datum.ToStringList(out_list, null_format_type, text_width);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#define RsrcUsage_FIRST_TIME				user_cpu_time
#define RsrcUsage_LAST_TIME				stopped_time
#define RsrcUsage_FIRST_VALUE				minor_pagef
#define RsrcUsage_LAST_VALUE				pagefile_usage_peak

#define RsrcUsage_GET_FIRST_TIME_PTR(rusage_ptr)				\
	((RsrcUsageTime *) ((void *) (((char *) rusage_ptr) +		\
	MBCOMPAT_Offsetof(RsrcUsage, RsrcUsage_FIRST_TIME))))
#define RsrcUsage_GET_LAST_TIME_PTR(rusage_ptr)					\
	((RsrcUsageTime *) ((void *) (((char *) rusage_ptr) +		\
	MBCOMPAT_Offsetof(RsrcUsage, RsrcUsage_LAST_TIME))))
#define RsrcUsage_GET_FIRST_VALUE_PTR(rusage_ptr)				\
	((RsrcUsageValue *) ((void *) (((char *) rusage_ptr) +	\
	MBCOMPAT_Offsetof(RsrcUsage, RsrcUsage_FIRST_VALUE))))
#define RsrcUsage_GET_LAST_VALUE_PTR(rusage_ptr)				\
	((RsrcUsageValue *) ((void *) (((char *) rusage_ptr) +	\
	MBCOMPAT_Offsetof(RsrcUsage, RsrcUsage_LAST_VALUE))))
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static const RsrcUsageTime RsrcUsageTimeMax(LONG_MAX, LONG_MAX);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#if _Windows
typedef union {
	FILETIME         struct_data;
	unsigned __int64 scalar_data;
} SDTIF_WIN32_FILETIME_64;
# define SDTIF_WIN32_FILETIME_EPOCH_BIAS	116444736000000000i64
#endif // #if _Windows
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	For use with Windows...
//	////////////////////////////////////////////////////////////////////////////
#if _Windows
typedef COMPAT_FN_TYPE(BOOL (WINAPI *GEN_Win32_FPtr_GetProcessTimes),
	(HANDLE, LPFILETIME, LPFILETIME, LPFILETIME, LPFILETIME));
# if _MSC_VER >= 1300
typedef COMPAT_FN_TYPE(BOOL (WINAPI *GEN_Win32_FPtr_GetMemInfoFPtr),
	(FARPROC, PROCESS_MEMORY_COUNTERS *, DWORD));
typedef COMPAT_FN_TYPE(BOOL (WINAPI *GEN_Win32_FPtr_GetProcessIoCounters),
	(HANDLE, PIO_COUNTERS));
# endif // # if _MSC_VER >= 1300
#endif // #if _Windows
//	////////////////////////////////////////////////////////////////////////////

#if _Windows
//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::GetRsrcUsageByWindowsHandle(HANDLE selector, RsrcUsage &datum)
{
	OS_ProcessTimes         process_times;
	PROCESS_MEMORY_COUNTERS mem_counters;
	IO_COUNTERS             io_counters;
	RsrcUsage               tmp_datum;

	try {
		OS_GetProcessTimes(selector, process_times);
		OS_GetProcessMemoryInfo(selector, mem_counters);
		OS_GetProcessIoCounters(selector, io_counters);
		tmp_datum.user_cpu_time         = process_times.user_time_;
		tmp_datum.system_cpu_time       = process_times.kernel_time_;
		tmp_datum.major_pagef           = mem_counters.PageFaultCount;
		tmp_datum.working_set_size      = mem_counters.WorkingSetSize;
		tmp_datum.working_set_size_peak = mem_counters.PeakWorkingSetSize;
		tmp_datum.pagefile_usage        = mem_counters.PagefileUsage;
		tmp_datum.pagefile_usage_peak   = mem_counters.PeakPagefileUsage;
		tmp_datum.messages_sent         =
			static_cast<RsrcUsageValue>(io_counters.WriteOperationCount);
		tmp_datum.messages_received     =
			static_cast<RsrcUsageValue>(io_counters.ReadOperationCount);
		tmp_datum.messages_other        =
			static_cast<RsrcUsageValue>(io_counters.OtherOperationCount);
		tmp_datum.chars_read_written    =
			static_cast<RsrcUsageValue>(io_counters.ReadTransferCount) +
			static_cast<RsrcUsageValue>(io_counters.WriteTransferCount);
		tmp_datum.chars_read            =
			static_cast<RsrcUsageValue>(io_counters.ReadTransferCount);
		tmp_datum.chars_written         =
			static_cast<RsrcUsageValue>(io_counters.WriteTransferCount);
		tmp_datum.chars_other           =
			static_cast<RsrcUsageValue>(io_counters.OtherTransferCount);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Call to 'GetRsrcUsageByWindowsHandle()' "
			"to retrieve resource usage information for process handle " +
			MLB::Utility::ValueToStringHex(selector) + " failed: " +
			std::string(except.what()));
	}

//	tmp_datum.trap_cpu_time         = RsrcUsageTimeMax;
//	tmp_datum.text_pagef_time       = RsrcUsageTimeMax;
//	tmp_datum.data_pagef_time       = RsrcUsageTimeMax;
//	tmp_datum.kernel_pagef_time     = RsrcUsageTimeMax;
//	tmp_datum.user_lock_time        = RsrcUsageTimeMax;
//	tmp_datum.other_sleep_time      = RsrcUsageTimeMax;
//	tmp_datum.wait_cpu_time         = RsrcUsageTimeMax;
//	tmp_datum.stopped_time          = RsrcUsageTimeMax;
//	tmp_datum.minor_pagef           = RsrcUsageValueMax;
//	tmp_datum.process_swaps         = RsrcUsageValueMax;
//	tmp_datum.input_blocks          = RsrcUsageValueMax;
//	tmp_datum.output_blocks         = RsrcUsageValueMax;
//	tmp_datum.signals_received      = RsrcUsageValueMax;
//	tmp_datum.vol_context_switch    = RsrcUsageValueMax;
//	tmp_datum.invol_context_switch  = RsrcUsageValueMax;
//	tmp_datum.system_calls          = RsrcUsageValueMax;

	datum.swap(tmp_datum);
}
//	////////////////////////////////////////////////////////////////////////////
#endif // #if _Windows

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
/*	BOH

   NAME        :	GetRsrcUsage

   SYNOPSIS    :	void RsrcUsage::GEN_GetRsrcUsage(selector);

						ProcessId selector;


   DESCRIPTION :	Determines the values of a number of operating system
						dependent usage metrics.

						Under BSD systems such as SunOS, this function uses the
						''struct rusage'' structure to populate the usage metrics.

						Under System V systems such as Solaris, this function uses
						the ''struct prusage'' structure to populate the usage
						metrics.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``rusage_ptr`` points to the ''RsrcUsage'' into which
						this function is to place the usage metrics determined by
						the function.

						(.) ``error_text`` points to a string into which this
						function will place text descriptive of any error which
						might occur.

						(-) The string allocated for this purpose should be at least
						''GENFUNCS_MAX_ERROR_TEXT'' characters in length.

   RETURNS     :	Void.


   NOTES       :	Not all of the usage metrics supplied as members in
						''RsrcUsage'' are available on all operating systems.
						This function populates members which are unavailable in
						following way:

						(.) ''RsrcUsageTime'' members are set to ''RsrcUsageTime''
						(equal to having the ``tv_sec`` and ``tv_nsec`` members set
						to ''LONG_MAX'').

						(.) ''RsrcUsageValue'' members are set to ''RsrcUsageValueMax''
						(equal to ''std::numeric_limits<RsrcUsageValue>::max()'').

						These values have special meaning to the usage formatting
						method ``ToStringList`` --- and should to any application
						programmers who desire to interrogate the members directly.

   CAVEATS     :	

   SEE ALSO    :	GetRsrcUsageDelta

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

	COPYRIGHT	:	Copyright 1992 - 2015 Michael L. Brock

   OUTPUT INDEX:	GetRsrcUsage
						Miscellaneous Functions:GetRsrcUsage
						Utility:Functions:GetRsrcUsage
						Utility:Miscellaneous Functions:GetRsrcUsage
						General Functions:See Utility

   PUBLISH XREF:	GetRsrcUsage

   PUBLISH NAME:	GetRsrcUsage

	ENTRY CLASS	:	Miscellaneous Functions

EOH */
#ifdef __MSDOS__
//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::GetRsrcUsage(ProcessId selector)
{
	*this = RsrcUsage();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#elif _Windows
void RsrcUsage::GetRsrcUsage(MLB::Utility::ProcessId selector)
{
	try {
		if (selector == MLB::Utility::CurrentProcessId())
			GetRsrcUsageByWindowsHandle(GetCurrentProcess(), *this);
		else {
			boost::shared_ptr<void> selector_handle(
				ResolveProcessIdToHandle(selector), ::CloseHandle);
			GetRsrcUsageByWindowsHandle(selector_handle.get(), *this);
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to retrieve resource usage "
			"information for process id " + MLB::Utility::AnyToString(selector) +
			" failed: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#elif __SVR4
void RsrcUsage::GetRsrcUsage(ProcessId selector)
{
# include <sys/resource.h>

/*
	int             file_handle = -1;
	struct prusage  tmp_prusage;
	struct timeval *timeval_ptr;
	RsrcUsage       rusage_datum;
	char            proc_file_name[1 + 4 + 1 + 11 + 1];

	memset(&tmp_prusage, '\0', sizeof(tmp_prusage));

	sprintf(proc_file_name, "/proc/%05d", selector);

	if ((file_handle = open(proc_file_name, O_RDONLY)) == -1)
		ThrowSystemError("Unable to open process control file '" +
			proc_file_name + "' for reading.");
*/

	int             file_handle = -1;
	struct prusage  tmp_prusage;
	struct timeval *timeval_ptr;
	RsrcUsage       rusage_datum;

	memset(&tmp_prusage, '\0', sizeof(tmp_prusage));

	std::string proc_file_name("/proc/" + ZeroFill(selector % 99999, 5));

	if ((file_handle = open(proc_file_name.c_str(), O_RDONLY)) == -1)
		ThrowSystemError("Unable to open process control file '" +
			proc_file_name + "' for reading.");
	
	
	if (ioctl(file_handle, PIOCUSAGE, &tmp_prusage) == -1) {
		close(file_handle);
		ThrowSystemError("Attempt to retrieve the process usage information "
			"for pid " << selector << " failed ('ioctl(" << file_handle <<
			PIOCUSAGE << " = PICOUSAGE, " << &tmp_prusage << ")').");
	}

	close(file_handle);

	rusage_datum.user_cpu_time        = RsrcUsageTime(tmp_prusage.pr_utime);
	rusage_datum.system_cpu_time      = RsrcUsageTime(tmp_prusage.pr_stime);
	rusage_datum.trap_cpu_time        = RsrcUsageTime(tmp_prusage.pr_ttime);
	rusage_datum.text_pagef_time      = RsrcUsageTime(tmp_prusage.pr_tftime);
	rusage_datum.data_pagef_time      = RsrcUsageTime(tmp_prusage.pr_dftime);
	rusage_datum.kernel_pagef_time    = RsrcUsageTime(tmp_prusage.pr_kftime);
	rusage_datum.user_lock_time       = RsrcUsageTime(tmp_prusage.pr_ltime);
	rusage_datum.other_sleep_time     = RsrcUsageTime(tmp_prusage.pr_slptime);
	rusage_datum.wait_cpu_time        = RsrcUsageTime(tmp_prusage.pr_wtime);
	rusage_datum.stopped_time         = RsrcUsageTime(tmp_prusage.pr_stoptime);
	rusage_datum.minor_pagef          = tmp_prusage.pr_minf;
	rusage_datum.major_pagef          = tmp_prusage.pr_majf;
	rusage_datum.process_swaps        = tmp_prusage.pr_nswap;
	rusage_datum.input_blocks         = tmp_prusage.pr_inblk;
	rusage_datum.output_blocks        = tmp_prusage.pr_oublk;
	rusage_datum.messages_sent        = tmp_prusage.pr_msnd;
	rusage_datum.messages_received    = tmp_prusage.pr_mrcv;
//	rusage_datum.messages_other       = RsrcUsageValueMax;
	rusage_datum.signals_received     = tmp_prusage.pr_sigs;
	rusage_datum.vol_context_switch   = tmp_prusage.pr_vctx;
	rusage_datum.invol_context_switch = tmp_prusage.pr_ictx;
	rusage_datum.system_calls         = tmp_prusage.pr_sysc;
	rusage_datum.chars_read_written   = tmp_prusage.pr_ioch;
//	rusage_datum.chars_read           = RsrcUsageValueMax;
//	rusage_datum.chars_written        = RsrcUsageValueMax;
//	rusage_datum.chars_other          = RsrcUsageValueMax;
//	rusage_datum.working_set_size     = RsrcUsageValueMax;
//	rusage_datum.working_set_size_peak= RsrcUsageValueMax;
//	rusage_datum.pagefile_usage       = RsrcUsageValueMax;
//	rusage_datum.pagefile_usage_peak  = RsrcUsageValueMax;

	timeval_ptr                       =
		RsrcUsage_GET_FIRST_TIME_PTR(&rusage_datum);

	while (timeval_ptr <= RsrcUsage_GET_LAST_TIME_PTR(&rusage_datum)) {
		timeval_ptr->tv_usec /= 1000000L;
		timeval_ptr++;
	}

	*this = rusage_datum;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
#else
void RsrcUsage::GetRsrcUsage(ProcessId selector)
{
# include <sys/resource.h>

	RsrcUsage    rusage_datum;
	struct rusage tmp_rusage;

	memset(&tmp_rusage, '\0', sizeof(tmp_rusage));

	if (getrusage(selector, &tmp_rusage))
		ThrowSystemError("Attempt to retrieve the process usage information "
			"for " + ((selector == RUSAGE_SELF) ? "this process" :
			((selector == RUSAGE_CHILDREN) ? "this process children" :
			"*** UNKNOWN ***")) + " failed");

	rusage_datum.user_cpu_time        = RsrcUsageTime(tmp_rusage.ru_utime);
	rusage_datum.system_cpu_time      = RsrcUsageTime(tmp_rusage.ru_stime);
//	rusage_datum.trap_cpu_time        = RsrcUsageTimeMax;
//	rusage_datum.text_pagef_time      = RsrcUsageTimeMax;
//	rusage_datum.data_pagef_time      = RsrcUsageTimeMax;
//	rusage_datum.kernel_pagef_time    = RsrcUsageTimeMax;
//	rusage_datum.user_lock_time       = RsrcUsageTimeMax;
//	rusage_datum.other_sleep_time     = RsrcUsageTimeMax;
//	rusage_datum.wait_cpu_time        = RsrcUsageTimeMax;
//	rusage_datum.stopped_time         = RsrcUsageTimeMax;
	rusage_datum.minor_pagef          =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_minflt);
	rusage_datum.major_pagef          =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_majflt);
	rusage_datum.process_swaps        =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_nswap);
	rusage_datum.input_blocks         =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_inblock);
	rusage_datum.output_blocks        =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_oublock);
	rusage_datum.messages_sent        =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_msgsnd);
	rusage_datum.messages_received    =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_msgrcv);
//	rusage_datum.messages_other       = RsrcUsageValueMax;
	rusage_datum.signals_received     =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_nsignals);
	rusage_datum.vol_context_switch   =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_nvcsw);
	rusage_datum.invol_context_switch =
		static_cast<RsrcUsageValue>(tmp_rusage.ru_nivcsw);
//	rusage_datum.system_calls         = RsrcUsageValueMax;
//	rusage_datum.chars_read_written   = RsrcUsageValueMax;
//	rusage_datum.chars_read           = RsrcUsageValueMax;
//	rusage_datum.chars_written        = RsrcUsageValueMax;
//	rusage_datum.chars_other          = RsrcUsageValueMax;
//	rusage_datum.working_set_size     = RsrcUsageValueMax;
//	rusage_datum.working_set_size_peak= RsrcUsageValueMax;
//	rusage_datum.pagefile_usage       = RsrcUsageValueMax;
//	rusage_datum.pagefile_usage_peak  = RsrcUsageValueMax;
}
#endif // #ifdef __MSDOS__
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::GetRsrcUsage()
{
#ifdef __MSDOS__
	GetRsrcUsage(0);
#elif _Windows
	GetRsrcUsageByWindowsHandle(GetCurrentProcess(), *this);
#elif __SVR4
	GetRsrcUsage(CurrentProcessId());
#else
	GetRsrcUsage(static_cast<ProcessId>(RUSAGE_SELF));
#endif // #ifdef __MSDOS__
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
/*	BOH

   NAME        :	GetRsrcUsageDelta

   SYNOPSIS    :	void GetRsrcUsageDelta(start_rusage,
							end_rusage, delta_rusage);

						const RsrcUsage &start_rusage;

						const RsrcUsage &end_rusage;

						RsrcUsage        delta_rusage;

   DESCRIPTION :	Calculates the difference (or ``delta``) between two
						''RsrcUsage'' structures.

						By determining the resource usage at the beginning of a
						section of code (with ``getrusage``) and after the section
						of code finishes, this function permits applications to
						determine the operating system resources needed to complete
						a task.

   PARAMETERS  :	Parameters to this function are as follow:

						(.) ``start_rusage`` is the ''RsrcUsage'' data captured by
						the application at the start of the measurement period.

						(.) ``end_rusage`` is the ''RsrcUsage'' data captured by
						the application at the end of the measurement period.

						(-) It is permissible for ``start_rusage`` and
						``delta_rusage`` to refer to the same ''RsrcUsage''
						structure. In this case, each member of ``delta_rusage``
						will be zero.

						(.) ``delta_rusage`` is the ''RsrcUsage'' data into which
						this function will place the delta of ``start_rusage`` and
						``end_rusage``.

   RETURNS     :	Void.

   NOTES       :	

   CAVEATS     :	

   SEE ALSO    :	GetRsrcUsage

   EXAMPLES    :	

   AUTHOR      :	Michael L. Brock

	COPYRIGHT	:	Copyright 1992 - 2015 Michael L. Brock

   OUTPUT INDEX:	GetRsrcUsageDelta
						Miscellaneous Functions:GetRsrcUsageDelta
						Utility:Functions:GetRsrcUsageDelta
						Utility:Miscellaneous Functions:GetRsrcUsageDelta
						Utility Functions:See Utility

   PUBLISH XREF:	GetRsrcUsageDelta

   PUBLISH NAME:	GetRsrcUsageDelta

	ENTRY CLASS	:	Miscellaneous Functions

EOH */
//	////////////////////////////////////////////////////////////////////////////
RsrcUsage &RsrcUsage::GetRsrcUsageDelta(const RsrcUsage &start_rusage,
	const RsrcUsage &end_rusage, RsrcUsage &delta_rusage)
{
	RsrcUsage tmp_delta_rusage;

	//	Get the usage time interval deltas...
	tmp_delta_rusage.user_cpu_time        =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.user_cpu_time,
		start_rusage.user_cpu_time);
	tmp_delta_rusage.system_cpu_time      =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.system_cpu_time,
		start_rusage.system_cpu_time);
	tmp_delta_rusage.trap_cpu_time        =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.trap_cpu_time,
		start_rusage.trap_cpu_time);
	tmp_delta_rusage.text_pagef_time      =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.text_pagef_time,
		start_rusage.text_pagef_time);
	tmp_delta_rusage.data_pagef_time      =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.data_pagef_time,
		start_rusage.data_pagef_time);
	tmp_delta_rusage.kernel_pagef_time    =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.kernel_pagef_time,
		start_rusage.kernel_pagef_time);
	tmp_delta_rusage.user_lock_time       =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.user_lock_time,
		start_rusage.user_lock_time);
	tmp_delta_rusage.other_sleep_time     =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.other_sleep_time,
		start_rusage.other_sleep_time);
	tmp_delta_rusage.wait_cpu_time        =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.wait_cpu_time,
		start_rusage.wait_cpu_time);
	tmp_delta_rusage.stopped_time         =
		RsrcUsageTime::GetDifferenceAbs(end_rusage.stopped_time,
		start_rusage.stopped_time);

	//	Get the usage value deltas...
	tmp_delta_rusage.minor_pagef          =
		std::max(start_rusage.minor_pagef, end_rusage.minor_pagef) -
		std::min(start_rusage.minor_pagef, end_rusage.minor_pagef);
	tmp_delta_rusage.major_pagef          =
		std::max(start_rusage.major_pagef, end_rusage.major_pagef) -
		std::min(start_rusage.major_pagef, end_rusage.major_pagef);
	tmp_delta_rusage.process_swaps        =
		std::max(start_rusage.process_swaps, end_rusage.process_swaps) -
		std::min(start_rusage.process_swaps, end_rusage.process_swaps);
	tmp_delta_rusage.input_blocks         =
		std::max(start_rusage.input_blocks, end_rusage.input_blocks) -
		std::min(start_rusage.input_blocks, end_rusage.input_blocks);
	tmp_delta_rusage.output_blocks        =
		std::max(start_rusage.output_blocks, end_rusage.output_blocks) -
		std::min(start_rusage.output_blocks, end_rusage.output_blocks);
	tmp_delta_rusage.messages_sent        =
		std::max(start_rusage.messages_sent, end_rusage.messages_sent) -
		std::min(start_rusage.messages_sent, end_rusage.messages_sent);
	tmp_delta_rusage.messages_received    =
		std::max(start_rusage.messages_received, end_rusage.messages_received) -
		std::min(start_rusage.messages_received, end_rusage.messages_received);
	tmp_delta_rusage.messages_other       =
		std::max(start_rusage.messages_other, end_rusage.messages_other) -
		std::min(start_rusage.messages_other, end_rusage.messages_other);
	tmp_delta_rusage.signals_received     =
		std::max(start_rusage.signals_received, end_rusage.signals_received) -
		std::min(start_rusage.signals_received, end_rusage.signals_received);
	tmp_delta_rusage.vol_context_switch   =
		std::max(start_rusage.vol_context_switch, end_rusage.vol_context_switch) -
		std::min(start_rusage.vol_context_switch, end_rusage.vol_context_switch);
	tmp_delta_rusage.invol_context_switch =
		std::max(start_rusage.invol_context_switch, end_rusage.invol_context_switch) -
		std::min(start_rusage.invol_context_switch, end_rusage.invol_context_switch);
	tmp_delta_rusage.system_calls         =
		std::max(start_rusage.system_calls, end_rusage.system_calls) -
		std::min(start_rusage.system_calls, end_rusage.system_calls);
	tmp_delta_rusage.chars_read_written   =
		std::max(start_rusage.chars_read_written, end_rusage.chars_read_written) -
		std::min(start_rusage.chars_read_written, end_rusage.chars_read_written);
	tmp_delta_rusage.chars_read           =
		std::max(start_rusage.chars_read, end_rusage.chars_read) -
		std::min(start_rusage.chars_read, end_rusage.chars_read);
	tmp_delta_rusage.chars_written        =
		std::max(start_rusage.chars_written, end_rusage.chars_written) -
		std::min(start_rusage.chars_written, end_rusage.chars_written);
	tmp_delta_rusage.chars_other          =
		std::max(start_rusage.chars_other, end_rusage.chars_other) -
		std::min(start_rusage.chars_other, end_rusage.chars_other);
	tmp_delta_rusage.working_set_size     =
		std::max(start_rusage.working_set_size, end_rusage.working_set_size) -
		std::min(start_rusage.working_set_size, end_rusage.working_set_size);
	tmp_delta_rusage.working_set_size_peak=
		std::max(start_rusage.working_set_size, end_rusage.working_set_size) -
		std::min(start_rusage.working_set_size, end_rusage.working_set_size);
	tmp_delta_rusage.pagefile_usage       =
		std::max(start_rusage.working_set_size, end_rusage.working_set_size) -
		std::min(start_rusage.working_set_size, end_rusage.working_set_size);
	tmp_delta_rusage.pagefile_usage_peak  =
		std::max(start_rusage.working_set_size, end_rusage.working_set_size) -
		std::min(start_rusage.working_set_size, end_rusage.working_set_size);

	delta_rusage.swap(tmp_delta_rusage);

	return(delta_rusage);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RsrcUsage RsrcUsage::GetRsrcUsageDelta(const RsrcUsage &start_rusage,
	const RsrcUsage &end_rusage)
{
	RsrcUsage delta_rusage;

	return(GetRsrcUsageDelta(start_rusage, end_rusage, delta_rusage));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::ToStringListTimes(RsrcUsageEmptyFormat null_format_type,
	unsigned int text_width, MLB::Utility::StringVector &out_list,
	const std::string &separator) const
{
	MLB::Utility::StringVector name_list;
	RsrcUsageTimeVector        data_list;

	GetNameListTimes(name_list);
	GetListTimes(data_list);

	int count_1;
	for (count_1 = 0; count_1 < static_cast<int>(name_list.size()); ++count_1) {
		std::ostringstream o_str;
		o_str << std::left << std::setw(text_width) << name_list[count_1] <<
			std::right << separator;
		if (data_list[count_1] != RsrcUsageTimeMax) 
			o_str << data_list[count_1].ToStringInterval();
		else if (null_format_type == RsrcUsageEmptyFormatZero)
			o_str << "000000 00:00:00.000000000";
		else if (null_format_type == RsrcUsageEmptyFormatNull)
			o_str << "?????? ??:??:??.?????????";
		else if (null_format_type == RsrcUsageEmptyFormatSkip)
			continue;
		out_list.push_back(o_str.str());
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::ToStringListValues(RsrcUsageEmptyFormat null_format_type,
	unsigned int text_width, MLB::Utility::StringVector &out_list,
	const std::string &separator) const
{
	MLB::Utility::StringVector   name_list;
	RsrcUsageValueVector data_list;

	GetNameListValues(name_list);
	GetListValues(data_list);

	int count_1;
	for (count_1 = 0; count_1 < static_cast<int>(name_list.size()); ++count_1) {
		std::ostringstream o_str;
		o_str << std::left << std::setw(text_width) << name_list[count_1] <<
			std::right << separator;
		if (data_list[count_1] != RsrcUsageValueMax)
			o_str <<
				std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned<RsrcUsageValue>()) <<
				data_list[count_1];
		else if (null_format_type == RsrcUsageEmptyFormatZero)
			o_str <<
				std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned<RsrcUsageValue>()) <<
				"0";
		else if (null_format_type == RsrcUsageEmptyFormatNull)
			o_str <<
				std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned<RsrcUsageValue>()) <<
				"?";
		else if (null_format_type == RsrcUsageEmptyFormatSkip)
			continue;
		out_list.push_back(o_str.str());
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::GetNameListTimes(MLB::Utility::StringVector &out_list) const
{
	out_list = MLB::Utility::MakeInlineVector<std::string>
		("User Level CPU Time")
		("System Call CPU Time")
		("Other System Trap CPU Time")
		("Text Page Fault Sleep Time")
		("Data Page Fault Sleep Time")
		("Kernel Page Fault Sleep Time")
		("User Lock Wait Sleep Time")
		("Other Sleep Time")
		("Wait-CPU Latency Time")
		("Stopped Time");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::GetNameListValues(MLB::Utility::StringVector &out_list) const
{
	out_list = MLB::Utility::MakeInlineVector<std::string>
		("Minor Page Faults")
		("Major Page Faults")
		("Process Swaps")
		("Input Blocks")
		("Output Blocks")
		("Messages Sent")
		("Messages Received")
		("Messages Other")
		("Signals Received")
		("Voluntary Context Switches")
		("Involuntary Context Switches")
		("System Calls")
		("Characters Read and Written")
		("Characters Read")
		("Characters Written")
		("Characters Other")
		("Working Set Size")
		("Working Set Size Peak")
		("Pagefile Usage")
		("Pagefile Usage Peak");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::GetListTimes(RsrcUsageTimeVector &out_list) const
{
	out_list.clear();

	out_list.push_back(user_cpu_time);
	out_list.push_back(system_cpu_time);
	out_list.push_back(trap_cpu_time);
	out_list.push_back(text_pagef_time);
	out_list.push_back(data_pagef_time);
	out_list.push_back(kernel_pagef_time);
	out_list.push_back(user_lock_time);
	out_list.push_back(other_sleep_time);
	out_list.push_back(wait_cpu_time);
	out_list.push_back(stopped_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RsrcUsage::GetListValues(RsrcUsageValueVector &out_list) const
{
	out_list.clear();

	out_list.push_back(minor_pagef);
	out_list.push_back(major_pagef);
	out_list.push_back(process_swaps);
	out_list.push_back(input_blocks);
	out_list.push_back(output_blocks);
	out_list.push_back(messages_sent);
	out_list.push_back(messages_received);
	out_list.push_back(messages_other);
	out_list.push_back(signals_received);
	out_list.push_back(vol_context_switch);
	out_list.push_back(invol_context_switch);
	out_list.push_back(system_calls);
	out_list.push_back(chars_read_written);
	out_list.push_back(chars_read);
	out_list.push_back(chars_written);
	out_list.push_back(chars_other);
	out_list.push_back(working_set_size);
	out_list.push_back(working_set_size_peak);
	out_list.push_back(pagefile_usage);
	out_list.push_back(pagefile_usage_peak);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const RsrcUsage &datum)
{
	o_str
		<< datum.ToString();

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::OSSupport;

#ifdef _Windows
//	////////////////////////////////////////////////////////////////////////////
static void WIN32_TEST_CheckTimeRanges()
{
	SDTIF_WIN32_FILETIME_64 user_time;
	MLB::Utility::TimeSpec                user_cpu_time_1(0);
	MLB::Utility::TimeSpec                user_cpu_time_2(0);
	MLB::Utility::TimeSpec                user_cpu_time_3(0);

	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
	std::cout << "WIN32_TEST_CheckTimeRanges():" << std::endl;
	std::cout << std::setfill('-') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	user_time.scalar_data   = 25384687500i64;
	user_cpu_time_1.tv_sec  =
			((long) (user_time.scalar_data   / 10i64)) / 1000000L;
	user_cpu_time_1.tv_nsec =
			((long) (user_time.scalar_data   / 10i64)) % 1000000000L;
	user_cpu_time_2.tv_sec  =
			((long) ((user_time.scalar_data   / 10i64) / 1000000i64));
	user_cpu_time_2.tv_nsec =
			((long) ((user_time.scalar_data   / 10i64) % 1000000000i64));
	std::cout << "Problem test case: CPU time       : " <<
		user_time.scalar_data << std::endl;
	std::cout << "Old-style interval as interval    : [" <<
		user_cpu_time_1.ToStringInterval() << "]" << std::endl;
	std::cout << "New-style interval as interval    : [" <<
		user_cpu_time_2.ToStringInterval() << "]" << std::endl;
	std::cout << "New-style interval as date        : [" <<
		"[" << user_cpu_time_2 << "]" << std::endl;
	std::cout << std::setfill('-') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
	//	----------------------------------------------------------------------
	user_time.scalar_data   = std::numeric_limits<unsigned __int64>::max();
	user_cpu_time_3.tv_sec  =
			((long) ((user_time.scalar_data   / 10i64) / 1000000i64));
	user_cpu_time_3.tv_nsec =
			((long) ((user_time.scalar_data   / 10i64) % 1000000000i64));
	std::cout << "Maximum 64-bit unsigned CPU time  : " <<
		user_time.scalar_data << std::endl;
	std::cout << "Maximum value interval as interval: [" <<
		user_cpu_time_3.ToStringInterval() << "]" << std::endl;
	std::cout << "Maximum value interval as date    : [" <<
		user_cpu_time_3 << "]" << std::endl;

	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
static void TEST_DoTheTest(const RsrcUsage &rusage_datum,
	RsrcUsage::RsrcUsageEmptyFormat null_format_type)
{
	std::cout << "===========================================================" <<
		std::endl;

	std::cout << "Test NULL Format Type: " <<
		((null_format_type == RsrcUsage::RsrcUsageEmptyFormatNone) ? "NONE" :
		((null_format_type == RsrcUsage::RsrcUsageEmptyFormatZero) ? "ZERO" :
		((null_format_type == RsrcUsage::RsrcUsageEmptyFormatNull) ? "NULL" :
		"SKIP"))) << std::endl;
	std::cout << "---- ---- ------ ----  ----" << std::endl;

	MLB::Utility::StringVector tmp_list;
	rusage_datum.ToStringList(tmp_list, null_format_type);

	std::copy(tmp_list.begin(), tmp_list.end(),
		std::ostream_iterator<MLB::Utility::StringVector::value_type>(std::cout, "\n"));

	std::cout << "===========================================================" <<
		std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test routine for 'GEN_FormatRsrcUsageList()'" << std::endl;
	std::cout << "---- ------- --- ---------------------------" <<
		std::endl << std::endl;

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ <process-id> [ <process-id> ... ] ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	try {
#ifdef _Windows
		WIN32_TEST_CheckTimeRanges();
#endif // #ifdef _Windows
		RsrcUsage    rusage_datum;
		unsigned int count_1;
		MLB::Utility::ProcessId    process_id;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			MLB::Utility::ParseCmdLineArg::ParseCmdLineDatum(argv[count_1], process_id);
			rusage_datum.GetRsrcUsage(process_id);
			TEST_DoTheTest(rusage_datum, RsrcUsage::RsrcUsageEmptyFormatZero);
		}
		rusage_datum.GetRsrcUsage();
		TEST_DoTheTest(rusage_datum, RsrcUsage::RsrcUsageEmptyFormatNone);
		TEST_DoTheTest(rusage_datum, RsrcUsage::RsrcUsageEmptyFormatZero);
		TEST_DoTheTest(rusage_datum, RsrcUsage::RsrcUsageEmptyFormatNull);
		TEST_DoTheTest(rusage_datum, RsrcUsage::RsrcUsageEmptyFormatSkip);
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

