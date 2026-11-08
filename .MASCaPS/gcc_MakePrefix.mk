# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Prefix Makefile for gcc/g++ Builds
# #############################################################################
#
# File Name       : gcc_MakePrefix.mk
#
# File Description: Makefile prefix inclusion file for gcc/g++ builds.
#
# Revision History: 2015-10-25 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2015 - 2026.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

# #############################################################################

CPPFLAGS	+=	\
			-Woverlength-strings	\
			-Wformat		\
			-Wuninitialized

CXXFLAGS	+=	\
			-Woverloaded-virtual

# #############################################################################

