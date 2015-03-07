# #############################################################################
# #############################################################################
# VFast Library Make File
# #############################################################################
#
# File Description: VFast library make file.
#
# Revision History: 2008-11-23 --- Creation.
#                       Michael L. Brock
#
#
#       Copyright Michael L. Brock 2008 - 2015.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.make/MakePrefix.mk

TARGET_LIBS	=	libVFast.a

TARGET_BINS	=

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

LDFLAGS		+=	-L$(IBASE)/lib -L/usr/lib64/boost-1.38.0/lib 

include ../.make/MakeSuffix.mk

