package main

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

func init() {
	fmt.Printf("[auto run] init \n")
}

func wait() {
	a := ""
	fmt.Println("wait...")
	fmt.Scanf("%s", &a)
	fmt.Println(a)
}

const _HeaderLength = 2
const _IDLength = 2

//封包
func enpack(message []byte) []byte {
	return append(append(intToBytes(len(message)+_IDLength), intToBytes(10)...), message...)
}

func depackContent(buffer []byte) {
	msgLen := len(buffer)
	if msgLen < _IDLength {
		println("error ")
	} else {
		msgID := bytesToInt(buffer[0:_HeaderLength])
		println("消息id:", msgID, "内容:", string(buffer[_HeaderLength:]))
	}
}

//解包
func depack(buffer []byte, readerChannel chan []byte) []byte {
	length := len(buffer)

	var i int
	for i = 0; i < length; i = i + 1 {
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
	}

	if i == length {
		return make([]byte, 0)
	}
	return buffer[i:]
}

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

func sendMsg(c net.Conn, msg []byte) (int, error) {
	return c.Write(enpack(msg))
}

//连接服务器
func connectServer() {
	//接通
	conn, err := net.Dial("tcp", "localhost:2017")
	checkError(err)
	fmt.Println("连接成功!")

	readerChannel := make(chan []byte, 2)

	//接收消息
	go dealContent(readerChannel)
	go readAll(conn, readerChannel)

	//输入
	inputReader := bufio.NewReader(os.Stdin)
	fmt.Println("who?")
	name, _ := inputReader.ReadString('\n')
	//
	trimName := strings.Trim(name, "\r\n")
	sendMsg(conn, []byte(trimName+" join\n "))
	for {
		fmt.Println("-按quit退出-")
		//读一行
		input, _ := inputReader.ReadString('\n')
		trimInput := strings.Trim(input, "\r\n")
		//如果quit就退出
		if trimInput == "quit" {
			fmt.Println("再见")
			sendMsg(conn, []byte(trimName+" 退出了 "))
			time.Sleep(1 * time.Second)
			return
		}
		//发送消息
		_, err = sendMsg(conn, []byte(trimName+" [says:]"+trimInput))
	}
}

func readAll(conn net.Conn, readerChannel chan []byte) {

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

		tmpBuffer = depack(append(tmpBuffer, buffer[:n]...), readerChannel)
	}
}

func dealContent(readerChannel chan []byte) {
	for {
		select {
		case data := <-readerChannel:
			depackContent(data)
		}
	}
}

func checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		os.Exit(1)
	}
}

func main() {

	//连接servser
	connectServer()
	fmt.Printf("[end...]  \n")
}
