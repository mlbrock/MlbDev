//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Support Program
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Monitors traffic rates on one or more IP multicast
								groups.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2017.
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
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/LogManager.hpp>
#include <SockLibX/SocketX.hpp>
#include <SockLibX/ParseCmdLineArg.hpp>
#include <Utility/StringSupport.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

# include <boost/scoped_array.hpp>

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
namespace Utility {
//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Enhanced version to be added to the Utility library.
std::string &StringToHexDebug(const std::string &in_string,
	std::string &out_string)
{
	std::string::size_type  count_1       = 0;
	std::string::size_type  data_length = in_string.size();

	while (count_1 < data_length) {
		std::string::size_type char_count = 0;
		std::string            buffer_hex;
		std::string            buffer_char;
		buffer_hex.reserve(16 * 3);
		buffer_char.reserve(16);
		while ((count_1 < data_length) && (char_count < 16)) {
			buffer_hex  += " " + MLB::Utility::ValueToStringHex(
				static_cast<unsigned char>(in_string[count_1]), false);
			buffer_char += ((in_string[count_1] < ' ') ||
				(in_string[count_1] > '~')) ? '.' : in_string[count_1];
			++count_1;
			++char_count;
		}
		std::string pad_string_1;
		std::string pad_string_2;
		out_string += "\n--->" +
			MLB::Utility::PadRight(buffer_hex,  pad_string_1, 16 * 3) + " " +
			MLB::Utility::PadRight(buffer_char, pad_string_2,     16);
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Enhanced version to be added to the Utility library.
std::string StringToHexDebug(const std::string &in_string)
{
	std::string out_string;

	return(StringToHexDebug(in_string, out_string));
}
//	////////////////////////////////////////////////////////////////////////////
} // namespace Utility
} // namespace MLB

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
typedef unsigned long long UnsignedQuantity;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MCServiceInfo {
public:
	MCServiceInfo()
		:port_()
		,port_number_(0)
		,port_string_()
		,service_address_()
		,service_address_string_()
		,last_update_time_(0, 0)
		,last_bytes_(0)
		,saved_last_bytes_(0)
		,total_seconds_(0)
		,total_bytes_(0)
		,min_bytes_(std::numeric_limits<unsigned int>::max())
		,max_bytes_(0)
		,mc_socket_(AddressFamily_Internet, SocketType_DataGram)
	{
	}
	MCServiceInfo(const std::string &service_port,
		const std::string &service_address)
		:port_(service_port)
		,port_number_(ResolveServToPort(service_port.c_str()))
		,port_string_()
		,service_address_(AddressIP(service_address, true), service_port)
		,service_address_string_()
		,last_update_time_(0, 0)
		,last_bytes_(0)
		,saved_last_bytes_(0)
		,total_seconds_(0)
		,total_bytes_(0)
		,min_bytes_(std::numeric_limits<unsigned int>::max())
		,max_bytes_(0)
		,mc_socket_(AddressFamily_Internet, SocketType_DataGram)
	{
		service_address_string_ = service_address_.GetHostIPAddress();
		port_string_            = (atoi(port_.c_str()) ==
			static_cast<int>(port_number_)) ?
			MLB::Utility::AnyToString(port_number_) : port_;
		std::ostringstream o_str;
		o_str <<
			std::setw(5) << port_number_ << " " <<
			std::left << std::setw(15) << service_address_string_ << std::right;
		SetMyName(o_str.str());
	}

	bool operator < (const MCServiceInfo &other) const {
		return(port_number_ < other.port_number_);
	}

	void SetMyName(const std::string &my_name) {
		my_name_ = my_name;
	}
	void JoinMulticastGroup() {
		mc_socket_.CheckNotConnected();
		//	Set the socket address reuse to true...
		BOOL reuse_addr_flag = TRUE;
		mc_socket_.SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &reuse_addr_flag,
			sizeof(reuse_addr_flag));
		//	Bind to the desired port...
		NativeSockAddr_In bind_info;
		bind_info.sin_family      = AF_INET; 
		bind_info.sin_addr.s_addr = htonl(INADDR_ANY); 
		bind_info.sin_port        = htons(
			static_cast<unsigned short>(port_number_)); 
		mc_socket_.Bind(bind_info);
		//	Join the multicast group.
		struct ip_mreq mc_info;
		mc_info.imr_multiaddr.s_addr = inet_addr(service_address_string_.c_str()); 
		mc_info.imr_interface.s_addr = INADDR_ANY; 
		mc_socket_.SetSockOpt(IPPROTO_IP, IP_ADD_MEMBERSHIP, &mc_info,
			sizeof(mc_info));
	}
	void SetFD(NativeFDSet_T &recv_fd_set) {
		mc_socket_.AppendToFDSet(recv_fd_set);
	}
	bool ReadIfReady(const NativeFDSet_T &recv_fd_set,
		unsigned int buffer_size, char *buffer_ptr) {
		if (!FD_ISSET(mc_socket_.GetSocket(), &recv_fd_set))
			return(false);
#if 0
		last_bytes_ += mc_socket_.RecvFrom(buffer_ptr, buffer_size);
#else
		EndPointIP peer_name;
		unsigned int these_bytes = mc_socket_.RecvFrom(buffer_ptr, buffer_size,
			0, peer_name);
		if (these_bytes) {
			last_bytes_ += these_bytes;
std::string in_string(buffer_ptr, these_bytes);
std::string out_string;
LogInfo << "Peer Name  : " << peer_name << std::endl;
LogInfo << "Recv Length: " << these_bytes << std::endl;
LogInfo << MLB::Utility::StringToHexDebug(in_string, out_string) << std::endl;
		}
#endif // #if 0
		return(true);
	}
	void Recap() {
		++total_seconds_;
		total_bytes_      += last_bytes_;
		min_bytes_         = std::min(min_bytes_, last_bytes_);
		max_bytes_         = std::max(max_bytes_, last_bytes_);
		saved_last_bytes_  = last_bytes_;
		last_bytes_        = 0;
	}
	void Recap(MCServiceInfo &totals_info) {
		totals_info.last_bytes_ += last_bytes_;
		Recap();
	}
	void EmitRecap() const {
		std::ostringstream o_str;
		o_str <<
			MLB::Utility::TimeVal::Now().ToString(19) << " " <<
			std::left << std::setw(21) << my_name_ << " " << std::right <<
			std::setw(10) << saved_last_bytes_ << " " <<
			std::setw(10) << total_seconds_ << " " <<
			std::setw(22) << total_bytes_ << " " <<
			std::setw(10) << min_bytes_ << " " <<
			std::setw(10) << max_bytes_;
		MyLogManager.EmitLiteral(o_str.str());
	}

