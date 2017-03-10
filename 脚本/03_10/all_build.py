
import os
import platform

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


def Build():
	SetPath()
	#初始化
	list_server = []
	list_server.append("centerserver")
	list_server.append("dbserver")
	list_server.append("fightserver")
	list_server.append("gameserver")
	list_server.append("gateserver")
	list_server.append("loginserver")
	list_server.append("nameserver")

	for one_ in list_server:
		str_cmd = "cd [path] && cmake . && make -j4"
		str_cmd = str_cmd.replace("[path]" , one_)
		os.system(str_cmd)

	ResetPath()
	

if __name__ == "__main__":
	Build()
