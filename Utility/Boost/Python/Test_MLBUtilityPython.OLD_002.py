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

def EmitSeparaterHyphenShort():
    print "   --------------------------------------------------------------------------"
# #############################################################################

# #############################################################################
# Test TimeT...
# -----------------------------------------------------------------------------
EmitSeparaterEquals()
EmitSeparaterEquals()
print "MLB::Utility::TimeT Tests:"
EmitSeparaterEquals()

time_epoch   = MLBUtilityPython.TimeT(0)
time_now     = MLBUtilityPython.TimeT()
time_plus_s  = MLBUtilityPython.TimeT(time_now)
time_plus_ms = MLBUtilityPython.TimeT(time_now)
time_plus_us = MLBUtilityPython.TimeT(time_now)
time_plus_ns = MLBUtilityPython.TimeT(time_now)
time_plus_s.AddSeconds(11)
time_plus_ms.AddMilliseconds(22)
time_plus_us.AddMicroseconds(33)
time_plus_ns.AddNanoseconds(44)

print "TimeT Epoch             :", time_epoch
print "TimeT Now               :", time_now

EmitSeparaterHyphenShort()

print "TimeT Now + 11 Seconds  :", time_plus_s
print "TimeT Now + 22 MSecs    :", time_plus_ms
print "TimeT Now + 33 USecs    :", time_plus_us
print "TimeT Now + 44 NSecs    :", time_plus_ns

EmitSeparaterHyphenShort()

print "FromString              :", MLBUtilityPython.TimeT("2001-02-03 04:05:06.123456789")

EmitSeparaterHyphenShort()

print "Epoch == Now            :", (time_epoch == time_now)
print "Epoch != Now            :", (time_epoch != time_now)
print "Epoch <  Now            :", (time_epoch <  time_now)
print "Epoch >  Now            :", (time_epoch >  time_now)
print "Epoch <= Now            :", (time_epoch <= time_now)
print "Epoch >= Now            :", (time_epoch >= time_now)

EmitSeparaterHyphenShort()

print "TimeT Now String        :", time_now.ToString()
print "TimeT Now StringLocal   :", time_now.ToStringLocal()
print "TimeT Now StringInterval:", time_now.ToStringInterval()

EmitSeparaterHyphenShort()

time_now_copy = MLBUtilityPython.TimeT(time_now)

print "TimeT SetToNow          :", time_now_copy.SetToNow()
print "TimeT SetToMinimumValue :", time_now_copy.SetToMinimumValue()
print "TimeT SetToMaximumValue :", time_now_copy.SetToMaximumValue()

EmitSeparaterEquals()

print
# #############################################################################

# #############################################################################
# Test TimeVal...
# -----------------------------------------------------------------------------
EmitSeparaterEquals()
EmitSeparaterEquals()
print "MLB::Utility::TimeVal Tests:"
EmitSeparaterEquals()

time_epoch   = MLBUtilityPython.TimeVal(0)
time_now     = MLBUtilityPython.TimeVal()
time_plus_s  = MLBUtilityPython.TimeVal(time_now)
time_plus_ms = MLBUtilityPython.TimeVal(time_now)
time_plus_us = MLBUtilityPython.TimeVal(time_now)
time_plus_ns = MLBUtilityPython.TimeVal(time_now)
time_plus_s.AddSeconds(11)
time_plus_ms.AddMilliseconds(22)
time_plus_us.AddMicroseconds(33)
time_plus_ns.AddNanoseconds(44)

print "TimeVal Epoch             :", time_epoch
print "TimeVal Now               :", time_now

EmitSeparaterHyphenShort()

print "TimeVal Now + 11 Seconds  :", time_plus_s
print "TimeVal Now + 22 MSecs    :", time_plus_ms
print "TimeVal Now + 33 USecs    :", time_plus_us
print "TimeVal Now + 44 NSecs    :", time_plus_ns

EmitSeparaterHyphenShort()

print "FromString                :", MLBUtilityPython.TimeVal("2001-02-03 04:05:06.123456789")

EmitSeparaterHyphenShort()

print "Epoch == Now              :", (time_epoch == time_now)
print "Epoch != Now              :", (time_epoch != time_now)
print "Epoch <  Now              :", (time_epoch <  time_now)
print "Epoch >  Now              :", (time_epoch >  time_now)
print "Epoch <= Now              :", (time_epoch <= time_now)
print "Epoch >= Now              :", (time_epoch >= time_now)

EmitSeparaterHyphenShort()

print "TimeVal Now String        :", time_now.ToString()
print "TimeVal Now StringLocal   :", time_now.ToStringLocal()
print "TimeVal Now StringInterval:", time_now.ToStringInterval()

EmitSeparaterHyphenShort()

time_now_copy = MLBUtilityPython.TimeVal(time_now)

print "TimeVal SetToNow          :", time_now_copy.SetToNow()
print "TimeVal SetToMinimumValue :", time_now_copy.SetToMinimumValue()
print "TimeVal SetToMaximumValue :", time_now_copy.SetToMaximumValue()

EmitSeparaterHyphenShort()

print "Extract TimeT Source      :", time_now
print "Extract TimeT Result      :", time_now.ToTimeT()

EmitSeparaterEquals()

print
# #############################################################################


