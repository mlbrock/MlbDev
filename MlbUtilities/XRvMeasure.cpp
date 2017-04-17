// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Program
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of a Tib/Rv measurement program.

	Revision History	:	2005-06-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RvCallback.hpp>
#include <RvUtilX/RvParseCmdLineArg.hpp>
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/LogManager.hpp>

#include <map>
#include <iomanip>

#ifdef _Windows
# pragma warning(disable:4217 4668)
# include <boost/shared_ptr.hpp>
# pragma warning(default:4217 4668)
#else
# include <boost/shared_ptr.hpp>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Defines the global logging objects...
//	////////////////////////////////////////////////////////////////////////////
LogManagerMacroDefinition(MB_LIB_LOCAL)
//	////////////////////////////////////////////////////////////////////////////

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
typedef unsigned long long QuantityType;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct MeasurementPoint {
	MeasurementPoint() :
		 active_flag_(false)
		,time_stamp_(0, 0)
		,last_time_(0, 0)
		,message_count_(0)
		,byte_count_(0)
	{
	}
	MeasurementPoint(const MLB::Utility::TimeVal &time_stamp) :
		 active_flag_(false)
		,time_stamp_(time_stamp)
		,last_time_(time_stamp)
		,message_count_(0)
		,byte_count_(0)
	{
	}

	void AddMessage(TibrvMsg &msg_recv) {
		tibrv_u32 msg_byte_length;
		RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.getByteSize,
			(msg_byte_length));
		++message_count_;
		byte_count_  += static_cast<QuantityType>(msg_byte_length);
		active_flag_  = true;
	}

	void Clear() {
		time_stamp_.tv_sec  = 0;
		time_stamp_.tv_usec = 0;
		message_count_      = static_cast<QuantityType>(0);
		byte_count_         = static_cast<QuantityType>(0);
	}
	void Clear(const MLB::Utility::TimeVal &new_time_stamp) {
		time_stamp_    = new_time_stamp;
		last_time_     = new_time_stamp;
		message_count_ = static_cast<QuantityType>(0);
		byte_count_    = static_cast<QuantityType>(0);
	}

	std::string ToStringSimple() const {
		return(MLB::Utility::AnyToString(message_count_) + "/" +
			MLB::Utility::AnyToString(byte_count_));
	}

	std::string ToMeasure() const {
		std::ostringstream o_str;
		o_str <<
			std::setw(22) << message_count_ << " " <<
			std::setw(22) << byte_count_;
		return(o_str.str());
	}

	bool                  active_flag_;
	MLB::Utility::TimeVal time_stamp_;
	MLB::Utility::TimeVal last_time_;
	QuantityType          message_count_;
	QuantityType          byte_count_;
};
inline std::ostream & operator << (std::ostream &o_str,
	const MeasurementPoint &datum)
{
	o_str <<
		datum.time_stamp_ << " " << datum.ToMeasure();

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
enum MeasurementType {
	Current = 0,
	Minimum = 1,
	Maximum = 2,
	Total   = 3,
	Entries = 4
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<MeasurementPoint> MeasurementList;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::map<std::string, MeasurementPoint> NameMeasureMap;
typedef NameMeasureMap::iterator                NameMeasureMapIter;
typedef NameMeasureMap::const_iterator          NameMeasureMapIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class MeasurementControl {
public:
	MeasurementControl() :
		 rv_context_()
		,log_flag_(false)
		,log_dir_("./")
		,name_info_flag_(false)
		,name_elements_count_(MaxElementCount)
		,snd_spec_()
		,subject_list_()
		,exclude_list_()
		,measure_list_(Entries)
		,name_measure_map_()
		,queue_()
		,transport_()
		,timer_()
		,listener_()
		,listener_list_()
		,callback_data_(GetThisRef())
		,callback_timer_(GetThisRef()) {
	}

	void ParseCmdLine(int argc, char **argv);
	void Start(const std::string &my_name);
	void ProcessEventOnMsg(TibrvMsgCallback *, TibrvListener *listener,
		TibrvMsg &msg_recv);
	void ProcessEventOnTimer(TibrvTimerCallback *, TibrvTimer *);
	void HandleMeasure(const char *measure_text =
		"Recap for the current second");

	void ReportException(const std::exception *except_ptr) {
		LogError << "Exception encountered in Tib/Rendezvous callback: " <<
			((except_ptr != NULL) ? except_ptr->what() : "*UNSPECIFIED*") <<
			std::endl;
	}

	RvContext                                              rv_context_;
	bool                                                   log_flag_;
	std::string                                            log_dir_;
	bool                                                   name_info_flag_;
	unsigned int                                           name_elements_count_;
	MLB::RvUtilX::SNDSpec                                  snd_spec_;
	MLB::RvUtilX::SubjectNameFlatSet                       subject_list_;
	MLB::RvUtilX::SubjectNameFlatSet                       exclude_list_;
	MeasurementList                                        measure_list_;
	NameMeasureMap                                         name_measure_map_;
	TibrvQueue                                             queue_;
	TibrvNetTransport                                      transport_;
	TibrvTimer                                             timer_;
	TibrvListener                                          listener_;
	std::deque<boost::shared_ptr<TibrvListener> >          listener_list_;
	MLB::RvUtilX::RvCallbackMsgEvent<MeasurementControl>   callback_data_;
	MLB::RvUtilX::RvCallbackTimerEvent<MeasurementControl> callback_timer_;

	MeasurementControl &GetThisRef() {
		return(*this);
	}

	MLB_Utility_NonCopyable_Macro(MeasurementControl);
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MeasurementControl::ParseCmdLine(int argc, char **argv)
{
	unsigned int count_1;
	std::string  subject_name;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (snd_spec_.ParseCmdLineServiceNetworkDaemon(count_1, argc,
			argv))
			;
		else if (RvParseCmdLineArg::ParseLoggingFlag(count_1, argc, argv,
			log_flag_))
			;
		else if (RvParseCmdLineArg::ParseLoggingDir(count_1, argc, argv,
			log_dir_, "", true))
			;
		else if (RvParseCmdLineArg::ParseCmdLineDatum("-NAME_INFO", count_1,
			argc, argv, name_info_flag_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(
			"-NAME_ELEMENTS", count_1, argc, argv, name_elements_count_, 1,
			MaxElementCount))
			;
		else if (RvParseCmdLineArg::ParseCmdLineSubject("-EXCLUDE", count_1,
			argc, argv, subject_name))
			exclude_list_.insert(subject_name);
		else if (RvParseCmdLineArg::ParseCmdLineSubject(argv[count_1],
			subject_name))
			subject_list_.insert(subject_name);
	}

	//	Must have at least one subject name pattern...
	if (subject_list_.empty())
		MLB::Utility::ThrowInvalidArgument("No subject names specified.");

	//	Fix-up the logging directory...
	std::string tmp_string(log_dir_);
	MLB::Utility::ResolveFilePathGeneral(tmp_string, tmp_string, "",
		true, true, false);
	log_dir_ = tmp_string;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MeasurementControl::Start(const std::string &my_name)
{
	MLB::Utility::TimeVal start_up_time;

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Start-up logging...
	//	//////////////////////////////////////////////////////////////////////
	std::string tmp_date_time(start_up_time.ToTimeT().ToString());
	std::string file_name;
	tmp_date_time[10]  = '.';
	tmp_date_time[13]  = '.';
	tmp_date_time[16]  = '.';
	file_name = log_dir_ + "/" + my_name + "." + tmp_date_time + "." +
		MLB::Utility::GetHostNameCanonical() + "." +
		MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()) + ".log";
	boost::shared_ptr<MLB::Utility::LogHandlerFile>
		my_log_handler(new MLB::Utility::LogHandlerFile(file_name,
			MLB::Utility::LogHandlerFile::None));
	MyLogManager.SetLogLevelConsole(MLB::Utility::LogLevel_Info);
	MyLogManager.SetLogLevelFile(MLB::Utility::LogLevel_Detail);
	MyLogManager.HandlerInstall(my_log_handler);
	//	//////////////////////////////////////////////////////////////////////

	LogInfo.LogSeparator('=');
	LogInfo.LogSeparator('=');

	LogInfo << "Host Name           : " <<
		MLB::Utility::GetHostNameCanonical() << std::endl;
	LogInfo << "Process Id          : " <<
		MLB::Utility::CurrentProcessId() << std::endl;
	LogInfo << "RV Comms Basic      : " << snd_spec_ << std::endl;
	LogInfo << "Logging Directory   : " << log_dir_ << std::endl;
	LogInfo << "Logging File        : " << file_name << std::endl;
	LogInfo << "Name Info Flag      : " <<
		MLB::Utility::AnyToString(name_info_flag_) << std::endl;
	LogInfo << "Name Info Elements  : " <<
		((name_info_flag_) ? MLB::Utility::AnyToString(name_elements_count_) :
		"N/A") << std::endl;
	{
		bool                                  is_first_flag = true;
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_b(subject_list_.begin());
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_e(subject_list_.end());
		while (iter_b != iter_e) {
			LogInfo << ((is_first_flag) ? "Include Subject(s)  : " :
				"                    : ") << iter_b->ToString() << std::endl;
			++iter_b;
			is_first_flag = false;
		}
	}
	{
		bool                                  is_first_flag = true;
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_b(exclude_list_.begin());
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_e(exclude_list_.end());
		while (iter_b != iter_e) {
			LogInfo << ((is_first_flag) ? "Exclude Subject(s)  : " :
				"                    : ") << iter_b->ToString() << std::endl;
			++iter_b;
			is_first_flag = false;
		}
	}

	LogInfo.LogSeparator('=');

	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.create,
		());
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setLimitPolicy,
		(TIBRVQUEUE_DISCARD_NONE, 0, 0));
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setPriority,
		(2));
	RvUtilX_THROW_TIBRV_STATUS_IF(queue_.setName,
		((my_name + ": Queue").c_str()));

	//	Create the transport...
	snd_spec_.CreateTransport(transport_);
	RvUtilX_THROW_TIBRV_STATUS_IF(transport_.setDescription,
		((my_name + ": Transport").c_str()));

	{
		//	Subscribe to the subjects of interest...
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_b(subject_list_.begin());
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_e(subject_list_.end());
		while (iter_b != iter_e) {
			boost::shared_ptr<TibrvListener> the_listener(new TibrvListener);
			RvUtilX_THROW_TIBRV_STATUS_IF(the_listener->create,
				(&queue_, &callback_data_, &transport_,
				iter_b->ToString().c_str()));
			LogInfo << "Subscribed to to subject " << iter_b->ToString() <<
				std::endl;
			listener_list_.push_back(the_listener);
			++iter_b;
		}
	}

	LogInfo.LogSeparator('-');

	RvUtilX_THROW_TIBRV_STATUS_IF(timer_.create,
		(&queue_, &callback_timer_, 1.0));

	MLB::Utility::TimeVal start_time;

	measure_list_[Current].time_stamp_ = start_time;

	//	Dispatch RV events...
	TibrvStatus rv_status;
	do {
		rv_status = queue_.timedDispatch(0.1);
		if (MLB::Utility::CriticalEventTest()) {
			std::ostringstream log_text;
			LogInfo << my_name << " is performing an asynchronous exit " <<
				"due to receipt of a signal." << std::endl;
			break;
		}
	} while ((rv_status == TIBRV_OK) || (rv_status == TIBRV_TIMEOUT));

	if ((rv_status != TIBRV_OK) && (rv_status != TIBRV_TIMEOUT))
		throw RvExceptionStatus(rv_status,
			"Error occurred while dispatching from a Rendezvous queue.");

	MLB::Utility::TimeVal end_time;

	if (measure_list_[Current].message_count_) {
		if (measure_list_[Current].byte_count_ <
			measure_list_[Minimum].byte_count_)
			measure_list_[Minimum] = measure_list_[Current];
		if (measure_list_[Current].byte_count_ >
			measure_list_[Maximum].byte_count_)
			measure_list_[Maximum] = measure_list_[Current];
		measure_list_[Total].time_stamp_     = measure_list_[Current].time_stamp_;
		measure_list_[Total].message_count_ += measure_list_[Current].message_count_;
		measure_list_[Total].byte_count_    += measure_list_[Current].byte_count_;
		HandleMeasure("Recap for the last second");
	}

	LogInfo.LogSeparator('=');
	LogInfo << "Measurement time start   : " << start_time << std::endl;
	LogInfo << "Measurement time end     : " << end_time << std::endl;
	LogInfo << "Measurement time interval: " <<
		MLB::Utility::TimeVal::GetDifferenceAbs(start_time, end_time).
		ToStringInterval() << std::endl;
	LogInfo.LogSeparator('=');
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MeasurementControl::ProcessEventOnMsg(TibrvMsgCallback *, TibrvListener *,
	TibrvMsg &msg_recv)
{
	std::string subject_name(MLB::RvUtilX::GetSendSubject(msg_recv));

	if (!exclude_list_.empty()) {
		MLB::RvUtilX::SubjectNameFlat         match_name(subject_name);
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_b(exclude_list_.begin());
		MLB::RvUtilX::SubjectNameFlatSetIterC iter_e(exclude_list_.end());
		while (iter_b != iter_e) {
			if (iter_b->MatchSubjectName(match_name))
				return;
			++iter_b;
		}
	}

	MLB::Utility::TimeVal this_time;

	if (measure_list_[Current].time_stamp_.tv_sec != this_time.tv_sec) {
		if (!measure_list_[Minimum].byte_count_) {
			measure_list_[Minimum] = measure_list_[Current];
			measure_list_[Maximum] = measure_list_[Current];
			measure_list_[Total]   = measure_list_[Current];
		}
		else {
			if (measure_list_[Current].byte_count_ <
				measure_list_[Minimum].byte_count_)
				measure_list_[Minimum] = measure_list_[Current];
			if (measure_list_[Current].byte_count_ >
				measure_list_[Maximum].byte_count_)
				measure_list_[Maximum] = measure_list_[Current];
			measure_list_[Total].time_stamp_     = measure_list_[Current].time_stamp_;
			measure_list_[Total].message_count_ += measure_list_[Current].message_count_;
			measure_list_[Total].byte_count_    += measure_list_[Current].byte_count_;
		}
		HandleMeasure();
		measure_list_[Current].Clear(this_time);
		measure_list_[Current].AddMessage(msg_recv);
	}
	else
		measure_list_[Current].AddMessage(msg_recv);

	if (name_info_flag_) {
		try {
			MLB::Utility::StringVector name_list;
			SubjectNameToMultiPartList(subject_name, name_list);
			MLB::Utility::StringVectorIterC iter_b(name_list.begin());
			MLB::Utility::StringVectorIterC iter_e(name_list.end());
			unsigned int                    elements_done = 0;
			while ((iter_b != iter_e) && (elements_done < name_elements_count_)) {
				NameMeasureMapIter iter_f(name_measure_map_.find(*iter_b));
				if (iter_f != name_measure_map_.end()) {
					iter_f->second.AddMessage(msg_recv);
					iter_f->second.last_time_ = this_time;
				}
				else {
					MeasurementPoint new_name_measure;
					new_name_measure.Clear(this_time);
					new_name_measure.AddMessage(msg_recv);
					name_measure_map_[*iter_b] = new_name_measure;
				}
				++iter_b;
				++elements_done;
			}
		}
		catch (const std::exception &except) {
			LogError << except.what() << std::endl;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MeasurementControl::ProcessEventOnTimer(TibrvTimerCallback *, TibrvTimer *)
{
	MLB::Utility::TimeVal this_time;

	if ((measure_list_[Minimum].byte_count_) &&
		(measure_list_[Current].time_stamp_.tv_sec != this_time.tv_sec))
		HandleMeasure();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MeasurementControl::HandleMeasure(const char *measure_text)
{
	try {
		if (!log_flag_)
			std::cout << measure_list_[Current].time_stamp_.ToString(19) <<
				std::endl <<
				"*  Current: " << measure_list_[Current].ToMeasure() << std::endl <<
				"*  Minimum: " << measure_list_[Minimum].ToMeasure() << std::endl <<
				"*  Maximum: " << measure_list_[Maximum].ToMeasure() << std::endl <<
				"*  Total  : " << measure_list_[Total].ToMeasure()   << std::endl;
		else {
			LogInfo << measure_text << ":" << std::endl;
			MyLogManager.EmitLiteral(std::string("*  Current: ") +
				measure_list_[Current].ToMeasure());
			MyLogManager.EmitLiteral(std::string("*  Minimum: ") +
				measure_list_[Minimum].ToMeasure());
			MyLogManager.EmitLiteral(std::string("*  Maximum: ") +
				measure_list_[Maximum].ToMeasure());
			MyLogManager.EmitLiteral(std::string("*  Total  : ") +
				measure_list_[Total].ToMeasure());
		}
	}
	catch (const std::exception &except) {
		LogError << except.what() << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code = EXIT_SUCCESS;
	std::string  my_name     = MLB::Utility::GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (RvParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -name_info ( = false ) ] " <<
			"[ -name_elements <maximum-elements-to-use> ( = unlimited ) ] " <<
			"[ -logging <boolean> ( = FALSE ) ] " <<
			"[ -logging_directory <directory-in-which-to-log> ( = ./ ) ] " <<
			"[ -service <RV-service> ] " <<
			"[ -network <RV-network> ] " <<
			"[ -daemon <RV-daemon> ] " <<
			"[ -exclude <subject-name-pattern> ] " <<
			"<subject-name-pattern> [ <subject-name-pattern> ...] " <<
			" *** NOTE: The <subject-name-pattern> parameter(s) must be last " <<
			"on the command line.\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -name_info <boolean>\n\
      Specifies whether the program is to emit a subject name analysis\n\
      upon exit.\n\n\
   -name_elements <maximum-elements-to-use>\n\
      Specifies the number of name elements to use in the subject name\n\
      analysis. Can only be used if the '-name_info' parameter has been\n\
      specified.\n\n\
   -logging <boolean>\n\
      Specifies whether logging is to be performed.\n\n\
   -logging_directory <directory-in-which-to-log>\n\
      Specifies the directory in which to log.\n\n\
   -service <RV-service>\n\
      The Tib/Rendezvous service to be used for message capture.\n\n\
   -network <RV-network>\n\
      The Tib/Rendezvous network to be used for message capture.\n\n\
   -daemon <RV-daemon>\n\
      The Tib/Rendezvous daemon to be used for message capture.\n\n\
   -exclude <subject-name-pattern>\n\
      Exclude <subject-name-pattern> from message capture.\n\n\
   <subject-name-pattern> [ <subject-name-pattern> ... ]\n\
      Specifies the subject names to be captured." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		MeasurementControl measure_control;
		measure_control.ParseCmdLine(argc, argv);
		measure_control.Start(my_name);
		if (!measure_control.name_measure_map_.empty()) {
			LogInfo.LogSeparator('=');
			MyLogManager.EmitLiteral("Subject Name Analysis");
			LogInfo.LogSeparator('-');
			NameMeasureMapIterC iter_b(measure_control.name_measure_map_.begin());
			NameMeasureMapIterC iter_e(measure_control.name_measure_map_.end());
			while (iter_b != iter_e) {
				std::ostringstream log_text;
				log_text << iter_b->second.ToMeasure() << " " <<
					iter_b->second.time_stamp_ << " " <<
					iter_b->second.last_time_ << " " << iter_b->first;
				MyLogManager.EmitLiteral(log_text.str());
				++iter_b;
			}
		}
		LogInfo.LogSeparator('=');
	}
	catch (const std::exception &except) {
		LogError << my_name << ": fatal error encountered: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

