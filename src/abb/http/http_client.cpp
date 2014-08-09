#include "abb/http/http_client.hpp"
#include "abb/net/tcp_client.hpp"
#include "abb/http/http_request.hpp"
#include "abb/http/http_responce.hpp"
#include "abb/http/http_const.hpp"
#include "abb/http/http_decoder.hpp"
#include "abb/base/thread.hpp"
namespace abb{
namespace http{

class HttpClient:public net::ITcpClientListener,public RefObject{
public:
	HttpClient(Request* req,IRequestHandler* handler)
	:req_(req),
	handler_(handler),
	read_responced_(false),
	error_(-1)
	{
		this->Ref();
	}
	virtual ~HttpClient(){
		req_->UnRef();
		rsp_->UnRef();
	}
	void Wait(){
		notify_.Wait();
	}
	virtual void L_TcpClient_OnConnectFail(int error){
		handler_->HandleError(error);
		notify_.Notify();
		this->UnRef();
	}
	virtual void L_TcpClient_OnConnection(net::ConnectionRef* conn){
		abb::Buffer* buf;
		if( conn->LockWrite(&buf)){
			req_->Encode(*buf);
			conn->UnLockWrite(true);
			conn->Data = new ResponceDecoder();
		}
	}
	virtual void L_TcpClient_OnMessage(net::ConnectionRef* conn,Buffer& buf){
		ResponceDecoder* d = static_cast<ResponceDecoder*>(conn->Data);
		if( !d->Decode(buf) ){
			conn->Close();
			error_ = 1001;
		}else{
			rsp_ = d->GetResponce();
			if(rsp_){
				read_responced_= true;
				notify_.Notify();
				if(this->handler_)this->handler_->HandleResponce(rsp_);
				conn->Close();
			}
		}
	}
	virtual void L_TcpClient_OnClose(net::ConnectionRef* conn,int error){
		if(!read_responced_){
			if(error_ > 0){
				if(this->handler_)this->handler_->HandleError(error_);
			}else{
				if(this->handler_)this->handler_->HandleError(error);
			}
			notify_.Notify();
		}
		this->UnRef();
	}
	int GetError(){
		return error_;
	}
	Responce* GetResponce(){
		if(rsp_) rsp_->Ref();
		return rsp_;
	} 
private:
	int error_;
	bool read_responced_;
	IRequestHandler* handler_;
	Request* req_;
	Responce* rsp_;
	Notification notify_;
};

bool Post(net::EventLoop* loop,
				const std::string& url,
				const std::string& body_type,
				const void* body,
				int size,
				IRequestHandler* handler){
	Request* req = new Request(http::method::POST,"HTTP/1.1");
	if(!req->SetUrl(url)){
		return false;
	}else{
		req->Body().Write(body,size);
		return Do(loop,req,handler);
	}
}
bool Get(net::EventLoop* loop,const std::string& url,IRequestHandler* handler){
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
	htc->UnRef();
	return true;
}

extern Responce* SyncDo(net::EventLoop* loop,Request* req,int* error){
	if(loop->IsInEventLoop()){
		return NULL;
	}
	net::IPAddr addr;
	if( req->GetURL().Host.find(":") == std::string::npos){
		if(!addr.SetByString(req->GetURL().Host + ":80")){
			return NULL;
		}
	}else{
		if( !addr.SetByString(req->GetURL().Host) ){
			return NULL;
		}
	}
	HttpClient* htc = new HttpClient(req,NULL);
	net::tcp::Connect(loop,addr,htc);
	htc->Wait();
	Responce* rsp = htc->GetResponce();
	if(!rsp && error) *error = htc->GetError();
	htc->UnRef();
	return rsp;
}

}}
