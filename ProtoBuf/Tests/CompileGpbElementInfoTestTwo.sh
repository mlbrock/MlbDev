#!/bin/sh

SRC_DIR=${PWD}

DST_DIR=${SRC_DIR}/CppOut

protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/GpbElementInfoTestTwo_AddressBook.proto
protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/GpbElementInfoTestTwo_Person.proto
protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/GpbElementInfoTestTwo_PhoneNumber.proto
protoc -I=${SRC_DIR} --cpp_out=${DST_DIR} ${SRC_DIR}/GpbElementInfoTestTwo_PhoneType.proto

