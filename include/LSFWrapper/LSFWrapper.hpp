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


#ifndef HH__MLB__LSFWrapper_hpp__HH

#define HH__MLB__LSFWrapper_hpp__HH	1

#define MLB__LIB__LSFWrapper

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/ExecProcess.hpp>
#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/C_StringSupport.hpp>

#ifdef MLB__LIB__LSFWrapper
# ifdef LSFWRAPPER_DLL_EXPORTS
#  define API_LSFWRAPPER		MB_LIB_EXPORT
# else
#  define API_LSFWRAPPER		MB_LIB_IMPORT
# endif // #ifdef LSFWRAPPER_DLL_EXPORTS
#else
#  define API_LSFWRAPPER
#endif // #ifdef MLB__LIB__LSFWrapper

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_LSFWRAPPER

#ifdef _Windows
	//	Necessary because otherwise 'lsf.h' will 'typedef long time_t;'
# ifndef _TIME_T
# define _TIME_T
# endif // # ifndef _TIME_T
# pragma warning(disable:4668)
# include <lsf/lsf.h>
# pragma warning(default:4668)
#else
# include <lsf/lsf.h>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

// ////////////////////////////////////////////////////////////////////////////
typedef MLB::Utility::ArgvList     ArgvList;
typedef MLB::Utility::EnvpList     EnvpList;
typedef MLB::Utility::StringVector HostList;
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_LSFWRAPPER) LSFException :
	public MLB::Utility::ExceptionGeneral {
public:
	LSFException() throw() :
		ExceptionGeneral("Unspecified Platform/LSF exception.") { }
	LSFException(const char *except_string) throw() :
		ExceptionGeneral(except_string) { }
	LSFException(const std::string &except_string) throw() :
		ExceptionGeneral(except_string) { }
	LSFException(const std::ostringstream &except_string) throw() :
		ExceptionGeneral(except_string) { }
	~LSFException() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		LSFException tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_LSFWRAPPER) LSFExceptionStatus :
	public LSFException {
public:
	//	Default constructor.
	LSFExceptionStatus() throw() :
		 LSFException(GetStatusString())
		,lsf_error_code_(lserrno) { }
	//	String-only constructors...
	LSFExceptionStatus(const char *except_string) throw() :
		 LSFException(GetStatusString(lserrno, except_string))
		,lsf_error_code_(lserrno) { }
	LSFExceptionStatus(const std::string &except_string) throw() :
		 LSFException(GetStatusString(lserrno, except_string.c_str()))
		,lsf_error_code_(lserrno) { }
	LSFExceptionStatus(const std::ostringstream &except_string) throw() :
		 LSFException(GetStatusString(lserrno, except_string.str().c_str()))
		,lsf_error_code_(lserrno) { }
	//	Constructors with an error code (always the global lserrno)...
	LSFExceptionStatus(int lsf_error_code) throw() :
		 LSFException(GetStatusString(lsf_error_code))
		,lsf_error_code_(lsf_error_code) { }
	LSFExceptionStatus(int lsf_error_code,
		const char *except_string, const char *user_msg_string = NULL) throw() :
		 LSFException(GetStatusString(lsf_error_code, except_string,
			user_msg_string))
		,lsf_error_code_(lsf_error_code) { }
	LSFExceptionStatus(int lsf_error_code,
		const std::string &except_string,
			const char *user_msg_string = NULL) throw() :
		 LSFException(GetStatusString(lsf_error_code, except_string.c_str(),
			user_msg_string))
		,lsf_error_code_(lsf_error_code) { }
	LSFExceptionStatus(int lsf_error_code,
		const std::ostringstream &except_string,
			const char *user_msg_string = NULL) throw() :
		 LSFException(GetStatusString(lsf_error_code, except_string.str().c_str(),
			user_msg_string))
		,lsf_error_code_(lsf_error_code) { }

	~LSFExceptionStatus() throw() { }

	int GetStatus() const {
		return(lsf_error_code_);
	}

	virtual void Rethrow(const char *except_string = NULL) const {
		LSFExceptionStatus tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}

	static std::string GetStatusString(int lsf_error_code = lserrno,
		const char *other_text = NULL, const char *user_msg_string = NULL) {
		const char         *user_msg_string_ptr;
		std::ostringstream  status_string;
		user_msg_string_ptr = MLB::Utility::NullOrEmptyToString(user_msg_string,
			"*UNSPECIFIED ERROR SOURCE*");
		status_string << "Platform/LSF exception resulted from error " <<
			"code = " << lsf_error_code << ", current error text = '" <<
			ls_sperror(const_cast<char *>(user_msg_string_ptr)) <<
			((other_text != NULL) ? "': " : "") <<
			((other_text != NULL) ? other_text : "");
		return(status_string.str());
	}

protected:
	int lsf_error_code_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_LSFWRAPPER void ThrowLSFStatus(int error_code,
	const char *error_text)
{
	throw LSFExceptionStatus(error_code, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_LSFWRAPPER void ThrowLSFStatus(int error_code,
	const std::string &error_text)
{
	throw LSFExceptionStatus(error_code, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_LSFWRAPPER void ThrowLSFStatus(int error_code,
	const std::ostringstream &error_text)
{
	throw LSFExceptionStatus(error_code, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_LSFWRAPPER void ThrowLSFStatus(const char *error_text)
{
	ThrowLSFStatus(lserrno, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_LSFWRAPPER void ThrowLSFStatus(const std::string &error_text)
{
	ThrowLSFStatus(lserrno, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_LSFWRAPPER void ThrowLSFStatus(const std::ostringstream &error_text)
{
	ThrowLSFStatus(lserrno, error_text);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Should be invoked as:
//		LSFWrapper_THROW_IF_NONZERO_ANON(ls_unlockhost, ());
#define LSFWrapper_THROW_IF_NONZERO_ANON(method_name, method_args)		\
	{																							\
		int INTERNAL_result_value;														\
		INTERNAL_result_value = method_name method_args ;						\
		if (INTERNAL_result_value != 0) {											\
			std::ostringstream INTERNAL_error_text;								\
			INTERNAL_error_text << "Invocation of '" << #method_name <<		\
				"()' failed with a non-zero return code (" <<					\
				INTERNAL_result_value << ").";										\
			throw MLB::LSFWrapper::LSFExceptionStatus(lserrno,					\
				INTERNAL_error_text, #method_name);									\
		}																						\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Throws an exception of type 'MLB::LSFWrapper::LSFExceptionStatus' if the
//	return from the specified function is negative.
//
// Should be invoked as:
//
//		int r_tid;
//		LSFWrapper_THROW_IF_NEGATIVE(r_tid, ls_rtask, (host, argv, options));
//
//	If you're not interested in the value of the return, use the related macro
//	'LSFWrapper_THROW_IF_NEGATIVE_ANON()' instead.
#define LSFWrapper_THROW_IF_NEGATIVE(result_name, method_name, method_args)	\
	{																									\
		result_name = method_name method_args ;											\
		if (result_name < 0) {																	\
			std::ostringstream INTERNAL_error_text;										\
			INTERNAL_error_text << "Invocation of '" << #method_name <<				\
				"()' failed with a negative return code (" << result_name <<		\
				").";																					\
			throw MLB::LSFWrapper::LSFExceptionStatus(lserrno,							\
				INTERNAL_error_text, #method_name);											\
		}																								\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Should be invoked as:
//		int r_tid;
//		LSFWrapper_THROW_IF_NEGATIVE_ANON(ls_rtask, (host, argv, options));
#define LSFWrapper_THROW_IF_NEGATIVE_ANON(method_name, method_args)			\
	{																								\
		const void *INTERNAL_result_name;												\
		LSFWrapper_THROW_IF_NEGATIVE(INTERNAL_result_name, method_name,		\
			method_args);																		\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Throws an exception of type 'MLB::LSFWrapper::LSFExceptionStatus' if the
//	return from the specified function is 'NULL'.
//
// Should be invoked as:
//
//		char *clustername_result;
//		LSFWrapper_THROW_IF_NULL(clustername_result, ls_getclustername, ());
//
//	If you're not interested in the value of the return, use the related macro
//	'LSFWrapper_THROW_IF_NULL_ANON()' instead.
#define LSFWrapper_THROW_IF_NULL(result_name, method_name, method_args)	\
	{																							\
		result_name = method_name method_args ;									\
		if (result_name == NULL) {														\
			std::ostringstream INTERNAL_error_text;								\
			INTERNAL_error_text << "Invocation of '" << #method_name <<		\
				"()' failed";																\
			throw MLB::LSFWrapper::LSFExceptionStatus(lserrno,					\
				INTERNAL_error_text, #method_name);									\
		}																						\
	}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
// Should be invoked as:
//		char *clustername_result;
//		LSFWrapper_THROW_IF_NULL_ANON(ls_getclustername, ());
#define LSFWrapper_THROW_IF_NULL_ANON(method_name, method_args)			\
	{																							\
		const void *INTERNAL_result_name;											\
		LSFWrapper_THROW_IF_NULL(INTERNAL_result_name, method_name,			\
			method_args);																	\
	}
//	////////////////////////////////////////////////////////////////////////////

#ifdef _Windows
# pragma warning(disable:4251)
#endif // #ifdef _Windows
//	////////////////////////////////////////////////////////////////////////////
//	C++ analogue of the LSF 'hostInfo' structure...
struct API_LSFWRAPPER LSF_HostInfo {
	LSF_HostInfo() :
		 hostName()
		,hostType()
		,hostModel()
		,cpuFactor()
		,maxCpus()
		,maxMem()
		,maxSwap()
		,maxTmp()
		,nDisks()
		,nRes()
		,resources()
		,windows()
		,numIndx()
		,busyThreshold()
		,isServer()
		,licensed()
		,rexPriority()
		,licFeaturesNeeded()
	{
	}
	LSF_HostInfo(const struct hostInfo &datum) :
		 hostName(datum.hostName)
		,hostType(MLB::Utility::NullToString(datum.hostType))
		,hostModel(MLB::Utility::NullToString(datum.hostModel))
		,cpuFactor(datum.cpuFactor)
		,maxCpus(datum.maxCpus)
		,maxMem(datum.maxMem)
		,maxSwap(datum.maxSwap)
		,maxTmp(datum.maxTmp)
		,nDisks(datum.nDisks)
		,nRes(datum.nRes)
		,resources()
		,windows(MLB::Utility::NullToString(datum.windows))	//	CODE NOTE: Check semantics of member.
		,numIndx(datum.numIndx)
		,busyThreshold()
		,isServer(datum.isServer)
		,licensed(datum.licensed)
		,rexPriority(datum.rexPriority)
		,licFeaturesNeeded(datum.licFeaturesNeeded)
	{
		MLB::Utility::CheckCongruenceCountAndList(datum.nRes, datum.resources,
			"Platform/LSF 'hostInfo' structure resource list (members 'nRes' and \
'resources')");
		if (datum.nRes > 0) {
			int count_1;
			for (count_1 = 0; count_1 < nRes; ++count_1)
				resources.push_back(datum.resources[count_1]);
		}
		MLB::Utility::CheckCongruenceCountAndList(datum.numIndx,
			datum.busyThreshold, "Platform/LSF 'hostInfo' structure busy \
threshold list (members 'numIndx' and 'busyThreshold')");
		if (datum.numIndx > 0) {
			int count_1;
			for (count_1 = 0; count_1 < numIndx; ++count_1)
				busyThreshold.push_back(datum.busyThreshold[count_1]);
		}
	}

	struct hostInfo ToNative() const;

	std::string ToString() const {
		return("*WHATEVER*");
	}

	bool operator < (const LSF_HostInfo &other) const {
		return(strcmp(hostName.c_str(), other.hostName.c_str()) < 0);
	}

	static void FreeNative(struct hostInfo &datum);

	std::string             hostName;
	std::string             hostType;
	std::string             hostModel;
	float                   cpuFactor;
	int                     maxCpus;
	int                     maxMem;
	int                     maxSwap;
	int                     maxTmp;
	int                     nDisks;
	int                     nRes;
	std::deque<std::string> resources;
	std::string             windows;		
	int                     numIndx;
	std::deque<float>       busyThreshold;
	char                    isServer;
	char                    licensed;
	int                     rexPriority;
	int                     licFeaturesNeeded;
};
inline API_LSFWRAPPER std::ostream & operator << (std::ostream &o_str,
	const LSF_HostInfo &datum)
{
	o_str << datum.ToString();

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
# pragma warning(default:4251)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
typedef std::vector<LSF_HostInfo> LSF_HostInfoVector;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_LSFWRAPPER LSF_RWaitResult {
	LSF_RWaitResult(int process_id = 0, LS_WAIT_T process_status = 0) :
		 process_id_(process_id)
		,process_status_(process_status) {
		memset(&rusage_data_, '\0', sizeof(rusage_data_));
	}

	int           process_id_;
	LS_WAIT_T     process_status_;
	struct rusage rusage_data_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Solaris distribution has octal 0100 ( = decimal 64), but 1 seems to work
//	under Windows.
//
//	IMPLEMENTATION NOTE: This may have to conditionally compiled for platforms
//	besides Windows.
const int LSF_WNOHANG = 0x01;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Level 1 LSF API functions. These call the underlying Platform-provided
//	functions (which we consider to be level 0).
//	////////////////////////////////////////////////////////////////////////////
API_LSFWRAPPER int               LSF_donerex();
API_LSFWRAPPER char             *LSF_getclustername();
API_LSFWRAPPER float             LSF_gethostfactor(const char *host_name);
API_LSFWRAPPER struct hostInfo  *LSF_gethostinfo(const char *resreq,
	unsigned int *numhosts, char **hostlist, unsigned int listsize, int options);
API_LSFWRAPPER char             *LSF_gethostmodel(const char *host_name);
API_LSFWRAPPER char             *LSF_gethosttype(const char *host_name);
API_LSFWRAPPER int               LSF_initrex(unsigned int numports,
	int options);
API_LSFWRAPPER int               LSF_lockhost(
	time_t lock_seconds = static_cast<time_t>(0),
	bool ignore_applicability_flag = true);
API_LSFWRAPPER char            **LSF_placereq(const char *resreq,
	unsigned int *num, int options, const char *fromhost);
API_LSFWRAPPER int               LSF_rtask(const char *host, char **argv,
	int options);
API_LSFWRAPPER int               LSF_rtaske(const char *host, char **argv,
	int options, char **envp);
API_LSFWRAPPER int               LSF_rwait(LS_WAIT_T *status, int options = 0,
	struct rusage *ru = NULL, bool task_not_required = false);
API_LSFWRAPPER int               LSF_rwaittid(int tid, LS_WAIT_T *status,
	int options, struct rusage *ru);
API_LSFWRAPPER int               LSF_stoprex();
API_LSFWRAPPER int               LSF_unlockhost(
	bool ignore_applicability_flag = true);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Level 2 LSF API functions. These provide a C++ interface.
//	////////////////////////////////////////////////////////////////////////////
API_LSFWRAPPER int         DoneRex();
API_LSFWRAPPER std::string GetClusterName();
API_LSFWRAPPER float       GetHostFactor(const std::string &host_name);
API_LSFWRAPPER void        GetHostInfo(LSF_HostInfoVector &out_list,
	int host_info_options = 0, const std::string &resource_requirements = "",
	const HostList &candidate_host_list = HostList());
API_LSFWRAPPER LSF_HostInfoVector GetHostInfo(int host_info_options = 0,
	const std::string &resource_requirements = "",
	const HostList &candidate_host_list = HostList());
API_LSFWRAPPER std::string GetHostModel(const std::string &host_name);
API_LSFWRAPPER std::string GetHostType(const std::string &host_name);
API_LSFWRAPPER int         InitRex(unsigned int number_of_ports,
	int init_rex_options = 0);
API_LSFWRAPPER void        LockHost(
	time_t lock_seconds = static_cast<time_t>(0), 
	bool ignore_applicability_flag = true);
API_LSFWRAPPER void        PlaceReq(HostList &out_list,
	const std::string &resource_requirements = "",
	unsigned int number_of_hosts = 0, int place_req_options = 0,
	const std::string &from_host_name = "");
API_LSFWRAPPER HostList    PlaceReq(
	const std::string &resource_requirements = "",
	unsigned int number_of_hosts = 0, int place_req_options = 0,
	const std::string &from_host_name = "");
API_LSFWRAPPER int         RTask(const std::string &host_name,
	const ArgvList &argument_list, int rtask_options = 0);
API_LSFWRAPPER int         RTask(const std::string &host_name,
	const std::string &argument_0, int rtask_options = 0);
API_LSFWRAPPER int         RTaskE(const std::string &host_name,
	const ArgvList &argument_list, int rtask_options = 0,
	const EnvpList &environment_list = EnvpList());
API_LSFWRAPPER int         RTaskE(const std::string &host_name,
	const std::string &argument_0, int rtask_options = 0,
	const EnvpList &environment_list = EnvpList());
API_LSFWRAPPER bool        RWait(LSF_RWaitResult &wait_result,
	int rwait_options = 0, bool task_not_required = false);
API_LSFWRAPPER bool        RWaitTid(int rtask_id, LSF_RWaitResult &wait_result,
	int rwait_options);
API_LSFWRAPPER int         StopRex();
API_LSFWRAPPER void        UnlockHost(bool ignore_applicability_flag = true);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Provides automatic invocation of 'InitRex()' and 'DoneRex()'...
//	////////////////////////////////////////////////////////////////////////////
struct API_LSFWRAPPER RexInitDoneContext {
	RexInitDoneContext(unsigned int number_of_ports = 1,
		int init_rex_options = 0) {
		InitRex(number_of_ports, init_rex_options);
	}
	virtual ~RexInitDoneContext() {
		try { DoneRex(); } catch (...) { }
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Provides automatic invocation of 'InitRex()' and 'StopRex()'...
//	////////////////////////////////////////////////////////////////////////////
struct API_LSFWRAPPER RexInitStopWrapper {
	RexInitStopWrapper(unsigned int number_of_ports = 1,
		int init_rex_options = 0) {
		InitRex(number_of_ports, init_rex_options);
	}
	virtual ~RexInitStopWrapper() {
		try { StopRex(); } catch (...) { }
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_LSFWRAPPER LSFParseCmdLineArg :
	public MLB::Utility::ParseCmdLineArg {
	static bool ParseLSFResourceSpec(unsigned int &current_index,
		int argc, char **argv, std::string &out_datum) {
		return(ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-RESOURCE_SPECIFICATIONS")
				("-RESOURCESPECIFICATIONS")
				("-RESOURCE_SPECIFICATION")
				("-RESOURCESPECIFICATION")
				("-RESOURCE_SPECS")
				("-RESOURCESPECS")
				("-RESOURCE_SPEC")
				("-RESOURCESPEC")
				("-R_SPECS")
				("-RSPECS")
				("-R_SPEC")
				("-RSPEC"),
			current_index, argc, argv, out_datum));
	}
	static bool ParseLSFResourceSpecList(unsigned int &current_index,
		int argc, char **argv, MLB::Utility::StringVector &out_datum) {
		std::string tmp_string;
		if (!ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-RESOURCE_SPECIFICATIONS")
				("-RESOURCESPECIFICATIONS")
				("-RESOURCE_SPECIFICATION")
				("-RESOURCESPECIFICATION")
				("-RESOURCE_SPECS")
				("-RESOURCESPECS")
				("-RESOURCE_SPEC")
				("-RESOURCESPEC")
				("-R_SPECS")
				("-RSPECS")
				("-R_SPEC")
				("-RSPEC"),
			current_index, argc, argv, tmp_string))
			return(false);
		MLB::Utility::StringVector tmp_list;
		MLB::Utility::SplitString(tmp_string, ",", tmp_list, 0, true);
		MLB::Utility::StringSet    tmp_set(tmp_list.begin(), tmp_list.end());
		MLB::Utility::StringVector tmp_datum(tmp_set.begin(), tmp_set.end());
		out_datum.swap(tmp_datum);
		return(true);
	}
	static bool ParseCmdLineRTaskOptions(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum) {
		return(ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-RTASK_OPTIONS")
				("-RTASK_OPTION")
				("-RTASK_OPT")
				("-RTASKE_OPTIONS")
				("-RTASKE_OPTION")
				("-RTASKE_OPT"),
			current_index, argc, argv, out_datum));
	}
	static bool ParseCmdLineInitRexPorts(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum) {
		std::string tmp_string;
		if (!ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-INIT_REX_PORTS")
				("-INIT_REXPORTS")
				("-INITREX_PORTS")
				("-INITREXPORTS")
				("-INIT_REX_PORT")
				("-INIT_REXPORT")
				("-INITREX_PORT")
				("-INITREXPORT")
				("-LSF_REX_PORTS")
				("-LSF_REXPORTS")
				("-LSFREX_PORTS")
				("-LSFREXPORTS")
				("-LSF_REX_PORT")
				("-LSF_REXPORT")
				("-LSFREX_PORT")
				("-LSFREXPORT")
				("-REX_PORTS")
				("-REXPORTS")
				("-REX_PORT")
				("-REXPORT"),
			current_index, argc, argv, tmp_string))
			return(false);
			return(ParseCmdLineNumeric<unsigned int>(tmp_string, out_datum, 1));
	}
	static bool ParseCmdLineDoneRex(unsigned int &current_index, int argc,
		char **argv, bool &out_datum) {
		return(ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-LSF_DONE_REX")
				("-LSF_DONEREX")
				("-LSFDONE_REX")
				("-LSFDONEREX")
				("-DONE_REX")
				("-DONEREX"),
			current_index, argc, argv, out_datum));
	}
	static bool ParseCmdLineStopRex(unsigned int &current_index, int argc,
		char **argv, bool &out_datum) {
		return(ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-LSF_STOP_REX")
				("-LSF_STOPREX")
				("-LSFSTOP_REX")
				("-LSFSTOPREX")
				("-STOP_REX")
				("-STOPREX"),
			current_index, argc, argv, out_datum));
	}
	static bool ParseCmdLineNodeAcquisitionSleep(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum) {
		return(ParseCmdLineNumeric<unsigned int>("-NODE_ACQUISITION_SLEEP",
			current_index, argc, argv, out_datum, 0) ||
			ParseCmdLineNumeric<unsigned int>("-NODE_ACQ_SLEEP", current_index,
			argc, argv, out_datum, 0));
	}
	static bool ParseCmdLinePermitDuplicateNodes(unsigned int &current_index,
		int argc, char **argv, bool &out_datum) {
		return(ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-PERMIT_DUPLICATE_NODES")
				("-PERMIT_DUPLICATENODES")
				("-PERMITDUPLICATE_NODES")
				("-PERMITDUPLICATENODES")
				("-PERMIT_DUPLICATE_NODE")
				("-PERMIT_DUPLICATENODE")
				("-PERMITDUPLICATE_NODE")
				("-PERMITDUPLICATENODE")
				("-PERMIT_DUP_NODES")
				("-PERMIT_DUPNODES")
				("-PERMITDUP_NODES")
				("-PERMITDUPNODES")
				("-PERMIT_DUP_NODE")
				("-PERMIT_DUPNODE")
				("-PERMITDUP_NODE")
				("-PERMITDUPNODE")
				("-DUPLICATE_NODES")
				("-DUPLICATENODES")
				("-DUPLICATE_NODE")
				("-DUPLICATENODE")
				("-DUP_NODES")
				("-DUPNODES")
				("-DUP_NODE")
				("-DUPNODE"),
			current_index, argc, argv, out_datum));
	}
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__LSFWrapper_hpp__HH

