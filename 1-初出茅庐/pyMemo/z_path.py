#设置系统运行路径;获取ip;获取用户名
import os
import platform
import time


#获取操作系统类型
g_is_win_platform =  False
if platform.uname()[0] == "Windows":
	g_is_win_platform =  True

#获取_机器名
import getpass
g_str_name = getpass.getuser()

#设置工作目录
g_old_path = ""
def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )

def ResetPath():
	os.chdir(g_old_path )

#获取本地IP
def InitLocalIP():
	str_my_ip = ""
	is_win_platform = False
	if platform.uname()[0] == "Windows":
		is_win_platform =  True
		
		import socket
		myname = socket.getfqdn(socket.gethostname())
		str_my_ip = socket.gethostbyname(myname)
		
	else:
		str_file = "cmd_test.txt"
		os.system("/sbin/ifconfig eth0 > " + str_file)
		f_ = open(str_file , "r")
		list_ = f_.readlines()
		f_.close()
		os.system("rm -rf " + str_file)
		
		str_my_ip = subString(list_[1] , "inet addr:" , "Bcast:").replace(" " , "")
	#
	return str_my_ip , is_win_platform
#