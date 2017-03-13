#!/usr/bin/env python 
#encoding: utf-8

import os
import csv
import os.path
import re
import util
from util  import Insert_line 

#设置工作目录
py_file_path = os.path.split(os.path.realpath(__file__))[0]
os.chdir(py_file_path)

class field_type():
	def __init__(self, str_des ,str_name , str_ , is_key):
		self.name_ = str_name.replace(" " , "")	#变量名字
		self.type_ = ""							#变量类型
		self.des_ = str_des						#字段说明
		self.class_name = ""
		self.is_key = is_key
		self.list_type = []
		self.list_member = []
		self.DecodeStr(str_)
		
	def subStr(self , str_src , str_left , str_right):
		return str_src[str_src.find(str_left) + len(str_left) : str_src.find(str_right)]
		
	def DecodeClass(self ,str_):
		self.class_name = self.subStr(str_ ,"vector:", "[")
		self.type_ = "vector"
		str_2 = self.subStr(str_ ,"[", "]")
		list_all = str_2.split(";")
		for one_ in list_all:
			list_ = one_.split(":")
			if len(list_) == 2:
				self.list_type.append( list_[0] )
				self.list_member.append( list_[1] )
			

	def DecodeStr(self ,str_):
		str_ = str_.replace(" " , "")
		if "vector" in str_:
			self.DecodeClass(str_)
		else:
			self.type_ = str_

class GDate:
	g_config_path = "../bin/config/"
	g_code_path   = "config_code_py/"
	
	list_field_type = [] # list<field_type>
	
	g_config_name  = ""
	list_file_head = {}

g_data = GDate()

g_data.list_file_head["structs_py.h"] = '''
#pragma once
#include <string>
#include <set>
#include <map>
#include <vector>
#include "type.h"
using namespace std;

'''

g_data.list_file_head["detail_config_py.h"] = '''
#pragma once
#include "type.h"
#include "structs_py.h"
#include <map>
#include <vector>
using namespace std;

'''

g_data.list_file_head["config_py.h"] = '''
#pragma once
#include "structs_py.h"
#include "detail_config_py.h"

class CConfigPy
{
public:
	bool BaseLoad();

public:

//Merge1
//private:

//Merge2
};
'''

g_data.list_file_head["detail_config_py.cpp"] = '''
#include "detail_config_py.h"
#include "utility.h"
#include "utf.h"
#include "csv.h"
#include "stream_string.h"

#if defined(WIN32) || defined(WIN64)
#else
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#endif

inline void DecodeVecStr(const std::string &str_src , std::vector<std::vector<std::string> > & o_out)
{
	std::vector<std::string> vec_;
	GNDP::StrSplit(str_src.c_str(), "; ", vec_);
	for (size_t vec_i = 0; vec_i < vec_.size(); ++vec_i) {
		const std::string & str_ = vec_[vec_i];
		std::vector<std::string> vec_2;
		GNDP::StrSplit(str_.c_str(), ": ", vec_2);
		o_out.push_back(vec_2);
	}
}

'''

g_data.list_file_head["config_py.cpp"] = '''
#include "config_py.h"
#include "stdlib.h"
#include "utility.h"

bool CConfigPy::BaseLoad()
{
	char szFileExePath[MAX_PATH] = {0};
	const char *pszExePath = GNDP::GetExePath();
	
//Merge3
	return true;
}
'''

for k , v in g_data.list_file_head.items():
	util.InitFile(g_data.g_code_path , k , v)


def read_config_header(fullpath):

	reader = csv.reader(open(fullpath))

	index = 0
	list_dsc = []
	list_name = []
	list_type = []
	for line in reader:
		if index==3:
			break
		index = index + 1

		if index==1:
			list_dsc = line
		elif index==2:
			list_name = line
		elif index==3:
			list_type = line

	str_result = ""
	if len(list_dsc) != len(list_type) or len(list_dsc) != len(list_name) :
		return False

	key_num = 0
	for i in range(0, len(list_dsc)):
		has_key = False
		str_name = list_name[i]
		if "#" in str_name:
			has_key = True
			key_num += 1
			str_name = str_name.replace('#', '')
		print "读取表字段".decode("utf").encode("gbk"), str_name
		g_data.list_field_type.append(field_type(list_dsc[i] ,str_name , list_type[i] ,has_key ) )

	if key_num != 1:
		print "read" + fullpath + "Error, key count = ", key_num
		return False

	return True

def init_global_name(config_name):
	g_data.g_config_name = config_name
	return True

