#!/bin/bash
# #############################################################################
# #############################################################################
# Build test messages for the MlbDev/ProtoBuf library
# #############################################################################
# 
# File Name       : CompileGpbElementInfoTestOne.sh
#
# File Description: Build test messages for the MlbDev/ProtoBuf library.
#
# Revision History: 2015-06-20 --- Created in GpbUtils / MiscProjects repo
#                      Michael L. Brock
#                   2016-06-03 --- Copied over to ProtoBuf / MlbDev repo.
#                      Michael L. Brock
# 
#       Copyright Michael L. Brock 2015 - 2026.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

SRC_DIR=${PWD}

DST_DIR=${SRC_DIR}/CppOut

protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/GpbElementInfoTestOne.proto

# #############################################################################

