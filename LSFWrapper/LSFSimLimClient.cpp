// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Simulation LIM Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the LSFWrapper simulation LIM.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/CriticalEventHandler.hpp>
#include <RvUtilX/AddRvField.hpp>
#include <RvUtilX/GetRvField.hpp>

#include "LSFWrapper/LSFSimLim.hpp"
#include "LSFWrapper/LSF_Request.hpp"

#include <deque>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

// ////////////////////////////////////////////////////////////////////////////
class LIM_ClientBase {
public:
	LIM_ClientBase() { }
	virtual ~LIM_ClientBase() { }

	virtual MLB::RvUtilX::SNDSpec GetSNDSpec() const = 0;
	virtual std::string           GetAsynchReplySubject() const = 0;
	virtual std::string           GetMyUniqueId() const = 0;

	virtual void ProcessLimAsynchReply(TibrvMsg &msg_recv) = 0;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LIM_ClientLimCB : public TibrvMsgCallback  {
public:
	LIM_ClientLimCB(LIM_ClientBase &control_ref) :
		 control_ref_(control_ref)
		,dispatch_flag_(true)
	{ }

	void onMsg(TibrvListener *, TibrvMsg &msg_recv) {
		try {
			control_ref_.ProcessLimAsynchReply(msg_recv);
		}
		catch (const std::exception &except) {
			std::cerr << "LIM: Error on LIM message callback: " <<\
				except.what() << std::endl;
		}
	}

	void CancelDispatch() {
		dispatch_flag_ = false;
	}

private:

	MLB_Utility_NonCopyable_Macro(LIM_ClientLimCB);

	LIM_ClientBase &control_ref_;
	bool            dispatch_flag_;
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class AsynchReplyThread : public pt::thread {
public:
	AsynchReplyThread(LIM_ClientBase &control_ref, bool autofree) :
		 thread(autofree)
		,rv_context_()
		,control_ref_(control_ref)
		,snd_spec_()
		,asynch_subject_()
		,my_unique_id_()
		,my_description_()
		,lim_queue_()
		,transport_()
		,lim_listener_()
		,callback_lim_(control_ref)
	{
	}
	~AsynchReplyThread() {
	}

	void StartThread() {
		//	Get some data from the control instance...
		snd_spec_ = control_ref_.GetSNDSpec();
		asynch_subject_ = control_ref_.GetAsynchReplySubject();
		my_unique_id_   = control_ref_.GetMyUniqueId();
		my_description_ = "LSF LIM Client Simulator Asynch Handler Thread (" +
			my_unique_id_ + ")";
		// Create the RV basic transport...
		snd_spec_.CreateTransport(transport_);
		//	Name the transport...
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_.setDescription,
			(my_description_.c_str()));
		//	Create the RV queue...
		RvUtilX_THROW_TIBRV_STATUS_IF(lim_queue_.create,
			());
		//	Create the RV listener for LIM subjects...
		RvUtilX_THROW_TIBRV_STATUS_IF(lim_listener_.create,
			(&lim_queue_, &callback_lim_, &transport_,
			asynch_subject_.c_str()));

		start();
	}

	void execute() {
		try {
			std::ostringstream out_text;
			out_text << "Executing thread id " << get_id() << "." << std::endl;
			std::cout << out_text.str();


			//	Dispatch RV events...
			TibrvStatus rv_status;
			do {
				rv_status = lim_queue_.timedDispatch(0.1);
				if (MLB::Utility::CriticalEventTest()) {
					std::ostringstream log_text;
					log_text << "LSFLimSimServer is performing an asynchronous " <<
						"exit due to receipt of a signal." << std::endl;
					std::cout << out_text.str();
					break;
				}
				if (get_signaled()) {
					std::ostringstream out_text;
					out_text << "Asynchronous LIM callback thread " << get_id() <<
						" is exiting due to signaled state." << std::endl;
					std::cout << out_text.str();
					break;
				}
			} while ((rv_status == TIBRV_OK) || (rv_status == TIBRV_TIMEOUT));
			if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
				throw MLB::RvUtilX::RvExceptionStatus(rv_status,
					"Error occurred while dispatching from the default queue.");
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text << "Error on asynchrnous LIM callback thread: " <<
				except.what() << std::endl;
			std::cout << error_text.str();
		}
	}
	void cleanup() {
		std::ostringstream out_text;
		out_text << "Clean-up of thread id " << get_id() << "." << std::endl;
		std::cout << out_text.str();
	}

private:
	MLB_Utility_NonCopyable_Macro(AsynchReplyThread);

	MLB::RvUtilX::RvContext  rv_context_;
	LIM_ClientBase          &control_ref_;
	MLB::RvUtilX::SNDSpec    snd_spec_;
	std::string              asynch_subject_;
	std::string              my_unique_id_;
	std::string              my_description_;
	TibrvQueue               lim_queue_;
	TibrvNetTransport        transport_;
	TibrvListener            lim_listener_;
	LIM_ClientLimCB          callback_lim_;
};
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct WaitResult {
	WaitResult(int process_id = 0, int process_exit_code = 0) :
		 process_id_(process_id)
		,process_exit_code_(process_exit_code) { }

	int process_id_;
	int process_exit_code_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LIM_Client : LIM_ClientBase {
public:
	LIM_Client() :
		 LIM_ClientBase()
		,rv_context_()
		,snd_spec_("", ";239.255.1.45")
		,transport_()
		,my_unique_id_()
		,my_description_()
		,asynch_subject_()
		,release_subject_()
		,wait_list_()
		,wait_list_lock_()
	{
		//	Create the subject names...
		Initialize();
		// Create the RV basic transport...
		snd_spec_.CreateTransport(transport_);
		//	Name the transport...
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_.setDescription,
			((my_description_).c_str()));

		AsynchReplyThread *asynch_thread_ptr =
			new AsynchReplyThread(*this, true);
		asynch_thread_ptr->StartThread();
	}
	//	As this object was instantiated statically, we don't want to try to
	//	clean up with 'Tibrv::close()'... The Rv library takes it especially
	//	hard.
#ifdef _Windows
	//	Disable warnings about unreachable code under MSVC++
# pragma warning(disable:4702)
	~LIM_Client() {
		try {
			ReleaseRex();
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text << "ERROR IN LIM_Client::~LIM_Client(): " <<
				except.what() << std::endl;
			std::cout << error_text.str();
		}
		_exit(0);
	}
# pragma warning(default:4702)
#else
	~LIM_Client() {
		try {
			ReleaseRex();
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text << "ERROR IN LIM_Client::~LIM_Client(): " <<
				except.what() << std::endl;
			std::cout << error_text.str();
		}
		_exit(0);
	}
#endif // #ifdef _Windows

	TibrvMsg SendRequest(const TibrvMsg &msg_send, double time_out = 3.0) {
		//	Reply messages to go here...
		TibrvMsg msg_reply;
		//	Send the request and wait...
		RvUtilX_THROW_TIBRV_TIMEOUT_IF(transport_.sendRequest,
			(msg_send, msg_reply, time_out), time_out);
		//	Check to see if the command was performed by the RPM server...
		tibrv_bool commandOk;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_reply.getBool,
			("commandOk", commandOk));
		if (!commandOk) {
			const char *commandError;
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_reply.getString,
				("commandError", commandError));
			std::ostringstream error_text;
			error_text << "Server returned a command error reply (" << commandOk <<
				"): " << commandError;
			throw MLB::RvUtilX::RvException(error_text);
		}
		return(msg_reply);
	}

	void ReleaseRex() {
		TibrvMsg msg_send;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
			(release_subject_.c_str()));
		//	Send the advisory that this client is stopping REX operations...
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_.send,
			(msg_send));
	}

	MLB::RvUtilX::SNDSpec GetSNDSpec() const {
		return(snd_spec_);
	}
	std::string           GetAsynchReplySubject() const {
		return(asynch_subject_);
	}
	std::string           GetClientReleaseSubject() const {
		return(release_subject_);
	}
	std::string           GetMyUniqueId() const {
		return(my_unique_id_);
	}

	typedef std::deque<WaitResult> WaitList;

	void ProcessLimAsynchReply(TibrvMsg &msg_recv) {
		const char *msg_subject;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getSendSubject,
			(msg_subject));
		if (strcmp(msg_subject, asynch_subject_.c_str())) {
			std::ostringstream error_text;
			error_text << "Invalid subject name ecountered in asychronous " <<
				"callback ('" << msg_subject << "') --- expected '" <<
				asynch_subject_ << "'.";
			throw MLB::RvUtilX::RvException(error_text);
		}
		int processId;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getI32,
			("processId", processId));
		int processExitCode;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getI32,
			("processExitCode", processExitCode));
		pt::scopelock lock(wait_list_lock_);
		wait_list_.push_back(WaitResult(processId, processExitCode));
	}
	bool WaitListHasItems() const {
		pt::scopelock lock(wait_list_lock_);
		return(!wait_list_.empty());
	}
	bool WaitListPopTid(int tid, WaitResult &wait_result) {
		pt::scopelock lock(wait_list_lock_);
		WaitList::iterator iter_b(wait_list_.begin());
		WaitList::iterator iter_e(wait_list_.end());
		while (iter_b != iter_e) {
			if (iter_b->process_id_ == tid) {
				wait_result = *iter_b;
				wait_list_.erase(iter_b);
				return(true);
			}
			++iter_b;
		}
		return(false);
	}
	bool PopWaitList(WaitResult &wait_result) {
		pt::scopelock lock(wait_list_lock_);
		if (wait_list_.empty())
			return(false);
		wait_result = wait_list_.front();
		wait_list_.pop_front();
		return(true);
	}

	MLB::RvUtilX::RvContext rv_context_;
	MLB::RvUtilX::SNDSpec   snd_spec_;
	TibrvNetTransport       transport_;
	std::string             my_unique_id_;
	std::string             my_description_;
	std::string             asynch_subject_;
	std::string             release_subject_;
	WaitList                wait_list_;
	mutable pt::mutex       wait_list_lock_;

