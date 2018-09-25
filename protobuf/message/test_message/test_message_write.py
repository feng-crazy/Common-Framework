#!/usr/bin/python3
# -*- coding: utf-8 -*-
# @Time    : 2018/6/13 17:25
# @Author  : hedengfeng
# @Site    : 
# @File    : test_message_write.py
# @Software: LT_controller

from utility import message_pb2

ctrl_broadcast = message_pb2.ControllerBroadcast()
ctrl_broadcast.ip_addr = '192.168.0.150'
ctrl_broadcast.terminal_port = 5200
ctrl_broadcast.manager_port = 5201
ctrl_broadcast.loc_xsub_port = 5202

msg_string = ctrl_broadcast.SerializeToString()

print(msg_string)

# Write the new address book back to disk.
with open("testData/msg.log", "wb") as f:
    f.write(ctrl_broadcast.SerializeToString())

broadcase_rep = message_pb2.BroadcastResponse()   # type: message_pb2.BroadcastResponse
broadcase_rep.type = message_pb2.BroadcastResponse.MANAGER_TYPE  #TERMINAL_TYPE
broadcase_rep.ip_addr = '192.168.0.155'
broadcase_rep.mac_addr = 'd2:d2:d2:e2:e2:a1'
broadcase_rep.host_name = 'bendan'


print(broadcase_rep.type)
print(broadcase_rep)

# Write the new address book back to disk.
with open("testData/msg1.log", "wb") as f:
    f.write(broadcase_rep.SerializeToString())


TerminalHeartbeatRequest_ = message_pb2.TerminalHeartbeatRequest()
TerminalHeartbeatRequest_.sequence = 128
# TerminalHeartbeatRequest_.status = message_pb2.TerminalHeartbeatRequest.CONFIG_ERROR # self.terminal_status
# TerminalHeartbeatRequest_.detail = 'sb'
print('TerminalHeartbeatRequest_: ', TerminalHeartbeatRequest_)

TerminalHeartbeatResponse_ = message_pb2.TerminalHeartbeatResponse()
TerminalHeartbeatResponse_.sequence = 128
TerminalHeartbeatResponse_.status.code = message_pb2.ResponseStatus.FAIL
TerminalHeartbeatResponse_.status.detail = 'wo shi yi ge bei shang de ren'
TerminalHeartbeatResponse_.event.event = message_pb2.UpdateEvent.CONFIG_UPDATE
TerminalHeartbeatResponse_.action_request_from_gui.sequence = 25
TerminalHeartbeatResponse_.action_request_from_gui.type = message_pb2.TerminalActionRequest.UPDATE_BIN
TerminalHeartbeatResponse_.action_request_from_gui.update_url = 'www.baidu.com'

with open("testData/msg2.log", "wb") as f:
    print(' TerminalHeartbeatResponse_.SerializeToString', TerminalHeartbeatResponse_.SerializeToString())
    f.write(TerminalHeartbeatResponse_.SerializeToString())


print('TerminalHeartbeatResponse_', TerminalHeartbeatResponse_)

TerminalPropertyRequest_ = message_pb2.TerminalPropertyRequest()
TerminalPropertyRequest_.type = message_pb2.TerminalPropertyRequest.UPDATE
terminal1 = TerminalPropertyRequest_.terminal_list.add()
print('terminal1', type(terminal1))
print('terminal_list type', type(TerminalPropertyRequest_.terminal_list))
terminal1.host_name = "terminal1"
terminal1.ip_addr = '202.114.4.25'
terminal1.mac_addr = 'd5d5a6d63ad1'
terminal2 = TerminalPropertyRequest_.terminal_list.add()
terminal2.host_name = "terminal2"
terminal2.ip_addr = '202.114.4.26'
terminal2.mac_addr = 'd5d5a6d63ad1'

terminal3 = message_pb2.TerminalProperty()
terminal3.host_name = "terminal3"
terminal3.ip_addr = '202.114.4.26'
terminal3.mac_addr = 'd5d5a6d63ad1'

terminal4 = message_pb2.TerminalProperty()
terminal4.host_name = "terminal4"
# terminal4.ip_addr = '202.114.4.26'
terminal4.mac_addr = 'd5d5a6d63ad1'

TerminalPropertyRequest_.terminal_list.extend([terminal3, terminal4])

msg = TerminalPropertyRequest_.SerializeToString()
with open("testData/msg3.log", "wb") as f:
    f.write(msg)

print(TerminalPropertyRequest_)
print(type(msg))