	std::string           port_;
	unsigned int          port_number_;
	std::string           port_string_;
	EndPointIP            service_address_;
	std::string           service_address_string_;
	MLB::Utility::TimeVal last_update_time_;
	unsigned int          last_bytes_;
	unsigned int          saved_last_bytes_;
	unsigned int          total_seconds_;
	UnsignedQuantity      total_bytes_;
	unsigned int          min_bytes_;
	unsigned int          max_bytes_;
	SocketX               mc_socket_;
	std::string           my_name_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<MCServiceInfo>          MCServiceInfoSet;
typedef MCServiceInfoSet::iterator       MCServiceInfoSetIter;
typedef MCServiceInfoSet::const_iterator MCServiceInfoSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<std::ofstream>   CsvFilePtr;
typedef std::map<unsigned int, CsvFilePtr> CsvFileMap;
typedef CsvFileMap::iterator               CsvFileMapIter;
typedef CsvFileMap::const_iterator         CsvFileMapIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MCMonitor {
public:
	MCMonitor()
		:socket_lib_()
		,csv_flag_(false)
		,log_flag_(false)
		,log_dir_("./")
		,mc_info_set_()
		,csv_file_map_()
		,csv_file_name_base_()
		,max_socket_handle_(0)
		,recv_fd_set_()
		,totals_info_()
	{
		FD_ZERO(&recv_fd_set_);
		totals_info_.SetMyName("Totals");
	}

	bool ParseCmdLineMcServiceInfo(unsigned int &current_index,
		int argc, char **argv, MCServiceInfo &out_datum);
	void ParseCmdLine(int argc, char **argv);
	void Start();
	void Run();

private:
	SockLibXContext      socket_lib_;
	bool                 csv_flag_;
	bool                 log_flag_;
	std::string          log_dir_;
	MCServiceInfoSet     mc_info_set_;
	CsvFileMap           csv_file_map_;
	std::string          csv_file_name_base_;
	NativeSocketHandle   max_socket_handle_;
	NativeFDSet_T        recv_fd_set_;
	MCServiceInfo        totals_info_;

	void WriteCsv(const MCServiceInfo &recap_info) {
		if (csv_flag_) {
			CsvFileMapIter iter_f(GetCsv(recap_info.port_number_));
			*iter_f->second.get()                                <<
				recap_info.last_update_time_                      << "," <<
				recap_info.port_number_                           << "," <<
				recap_info.service_address_string_                << "," <<
				recap_info.saved_last_bytes_                      << "," <<
				recap_info.total_seconds_                         << "," <<
				recap_info.total_bytes_                           << "," <<
				recap_info.min_bytes_                             << "," <<
				recap_info.max_bytes_                             << "," <<
				std::endl;
		}
	}

	CsvFileMapIter GetCsv(unsigned int service_number) {
		CsvFileMapIter iter_f(csv_file_map_.find(service_number));
		if (iter_f == csv_file_map_.end()) {
			std::string file_name(csv_file_name_base_ + ".SERVICE=" +
				MLB::Utility::ZeroFill(service_number, 5) + ".csv");
			CsvFilePtr tmp_file_ptr(new std::ofstream(file_name.c_str(),
					std::ios_base::app | std::ios_base::ate));
			if (tmp_file_ptr->fail())
				MLB::Utility::ThrowSystemError("Attempt to open CSV file '" +
					file_name + "' for writing failed");
			*tmp_file_ptr                         <<
				"\"UTC Date/Time\","               <<
				"\"Port\","                        <<
				"\"Multicast Group\","             <<
				"\"Last Byte Count\","             <<
				"\"Total Seconds\","               <<
				"\"Total Byte Count\","            <<
				"\"Minimum Byte Count\","          <<
				"\"Maximum Byte Count\","          << std::endl;
			csv_file_map_[service_number] = tmp_file_ptr;
			iter_f = csv_file_map_.find(service_number);
		}
		return(iter_f);
	}
};
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool MCMonitor::ParseCmdLineMcServiceInfo(unsigned int &current_index,
	int argc, char **argv, MCServiceInfo &out_datum)
{
	std::string tmp_string;

	if (!MLB::Utility::ParseCmdLineArg::ParseCmdLineFollowingSpec(
		MLB::Utility::MakeInlineVector<std::string>
		("-MULTI_CAST_GROUP")
		("-MULTI_CASTGROUP")
		("-MULTICAST_GROUP")
		("-MULTICASTGROUP")
		("-MULTI_CAST")
		("-MULTICAST")
		("-M_C_GROUP")
		("-M_CGROUP")
		("-MC_GROUP")
		("-MCGROUP")
		("-M_C")
		("-MC"),
		current_index, argc, argv, tmp_string))
		return(false);

	std::string::size_type found_pos = tmp_string.find(":");

	if ((found_pos == std::string::npos) || (!found_pos))
		MLB::Utility::ThrowInvalidArgument("Invalid port/multicast group ('" +
			tmp_string + "') --- expected '<port-or-service>:"
			"<multicast-address>'.");

	out_datum = MCServiceInfo(tmp_string.substr(0, found_pos),
		tmp_string.substr(found_pos + 1));

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MCMonitor::ParseCmdLine(int argc, char **argv)
{
	unsigned int count_1;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		std::string   tmp_string;
		MCServiceInfo tmp_mc_group;
		if (ParseCmdLineMcServiceInfo(count_1, argc, argv, tmp_mc_group))
			mc_info_set_.insert(tmp_mc_group);
		else if (MLB::Utility::ParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-CSV_FLAG")
			("-CSVFLAG")
			("-CSV"),
			count_1, argc, argv, tmp_string))
			MLB::Utility::ParseCmdLineArg::ParseCmdLineDatum(
				tmp_string, csv_flag_);
		else if (MLB::Utility::ParseCmdLineArg::ParseLoggingFlag(count_1,
			argc, argv, log_flag_))
			;
		else if (MLB::Utility::ParseCmdLineArg::ParseLoggingDir(count_1,
			argc, argv, log_dir_, "", true))
			;
		else
			MLB::Utility::ParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	if (mc_info_set_.empty())
		MLB::Utility::ThrowInvalidArgument("No multicast groups were specified "
			"with the '-multicast_group' parameter --- use '-h' for help.");

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Ensure logging directory is fully resolved...
	//	//////////////////////////////////////////////////////////////////////
	MLB::Utility::ResolveDirectoryPath(log_dir_, "", true);
	//	//////////////////////////////////////////////////////////////////////
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MCMonitor::Start()
{
	std::string           my_name(MLB::Utility::GetHostNameCanonical() + "/" +
		MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()));

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
	file_name = log_dir_ + "/MCMonitor." + tmp_date_time + "." +
		MLB::Utility::GetHostNameCanonical() + "." +
		MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId());
	csv_file_name_base_  = file_name;
	file_name           += ".log";

