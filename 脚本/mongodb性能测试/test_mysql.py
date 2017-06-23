#coding:utf-8

import os
import sys
import time
import run_test
import MySQLdb

g_total = run_test.g_total_data/ run_test.g_run_times

# 打开数据库连接
db = MySQLdb.connect("192.168.0.49" ,"root","123456","hbb_test")

# 使用cursor()方法获取操作游标 
cursor = db.cursor()
cursor.execute("SET NAMES utf8")



def getCount():
	cursor.execute("select count(*) from role;")
	print cursor.fetchone()
	
	
def ClearDB():
	getCount()
	cursor.execute("TRUNCATE TABLE `role`;")
	getCount()
	print "clean ok"

if __name__ == "__main__":
	n_begin_t = time.time()
	for i in range(g_total):
		cursor.execute("INSERT INTO `role`( `name`,`lv`,`exp`) VALUES ( 'hbb','11','22');")
		db.commit()
	print "g_total:",g_total
	str_dirr = str(time.time() - n_begin_t)
	list_diff = str_dirr.split(".")
	run_test.Add2File(str_dirr)
	print "n_diff_t:", list_diff[0] , "s" , list_diff[1][0:3] , "ms"

	getCount()
	#os.system("pause")
	
	# 关闭数据库连接
	db.close()

