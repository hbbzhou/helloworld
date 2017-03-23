package client

import (
	"log"
	"math/rand"

	"SProtoSpace"

	"github.com/golang/protobuf/proto"
)

//DealGate 处理xx协议
type DealGate struct {
}

func (p *DealGate) initDealFun(pClient *NetClient) {
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_rand_names_ack_id)] = new(DRandNamesAck)
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_role_check_ack_id)] = new(DRoleCheckAck)
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_enter_game_ack_id)] = new(DEnterGameAck)
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_group_player_info_id)] = new(DGroupPlayerInfo)
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_create_role_of_game_ack_id)] = new(DCreateRoleOfGameAck)
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_role_info_ntf_id)] = new(DRoleInfoNtf)
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_gm_operation_ack_id)] = new(DGmOperationAck)
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_job_all_list_ntf_id)] = new(DJobAllListNtf)
	pClient.pMapDealFun[int(SProtoSpace.EProtoID_job_update_ntf_id)] = new(DJobUpdateNtf)
	//Merge1
}

//DRandNamesAck zan wu
type DRandNamesAck struct {
}

//处理消息
//登录step_5_1//创建帐号流程
func (p *DRandNamesAck) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.RandNamesAck)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	println("##[receive]##", pProto.String())

	nLen := len(pProto.GetNames())
	if nLen <= 0 {
		return 2
	}
	nRand := rand.Intn(nLen)
	pClient.pPlayer.strName = pProto.GetNames()[nRand]
	SendCreateRoleOfGame(pClient)
	println("处理完成")
	return 0
}

//DRoleCheckAck zan wu
type DRoleCheckAck struct {
}

//处理消息
func (p *DRoleCheckAck) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.RoleCheckAck)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	println("##[receive]##", pProto.String())
	println("处理完成")
	return 0
}

//DEnterGameAck zan wu
type DEnterGameAck struct {
}

//处理消息
func (p *DEnterGameAck) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.EnterGameAck)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	println("##[receive]##", pProto.String())
	println("处理完成")
	return 0
}

//DGroupPlayerInfo zan wu
type DGroupPlayerInfo struct {
}

//处理消息
//登录step_4
func (p *DGroupPlayerInfo) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.GroupPlayerInfo)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}

	if pProto.GetPlayerid() == ^uint64(0) {
		SendRandNamesReq(pClient)
	} else {
		pClient.pPlayer.nUUID = pProto.GetPlayerid()
		pClient.pPlayer.strName = pProto.GetPlayername()
		SendEnterGameReq(pClient)
	}

	println("##[receive]##", pProto.String())
	println("处理完成")
	return 0
}

//DCreateRoleOfGameAck zan wu
type DCreateRoleOfGameAck struct {
}

//处理消息
//登录step_5_2//创建帐号流程
func (p *DCreateRoleOfGameAck) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.CreateRoleOfGameAck)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	println("##[receive]##", pProto.String())

	if pProto.GetCreateRet() != int32(SProtoSpace.ECreateResult_create_result_ok) {
		log.Fatal("注册帐号失败")
		return 2
	}
	pClient.pPlayer.nUUID = pProto.GetPlayerid()
	SendEnterGameReq(pClient)
	println("处理完成")
	return 0
}

//DRoleInfoNtf zan wu
type DRoleInfoNtf struct {
}

//处理消息
//登录step_7
func (p *DRoleInfoNtf) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.RoleInfoNtf)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	println("##[receive]##", pProto.String())
	println("登录完成_")
	return 0
}

//DGmOperationAck zan wu
type DGmOperationAck struct {
}

//处理消息
func (p *DGmOperationAck) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.GmOperationAck)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	println("##[receive]##", pProto.String())
	println("处理完成")
	return 0
}


//DJobAllListNtf zan wu
type DJobAllListNtf struct {
}

//处理消息
func (p *DJobAllListNtf) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.JobAllListNtf)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	println("##[receive]##", pProto.String())
	return 0
}


//DJobUpdateNtf zan wu
type DJobUpdateNtf struct {
}

//处理消息
func (p *DJobUpdateNtf) dealMsgStep3(pmsgContent []byte, pClient *NetClient) int {
	pProto := new(SProtoSpace.JobUpdateNtf)
	err := proto.Unmarshal(pmsgContent, pProto)
	if err != nil {
		log.Fatal("unmarshaling error: ", err)
		return 1
	}
	println("##[receive]##", pProto.String())
	return 0
}

//Merge3

//SendEnterGameReq zan wu
//登录step_6
func SendEnterGameReq(pClient *NetClient) {
	pProto := new(SProtoSpace.EnterGameReq)
	pProto.Playerid = proto.Uint64(pClient.pPlayer.nUUID)
	data, err := proto.Marshal(pProto)
	if err != nil {
		log.Fatal("marshaling error: ", err)
		return
	}
	println("##[send]##", pProto.String())
	pClient.sendMsg(data, int(SProtoSpace.EProtoID_enter_game_req_id))
}

//SendCreateRoleOfGame zan wu
func SendCreateRoleOfGame(pClient *NetClient) {
	pProto := new(SProtoSpace.CreateRoleOfGame)

	pProto.Account = proto.String(pClient.pPlayer.strAccount)
	pProto.Playername = proto.String(pClient.pPlayer.strName)

	data, err := proto.Marshal(pProto)
	if err != nil {
		log.Fatal("marshaling error: ", err)
		return
	}
	println("##[send]##", pProto.String())
	pClient.sendMsg(data, int(SProtoSpace.EProtoID_create_role_of_game_id))
}

//SendRoleCheckReq zan wu
//登录step_3
func SendRoleCheckReq(pClient *NetClient) {
	pProto := new(SProtoSpace.RoleCheckReq)

	pProto.Account = proto.String(pClient.pPlayer.strAccount)
	pProto.CenterServerId = proto.Uint64(pClient.pPlayer.nCheckServerID)
	data, err := proto.Marshal(pProto)
	if err != nil {
		log.Fatal("marshaling error: ", err)
		return
	}
	println("##[send]##", pProto.String())
	pClient.sendMsg(data, int(SProtoSpace.EProtoID_role_check_req_id))
}

//SendRandNamesReq zan wu
func SendRandNamesReq(pClient *NetClient) {
	pProto := new(SProtoSpace.RandNamesReq)
	data, err := proto.Marshal(pProto)
	if err != nil {
		log.Fatal("marshaling error: ", err)
		return
	}
	println("##[send]##", pProto.String())
	pClient.sendMsg(data, int(SProtoSpace.EProtoID_rand_names_req_id))
}

//SendGmOperationReq zan wu
func SendGmOperationReq(pClient *NetClient) {
	pProto := new(SProtoSpace.GmOperationReq)

	data, err := proto.Marshal(pProto)
	if err != nil {
		log.Fatal("marshaling error: ", err)
		return
	}
	println("##[send]##", pProto.String())
	pClient.sendMsg(data, int(SProtoSpace.EProtoID_gm_operation_req_id))
}

//Merge2
