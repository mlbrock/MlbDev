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
#                   2000-01-13 --- Split into as .MASCaPS/MakeSuffix.mk.
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
OBJS		=	${SRCS:.c=.o}
OBJS		=	${SRCS:.cpp=.o}
DEPS		=	${SRCS:.c=.P}
DEPS		=	${SRCS:.cpp=.P}

TARGET_LIBS_DST	   = ${addprefix ${MASCaPS_TARGET_LIB},${TARGET_LIBS}}
TARGET_LIBS_SO_DST = ${addprefix ${MASCaPS_TARGET_LIB},${TARGET_LIBS_SO}}
TARGET_BINS_DST	   = ${addprefix ${MASCaPS_TARGET_BIN},${TARGET_BINS}}

all		:	${TARGET_LIBS}	\
			${TARGET_LIBS_SO} \
			${TARGET_BINS}

#XXX = @echo ${TARGET_LIBS_DST}

${TARGET_LIBS}	:	${OBJS} ${DEPS}
	@${AR} ${ARFLAGS} $@ ${OBJS}
	@ranlib $@
	@cp -p $@ ${MASCaPS_TARGET_LIB}/.

# ##### For shared libraries.
${TARGET_LIBS_SO}	:	${OBJS} ${DEPS}
	gcc -shared -W1,-soname,$@ -o $@.0 ${OBJS}
	@cp -p $@.0 ${MASCaPS_TARGET_LIB}/.

${TARGET_BINS}	:	${MLB_LIB_FULL}

%.P		:	%.cpp
	$(COMPILE.cc) -MD -o $@ $<
	@cp $*.d $*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
		rm -f $*.d

-include $(SRCS:.cpp=.P)

.PHONY		:	clean clean_objs clean_libs clean_bins

clean_objs	:
	-@/bin/rm ${OBJS} > /dev/null 2>&1

clean_libs	:
	-@rm ${TARGET_LIBS} ../lib/${TARGET_LIBS} > /dev/null 2>&1

clean_bins	:
	-@rm ${TARGET_BINS} ../bin/${TARGET_BINS} > /dev/null 2>&1

clean		:	clean_objs	\
			clean_libs	\
			clean_bins
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

# ###################################################################
# ###################################################################
# Automatic builds of modules with 'TEST_MAIN' harnesses . . .
# ###################################################################
TEST_BASE_NAMES_CMD		=	\
	grep TEST_MAIN ${SRCS} ${BIN_SRCS} | \
	/usr/bin/perl -e \
		'while (<>) { \
			chop; \
			if (($$_ =~ /^.+:\s*\#\s*ifdef\s+TEST_MAIN/) && \
				($$_ !~ /^test_|^testpurify_|^testquantify_|^testpurecov_/)) { \
				$$_ =~ s/^(.+)\..+:.+/\1/; \
				print $$_, "\n"; \
			} \
		} \
		exit(0);'

TEST_BASE_NAMES			=	${shell ${TEST_BASE_NAMES_CMD}}

TEST_NAMES			=	$(TEST_BASE_NAMES:%=test_%)
TEST_PURIFY_NAMES		=	$(TEST_BASE_NAMES:%=testpurify_%)
TEST_QUANTIFY_NAMES		=	$(TEST_BASE_NAMES:%=testquantify_%)
TEST_COVERAGE_NAMES		=	$(TEST_BASE_NAMES:%=testcoverage_%)

test_all			:	${TEST_NAMES} ${SRCS}

testpurify_all			:	${TEST_PURIFY_NAMES} ${SRCS}

testquantify_all		:	${TEST_QUANTIFY_NAMES} ${SRCS}

testcoverage_all		:	${TEST_COVERAGE_NAMES} ${SRCS}

# ###################################################################

