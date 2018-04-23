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
#
#       Copyright Michael L. Brock 2007 - 2018.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
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
# Has nanosecond granularity which all time types don't use...
Time_FromStringSrc = "2001-02-03 04:05:06.123456789";
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

print "   Epoch                 :", time_epoch
print "   Now                   :", time_now

EmitSeparaterHyphenShort()

print "   Now + 11 Seconds      :", time_plus_s
print "   Now + 22 MSecs        :", time_plus_ms
print "   Now + 33 USecs        :", time_plus_us
print "   Now + 44 NSecs        :", time_plus_ns

EmitSeparaterHyphenShort()

print "   FromString Source     :", Time_FromStringSrc
print "   FromString Result     :", MLBUtilityPython.TimeT(Time_FromStringSrc)

EmitSeparaterHyphenShort()

print "   Epoch == Now          :", (time_epoch == time_now)
print "   Epoch != Now          :", (time_epoch != time_now)
print "   Epoch <  Now          :", (time_epoch <  time_now)
print "   Epoch >  Now          :", (time_epoch >  time_now)
print "   Epoch <= Now          :", (time_epoch <= time_now)
print "   Epoch >= Now          :", (time_epoch >= time_now)

EmitSeparaterHyphenShort()

print "   Now String            :", time_now.ToString()
print "   Now StringLocal       :", time_now.ToStringLocal()
print "   Now StringInterval    :", time_now.ToStringInterval()

EmitSeparaterHyphenShort()

time_now_copy = MLBUtilityPython.TimeT(time_now)

print "   SetToNow              :", time_now_copy.SetToNow()
print "   SetToMinimumValue     :", time_now_copy.SetToMinimumValue()
print "   SetToMaximumValue     :", time_now_copy.SetToMaximumValue()

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

print "   Epoch                 :", time_epoch
print "   Now                   :", time_now

EmitSeparaterHyphenShort()

print "   Now + 11 Seconds      :", time_plus_s
print "   Now + 22 MSecs        :", time_plus_ms
print "   Now + 33 USecs        :", time_plus_us
print "   Now + 44 NSecs        :", time_plus_ns

EmitSeparaterHyphenShort()

print "   FromString Source     :", Time_FromStringSrc
print "   FromString Result     :", MLBUtilityPython.TimeVal(Time_FromStringSrc)

EmitSeparaterHyphenShort()

print "   Epoch == Now          :", (time_epoch == time_now)
print "   Epoch != Now          :", (time_epoch != time_now)
print "   Epoch <  Now          :", (time_epoch <  time_now)
print "   Epoch >  Now          :", (time_epoch >  time_now)
print "   Epoch <= Now          :", (time_epoch <= time_now)
print "   Epoch >= Now          :", (time_epoch >= time_now)

EmitSeparaterHyphenShort()

print "   Now String            :", time_now.ToString()
print "   Now StringLocal       :", time_now.ToStringLocal()
print "   Now StringInterval    :", time_now.ToStringInterval()

EmitSeparaterHyphenShort()

time_now_copy = MLBUtilityPython.TimeVal(time_now)

print "   SetToNow              :", time_now_copy.SetToNow()
print "   SetToMinimumValue     :", time_now_copy.SetToMinimumValue()
print "   SetToMaximumValue     :", time_now_copy.SetToMaximumValue()

EmitSeparaterHyphenShort()

print "   Extract Source        :", time_now
print "   Extract Result TimeT  :", time_now.ToTimeT()

EmitSeparaterEquals()

print
# #############################################################################

# #############################################################################
# Test TimeSpec...
# -----------------------------------------------------------------------------
EmitSeparaterEquals()
EmitSeparaterEquals()
print "MLB::Utility::TimeSpec Tests:"
EmitSeparaterEquals()

