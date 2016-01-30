// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the LSFWrapper utility library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
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

#include "LSFWrapper/LSFWrapper.hpp"

#ifdef _Windows
# pragma warning(disable:4018 4100 4146 4244 4290 4511 4512 4663)
# include <set>
# include <vector>
# pragma warning(default:4018 4100 4146 4244 4290 4511 4512 4663)
#else
# include <set>
# include <vector>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int lserrno = LSE_NO_ERR;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
static char LSF_ClusterName[] = "LSFCluster";
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct hostInfo LSF_HostInfoList[] = {
	{ "MyHostName",  "MyHostName", "MyHostModel", 1.0, 1, 100, 100, 100, 4, 0, NULL, "", 0, NULL, 'Y', 'Y', 0, 0 },
	{ "MyWindows01", "NTX86",      "PENT200",     6.0, 1, 100, 100, 100, 4, 0, NULL, "", 0, NULL, 'Y', 'Y', 0, 0 },
	{ "MySolaris01", "SUNSOL",     "SunSparc",    6.0, 1, 100, 100, 100, 4, 0, NULL, "", 0, NULL, 'Y', 'Y', 0, 0 },
};
int             LSF_HostInfoCount  =
	sizeof(LSF_HostInfoList) / sizeof(LSF_HostInfoList[0]);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static bool LSF_InitRexDone = false;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static MLB::Utility::C_ListTypeString<int> SAVED_ls_placereq;
