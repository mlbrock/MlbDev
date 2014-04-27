//	//////////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////////
//	Network Programming Support Library (NPSL) Source Module
//	//////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for object-based NPSL support.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	//////////////////////////////////////////////////////////////////////////

#include <Utility/InlineContainer.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/PathName.hpp>
#include <Utility/CriticalEventHandler.hpp>
#include <SockLibX/ServerX.hpp>
#include <SockLibX/ParseCmdLineArg.hpp>
#include <Utility/OSFileSupport.hpp>
#include <Utility/StringSupport.hpp>

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

#ifdef _Windows
# pragma warning(disable:4217 4668 4625 4626)
# if defined(_MSC_VER) && (_MSC_VER >= 1300) && (_MSC_VER <= 1500)
#  pragma warning(push)
#  pragma warning(disable:4061 4101 4244 4302 4350 4365)
# endif // # if defined(_MSC_VER) && (_MSC_VER >= 1300) && (_MSC_VER <= 1500)
#endif // #ifdef _Windows

#include <boost/thread/thread.hpp>

#ifdef _Windows
# if defined(_MSC_VER) && (_MSC_VER >= 1300) && (_MSC_VER <= 1500)
#  pragma warning(pop)
# endif // # if defined(_MSC_VER) && (_MSC_VER >= 1300) && (_MSC_VER <= 1500)
#endif // #ifdef _Windows

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

