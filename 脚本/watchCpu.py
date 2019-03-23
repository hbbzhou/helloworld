
import os
import sys
import ctypes
import random
import base64
import time


#设置工作目录
g_old_path = ""
def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
	os.chdir(g_old_path )
	
	
#输出年月日#2019-03-22 21:58:30
def ShowTime():
	import datetime
	now = datetime.datetime.now()
	return now.strftime('%y-%m-%d~%H:%M')
	
def Add2File( file_ , str_):
	f = open(file_, 'a')
	f.write(str_)
	f.close()
	
def delFile(path):
	if os.path.exists(path): # 如果文件存在
		os.remove(path)
	
def dealOne(str_):
	str_ = str_.replace("\n" , "")
	list_ = str_.split(" ")
	listNew_ = []
	for one_ in list_:
		if one_ in [" " , "", "\t" , "\n"]:
			continue
		listNew_.append(one_)
	return listNew_
	
	
def dealCpuFile(strFile , listKey):
	f_ = open(strFile)
	list_ = f_.readlines()
	f_.close()
	
	for one_ in list_:
		list2 = dealOne(one_)
		if len(list2) < 4:
			continue
			
		str1 = str(GetDiffTime()) + "," + list2[2] + "," + list2[3] + "\n"
		print str1
		for oneKey in listKey:
			if oneKey in list2:
				strReportFile = oneKey.replace("./" , "")+ ".csv"
				Add2File( strReportFile , str1)
		#
	#
	pass
	
gStartTime = int(time.time() )
def GetDiffTime():
	global gStartTime
	nCurT = int(time.time() )
	n1 = nCurT - gStartTime
	return n1

if __name__ == "__main__":
	##hbb
	listKey = ["./gameserver" , "./gameserver2"]
	#
	delFile("temp_hbb_cup.txt")
	str1 = ShowTime() + ",CPU%, MEM%\n"
	for oneKey in listKey:
		strReportFile = oneKey.replace("./" , "") + ".csv"
		delFile(strReportFile)
		Add2File( strReportFile , str1)
	#
	while 1:
		os.system("ps aux |grep gameserver |grep -v grep  > temp_hbb_cup.txt")
		time.sleep(1)
		dealCpuFile("temp_hbb_cup.txt" , listKey)
		time.sleep(1)
	pass

