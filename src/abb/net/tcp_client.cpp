/*
 * tcp_client.cpp
 *
 *  Created on: 2014-6-3
 *      Author: wd
 */

#include "abb/net/tcp_client.hpp"
#include "abb/net/event_loop.hpp"
#include "abb/net/connector.hpp"
#include "abb/net/connection.hpp"
#include "abb/net/connection_ref.hpp"
namespace abb {
namespace net {

class TcpClient:public IConnectorListener,IConnectionListener{
public:
	TcpClient(EventLoop* loop,ITcpClientListener* lis,const SocketAddress& addr)
	:loop_(loop),
	lis_(lis),
	addr_(addr)
	{
		ctor_ = new Connector(loop);
		ctor_->SetListener(this);
	}
	void Start(){
		ctor_->Connect(addr_);
	}
	virtual ~TcpClient(){
		ctor_->Destroy();
	};
	void L_Connection_OnMessage(Connection* conn,Buffer& buf){
		ConnectionRef* ref = (ConnectionRef*)conn->GetData();
		if(ref->Connected()){
			this->lis_->L_TcpClient_OnMessage(ref,buf);
		}else{
			buf.Clear();
		}
	}
	void L_Connection_OnClose(Connection* conn,int error){
		ConnectionRef* ref = (ConnectionRef*)conn->GetData();
		lis_->L_TcpClient_OnClose(ref,error);
		ref->UnRef();
		delete this;
	}
	void L_Connector_OnOpen(Connector* cotr,int fd){
		Connection* conn = new Connection(	loop_,fd,cotr->GetIpAddr(),cotr->GetIpAddr());
		ConnectionRef* conn_ref_ = new ConnectionRef(conn);
		conn->SetData(conn_ref_);
		conn->SetListener(this);
		conn->Established();
	}
	void L_Connection_OnOpen(Connection* conn){
		this->lis_->L_TcpClient_OnConnection((ConnectionRef*)conn->GetData());
	}
	void L_Connector_OnClose(Connector* cotr,int error){
		lis_->L_TcpClient_OnConnectFail(error);
		delete this;
	}
	EventLoop* loop_;
	ITcpClientListener* lis_;
	Connector *ctor_;
	SocketAddress addr_;
};
namespace tcp{

namespace{
	void RealConnect(void* arg){
		TcpClient* cli = (TcpClient*)arg;
		cli->Start();
	}
}

extern void Connect(EventLoop* loop,const SocketAddress& addr,ITcpClientListener* lis){
	TcpClient* cli = new TcpClient(loop,lis,addr);
	cli->Start();
}
}
}
}
