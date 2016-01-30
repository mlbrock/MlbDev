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

		Copyright Michael L. Brock 2002 - 2016.
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
#include <Utility/Sleep.hpp>
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <Utility/LogManager.hpp>
#include <Utility/StringSupport.hpp>

#include <RvUtilX/RvParseCmdLineArg.hpp>

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

/*
[2007-05-16 09:05:14]: subject=_RV.INFO.SYSTEM.HOST.STATUS.AC1C4815, message={{0
}ADV_CLASS(0)="INFO", {0}ADV_SOURCE(0)="SYSTEM", {0}ADV_NAME(0)="HOST.STATUS.AC1
C4815", {0}hostaddr(0)=172.28.72.21, {0}sn(0)=222638, {0}os(0)=2, {0}ver(0)="6.9
.8", {0}up(0)=157590, {0}httpaddr(0)=172.28.72.21, {0}httpport(0)=7580, {0}ms(0)
=84412, {0}bs(0)=29800824, {0}mr(0)=43272063, {0}br(0)=16275633306, {0}ps(0)=247
138, {0}pr(0)=48428531, {0}rx(0)=8382, {0}pm(0)=1338}
*/

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
const unsigned int DefaultCycleSleepMs = 10000;
const unsigned int DefaultHostSleepMs  = 0;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvInfoHostFilter {
public:
	RvInfoHostFilter();

	bool IsMatch(const std::string &host_name) const;

	bool ParseCmdLineHost(unsigned int &current_index, int argc, char **argv);
	bool ParseCmdLineHost(const MLB::Utility::StringVector &param_list,
		bool is_include_param, unsigned int &current_index, int argc,
		char **argv);
	bool ParseCmdLineHostExclude(unsigned int &current_index, int argc,
		char **argv);
	bool ParseCmdLineHostInclude(unsigned int &current_index, int argc,
		char **argv);

	static bool ParseCmdLineHost(unsigned int &current_index, int argc,
		char **argv, RvInfoHostFilter &out_datum);
	static bool ParseCmdLineHost(const MLB::Utility::StringVector &param_list,
		unsigned int &current_index, int argc, char **argv,
		MLB::Utility::StringSet &clude_set);
	static bool ParseCmdLineHostExclude(unsigned int &current_index, int argc,
		char **argv, RvInfoHostFilter &out_datum);
	static bool ParseCmdLineHostInclude(unsigned int &current_index, int argc,
		char **argv, RvInfoHostFilter &out_datum);

	MLB::Utility::StringSet include_set_;
	MLB::Utility::StringSet exclude_set_;

private:
	bool IsMatchInternal(const std::string &host_name) const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoHostFilter::RvInfoHostFilter()
	:include_set_()
	,exclude_set_()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::IsMatch(const std::string &host_name) const
{
	if (exclude_set_.empty() && include_set_.empty())
		return(true);

	std::string tmp_name(MLB::Utility::LowerCase(host_name));

	if (IsMatchInternal(tmp_name))
		return(true);

	std::string canon_name(MLB::Utility::ConvertHostNameToCanonical(tmp_name));

	return((canon_name != tmp_name) ? IsMatchInternal(canon_name) : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::IsMatchInternal(const std::string &host_name) const
{
	if ((!include_set_.empty()) &&
		(include_set_.find(host_name) == include_set_.end()))
		return(false);

	return(exclude_set_.find(host_name) == exclude_set_.end());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::ParseCmdLineHost(unsigned int &current_index, int argc,
	char **argv)
{
	return(ParseCmdLineHost(current_index, argc, argv, *this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::ParseCmdLineHost(
	const MLB::Utility::StringVector &param_list, bool is_include_param,
	unsigned int &current_index, int argc, char **argv)
{
	return(ParseCmdLineHost(param_list, current_index, argc, argv,
		(is_include_param) ? include_set_ : exclude_set_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::ParseCmdLineHostExclude(unsigned int &current_index,
	int argc, char **argv)
{
	return(ParseCmdLineHostExclude(current_index, argc, argv, *this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::ParseCmdLineHostInclude(unsigned int &current_index,
	int argc, char **argv)
{
	return(ParseCmdLineHostInclude(current_index, argc, argv, *this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::ParseCmdLineHost(unsigned int &current_index, int argc,
	char **argv, RvInfoHostFilter &out_datum)
{
	return(ParseCmdLineHostExclude(current_index, argc, argv, out_datum) ||
		ParseCmdLineHostInclude(current_index, argc, argv, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::ParseCmdLineHost(
	const MLB::Utility::StringVector &param_list, unsigned int &current_index,
	int argc, char **argv, MLB::Utility::StringSet &clude_set)
{
	std::string tmp_datum;

	if (!RvParseCmdLineArg::ParseCmdLineFollowingSpec(param_list, current_index,
		argc, argv, tmp_datum))
		return(false);

	try {
		MLB::Utility::StringVector host_list;
		MLB::Utility::SplitString(tmp_datum, ",", host_list, 0, false);
		MLB::Utility::StringVectorIterC iter_b(host_list.begin());
		MLB::Utility::StringVectorIterC iter_e(host_list.end());
		for ( ; iter_b != iter_e ; ++iter_b) {
			if (iter_b->empty())
				MLB::Utility::ThrowInvalidArgument("Empty host name encountered.");
			std::string name_1(MLB::Utility::LowerCase(*iter_b));
			clude_set.insert(name_1);
			std::string name_2(MLB::Utility::ConvertHostNameToCanonical(name_1));
			if (name_1 != name_2)
				clude_set.insert(name_2);
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to complete parsing of the '" +
			std::string(argv[current_index - 1]) + "' parameter: " +
			std::string(except.what()));
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::ParseCmdLineHostExclude(unsigned int &current_index,
	int argc, char **argv, RvInfoHostFilter &out_datum)
{
	return(ParseCmdLineHost(MLB::Utility::MakeInlineVector<std::string>
		("-EXCLUDE_HOST_NAME")
		("-EXCLUDE_HOSTNAME")
		("-EXCLUDEHOST_NAME")
		("-EXCLUDEHOSTNAME")

		("-EXCLUDE_HOST")
		("-EXCLUDEHOST")

		("-EXC_HOST_NAME")
		("-EXC_HOSTNAME")
		("-EXCHOST_NAME")
		("-EXCHOSTNAME")

		("-EXC_HOST")
		("-EXCHOST")

		("-EXCLUDE")

		("-EXC"),
		current_index, argc, argv, out_datum.exclude_set_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RvInfoHostFilter::ParseCmdLineHostInclude(unsigned int &current_index,
	int argc, char **argv, RvInfoHostFilter &out_datum)
{
	return(ParseCmdLineHost(MLB::Utility::MakeInlineVector<std::string>
		("-INCLUDE_HOST_NAME")
		("-INCLUDE_HOSTNAME")
		("-INCLUDEHOST_NAME")
		("-INCLUDEHOSTNAME")

		("-INCLUDE_HOST")
		("-INCLUDEHOST")

		("-INC_HOST_NAME")
		("-INC_HOSTNAME")
		("-INCHOST_NAME")
		("-INCHOSTNAME")

		("-INC_HOST")
		("-INCHOST")

		("-INCLUDE")

		("-INC"),
		current_index, argc, argv, out_datum.include_set_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef unsigned int                            RvInfo_ServiceNumber;
typedef std::set<RvInfo_ServiceNumber>          RvInfo_ServiceNumberSet;
typedef RvInfo_ServiceNumberSet::iterator       RvInfo_ServiceNumberSetIter;
typedef RvInfo_ServiceNumberSet::const_iterator RvInfo_ServiceNumberSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_RVUTILX) RvProxy : public RvException {
public:
	RvProxy() throw() :
		 RvException("Unspecified TibCo/Rendezvous proxy exception.") { }
	RvProxy(const char *except_string) throw() :
		RvException(except_string) { }
	RvProxy(const std::string &except_string) throw() :
		RvException(except_string) { }
	RvProxy(const std::ostringstream &except_string) throw() :
		RvException(except_string) { }
	~RvProxy() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		RvProxy tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_RVUTILX) RvProxyCriticalEvent :
	public RvProxy {
public:
	RvProxyCriticalEvent() throw() :
		 RvProxy("Signal or shut-down message received by process.") { }
	~RvProxyCriticalEvent() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		RvProxyCriticalEvent tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_RVUTILX) RvProxyServiceDisabled :
	public RvProxy {
public:
	RvProxyServiceDisabled() throw() :
		 RvProxy("Unspecified TibCo/Rendezvous proxy service exception.") { }
	RvProxyServiceDisabled(const char *except_string) throw() :
		RvProxy(except_string) { }
	RvProxyServiceDisabled(const std::string &except_string) throw() :
		RvProxy(except_string) { }
	RvProxyServiceDisabled(const std::ostringstream &except_string) throw() :
		RvProxy(except_string) { }
	~RvProxyServiceDisabled() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		RvProxyServiceDisabled tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_RVUTILX) RvProxyFileNotFound :
	public RvProxy {
public:
	RvProxyFileNotFound() throw() :
		 RvProxy("TibCo/Rendezvous request reulted in 404 File not Found.") { }
	RvProxyFileNotFound(const char *except_string) throw() :
		RvProxy(except_string) { }
	RvProxyFileNotFound(const std::string &except_string) throw() :
		RvProxy(except_string) { }
	RvProxyFileNotFound(const std::ostringstream &except_string) throw() :
		RvProxy(except_string) { }
	~RvProxyFileNotFound() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		RvProxyFileNotFound tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvInfoComponent {
protected:
	RvInfoComponent(const std::string &daemon_host = "localhost",
		const std::string &daemon_port = "7580") :
		 socket_lib_()
		,src_endpoint_()
	{
		SetAddressInfo(daemon_host, daemon_port);
	}
	virtual ~RvInfoComponent() 
	{
	}

	void SetAddressInfo(const std::string &host_name,
		const std::string &service_port) {
		std::string src_host(MLB::SockLibX::ResolveHostToName(host_name));
		int         src_port = MLB::SockLibX::ResolveServToPort(service_port);
		src_endpoint_.SetAddressAndPort(MLB::SockLibX::AddressIP(src_host),
			src_port);
	}

	virtual std::string GetComponentName() const = 0;

public:
	static std::string::size_type GetOffset(const std::string &search_string,
		const char *target_string, std::string::size_type start_offset = 0,
		bool required_flag = false);
	static std::string::size_type GetOffsetRequired(
		const std::string &search_string, const char *target_string,
		std::string::size_type start_offset = 0);
	static std::string            GetToString(const std::string &search_string,
		const char *target_string, std::string::size_type &start_offset);
	static std::string            GetToOpenBracket(
		const std::string &search_string, std::string::size_type &start_offset);
	static std::string            GetToCloseBracket(
		const std::string &search_string, std::string::size_type &start_offset);
	static std::string::size_type MoveToString(const std::string &search_string,
		const char *target_string, std::string::size_type start_offset,
		bool required_flag = true);
	static std::string::size_type MoveToString(const std::string &search_string,
		const std::string &target_string, std::string::size_type start_offset,
		bool required_flag = true);
	static std::string::size_type MoveToOpenBracket(
		const std::string &search_string, std::string::size_type start_offset = 0,
		bool required_flag = true);
	static std::string::size_type MoveToCloseBracket(
		const std::string &search_string, std::string::size_type start_offset = 0,
		bool required_flag = true);
	static std::string::size_type MovePastBR(const std::string &search_string,
		std::string::size_type start_offset = 0, unsigned int br_count = 1,
		bool required_flag = true);
	static std::string &GetDataBasic(
		const MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &request_string, std::string &result_string);

private:
	MLB::SockLibX::SockLibXContext socket_lib_;

protected:
	MLB::SockLibX::EndPointIP      src_endpoint_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoComponent::GetOffset(
	const std::string &search_string, const char *target_string,
	std::string::size_type start_offset, bool required_flag)
{
	std::string::size_type target_offset =
		search_string.find(target_string, start_offset);

	if ((target_offset == std::string::npos) && required_flag)
		MLB::Utility::ThrowStdException("Unable to locate the desired string ('"+
			std::string(target_string) + "') on the HTML page.");
	return(target_offset);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoComponent::GetOffsetRequired(
	const std::string &search_string, const char *target_string,
	std::string::size_type start_offset)
{
	return(GetOffset(search_string, target_string, start_offset, true));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoComponent::GetToString(const std::string &search_string,
	const char *target_string, std::string::size_type &start_offset)
{
	std::string::size_type old_offset = start_offset;

	start_offset = GetOffsetRequired(search_string, target_string,
		start_offset);

	return(search_string.substr(old_offset, start_offset - old_offset));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoComponent::GetToOpenBracket(const std::string &search_string,
	std::string::size_type &start_offset)
{
	return(GetToString(search_string, "<", start_offset));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoComponent::GetToCloseBracket(const std::string &search_string,
	std::string::size_type &start_offset)
{
	return(GetToString(search_string, ">", start_offset));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoComponent::MoveToString(
	const std::string &search_string, const char *target_string,
	std::string::size_type start_offset, bool required_flag)
{
	return(GetOffset(search_string, target_string, start_offset,
		required_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoComponent::MoveToString(
	const std::string &search_string, const std::string &target_string,
	std::string::size_type start_offset, bool required_flag)
{
	return(GetOffset(search_string, target_string.c_str(), start_offset,
		required_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoComponent::MoveToOpenBracket(
	const std::string &search_string, std::string::size_type start_offset,
	bool required_flag)
{
	return(MoveToString(search_string, "<", start_offset, required_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoComponent::MoveToCloseBracket(
	const std::string &search_string, std::string::size_type start_offset,
	bool required_flag)
{
	return(MoveToString(search_string, ">", start_offset, required_flag));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoComponent::MovePastBR(
	const std::string &search_string, std::string::size_type start_offset,
	unsigned int br_count, bool required_flag)
{
	while (br_count--) {
		start_offset = MoveToString(search_string, "<br>", start_offset,
			required_flag);
		start_offset = MoveToCloseBracket(search_string, start_offset,
			required_flag) + 1;
	}

	return(start_offset);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoComponent::GetDataBasic(
	const MLB::SockLibX::EndPointIP &src_endpoint,
	const std::string &request_string, std::string &result_string)
{
	try {
		if (!src_endpoint.GetPort())
			throw RvException("Rendezous component host and port not specified.");
		MLB::SockLibX::SocketX my_socket(src_endpoint);
		std::string            http_request("GET " + request_string +
			" HTTP/1.1\r\n"
			"Accept: */*\r\n"
			"Accept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\n"
			"User-Agent:Mozilla/4.0 (compatible; MSIE 6.0; "
				"Windows NT 5.0; .NET CLR 1.1.4322)\r\n"
			"Host: " + src_endpoint.GetHostName() + ":" +
				MLB::Utility::AnyToString(src_endpoint.GetPort()) + "\r\n"
			"Connection: Keep-Alive\r\n\r\n");
		if (MLB::Utility::CriticalEventTest())
			throw RvProxyCriticalEvent();
		my_socket.Send(http_request);
		while ((!MLB::Utility::CriticalEventTest()) &&
			my_socket.RecvAppend(result_string)) {
			MLB::Utility::SleepMilliSecs(10);
		}
		if (MLB::Utility::CriticalEventTest())
			throw RvProxyCriticalEvent();
	}
	catch (const RvProxyCriticalEvent &) {
		throw;
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to retrieve HTTP page from Tib "
			"provider using GET request string '" + request_string + "': " +
			std::string(except.what()));
	}

	return(result_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvInfoComponent_Instance : public RvInfoComponent {
protected:
	RvInfoComponent_Instance(const std::string component_name = "*UNKNOWN*") :
		 RvInfoComponent()
		,component_name_(component_name)
		,src_host_name_()
		,src_port_number_(0)
		,src_port_string_()
		,component_()
		,version_()
		,license_()
		,address_info_()
	{
	}

	std::string GetComponentName() const {
		return(component_name_);
	}

	void SetSrcInfo(const std::string &host_name,
		const std::string &service_port) {
		src_host_name_   = MLB::SockLibX::ResolveHostToName(host_name);
		src_port_number_ = MLB::SockLibX::ResolveServToPort(service_port);
		src_port_string_ = MLB::Utility::AnyToString(src_port_number_);
		SetAddressInfo(host_name, service_port);
	}

	std::string &GetData(const std::string &request_string,
		std::string &result_string) {
		return(GetDataBasic(src_endpoint_, request_string, result_string));
	}

public:
	std::string               component_name_;
	std::string               src_host_name_;
	int                       src_port_number_;
	std::string               src_port_string_;
	std::string               component_;
	std::string               version_;
	std::string               license_;
	MLB::SockLibX::EndPointIP address_info_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvInfoRvd_Transport : public RvInfoComponent {
public:
	RvInfoRvd_Transport() :
		 RvInfoComponent()
		,description_()
		,user_name_()
		,service_(0)
		,identifier_()
		,detail_id_()
	{
	}
	RvInfoRvd_Transport(const std::string &description,
		const std::string &user_name, unsigned int service,
		const std::string &identifier, const std::string &detail_id) :
		 RvInfoComponent()
		,description_(description)
		,user_name_(user_name)
		,service_(service)
		,identifier_(identifier)
		,detail_id_(detail_id)
	{
	}

	std::string                &GetDescription(std::string &out_datum) const {
return(out_datum);
	}
	std::string                 GetDescription() const {
		std::string out_datum;
		return(GetDescription(out_datum));
	}

	std::string                &GetIdentifier(std::string &out_datum) const {
return(out_datum);
	}
	std::string                 GetIdentifier() const {
		std::string out_datum;
		return(GetIdentifier(out_datum));
	}

	int                         GetService() const {
return(0);
	}

	MLB::Utility::StringVector &GetSubscriptions(
		MLB::Utility::StringVector &out_datum) const {
return(out_datum);
	}
	MLB::Utility::StringVector  GetSubscriptions() const {
		MLB::Utility::StringVector out_datum;
		return(GetSubscriptions(out_datum));
	}

	std::string                &GetUsername(std::string &out_datum) const {
return(out_datum);
	}
	std::string                 GetUsername() const {
		std::string out_datum;
		return(GetUsername(out_datum));
	}

	std::string &ToStringTabular(std::string &out_string) const;
	std::string  ToStringTabular() const;

	static std::string &ToStringTabular(
		const std::vector<RvInfoRvd_Transport> &data_list,
		std::string &out_string, unsigned int indent_count = 0);
	static std::string  ToStringTabular(
		const std::vector<RvInfoRvd_Transport> &data_list,
		unsigned int indent_count = 0);

	std::string  description_;
	std::string  user_name_;
	unsigned int service_;
	std::string  identifier_;
	std::string  detail_id_;

private:
	std::string GetComponentName() const {
		return("Client Transport");
	}
};
inline std::ostream & operator << (std::ostream &o_str,
	const RvInfoRvd_Transport &datum)
{

	std::string out_string;

	o_str <<
		datum.ToStringTabular(out_string);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_Transport::ToStringTabular(std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		identifier_ << " " <<
		std::right << std::setw(5) << service_ << std::left << " " <<
		std::right << std::setw(5) << detail_id_ << std::left << " " <<
		std::setw(16) << user_name_ << " " <<
		description_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Transport::ToStringTabular() const
{
	std::string out_string;

	return(ToStringTabular(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_Transport::ToStringTabular(
	const std::vector<RvInfoRvd_Transport> &data_list,
	std::string &out_string, unsigned int indent_count)
{
	std::vector<RvInfoRvd_Transport>::const_iterator iter_b(data_list.begin());
	std::vector<RvInfoRvd_Transport>::const_iterator iter_e(data_list.end());
	std::string                                      tmp_string_1;
	std::string                                      tmp_string_2;

	while (iter_b != iter_e) {
		tmp_string_1 += ((tmp_string_1.empty()) ? "" : "\n") +
			MLB::Utility::PadLeft("", indent_count) +
			iter_b->ToStringTabular(tmp_string_2);
		++iter_b;
	}

	return(out_string.assign(tmp_string_1));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Transport::ToStringTabular(
	const std::vector<RvInfoRvd_Transport> &data_list,
	unsigned int indent_count)
{
	std::string out_string;

	return(ToStringTabular(data_list, out_string, indent_count));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::vector<RvInfoRvd_Transport>          RvInfoRvd_TransportVector;
typedef RvInfoRvd_TransportVector::iterator       RvInfoRvd_TransportVectorIter;
typedef RvInfoRvd_TransportVector::const_iterator RvInfoRvd_TransportVectorIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvInfoRvd_Host : public RvInfoComponent {
public:
	RvInfoRvd_Host() :
		 RvInfoComponent()
		,host_name_()
		,host_name_canonical_()
		,ip_address_()
		,http_address_()
		,http_address_ip_()
		,http_port_()
		,serial_number_()
		,uptime_()
		,version_()
	{
	}
	RvInfoRvd_Host(const std::string &host_name, const std::string &ip_address,
		const std::string &http_address, const std::string &http_address_ip,
		const std::string &http_port, const std::string &serial_number,
		const std::string &uptime, const std::string &version) :
		 RvInfoComponent()
		,host_name_(FixupHostName(host_name))
		,host_name_canonical_(MLB::Utility::ConvertHostNameToCanonical(host_name))
		,ip_address_(ip_address)
		,http_address_(http_address)
		,http_address_ip_(http_address_ip)
		,http_port_(http_port)
		,serial_number_(serial_number)
		,uptime_(uptime)
		,version_(version)
	{
	}
	//	For container searches...
	RvInfoRvd_Host(const std::string &host_name_canonical) :
		 RvInfoComponent()
		,host_name_(MLB::Utility::ConvertHostNameToCanonical(host_name_canonical))
		,host_name_canonical_(
			MLB::Utility::ConvertHostNameToCanonical(host_name_canonical))
		,ip_address_()
		,http_address_()
		,http_address_ip_()
		,http_port_()
		,serial_number_()
		,uptime_()
		,version_()
	{
	}

	bool operator < (const RvInfoRvd_Host &other) const {
		return(host_name_canonical_ < other.host_name_canonical_);
	}

	void SetHostName(const std::string &in_host_name) {
		std::string tmp_name(FixupHostName(in_host_name));
		host_name_           = tmp_name;
		host_name_canonical_ = MLB::Utility::ConvertHostNameToCanonical(tmp_name);
	}

	std::string &ToStringTabular(std::string &out_string) const;
	std::string  ToStringTabular() const;

	static std::string &ToStringTabular(
		const std::vector<RvInfoRvd_Host> &data_list, std::string &out_string,
		unsigned int indent_count = 0);
	static std::string  ToStringTabular(
		const std::vector<RvInfoRvd_Host> &data_list,
		unsigned int indent_count = 0);

	static RvInfoRvd_Host &ExtractInfo(std::string &result_string,
		std::string::size_type &my_offset, RvInfoRvd_Host &out_datum);
	static RvInfoRvd_Host  ExtractInfo(std::string &result_string,
		std::string::size_type &my_offset);

	std::string host_name_;
	std::string host_name_canonical_;
	std::string ip_address_;
	std::string http_address_;
	std::string http_address_ip_;
	std::string http_port_;
	std::string serial_number_;
	std::string uptime_;
	std::string version_;

private:
	std::string GetComponentName() const {
		return("Host");
	}
	static std::string FixupHostName(const std::string &in_host_name) {
		try {
			std::string out_host_name;
			MLB::SockLibX::ResolveHostToName(in_host_name, out_host_name);
			return(out_host_name);
		}
		catch (...) {
		}
		return(in_host_name);
	}
};
inline std::ostream & operator << (std::ostream &o_str,
	const RvInfoRvd_Host &datum)
{

	std::string out_string;

	o_str <<
		datum.ToStringTabular(out_string);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::vector<RvInfoRvd_Host>          RvInfoRvd_HostVector;
typedef RvInfoRvd_HostVector::iterator       RvInfoRvd_HostVectorIter;
typedef RvInfoRvd_HostVector::const_iterator RvInfoRvd_HostVectorIterC;

typedef std::set<RvInfoRvd_Host>             RvInfoRvd_HostSet;
typedef RvInfoRvd_HostSet::iterator          RvInfoRvd_HostSetIter;
typedef RvInfoRvd_HostSet::const_iterator    RvInfoRvd_HostSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_Host::ToStringTabular(std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		std::left << std::setw(10) << serial_number_ <<
			std::right << " " <<
		std::right << std::setw(12) << uptime_ << std::left << " " <<
		std::right << std::setw(6) << version_ << std::left << " " <<
		std::setw(15) << ip_address_ << " " <<
		std::setw(5) << http_port_ << " " <<
		host_name_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Host::ToStringTabular() const
{
	std::string out_string;

	return(ToStringTabular(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_Host::ToStringTabular(
	const std::vector<RvInfoRvd_Host> &data_list, std::string &out_string,
	unsigned int indent_count)
{
	std::vector<RvInfoRvd_Host>::const_iterator iter_b(data_list.begin());
	std::vector<RvInfoRvd_Host>::const_iterator iter_e(data_list.end());
	std::string                                 tmp_string_1;
	std::string                                 tmp_string_2;

	while (iter_b != iter_e) {
		tmp_string_1 += ((tmp_string_1.empty()) ? "" : "\n") +
			MLB::Utility::PadLeft("", indent_count) +
			iter_b->ToStringTabular(tmp_string_2);
		++iter_b;
	}

	return(out_string.assign(tmp_string_1));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Host::ToStringTabular(
	const std::vector<RvInfoRvd_Host> &data_list, unsigned int indent_count)
{
	std::string out_string;

	return(ToStringTabular(data_list, out_string, indent_count));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Requires positioning at <tr> which begins a host table row.
RvInfoRvd_Host &RvInfoRvd_Host::ExtractInfo(std::string &result_string,
	std::string::size_type &my_offset, RvInfoRvd_Host &out_datum)
{
	if (result_string.find("<tr>", my_offset) != my_offset)
		MLB::Utility::ThrowInvalidArgument("Attempt to extract host "
			"information failed because of the current positioning.");

	RvInfoRvd_Host tmp_datum;

	//	Get the hostname-based http address...
	my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
	my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
	if (result_string.find("<b>No information available</b>", my_offset) ==
		my_offset)
		MLB::Utility::ThrowInvalidArgument("Attempt to extract host "
			"information from failed because there are no hosts for the "
			"specified service.");
	my_offset = MoveToString(result_string, "=http:", my_offset) + 1;
	tmp_datum.http_address_ = GetToCloseBracket(result_string, my_offset);

	//	Find the location of the HTTP port number in the link...
	std::string::size_type tmp_offset = 0;
	tmp_offset = MoveToString(tmp_datum.http_address_, "http://", tmp_offset) + 7;
	tmp_offset = MoveToString(tmp_datum.http_address_, ":", tmp_offset) + 1;
	tmp_datum.http_port_ = GetToString(tmp_datum.http_address_, "/", tmp_offset);

	//	Get the host name...
	++my_offset;
	tmp_datum.SetHostName(GetToOpenBracket(result_string, my_offset));

	//	Get the IP-address-based http address...
	my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
	my_offset = MoveToString(result_string, "=http:", my_offset) + 1;
	tmp_datum.http_address_ip_ = GetToCloseBracket(result_string, my_offset);

	//	Get the IP address...
	++my_offset;
	tmp_datum.ip_address_ = GetToOpenBracket(result_string, my_offset);

	//	Get the version number...
	my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
	my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
	tmp_datum.version_ = GetToOpenBracket(result_string, my_offset);

	//	Get the serial number...
	my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
	my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
	tmp_datum.serial_number_ = GetToOpenBracket(result_string, my_offset);

	//	Get the uptime string...
	my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
	my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
	tmp_datum.uptime_ = GetToOpenBracket(result_string, my_offset);

	out_datum = tmp_datum;

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_Host RvInfoRvd_Host::ExtractInfo(std::string &result_string,
	std::string::size_type &my_offset)
{
	RvInfoRvd_Host out_datum;

	return(ExtractInfo(result_string, my_offset, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef long double        FloatingQuantity;
typedef unsigned long long UnsignedQuantity;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const unsigned int UnsignedQuantityWidth =
	MLB::Utility::IntegralValueMaxLengthDecUnsigned<UnsignedQuantity>();
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvInfoRvd_Rates {
	RvInfoRvd_Rates() :
		 msgs_(0.0)
		,bytes_(0.0)
		,pkts_(0.0)
	{
	}
	~RvInfoRvd_Rates()
	{
	}

	RvInfoRvd_Rates &GetRatesInbound(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service) {
		return(GetInboundRates(src_endpoint, rv_service, *this));
	}
	RvInfoRvd_Rates &GetRatesOutbound(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service) {
		return(GetOutboundRates(src_endpoint, rv_service, *this));
	}

	RvInfoRvd_Rates &SetRatesInbound(std::string &result_string) {
		return(ExtractInboundRates(result_string, *this));
	}
	RvInfoRvd_Rates &SetRatesOutbound(std::string &result_string) {
		return(ExtractOutboundRates(result_string, *this));
	}

	std::string &ToStringTabular(std::string &out_string) const;
	std::string  ToStringTabular() const;

	static void GetAllRates(std::string &result_string,
		RvInfoRvd_Rates &rates_inbound, RvInfoRvd_Rates &rates_outbound);

	static void GetAllRates(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service, RvInfoRvd_Rates &rates_inbound,
		RvInfoRvd_Rates &rates_outbound) {
		std::string result_string;
		RvInfoComponent::GetDataBasic(src_endpoint,
			"/service_detail?" + rv_service, result_string);
		GetAllRates(result_string, rates_inbound, rates_outbound);
	}

	static RvInfoRvd_Rates &ExtractInboundRates(std::string &result_string,
		RvInfoRvd_Rates &out_datum) {
		RvInfoRvd_Rates other_datum;
		GetAllRates(result_string, out_datum, other_datum);
		return(out_datum);
	}
	static RvInfoRvd_Rates  ExtractInboundRates(std::string &result_string) {
		RvInfoRvd_Rates out_datum;
		return(ExtractInboundRates(result_string, out_datum));
	}
	static RvInfoRvd_Rates &ExtractOutboundRates(std::string &result_string,
		RvInfoRvd_Rates &out_datum) {
		RvInfoRvd_Rates other_datum;
		GetAllRates(result_string, other_datum, out_datum);
		return(out_datum);
	}
	static RvInfoRvd_Rates  ExtractOutboundRates(std::string &result_string) {
		RvInfoRvd_Rates out_datum;
		return(ExtractOutboundRates(result_string, out_datum));
	}

	static RvInfoRvd_Rates &GetInboundRates(
		MLB::SockLibX::EndPointIP &src_endpoint, const std::string &rv_service,
		RvInfoRvd_Rates &out_datum) {
		RvInfoRvd_Rates other_datum;
		GetAllRates(src_endpoint, rv_service, out_datum, other_datum);
		return(out_datum);
	}
	static RvInfoRvd_Rates &GetOutboundRates(
		MLB::SockLibX::EndPointIP &src_endpoint, const std::string &rv_service,
		RvInfoRvd_Rates &out_datum) {
		RvInfoRvd_Rates other_datum;
		GetAllRates(src_endpoint, rv_service, other_datum, out_datum);
		return(out_datum);
	}

	FloatingQuantity msgs_;
	FloatingQuantity bytes_;
	FloatingQuantity pkts_;
};
inline std::ostream & operator << (std::ostream &o_str,
	const RvInfoRvd_Rates &datum)
{
	std::string out_string;

	o_str <<
		datum.ToStringTabular(out_string);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_Rates::ToStringTabular(std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		std::setw(20) << msgs_  << " " <<
		std::setw(20) << bytes_ << " " <<
		std::setw(20) << pkts_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Rates::ToStringTabular() const
{
	std::string out_string;

	return(ToStringTabular(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvInfoRvd_Rates::GetAllRates(std::string &result_string,
	RvInfoRvd_Rates &rates_inbound, RvInfoRvd_Rates &rates_outbound)
{
	std::string::size_type my_offset = 0;

	my_offset = RvInfoComponent::MoveToString(result_string,
		"<b>Inbound Rates (per second)</b>", my_offset);

	//	Move to the end of the first row ({in|out}bound rates)...
	my_offset = RvInfoComponent::MoveToString(result_string,
		"</tr>", my_offset) + 1;

	//	Move to the end of the second row (rates metric per second)...
	my_offset = RvInfoComponent::MoveToString(result_string,
		"</tr>", my_offset) + 1;

	//	Move to the start of the rates data row...
	my_offset = RvInfoComponent::MoveToString(result_string,
		"<tr>", my_offset) + 4;
		
	unsigned int     count_1 = 0;
	FloatingQuantity rates_list[6];

	for (count_1 = 0; count_1 < 6; ++count_1) {
		my_offset = RvInfoComponent::MoveToString(result_string,
			"<font face=", my_offset) + 1;
		my_offset = RvInfoComponent::MoveToCloseBracket(result_string,
			my_offset) + 1;
		rates_list[count_1] =
			atof(RvInfoComponent::GetToOpenBracket(result_string, my_offset).
			c_str());
	}

	rates_inbound.msgs_   = rates_list[0];
	rates_inbound.bytes_  = rates_list[1];
	rates_inbound.pkts_   = rates_list[2];
	rates_outbound.msgs_  = rates_list[3];
	rates_outbound.bytes_ = rates_list[4];
	rates_outbound.pkts_  = rates_list[5];
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvInfoRvd_RatesInbound : public RvInfoRvd_Rates {
	RvInfoRvd_RatesInbound() :
		RvInfoRvd_Rates()
	{
	}

	RvInfoRvd_Rates &GetRates(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service) {
		return(GetInboundRates(src_endpoint, rv_service, *this));
	}

	RvInfoRvd_Rates &SetRates(std::string &result_string) {
		return(ExtractInboundRates(result_string, *this));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvInfoRvd_RatesOutbound : public RvInfoRvd_Rates {
	RvInfoRvd_RatesOutbound() :
		RvInfoRvd_Rates()
	{
	}

	RvInfoRvd_Rates &GetRates(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service) {
		return(GetOutboundRates(src_endpoint, rv_service, *this));
	}

	RvInfoRvd_Rates &SetRates(std::string &result_string) {
		return(ExtractOutboundRates(result_string, *this));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvInfoRvd_Totals {
	RvInfoRvd_Totals() :
		 msgs_(0)
		,bytes_(0)
		,pkts_(0)
	{
	}
	virtual ~RvInfoRvd_Totals()
	{
	}

	void Clear() {
		msgs_  = 0;
		bytes_ = 0;
		pkts_  = 0;
	}

	RvInfoRvd_Totals & operator += (const RvInfoRvd_Totals &rhs) {
		msgs_  += rhs.msgs_;
		bytes_ += rhs.bytes_;
		pkts_  += rhs.pkts_;
		return(*this);
	}

	std::string &ToStringTabular(std::string &out_string) const;
	std::string  ToStringTabular() const;

	UnsignedQuantity msgs_;
	UnsignedQuantity bytes_;
	UnsignedQuantity pkts_;

protected:
	static RvInfoRvd_Totals &ExtractInboundTotalsRaw(std::string &result_string,
		RvInfoRvd_Totals &out_datum, UnsignedQuantity &other_1,
		UnsignedQuantity &other_2, UnsignedQuantity &other_3);
	static RvInfoRvd_Totals &ExtractOutboundTotalsRaw(std::string &result_string,
		RvInfoRvd_Totals &out_datum, UnsignedQuantity &other_1,
		UnsignedQuantity &other_2, UnsignedQuantity &other_3);
	static RvInfoRvd_Totals &ExtractTotalsRaw(std::string &result_string,
		std::string::size_type my_offset, RvInfoRvd_Totals &out_datum,
		UnsignedQuantity &other_1, UnsignedQuantity &other_2,
		UnsignedQuantity &other_3);
	static void GetInboundTotalsRaw(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service, RvInfoRvd_Totals &out_datum,
		UnsignedQuantity &other_1, UnsignedQuantity &other_2,
		UnsignedQuantity &other_3);
	static void GetOutboundTotalsRaw(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service, RvInfoRvd_Totals &out_datum,
		UnsignedQuantity &other_1, UnsignedQuantity &other_2,
		UnsignedQuantity &other_3);

};
inline std::ostream & operator << (std::ostream &o_str,
	const RvInfoRvd_Totals &datum)
{
	std::string out_string;

	o_str <<
		datum.ToStringTabular(out_string);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_Totals::ToStringTabular(std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << msgs_ << " " <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << bytes_ << " " <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << pkts_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Totals::ToStringTabular() const
{
	std::string out_string;

	return(ToStringTabular(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_Totals &RvInfoRvd_Totals::ExtractInboundTotalsRaw(
	std::string &result_string, RvInfoRvd_Totals &out_datum,
	UnsignedQuantity &other_1, UnsignedQuantity &other_2,
	UnsignedQuantity &other_3)
{
	std::string::size_type my_offset = 0;

	my_offset = RvInfoComponent::MoveToString(result_string,
		"<b>Inbound Totals</b>", my_offset);

	return(ExtractTotalsRaw(result_string, my_offset, out_datum, other_1,
		other_2, other_3));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_Totals &RvInfoRvd_Totals::ExtractOutboundTotalsRaw(
	std::string &result_string, RvInfoRvd_Totals &out_datum,
	UnsignedQuantity &other_1, UnsignedQuantity &other_2,
	UnsignedQuantity &other_3)
{
	std::string::size_type my_offset = 0;

	my_offset = RvInfoComponent::MoveToString(result_string,
		"<b>Outbound Totals</b>", my_offset);

	return(ExtractTotalsRaw(result_string, my_offset, out_datum, other_1,
		other_2, other_3));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Must be positioned at <b>{In|Out}bound Totals</b>...
RvInfoRvd_Totals &RvInfoRvd_Totals::ExtractTotalsRaw(std::string &result_string,
	std::string::size_type my_offset, RvInfoRvd_Totals &out_datum,
	UnsignedQuantity &other_1, UnsignedQuantity &other_2,
	UnsignedQuantity &other_3)
{
	if ((result_string.find("<b>Inbound Totals</b>", my_offset) != my_offset) &&
		 (result_string.find("<b>Outbound Totals</b>", my_offset) != my_offset))
		MLB::Utility::ThrowInvalidArgument("Attempt to extract in/out- "
			"bound totals information failed because of the current "
			"positioning.");

	//	Move to the end of the first row ({in|out}bound totals)...
	my_offset =
		RvInfoComponent::MoveToString(result_string, "</tr>", my_offset) + 1;

	//	Move to the end of the second row (totals counts)...
	my_offset =
		RvInfoComponent::MoveToString(result_string, "</tr>", my_offset) + 1;

	//	Move to the start of the rates data row...
	my_offset =
		RvInfoComponent::MoveToString(result_string, "<tr>", my_offset) + 4;

	unsigned int     count_1 = 0;
	UnsignedQuantity rates_list[6];

	for (count_1 = 0; count_1 < 6; ++count_1) {
		my_offset = RvInfoComponent::MoveToString(result_string,
			"<font face=", my_offset) + 1;
		my_offset = RvInfoComponent::MoveToCloseBracket(result_string,
			my_offset) + 1;
		rates_list[count_1] = static_cast<UnsignedQuantity>(
			atof(RvInfoComponent::GetToOpenBracket(result_string, my_offset).
			c_str()));
	}

	out_datum.msgs_  = rates_list[0];
	out_datum.bytes_ = rates_list[1];
	out_datum.pkts_  = rates_list[2];
	other_1          = rates_list[3];
	other_2          = rates_list[4];
	other_3          = rates_list[5];

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvInfoRvd_Totals::GetInboundTotalsRaw(
	MLB::SockLibX::EndPointIP &src_endpoint, const std::string &rv_service,
	RvInfoRvd_Totals &out_datum, UnsignedQuantity &other_1,
	UnsignedQuantity &other_2, UnsignedQuantity &other_3)
{
	std::string result_string;

	RvInfoComponent::GetDataBasic(src_endpoint,
		"/service_detail?" + rv_service, result_string);
	ExtractInboundTotalsRaw(result_string, out_datum, other_1, other_2,
		other_3);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvInfoRvd_Totals::GetOutboundTotalsRaw(
	MLB::SockLibX::EndPointIP &src_endpoint, const std::string &rv_service,
	RvInfoRvd_Totals &out_datum, UnsignedQuantity &other_1,
	UnsignedQuantity &other_2, UnsignedQuantity &other_3)
{
	std::string result_string;

	RvInfoComponent::GetDataBasic(src_endpoint,
		"/service_detail?" + rv_service, result_string);
	ExtractOutboundTotalsRaw(result_string, out_datum, other_1, other_2,
		other_3);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvInfoRvd_TotalsInbound : public RvInfoRvd_Totals {
	RvInfoRvd_TotalsInbound() :
		 RvInfoRvd_Totals()
		,missed_(0)
		,lostMc_(0)
		,lostPtP_(0)
	{
	}

	RvInfoRvd_TotalsInbound & operator += (const RvInfoRvd_TotalsInbound &rhs) {
		this->RvInfoRvd_Totals::operator+=(rhs);
		missed_  += rhs.missed_;
		lostMc_  += rhs.lostMc_;
		lostPtP_ += rhs.lostPtP_;
		return(*this);
	}

	RvInfoRvd_TotalsInbound &GetTotals(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service) {
		GetInboundTotalsRaw(src_endpoint, rv_service, *this,
			missed_, lostMc_, lostPtP_);
		return(*this);
	}

	RvInfoRvd_TotalsInbound &SetTotals(std::string &result_string) {
		ExtractInboundTotalsRaw(result_string, *this, missed_, lostMc_,
			lostPtP_);
		return(*this);
	}

	void Clear() {
		this->RvInfoRvd_Totals::Clear();
		missed_  = 0;
		lostMc_  = 0;
		lostPtP_ = 0;
	}

	int CompareProblemStats(const RvInfoRvd_TotalsInbound &other) const {
		if (missed_ < other.missed_)
			return(-1);
		else if (missed_ == other.missed_) {
			if (lostMc_ < other.lostMc_)
				return(-1);
			else if (lostMc_ == other.lostMc_) {
				if (lostPtP_ < other.lostPtP_)
					return(-1);
				else if (lostPtP_ == other.lostPtP_)
					return(0);
			}
		}
		return(1);
	}

	std::string &ToStringTabular(std::string &out_string) const;
	std::string  ToStringTabular() const;

	UnsignedQuantity missed_;
	UnsignedQuantity lostMc_;
	UnsignedQuantity lostPtP_;
};
inline std::ostream & operator << (std::ostream &o_str,
	const RvInfoRvd_TotalsInbound &datum)
{
	std::string out_string;

	o_str <<
		datum.ToStringTabular(out_string);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_TotalsInbound::ToStringTabular(
	std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		RvInfoRvd_Totals::ToStringTabular() << " " <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << missed_ << " " <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << lostMc_ << " " <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << lostPtP_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_TotalsInbound::ToStringTabular() const
{
	std::string out_string;

	return(ToStringTabular(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvInfoRvd_TotalsOutbound : public RvInfoRvd_Totals {
	RvInfoRvd_TotalsOutbound() :
		 RvInfoRvd_Totals()
		,retrans_(0)
		,rejects_(0)
		,hostDown_(0)
	{
	}

	RvInfoRvd_TotalsOutbound & operator += (const RvInfoRvd_TotalsOutbound &rhs) {
		this->RvInfoRvd_Totals::operator+=(rhs);
		retrans_  += rhs.retrans_;
		rejects_  += rhs.rejects_;
		hostDown_ += rhs.hostDown_;
		return(*this);
	}

	RvInfoRvd_TotalsOutbound &GetTotals(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service) {
		GetOutboundTotalsRaw(src_endpoint, rv_service, *this,
			retrans_, rejects_, hostDown_);
		return(*this);
	}

	RvInfoRvd_TotalsOutbound &SetTotals(std::string &result_string) {
		ExtractOutboundTotalsRaw(result_string, *this, retrans_, rejects_,
			hostDown_);
		return(*this);
	}

	void Clear() {
		this->RvInfoRvd_Totals::Clear();
		retrans_  = 0;
		rejects_  = 0;
		hostDown_ = 0;
	}

	std::string &ToStringTabular(std::string &out_string) const;
	std::string  ToStringTabular() const;

	int CompareProblemStats(const RvInfoRvd_TotalsOutbound &other) const {
		if (retrans_ < other.retrans_)
			return(-1);
		else if (retrans_ == other.retrans_) {
			if (rejects_ < other.rejects_)
				return(-1);
			else if (rejects_ == other.rejects_) {
				if (hostDown_ < other.hostDown_)
					return(-1);
				else if (hostDown_ == other.hostDown_)
					return(0);
			}
		}
		return(1);
	}

	UnsignedQuantity retrans_;
	UnsignedQuantity rejects_;
	UnsignedQuantity hostDown_;
};
inline std::ostream & operator << (std::ostream &o_str,
	const RvInfoRvd_TotalsOutbound &datum)
{
	std::string out_string;

	o_str <<
		datum.ToStringTabular(out_string);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_TotalsOutbound::ToStringTabular(
	std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		RvInfoRvd_Totals::ToStringTabular() << " " <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << retrans_ << " " <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << rejects_ << " " <<
		std::setw(MLB::Utility::IntegralValueMaxLengthDecUnsigned
			<UnsignedQuantity>()) << hostDown_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_TotalsOutbound::ToStringTabular() const
{
	std::string out_string;

	return(ToStringTabular(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct RvInfoRvd_TotalsBoth {
	RvInfoRvd_TotalsBoth() :
		 totals_inbound_()
		,totals_outbound_()
	{
	}
	RvInfoRvd_TotalsBoth(const RvInfoRvd_TotalsInbound &totals_inbound,
		const RvInfoRvd_TotalsOutbound &totals_outbound) :
		 totals_inbound_(totals_inbound)
		,totals_outbound_(totals_outbound)
	{
	}

	void Clear() {
		totals_inbound_.Clear();
		totals_outbound_.Clear();
	}

	RvInfoRvd_TotalsBoth & operator += (const RvInfoRvd_TotalsBoth &rhs) {
		totals_inbound_  += rhs.totals_inbound_;
		totals_outbound_ += rhs.totals_outbound_;
		return(*this);
	}

	RvInfoRvd_TotalsInbound  totals_inbound_;
	RvInfoRvd_TotalsOutbound totals_outbound_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvInfoRvd_Service : public RvInfoComponent {
public:
	RvInfoRvd_Service() :
		 RvInfoComponent()
		,service_address_()
		,service_id_(0)
		,service_id_string_()
		,service_id_neg_(0)
		,service_id_string_neg_()
		,rv_service_()
		,rv_network_()
		,is_multicast_flag_(false)
		,reliability_(0)
		,creation_time_(0, 0)
		,rates_inbound_()
		,rates_outbound_()
		,totals_inbound_()
		,totals_outbound_()
	{
	}

	bool operator < (const RvInfoRvd_Service &other) const {
		return(service_address_ < other.service_address_);
	}

	unsigned int GetClientCount() const {
		return(atoi(GetValueAtCountBR(4).c_str()));
	}

	RvInfoRvd_TransportVector &GetTransports(
		RvInfoRvd_TransportVector &out_datum) const;
	RvInfoRvd_TransportVector  GetTransports() const;

	unsigned int GetHostCount() const {
		return(atoi(GetValueAtCountBR(5).c_str()));
	}

	RvInfoRvd_HostVector &GetHosts(const RvInfoHostFilter &host_filter,
		RvInfoRvd_HostVector &out_datum) const;
	RvInfoRvd_HostVector  GetHosts(const RvInfoHostFilter &host_filter) const;
	RvInfoRvd_HostVector &GetHosts(RvInfoRvd_HostVector &out_datum) const;
	RvInfoRvd_HostVector  GetHosts() const;

	RvInfoRvd_Host        GetHostForSelf() const {
		RvInfoRvd_Host out_datum;
		return(GetHostForSelf(out_datum));
	}
	RvInfoRvd_Host       &GetHostForSelf(RvInfoRvd_Host &out_datum) const {
		return(out_datum);
	}

	RvInfoRvd_RatesInbound  &GetInboundRates(
		RvInfoRvd_RatesInbound &out_datum) const {
		return(out_datum);
	}
	RvInfoRvd_RatesInbound   GetInboundRates() const {
		RvInfoRvd_RatesInbound out_datum;
		return(GetInboundRates(out_datum));
	}

	RvInfoRvd_TotalsInbound &GetInboundTotals(
		RvInfoRvd_TotalsInbound &out_datum) const {
		return(out_datum);
	}
	RvInfoRvd_TotalsInbound  GetInboundTotals() const {
		RvInfoRvd_TotalsInbound out_datum;
		return(GetInboundTotals(out_datum));
	}

//	GetNetwork();

	RvInfoRvd_RatesOutbound  &GetOutboundRates(
		RvInfoRvd_RatesOutbound &out_datum) const {
		return(out_datum);
	}
	RvInfoRvd_RatesOutbound   GetOutboundRates() const {
		RvInfoRvd_RatesOutbound out_datum;
		return(GetOutboundRates(out_datum));
	}

	RvInfoRvd_TotalsOutbound &GetOutboundTotals(
		RvInfoRvd_TotalsOutbound &out_datum) const {
		return(out_datum);
	}
	RvInfoRvd_TotalsOutbound  GetOutboundTotals() const {
		RvInfoRvd_TotalsOutbound out_datum;
		return(GetOutboundTotals(out_datum));
	}

//	GetPortNumber();

	MLB::Utility::StringVector &GetSubscriptions(
		MLB::Utility::StringVector &out_datum) const;
	MLB::Utility::StringVector  GetSubscriptions() const;

	RvInfoRvd_Service &GetServiceInfo(MLB::SockLibX::EndPointIP &src_endpoint,
		const std::string &rv_service);
	RvInfoRvd_Service &GetServiceInfo(MLB::SockLibX::EndPointIP &src_endpoint,
		RvInfo_ServiceNumber rv_service);

	std::string &ToStringTabular(std::string &out_string) const;
	std::string  ToStringTabular() const;

	static std::string &ToStringTabular(
		const std::set<RvInfoRvd_Service> &data_list, std::string &out_string,
		unsigned int indent_count = 0);
	static std::string  ToStringTabular(
		const std::set<RvInfoRvd_Service> &data_list,
		unsigned int indent_count = 0);

	static RvInfoRvd_Service &CreateService(const std::string &http_host,
		const std::string &http_port, const std::string &service,
		RvInfoRvd_Service &service_info);
	static RvInfoRvd_Service  CreateService(const std::string &http_host,
		const std::string &http_port, const std::string &service);

	MLB::SockLibX::EndPointIP service_address_;
	unsigned int              service_id_;
	std::string               service_id_string_;
	int                       service_id_neg_;
	std::string               service_id_string_neg_;
	std::string               rv_service_;
	std::string               rv_network_;
	bool                      is_multicast_flag_;
	unsigned int              reliability_;
	MLB::Utility::TimeVal     creation_time_;
	RvInfoRvd_RatesInbound    rates_inbound_;
	RvInfoRvd_RatesOutbound   rates_outbound_;
	RvInfoRvd_TotalsInbound   totals_inbound_;
	RvInfoRvd_TotalsOutbound  totals_outbound_;

private:
	std::string GetComponentName() const {
		return("Service Details");
	}

	std::string::size_type MoveToServiceInfoTextArea(
		const MLB::SockLibX::EndPointIP &src_endpoint,
		std::string &result_string) const;
	std::string::size_type MoveToServiceInfoDataArea(
		const MLB::SockLibX::EndPointIP &src_endpoint,
		std::string &result_string) const;
	std::string GetValueAtCountBR(unsigned int br_count) const;

};
inline std::ostream & operator << (std::ostream &o_str,
	const RvInfoRvd_Service &datum)
{
	std::string out_string;

	o_str <<
		datum.ToStringTabular(out_string);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<RvInfoRvd_Service>          RvInfoRvd_ServiceSet;
typedef RvInfoRvd_ServiceSet::iterator       RvInfoRvd_ServiceSetIter;
typedef RvInfoRvd_ServiceSet::const_iterator RvInfoRvd_ServiceSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_TransportVector &RvInfoRvd_Service::GetTransports(
	RvInfoRvd_TransportVector &out_datum) const
{
	std::string result_string;

	GetDataBasic(src_endpoint_, "/clients?" + service_id_string_, result_string);

	std::string::size_type my_offset = 0;

	try {
		my_offset = MoveToString(result_string,
			"<b>Client List </b>[service " + service_id_string_neg_ + "]",
			my_offset, true) + 1;
	}
	catch (const std::exception &) {
		if (GetOffset(result_string, "<center>Error 404: File Not Found", 0,
			false) != std::string::npos)
			throw RvProxyFileNotFound("Attempt to get the list of clients for "
				"service " + rv_service_ + " from Rendezvous HTTP provider " +
				src_endpoint_.ToString() + " resulted in HTTP error 404 (File " +
				"not Found).");
		throw;
	}


	RvInfoRvd_TransportVector tmp_datum;

	while (my_offset < result_string.size()) {
		my_offset = MoveToString(result_string, "<tr>", my_offset, false);
		if (my_offset == std::string::npos)
			break;
		++my_offset;
		my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
		std::string tmp_descrip(GetToOpenBracket(result_string, my_offset));
		if (tmp_descrip.empty()) {
			if (result_string.find("<b>No information available</b>",
				my_offset) == my_offset)
				break;
		}
		my_offset = MoveToString(result_string, "</td>", my_offset) + 5;
		my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
		std::string tmp_user(GetToOpenBracket(result_string, my_offset));
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;

		my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
		std::string tmp_service(GetToOpenBracket(result_string, my_offset));
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;

		my_offset = MoveToString(result_string, "><a href=/client_detail?" +
			rv_service_ + ":", my_offset) + 1;
		my_offset = MoveToString(result_string, ":", my_offset) + 1;
		std::string tmp_detail_id(GetToCloseBracket(result_string, my_offset));
		++my_offset;
		std::string tmp_trans_id(GetToOpenBracket(result_string, my_offset));
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;

		my_offset = MoveToString(result_string, "</tr>", my_offset) + 5;
		tmp_datum.push_back(RvInfoRvd_Transport(tmp_descrip,
			tmp_user, atoi(tmp_service.c_str()), tmp_trans_id, tmp_detail_id));
	}

	out_datum.swap(tmp_datum);

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_TransportVector RvInfoRvd_Service::GetTransports() const
{
	RvInfoRvd_TransportVector out_datum;

	return(GetTransports(out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_HostVector &RvInfoRvd_Service::GetHosts(
	const RvInfoHostFilter &host_filter, RvInfoRvd_HostVector &out_datum) const
{
	std::string result_string;

	GetDataBasic(src_endpoint_, "/hosts?" + service_id_string_, result_string);

	std::string::size_type my_offset = 0;

	try {
		my_offset = MoveToString(result_string,
			"<b>Host List </b>[service " + rv_service_ + "]",
			my_offset, true) + 1;
	}
	catch (const std::exception &) {
		if (GetOffset(result_string, "<center>Error 404: File Not Found", 0,
			false) != std::string::npos)
			throw RvProxyFileNotFound("Attempt to get the list of hosts for "
				"service " + rv_service_ + " from Rendezvous HTTP provider " +
				src_endpoint_.ToString() + " resulted in HTTP error 404 (File " +
				"not Found).");
		throw;
	}

	RvInfoRvd_HostVector tmp_datum;

	while (my_offset < result_string.size()) {
		my_offset = MoveToString(result_string, "<tr>", my_offset, false);
		if (my_offset == std::string::npos)
			break;
		std::string::size_type start_offset = my_offset;
		//	Look ahead to see determine whether any data's available...
		++my_offset;
		my_offset = MoveToString(result_string, "<font face=", my_offset) + 1;
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
		if (result_string.find("<b>No information available</b>",
			my_offset) == my_offset)
			break;
		//	Extract this host...
		RvInfoRvd_Host tmp_host;
		RvInfoRvd_Host::ExtractInfo(result_string, start_offset, tmp_host);
//std::string::size_type test_pos = result_string.find("lndvs108.ln.rbccm.com", start_offset);
		if (host_filter.IsMatch(tmp_host.host_name_))
{
			tmp_datum.push_back(tmp_host);
//std::cout << "********** INCLUDED HOST: [" << tmp_host << "]" << std::endl;
}
		//	Move just beyond the current row...
		my_offset = MoveToString(result_string, "</tr>", my_offset) + 5;
	}

	out_datum.swap(tmp_datum);

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_HostVector RvInfoRvd_Service::GetHosts(
	const RvInfoHostFilter &host_filter) const
{
	RvInfoRvd_HostVector out_datum;

	return(GetHosts(host_filter, out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_HostVector &RvInfoRvd_Service::GetHosts(
	RvInfoRvd_HostVector &out_datum) const
{
	return(GetHosts(RvInfoHostFilter(), out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_HostVector RvInfoRvd_Service::GetHosts() const
{
	RvInfoRvd_HostVector out_datum;

	return(GetHosts(out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector &RvInfoRvd_Service::GetSubscriptions(
	MLB::Utility::StringVector &out_datum) const
{
	std::string result_string;

	GetDataBasic(src_endpoint_, "/subs?" + service_id_string_,
		result_string);

	std::string::size_type my_offset = 0;

	try {
		my_offset = MoveToString(result_string,
			"<b>Subscription List </b>[service " + service_id_string_neg_ + "]",
			my_offset, true) + 1;
	}
	catch (const std::exception &) {
		if (GetOffset(result_string, "<center>Error 404: File Not Found", 0,
			false) != std::string::npos)
			throw RvProxyFileNotFound("Attempt to get the list of subscriptions "
				"for service " + rv_service_ + " from Rendezvous HTTP provider " +
				src_endpoint_.ToString() + " resulted in HTTP error 404 (File " +
				"not Found).");
		throw;
	}

	unsigned int               current_count = 1;
	MLB::Utility::StringVector tmp_datum;

	while (current_count <= 9999) {
		my_offset = MoveToString(result_string, "<b>" +
			MLB::Utility::AnyToString(current_count++) + ".&nbsp;",
			my_offset, false);
		if (my_offset == std::string::npos)
			break;
		++my_offset;
		my_offset = MoveToString(result_string, "</b>", my_offset) + 1;
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
		std::string this_subject(GetToOpenBracket(result_string, my_offset));
		tmp_datum.push_back(this_subject);
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
	}

	out_datum.swap(tmp_datum);

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector RvInfoRvd_Service::GetSubscriptions() const
{
	MLB::Utility::StringVector out_datum;

	return(GetSubscriptions(out_datum));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_Service &RvInfoRvd_Service::GetServiceInfo(
	MLB::SockLibX::EndPointIP &src_endpoint, const std::string &rv_service)
{
	rv_service_ = rv_service;

	std::string            result_string;
	std::string::size_type my_offset = MoveToServiceInfoTextArea(src_endpoint,
		result_string);

	my_offset = MoveToString(result_string, "<a href=/subs?", my_offset) + 1;
	my_offset = MoveToString(result_string, "?", my_offset) + 1;
	service_id_string_ = GetToCloseBracket(result_string, my_offset);
	service_id_ = atoi(service_id_string_.c_str());

	//	Fixup for numbers greater than 2^15-1... Should be future-proof.
	if (service_id_ > std::numeric_limits<unsigned short>::max())
		service_id_ = static_cast<unsigned short>(
			(0 - (std::numeric_limits<unsigned int>::max() - (service_id_ - 1))));

	service_id_neg_        = static_cast<signed short>(service_id_);
	service_id_string_neg_ = MLB::Utility::AnyToString(service_id_neg_);

	//	Get the IP address of the interface used by the host for the service.
	my_offset = MoveToString(result_string, ">" + service_id_string_,
		my_offset) + 1;
	my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
	std::string service_ip(GetToOpenBracket(result_string, my_offset));
	//	Hack the string as I've noticed that Rvd 7.5.2 can return a string in
	//	form: <ip-addr>;<mc-group>;<mc-group>
	std::string::size_type sep_index = service_ip.find(';');
	if (sep_index != std::string::npos)
		service_ip.substr(0, sep_index).swap(service_ip);

	my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
	std::string tmp_string(GetToOpenBracket(result_string, my_offset));
	reliability_ = atoi(tmp_string.c_str());
	my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
	tmp_string = GetToOpenBracket(result_string, my_offset);

	if ((tmp_string.size() == 22) && (tmp_string[12] == '(') &&
		(tmp_string[21] == ')')) {
		std::string tmp_date(tmp_string.substr(0, 10) + " " +
			tmp_string.substr(13, 8));
		creation_time_ = MLB::Utility::TimeVal::FromString(tmp_date);
	}

	src_endpoint_ = src_endpoint;
	service_address_ = MLB::SockLibX::EndPointIP(MLB::SockLibX::AddressIP(
		MLB::SockLibX::INetToAddr(service_ip)), service_id_);

	rv_network_        = service_ip;
	is_multicast_flag_ = MLB::SockLibX::IsMulticastIP(service_ip);

	rates_inbound_.SetRates(result_string);
	rates_outbound_.SetRates(result_string);
	totals_inbound_.SetTotals(result_string);
	totals_outbound_.SetTotals(result_string);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_Service &RvInfoRvd_Service::GetServiceInfo(
	MLB::SockLibX::EndPointIP &src_endpoint, RvInfo_ServiceNumber rv_service)
{
	return(GetServiceInfo(src_endpoint, MLB::Utility::AnyToString(rv_service)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_Service::ToStringTabular(std::string &out_string) const
{
	std::ostringstream o_str;

	o_str <<
		std::left << std::setw(15) << service_address_.GetHostIPAddress() <<
			std::right << " " <<
		std::right << std::setw(5) << service_address_.GetPort() <<
			std::left << " " <<
		std::right << std::setw(6) << reliability_ << std::left << " " <<
		creation_time_ << " " <<
		rates_inbound_ << " " <<
		rates_outbound_ << " " <<
		totals_inbound_ << " " <<
		totals_outbound_;

	return(out_string.assign(o_str.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Service::ToStringTabular() const
{
	std::string out_string;

	return(ToStringTabular(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &RvInfoRvd_Service::ToStringTabular(
	const std::set<RvInfoRvd_Service> &data_list, std::string &out_string,
	unsigned int indent_count)
{
	std::set<RvInfoRvd_Service>::const_iterator iter_b(data_list.begin());
	std::set<RvInfoRvd_Service>::const_iterator iter_e(data_list.end());
	std::string                                 tmp_string_1;
	std::string                                 tmp_string_2;

	while (iter_b != iter_e) {
		tmp_string_1 += ((tmp_string_1.empty()) ? "" : "\n") +
			MLB::Utility::PadLeft("", indent_count) +
			iter_b->ToStringTabular(tmp_string_2);
		++iter_b;
	}

	return(out_string.assign(tmp_string_1));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Service::ToStringTabular(
	const std::set<RvInfoRvd_Service> &data_list, unsigned int indent_count)
{
	std::string out_string;

	return(ToStringTabular(data_list, out_string, indent_count));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_Service &RvInfoRvd_Service::CreateService(
	const std::string &http_host, const std::string &http_port,
	const std::string &service, RvInfoRvd_Service &service_info)
{
	RvInfoRvd_Service tmp_service;

	tmp_service.SetAddressInfo(http_host, http_port);
	tmp_service.GetServiceInfo(tmp_service.src_endpoint_, service);
	service_info = tmp_service;

	return(service_info);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_Service RvInfoRvd_Service::CreateService(const std::string &http_host,
	const std::string &http_port, const std::string &service)
{
	RvInfoRvd_Service service_info;

	return(CreateService(http_host, http_port, service, service_info));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoRvd_Service::MoveToServiceInfoTextArea(
	const MLB::SockLibX::EndPointIP &src_endpoint,
	std::string &result_string) const
{
	GetDataBasic(src_endpoint, "/service_detail?" + rv_service_,
		result_string);

	std::string::size_type my_offset = 0;

	try {
		//	If the Rendezvous daemon drops a service because of extended disuse,
		//	queries for the service HTTP will return the Service List page (as
		//	if the HTTP request had been 'http://<host><port>/services' instead
		//	of 'http://<host>:<port>/service_detail?<service>'. This logic will
		//	detect that problem and throw an exception which reflects it.
		my_offset = MoveToString(result_string,
			"<b>Service Information</b>", my_offset) + 1;
	}
	catch (const std::exception &) {
		//	If we got the service page then the service is not (or was formerly,
		//	but is not now) available.
		if (MoveToString(result_string, "<b>Service List</b>", my_offset,
			false) != std::string::npos)
			throw RvProxyServiceDisabled("Attempt to retrieve service "
				"information for " + rv_service_ + " on daemon HTTP provider " +
				src_endpoint.ToString() + " failed because that service is "
				"not at present registered with the daemon.");
		throw;
	}

	return(MoveToString(result_string, ">service:<br>", my_offset) + 1);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string::size_type RvInfoRvd_Service::MoveToServiceInfoDataArea(
	const MLB::SockLibX::EndPointIP &src_endpoint,
	std::string &result_string) const
{
	std::string::size_type my_offset = MoveToServiceInfoTextArea(src_endpoint,
		result_string);

	my_offset = MoveToString(result_string, "<a href=/subs?", my_offset) + 1;
	my_offset = MoveToCloseBracket(result_string, my_offset) + 1;

	return(MoveToString(result_string, ">" + service_id_string_,
		my_offset) + 1);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string RvInfoRvd_Service::GetValueAtCountBR(unsigned int br_count) const
{
	std::string            result_string;
	std::string::size_type my_offset =
		MoveToServiceInfoDataArea(src_endpoint_, result_string);

	MovePastBR(result_string, my_offset, br_count);

	return(GetToCloseBracket(result_string, my_offset));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvInfoRvd_Instance : public RvInfoComponent_Instance {
public:
	RvInfoRvd_Instance() :
		 RvInfoComponent_Instance("Rvd")
		,user_name_()
	{
	}

	void Initialize(const std::string &host_name = "localhost",
		const std::string &service_port = "7580");

	RvInfo_ServiceNumberSet &GetServiceNumberList(
		RvInfo_ServiceNumberSet &rv_service_list);
	RvInfo_ServiceNumberSet  GetServiceNumberList();

	RvInfoRvd_ServiceSet &GetServiceList(RvInfoRvd_ServiceSet &service_list,
		unsigned int service = 0);
	RvInfoRvd_ServiceSet GetServiceList(unsigned int service = 0);
	RvInfoRvd_ServiceSet &GetServiceList(RvInfoRvd_ServiceSet &service_list,
		const std::string &service);
	RvInfoRvd_ServiceSet GetServiceList(const std::string &service);

	std::string             user_name_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvInfoRvd_Instance::Initialize(const std::string &host_name,
	const std::string &service_port)
{
	SetSrcInfo(host_name, service_port);

	std::string result_string;

	GetData("/", result_string);

	//	Locate the information area for the rvd main page...
	std::string::size_type my_offset;

	my_offset = GetOffsetRequired(result_string,
		">component:<br>version:<br>"
		"license ticket:<br>host name:<br>user name:<br>IP address:<br>"
		"client port:<br>network services:</font></td>");

	my_offset  = GetOffsetRequired(result_string, ">rvd", my_offset) + 1;
	component_ = GetToOpenBracket(result_string, my_offset);
	my_offset  = MoveToCloseBracket(result_string, my_offset) + 1;
	version_   = GetToOpenBracket(result_string, my_offset);
	my_offset  = MoveToCloseBracket(result_string, my_offset) + 1;
	license_   = GetToOpenBracket(result_string, my_offset);
	my_offset  = MoveToCloseBracket(result_string, my_offset) + 1;
	//	Skip host name...
	my_offset  = MoveToCloseBracket(result_string, my_offset) + 1;
	user_name_ = GetToOpenBracket(result_string, my_offset);
	my_offset  = MoveToCloseBracket(result_string, my_offset) + 1;
	std::string tmp_ip(GetToOpenBracket(result_string, my_offset));
	my_offset  = MoveToCloseBracket(result_string, my_offset) + 1;
	std::string tmp_port(GetToOpenBracket(result_string, my_offset));
	address_info_.SetAddressAndPort(tmp_ip, tmp_port);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfo_ServiceNumberSet &RvInfoRvd_Instance::GetServiceNumberList(
	RvInfo_ServiceNumberSet &rv_service_list)
{
	std::string result_string;

	GetData("/services", result_string);

	//	Locate the information area for the rvd Service List page...
	std::string::size_type  my_offset = 0;
	RvInfo_ServiceNumberSet tmp_list;

	while ((my_offset = GetOffset(result_string,
		"<a href=/service_detail?", my_offset, false)) != std::string::npos) {
		my_offset = MoveToCloseBracket(result_string, my_offset) + 1;
		std::string rv_service(GetToOpenBracket(result_string, my_offset));
		tmp_list.insert(atoi(rv_service.c_str()));
	}

	rv_service_list.swap(tmp_list);

	return(rv_service_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfo_ServiceNumberSet RvInfoRvd_Instance::GetServiceNumberList()
{
	RvInfo_ServiceNumberSet rv_service_list;

	return(GetServiceNumberList(rv_service_list));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_ServiceSet &RvInfoRvd_Instance::GetServiceList(
	RvInfoRvd_ServiceSet &service_list, unsigned int service)
{
	RvInfo_ServiceNumberSet rv_service_list;
	RvInfoRvd_ServiceSet    tmp_list;

	if (!GetServiceNumberList(rv_service_list).empty()) {
		RvInfo_ServiceNumberSetIterC iter_b(rv_service_list.begin());
		RvInfo_ServiceNumberSetIterC iter_e(rv_service_list.end());
		while (iter_b != iter_e) {
			if ((!service) || (service == *iter_b)) {
				RvInfoRvd_Service this_service;
				tmp_list.insert(this_service.GetServiceInfo(src_endpoint_,
					*iter_b));
				if (service)
					break;
			}
			++iter_b;
		}
	}

	service_list.swap(tmp_list);

	return(service_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_ServiceSet RvInfoRvd_Instance::GetServiceList(unsigned int service)
{
	RvInfoRvd_ServiceSet service_list;

	return(GetServiceList(service_list, service));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_ServiceSet &RvInfoRvd_Instance::GetServiceList(
	RvInfoRvd_ServiceSet &service_list, const std::string &service)
{
	return(GetServiceList(service_list,
		MLB::SockLibX::ResolveServToPort(service)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RvInfoRvd_ServiceSet RvInfoRvd_Instance::GetServiceList(
	const std::string &service)
{
	return(GetServiceList(MLB::SockLibX::ResolveServToPort(service)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvdInspectorHost : public RvInfoRvd_Host {
public:
	RvdInspectorHost() :
		 RvInfoRvd_Host()
		,service_()
		,service_number_(0)
		,service_dropped_flag_(false)
		,update_count_(0)
		,last_update_time_(0, 0)
		,old_totals_()
		,new_totals_()
		,has_alert_flag_(false)
	{
	}
	RvdInspectorHost(const RvInfoRvd_Host &host_info,
		const std::string &service) :
		 RvInfoRvd_Host(host_info)
		,service_(service)
		,service_number_(atoi(service.c_str()))
		,service_dropped_flag_(false)
		,update_count_(0)
		,last_update_time_(0, 0)
		,old_totals_()
		,new_totals_()
		,has_alert_flag_(false)
	{
	}
	//	For container searches...
	RvdInspectorHost(const std::string &service,
		const std::string &host_name_canonical) :
		 RvInfoRvd_Host(host_name_canonical)
		,service_(service)
		,service_number_(atoi(service.c_str()))
		,service_dropped_flag_(false)
		,update_count_(0)
		,last_update_time_(0, 0)
		,old_totals_()
		,new_totals_()
		,has_alert_flag_(false)
	{
	}

	bool operator < (const RvdInspectorHost &other) const {
		int cmp = service_number_ - other.service_number_;
		if (cmp < 0)
			return(true);
		return((!cmp) ? (host_name_canonical_ < other.host_name_canonical_) :
			false);
	}

	bool UpdateValues() {
		RvInfoRvd_Service host_service;
		try {
			RvInfoRvd_Service::CreateService(host_name_, http_port_, service_,
				host_service);
			if (service_dropped_flag_) {
				LogInfo << "Tib/Rendezvous daemon on host " << host_name_ <<
					" has re-activated service " << service_ << "." << std::endl;
				service_dropped_flag_ = false;
			}
		}
		catch (const RvProxyCriticalEvent &) {
			throw;
		}
		catch (const RvProxyServiceDisabled &) {
			if (!service_dropped_flag_) {
				LogInfo << "Tib/Rendezvous daemon on host " << host_name_ <<
					" has dropped service " << service_ << "." << std::endl;
				service_dropped_flag_ = true;
			}
			return(false);
		}
		catch (const MLB::SockLibX::SockLibXConnectRefused &except) {
			//	Machine available; daemon probably dead.
			if (!service_dropped_flag_) {
				LogInfo << "Tib/Rendezvous daemon on host " << host_name_ <<
					" appears to have exited --- attempt to retrieve the HTTP "
					"page for service " << service_ << " resulted in an error: " <<
					except.what() << std::endl;
				service_dropped_flag_ = true;
			}
			return(false);
		}
		catch (const std::exception &except) {
			//	Some other error; re-throw so caller can remove from the list...
			LogWarning << "Tib/Rendezvous daemon on host " << host_name_ <<
				" is not contactable --- attempt to retrieve the HTTP "
				"page for service " << service_ << " resulted in an error: " <<
				except.what() << std::endl;
			throw;
		}
		new_totals_ = RvInfoRvd_TotalsBoth(host_service.totals_inbound_,
			host_service.totals_outbound_);
		return(true);
	}

	bool CheckValues() {
		has_alert_flag_ = false;
		if (update_count_++) {
			CheckOldVersusNew(" Inbound Missed Packets   : ",
				old_totals_.totals_inbound_.missed_,
				new_totals_.totals_inbound_.missed_);
			CheckOldVersusNew(" Inbound Lost Multicast   : ",
				old_totals_.totals_inbound_.lostMc_,
				new_totals_.totals_inbound_.lostMc_);
			CheckOldVersusNew(" Inbound Lost PtP         : ",
				old_totals_.totals_inbound_.lostPtP_,
				new_totals_.totals_inbound_.lostPtP_);
			CheckOldVersusNew(" Outbound Retrans Requests: ",
				old_totals_.totals_outbound_.retrans_,
				new_totals_.totals_outbound_.retrans_);
			CheckOldVersusNew(" Outbound Retrans Rejects : ",
				old_totals_.totals_outbound_.rejects_,
				new_totals_.totals_outbound_.rejects_);
			CheckOldVersusNew(" Outbound Host Down       : ",
				old_totals_.totals_outbound_.hostDown_,
				new_totals_.totals_outbound_.hostDown_);
		}
		else {
			CheckFirst(" Inbound Missed Packets   : ",
				new_totals_.totals_inbound_.missed_);
			CheckFirst(" Inbound Lost Multicast   : ",
				new_totals_.totals_inbound_.lostMc_);
			CheckFirst(" Inbound Lost PtP         : ",
				new_totals_.totals_inbound_.lostPtP_);
			CheckFirst(" Outbound Retrans Requests: ",
				new_totals_.totals_outbound_.retrans_);
			CheckFirst(" Outbound Retrans Rejects : ",
				new_totals_.totals_outbound_.rejects_);
			CheckFirst(" Outbound Host Down       : ",
				new_totals_.totals_outbound_.hostDown_);
		}
		old_totals_       = new_totals_;
		last_update_time_ = MLB::Utility::TimeVal();
		return(has_alert_flag_);
	}
	bool CheckValues(RvInfoRvd_TotalsBoth &recap_totals) {
		CheckValues();
		recap_totals += new_totals_;
		return(has_alert_flag_);
	}

	std::string           service_;
	int                   service_number_;
	bool                  service_dropped_flag_;
	UnsignedQuantity      update_count_;
	MLB::Utility::TimeVal last_update_time_;
	RvInfoRvd_TotalsBoth  old_totals_;
	RvInfoRvd_TotalsBoth  new_totals_;

private:
	bool has_alert_flag_;

	void CheckOldVersusNew(const char *text, UnsignedQuantity old_value,
		UnsignedQuantity new_value) {
		if (old_value != new_value) {
			LogInfo << text;
			EmitValue(old_value);
			LogInfo << " ---> ";
			EmitValue(new_value);
			LogInfo << ": " << host_name_ << std::endl;
			has_alert_flag_ = true;
		}
	}
	void CheckFirst(const char *text, UnsignedQuantity new_value) {
		if (new_value) {
			LogInfo << text;
			EmitValue(new_value);
			LogInfo << "      " << std::setw(UnsignedQuantityWidth) << "";
			LogInfo << ": " << host_name_ << std::endl;
			has_alert_flag_ = true;
		}
	}
	void EmitValue(UnsignedQuantity value) {
		LogInfo << std::setw(UnsignedQuantityWidth) << value;
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<RvdInspectorHost>          RvdInspectorHostSet;
typedef RvdInspectorHostSet::iterator       RvdInspectorHostSetIter;
typedef RvdInspectorHostSet::const_iterator RvdInspectorHostSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvdInspectorService : public RvInfoRvd_Service {
public:
	RvdInspectorService() :
		 RvInfoRvd_Service()
		,service_dropped_flag_(false)
		,update_count_(0)
		,last_update_time_(0, 0)
	{
	}

	bool                  service_dropped_flag_;
	UnsignedQuantity      update_count_;
	MLB::Utility::TimeVal last_update_time_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::set<RvdInspectorService>          RvdInspectorServiceSet;
typedef RvdInspectorServiceSet::iterator       RvdInspectorServiceSetIter;
typedef RvdInspectorServiceSet::const_iterator RvdInspectorServiceSetIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<std::ofstream>   CsvFilePtr;
typedef std::map<int, CsvFilePtr>          CsvFileMap;
typedef CsvFileMap::iterator               CsvFileMapIter;
typedef CsvFileMap::const_iterator         CsvFileMapIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class RvdInspector : public RvInfoComponent {
public:
	RvdInspector(const std::string &http_host = "localhost",
		const std::string &http_port = "7580") :
		 RvInfoComponent(http_host, http_port)
		,rv_context_()
		,csv_flag_(false)
		,cycle_sleep_ms_(DefaultCycleSleepMs)
		,host_sleep_ms_(DefaultHostSleepMs)
		,host_filter_()
		,log_flag_(false)
		,log_dir_("./")
		,snd_spec_set_()
		,daemon_(http_host + ":7500")
		,http_host_(http_host)
		,http_port_(http_port)
		,host_set_()
		,csv_file_map_()
		,csv_file_name_base_()
	{
	}

	void ParseCmdLine(int argc, char **argv);
	void Start();
	void DetermineServiceList();
	void Run();

	typedef std::set<SNDSpec>          SNDSpecSet;
	typedef SNDSpecSet::iterator       SNDSpecSetIter;
	typedef SNDSpecSet::const_iterator SNDSpecSetIterC;

	typedef boost::shared_ptr<TibrvNetTransport> TibrvNetTransportPtr;
	typedef std::vector<TibrvNetTransportPtr>    TibrvNetTransportPtrVector;

	RvContext                      rv_context_;
	bool                           csv_flag_;
	unsigned int                   cycle_sleep_ms_;
	unsigned int                   host_sleep_ms_;
	RvInfoHostFilter               host_filter_;
	bool                           log_flag_;
	std::string                    log_dir_;
	SNDSpecSet                     snd_spec_set_;
	TibrvNetTransportPtrVector     transport_ptr_list_;
	std::string                    daemon_;
	std::string                    http_host_;
	std::string                    http_port_;
	RvdInspectorServiceSet         service_list_;
	RvdInspectorHostSet            host_set_;
	CsvFileMap                     csv_file_map_;
	std::string                    csv_file_name_base_;

protected:
	virtual std::string GetComponentName() const {
		return("Rvd Inspector");
	}

private:
	void WriteCsv(const RvdInspectorHost &recap_info,
		unsigned int total_hosts_count, unsigned int problem_hosts_count) {
		if (csv_flag_) {
			CsvFileMapIter iter_f(GetCsv(recap_info.service_number_));
			*iter_f->second.get()                                <<
				recap_info.last_update_time_                      << "," <<
				recap_info.service_                               << "," <<
				total_hosts_count                                 << "," <<
				problem_hosts_count                               << "," <<
				recap_info.new_totals_.totals_inbound_.msgs_      << "," <<
				recap_info.new_totals_.totals_inbound_.bytes_     << "," <<
				recap_info.new_totals_.totals_inbound_.pkts_      << "," <<
				recap_info.new_totals_.totals_inbound_.missed_    << "," <<
				recap_info.new_totals_.totals_inbound_.lostMc_    << "," <<
				recap_info.new_totals_.totals_inbound_.lostPtP_   << "," <<
				recap_info.new_totals_.totals_outbound_.msgs_     << "," <<
				recap_info.new_totals_.totals_outbound_.bytes_    << "," <<
				recap_info.new_totals_.totals_outbound_.pkts_     << "," <<
				recap_info.new_totals_.totals_outbound_.retrans_  << "," <<
				recap_info.new_totals_.totals_outbound_.rejects_  << "," <<
				recap_info.new_totals_.totals_outbound_.hostDown_ << std::endl;
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
				"\"Rv Service\","                  <<
				"\"Total Hosts\","                 <<
				"\"Problem Hosts\","               <<
				"\"Inbound Messages\","            <<
				"\"Inbound Bytes\","               <<
				"\"Inbound Packets\","             <<
				"\"Inbound Missed\","              <<
				"\"Inbound Lost Multicast\","      <<
				"\"Inbound Lost Point-to-Point\"," <<
				"\"Outbound Messages\","           <<
				"\"Outbound Bytes\","              <<
				"\"Outbound Packets\","            <<
				"\"Outbound Retrans Requests\","   <<
				"\"Outbound Retrans Rejects\","    <<
				"\"Outbound Hosts Down\""          << std::endl;
			csv_file_map_[service_number] = tmp_file_ptr;
			iter_f = csv_file_map_.find(service_number);
		}
		return(iter_f);
	}

	bool                 HostIsInSet(const std::string &service,
		const std::string &host_name_canonical) {
		return(GetHostRef(service, host_name_canonical) != host_set_.end());
	}

	RvdInspectorHostSetIter GetHostRef(const std::string &service,
		const std::string &host_name_canonical) {
		return(host_set_.find(RvdInspectorHost(service, host_name_canonical)));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdInspector::ParseCmdLine(int argc, char **argv)
{
	unsigned int count_1;
	std::string  subject_name;
	SNDSpecSet   tmp_snd_spec_set;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		std::string tmp_string;
		SNDSpec     tmp_snd_spec;
		if (RvParseCmdLineArg::ParseCmdLineCombinedServiceNetwork(count_1, argc,
			argv, tmp_snd_spec))
			tmp_snd_spec_set.insert(tmp_snd_spec);
		else if (RvParseCmdLineArg::ParseCmdLineDaemon(count_1, argc,
			argv, daemon_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-CSV_FLAG")
			("-CSVFLAG")
			("-CSV"),
			count_1, argc, argv, tmp_string))
			RvParseCmdLineArg::ParseCmdLineDatum(tmp_string, csv_flag_);
		else if (RvParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-HTTP_PORT_NUMBER")
			("-HTTP_PORTNUMBER")
			("-HTTPPORT_NUMBER")
			("-HTTPPORTNUMBER")
			("-HTTP_PORT_NUM")
			("-HTTP_PORTNUM")
			("-HTTPPORT_NUM")
			("-HTTPPORTNUM")
			("-HTTP_PORT")
			("-HTTPPORT"),
			count_1, argc, argv, http_port_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineNumeric(
			MLB::Utility::MakeInlineVector<std::string>
			("-CYCLE_SLEEP_MILLISECONDS")
			("-CYCLE_SLEEPMILLISECONDS")
			("-CYCLESLEEP_MILLISECONDS")
			("-CYCLESLEEPMILLISECONDS")

			("-CYCLE_SLEEP_MILLISECOND")
			("-CYCLE_SLEEPMILLISECOND")
			("-CYCLESLEEP_MILLISECOND")
			("-CYCLESLEEPMILLISECOND")

			("-CYCLE_SLEEP_MILLISECS")
			("-CYCLE_SLEEPMILLISECS")
			("-CYCLESLEEP_MILLISECS")
			("-CYCLESLEEPMILLISECS")

			("-CYCLE_SLEEP_MILLISEC")
			("-CYCLE_SLEEPMILLISEC")
			("-CYCLESLEEP_MILLISEC")
			("-CYCLESLEEPMILLISEC")

			("-CYCLE_SLEEP_MS")
			("-CYCLE_SLEEPMS")
			("-CYCLESLEEP_MS")
			("-CYCLESLEEPMS")

			("-CYCLE_SLEEP")
			("-CYCLESLEEP"),
			count_1, argc, argv, cycle_sleep_ms_))
			;
		else if (RvParseCmdLineArg::ParseCmdLineNumeric(
			MLB::Utility::MakeInlineVector<std::string>
			("-HOSTS_SLEEP_MILLISECONDS")
			("-HOSTS_SLEEPMILLISECONDS")
			("-HOSTSSLEEP_MILLISECONDS")
			("-HOSTSSLEEPMILLISECONDS")

			("-HOSTS_SLEEP_MILLISECOND")
			("-HOSTS_SLEEPMILLISECOND")
			("-HOSTSSLEEP_MILLISECOND")
			("-HOSTSSLEEPMILLISECOND")

			("-HOSTS_SLEEP_MILLISECS")
			("-HOSTS_SLEEPMILLISECS")
			("-HOSTSSLEEP_MILLISECS")
			("-HOSTSSLEEPMILLISECS")

			("-HOSTS_SLEEP_MILLISEC")
			("-HOSTS_SLEEPMILLISEC")
			("-HOSTSSLEEP_MILLISEC")
			("-HOSTSSLEEPMILLISEC")

			("-HOSTS_SLEEP_MS")
			("-HOSTS_SLEEPMS")
			("-HOSTSSLEEP_MS")
			("-HOSTSSLEEPMS")

			("-HOSTS_SLEEP")
			("-HOSTSSLEEP")

			("-HOST_SLEEP_MILLISECONDS")
			("-HOST_SLEEPMILLISECONDS")
			("-HOSTSLEEP_MILLISECONDS")
			("-HOSTSLEEPMILLISECONDS")

			("-HOST_SLEEP_MILLISECOND")
			("-HOST_SLEEPMILLISECOND")
			("-HOSTSLEEP_MILLISECOND")
			("-HOSTSLEEPMILLISECOND")

			("-HOST_SLEEP_MILLISECS")
			("-HOST_SLEEPMILLISECS")
			("-HOSTSLEEP_MILLISECS")
			("-HOSTSLEEPMILLISECS")

			("-HOST_SLEEP_MILLISEC")
			("-HOST_SLEEPMILLISEC")
			("-HOSTSLEEP_MILLISEC")
			("-HOSTSLEEPMILLISEC")

			("-HOST_SLEEP_MS")
			("-HOST_SLEEPMS")
			("-HOSTSLEEP_MS")
			("-HOSTSLEEPMS")

			("-HOST_SLEEP")
			("-HOSTSLEEP"),
			count_1, argc, argv, host_sleep_ms_))
			;
		else if (host_filter_.ParseCmdLineHost(count_1, argc, argv))
			;
		else if (RvParseCmdLineArg::ParseLoggingFlag(count_1, argc, argv,
			log_flag_))
			;
		else if (RvParseCmdLineArg::ParseLoggingDir(count_1, argc, argv,
			log_dir_, "", true))
			;
		else
			RvParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Ensure logging directory is fully resolved...
	//	//////////////////////////////////////////////////////////////////////
	MLB::Utility::ResolveDirectoryPath(log_dir_, "", true);
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Use the daemon specified to determine the HTTP connection info...
	//	//////////////////////////////////////////////////////////////////////
	std::string::size_type found_pos = daemon_.find(":");
	http_host_ = ((found_pos == std::string::npos) || (!found_pos)) ?
		"localhost" : daemon_.substr(0, found_pos);
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	SetAddressInfo(http_host_, http_port_);
	if (http_host_ == "localhost")
		http_host_ = MLB::Utility::GetHostName();
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	Create the list of services using the specified daemon...
	//	//////////////////////////////////////////////////////////////////////
	SNDSpecSetIterC iter_b(tmp_snd_spec_set.begin());
	SNDSpecSetIterC iter_e(tmp_snd_spec_set.end());
	while (iter_b != iter_e) {
		snd_spec_set_.insert(SNDSpec(iter_b->service_, iter_b->network_,
			daemon_));
		++iter_b;
	}
	//	//////////////////////////////////////////////////////////////////////
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdInspector::Start()
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
	file_name = log_dir_ + "/RvInfoRvd." + tmp_date_time + "." +
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
	LogInfo << "Rendezvous Daemon    : " << daemon_ << std::endl;
	LogInfo << "Http Daemon Host     : " << http_host_ << std::endl;
	LogInfo << "Http Daemon Port     : " << http_port_ << std::endl;
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
		if (snd_spec_set_.empty()) {
			DetermineServiceList();
			LogInfo.LogSeparator('-');
		}
		RvInfoRvd_Instance rv_component;
		rv_component.Initialize(http_host_, http_port_);
		SNDSpecSetIterC iter_b(snd_spec_set_.begin());
		SNDSpecSetIterC iter_e(snd_spec_set_.end());
		SNDSpecSetIterC iter_first(iter_b);
		while (iter_b != iter_e) {
			std::string xport_name(iter_b->service_ + ":" + iter_b->network_);
			try {
				transport_ptr_list_.
					push_back(TibrvNetTransportPtr(new TibrvNetTransport));
				iter_b->CreateTransport(*transport_ptr_list_.back().get());
				RvUtilX_THROW_TIBRV_STATUS_IF(
					transport_ptr_list_.back()->setDescription,
					(("RvInfoRvd Transport on " + my_name + " for "+ xport_name).
					c_str()));
				LogInfo << ((iter_b == iter_first) ? "Service/Network      : " :
					"                     : ") << xport_name << std::endl;
				RvdInspectorService tmp_service;
				tmp_service.GetServiceInfo(src_endpoint_, iter_b->service_);
				service_list_.insert(tmp_service);
				//	Insert ourselves into the host set...
				RvInfoRvd_Host this_host(http_host_, "",
					"http://" + http_host_ + ":" + http_port_, "",
					http_port_, "0", "0", "0");
				host_set_.insert(RvdInspectorHost(this_host, iter_b->service_));
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
	catch (const RvProxyCriticalEvent &except) {
		LogInfo << "Program exit scheduled: " << except.what() << std::endl;
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
void RvdInspector::DetermineServiceList()
{
	LogInfo << "Attempting to locate active services on daemon " << daemon_ <<
		"..." << std::endl;

	RvInfoRvd_Instance rv_component;

	rv_component.Initialize(http_host_, http_port_);

	RvInfoRvd_ServiceSet service_list;
	rv_component.GetServiceList(service_list);

	if (service_list.empty())
		MLB::Utility::ThrowStdException("No active services were located when "
			"interrogating the HTTP page daemon " + http_host_ +
			", HTTP port " + http_port_ + ".");

	RvInfoRvd_ServiceSetIterC iter_b(service_list.begin());
	RvInfoRvd_ServiceSetIterC iter_e(service_list.end());

	while (iter_b != iter_e) {
		snd_spec_set_.insert(SNDSpec(iter_b->rv_service_,
			((iter_b->is_multicast_flag_) ? ";" : "") + iter_b->rv_network_,
			daemon_));
		++iter_b;
	}

	LogInfo << "Located " << MLB::Utility::AnyToString(snd_spec_set_.size()) <<
		" service" << ((snd_spec_set_.size() == 1) ? "" : "s") <<
		" on daemon " << daemon_ << "." << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RvdInspector::Run()
{
	bool                first_iteration = true;
	RvdInspectorHostSet recap_list;

	while (!MLB::Utility::CriticalEventTest()) {
		RvdInspectorServiceSetIterC iter_b(service_list_.begin());
		RvdInspectorServiceSetIterC iter_e(service_list_.end());
		while (iter_b != iter_e) {
			RvdInspectorHost recap_key(iter_b->rv_service_,
				"Recap for service " + iter_b->rv_service_);
			if (first_iteration) {
				recap_list.insert(recap_key);
			}
			{
				RvInfoRvd_HostVector host_list;
				try {
					iter_b->GetHosts(host_filter_, host_list);
				}
				catch (const RvProxyFileNotFound &) {
					LogInfo << "Tib/Rendezvous daemon on host " << http_host_ <<
					" has dropped the list of hosts for service " <<
					iter_b->rv_service_ + "." << std::endl;
				}
				catch (const MLB::SockLibX::SockLibXConnectFailure &except) {
					LogInfo << "Tib/Rendezvous daemon on host " << http_host_ <<
						" appears to have exited --- attempt to retrieve the HTTP "
						"page containing known hosts for service " <<
						iter_b->rv_service_ << " resulted in an error: " <<
						except.what() << std::endl;
				}
				RvInfoRvd_HostVectorIterC iter_host_b(host_list.begin());
				RvInfoRvd_HostVectorIterC iter_host_e(host_list.end());
				while (iter_host_b != iter_host_e) {
					if (!HostIsInSet(iter_b->rv_service_,
						iter_host_b->host_name_canonical_)) {
						LogInfo << "New host " << iter_host_b->host_name_ <<
							" located on Rendezvous service " <<
							iter_b->rv_service_ << "." << std::endl;
						host_set_.insert(RvdInspectorHost(*iter_host_b,
							iter_b->rv_service_));
					}
					++iter_host_b;
				}
			}
			RvdInspectorHostSetIter recap_iter(recap_list.find(recap_key));
			const_cast<RvdInspectorHost *>(&(*recap_iter))->new_totals_.Clear();
			RvInfoRvd_TotalsBoth    recap_totals;
			unsigned int            total_hosts_count   = 0;
			unsigned int            problem_hosts_count = 0;
			RvdInspectorHostSetIter iter_host_b(host_set_.begin());
			RvdInspectorHostSetIter iter_host_e(host_set_.end());
			while (iter_host_b != iter_host_e) {
				if (iter_host_b->service_ == iter_b->rv_service_) {
					try {
						if (const_cast<RvdInspectorHost *>(&(*iter_host_b))->UpdateValues()) {
							++total_hosts_count;
							if (const_cast<RvdInspectorHost *>(&(*iter_host_b))->
								CheckValues(const_cast<RvdInspectorHost *>
								(&(*recap_iter))->new_totals_))
								++problem_hosts_count;
						}
						++iter_host_b;
					}
					catch (const RvProxyCriticalEvent &) {
						throw;
					}
					catch (...) {
						LogWarning << "Removing host " << iter_host_b->host_name_ <<
							" from the list of active hosts on service " <<
							iter_b->rv_service_ << "." << std::endl;
						iter_host_b = host_set_.erase(iter_host_b);
					}
				}
				else
					++iter_host_b;
			}
			((problem_hosts_count) ? LogInfo : LogDetail) <<
				"Hosts on service " << iter_b->rv_service_ <<
				" with Tib/Rendezvous problems: " << problem_hosts_count <<
				" of " << total_hosts_count << ":" << std::endl;
			const_cast<RvdInspectorHost *>(&(*recap_iter))->CheckValues();
			if (csv_flag_)
				WriteCsv(*recap_iter, total_hosts_count, problem_hosts_count);
			++iter_b;
			if (host_sleep_ms_)
				MLB::Utility::SleepMilliSecs(host_sleep_ms_);
		}
		first_iteration = false;
		if (cycle_sleep_ms_) {
			LogMinutiae << "Sleeping for " <<
				(static_cast<double>(host_sleep_ms_) / 1000.0) <<
				" seconds..." << std::endl;
std::cout << "********** SLEEP START" << std::endl;
			MLB::Utility::TimeVal sleep_end_time;
			sleep_end_time.AddMilliseconds(cycle_sleep_ms_);
			while (sleep_end_time > MLB::Utility::TimeVal()) {
				if (MLB::Utility::CriticalEventTest())
					return;
				MLB::Utility::SleepMilliSecs(10);
			}
std::cout << "********** SLEEP END" << std::endl;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

#include <SockLibX/ParseCmdLineArg.hpp>

using namespace MLB::RvUtilX;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code = EXIT_SUCCESS;
	std::string  my_name     = MLB::Utility::GetFileNamePortion(argv[0]);

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (MLB::SockLibX::SockLibXParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -service_net <RV-service>[:<RV-network>] ... ] " <<
			"[ -daemon <RV-daemon> ] " <<
			"[ -http_port <RV-http-daemon-port> ( = 7580 ) ]" <<
			"[ -csv <boolean> ( = FALSE ) ] " <<
			"[ -cycle_sleep_ms <milliseconds-to-sleep-between-cycles> ( = " <<
				DefaultCycleSleepMs << " ) ] " <<
			"[ -host_sleep_ms <milliseconds-to-sleep-between-hosts> ( = " <<
				DefaultHostSleepMs << " ) ] " <<
			"[ -include_host <host-name>[,<host-name>[,...] ] " <<
			"[ -exclude_host <host-name>[,<host-name>[,...] ] " <<
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
   -http_port <RV-http-daemon-host>\n\
      The port number to be used when connecting to the Tib/Rendezvous daemon\n\
      HTTP server.\n\n\
   -csv <boolean>\n\
      Specifies whether recap information is to be written to a file in CSV\n\
      format.\n\n\
   -cycle_sleep_ms <milliseconds-to-sleep-between-cycles>\n\
      The number of milliseconds to sleep between the host interrogation\n\
      cycles.\n\n\
   -host_sleep_ms <milliseconds-to-sleep-between-hosts>\n\
      The number of milliseconds to sleep between the interrogation of each\n\
      host.\n\n\
   -include_host <host-name>[,<host-name>[,...]]\n\
      Specifies one or more host names to be included in the analysis. If any\n\
      hosts are specified in an '-include_host' parameter, then hosts not so\n\
      specified will not be included in the analysis.\n\n\
      This parameter may be specified multiple times.\n\n\
   -exclude_host <host-name>[,<host-name>[,...]]\n\
      Specifies one or more host names to be excluded from the analysis.\n\n\
      This parameter may be specified multiple times.\n\n\
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
		MLB::SockLibX::SockLibXContext     socket_lib;
		RvdInspector                       rvd_inspector;
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

