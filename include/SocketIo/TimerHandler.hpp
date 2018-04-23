//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class TimerHandler.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__TimerHandler_hpp__HH

#define HH__MLB__SocketIo__TimerHandler_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/BoostAsioInclude.hpp>

#include <Utility/TimeSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class TimerHandler {
public:
	TimerHandler(boost::asio::io_service &io, unsigned int gran_ticks,
		unsigned int gran_usecs = 100000);
	TimerHandler(bool immediate_start, boost::asio::io_service &io,
		unsigned int gran_ticks, unsigned int gran_usecs = 100000);
	virtual ~TimerHandler();

	bool IsRunning() const;
	bool Run();
	void TimerEvent(const boost::system::error_code &asio_code);

protected:	
	unsigned long long          counter_;
	boost::asio::deadline_timer timer_;
	unsigned int                gran_ticks_;
	unsigned int                gran_usecs_;
	MLB::Utility::TimeSpec      start_time_;
	bool                        is_running_;

	virtual bool RunImpl();
	virtual bool TimerEventImpl(const boost::system::error_code &asio_code);

private:
	static unsigned long long CheckGranularity(unsigned int gran_ticks,
		unsigned int gran_usecs);

	TimerHandler(const TimerHandler &other);
	TimerHandler operator = (const TimerHandler &);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__TimerHandler_hpp__HH

