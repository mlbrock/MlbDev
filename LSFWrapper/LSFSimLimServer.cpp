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

#include <RvUtilX/AddRvField.hpp>
#include <Utility/Sleep.hpp>
#include <Utility/UniqueId.hpp>
#include <Utility/CriticalEventHandler.hpp>

#include "LSFWrapper/LSFSimLim.hpp"

#ifdef _Windows
# pragma warning(disable:4018 4100 4146 4244 4290 4511 4512 4663)
# include <process.h>
# include <map>
# include <set>
# include <vector>
# pragma warning(default:4018 4100 4146 4244 4290 4511 4512 4663)
#else
# include <map>
# include <set>
# include <vector>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

/*
//	////////////////////////////////////////////////////////////////////////////
class RVBadSubject : public RvException {
public:
	//	Default ctor.
	RVBadSubject() throw() :
		 RvException("Invalid/unsuitable TibCo/Rendezvous subject name.")
		,subject_name_("Unspecified  TibCo/Rendezvous subject name.") { }

	//	Ctors with descriptions...
	RVBadSubject(const char *except_string) throw() :
		RvException(except_string)
		,subject_name_("Unspecified  TibCo/Rendezvous subject name.") { }
	RVBadSubject(const std::string &except_string) throw() :
		RvException(except_string.c_str())
		,subject_name_("Unspecified  TibCo/Rendezvous subject name.") { }
	RVBadSubject(const std::ostringstream &except_string) throw() :
		RvException(except_string.str().c_str())
		,subject_name_("Unspecified  TibCo/Rendezvous subject name.") { }

	~RVBadSubject() throw() { }
};
//	////////////////////////////////////////////////////////////////////////////
*/

} // namespace RvUtilX

} // namespace MLB

#include <RvUtilX/GetRvField.hpp>

#include <iomanip>

namespace MLB {

namespace LSFWrapper {

//	////////////////////////////////////////////////////////////////////////////
std::string GetLIMHostName(unsigned int lim_number)
{
	std::ostringstream host_name;

	host_name << MLB::Utility::GetHostName() << "_" << std::setfill('0') <<
		std::setw(4) << lim_number << std::setfill(' ');

	return(host_name.str());
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LIM_CmdThreadCB : public TibrvMsgCallback  {
public:
	LIM_CmdThreadCB() { }

	void onMsg(TibrvListener *listener, TibrvMsg &) {
		listener->destroy();
	}
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class CmdThread : public pt::thread {
public:
	CmdThread(TibrvListener *listener, TibrvMsg &msg_recv, bool autofree) :
		 thread(autofree)
		,rv_context_()
		,snd_spec_(*MLB::RvUtilX::GetTransport(listener))
		,reply_subject_()
		,asynch_subject_()
		,release_subject_()
		,my_description_()
		,argv_list_()
		,envp_list_()
		,envp_flag_(false)
		,lim_queue_()
		,transport_()
		,lim_listener_()
		,callback_lim_() {
		//	Get the reply subject name...
		const char *tmp_ptr;
		msg_recv.getReplySubject(tmp_ptr);
		if (tmp_ptr == NULL) {
			std::ostringstream error_text;
			error_text << "Reply subject is 'NULL'.";
			MLB::Utility::ThrowStdException(error_text);
		}
		reply_subject_ = tmp_ptr;
		//	Get the task completion asynchronous callback subject name...
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getString,
			("asynchSubject", tmp_ptr));
		asynch_subject_ = tmp_ptr;
		//	Get the client release subject name...
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getString,
			("releaseSubject", tmp_ptr));
		release_subject_ = tmp_ptr;
my_description_ = "ls_rtask[e]() processing for " + asynch_subject_;
		// Create the RV transport for the initial and the asynch replies...
		snd_spec_.CreateTransport(transport_);
		//	Create the RV queue...
		RvUtilX_THROW_TIBRV_STATUS_IF(lim_queue_.create,
			());
		//	Set the description. Will change when the thread is started...
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_.setDescription,
			((my_description_).c_str()));
		MLB::RvUtilX::GetRvFieldStringList(msg_recv, "argvList", argv_list_);
		try {
			MLB::RvUtilX::GetRvFieldStringList(msg_recv, "envpList", envp_list_);
			envp_flag_ = true;
		}
		catch (...) {
		}
	}
	~CmdThread() {
		if (transport_.isValid())
			transport_.destroy();
	}

