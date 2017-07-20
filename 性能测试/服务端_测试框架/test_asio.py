
#coding=utf-8
import sys
import os
import base64
import struct
import socket
import time 
import threading
import run_test

g_total = run_test.g_part_data
g_receive_total = 0
#发送消息
def EncodeSendMsg(msg_id , str_buff):
	#print "===>>encode msg is " , msg_id
	#print "data:" ,str(str_buff)
	#print "==============="
	str_format = "!2h" + str(len(str_buff) ) + "s"
	
	str_ = struct.pack(str_format, len(str_buff) + 2 ,msg_id, str_buff)
	return str_
	
g_session = None
g_send_msg = EncodeSendMsg(10, "aaaa")
def ConnectLogin():
	print "do ConnectLogin ..."
	if len(sys.argv) < 3:
		print "error argv"
		os._exit(0)
	n_app_id = int(sys.argv[2])
	#n_port = 2017 + (n_app_id % 2)
	n_port = 2017
	
	global g_session
	global g_receive_total

	g_session=socket.socket(socket.AF_INET,socket.SOCK_STREAM)		#定义socket类型，网络通信，TCP
	g_session.connect(("192.168.0.49", n_port))		#要连接的IP与端口
	
	while 1:
		data=g_session.recv(2)   #把接收的数据定义为变量
		#print "receive heard :",len(data)
		if len(data) <= 0:
			break
			
		n_len = struct.unpack("!h", data)
		data_body=g_session.recv(n_len[0])
		if len(data_body) <= 0:
			print "receive body error"
			break

		str_form = "!h" + str(len(data_body) - 2) + "s"
		n_msg_id , str_body = struct.unpack(str_form, data_body)
		if n_msg_id == 12:
			if g_receive_total < g_total: 
				g_session.sendall( g_send_msg )
				
			g_receive_total += 1
		#print g_receive_total ,"msg_id:" , n_msg_id , "str_body:" , str_body
		
	g_session.close()   #关闭连接
	print "login net end"


def getCount():
	pass
	
	
def ClearDB():
	pass

def IsBegin():
	if len(sys.argv) < 2:
		print "error argv"
		os._exit(0)
	n_begin_time = long(sys.argv[1])
	run_test.waitTo(n_begin_time)

if __name__ == "__main__":

	t_login = threading.Thread(target=ConnectLogin,args=())
	t_login.start()

	IsBegin()
	n_begin_t = time.time()
	g_session.sendall( g_send_msg )
	while g_receive_total < g_total:
		time.sleep(0.01)
		
	print "g_total:",g_total
	str_dirr = str(time.time() - n_begin_t)
	list_diff = str_dirr.split(".")
	run_test.Add2File(str_dirr)
	print "n_diff_t:", list_diff[0] , "s" , list_diff[1][0:3] , "ms"

	getCount()
	
	# 关闭连接
	g_session.sendall( EncodeSendMsg( 8 , "exit" ) )
	
	#os.system("pause")








