
import xlrd
import sys
import os
import getpass
import time
from datetime import date,datetime

g_str_name = getpass.getuser()

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


g_csv_dir = "config/"

g_str_dst_file ='''
Hero.xlsx
ChatShop.xlsx
'''

#####config_beign

#csv 输出目录
g_csv_dir = "config/"

#excel svn默认路径
g_svn_checkout_path = "C:/Users/binbin.huang/Desktop/config_test_dir/"

#excel 路径
g_excel_dir = g_svn_checkout_path + "ConfigXlsx/"

#自定义 变量
if g_str_name == "ji.zhou": 
	g_svn_checkout_path = "e:/f-game/"
	g_excel_dir = g_svn_checkout_path + "intermediate/ConfigXlsx/"
	g_csv_dir = "config/"
else:
	print "没有找到,个人设置".decode("utf").encode("gbk")

#####config_end
	
def isNewCsv(csv_file):
	if os.path.exists(g_csv_dir + csv_file) == False:
		return True
	return False

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

#去掉备注中的逗号
def _tostr_2(cell):
	if type(u'') == type(cell):
		return cell.encode('gbk').replace("," , ";").replace("\r" , " ").replace("\n" , " ")
	else:
		if int(cell) == float(cell):
			n_ = int(cell)
			return str(n_)
		else:
			return str(cell)
			
#首字符大写
def _tostr_3(cell):
	if type(u'') == type(cell):
		str_ = str( cell.encode('gbk') )
		if len(str_) > 0:
			str_ = str_[0].upper() + str_[1:]
		return str_
	else:
		if int(cell) == float(cell):
			n_ = int(cell)
			return str(n_)
		else:
			return str(cell)
			
#将回车替换成 '|'
def _tostr_4(cell):
	if type(u'') == type(cell):
		return cell.encode('gbk').replace("," , ";").replace("\r" , " ").replace("\n" , "|")
	else:
		if int(cell) == float(cell):
			n_ = int(cell)
			return str(n_)
		else:
			return str(cell)

#获取第一张sheet
def GetOneSheet(parent,filename):
	fullpath = os.path.join(parent,filename)
	print fullpath
	workbook = xlrd.open_workbook(fullpath)
	return workbook.sheet_by_index(0)

#读文件
def read_excel(parent,filename):
	sheet2 = GetOneSheet(parent,filename)
	str_1 = ""
	line_num = 5	#指定关键行
	if sheet2.nrows > line_num :
		str_1 = sheet2.cell_value(line_num, 0)

	if str_1 not in ["a","c","s","A","C","S"]:
		print "数据表_出错..错误码001".decode("utf").encode("gbk")
		os._exit(0)
	#
	read_excel_4(parent,filename , line_num)
		
def read_excel_4(parent,filename, line_num):
	sheet2 = GetOneSheet(parent,filename)
	
	csv_file = filename.split(".")[0] + ".csv";
	newfile(csv_file )
	
	row_1 = sheet2.row_values(line_num)
	for row in xrange(0, sheet2.nrows):
		if row in [3,5]:
			continue
			
		rows = sheet2.row_values(row)
		list_content = []
		for j_ in xrange(0, sheet2.ncols):
			if row_1[j_] not in ["a","A" , "s" , "S"]:#排除 不是 服务端的
				continue

			str_data = ""
			if row == 0:
				str_data = _tostr_2( rows[j_] )
			elif row == 1:
				str_data = _tostr_3( rows[j_] )
			elif row == 2:
				str_data = _tostr_4( rows[j_] )
			else:
				str_data = _tostr( rows[j_] )
			#
			if "," in str_data:
				print "数据表_出错..错误码002".decode("utf").encode("gbk")
				print str_data
				os._exit(0)
				
			list_content.append(str_data)
		#
		str_ = ','.join(list_content)
		add2file(csv_file , str_ + "\n")
		
#解析setting表
def read_Setting(parent,filename):
	sheet2 = GetOneSheet(parent,filename)
	csv_file = filename.split(".")[0] + ".csv";
	newfile(csv_file )
	
	n_begin_line = 6	#开始行号
	
	if sheet2.ncols < n_begin_line:
		print "error"
		os._exit(0)
		
	list_dsc = sheet2.col_values(1)
	list_name = sheet2.col_values(3)
	list_type = sheet2.col_values(4)
	list_data = sheet2.col_values(2)
	list_del = []
	
	#跳过前5行
	for one_ in range(n_begin_line):
		list_dsc.pop(0)
		list_name.pop(0)
		list_type.pop(0)
		list_data.pop(0)
	
	index = 0
	while True:
		if index >= len(list_name):
			break
			
		if "ignore" in list_name[index]:
			list_dsc.pop(index)
			list_name.pop(index)
			list_type.pop(index)
			list_data.pop(index)
		elif list_name[index] == "":
			list_dsc.pop(index)
			list_name.pop(index)
			list_type.pop(index)
			list_data.pop(index)
		else:
			index += 1
	
	str_ = ','.join([_tostr(cell) for cell in list_dsc ])
	add2file(csv_file , "_ID," + str_ + "\n")
	
	str_ = ','.join([_tostr(cell) for cell in list_name ])
	add2file(csv_file , "#ID," + str_ + "\n")
	
	str_ = ','.join([_tostr(cell) for cell in list_type ])
	add2file(csv_file , "int," + str_ + "\n")
	
	str_ = ','.join([_tostr(cell) for cell in list_data ])
	add2file(csv_file , "0," + str_ + "\n")
	
