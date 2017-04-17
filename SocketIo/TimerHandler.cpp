// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the TimerHandler class.

	Revision History	:	2008-12-20 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/TimerHandler.hpp>

#include <Utility/Utility_Exception.hpp>

#include <boost/bind.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
TimerHandler::TimerHandler(boost::asio::io_service &io,
	unsigned int gran_ticks, unsigned int gran_usecs)
	:counter_(CheckGranularity(gran_ticks, gran_usecs))
	,timer_(io, boost::posix_time::microseconds(gran_usecs))
	,gran_ticks_(gran_ticks)
	,gran_usecs_(gran_usecs)
	,start_time_()
	,is_running_(false)
{
	Run();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TimerHandler::TimerHandler(bool immediate_start,
	boost::asio::io_service &io, unsigned int gran_ticks,
	unsigned int gran_usecs)
	:counter_(CheckGranularity(gran_ticks, gran_usecs))
	,timer_(io, boost::posix_time::microseconds(gran_usecs))
	,gran_ticks_(gran_ticks)
	,gran_usecs_(gran_usecs)
	,start_time_()
	,is_running_(false)
{
	if (immediate_start)
		Run();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TimerHandler::~TimerHandler()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool TimerHandler::IsRunning() const
{
	return(is_running_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool TimerHandler::Run()
{
	if (!RunImpl())
		return(false);

	if (!is_running_) {
		timer_.async_wait(boost::bind(&TimerHandler::TimerEvent,
			this, boost::asio::placeholders::error));
		is_running_ = true;
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TimerHandler::TimerEvent(const boost::system::error_code &asio_code)
{
	if (TimerEventImpl(asio_code)) {
		timer_.expires_at(timer_.expires_at() +
			boost::posix_time::microseconds(gran_usecs_));
		timer_.async_wait(boost::bind(&TimerHandler::TimerEvent,
			this, boost::asio::placeholders::error));
	}
	else
		timer_.cancel();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool TimerHandler::RunImpl()
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool TimerHandler::TimerEventImpl(
	const boost::system::error_code & /* asio_code */)
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned long long TimerHandler::CheckGranularity(unsigned int gran_ticks,
	unsigned int gran_usecs)
{
	if (gran_ticks < 1)
		MLB::Utility::ThrowInvalidArgument("The time-out handler tick "
			"granularity count may not be zero.");
	else if (gran_usecs < 1)
		MLB::Utility::ThrowInvalidArgument("The time-out handler tick "
			"granularity microcseconds may not be zero.");

	return(0);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/CriticalEventHandler.hpp>

using namespace MLB::SocketIo;

// ////////////////////////////////////////////////////////////////////////////
class Test1 : public TimerHandler {
public:
	Test1(boost::asio::io_service &io_service)
		:TimerHandler(io_service, 10)
	{
		std::cout << "********** Test1: Fires every 10 * 100000 microseconds." <<
			std::endl;
	}
	virtual ~Test1()
	{
		std::cout << MLB::Utility::TimeSpec() <<
			" " << "Test1: Exiting." << std::endl; 
	}

protected:
	virtual bool TimerEventImpl(const boost::system::error_code &asio_code)
	{
if (asio_code)
	std::cout << "Test2: error_code = " << asio_code << std::endl;

		if (!(++counter_ % gran_ticks_))
			std::cout << MLB::Utility::TimeSpec() <<
				" " << "Test1: Fired1." << std::endl; 
		return(true);
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class Test2 : public TimerHandler {
public:
	Test2(boost::asio::io_service &io_service)
		:TimerHandler(io_service, 50)
	{
		std::cout << "********** Test2: Fires every 50 * 100000 microseconds. "
			"Looks for critical events." << std::endl;
	}
	virtual ~Test2()
	{
		std::cout << MLB::Utility::TimeSpec() <<
			" " << "Test2: Exiting." << std::endl; 
	}

protected:
	virtual bool TimerEventImpl(const boost::system::error_code &asio_code)
	{
if (asio_code)
	std::cout << "Test2: error_code = " << asio_code << std::endl;

		if (MLB::Utility::CriticalEventTest()) {
			std::cout << MLB::Utility::TimeSpec() <<
				" " << "Test2: Stopping I/O service." << std::endl; 
			try {
				timer_.get_io_service().stop();
				std::cout << MLB::Utility::TimeSpec() <<
					" " << "Test2: Stop of I/O service succeeded." <<
					std::endl; 
			}
			catch (std::exception &) {
				std::cout << MLB::Utility::TimeSpec() <<
					" " << "Test2: Stop of I/O service failed." <<
					std::endl;
				throw;
			}
			return(false);
		}

		if (!(++counter_ % gran_ticks_))
			std::cout << MLB::Utility::TimeSpec() <<
				" " << "Test2: Fired1." << std::endl; 
		return(true);
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class Test3 : public TimerHandler {
public:
	Test3(boost::asio::io_service &io_service)
		:TimerHandler(io_service, 30)
	{
		std::cout << "********** Test1: Fires once at 30 * 100000 "
			"microseconds." << std::endl;
	}
	virtual ~Test3()
	{
		std::cout << MLB::Utility::TimeSpec() <<
			" " << "Test3: Exiting." << std::endl; 
	}

protected:
	virtual bool TimerEventImpl(const boost::system::error_code &asio_code)
	{
if (asio_code)
	std::cout << "Test3: error_code = " << asio_code << std::endl;

		if (++counter_ % gran_ticks_)
			return(true);

		std::cout << MLB::Utility::TimeSpec() <<
			" " << "Test3: Cancelling timer." << std::endl; 

		try {
			timer_.cancel();
			std::cout << MLB::Utility::TimeSpec() <<
				" " << "Test3: Cancel of timer succeeded." << std::endl; 
		}
		catch (std::exception &) {
			std::cout << MLB::Utility::TimeSpec() <<
				" " << "Test2: Cancel of timer failed." << std::endl;
		}

		return(false);
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunHandlers()
{
	std::cout << "Entered 'TEST_RunHandler' " << std::endl;

	boost::asio::io_service io_service; 

	Test1 test_1(io_service);
	Test2 test_2(io_service);
	Test3 test_3(io_service);

	io_service.run();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventContext critical_event_context;
		TEST_RunHandlers();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

