
#coding=utf-8
import sys
import os
import socket


#配置联网参数
class Net:
	str_host='127.0.0.1'
	port=2017

	login_session = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#	str_gate_host = '192.168.0.50'
#	gate_port = 0
#
#	#exit , to_login = 连接登录服务器 , to_gate = 连接网关 , wait = "等待消息" , "ok= 登录成功"
#	str_status = "wait" 
	
	
class Player:
	#to_login = 显示登录界面, to_main = 显示主界面
	str_ui_status = "to_login"
	
#	list_log_info = []
#	
#	#注册协议
#	login_dict = {}
#	gate_dict = {}
#
#	#玩家数据
#	str_account_name = ""
#	str_pwd = ""
#	
#	str_account_id = ""
#	server_id = 1
#	n_player_id = 1
#	str_player_name = ""
	
	
g_net = Net()
g_player = Player()










