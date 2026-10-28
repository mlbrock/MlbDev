// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// SocketIo Portable Socket Library Include File.
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  BoostAsioInclude.hpp

   File Description  :  Include file which manages the include of the Boost
                        asio include files.

   Revision History  :  2002-01-26 --- Creation
                           Michael L. Brock

      Copyright Michael L. Brock 2002 - 2026.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__BoostAsioInclude_hpp__HH

#define HH__MLB__SocketIo__BoostAsioInclude_hpp__HH      1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Include necessary header files . . .
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo.hpp>

#define _WIN32_WINNT 0x0501

#ifdef _MSC_VER
# pragma warning(push)
# if _MSC_VER < 1400
#  pragma warning(disable:4302 4511 4619 4217 4710)
# endif // # if _MSC_VER < 1400
# pragma warning(disable:4061 4100 4191 4242 4365 4548 4555 4571 4619 4625 4626 4640 4820)
#endif // #ifdef _MSC_VER

#include <boost/asio.hpp>

#include <boost/version.hpp>

#ifdef _MSC_VER
# pragma warning(pop)
#endif // #ifdef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
   ASIO version 1.33.0 removed many types and functions which had previously
   been deprecated.

   The last Boost version which contained the removed elements was Boost
   version 1.86.0 / ASIO version 1.32.0.

   Beginning with Boost version 1.87.0 / ASIO 1.34.0 we need to shim the use
   of the following removed elements:
      o io_service
      o const_buffers_1
      o mutable_buffers_1
*/
#if (BOOST_VERSION > 108600)
# include <boost/asio/io_context.hpp>
# include <boost/asio/buffer.hpp>

namespace boost {
namespace asio {

// Shim for deprecated and removed types.
typedef io_context     io_service;
typedef const_buffer   const_buffers_1;
typedef mutable_buffer mutable_buffers_1;

} // namespace asio
} // namespace boost
#endif // #if (BOOST_VERSION > 108600)
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifndef HH__MLB__SocketIo__BoostAsioInclude_hpp__HH

