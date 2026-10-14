# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Prefix Makefile for clang
# #############################################################################
#
# File Name       : clang_MakePrefix.mk
#
# File Description: Makefile prefix inclusion file for clang.
#
# Revision History: 2016-01-19 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2016 - 2018.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

# #############################################################################
# ##### Copy the gcc settings.
# ##### Really just for illustration purposes...
include ${MASCaPS_DIR_NAME}/gcc_MakePrefix.mk

CC	=	clang
CXX	=	clang

# ##### Use g++ for the link stage because clang barfs on fully-static build.
# ##### LINK.cpp and LINK.C are defined as equal to LINK.cc by GNU Make.
LINK.c  = gcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)
LINK.cc = g++ $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)

# #############################################################################

