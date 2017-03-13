
import xlrd
import sys
import os
from datetime import date,datetime

g_xecl_dir = "xlsx_dir/"
g_csv_dir = "csv_dir/"

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
		def _tostr(cell):
			if type(u'') == type(cell): 
				return cell.encode('gbk')
			elif type(u'float') == type(cell): 
				return str(cell)
			else:
				n_ = int(cell)
				return str(n_)
		str_ = ','.join([_tostr(cell) for cell in rows ])
		add2file(csv_file , str_ + "\n")
	
if __name__ == '__main__':
	for parent, dirnames, filenames in os.walk(g_xecl_dir): 
		for filename in filenames:
			if filename.find(".xlsx") < 0:
				continue
				
			read_excel(parent,filename)
			
	print "生成_成功...".decode("utf8").encode("gbk")
	os.system("pause")
	



