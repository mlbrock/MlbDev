// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Xerces-C Utilities Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the XercesContext class.

	Revision History	:	2005-01-07 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <XercesUtils/XercesContext.hpp>

#include <Utility/Utility_Exception.hpp>

#include <xercesc/util/PlatformUtils.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4217 4275 4625 4626)
# if _MSC_VER >= 1500
#  pragma warning(disable:4061 4242 4244 4365 4640)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/thread/mutex.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace XercesUtils {

namespace {

// ////////////////////////////////////////////////////////////////////////////
static boost::mutex XercesContext_InitializationLock;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void XercesContext_Initialization()
{
	try {
		try {
			boost::mutex::scoped_lock my_lock(XercesContext_InitializationLock);
			xercesc::XMLPlatformUtils::Initialize();
		}
		catch (const xercesc::XMLException &except) {
			std::ostringstream o_str;
			o_str << "xercesc::XMLException: [getType() = " <<
				except.getType() << "][getCode() = " << except.getCode() <<
				"][getMessage = " << except.getMessage() << "][getSrcFile() = " <<
				except.getSrcFile() << "][getSrcLine() = " <<
				except.getSrcLine() << "][getErrorType() = " <<
				except.getErrorType() << "]" << std::endl;
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		catch (const std::exception &) {
			throw;
		}
		catch (...) {
			MLB::Utility::ThrowLogicError("Unknown exception thrown from "
				"Xerces-C library.");
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to initialize the Xerces-C "
			"run-time with a call to 'xercesc::XMLPlatformUtils::Initialize()' "
			"failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
XercesContext::XercesContext()
{
	XercesContext_Initialization();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XercesContext::~XercesContext()
{
	try {
		boost::mutex::scoped_lock my_lock(XercesContext_InitializationLock);
		xercesc::XMLPlatformUtils::Terminate();
	}
	catch (...) {
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
XercesContext::XercesContext(const XercesContext &)
{
	XercesContext_Initialization();
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace XercesUtils

} // namespace MLB

