
import os
import time
#中序遍历
#相关资料
#完美二叉树.png

#Node=[value,L,R]
m = 65536
gList = [
[30, 1, 2],	#0
[10, 3, 4],	#1
[50, 5, 6],	#2
[5, 7, 8],	#3
[20, 9,10],	#4
[40,11,12],	#5
[60,13,14],	#6
[4, m, m],	#7
[6, m, m],	#8
[19, m, m],	#9
[21, m, m],	#10
[39, m, m],	#11
[41, m, m],	#12
[59, m, m],	#13
[61, m, m],	#14
]
gSelect=[]
gZSpace=[m]	#栈排序, m为哨兵

def zType():	#栈排序
	curNodeIdx = 0
	bIsZSpaceValue = False	#是否为gZSpace中的数据
	while curNodeIdx != m:
		print curNodeIdx
		dwTmpL = gList[curNodeIdx][1]
		if bIsZSpaceValue: 
			bIsZSpaceValue = False
			dwTmpL = m	#栈空间的节点,对应的L都已经处理过了,故设置为m
		#
		if dwTmpL != m :	#存在L ,就入栈,并执行L
			gZSpace.append(curNodeIdx)
			curNodeIdx = dwTmpL
			continue
		else:	#没有L
			gSelect.append(gList[curNodeIdx][0])	#输出value
			if gList[curNodeIdx][2] != m: #存在R, 就执行R
				curNodeIdx = gList[curNodeIdx][2]
				continue
			else:	#不存在R, 就从栈中获取一个节点
				curNodeIdx = gZSpace.pop(-1)
				bIsZSpaceValue = True
		#
	#
#

#递归
def dType(curNodeIdx):
	print curNodeIdx
	if gList[curNodeIdx][1] != m :	#存在L,就递归
		dType(gList[curNodeIdx][1])
	#
	gSelect.append(gList[curNodeIdx][0])	#输出value
	if gList[curNodeIdx][2] != m:	#存在R ,就递归
		dType(gList[curNodeIdx][2])
	#
#
if __name__ == "__main__":
	#zType()
	dType(0)
	print "gSelect:", gSelect
#

