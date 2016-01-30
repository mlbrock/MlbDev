# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	First Suffix Makefile
# #############################################################################
#
# File Name       : MakeSuffixFirst.mk
#
# File Description: Makefile suffix inclusion file (first).
#
# Revision History: 1992-08-04 --- Creation as .MASCaPS/postmake.mk.
#                       Michael L. Brock
#                   2000-01-13 --- Split into .MASCaPS/MakeIncludeSuffix.mk.
#                       Michael L. Brock
#                   2015-03-27 --- Modified as .MASCaPS/MakeSuffixFirst.mk.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 1992 - 2016.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ${MASCaPS_DIR_NAME}/MakeCheckInit.mk

-include ${MASCaPS_SUFFIX_LIST}

# #############################################################################

