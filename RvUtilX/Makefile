# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	RvUtilX Library Make File
# #############################################################################
#
# File Name       : RvUtilX/Makefile
#
# File Description: Tib/Rendezvous wrapper library make file.
#
# Revision History: 2001-10-01 --- Creation.
#                       Michael L. Brock
#                   2026-10-24 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2001 - 2026.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

CPPFLAGS	+=	-I${INC_TIBRV_DIR}/tibrv \
			-I${INC_TIBRV_DIR}/../src/librvcpp

TARGET_LIBS	=	libRvUtilX.a

TARGET_BINS	=

SRCS		=	\
			AddRvField.cpp \
			CopyMsg.cpp \
			DetachMsg.cpp \
			FtSpec.cpp \
			GetRvField.cpp \
			GetSubject.cpp \
			ReplyInfoRv.cpp \
			RequestApiRv.cpp \
			RvAdvFlags.cpp \
			RvAdvisoryControl.cpp \
			RvAdvMsg.cpp \
			RvDateTime.cpp \
			RvEnvironment.cpp \
			RvEventType.cpp \
			RvException.cpp \
			RvFieldType.cpp \
			RvFtAction.cpp \
			RvFtMember.cpp \
			RvFtMonitor.cpp \
			RvIOType.cpp \
			RvListener.cpp \
			RvMsgAdd.cpp \
			RvMsg.cpp \
			RvMsgDataType.cpp \
			RvMsgFieldDataToString.cpp \
			RvMsgSnapshot.cpp \
			RvMsgToString.cpp \
			RvParseCmdLineArg.cpp \
			RvParseCmdLineArgFtSpec.cpp \
			RvQueue.cpp \
			RvQueueGroup.cpp \
			RvQueueLimitPolicy.cpp \
			RvTransportCm.cpp \
			RvTransportCmQueue.cpp \
			RvTransport.cpp \
			RvTransportNet.cpp \
			SendRequestSpecificReply.cpp \
			SNDSpec.cpp \
			SubjectNameFlat.cpp \
			SubjectNameToMultiPartList.cpp \
			SubjectNameWork.cpp \
			ThrowRvException.cpp \
			UpdateRvField.cpp \
			WaitForMessage.cpp \
			XRvSendControl.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

