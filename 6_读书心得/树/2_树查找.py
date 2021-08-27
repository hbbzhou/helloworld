
import os
import time
#树查找
#相关资料
#完美二叉树.png

#Node=[value,L,R]
m = 65536
gList = [
[30, 1, 2],	#0
[10, 3, 4],	#1
[50, 5, 6],	#2
[0, m, m],	#3
[20, m, m],	#4
[40, m, m],	#5
[60, m, m],	#6
]

def findData():
	dwDst = 31
	dwCurIdx = 0
	while dwCurIdx != m :
		print dwCurIdx
		if gList[dwCurIdx][0] == dwDst:
			print "ok"
			return True
		elif gList[dwCurIdx][0] < dwDst:
			dwCurIdx = gList[dwCurIdx][2]
		else:
			dwCurIdx = gList[dwCurIdx][1]
		#
	#
	print "not found"
	return False
#

if __name__ == "__main__":
	findData()
#

