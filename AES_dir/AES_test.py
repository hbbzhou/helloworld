



def aes_encrypt(str_key , str_): 
	from Crypto.Cipher import AES
	obj = AES.new(str_key , AES.MODE_ECB)
	ciphertext = obj.encrypt(str_)
	result = base64.b64encode(ciphertext)  #base64 encode
	return result 
	
def aes_decode(str_key , str_base64):
	from Crypto.Cipher import AES
	str_data = base64.b64decode(str_base64)
	obj2 = AES.new(str_key , AES.MODE_ECB)
	return  obj2.decrypt(str_data)
	
def test_AES():
	str_key = 'hbbhehedadatttts'
	str_msg = '1234567890123456'
	str_ = aes_encrypt(str_key , str_msg)
	str_2 = aes_decode(str_key ,str_)
	print 'python'
	print "源数据\t\t:".decode("utf").encode("gbk"),str_msg
	print "加密数据\t:".decode("utf").encode("gbk"),str_
	print "解密数据\t:".decode("utf").encode("gbk"),str_2
	
	
def test_CRC():
	from zlib import crc32
	str_ = "womemjintianqushangxuezhou"
	ret_ = crc32(str_)
	print 'python'
	print "源数据\t\t:".decode("utf").encode("gbk"),str_
	print "校验值\t\t:".decode("utf").encode("gbk"),(ret_ & 0xFFFFFFFF )