private:
	LIM_Client &GetThisRef() {
		return(*this);
	}
	void Initialize() {
		my_unique_id_    = MLB::Utility::UniqueId().ToString();
		my_description_  = "LSF LIM Client Simulator (" + my_unique_id_ + ")";
		asynch_subject_  = std::string(SubjectNameBase_LimReply) + "." +
			my_unique_id_;
		release_subject_ = std::string(SubjectNameBase_ClientExit) + "." +
			my_unique_id_;
	}
	static std::string GetHostName() {
		std::ostringstream tmp_string;
		tmp_string << "LIM_" << MLB::Utility::CurrentProcessId();
		return(tmp_string.str());
	}

	MLB_Utility_NonCopyable_Macro(LIM_Client);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

//	////////////////////////////////////////////////////////////////////////////
static MLB::LSFWrapper::LIM_Client ClientInstance;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static bool LSF_InitRexDone = false;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int lserrno = 0;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static char LSF_ErrorText[1023 + 1];
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static MLB::Utility::C_ListTypeString<unsigned int> SAVED_ls_placereq;
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
char  *ls_sperror (char *usrMsg)
{
	std::ostringstream error_text;

	error_text << ((usrMsg != NULL) ? usrMsg : "*NULL*") << ": error code = " <<
		lserrno << ": " << ls_sysmsg();

	return(MLB::Utility::nstrcpy(LSF_ErrorText, error_text.str().c_str(),
		sizeof(LSF_ErrorText) - 1));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *ls_sysmsg()
{
	return("*SOME-ERROR-TEXT*");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *ls_getclustername()
{
	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Necessary to imitate the semantics of the Platform LSF API...
	//	//////////////////////////////////////////////////////////////////////
	static char *SAVED_ls_getclustername = NULL;
	if (SAVED_ls_getclustername != NULL) {
		free(SAVED_ls_getclustername);
		SAVED_ls_getclustername = NULL;
	}
	//	//////////////////////////////////////////////////////////////////////

	std::string send_subject(MLB::LSFWrapper::SubjectNameBase_MLim);
	send_subject += ".ls_getclustername";

	TibrvMsg msg_send;
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
		(send_subject.c_str()));

	TibrvMsg msg_reply = ClientInstance.SendRequest(msg_send);

	const char *TMP_ls_cluster_name;
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_reply.getString,
		("replyText", TMP_ls_cluster_name));

	return(SAVED_ls_getclustername = strdup(TMP_ls_cluster_name));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char **ls_placereq(char *resreq, int *num, int options, char *fromhost)
{
/*
	int desired_count = (*num == NULL) ? 1 : ((*num) ? *num :
		std::numeric_limits<int>::max());

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Necessary to imitate the semantics of the Platform LSF API...
	//	//////////////////////////////////////////////////////////////////////
	static MLB::Utility::C_ListTypeString<unsigned int> SAVED_ls_placereq;
	SAVED_ls_placereq.Free();
	//	//////////////////////////////////////////////////////////////////////

	std::string send_subject(MLB::LSFWrapper::SubjectNameBase_MLim);
	send_subject += ".ls_placereq";

	TibrvMsg msg_send;
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
		(send_subject.c_str()));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateU32,
		("numberOfHosts", desired_count));

	TibrvMsg msg_reply = ClientInstance.SendRequest(msg_send);

	MLB::Utility::C_ListTypeString<unsigned int> tmp_list;
	MLB::RvUtilX::GetRvFieldStringList(msg_reply, "replyList", tmp_list);

	SAVED_ls_placereq.SetCount(tmp_list.GetCount());
	SAVED_ls_placereq.SetList(tmp_list.GetList());
	tmp_list.Reset();

//	SAVED_ls_placereq = tmp_list;
	
	if (num != NULL)
		*num = SAVED_ls_placereq.GetCount();

	return(SAVED_ls_placereq.GetList());
*/
///*
	int desired_count = (num == NULL) ? 1 : ((*num) ? *num :
		std::numeric_limits<int>::max());

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Necessary to imitate the semantics of the Platform LSF API...
	//	//////////////////////////////////////////////////////////////////////
	SAVED_ls_placereq.Free();
	//	//////////////////////////////////////////////////////////////////////

	std::string send_subject(MLB::LSFWrapper::SubjectNameBase_MLim);
	send_subject += ".ls_placereq";

	TibrvMsg msg_send;
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
		(send_subject.c_str()));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
		("resourceRequirements", ((resreq != NULL) ? resreq : "")));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateU32,
		("numberOfHosts", desired_count));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateI32,
		("options", options));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
		("fromHost", ((fromhost != NULL) ? fromhost : "")));

	TibrvMsg msg_reply = ClientInstance.SendRequest(msg_send);

	MLB::Utility::C_ListTypeString<unsigned int> out_list;
	MLB::RvUtilX::GetRvFieldStringList(msg_reply, "replyList", out_list);

	if (num != NULL)
		*num = static_cast<int>(out_list.GetCount());

	char **result = out_list.GetList();
	out_list.Reset();

	return(result);