	void execute() {
		MLB::Utility::ExecProcessInfo exec_data;
		TibrvMsg                      msg_send;
		try {
			//	Create the RV listener for LIM subjects...
			RvUtilX_THROW_TIBRV_STATUS_IF(lim_listener_.create,
				(&lim_queue_, &callback_lim_, &transport_,
				release_subject_.c_str()));
			std::ostringstream out_text;
			out_text << "Executing thread id " << get_id() << "." << std::endl;
			std::cout << out_text.str();
			exec_data = (!envp_flag_) ? MLB::Utility::ExecProcess(argv_list_) :
				MLB::Utility::ExecProcess(argv_list_, envp_list_);
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text << "Attempt to start process failed: " << except.what();
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
				(reply_subject_.c_str()));
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateBool,
				("commandOk", TIBRV_FALSE));
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
				("commandError", error_text.str().c_str()));
			//	Send the synchronous reply...
			RvUtilX_THROW_TIBRV_STATUS_IF(transport_.send,
				(msg_send));
			error_text << std::endl;
			std::cout << error_text.str();
			return;
		}
		try {
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateI32,
				("processId", exec_data.process_id_));
			//	Set the synchronous reply subject...
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
				(reply_subject_.c_str()));
			//	Send the synchronous reply...
			SendReply(msg_send);

			//	Dispatch RV events...
			TibrvStatus                   rv_status;
			MLB::Utility::WaitProcessInfo wait_data;
			do {
				rv_status = lim_queue_.timedDispatch(0.1);
				if (MLB::Utility::CriticalEventTest()) {
					std::ostringstream log_text;
					log_text << "LSFLimSimServer is performing an asynchronous " <<
						"exit due to receipt of a signal." << std::endl;
					std::cout << log_text.str();
					break;
				}
				if (get_signaled()) {
					std::ostringstream out_text;
					out_text << "Asynchronous LIM callback thread " << get_id() <<
						" is exiting due to signaled state." << std::endl;
					std::cout << out_text.str();
					break;
				}
				if (MLB::Utility::WaitProcess(exec_data, wait_data, false))
					break;
				if (!lim_listener_.isValid())
					break;
			} while ((rv_status == TIBRV_OK) || (rv_status == TIBRV_TIMEOUT));
			if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
				throw MLB::RvUtilX::RvExceptionStatus(rv_status,
					"Error occurred while dispatching from the queue.");
			if (!wait_data.process_id_) {
				try {
					if (KillProcess(exec_data, wait_data, false, false)) {
						for (unsigned int count_1 = 0; count_1 < 10; ++count_1) {
							if (MLB::Utility::WaitProcess(exec_data, wait_data, false))
								break;
							MLB::Utility::SleepMilliSecs(25);
						}
					}
				}
				catch (...) {
				}
				if (wait_data.process_id_ != exec_data.process_id_)
					wait_data.process_exit_code_ = -1;
			}
			try {
				//	Add the process exit code field...
				RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateI32,
					("processExitCode", wait_data.process_exit_code_));
				//	Change the subject on which the message is to be sent...
				RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
					(asynch_subject_.c_str()));
				//	Send the asynchronous reply...
				SendReply(msg_send);
			}
			catch (const std::exception &except) {
				std::ostringstream error_text;
				error_text << "Unable to send final process status for process "
					"id " << exec_data.process_id_ << " (" <<
					wait_data.process_exit_code_ << ") on subject '" <<
					asynch_subject_ << "': " << except.what() << std::endl;
				std::cout << error_text.str();
			}
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text << "Error in " << my_description_ << ": " << except.what();
			error_text << std::endl;
			std::cout << error_text.str();
		}
		if (transport_.isValid())
			transport_.destroy();
	}
	void cleanup() {
		std::ostringstream out_text;
		out_text << "Clean-up of thread id " << get_id() << "." << std::endl;
		std::cout << out_text.str();
		if (transport_.isValid())
			transport_.destroy();
		MLB::Utility::SleepMilliSecs(rand() % 25);
	}

	void SendReply(TibrvMsg &msg_send, bool commandOk = true,
		const std::string &commandError = "") {
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateBool,
			("commandOk", (commandOk) ? TIBRV_TRUE : TIBRV_FALSE));
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
			("commandError", commandError.c_str()));
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_.send,
			(msg_send));
	}

