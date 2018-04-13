
import win32api
import win32con
import win32gui
import os
from ctypes import *
import time
VK_CODE = {
 'backspace':0x08,
 'tab':0x09,
 'clear':0x0C,
 'enter':0x0D,
 'shift':0x10,
 'ctrl':0x11,
 'alt':0x12,
 'pause':0x13,
 'caps_lock':0x14,
 'esc':0x1B,
 'spacebar':0x20,
 'page_up':0x21,
 'page_down':0x22,
 'end':0x23,
 'home':0x24,
 'left_arrow':0x25,
 'up_arrow':0x26,
 'right_arrow':0x27,
 'down_arrow':0x28,
 'select':0x29,
 'print':0x2A,
 'execute':0x2B,
 'print_screen':0x2C,
 'ins':0x2D,
 'del':0x2E,
 'help':0x2F,
 '0':0x30,
 '1':0x31,
 '2':0x32,
 '3':0x33,
 '4':0x34,
 '5':0x35,
 '6':0x36,
 '7':0x37,
 '8':0x38,
 '9':0x39,
 'a':0x41,
 'b':0x42,
 'c':0x43,
 'd':0x44,
 'e':0x45,
 'f':0x46,
 'g':0x47,
 'h':0x48,
 'i':0x49,
 'j':0x4A,
 'k':0x4B,
 'l':0x4C,
 'm':0x4D,
 'n':0x4E,
 'o':0x4F,
 'p':0x50,
 'q':0x51,
 'r':0x52,
 's':0x53,
 't':0x54,
 'u':0x55,
 'v':0x56,
 'w':0x57,
 'x':0x58,
 'y':0x59,
 'z':0x5A,
 'numpad_0':0x60,
 'numpad_1':0x61,
 'numpad_2':0x62,
 'numpad_3':0x63,
 'numpad_4':0x64,
 'numpad_5':0x65,
 'numpad_6':0x66,
 'numpad_7':0x67,
 'numpad_8':0x68,
 'numpad_9':0x69,
 'multiply_key':0x6A,
 'add_key':0x6B,
 'separator_key':0x6C,
 'subtract_key':0x6D,
 'decimal_key':0x6E,
 'divide_key':0x6F,
 'F1':0x70,
 'F2':0x71,
 'F3':0x72,
 'F4':0x73,
 'F5':0x74,
 'F6':0x75,
 'F7':0x76,
 'F8':0x77,
 'F9':0x78,
 'F10':0x79,
 'F11':0x7A,
 'F12':0x7B,
 'F13':0x7C,
 'F14':0x7D,
 'F15':0x7E,
 'F16':0x7F,
 'F17':0x80,
 'F18':0x81,
 'F19':0x82,
 'F20':0x83,
 'F21':0x84,
 'F22':0x85,
 'F23':0x86,
 'F24':0x87,
 'num_lock':0x90,
 'scroll_lock':0x91,
 'left_shift':0xA0,
 'right_shift ':0xA1,
 'left_control':0xA2,
 'right_control':0xA3,
 'left_menu':0xA4,
 'right_menu':0xA5,
 'browser_back':0xA6,
 'browser_forward':0xA7,
 'browser_refresh':0xA8,
 'browser_stop':0xA9,
 'browser_search':0xAA,
 'browser_favorites':0xAB,
 'browser_start_and_home':0xAC,
 'volume_mute':0xAD,
 'volume_Down':0xAE,
 'volume_up':0xAF,
 'next_track':0xB0,
 'previous_track':0xB1,
 'stop_media':0xB2,
 'play/pause_media':0xB3,
 'start_mail':0xB4,
 'select_media':0xB5,
 'start_application_1':0xB6,
 'start_application_2':0xB7,
 'attn_key':0xF6,
 'crsel_key':0xF7,
 'exsel_key':0xF8,
 'play_key':0xFA,
 'zoom_key':0xFB,
 'clear_key':0xFE,
 '+':0xBB,
 ',':0xBC,
 '-':0xBD,
 '.':0xBE,
 '/':0xBF,
 '`':0xC0,
 ';':0xBA,
 '[':0xDB,
 '\\':0xDC,
 ']':0xDD,
 "'":0xDE,
 '`':0xC0}
class POINT(Structure):
	_fields_ = [("x", c_ulong),("y", c_ulong)]

def key_input(str=''):
	for c in str:
		win32api.keybd_event(VK_CODE[c],0,0,0)
		win32api.keybd_event(VK_CODE[c],0,win32con.KEYEVENTF_KEYUP,0)
		time.sleep(0.01)

