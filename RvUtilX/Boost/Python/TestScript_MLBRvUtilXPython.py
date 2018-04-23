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
#       Copyright Michael L. Brock 2007 - 2018.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

# #############################################################################
# ******** import the time module for testing of MLB::Utility time classes...
# ******** import time

# Import the MLBRvUtilXPython module...
import MLBRvUtilXPython
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
# Test Environment...
# -----------------------------------------------------------------------------
def RunEnvironmentTests():
    EmitSeparaterEquals()
    EmitSeparaterEquals()
    print "MLB::RvUtilX::Environment Tests:"
    EmitSeparaterEquals()

    print "   Performing Environment::Open()... "
    MLBRvUtilXPython.Environment.Open()

    EmitSeparaterHyphenShort()

    print "   Environment::DefaultQueue()       :", MLBRvUtilXPython.Environment.DefaultQueue

    print "   Environment::IntraProcessTransport:", MLBRvUtilXPython.Environment.IntraProcessTransport
    
    print "   Environment::Version              :", MLBRvUtilXPython.Environment.Version

    EmitSeparaterHyphenShort()

    print "   Performing Environment::Close()... "
    MLBRvUtilXPython.Environment.Close()

    EmitSeparaterEquals()

    print
# #############################################################################

# #############################################################################
def TestQueueInfo(this_queue):
    print "   Handle()     :", this_queue.Handle
    EmitSeparaterHyphenShort()
    
    old_policy = this_queue.LimitPolicy
    print "   LimitPolicy:", old_policy
    if (this_queue.IsDefaultQueue):
        print "   This is the default queue --- LimitPolicy() test skipped."
    else:
        new_policy = MLBRvUtilXPython.LimitPolicy(MLBRvUtilXPython.LimitPolicyStrategy.DiscardFirst, 100, 10)
        print "   LimitPolicy: (NEW       =", new_policy, ")"
        this_queue.LimitPolicy = new_policy
        print "   LimitPolicy:", this_queue.LimitPolicy
        print "   LimitPolicy: (RESTORING =", old_policy, ")"
        this_queue.LimitPolicy = old_policy
        print "   LimitPolicy:", this_queue.LimitPolicy
    
    EmitSeparaterHyphenShort()
    
    old_name = this_queue.Name
    print "   Name:", old_name
    new_name = "This is the new queue name"
    print "   Name: (NEW       =", new_name, ")"
    this_queue.Name = new_name
    print "   Name:", this_queue.Name
    print "   Name: (RESTORING =", old_name, ")"
    this_queue.Name = old_name
    print "   Name:", this_queue.Name
    
    EmitSeparaterHyphenShort()
    
    old_priority = this_queue.Priority
    print "   Priority:", old_priority
    new_priority = 666
    print "   Priority: (NEW       =", new_priority, ")"
    this_queue.Priority = new_priority
    print "   Priority:", this_queue.Priority
    print "   Priority: (RESTORING =", old_priority, ")"
    this_queue.Priority = old_priority
    print "   Priority:", this_queue.Priority
    
    EmitSeparaterHyphenShort()
# #############################################################################

# #############################################################################
# Test Queue...
# -----------------------------------------------------------------------------
def RunQueueTests():
    EmitSeparaterEquals()
    EmitSeparaterEquals()
    print "MLB::RvUtilX::Queue Tests:"
    EmitSeparaterEquals()

    MLBRvUtilXPython.Environment.Open()

    print "   Performing Queue::Default()... "
    default_queue = MLBRvUtilXPython.Queue.Default

    EmitSeparaterHyphenShort()
		
    TestQueueInfo(default_queue)

    EmitSeparaterHyphenShort()
    
    new_queue = MLBRvUtilXPython.Queue()
    new_policy = MLBRvUtilXPython.LimitPolicy(MLBRvUtilXPython.LimitPolicyStrategy.DiscardFirst, 77, 7)
    new_queue.LimitPolicy = new_policy
    new_queue.Name = "A brand-new queue"
    new_queue.Priority = 777
    TestQueueInfo(new_queue)

    new_queue.Destroy()

    MLBRvUtilXPython.Environment.Close()

    EmitSeparaterEquals()

    print
# #############################################################################

# #############################################################################
# Main tests dispatch...
# -----------------------------------------------------------------------------
RunEnvironmentTests()
RunQueueTests()
# #############################################################################


