#include "abb/http/http_client.hpp"
#include "abb/net/tcp_client.hpp"
#include "abb/http/http_request.hpp"
#include "abb/http/http_responce.hpp"
#include "abb/http/http_const.hpp"
#include "abb/http/http_decoder.hpp"
namespace abb{
namespace http{

class HttpClient:public net::ITcpClientListener{
public:
	HttpClient(Request* req,IRequestHandler* handler)
	:req_(req),
	handler_(handler),
	read_responced_(false),
	error_(-1)
	{
	}
	virtual ~HttpClient(){
		req_->UnRef();
	}
	virtual void L_TcpClient_OnConnectFail(int error){
		handler_->HandleError(error);
		delete this;
	}
	virtual void L_TcpClient_OnConnection(net::ConnectionRef* conn){
		abb::base::Buffer* buf;
		if( conn->LockWrite(&buf)){
			req_->Encode(*buf);
			conn->UnLockWrite();
			conn->Data = new ResponceDecoder();
		}
	}
	virtual void L_TcpClient_OnMessage(net::ConnectionRef* conn,base::Buffer& buf){
		LOG(DEBUG) << "L_TcpClient_OnMessage" << std::string((char*)buf.Data(),buf.Size());
		ResponceDecoder* d = static_cast<ResponceDecoder*>(conn->Data);
		if( !d->Decode(buf) ){
			conn->Close();
			error_ = 1001;
		}else{
			Responce* rsp = d->GetResponce();
			if(rsp){
				read_responced_= true;
				this->handler_->HandleResponce(rsp);
				rsp->UnRef();
				conn->Close();
			}
		}
	}
	virtual void L_TcpClient_OnClose(net::ConnectionRef* conn,int error){
		if(!read_responced_){
			if(error_ > 0){
				this->handler_->HandleError(error_);
			}else{
				this->handler_->HandleError(error);
			}
		}
		delete this;
	}
	int error_;
	bool read_responced_;
	IRequestHandler* handler_;
	Request* req_;
};

bool Post(net::EventLoop* loop,
				const std::string& url,
				const std::string& body_type,
				void* body,
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
	return true;
}


}}