//	//////////////////////////////////////////////////////////////////////////
enum SocketPlugOperationType {
	OperationNone = -1,
	OperationRecv =  0,
	OperationSend =  1
};
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
enum SocketPlugSideType {
	SideNone = -1,
	SideNear =  0,
	SideFar  =  1
};
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string OperationTypeToString(SocketPlugOperationType datum)
{
	return((datum == OperationNone) ? "none" : (datum == OperationRecv) ?
		"recv" : (datum == OperationSend) ? "send" : "????");
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
std::string SideTypeToString(SocketPlugSideType datum)
{
	return((datum == SideNone) ? "none" : (datum == SideNear) ?
		"near" : (datum == SideFar) ? "far" : "????");
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
class SocketPlugParams {
public:
	SocketPlugParams() :
		 endpoint_near_(EndPointIP(AddressIP("localhost"), 0))
		,endpoint_far_(EndPointIP(AddressIP("localhost"), 0))
		,working_dir_("./")
		,logging_dir_("./")
		,logging_flag_(false)
		,logging_data_(false)
		,single_threaded_flag_(false)
	{
	}

	EndPointIP  endpoint_near_;
	EndPointIP  endpoint_far_;
	std::string working_dir_;
	std::string logging_dir_;
	bool        logging_flag_;
	bool        logging_data_;
	bool        single_threaded_flag_;
};
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
struct SocketBuffer {
	SocketBuffer(unsigned int data_length = 0, const char *data_ptr = NULL) :
		 data_buffer_(data_ptr, data_length)
		,data_offset_(0)
	{
	}

	bool IsSendPending() const {
		return(data_offset_ < data_buffer_.size());
	}

	void AddToOffset(unsigned int add_count) {
		data_offset_ += add_count;
	}

	std::string  &GetBufferRef() {
		return(data_buffer_);
	}

	const char   *GetPtr() const {
		return(data_buffer_.c_str());
	}
	const char   *GetPtrAtOffset() const {
		return(GetPtr() + data_offset_);
	}

	unsigned int  GetLength() const {
		return(static_cast<unsigned int>(data_buffer_.size()));
	}
	unsigned int  GetLengthAtOffset() const {
		return((IsSendPending()) ? (GetLength() - data_offset_) : 0);
	}

	std::string  data_buffer_;
	unsigned int data_offset_;
};
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<SocketBuffer>          SocketBufferPtr;

typedef std::deque<SocketBufferPtr>              SocketBufferPtrSendQueue;
typedef SocketBufferPtrSendQueue::iterator       SocketBufferPtrSendQueueIter;
typedef SocketBufferPtrSendQueue::const_iterator SocketBufferPtrSendQueueIterC;
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
struct LogBufferRaw {
	LogBufferRaw() :
		 action_time_()
		,operation_(OperationNone)
		,side_(SideNone)
		,near_addr_()
//		,near_port_(0)
		,far_addr_()
//		,far_port_(0)
		,send_segment_(0)
		,data_length_(0)
		,data_offset_(0)
	{
		memset(&near_addr_, '\0', sizeof(near_addr_));
		memset(&far_addr_, '\0', sizeof(far_addr_));
	}

	LogBufferRaw(SocketPlugOperationType operation, SocketPlugSideType side,
		const EndPointIP &near_endpoint, const EndPointIP &far_endpoint,
		unsigned int send_segment, unsigned int data_length,
		unsigned int data_offset) :
		 action_time_(MLB::Utility::TimeSpec())
		,operation_(operation)
		,side_(side)
		,near_addr_(near_endpoint.GetSockaddrIn())
		,far_addr_(far_endpoint.GetSockaddrIn())
		,send_segment_(send_segment)
		,data_length_(data_length)
		,data_offset_(data_offset)
	{
	}

	void WriteToLog(MLB::Utility::OSFileMMap &data_log,
		const std::string &data_buffer) const {
		WriteToLog(data_log, *this, data_buffer);
	}
	void ReadFromLog(MLB::Utility::OSFileMMap &data_log,
		std::string &data_buffer) {
		ReadFromLog(data_log, *this, data_buffer);
	}

	static void WriteToLog(MLB::Utility::OSFileMMap &data_log,
		const LogBufferRaw &datum, const std::string &data_buffer) {
		unsigned int total_length = sizeof(datum) + datum.data_length_;
		data_log.EnsureSpaceForLengthFromOffset(total_length,
			data_log.GetInternalOffset());
		memcpy(data_log.GetPtr(data_log.GetInternalOffset()), &datum, sizeof(datum));
		data_log.SetInternalOffset(data_log.GetInternalOffset() + sizeof(datum));
		if (datum.data_length_) {
			memcpy(data_log.GetPtr(data_log.GetInternalOffset()),
				data_buffer.c_str() + datum.data_offset_, datum.data_length_);
			data_log.SetInternalOffset(data_log.GetInternalOffset() + datum.data_length_);
		}
	}

	static void ReadFromLog(MLB::Utility::OSFileMMap &data_log,
		LogBufferRaw &datum, std::string &data_buffer) {
		data_log.EnsureExtentIsMapped(sizeof(datum),
			data_log.GetInternalOffset());
		memcpy(&datum, data_log.GetPtr(data_log.GetInternalOffset()), sizeof(datum));
		if (datum.data_length_) {
			data_log.EnsureExtentIsMapped(datum.data_length_,
				data_log.GetInternalOffset() + sizeof(datum));
			data_buffer.assign(
				data_log.GetPtr_char(data_log.GetInternalOffset() + sizeof(datum)),
				datum.data_length_);
		}
	}

	std::string  ToString() const {
		std::string out_string;
		return(ToString(out_string));
	}
	std::string &ToString(std::string &out_string) const {
		std::ostringstream o_str;
		o_str <<
			action_time_           << " " <<
			OperationTypeToString(operation_)   << " " <<
			std::setw(4) << SideTypeToString(side_) << " " <<
			EndPointIP(near_addr_) << " " <<
			EndPointIP(far_addr_)  << " " <<
			std::setw(static_cast<std::streamsize>(
				MLB::Utility::IntegralValueMaxLengthDec<unsigned int>())) <<
			send_segment_          << " " <<
			std::setw(static_cast<std::streamsize>(
				MLB::Utility::IntegralValueMaxLengthDec<unsigned int>())) <<
			data_length_           << " " <<
			std::setw(static_cast<std::streamsize>(
				MLB::Utility::IntegralValueMaxLengthDec<unsigned int>())) <<
			data_offset_;
		return(out_string.assign(o_str.str()));
	}

	MLB::Utility::TimeSpec  action_time_;
	SocketPlugOperationType operation_;
	SocketPlugSideType      side_;
	NativeSockAddr_In        near_addr_;
	NativeSockAddr_In        far_addr_;
	unsigned int            send_segment_;
	unsigned int            data_length_;
	unsigned int            data_offset_;
};
inline std::ostream & operator << (std::ostream &o_str,
	const LogBufferRaw &datum)
{
	o_str <<
		datum.ToString();

	return(o_str);
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
struct LogBuffer : public LogBufferRaw {
	LogBuffer() :
		 LogBufferRaw()
		,data_buffer_(new SocketBuffer)
		,is_last_segment_(true)
	{
	}
	LogBuffer(SocketPlugOperationType operation, SocketPlugSideType side,
		const EndPointIP &near_endpoint, const EndPointIP &far_endpoint,
		unsigned int send_segment, unsigned int data_length,
		unsigned int data_offset, SocketBufferPtr data_buffer,
		bool is_last_segment) :
		 LogBufferRaw(operation, side, near_endpoint, far_endpoint, send_segment,
			data_length, data_offset)
		,data_buffer_(data_buffer)
		,is_last_segment_(is_last_segment)
	{
	}

	void WriteToLog(MLB::Utility::OSFileMMap &data_log) const {
		WriteToLog(data_log, *this);
	}
	void ReadFromLog(MLB::Utility::OSFileMMap &data_log) {
		ReadFromLog(data_log, *this);
	}

	std::string  ToString() const {
		std::string out_string;
		return(ToString(out_string));
	}
	std::string &ToString(std::string &out_string) const {
		LogBufferRaw::ToString(out_string);
		return(MLB::Utility::StringToHexDebug(data_buffer_->data_buffer_,
			out_string));
	}

	static void WriteToLog(MLB::Utility::OSFileMMap &data_log,
		const LogBuffer &datum) {
		LogBufferRaw::WriteToLog(data_log, datum,
			datum.data_buffer_->data_buffer_);
	}

	static void ReadFromLog(MLB::Utility::OSFileMMap &data_log,
		LogBuffer &datum) {
		LogBufferRaw::ReadFromLog(data_log, datum,
			datum.data_buffer_->data_buffer_);
	}

	SocketBufferPtr data_buffer_;
	bool            is_last_segment_;
};
inline std::ostream & operator << (std::ostream &o_str,
	const LogBuffer &datum)
{
	o_str <<
		datum.ToString();

	return(o_str);
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
class SocketPlugSide {
public:
	//	Use this to construct the near-side socket plug side.
	SocketPlugSide(NativeSocketHandle near_handle,
		const EndPointIP &near_endpoint, const EndPointIP &far_endpoint) :
		 is_far_(false)
		,near_endpoint_(near_endpoint)
		,far_endpoint_(far_endpoint)
		,socket_(near_handle, near_endpoint)
		,send_current_()
		,send_queue_()
	{
	}
	//	Use this to construct the far-side socket plug side.
	SocketPlugSide(const EndPointIP &far_endpoint,
		const EndPointIP &near_endpoint) :
		 is_far_(true)
		,near_endpoint_(near_endpoint)
		,far_endpoint_(far_endpoint)
		,socket_(far_endpoint)
		,send_current_()
		,send_queue_()
	{
	}

	bool IsValid() const {
		return(socket_.IsConnected());
	}

	std::string GetSideName() const {
		return((is_far_) ? "far" : "near");
	}

	void          PutSendBuffer(const SocketBufferPtr new_send_buffer) {
		send_queue_.push_back(new_send_buffer);
	}

	bool IsSendPending() {
		return(GetSendBuffer() != NULL);
	}

	SocketBufferPtr GetSendBuffer() {
		while (((send_current_ == NULL) || (!send_current_->IsSendPending())) &&
			(!send_queue_.empty())) {
			send_current_ = send_queue_.front();
			send_queue_.pop_front();
		}
		return(((send_current_ != NULL) && send_current_->IsSendPending()) ?
			send_current_ : SocketBufferPtr());
	}

	void SetOtherSidePtr(SocketPlugSide *other_side_ptr) {
		other_side_ptr_ = other_side_ptr;
	}

	void SetFD(NativeFDSet_T &recv_fd_set, NativeFDSet_T &send_fd_set) {
#ifdef _MSC_VER
# pragma warning(disable:4127)
#endif /* # ifdef _MSC_VER */
		NativeSocketHandle socket_handle = socket_.GetSocket();
		if (socket_handle != NPSL_INVALID_SOCKET) {
			FD_SET(socket_handle, &recv_fd_set);
			if (IsSendPending())
				FD_SET(socket_handle, &send_fd_set);
		}
#ifdef _MSC_VER
# pragma warning(default:4127)
#endif /* # ifdef _MSC_VER */
	}

	int RecvBuffer(NativeFDSet_T &recv_fd_set, SocketPlugSideType side);
	int SendBuffer(NativeFDSet_T &send_fd_set, SocketPlugSideType side);

	bool                      is_far_;
	const EndPointIP         &near_endpoint_;	//	For convenience
	const EndPointIP         &far_endpoint_;	//	For convenience
	SocketX                   socket_;
	SocketPlugSide           *other_side_ptr_;
	SocketBufferPtr           send_current_;
	SocketBufferPtrSendQueue  send_queue_;

private:
	MLB_Utility_NonCopyable_Macro(SocketPlugSide);
};
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
int SocketPlugSide::RecvBuffer(NativeFDSet_T &recv_fd_set,
	SocketPlugSideType side)
{
//	unsigned int    recv_bytes;
//	int             npsl_code;
//	struct timespec action_time;
//	char            inet_buffer[NPSL_MAX_IPV4_ADDR_STR + 1];
//	char            npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (!other_side_ptr_->IsValid())
		return(0);

	if (socket_.GetSocket() == NPSL_INVALID_SOCKET)
		return(0);

	if (!FD_ISSET(socket_.GetSocket(), &recv_fd_set))
		return(0);

	try {
		socket_.SetBlockingModeOff();
		try {
//			char          data_buffer[8192];
//			unsigned int  data_length = sizeof(data_buffer);
			unsigned int  byte_count;
			SocketBuffer  recv_buffer;
			byte_count = socket_.Recv(recv_buffer.GetBufferRef());
/*
			if (!byte_count)
				throw SockLibXExceptionError("Connection to " +
					socket_.GetEndPointIP().ToString() + " broken detected "
					"on receive attempt.");
*/
			if (!byte_count) {
				std::cerr << "Connection to " + socket_.GetEndPointIP().ToString() +
					" broken detected on receive attempt." << std::endl;
				socket_.Disconnect();
			}
LogBuffer log_buffer(OperationRecv, side,
	near_endpoint_, far_endpoint_, 0, byte_count, 0,
	SocketBufferPtr(new SocketBuffer(recv_buffer)), true);
std::cout << log_buffer << std::endl;
			other_side_ptr_->PutSendBuffer(SocketBufferPtr(
				new SocketBuffer(recv_buffer)));
		}
		catch (...) {
			try {
				socket_.SetBlockingModeOn();
			}
			catch (...) {
			}
			throw;
		}
		socket_.SetBlockingModeOn();
/*
	CODE NOTE: Fix me!!!
		if (npsl_code == NPSL_EWOULDBLOCK)
			return(SOCKICPT_SUCCESS);
*/
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to complete a " +
			GetSideName() + "-side receive: " + std::string(except.what()));
	}

	return(0);
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
int SocketPlugSide::SendBuffer(NativeFDSet_T &send_fd_set,
	SocketPlugSideType side)
{
//	unsigned int    send_bytes;
//	unsigned int    sent_offset;
//	int             npsl_code;
//	struct timespec action_time;
//	char            inet_buffer[NPSL_MAX_IPV4_ADDR_STR + 1];
//	char            npsl_error_text[NPSL_MAX_ERROR_TEXT];

	if (socket_.GetSocket() == NPSL_INVALID_SOCKET)
		return(0);

	SocketBufferPtr send_buffer_ptr = GetSendBuffer();

	if ((send_buffer_ptr == NULL) ||
		(!FD_ISSET(socket_.GetSocket(), &send_fd_set)))
		return(0);

	try {
		socket_.SetBlockingModeOff();
		try {
			unsigned int byte_count;
			byte_count = socket_.Send(send_buffer_ptr->GetPtrAtOffset(),
				send_buffer_ptr->GetLengthAtOffset());
			send_buffer_ptr->AddToOffset(byte_count);
LogBuffer log_buffer(OperationSend, side,
	near_endpoint_, far_endpoint_, 0, byte_count, 0,
	send_buffer_ptr, true);
std::cout << log_buffer << std::endl;
		}
		catch (...) {
			try {
				socket_.SetBlockingModeOn();
			}
			catch (...) {
			}
			throw;
		}
		socket_.SetBlockingModeOn();
/*
	CODE NOTE: Fix me!!!
		if (npsl_code == NPSL_EWOULDBLOCK)
			return(SOCKICPT_SUCCESS);
*/
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to complete a " +
			GetSideName() + "-side send: " + std::string(except.what()));
	}

	return(0);
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<SocketPlugSide> SocketPlugSidePtr;
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
class SocketPlug {
	typedef boost::shared_ptr<SocketPlugSide> SocketPlugSidePtr;
public:
	SocketPlug(const SocketPlugParams &plug_params,
		NativeSocketHandle near_handle) :
		 is_ready_(false)
		,plug_params_(plug_params)
		,near_handle_(near_handle)
		,near_side_ptr_()
		,far_side_ptr_()
		,data_log_ptr_()
	{
	}

	void Start();

	bool                                        is_ready_;
	SocketPlugParams                            plug_params_;
	NativeSocketHandle                          near_handle_;
	SocketPlugSidePtr                           near_side_ptr_;
	SocketPlugSidePtr                           far_side_ptr_;
	boost::shared_ptr<MLB::Utility::OSFileMMap> data_log_ptr_;
};
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
void SocketPlug::Start()
{
	if (is_ready_)
		MLB::Utility::ThrowStdException("Socket plug is already started.");

	try {
		near_side_ptr_.reset(new SocketPlugSide(near_handle_,
			plug_params_.endpoint_near_, plug_params_.endpoint_far_));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create the near-side socket: " +
			std::string(except.what()));
	}

	try {
		far_side_ptr_.reset(new SocketPlugSide(plug_params_.endpoint_far_,
			plug_params_.endpoint_near_));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create the far-side socket: " +
			std::string(except.what()));
	}

	plug_params_.endpoint_near_ = near_side_ptr_->socket_.GetEndPointIP();
	plug_params_.endpoint_far_  = far_side_ptr_->socket_.GetEndPointIP();

	if (plug_params_.logging_data_) {
		try {
			std::string log_time(MLB::Utility::TimeT().ToString());
			log_time[4 + 1 + 2 + 1 + 2]                 = '_';
			log_time[4 + 1 + 2 + 1 + 2 + 1 + 2]         = '.';
			log_time[4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2] = '.';
			std::string data_log_name(log_time + "." +
				"SocketZPlug." + MLB::Utility::GetHostNameCanonical() + "." +
				MLB::Utility::AnyToString(MLB::Utility::CurrentProcessId()) + "." +
				plug_params_.endpoint_near_.GetHostIPAddressZeroFilled() + "-" +
				MLB::Utility::ZeroFill(plug_params_.endpoint_near_.GetPort(),
					MLB::Utility::IntegralValueMaxLengthDec<unsigned short>()) +
				"." +
				plug_params_.endpoint_far_.GetHostIPAddressZeroFilled() + "-" +
				MLB::Utility::ZeroFill(plug_params_.endpoint_far_.GetPort(),
					MLB::Utility::IntegralValueMaxLengthDec<unsigned short>()) +
				".log");
data_log_name="XXX.log";
			data_log_ptr_.reset(new MLB::Utility::OSFileMMap(data_log_name));
			data_log_ptr_->Open(data_log_name, false, false);
			data_log_ptr_->SetTruncateToOffsetOnClose(true);
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Unable to create the far-side "
				"socket: " + std::string(except.what()));
		}
	}

	near_side_ptr_->SetOtherSidePtr(far_side_ptr_.get());
	far_side_ptr_->SetOtherSidePtr(near_side_ptr_.get());

	NativeSocketHandle max_socket_handle =
		std::max(near_side_ptr_->socket_.GetSocket(),
		far_side_ptr_->socket_.GetSocket());

	NativeFDSet_T recv_fd_set;
	NativeFDSet_T send_fd_set;

	while (near_side_ptr_->IsValid() || far_side_ptr_->IsValid()) {
		FD_ZERO(&recv_fd_set);
		FD_ZERO(&send_fd_set);
		near_side_ptr_->SetFD(recv_fd_set, send_fd_set);
		far_side_ptr_->SetFD(recv_fd_set, send_fd_set);
		MLB::Utility::TimeVal time_out(0, 1);
		unsigned int ready_fd_count = Select(max_socket_handle + 1,
			&recv_fd_set, &send_fd_set, NULL, &time_out);
		if (MLB::Utility::CriticalEventTest())
			break;
		if (ready_fd_count) {
			if (near_side_ptr_->IsValid()) {
				near_side_ptr_->SendBuffer(send_fd_set, SideNear);
				near_side_ptr_->RecvBuffer(recv_fd_set, SideNear);
			}
			if (far_side_ptr_->IsValid()) {
				far_side_ptr_->RecvBuffer(recv_fd_set, SideFar);
				far_side_ptr_->SendBuffer(send_fd_set, SideFar);
			}
			if (MLB::Utility::CriticalEventTest())
				break;
			if ((!near_side_ptr_->IsValid()) && (!far_side_ptr_->IsSendPending()))
				break;
			if ((!far_side_ptr_->IsValid()) && (!near_side_ptr_->IsSendPending()))
				break;
		}
	}
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
class SocketPlugControl : public SocketPlugParams {
public:
	SocketPlugControl() :
		 SocketPlugParams()
		,socket_lib_()
	{
	}

	void ParseCmdLine(int argc, char **argv);
	void Start();

	SockLibXContext socket_lib_;
};
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
void SocketPlugControl::ParseCmdLine(int argc, char **argv)
{
	unsigned int count_1;
	std::string  tmp_string;

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (SockLibXParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-PORT")
			("-NEAR_PORT")
			("-NEARPORT")
			("-SERVICE")
			("-NEAR_SERVICE")
			("-NEARSERVICE")
			("-SERV")
			("-NEAR_SERV")
			("-NEARSERV"),
			count_1, argc, argv, tmp_string)) {
			try {
				int  tmp_port;
				char npsl_error_text[NPSL_MAX_ERROR_TEXT];
				if (NPSL_ResolveServToPort(tmp_string.c_str(), NULL,
					&tmp_port, npsl_error_text) != NPSL_SUCCESS)
					throw SockLibXException(npsl_error_text);
				endpoint_near_.SetPort(tmp_port);
			}
			catch (const std::exception &except) {
				MLB::Utility::ThrowInvalidArgument("Unable to parse the '" +
					std::string(argv[count_1 - 1]) + "' parameter: " +
					std::string(except.what()));
			}
		}
		else if (SockLibXParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-FAR_PORT")
			("-FARPORT")
			("-FAR_SERVICE")
			("-FARSERVICE")
			("-FAR_SERV")
			("-FARSERV"),
			count_1, argc, argv, tmp_string)) {
			try {
				int  tmp_port;
				char npsl_error_text[NPSL_MAX_ERROR_TEXT];
				if (NPSL_ResolveServToPort(tmp_string.c_str(), NULL,
					&tmp_port, npsl_error_text) != NPSL_SUCCESS)
					throw SockLibXException(npsl_error_text);
				endpoint_far_.SetPort(tmp_port);
			}
			catch (const std::exception &except) {
				MLB::Utility::ThrowInvalidArgument("Unable to parse the '" +
					std::string(argv[count_1 - 1]) + "' parameter: " +
					std::string(except.what()));
			}
		}
		else if (SockLibXParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-FAR_HOST_NAME")
			("-FAR_HOSTNAME")
			("-FARHOST_NAME")
			("-FARHOSTNAME")
			("-FARHOST")
			("-FAR_HOST")
			("-FARHOST"),
			count_1, argc, argv, tmp_string)) {
			try {
				char tmp_host_name[MLB::Utility::MaxHostNameLength + 1];
				char npsl_error_text[NPSL_MAX_ERROR_TEXT];
				if (NPSL_ResolveHostToName(tmp_string.c_str(), tmp_host_name,
					npsl_error_text) != NPSL_SUCCESS)
				throw SockLibXException(npsl_error_text);
				endpoint_far_.SetAddress(tmp_host_name);
			}
			catch (const std::exception &except) {
				MLB::Utility::ThrowInvalidArgument("Unable to parse the '" +
					std::string(argv[count_1 - 1]) + "' parameter: " +
					std::string(except.what()));
			}
		}
		else if (SockLibXParseCmdLineArg::ParseWorkingDir(count_1, argc, argv,
			working_dir_, "", true))
			;
		else if (SockLibXParseCmdLineArg::ParseLoggingDir(count_1, argc, argv,
			logging_dir_, "", true))
			;
		else if (SockLibXParseCmdLineArg::ParseLoggingFlag(count_1, argc, argv,
			logging_flag_))
			;
		else if (SockLibXParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-SINGLE_THREADING")
				("-SINGLETHREADING")
				("-SINGLE_THREADED")
				("-SINGLETHREADED")
				("-SINGLE_THREAD")
				("-SINGLETHREAD"),
			count_1, argc, argv, single_threaded_flag_))
			;
		else if (SockLibXParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
			("-LOGGING_DATA")
			("-LOGGINGDATA")
			("-LOG_DATA")
			("-LOGDATA"),
			count_1, argc, argv, logging_data_))
			;
		else
			SockLibXParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	//	Port must be specified for the near side...
	if (endpoint_near_.GetPort() == 0)
		MLB::Utility::ThrowInvalidArgument("Invalid near-side port of '0'.");

	// If the far port isn't specified, make it the same as the near port...
	if (endpoint_far_.GetPort() == 0)
		endpoint_far_.SetPort(endpoint_near_.GetPort());

	//	The completely-specified near and far endpoints must differ...
	if (endpoint_near_ == endpoint_far_)
		MLB::Utility::ThrowInvalidArgument("Near-side and far-side ports "
			"specified (" + MLB::Utility::AnyToString(endpoint_near_.GetPort()) +
			") may not be specified as identical when the near-side and far-side "
			"hosts are also identical (" + endpoint_near_.GetAddress().ToString() +
			").");
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
void SocketPlugControl::Start()
{
	try {
		std::cout << "Attempting to start the server on port " <<
			endpoint_near_.GetPort() << std::flush;
		SocketX my_socket(AddressFamily_Internet, SocketType_Stream,
			ProtocolType_DEFAULT);
		NativeSockAddr_In this_address;
		memset(&this_address, '\0', sizeof(this_address));
		this_address.sin_family      = AF_INET;
		this_address.sin_port        = htons(((unsigned short) endpoint_near_.GetPort()));
		this_address.sin_addr.s_addr = INADDR_ANY;
		my_socket.Bind(reinterpret_cast<const NativeSockAddr *>(&this_address),
			sizeof(this_address));
		std::cout << " --- listening for connections..." << std::endl;
		my_socket.Listen(200);
		bool accept_flag = true;
		while (accept_flag) {
			MLB::Utility::TimeVal time_out(0, 100 * 1000);	//	Sleep for 100 ms.
			while ((!my_socket.SelectRead(&time_out)) &&
				(!MLB::Utility::CriticalEventTest()))
				;
			if (MLB::Utility::CriticalEventTest()) {
				std::cout << "Exiting because a signal was received." << std::endl;
				break;
			}
			NativeSocketHandle client_handle = NPSL_INVALID_SOCKET;
			NativeSockAddr      client_addr;
			NativeSockLen_T     client_addrlen;
			client_addrlen = sizeof(client_addr);
			client_handle = my_socket.Accept(&client_addr, &client_addrlen);
			std::cout << "Connection from " << EndPointIP(client_addr) <<
				" started." << std::endl;
			try {
				SocketPlug this_plug(*this, client_handle);
				this_plug.Start();
			}
			catch (const std::exception &except) {
				std::cerr << "Error occurred while processing new socket "
					"connection: " << except.what() << std::endl;
			}
			std::cout << "Connection from " << EndPointIP(client_addr) <<
				" ended." << std::endl;
/*
			my_socket.Accept(&client_handle, &client_addr, &client_addrlen);
			Socket      client_socket(client_handle, client_addr);
			EndPointIP  client_endpoint(client_addr);
			std::string client_name(client_endpoint.ToString());
			std::cout << "*[" << client_name <<
				"]: *** Connection from client " << client_socket.GetPeerName() <<
				std::endl;
			client_socket.SetBlockingModeOff();
			bool recv_flag = true;
			MLB::Utility::TimeVal expiration_time;
			expiration_time.tv_sec += 10;
			struct timeval recv_time_out = { 1, 0 };
			while (recv_flag) {
				if (MLB::Utility::CriticalEventTest())
					break;
				try {
					if (client_socket.SelectWaitOnRead(&recv_time_out)) {
						std::string recv_buffer;
						if (client_socket.Recv(recv_buffer, 32767)) {
							std::cout << " [" << client_name << "]: " <<
								recv_buffer;
							if (recv_buffer[recv_buffer.size() - 1] != '\n')
								std::cout << std::endl;
							expiration_time         = MLB::Utility::TimeVal();
							expiration_time.tv_sec += 10;
						}
						else {
							std::cout << "*[" << client_name << "]: " <<
								"*** Client closed socket --- disconnecting." <<
								std::endl;
							recv_flag = false;
						}							
					}
					else if (expiration_time <= MLB::Utility::TimeVal()) {
						std::cout << "*[" << client_name << "]: " <<
							"*** Client idle time expiration --- disconnecting." <<
							std::endl;
						recv_flag = false;
					}
					else
						Sleep(10);
				}
				catch (const std::exception &except) {
					std::cout << "*[" << client_name << "]: " <<
						"*** Client error: " << except.what() << std::endl;
					recv_flag = false;
				}
			}
*/
		}
	}
	catch (std::exception &except) {
		MLB::Utility::Rethrow(except, "SocketZPlug fatal error: " +
			std::string(except.what()));
	}
}
//	//////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

