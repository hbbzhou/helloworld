#!/usr/bin/env python 
#encoding: utf-8

import os
import sys
import time
import run_text
import pymongo


#设置工作目录
g_old_path = ""
def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
	os.chdir(g_old_path )


connection = pymongo.MongoClient('192.168.0.49',27017)

hbb_db = connection['hbb_db']
role_table = hbb_db["role"]

g_total = 10000/ run_text.g_run_times

if 0:
	raw_input("[begin] input one to clean\n")
	for one_ in role_table.find():
		role_table.remove(one_)
	print "clean ok"
	os._exit(0)
else:
	pass
	#raw_input("[begin] input one to start\n")

n_begin_t = time.time()
for i in range(g_total):
	o_data = {'name':'zhou', 'lv': '22', 'exp': '880'}
	role_table.insert(o_data)
print "g_total:",g_total
str_dirr = str(time.time() - n_begin_t)
list_diff = str_dirr.split(".")
run_text.Add2File(str_dirr)
print "n_diff_t:", list_diff[0] , "s" , list_diff[1][0:3] , "ms"

if 0:
	for one_ in role_table.find():
		print one_

print role_table.count()
#os.system("pause")