def output_struct_h():
	str_result = '''struct	S[config_name]
{
[content]
};
'''
	str_content = ""
	length = len(g_data.list_field_type)
	for i in range(0 , length):
		field_ = g_data.list_field_type[i]
		str_ = ""
		if field_.type_ == "vector":
			str_ = "	std::vector<[class_name]> m_vec[field_name];\n"
		elif field_.type_ == "int":
			str_ = "	int m_n[field_name];\n"
		elif field_.type_ == "float":
			str_ = "	float m_f[field_name];\n"
		elif field_.type_ == "string":
			str_ = "	std::string m_str[field_name];\n"
		
		str_content += str_.replace("[class_name]" , field_.class_name).replace("[field_name]" , field_.name_)

	str_result = str_result.replace("[config_name]" , g_data.g_config_name).replace("[content]" , str_content)

	str_base_struct = ""
	global g_base_struct
	for i in range(0 , length):
		field_ = g_data.list_field_type[i]
		if field_.type_ != "vector":
			continue

		if field_.class_name == "int":
			continue
		elif field_.class_name == "float":
			continue
		elif field_.class_name == "string":
			continue

		str_ = "struct " + field_.class_name + " {"
		if str_ in g_base_struct:
			continue
		g_base_struct.append(str_)
		str_base_struct += str_ + "\n"
		for j_ in range(len(field_.list_type) ):
			str_base_struct += "\t" + field_.list_type[j_]+ " "+field_.list_member[j_] +";"+ "\n"
		str_base_struct += "};" + "\n"

	return str_result , str_base_struct

def out_load_vec_code_2(field_):
	str_2 = ""
	if field_.class_name == "int":
		str_2 = "				[class_name] o_[class_name] = atoi(vec_all[vec_i].c_str());\n"
	elif field_.class_name == "float":
		str_2 = "				[class_name] o_[class_name] = (float)atof(vec_all[vec_i].c_str());\n"
	elif field_.class_name == "string":
		str_2 = "				[class_name] o_[class_name] = vec_all[vec_i];\n"
	else:
		print "vector type error"
		os._exit(1)

		str_2 += str_3.replace("[sub_field]" ,field_.list_member[index_] ).replace("[vec_index]" ,str(index_) )

	str_ = '''
		{
			std::vector<std::string> vec_all;
			GNDP::StrSplit(oParser.GetField(i, [nColumn]).c_str(), "; ", vec_all);
			for (size_t vec_i = 0; vec_i < vec_all.size(); ++vec_i) {
[content_2]
				st_S[config_name].m_vec[field].push_back(o_[class_name]);
			}
		}
'''
	return str_.replace("[content_2]" , str_2).replace("[class_name]" , field_.class_name)

def out_load_vec_code(field_):
	str_2 = ""
	n_vecSize = len(field_.list_type)
	for index_ in range(n_vecSize ):
		str_3 = ""
		if field_.list_type[index_] == "int":
			str_3 += "					o_[class_name].[sub_field] = atoi(vec_2[[vec_index]].c_str());\n"
		elif field_.list_type[index_] == "float":
			str_3 += "					o_[class_name].[sub_field] = (float)atof(vec_2[[vec_index]].c_str());\n"
		elif field_.list_type[index_] == "string":
			str_3 += "					o_[class_name].[sub_field] = vec_2[[vec_index]];\n"
		else:
			print "vector type error"
			os._exit(1)

		str_2 += str_3.replace("[sub_field]" ,field_.list_member[index_] ).replace("[vec_index]" ,str(index_) )

	str_ = '''
		{
			std::vector<std::vector<std::string> > vec_all;
			DecodeVecStr(oParser.GetField(i, [nColumn]), vec_all);
			for (size_t vec_i = 0; vec_i < vec_all.size(); ++vec_i) {
				const std::vector<std::string> & vec_2 = vec_all[vec_i];
				if (vec_2.size() == [max_len]) {
					[class_name] o_[class_name];
[content_2]
					st_S[config_name].m_vec[field].push_back(o_[class_name]);
				}
			}
		}
'''
	return str_.replace("[content_2]" , str_2).replace("[max_len]" , str(n_vecSize) ).replace("[class_name]" , field_.class_name)

def output_config_template_h():
	str_result = '''
class C[config_name]Template 
{
public:
	C[config_name]Template() {};
	~C[config_name]Template(){};
	
	bool Load(const char* szFileName);
	void Clear() { m_map[config_name]Template.clear(); }
	
public:
	const S[config_name] *Find[config_name]Template(int ID) const;

	std::map<int, S[config_name]> m_map[config_name]Template;
};
'''
	str_result = str_result.replace("[config_name]" , g_data.g_config_name)
	return str_result	

