
import xlrd
import sys
import os
from datetime import date,datetime

def get_cmd_log(str):
	str_file = "cmd_log.txt"
	os.system(str + " > " + str_file )
	f = open(str_file , "r")
	list_ = f.readlines()
	f.close()
	os.system("del -f " + str_file)
	str_ = ""
	for one_ in list_:
		str_ += one_
	return str_

def init_fun():
	str_ = get_cmd_log("svn --version")
	if "Copyright" not in str_:
		print "请设置 svn的 环境".decode("utf").encode("gbk")
		print "[盘符]:\[svn安装 目录]\TortoiseSVN\bin".decode("utf").encode("gbk")
		os.system("pause")
		os._exit(0)



def get_local_ip():
	import socket
	myname = socket.getfqdn(socket.gethostname())
	return socket.gethostbyname(myname)

g_my_ip = get_local_ip()

#csv 输出目录
g_csv_dir = "config/"

#excel svn默认路径
g_svn_checkout_path = "E:/"

#excel 路径
g_excel_dir = g_svn_checkout_path + "intermediate/ConfigXlsx/"

#自定义 变量
if  "192.168.3.33" in g_my_ip:
	g_svn_checkout_path = "E:/"
	g_excel_dir = g_svn_checkout_path + "intermediate/ConfigXlsx/"
if 	"192.168.3.52" in g_my_ip:
	g_csv_dir = "E:/Project/SNK/SNK_C_PC/Assets/Content/Config/"


def newfile(csv_file ):
	if os.path.exists(g_csv_dir) == False:
		os.mkdir(g_csv_dir)
	f = open(g_csv_dir + csv_file , "w")
	f.close()

def add2file(csv_file , str_):
	if os.path.exists(g_csv_dir) == False:
		os.mkdir(g_csv_dir)
	f = open(g_csv_dir + csv_file , "a")
	f.write(str_)
	f.close()

def _tostr(cell):
	if type(u'') == type(cell): 
		return cell.encode('gbk')
	else:
		if int(cell) == float(cell):
			n_ = int(cell)
			return str(n_)
		else:
			return str(cell)

	
def read_excel(parent,filename):
	fullpath = os.path.join(parent,filename)
	print fullpath
	
	csv_file = filename.split(".")[0] + ".csv";
	workbook = xlrd.open_workbook(fullpath)
	# print sheet2.name,sheet2.nrows,sheet2.ncols
	sheet2 = workbook.sheet_by_index(0)
	newfile(csv_file )
	for row in xrange(0, sheet2.nrows):
		rows = sheet2.row_values(row)
		str_ = ','.join([_tostr(cell) for cell in rows ])
		add2file(csv_file , str_ + "\n")

if __name__ == '__main__':
	init_fun()

	#checkout svn
	if os.path.exists(g_excel_dir) == False:
		str_cmd = "cd /d " + g_svn_checkout_path
		str_cmd += "&& svn checkout http://192.168.0.11/svn/snk/trunk/intermediate"
		os.system(str_cmd)

	print "准备更新svn".decode("utf").encode("gbk")
	str_cmd = "cd /d " + g_excel_dir + "&& svn up "
	os.system(str_cmd)
	print "==========="

	for parent, dirnames, filenames in os.walk(g_excel_dir): 	
		for filename in filenames:
			if filename.find(".xlsx") < 0:
				continue
			read_excel(parent,filename)

	print "生成_成功...".decode("utf8").encode("gbk")
	os.system("pause")
	



