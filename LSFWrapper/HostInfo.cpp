// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LSF_HostInfo class.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>

#include <LSFWrapper/LSFWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

//	////////////////////////////////////////////////////////////////////////////
struct hostInfo LSF_HostInfo::ToNative() const
{
	struct hostInfo datum;

	memset(&datum, '\0', sizeof(datum));

	try {
		memcpy(datum.hostName, hostName.c_str(),
			std::min<size_t>(sizeof(datum.hostName) - 1, hostName.size()));
		datum.hostType          = MLB::Utility::C_strdup(hostType.c_str(),
											"Allocation of hostInfo.hostType");
		datum.hostModel         = MLB::Utility::C_strdup(hostModel.c_str(),
											"Allocation of hostInfo.hostModel");
		datum.cpuFactor         = cpuFactor;
		datum.maxCpus           = maxCpus;
		datum.maxMem            = maxMem;
		datum.maxSwap           = maxSwap;
		datum.maxTmp            = maxTmp; 
		datum.nDisks            = nDisks;
		datum.nRes              = static_cast<int>(resources.size());
		if (datum.nRes) {
			datum.resources      = static_cast<char **>(
											MLB::Utility::C_calloc(resources.size(),
											sizeof(char *),
											"Allocation of hostInfo.resources"));
			unsigned int count_1;
			for (count_1 = 0; count_1 < resources.size(); ++count_1)
				datum.resources[count_1] =
					MLB::Utility::C_strdup(resources[count_1].c_str(),
					"Allocation of a hostInfo.resources element");
		}
		datum.windows           = MLB::Utility::C_strdup(windows.c_str(),
											"Allocation of hostInfo.windows");
		datum.numIndx           = static_cast<int>(busyThreshold.size());
		if (datum.numIndx) {
			datum.busyThreshold  = static_cast<float *>(
											MLB::Utility::C_calloc(busyThreshold.size(),
											sizeof(datum.busyThreshold[0]),
											"Allocation of hostInfo.busyThreshold"));
			unsigned int count_1;
			for (count_1 = 0; count_1 < busyThreshold.size(); ++count_1)
				datum.busyThreshold[count_1] = busyThreshold[count_1];
		}
		datum.isServer          = isServer;
		datum.licensed          = licensed;
		datum.rexPriority       = rexPriority;
		datum.licFeaturesNeeded = licFeaturesNeeded;
	}
	catch (const std::exception &except) {
		std::ostringstream error_text;
		error_text << "Unable to convert an 'LSFWrapper::LSF_HostInfo' " <<
			"instance into a Platform/LSF 'hostInfo' structure: " << except.what();
		FreeNative(datum);
		MLB::Utility::Rethrow(except, error_text);
	}

	return(datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LSF_HostInfo::FreeNative(struct hostInfo &datum)
{
	if (datum.hostType != NULL)
		free(datum.hostType);

	if (datum.hostModel != NULL)
		free(datum.hostModel);

	if ((datum.nRes > 0) && (datum.resources != NULL)) {
		int count_1;
		for (count_1 = 0; count_1 < datum.nRes; ++count_1) {
			if (datum.resources[count_1] != NULL)
				free(datum.resources[count_1]);
		}
	}

	if (datum.windows != NULL)
		free(datum.windows);


	if ((datum.numIndx > 0) && (datum.busyThreshold != NULL))
		free(datum.busyThreshold);

	memset(&datum, '\0', sizeof(datum));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#include <iomanip>

#ifdef TEST_MAIN

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
static void TEST_LSF_HostInfo()
{
	LSF_HostInfoVector host_info_list = GetHostInfo();

	LSF_HostInfoVector::const_iterator iter_b(host_info_list.begin());
	LSF_HostInfoVector::const_iterator iter_e(host_info_list.end());

	while (iter_b != iter_e) {
		std::cout << *iter_b << std::endl;
		++iter_b;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test of function 'LSF_HostInfo()'" << std::endl;
	std::cout << "---- -- -------- ----------------" << std::endl;

	try {
		TEST_LSF_HostInfo();
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

