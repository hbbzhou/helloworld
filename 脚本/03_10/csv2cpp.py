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

class GDate:
	g_struct_value_num = {}
	g_struct_value = {}
	
	g_config_path = "../bin/config/"
	g_code_path   = "config_code_py/"
	
	g_config_key = {}
	g_config_key_list = []
	g_key_type   = "int"
	
	g_list_dsc  = {}
	g_list_name = {}
	g_list_type = {}
	
	g_config_name  = ""
	g_struct_name  = ""
	g_class_name   = ""
	g_contain_name = ""
	
	list_file_head = {}

g_data = GDate()

g_data.list_file_head["structs_py.h"] = '''
#pragma once
#include <string>
#include <set>
#include "type.h"
using namespace std;

'''

g_data.list_file_head["detail_config_py.h"] = '''
#pragma once
#include "type.h"
#include "structs_py.h"
#include <map>
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

#zhou 整理
for k , v in g_data.list_file_head.items():
	util.InitFile(g_data.g_code_path , k , v)


def read_config_header(fullpath):

	reader = csv.reader(open(fullpath))

	index = 0
	for line in reader:
		if index==3:
			break
		index = index + 1

		if index==1:
			g_data.g_list_dsc = line
		elif index==2:
			g_data.g_list_name = line
		elif index==3:
			g_data.g_list_type = line

	str_result = ""
	if len(g_data.g_list_dsc) != len(g_data.g_list_type) or len(g_data.g_list_dsc) != len(g_data.g_list_name) or len(g_data.g_list_type) != len(g_data.g_list_name):
		return False

	has_key = False
	for i in range(0, len(g_data.g_list_dsc)):

		if "#" in g_data.g_list_name[i]:
			has_key = True
			str_src = g_data.g_list_name[i]		
			str_src = str_src.replace('#', '')
			g_data.g_config_key[str_src] = g_data.g_list_type[i]
			g_data.g_config_key_list.append(str_src)

	if has_key == False:
		print "read" + fullpath + "Error, has no key!!"
		return False

	if len(g_data.g_config_key) == 2:
		is_two_int = True
		for key, value in g_data.g_config_key.items():
			if value != 'int':
				is_two_int = False

		if is_two_int:
			g_data.g_key_type = 'uint64'	

	return True

def init_global_name(config_name):

	g_data.g_config_name = config_name
	g_data.g_struct_name = "S" + config_name
	g_data.g_class_name  = "C" + config_name + "Template"
	g_data.g_contain_name = "m_map" + config_name + "Template"

	return True

#array[STestInfo[2]:2]
def read_array_type_define(array_define):
	struct_args = util.txt_wrap_by("[", "]", array_define)

	array_struct_info = struct_args.split(':')[0]
	array_struct_num = struct_args.split(':')[1]	

	struct_member_count = util.txt_wrap_by("[", "]", array_struct_info)
	array_struct_name = array_struct_info[0 : array_struct_info.find('[')]

	return array_struct_name, array_struct_num

#key = value, key = value
def get_key_param_string():
	str_result = ""
	config_key_count = len(g_data.g_config_key)
	for key, value in g_data.g_config_key.items():
		str_result += value+"\t" + key
		config_key_count = config_key_count - 1
		if config_key_count != 0:
			str_result += ","

	return str_result

def get_key_convert_string(key_name):

	str_result = ""
	if g_data.g_key_type == 'uint64':
		str_result += "uint64 " + key_name
		str_result += " = GNDP::GetUint64("

		config_key_count = len(g_data.g_config_key)
		for key, value in g_data.g_config_key.items():
			str_result += key
			config_key_count = config_key_count - 1
			if config_key_count != 0:
				str_result += ","
		str_result +=");\n\n"

	elif g_data.g_key_type == 'int':
		if len(g_data.g_config_key) != 1:
			print "g_data.g_key_type error, not 'int'" 

		str_result += "int " + key_name
		str_result += " = " 
		for key, value in g_data.g_config_key.items():
			str_result += key

		str_result +=";\n\n"

	return str_result

def output_struct_h():
	str_result = "struct	" + g_data.g_struct_name + "\n";
	str_result += "{\n";

	array_struct_name = '####'
	array_struct_num  = 0

	i = 0
	length = len(g_data.g_list_dsc)
	while i < length:
		if "array" in g_data.g_list_type[i]:
			array_struct_name, array_struct_num = read_array_type_define(g_data.g_list_type[i])
			str_result +='\t' + '//' + g_data.g_list_dsc[i].decode('gbk').encode('utf-8') + '\n';
			str_result +='\t' + array_struct_name + '\t';
			str_result +='\t' + 'arr' + array_struct_name + '['+ array_struct_num + '];\n';

			i = i + 1
			continue

		if array_struct_name in g_data.g_list_type[i]:
			struct_member_count = util.txt_wrap_by("[", "]", g_data.g_list_type[i] )
			g_data.g_struct_value_num[array_struct_name] = int(struct_member_count) #{'STestInfo' :2}
			dict_ = {}
			for j_ in range(i + 1 , i + 1 + int(struct_member_count)) :
				#{'STestInfo' :{"id" : "int", "count" : "int"}}
				dict_[g_data.g_list_name[j_] ] = g_data.g_list_type[j_]
			g_data.g_struct_value[array_struct_name] = dict_
			
			i = i + int(struct_member_count) + 1
			continue

		real_name = g_data.g_list_name[i] 
		if len(real_name.strip()) == 0:
			break

		if "#" in real_name:	
			real_name = real_name.replace('#', '').strip()

		if "date" in g_data.g_list_type[i]:
			pass

		elif "time" in g_data.g_list_type[i]:
			pass

		else:
			str_result +='\t' + '//' + g_data.g_list_dsc[i].decode('gbk').encode('utf-8') + '\n';
			str_result +='\t' + g_data.g_list_type[i] + '\t';
			str_result +='\t' + real_name + ';\n';

		i += 1

	#for i in range(0, len(g_data.g_list_dsc)):

	str_result += "};\n";

	str_base_struct = ""
	#{'STestInfo' :{"id" : "int", "count" : "int"}}
	global g_base_struct
	for s_name , s_info in g_data.g_struct_value.items():
		str_ = "struct " + s_name + " {"
		if str_ in g_base_struct:
			continue

		g_base_struct.append(str_)
		
		str_base_struct += str_ + "\n"
		for s_member_name , s_member_type in s_info.items():
			str_base_struct += "\t" + s_member_type+ " "+s_member_name +";"+ "\n"
		str_base_struct += "};" + "\n"
	
	return str_result , str_base_struct

def output_config_template_h():
	str_result = "class " + g_data.g_class_name + " \n\
{\n\
public:\n\
	" + g_data.g_class_name +"() {};\n\
	~" + g_data.g_class_name + "(){};\n\
	\n\
	bool Load(const char* szFileName);\n\
	void Clear() { " + g_data.g_contain_name +".clear(); }\n\
	\n\
public:\n\
	const " + g_data.g_struct_name +" *Find" +g_data.g_config_name+"Template(" + get_key_param_string() + ") const;\n\
\n\
	map<"+g_data.g_key_type+", "+g_data.g_struct_name+"> "+g_data.g_contain_name+";\n\
};\n"

	return str_result	

def output_config_template_cpp_func_load():

	obj_name = "st" + g_data.g_config_name

	str_result = "\
bool " +g_data.g_class_name+ "::Load(const char* szFileName)\n\
{\n\
	CCSVParser oParser;\n\
	if (!oParser.Load(szFileName))\n\
	{\n\
		return false;\n\
	}\n\
	for (int i = 3; i < oParser.GetLineCount(); ++i) {\n\
"	
	str_result += "\t\t" + g_data.g_struct_name + " " + obj_name + ";\n"
	str_result += "\t\tint nColumn = 1;\n"

	struct_class_name = '#####'
	read_start = False
	out_put_key = False
	key_name    = "llKey"

	#print "key"
	for key in g_data.g_config_key_list:	
		value = g_data.g_config_key[key]
		if "int" in value:
			str_result += "\t\t" + value + " " + key + " = atoi(oParser.GetField(i, nColumn++).c_str());\n"

	for key in g_data.g_config_key_list:	
		value = g_data.g_config_key[key]
		if "int" in value:
			str_result += "\t\t" + obj_name+ "."+ key + " = " + key +";\n"	

	str_result += "\t\t" + get_key_convert_string(key_name)

	i = 0
	length = len(g_data.g_list_name)
	while i < length:
	#for i in range(0, len(g_data.g_list_name)):
		if "#" in g_data.g_list_name[i]:
			read_start = True
			i += 1
			continue
			
		if read_start == False:
			i += 1
			continue

		#read start
		#read other column
		if "array" in g_data.g_list_type[i]:
			struct_class_name,  struct_class_num = read_array_type_define(g_data.g_list_type[i])
			struct_class_array_name = obj_name + "." + 'arr' + struct_class_name

			str_result += "\t\tnColumn++;\n"
			str_result += "\t\tfor (size_t j = 0; j < MyArraySize(" + struct_class_array_name +"); j++)\n"
			str_result += "\t\t{\n"
			
			is_output_skip = False
			for key1, value1 in g_data.g_struct_value[struct_class_name].items():
				if is_output_skip == False:
					str_result += "\t\t\tnColumn++;\n"
					is_output_skip = True
				
				str_result +='\t\t\t' + struct_class_array_name + '[j].';
				if value == 'int':
					str_result +=  key1 + " = atoi(oParser.GetField(i, nColumn++).c_str());\n"
			str_result += "\t\t"
			str_result += "}\n\n"

			i += 1
			continue

		elif struct_class_name in g_data.g_list_type[i]:
				i = i + g_data.g_struct_value_num[struct_class_name] + 1
				continue

		elif "float" in g_data.g_list_type[i]:
			str_result += "\t\t" + obj_name+ "."+ g_data.g_list_name[i] + " = (float)atof(oParser.GetField(i, nColumn++).c_str());\n"	

		elif "int" in g_data.g_list_type[i]:
			str_result += "\t\t" + obj_name+ "."+ g_data.g_list_name[i] + " = atoi(oParser.GetField(i, nColumn++).c_str());\n"	

		elif "string" in g_data.g_list_type[i]:
			str_result += "\t\t" + obj_name+ "."+ g_data.g_list_name[i] + " = oParser.GetField(i, nColumn++).c_str();\n"	

		elif "date" in g_data.g_list_type[i]:
			pass
		elif "time" in g_data.g_list_type[i]:
			pass

		i += 1

	str_result += "\t\t" + g_data.g_contain_name + "[" + key_name + "] = " +  obj_name + ";\n"
	str_result += "\t}\n"
	str_result += "\treturn true;\n"	
	str_result += "}\n"

	return str_result

def output_config_template_cpp_func_find():

	contain_type = "map<"+g_data.g_key_type+", "+g_data.g_struct_name+">"	
	str_result = "const " + g_data.g_struct_name +" *" + g_data.g_class_name+ "::Find" +g_data.g_config_name+"Template(" + get_key_param_string() + ") const\n{\n"
	key_name  = "llKey"

	str_result += "\t" + get_key_convert_string(key_name)

	str_result += "\t" + contain_type + "::const_iterator it = " + g_data.g_contain_name + ".find(" + key_name + ");\n\
\tif (it != " +g_data.g_contain_name+ ".end())\n\
\t{\n\
\t\treturn &it->second;\n\
\t}\n\
\treturn NULL;\n\
}\n"

	return str_result

def output_config_mgr_merge1():
	str_result = "\tconst " + g_data.g_class_name + " &Get" + g_data.g_config_name + "Config() const {\n\t\treturn m_o" + g_data.g_config_name + "Config;\t}"
	return str_result

def output_config_mgr_merge2():
	str_result = "\t" + g_data.g_class_name + " m_o" + g_data.g_config_name + "Config;" 
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


def convert_config_class_name(filename):
	filename = filename[0:filename.find('.csv')]
	names = filename.split('_')

	str_result = ""
	for name in names:
		str_result += name.capitalize()

	return "C" + str_result + "Template"




g_base_struct = util.load_base_struct(g_data.g_code_path + 'structs_py.h')

for parent, dirnames, filenames in os.walk(g_data.g_config_path): 

	for filename in filenames:
		if filename.find(".csv") < 0:
			continue

		g_data.g_config_key = {}
		g_data.g_config_key_list = []
		g_data.g_key_type   = "int"

		g_data.g_list_dsc  = {}
		g_data.g_list_name = {}
		g_data.g_list_type = {}
		
		g_data.g_struct_value_num = {}
		g_data.g_struct_value = {}
		
		g_data.g_config_name  = ""
		g_data.g_struct_name  = ""
		g_data.g_class_name   = ""
		g_data.g_contain_name = ""

		fullpath = os.path.join(parent,filename)
		
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
		
		
		