using namespace MLB::SockLibX;

//	//////////////////////////////////////////////////////////////////////////
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
			"-near_port <near-port-or-service>" <<
			"[ -far_port <far-port-or-service> ] " <<
			"[ -far_host <far-host-name-or-IP-address> ( = localhost ) ]" <<
			"[ -working_dir <directory-path-to-make-current> ( = " <<
				SocketPlugParams().working_dir_ << " ) ] " <<
			"[ -logging_dir <directory-path-for-log-files> ( = " <<
				SocketPlugParams().logging_dir_ << " ) ] " <<
			"[ -logging_flag <boolean> ( = " <<
				MLB::Utility::AnyToString(SocketPlugParams().logging_flag_) <<
				" ) ] " <<
			"[ -logging_data <boolean> ( = " <<
				MLB::Utility::AnyToString(SocketPlugParams().logging_data_) <<
				" ) ] " <<
			"[ -single_threaded <boolean> ( = " <<
				MLB::Utility::AnyToString(SocketPlugParams().
				single_threaded_flag_) << " ) ]" <<
         std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		SockLibXContext                    socket_lib_context;
		SocketPlugControl                  plug_control;
		plug_control.ParseCmdLine(argc, argv);
		plug_control.Start();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << my_name << ": fatal error encountered: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	//////////////////////////////////////////////////////////////////////////

