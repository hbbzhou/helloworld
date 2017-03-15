package client

//DealLogin 处理登录协议
type DealLogin struct {
}

//处理消息
func (p *DealLogin) dealMsg(readerChannel chan []byte, oClient NetClient) {

	for {
		select {
		case data := <-readerChannel:
			msgLen := len(data)
			if msgLen < _IDLength {
				println("error ")
			} else {
				msgID := bytesToInt(data[0:_HeaderLength])
				println("[login]消息id:", msgID, "内容:", string(data[_HeaderLength:]))
				//oClient.sendMsg([]byte(" deal msg ok\n "))
			}
		}
	}
}

func initDealFun() {
	mapDealFun := make(map[int]iGameDealOne)
	mapDealFun[1] = new(msg1)
}

//IGameDeal 处理消息的接口//把这个看成函数指针
type iGameDealOne interface {
	dealOneMsg(msgContent string, oClient NetClient)
}

type msg1 struct {
}

//处理消息
func (p *msg1) dealOneMsg(msgContent string, oClient NetClient) {
	//oClient.sendMsg([]byte(" deal msg ok\n "))
}
