﻿

import os

#待需改
g_find_info = ["鲤鱼" , "海" , "金刚"]

g_job_s = []
class Job:
	def __init__(self , name , url):
		global g_job_s
		self.name = name
		self.url = url
		g_job_s.append(self)
	
	def Deal(self , str_):
		if str_ in self.name:
			#打开ie zhou
			print self.name.decode("utf").encode("gbk")
			print "open url" , self.url
			str_cmd = "start iexplore " + self.url
			os.system(str_cmd)
			return True
		return False	
	
def AddJob():
	Job("涂壁" , "http://www.paopaoche.net/sj/109175.html")
	Job("水池" , "http://www.paopaoche.net/sj/109323.html")
	Job("提灯小僧" , "http://www.paopaoche.net/sj/109140.html")
	Job("天邪鬼绿" , "http://www.paopaoche.net/sj/109163.html")
	Job("天邪鬼青" , "http://www.paopaoche.net/sj/109171.html")
	Job("天邪鬼黄" , "http://www.paopaoche.net/sj/109166.html")
	Job("管狐" , "http://www.paopaoche.net/sj/109099.html")
	Job("寄生魂" , "http://www.paopaoche.net/sj/109174.html")
	Job("酒吞童子" , "http://www.paopaoche.net/sj/108968.html")
	Job("纸扇_书生_面具" , "http://www.paopaoche.net/sj/109254.html")
	Job("河童" , "http://www.paopaoche.net/sj/109102.html")
	Job("山童" , "http://www.paopaoche.net/sj/109121.html")
	Job("山兔" , "http://www.paopaoche.net/sj/109125.html")
	Job("黑豹" , "http://www.paopaoche.net/sj/108013.html")
	Job("跳跳犬" , "http://www.paopaoche.net/sj/111887.html")
	Job("饿鬼" , "http://www.paopaoche.net/sj/109116.html")
	Job("童女" , "http://www.paopaoche.net/sj/109088.html")
	Job("凤凰火" , "http://www.paopaoche.net/sj/109045.html")
	Job("孟婆" , "http://www.paopaoche.net/sj/109039.html")
	Job("帚神" , "http://www.paopaoche.net/sj/109172.html")
	Job("傀儡师" , "http://www.paopaoche.net/sj/109014.html")
	Job("唐纸伞妖" , "http://www.paopaoche.net/sj/109148.html")
	Job("灯笼鬼" , "http://www.paopaoche.net/sj/109136.html")
	Job("盗墓小鬼" , "http://www.paopaoche.net/sj/109149.html")
	Job("青蛙瓷器" , "http://www.paopaoche.net/sj/109110.html")
	Job("鲤鱼精" , "http://www.paopaoche.net/sj/109052.html")
	Job("海坊主" , "http://www.paopaoche.net/sj/109038.html")
	Job("独眼小僧_金刚经" , "http://www.paopaoche.net/sj/109038.html")
	


def DealStep1(str_):
	for one_ in g_job_s:
		if one_.Deal(str_) :
			return
	str_1 =  "没有找到任务:" + str_
	print str_1.decode("utf").encode("gbk") 
	
if __name__ == "__main__":
	AddJob()	
	for one_ in g_find_info:
		DealStep1(one_)





