#coding:utf-8

import os
import sys
import time
import run_test
import MySQLdb

g_total = run_test.g_part_data

# 打开数据库连接
db = MySQLdb.connect("192.168.0.49" ,"root","123456","hbb_test")

# 使用cursor()方法获取操作游标 
cursor = db.cursor()
cursor.execute("SET NAMES utf8")


def GetAppID():
	if len(sys.argv) < 3:
		print "error argv"
		os._exit(0)
	return int(sys.argv[2])
	


def getCount():
	cursor.execute("select count(*) from role;")
	print cursor.fetchone()
	
	
def ClearDB():
	getCount()
	cursor.execute("TRUNCATE TABLE `role`;")
	getCount()
	print "clean ok"
	
def IsBegin():
	if len(sys.argv) < 2:
		print "error argv"
		os._exit(0)
	n_begin_time = long(sys.argv[1])
	run_test.waitTo(n_begin_time)
	
if __name__ == "__main__":
	IsBegin()
	n_begin_t = time.time()
	for i in range(g_total):
		cursor.execute("INSERT INTO `role`( `name`,`lv`,`exp`) VALUES ( 'hbb','11','22');")
		db.commit()
	print "g_total:",g_total
	str_dirr = str(time.time() - n_begin_t)
	list_diff = str_dirr.split(".")
	run_test.Add2File(str_dirr , GetAppID() )
	print "n_diff_t:", list_diff[0] , "s" , list_diff[1][0:3] , "ms"

	getCount()
	#os.system("pause")
	
	# 关闭数据库连接
	db.close()

