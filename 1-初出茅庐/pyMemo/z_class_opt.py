
#类操作大全;class;





#定义私有类
class ServerCount2:
	def __init__(self):
		self.battlegateserver = "1"
		self.battleserver = "1"
		self.centerserver = "1"
		self.dbserver = "1"
		self.gameserver = "1"

#定义静态类
class ServerCount:
	battlegateserver = "1"
	battleserver = "1"
	centerserver = "1"
	dbserver = "1"
	gameserver = "1"
	gateserver = "1"
	loginserver = "1"
	proxyserver = "1"

gServerCount = ServerCount()

#获取变量#遍历类对象#反射
def GetServerCount(strName):
	global gServerCount
	if strName in dir(gServerCount):
		nCount = int(getattr(gServerCount ,strName ))
		#setattr(gServerCount ,strName , v_)		#二次修改变量
		print nCount