def output_config_template_cpp_func_load():

	str_result = '''
bool C[config_name]Template::Load(const char* szFileName)
{
	CCSVParser oParser;
	if (!oParser.Load(szFileName))
	{
		return false;
	}
	for (int i = 3; i < oParser.GetLineCount(); ++i) {
		S[config_name] st_S[config_name];
[content]
		m_map[config_name]Template[llKey] = st_S[config_name];
	}
	return true;
}
'''
	str_content = ""
	length = len(g_data.list_field_type)
	for i in range(0 , length):
		field_ = g_data.list_field_type[i]
		str_ = ""
		if field_.type_ == "float":
			str_ = '''		st_S[config_name].m_f[field] = (float)atof(oParser.GetField(i, [nColumn]).c_str());\n'''
		elif field_.type_ == "int":
			str_ = '''		st_S[config_name].m_n[field] = atoi(oParser.GetField(i, [nColumn]).c_str());\n'''
		elif field_.type_ == "string" :
			str_ = '''		st_S[config_name].m_str[field] = oParser.GetField(i, [nColumn]).c_str();\n'''
		elif field_.type_ == "vector" :
			if field_.class_name in ['int','float','string']:
				str_ = out_load_vec_code_2(field_ )
			else:
				str_ = out_load_vec_code(field_)
		
		if field_.is_key == True:
			str_ += '''		int llKey = st_S[config_name].m_n[field];\n'''
		str_content += str_.replace("[nColumn]" , str(i) ).replace("[field]" , field_.name_ )

	str_result = str_result.replace("[content]" , str_content)
	str_result = str_result.replace("[config_name]" , g_data.g_config_name)
	

	return str_result

def output_config_template_cpp_func_find():

	str_result = '''
const S[config_name] *C[config_name]Template::Find[config_name]Template(int ID) const
{
	int llKey = ID;
	std::map<int, S[config_name]>::const_iterator it = m_map[config_name]Template.find(llKey);
	if (it != m_map[config_name]Template.end())
	{
		return &it->second;
	}
	return NULL;
}
'''
	str_result = str_result.replace("[config_name]" , g_data.g_config_name)
	return str_result

def output_config_mgr_merge1():
	str_result = '''	const C[config_name]Template &Get[config_name]Config() const { return m_o[config_name]Config; }'''
	str_result = str_result.replace("[config_name]" , g_data.g_config_name)
	return str_result

def output_config_mgr_merge2():
	str_result = '''	C[config_name]Template m_o[config_name]Config;'''
	str_result = str_result.replace("[config_name]" , g_data.g_config_name)
	return str_result

#{"FeatsChest.csv",              &m_oWorldWarFEatsChestConfig },
def output_config_mgr_merge3(filename):
	str_ = '''
	snprintf(szFileExePath, sizeof(szFileExePath) - 1, "%s/%s", pszExePath, "../../config/[file]");
	if (!m_o[obj]Config.Load(szFileExePath)){
		USR_CRITICALEX(true, "load %s error", szFileExePath);
		return false;
	}
'''	
	return str_.replace("[file]" , filename).replace("[obj]" ,g_data.g_config_name )

def convert_config_name(filename):
	filename = filename[0:filename.find('.csv')]
	names = filename.split('_')

	str_result = ""
	for name in names:
		str_result += name.capitalize()

	return str_result

g_base_struct = util.load_base_struct(g_data.g_code_path + 'structs_py.h')

for parent, dirnames, filenames in os.walk(g_data.g_config_path): 

	for filename in filenames:
		if filename.find(".csv") < 0:
			continue
		
		g_data.g_config_name  = ""
		g_data.list_field_type = []

		fullpath = os.path.join(parent,filename)
		print "read " + fullpath
		if read_config_header(fullpath) == False:
			print "read " + fullpath + " Error";
			break

		if init_global_name(convert_config_name(filename)) == False:
			print "read " + fullpath + " Error, init global name Error";
			break

		#structs_py.h
		str_result , str_base_struct = output_struct_h()
		util.Add2File(g_data.g_code_path ,"structs_py.h" , str_base_struct)
		util.Add2File(g_data.g_code_path ,"structs_py.h" , str_result)

		#detail_config_py.h
		util.Add2File(g_data.g_code_path , "detail_config_py.h", output_config_template_h())

		#detail_config_py.cpp
		util.Add2File(g_data.g_code_path ,"detail_config_py.cpp" , output_config_template_cpp_func_load() )
		util.Add2File(g_data.g_code_path ,"detail_config_py.cpp" , output_config_template_cpp_func_find() )

		#config_py.h
		util.Insert_line(g_data.g_code_path + "config_py.h", "//Merge1", output_config_mgr_merge1()).Run()
		util.Insert_line(g_data.g_code_path + "config_py.h", "//Merge2", output_config_mgr_merge2()).Run()

		#config_py.cpp
		util.Insert_line(g_data.g_code_path + "config_py.cpp", "//Merge3", output_config_mgr_merge3(filename)).Run()
		
os.system("pause")		
		