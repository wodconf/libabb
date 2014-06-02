#include <abb/base/log.hpp>
#include "abb/net/tcp_client_manager.hpp"
#include "abb/net/connection_ref.hpp"
using namespace abb::net;
class ConnectCB:public TcpClientManager::Listener{
public:
	ConnectCB():conn(NULL),index(0){

	}
	virtual ~ConnectCB(){}
	virtual void L_TcpClient_OnConnectFail(int error){

	}
	virtual void L_TcpClient_OnConnection(ConnectionRef* ref){
		conn = ref;
		this->Send();
	}
	virtual void L_TcpClient_OnMessage(ConnectionRef* conn,abb::base::Buffer& buf){
		this->Send();
	}
	virtual void L_TcpClient_OnClose(ConnectionRef* conn,int error){

	}
	void Send(){
		index++;
		if(conn)
			this->conn->Send(&index,sizeof(index));
	}
	int index ;
	ConnectionRef* conn;
};
int main(){
	LOG(INFO) << "c";
	abb::net::IPAddr addr;
	if( ! addr.SetV4("127.0.0.1",9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	TcpClientManager mgr;

	mgr.Init(4,true);
	mgr.Connect(addr,new ConnectCB());
	mgr.Start();
}
