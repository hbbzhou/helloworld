
import os
import time
#快速排序
#大话数据结构447
gList= [50,60,10,50,40,30,70,80,20,90,50]
#gList= [50,60,10,40,30,70,80,20,90]
gZSpace = []	#栈空间

def swap(t1, t2):
	print "swap:", t1, t2
	return t2, t1

#拆分
def splitList(dwLow, dwHigh):
	if dwLow >= dwHigh:
		return
	#
	dwLowBak = dwLow
	dwHighBak = dwHigh
	print "begin:", gList[dwLowBak:dwHighBak+1]
	while dwLow < dwHigh:
		#基准点在dwLow
		while dwLow < dwHigh and gList[dwLow] <= gList[dwHigh]:
			dwHigh -= 1
		#
		if dwLow == dwHigh:
			break
		#
		#进行交换
		print dwLow, dwHigh
		gList[dwLow], gList[dwHigh] = swap(gList[dwLow], gList[dwHigh])
		print gList[dwLowBak:dwHighBak+1]
		#基准点在dwHigh
		while dwLow < dwHigh and gList[dwHigh] >= gList[dwLow]:
			dwLow += 1
		#
		if dwLow == dwHigh:
			break
		#
		print dwLow, dwHigh
		gList[dwLow], gList[dwHigh] = swap(gList[dwLow], gList[dwHigh])
		print gList[dwLowBak:dwHighBak+1]
	#
	#dwLow==dwHigh 成为基准点
	print "end dwLow:",dwLow, gList[dwLowBak:dwLow+1], gList[dwLow+1:dwHighBak+1]
	splitList(dwLowBak, dwLow)
	splitList(dwLow+1, dwHighBak)
#
if __name__ == "__main__":
	splitList(0, len(gList) -1)
	print gList
#

