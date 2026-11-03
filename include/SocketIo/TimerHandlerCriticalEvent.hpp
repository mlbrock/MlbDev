// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// SocketIo Portable Socket Library Include File.
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  TimerHandlerCriticalEvent.hpp

   File Description  :  Include file for class TimerHandlerCriticalEvent.

   Revision History  :  2002-01-26 --- Creation
                           Michael L. Brock

      Copyright Michael L. Brock 2002 - 2026.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__TimerHandlerCriticalEvent_hpp__HH

#define HH__MLB__SocketIo__TimerHandlerCriticalEvent_hpp__HH      1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Include necessary header files . . .
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/TimerHandler.hpp>

#include <Utility/CriticalEventHandler.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class TimerHandlerCriticalEvent : public MLB::SocketIo::TimerHandler {
public:
	TimerHandlerCriticalEvent(boost::asio::io_service &io_service,
		unsigned int gran_ticks, unsigned int gran_usecs = 100000)
		:MLB::SocketIo::TimerHandler(io_service, gran_ticks, gran_usecs)
	{
	}
	virtual ~TimerHandlerCriticalEvent()
	{
	}

protected:
	virtual bool TimerEventImpl(const boost::system::error_code &/* asio_code */)
	{
		if (MLB::Utility::CriticalEventTest()) {
			try {
#if (BOOST_VERSION <= 106900)
				/*
					The get_io_service() method was removed after Boost version
					1.69.0 / ASIO version 1.12.2 ...
				*/
				timer_.get_io_service().stop();
#else
				timer_.get_executor().context();
#endif // #if (BOOST_VERSION <= 106900)
			}
			catch (std::exception &) {
				throw;
			}
			return(false);
		}

		return(true);
	}

private:
	TimerHandlerCriticalEvent(const TimerHandlerCriticalEvent &other);
	TimerHandlerCriticalEvent & operator = (
		const TimerHandlerCriticalEvent &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__TimerHandlerCriticalEvent_hpp__HH

