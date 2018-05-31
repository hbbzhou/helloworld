
import hbb_tool

g_str_list = '''	for ([type]::const_iterator it_ = [obj].begin();
	it_ != [obj].end(); ++it_) {
	
}
'''
g_str_vec = '''	for (size_t i_ = 0; i_ < [obj].size(); ++i_) {
	const [type] & one_ = [obj][i_];

}
'''

g_str_get_set = '''void	Set[name](const [type] & v_) { [obj] = v_; }
[type]	Get[name]() const { return [obj]; }'''


g_str_if_ptr = '''if([obj] == NULL){
		USR_INFOEX(true, "##error");
		return;//to_be_fix
}'''

#组成get_set_函数
def deal_code_1(str_type , str_name , n_index):
	str_ = g_str_get_set.replace("[type]" , str_type).replace("[obj]" ,str_name )
	str_ = str_.replace("[name]" , str_name[n_index:] )
	hbb_tool.SetText(str_)


def DealMapCode(str_):
	str_name = ""
	str_map_type = ""
	n1 = 0
	for i_ in range(len(str_) ):
		if str_[i_] == "<":
			n1 += 1
		elif str_[i_] == ">":
			n1 -= 1
			if n1 == 0:
				#提前名字
				str_name = str_[i_+1 :]
				str_map_type = str_[0:i_+1]
				break

	str_new_name = ""
	for one_ in str_name:
		if one_ == ";":
			break
		elif one_ not in [" " , "\t"]:
			str_new_name += one_
		
	hbb_tool.SetText(g_str_list.replace("[obj]" ,str_new_name).replace("[type]" , str_map_type) )
	
def DealVecCode(str_):
	str_name = ""
	str_type = ""
	n1 = 0
	for i_ in range(len(str_) ):
		if str_[i_] == "<":
			n1 += 1
		elif str_[i_] == ">":
			n1 -= 1
			if n1 == 0:
				#提前名字
				str_name = str_[i_+1 :]
				str_type = str_[1:i_]
				break

	str_new_name = ""
	for one_ in str_name:
		if one_ == ";":
			break
		elif one_ not in [" " , "\t"]:
			str_new_name += one_
		
					
	str_new_type = ""
	for one_ in str_type:
		if one_ not in [" " , "\t"]:
			str_new_type += one_
			
	hbb_tool.SetText(g_str_vec.replace("[obj]" ,str_new_name).replace("[type]" , str_new_type) )

def DealCode1( str_):
	str_ = str_.replace("\r" , "").replace("\n" , "")
	str_dst = ""
	
	#去掉_头
	for i_ in range(len(str_) ):
		if str_[i_] in [" " , "\t"]:
			continue
		
		str_dst = str_[i_:]
		break
		
	if str_dst[0:11] == "std::vector":
		DealVecCode(str_dst[11:])
	elif str_dst[0:8] == "std::map":
		DealMapCode(str_dst)
	elif str_dst[0:8] == "std::set":
		DealMapCode(str_dst)


def base_deal_fun( str_code):
	str_type = ""
	if str_code[0:1] == "p":
		if str_code[1:2] == "_":
			str_type = "ptr"
		elif str_code[1:2].isupper() :
			str_type = "ptr"
	
	#处理 各个类型
	if str_type == "ptr":
		str_ = g_str_if_ptr.replace("[obj]" , str_code)
		hbb_tool.SetText(str_)
	else:
		DealCode1(str_code)
	


def sub_member(str_code):
	str_ = ""
	for one_ in  str_code.split("\n"):
		n1 = one_.rfind(" ")
		if "bool" in one_:
			str_ += one_[n1 + 1 :] + " = false;\n"
		else:
			str_ += one_[n1 + 1 :] + " = 0;\n"
			
	hbb_tool.SetText(str_[0:-1])

def decode_fun(str_code):
	if "std::" in str_code:
		base_deal_fun( str_code)
	else:
		str_code = str_code.replace(";" , "")
		str_code = str_code.replace("\r" , "")
		sub_member(str_code)




if __name__ == "__main__":
	decode_fun(hbb_tool.GetText())
	
	
