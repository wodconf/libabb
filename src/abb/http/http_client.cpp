#include "http_client.hpp"

namespace abb{
namespace http{

class HttpClient:public net::ITcpClientListener{
	HttpClient(Request* req,IRequestHandler* handler)
	:req_(req),
	handler_(handler){

	}
	virtual ~HttpClient(){

	}
	virtual void L_TcpClient_OnConnectFail(int error){
		handler_->HandleError(error);
	}
	virtual void L_TcpClient_OnConnection(ConnectionRef* conn){
		abb::base::Buffer* buf;
		if( this->conn->LockWrite(&buf)){
			req_->Encode(*buf);
			this->conn->UnLockWrite();
		}
	}
	virtual void L_TcpClient_OnMessage(ConnectionRef* conn,base::Buffer& buf){

	}
	virtual void L_TcpClient_OnClose(ConnectionRef* conn,int error){
		
	}
	bool read_responced_;
	IRequestHandler* handler_;
	Request* req_;
};

bool POST(EventLoop* loop,
				std::string& url,
				const std::string& body_type,
				void* body,
				int size,
				IRequestHandler* handler){
	Request* req = new Request(http::method::POST,"HTTP/1.1");
	if(!req->SetUrl(url)){
		return false;
	}else{
		Do(loop,req,handler);
	}
}
bool GET(EventLoop* loop,std::string& url,IRequestHandler* handler){
	Request* req = new Request(http::method::GET,"HTTP/1.1");
	if(!req->SetUrl(url)){
		return false;
	}else{
		return Do(req);
	}
}
bool Do(EventLoop* loop,Request* req,IRequestHandler* handler){
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
	int err;
	return net::tcp::Connect(loop,addr,&err,htc);
}


}}