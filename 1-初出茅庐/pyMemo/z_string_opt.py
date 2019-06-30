



#显示二进制
def ShowBuff(str_buff):
	str_out = ""
	for one_ in str_buff:
		str_out = str_out + "/" + str( ord(one_) )
	print str_out
	return str_out
	
#加载二进制
def DecodeBuff(encode_buff):
	str_out = ""
	for one_ in encode_buff.split("/"):
		if len(one_) == 0:
			continue

		str_out = str_out + chr( int(one_) )
		
	return str_out

def Qstring2utf(str_):
	return unicode(str_).encode("utf")



#获取变量名字
def GetNameInCode(strCode):
	nIndex = 0
	nIndex_1 = 0
	nIndex_2 = 0
	for one_ in strCode:
		nIndex += 1
		if one_ in [" " , "\t"]:
			nIndex_1 = nIndex
		elif one_ == ";":
			nIndex_2 = nIndex
			break
		else:
			continue
	#
	return strCode[nIndex_1:nIndex_2]

#名字排序
def NameSort():
	str_ = '''
	3: string Signature;    //邮件署名
	4: string Title;    //邮件标题
	1: string Body;     //邮件正文
	2: list<MailItem> Itmes; //物品列表
    5: string UserID;	//玩家唯一ID 
	'''
	
	list_2 = []
	d_ = {}
	list_ = str_.split("\n")
	for one_ in list_:
		strKey = GetNameInCode(one_)
		list_2.append(strKey )
		d_[strKey] = one_
	#
	list_2.sort()
	for one_ in list_2:
		print d_[one_].decode("utf").encode("gbk")
	pass









































