# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	ProtoBuf Library Make File
# #############################################################################
#
# File Name       : ProtoBuf/Makefile
#
# File Description: Google ProtoBuf interface library make file.
#
# Revision History: 2015-06-14 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2015 - 2026.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libProtoBuf.a

TARGET_BINS	=

SRCS		=	\
			AreGpbTypesCompatible.cpp \
			GetMessageDescriptor.cpp \
			GetMessageNew.cpp \
			GetMessagePrototype.cpp \
			GpbAccessor.cpp \
			GpbElementInfo.cpp \
			GpbElementInfoMaxLengths.cpp \
			GpbMessageConverter.cpp \
			GpbMessageTypes.cpp \
			IsGpbType.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	ProtoBuf Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

