#!/usr/bin/env python 
#encoding: utf-8

import os
#全局变量定义
g_src_path = "D:\\hbb_release_file\\"
g_build_log_file = "build_log.txt"
g_svn_st_log_file = "svn_st_log.txt"

#一般_不要修改
g_vs_path = "c:\\program files (x86)\\microsoft visual studio 14.0\\vc\\bin"

#建议_不要修改
g_build_is_ok = False
g_list_server = [];

class BuildAll:
	#定义基本属性
	list_ = [""]
	#定义构造方法
	def __init__(self):
		cmd_svn_up = "cd /d "+g_src_path+" && svn up"
		
		cmd_build ="cd /d \""+g_vs_path+"\" &&\
vcvars32.bat	&&\
cd /d "+g_src_path+"servers &&\
devenv server.sln /build \"Release\" && \
devenv server.sln /build \"Release\" >"  + g_src_path + g_build_log_file
		
		cmd_svn_st = "cd /d "+g_src_path+"bin && svn st -q >" + g_src_path + g_svn_st_log_file

		self.list_.append(cmd_svn_up)
		self.list_.append(cmd_build)
		self.list_.append(cmd_svn_st)


def g_run_cmd(cmd_):
	for v_ in cmd_.list_:
		os.system(v_)
		
	f = open(g_src_path + g_build_log_file ,"r")
	lines = f.readlines()#读取全部内容

	global g_build_is_ok
	g_build_is_ok = True
	for line in lines :
		if line.find("): error") > 0 :
			print "have error"
			g_build_is_ok = False
			break;
	f.close();


def g_is_must_commit(str_):	
	list_str_ = [".exe" , ".map" , ".pdb"]
	
	if str_[0:1] != 'M':
		return
	
	for one_ in list_str_:
		if str_.find(one_ ) > 0 :
			g_list_server.append(str_[7:-1]);
			

#提交svn
def g_svn_commit():	
	if g_build_is_ok == False:
		print "build failed"
		return

	f = open(g_src_path + g_svn_st_log_file ,"r")
	lines = f.readlines()#读取全部内容		
	for line in lines :
		g_is_must_commit(line)
	f.close();
	
	if len(g_list_server) == 0 :
		print "none diff file"
		return;
	
	cmd_svn_commit = "cd /d "+g_src_path+"bin "
	cmd_svn_commit += "&& svn commit "
	for one_ in g_list_server :
		cmd_svn_commit += " " + one_
	
	cmd_svn_commit += "  -m \"update release version by vs_build.py\""
	
	print cmd_svn_commit
	print len(g_list_server)
	os.system(cmd_svn_commit)
	
	#执行关机
	
	
	
g_run_cmd(BuildAll() )
g_svn_commit()


#os.system("shutdown -s -t 300")
#print "shutdown -a"

print "now path:"
os.system("echo %cd%")
os.system("pause")




























