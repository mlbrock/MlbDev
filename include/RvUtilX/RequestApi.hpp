// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the ReplyInfoRv and the
								RequestApiRv classes.

	Revision History	:	2006-09-21 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__RvUtilX__RequestApi_hpp__HH

#define HH__RvUtilX__RequestApi_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/RequestApi.hpp>

#include <RvUtilX.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_ptr.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

#ifdef _Windows
# pragma warning(disable:4265 4275)
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////
struct API_RVUTILX ReplyInfoRvCb : public TibrvMsgCallback {
	ReplyInfoRvCb()
		:TibrvMsgCallback()
		,done_flag_(false)
		,success_flag_(false)
		,msg_recv_()
		,error_text_()
	{
	}

	void onMsg(TibrvListener *, TibrvMsg &msg_recv) {
		try {
			done_flag_ = true;
			RvUtilX_THROW_TIBRV_STATUS_IF(msg_recv.createCopy,
				(msg_recv_));
			success_flag_ = true;
		}
		catch (const std::exception &except) {
			success_flag_ = false;
			error_text_   = except.what();
		}
		catch (...) {
			success_flag_ = false;
			error_text_   = "Unspecified non-std::exception thrown.";
		}
	}

	void Reset() {
		msg_recv_.reset();
		done_flag_    = false;
		success_flag_ = false;
		error_text_.clear();
	}

	bool        done_flag_;
	bool        success_flag_;
#ifdef _Windows
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	TibrvMsg    msg_recv_;
#ifdef _Windows
# pragma warning(default:4251)
#endif // #ifdef _Windows
	std::string error_text_;

private:
	MLB_Utility_NonCopyable_Macro(ReplyInfoRvCb);
};
// ////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
# pragma warning(default:4265 4275)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<ReplyInfoRvCb> ReplyInfoRvCbSPtr;
//	////////////////////////////////////////////////////////////////////////////

#ifdef _Windows
# pragma warning(disable:4275)
#endif // #ifdef _Windows
//	////////////////////////////////////////////////////////////////////////////
class API_RVUTILX ReplyInfoRv :
	public MLB::Utility::ReplyInfoUtility<TibrvMsg, TibrvMsg> {
public:
	ReplyInfoRv();
	ReplyInfoRv(boost::shared_ptr<TibrvNetTransport> &transport_ptr,
		const char *recv_subject);
	ReplyInfoRv(boost::shared_ptr<TibrvNetTransport> &transport_ptr,
		const std::string &recv_subject);
	ReplyInfoRv(boost::shared_ptr<TibrvNetTransport> &transport_ptr,
		const TibrvMsg &msg_sent);

	ReplyInfoRv(const ReplyInfoRv &other);
	ReplyInfoRv & operator = (const ReplyInfoRv &other);
	void swap(ReplyInfoRv &other);

	virtual void         Check() const;

	virtual bool         GetDoneFlag() const;
	virtual bool         GetSuccessFlag() const;
	virtual std::string  GetErrorText() const;
	virtual TibrvMsg    &GetRvMessage(TibrvMsg &rv_msg) const;

	void SendRvMessage(const TibrvMsg &rv_msg);

	boost::shared_ptr<TibrvNetTransport> GetRvTransportSPtr() const;
	boost::shared_ptr<TibrvQueue>        GetRvQueueSPtr() const;
	boost::shared_ptr<TibrvListener>     GetRvListenerSPtr() const;

protected:
	bool DoRecvImpl(TibrvMsg &recv_data, int wait_ms = 0,
		bool throw_on_time_out_flag = false) const;
	void ResetRecvImpl();

private:
#ifdef _Windows
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	boost::shared_ptr<RvContext>         rv_context_ptr_;
	boost::shared_ptr<TibrvNetTransport> transport_ptr_;
	boost::shared_ptr<TibrvQueue>        queue_ptr_;
	boost::shared_ptr<TibrvListener>     listener_ptr_;
	ReplyInfoRvCbSPtr                    callback_ptr_;
#ifdef _Windows
# pragma warning(default:4251)
#endif // #ifdef _Windows

	void InitializeListen(const char *recv_subject);
};
//	////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
# pragma warning(default:4251)
#endif // #ifdef _Windows

#ifdef _Windows
# pragma warning(disable:4275)
#endif // #ifdef _Windows
//	////////////////////////////////////////////////////////////////////////////
/**
	\brief RequestApiRv manages communications with a Tib/Rendezvous server.
*/
class API_RVUTILX RequestApiRv :
	public MLB::Utility::RequestApiUtility<ReplyInfoRv> {
public:
	RequestApiRv(const std::string &service = "",
		const std::string &network = "", const std::string &daemon = "");
	RequestApiRv(const SNDSpec &snd_spec);

	RequestApiRv(const RequestApiRv &other);
	RequestApiRv & operator = (const RequestApiRv &other);
	void swap(RequestApiRv &other);

	void SendMsg(TibrvMsg &msg_send, TibrvMsg &msg_recv,
		const std::string &send_subject, const std::string &recv_subject,
		int wait_ms = 0);
	ReplyInfoRv SendMsg(TibrvMsg &msg_send, const std::string &send_subject,
		const std::string &recv_subject);

	SNDSpec GetSNDSpec() const;
	SNDSpec ChangeTransport(const std::string &service = "",
		const std::string &network = "", const std::string &daemon = "");
	SNDSpec ChangeTransport(const SNDSpec &snd_spec);

	boost::shared_ptr<TibrvNetTransport> GetTransportPtr() const {
		return(transport_ptr_);
	}

	virtual void SetTransportDescription(
		const std::string &transport_description = "");
	virtual void SetTransportDescription(TibrvNetTransport *transport_ptr,
		const std::string &transport_description = "");

	virtual void        SetTransportDescriptionPrefix(const std::string &prefix);
	virtual std::string GetTransportDescriptionPrefix() const;

	static std::string &GetDescriptionBasicId(std::string &out_string);
	static std::string  GetDescriptionBasicId();

protected:
#ifdef _Windows
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	boost::shared_ptr<RvContext>         rv_context_ptr_;
#ifdef _Windows
# pragma warning(default:4251)
#endif // #ifdef _Windows
	MLB::RvUtilX::SNDSpec                snd_spec_;
#ifdef _Windows
# pragma warning(disable:4251)
#endif // #ifdef _Windows
	boost::shared_ptr<TibrvNetTransport> transport_ptr_;
#ifdef _Windows
# pragma warning(default:4251)
#endif // #ifdef _Windows
	std::string                          transport_name_prefix_;

	void Initialize();

	ReplyInfoRv DoSendImpl(const TibrvMsg &send_data);
};
//	////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
# pragma warning(default:4251)
#endif // #ifdef _Windows

} // namespace RvUtilX

} // namespace MLB

#endif // #ifndef HH__RvUtilX__RequestApi_hpp__HH