def key_input_enter():
	win32api.keybd_event(VK_CODE["enter"],0,0,0)
	win32api.keybd_event(VK_CODE["enter"],0,win32con.KEYEVENTF_KEYUP,0)
	
	
#参考连接
#https://www.cnblogs.com/eatPython/p/5958850.html

#鼠标点击
def clickLeftCur():
	win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN|win32con.MOUSEEVENTF_LEFTUP, 0, 0)

#鼠标移动
def moveCurPos(x,y):
	windll.user32.SetCursorPos(x, y)
	
#获取鼠标
def getCurPos():
	return win32gui.GetCursorPos()
	
#获取xx进程的句柄
def getWinHandle():
	wdname1=u"BlueStacks App Player"	#名字
	w1hd=win32gui.FindWindow(0,wdname1)
	print w1hd
	return w1hd
	
#获取鼠标_移动轨迹
def getPosList():
	while True:
		time.sleep(1)
		pos=getCurPos()
		print pos
		

class mouseOpt:
	def __init__(self , x , y , nTime , str_des):
		self.x = x
		self.y = y
		self.time = nTime
		self.des = str_des
	

def waitFor(nTime):
	n_1 = 5
	
	while nTime > n_1:
		nTime -= n_1
		time.sleep(n_1)
		print '.',
	
	if nTime > 0:
		time.sleep(nTime)
		
	print(".")
		

is_first_set = True
g_time = 0 #如果 机器慢 就等xx秒
#f_change_ratio 屏幕的缩小系数

def dealOneEvent(one_):
	global is_first_set
	f_change_ratio = 1 #缩小系数
	n_new_x = 2098
	n_new_y = 68
	n_init_x = 218
	n_init_y = 57
	n_w = int(1612 * f_change_ratio)
	n_h = int(992 * f_change_ratio)
	
	n_diff_x = n_new_x - n_init_x
	n_diff_y = n_new_y - n_init_y
	nID=getWinHandle()
	if is_first_set == True:
		is_first_set = False
		#win32gui.SetWindowPos(nID, win32con.HWND_TOPMOST, n_new_x, n_new_y ,0,0,  win32con.SWP_NOZORDER|win32con.SWP_NOOWNERZORDER|win32con.SWP_NOSIZE)
		win32gui.SetWindowPos(nID, win32con.HWND_TOPMOST, n_new_x, n_new_y ,n_w,n_h,  win32con.SWP_NOZORDER|win32con.SWP_NOOWNERZORDER)
		time.sleep(1)
		
	win32gui.SetForegroundWindow(nID)
	#print win32gui.GetWindowRect(nID) #获取 左上角坐标 和 右下角坐标
	
	old_x_y = getCurPos()
	
	#time.sleep(0.2)
	dst_x = int( (one_.x + n_diff_x)*f_change_ratio )
	dst_y = int( (one_.y + n_diff_y)*f_change_ratio )
	moveCurPos( dst_x , dst_y )
	print one_.x, one_.y
	#time.sleep(0.2)
	clickLeftCur()
	
	#time.sleep(0.2)
	#moveCurPos(old_x_y[0], old_x_y[1] ) #还原坐标
	
	print one_.des.decode("utf").encode("gbk")
		
def dealEvent(list_opt ):
	global g_time
	for one_ in list_opt:
		dealOneEvent(one_)
		waitFor(one_.time +g_time)
		
#公会结界突破
def runGHJieJ():
	n_cur_num = 2	#现有次数
	
	i_ = 0
	run_min = 60 #xx 分钟
	n_end_time = int(time.time() + (run_min * 60) )
	while time.time() < n_end_time:
		i_ += 1
		print i_
		list_opt = []
		
		if i_ % 2 == 1:
			#1
			list_opt.append(mouseOpt(1048, 443 , 1 , "选择对手" ) )
			list_opt.append(mouseOpt(1128, 688 , 50 , "开始挑战" ) )
			list_opt.append(mouseOpt(743, 593 , 5 , "结束挑战" ) )
			list_opt.append(mouseOpt(743, 593 , 2 , "结束挑战" ) )
			list_opt.append(mouseOpt(743, 593 , 2 , "结束挑战" ) )
		else:
			#2
			#list_opt.append(mouseOpt(1130, 633 , 1 , "选择对手" ) )
			#list_opt.append(mouseOpt(1147, 878 , 50 , "开始挑战" ) )
			#list_opt.append(mouseOpt(743, 593 , 5 , "结束挑战" ) )
			#list_opt.append(mouseOpt(743, 593 , 2 , "结束挑战" ) )
			#list_opt.append(mouseOpt(743, 593 , 1 , "结束挑战" ) )
			

			list_opt.append(mouseOpt(1048, 823 , 1 , "选择对手" ) )
			list_opt.append(mouseOpt(1128, 660 , 50 , "开始挑战" ) )
			list_opt.append(mouseOpt(743, 593 , 5 , "结束挑战" ) )
			list_opt.append(mouseOpt(743, 593 , 2 , "结束挑战" ) )
			list_opt.append(mouseOpt(743, 593 , 2 , "结束挑战" ) )
		#
		dealEvent(list_opt)
		if i_ > n_cur_num:
			waitFor(60)
	
	
