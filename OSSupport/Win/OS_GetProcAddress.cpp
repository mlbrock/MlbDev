// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of wrappers to the Windows function
								GetProcAddress().

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport_Win.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
FARPROC OS_GetProcAddress(HMODULE module_handle, const std::string &proc_name,
	bool require_proc)
{
	FARPROC proc_addr;

	if ((proc_addr = ::GetProcAddress(module_handle, proc_name.c_str())) ==
		NULL) {
		if (require_proc)
			MLB::Utility::ThrowSystemError("Call to 'GetProcAddress()' for "
				"procedure '" + proc_name + "()' (module handle " +
				MLB::Utility::ValueToStringHex(module_handle) + ") failed");
	}

	return(proc_addr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FARPROC OS_GetProcAddress(const std::string &module_name,
	const std::string &proc_name, bool require_proc)
{
	FARPROC proc_addr = NULL;

	try {
		proc_addr = OS_GetProcAddress(EnsureLoadedLibrary(module_name, true),
			proc_name, require_proc);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to determine the procedure "
			"address for a module in '" + module_name + "': " +
			std::string(except.what()));
	}

	return(proc_addr);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