private:
	MLB_Utility_NonCopyable_Macro(CmdThread);

	MLB::RvUtilX::RvContext  rv_context_;
	MLB::RvUtilX::SNDSpec    snd_spec_;
	std::string              reply_subject_;
	std::string              asynch_subject_;
	std::string              release_subject_;
	std::string              my_description_;
	MLB::Utility::ArgvList   argv_list_;
	MLB::Utility::EnvpList   envp_list_;
	bool                     envp_flag_;
	TibrvQueue               lim_queue_;
	TibrvNetTransport        transport_;
	TibrvListener            lim_listener_;
	LIM_CmdThreadCB          callback_lim_;
};
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LIM_ServerBase {
public:
	LIM_ServerBase() { }
	virtual ~LIM_ServerBase() { }

	virtual void ProcessCmdLim(TibrvListener *listener, TibrvMsg &msg_recv) = 0;
	virtual void EmitLog(const std::ostringstream &log_message) const = 0;
	virtual void EmitLog(const std::string &log_message) const = 0;
};
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
class LIM_ServerLimCB : public TibrvMsgCallback  {
public:
	LIM_ServerLimCB(LIM_ServerBase &control_ref) :
		control_ref_(control_ref)
	{ }

	void onMsg(TibrvListener *listener, TibrvMsg &msg_recv) {
		try {
			control_ref_.ProcessCmdLim(listener, msg_recv);
		}
		catch (const std::exception &except) {
			std::cerr << "LIM: Error on LIM message callback: " <<\
				except.what() << std::endl;
		}
	}

private:

	MLB_Utility_NonCopyable_Macro(LIM_ServerLimCB);

	LIM_ServerBase &control_ref_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LIM_Server : LIM_ServerBase {
public:
	LIM_Server(unsigned int lim_number,
		volatile MLB::Utility::AtomicFlag &active_flag_ref,
		volatile MLB::Utility::AtomicFlag &open_flag_ref) :
		 LIM_ServerBase()
		,rv_context_()
		,lim_number_(lim_number)
		,active_flag_ref_(active_flag_ref)
		,open_flag_ref_(open_flag_ref)
		,snd_spec_("", ";239.255.1.45")
		,callback_lim_(GetThisRef())
		,host_name_()
		,my_description_()
		,lim_subject_()
		,thread_list_()
		,total_thread_count_(0)
		,lim_command_map_()
		,lim_listener_()
		,queue_()
		,queue_group_()
		,transport_()
	{
		//	Create the subject names...
		Initialize();
		//	Set-up LIM commands...
		lim_command_map_["ls_rtask"]           = (&LIM_Server::Cmd_ls_rtask);
		lim_command_map_["ls_rtaske"]          = (&LIM_Server::Cmd_ls_rtask);
	}
	~LIM_Server() {
		MLB::Utility::SleepMilliSecs(rand() % 100);
		PerformThreadCleanUp();
		if (transport_.isValid())
			transport_.destroy();
		MLB::Utility::AtomicFlagClear(&active_flag_ref_);
		MLB::Utility::AtomicFlagClear(&open_flag_ref_);
	}

	void Run() {
		//	Create the general RV queue...
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_.create,
			());
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setLimitPolicy,
			(TIBRVQUEUE_DISCARD_NONE, 0, 0));
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setName,
			((my_description_ + ": General queue").c_str()));
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setPriority,
			(2));

		//	Create the RV queue group...
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_group_.create,
			());
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_group_.add,
			(&queue_));

		// Create the RV basic transport...
		snd_spec_.CreateTransport(transport_);
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_.setDescription,
			((my_description_ + ": General transport").c_str()));

		//	Create the RV listener for LIM subjects...
		TibrvStatus rv_status;
		RvUtilX_THROW_TIBRV_STATUS_IF(lim_listener_.create,
			(&queue_, &callback_lim_, &transport_,
			lim_subject_.c_str()));

		MLB::Utility::AtomicFlagSet(&active_flag_ref_);
		MLB::Utility::AtomicFlagSet(&open_flag_ref_);

		//	Dispatch RV events...
		do {
			rv_status = queue_group_.timedDispatch(0.1);
			if (MLB::Utility::CriticalEventTest()) {
				std::ostringstream log_text;
				log_text << "LSFLimSimServer is performing an asynchronous exit " <<
					"due to receipt of a signal.";
				EmitLog(log_text);
				return;
			}
			PerformThreadCleanUp();
		} while ((rv_status == TIBRV_OK) || (rv_status == TIBRV_TIMEOUT));
		if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
			throw MLB::RvUtilX::RvExceptionStatus(rv_status,
				"Error occurred while dispatching from a Rendezvous queue.");
	}

	void PerformThreadCleanUp() {
		std::deque<CmdThread *>::iterator iter_b(thread_list_.begin());
		std::deque<CmdThread *>::iterator iter_e(thread_list_.end());
		while (iter_b != iter_e) {
			if ((*iter_b)->get_finished()) {
				delete *iter_b;
				thread_list_.erase(iter_b++);
			}
			else
				++iter_b;
		}
	}

	void ProcessCmdLim(TibrvListener *listener, TibrvMsg &msg_recv) {
		ProcessCmd(listener, msg_recv, "LIM", 4, lim_command_map_);
	}

	void EmitLog(const std::ostringstream &log_message) const {
		EmitLog(log_message.str());
	}
	void EmitLog(const std::string &log_message) const {
		std::ostringstream log_text;
		log_text << "[" << MLB::Utility::CurrentProcessId() << "] " <<
			MLB::Utility::TimeVal().ToString(MLB::Utility::Length_TimeT) <<
			" " << log_message << std::endl;
//	IMPLEMENTATION NOTE: No actual printing from this thread.
//		std::cout << log_text.str();
	}

	typedef void (LIM_Server::*CommandHandlerData)
		(TibrvListener *listener, TibrvMsg &msg_recv);
	typedef std::map<std::string, CommandHandlerData> CmdMap;

	MLB::RvUtilX::RvContext                    rv_context_;
	unsigned int                               lim_number_;
	mutable volatile MLB::Utility::AtomicFlag &active_flag_ref_;
	mutable volatile MLB::Utility::AtomicFlag &open_flag_ref_;
	MLB::RvUtilX::SNDSpec                      snd_spec_;
	LIM_ServerLimCB                            callback_lim_;

	std::string                                host_name_;
	std::string                                my_description_;
	std::string                                lim_subject_;

	std::deque<CmdThread *>                    thread_list_;
	unsigned int                               total_thread_count_;
	CmdMap                                     lim_command_map_;

	TibrvListener                              lim_listener_;
	TibrvQueue                                 queue_;
	TibrvQueueGroup                            queue_group_;
	TibrvNetTransport                          transport_;

