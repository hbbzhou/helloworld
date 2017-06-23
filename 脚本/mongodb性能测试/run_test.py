#!/usr/bin/env python 
#encoding: utf-8

import os
import sys
import time


#设置工作目录
g_old_path = ""
g_run_times = 10
g_str_file = "temp1.txt"
g_run_app = "mongodb_.py"
def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
	os.chdir(g_old_path )
	
def ClearFile():
	f = open(g_str_file , "w")
	f.close()
	
def Add2File(str_):
	f = open(g_str_file , "a")
	f.write(str_ + "\n")
	f.close()
	
def DealFile():
	f = open(g_str_file , "r")
	f_total = 0.0
	for one_ in f.readlines():
		print one_
		f_total += float(one_)
	f.close()
	print "g_run_times:" , g_run_times
	print f_total/ g_run_times , "s"

if __name__ == "__main__":
	ClearFile()
	for i in range(g_run_times):
		os.system("start " + g_run_app)

	os.system("pause")
	DealFile()
	
	raw_input("[begin] input one to clean\n")
	
	import mongodb_
	mongodb_.ClearDB()
	
	
	
	