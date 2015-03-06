// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the RsrcUsage class.

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


#ifndef HH__MLB__Utility__RsrcUsage_hpp__HH

#define HH__MLB__Utility__RsrcUsage_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file RsrcUsage.hpp

	\brief	The RsrcUsage header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/TimeSupport.hpp>

#include <vector>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
typedef TimeSpec           RsrcUsageTime;
typedef unsigned long long RsrcUsageValue;
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
struct API_UTILITY RsrcUsage {
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

	void GetRsrcUsage(ProcessId selector);
	void GetRsrcUsage();

	std::string   ToString() const;
	std::string   ToStringLines(
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28, const std::string &separator = ": ") const;
	std::string &ToStringLines(std::string &out_string,
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28, const std::string &separator = ": ") const;

	std::vector<std::string>  ToStringList(
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28, const std::string &separator = ": ") const;
	std::vector<std::string> &ToStringList(std::vector<std::string> &out_list,
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28, const std::string &separator = ": ") const;

	RsrcUsage GetRsrcUsageDelta() const;
	RsrcUsage GetRsrcUsageDelta(const RsrcUsage &end_rusage) const;

	static RsrcUsage   GetRsrcUsageInstance(ProcessId selector);
	static RsrcUsage   GetRsrcUsageInstance();
	static std::vector<std::string> ToStringList(const RsrcUsage &rusage_datum,
		RsrcUsageEmptyFormat null_format_type = RsrcUsageEmptyFormatZero,
		unsigned int text_width = 28);
	static void ToStringList(std::vector<std::string> out_list,
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
		unsigned int text_width, std::vector<std::string> &out_list,
		const std::string &separator) const;
	void ToStringListValues(RsrcUsageEmptyFormat null_format_type,
		unsigned int text_width, std::vector<std::string> &out_list,
		const std::string &separator) const;

	void GetNameListTimes(std::vector<std::string> &out_list) const;
	void GetNameListValues(std::vector<std::string> &out_list) const;
	void GetListTimes(RsrcUsageTimeVector &out_list) const;
	void GetListValues(RsrcUsageValueVector &out_list) const;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const RsrcUsage &datum);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__RsrcUsage_hpp__HH

