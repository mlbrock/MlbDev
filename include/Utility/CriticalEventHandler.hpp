// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the MLB critical event support logic.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__CriticalEventHandler_hpp__HH

#define HH__MLB__Utility__CriticalEventHandler_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file CriticalEventHandler.hpp

	\brief	The MLB critical event support logic header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/AtomicTypes.hpp>
#include <Utility/SignalHandler.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY void CriticalEventClear();
API_UTILITY void CriticalEventSet();
API_UTILITY bool CriticalEventTest();
API_UTILITY void CriticalEventInstallSignal(unsigned int sig_count,
	const int *sig_list, SignalInstallDefaultData &signal_data);
API_UTILITY void CriticalEventInstallSignal(unsigned int sig_count,
	const int *sig_list);
API_UTILITY void CriticalEventInstallSignal(int signal_id);
API_UTILITY void CriticalEventInstallSignal(int signal_id,
	SignalInstallDefaultData &signal_data);
API_UTILITY void CriticalEventInstallDefault();
API_UTILITY void CriticalEventInstall(SignalInstallDefaultData &signal_data);
API_UTILITY void CriticalEventUninstall(SignalInstallDefaultData &signal_data);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY CriticalEventContext : public SignalInstallDefaultData {
	CriticalEventContext() :
		SignalInstallDefaultData() {
		CriticalEventInstall(*this);
	}
	virtual ~CriticalEventContext() {
		CriticalEventUninstall(*this);
	}
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__CriticalEventHandler_hpp__HH

