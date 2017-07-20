#!/usr/bin/env python 
#encoding: utf-8

import os
import sys
import redis
import time
import run_test


#设置工作目录

g_total = run_test.g_part_data


def GetAppID():
	if len(sys.argv) < 3:
		print "error argv"
		os._exit(0)
	return int(sys.argv[2])
	
def ClearDB():
	r = redis.Redis(host='192.168.0.49', port=6379,db=15)
	print r.llen("role_list")
	print r.flushdb()
	print r.llen("role_list")
	print "clean ok"

def IsBegin():
	if len(sys.argv) < 2:
		print "error argv"
		os._exit(0)
	n_begin_time = long(sys.argv[1])
	run_test.waitTo(n_begin_time)

if __name__ == "__main__":
	r = redis.Redis(host='192.168.0.49', port=6379,db=15)
	
	IsBegin()
	str_data = "{'name':'zhou', 'lv': '22', 'exp': '880'}"
	n_begin_t = time.time()
	for i in range(g_total):
		r.lpush("role_list" , str_data)
	print "g_total:",g_total
	str_dirr = str(time.time() - n_begin_t)
	run_test.Add2File(str_dirr , GetAppID() )

	#os.system("pause")

