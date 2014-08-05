#include <abb/base/log.hpp>
#include "abb/net/tcp_client.hpp"
#include "abb/net/connection_ref.hpp"
#include "abb/net/event_loop.hpp"
#include "abb/http/http_request.hpp"
#include "abb/http/http_const.hpp"
#include <sys/time.h>
using namespace abb::net;
using namespace abb;
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
		LOG(DEBUG) << "L_TcpClient_OnConnection";
		
		Send();
	}
	virtual void L_TcpClient_OnMessage(ConnectionRef* conn,abb::base::Buffer& buf){
		Send();
	}
	virtual void L_TcpClient_OnClose(ConnectionRef* conn,int error){
		LOG(DEBUG) << "ONCLOSE" << strerror(error);
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
static uint64_t MSNow(){
	struct timeval time;
	gettimeofday(&time,0);
	uint64_t tmp =  time.tv_sec;
	tmp*=1000;
	tmp+=time.tv_usec/1000;
	return tmp;
}
uint64_t pre;
static void do_timer(void* arg){
	uint64_t now = MSNow();
	LOG(INFO) <<now-pre;
	pre = now;
}
int main(){
	abb::net::IPAddr addr;
	if( ! addr.SetV4("localhost",9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	EventLoop loop;
	ConnectCB lis;
	tcp::Connect(&loop,addr,&lis);
	loop.RunEvery(50,do_timer,NULL);
	loop.Loop();

}
