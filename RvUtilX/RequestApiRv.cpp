// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for the RequestApiRv class.

	Revision History	:	2006-09-21 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RequestApi.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

namespace {
//	////////////////////////////////////////////////////////////////////////////
const char RequestApiRv_DefaultTransportNamePrefix[] = "RequestApiRv Client: ";
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
RequestApiRv::RequestApiRv(const std::string &service,
	const std::string &network, const std::string &daemon)
	:MLB::Utility::RequestApiUtility<ReplyInfoRv>()
	,rv_context_ptr_(new RvContext)
	,snd_spec_(service, network, daemon)
	,transport_ptr_(new TibrvNetTransport)
	,transport_name_prefix_(RequestApiRv_DefaultTransportNamePrefix)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RequestApiRv::RequestApiRv(const SNDSpec &snd_spec)
	:MLB::Utility::RequestApiUtility<ReplyInfoRv>()
	,rv_context_ptr_(new RvContext)
	,snd_spec_(snd_spec)
	,transport_ptr_(new TibrvNetTransport)
	,transport_name_prefix_(RequestApiRv_DefaultTransportNamePrefix)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RequestApiRv::RequestApiRv(const RequestApiRv &other)
	:MLB::Utility::RequestApiUtility<ReplyInfoRv>()
	,rv_context_ptr_(new RvContext)
	,snd_spec_(other.snd_spec_)
	,transport_ptr_(new TibrvNetTransport)
	,transport_name_prefix_(other.transport_name_prefix_)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
RequestApiRv & RequestApiRv::operator = (const RequestApiRv &other)
{
	RequestApiRv tmp_datum(other);

	swap(tmp_datum);

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void RequestApiRv::swap(RequestApiRv &other)
{
	rv_context_ptr_.swap(other.rv_context_ptr_);
	snd_spec_.swap(other.snd_spec_);
	transport_ptr_.swap(other.transport_ptr_);
	transport_name_prefix_.swap(other.transport_name_prefix_);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RequestApiRv::Initialize()
{
	snd_spec_.CreateTransport(*transport_ptr_);

	SetTransportDescription();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SNDSpec RequestApiRv::GetSNDSpec() const
{
	return(snd_spec_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SNDSpec RequestApiRv::ChangeTransport(const std::string &service,
	const std::string &network, const std::string &daemon)
{
	return(ChangeTransport(SNDSpec(service, network, daemon)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SNDSpec RequestApiRv::ChangeTransport(const SNDSpec &in_snd_spec)
{
	SNDSpec old_snd_spec(snd_spec_);

	try {
		SNDSpec new_snd_spec(in_snd_spec);
		boost::shared_ptr<TibrvNetTransport>
			new_transport_ptr_(new TibrvNetTransport);
		new_snd_spec.CreateTransport(*new_transport_ptr_);
		SetTransportDescription(new_transport_ptr_.get());
		snd_spec_.swap(new_snd_spec);
		transport_ptr_.swap(new_transport_ptr_);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to change the Tib/Rendezvous "
			"transport to '" + in_snd_spec.ToString() + "': " +
			std::string(except.what()));
	}

	return(old_snd_spec);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RequestApiRv::SetTransportDescription(
	const std::string &transport_description)
{
	SetTransportDescription(transport_ptr_.get(), transport_description);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RequestApiRv::SetTransportDescription(TibrvNetTransport *transport_ptr,
	const std::string &transport_description)
{
	if (transport_description.empty()) {
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_ptr->setDescription,
			(std::string(GetTransportDescriptionPrefix() +
			GetDescriptionBasicId()).c_str()));
	}
	else {
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_ptr->setDescription,
			(transport_description.c_str()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RequestApiRv::SetTransportDescriptionPrefix(const std::string &prefix)
{
	transport_name_prefix_ = prefix;
}
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
std::string RequestApiRv::GetTransportDescriptionPrefix() const
{
	return(transport_name_prefix_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &RequestApiRv::GetDescriptionBasicId(std::string &out_string)
{
	return(out_string.assign("HOST=" + MLB::Utility::GetHostNameCanonical() +
		", PID=" +  MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()) +
		", TID=" + MLB::Utility::AnyToString(MLB::Utility::CurrentThreadId())));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string RequestApiRv::GetDescriptionBasicId()
{
	std::string out_string;

	return(GetDescriptionBasicId(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RequestApiRv::SendMsg(TibrvMsg &msg_send, TibrvMsg &msg_recv,
	const std::string &send_subject, const std::string &recv_subject,
	int wait_ms)
{
	SendMsg(msg_send, send_subject, recv_subject).
		DoRecv(msg_recv, wait_ms, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ReplyInfoRv RequestApiRv::SendMsg(TibrvMsg &msg_send,
	const std::string &send_subject, const std::string &recv_subject)
{
	SubjectNameFlat::CheckFullNameSubjectName(send_subject);
	SubjectNameFlat::CheckReplySubjectName(recv_subject);

	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setSendSubject,
		(send_subject.c_str()));

	RvUtilX_THROW_TIBRV_STATUS_IF(msg_send.setReplySubject,
		(recv_subject.c_str()));
	
	ReplyInfoRv reply_info(transport_ptr_, recv_subject);

	RvUtilX_THROW_TIBRV_STATUS_IF(transport_ptr_->send,
		(msg_send));

	return(reply_info);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ReplyInfoRv RequestApiRv::DoSendImpl(const TibrvMsg &send_data)
{
	ReplyInfoRv reply_info;

	try {
		ReplyInfoRv(transport_ptr_, send_data).swap(reply_info);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to send a Tib/Rendezvous "
			"message: " + std::string(except.what()));
	}

	return(reply_info);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

#ifdef TEST_MAIN

#include <RvUtilX/AddRvField.hpp>
#include <RvUtilX/GetRvField.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/Sleep.hpp>

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
const char TEST_SubjectSend[] =
"CATSTEST1.Request.PmqManager.0.MLB_DOMAIN";

const char TEST_SubjectRecv[] =
"CATSTEST1.Reply.PmqManager.0.MLB_DOMAIN.2SDReL3zJoUL0gelIr1Tz@PC.2I3hst2JGoSs1mDKsd1eRRaX";
const char TEST_MessageText[] =
"<polaris class_id=\"0\" tracking_level=\"0\" version=\"0\">\n\
        <request>\n\
                <type>program</type>\n\
                <action>PingRequest</action>\n\
                <pingRequestTime>\n\
                        <dateTime>2006-09-22 11:56:12.238903</dateTime>\n\
                </pingRequestTime>\n\
        </request>\n\
</polaris>\n";
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_ShowData(const char *data_type, const std::string &data_value)
{
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << data_type << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
	std::cout << data_value;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_Sync(int &return_code)
{
	try {
		RequestApiRv rv_api("7500", ";239.255.1.45");
		TibrvMsg     msg_send;
		TibrvMsg     msg_recv;
		AddRvFieldXml(msg_send, "xmlData", TEST_MessageText);
		rv_api.SendMsg(msg_send, msg_recv, TEST_SubjectSend, TEST_SubjectRecv,
			10 * 1000);
		TEST_ShowData("Send Data (sync): ", TEST_MessageText);
		std::string xmlData;
		GetRvFieldXml(msg_recv, "xmlData", xmlData);
		TEST_ShowData("Recv Data (sync): ", xmlData);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR IN SYNCHRONOUS TEST : " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_Async(int &return_code, unsigned int sleep_seconds)
{
	try {
		RequestApiRv rv_api("7500", ";239.255.1.45");
		TibrvMsg     msg_send;
		TibrvMsg     msg_recv;
		AddRvFieldXml(msg_send, "xmlData", TEST_MessageText);
		ReplyInfoRv reply_info(rv_api.SendMsg(msg_send, TEST_SubjectSend,
			TEST_SubjectRecv));
		TEST_ShowData("Send Data (async): ", TEST_MessageText);
		std::cout << "Sleeping for " << sleep_seconds << " seconds..." <<
			std::flush;
		MLB::Utility::SleepSecs(sleep_seconds);
		std::cout << std::endl;
		reply_info.DoRecv(msg_recv, 0, true);
		std::string xmlData;
		GetRvFieldXml(msg_recv, "xmlData", xmlData);
		TEST_ShowData("Recv Data (async): ", xmlData);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR IN ASYNCHRONOUS TEST: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	TEST_Sync(return_code);
	TEST_Async(return_code, 3);

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

