# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	First Prefix Makefile
# #############################################################################
#
# File Name       : MakePrefixFirst.mk
#
# File Description: Makefile prefix inclusion file (first).
#
# Revision History: 1992-08-04 --- Creation as .MASCaPS/premake.mk.
#                       Michael L. Brock
#                   2000-01-13 --- Split into .MASCaPS/MakeIncludePrefix.mk.
#                       Michael L. Brock
#                   2015-03-27 --- Modified as .MASCaPS/MakePrefixFirst.mk.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 1992 - 2015.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

# We are not initialized
MASCaPS_INIT_FLAG	:=

# Needed for comparisons...
MASCaPS_EMPTY_STRING	:=	"SOMETHING"
MASCaPS_EMPTY_STRING	:=

# You can define the MASCaPS_SPEC_DIR here (or in an invoking makefile).
# For example:
	MASCaPS_SPEC_DIR := Linux/3.12.8/x86_64/gcc/4.8.2/debug
# Otherwise, we'll base it upon the directory structure.

# Uncomment the following line to use the flattened makefile include structure.
# Alternatively, define within an invoking makefile.
MASCaPS_FLAT	:=	--flattened

# Ensure that this makefile was invoked from within another makefile...
ifeq (1 , ${words ${MAKEFILE_LIST}})
	THIS_FILE_NAME := ${lastword ${MAKEFILE_LIST}}
	TMP_VALUE      := ${error "This makefile ('${THIS_FILE_NAME}') must be a nested include of another makefile."}
endif

# Name of this makefile
CURR_FILE_NAME	:=	${lastword ${MAKEFILE_LIST}}
# Full, canonical name of this makefile
CURR_FILE_PATH	:=	${realpath ${CURR_FILE_NAME}}
# Directory in which this makefile resides (named /?/.MASCaPS)
MASCaPS_DIR_NAME	:=	${shell /bin/dirname ${CURR_FILE_PATH}}
# Directory in which the .MASCaPS directory resides
MASCaPS_DIR_PARENT	:=	${shell /bin/dirname ${MASCaPS_DIR_NAME}}
# Same, but with its trailing slash
MASCaPS_DIR_PARENT_S	:=	${MASCaPS_DIR_PARENT}/

# Name of the invoking makefile
PREV_FILE_NAME	:=	${lastword ${filter-out ${CURR_FILE_NAME}, \
	${MAKEFILE_LIST}}}
# Full, canonical name of the invoking makefile
PREV_FILE_PATH  :=	${realpath ${PREV_FILE_NAME}}
# Directory in which the invoking makefile resides
PREV_FILE_DIR	:=	${shell /bin/dirname ${PREV_FILE_PATH}}

# Unique name to assist in include list construction 
MASCaPS_UIDNAME	:=	${shell /bin/uuidgen}
MASCaPS_UIDNAME	:=	MASCaPS-UNIQUE-NAME-${MASCaPS_UIDNAME}

# If the MASCaPS_SPEC_DIR wasn't defined earlier, do so now...
ifeq (${MASCaPS_SPEC_DIR} , ${MASCaPS_EMPTY_STRING})
	# Strip the leading directory common to invoking makefile and
	# .MASCaPS directory
	TMP_SPEC_DIR := ${subst ${MASCaPS_DIR_PARENT_S},"",${PREV_FILE_DIR}}
	# If it hasn't changed, it wasn't common
	ifeq (${TMP_SPEC_DIR} , ${PREV_FILE_DIR})
		TMP_VALUE := ${error "The invoking makefile ('${PREV_FILE_PATH}') and the basic MASCaPS makefile ('${CURR_FILE_PATH}') do not share a common directory at the appropriate level ('${MASCaPS_DIR_PARENT}')"}
	endif
	MASCaPS_SPEC_DIR := ${TMP_SPEC_DIR}
endif

MASCaPS_FILE_LIST	:=	${shell ${MASCaPS_DIR_NAME}/CreateSpecList.pl \
	${MASCaPS_FLAT} --prefix=${MASCaPS_DIR_NAME} \
	--base_name=${MASCaPS_UIDNAME} ${MASCaPS_SPEC_DIR}}
