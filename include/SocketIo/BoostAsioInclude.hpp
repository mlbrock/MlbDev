//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file which manages the include of the Boost
								asio include files.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__BoostAsioInclude_hpp__HH

#define HH__MLB__SocketIo__BoostAsioInclude_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <mbcompat.h>

#define _WIN32_WINNT	0x0501

#ifdef _MSC_VER
# pragma warning(push)
# if _MSC_VER < 1400
#  pragma warning(disable:4302 4511 4619 4217 4710)
# endif // # if _MSC_VER < 1400
# pragma warning(disable:4061 4100 4191 4242 4365 4548 4555 4571 4619 4625 4626 4640 4820)
#endif // #ifdef _MSC_VER

#include <boost/asio.hpp> 

#ifdef _MSC_VER
# pragma warning(pop)
#endif // #ifdef _MSC_VER

#include <SocketIo.hpp>

//	////////////////////////////////////////////////////////////////////////////

#endif // #ifndef HH__MLB__SocketIo__BoostAsioInclude_hpp__HH

