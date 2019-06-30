


#文件拷贝;文件是否存在;目录是否存在
def copy_to_dir(str_dir_0 , str_dir_1):
	if os.path.exists(str_dir_0) == False:
		print "##err"
		return

	if os.path.exists(str_dir_1) == False:
		os.mkdir(str_dir_1)

	for parent, dirnames, filenames in os.walk(str_dir_0):
		for filename in filenames:
			str_file_0 = parent +"/"+ filename
			shutil.copy (str_file_0, str_dir_1)
			print "#copy " , str_file_0 , "\t->>" , str_dir_1
		break #仅仅 遍历 第一层目录

#写文件
def newfile(csv_file ):
	if os.path.exists(g_csv_dir) == False:
		os.mkdir(g_csv_dir)
	f = open(g_csv_dir + csv_file , "w")
	f.close()

#追加方式打开文件
def add2file(csv_file , str_):
	if os.path.exists(g_csv_dir) == False:
		os.mkdir(g_csv_dir)
	f = open(g_csv_dir + csv_file , "a")
	f.write(str_)
	f.close()

#创建utf类型的文件
def InitFile(path , file_ ,str_):
	import codecs
	if os.path.exists(path) == False:
		os.mkdir(path)
	f = codecs.open(path+file_, "w", "utf-8")
	f.write(str_)
	f.close()

#清空某个文件夹
def ClearFolder(path_):
	ls = os.listdir(path_)
	for i in ls:
		one_path = os.path.join(path_, i)
		print one_path
		if os.path.isdir(one_path):
			ClearFolder(one_path)
		else:
			os.remove(one_path)

#删除文件夹
def DelFolder(path_):
	ClearFolder(path_)
	os.rmdir(path_)

