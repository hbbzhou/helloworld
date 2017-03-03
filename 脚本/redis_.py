#!/usr/bin/env python 
#encoding: utf-8
import os
import sys
import redis



#设置工作目录
g_old_path = ""
def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
	os.chdir(g_old_path )

r = redis.Redis(host='192.168.0.49', port=6379,db=0)
r.set('name', 'huangbinbin')
print (r.get('name'))



raw_input("input one\n")














