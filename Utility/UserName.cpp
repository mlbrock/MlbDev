// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the portable user name functions.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>

#ifndef _MSC_VER
# include <pwd.h>
#endif // #ifndef _MSC_VER

#ifdef __GNUC__
# include <string.h>
#endif // #ifdef __GNUC__

#if defined(__unix__)
# include <unistd.h>
#endif // #if defined(__unix__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
std::string GetCurrentUserName()
{
	char datum[UserNameLength + 1];

	return(GetCurrentUserName(datum, sizeof(datum) - 1));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *GetCurrentUserName(char *user_name, unsigned int user_name_length)
{
#ifdef __MSDOS__
	if (user_name_length < ::strlen("MSDOS"))
		ThrowInvalidArgument("Unable to get user name because the length of "
			"the provided buffer (" + AnyToString(user_name_length) + ") was "
			"less than that required (" + AnyToString(::strlen("MSDOS")) + ").");

	::strcpy(user_name, "MSDOS");
#elif _Windows
	// Under Windows, the length must include space for the ASCII 0
	unsigned int out_user_name_length = user_name_length + 1;

	if (::GetUserName(static_cast<LPTSTR>(user_name),
		reinterpret_cast<LPDWORD>(&out_user_name_length)) == 0)
		ThrowSystemError("Invocation of 'GetUserName()' failed");
#else
# if _POSIX_PTHREAD_SEMANTICS
	struct passwd  pentry;
	struct passwd *pentry_ptr;
	char           buffer[2047 + 1];
	int            result_errno;

	if ((result_errno = ::getpwuid_r(getuid(), &pentry, buffer, sizeof(buffer),
		&pentry_ptr)) != 0)
		ThrowErrno(result_errno,
			"Invocation of 'getpwuid_r()' failed");
# else
	struct passwd *pentry_ptr;

	if ((pentry_ptr = ::getpwuid(getuid())) == NULL)
		ThrowErrno("Invocation of 'getpwuid_r()' failed");
# endif // # if _POSIX_PTHREAD_SEMANTICS
	if (pentry_ptr->pw_name == NULL)
		ThrowStdException("Password entry member 'pw_name' is 'NULL'.");

	unsigned int pw_name_length = ::strlen(pentry_ptr->pw_name);

	if (user_name_length < pw_name_length)
		ThrowInvalidArgument("Unable to get user name because the length of "
			"the provided buffer (" + AnyToString(user_name_length) + ") was "
			"less than that required (" + AnyToString(pw_name_length) + ").");

	::strcpy(user_name, pentry_ptr->pw_name);
#endif // #ifdef __MSDOS__

	return(user_name);
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
		std::cout << "GetCurrentUserName(): " << GetCurrentUserName() << std::endl;
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

