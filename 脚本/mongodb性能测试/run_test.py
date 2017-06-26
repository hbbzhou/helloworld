#!/usr/bin/env python 
#encoding: utf-8

import os
import sys
import time


#设置工作目录
g_old_path = ""
#g_total_data = 100000
g_run_times = 80
g_part_data = 2500
g_str_file = "temp1.txt"
g_run_app = "test_mysql.py"

def waitTo(n_begin_t):
	index = 0
	while 1:
		index += 1
		if long(time.time()) == n_begin_t:
			break
		
		if index % 100 == 0:
			print n_begin_t - long(time.time()) , "ms"
			pass
			
		time.sleep(0.02)
	

def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
	os.chdir(g_old_path )
	
def ClearFile():
	f = open(g_str_file , "w")
	f.close()
	
def Add2File(str_ ):
	f = open(g_str_file , "a")
	f.write(str_ + "\n")
	f.close()
	
def DealFile():
	f = open(g_str_file , "r")
	f_total = 0.0
	n_max = 0
	list_data = f.readlines()
	f.close()
	
	for one_ in list_data:
		if len(one_) > 1:
			n_max += 1
			f_total += float(one_)
			
	if n_max > 0:
		print "g_run_times:" , g_run_times , "file_line:" , n_max
		print f_total/ n_max , "s"
		return f_total/ n_max
	
	return 0

if __name__ == "__main__":
	ClearFile()
	n_begin_t = time.time() + 6
	
	str_cmd = "start " + g_run_app + " " + str( long(n_begin_t) )
	for i in range(g_run_times):
		os.system(str_cmd)

	while 1:
		time.sleep(1)
		f_value = DealFile()
		if f_value == 0:
			continue

		if DealFile() + 2 < time.time() - n_begin_t:
		 break
	
	print "n_diff_t:", time.time() - n_begin_t , "s"
	
	import test_mysql
	test_mysql.ClearDB()
	
	
	
	