

import sys
import os
import base64
import struct
import time 
import random
import ctypes
import threading

from PyQt4 import QtCore, QtGui, uic
from PyQt4.QtCore import *  
from PyQt4.QtGui import *  

#配置ui界面
g_str_ui_main = "main.ui"
test_class, base_test_class = uic.loadUiType(g_str_ui_main)


def ShowBuff(str_buff):
	str_out = ""
	for one_ in str_buff:
		str_out = str_out + "/" + str( ord(one_) )
	print str_out
	return str_out

def DecodeBuff(encode_buff):
	str_out = ""
	for one_ in encode_buff.split("/"):
		if len(one_) == 0:
			continue

		str_out = str_out + chr( int(one_) )
		
	return str_out

def Qstring2utf(str_):
	return unicode(str_).encode("utf")


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

#驼峰转下划线
def Change2(str_):
	is_big = True
	new_str_ = ""
	for ch in str_:
		if ch.isupper() :
			if is_big == False:
				new_str_ += '_'
			is_big = True
		else:
			is_big = False
		
		new_str_ += ch.lower()
	return new_str_


g_end_char_s = ["=" , "/" , "\r" , "\n"]
g_ignore_char_s = [" " ,  "	"]

def DealNameStr(str_):
	str_new = ""
	for one_ in str_:
		if one_ in g_end_char_s:
			break
		elif one_ in g_ignore_char_s:
			continue

		str_new += one_

	return str_new

def DealMemberStr(str_):
	str_key = ""
	list_key = []
	is_begin = False
	for one_ in str_:
		if is_begin == False:
			if one_ in g_ignore_char_s:
				continue
			else :
				is_begin = True
	
		if one_ in g_end_char_s:
			break
			
		if one_ in g_ignore_char_s:
			if str_key != "":
				list_key.append(str_key)
				str_key = ""
				
			continue
			
		str_key += one_
		
	return list_key

	
class ObjMember():
	def __init__(self , list_):
		self.type1 = list_[0]
		self.type2 = list_[1]
		self.name = list_[2]
		
	def __str__(self):
		str_ = "\t" + self.type1 + " " + self.type2 + " " + self.name + "\n"
		return str_ 
	
class ObjInfo():
	def __init__(self ,str_name ,member_s , str_desc):
		self.str_name = str_name
		self.member_s = member_s
		self.desc = str_desc
		
	def __str__(self):
		str_ =  self.str_name + "\n"
		str_ += self.desc
		return str_ + "}"
		


class UiMain(QtGui.QDialog, test_class):
	def __init__(self, *args):
		super(UiMain, self).__init__(*args)
		self.setupUi(self)
		self.dict_proto = {}
		
		self.InItProto("E:/intermediate/proto/client.proto")
		self.InItProto("E:/intermediate/proto/base.proto")
		self.InItProto("E:/intermediate/proto/gs_cross.proto")
		self.InItProto("E:/intermediate/proto/gs_db.proto")
		self.InItProto("E:/intermediate/proto/gs_ft.proto")
		self.InItProto("E:/intermediate/proto/gs_gt.proto")
		self.InItProto("E:/intermediate/proto/gt_center.proto")
		self.InItProto("E:/intermediate/proto/gt_db.proto")
		self.InItProto("E:/intermediate/proto/login.proto")
		self.InItProto("E:/intermediate/proto/ls_center.proto")
		self.InItProto("E:/intermediate/proto/mail.proto")
		self.InItProto("E:/intermediate/proto/manager.proto")
		
		#
		list_ = []
		for k_, v_ in self.dict_proto.items():
			list_.append(k_)
			
		list_.sort()
		for one_ in list_:
			self.box1.addItem( one_ )
		

	def InItProto(self , str_path):
		f_1 = open(str_path , "r")
		list_content = f_1.readlines()
		f_1.close()
		
		cur_obj_name = ""
		cur_desc = ""
		cur_member_s = []
		for one_ in list_content:
			if one_[0:7] == "message" :
				cur_obj_name = DealNameStr(one_[7:])
			elif one_[0:4] == "enum" :
				cur_obj_name = DealNameStr(one_[4:])
			elif "}" in one_:	#结束
				if cur_obj_name == "":
					continue
					
				self.dict_proto[cur_obj_name] = ObjInfo(cur_obj_name , cur_member_s , cur_desc)

				cur_obj_name = ""
				cur_desc = ""
				cur_member_s = []
			else:
				if cur_obj_name == "":
					continue
				#处理成员
				cur_desc += one_
				list_ = DealMemberStr(one_)
				if len(list_) == 3:
					cur_member_s.append(ObjMember(list_) )
		
	def update1(self):
		str_key = Qstring2utf(self.input1.text() )
		one = self.dict_proto.get( str_key , None)
		if one != None:
			self.input2.setText( str(one).decode("utf") )
		else:
			print "not find obj"
			return None
		
		##
		out_str2 = "C"+Change1(str_key) + "\n{\n"
		for member in one.member_s:
			if member.type1 in ["optional" , "required"] :
				if member.type2 == "string":
					out_str2 += "\tstd::string m_str[m1];\n".replace("[m1]" , Change1(member.name) )
				elif member.type2 in ["int32" , "uint32" , "uint64" , "int64"]:
					out_str2 += "\t"+member.type2 + " m_n[m1];\n".replace("[m1]" , Change1(member.name) )
		out_str2 += "};"
		self.input3.setText(out_str2 )
		return one
		
	def on_box1_activated(self , num_or_str):
		if type(num_or_str) != type(1):
			self.input1.setText(num_or_str)
		
	#加载到c++对象
	def on_b1_pressed(self ):
		str_key = Qstring2utf(self.input1.text() )
		one = self.update1()
		if one == None:
			return
		#
		out_str = ""
		for member in one.member_s:
			if member.type1 in ["optional" , "required"] :
				if member.type2 == "string":
					out_str += "oData.m_str[m1] = oProto.[m2]();\n".replace("[m1]" , Change1(member.name) ).replace("[m2]" , member.name )
				elif member.type2 in ["int32" , "uint32" , "uint64" , "int64"]:
					out_str += "oData.m_n[m1] = oProto.[m2]();\n".replace("[m1]" , Change1(member.name) ).replace("[m2]" , member.name )
		
		self.out1.setText(out_str )
		
		
	#保存到proto对象
	def on_b2_pressed(self ):
		str_key = Qstring2utf(self.input1.text() )
		one = self.update1()
		if one == None:
			return
		#
		out_str = ""
		for member in one.member_s:
			if member.type1 in ["optional" , "required"] :
				if member.type2 == "string":
					out_str += "oProto.set_[m2]( oData.m_str[m1] );\n".replace("[m1]" , Change1(member.name) ).replace("[m2]" , member.name )
				elif member.type2 in ["int32" , "uint32" , "uint64" , "int64"]:
					out_str += "oProto.set_[m2](oData.m_n[m1]);\n".replace("[m1]" , Change1(member.name) ).replace("[m2]" , member.name )
		
		self.out1.setText(out_str )
		
	def on_b3_pressed(self ):
		import hbb_tool
		
		str_key = ""
		for one_ in hbb_tool.GetText():
			if one_ == "\0":
				break
			str_key += one_;
		
		one = self.dict_proto.get( str_key , None)
		if one != None:
			self.input2.setText( str(one).decode("utf") )
		else:
			print "not find obj"

#类_初始化
app = QtGui.QApplication(sys.argv)

ui_test = UiMain()
ui_test.show()
#ui_test.hide()

sys.exit(app.exec_())