time_epoch   = MLBUtilityPython.TimeSpec(0)
time_now     = MLBUtilityPython.TimeSpec()
time_plus_s  = MLBUtilityPython.TimeSpec(time_now)
time_plus_ms = MLBUtilityPython.TimeSpec(time_now)
time_plus_us = MLBUtilityPython.TimeSpec(time_now)
time_plus_ns = MLBUtilityPython.TimeSpec(time_now)
time_plus_s.AddSeconds(11)
time_plus_ms.AddMilliseconds(22)
time_plus_us.AddMicroseconds(33)
time_plus_ns.AddNanoseconds(44)

print "   Epoch                 :", time_epoch
print "   Now                   :", time_now

EmitSeparaterHyphenShort()

print "   Now + 11 Seconds      :", time_plus_s
print "   Now + 22 MSecs        :", time_plus_ms
print "   Now + 33 USecs        :", time_plus_us
print "   Now + 44 NSecs        :", time_plus_ns

EmitSeparaterHyphenShort()

print "   FromString Source     :", Time_FromStringSrc
print "   FromString Result     :", MLBUtilityPython.TimeSpec(Time_FromStringSrc)

EmitSeparaterHyphenShort()

print "   Epoch == Now          :", (time_epoch == time_now)
print "   Epoch != Now          :", (time_epoch != time_now)
print "   Epoch <  Now          :", (time_epoch <  time_now)
print "   Epoch >  Now          :", (time_epoch >  time_now)
print "   Epoch <= Now          :", (time_epoch <= time_now)
print "   Epoch >= Now          :", (time_epoch >= time_now)

EmitSeparaterHyphenShort()

print "   Now String            :", time_now.ToString()
print "   Now StringLocal       :", time_now.ToStringLocal()
print "   Now StringInterval    :", time_now.ToStringInterval()

EmitSeparaterHyphenShort()

time_now_copy = MLBUtilityPython.TimeSpec(time_now)

print "   SetToNow              :", time_now_copy.SetToNow()
print "   SetToMinimumValue     :", time_now_copy.SetToMinimumValue()
print "   SetToMaximumValue     :", time_now_copy.SetToMaximumValue()

EmitSeparaterHyphenShort()

print "   Extract Source        :", time_now
print "   Extract Result TimeT  :", time_now.ToTimeT()
print "   Extract Result TimeVal:", time_now.ToTimeVal()

EmitSeparaterEquals()

print
# #############################################################################

# #############################################################################
# Test translation of the various MLB::Utility exception classes...

EmitSeparaterEquals()
EmitSeparaterEquals()
print "MLB::Utility Exception Translation Tests"
EmitSeparaterEquals()

try:
    print "   Testing MLB::Utility::ExceptionGeneral..."
    MLBUtilityPython.TEST_MLB_Utility_Python_ThrowExceptionGeneral()
    print "   ***** ERROR: Expected exception not thrown!!!"
except Exception, exc:
    print "   Caught expected exception:"
    print "      Value: ", exc.__str__()
EmitSeparaterHyphenShort()

try:
    print "   Testing MLB::Utility::ExceptionErrno..."
    MLBUtilityPython.TEST_MLB_Utility_Python_ThrowExceptionErrno()
    print "   ***** ERROR: Expected exception not thrown!!!"
except Exception, exc:
    print "   Caught expected exception:"
    print "      Value: ", exc.__str__()
EmitSeparaterHyphenShort()

try:
    print "   Testing MLB::Utility::ExceptionSystemError..."
    MLBUtilityPython.TEST_MLB_Utility_Python_ThrowExceptionSystemError()
    print "   ***** ERROR: Expected exception not thrown!!!"
except Exception, exc:
    print "   Caught expected exception:"
    print "      Value: ", exc.__str__()
EmitSeparaterHyphenShort()

try:
    print "   Testing MLB::Utility::ExceptionCriticalEvent..."
    MLBUtilityPython.TEST_MLB_Utility_Python_ThrowExceptionCriticalEvent()
    print "   ***** ERROR: Expected exception not thrown!!!"
