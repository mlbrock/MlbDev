# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	VFast Library Make File
# #############################################################################
#
# File Name       : VFast/Makefile
#
# File Description: VFast library make file.
#
# Revision History: 2008-11-23 --- Creation.
#                       Michael L. Brock
#                   2015-03-29 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2008 - 2016.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libVFast.a

TARGET_BINS	=	\
			Tools/AnalyzeFieldSpecs				\
			Tools/CheckFieldDispatch			\
			Tools/EbsDeltaAnalyzer				\
			Tools/EbsOrderBookEventAnalyzer			\
			Tools/ExtractEbsStreams				\
			Tools/VFastMsgParser

SRCS		=	\
			CmeConfigFile.cpp				\
			CmeSupport.cpp					\
			DataType.cpp					\
			DictValue.cpp					\
			ExcContext.cpp					\
			ExcEntry.cpp					\
			ExcEntryLookup.cpp				\
			ExcResults.cpp					\
			FieldOperator.cpp				\
			InsContext.cpp					\
			InsItem.cpp					\
			PacketDecodeRegime.cpp				\
			PacketFileInterface.cpp				\
			SpecExceptionBase.cpp				\
			VFastException.cpp				\
			VFastParseCmdLineArg.cpp			\
			VFastUtility.cpp				\
			XdpSupport.cpp

#LDFLAGS		+=	-L$(IBASE)/lib -L/usr/lib64/boost-1.38.0/lib 

LINK_STATIC	=	${LINK_STATIC_STD}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	VFast XercesUtils SocketIo Utility

LDLIBS		:=	-L${LIB_XERCES_DIR} -lxerces-c ${LDLIBS}

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

