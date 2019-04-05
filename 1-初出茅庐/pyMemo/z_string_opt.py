



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












