private:
	LIM_Server &GetThisRef() {
		return(*this);
	}
	void Initialize() {
		host_name_      = GetLIMHostName(lim_number_);
		my_description_ = "LSF LIM Server Simulator (" + host_name_ + ")";
		lim_subject_    = std::string(SubjectNameBase_Lim) + "." + host_name_ + ".>";
	}

	void EmitCmd(const char *log_message) const {
		std::string log_text("CMD:");
		EmitLog(log_text += log_message);
	}

	void ProcessCmd(TibrvListener *listener, TibrvMsg &msg_recv,
		const char *cmd_type, unsigned int element_count, CmdMap &cmd_map) {
		const char *msg_subject;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getSendSubject,
			(msg_subject));
		MLB::RvUtilX::SubjectNameFlat subject_flat(msg_subject);
		if (subject_flat.element_count_ != element_count) {
			std::ostringstream error_text;
			error_text << "Invalid " << cmd_type << " subject name ('" <<
				msg_subject << "') --- should have " << element_count <<
				"elements.";
			throw MLB::RvUtilX::RvException(error_text);
		}
		CmdMap::const_iterator
			cmd_iter(cmd_map.find(subject_flat.element_list_[element_count - 1]));
		if (cmd_iter == cmd_map.end()) {
			std::ostringstream error_text;
			error_text << "Invalid " << cmd_type << " command encountered (" <<
				subject_flat.element_list_[element_count - 1] << "')";
			MLB::Utility::ThrowStdException(error_text);
		}
		(this->*cmd_iter->second)(listener, msg_recv);
		EmitCmd(subject_flat.element_list_[element_count - 1]);
	}

	void Cmd_ls_rtask(TibrvListener *listener, TibrvMsg &msg_recv) {
		try {
			thread_list_.push_back(new CmdThread(listener, msg_recv, false));
			thread_list_.back()->start();
		}
		catch (pt::exception *except) {
			std::ostringstream error_text;
			error_text << "Encountered pt::exception : " <<
				except->get_message();
			delete except;
			MLB::PTypesX::ThrowPTypesException(error_text);
		}
	}
	void Cmd_SendCommandReply(TibrvListener *listener, TibrvMsg &msg_recv,
		TibrvMsg &msg_send, const char *cmd_name, bool commandOk = true,
		const std::string &commandError = "") {
		try {
			const char *reply_subject;
			msg_recv.getReplySubject(reply_subject);
			if (reply_subject == NULL) {
				std::ostringstream error_text;
				error_text << "Reply subject is 'NULL' in invocation of '" <<
					cmd_name << "'.";
				MLB::Utility::ThrowStdException(error_text);
			}
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateBool,
				("commandOk", (commandOk) ? TIBRV_TRUE : TIBRV_FALSE));
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
				("commandError", commandError.c_str()));
			RvUtilX_THROW_TIBRV_STATUS_IF(
				MLB::RvUtilX::GetTransport(listener)->sendReply,
				(msg_send, msg_recv));
		}
		catch (const std::exception &except) {
			std::string error_text("Reply attempt failed: ");
			EmitLog(error_text += except.what());
		}
	}

	MLB_Utility_NonCopyable_Macro(LIM_Server);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LIM_EntryThread : public pt::thread {
public:
	LIM_EntryThread(unsigned int lim_number, const std::string host_name,
		volatile MLB::Utility::AtomicFlag &active_flag_ref,
		volatile MLB::Utility::AtomicFlag &open_flag_ref) :
		 thread(true)
		,lim_number_(lim_number)
		,host_name_(host_name)
		,active_flag_ref_(active_flag_ref)
		,open_flag_ref_(open_flag_ref) {
	}
	~LIM_EntryThread() {
		MLB::Utility::AtomicFlagClear(&active_flag_ref_);
		MLB::Utility::AtomicFlagClear(&open_flag_ref_);
	}

	void execute() {
		try {
			MLB::Utility::SleepMilliSecs(0);
			LIM_Server lim_server(lim_number_, active_flag_ref_, open_flag_ref_);
			MLB::Utility::SleepMilliSecs(0);
			lim_server.Run();
			MLB::Utility::AtomicFlagClear(&active_flag_ref_);
			MLB::Utility::AtomicFlagClear(&open_flag_ref_);
			MLB::Utility::SleepMilliSecs(0);
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text << "Error on server host name '" << host_name_ <<
				"': " << except.what() << std::endl;
			std::cout << error_text.str();
		}
	}

	void cleanup() {
		MLB::Utility::AtomicFlagClear(&active_flag_ref_);
		std::ostringstream log_text;
		log_text << "Server host name '" << host_name_ <<
			"' is exiting from thread " << get_id() << "." << std::endl;
		std::cout << log_text.str();
	}

	unsigned int                               lim_number_;
	std::string                                host_name_;
	mutable volatile MLB::Utility::AtomicFlag &active_flag_ref_;
	mutable volatile MLB::Utility::AtomicFlag &open_flag_ref_;
 
private:
	MLB_Utility_NonCopyable_Macro(LIM_EntryThread);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LIM_Entry {
public:
	LIM_Entry(unsigned int lim_number = std::numeric_limits<unsigned int>::max(),
		unsigned int cpu_count = 1) :
		 lim_number_(lim_number)
		,cpu_count_(cpu_count)
		,host_name_(GetLIMHostName(lim_number))
		,host_info_()
		,started_flag_(false)
		,active_flag_(0)
		,open_flag_(0) {
		host_info_.hostName  = host_name_;
		host_info_.hostType  = "NTX86";
		host_info_.hostModel = "PENT200";
		host_info_.cpuFactor = 6.0;
		host_info_.maxCpus   = cpu_count_;
		host_info_.maxMem    = 2000;
		host_info_.maxSwap   = 4000;
		host_info_.maxTmp    = 8000;
		host_info_.nDisks    = 4;
	}
	~LIM_Entry() {
		MLB::Utility::AtomicFlagClear(&active_flag_);
		MLB::Utility::AtomicFlagClear(&open_flag_);
		MLB::Utility::SleepMilliSecs(0);
	}

	void Start() {
		if (started_flag_)
			MLB::Utility::ThrowStdException(
				std::string("Already started SIM LIM host '") + host_name_ +
				std::string("'."));
		LIM_EntryThread *new_thread = new LIM_EntryThread(lim_number_, host_name_,
			active_flag_, open_flag_);
		new_thread->start();
		started_flag_ = true;
	}

	unsigned int                              lim_number_;
	unsigned int                              cpu_count_;
	std::string                               host_name_;
	LSF_HostInfo                              host_info_;
	bool                                      started_flag_;
	mutable volatile MLB::Utility::AtomicFlag active_flag_;
	mutable volatile MLB::Utility::AtomicFlag open_flag_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class MLIM_ServerBase {
public:
	MLIM_ServerBase() { }
	virtual ~MLIM_ServerBase() { }

	virtual void ProcessCmdMLim(TibrvListener *listener, TibrvMsg &msg_recv) = 0;

	virtual void ShowLimCounts() const = 0;

	virtual void EmitLog(const std::ostringstream &log_message) const = 0;
	virtual void EmitLog(const std::string &log_message) const = 0;
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MLIM_ServerTimerCB : public TibrvTimerCallback
{
public:
	MLIM_ServerTimerCB(MLIM_ServerBase &control_ref) :
		 control_ref_(control_ref)
		,display_count_(-1)
		{ }

	void onTimer(TibrvTimer *) {
		try {
			// Check signal arrival here.
			//	...
			//	Every 10 seconds display the status...
			if ((display_count_ == -1) || (++display_count_ == 100)) {
				control_ref_.ShowLimCounts();
				display_count_ = 0;
			}
		}
		catch (const std::exception &except) {
			std::cerr << "MLIM: Error on timer callback: " << except.what() <<
				std::endl;
		}
	}

private:

	MLB_Utility_NonCopyable_Macro(MLIM_ServerTimerCB);

	MLIM_ServerBase &control_ref_;
	int              display_count_;
};
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class MLIM_ServerMLimCB : public TibrvMsgCallback  {
public:
	MLIM_ServerMLimCB(MLIM_ServerBase &control_ref) :
		control_ref_(control_ref)
	{ }

	void onMsg(TibrvListener *listener, TibrvMsg &msg_recv) {
		try {
	  		control_ref_.ProcessCmdMLim(listener, msg_recv);
		}
		catch (const std::exception &except) {
			std::cerr << "MLIM: Error on MLIM message callback: " <<\
				except.what() << std::endl;
		}
	}

private:

	MLB_Utility_NonCopyable_Macro(MLIM_ServerMLimCB);

	MLIM_ServerBase &control_ref_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class MLIM_Server : MLIM_ServerBase {
public:
	MLIM_Server(unsigned int lim_count, unsigned int cpu_count = 1) :
		 MLIM_ServerBase()
		,rv_context_()
		,snd_spec_("", ";239.255.1.45")
		,callback_mlim_(GetThisRef())
		,callback_timer_(GetThisRef())
		,mlim_subject_(std::string(SubjectNameBase_MLim) + ".>")
		,mlim_listener_()
		,queue_()
		,transport_()
		,timer_()
		,lim_server_count_(lim_count)
		,cpu_count_(cpu_count)
		,mlim_command_map_()
		,lim_list_() {
		//	Set-up MLIM commands...
		mlim_command_map_["ls_getclustername"] = (&MLIM_Server::Cmd_ls_getclustername);
		mlim_command_map_["ls_placereq"]       = (&MLIM_Server::Cmd_ls_placereq);
		//	Create the general RV queue...
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_.create,
			());
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setName,
			("MLIM: General queue"));
		RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setPriority,
			(2));

		// Create the RV basic transport...
		snd_spec_.CreateTransport(transport_);
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_.setDescription,
			("MLIM: General transport"));

		//	Create the RV listener for LIM subjects...
		TibrvStatus rv_status;
		//	Create the RV listener for MLIM subjects...
		RvUtilX_THROW_TIBRV_STATUS_IF(mlim_listener_.create,
			(&queue_, &callback_mlim_, &transport_,
			mlim_subject_.c_str()));

		// Create the timer event...
		RvUtilX_THROW_TIBRV_STATUS_IF(timer_.create,
			(&queue_, &callback_timer_, 0.1));

		//	Dispatch RV events...
		do {
			if (lim_list_.size() < lim_server_count_) {
				lim_list_.push_back(
					LIM_Entry(static_cast<unsigned int>(lim_list_.size())));
				lim_list_.back().Start();
			}
			rv_status = queue_.timedDispatch(0.1);
			if (MLB::Utility::CriticalEventTest()) {
				std::ostringstream log_text;
				log_text << "LSFLimSimServer is performing an asynchronous exit " <<
					"due to receipt of a signal.";
				EmitLog(log_text);
				return;
			}
		} while ((rv_status == TIBRV_OK) || (rv_status == TIBRV_TIMEOUT));
		if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
			throw MLB::RvUtilX::RvExceptionStatus(rv_status,
				"Error occurred while dispatching from a Rendezvous queue.");
	}
	~MLIM_Server() {
		MLB::Utility::CriticalEventSet();
		MLB::Utility::SleepMilliSecs(0);
/*
		while (!lim_list_.empty()) {
			bool erased_flag = false;
			if (!MLB::Utility::AtomicFlagTest(&lim_list_.front().active_flag_)) {
				lim_list_.erase(lim_list_.begin());
				erased_flag = true;
			}
			else {
				std::deque<LIM_Entry>::iterator iter_b(lim_list_.begin());
				while (iter_b != lim_list_.end()) {
					LIM_Entry *lim_ptr = &(*iter_b);
					if (!MLB::Utility::AtomicFlagTest(&iter_b->active_flag_)) {
						lim_list_.erase(iter_b);
//						iter_b = lim_list_.begin();
break;
						erased_flag = true;
					}
					else
						++iter_b;
				}
			}
if (erased_flag)
	std::cout << "Active count: " <<
		static_cast<unsigned int>(lim_list_.size()) << std::endl;
			MLB::Utility::SleepMilliSecs(0);
MLB::Utility::SleepMilliSecs(100);
		}
*/
MLB::Utility::SleepMilliSecs(500);
		if (transport_.isValid())
			transport_.destroy();
MLB::Utility::SleepMilliSecs(100);
	}

	void ProcessCmdMLim(TibrvListener *listener, TibrvMsg &msg_recv) {
		const char         *cmd_type      = "MLIM";
		const unsigned int  element_count = 3;
		const char         *msg_subject;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getSendSubject,
			(msg_subject));
		MLB::RvUtilX::SubjectNameFlat subject_flat(msg_subject);
		if (subject_flat.element_count_ != element_count) {
			std::ostringstream error_text;
			error_text << "Invalid " << cmd_type << " subject name ('" <<
				msg_subject << "') --- should have " << element_count <<
				"elements.";
			throw MLB::RvUtilX::RvException(error_text);
		}
		CmdMap::const_iterator
			cmd_iter(mlim_command_map_.find(subject_flat.element_list_[element_count - 1]));
		if (cmd_iter == mlim_command_map_.end()) {
			std::ostringstream error_text;
			error_text << "Invalid " << cmd_type << " command encountered (" <<
				subject_flat.element_list_[element_count - 1] << "')";
			MLB::Utility::ThrowStdException(error_text);
		}
		(this->*cmd_iter->second)(listener, msg_recv);
		EmitCmd(subject_flat.element_list_[element_count - 1]);
	}

	void ShowLimCounts() const {
		unsigned int                     active_count = 0;
		unsigned int                     open_count   = 0;
		std::deque<LIM_Entry>::size_type count_1;
		for (count_1 = 0; count_1 < lim_list_.size(); ++count_1) {
			active_count +=
				(MLB::Utility::AtomicFlagTest(&(lim_list_[count_1]).active_flag_)) ?
				1 : 0;
			open_count   +=
				(MLB::Utility::AtomicFlagTest(&(lim_list_[count_1]).open_flag_))   ?
				1 : 0;
		}
		std::ostringstream log_text;
		log_text <<
			std::setw(10) << static_cast<unsigned int>(lim_list_.size()) << " " <<
			std::setw(10) << active_count << " " <<
			std::setw(10) << open_count;
		EmitLog(log_text);
	}
	void Cmd_ls_getclustername(TibrvListener *listener, TibrvMsg &msg_recv) {
		TibrvMsg msg_send;
		//	Put the cluster name into the reply message...
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
			("replyText", "LimSimCluster"));
		//	Send the reply...
		Cmd_SendCommandReply(listener, msg_recv, msg_send, "ls_getclustername()");
	}
	void Cmd_ls_placereq(TibrvListener *listener, TibrvMsg &msg_recv) {
		TibrvMsg msg_send;
		//	Get the number of host desired by the requestor...
		unsigned int numberOfHosts;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getU32,
			("numberOfHosts", numberOfHosts));
		//	Fix-up the number of desired hosts (0 = all)...
		numberOfHosts = (numberOfHosts) ? numberOfHosts :
			std::numeric_limits<unsigned int>::max();
		//	Plae the host names into a list...
		std::deque<std::string>               host_list;
		std::deque<LIM_Entry>::const_iterator iter_b(lim_list_.begin());
		std::deque<LIM_Entry>::const_iterator iter_e(lim_list_.end());
		while ((iter_b != iter_e) && (host_list.size() < numberOfHosts)) {
			if (MLB::Utility::AtomicFlagTest(&iter_b->active_flag_) &&
				MLB::Utility::AtomicFlagTest(&iter_b->open_flag_)) {
				for (unsigned int count_1 = 0; count_1 < cpu_count_; ++count_1)
					host_list.push_back(iter_b->host_name_);
			}
			++iter_b;
		}
		//	Put the list into the reply message...
		MLB::RvUtilX::AddRvFieldStringList(msg_send, "replyList", host_list);
		//	Send the reply...
		Cmd_SendCommandReply(listener, msg_recv, msg_send, "ls_placereq()");
	}

	void Cmd_SendCommandReply(TibrvListener *listener, TibrvMsg &msg_recv,
		TibrvMsg &msg_send, const char *cmd_name, bool commandOk = true,
		const std::string &commandError = "") {
		try {
			const char *reply_subject;
			msg_recv.getReplySubject(reply_subject);
			if (reply_subject == NULL) {
				std::ostringstream error_text;
				error_text << "Reply subject is 'NULL' in invocation of '" <<
					cmd_name << "'.";
				MLB::Utility::ThrowStdException(error_text);
			}
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateBool,
				("commandOk", (commandOk) ? TIBRV_TRUE : TIBRV_FALSE));
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.updateString,
				("commandError", commandError.c_str()));
			RvUtilX_THROW_TIBRV_STATUS_IF(
				MLB::RvUtilX::GetTransport(listener)->sendReply,
				(msg_send, msg_recv));
		}
		catch (const std::exception &except) {
			std::string error_text("Reply attempt failed: ");
			EmitLog(error_text += except.what());
		}
	}
	void EmitCmd(const char *log_message) const {
		std::string log_text("CMD:");
		EmitLog(log_text += log_message);
	}
	void EmitLog(const std::ostringstream &log_message) const {
		EmitLog(log_message.str());
	}
	void EmitLog(const std::string &log_message) const {
		std::ostringstream log_text;
		log_text << "[" << MLB::Utility::CurrentProcessId() << "] " <<
			MLB::Utility::TimeVal().ToString(MLB::Utility::Length_TimeT) <<
			" " << log_message << std::endl;
		std::cout << log_text.str();
	}

	typedef void (MLIM_Server::*CommandHandlerData)
		(TibrvListener *listener, TibrvMsg &msg_recv);
	typedef std::map<std::string, CommandHandlerData> CmdMap;

	MLB::RvUtilX::RvContext rv_context_;
	MLB::RvUtilX::SNDSpec   snd_spec_;
	MLIM_ServerMLimCB       callback_mlim_;
	MLIM_ServerTimerCB      callback_timer_;
	std::string             mlim_subject_;
	TibrvListener           mlim_listener_;
	TibrvQueue              queue_;
	TibrvNetTransport       transport_;
	TibrvTimer              timer_;
	unsigned int            lim_server_count_;
	unsigned int            cpu_count_;
	CmdMap                  mlim_command_map_;
	std::deque<LIM_Entry>   lim_list_;

private:

	MLIM_Server &GetThisRef() {
		return(*this);
	}

	MLB_Utility_NonCopyable_Macro(MLIM_Server);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code = EXIT_SUCCESS;
	unsigned int lim_count   = 300;
	unsigned int cpu_count   =   2;

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -lim_count <number-of-lim-nodes> ( = " << lim_count << " ) ]" <<
			"[ -cpu_count <number-of-cpus-per-node> ( = " << cpu_count << " ) ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	try {
		unsigned int count_1;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (MLB::Utility::ParseCmdLineArg::
				ParseCmdLineNumeric<unsigned int>("-LIM_COUNT", count_1, argc,
				argv, lim_count, 1))
				;
			else if (MLB::Utility::ParseCmdLineArg::
				ParseCmdLineNumeric<unsigned int>("-CPU_COUNT", count_1, argc,
				argv, cpu_count, 1))
				;
			else
				MLB::Utility::ParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		MLB::Utility::CriticalEventContext critical_event_context;
		MLB::LSFWrapper::MLIM_Server       mlim_control(lim_count, cpu_count);
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

