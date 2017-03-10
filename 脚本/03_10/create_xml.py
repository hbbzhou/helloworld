#!/usr/bin/env python 
#encoding: utf-8

import os

#全局变量定义
g_game_name = 'hbb_win'
g_game_id = '103'
g_db_name = 'test'
g_db_pwd = '123456'
g_db_ip = '127.0.0.1'
g_my_ip = ''		#对内的ip 
g_my_ip_used_client_connect = ''		#对外的ip

#一般不需要修改
g_center_game_id = '0' #inner 上的数值为0
g_center_ip = '192.168.0.50'	#inner 上的数值为192.168.0.50
g_out_file_path = './'

#建议_不要修改
g_dbserver_port = '8001'	#默认端口 8001
g_friend_port = '5010'	#默认端口 5010
g_global_port = '8666'	#默认端口 8666
g_gate_port = '8345'	#默认端口 8345
g_gate_client_port = '8111'	#默认端口 8111;提供给客户端访问
g_name_port = '9000'	#默认端口 9000
g_game_port = '8101'	#默认端口8101

g_cross_port = '10108'	#默认端口 10108
g_center_port = '8567'	#默认端口 8567
g_login_port = '8555'	#默认端口 8555;这个端口要和php中的端口保持一致
g_log_port = '5001'	#默认端口 5001


def set_local_ip():
	global g_my_ip
	global g_my_ip_used_client_connect
	if g_my_ip == "":
		import socket
		myname = socket.getfqdn(socket.gethostname())
		g_my_ip = socket.gethostbyname(myname)
		g_my_ip_used_client_connect = g_my_ip



#全局变量二次赋值
set_local_ip()

g_ditc_ = {}
g_ditc_["[103]"] = g_game_id
g_ditc_["[5010]"] = g_friend_port
g_ditc_["[8001]"] = g_dbserver_port
g_ditc_["[8345]"] = g_gate_port
g_ditc_["[8101]"] = g_game_port
g_ditc_["[10108]"] = g_cross_port
g_ditc_["[8666]"] = g_global_port
g_ditc_["[8111]"] = g_gate_client_port
g_ditc_["[9000]"] = g_name_port
g_ditc_["[8567]"] = g_center_port
g_ditc_["[8555]"] = g_login_port
g_ditc_["[5001]"] = g_log_port

g_ditc_["[g_db_ip]"] = g_db_ip
g_ditc_["[123456]"] = g_db_pwd
g_ditc_["[test]"] = g_db_name


g_ditc_["[hbb_win]"] = g_game_name
g_ditc_["[g_my_ip]"] = g_my_ip
g_ditc_["[g_my_ip_used_client_connect]"] = g_my_ip_used_client_connect

g_ditc_["[g_center_game_id]"] = g_center_game_id
g_ditc_["[g_center_ip]"] = g_center_ip



def ReplaceAll(str_ ):
	for (k , v) in g_ditc_.items():
		str_ = str_.replace(k , v)
	return str_

class FightServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'fightserver/'
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
		<server id="1-[103]-11-1"/>
		<connection mode="connect">
			<!-- connect to gate server -->
			<item id="1-[103]-3-1" remoteip="127.0.0.1" remoteport="[8345]" recvbuf="1310720" sendbuf="131072" />
			<!-- connect to game server -->
			<item id="1-[103]-1-1" remoteip="127.0.0.1" remoteport="[8101]" recvbuf="1310720" sendbuf="131072" />
		</connection>
	</connections>
</config>
'''

		self.dict_['fs_pipe_cfg.xml'] = ReplaceAll(str_)


class CrossServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'crossserver/'
		
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
		<server id="1-[103]-10-1"/>
		<connection mode="listen" localip="127.0.0.1" localport="[10108]" recvbuf="131072" sendbuf="65536" />
	</connections>
</config>
'''

		self.dict_['cross_pipe_cfg.xml'] = ReplaceAll(str_)

		str_ = '''<global>
	<connect ip="[g_db_ip]" port="3306" user="root" password="[123456]" db="[test]" charset="utf8"/>
	<createfile name="mysql.sql"/>
</global>
'''

		self.dict_['db_config.xml'] = ReplaceAll(str_ )




class DBServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'dbserver/'
		
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<configs>
	<!--game mysql connect information -->
	<group_game_mysql>
		<!-- connect mysql info -->
		<mysql group="0" ip="[g_db_ip]" port="3306" user="root" password="[123456]" db="[test]" charset="utf8"/>
	</group_game_mysql>
	<createfile name="mysql.sql"/>
