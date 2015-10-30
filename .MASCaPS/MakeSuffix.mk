# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Suffix Makefile
# #############################################################################
#
# File Name       : MakeSuffix.mk
#
# File Description: Makefile prefix inclusion file.
#
# Revision History: 1992-08-04 --- Creation as .MASCaPS/postmake.mk.
#                       Michael L. Brock
#                   2000-01-13 --- Split into .MASCaPS/MakeSuffix.mk.
#                       Michael L. Brock
#                   2015-03-27 --- Modified as .MASCaPS/MakeSuffix.mk.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 1992 - 2015.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

# #############################################################################

# -----------------------------------------------------------------------------
OBJS		:=	${SRCS:.c=.o}
OBJS		:=	${OBJS:.cpp=.o}
OBJS	   	:= 	${addprefix ${MASCaPS_TARGET_OBJ}/,${OBJS}}
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
DEPS		:=	${SRCS:.c=.dep}
DEPS		:=	${DEPS:.cpp=.dep}
DEPS	   	:= 	${addprefix ${MASCaPS_TARGET_DEP}/,${DEPS}}
# -----------------------------------------------------------------------------

${MASCaPS_TARGET_OBJ}/%.o	:	%.c ${MASCaPS_TARGET_DEP}/%.dep
	@mkdir -p ${@D}
	${COMPILE.c} -o $@ $<

${MASCaPS_TARGET_OBJ}/%.o	:	%.cpp ${MASCaPS_TARGET_DEP}/%.dep
	@mkdir -p ${@D}
	${COMPILE.cc} -o $@ $<

TARGET_LIBS_DST	   = ${addprefix ${MASCaPS_TARGET_LIB}/,${TARGET_LIBS}}
TARGET_LIBS_SO_DST = ${addprefix ${MASCaPS_TARGET_LIB}/,${TARGET_LIBS_SO}}
TARGET_BINS_DST	   = ${addprefix ${MASCaPS_TARGET_BIN}/,${TARGET_BINS}}

all		:	${TARGET_LIBS}	\
			${TARGET_LIBS_SO} \
			${TARGET_BINS_DST}

# -----------------------------------------------------------------------------
# ##### For static libraries.
${TARGET_LIBS}		:	${DEPS} ${OBJS}
	@${AR} ${ARFLAGS} $@ ${OBJS}
	@ranlib $@
	@cp -p $@ ${MASCaPS_TARGET_LIB}/.
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# ##### For shared libraries.
${TARGET_LIBS_SO}	: ${DEPS} ${OBJS}
	gcc -shared -Wl,-soname,$@ -o $@.0 ${OBJS}
	@cp -p $@.0 ${MASCaPS_TARGET_LIB}/.
# -----------------------------------------------------------------------------

${TARGET_BINS}		:	${DEPS} ${MLB_LIB_FULL}

${TARGET_BINS_DST}	:	${TARGET_BINS}
	@cp -p ${TARGET_BINS} ${MASCaPS_TARGET_BIN}/.

${MASCaPS_TARGET_DEP}/%.dep	:	%.cpp
	@mkdir -p ${@D}
	$(COMPILE.cc) -MD -o $@ $<
	@cp ${MASCaPS_TARGET_DEP}/$*.d ${MASCaPS_TARGET_DEP}/$*.dep; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < ${MASCaPS_TARGET_DEP}/$*.d >> \
		${MASCaPS_TARGET_DEP}/$*.dep; \
		rm -f ${MASCaPS_TARGET_DEP}/$*.d

${MASCaPS_TARGET_DEP}/%.dep	:	%.c
	@mkdir -p ${@D}
	$(COMPILE.c) -MD -o $@ $<
	@cp ${MASCaPS_TARGET_DEP}/$*.d ${MASCaPS_TARGET_DEP}/$*.dep; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < ${MASCaPS_TARGET_DEP}/$*.d >> \
		${MASCaPS_TARGET_DEP}/$*.dep; \
		rm -f ${MASCaPS_TARGET_DEP}/$*.d

-include ${DEPS}

# #############################################################################

# #############################################################################
# #############################################################################
# Basic test executables
# #############################################################################
test_%.c		:	%.c
	@rm -f $@
	@cp -p $< $@
	@chmod u+w $@

test_%.o		:	test_%.c
	${COMPILE.c} $< -DTEST_MAIN

test_%.cpp		:	%.cpp
	@rm -f $@
	@cp -p $< $@
	@chmod u+w $@

test_%.o		:	test_%.cpp
	${COMPILE.cc} $< -DTEST_MAIN

test_%			:	test_%.o
	${LINK.cc} -o $@ $< ${LDFLAGS} ${LDLIBS} ${LIBS} ${LDFLAGS}
# #############################################################################

# #############################################################################
# #############################################################################
# Purify test executables
# #############################################################################
testpurify_%.c		:	%.c
	@rm -f $@
	@cp -p $< $@
	@chmod u+w $@

testpurify_%.o		:	testpurify_%.c
	${COMPILE.c} $< -DTEST_MAIN

testpurify_%.cpp	:	%.cpp
	@rm -f $@
	@cp -p $< $@
	@chmod u+w $@

