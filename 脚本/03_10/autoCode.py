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

#放入 需要新增的协议
game_str_proto = '''
'''	


#下划线转驼峰
def Change1(str_):
	is_find = True
	new_str_ = ""
	for ch in str_:
		if is_find == True:
			ch = ch.upper()
		
		if ch == '_':
			is_find = True
		else:
			is_find = False
			new_str_ += ch	
	return new_str_


class GameServer:
	str_h = "gameserver/net/pipe_handler.h"
	str_cpp = "gameserver/net/pipe_handler.cpp"
	
	str_code_1 = "\tbool _P[value](const char* pData, uint32 dwLen, CClientSession* poSession, uint64 llId);"
	str_code_2 = "\t\tRegisterDealer(SProtoSpace::[key]_id, &CClientHandler::_P[value]) &&"
	str_code_3 = '''bool CClientHandler::_P[value](const char * pData, uint32 dwLen, CClientSession * poSession, uint64 llId)
{
	SProtoSpace::[key] o[value];
	PARSE_PROTO(SProtoSpace::[key], o[value], pData, dwLen);
	
	return false;
}'''	

	def decodeStr(self , str_proto):
		str_ = str_proto.replace(" " , "").replace("\t" , "").replace("=" , ",")
		dict_ = {}
		for one_ in str_.split("\n"):
			n_ = one_.find("_id")
			if n_ >= 0:
				
				key = one_[0:n_]
				dict_[key] = Change1(key)
		print dict_
		
		for k,v in dict_.iteritems():
			Insert_line(self.str_h, "//Merge1", self.str_code_1.replace("[key]" , k).replace("[value]" , v) ).Run()
			Insert_line(self.str_cpp, "//Merge3", self.str_code_3.replace("[key]" , k).replace("[value]" , v) ).Run()
			Insert_line(self.str_cpp, "//Merge2", self.str_code_2.replace("[key]" , k).replace("[value]" , v) ).Run()




GameServer().decodeStr(game_str_proto)

