package client

//DealGate 处理网关协议
type DealGate struct {
}

//处理消息
func (p *DealGate) dealMsg(readerChannel chan []byte, oClient NetClient) {
	for {
		select {
		case data := <-readerChannel:
			msgLen := len(data)
			if msgLen < _IDLength {
				println("error ")
			} else {
				msgID := bytesToInt(data[0:_HeaderLength])
				println("[gate]消息id:", msgID, "内容:", string(data[_HeaderLength:]))
				//oClient.sendMsg([]byte(" deal msg ok\n "))
			}
		}
	}
}