def ChangeFile2Utf( file_ ):
	import codecs
	f_0 = open(file_ ,"r")
	str_ = f_0.read(-1)
	f_0.close()
	str_ = str_.decode("gbk")
	f_1 = codecs.open( file_, "w", "utf-8-sig")
	f_1.write(str_)
	f_1.close()
	print file_ , "转 utf".decode("utf").encode("gbk")
	
#####
g_dict_md5 = {}
g_str_file = "all_md5.txt"

def ShowFileMd5(str_file):
	import hashlib
	
	if os.path.exists(str_file) == False:
		return "openFileError"
	
	f_0 = open(str_file , "rb")
	str_ret = hashlib.md5( f_0.read(-1)).hexdigest()
	f_0.close()
	
	return str_ret

def IsDiffOfMd5(str_key):
	str_csv = g_csv_dir + str_key + ".csv"
	str_xls = g_excel_dir + str_key + ".xlsx"
	
	global g_dict_md5
	if g_dict_md5.get(str_csv , None) != ShowFileMd5(str_csv):
		return True
		
	if g_dict_md5.get(str_xls , None) != ShowFileMd5(str_xls):
		return True

	return False
	
def UpdateMd5(str_key):
	str_csv = g_csv_dir + str_key + ".csv"
	str_xls = g_excel_dir + str_key + ".xlsx"
	
	g_dict_md5[str_csv] = ShowFileMd5(str_csv)
	g_dict_md5[str_xls] = ShowFileMd5(str_xls)
	
def LoadMd5():
	global g_str_file
	
	if os.path.exists(g_str_file) == False:
		return
	
	f_ = open( g_str_file, "r")
	list_ = f_.readlines()
	f_.close()
	
	global g_dict_md5
	str_file = ""
	for i_ in range(len(list_) ):
		one_ = list_[i_].replace("\r" , "").replace("\n" , "")
		if i_ % 2 == 0:
			str_file = one_
		else:
			g_dict_md5[str_file] = one_

	
def SaveMd5():
	global g_str_file
	f_ = open(g_str_file , "w")
	
	global g_dict_md5
	for k_ ,v_ in g_dict_md5.items():
		f_.write(k_ + "\n")
		f_.write(v_ + "\n")
		
	f_.close()

######

if __name__ == '__main__':
	n_cur_t = time.time()

	#init_fun()
	LoadMd5() #加载本地md5标识

	print "==========="

	list_log = []
	list_log3 = []
	for parent, dirnames, filenames in os.walk(g_excel_dir):
		for filename in filenames:
			if filename.find(".xlsx") < 0:
				continue

			if filename not in g_str_dst_file:
				list_log.append(filename)
				continue

			str_key = filename[0:-5]
			if IsDiffOfMd5( str_key ) == False:
				list_log3.append(filename)
				continue

			if "Setting.xlsx" in filename:
				read_Setting(parent,filename)
			else:
				read_excel(parent,filename)

			if str_key in ["SensitiveWords" , "MailInfo" , "Setting" ]:
				ChangeFile2Utf(g_csv_dir + str_key + ".csv")

			UpdateMd5(str_key)
				
			
		break #仅仅 遍历 第一层目录

	for one_ in list_log3:
		#print "md5一致,故忽略:".decode("utf").encode("gbk") , one_
		pass
	#
	print "=============="
	list_log2 = []
	for one_ in list_log:
		if isNewCsv(one_.replace(".xlsx" , ".csv") ) == True:
			print "忽略文件:".decode("utf").encode("gbk") , one_
		else:
			list_log2.append(one_)
	print "=============="
	for one_ in list_log2:
		print "遗漏文件:".decode("utf").encode("gbk") , one_
			
	print "生成_成功...".decode("utf8").encode("gbk")
	print "excel的路径..".decode("utf8").encode("gbk") , g_excel_dir
	#
	SaveMd5()
	
	print "diff time" , time.time() - n_cur_t , "s"
	os.system("pause")
	os.system("checkCsv.py")
	
	



