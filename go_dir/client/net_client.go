package client

/*
//使用说明
oClient := new(client.NetClient)	//定义一个客户端
oDealLogin := new(client.DealLogin)	//设置处理消息模块
oDealGate := new(client.DealGate)	//设置处理消息模块
oClient.Run("localhost:2017", oDealGate)	//连接网络
*/

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"fmt"
	"net"
	"os"
	"strings"
	"time"
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

const _HeaderLength = 2
const _IDLength = 2

//NetClient 客户端_联网
type NetClient struct {
	strName      string
	strIPAndPort string //localhost:2017
	oConn        net.Conn
	oReaderChan  chan []byte //receive data chan
	mapDealFun   map[int]IGameDealOne
}

//IGameDeal 处理消息的接口
type IGameDeal interface {
	initDealFun(NetClient)
	//dealMsgStep2(int, []byte, NetClient)
}

//IGameDealOne 处理消息的接口//把这个看成函数指针
type IGameDealOne interface {
	dealMsgStep3(msgContent []byte, oClient NetClient)
}

//封包
func (p *NetClient) enpack(message []byte) []byte {
	return append(append(intToBytes(len(message)+_IDLength), intToBytes(10)...), message...)
}

//解析正文
func (p *NetClient) depackContent(buffer []byte) {
	msgLen := len(buffer)
	if msgLen < _IDLength {
		println("error ")
	} else {
		msgID := bytesToInt(buffer[0:_HeaderLength])
		println("消息id:", msgID, "内容:", string(buffer[_HeaderLength:]))
	}
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
func (p *NetClient) sendMsg(msg []byte) (int, error) {
	return p.oConn.Write(p.enpack(msg))
}

//处理消息
func (p *NetClient) dealMsgStep1(readerChannel chan []byte, deal IGameDeal) {
	for {
		select {
		case data := <-readerChannel:
			msgLen := len(data)
			if msgLen < _IDLength {
				println("error ")
			} else {
				msgID := bytesToInt(data[0:_HeaderLength])
				println("[login]消息id:", msgID, "内容:", string(data[_HeaderLength:]))
				if v, ok := p.mapDealFun[msgID]; ok {
					v.dealMsgStep3(data[_HeaderLength:], *p)
				} else {
					println("not find deal func")
				}
			}
		}
	}
}

//Run 连接服务器
//等同于 c++模版connectServer(string, T)
//编写顺序:先定义 [类] , 接着 定义 [接口]
func (p *NetClient) Run(ip string, deal IGameDeal) {
	p.strIPAndPort = ip
	//连接
	var err error
	p.oConn, err = net.Dial("tcp", p.strIPAndPort)
	p.checkError(err)
	fmt.Println(p.strName, " 连接成功!")

	//初始化容器
	p.oReaderChan = make(chan []byte, 1024)

	//处理消息
	p.mapDealFun = make(map[int]IGameDealOne)
	deal.initDealFun(*p)
	go p.dealMsgStep1(p.oReaderChan, deal)
	//接收消息
	go p.readAll(p.oConn, p.oReaderChan)

	inputReader := bufio.NewReader(os.Stdin)
	//如果quit就退出
	for {
		input, _ := inputReader.ReadString('\n')
		trimInput := strings.Trim(input, "\r\n")
		if trimInput == "quit" {
			fmt.Println("再见")
			time.Sleep(1 * time.Second)
			return
		}

		p.sendMsg([]byte(trimInput))
	}
}

//
func (p *NetClient) readAll(conn net.Conn, readerChannel chan []byte) {

	// 缓冲区，存储被截断的数据
	defer conn.Close()
	tmpBuffer := make([]byte, 0)
	buffer := make([]byte, 1024)
	for {
		n, err := conn.Read(buffer)
		if err != nil {
			fmt.Println(conn.RemoteAddr().String(), " connection error: ", err)
			return
		}

		tmpBuffer = p.depack(append(tmpBuffer, buffer[:n]...), readerChannel)
	}
}

func (p *NetClient) checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s Fatal error: %s", p.strName, err.Error())
		os.Exit(1)
	}
}
