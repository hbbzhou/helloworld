
import os
import platform
import time


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
	

#定时	
def test2():
	time.sleep(2)
	os.system("pause")

#参数
def test1():
	if len(sys.argv) == 1:
		os.system("pause")
		return
	str_argv_1 = sys.argv[1]


if __name__ == "__main__":
	pass