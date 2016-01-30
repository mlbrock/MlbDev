# #############################################################################
# #############################################################################
# RapidXmlUtils Library Make File
# #############################################################################
#
# File Description: RapidXmlUtils library make file.
#
# Revision History: 2015-03-21 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2015 - 2016.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libRapidXmlUtils.a

TARGET_BINS	=

SRCS		=	\
			RapidXmlContext.cpp			\
			XmlDomElement.cpp			\
			XmlStringToString.cpp

LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	RapidXmlUtils Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