"""
import MLBRvUtilXPython
MLBRvUtilXPython.Environment.Open()
ipt=MLBRvUtilXPython.Environment.GetIntraProcessTransport()
print ipt
print ipt.GetHandle()
import MLBRvUtilXPython
nt=MLBRvUtilXPython.NetTransport("7500", ";239.255.1.45", "")
print nt
print nt.GetHandle()
print nt.CreateInbox()
print nt.Service, "/", nt.Network, "/", nt.Daemon

import MLBRvUtilXPython
MLBRvUtilXPython.Environment.Open()
dq_1=MLBRvUtilXPython.Queue.Default
print dq_1
dq_2=MLBRvUtilXPython.Environment.DefaultQueue
print dq_2
ipt=MLBRvUtilXPython.Environment.IntraProcessTransport
print ipt
ipt.BatchMode = MLBRvUtilXPython.TransportBatchMode.TimerBatch

import MLBRvUtilXPython
help(MLBRvUtilXPython.Environment)

import MLBRvUtilXPython
ipt = MLBRvUtilXPython.IntraProcessTransport.UniqueInstance
print ipt

import MLBRvUtilXPython
help(MLBRvUtilXPython.TransportBatchMode)

import MLBRvUtilXPython
ipt=MLBRvUtilXPython.Environment.IntraProcessTransport
print ipt
help(MLBRvUtilXPython.Environment.IntraProcessTransport)

import MLBRvUtilXPython
ipt=MLBRvUtilXPython.NetTransport
print ipt
help(MLBRvUtilXPython.NetTransport)

import MLBRvUtilXPython
MLBRvUtilXPython.Environment.Open()
nt=MLBRvUtilXPython.NetTransport("7500", ";239.255.1.45", "")
print nt
print nt.Handle
print nt.Description
nt.Description = "A net transport"
print nt.Description
#help(MLBRvUtilXPython.NetTransport)
print
ipt=MLBRvUtilXPython.Environment.IntraProcessTransport
print ipt
print ipt.Handle
print ipt.Description
ipt.Description = "The intra-process transport"
print ipt.Description

import MLBRvUtilXPython
MLBRvUtilXPython.Environment.Open()
msg=MLBRvUtilXPython.Message()
print msg.SendSubject
print msg.ReplySubject

msg.SendSubject="X.Y.Send"
msg.ReplySubject="X.Y.Reply"

print msg.SendSubject
print msg.ReplySubject
"""
















# #############################################################################
def TestTransportInfo(this_transport):
    print "   Handle()     :", this_transport.Handle
    EmitSeparaterHyphenShort()

    print "   CreateInbox():", this_transport.CreateInbox()
    EmitSeparaterHyphenShort()

    old_policy = this_queue.LimitPolicy
    print "   LimitPolicy:", old_policy
    if (this_queue.IsDefaultQueue):
        print "   This is the default queue --- LimitPolicy() test skipped."
    else:
        new_policy = MLBRvUtilXPython.LimitPolicy(MLBRvUtilXPython.LimitPolicyStrategy.DiscardFirst, 100, 10)
        print "   LimitPolicy: (NEW       =", new_policy, ")"
        this_queue.LimitPolicy = new_policy
        print "   LimitPolicy:", this_queue.LimitPolicy
        print "   LimitPolicy: (RESTORING =", old_policy, ")"
        this_queue.LimitPolicy = old_policy
        print "   LimitPolicy:", this_queue.LimitPolicy
    
    EmitSeparaterHyphenShort()
    
    old_name = this_queue.Name
    print "   Name:", old_name
    new_name = "This is the new queue name"
    print "   Name: (NEW       =", new_name, ")"
    this_queue.Name = new_name
    print "   Name:", this_queue.Name
    print "   Name: (RESTORING =", old_name, ")"
    this_queue.Name = old_name
    print "   Name:", this_queue.Name
    
    EmitSeparaterHyphenShort()
    
    old_priority = this_queue.Priority
    print "   Priority:", old_priority
    new_priority = 666
    print "   Priority: (NEW       =", new_priority, ")"
    this_queue.Priority = new_priority
    print "   Priority:", this_queue.Priority
    print "   Priority: (RESTORING =", old_priority, ")"
    this_queue.Priority = old_priority
    print "   Priority:", this_queue.Priority
    
    EmitSeparaterHyphenShort()
# #############################################################################


