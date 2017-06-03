package client

import (
	"SProtoSpace"
	"log"

	"github.com/golang/protobuf/proto"
)

//DealLogin 处理xx协议
type DealLogin struct {
}

func (p *DealLogin) initDealFun(pClient *NetClient) {
	pClient.pMapDealFun[int(SProtoSpace.ProtoId_LOGIN_ACK_ID)] = new(DLoginAck)
	pClient.pMapDealFun[int(SProtoSpace.ProtoId_area_list_info_ack_id)] = new(DAreaListInfoAck)
	//Merge1
}

//DAreaListInfoAck zan wu
type DAreaListInfoAck struct {
}

//处理消息
//登录step_2
func (p *DAreaListInfoAck) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.AreaListInfoAck)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}

	for _, v := range pProto.GetGroupNetInfo() {
		if v.GetIp() == pClient.pPlayer.StrGateIP {
			pClient.pPlayer.NGatePort = v.GetPort()
			pClient.pPlayer.strAccount = pProto.GetAccount()
			pClient.pPlayer.nCheckServerID = pProto.GetCheckServerId()
			pClient.pPlayer.nToken = pProto.GetToken()
			//断开login,并连接gate
			pClient.pPlayer.NStep = E_ToConnectGate
			break
		}
	}

	//println("##[receive]##", pProto.String())
	return 0
}

//Merge3

//DLoginAck zan wu
type DLoginAck struct {
}

//处理消息
func (p *DLoginAck) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.LoginAck)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	//println("##[receive]##", pProto.String())
	if pProto.GetRet() != 2 {
		return 2
	}
	return 0
}

//SendLoginReq zan wu
//登录step_1
func SendLoginReq(pClient *NetClient) {
	pProto := new(SProtoSpace.LoginReq)
	pProto.Account = proto.String(pClient.pPlayer.StrAcountName)
	pProto.LoginType = SProtoSpace.EAuthType_AUTH_SELF.Enum()
	pProto.Password = proto.String("123456")
	data, err := proto.Marshal(pProto)
	if err != nil {
		log.Fatal("marshaling error: ", err)
		return
	}
	pClient.sendMsg(data, int(SProtoSpace.ProtoId_LOGIN_REQ_ID))
}

//Merge2
