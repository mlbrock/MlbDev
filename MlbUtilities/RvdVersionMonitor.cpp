//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Program
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implements scanning of Tib/Rendezvous daemons so as
								to monitor transmission problems.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/InlineContainer.hpp>
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/VersionNumber.hpp>
#include <Utility/LogManager.hpp>

#include <RvUtilX/RvParseCmdLineArg.hpp>
#include <RvUtilX/RvCallback.hpp>
#include <RvUtilX/GetRvField.hpp>

#include <SockLibX/SocketX.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

# include <boost/shared_array.hpp>
# include <boost/shared_ptr.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Defines the global logging objects...
//	////////////////////////////////////////////////////////////////////////////
LogManagerMacroDefinition(MB_LIB_LOCAL)
//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

namespace RvInfo {

//	////////////////////////////////////////////////////////////////////////////
struct HostStatusMsg_Snapshot {
	HostStatusMsg_Snapshot()
		:hostaddr(0)
		,sn(0)
		,os(0)
		,ver(NULL)
		,up(0)
		,httpaddr(0)
		,httpport(0)
		,ms(0)
		,bs(0)
		,mr(0)
		,br(0)
		,ps(0)
		,pr(0)
		,rx(0)
		,pm(0)
	{
	}
	HostStatusMsg_Snapshot(const TibrvMsg &msg_recv)
		:hostaddr(ExtractField_hostaddr(msg_recv))
		,sn(ExtractField_sn(msg_recv))
		,os(ExtractField_os(msg_recv))
		,ver(ExtractField_ver(msg_recv))
		,up(ExtractField_up(msg_recv))
		,httpaddr(ExtractField_httpaddr(msg_recv))
		,httpport(ExtractField_httpport(msg_recv))
		,ms(ExtractField_ms(msg_recv))
		,bs(ExtractField_bs(msg_recv))
		,mr(ExtractField_mr(msg_recv))
		,br(ExtractField_br(msg_recv))
		,ps(ExtractField_ps(msg_recv))
		,pr(ExtractField_pr(msg_recv))
		,rx(ExtractField_rx(msg_recv))
		,pm(ExtractField_pm(msg_recv))
	{
	}

	static tibrv_ipaddr32 ExtractField_hostaddr(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "hostaddr", msg_field, TIBRVMSG_IPADDR32);
		return(msg_field.getData().ipaddr32);
	}
	static tibrv_u32       ExtractField_sn(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "sn", msg_field, TIBRVMSG_U32);
		return(msg_field.getData().u32);
	}
	static tibrv_u8        ExtractField_os(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "os", msg_field, TIBRVMSG_U8);
		return(msg_field.getData().u8);
	}
	static const char     *ExtractField_ver(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "ver", msg_field, TIBRVMSG_STRING);
		return(msg_field.getData().str);
	}
	static tibrv_u32       ExtractField_up(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "up", msg_field, TIBRVMSG_U32);
		return(msg_field.getData().u32);
	}
	static tibrv_ipaddr32  ExtractField_httpaddr(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "httpaddr", msg_field, TIBRVMSG_IPADDR32);
		return(msg_field.getData().ipaddr32);
	}
	static tibrv_ipport16  ExtractField_httpport(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "httpport", msg_field, TIBRVMSG_IPPORT16);
		return(::ntohs(msg_field.getData().ipport16));
	}
	static tibrv_u64       ExtractField_ms(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "ms", msg_field, TIBRVMSG_U64);
		return(msg_field.getData().u64);
	}
	static tibrv_u64       ExtractField_bs(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "bs", msg_field, TIBRVMSG_U64);
		return(msg_field.getData().u64);
	}
	static tibrv_u64       ExtractField_mr(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "mr", msg_field, TIBRVMSG_U64);
		return(msg_field.getData().u64);
	}
	static tibrv_u64       ExtractField_br(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "br", msg_field, TIBRVMSG_U64);
		return(msg_field.getData().u64);
	}
	static tibrv_u64       ExtractField_ps(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "ps", msg_field, TIBRVMSG_U64);
		return(msg_field.getData().u64);
	}
	static tibrv_u64       ExtractField_pr(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "pr", msg_field, TIBRVMSG_U64);
		return(msg_field.getData().u64);
	}
	static tibrv_u64       ExtractField_rx(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "rx", msg_field, TIBRVMSG_U64);
		return(msg_field.getData().u64);
	}
	static tibrv_u64       ExtractField_pm(const TibrvMsg &msg_recv) {
		TibrvMsgField msg_field;
		ExtractField(msg_recv, "pm", msg_field, TIBRVMSG_U64);
		return(msg_field.getData().u64);
	}

	static HostStatusMsg_Snapshot &ExtractMessage(const TibrvMsg &msg_recv,
		HostStatusMsg_Snapshot &host_status) {
		HostStatusMsg_Snapshot tmp_host_status(msg_recv);
		host_status = tmp_host_status;
		return(host_status);
	}
	static HostStatusMsg_Snapshot  ExtractMessage(const TibrvMsg &msg_recv) {
		HostStatusMsg_Snapshot host_status;
		return(ExtractMessage(msg_recv, host_status));
	}

	tibrv_ipaddr32  hostaddr;
	tibrv_u32       sn;						
	tibrv_u8        os;						//	??? Rvd=2, Rvrd=130
	const char     *ver;						//	Version 
	tibrv_u32       up;						//	Uptime
	tibrv_ipaddr32  httpaddr;
	tibrv_ipport16  httpport;
	tibrv_u64       ms;						//	Messages Sent
	tibrv_u64       bs;						//	Bytes Sent
	tibrv_u64       mr;						//	Messages Received
	tibrv_u64       br;						//	Bytes Received
	tibrv_u64       ps;						//	Packets Sent
	tibrv_u64       pr;						//	Packets Received
	tibrv_u64       rx;						//	Retransmission Requests
	tibrv_u64       pm;						//	Packets Missed

