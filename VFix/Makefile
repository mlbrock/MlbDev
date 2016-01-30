# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	VFix Library Make File
# #############################################################################
#
# File Name       : VFix/Makefile
#
# File Description: VFix library make file.
#
# Revision History: 2015-11-05 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2015 - 2016.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libVFix.a

TARGET_BINS	=

SRCS		=	\
			CopyToCharArray.cpp	\
			PFixComponent.cpp	\
			PFixEnum.cpp		\
			PFixField.cpp		\
			PFixFieldType.cpp	\
			PFixMessage.cpp		\
			PFixMsgContent.cpp	\
			PFixRepository.cpp	\
			VFixXPortType.cpp

#LDFLAGS		+=	-L$(IBASE)/lib -L/usr/lib64/boost-1.38.0/lib 

LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	VFix RapidXmlUtils SocketIo Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

