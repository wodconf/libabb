#include <abb/base/log.hpp>
#include "abb/net/tcp_client.hpp"
#include "abb/net/connection_ref.hpp"
#include "abb/net/event_loop.hpp"
#include "abb/http/http_request.hpp"
#include "abb/http/http_const.hpp"
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
		
		http::Request req(http::method::GET,"HTTP/1.1");
		req.SetUrl("http://www.baidu.com");
		req.GetHeader().Set(http::header::USER_AGENT," Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:30.0) Gecko/20100101 Firefox/30.0");
		req.GetHeader().Set(http::header::CONTENT_LENGTH,"0");
		req.GetHeader().Set(http::header::ACCEPT,"*/*");
		req.GetHeader().Set(http::header::CONNECTION,"Keep-Alive");
		req.GetHeader().Set(http::header::ACCEPT_LANGUAGE,"zh-cn");
		req.GetHeader().Set(http::header::ACCEPT_ENCODING,"gzip, deflate");
		abb::base::Buffer* buf;
		if( this->conn->LockWrite(&buf)){
			req.Encode(*buf);
			LOG(DEBUG) << std::string((char *)buf->Data(),buf->Size());
			this->conn->UnLockWrite();
		}
		LOG(DEBUG) << "end";
	}
	virtual void L_TcpClient_OnMessage(ConnectionRef* conn,abb::base::Buffer& buf){
		LOG(DEBUG) << std::string((char *)buf.Data(),buf.Size());
		LOG(DEBUG) << "L_TcpClient_OnMessage";
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
int main(){
	abb::net::IPAddr addr;
	if( ! addr.SetV4("localhost",9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	EventLoop loop;
	ConnectCB lis;
	tcp::Connect(&loop,addr,&lis);
	loop.Loop();

}
