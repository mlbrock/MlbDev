#!/bin/sh

SRC_DIR=${PWD}

DST_DIR=${SRC_DIR}/CppOut

protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/GpbElementInfoTestThree.proto

