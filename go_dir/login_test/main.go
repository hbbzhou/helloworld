package main

import (
	"client"
	"fmt"
	"os"
	"strconv"
	"time"
)

var gLoginTimes int32

func testOne(nLoginTimes int32, strName string) {
	pPlayer := new(client.Player)
	pPlayer.StrGateIP = "192.168.1.119"
	pPlayer.StrAcountName = strName
	pDealLogin := new(client.DealLogin) //设置处理消息模块
	pDealGate := new(client.DealGate)   //设置处理消息模块
	var pClient *client.NetClient
	var pGateClient *client.NetClient

	var nCurLoginTimes int32
	for nCurLoginTimes < nLoginTimes {
		time.Sleep(1 * time.Millisecond)

		if pPlayer.NStep == client.E_None {
			pPlayer.NStep = client.E_Waiting

			pClient = new(client.NetClient)                         //定义一个客户端
			pClient.Run("192.168.1.119", 8555, pDealLogin, pPlayer) //连接网络

			client.SendLoginReq(pClient)

		} else if pPlayer.NStep == client.E_ToConnectGate {
			pPlayer.NStep = client.E_Waiting
			pClient.Close()

			pGateClient = new(client.NetClient)                                       //定义一个客户端
			pGateClient.Run(pPlayer.StrGateIP, pPlayer.NGatePort, pDealGate, pPlayer) //连接网络
			client.SendRoleCheckReq(pGateClient)

		} else if pPlayer.NStep == client.E_LoginOk {
			pPlayer.NStep = client.E_None
			pGateClient.Close()
			nCurLoginTimes++
			gLoginTimes++
		}
	}
}

func waite(strShow string) {
	strShow = "[输入回车]" + strShow
	println(strShow)
	buff := make([]byte, 128)
	os.Stdin.Read(buff)
}

func main() {

	nLoginTimes := 1
	nTotalPlayer := 1000
	nBeginAcount := 1
	println("输入第一个帐号(数字):")

	fmt.Scanf("%d\n", &nBeginAcount)
	println("帐号范围:", nBeginAcount, "-", nBeginAcount+nTotalPlayer)
	waite("确认!!")

	t1 := time.Now().UnixNano()
	for i := 0; i < nTotalPlayer; i++ {
		strName := "iaf" + strconv.Itoa(i+nBeginAcount)
		go testOne(int32(nLoginTimes), strName)
	}

	n1 := 0
	for gLoginTimes != int32(nLoginTimes*nTotalPlayer) {
		time.Sleep(100 * time.Millisecond)
		n1++
		if n1%10 == 0 {
			println("当前登录次数:", gLoginTimes)
		}
	}
	ndiff := (time.Now().UnixNano() - t1) / int64(time.Millisecond)
	println("耗时:", ndiff/1000, "s", ndiff%1000, "ms")
	println("[end...]  ", gLoginTimes)

	waite("退出!!")
}
