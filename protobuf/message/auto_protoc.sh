#!/bin/sh

set -x 

protoc -I=./ --cpp_out=./ LT.message.proto

protoc -I=./ --python_out=./ LT.message.proto

cp LT/message_pb2.py E:/linux_share/code/python/LTController/utility

cp LT.message.proto E:/linux_share/code/python/LTController/utility

cp LT/message_pb2.py E:/linux_share/code/lingtrack/LTHub/LTHub/terminal/utility

cp LT.message.proto E:/linux_share/code/lingtrack/LTHub/LTHub/terminal/utility
