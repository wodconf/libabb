#include "abb/http/http_client.hpp"
#include "abb/net/tcp_client.hpp"
#include "abb/http/http_request.hpp"
#include "abb/http/http_const.hpp"
namespace abb{
namespace http{

class HttpClient:public net::ITcpClientListener{
public:
	HttpClient(Request* req,IRequestHandler* handler)
	:req_(req),
	handler_(handler),
	read_responced_(false)
	{
	}
	virtual ~HttpClient(){

	}
	virtual void L_TcpClient_OnConnectFail(int error){
		handler_->HandleError(error);
	}
	virtual void L_TcpClient_OnConnection(net::ConnectionRef* conn){
		abb::base::Buffer* buf;
		if( conn->LockWrite(&buf)){
			req_->Encode(*buf);
			conn->UnLockWrite();
		}
	}
	virtual void L_TcpClient_OnMessage(net::ConnectionRef* conn,base::Buffer& buf){

	}
	virtual void L_TcpClient_OnClose(net::ConnectionRef* conn,int error){
		
	}
	bool read_responced_;
	IRequestHandler* handler_;
	Request* req_;
};

bool POST(net::EventLoop* loop,
				std::string& url,
				const std::string& body_type,
				void* body,
				int size,
				IRequestHandler* handler){
	Request* req = new Request(http::method::POST,"HTTP/1.1");
	if(!req->SetUrl(url)){
		return false;
	}else{
		return Do(loop,req,handler);
	}
}
bool GET(net::EventLoop* loop,std::string& url,IRequestHandler* handler){
	Request* req = new Request(http::method::GET,"HTTP/1.1");
	if(!req->SetUrl(url)){
		return false;
	}else{
		return Do(loop,req,handler);
	}
}
bool Do(net::EventLoop* loop,Request* req,IRequestHandler* handler){
	net::IPAddr addr;
	if( req->GetURL().Host.find(":") == std::string::npos){
		if(!addr.SetByString(req->GetURL().Host + ":80")){
			return false;
		}
	}else{
		if( !addr.SetByString(req->GetURL().Host) ){
			return false;
		}
	}
	HttpClient* htc = new HttpClient(req,handler);
	net::tcp::Connect(loop,addr,htc);
	return true;
}


}}
