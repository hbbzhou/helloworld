package client

import (
	"sort"
	"time"
)

/*
//使用说明

*/

//ClientOp zan wu
type ClientOp struct {
	Index     int
	StartTime int64
	PSendFun  func(pClient *NetClient)
}

//Init zan wu
func (p *ClientOp) Init(index int, nTime int, f func(pClient *NetClient)) {
	p.Index = index
	p.StartTime = time.Now().UnixNano()/int64(time.Millisecond) + int64(nTime)
	p.PSendFun = f
}

type clientOpList []*ClientOp

func (p clientOpList) Len() int {
	return len(p)
}

func (p clientOpList) Less(i, j int) bool {
	return p[i].StartTime > p[j].StartTime
}

func (p clientOpList) Swap(i, j int) {
	p[i], p[j] = p[j], p[i]
}

//ClientOpMgr zanwu
type ClientOpMgr struct {
	list           clientOpList
	IsClose        bool
	AllLoginClient []*NetClient
}

//AddJob zan wu
func (p *ClientOpMgr) AddJob(index int, nTime int, f func(pClient *NetClient)) {
	poneOp := new(ClientOp)
	poneOp.Init(index, nTime, f)
	p.Insert2Vec(poneOp)
}

//Insert2Vec zan wu
func (p *ClientOpMgr) Insert2Vec(pdata *ClientOp) {
	nLen := len(p.list)
	b := sort.Search(nLen, func(i int) bool { return pdata.StartTime > p.list[i].StartTime })
	p.list = append(p.list, nil)
	if b == nLen {
		p.list[b] = pdata
	} else {
		copy(p.list[b+1:], p.list[b:])
		p.list[b] = pdata
	}
}

func (p *ClientOpMgr) popFront() *ClientOp {
	nLen := len(p.list)
	if nLen > 0 {
		pdata := p.list[0]
		p.list = p.list[1:]
		return pdata
	}
	return nil
}

func (p *ClientOpMgr) popBack() *ClientOp {
	nLen := len(p.list)
	if nLen > 0 {
		pdata := p.list[nLen-1]
		p.list = p.list[:nLen-1]
		return pdata
	}
	return nil
}

//取出一个满足条件的对象
func (p *ClientOpMgr) popOne() *ClientOp {
	nLen := len(p.list)
	nStart := time.Now().UnixNano() / int64(time.Millisecond)
	if nLen > 0 {
		pdata := p.list[nLen-1]
		if pdata.StartTime >= nStart {
			p.list = p.list[:nLen-1]
			return pdata
		}
	}
	return nil
}

//Run zanwu
func (p *ClientOpMgr) Run() {
	for p.IsClose == false {
		pdata := p.popOne()
		if pdata == nil {
			time.Sleep(1 * time.Millisecond)
			continue
		}

		if pdata.Index < len(p.AllLoginClient) {
			pdata.PSendFun(p.AllLoginClient[pdata.Index])
			//通过返回值//获取下一步的操作
			//最好让任务模块,全局化,让网络层可以访问
		}
	}
}
