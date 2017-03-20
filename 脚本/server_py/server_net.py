
#coding=utf-8
import sys
import os
import base64
import struct
import socket
#import traceback
import time 
import threading

#配置联网参数
host = '127.0.0.1'
port = 2017
max_listen = 100


#封装消息
def EncodeSendMsg(msg_id , body):
	str_form = "!2h" + str(len(body) ) + "s"
	str_ = struct.pack(str_form, len(body) + 2 ,msg_id, body)
	return str_

class NetStatus:
	isClose = False
	oSock = None
	nReadLen = 2
	isBody = False

#全局变量
g_dict = {}
g_dict_1 = {}
g_lock = threading.Lock()

def MoveNewSock():
	if g_lock.acquire():
		g_dict.update(g_dict_1)
		g_dict_1.clear()
		g_lock.release()

def AddNewSock( id , obj):
	if g_lock.acquire():
		g_dict_1[id] = obj
		g_lock.release()


def ReadAll():
	
	#加载新的数据
	MoveNewSock()
	
	list_del = []
	for (k,v) in  g_dict.items():
		while 1:
			try:
				data=v.oSock.recv(v.nReadLen)
				#if len(data) <= 0:
				#	v.isClose = True
				#	break

				if v.isBody == False:
					v.nReadLen = struct.unpack("!h", data)[0]
					print "receive head:",len(data) , "data:" , v.nReadLen
					v.isBody = True
				else :	
					v.isBody = False
					v.nReadLen = 2

					print "receive body :",len(data)
					#zhou 开始处理消息
					str_form = "!h" + str(len(data) - 2) + "s"
					n_msg_id , str_body = struct.unpack(str_form, data)
					print "msg_id:" , n_msg_id , "str_body:" , str_body.decode("utf").encode("gbk")
					v.oSock.sendall( EncodeSendMsg(12 , "ok" ) )
			except socket.error, e: 
				if str(e) != "[Errno 10035] ":
					list_del.append(k)
					v.isClose = True
					print "Error receiving data: ", e
				break

	#删除无效的数据
	for one_ in list_del:
		g_dict.pop(one_)
	
	global g_thread
	g_thread = threading.Timer(0.1, ReadAll)
	g_thread.start()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  
s.bind((host, port))  
s.listen(max_listen)
print "服务器 已开启...".decode("utf").encode("gbk")


#zhou 启动一个定时器
g_thread = None
ReadAll()

while 1:
	#when connect error happen, skip the error  
	ClientSock, ClientAddr = s.accept()
	
	#Get informaion form client and reply
	print "Get connect from:", ClientSock.getpeername()
	ClientSock.setblocking(False)
	
	
	#加入容器
	o_client = NetStatus()
	o_client.oSock = ClientSock
	AddNewSock(ClientSock.fileno() ,o_client)




