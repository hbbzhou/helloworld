#!/usr/bin/env python 
#encoding: utf-8

import os
import sys
import time
#import fcntl


#设置工作目录
g_old_path = ""
#g_total_data = 100000
g_run_times = 80
g_part_data = 2500
g_str_dir = "report_dir"
g_run_app = "test_asio.py"

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
	
def InitDir():
	os.system("del /q " + g_str_dir)
	if os.path.exists(g_str_dir) == False:
		os.mkdir(g_str_dir)

def Add2File(str_ , i):
	str_file = g_str_dir + "/" + str(i) + ".txt"
	f = open(str_file , "a")
	f.write(str_ + "\n")
	f.close()

def DealFile():
	list_data = []
	for parent, dirnames, filenames in os.walk("./" + g_str_dir):
		for filename in filenames:
			if filename.find(".txt") < 0:
				continue
				
			fullpath = os.path.join(parent,filename)
			f = open(fullpath , "r")
			list_data.append(f.readline() )
			f.close()
			
	if len(list_data) == g_run_times:
		f_total = 0.0
		n_max = 0
		for one_ in list_data:
			if len(one_) > 1:
				n_max += 1
				f_total += float(one_)
			
		if n_max > 0:
			print "g_run_times:" , g_run_times , "max_line:" , n_max
			print f_total/ n_max , "s"
			return f_total/ n_max
	
	return 0

if __name__ == "__main__":
	InitDir()
	n_begin_t = time.time() + 6
	
	str_cmd = "start " + g_run_app + " " + str( long(n_begin_t) )
	for i in range(g_run_times):
		os.system(str_cmd + " " + str(i) )

	while 1:
		time.sleep(1)
		f_value = DealFile()
		if f_value == 0:
			continue
		else:
			break

	print "n_diff_t:", time.time() - n_begin_t , "s"

	import test_asio
	test_asio.ClearDB()
	
	
	
	