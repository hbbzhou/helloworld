#!/usr/bin/env python 
#encoding: utf-8
import sys
import os
import codecs
import re
import csv
import os.path

#设置工作目录
py_file_path = os.path.split(os.path.realpath(__file__))[0]
os.chdir(py_file_path)

class csv_table():
	def __init__(self ):
		self.table_name = ""
		self.list_column_s = []		#list<one_column>


class d_check():
	def __init__(self ):
		self.key = ""		#cfg #select #range
		self.list_ = []		#0:1 #Hero.csv:ID
		

#全局变量
g_dict = {} 	#map<表名 , one_table>
g_key_i = 1		#字段名字
g_check_i = 2	#校验规则
g_type_i = 3	#类型
g_config_path = "config/"
#
g_cur_key = ""

def ExistBOM(file_name):
	file_obj = open(file_name,'r')
	code = file_obj.read(3)
	file_obj.close()
	if code == codecs.BOM_UTF8:#判断是否包含EF BB BF
		return True
	return False
	
def read_config_header(fullpath , filename):
	
	is_utf_file = ExistBOM(fullpath)
	reader = csv.reader(open(fullpath))

	obj_table = csv_table()
	#
	obj_table.table_name = filename
	for one_ in reader:
		while len(obj_table.list_column_s ) < len(one_):
			obj_table.list_column_s.append([])
		#
		for n_column_i in range(len(one_) ):
			str_column = one_[n_column_i]
			obj_table.list_column_s[n_column_i].append(str_column)
		#
	#
	global g_dict
	g_dict[obj_table.table_name] = obj_table
	return True

	

def check_range_int(list_ , member_ , key_ ,line_num):
	if len(list_) != 2:
		print key_ , "error ", line_num + 1 , "line"
		assert(0)

	n_min = int( list_[0] )
	n_max = 2147483647
	if list_[1] != 'n':
		n_max = int( list_[1] )
	
	if member_.isdigit() == False:
		print key_ , "error ", line_num + 1 , "line"
		assert(0)
	
	n_cur_num = int(member_ )
	if n_cur_num > n_max:
		print key_ , "error ", line_num + 1 , "line"
		assert(0)
	elif n_cur_num < n_min:
		print key_ , "error ", line_num + 1 , "line"
		assert(0)
	
#
def check_select_int(list_ , member_ , key_ ,line_num):
	if member_.isdigit() == False:
		print key_ , "error ", line_num + 1 , "line"
		assert(0)

	if member_ not in list_:
		print key_ , "error ", line_num + 1 , "line"
		assert(0)
#
def check_cfg_int(list_ , member_ , key_ ,line_num):
	str_file = list_[0]
	str_key = list_[1]
	
	obj_t = g_dict.get(str_file , None)
	if obj_t == None:
		print key_ , "error not find file" 
		assert(0)
	
	list_column_i = None
	for i_ in range( len(obj_t.list_column_s) ):
		if obj_t.list_column_s[i_][g_key_i] == str_key:
			list_column_i = i_
			break
	
	if list_column_i == None:
		print key_ , "error ", line_num + 1 , "line"
		assert(0)

	if member_ not in obj_t.list_column_s[list_column_i]:
		print key_ , "error ", line_num + 1 , "line"
		assert(0)
		

def check_int(list_data):
	str_check = list_data[g_check_i]
	str_check = str_check.replace("(" , "/").replace(")" , "/")
	list_check = str_check.split("/")
	
	if len(list_check) != 3:
		print list_data[g_key_i] , "error " , list_check
		assert(0)
	
	obj = d_check()
	obj.key = list_check[0]
	obj.list_ = list_check[1].split(":")
	
	for i_ in range( len(list_data) ):
		if i_ <= g_type_i:
			continue

		cur_check = obj
		cur_member = list_data[i_]
		if cur_check.key == "range":
			check_range_int(cur_check.list_ , cur_member , list_data[g_key_i] ,i_)
		elif cur_check.key == "select":
			check_select_int(cur_check.list_ , cur_member , list_data[g_key_i] ,i_)
		elif cur_check.key == "cfg":
			check_cfg_int(cur_check.list_ , cur_member , list_data[g_key_i] ,i_)
		else :
			print list_data[g_key_i] , "error ", i_ + 1 , "line"
			assert(0)
	#
	
def check_vector(list_data):
	str_check = list_data[g_check_i]
	list_check = str_check.split("|")
	
	all_check = []
	 
	for one_check in list_check :
		if one_check == "none":
			obj = d_check()
			obj.key = one_check
			obj.list_ = []
			all_check.append(obj)
			continue
			
		one_check = one_check.replace("(" , "/").replace(")" , "/")
		list_ = one_check.split("/")
		
		if len(list_) < 2:
			print list_data[g_key_i] , "error "
			assert(0)
		
		obj = d_check()
		obj.key = list_[0]
		obj.list_ = list_[1].split(":")
		all_check.append(obj)
	#
	
	for i_ in range( len(list_data) ):
		if i_ <= g_type_i:
			continue
		
		if list_data[i_] == "":
			continue
		
		list_obj = list_data[i_].split(";")
		for one_obj in list_obj :
			if one_obj == "":
				continue
				
			list_member_s = one_obj.split(":")
			if len(list_member_s) != len(all_check):
				print list_member_s
				print all_check
				print list_data[g_key_i] , "error "
				assert(0)
		
			for m_i in range(len(list_member_s) ):
				cur_check = all_check[m_i]
				cur_member = list_member_s[m_i]
				if cur_check.key == "range":
					check_range_int(cur_check.list_ , cur_member , list_data[g_key_i] ,i_)
				elif cur_check.key == "select":
					check_select_int(cur_check.list_ , cur_member , list_data[g_key_i] ,i_)
				elif cur_check.key == "none":
					pass
				elif cur_check.key == "cfg":
					check_cfg_int(cur_check.list_ , cur_member , list_data[g_key_i] ,i_)
				else :
					print list_data[g_key_i] , "error ", i_ + 1 , "line"
					assert(0)
	#
#

if __name__ == "__main__":
	import traceback
	try:
		for parent, dirnames, filenames in os.walk( g_config_path): 
			#
			for filename in filenames:
				if filename.find(".csv") < 0:
					continue
				
				fullpath = os.path.join(parent,filename)
				print "load " + fullpath
				
				if filename.find("ErrorCode.csv") >= 0:
					continue
				
				if read_config_header(fullpath , filename ) == False:
					print "read " + fullpath + " Error";
					break
		
					
		#校验格式
		for k_,v_ in g_dict.items():
			print "check " ,k_
			
			for one_list in v_.list_column_s:
				if len(one_list) < g_type_i + 1:
					print v_.table_name , "error"
					assert(0)
					
				if one_list[g_check_i] == "none":
					continue
					
				str_type = one_list[g_type_i]
				if str_type == "int":
					check_int(one_list)
				elif str_type == "float":
					check_int(one_list)
				elif str_type == "string":
					pass
				elif str_type[0:6] == "vector":
					check_vector(one_list)
				elif str_type[0:5] == "class":
					check_vector(one_list)
				else:
					print one_list[g_key_i] , "error " , g_type_i + 1, "line"
					assert(0)
			pass
	except Exception as e:
		print "=="
		print "====="
		print "========"
		print "=traceback="
		traceback.print_exc(file=sys.stdout)
		print "=============="
		os.system("pause")
		os._exit(0)

	print "====ok===="
	print "====ok===="
	print "====ok===="
	os.system("pause")
