
import sys
import os
import base64
import struct
import time 
import GData
import random
import ctypes





#宏定义
Login2Client = lambda msg_id , body: GData.g_net.login_session.sendall( EncodeSendMsg(msg_id , body))


def Test():
	str_ = "协议版本 1.0"
	print str_.decode("utf").encode("gbk")
	return str_
	
#封装消息
def EncodeSendMsg(msg_id , body):
	str_form = "!2h" + str(len(body) ) + "s"
	str_ = struct.pack(str_form, len(body) + 2 ,msg_id, body)
	return str_
	

	
	

	
	