	boost::shared_ptr<MLB::Utility::LogHandlerFile>
		my_log_handler(new MLB::Utility::LogHandlerFile(file_name,
			MLB::Utility::LogHandlerFile::None));
	MyLogManager.SetLogLevelConsole(MLB::Utility::LogLevel_Info);
	MyLogManager.SetLogLevelFile(MLB::Utility::LogLevel_Detail);
	MyLogManager.HandlerInstall(my_log_handler);
	//	//////////////////////////////////////////////////////////////////////

	LogInfo.LogSeparator('=');
	LogInfo.LogSeparator('=');

	LogInfo << "Host Name            : " <<
		MLB::Utility::GetHostNameCanonical() << std::endl;
	LogInfo << "Process Id           : " <<
		MLB::Utility::CurrentProcessId() << std::endl;
	LogInfo << "Csv Flag             : " <<
		MLB::Utility::AnyToString(csv_flag_) << std::endl;
	if (csv_flag_)
		LogInfo << "Csv File Name Pattern: " << csv_file_name_base_ <<
			".*.csv" << std::endl;
	else
		LogInfo << "Csv File Name Pattern: " << "N/A" << std::endl;
	LogInfo << "Logging Flag         : " <<
		MLB::Utility::AnyToString(log_flag_) << std::endl;
	LogInfo << "Logging Directory    : " << log_dir_ << std::endl;
	LogInfo << "Logging File         : " << ((log_flag_) ? file_name : "N/A") <<
		std::endl;

