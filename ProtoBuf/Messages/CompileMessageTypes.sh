#!/bin/sh
# #############################################################################
# #############################################################################
# Build messages for inclusion in the MlbDev/ProtoBuf library.
# #############################################################################
# 
# Revision History  :  2016-06-26 --- Creation.
#                         Michael L. Brock
# 
# #############################################################################

SRC_DIR=${PWD}

DST_DIR=${SRC_DIR}/CppOut

protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/ArrayTypes.proto
protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/FieldTypeTest.proto

# #############################################################################

