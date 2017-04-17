# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Check MASCaPS Initialization State Makefile
# #############################################################################
#
# File Name       : MakeCheckInit.mk
#
# File Description: Checks to ensure some basic MASCaPS state is initialized.
#
# Revision History: 2015-03-28 --- Modified as .MASCaPS/MakeSuffixFirst.mk.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2015 - 2017.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

ifndef MASCaPS_INIT_FLAG
	TMP_VALUE := $(error "The MASCaPS state macros have not been initialized.")
endif

override MASCaPS_EMPTY_STRING	:=

ifeq (${MASCaPS_INIT_FLAG} , ${MASCaPS_EMPTY_STRING})
	TMP_VALUE := $(error "The MASCaPS state macros have not been initialized.")
endif

# #############################################################################

