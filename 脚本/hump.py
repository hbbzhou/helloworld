
import hbb_tool

g_str_ = "huang_binbin"


#下划线转驼峰
def Change1():
	is_find = True
	new_str_ = ""
	for ch in g_str_:
		if is_find == True:
			ch = ch.upper()
		
		if ch == '_':
			is_find = True
		else:
			is_find = False
			new_str_ += ch	
	print new_str_
	return new_str_

#驼峰转下划线
def Change2():
	is_big = True
	new_str_ = ""
	for ch in g_str_:
		if ch.isupper() :
			if is_big == False:
				new_str_ += '_'
			is_big = True
		else:
			is_big = False
		
		new_str_ += ch.lower()
	print new_str_
	return new_str_			

def AutoChange():
	ch = g_str_[0:1]
	if ch.isupper() :
		return Change2()
	else:
		return Change1()
		
#g_str_ = hbb_tool.GetText()		
g_str_ = AutoChange()		
g_str_ = AutoChange()
g_str_ = AutoChange()		
