//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class TimerHandlerFwd.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__TimerHandlerFwd_hpp__HH

#define HH__MLB__SocketIo__TimerHandlerFwd_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/TimerHandler.hpp>

#include <Utility/CriticalEventHandler.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
template <typename FwdTarget>
	class TimerHandlerFwd : public MLB::SocketIo::TimerHandler {
public:
	TimerHandlerFwd(const FwdTarget &fwd_func,
		boost::asio::io_service &io_service, unsigned int gran_ticks,
		unsigned int gran_usecs = 100000)
		:MLB::SocketIo::TimerHandler(io_service, gran_ticks, gran_usecs)
		,fwd_func_(fwd_func)
	{
	}
	virtual ~TimerHandlerFwd()
	{
	}

protected:
	virtual bool TimerEventImpl(const boost::system::error_code &asio_code)
	{
		return((!(++counter_ % gran_ticks_)) ?
			fwd_func_(*this, asio_code) : true);
	}

private:
	FwdTarget fwd_func_;

	TimerHandlerFwd(const TimerHandlerFwd &other);
	TimerHandlerFwd & operator = (const TimerHandlerFwd &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__TimerHandlerFwd_hpp__HH