//*/
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_initrex(int numports, int /* PARAM_NOT_USED options */)
{
	if (LSF_InitRexDone)
		MLB::Utility::ThrowInvalidArgument("Second call to 'ls_initrex()'.");

	LSF_InitRexDone = true;

	return(numports);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_donerex()
{
	Check_InitRexDone();

	LSF_InitRexDone = false;

	ClientInstance.ReleaseRex();

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_stoprex()
{
	Check_InitRexDone();

	LSF_InitRexDone = false;

	ClientInstance.ReleaseRex();

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_rtask(char *host, char **argv, int options)
{
	Check_InitRexDone();

	std::string send_subject(MLB::LSFWrapper::SubjectNameBase_Lim);
	send_subject += "." + std::string(host) + ".ls_rtask";

	TibrvMsg msg_send;
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
		(send_subject.c_str()));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
		("hostName", host));
	MLB::RvUtilX::AddRvFieldStringList(msg_send, "argvList", argv);
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateI32,
		("options", options));

	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
		("asynchSubject", ClientInstance.asynch_subject_.c_str()));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
		("releaseSubject", ClientInstance.release_subject_.c_str()));

	TibrvMsg msg_reply = ClientInstance.SendRequest(msg_send);

	int result;
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_reply.getI32,
		("processId", result));

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_rtaske(char *host, char **argv, int options, char **envp)
{
	Check_InitRexDone();

	std::string send_subject(MLB::LSFWrapper::SubjectNameBase_Lim);
	send_subject += "." + std::string(host) + ".ls_rtask";

	TibrvMsg msg_send;
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
		(send_subject.c_str()));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
		("hostName", host));
	MLB::RvUtilX::AddRvFieldStringList(msg_send, "argvList", argv);
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateI32,
		("options", options));
	if (envp != NULL)
		MLB::RvUtilX::AddRvFieldStringList(msg_send, "envpList", envp);

	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
		("asynchSubject", ClientInstance.asynch_subject_.c_str()));
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
		("releaseSubject", ClientInstance.release_subject_.c_str()));

	TibrvMsg msg_reply = ClientInstance.SendRequest(msg_send);

	int result;
	RvUtilX_THROW_TIBRV_STATUS_IF(msg_reply.getI32,
		("processId", result));

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_rwait(LS_WAIT_T *status, int options, struct rusage *ru)
{
	Check_InitRexDone();

//	if (!(options & WNOHANG)) {
	if (!(options & 0x0001)) {
		while (!ClientInstance.WaitListHasItems())
			Sleep(100);
	}

	MLB::LSFWrapper::WaitResult wait_result;
	if (!ClientInstance.PopWaitList(wait_result))
		return(0);
	*status = wait_result.process_exit_code_;
	if (ru != NULL)
		memset(ru, '\0', sizeof(*ru));
	return(wait_result.process_id_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int ls_rwaittid(int tid, LS_WAIT_T *status, int options, struct rusage *ru)
{
	Check_InitRexDone();

	MLB::LSFWrapper::WaitResult wait_result;

//	if (!(options & WNOHANG)) {
	if (!(options & 0x0001)) {
		while (!ClientInstance.WaitListPopTid(tid, wait_result))
			Sleep(100);
		return(true);
	}

	if (!ClientInstance.WaitListPopTid(tid, wait_result))
		return(0);
	*status = wait_result.process_exit_code_;
	if (ru != NULL)
		memset(ru, '\0', sizeof(*ru));
	return(wait_result.process_id_);
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
	std::cout << "TEST: ls_rtask() and ls_rwait()" << std::endl;
	std::cout << "----- ---------- --- ----------" << std::endl;

	//	ls_initrex() already called by the main() test driver.

	int    place_req_count = 0;
	char **place_req_list  = ls_placereq(NULL, &place_req_count, 0, NULL);

	if (place_req_list == NULL)
		throw std::exception("ls_placereq() failed.");

//	char *argv_list[2] = { "echo", NULL };
//	char *argv_list[2] = { "C:\\Utility\\Other\\hname.exe", NULL };
	char *argv_list[3] = { "C:\\WINNT\\system32\\ping.exe", "localhost", NULL };
	int   r_tid        = ls_rtask(place_req_list[0], argv_list, 0);

	std::cout << "ls_rtask(): " << r_tid << std::endl;
	LS_WAIT_T status;
	ls_rwait(&status, 0, NULL);
	std::cout << "ls_rwait(): " << r_tid << " = " << status << std::endl;

	DoSeparator();
	std::cout << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class LSF_Request_MyPlaceReq :
	public MLB::LSFWrapper::LSF_Request_PlaceReq {
public:
	LSF_Request_MyPlaceReq(
		std::deque<MLB::LSFWrapper::LSF_Request_Base *> &request_queue,
		const std::string &resource_requirements = "",
		unsigned int number_of_hosts = 0, int place_req_options = 0,
		const std::string &from_host_name = "") :
		 LSF_Request_PlaceReq(resource_requirements, number_of_hosts,
			place_req_options, from_host_name)
		,request_queue_(request_queue) { }

protected:
	virtual void CallLSF_Internal() {
		//	Use the parent to perform the call to LSF...
		LSF_Request_PlaceReq::CallLSF_Internal();
		//	Create the argument list for 'ls_rtask()'...
		MLB::LSFWrapper::ArgvList argv_list;
		argv_list.push_back("C:\\WINNT\\system32\\ping.exe");
		argv_list.push_back("localhost");
		MLB::LSFWrapper::HostList::const_iterator iter_b(result_data_.host_name_list_.begin());
		MLB::LSFWrapper::HostList::const_iterator iter_e(result_data_.host_name_list_.end());
		while (iter_b != iter_e) {
			request_queue_.push_back(new MLB::LSFWrapper::LSF_Request_RTask(*iter_b,
				argv_list));
			++iter_b;
		}
	}

private:
	std::deque<MLB::LSFWrapper::LSF_Request_Base *> &request_queue_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_RequestQueueTest()
{
	DoSeparator();
	std::cout << "TEST: Request Queue Test" << std::endl;
	std::cout << "----- ------- ----- ----" << std::endl;

	typedef std::deque<MLB::LSFWrapper::LSF_Request_Base *> RequestQueue;

	RequestQueue src_queue;
	RequestQueue dst_queue;

	src_queue.push_back(new MLB::LSFWrapper::LSF_Request_InitRex(1));
	src_queue.push_back(new MLB::LSFWrapper::LSF_Request_GetClusterName());
	src_queue.push_back(new MLB::LSFWrapper::LSF_Request_PlaceReq());
	//	Specialization of LSF_Request_PlaceReq which enqueue a pointer to
	//	an instance of LSF_Request_RTask for each host returned by LSF
	//	in response to ls_placereq()...	
	src_queue.push_back(new LSF_Request_MyPlaceReq(src_queue));

	while (!src_queue.empty()) {
		MLB::LSFWrapper::LSF_Request_Base *ptr = src_queue.front();
		ptr->CallLSF();
		std::cout << "Result OK   = " << ptr->IsOK() << std::endl;
		if (ptr->IsComplete())
			std::cout << "Result Data = " << ptr->GetResult() << std::endl;
		else if (!ptr->IsOK())
			std::cout << "Error       = " << ptr->GetErrorText() << std::endl;
		dst_queue.push_back(src_queue.front());
		src_queue.pop_front();
	}

/*
	char *argv_list[3] = { "C:\\WINNT\\system32\\ping.exe", "localhost", NULL };

	// PlaceReq is the last...
	std::deque<std::string>::const_iterator
		iter_b(src_queue.back()->host_name_list_.begin());
	std::deque<std::string>::const_iterator
		iter_e(src_queue.back()->host_name_list_.end());
	while (iter_b != iter_e) {
		src_queue.push_back(new MLB::LSFWrapper::LSF_Request_RTask(*iter_b,
			argv_list));
		++iter_b;
	}

	while (!src_queue.empty()) {
		src_queue.front()->CallLSF();
		std::cout << "RTask Result = " << src_queue.front()->IsOK() << std::endl;
		dst_queue.push_back(src_queue.front());
		src_queue.pop_front();
	}
*/
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test harness for MLB LSF LIM Client Simulator" << std::endl;
	std::cout << "---- ------- --- --- --- --- ------ ---------" << std::endl;

	try {
TEST_RequestQueueTest();
/*
		TEST_ls_getclustername();
		TEST_ls_initrex();
		TEST_ls_placereq();
		TEST_ls_rtask();
*/
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