#结界突破
def runJieJ():
	 
	list_opt = []
	
	if 1: #第1个对手
		list_opt.append(mouseOpt(619, 308 , 1 , "选择对手" ) )
		list_opt.append(mouseOpt(724, 527 , 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
	if 1: #第2个对手
		list_opt.append(mouseOpt(1054, 298 , 1 , "选择对手" ) )
		list_opt.append(mouseOpt(1137, 528 , 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 1 , "结束挑战" ) )
	if 1: #第3个对手
		list_opt.append(mouseOpt(1470, 299, 1 , "选择对手" ) )
		list_opt.append(mouseOpt(1563, 526 , 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 1 , "结束挑战" ) )
	if 1: #第4个对手
		list_opt.append(mouseOpt(593, 455 , 1 , "选择对手" ) )
		list_opt.append(mouseOpt(712, 692, 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 1 , "结束挑战" ) )
	if 1: #第5个对手
		list_opt.append(mouseOpt(1037, 461 , 1 , "选择对手" ) )
		list_opt.append(mouseOpt(1120, 686, 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 1 , "结束挑战" ) )
	if 1: #第6个对手
		list_opt.append(mouseOpt(1470, 461 , 1 , "选择对手" ) )
		list_opt.append(mouseOpt(1547, 699, 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 1 , "结束挑战" ) )
	if 1: #第7个对手
		list_opt.append(mouseOpt(595, 634 , 1 , "选择对手" ) )
		list_opt.append(mouseOpt(718, 867, 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 1 , "结束挑战" ) )
	if 1: #第8个对手
		list_opt.append(mouseOpt(1027, 632 , 1 , "选择对手" ) )
		list_opt.append(mouseOpt(1133, 860, 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 1 , "结束挑战" ) )
	if 1: #第9个对手
		list_opt.append(mouseOpt(1480, 624, 1 , "选择对手" ) )
		list_opt.append(mouseOpt(1539, 859, 80 , "开始挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 5 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 2 , "结束挑战" ) )
		list_opt.append(mouseOpt(1260, 760 , 1 , "结束挑战" ) )
	#
	#
	dealEvent(list_opt)
	
#御魂副本
def runYuHun():
	list_opt = []
	list_opt.append(mouseOpt(1415,728 , 80 , "开始挑战" ) )
	list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
	list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
	list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
	#
	dealEvent(list_opt)
	
#觉醒副本
def runJueXing():
	run_min = 60 #xx 分钟
	n_end_time = int(time.time() + (run_min * 60) )
	while time.time() < n_end_time:
		list_opt = []
		list_opt.append(mouseOpt(1415,728 , 35 , "开始挑战" ) )
		list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
		list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
		list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
		#
		dealEvent(list_opt)
	
#原业火
def runYuanYeHuo():
	
	run_min = 60 #xx 分钟
	n_end_time = int(time.time() + (run_min * 60) )
	while time.time() < n_end_time:
		list_opt = []
		list_opt.append(mouseOpt(1415,728 , 60*4 , "开始挑战" ) )
		list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
		list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
		list_opt.append(mouseOpt(1415,728 , 3 , "结束挑战" ) )
		#
		dealEvent(list_opt)
	
def allRunYuHun():
	run_min = 60 #xx 分钟
	n_end_time = int(time.time() + (run_min * 60) )
	while time.time() < n_end_time:
		runYuHun()


if __name__ == "__main__":
	print getCurPos()#获取坐标
	if 1:
		allRunYuHun()# #御魂副本
		#runJieJ() #
		#runGHJieJ() ##公会结界突破
		#runJueXing() ##觉醒副本
		#runYuanYeHuo() #原业火
