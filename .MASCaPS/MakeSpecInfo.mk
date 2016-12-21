# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Determine MASCaPS Specification Information Makefile
# #############################################################################
#
# File Name       : MakeSpecInfo.mk
#
# File Description: Determines the MASCaPS specification information
#
# Revision History: 2015-10-24 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2015 - 2016.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

# #############################################################################

override MASCaPS_EMPTY_STRING	:=

# -----------------------------------------------------------------------------
# You can define the MASCaPS_SPEC_DIR here (or in an invoking makefile).
# For example, in this file un-comment the following line:
#	MASCaPS_SPEC_DIR := Linux/3.12.8/x86_64/gcc/4.8.2/debug
#
# Or you could specify in the command line of the invoiking makefile, like:
#	make --eval=MASCaPS_SPEC_DIR=Linux/3.12.8/x86_64/gcc/4.8.2/debug
#
# Or you could specify it as an environment variable. But you also must tell
# Gnu Make to give variables defined in the environment precedence over
# variables defined in makefiles:
#	export MASCaPS_SPEC_DIR=Linux/3.12.8/x86_64/gcc/4.8.2/debug
#	make --environment-overrides
#
# Otherwise, we'll construct a specification using the uname command. In this
# case, the compiler will default to 'gcc' and the build type to 'debug'.
#
# If the MASCaPS_SPEC_DIR wasn't defined earlier, do so now...
ifeq (${MASCaPS_SPEC_DIR} , ${MASCaPS_EMPTY_STRING})
	# Get the default spec info...
	ifeq (${MASCaPS_SPEC_OS} , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_OS	:=	${shell uname -s}
	endif
	ifeq (${MASCaPS_SPEC_OSVER} , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_OSVER	:=	${shell uname -r | cut -d ' ' -f 1 | cut -d '-' -f 1}
	endif
	ifeq (${MASCaPS_SPEC_MACH} , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_MACH	:=	${shell uname -m}
	endif
	ifeq (${MASCaPS_SPEC_CPP} , ${MASCaPS_EMPTY_STRING})
		TMP_CC_VERSION	:=	${shell ${CC} --version | head -1}
		TMP_GCC_NAME	:=	${or ${findstring GCC,${TMP_CC_VERSION}},${findstring Gcc,${TMP_CC_VERSION}},${findstring gcc,${TMP_CC_VERSION}}}
		ifneq (,${TMP_GCC_NAME})
			MASCaPS_SPEC_CPP	:=	gcc
		else
			MASCaPS_SPEC_CPP	:=	${shell ${CC} --version | head -1 | cut -d ' ' -f 1 | cut -d '-' -f 1 | cut -d '.' -f 1}
		endif
	endif
	ifeq (${MASCaPS_SPEC_CPPVER} , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_CPPVER	:=	${shell ${CC} --version | head -1 | perl -e '$$_=<>;$$_=~s/\(.+?\)/\(\-\)/g;$$_=~s/\[.+?\]/\[\-\]/g;$$_=~s/\<.+?\>/\<\-\>/g;$$_=~s/\{.+?\}/\{\-\}/g;print $$_;' | cut -d ' ' -f 3 | cut -d '-' -f 1}
	endif
	ifeq (${MASCaPS_SPEC_BUILD} , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_BUILD	:=	debug
	endif
	MASCaPS_SPEC_DIR	:=	${MASCaPS_SPEC_OS}/${MASCaPS_SPEC_OSVER}/${MASCaPS_SPEC_MACH}/${MASCaPS_SPEC_CPP}/${MASCaPS_SPEC_CPPVER}/${MASCaPS_SPEC_BUILD}
	MASCaPS_SPEC_SRC	:=	Calculated
else
	TMP_SPEC_DIR	:=	${MASCaPS_SPEC_DIR}
	TMP_SPEC_DIR	:=	$(strip ${TMP_SPEC_DIR})
	TMP_SPEC_DIR	:=	${subst /, ,${TMP_SPEC_DIR}}
	TMP_SPEC_DIR	:=	$(strip ${TMP_SPEC_DIR})
	ifeq (${TMP_SPEC_DIR} , ${MASCaPS_EMPTY_STRING})
		XXX := ${error "The MASCaPS specification string '${MASCaPS_SPEC_DIR}' is invalid."}
	endif
	ifneq ($(word 1, ${TMP_SPEC_DIR}) , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_OS := $(word 1, ${TMP_SPEC_DIR})
	else
		MASCaPS_SPEC_OS :=
	endif
	ifneq ($(word 2, ${TMP_SPEC_DIR}) , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_OSVER := $(word 2, ${TMP_SPEC_DIR})
	else
		MASCaPS_SPEC_OSVER :=
	endif
	ifneq ($(word 3, ${TMP_SPEC_DIR}) , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_MACH := $(word 3, ${TMP_SPEC_DIR})
	else
		MASCaPS_SPEC_MACH :=
	endif
	ifneq ($(word 4, ${TMP_SPEC_DIR}) , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_CPP := $(word 4, ${TMP_SPEC_DIR})
	else
		MASCaPS_SPEC_CPP :=
	endif
	ifneq ($(word 5, ${TMP_SPEC_DIR}) , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_CPPVER := $(word 5, ${TMP_SPEC_DIR})
	else
		MASCaPS_SPEC_CPPVER :=
	endif
	ifneq ($(word 6, ${TMP_SPEC_DIR}) , ${MASCaPS_EMPTY_STRING})
		MASCaPS_SPEC_BUILD := $(word 6, ${TMP_SPEC_DIR})
	else
		MASCaPS_SPEC_BUILD :=
	endif
	MASCaPS_SPEC_SRC	:=	Specified
endif
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# If we were included from MakePrefixFirst.mk it was to set the various
# specification values. Otherwise, we assume the specification output was
# desired.
# -----------------------------------------------------------------------------
ifeq (1 , ${words ${MAKEFILE_LIST}})
	TMP_SHOW_MASCaPS_SPEC_INFO	:=	Yes
else
	TMP_SHOW_MASCaPS_SPEC_INFO	:=	No
	TMP_CURR_FILE_NAME := ${lastword ${MAKEFILE_LIST}}
	TMP_PREV_FILE_NAME := ${lastword ${filter-out ${TMP_CURR_FILE_NAME}, \
		${MAKEFILE_LIST}}}
	TMP_PREV_FILE_PATH := ${realpath ${TMP_PREV_FILE_NAME}}
	TMP_PREV_FILE_BASE := ${shell basename ${TMP_PREV_FILE_PATH}}
	ifneq (${TMP_PREV_FILE_BASE} , MakePrefixFirst.mk)
		TMP_SHOW_MASCaPS_SPEC_INFO	:=	Yes
	endif
endif

# -----------------------------------------------------------------------------
ifeq (${TMP_SHOW_MASCaPS_SPEC_INFO} , Yes)
 X:=${info ===================================================================}
 X:=${info MASCaPS_SPEC_SRC    : ${MASCaPS_SPEC_SRC}}
 X:=${info MASCaPS_SPEC_DIR    : ${MASCaPS_SPEC_DIR}}
 X:=${info -------------------------------------------------------------------}
 X:=${info MASCaPS_SPEC_OS     : ${MASCaPS_SPEC_OS}}
 X:=${info MASCaPS_SPEC_OSVER  : ${MASCaPS_SPEC_OSVER}}
 X:=${info MASCaPS_SPEC_MACH   : ${MASCaPS_SPEC_MACH}}
 X:=${info MASCaPS_SPEC_CPP    : ${MASCaPS_SPEC_CPP}}
 X:=${info MASCaPS_SPEC_CPPVER : ${MASCaPS_SPEC_CPPVER}}
 X:=${info MASCaPS_SPEC_BUILD  : ${MASCaPS_SPEC_BUILD}}
 X:=${info ===================================================================}
endif
# -----------------------------------------------------------------------------

# #############################################################################

