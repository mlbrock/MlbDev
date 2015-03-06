// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the calculation of IP protocol
								checksums.

	Revision History	:	2011-11-03 --- Creation
									Michael L. Brock

	* Copyright (c) 1982, 1986, 1988, 1990, 1993, 1994
	*     The Regents of the University of California.  All rights reserved.
	*
	* Redistribution and use in source and binary form, with or without
	* modifications, are permitted provided that the following conditions
	* are met:
	* 1. Redistributions of source code must retain the above copyright
	*    notice, this list of conditions and the following disclaimer.
	* 2. Redistributions in binary form must reproduce the above copyright
	*    notice, this list of conditions and the following disclaimer in the
	*    documentation and/or other materials provided with the distribution.
	* 3. All advertising materials mentioning features or use of this software
	*    must display the following acknowledgement:
	*      This product includes software developed by the University of 
	*      California, Berkeley and its contributors.
	* 4. Neither the name of the University nor the names of its contributors
	*    may be used to endorse or promote products derived from this software
	*    without specific prior written permission.
	*
	* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ''AS IS'' AND
	* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
	* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSITUTE GOODS
	* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
	* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	* SUCH DAMAGE.

		This code is a modification from TCP/IP Illustrated Vol. 2 by
		Gary R. Wright and W. Richard Stevens.

		Portions Copyright Michael L. Brock 2011 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__SocketIo__CalcIPChecksum_hpp__HH

#define HH__MLB__SocketIo__CalcIPChecksum_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file CalcIPChecksum.hpp

	\brief	For the calculation of IP protocol checksums.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo.hpp>

#include <mbtypes.h>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
inline Native_UInt16 CalcIPChecksum(std::size_t len, const Native_UInt16 *ip)
{
	Native_UInt32 sum = 0;	/* assume 32 bit long, 16 bit short */

	while ( len > 1 ) {
		sum += *ip++;
		if ( sum & 0x80000000 )	  /* if high order bit set, fold */
			sum = (sum & 0xFFFF) + (sum >> 16);
		len -= 2;
	}

	if ( len )		  /* take care of left over byte */
		sum += static_cast<Native_UInt16>(
			*reinterpret_cast<const Native_UInt8 *>(ip));

	while ( sum>>16 )
		sum = (sum & 0xFFFF) + (sum >> 16);

	return (static_cast<Native_UInt16>(~sum));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__CalcIPChecksum_hpp__HH

