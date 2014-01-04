# ##########################################################################
# ##########################################################################
# Network Programming Support Library (NPSL) Makefile
# ##########################################################################
#
# File Name       : Makefile
#
# File Version    : %I%
#
# Last Extracted  : %D%   %T%
#
# Last Updated    : %E%   %U%
#
# File Description: Makefile for the NPSL library.
#
# Revision History: 1993-04-12 --- Creation
#                        Michael L. Brock
#
#       Copyright Michael L. Brock 1993 - 2014.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# ##########################################################################

.KEEP_STATE:

THIS_HOME:sh				=	echo $HOME

include ${THIS_HOME}/src/.MASCAPS/premake.mk

TARGET_LIBS					=	npsl.a

ADF_INCS						=  ${INCLUDE_DIR}/npsl.h

SRCS							=	\
									accept.c \
									addrinet.c \
									bind.c \
									ckhentp.c \
									ckpentp.c \
									cksentp.c \
									cksock.c \
									cleanup.c \
									close.c \
									connect.c \
									cphent.c \
									cppent.c \
									cpsent.c \
									errortxt.c \
									exphostl.c \
									exphosts.c \
									frhent.c \
									frpent.c \
									frsent.c \
									gethent.c \
									gethentb.c \
									getmac.c \
									getpent.c \
									getpentb.c \
									getpname.c \
									getsent.c \
									getsentb.c \
									getsname.c \
									getsopt.c \
									hostname.c \
									inetaddr.c \
									inhent.c \
									inpent.c \
									insent.c \
									lenshent.c \
									lenspent.c \
									lenssent.c \
									listen.c \
									recv.c \
									recvfrom.c \
									rslvhost.c \
									rslvhsp.c \
									rslvprot.c \
									rslvserv.c \
									select.c \
									send.c \
									sendto.c \
									setbmode.c \
									setsopt.c \
									shutdown.c \
									socket.c \
									startup.c \
									support.c

LIBS							=	${TARGET_LIBS} ${LOCAL_LIBS}

${TARGET_LIBS}				:	${OBJS}
	@${AR} ${ARFLAGS} $@ ${OBJS}
	@ranlib $@
	@cp -p $@ ${LOCAL_LIB_DIR}/.

include ${THIS_HOME}/src/.MASCAPS/postmake.mk

