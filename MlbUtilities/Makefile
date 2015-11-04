# #############################################################################
# #############################################################################
# MLB Utilities Executable Programs Make File
# #############################################################################
#
# File Description: Executable programs make file for miscellaneous MLB
#                   utilities.
#
# Revision History: 2008-07-31 --- Creation.
#                       Michael L. Brock
#                   2015-10-24 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2008 - 2015.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_BINS	=	\
			CannedPacketConverter		\
			CannedPacketConverterToPCap	\
			CannedPacketDumper		\
			CannedPacketRecv		\
			CannedPacketSend

SRCS		=

LINK_STATIC	=	${LINK_STATIC_STD}

# Only the MLB libraries below are needed for the executables in this Makefile.
MLB_LIB_NAMES	=	SocketIo Utility

LDLIBS		:=	-L${LIB_PCAP_DIR} -lpcap ${LDLIBS}

include ../.MASCaPS/MakeSuffixFirst.mk