FIRST_FILE_STRING	:=	${firstword ${MASCaPS_FILE_LIST}}
ifeq (${FIRST_FILE_STRING} , ERROR:)
	TMP_VALUE := ${error "Unable to construct the makefile include list: ${MASCaPS_FILE_LIST}"}
endif

MASCaPS_TARGET_BASE	:=	${MASCaPS_DIR_PARENT}/TargetPlatform/${MASCaPS_SPEC_DIR}
MASCaPS_TARGET_BIN	:=	${MASCaPS_TARGET_BASE}/bin
MASCaPS_TARGET_LIB	:=	${MASCaPS_TARGET_BASE}/lib

MASCaPS_PREFIX_LIST	:=	${subst ${MASCaPS_UIDNAME},MakePrefix, \
	${MASCaPS_FILE_LIST}}
MASCaPS_SUFFIX_LIST	:=	${subst ${MASCaPS_UIDNAME},MakeSuffix, \
	${MASCaPS_FILE_LIST}}
MASCaPS_CONTEXT_LIST	:=	${subst ${MASCaPS_UIDNAME},MakeContext, \
	${MASCaPS_FILE_LIST}}

# MASCaPS is now initialized
MASCaPS_INIT_FLAG	:=	"MASCaPS"

# Include the prefix makefiles...
-include ${MASCaPS_PREFIX_LIST}

# ... then include the context makefiles.
-include ${MASCaPS_CONTEXT_LIST}

# The per-target invoking makefile should include as its final command
# an invocation of the suffix makefiles as follows:
#	include ${MASCaPS_DIR_NAME}/MakeSuffixFirst.mk

# #############################################################################

ifdef (MASCaPS_DEBUG)
	XXX := ${info "===================== =========================="}
	XXX := ${info "MASCaPS_FLAT        : " ${MASCaPS_FLAT}}
	XXX := ${info "--------------------- --------------------------"}
	XXX := ${info "CURR_FILE_NAME      : " ${CURR_FILE_NAME}}
	XXX := ${info "CURR_FILE_PATH      : " ${CURR_FILE_PATH}}
	XXX := ${info "MASCaPS_DIR_NAME    : " ${MASCaPS_DIR_NAME}}
	XXX := ${info "MASCaPS_DIR_PARENT  : " ${MASCaPS_DIR_PARENT}}
	XXX := ${info "MASCaPS_DIR_PARENT_S: " ${MASCaPS_DIR_PARENT_S}}
	XXX := ${info "--------------------- --------------------------"}
	XXX := ${info "PREV_FILE_NAME      : " ${PREV_FILE_NAME}}
	XXX := ${info "PREV_FILE_PATH      : " ${PREV_FILE_PATH}}
	XXX := ${info "PREV_FILE_DIR       : " ${PREV_FILE_DIR}}
	XXX := ${info "--------------------- --------------------------"}
	XXX := ${info "MASCaPS_UIDNAME     : " ${MASCaPS_UIDNAME}}
	XXX := ${info "--------------------- --------------------------"}
	XXX := ${info "MASCaPS_TARGET_BASE : " ${MASCaPS_TARGET_BASE}}
	XXX := ${info "--------------------- --------------------------"}
	XXX := ${info "MASCaPS_SPEC_DIR    : " ${MASCaPS_SPEC_DIR}}
	XXX := ${info "MASCaPS_FILE_LIST   : " ${MASCaPS_FILE_LIST}}
	XXX := ${info "--------------------- --------------------------"}
	XXX := ${info "MASCaPS_PREFIX_LIST : " ${MASCaPS_PREFIX_LIST}}
	XXX := ${info "MASCaPS_SUFFIX_LIST : " ${MASCaPS_SUFFIX_LIST}}
	XXX := ${info "MASCaPS_CONTEXT_LIST: " ${MASCaPS_CONTEXT_LIST}}
	XXX := ${info "===================== =========================="}
endif

