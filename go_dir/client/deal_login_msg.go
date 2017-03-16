package client

//DealLogin 处理xx协议
type DealLogin struct {
}

func (p *DealLogin) initDealFun(oClient NetClient) {
	oClient.mapDealFun[12] = new(msg1)
}

//请在initDealFun()中 进行注册
type msg2 struct {
}

//处理消息
func (p *msg2) dealMsgStep3(msgContent []byte, oClient NetClient) {
	//oClient.sendMsg([]byte(" deal msg ok\n "))
	println("处理完成")
}
