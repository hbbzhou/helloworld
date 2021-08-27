
import os
import time
#克鲁斯卡尔算法
#最小树#连接所有节点
#0-5个节点,它们连接路径值如下

gWeight = [
	#节点A,节点B , 路径值
	[0,		1, 		6],
	[0,		2, 		1],
    [0,		3, 		5],
    [1,		2, 		5],
    [1,		4, 		3],
    [2,		3, 		5],
    [2,		4, 		6],
    [2,		5, 		4],
    [3,		5, 		2],
    [4,		5, 		6],
]
#升序
def takeKey(elem):
    return elem[2]
#
gWeight.sort(key=takeKey, reverse=False)

#集合
gSpace=[]
#各个节点的状态, -1代表未加载, 反之就是对应集合下标
gNode= [-1,-1,-1,-1,-1,-1]
gSelect = []
#新建一个集合ID
def newSpace():
	dwLen = len(gSpace) 
	gSpace.append(dwLen + 1)
	return dwLen

#建立链接
def doLink(dwNodeA, dwNodeB):
	dwIdxA = gNode[dwNodeA]
	dwIdxB = gNode[dwNodeB]
	if dwIdxA == -1 and dwIdxA == dwIdxB:	#A,B都未加载
		dwSpaceIdx = newSpace()
		#加载A,B 并设置为同集合
		gNode[dwNodeA] = dwSpaceIdx
		gNode[dwNodeB] = dwSpaceIdx
		print "add Node", dwNodeA, dwNodeB
		return True
	elif dwIdxA > -1 and dwIdxB > -1 :	#A,B都加载
		if gSpace[dwIdxA] == gSpace[dwIdxB]:	#A,B 属于同集合
			print "##ignore Node", dwNodeA, dwNodeB
			return False
		else:	#不属于同集合
			#合并集合
			gSpace[dwIdxA] = gSpace[dwIdxB]
			print "merge", dwNodeA, dwNodeB
			return True
	else :	#其中有一个未加载
		if dwIdxA == -1:
			gNode[dwNodeA] = dwIdxB
			print "add Node", dwNodeA
			return True
		else:
			gNode[dwNodeB] = dwIdxA
			print "add Node", dwNodeB
			return True
#
if __name__ == "__main__":
	for one in gWeight:
		dwNodeA = one[0]
		dwNodeB = one[1]
		if doLink(dwNodeA, dwNodeB) :
			gSelect.append(one)
		#
	#
	print gSelect
#