//	////////////////////////////////////////////////////////////////////////////

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
static const struct hostInfo *LSF_ApiSim_FindHostInfo(const char *host_name)
{
	int count_1;

	for (count_1 = 0; count_1 < LSF_HostInfoCount; ++count_1) {
		if (!strcmp(LSF_HostInfoList[count_1].hostName, host_name))
			return(LSF_HostInfoList + count_1);
	}

	return(NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void Check_InitRexDone()
{
	if (!LSF_InitRexDone)
		MLB::Utility::ThrowInvalidArgument(
			"No call to 'ls_initrex()' has been made.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *ls_sperror(char *usrMsg)
{
	static char ls_sperror_SavedData[1023 + 1];

	std::ostringstream error_text;
	error_text << usrMsg << ": error code = " << lserrno << ": " << ls_sysmsg();

	return(strcpy(ls_sperror_SavedData, error_text.str().c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *ls_sysmsg()
{
	return("*WHATEVER*");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *ls_getclustername()
{
	return(LSF_ClusterName);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	NOTE: The 'resreq' and 'options' parameters are ignored by this function.
struct hostInfo *ls_gethostinfo(char * /* PARAM_NOT_USED resreq */,
	int *numhosts, char **hostlist, int listsize, int /* PARAM_NOT_USED options */)
{
	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Necessary to imitate the semantics of the Platform LSF API...
	//	//////////////////////////////////////////////////////////////////////
	static unsigned int     ls_gethostinfo_SavedDataCount = 0;
	static struct hostInfo *ls_gethostinfo_SavedDataList  = NULL;
	if (ls_gethostinfo_SavedDataList) {
		unsigned int count_1;
		for (count_1 = 0; count_1 < ls_gethostinfo_SavedDataCount; ++count_1)
			LSF_HostInfo::FreeNative(ls_gethostinfo_SavedDataList[count_1]);
		free(ls_gethostinfo_SavedDataList);
		ls_gethostinfo_SavedDataCount = 0;
		ls_gethostinfo_SavedDataList  = NULL;
	}
	//	//////////////////////////////////////////////////////////////////////

	/*
		My API Simulator doesn't use these parameters yet...
	options;
	reseq;
	*/

	if (listsize < 0) {
		lserrno = LSE_BAD_ARGS;
		return(NULL);
	}

	std::vector<struct hostInfo> out_list;

	std::set<const char *> host_set;
	int count_1;

	for (count_1 = 0; count_1 < listsize; ++count_1)
		host_set.insert(hostlist[count_1]);

	*numhosts = 0;

	for (count_1 = 0; count_1 < LSF_HostInfoCount; ++count_1) {
		if ((!listsize) ||
			(host_set.find((LSF_HostInfoList[count_1].hostName)) != host_set.end())) {
			out_list.push_back(LSF_HostInfo(LSF_HostInfoList[count_1]).ToNative());
			++(*numhosts);
		}
	}

	ls_gethostinfo_SavedDataCount = static_cast<unsigned int>(*numhosts);
	ls_gethostinfo_SavedDataList  = static_cast<struct hostInfo *>(
		MLB::Utility::C_memdup(&out_list[0], *numhosts * sizeof(struct hostInfo)));

	return(ls_gethostinfo_SavedDataList);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *ls_gethosttype(char *host_name)
{
	const struct hostInfo *host_info_ptr = LSF_ApiSim_FindHostInfo(host_name);

	return((host_info_ptr != NULL) ? host_info_ptr->hostType : NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *ls_gethostmodel(char *host_name)
{
	const struct hostInfo *host_info_ptr = LSF_ApiSim_FindHostInfo(host_name);

	return((host_info_ptr != NULL) ? host_info_ptr->hostModel : NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
float *ls_gethostfactor(char *host_name)
{
	const struct hostInfo *host_info_ptr = LSF_ApiSim_FindHostInfo(host_name);

	return((host_info_ptr != NULL) ?
		const_cast<float *>(&host_info_ptr->cpuFactor) : NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char **ls_placereq(char * /* PARAM_NOT_USED resreq */, int *num,
	int /* PARAM_NOT_USED options */, char * /* PARAM_NOT_USED fromhost */)
{
	int desired_count = (num == NULL) ? 1 : ((*num) ? *num :
		std::numeric_limits<int>::max());

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Necessary to imitate the semantics of the Platform LSF API...
	//	//////////////////////////////////////////////////////////////////////
	SAVED_ls_placereq.Free();
	//	//////////////////////////////////////////////////////////////////////

	int count_1;
	for (count_1 = 0; (count_1 < LSF_HostInfoCount) && (count_1 < desired_count);
		++count_1) {
		SAVED_ls_placereq.Append(LSF_HostInfoList[count_1].hostName);
	}

	if (*num != NULL)
		*num = SAVED_ls_placereq.GetCount();

	return(SAVED_ls_placereq.GetList());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_initrex(int numhosts, int options)
{
	if (LSF_InitRexDone)
		MLB::Utility::ThrowInvalidArgument("Second call to 'ls_initrex(" +
			MLB::Utility::AnyToString(options) + ")'.");

	LSF_InitRexDone = true;

	return(numhosts);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_donerex()
{
	Check_InitRexDone();

	LSF_InitRexDone = false;

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_stoprex()
{
	Check_InitRexDone();

	LSF_InitRexDone = false;

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// CODE NOTE: FIX ME!!!
int ls_rtask(char * /* PARAM_NOT_USED host */,
	char ** /* PARAM_NOT_USED argv */, int /* PARAM_NOT_USED options */)
{
	Check_InitRexDone();

	return(rand());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// CODE NOTE: FIX ME!!!
int ls_rtaske(char * /* PARAM_NOT_USED host */,
	char ** /* PARAM_NOT_USED argv */, int /* PARAM_NOT_USED options */,
	char ** /* PARAM_NOT_USED envp */)
{
	Check_InitRexDone();

	return(rand());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// CODE NOTE: FIX ME!!!
int ls_rwait(LS_WAIT_T * /* PARAM_NOT_USED status */,
	int /* PARAM_NOT_USED options */, struct rusage * /* PARAM_NOT_USED ru */)
{
	return(rand());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// CODE NOTE: FIX ME!!!
int ls_rwaittid(int /* PARAM_NOT_USED tid */,
	LS_WAIT_T * /* PARAM_NOT_USED status */, int /* PARAM_NOT_USED options */,
	struct rusage * /* PARAM_NOT_USED ru */)
{
	return(rand());
}
//	////////////////////////////////////////////////////////////////////////////

#include <iomanip>

#ifdef TEST_MAIN

//	////////////////////////////////////////////////////////////////////////////
static void DoSeparator()
{
	std::cout << std::setfill('=') << std::setw(79) << "" << std::setfill(' ') <<
		std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_ls_getclustername()
{
	DoSeparator();
	std::cout << "TEST_ls_getclustername()" << std::endl;
	std::cout << "------------------------" << std::endl;

	const char *cluster_name = ls_getclustername();

	if (cluster_name == NULL)
		throw std::exception("ls_getclustername() failed.");

	DoSeparator();
	std::cout << "Platform/LSF Cluster Name: " << cluster_name << std::endl;
	DoSeparator();
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_ls_initrex()
{
	DoSeparator();
	std::cout << "TEST_ls_initrex()" << std::endl;
	std::cout << "------------------" << std::endl;

	std::cout << "ls_initrex(1, 0): " << ls_initrex(1, 0) << std::endl;

	DoSeparator();
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_ls_placereq()
{
	DoSeparator();
	std::cout << "TEST_ls_placereq()" << std::endl;
	std::cout << "------------------" << std::endl;

	int    place_req_count = 0;
	char **place_req_list  = ls_placereq(NULL, &place_req_count, 0, NULL);

	if (place_req_list == NULL)
		throw std::exception("ls_placereq() failed.");

	int count_1;
	for (count_1 = 0; count_1 < place_req_count; count_1++)
		std::cout << std::setw(10) << count_1 << ": " <<
			place_req_list[count_1] << std::endl;

	DoSeparator();
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_ls_rtask()
{
	DoSeparator();
	std::cout << "TEST_ls_rtask()" << std::endl;
	std::cout << "---------------" << std::endl;

	//	ls_initrex() already called by the main() test driver.

	int    place_req_count = 0;
	char **place_req_list  = ls_placereq(NULL, &place_req_count, 0, NULL);

	if (place_req_list == NULL)
		throw std::exception("ls_placereq() failed.");

	char *argv_list[2] = { "echo", NULL };
	int   r_tid        = ls_rtask(place_req_list[0], argv_list, 0);

	std::cout << "ls_rtask(): " << r_tid << std::endl;

	DoSeparator();
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_ls_gethostinfo()
{
	DoSeparator();
	std::cout << "TEST_ls_gethostinfo()" << std::endl;
	std::cout << "---------------------" << std::endl;

	int              host_info_count = 0;
	struct hostInfo *host_info_list  = NULL;
	host_info_list = ls_gethostinfo(NULL, &host_info_count, NULL, 0, 0);

	if (host_info_list == NULL)
		throw std::exception("ls_gethostinfo() failed.");

	DoSeparator();
	std::cout <<
		std::setw(15) << "Host Type" << " " <<
		std::setw(15) << "Host Model" << " " <<
		std::setw(31) << "Host Name" << std::endl;
	std::cout << std::setfill('-') <<
		std::setw(15) << "" << " " <<
		std::setw(15) << "" << " " <<
		std::setw(31) << "" << " " << std::setfill(' ') << std::endl;

	int count_1;
	for (count_1 = 0; count_1 < host_info_count; ++count_1)
		std::cout <<
			std::setw(15) << host_info_list[count_1].hostType << " " <<
			std::setw(15) << host_info_list[count_1].hostModel << " " <<
			host_info_list[count_1].hostName << " " << std::endl;

	std::cout << std::setfill('-') <<
		std::setw(15) << "" << " " <<
		std::setw(15) << "" << " " <<
		std::setw(31) << "" << " " << std::setfill(' ') << std::endl;

	DoSeparator();
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test harness for MLB Platform/LSF API Simulator" << std::endl;
	std::cout << "---- ------- --- --- ------------ --- ---------" << std::endl;

	try {
		TEST_ls_getclustername();
		TEST_ls_initrex();
		TEST_ls_placereq();
		TEST_ls_rtask();
		TEST_ls_gethostinfo();
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

