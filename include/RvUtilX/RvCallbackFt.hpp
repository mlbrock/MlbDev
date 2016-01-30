// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the RvUtilX callback template classes
								for use with fault-tolerance callbacks.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RvUtilX_RvCallbackFt_hpp__HH

#define HH__MLB__RvUtilX_RvCallbackFt_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
template <typename ControlBaseClass> class RvCallbackFtActionEvent :
	public TibrvFtMemberCallback {
public:
	RvCallbackFtActionEvent(ControlBaseClass *base_ptr) :
		 base_ptr_(base_ptr) { }
	RvCallbackFtActionEvent(ControlBaseClass &base_ref) :
		 base_ptr_(&base_ref) { }
	virtual ~RvCallbackFtActionEvent() { }

	virtual void onFtAction(TibrvFtMember *ftMember, const char *groupName,
		tibrvftAction action) {
		try {
			try {
				PerformControlCallback(ftMember, groupName, action);
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
	virtual void PerformControlCallback(TibrvFtMember *ftMember,
		const char *groupName, tibrvftAction action) {
		base_ptr_->ProcessEventOnFtAction(this, ftMember, groupName, action);
	}

protected:
	ControlBaseClass *base_ptr_;

private:
	MLB_Utility_NonCopyable_Macro(RvCallbackFtActionEvent);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ControlBaseClass> class RvCallbackFtMonitorEvent :
	public TibrvFtMonitorCallback {
public:
	RvCallbackFtMonitorEvent(ControlBaseClass *base_ptr) :
		 base_ptr_(base_ptr) { }
	RvCallbackFtMonitorEvent(ControlBaseClass &base_ref) :
		 base_ptr_(&base_ref) { }
	virtual ~RvCallbackFtMonitorEvent() { }

	virtual void ononFtMonitor(TibrvFtMonitor *ftMonitor, const char *groupName,
		tibrv_u32 numActiveMembers) {
		try {
			try {
				PerformControlCallback(ftMonitor, groupName, numActiveMembers);
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
	virtual void PerformControlCallback(TibrvFtMonitor *ftMonitor,
		const char *groupName, tibrv_u32 numActiveMembers) {
		base_ptr_->ProcessEventOnFtMonitor(this, ftMonitor, groupName,
			numActiveMembers);
	}

protected:
	ControlBaseClass *base_ptr_;

private:
	MLB_Utility_NonCopyable_Macro(RvCallbackFtMonitorEvent);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilx

} // namespace MLB

#endif // #ifndef HH__MLB__RvUtilX_RvCallbackFt_hpp__HH

