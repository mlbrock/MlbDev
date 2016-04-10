// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the portable host name functions.

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

#include <Utility/StringSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __GNUC__
# include <string.h>
#endif // #ifdef __GNUC__

#if defined(__unix__) && defined(__GNUC__)
# include <unistd.h>
#endif // #if defined(__unix__) && defined(__GNUC__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
std::string GetHostName()
{
	char datum[MaxHostNameLength + 1];

	return(GetHostName(datum, sizeof(datum) - 1));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *GetHostName(char *host_name, unsigned int host_name_length)
{
#ifdef _Windows
	// Under Windows, the length must include space for the ASCII 0
	unsigned int out_host_name_length = host_name_length + 1;
	if (::GetComputerNameA(host_name,
		reinterpret_cast<LPDWORD>(&out_host_name_length)) == 0)
		ThrowSystemError("Invocation of 'GetComputerNameA()' failed");
#else
# ifndef __MSDOS__
	if (::gethostname(host_name, static_cast<int>(host_name_length)) != 0)
		ThrowSystemError("Invocation of 'gethostname()' failed");
# else
	if (host_name_length < ::strlen("MS-DOS-Machine"))
		ThrowStdException("Unable to get the host name because the \
minimum length of the string is 14.");
	else
		::strcpy(host_name, "MS-DOS-Machine");
# endif // # ifndef __MSDOS__
#endif // #ifdef _Windows

	return(host_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetHostNameCanonical()
{
	return(ConvertHostNameToCanonical(GetHostName()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SetHostName(const std::string &host_name)
{
	SetHostName(host_name.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SetHostName(const char *host_name)
{
#ifdef _Windows
	if (::SetComputerNameA(host_name) == 0)
		ThrowSystemError("Invocation of 'SetComputerNameA()' failed");
#else
# ifndef __MSDOS__
	if (::sethostname(host_name, ::strlen(host_name)) != 0)
		ThrowSystemError("Invocation of 'sethostname()' failed");
# endif // # ifndef __MSDOS__
#endif // #ifdef _Windows
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ConvertHostNameToCanonical(const std::string &host_name)
{
	std::string            tmp_host_name = host_name;
	std::string::size_type dot_index     = tmp_host_name.find_first_of('.');

	if (dot_index != std::string::npos)
		tmp_host_name.erase(dot_index);

	return(LowerCase(Trim(tmp_host_name)));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <iostream>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << "GetHostName(): " << GetHostName() << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << std::endl;
		std::cout << "Regression test error: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

