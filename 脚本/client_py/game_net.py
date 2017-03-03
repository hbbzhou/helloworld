
#coding=utf-8
import sys
import os
import base64
import struct
import socket
import time 
import threading
import protocol_deal
import GData

def ConnectLogin():
	print "do ConnectLogin ..."

	GData.g_net.login_session=socket.socket(socket.AF_INET,socket.SOCK_STREAM)		#定义socket类型，网络通信，TCP
	GData.g_net.login_session.connect((GData.g_net.str_host, GData.g_net.port))		#要连接的IP与端口
	
	while 1:
		data=GData.g_net.login_session.recv(2)   #把接收的数据定义为变量
		print "receive heard :",len(data)
		if len(data) <= 0:
			break
			
		n_len = struct.unpack("!h", data)
		data=GData.g_net.login_session.recv(n_len[0])
		if len(data) <= 0:
			print "receive body error"
			break

		str_form = "!h" + str(len(data) - 2) + "s"
		n_msg_id , str_body = struct.unpack(str_form, data)
		print "msg_id:" , n_msg_id , "str_body:" , str_body
		
	GData.g_net.login_session.close()   #关闭连接
	print "login net end"

t_login = threading.Thread(target=ConnectLogin,args=())
t_login.start()

while 1:
	str_body = raw_input("输入:".decode("utf").encode("gbk") )
	protocol_deal.Login2Client(10 , str_body)








