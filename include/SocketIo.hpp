//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for SocketIo support.

	Revision History	:	1996-04-10 --- Based upon the portable Network
													Programming Suppoprt Library (NPSL).
									Michael L. Brock
								2008-01-26 --- Adapted for use with Boost::Asio.
									Michael L. Brock

		Copyright Michael L. Brock 1996 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__SocketIo_hpp__HH

#define HH__MLB__SocketIo__SocketIo_hpp__HH		1

#define MLB__LIB__SocketIo

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <mbcompat.h>
#include <mbtypes.h>

#include <Utility/Utility_Exception.hpp>

#ifdef MLB__LIB__SocketIo
# ifdef SOCKETIO_DLL_EXPORTS
#  define API_SOCKETIO		MB_LIB_EXPORT
# else
#  define API_SOCKETIO		MB_LIB_IMPORT
# endif // #ifdef SOCKETIO_DLL_EXPORTS
#else
#  define API_SOCKETIO
#endif // #ifdef MLB__LIB__SocketIo

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_SOCKETIO

//	////////////////////////////////////////////////////////////////////////////

/**
	\namespace MLB

	\brief The corporate namespace.
*/
namespace MLB {

/**
	\namespace SocketIo

	\brief The Boost Asio library wrapper project namespace.
*/
namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
typedef Native_UInt32 IpAddressType;
typedef Native_UInt16 IpPortType;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const unsigned int Ip4AddPortSize = sizeof(IpAddressType) + sizeof(IpPortType);
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__SocketIo_hpp__HH

