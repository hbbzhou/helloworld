
import os
import time
#树查找
#相关资料
#完美二叉树.png
#Node=[value,L,R]
m = 65536
gList = [
[8, 1, 2],	#0
[4, 3, 4],	#1
[12, 5, 6],	#2
[2, 7, 8],	#3
[6, 9,10],	#4
[10,11,12],	#5
[14,13,14],	#6
[1, m, m],	#7
[3, m, m],	#8
[5, m, m],	#9
[7, m, m],	#10
[9, m, m],	#11
[11, m, m],	#12
[13, m, m],	#13
[15, m, m],	#14
]
gRootIdx = 0
def delNode(dwDst):
	print "will del:", dwDst
	dwRet, dwCurIdx, dwPreIdx,dwPreSubFlag = findData(dwDst)
	if dwRet == False:
		print "del err"
		return
	#
	global gRootIdx
	if gList[dwCurIdx][1] == m and gList[dwCurIdx][2] == m:
		if dwCurIdx == gRootIdx: 
			#没有,L,R 直接删除根节点
			gList[dwCurIdx] = [m,m,m]
		else:
			#没有,L,R 解除关联,删除当前节点
			gList[dwPreIdx][dwPreSubFlag] = m
			gList[dwCurIdx] = [m,m,m]
		
		print "delNode logic [not L,R] , dwCurIdx:" , dwCurIdx
	#
	elif gList[dwCurIdx][1] == m: #L不存在,R存在.
		#更换根节点 并删除当前节点
		if dwCurIdx == gRootIdx: 
			gRootIdx = gList[dwCurIdx][2]
			gList[dwCurIdx] = [m,m,m]
			print "delNode logic [change rootNode][not L], dwCurIdx:" , dwCurIdx
		else:
			#上移R 并删除当前节点
			gList[dwPreIdx][dwPreSubFlag] = gList[dwCurIdx][2]
			gList[dwCurIdx] = [m,m,m]
			print "delNode logic [not L], dwCurIdx:" , dwCurIdx
		#
	elif gList[dwCurIdx][2] == m: #L存在,R不存在.
		#更换根节点 并删除当前节点
		if dwCurIdx == gRootIdx: 
			gRootIdx = gList[dwCurIdx][1]
			gList[dwCurIdx] = [m,m,m]
			print "delNode logic [change rootNode][not R], dwCurIdx:" , dwCurIdx
		else:
			#上移L 并删除当前节点
			gList[dwPreIdx][dwPreSubFlag] = gList[dwCurIdx][1]
			gList[dwCurIdx] = [m,m,m]
			print "delNode logic [not R], dwCurIdx:" , dwCurIdx
		#
	else:
		#从左边选出X 替代当前节点,并删除当前节点
		dwSelectIdx = popR(dwCurIdx)
		gList[dwSelectIdx][1] = gList[dwCurIdx][1]
		gList[dwSelectIdx][2] = gList[dwCurIdx][2]
		gList[dwCurIdx] = [m,m,m]
		
		if dwCurIdx == gRootIdx: #更换根节点
			gRootIdx = dwSelectIdx
			print "delNode logic [change rootNode], dwCurIdx:" , dwCurIdx
		else:
			#更换关联
			gList[dwPreIdx][dwPreSubFlag] = dwSelectIdx
			print "delNode logic, dwCurIdx:" , dwCurIdx
		#
	#
#
#弹出指定节点->L节点-> R节点->...直到最后一个R节点
def popR(dwPreIdx):
	dwPreSubFlag = 1
	dwCurIdx = gList[dwPreIdx][dwPreSubFlag]
	if dwCurIdx == m:
		return None
	#
	while dwCurIdx != m :
		print "popR logic " , gList[dwCurIdx]
		if gList[dwCurIdx][2] != m:	#R存在
			dwPreIdx = dwCurIdx
			dwPreSubFlag = 2
			dwCurIdx = gList[dwCurIdx][2]
			continue
		#
		if gList[dwCurIdx][1] != m :	#L存在 R不存在
			#将L上移
			gList[dwPreIdx][dwPreSubFlag] = gList[dwCurIdx][1]
			#解除当前节点的关联
			gList[dwCurIdx][1] = m
			#返回当前节点
			return dwCurIdx
		else:	#L, R不存在
			#解除上个节点的关联
			gList[dwPreIdx][dwPreSubFlag] = m
			#返回当前节点
			return dwCurIdx
		#
	#
	return None
#

def findData(dwDst):
	dwCurIdx = gRootIdx
	dwPreIdx = dwCurIdx
	dwPreSubFlag = 0	#接连上个节点的标识
	while dwCurIdx != m :
		print "find logic, Idx:" , dwCurIdx
		if gList[dwCurIdx][0] == dwDst:
			print "ok"
			return True, dwCurIdx, dwPreIdx, dwPreSubFlag
		#
		dwPreIdx = dwCurIdx
		if gList[dwCurIdx][0] < dwDst:
			dwPreSubFlag = 2
			dwCurIdx = gList[dwCurIdx][2]
		else:
			dwPreSubFlag = 1
			dwCurIdx = gList[dwCurIdx][1]
		#
	#
	print "not found"
	return False, None, None , None
#

def showTree(curNodeIdx, listShow, list2):
	if curNodeIdx == m or len(list2) > 20:
		print "--end--"
		return
	#
	list2.append(curNodeIdx)
	print curNodeIdx,gList[curNodeIdx]
	if gList[curNodeIdx][1] != m :	#存在L,就递归
		showTree(gList[curNodeIdx][1], listShow, list2)
	#
	listShow.append(gList[curNodeIdx][0])
	if gList[curNodeIdx][2] != m:	#存在R ,就递归
		showTree(gList[curNodeIdx][2], listShow, list2)
	#
#
def showAll():
	print "rootNode:", gRootIdx, gList[gRootIdx]
	list1 = []
	list2 = []
	showTree(gRootIdx, list1, list2)
	print "showNodeIdx:", list2
	print "showValue:", list1
#
if __name__ == "__main__":
	for one in [100,8,15,4,9,2,13,3,6,10,11,5,12,1,7,14]:
		delNode(one)
		showAll()
		os.system("pause")
	#
#

