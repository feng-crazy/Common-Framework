#!/usr/bin/python3
# -*- coding: utf-8 -*-
# @Time    : 2018/7/20 11:19
# @Author  : hedengfeng
# @Site    : 
# @File    : test_message.py
# @Software: LTController
# @description: 

from utility import message_pb2


# TerminalHeartbeatRequest_ = message_pb2.TerminalHeartbeatRequest()
# TerminalHeartbeatRequest_.sequence = 128
# TerminalHeartbeatRequest_.ok_status = False
# TerminalHeartbeatRequest_.except_status_detail.mac_addr = '8252213464974'
# print('TerminalHeartbeatRequest_: ', TerminalHeartbeatRequest_)
# TerminalHeartbeatRequest_str = TerminalHeartbeatRequest_.SerializeToString()
# print('TerminalHeartbeatRequest_str:', TerminalHeartbeatRequest_str)
# TerminalHeartbeatRequest_.ParseFromString(TerminalHeartbeatRequest_str)
# print('TerminalHeartbeatRequest_: ', TerminalHeartbeatRequest_)
#
# except_status_detail = TerminalHeartbeatRequest_.except_status_detail
# except_status_detail_str = except_status_detail.SerializeToString()
# print('except_status_detail_str:', except_status_detail_str)
# exit(0)

base_property = message_pb2.BaseStationProperty()
base_property.chip_id = '00000000'
base_property.bs_id = 1
base_property.host_name = 'sb'
base_property.ingroup_addr = 21
base_property.frequency = 115200
base_property.start_angle = 3.264

base_property.coordinates.x = 4
base_property.coordinates.y = 5
base_property.coordinates.z = 6


coordinates1 = message_pb2.PointXYZ()
coordinates1.x = 1
coordinates1.y = 2
coordinates1.z = 3
print(coordinates1, type(coordinates1))
print(coordinates1.z)
msg_content = coordinates1.SerializeToString()
print('msg_content: ', msg_content)

coordinates_parse = message_pb2.PointXYZ()
coordinates_parse.ParseFromString(msg_content)
print('coordinates_parse:',  coordinates_parse)
print(coordinates_parse.z)

exit(0)
print('--------coordinates2-------')
# coordinates2 = message_pb2.PointXYZ()
coordinates2 = coordinates1
print(coordinates2, type(coordinates2))


print('--------coordinates3-------')
coordinates3 = base_property.coordinates
print(coordinates3, type(coordinates3))

# coordinates_list = []
# coordinates_list = coordinates2
# print(type(coordinates_list))
# print(coordinates_list)

print('--------CopyFrom-------')
base_property.coordinates.CopyFrom(coordinates1)
print(base_property.coordinates, type(base_property.coordinates))

print('-------MergeFrom-------')
coordinates4 = message_pb2.PointXYZ()
coordinates4.x = 1
coordinates4.y = 2
coordinates4.z = 6
base_property.coordinates.MergeFrom(coordinates4)
print(base_property.coordinates, type(base_property.coordinates))