testpurify_%.o		:	testpurify_%.cpp
	${COMPILE.cc} $< -DTEST_MAIN

testpurify_%		:	testpurify_%.o
	purify ${LINK.cc} -o $@ $< ${LDFLAGS} ${LDLIBS} ${LIBS} ${LDFLAGS}
# #############################################################################

# #############################################################################
# #############################################################################
# Quantify test executables
# #############################################################################
testquantify_%.c	:	%.c
	@rm -f $@
	@cp -p $< $@
	@chmod u+w $@

testquantify_%.o	:	testquantify_%.c
	${COMPILE.c} $< -DTEST_MAIN

testquantify_%.cpp	:	%.cpp
	@rm -f $@
	@cp -p $< $@
	@chmod u+w $@

testquantify_%.o	:	testquantify_%.cpp
	${COMPILE.cc} $< -DTEST_MAIN

testquantify_%		:	testquantify_%.o
	quantify ${LINK.cc} -o $@ $< ${LDFLAGS} ${LDLIBS} ${LIBS} ${LDFLAGS}
# #############################################################################

# #############################################################################
# #############################################################################
# Pure Coverage test executables
# #############################################################################
testcoverage_%.c	:	%.c
	@rm -f $@
	@cp -p $< $@
	@chmod u+w $@

testcoverage_%.o	:	testcoverage_%.c
	${COMPILE.c} $< -DTEST_MAIN

testcoverage_%.cpp	:	%.cpp
	@rm -f $@
	@cp -p $< $@
	@chmod u+w $@

testcoverage_%.o	:	testcoverage_%.cpp
	${COMPILE.cc} $< -DTEST_MAIN

testcoverage_%		:	testcoverage_%.o
	coverage ${LINK.cc} -o $@ $< ${LDFLAGS} ${LDLIBS} ${LIBS} ${LDFLAGS}
# #############################################################################

# #############################################################################
# #############################################################################
# Automatic builds of modules with 'TEST_MAIN' harnesses . . .
# #############################################################################
TEST_SRC_LIST			 =	${SRCS} ${BIN_SRCS}
TEST_SRC_LIST			:=	$(strip ${TEST_SRC_LIST})
ifeq (${TEST_SRC_LIST} , ${MASCaPS_EMPTY_STRING})
	TEST_BASE_NAMES_CMD	=
else
	TEST_BASE_NAMES_CMD	=	\
	grep TEST_MAIN ${TEST_SRC_LIST} | \
	perl -e \
		'while (<>) { \
			chop; \
			if (($$_ =~ /^.+:\s*\#\s*ifdef\s+TEST_MAIN/) && \
				($$_ !~ /^test_|^testpurify_|^testquantify_|^testpurecov_/)) { \
				$$_ =~ s/^(.+)\..+:.+/\1/; \
				print $$_, "\n"; \
			} \
		} \
		exit(0);'
endif

TEST_BASE_NAMES			=	${shell ${TEST_BASE_NAMES_CMD}}

TEST_NAMES			=	$(TEST_BASE_NAMES:%=test_%)
TEST_PURIFY_NAMES		=	$(TEST_BASE_NAMES:%=testpurify_%)
TEST_QUANTIFY_NAMES		=	$(TEST_BASE_NAMES:%=testquantify_%)
TEST_PURECOV_NAMES		=	$(TEST_BASE_NAMES:%=testpurecov_%)

test_all			:	${TEST_NAMES} ${SRCS}

tests_all			:	test_all

testpurify_all			:	${TEST_PURIFY_NAMES} ${SRCS}

testquantify_all		:	${TEST_QUANTIFY_NAMES} ${SRCS}

testpurecov_all			:	${TEST_PURECOV_NAMES} ${SRCS}

# #############################################################################

# #############################################################################
.PHONY		:	\
			MASCaPS_FORCED_TARGET \
			clean clean_deps clean_objs clean_libs clean_bins \
			clean_tests clean_tests_purify clean_tests_quantify \
			clean_tests_purecov
# #############################################################################

# #############################################################################
# #############################################################################
# Clean support...
# #############################################################################

clean_deps	:
	-@rm ${DEPS} > /dev/null 2>&1

clean_objs	:
	-@rm ${OBJS} > /dev/null 2>&1

clean_libs	:
	-@rm ${TARGET_LIBS} ${TARGET_LIBS_SO} ${TARGET_LIBS_DST} ${TARGET_LIBS_SO_DST} > /dev/null 2>&1

clean_bins	:
	-@rm ${TARGET_BINS} ${TARGET_BINS_DST} > /dev/null 2>&1

clean_tests	:
	-@rm test_* > /dev/null 2>&1

clean_tests_purify:
	-@rm testpurify_* > /dev/null 2>&1

clean_tests_quantify:
	-@rm testquantify_* > /dev/null 2>&1

clean_tests_purecov:
	-@rm testpurecov_* > /dev/null 2>&1

clean_tests_all	:	\
			clean_tests		\
			clean_tests_purify	\
			clean_tests_quantify	\
			clean_tests_purecov

clean		:	\
			clean_deps	\
			clean_objs	\
			clean_libs	\
			clean_bins	\
			clean_tests_all
# #############################################################################

