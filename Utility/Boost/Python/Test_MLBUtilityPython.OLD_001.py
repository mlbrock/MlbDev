# #############################################################################
# #############################################################################
# MLB Utility Boost Python Interface Module Test Script
# #############################################################################
#
#	File Name			:	%M%
#
#	File Version		:	%I%
#
#	Last Extracted		:	%D%	%T%
#
#	Last Updated		:	%E%	%U%
#
#	File Description	:	Test file for the MLB::Utility Python interface.
#
#	Revision History	:	2007-10-19 --- Creation.
#    							Michael L. Brock
#
# #############################################################################

# #############################################################################
# import the time module for testing of MLB::Utility time classes...
import time

# Import the MLBUtilityPython module...
import MLBUtilityPython
# #############################################################################

# #############################################################################
# Utility function for emitting separators...
# -----------------------------------------------------------------------------
def EmitSeparaterHyphen():
    print "-----------------------------------------------------------------------------"

def EmitSeparaterEquals():
    print "============================================================================="
# #############################################################################

# #############################################################################
# Test TimeT...
# -----------------------------------------------------------------------------
EmitSeparaterEquals()
EmitSeparaterEquals()
print "MLB::Utility::TimeT Tests:"
EmitSeparaterEquals()

src_TimeT_value_epoch   = MLBUtilityPython.TimeT(0)
src_TimeT_value_now     = MLBUtilityPython.TimeT()
src_TimeT_value_plus_s  = MLBUtilityPython.TimeT(src_TimeT_value_now)
src_TimeT_value_plus_ms = MLBUtilityPython.TimeT(src_TimeT_value_now)
src_TimeT_value_plus_us = MLBUtilityPython.TimeT(src_TimeT_value_now)
src_TimeT_value_plus_ns = MLBUtilityPython.TimeT(src_TimeT_value_now)
src_TimeT_value_plus_s.AddSeconds(11)
src_TimeT_value_plus_ms.AddMilliseconds(22)
src_TimeT_value_plus_us.AddMicroseconds(33)
src_TimeT_value_plus_ns.AddNanoseconds(44)

print "TimeT Epoch             :", src_TimeT_value_epoch
print "TimeT Now               :", src_TimeT_value_now

EmitSeparaterHyphen()

print "TimeT Now + 11 Seconds  :", src_TimeT_value_plus_s
print "TimeT Now + 22 MSecs    :", src_TimeT_value_plus_ms
print "TimeT Now + 33 USecs    :", src_TimeT_value_plus_us
print "TimeT Now + 44 NSecs    :", src_TimeT_value_plus_ns

EmitSeparaterHyphen()

print "FromString              :", MLBUtilityPython.TimeT("2001-02-03 04:05:06.123456789")

EmitSeparaterHyphen()

print "Epoch == Now            :", (src_TimeT_value_epoch == src_TimeT_value_now)
print "Epoch != Now            :", (src_TimeT_value_epoch != src_TimeT_value_now)
print "Epoch <  Now            :", (src_TimeT_value_epoch <  src_TimeT_value_now)
print "Epoch >  Now            :", (src_TimeT_value_epoch >  src_TimeT_value_now)
print "Epoch <= Now            :", (src_TimeT_value_epoch <= src_TimeT_value_now)
print "Epoch >= Now            :", (src_TimeT_value_epoch >= src_TimeT_value_now)

EmitSeparaterHyphen()

print "TimeT Now String        :", src_TimeT_value_now.ToString()
print "TimeT Now StringLocal   :", src_TimeT_value_now.ToStringLocal()
print "TimeT Now StringInterval:", src_TimeT_value_now.ToStringInterval()

EmitSeparaterHyphen()

print "TimeT SetToNow          :", src_TimeT_value_now.SetToNow()
print "TimeT SetToMinimumValue :", src_TimeT_value_now.SetToMinimumValue()
print "TimeT SetToMaximumValue :", src_TimeT_value_now.SetToMaximumValue()

