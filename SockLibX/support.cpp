/* *********************************************************************** */
/* *********************************************************************** */
/*	Network Programming Support Library (NPSL) Source Module						*/
/* *********************************************************************** */
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Miscellaneous support for the library.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/* *********************************************************************** */

/* *********************************************************************** */
/* *********************************************************************** */
/*    Include necessary header files . . .                                 */
/* *********************************************************************** */
 
#include <errno.h>
#include <string.h>

#include "npsli.h"

/* *********************************************************************** */

/* *********************************************************************** */
typedef struct {
	int native_error;
	int npsl_error;
} NPSL_SUPP_ERROR_MAP;
/* *********************************************************************** */

/* *********************************************************************** */
static const NPSL_SUPP_ERROR_MAP NPSL_SUPP_NativeErrorMapList[] = {
#ifdef _Windows
# ifdef WSAEACCES
	{	WSAEACCES,				NPSL_EACCES				},
# endif /* # ifdef WSAEACCES */
# ifdef WSAEADDRINUSE
	{	WSAEADDRINUSE,			NPSL_EADDRINUSE		},
# endif /* # ifdef WSAEADDRINUSE */
# ifdef WSAEADDRNOTAVAIL
	{	WSAEADDRNOTAVAIL,		NPSL_EADDRNOTAVAIL	},
# endif /* # ifdef WSAEADDRNOTAVAIL */
# ifdef WSAEAFNOSUPPORT
	{	WSAEAFNOSUPPORT,		NPSL_EAFNOSUPPORT		},
# endif /* # ifdef WSAEAFNOSUPPORT */
# ifdef WSAEALREADY
	{	WSAEALREADY,			NPSL_EALREADY			},
# endif /* # ifdef WSAEALREADY */
# ifdef WSAECONNREFUSED
	{	WSAECONNREFUSED,		NPSL_ECONNREFUSED		},
# endif /* # ifdef WSAECONNREFUSED */
# ifdef WSAECONNRESET
	{	WSAECONNRESET,			NPSL_ECONNRESET		},
# endif /* # ifdef WSAECONNRESET */
# ifdef WSAEDESTADDRREQ
	{	WSAEDESTADDRREQ,		NPSL_EDESTADDRREQ		},
# endif /* # ifdef WSAEDESTADDRREQ */
# ifdef WSAEFAULT
	{	WSAEFAULT,				NPSL_EFAULT				},
# endif /* # ifdef WSAEFAULT */
# ifdef WSAEHOSTUNREACH
	{	WSAEHOSTUNREACH,		NPSL_EHOSTUNREACH		},
# endif /* # ifdef WSAEHOSTUNREACH */
# ifdef WSAEINPROGRESS
	{	WSAEINPROGRESS,		NPSL_EINPROGRESS		},
# endif /* # ifdef WSAEINPROGRESS */
# ifdef WSAEINTR
	{	WSAEINTR,				NPSL_EINTR				},
# endif /* # ifdef WSAEINTR */
# ifdef WSAEINVAL
	{	WSAEINVAL,				NPSL_EINVAL				},
# endif /* # ifdef WSAEINVAL */
# ifdef WSAEISCONN
	{	WSAEISCONN,				NPSL_EISCONN			},
# endif /* # ifdef WSAEISCONN */
# ifdef WSAEMSGSIZE
	{	WSAEMSGSIZE,			NPSL_EMSGSIZE			},
# endif /* # ifdef WSAEMSGSIZE */
# ifdef WSAENETDOWN
	{	WSAENETDOWN,			NPSL_ENETDOWN			},
# endif /* # ifdef WSAENETDOWN */
# ifdef WSAENETUNREACH
	{	WSAENETUNREACH,		NPSL_ENETUNREACH		},
# endif /* # ifdef WSAENETUNREACH */
# ifdef WSAENOBUFS
	{	WSAENOBUFS,				NPSL_ENOBUFS			},
# endif /* # ifdef WSAENOBUFS */
# ifdef WSAENOPROTOOPT
	{	WSAENOPROTOOPT,		NPSL_ENOPROTOOPT		},
# endif /* # ifdef WSAENOPROTOOPT */
# ifdef WSAENOTCONN
	{	WSAENOTCONN,			NPSL_ENOTCONN			},
# endif /* # ifdef WSAENOTCONN */
# ifdef WSAENOTSOCK
	{	WSAENOTSOCK,			NPSL_ENOTSOCK			},
# endif /* # ifdef WSAENOTSOCK */
# ifdef WSAEOPNOTSUPP
	{	WSAEOPNOTSUPP,			NPSL_EOPNOTSUPP		},
# endif /* # ifdef WSAEOPNOTSUPP */
# ifdef WSAETIMEDOUT
	{	WSAETIMEDOUT,			NPSL_ETIMEDOUT			},
# endif /* # ifdef WSAETIMEDOUT */
# ifdef WSAEWOULDBLOCK
	{	WSAEWOULDBLOCK,		NPSL_EWOULDBLOCK		},
# endif /* # ifdef WSAEWOULDBLOCK */
#else
# ifdef EACCES
	{	EACCES,					NPSL_EACCES				},
# endif /* # ifdef EACCES */
# ifdef EADDRINUSE
	{	EADDRINUSE,				NPSL_EADDRINUSE		},
# endif /* # ifdef EADDRINUSE */
# ifdef EADDRNOTAVAIL
	{	EADDRNOTAVAIL,			NPSL_EADDRNOTAVAIL	},
# endif /* # ifdef EADDRNOTAVAIL */
# ifdef EAFNOSUPPORT
	{	EAFNOSUPPORT,			NPSL_EAFNOSUPPORT		},
# endif /* # ifdef EAFNOSUPPORT */
# ifdef EALREADY
	{	EALREADY,				NPSL_EALREADY			},
# endif /* # ifdef EALREADY */
# ifdef ECONNREFUSED
	{	ECONNREFUSED,			NPSL_ECONNREFUSED		},
# endif /* # ifdef ECONNREFUSED */
# ifdef ECONNRESET
	{	ECONNRESET,				NPSL_ECONNRESET		},
# endif /* # ifdef ECONNRESET */
# ifdef EDESTADDRREQ
	{	EDESTADDRREQ,			NPSL_EDESTADDRREQ		},
# endif /* # ifdef EDESTADDRREQ */
# ifdef EFAULT
	{	EFAULT,					NPSL_EFAULT				},
# endif /* # ifdef EFAULT */
# ifdef EHOSTUNREACH
	{	EHOSTUNREACH,			NPSL_EHOSTUNREACH		},
# endif /* # ifdef EHOSTUNREACH */
# ifdef EINPROGRESS
	{	EINPROGRESS,			NPSL_EINPROGRESS		},
# endif /* # ifdef EINPROGRESS */
# ifdef EINTR
	{	EINTR,					NPSL_EINTR				},
# endif /* # ifdef EINTR */
# ifdef EINVAL
	{	EINVAL,					NPSL_EINVAL				},
# endif /* # ifdef EINVAL */
# ifdef EISCONN
	{	EISCONN,					NPSL_EISCONN			},
# endif /* # ifdef EISCONN */
# ifdef EMSGSIZE
	{	EMSGSIZE,				NPSL_EMSGSIZE			},
# endif /* # ifdef EMSGSIZE */
# ifdef ENETDOWN
	{	ENETDOWN,				NPSL_ENETDOWN			},
# endif /* # ifdef ENETDOWN */
# ifdef ENETUNREACH
	{	ENETUNREACH,			NPSL_ENETUNREACH		},
# endif /* # ifdef ENETUNREACH */
# ifdef ENOBUFS
	{	ENOBUFS,					NPSL_ENOBUFS			},
# endif /* # ifdef ENOBUFS */
# ifdef ENOPROTOOPT
	{	ENOPROTOOPT,			NPSL_ENOPROTOOPT		},
# endif /* # ifdef ENOPROTOOPT */
# ifdef ENOTCONN
	{	ENOTCONN,				NPSL_ENOTCONN			},
# endif /* # ifdef ENOTCONN */
# ifdef ENOTSOCK
	{	ENOTSOCK,				NPSL_ENOTSOCK			},
# endif /* # ifdef ENOTSOCK */
# ifdef EOPNOTSUPP
	{	EOPNOTSUPP,				NPSL_EOPNOTSUPP		},
# endif /* # ifdef EOPNOTSUPP */
# ifdef ETIMEDOUT
	{	ETIMEDOUT,				NPSL_ETIMEDOUT			},
# endif /* # ifdef ETIMEDOUT */
# ifdef EWOULDBLOCK
	{	EWOULDBLOCK,			NPSL_EWOULDBLOCK		},
# endif /* # ifdef EWOULDBLOCK */
#endif /* #ifdef _Windows */
	{	0,	0	}									/* This place-holder is not used.	*/
};
static const unsigned int        NPSL_SUPP_NativeErrorMapCount  =
	sizeof(NPSL_SUPP_NativeErrorMapList) /
	sizeof(NPSL_SUPP_NativeErrorMapList[0]);
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_SUPP_MapLastError()
{
#ifdef _Windows
	return(NPSL_SUPP_MapError(WSAGetLastError()));
#else
	return(NPSL_SUPP_MapError(errno));
#endif /* #ifdef _Windows */
}
/* *********************************************************************** */

/* *********************************************************************** */
int NPSL_SUPP_MapError(int native_error)
{
	int          return_code = NPSL_SOCKET_FAILURE;
	unsigned int count_1;

	for (count_1 = 0; count_1 < (NPSL_SUPP_NativeErrorMapCount - 1); count_1++) {
		if (NPSL_SUPP_NativeErrorMapList[count_1].native_error == native_error) {
			return_code = NPSL_SUPP_NativeErrorMapList[count_1].npsl_error;
			break;
		}
	}

	return(return_code);
}
/* *********************************************************************** */

