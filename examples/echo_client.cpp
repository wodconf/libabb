#include <abb/base/log.hpp>
#include "abb/net/tcp_client.hpp"
#include "abb/net/connection_ref.hpp"
#include "abb/net/context.hpp"
using namespace abb::net;
Context ctx;
class ConnectCB:public ITcpClientListener{
public:
	ConnectCB():conn(NULL),index(0){

	}
	virtual ~ConnectCB(){}
	virtual void L_TcpClient_OnConnectFail(int error){
		LOG(DEBUG) << "L_TcpClient_OnConnectFail";
	}
	virtual void L_TcpClient_OnConnection(ConnectionRef* ref){
		conn = ref;
		this->Send();
	}
	virtual void L_TcpClient_OnMessage(ConnectionRef* conn,abb::base::Buffer& buf){
		this->Send();
	}
	virtual void L_TcpClient_OnClose(ConnectionRef* conn,int error){
		LOG(DEBUG) << "ONCLOSE";
	}
	void Send(){
		index++;
		if(conn){
			abb::base::Buffer* buf;
			if( this->conn->LockWrite(&buf)){
				*buf << index;
				this->conn->UnLockWrite();
			}
		}
	}
	int index ;
	ConnectionRef* conn;
};
void sleep(int ms){
	struct timeval tv;
	tv.tv_sec = ms/1000;
	tv.tv_usec = ( ms- tv.tv_sec*1000)*1000;
	select(0,0,0,0,&tv);
}
int main(){
	abb::net::IPAddr addr;
	if( ! addr.SetV4("127.0.0.1",9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}



	ctx.SetNumThread(4);
	ctx.SetRunCureentThread(true);
	ctx.Init();
	ConnectCB lis;
	tcp::Connect(ctx.GetFreeLoop(),addr,NULL,&lis);
	ctx.Start();

}
