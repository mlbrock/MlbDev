// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the RequestApiUtility class.

	Revision History	:	2006-09-21 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__Utility_RequestApi_hpp__HH

#define HH__MLB__Utility__Utility_RequestApi_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class BaseReplyInfoUtility {
protected:
	BaseReplyInfoUtility()
	{
	}
	virtual ~BaseReplyInfoUtility()
	{
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename MySendType, typename MyRecvType>
	class ReplyInfoUtility : BaseReplyInfoUtility {
public:
	ReplyInfoUtility()
		:BaseReplyInfoUtility()
	{
	}

	typedef MySendType SendType;
	typedef MyRecvType RecvType;

	bool DoRecv(RecvType &recv_data, int wait_ms = 0,
		bool throw_on_time_out_flag = false) const {
		return(DoRecvImpl(recv_data, wait_ms, throw_on_time_out_flag));
	}
	void ResetRecv() {
		ResetRecvImpl();
	}

protected:
	virtual bool DoRecvImpl(RecvType &recv_data, int wait_ms = 0,
		bool throw_on_time_out_flag = false) const = 0;
	virtual void ResetRecvImpl() {
		MLB::Utility::ThrowLogicError("A type derived from 'MLB::Utility::"
			"ReplyInfoUtility' does not implement method 'ResetRecvImpl()'.");
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class BaseRequestApiUtility {
protected:
	BaseRequestApiUtility()
	{
	}
	virtual ~BaseRequestApiUtility()
	{
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename MyReplyInfoType>
	class RequestApiUtility : BaseRequestApiUtility {
public:
	RequestApiUtility()
		:BaseRequestApiUtility()
	{
	}

	typedef          MyReplyInfoType           ReplyInfoType;
	typedef typename MyReplyInfoType::SendType SendType;
	typedef typename MyReplyInfoType::RecvType RecvType;

	ReplyInfoType DoSend(const SendType &send_data) {
		return(DoSendImpl(send_data));
	}

protected:
	virtual ReplyInfoType DoSendImpl(const SendType &send_data) = 0;
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility_RequestApi_hpp__HH

