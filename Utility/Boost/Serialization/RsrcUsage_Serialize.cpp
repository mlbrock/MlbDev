//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of Boost serialization for the class
								RsrcUsage.

	Revision History	:	2006-12-22 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/RsrcUsage.hpp>
#include <Utility/Boost/Serialization/BoostSerialization.ipp>

//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Use macros to get declarations and forced instantiations done...
// ////////////////////////////////////////////////////////////////////////////

	//	Declare the template functions...
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::RsrcUsage)

	//	Force instantiation of the serialize free function...
MLB_BOOSTX_SERIAL_DECL_Serialize(MLB::Utility::RsrcUsage)

	// Force instantiation of the Serialize{Load|Save}{Binary|Text|Xml} funcs.
MLB_BOOSTX_SerializeLoadSave(MLB::Utility::RsrcUsage)

// ////////////////////////////////////////////////////////////////////////////

namespace boost {

namespace serialization {

// ////////////////////////////////////////////////////////////////////////////
template <typename SArchive>
	void serialize(SArchive &s_archive, MLB::Utility::RsrcUsage &datum,
	const unsigned int)
{
	s_archive
		& boost::serialization::make_nvp("userLevelCpuTime",
			datum.user_cpu_time)
		& boost::serialization::make_nvp("systemCallCpuTime",
			datum.system_cpu_time)
		& boost::serialization::make_nvp("otherSystemTrapCpuTime",
			datum.trap_cpu_time)
		& boost::serialization::make_nvp("textPageFaultSleepTime",
			datum.text_pagef_time)
		& boost::serialization::make_nvp("dataPageFaultSleepTime",
			datum.data_pagef_time)
		& boost::serialization::make_nvp("kernelPageFaultSleepTime",
			datum.kernel_pagef_time)
		& boost::serialization::make_nvp("userLockWaitSleepTime",
			datum.user_lock_time)
		& boost::serialization::make_nvp("otherSleepTime",
			datum.other_sleep_time)
		& boost::serialization::make_nvp("waitCpuLatencyTime",
			datum.wait_cpu_time)
		& boost::serialization::make_nvp("stoppedTime",
			datum.stopped_time)
		& boost::serialization::make_nvp("minorPageFaults",
			datum.minor_pagef)
		& boost::serialization::make_nvp("majorPageFaults",
			datum.major_pagef)
		& boost::serialization::make_nvp("processSwaps",
			datum.process_swaps)
		& boost::serialization::make_nvp("inputBlocks",
			datum.input_blocks)
		& boost::serialization::make_nvp("outputBlocks",
			datum.output_blocks)
		& boost::serialization::make_nvp("messagesSent",
			datum.messages_sent)
		& boost::serialization::make_nvp("messagesReceived",
			datum.messages_received)
		& boost::serialization::make_nvp("messagesOther",
			datum.messages_other)
		& boost::serialization::make_nvp("signalsReceived",
			datum.signals_received)
		& boost::serialization::make_nvp("voluntaryContextSwitches",
			datum.vol_context_switch)
		& boost::serialization::make_nvp("involuntaryContextSwitches",
			datum.invol_context_switch)
		& boost::serialization::make_nvp("systemCalls",
			datum.system_calls)
		& boost::serialization::make_nvp("charactersReadAndWritten",
			datum.chars_read_written)
		& boost::serialization::make_nvp("charactersRead",
			datum.chars_read)
		& boost::serialization::make_nvp("charactersWritten",
			datum.chars_written)
		& boost::serialization::make_nvp("charactersOther",
			datum.chars_other)
		& boost::serialization::make_nvp("workingSetSize",
			datum.working_set_size)
		& boost::serialization::make_nvp("workingSetSizePeak",
			datum.working_set_size_peak)
		& boost::serialization::make_nvp("pagefileUsage",
			datum.pagefile_usage)
		& boost::serialization::make_nvp("pagefileUsagePeak",
			datum.pagefile_usage_peak);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace serialization

} // namespace boost

#ifdef TEST_MAIN

using namespace MLB::Utility;

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout <<
		"Test routine for Boost Serialization of class 'RsrcUsage'" << std::endl;
	std::cout <<
		"---- ------- --- ----- ------------- -- ----- -----------" << std::endl;

	try {
		RsrcUsage test_datum;
		TEST_Serialization(test_datum, "RsrcUsage");
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl;
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

