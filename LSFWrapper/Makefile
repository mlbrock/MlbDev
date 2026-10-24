# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	LSFWrapper Library Make File
# #############################################################################
#
# File Name       : LSFWrapper/Makefile
#
# File Description: Tib/Rendezvous wrapper library make file.
#
# Revision History: 1998-04-08 --- Creation.
#                       Michael L. Brock
#                   2026-10-24 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 1998 - 2026.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

CPPFLAGS	+=	-I/opt/Platform/LSF/5.1/include \
			-I${INC_TIBRV_DIR}/tibrv \
			-I${INC_TIBRV_DIR}/../src/librvcpp

TARGET_LIBS	=	libLSFWrapper.a

TARGET_BINS	=

PENDING_SRCS	=	\
			LSFSimLimClient.cpp \
			LSFSimLimServer.cpp

SRCS		=	\
			DoneRex.cpp \
			GetClusterName.cpp \
			GetHostFactor.cpp \
			GetHostInfo.cpp \
			GetHostModel.cpp \
			GetHostType.cpp \
			HostInfo.cpp \
			InitRex.cpp \
			LockHost.cpp \
			LSFApiSim.cpp \
			PlaceReq.cpp \
			RKill.cpp \
			RTask.cpp \
			RTaskE.cpp \
			RWait.cpp \
			RWaitTid.cpp \
			StopRex.cpp \
			UnlockHost.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

