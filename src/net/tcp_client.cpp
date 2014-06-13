/*
 * tcp_client.cpp
 *
 *  Created on: 2014-6-3
 *      Author: wd
 */

#include "abb/net/tcp_client.hpp"
#include "loop.hpp"
#include "connector.hpp"
#include "connection.hpp"
#include "abb/net/connection_ref.hpp"
namespace abb {
namespace net {

class TcpClient:public IConnectorListener,IConnectionListener{
public:
	TcpClient(Loop* loop,ITcpClientListener* lis):loop_(loop),lis_(lis) {
	}
	virtual ~TcpClient(){};
	void L_Connection_OnMessage(Connection* conn,base::Buffer& buf){
		ConnectionRef* ref = (ConnectionRef*)conn->GetData();
		if(ref->IsClosed()){
			buf.Clear();
		}else{
			this->lis_->L_TcpClient_OnMessage(ref,buf);
		}
	}
	void L_Connection_OnClose(Connection* conn,int error){
		ConnectionRef* ref = (ConnectionRef*)conn->GetData();
		int id = conn->GetId();
		conn->ShutDown();
		conn->Destroy();
		ref->TestAndSetNULL();
		lis_->L_TcpClient_OnClose(ref,error);
		ref->UnRef();
		delete this;
	}
	void L_Connector_OnOpen(Connector* cotr,int fd){
		cotr->Destroy();
		Connection* conn = new Connection(	loop_,fd,cotr->GetIpAddr(),cotr->GetIpAddr(),0);
		ConnectionRef* conn_ref_ = new ConnectionRef(conn);
		conn->SetData(conn_ref_);
		conn->SetListener(this);
		conn->SetEnable(true);
		lis_->L_TcpClient_OnConnection(conn_ref_);
	}
	void L_Connector_OnClose(Connector* cotr,int error){
		cotr->Destroy();
		lis_->L_TcpClient_OnConnectFail(error);
		delete this;
	}
	ITcpClientListener* lis_;
	Loop* loop_;
};
namespace tcp{
extern bool Connect(Loop* loop,const IPAddr& addr,int* save_error,ITcpClientListener* lis){
	Connector *ctor_ = new Connector(loop);
	TcpClient* cli = new TcpClient(loop,lis);
	ctor_->SetListener(cli);
	if(ctor_->Connect(addr,save_error)){
		return true;
	}else{
		delete cli;
		ctor_->Destroy();
		return false;
	}
}
}
} /* namespace monprxoy */
} /* namespace adcloud */
