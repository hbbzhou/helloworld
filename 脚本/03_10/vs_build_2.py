#!/usr/bin/env python 
#encoding: utf-8

import os

import smtplib
import email.mime.multipart
import email.mime.text
#from email.header import Header

# 第三方 SMTP 服务
mail_host="smtp.163.com"  #设置服务器
mail_user="huangbinbin_fd@163.com"    #用户名
mail_pass="123456jkl"   #口令 

sender = mail_user
list_receivers = ["binbin.huang@mokun.net"]  
#list_receivers = ["binbin.huang@mokun.net","huang_binbin1@163.com"]


#全局变量定义
g_src_path = "E:\\snk_code_3\\"
g_build_log_file = "build_log.txt"
g_svn_log_file = "svn_up_log.txt"
g_str_log = ""

#一般_不要修改
g_vs_path = "c:\\program files (x86)\\microsoft visual studio 14.0\\vc\\bin"

#建议_不要修改
g_build_is_ok = False
g_last_build_is_ok = False

class BuildAll:
	#定义构造方法
	def __init__(self):
		cmd_del_file = "cd /d "+g_src_path + "servers && rm -rf config_code_py"
		cmd_svn_up = "cd /d "+g_src_path+" && svn up >" + g_src_path + g_svn_log_file
		cmd_run_py = "cd /d "+g_src_path + "servers && start csv2cpp.py none"
		
		cmd_build ="cd /d \""+g_vs_path+"\" &&\
vcvars32.bat	&&\
cd /d "+g_src_path+"servers &&\
devenv servers.sln /build \"Debug\" && \
devenv servers.sln /build \"Debug\" >"  + g_src_path + g_build_log_file

		self.list_ = []
		self.list_2 = []
		self.list_.append(cmd_del_file)
		self.list_.append(cmd_svn_up)
		self.list_2.append(cmd_run_py)
		self.list_2.append(cmd_build)


def g_run_cmd(cmd_):
	str_vesion = GetSvnRevision()
	print str_vesion
	for v_ in cmd_.list_:
		os.system(v_)
	if str_vesion == GetSvnRevision():
		print "该 svn 版本 不需要 重复处理".decode("utf").encode("gbk")
		os._exit(0)
		
	for v_ in cmd_.list_2:
		os.system(v_)
		
	f = open(g_src_path + g_build_log_file ,"r")
	lines = f.readlines()#读取全部内容

	global g_build_is_ok
	global g_str_log
	g_build_is_ok = True
	for line in lines :
		g_str_log += line
		if line.find("): error") >= 0 :
			print "have error"
			g_build_is_ok = False
			break;
	f.close();
	
def GetSvnRevision():
	if os.path.exists(g_src_path + g_svn_log_file) == False:
		return "none"

	f = open(g_src_path + g_svn_log_file ,"r")
	lines = f.readlines()#读取全部内容
	
	for one_ in lines:
		n_1 = one_.find("At revision")
		if n_1 >= 0:
			return one_[n_1:]

	return "none"

def GetLastStatus():
	global g_last_build_is_ok
	g_last_build_is_ok = True
	
	if os.path.exists(g_src_path + g_build_log_file) == False:
		return

	f = open(g_src_path + g_build_log_file ,"r")
	lines = f.readlines()#读取全部内容

	for line in lines :
		if line.find("): error") >= 0 :
			print "have error"
			g_last_build_is_ok = False
			break;
	f.close();

def SendMail():
	print g_build_is_ok
	print g_last_build_is_ok
	if g_build_is_ok == False:
		if g_last_build_is_ok == True:
			SendMailStep1("服务端代码_编译失败" , g_str_log)
	else:
		if g_last_build_is_ok == False:
			SendMailStep1("服务端代码_编译成功" , "^_^")
			

def SendMailStep2(str_receiver , str_title , str_content):
	message = email.mime.multipart.MIMEMultipart() 
	message['From'] = sender
	message['To'] =  str_receiver
	message['Subject'] = str_title
		
	txt = email.mime.text.MIMEText(str_content,'plain','utf-8')
	message.attach(txt)
	return message
	
	
def SendMailStep1(str_title , str_content):
	smtpObj = smtplib.SMTP() 
	smtpObj.connect(mail_host,25)    # 25 为 SMTP 端口号
	smtpObj.login(mail_user,mail_pass)
	for one_ in list_receivers:
		msg = SendMailStep2(one_ , str_title , str_content)
		smtpObj.sendmail(sender, one_, msg.as_string())
	print "send ok"

GetLastStatus()
g_run_cmd(BuildAll() )
SendMail()

#os.system("shutdown -s -t 300")
#print "shutdown -a"





























