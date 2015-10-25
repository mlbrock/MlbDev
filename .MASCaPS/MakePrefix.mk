# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Prefix Makefile
# #############################################################################
#
# File Name       : MakePrefix.mk
#
# File Description: Makefile prefix inclusion file.
#
# Revision History: 1992-08-04 --- Creation as .MASCaPS/premake.mk.
#                       Michael L. Brock
#                   2000-01-13 --- Split into as .MASCaPS/MakePrefix.mk.
#                       Michael L. Brock
#                   2015-03-27 --- Modified as .MASCaPS/MakePrefix.mk.
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

TMP_FILE_NAME	:=	${lastword ${MAKEFILE_LIST}}
TMP_DIR_NAME_1	:=	${shell /bin/dirname ${TMP_FILE_NAME}}
TMP_DIR_NAME_2	:=	${shell /bin/dirname ${TMP_DIR_NAME_1}}

CPP_BUILD_TYPE	=

LIB_BASE	=	/usr/lib64

VER_BOOST	=	boost_1_54_0
VER_BOOST_FS	=	3
VER_TIBRV	=	8.1
#VER_XERCES	=	xercesc-3.0.1
VER_XERCES	=	xercesc-c
VER_PCAP	=	/opt/SomeLocationTBD/LibPCap/1.2.0

INC_BASIC_DIR	=	./
INC_OTHER_DIR	=	/usr/include
#INC_BOOST_DIR	=       /usr/include/boost/${VER_BOOST}/include
INC_BOOST_DIR	=       /usr/include
INC_TIBRV_DIR	=	/opt/tibco/tibrv/${VER_TIBRV}/include
INC_LBM_DIR	=	/home/brockm/Downloads/29West/UMS_5.0/Linux-glibc-2.5-x86_64/include
#INC_XERCES_DIR	=	/usr/include/${VER_XERCES}/include
INC_CELOX_DIR	=	${VER_CELOX}/share/celoxica/include
INC_PCAP_DIR	=	${VER_PCAP}/include

LIB_BASIC_DIR	=	./
LIB_OTHER_DIR	=	/usr/lib64
#LIB_BOOST_DIR	=	${LIB_BASE}/${VER_BOOST}
LIB_BOOST_DIR	=	${LIB_OTHER_DIR}
LIB_TIBRV_DIR	=	/opt/tibco/tibrv/${VER_TIBRV}/lib
LIB_LBM_DIR	=	/home/brockm/Downloads/29West/UMS_5.0/Linux-glibc-2.5-x86_64/lib
LIB_XERCES_DIR	=	${LIB_BASE}/${VER_XERCES}/lib
LIB_PCAP_DIR	=	${VER_PCAP}/lib64

# Not supported in g++ 4.1.
#			-Woverlength-strings \

#			-DBOOST_FILESYSTEM_VERSION=${VER_BOOST_FS} \

CPPFLAGS	+=	\
			-rdynamic \
			-D__STDC_FORMAT_MACROS \
			${DEF_CELOX_CPP} \
			${CPP_BUILD_TYPE} \
			-I ./ \
			-I ${INC_BASIC_DIR} \
			-I ../include \
			${CPPFLAGS_ADDED} \
			-I ${INC_OTHER_DIR} \
			-I ${INC_LBM_DIR} \
			-I ${INC_TIBRV_DIR} \
			-I ${INC_BOOST_DIR} \
			-I ${INC_CELOX_DIR} \
			-I ${INC_PCAP_DIR} \
			-D_POSIX_PTHREAD_SEMANTICS \
			-Wall \
			-W \
			-Wextra \
			-Winit-self \
			-fmessage-length=0 \
			-Wno-deprecated-declarations \
			-Wno-write-strings \
			-Wno-invalid-offsetof \
			-Wredundant-decls \
			-Wpointer-arith \
			-Woverloaded-virtual \
			-Wno-conversion \
			-Wno-strict-aliasing \
			-pthread \
			-m64

OTHER_LIBS	=

BOOST_LIBS	=	\
			${LIB_BOOST_DIR}/libboost_filesystem.a   \
			${LIB_BOOST_DIR}/libboost_date_time.a \
			${LIB_BOOST_DIR}/libboost_thread.a \
			${LIB_BOOST_DIR}/libboost_regex.a \
			${LIB_BOOST_DIR}/libboost_system.a

TIBRV_LIB	=	\
			${LIB_TIBRV_DIR}/libtibrvcmq64.a	\
			${LIB_TIBRV_DIR}/libtibrvcm64.a		\
			${LIB_TIBRV_DIR}/libtibrvft64.a		\
			${LIB_TIBRV_DIR}/libtibrvcpp64.a	\
			${LIB_TIBRV_DIR}/libtibrv64.a

LBM_LIBS	=	\
			liblbm.a

MLB_LIB_NAMES	=	\
			Utility

MLB_LIB_FULL	=	\
			${addsuffix .a,${addprefix ${MASCaPS_TARGET_LIB}/lib,${MLB_LIB_NAMES}}}

LDLIBS		=	\
			-Bstatic	\
			${addprefix -l,${MLB_LIB_NAMES}}	\
			-Bdynamic	\
			${TIBRV_LIBS}	\
			${OTHER_LIBS}	\
			${BOOST_LIBS}	\
			-lm		\
			-lrt		\
			$(OTHER_LIBS)	\
			-lxerces-c	\
			${BOOST_LIBS}
	
LDFLAGS		+=	\
			-ldl			\
			-lpthread		\
			-lm			\
			-lrt			\
			-L /usr/lib64		\
			-L ${MASCaPS_TARGET_LIB}\
			-L ${LIB_PCAP_DIR}	\
			-L ${LIB_BOOST_DIR}	\
			-L ${LIB_XERCES_DIR}

TARGET_LIBS	=

TARGET_BINS	=

# #############################################################################

