// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Wrapper functions for the Tibrv class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
void OpenRvEnv()
{
	try {
		RvUtilX_THROW_TIBRV_STATUS_IF(Tibrv::open,
			());
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to open the Tib/Rv environment "
			"with the static method 'Tibrv::open()' failed: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void CloseRvEnv()
{
	try {
		RvUtilX_THROW_TIBRV_STATUS_IF(Tibrv::close,
			());
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to close the Tib/Rv environment "
			"with the static method 'Tibrv::close()' failed: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvQueue *GetDefaultQueue()
{
	TibrvQueue *return_ptr = Tibrv::defaultQueue();

	if (return_ptr == NULL)
		throw RvException("Tib/Rv static method 'Tibrv::defaultQueue()' "
			"returned 'NULL'.");

	return(return_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvProcessTransport *GetProcessTransportPtr()
{
	TibrvProcessTransport *return_ptr = Tibrv::processTransport();

	if (return_ptr == NULL)
		throw RvException("Tib/Rv static method 'Tibrv::processTransport()' "
			"returned 'NULL'.");

	return(return_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GetRvEnvVersionPtr()
{
	const char *return_ptr = Tibrv::version();

	if (return_ptr == NULL)
		throw RvException("Tib/Rv static method 'Tibrv::version()' returned "
			"'NULL'.");

	return(return_ptr);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &GetRvEnvVersion(std::string &out_string)
{
	return(out_string.assign(GetRvEnvVersionPtr()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GetRvEnvVersion()
{
	std::string out_string;

	return(GetRvEnvVersion(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB


#ifdef TEST_MAIN

#include <Utility/ValueToStringRadix.hpp>

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	using namespace MLB::Utility;
	using namespace MLB::RvUtilX;

	try {
		std::cout << "Opening Tib/Rv environment..." << std::flush;
		OpenRvEnv();
		std::cout << " done!" << std::endl;
		//	----------------------------------------------------------------
		std::cout << "Default queue at address    : " <<
			ValueToStringHex(GetDefaultQueue()) << std::endl;
		std::cout << "Process transport at address: " <<
			ValueToStringHex(GetProcessTransportPtr()) << std::endl;
		std::cout << "Tib/Rv version number       : " <<
			GetRvEnvVersionPtr() << std::endl;
		//	----------------------------------------------------------------
		std::cout << "Closing Tib/Rv environment..." << std::flush;
		CloseRvEnv();
		std::cout << " done!" << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: in regression test: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

