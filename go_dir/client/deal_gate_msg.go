package client

//DealGate 处理xx协议
type DealGate struct {
}

func (p *DealGate) initDealFun(oClient NetClient) {
	oClient.mapDealFun[12] = new(msg1)
}

//请在initDealFun()中 进行注册
type msg1 struct {
}

//处理消息
func (p *msg1) dealMsgStep3(msgContent []byte, oClient NetClient) {
	println("处理完成")
	//oClient.sendMsg([]byte(" deal msg ok\n "))
}
