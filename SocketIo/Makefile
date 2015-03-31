# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	SocketIo Library Make File
# #############################################################################
#
# File Name       : SocketIo/Makefile
#
# File Description: SocketIo library make file.
#
# Revision History: 2008-07-31 --- Creation.
#                       Michael L. Brock
#                   2015-03-31 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2008 - 2015.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libSocketIo.a

TARGET_BINS	=

SRCS		=	\
			IpAddressGeneric.cpp		\
			MulticastIP.cpp			\
			PacketFileReader.cpp		\
			PacketFileWriter.cpp		\
			PacketFormat.cpp		\
			PacketReceiver.cpp		\
			PacketReceiverAsyncMC.cpp	\
			PacketReceiverAsyncMCast.cpp	\
			PacketReceiverMC.cpp		\
			PacketReceiverMCast.cpp		\
			PacketReceiverUdp.cpp		\
			PacketSender.cpp		\
			PacketSenderMC.cpp		\
			PacketSenderMCast.cpp		\
			PacketSenderUdp.cpp		\
			PacketSocket.cpp		\
			PacketSocketAsync.cpp		\
			ResolveHost.cpp			\
			ResolveService.cpp		\
			SocketIoParseCmdLineArg.cpp	\
			SocketSpec.cpp			\
			SocketSpecLocal.cpp		\
			StreamAcceptor.cpp		\
			StreamAcceptorAsync.cpp		\
			StreamSocket.cpp		\
			StreamSocketAsync.cpp		\
			StringToIpAddress.cpp		\
			TimerHandler.cpp

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	SocketIo Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

