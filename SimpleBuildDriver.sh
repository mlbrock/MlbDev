#!/bin/bash
# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Simple Build Driver Script
# #############################################################################
#
# File Name       : SimpleBuildDriver.sh
#
# File Description: Simple build driver script.
#
# Revision History: 2016-01-29 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2016 - 2017.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

MY_DIR_NAME=${PWD}
MY_BASE_DIR=`basename ${MY_DIR_NAME}`
MY_CURR_DATE=`date -u +%Y_%m_%d_%H_%M_%S_%N`
MY_HOST_NAME=`hostname -s`
MY_USER_NAME=`whoami`
MY_FILE_NAME="${MY_DIR_NAME}/BuildResults.${MY_BASE_DIR}.${MY_CURR_DATE}.${MY_HOST_NAME}.${MY_USER_NAME}.${BASHPID}.log"

# ##### yyyy_mm_dd_hh_mm_ss_nnnnnnnnn
# ##### 01234567890123456789012345678
MY_DATE_NICE="\
${MY_CURR_DATE:0:4}-\
${MY_CURR_DATE:5:2}-\
${MY_CURR_DATE:8:2} \
${MY_CURR_DATE:11:2}:\
${MY_CURR_DATE:14:2}:\
${MY_CURR_DATE:17:2}.\
${MY_CURR_DATE:20}\
"

MY_SEP_EQUALS="\
=============================================================================="
MY_SEP_HYPHENS="\
------------------------------------------------------------------------------"

source SimpleBuildDriverLocalList.sh

echo "Build output file is ${MY_FILE_NAME}"

echo "${MY_SEP_EQUALS}"                        &>  ${MY_FILE_NAME}
echo "${MY_SEP_EQUALS}"                        &>> ${MY_FILE_NAME}
echo "Current Directory     : ${MY_DIR_NAME}"  &>> ${MY_FILE_NAME}
echo "Current Date (UTC/GMT): ${MY_DATE_NICE}" &>> ${MY_FILE_NAME}
echo "Base Directory Name   : ${MY_BASE_DIR}"  &>> ${MY_FILE_NAME}
echo "Host Name             : ${MY_HOST_NAME}" &>> ${MY_FILE_NAME}
echo "User Name             : ${MY_USER_NAME}" &>> ${MY_FILE_NAME}
echo "Build Log File Name   : ${MY_FILE_NAME}" &>> ${MY_FILE_NAME}
echo "Command Line Arguments: $@"              &>> ${MY_FILE_NAME}
echo "${MY_SEP_EQUALS}"                        &>> ${MY_FILE_NAME}
echo "${MY_SEP_EQUALS}"                        &>> ${MY_FILE_NAME}
echo                                           &>> ${MY_FILE_NAME}

for this_dir in ${MY_DIR_LIST}
do
    echo "${MY_SEP_EQUALS}"  &>> ${MY_FILE_NAME}
    echo ${this_dir}         &>> ${MY_FILE_NAME}
    echo "${MY_SEP_HYPHENS}" &>> ${MY_FILE_NAME}
    ( pushd ${this_dir} &>> /dev/null ; make $@ &>> ${MY_FILE_NAME} )
    echo "${MY_SEP_EQUALS}"  &>> ${MY_FILE_NAME}
    echo                     &>> ${MY_FILE_NAME}
done

# #############################################################################

