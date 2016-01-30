// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Given a string which represents a process identifier
								or a process name, resolves it.

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

#include <OSSupport.h>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
std::pair<MLB::Utility::ProcessId, std::string>
	ResolveProcessSpecifier(const char *process_specifier)
{
	std::pair<MLB::Utility::ProcessId, std::string> return_value;

	try {
		if ((process_specifier == NULL) || (!(*process_specifier)))
			MLB::Utility::ThrowInvalidArgument("The process specifier passed to "
				"'ResolveProcess()' is " + std::string((process_specifier == NULL) ?
				"'NULL'" : "an empty string") + ".");
		const char *tmp_ptr = process_specifier;
		while (*tmp_ptr) {
			if (!isdigit(*tmp_ptr)) {
				return_value.first  = ProcessNameToId(process_specifier);
				return_value.second = ProcessIdToName(return_value.first);
				return(return_value);
			}
			++tmp_ptr;
		}
		return_value.second = ProcessIdToName(
			static_cast<MLB::Utility::ProcessId>(atoi(process_specifier)));
		return_value.first  = ProcessNameToId(return_value.second);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to resolve a process specifier "
			"failed: " + std::string(except.what()));
	}

	return(return_value);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::pair<MLB::Utility::ProcessId, std::string>
	ResolveProcessSpecifier(const std::string &process_specifier)
{
	return(ResolveProcessSpecifier(process_specifier.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ { <process-name> | <process-id> } [ ... ] ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		std::vector<std::string> process_list;
		if (argc == 1)
			process_list.push_back(argv[0]);
		else
			std::vector<std::string>(argv + 1, argv + argc).swap(process_list);
		std::cout << "Test ResolveProcessSpecifier():" << std::endl;
		std::vector<std::string>::const_iterator iter_b(process_list.begin());
		std::vector<std::string>::const_iterator iter_e(process_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::pair<MLB::Utility::ProcessId, std::string> proc_spec;
			std::cout << "   " << *iter_b <<    ": " << std::flush;
			try {
				proc_spec = ResolveProcessSpecifier(*iter_b);
				std::cout << "   " << *iter_b <<    ": " << proc_spec.first <<
					" = " << proc_spec.second << std::endl;
			}
			catch (const std::exception &except) {
				std::cout << "FAILED: " << except.what() << std::endl;
				return_code = EXIT_FAILURE;
			}
		}
		std::cout << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/ResolveProcessSpecifier.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

