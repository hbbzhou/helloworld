
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
	
time.sleep(2)
os.system("pause")

if __name__ == "__main__":
	pass