private:
	static TibrvMsgField &ExtractField(const TibrvMsg &msg_recv,
		const char *field_name, TibrvMsgField &msg_field,
		tibrv_u8 required_type = TIBRVMSG_NO_TAG, tibrv_u16 field_id = 0) {
		try {
			GetRvField(msg_recv, field_name, msg_field, field_id, true,
				required_type);
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Unable to extract a field in a "
				"Tib/Rv host status message: " + std::string(except.what()));
		}
		return(msg_field);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvdVersionMonHost {
public:
	RvdVersionMonHost(unsigned int host_address = 0,
		const char *version_number = "", int os_value = 0,
		unsigned int http_address = 0, unsigned int http_port = 0)
		:host_address_(static_cast<unsigned long>(host_address))
		,version_number_(version_number)
		,os_value_(os_value)
		,http_address_(static_cast<unsigned long>(http_address))
		,http_port_(static_cast<unsigned int>(http_port))
		,host_name_()
	{
	}

	bool operator < (const RvdVersionMonHost &other) const {
		return(host_address_ < other.host_address_);
	}

	std::string &GetHostNameRef() const {
		return(ResolveHostName());
	}

	std::string ToStringFormatted() const {
		std::ostringstream o_str;
		o_str << std::setw(10) << version_number_ << " " << std::setw(15) <<
			host_address_ << " " << std::left << std::setw(64) <<
			GetHostNameRef() << " (HTTP Access: " <<
			MLB::SockLibX::EndPointIP(http_address_, http_port_) << ")";
		return(o_str.str());
	}

	bool IsVersionChange(const char *new_version_number) const {
		return(version_number_ != new_version_number);
	}

	RvdVersionMonHost &ApplyMessage(const TibrvMsg &msg_recv);

	MLB::SockLibX::AddressIP host_address_;
	std::string              version_number_;
	int                      os_value_;
	MLB::SockLibX::AddressIP http_address_;
	unsigned int             http_port_;
	mutable std::string      host_name_;		//	Lazily-populated, hence mutable.

private:
	std::string &ResolveHostName() const {
		if (host_name_.empty()) {
			try {
				MLB::SockLibX::ResolveHostToName(host_address_.ToString(),
					host_name_);
			}
			catch (const std::exception &) {
				host_name_ = "UNABLE.TO.RESOLVE.HOST";
			}
		}
		return(host_name_);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<RvdVersionMonHost>          RvdVersionMonHostSet;
typedef RvdVersionMonHostSet::iterator       RvdVersionMonHostSetIter;
typedef RvdVersionMonHostSet::const_iterator RvdVersionMonHostSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvdVersionMonHost &RvdVersionMonHost::ApplyMessage(const TibrvMsg &msg_recv)
{
	host_address_   = MLB::SockLibX::AddressIP(static_cast<unsigned long>(
		HostStatusMsg_Snapshot::ExtractField_hostaddr(msg_recv)));
	version_number_ = HostStatusMsg_Snapshot::ExtractField_ver(msg_recv);
	os_value_       = HostStatusMsg_Snapshot::ExtractField_os(msg_recv);
	http_address_   = MLB::SockLibX::AddressIP(static_cast<unsigned long>(
		HostStatusMsg_Snapshot::ExtractField_httpaddr(msg_recv)));
	http_port_      = HostStatusMsg_Snapshot::ExtractField_httpport(msg_recv);
	host_name_.clear();

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvdVersionMonService {
public:
	RvdVersionMonService(const SNDSpec &snd_spec);

	bool operator < (const RvdVersionMonService &other) const;

	void ApplyMessage(const MLB::Utility::StringSet &versions_set,
		unsigned int longest_snd_spec, const TibrvMsg &msg_recv,
		unsigned int &bad_count);

	int                                  service_id_;
	SNDSpec                              snd_spec_;
	std::string                          snd_spec_name_;
	boost::shared_ptr<TibrvNetTransport> transport_ptr_;
	RvdVersionMonHostSet                 host_set_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<RvdVersionMonService>          RvdVersionMonServiceSet;
typedef RvdVersionMonServiceSet::iterator       RvdVersionMonServiceSetIter;
typedef RvdVersionMonServiceSet::const_iterator RvdVersionMonServiceSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvdVersionMonService::RvdVersionMonService(const SNDSpec &snd_spec)
	:service_id_(::atoi(snd_spec.service_.c_str()))
	,snd_spec_(snd_spec)
	,snd_spec_name_(snd_spec.ToStringSimple())
	,transport_ptr_(new TibrvNetTransport)
	,host_set_()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvdVersionMonService::operator < (const RvdVersionMonService &other) const
{
	int cmp = service_id_ - other.service_id_;

	return((cmp < 0) ? true : ((cmp > 0) ? false :
		(snd_spec_ < other.snd_spec_)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMonService::ApplyMessage(
	const MLB::Utility::StringSet &versions_set, unsigned int longest_snd_spec,
	const TibrvMsg &msg_recv, unsigned int &bad_count)
{
	tibrv_ipaddr32  hostaddr =
		HostStatusMsg_Snapshot::ExtractField_hostaddr(msg_recv);
	const char     *ver       =
		HostStatusMsg_Snapshot::ExtractField_ver(msg_recv);

	bool version_is_ok = (versions_set.empty() ||
		(versions_set.find(ver) != versions_set.end()));

	RvdVersionMonHostSetIter iter_f(
										host_set_.find(RvdVersionMonHost(hostaddr)));

	if (iter_f == host_set_.end()) {
		RvdVersionMonHost new_host;
		new_host.ApplyMessage(msg_recv);
		iter_f = host_set_.insert(new_host).first;
		bad_count += (version_is_ok) ? 0 : 1;
		if (version_is_ok)
			LogDetail << "New host, version ok        : " << std::left <<
				std::setw(longest_snd_spec) << snd_spec_name_ << " " <<
				iter_f->ToStringFormatted() << std::endl;
		else
			LogWarning << "New host, version invalid   : " << std::left <<
				std::setw(longest_snd_spec) << snd_spec_name_ << " " <<
				iter_f->ToStringFormatted() << std::endl;
	}
	else if (iter_f->IsVersionChange(ver)) {
		if (version_is_ok) {
			LogDetail << "Host change, version ok      : " << std::left <<
				std::setw(longest_snd_spec) << snd_spec_name_ << " " <<
				iter_f->ToStringFormatted() << std::endl;
			--bad_count;
		}
		else {
			LogWarning << "Host change, version invalid: " << std::left <<
				std::setw(longest_snd_spec) << snd_spec_name_ << " " <<
				iter_f->ToStringFormatted() << std::endl;
			++bad_count;
		}
	}
	else
		LogDebug << "Host heartbeat, no change      : " << std::left <<
				std::setw(longest_snd_spec) << snd_spec_name_ << " " <<
				iter_f->ToStringFormatted() << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvdVersionMonRecap {
public:
	RvdVersionMonRecap(const RvdVersionMonService &service_ref,
		const RvdVersionMonHost &host_ref)
		:service_ptr_(&service_ref)
		,host_ptr_(&host_ref)
	{
	}

	bool operator < (const RvdVersionMonRecap &other) const {
		int cmp =
			host_ptr_->version_number_.compare(other.host_ptr_->version_number_);
		if (cmp)
			return(cmp < 0);
		cmp = service_ptr_->snd_spec_.ToStringSimple().compare(
			other.service_ptr_->snd_spec_.ToStringSimple());
		if (cmp)
			return(cmp < 0);
		return(host_ptr_->GetHostNameRef().compare(
			other.host_ptr_->GetHostNameRef()) < 0);
	}

	std::string ToString(unsigned int longest_snd_spec) const {
		std::ostringstream o_str;
		o_str << std::setw(10) << host_ptr_->version_number_ << " " <<
			std::setw(longest_snd_spec) << service_ptr_->snd_spec_name_ << " " <<
			std::setw(15) << host_ptr_->host_address_ << " " <<
			host_ptr_->GetHostNameRef();
		return(o_str.str());
	}

	static void EmitRecap(const RvdVersionMonServiceSet &service_set,
		unsigned int longest_snd_spec);

	const RvdVersionMonService *service_ptr_;
	const RvdVersionMonHost    *host_ptr_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMonRecap::EmitRecap(const RvdVersionMonServiceSet &service_set,
	unsigned int longest_snd_spec)
{
	typedef std::set<RvdVersionMonRecap>          RvdVersionMonRecapSet;
	typedef RvdVersionMonRecapSet::const_iterator RvdVersionMonRecapSetIterC;

	RvdVersionMonRecapSet        recap_set;
	RvdVersionMonServiceSetIterC iter_s_b(service_set.begin());
	RvdVersionMonServiceSetIterC iter_s_e(service_set.end());

	for ( ; iter_s_b != iter_s_e; ++iter_s_b) {
		RvdVersionMonHostSetIterC iter_h_b(iter_s_b->host_set_.begin());
		RvdVersionMonHostSetIterC iter_h_e(iter_s_b->host_set_.end());
		for ( ; iter_h_b != iter_h_e; ++iter_h_b)
			recap_set.insert(RvdVersionMonRecap(*iter_s_b, *iter_h_b));
	}

	LogInfo.LogSeparator('-');

	RvdVersionMonRecapSetIterC iter_r_b(recap_set.begin());
	RvdVersionMonRecapSetIterC iter_r_e(recap_set.end());

	for ( ; iter_r_b != iter_r_e; ++iter_r_b)
		LogInfo << "Version Number Recap: " <<
			iter_r_b->ToString(longest_snd_spec) << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvdVersionMon {
public:
	RvdVersionMon()
		:rv_context_()
		,version_number_(1, 0, 0, 0)
		,service_set_()
		,valid_versions_set_()
		,log_flag_(true)
		,log_dir_("./")
		,queue_()
		,listener_list_()
		,callback_data_(GetThisRef())
		,timer_()
		,callback_timer_(GetThisRef())
		,timer_count_(-1)
		,longest_snd_spec_(0)
		,bad_count_(0)
		,heartbeat_count_(0)
	{
	}

	void ParseCmdLine(int argc, char **argv);
	void Start();
	void DetermineServiceList();
	void Run();

	RvContext                                         rv_context_;
	MLB::Utility::VersionNumber                       version_number_;
	RvdVersionMonServiceSet                           service_set_;
	MLB::Utility::StringSet                           valid_versions_set_;
	bool                                              log_flag_;
	std::string                                       log_dir_;
	TibrvQueue                                        queue_;
	std::deque<boost::shared_ptr<TibrvListener> >     listener_list_;
	MLB::RvUtilX::RvCallbackMsgEvent<RvdVersionMon>   callback_data_;
	TibrvTimer                                        timer_;
	MLB::RvUtilX::RvCallbackTimerEvent<RvdVersionMon> callback_timer_;
	int                                               timer_count_;
	unsigned int                                      longest_snd_spec_;
	unsigned int                                      bad_count_;
	unsigned long long                                heartbeat_count_;

	void ProcessEventOnMsg(TibrvMsgCallback *call_back_ptr,
		TibrvListener *listener_ptr, TibrvMsg &msg_recv);
	void ProcessEventOnTimer(TibrvTimerCallback *callback_ptr,
		TibrvTimer *timer_ptr);
	void ReportException(const std::exception *except_ptr);

private:
	MLB_Utility_NonCopyable_Macro(RvdVersionMon);

	RvdVersionMon &GetThisRef() {
		return(*this);
	}

	void StartInternal(const std::string &my_name);
	void EmitStatus() const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMon::ParseCmdLine(int argc, char **argv)
{
	unsigned int count_1;
	SNDSpec      snd_template;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		std::string tmp_string;
		if (RvParseCmdLineArg::ParseCmdLineCombinedServiceNetwork(count_1, argc,
			argv, snd_template))
			service_set_.insert(RvdVersionMonService(snd_template));
		else if (RvParseCmdLineArg::ParseCmdLineDaemon(count_1, argc,
			argv, snd_template.daemon_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-VALID_VERSIONS")
			("-VALIDVERSIONS")
			("-VALID_VERSION")
			("-VALIDVERSION")
			("-VALID_VERS")
			("-VALIDVERS")
			("-VALID_VER")
			("-VALIDVER"),
			count_1, argc, argv, tmp_string)) {
			MLB::Utility::StringVector tmp_list;
			MLB::Utility::SplitString(tmp_string, ",", tmp_list, 0, true);
			MLB::Utility::StringVectorIter iter_b(tmp_list.begin());
			MLB::Utility::StringVectorIter iter_e(tmp_list.end());
			for ( ; iter_b != iter_e ; ++iter_b) {
				if (!iter_b->empty())
					valid_versions_set_.insert(MLB::Utility::UpperCase(*iter_b));
			}
		}
		else if (RvParseCmdLineArg::ParseLoggingFlag(count_1, argc, argv,
			log_flag_))
			;
		else if (RvParseCmdLineArg::ParseLoggingDir(count_1, argc, argv,
			log_dir_, "", true))
			;
		else
			RvParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	//	If no services have been specified, use the default Tib/Rv one...
	if (service_set_.empty())
		service_set_.insert(RvdVersionMonService(SNDSpec()));

	//	Ensure logging directory is fully resolved...
	if (log_flag_)
		MLB::Utility::ResolveDirectoryPath(log_dir_, "", true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMon::Start()
{
	MLB::Utility::TimeVal start_up_time;
	std::string           my_name("RvdVersionMonitor on host " +
		MLB::Utility::GetHostNameCanonical() + ", PID " +
		MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()));

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Start-up logging...
	//	//////////////////////////////////////////////////////////////////////
	std::string log_file_name;
	try {
		MyLogManager.SetLogLevelConsole(MLB::Utility::LogLevel_Info);
		MyLogManager.SetLogLevelFile(MLB::Utility::LogLevel_Detail);
		if (log_flag_) {
			std::string tmp_date_time(start_up_time.ToTimeT().ToString());
			tmp_date_time[10]  = '.';
			tmp_date_time[13]  = '.';
			tmp_date_time[16]  = '.';
			log_file_name = log_dir_ + "/RvdVersionMonitor." + tmp_date_time +
				"." + MLB::Utility::GetHostNameCanonical() + "." +
				MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()) +
				".log";
			boost::shared_ptr<MLB::Utility::LogHandlerFile>
				my_log_handler(new MLB::Utility::LogHandlerFile(log_file_name,
					MLB::Utility::LogHandlerFile::None));
			MyLogManager.HandlerInstall(my_log_handler);
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to install logging facility: " +
			std::string(except.what()));
	}
	//	//////////////////////////////////////////////////////////////////////

	LogInfo.LogSeparator('=');
	LogInfo.LogSeparator('=');

	LogInfo << "Program Name      : RvdVersionMonitor" << std::endl;
	LogInfo << "Program Version   : " << version_number_ << std::endl;
	LogInfo << "Host Name         : " <<
		MLB::Utility::GetHostNameCanonical() << std::endl;
	LogInfo << "Process Id        : " <<
		MLB::Utility::CurrentProcessId() << std::endl;
	LogInfo << "Logging Flag      : " <<
		MLB::Utility::AnyToString(log_flag_) << std::endl;
	LogInfo << "Logging Directory : " <<
		((log_flag_) ? log_dir_ : "N/A") << std::endl;
	LogInfo << "Logging File      : " <<
		((log_flag_) ? log_file_name : "N/A") << std::endl;

	LogInfo.LogSeparator('-');

	//	Log information about the Tib/Rv services covered...
	{
		RvdVersionMonServiceSetIterC iter_b(service_set_.begin());
		RvdVersionMonServiceSetIterC iter_e(service_set_.end());
		for ( ; iter_b != iter_e ; ++iter_b) {
			LogInfo << ((iter_b == service_set_.begin()) ?
				"Rendezvous Service: " : "                  : ") <<
				iter_b->snd_spec_ << std::endl;
			longest_snd_spec_ = std::max(longest_snd_spec_,
				static_cast<unsigned int>(iter_b->snd_spec_name_.size()));
		}
	}

	LogInfo.LogSeparator('=');

	try {
		StartInternal(my_name);
	}
	catch (const std::exception &except) {
		LogError << except.what() << std::endl;
	}

	if (!service_set_.empty()) {
		LogInfo.LogSeparator('=');
		try {
			RvdVersionMonRecap::EmitRecap(service_set_, longest_snd_spec_);
		}
		catch (const std::exception &except) {
			LogError << "Unable to produce the version number recap: " <<
				except.what() << std::endl;
		}
		LogInfo.LogSeparator('=');
	}

	LogInfo << "Exiting program..." << std::endl;

	LogInfo.LogSeparator('=');
	LogInfo << "End of Log File" << std::endl;
	LogInfo.LogSeparator('=');
	LogInfo.LogSeparator('=');
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMon::StartInternal(const std::string &my_name)
{
	//	Create the Tib/Rv message queue...
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.create,
		());
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setLimitPolicy,
		(TIBRVQUEUE_DISCARD_NONE, 0, 0));
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setPriority,
		(2));
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setName,
		((my_name + ": Queue").c_str()));

	//	Create a transport for of the Tib/Rv services covered...
	RvdVersionMonServiceSetIterC iter_b(service_set_.begin());
	RvdVersionMonServiceSetIterC iter_e(service_set_.end());

	for ( ; iter_b != iter_e ; ++iter_b) {
		//	Create the transport...
		iter_b->snd_spec_.CreateTransport(*iter_b->transport_ptr_);
		//	Give it a descriptive name...
		RvUtilX_THROW_TIBRV_STATUS_IF(iter_b->transport_ptr_->setDescription,
			((my_name + ": Transport").c_str()));
		//	Create the listener...
		boost::shared_ptr<TibrvListener> the_listener(new TibrvListener);
		//	Bind the listener to its transport... Note that we use a pointer
		//	to the RvdVersionMonService instance as the closure...
		RvUtilX_THROW_TIBRV_STATUS_IF(the_listener->create,
			(&queue_, &callback_data_, iter_b->transport_ptr_.get(),
			"_RV.INFO.SYSTEM.HOST.STATUS.>", &(*iter_b)));
		listener_list_.push_back(the_listener);
	}

	//	Add the interval timer event...
	RvUtilX_THROW_TIBRV_STATUS_IF(timer_.create,
		(&queue_, &callback_timer_, 0.1));

	//	Dispatch RV events...
	TibrvStatus rv_status;

	do {
		rv_status = queue_.timedDispatch(0.1);
		if (MLB::Utility::CriticalEventTest()) {
			LogInfo << "RvdVersionMonitor is performing an asynchronous exit "
				"due to receipt of a signal." << std::endl;
			break;
		}
	} while ((rv_status == TIBRV_OK) || (rv_status == TIBRV_TIMEOUT));

	if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
		throw RvExceptionStatus(rv_status,
			"Error occurred while dispatching from a Rendezvous queue.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMon::EmitStatus() const
{
	unsigned int                 total_count = 0;
	RvdVersionMonServiceSetIterC iter_b(service_set_.begin());
	RvdVersionMonServiceSetIterC iter_e(service_set_.end());

	for ( ; iter_b != iter_e ; ++iter_b)
		total_count += static_cast<unsigned int>(iter_b->host_set_.size());

	std::ostringstream out_string;

	out_string <<
		MLB::Utility::TimeVal::Now().ToString().substr(0, 19) <<
		std::setw(10)  << static_cast<unsigned int>(service_set_.size()) << " " <<
		std::setw(10)  << total_count << " " <<
		std::setw(10)  << bad_count_ << " " <<
		std::setw(22) << heartbeat_count_;

	if (log_flag_)
		MyLogManager.EmitLiteral(out_string.str());
	else
		std::cout << out_string.str() << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMon::ProcessEventOnMsg(TibrvMsgCallback *,
	TibrvListener *listener_ptr, TibrvMsg &msg_recv)
{
	try {
		RvdVersionMonService *service_ptr =
			static_cast<RvdVersionMonService *>(listener_ptr->getClosure());
		service_ptr->ApplyMessage(valid_versions_set_, longest_snd_spec_,
			msg_recv, bad_count_);
		++heartbeat_count_;
	}
	catch (const std::exception &except) {
		LogError << "Error processing message: " << except.what() << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMon::ProcessEventOnTimer(TibrvTimerCallback *, TibrvTimer *)
{
	if (timer_count_ == -1) {
		EmitStatus();
		timer_count_ = 0;
	}
	else {
		++timer_count_;
		if (timer_count_ && (!(timer_count_ % 100))) {
			EmitStatus();
			timer_count_ = 0;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdVersionMon::ReportException(const std::exception *except_ptr)
{
	LogWarning << "Exception encountered in Tib/Rendezvous callback: " <<
		((except_ptr != NULL) ? except_ptr->what() : "*UNSPECIFIED*") <<
		std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvInfo

} // namespace RvUtilX

} // namespace MLB

//#include <SockLibX/ParseCmdLineArg.hpp>

//using namespace MLB::RvUtilX::RvInfo;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code = EXIT_SUCCESS;
	std::string  my_name     = MLB::Utility::GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (MLB::RvUtilX::RvParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -service_net <RV-service>[:<RV-network>] ... ] " <<
			"[ -daemon <RV-daemon> ] " <<
			"[ -valid_versions <rvd-version>[,<rvd-version>[,...]] ] " <<
			"[ -logging <boolean> ( = FALSE ) ] " <<
			"[ -logging_directory <directory-path-for-log-files> ( = ./ ) ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -service_net <RV-service>[:<RV-network>]\n\
      Specifies a Tib/Rendezvous service:network communications for\n\
      analysis by this program.\n\n\
      This parameter may be specified multiple times.\n\n\
   -daemon <RV-daemon>\n\
      The Tib/Rendezvous daemon to which the connection is to be made.\n\n\
   -valid_versions <rvd-version>[,<rvd-version>[,...]]\n\
      Specifies one or more versions considered to be valid by this program.\n\
      Multiple rvd version numbers may be specified as a single parameter by\n\
      separating them with comma (','). In addition, this parameter may be\n\
      specified multiple times to produce an aggregated list of versions.\n\n\
   -logging <boolean>\n\
      Specifies whether logging is to be performed.\n\n\
   -logging_directory <directory-in-which-to-log>\n\
      Specifies the directory in which to log." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		MLB::Utility::CriticalEventContext  critical_event_context;
		MLB::SockLibX::SockLibXContext      socket_lib;
		MLB::RvUtilX::RvInfo::RvdVersionMon rvd_inspector;
		rvd_inspector.ParseCmdLine(argc, argv);
		rvd_inspector.Start();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << my_name << ": fatal error encountered: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

