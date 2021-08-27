
import os
import time
#迪杰斯特拉
#起点到终点的最小路径
#参考书本<<大话数据结构>> 285页
m = 65536
gWeight = [
	#v0,v1,v2,v3,v4,v5,v6,v7,v8,
	[ 0, 1, 5, m, m, m, m, m, m],
	[ 1, 0, 3, 7, 5, m, m, m, m],
    [ 5, 3, 0, m, 1, 7, m, m, m],
    [ m, 7, m, 0, 2, m, 3, m, m],
    [ m, 5, 1, 2, 0, 3, 6, 9, m],
    [ m, m, 7, m, 3, 0, m, 5, m],
    [ m, m, m, 3, 6, m, 0, 2, 7],
    [ m, m, m, m, 9, 5, 2, 0, 4],
    [ m, m, m, m, m, m, 7, 4, 0],
]

#v0到各个节点最短距离
gDiff = [0,m,m,m,m,m,m,m,m]

#最优路线,各个节点的前置节点
gPreNode = [0,m,m,m,m,m,m,m,m]

#需要以xx节点为起点,重新刷新关联节点的路径值
gNeedReCal = [False,False,False,False,False,False,False,False,False]
gNeedReCalNodeID = []

#新增需要重新计算的节点
def addReCal(dwNodeIdx):
	if gNeedReCal[dwNodeIdx] == False:
		gNeedReCal[dwNodeIdx] = True
		gNeedReCalNodeID.append(dwNodeIdx)

#以xx节点为起点,刷新关联节点的路径值
def doNextStep(dwBegin):
	dwBase = gDiff[dwBegin]
	for y in range(0,9):
		dwWeight = gWeight[dwBegin][y] + dwBase
		if gDiff[y] > dwWeight:
			gDiff[y] = dwWeight	#更新v0-Vn最小路径
			gPreNode[y] = dwBegin		#更新前置节点
			addReCal(y)
		#
	#

#
if __name__ == "__main__":
	#v0-vN
	addReCal(0)
	while len(gNeedReCalNodeID) > 0:
		x = gNeedReCalNodeID.pop()
		gNeedReCal[x] = False
		doNextStep(x)
	#
	print "gDiff:", gDiff
	print "gPreNode:",gPreNode
	curNode = 8
	listNode = [8]
	for i in range(0,9):
		curNode = gPreNode[curNode]
		listNode.append(curNode)
	#
	print "listNode;", listNode
#

