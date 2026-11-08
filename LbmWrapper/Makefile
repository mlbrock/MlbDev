# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	LbmWrapper Library Make File
# #############################################################################
#
# File Name       : LbmWrapper/Makefile
#
# File Description: LbmWrapper library make file.
#
# Revision History: 2008-08-17 --- Creation.
#                       Michael L. Brock
#                   2026-10-24 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2008 - 2026.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libLbmWrapper.a

TARGET_BINS	=

SRCS		=	\
			LbmAttrSupport.cpp				\
			LbmContext.cpp					\
			LbmEventQueue.cpp				\
			LbmException.cpp				\
			LbmExceptionStatus.cpp				\
			LbmLogSupport.cpp				\
			LbmMiscSupport.cpp				\
			LbmRcv.cpp					\
			LbmSrc.cpp					\
			LbmStatsContext.cpp				\
			LbmStatsEventQueue.cpp				\
			LbmStatsRcv.cpp					\
			LbmStatsSrc.cpp					\
			LbmTopicRcv.cpp					\
			LbmTopicSrc.cpp					\
			LbmWildcardRcv.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	LbmWrapper Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

