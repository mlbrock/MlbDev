#!/bin/sh
# #############################################################################
# #############################################################################
# Test message for the MlbDev/ProtoBuf library.
# #############################################################################
# 
# Revision History  :  2015-06-20 --- Created in GpbUtils / MiscProjects repo
#                         Michael L. Brock
#                      2016-06-03 --- Copied over to ProtoBuf / MlbDev repo.
#                         Michael L. Brock
# 
# #############################################################################

SRC_DIR=${PWD}

DST_DIR=${SRC_DIR}/CppOut

protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/GpbElementInfoTestThree.proto

