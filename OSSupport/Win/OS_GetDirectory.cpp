// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of wrappers to the Windows get directory
								functions:
									GetCurrentDirectory();
									GetSystemDirectory();
									GetWindowsDirectory();

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

#include <OSSupport/Win/OSSupport_Win.hpp>

#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

namespace {
// ////////////////////////////////////////////////////////////////////////////
void HandleGetDirResult(unsigned int name_size, DWORD needed_size,
	char *func_name)
{
	if (needed_size == 0)
		MLB::Utility::ThrowSystemError("Call to '" + std::string(func_name) +
			"()' with a buffer size of " + MLB::Utility::AnyToString(name_size) +
			" failed.");
	else if (needed_size > name_size)
		MLB::Utility::ThrowSystemError("Call to '" + std::string(func_name) +
			"()' with a buffer size of " + MLB::Utility::AnyToString(name_size) +
			" resulted in a return value which indicated that the required "
			"buffer size is " + MLB::Utility::AnyToString(needed_size) + ".");
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
char *OS_GetCurrentDirectory(char *name_buffer, unsigned int name_size)
{
	HandleGetDirResult(name_size,
		::GetCurrentDirectoryA(name_size, name_buffer), "GetCurrentDirectory");

	return(name_buffer);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &OS_GetCurrentDirectory(std::string &name_buffer)
{
	char tmp_buffer[MLB::Utility::MaxPathNameLength + 1];

	return(name_buffer.assign(OS_GetCurrentDirectory(tmp_buffer,
		sizeof(tmp_buffer) - 1)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OS_GetCurrentDirectory()
{
	char tmp_buffer[MLB::Utility::MaxPathNameLength + 1];

	return(OS_GetCurrentDirectory(tmp_buffer, sizeof(tmp_buffer) - 1));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *OS_GetSystemDirectory(char *name_buffer, unsigned int name_size)
{
	HandleGetDirResult(name_size,
		::GetSystemDirectoryA(name_buffer, name_size), "GetSystemDirectory");

	return(name_buffer);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &OS_GetSystemDirectory(std::string &name_buffer)
{
	char tmp_buffer[MLB::Utility::MaxPathNameLength + 1];

	return(name_buffer.assign(OS_GetSystemDirectory(tmp_buffer,
		sizeof(tmp_buffer) - 1)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OS_GetSystemDirectory()
{
	char tmp_buffer[MLB::Utility::MaxPathNameLength + 1];

	return(OS_GetSystemDirectory(tmp_buffer, sizeof(tmp_buffer) - 1));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *OS_GetWindowsDirectory(char *name_buffer, unsigned int name_size)
{
	HandleGetDirResult(name_size,
		::GetWindowsDirectoryA(name_buffer, name_size), "GetWindowsDirectory");

	return(name_buffer);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &OS_GetWindowsDirectory(std::string &name_buffer)
{
	char tmp_buffer[MLB::Utility::MaxPathNameLength + 1];

	return(name_buffer.assign(OS_GetWindowsDirectory(tmp_buffer,
		sizeof(tmp_buffer) - 1)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string OS_GetWindowsDirectory()
{
	char tmp_buffer[MLB::Utility::MaxPathNameLength + 1];

	return(OS_GetWindowsDirectory(tmp_buffer, sizeof(tmp_buffer) - 1));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << "Test OS_GetCurrentDirectory(): " <<
			OS_GetCurrentDirectory() << std::endl;
		std::cout << "Test OS_GetSystemDirectory() : " <<
			OS_GetSystemDirectory()  << std::endl;
		std::cout << "Test OS_GetWindowsDirectory(): " <<
			OS_GetWindowsDirectory() << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetDirectory.cpp: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