</configs>
'''

		self.dict_['db_mysql_cfg.xml'] = ReplaceAll(str_ )
		
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
		<server id="1-[103]-2-1"/>
		<connection mode="listen" localip="127.0.0.1" localport="[8001]" recvbuf="131072" sendbuf="65536" />
    </connections>
</config>
'''
		self.dict_['db_pipe_cfg.xml'] = ReplaceAll(str_ )




class FriendServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'friendserver/'
		
		str_ = '''<global>
	<connect ip="[g_db_ip]" port="3306" user="root" password="[123456]" db="[test]" charset="utf8"/>
	<createfile name="mysql.sql"/>
</global>
'''
		self.dict_['db_config.xml'] = ReplaceAll(str_ )

		
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
	<server id="1-[103]-9-1"/>
	<connection mode="listen" localip="127.0.0.1" localport="[5010]" recvbuf="131072" sendbuf="65536" />
	</connections>
</config>
'''

		self.dict_['friend_pipe_cfg.xml'] = ReplaceAll(str_ )




class GameServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法  
	def __init__(self):
		self.path_ = g_out_file_path + 'gameserver/'
		
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
	<server id="1-[103]-1-1"/>
	<!-- listen to fight to connect -->
	<connection mode="listen" localip="0" localport="[8101]" recvbuf="1310720" sendbuf="131072" />
	<connection mode="connect">
		<!--global server-->
		<item id="1-[103]-6-1" remoteip="127.0.0.1" remoteport="[8666]" recvbuf="1310720" sendbuf="131072" />
		<!-- connect to dbserver -->
		<item id="1-[103]-2-1" remoteip="127.0.0.1" remoteport="[8001]" recvbuf="1310720" sendbuf="131072" />
		<!-- connect to gate server -->
		<item id="1-[103]-3-1" remoteip="127.0.0.1" remoteport="[8345]" recvbuf="1310720" sendbuf="131072" />
		<!-- connect to friend server -->
		<item id="1-[103]-9-1" remoteip="127.0.0.1" remoteport="[5010]" recvbuf="1310720" sendbuf="131072" />
		<!-- connect to cross server -->
		<item id="1-[103]-10-1" remoteip="127.0.0.1" remoteport="[10108]" recvbuf="1310720" sendbuf="131072" />
	</connection>
</connections>
</config>
'''
		self.dict_['gs_pipe_cfg.xml'] = ReplaceAll(str_ )
	



class GateServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'gateserver/'
		
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
		<server id="1-[103]-3-1"/>
		<!-- listen to game to connect -->
		<connection mode="listen" localip="0" localport="[8345]" recvbuf="1310720" sendbuf="131072" />
		<connection mode="connect">
			<!-- connect to db -->
			<item id="1-[103]-2-1" remoteip="127.0.0.1" remoteport="[8001]" recvbuf="131072" sendbuf="131072" />
			<!-- connect to center -->
			<item id="1-[g_center_game_id]-5-1" remoteip="[g_center_ip]" remoteport="[8567]" recvbuf="131072" sendbuf="131072" />
			<!-- connect to name -->
			<item id="1-[103]-7-1" remoteip="127.0.0.1" remoteport="[9000]" recvbuf="131072" sendbuf="131072" />
			<!-- connect to log server -->
			<!--item id="1-[103]-8-1" remoteip="127.0.0.1" remoteport="[5001]" recvbuf="1310720" sendbuf="131072" /-->
		</connection>
	</connections>
</config>
'''
		
		self.dict_['gt_pipe_cfg.xml'] = ReplaceAll(str_ )


		str_ = '''<Gate>
	<Listen>
		<!-- server listen info -->
		<seritem serlistenip="[g_my_ip]" serlistenport="[8111]" serlistenrecvbuf="81920" serlistensendbuf="81920"/>
		<!-- client connect info -->
		<cliitem clilistenip="[g_my_ip_used_client_connect]" clilistenport="[8111]" group_name="[hbb_win]"/>
	</Listen>
	<ServerLoad>
		<loaditem firstload = "3" secondload = "2" />
	</ServerLoad>