	try {
		LogInfo.LogSeparator('-');
		MCServiceInfoSetIter iter_b(mc_info_set_.begin());
		MCServiceInfoSetIter iter_e(mc_info_set_.end());
		MCServiceInfoSetIter iter_first(iter_b);
		FD_ZERO(&recv_fd_set_);
		while (iter_b != iter_e) {
			std::string xport_name(iter_b->port_ + ":" +
				iter_b->service_address_string_);
			if (iter_b->port_ != MLB::Utility::AnyToString(iter_b->port_number_))
				xport_name += " (port number " +
					MLB::Utility::AnyToString(iter_b->port_number_) + ")";
			try {
				LogInfo << ((iter_b == iter_first) ? "Port/Multicast Group : " :
					"                     : ") << xport_name << std::endl;
				iter_b->SetFD(recv_fd_set_);
				iter_b->JoinMulticastGroup();
				if (max_socket_handle_ < iter_b->mc_socket_.GetSocket())
					max_socket_handle_ = iter_b->mc_socket_.GetSocket();
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except, "Attempt to start service "
					"for service " + xport_name + " failed: " +
					std::string(except.what()));
			}
			++iter_b;
		}
		LogInfo.LogSeparator('=');
		Run();
	}
	catch (const std::exception &except) {
		LogError << except.what() << std::endl;
	}

	LogInfo << "Exiting program..." << std::endl;

	LogInfo.LogSeparator('=');
	LogInfo << "End of Log File" << std::endl;
	LogInfo.LogSeparator('=');
	LogInfo.LogSeparator('=');
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void MCMonitor::Run()
{
	//	Large buffer for our multicast messages...
	unsigned int mc_buffer_size = 1 << 20;
	boost::scoped_array<char> mc_buffer(new char[mc_buffer_size]);

	//	We'll sleep waiting for messages up to 100 ms at a time...
	MLB::Utility::TimeVal time_out(0, 100);

	time_t last_time = ::time(NULL);

	while (!MLB::Utility::CriticalEventTest()) {
		NativeFDSet_T recv_fd_set = recv_fd_set_;
		unsigned int  ready_count;
		ready_count = Select(max_socket_handle_, &recv_fd_set,
			NULL, NULL, &time_out);
		if (MLB::Utility::CriticalEventTest())
			break;
		if (ready_count) {
			MCServiceInfoSetIter iter_b(mc_info_set_.begin());
			MCServiceInfoSetIter iter_e(mc_info_set_.end());
			while (iter_b != iter_e) {
				iter_b->ReadIfReady(recv_fd_set, mc_buffer_size, mc_buffer.get());
				++iter_b;
			}
		}
		if (last_time != ::time(NULL)) {
			MCServiceInfoSetIter iter_b(mc_info_set_.begin());
			MCServiceInfoSetIter iter_e(mc_info_set_.end());
			while (iter_b != iter_e) {
				iter_b->Recap(totals_info_);
				iter_b->EmitRecap();
				if (csv_flag_)
					WriteCsv(*iter_b);
				++iter_b;
			}
			totals_info_.Recap();
			if (mc_info_set_.size() > 1) {
				totals_info_.EmitRecap();
				MyLogManager.EmitLiteral("");
			}
			last_time = ::time(NULL);
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

using namespace MLB::SockLibX;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code = EXIT_SUCCESS;
	std::string  my_name     = MLB::Utility::GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (SockLibXParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -multicast_group <port-or-service>:<multi-cast-group> ... ] " <<
			"[ -csv <boolean> ( = FALSE ) ] " <<
			"[ -logging <boolean> ( = FALSE ) ] " <<
			"[ -logging_directory <directory-path-for-log-files> ( = ./ ) ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -multicast_group <port-or-service>:<multi-cast-group>\n\
      The port/service and multicast group for message capture.\n\n\
   -csv <boolean>\n\
      Specifies whether recap information is to be written to a file in CSV\n\
      format.\n\n\
   -logging <boolean>\n\
      Specifies whether logging is to be performed.\n\n\
   -logging_directory <directory-in-which-to-log>\n\
      Specifies the directory in which to log." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		MCMonitor                          mc_monitor;
		mc_monitor.ParseCmdLine(argc, argv);
		mc_monitor.Start();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << my_name << ": fatal error encountered: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