EmitSeparaterEquals()
print
# #############################################################################









# import MLBUtilityPython
# src_TimeT_value_epoch    = MLBUtilityPython.TimeT(0)
# src_TimeT_value_now      = MLBUtilityPython.TimeT()
# print
# print
# 
# print src_TimeT_value_now
# 
# src_TimeT_value_plus_s = MLBUtilityPython.TimeT(src_TimeT_value_now)
# src_TimeT_value_plus_s.AddSeconds(10)
# print
# print
# 
# print src_TimeT_value_now
# print src_TimeT_value_plus_s
# print
# print










# #############################################################################
# Test TimeVal...
# -----------------------------------------------------------------------------
EmitSeparaterEquals()
EmitSeparaterEquals()
print "MLB::Utility::TimeVal Tests:"
EmitSeparaterEquals()

src_TimeVal_value_epoch   = MLBUtilityPython.TimeVal(0)
src_TimeVal_value_now     = MLBUtilityPython.TimeVal()
src_TimeVal_value_plus_s  = MLBUtilityPython.TimeVal(src_TimeVal_value_now)
src_TimeVal_value_plus_ms = MLBUtilityPython.TimeVal(src_TimeVal_value_now)
src_TimeVal_value_plus_us = MLBUtilityPython.TimeVal(src_TimeVal_value_now)
src_TimeVal_value_plus_ns = MLBUtilityPython.TimeVal(src_TimeVal_value_now)
src_TimeVal_value_plus_s.AddSeconds(11)
src_TimeVal_value_plus_ms.AddMilliseconds(22)
src_TimeVal_value_plus_us.AddMicroseconds(33)
src_TimeVal_value_plus_ns.AddNanoseconds(44)

print "TimeVal Epoch             :", src_TimeVal_value_epoch
print "TimeVal Now               :", src_TimeVal_value_now

EmitSeparaterHyphen()

print "TimeVal Now + 11 Seconds  :", src_TimeVal_value_plus_s
print "TimeVal Now + 22 MSecs    :", src_TimeVal_value_plus_ms
print "TimeVal Now + 33 USecs    :", src_TimeVal_value_plus_us
print "TimeVal Now + 44 NSecs    :", src_TimeVal_value_plus_ns

EmitSeparaterHyphen()

print "FromString                :", MLBUtilityPython.TimeVal("2001-02-03 04:05:06.123456789")

EmitSeparaterHyphen()

print "Epoch == Now              :", (src_TimeVal_value_epoch == src_TimeVal_value_now)
print "Epoch != Now              :", (src_TimeVal_value_epoch != src_TimeVal_value_now)
print "Epoch <  Now              :", (src_TimeVal_value_epoch <  src_TimeVal_value_now)
print "Epoch >  Now              :", (src_TimeVal_value_epoch >  src_TimeVal_value_now)
print "Epoch <= Now              :", (src_TimeVal_value_epoch <= src_TimeVal_value_now)
print "Epoch >= Now              :", (src_TimeVal_value_epoch >= src_TimeVal_value_now)

EmitSeparaterHyphen()

print "TimeVal Now String        :", src_TimeVal_value_now.ToString()
print "TimeVal Now StringLocal   :", src_TimeVal_value_now.ToStringLocal()
print "TimeVal Now StringInterval:", src_TimeVal_value_now.ToStringInterval()

EmitSeparaterHyphen()

print "TimeVal SetToNow          :", src_TimeVal_value_now.SetToNow()
print "TimeVal SetToMinimumValue :", src_TimeVal_value_now.SetToMinimumValue()
print "TimeVal SetToMaximumValue :", src_TimeVal_value_now.SetToMaximumValue()

EmitSeparaterEquals()

print "TimeVal Extract TimeT     :", src_TimeVal_value_now.ToTimeT()

EmitSeparaterEquals()

print
# #############################################################################


