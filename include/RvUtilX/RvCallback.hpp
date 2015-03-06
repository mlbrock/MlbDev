// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the RvUtilX callback template classes.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_RvCallback_hpp__HH

#define HH__MLB__RvUtilX_RvCallback_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
template <typename ControlBaseClass> class RvCallbackMsgEvent :
	public TibrvMsgCallback {
public:
	RvCallbackMsgEvent(ControlBaseClass *base_ptr) :
		 base_ptr_(base_ptr) { }
	RvCallbackMsgEvent(ControlBaseClass &base_ref) :
		 base_ptr_(&base_ref) { }
	virtual ~RvCallbackMsgEvent() { }

	virtual void onMsg(TibrvListener *listener, TibrvMsg &msg_recv) {
		try {
			try {
				PerformControlCallback(listener, msg_recv);
			}
			catch (const std::exception &except) {
				base_ptr_->ReportException(&except);
			}
			catch (...) {							//	Safety net.
				base_ptr_->ReportException(NULL);
			}
		}
		catch (...) {
			;	//	If an exception reporting method throws, swallow it here.
		}
	}

	//	Re-implement this method in derived classes to call a different method
	//	in the control class instance...
	virtual void PerformControlCallback(TibrvListener *listener,
		TibrvMsg &msg_recv) {
		base_ptr_->ProcessEventOnMsg(this, listener, msg_recv);
	}

protected:
	ControlBaseClass *base_ptr_;

private:
	MLB_Utility_NonCopyable_Macro(RvCallbackMsgEvent);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ControlBaseClass> class RvCallbackTimerEvent :
	public TibrvTimerCallback {
public:
	RvCallbackTimerEvent(ControlBaseClass *base_ptr) :
		 base_ptr_(base_ptr) { }
	RvCallbackTimerEvent(ControlBaseClass &base_ref) :
		 base_ptr_(&base_ref) { }
	virtual ~RvCallbackTimerEvent() { }

	virtual void onTimer(TibrvTimer *in_timer) {
		try {
			try {
				PerformControlCallback(in_timer);
			}
			catch (const std::exception &except) {
				base_ptr_->ReportException(&except);
			}
			catch (...) {							//	Safety net.
				base_ptr_->ReportException(NULL);
			}
		}
		catch (...) {
			;	//	If an exception reporting method throws, swallow it here.
		}
	}

	//	Re-implement this method in derived classes to call a different method
	//	in the control class instance...
	virtual void PerformControlCallback(TibrvTimer *in_timer) {
		base_ptr_->ProcessEventOnTimer(this, in_timer);
	}

protected:
	ControlBaseClass *base_ptr_;

private:
	MLB_Utility_NonCopyable_Macro(RvCallbackTimerEvent);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ControlBaseClass> class RvCallbackIOEvent :
	public TibrvIOCallback {
public:
	RvCallbackIOEvent(ControlBaseClass *base_ptr) :
		 base_ptr_(base_ptr) { }
	RvCallbackIOEvent(ControlBaseClass &base_ref) :
		 base_ptr_(&base_ref) { }
	virtual ~RvCallbackIOEvent() { }

	virtual void onIOEvent(TibrvIOEvent *io_event) {
		try {
			try {
				PerformControlCallback(io_event);
			}
			catch (const std::exception &except) {
				base_ptr_->ReportException(&except);
			}
			catch (...) {							//	Safety net.
				base_ptr_->ReportException(NULL);
			}
		}
		catch (...) {
			;	//	If an exception reporting method throws, swallow it here.
		}
	}

	//	Re-implement this method in derived classes to call a different method
	//	in the control class instance...
	virtual void PerformControlCallback(TibrvIOEvent *in_timer) {
		base_ptr_->ProcessEventOnIO(this, in_timer);
	}

protected:
	ControlBaseClass *base_ptr_;

private:
	MLB_Utility_NonCopyable_Macro(RvCallbackIOEvent);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvCallback_hpp__HH

