# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	XercesUtils Library Make File
# #############################################################################
#
# File Name       : XercesUtils/Makefile
#
# File Description: XercesUtils library make file.
#
# Revision History: 2005-01-08 --- Creation.
#                       Michael L. Brock
#                   2015-03-29 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2005 - 2016.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libXercesUtils.a

TARGET_BINS	=

SRCS		=	\
			XercesContext.cpp			\
			XmlDomElement.cpp			\
			XmlStringToString.cpp

LINK_STATIC	=	${LINK_STATIC_STD}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	XercesUtils Utility

LDLIBS		:=	-L${LIB_XERCES_DIR} -lxerces-c ${LDLIBS}

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

