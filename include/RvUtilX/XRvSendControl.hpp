// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the XRvSendControl class.

	Revision History	:	2005-07-04 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__XRvSendControl_hpp__HH

#define HH__MLB__XRvSendControl_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
class XRvSendControl {
public:
	XRvSendControl();

	bool CheckCmdLineForHelpRequest(int argc, char **argv);
	void ParseCmdLine(int argc, char **argv);
	void LoadFile(const std::string &file_name);
	void SendMessage();

	std::string  my_name_;
	std::string  field_name_;
	tibrv_u16    field_id_;
	tibrv_u8     field_type_;
	std::string  subject_name_;
	std::string  reply_subject_;
	bool         wait_for_reply_flag_;
	unsigned int reply_delay_;
	std::string  field_data_;
	bool         file_flag_;
	SNDSpec      snd_spec_;
	unsigned int repeat_count_;
	unsigned int delay_ms_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // MLB

#endif // #ifndef HH__MLB__XRvSendControl_hpp__HH

