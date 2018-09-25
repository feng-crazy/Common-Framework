#!/usr/bin/python3
# -*- coding: utf-8 -*-
# @Time    : 2018/6/13 17:25
# @Author  : hedengfeng
# @Site    : 
# @File    : test_message_read.py
# @Software: LT_controller

from utility import message_pb2

ctrl_broadcast = message_pb2.ControllerBroadcast()

# Read the existing address book.
with open('testData/msg.log', "rb") as f:
    msg_str = f.read()
    print(msg_str)
    ctrl_broadcast.ParseFromString(msg_str)

print(ctrl_broadcast)

broadcast_rep = message_pb2.BroadcastResponse()

# Read the existing address book.
with open('testData/msg1.log', "rb") as f:
    msg_str = f.read()
    print(msg_str)
    broadcast_rep.ParseFromString(msg_str)

print(broadcast_rep)


TerminalHeartbeatResponse_ = message_pb2.TerminalHeartbeatResponse()

# Read the existing address book.
with open('testData/msg2.log', "rb") as f:
    msg_str = f.read()
    print(msg_str)
    TerminalHeartbeatResponse_.ParseFromString(msg_str)

# print(TerminalHeartbeatResponse_.sequence)
# print('hdf', TerminalHeartbeatResponse_.status.detail)
# print(TerminalHeartbeatResponse_.action_request_from_gui.sequence)
# print(TerminalHeartbeatResponse_.event.event)

rep_status = TerminalHeartbeatResponse_.status

rep_status_str = rep_status.SerializeToString()
print('hdf rep_status_str: ', rep_status_str)

rep_status_tmp = message_pb2.ResponseStatus()
rep_status_tmp.ParseFromString(rep_status_str)
print('rep_status_tmp: ', rep_status_tmp)

print('hdf TerminalHeartbeatResponse_: ', TerminalHeartbeatResponse_)


TerminalPropertyRequest_ = message_pb2.TerminalPropertyRequest()
# Read the existing address book.
with open('testData/msg3.log', "rb") as f:
    msg_str = f.read()
    print(msg_str)
    TerminalPropertyRequest_.ParseFromString(msg_str)

print(type(TerminalPropertyRequest_))
print(type(TerminalPropertyRequest_.terminal_list))

for terminal in TerminalPropertyRequest_.terminal_list:
    print(type(terminal))
    # print(terminal)
    print(terminal.host_name)
    print(terminal.ip_addr)
    if terminal.ip_addr is None:
        print('terminal.ip_addr is None')
    if terminal.ip_addr == '':
        print('terminal.ip_addr == '' ')
    if not terminal.ip_addr:
        print('not terminal.ip_addr')
    if not len(terminal.ip_addr):
        print('len terminal.ip_addr', len(terminal.ip_addr))
    print(terminal.report_ip_addr)
    if not terminal.report_ip_addr:
        print('terminal.report_ip_addr is None')
