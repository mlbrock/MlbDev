# ##### Copy the gcc settings.
# ##### Really just for illustration purposes...
include ${MASCaPS_DIR_NAME}/gcc_MakePrefix.mk

CC	=	clang
CXX	=	clang

# ##### Use g++ for the link stage because clang barfs on fully-static build.
# ##### LINK.cpp and LINK.C are defined as equal to LINK.cc by GNU Make.
LINK.c  = gcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)
LINK.cc = g++ $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)

