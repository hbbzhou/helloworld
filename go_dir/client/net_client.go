package client

/*
//使用说明
pClient := new(client.NetClient) //定义一个客户端
//pDealLogin := new(client.DealLogin) //设置处理消息模块
pDealGate := new(client.DealGate)        //设置处理消息模块
pClient.Run("localhost:2017", pDealGate) //连接网络
*/

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"net"
	"os"
	"strconv"
)

//整形转换成字节
func intToBytes(n int) []byte {
	x := uint16(n)

	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)
	return bytesBuffer.Bytes()
}

//字节转换成整形
func bytesToInt(b []byte) int {
	bytesBuffer := bytes.NewBuffer(b)

	var x uint16
	binary.Read(bytesBuffer, binary.BigEndian, &x)
	return int(x)
}

const _HeaderLength = 2 //2字节 描述消息长度
const _IDLength = 2     //2字节 描述消息的id

//Player 玩家数据
type Player struct {
	StrGateIP      string
	NGatePort      int32
	strAccount     string
	nCheckServerID uint64
	nUUID          uint64
	strName        string
}

//NetClient 客户端_联网
type NetClient struct {
	strName      string
	strIPAndPort string //localhost:2017
	pConn        net.Conn
	IsColse      bool
	pReaderChan  chan []byte //receive data chan
	pMapDealFun  map[int]IGameDealOne
	pDealInit    IGameDeal
	pPlayer      *Player
}

//IGameDeal 处理消息的接口
type IGameDeal interface {
	initDealFun(*NetClient)
	//dealMsgStep2(int, []byte, NetClient)
}

//IGameDealOne 处理消息的接口//把这个看成函数指针
type IGameDealOne interface {
	dealMsgStep3([]byte, *NetClient) int
}

//封包
func (p *NetClient) enpack(message []byte, msgID int) []byte {
	return append(append(intToBytes(len(message)+_IDLength), intToBytes(msgID)...), message...)
}

//解包
func (p *NetClient) depack(buffer []byte, readerChannel chan []byte) []byte {
	length := len(buffer)

	var i int
	for i = 0; i < length; {
		if length < i+_HeaderLength {
			break
		}
		msgLen := bytesToInt(buffer[i : i+_HeaderLength])
		println("消息长度:", msgLen)

		if length < i+_HeaderLength+msgLen {
			break
		}
		data := buffer[i+_HeaderLength : i+_HeaderLength+msgLen]
		readerChannel <- data
		i = i + _HeaderLength + msgLen
	}

	if i == length {
		return make([]byte, 0)
	}
	return buffer[i:]
}

//发送消息
func (p *NetClient) sendMsg(msg []byte, msgID int) (int, error) {
	return p.pConn.Write(p.enpack(msg, msgID))
}

//处理消息
func (p *NetClient) dealMsgStep1(readerChannel chan []byte, deal IGameDeal) {
	defer println("end dealMsgStep1")
	for {
		select {
		case data, ok := <-readerChannel:
			if ok == false {
				return
			}

			msgLen := len(data)
			if msgLen < _IDLength {
				println("error ")
			} else {
				msgID := bytesToInt(data[0:_HeaderLength])
				println("[receive]消息id:", msgID)
				if v, ok := p.pMapDealFun[msgID]; ok {
					v.dealMsgStep3(data[_HeaderLength:], p)
				} else {
					println("not find deal func")
				}
			}
		}
	}

	// for data := range readerChannel {
	// 	msgLen := len(data)
	// 	if msgLen < _IDLength {
	// 		println("error ")
	// 	} else {
	// 		msgID := bytesToInt(data[0:_HeaderLength])
	// 		println("[receive]消息id:", msgID)
	// 		if v, ok := p.pMapDealFun[msgID]; ok {
	// 			v.dealMsgStep3(data[_HeaderLength:], p)
	// 		} else {
	// 			println("not find deal func")
	// 		}
	// 	}
	// }

}

//Run 连接服务器
//等同于 c++模版connectServer(string, T)
//编写顺序:先定义 [类] , 接着 定义 [接口]
func (p *NetClient) Run(ip string, port int32, deal IGameDeal, pPlayer *Player) {
	p.pDealInit = deal
	p.pPlayer = pPlayer
	p.strIPAndPort = ip + ":" + strconv.Itoa(int(port))

	//初始化
	p.pMapDealFun = make(map[int]IGameDealOne)
	p.pDealInit.initDealFun(p)

	p.baseRun()

	// inputReader := bufio.NewReader(os.Stdin)
	// //如果quit就退出
	// for {
	// 	input, _ := inputReader.ReadString('\n')
	// 	trimInput := strings.Trim(input, "\r\n")
	// 	if trimInput == "quit" {
	// 		fmt.Println("再见")
	// 		time.Sleep(1 * time.Second)
	// 		return
	// 	}

	// 	p.sendMsg([]byte(trimInput), 10)
	// }
}

func (p *NetClient) baseRun() {
	//连接
	var err error
	p.pConn, err = net.Dial("tcp", p.strIPAndPort)
	p.checkError(err)
	fmt.Println(p.strName, " 连接成功!")

	p.IsColse = false
	p.pReaderChan = make(chan []byte, 1024)
	//处理消息
	go p.dealMsgStep1(p.pReaderChan, p.pDealInit)
	//接收消息
	go p.readAll(p.pConn, p.pReaderChan)
}

//Rerun zan wu
func (p *NetClient) Rerun() {
	if p.IsColse == true {
		p.baseRun()
	}
}

//Close zan wu
func (p *NetClient) Close() {
	p.baseClose(p.pConn, p.pReaderChan)
}

func (p *NetClient) baseClose(conn net.Conn, readerChannel chan []byte) {
	if p.IsColse == false {
		p.IsColse = true
		conn.Close()
		close(readerChannel)
	}
}

//
func (p *NetClient) readAll(conn net.Conn, readerChannel chan []byte) {

	// 缓冲区，存储被截断的数据
	defer p.baseClose(conn, readerChannel)
	pTmpBuffer := make([]byte, 0)
	pBuffer := make([]byte, 1024)
	for {
		n, err := conn.Read(pBuffer)
		if err != nil {
			fmt.Println(conn.RemoteAddr().String(), " connection error: ", err)
			return
		}

		pTmpBuffer = p.depack(append(pTmpBuffer, pBuffer[:n]...), readerChannel)
	}
}

func (p *NetClient) checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s Fatal error: %s", p.strName, err.Error())
		os.Exit(1)
	}
}