except Exception, exc:
    print "   Caught expected exception:"
    print "      Value: ", exc.__str__()
EmitSeparaterHyphenShort()

try:
    print "   Testing MLB::Utility::ExceptionMMapVMFailure..."
    MLBUtilityPython.TEST_MLB_Utility_Python_ThrowExceptionMMapVMFailure()
    print "   ***** ERROR: Expected exception not thrown!!!"
except Exception, exc:
    print "   Caught expected exception:"
    print "      Value: ", exc.__str__()

EmitSeparaterEquals()

print
# #############################################################################

# #############################################################################
Version_FromStringSrc = "1.23.456.7890"
# #############################################################################

# #############################################################################
# Test VersionNumber...
# -----------------------------------------------------------------------------
EmitSeparaterEquals()
EmitSeparaterEquals()
print "MLB::Utility::VersionNumber Tests:"
EmitSeparaterEquals()

version_zero    = MLBUtilityPython.VersionNumber()
version_test    = MLBUtilityPython.VersionNumber(1, 2, 3, 4)

print "   Zero                  :", version_zero
print "   Test                  :", version_test

EmitSeparaterHyphenShort()

print "   FromString Source     :", Version_FromStringSrc
print "   FromString Result     :", MLBUtilityPython.VersionNumber(Version_FromStringSrc)

EmitSeparaterHyphenShort()

print "   Zero == Test          :", (version_zero == version_test)
print "   Zero != Test          :", (version_zero != version_test)
print "   Zero <  Test          :", (version_zero <  version_test)
print "   Zero >  Test          :", (version_zero >  version_test)
print "   Zero <= Test          :", (version_zero <= version_test)
print "   Zero >= Test          :", (version_zero >= version_test)

EmitSeparaterHyphenShort()

print "   Test String           :", version_test.ToString()

EmitSeparaterHyphenShort()

version_test_copy = MLBUtilityPython.VersionNumber(version_test)

print "   SetToMinimumValue     :", version_test_copy.SetToMinimumValue()
print "   SetToMaximumValue     :", version_test_copy.SetToMaximumValue()

EmitSeparaterHyphenShort()


print "   GetMajor              :", version_test.GetMajor()
print "   GetMinor              :", version_test.GetMinor()
print "   GetRelease            :", version_test.GetRelease()
print "   GetBuild              :", version_test.GetBuild()

EmitSeparaterEquals()

print
# #############################################################################

# #############################################################################
# Test MACAddress...
# -----------------------------------------------------------------------------
EmitSeparaterEquals()
EmitSeparaterEquals()
print "MLB::Utility::MACAddress Tests:"
EmitSeparaterEquals()

mac_addr_zero   = MLBUtilityPython.MACAddress()
mac_addr_this   = MLBUtilityPython.MACAddress()
mac_addr_this.Get()

print "   Zero                  :", mac_addr_zero
print "   Now                   :", mac_addr_this

EmitSeparaterHyphenShort()

print "   Zero == This          :", (mac_addr_zero == mac_addr_this)
print "   Zero != This          :", (mac_addr_zero != mac_addr_this)
print "   Zero <  This          :", (mac_addr_zero <  mac_addr_this)
print "   Zero >  This          :", (mac_addr_zero >  mac_addr_this)
print "   Zero <= This          :", (mac_addr_zero <= mac_addr_this)
print "   Zero >= This          :", (mac_addr_zero >= mac_addr_this)

EmitSeparaterHyphenShort()

print "   Zero String           :", mac_addr_zero.ToString()
print "   This String           :", mac_addr_this.ToString()

EmitSeparaterHyphenShort()

mac_addr_this_copy = MLBUtilityPython.MACAddress(mac_addr_this)

print "   SetToMinimumValue     :", mac_addr_this_copy.SetToMinimumValue()
print "   SetToMaximumValue     :", mac_addr_this_copy.SetToMaximumValue()

EmitSeparaterEquals()

print
# #############################################################################


