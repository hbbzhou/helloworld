
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

def DealDiffFile(str_ver_1 , str_ver_2):
	str_file = "D:\\temp_diff.txt"
	str_cmd = "cd /d E:\\hbb\\ "
	str_cmd += "&& svn diff -r [ver1]:[ver2] > [file]"
	str_cmd = str_cmd.replace("[ver1]" , str_ver_1).replace("[ver2]" , str_ver_2).replace("[file]" , str_file)
	print str_cmd
	os.system(str_cmd)


	list_igno = [".pb.cc" ,".pb.h"]
	f_0 = open(str_file , "r")
	list_ = f_0.readlines()
	f_0.close()
	
	n_all_line = 0
	is_file = False
	for one_ in list_:
		if one_.find("Index:") == 0:
			is_file = True
			for one_igno in list_igno:
				if one_.find(one_igno) > 0:
					is_file = False
					break

		if is_file == False:
			continue
		
		if one_[1:2] == "+":
			continue
		elif one_[0:1] == "+":
			n_all_line += 1
	
	return n_all_line


def DealLogFile():
	f_0 = open("temp_log.txt" , "r")
	list_ = f_0.readlines()
	f_0.close()
	
	str_day = "2018-01-15"
	str_name = "shirao.wang"
	
	list_version = []
	list_name = []
	list_day = []
	
	for one_ in list_:
		if one_[0:1] == "r":
			if one_.find("|") < 0:
				continue
			list_2 = one_.split("|")
			list_version.append( list_2[0][1:-1] )
			list_name.append( list_2[1].replace(" " , "") )
			list_day.append(list_2[2] )

	n_total_line = 0
	n_size = len(list_version)
	for i_0 in range(n_size ):
		next_i = i_0 + 1
		if next_i >= n_size:
			break
			
		#选出_指定日期
		#if list_day[i_0].find(str_day) < 0:
		#	continue
		
		if list_name[i_0].find(str_name) >= 0:#选出_指定用户
			n_total_line += DealDiffFile(list_version[next_i]  ,list_version[i_0])
			
	print "total code line:" , n_total_line
	
if __name__ == "__main__":
	SetPath()
	ResetPath()
	
	#svn log -l 50 > temp_log.txt
	#svn log -r {2018-1-15T23:59:00}:{2017-12-22T0:00:01} > temp_log.txt
	DealLogFile()

#raw_input("input one\n")