</Gate>
'''
		self.dict_['gt_server_cfg.xml'] = ReplaceAll(str_ )

	




class GlobalServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'globalserver/'
		
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
		<server id="1-[103]-6-1"/>
		<!-- listen to gate to connect -->	
		<connection mode="listen" localip="127.0.0.1" localport="[8666]" recvbuf="1310720" sendbuf="131072" />
		<connection mode="connect">
			<!-- connect to log server -->
			<!--item id="1-[103]-8-1" remoteip="127.0.0.1" remoteport="[5001]" recvbuf="1310720" sendbuf="131072" /-->
		</connection>
	</connections>
</config>
'''

		self.dict_['global_pipe_cfg.xml'] = ReplaceAll(str_ )


		str_ = '''<global>
	<listen>
	<item ip="127.0.0.1" port="8999" sendbuf="81920" recvbuf="81920"/>
	</listen>
</global>
'''

		self.dict_['global_server.xml'] = str_

		str_ = '''<global>
	<connect ip="[g_db_ip]" port="3306" user="root" password="[123456]" db="[test]" charset="utf8"/>
	<createfile name="mysql.sql"/>
</global>
'''		
		self.dict_['db_config.xml'] = ReplaceAll(str_ )



class LoginServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'loginserver/'
		
		str_ = '''<login>
	<listen>
	<item ip="[g_my_ip]" port="[8555]" recvbuf="81920" sendbuf="81920"/>
	</listen>
</login>
'''
		self.dict_['login_server.xml'] = ReplaceAll(str_ )

		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
		<server id="1-[103]-4-1"/>
		<connection mode="connect">
			<!-- connect to center -->
			<item id="1-[g_center_game_id]-5-1" remoteip="127.0.0.1" remoteport="[8567]" recvbuf="131072" sendbuf="131072" />
			<!-- connect to log server -->
			<!--item id="1-[103]-8-1" remoteip="127.0.0.1" remoteport="[5001]" recvbuf="1310720" sendbuf="131072" /-->
		</connection>
	</connections>
</config>
'''
		self.dict_['login_pipe_cfg.xml'] =ReplaceAll(str_ )

		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<db_verify>
	<connect ip="[g_db_ip]" port="3306" user="root" password="[123456]" db="[test]" charset="utf8"/>
</db_verify>
'''
		self.dict_['db_config.xml'] =ReplaceAll(str_ )




class NameServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'nameserver/'
		
		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
		<server id="1-[103]-7-1"/> 
		<connection mode="listen" localip="127.0.0.1" localport="[9000]" recvbuf="131072" sendbuf="65536" />
		<connection mode="connect">
			<!-- connect to log server -->
			<!--item id="1-[103]-8-1" remoteip="127.0.0.1" remoteport="[5001]" recvbuf="1310720" sendbuf="131072" /-->	
		</connection>
	</connections>
</config>
'''
		self.dict_['name_pipe_cfg.xml'] = ReplaceAll(str_ )

		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<db_connect>
	<connect ip="[g_db_ip]" port="3306" user="root" password="[123456]" db="[test]" charset="utf8"/>
	<createfile name="mysql.sql"/>
</db_connect>
'''
		self.dict_['name_config.xml'] =ReplaceAll(str_ )



class CenterServer:
	#定义基本属性
	dict_ = {}
	path_ = ''
	#定义构造方法
	def __init__(self):
		self.path_ = g_out_file_path + 'centerserver/'

		str_ = '''<?xml version="1.0" encoding="utf-8"?>
<config>
	<connections>
		<server id="1-[g_center_game_id]-5-1"/> 
		<connection mode="listen" localip="0" localport="[8567]" recvbuf="131072" sendbuf="65536" />
		<connection mode="connect">
			<!-- connect to log server -->
			<!--item id="1-[103]-8-1" remoteip="127.0.0.1" remoteport="[5001]" recvbuf="1310720" sendbuf="131072" /-->
		</connection>
	</connections>
</config>
'''
		self.dict_['center_pipe_cfg.xml'] =ReplaceAll(str_ )



def g_write2file(o_server):
	tup_path = ("./" , "debug/" , "release/")
	
	for index in range(len(tup_path)):
		now_path = o_server.path_ + tup_path[index]
		if os.path.exists(now_path) == False:
			os.mkdir(now_path)

		for k_,v_ in o_server.dict_.items():
			fo = open(now_path + k_, "w")
			fo.write(v_);
			fo.close()
			print("##create file:%s\n "%(now_path + k_ ) )



g_write2file(CenterServer() )

g_write2file( CrossServer() )

g_write2file(DBServer() )

g_write2file(FriendServer() )

g_write2file(GateServer() )

g_write2file(GameServer() )

g_write2file(GlobalServer() )

g_write2file(LoginServer() )

g_write2file(NameServer() )

g_write2file(FightServer() )


print("##work path:")
os.system("echo %cd%")
os.system("pause")

































