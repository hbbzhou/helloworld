
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


sys.path.append("proto_py/")
#加载proto
import login_pb2
import client_pb2
import base_pb2
import mail_pb2


#配置ui界面
g_str_ui_test = "test_main.ui"
test_class, base_test_class = uic.loadUiType(g_str_ui_test)



class Uitest(QtGui.QDialog, test_class):
	OnClicked = pyqtSignal([str,str])
	def __init__(self, *args):
		super(Uitest, self).__init__(*args)
		self.setupUi(self)

		self.init_box1()

	def AddLog(self , str_ , str_type):
		self.OnClicked.emit(str_.decode("utf") ,str_type )
		
	def OnAddLog(self , str_, str_type):
		if str_type == "out1":
			self.out1.setText(str_ )

	def base_show_tree2(self, one_proto , node_):
		
		str_member = unicode(node_.text(0)).encode("utf")
		str_type = unicode(node_.text(1)).encode("utf")
		str_value = unicode(node_.text(2)).encode("utf")
		if "<class" in str_type:
			one_proto2 = getattr(one_proto , str_member )
			for index_2 in range( node_.childCount() ):
				one_proto2 = self.base_show_tree2(one_proto2 ,node_.child(index_2) )
		elif "<type" in str_type:
			if str_type == str(type(1) ):
				setattr(one_proto , str_member, int(str_value) )
			elif str_type == str(type(0.1)):
				setattr(one_proto , str_member, float(str_value) )
			elif str_type == str(type(1L)):
				setattr(one_proto , str_member, long(str_value) )
			elif str_type == str(type(False)):
				setattr(one_proto , str_member, bool(str_value) )
			elif str_type == str(type(u'')):
				setattr(one_proto , str_member, str_value.decode("utf") )
			else :
				print "没有匹配的类型".decode("utf").encode("gbk")
		return one_proto

	def show_tree2(self ):
		str_cur_msg_name = unicode(self.box1.currentText()).encode("utf")
		pFun = getattr(client_pb2 , str_cur_msg_name )
		one_proto = pFun()

		for index_ in range( self.tree2.topLevelItemCount() ):
			node_ = self.tree2.topLevelItem(index_)
			one_proto = self.base_show_tree2(one_proto ,node_  )
		
		return one_proto



	def on_b3_pressed(self ):

		str_ = unicode(self.box1.currentText()).encode("utf")
		n_msg_id = getattr(client_pb2 , str_ + "_id") 
		print n_msg_id

	def init_box1(self):
		self.box1.clear()

		is_find = False
		for one_ in dir(client_pb2):
			if one_ == "_reflection":
				is_find = True

			if is_find == False:
				continue

			if one_.find("_req") == one_.find("_req_id"):
				continue

			self.box1.addItem(one_)

		#手动更新xx控件一次
		if is_find == True:
			self.on_box1_activated(self.box1.currentText() )

	def reset_tree2(self , cur_tree ,one_item):
		is_find = False
		for one_member in dir(one_item):
			if one_member == "_unknown_fields":
				is_find = True
				continue

			if is_find == True:
				one_value = getattr(one_item, one_member)
				str_type = str(type(one_value) )

				#初始化对象
				if "<type" in str_type:
					setattr(one_item , one_member, getattr(one_item, one_member) )
					#
					root= QTreeWidgetItem(cur_tree)
					root.setFlags(Qt.ItemIsEditable | Qt.ItemIsEnabled)
					root.setText(0,one_member)
					root.setText(1,str_type)
					root.setText(2,str(one_value) )
				elif "<class" in str_type:
					root= QTreeWidgetItem(cur_tree)
					root.setText(0,one_member)
					root.setText(1,str_type)
					#
					one_ = getattr(one_item , one_member )
					one_ = self.reset_tree2(root ,one_)

		return one_item

	def on_box1_activated(self , num_or_str):
		if type(num_or_str) != type(1):
			str_ = unicode(num_or_str).encode("utf")
			pFun = getattr(client_pb2 , str_ )
			one_ = pFun()

			#更新数据到树_控件
			self.tree2.clear()
			one_ = self.reset_tree2(self.tree2 ,one_)

	def on_b4_pressed(self ):
		self.out1.setText( str(self.show_tree2()) )


#类_初始化
g_thread = None
app = QtGui.QApplication(sys.argv)

ui_test = Uitest()
ui_test.show()
#ui_test.hide()


def OnUiManage():

	global g_thread
	g_thread = threading.Timer(0.2, OnUiManage)
	g_thread.start()

ui_test.OnClicked.connect(ui_test.OnAddLog,Qt.QueuedConnection)
OnUiManage()

sys.exit(app.exec_())

