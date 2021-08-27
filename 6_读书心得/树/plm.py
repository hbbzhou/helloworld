
import os
import time
#普利姆算法
#最小树#连接所有节点
mm = 65536
gList = [
	[mm,06,01,05,mm,mm],
	[06,mm,05,mm,03,mm],
	[01,05,mm,05,06,04],
	[05,mm,05,mm,mm,02],
	[mm,03,06,mm,mm,06],
	[mm,mm,04,02,06,mm],
]
#起点到 终点集合的 最小距离
def getMinDiff(dwBegin, listEnd):
	dwCur = mm
	for dwEnd in listEnd:
		dwTmp =gList[dwBegin][dwEnd]
		if dwTmp < dwCur:
			dwCur = dwTmp
		#
	#
	return dwCur

#最小消耗对应的B中的成员
def getBByMinCost(B, Cost):
	dwCur = mm
	dwCurIdx = 0
	print "Cost:" , Cost
	for dwIdx in range(len(Cost)):
		if Cost[dwIdx] < dwCur:
			dwCur = Cost[dwIdx]
			dwCurIdx = dwIdx
		#
	#
	dwDst = B.pop(dwCurIdx)
	print "dwDst:", dwDst
	return dwDst

if __name__ == "__main__":
	A = [0]
	B = [1,2,3,4,5]
	#如果B的集合不空,就执行循环
	while len(B) > 0:
		Cost = []
		for dwBegin in B:
			#B[x] -> A #B中成员到A集合的最小值
			Cost.append( getMinDiff(dwBegin, A) )
		#
		#找到Cost中最小值X, X对应的下标为Y, pop B[Y], 并将B[Y]加入A
		A.append(getBByMinCost(B, Cost) )
	#
	print "result:" , A
	pass
	
	