
import hbb_tool

g_str_list = '''	for ([type]::const_iterator it_ = [obj].begin();
	it_ != [obj].end(); ++it_) {
	
}
'''
g_str_vec = '''	for (size_t i_ = 0; i_ < [obj].size(); ++i_) {
	const class_name & one_ = [obj][i_];

}
'''

g_str_get_set = '''void Set[name](const [type] & v_) { [obj] = v_; }
[type] Get[name]() const { return [obj]; }'''

def for_fun( str_code):
	n_1 = str_code.rfind(" ")
	if n_1 >= 0:
		str_name = str_code[n_1+1:]
		str_type = str_code[0:n_1]
		str_type = str_type.replace(" " , "")
		if str_name[0:3] in ["m_n" , "m_f" , "m_b"]:
			str_ = g_str_get_set.replace("[type]" , str_type).replace("[obj]" ,str_name )
			str_ = str_.replace("[name]" , str_name[3:] )
			hbb_tool.SetText(str_)
		elif str_name[0:5] in ["m_str" ]:
			str_ = g_str_get_set.replace("[type]" , str_type).replace("[obj]" ,str_name )
			str_ = str_.replace("[name]" , str_name[5:] )
			hbb_tool.SetText(str_)
		else:
			str_ = g_str_list.replace("[type]" , str_type).replace("[obj]" ,str_name )
			hbb_tool.SetText(str_)
	else:
		#处理vector
		if "vec_" in str_code or "m_vec" in str_code:
			str_name = str_code
			str_ = g_str_vec.replace("[obj]" ,str_name )
			hbb_tool.SetText(str_)

def sub_member(str_code):
	str_ = ""
	for one_ in  str_code.split("\n"):
		n1 = one_.rfind(" ")
		if "bool" in one_:
			str_ += one_[n1 + 1 :] + " = false;\n"
		else:
			str_ += one_[n1 + 1 :] + " = 0;\n"
			
	hbb_tool.SetText(str_[0:-1])

def decode_fun():
	str_code = hbb_tool.GetText()
	str_code = str_code.replace(";" , "")
	str_code = str_code.replace("\r" , "")
	if "\n" in str_code:
		sub_member(str_code)
	else:
		for_fun( str_code)
		
decode_fun()