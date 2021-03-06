

#include <abb/base/log.hpp>
#include <abb/net/tcp_server.hpp>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
using namespace abb;
using namespace abb::net;
void sleep(int ms){
	struct timeval tv;
	tv.tv_sec = ms/1000;
	tv.tv_usec = ( ms- tv.tv_sec*1000)*1000;
	select(0,0,0,0,&tv);
}
class EchoServer:public TcpServer::Listener{
public:
	EchoServer():num_pkt(0){
		tcp_server.Init(4,false);
		tcp_server.SetListener(this);
	}
	virtual ~EchoServer(){};
	void Start(const abb::net::SocketAddress& addr){
		int err;
		if( ! this->tcp_server.Bind(addr,&err) ){
			LOG(INFO) << "Bind fail" << strerror(err);
			exit(0);
		}
		tcp_server.Start();
	}
	virtual void L_TcpServer_OnConnection(ConnectionRef*ref){
		LOG(DEBUG) << "OnConnection";

	}
	virtual void L_TcpServer_OnMesssage(ConnectionRef* ref,Buffer& buf){
		num_pkt++;
		buf.Clear();
		int index = 1;
		ref->WriteAndFlush(&index,sizeof(int));
	}
	virtual void L_TcpServer_OnClose(ConnectionRef*,int error){
		LOG(DEBUG) << "L_TcpServer_OnClose";

	}
	int num_pkt;
private:
	TcpServer tcp_server;
};
int main(){
	abb::net::SocketAddress addr;
	if( ! addr.SetUnix("echo_svr",4) ){
		LOG(INFO) << "SetUnix fail";
		return -1;
	}
	EchoServer svr;
	svr.Start(addr);
	while(true){
		if(svr.num_pkt)
			LOG(INFO) << "num_pkt:" << svr.num_pkt;
		svr.num_pkt = 0;
		sleep(1000);
	}
}
