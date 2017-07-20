
import os
import platform
import time

#全局变量


#获取操作系统类型
g_is_win_platform =  False
if platform.uname()[0] == "Windows":
	g_is_win_platform =  True
	
#设置工作目录
g_old_path = ""
def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
	os.chdir(g_old_path )
	

#初始化
g_list_server = []

g_list_server.append("gameserver")
g_list_server.append("gameserver2")



def GetPid(str_):
	list_ = str_.split(" ")
	for i in range(4 , 10):
		if list_[i] != "":
			return list_[i]
	return ""

def KillOneLinux(str_):
	str_ = "./" + str_
	str_file = "ps_.txt"
	os.system("ps aux |grep server > " + str_file)
	f = open(str_file)
	list_ = f.readlines()
	f.close()
	os.system("rm -rf " + str_file)
	str_ret = "ok"
	for one_ in list_:
		if one_.find(str_) >= 0:
			print str_ , "will close .."
			os.system("kill " + GetPid(one_) )
			print "kill " + GetPid(one_)
			str_ret = "wait"
			
	return str_ret
			
def KillOneWin(str_):
	str_cmd = "TASKKILL /F /IM [name].exe "
	str_cmd = str_cmd.replace("[name]" , str_)
	print str_cmd
	os.system(str_cmd)
	return "ok"
	
def KillOne(str_):
	if g_is_win_platform == True:
		return KillOneWin(str_)
	else :
		return KillOneLinux(str_)
			
def Kill():
	SetPath()
	
	for one_ in g_list_server:
		print one_ , "##begin##================"
		while KillOne(one_) == "wait":
			print one_ , " ##wait"
			time.sleep(2)
		print one_ , "##end##=================="
		print "==========="
		print "====="
	ResetPath()


if __name__ == "__main__":
	Kill()	
	
	